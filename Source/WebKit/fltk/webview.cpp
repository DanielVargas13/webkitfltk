/*
WebkitFLTK
Copyright (C) 2014 Lauri Kasanen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"

#include "kbd.h"
#include "webview.h"
#include "webviewpriv.h"

#include <cairo-xlib.h>
#include <fcntl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Menu_Item.H>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#undef None // X11 collision with JSC
#undef Status

#include <BackForwardController.h>
#include <ContextMenuController.h>
#include <Editor.h>
#include <EventListener.h>
#include <FocusController.h>
#include <FrameLoadRequest.h>
#include <FrameSelection.h>
#include <FrameView.h>
#include <HTMLAnchorElement.h>
#include <HTMLInputElement.h>
#include <HTMLLinkElement.h>
#include <InspectorController.h>
#include <MainFrame.h>
#include <markup.h>
#include <NodeList.h>
#include <PlatformKeyboardEvent.h>
#include <ScriptController.h>
#include <bindings/ScriptValue.h>
#include <Settings.h>
#include <WindowsKeyboardCodes.h>
#include <wtf/CurrentTime.h>

using namespace WTF;
using namespace WebCore;

extern int wheelspeed;
extern const char * (*downloaddirfunc)();
extern void (*newdownloadfunc)();

webview::webview(int x, int y, int w, int h, bool noGui): Fl_Widget(x, y, w, h),
			noGUI(noGui) {

	priv = new privatewebview;
	priv->gc = NULL;
	priv->cairo = NULL;
	priv->w = w;
	priv->h = h;
	priv->editing = priv->hoveringlink = false;
	priv->statusbartext = priv->title = priv->url = NULL;
	priv->titleChanged = NULL;
	priv->loadStateChanged = NULL;
	priv->progressChanged = NULL;
	priv->faviconChanged = NULL;
	priv->statusChanged = NULL;
	priv->historyAdd = NULL;
	priv->siteChanging = NULL;
	priv->error = NULL;
	priv->resourceStateChanged = NULL;
	priv->quietdiags = false;

	Fl_Widget *wid = this;

	if (!noGUI) {
		while (wid->parent())
			wid = wid->parent();
		priv->window = wid->as_window();

		fl_open_display();
		priv->depth = fl_visual->depth;
	}

	Page::PageClients clients;
	clients.chromeClient = new FlChromeClient(this);
	#if ENABLE(CONTEXT_MENUS)
	clients.contextMenuClient = new FlContextMenuClient(this);
	#endif
	#if ENABLE(DRAG_SUPPORT)
	clients.dragClient = new FlDragClient(this);
	#endif
	clients.editorClient = new FlEditorClient(this);
	clients.inspectorClient = new FlInspectorClient(this);
	clients.loaderClientForMainFrame = new FlFrameLoaderClient(this, (Frame*) 1);
	clients.progressTrackerClient = new FlProgressTrackerClient(this);

	priv->page = new Page(clients);
	priv->page->addLayoutMilestones(DidFirstVisuallyNonEmptyLayout);

	((FlFrameLoaderClient *) clients.loaderClientForMainFrame)->
			setFrame(&priv->page->mainFrame());

	//WebCore::provideGeolocationTo(priv->page, new FlGeoClient(this));
	//WebCore::provideUserMediaTo(priv->page, new FlMediaClient(this));

	priv->page->setGroupName("org.webkit.fltk.WebKitFLTK");

	// inspector exposing

	// viewportattributes?

	// backfwdlist?

	// window features?

	MainFrame *f = &priv->page->mainFrame();
	f->init();

	// Default settings
	Settings &set = priv->page->mainFrame().settings();
	set.setLoadsImagesAutomatically(true);
	set.setShrinksStandaloneImagesToFit(true);
	set.setScriptEnabled(true);
	set.setDNSPrefetchingEnabled(true);
	set.setDefaultMinDOMTimerInterval(0.016);
	set.setMinDOMTimerInterval(0.016);
	set.setHiddenPageDOMTimerAlignmentInterval(10);
	set.setHiddenPageDOMTimerThrottlingEnabled(true);
	set.setMinimumFontSize(8);
	set.setDefaultFontSize(16);
	set.setDefaultFixedFontSize(16);
	set.setDownloadableBinaryFontsEnabled(false);
	set.setAcceleratedCompositingEnabled(false);

	priv->page->focusController().setActive(true);
	priv->page->focusController().setFocusedFrame(&priv->page->mainFrame());

	// Cairo
	resize();

	clock_gettime(CLOCK_MONOTONIC, &priv->lastdraw);
}

webview::~webview() {
	// If any downloads exist, nuke them here.
	const unsigned downs = priv->downloads.size();
	for (unsigned i = 0; i < downs; i++) {
		delete priv->downloads[i];
	}

	if (priv->gc)
		delete priv->gc;

	delete priv->page;
	delete priv;
}

void webview::draw() {
	if (!priv->cairo)
		return;
	ASSERT(isMainThread());

	if (noGUI) {
		priv->clipx = 0;
		priv->clipy = 0;
		priv->clipw = w();
		priv->cliph = h();
//		drawWeb();
		return;
	}

	// Don't draw at over 60 fps. Save power and penguins.
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);

	unsigned usecs = (now.tv_sec - priv->lastdraw.tv_sec) * 1000 * 1000;
	usecs += (now.tv_nsec - priv->lastdraw.tv_nsec) / 1000;
	if (usecs < 16600)
		usleep(16600 - usecs);

	int cx, cy, cw, ch;
	fl_clip_box(x(), y(), w(), h(), cx, cy, cw, ch);
	if (!cw) return;
	priv->clipx = cx - x();
	priv->clipy = cy - y();
	priv->clipw = cw;
	priv->cliph = ch;

	drawWeb(); // for now here

	const int tgtx = cx, tgty = cy;

	// If the widget is offset somewhere, copy the right parts
	cx -= x();
	cy -= y();

	XCopyArea(fl_display, priv->cairopix, fl_window, fl_gc, cx, cy, cw, ch,
			tgtx, tgty);

	priv->lastdraw = now;
}

void webview::drawWeb() {

	Frame *f = &priv->page->mainFrame();
	if (!f->contentRenderer() || !f->view() || !priv->cairo)
		return;

	f->view()->updateLayoutAndStyleIfNeededRecursive();

	priv->gc->applyDeviceScaleFactor(f->page()->deviceScaleFactor());
	f->view()->paint(priv->gc, IntRect(priv->clipx, priv->clipy,
						priv->clipw, priv->cliph));
	priv->page->inspectorController().drawHighlight(*priv->gc);
}

void webview::load(const char *url) {

	MainFrame *f = &priv->page->mainFrame();

	// Local path?
	String orig;
	String str = orig = String::fromUTF8(url);
	if (url[0] == '/') {
		str = "file://" + orig;
	} else if (url[0] == '~') {
		const char *home = getenv("HOME");
		if (!home)
			home = "/tmp";
		str = "file://";
		str.append(home);
		str.append(url + 1);
	}

	// No protocol?
	if (!str.contains("://") && !str.startsWith("about:")) {
		str = "http://";
		str.append(orig);
	}

	FrameLoadRequest req(f, ResourceRequest(URL(URL(), str)));

	f->loader().load(req);
}

void webview::loadString(const char * const str, const char * const mime,
				const char * const enc, const char * const baseurl,
				const void * const frame) {
	Frame *f = frame ? (Frame *) frame : &priv->page->mainFrame();

	URL base = baseurl ? URL(URL(), String::fromUTF8(baseurl)) : blankURL();
	ResourceRequest req(base);

	RefPtr<SharedBuffer> sharedBuffer = SharedBuffer::create(str, strlen(str));
	SubstituteData substituteData(sharedBuffer.release(),
		mime ? String::fromUTF8(mime) : String::fromUTF8("text/html"),
		enc ? String::fromUTF8(enc) : String::fromUTF8("UTF-8"),
		base, base);

	f->loader().load(FrameLoadRequest(f, req, substituteData));
}

void webview::resize() {
	ASSERT(isMainThread());

	bool old = false;

	if (priv->cairo) {
		cairo_destroy(priv->cairo);
		old = true;

		priv->w = w();
		priv->h = h();
	}

	if (noGUI) {
		cairo_surface_t *surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w(), h());
		priv->cairo = cairo_create(surf);
		cairo_surface_destroy(surf);

		if (priv->gc)
			delete priv->gc;
		priv->gc = new GraphicsContext(priv->cairo);

		if (old)
			priv->page->mainFrame().view()->resize(priv->w, priv->h);

		return;
	}

	if (old)
		XFreePixmap(fl_display, priv->cairopix);
	priv->cairopix = XCreatePixmap(fl_display, DefaultRootWindow(fl_display),
					priv->w, priv->h, priv->depth);

	cairo_surface_t *surf = cairo_xlib_surface_create(fl_display, priv->cairopix,
								fl_visual->visual,
								priv->w, priv->h);
	priv->cairo = cairo_create(surf);
	priv->cairosurf = surf;
	cairo_surface_destroy(surf);

	if (priv->gc)
		delete priv->gc;
	priv->gc = new GraphicsContext(priv->cairo);

	if (old)
		priv->page->mainFrame().view()->resize(priv->w, priv->h);
}

static bool keyscroll(Frame &f, const unsigned key, const bool shift) {

	ScrollDirection dir;
	ScrollGranularity gran;

	switch (key) {
		case VK_SPACE:
		case VK_NEXT:
		case VK_PRIOR:
			gran = ScrollByPage;
			if (shift || key == VK_PRIOR)
				dir = ScrollUp;
			else
				dir = ScrollDown;
		break;
		case VK_HOME:
			gran = ScrollByDocument;
			dir = ScrollUp;
		break;
		case VK_END:
			gran = ScrollByDocument;
			dir = ScrollDown;
		break;
		case VK_LEFT:
			gran = ScrollByLine;
			dir = ScrollLeft;
		break;
		case VK_RIGHT:
			gran = ScrollByLine;
			dir = ScrollRight;
		break;
		case VK_UP:
			gran = ScrollByLine;
			dir = ScrollUp;
		break;
		case VK_DOWN:
			gran = ScrollByLine;
			dir = ScrollDown;
		break;
		default:
			return false;
	}

	if (f.eventHandler().scrollOverflow(dir, gran))
		return true;
	f.view()->scroll(dir, gran);

	return true;
}

static unsigned remapkey(const unsigned key, unsigned *mod) {

	switch (key) {
		case FL_Down:
		case FL_Right:
			if (Fl::event_shift() && !Fl::event_ctrl() && !Fl::event_alt()) {
				*mod &= ~PlatformEvent::ShiftKey;
				return FL_Tab;
			}
		break;
		case FL_Up:
		case FL_Left:
			if (Fl::event_shift() && !Fl::event_ctrl() && !Fl::event_alt()) {
				return FL_Tab;
			}
		break;
	}

	return key;
}

int webview::handle(const int e) {

	if (noGUI) {
		return 0;
	}

	EventHandler *ev = &priv->page->mainFrame().eventHandler();

	switch (e) {
		case FL_PUSH:
		case FL_RELEASE:
		case FL_MOVE:
		case FL_DRAG:
			{
			const IntPoint pos(Fl::event_x() - x(), Fl::event_y() - y());
			const IntPoint gpos(Fl::event_x_root(), Fl::event_y_root());
			MouseButton btn = NoButton;
			PlatformEvent::Type type;
			unsigned clicks = 0;

			if (Fl::focus() != this && e == FL_PUSH) {
				Fl::focus(this);
				handle(FL_FOCUS);
			}

			if (e == FL_PUSH || e == FL_RELEASE || e == FL_DRAG) {
				switch (Fl::event_button()) {
					case FL_LEFT_MOUSE:
						btn = LeftButton;
					break;
					case FL_MIDDLE_MOUSE:
						btn = MiddleButton;
					break;
					case FL_RIGHT_MOUSE:
						btn = RightButton;
					break;
				}
				clicks = Fl::event_clicks() ? 2 : 1;
			}

			if (e == FL_PUSH)
				type = PlatformEvent::MousePressed;
			else if (e == FL_RELEASE)
				type = PlatformEvent::MouseReleased;
			else
				type = PlatformEvent::MouseMoved;


			PlatformMouseEvent pev(pos, gpos, btn, type, clicks,
						Fl::event_shift(), Fl::event_ctrl(),
						Fl::event_alt(),
						Fl::event_command(),
						currentTime());

			if (e == FL_PUSH) {
				ev->handleMousePressEvent(pev);
				if (btn == RightButton)
					handlecontextmenu(&pev);
			} else if (e == FL_RELEASE) {
				ev->handleMouseReleaseEvent(pev);
			} else {
				ev->mouseMoved(pev);
			}

			return 1;
			}
		break;
		case FL_MOUSEWHEEL:
			{
			const IntPoint pos(Fl::event_x() - x(), Fl::event_y() - y());
			const IntPoint gpos(Fl::event_x_root(), Fl::event_y_root());

			PlatformWheelEvent pev(pos, gpos,
						Fl::event_dx() * wheelspeed,
						-Fl::event_dy() * wheelspeed,
						1, 1, ScrollByPixelWheelEvent,
						Fl::event_shift(), Fl::event_ctrl(),
						Fl::event_alt(),
						Fl::event_command());

			ev->handleWheelEvent(pev);

			return 1;
			}
		break;
		case FL_ENTER:
		case FL_LEAVE:
			fl_cursor(FL_CURSOR_DEFAULT);
			return 1;
		case FL_FOCUS:
			Fl::focus(this);
			priv->page->focusController().setActive(true);

			if (priv->page->focusController().focusedFrame())
				priv->page->focusController().setFocused(true);
			else
				priv->page->focusController().setFocusedFrame(&priv->page->mainFrame());
			return 1;
		case FL_UNFOCUS:
			priv->page->focusController().setActive(false);
			priv->page->focusController().setFocused(false);
			return 1;
		case FL_KEYDOWN:
		case FL_KEYUP:
			{
			unsigned key = Fl::event_key();
			// First of all: only take up events we got a down event for
			if (e == FL_KEYDOWN) {
				priv->pressedkeys.insert(key);
			} else {
				if (priv->pressedkeys.count(key) == 0)
					return 1;

				priv->pressedkeys.erase(key);
			}

			// Keyboard events need to go to the focused frame, others to main
			ev = &priv->page->focusController().focusedOrMainFrame().eventHandler();

			PlatformEvent::Type type = PlatformEvent::KeyDown;
			if (e == FL_KEYUP)
				type = PlatformEvent::KeyUp;

			String text = String::fromUTF8(Fl::event_text(), Fl::event_length());

			unsigned modifiers = 0;
			if (Fl::event_shift())
				modifiers |= PlatformEvent::ShiftKey;
			if (Fl::event_alt())
				modifiers |= PlatformEvent::AltKey;
			if (Fl::event_ctrl())
				modifiers |= PlatformEvent::CtrlKey;
			if (Fl::event_command())
				modifiers |= PlatformEvent::MetaKey;

			bool iskeypad = false;
			key = remapkey(key, &modifiers);

			if (key >= FL_KP && key <= FL_KP_Last)
				iskeypad = true;

			String keyid = keyidfor(key);
			unsigned win = winkeyfor(key);

			PlatformKeyboardEvent pev(type, text, text,
						keyid, win, 0, 0,
						false, iskeypad, false,
						(PlatformEvent::Modifiers) modifiers,
						currentTime());

			bool ret = ev->keyEvent(pev);

			if (e == FL_KEYDOWN && !ret)
				ret = keyscroll(priv->page->focusController().focusedOrMainFrame(),
						win, Fl::event_shift());

			// Ctrl-tab and shortcuts must never be caught by the view.
			if (key == FL_Tab && Fl::event_ctrl())
				return 0;
			if (Fl::event_ctrl() ||
				key == FL_Control_L ||
				key == FL_Control_R ||
				(key >= FL_F && key <= FL_F_Last))
				return 0;

			if (priv->editing || ret || key == FL_Tab)
				return 1;
			else
				return 0;
			}
		default:
			return Fl_Widget::handle(e);
	}
}

void webview::show() {
	priv->page->setIsVisible(true);
	Fl_Widget::show();
}

void webview::hide() {
	priv->page->setIsVisible(false);
	Fl_Widget::hide();
}

static void additem(Vector<Fl_Menu_Item> &items, const ContextMenuItem &cur) {

	const ContextMenuItemType type = cur.type();
	//const ContextMenuAction action = cur.action();
	const char * const title = strdup(cur.title().utf8().data());
	const bool enabled = cur.enabled();
	const bool checked = cur.checked();

	Fl_Menu_Item it = {
		title, 0, 0, (void *) &cur,
		0,
		FL_NORMAL_LABEL, FL_HELVETICA,
		FL_NORMAL_SIZE, FL_FOREGROUND_COLOR
	};

	if (!enabled)
		it.flags |= FL_MENU_INACTIVE;
	if (type == CheckableActionType) {
		it.flags |= FL_MENU_TOGGLE;
		if (checked)
			it.flags |= FL_MENU_VALUE;
	} else if (type == SeparatorType) {
		if (items.size())
			items[items.size() - 1].flags |= FL_MENU_DIVIDER;
		free((char *) title);
		return;
	} else if (type == SubmenuType) {
		it.flags |= FL_SUBMENU;
	}

	items.append(it);
}

void webview::handlecontextmenu(void *ptr) {
	const PlatformMouseEvent * const pev = (PlatformMouseEvent *) ptr;

	Frame * const focused = &priv->page->focusController().focusedOrMainFrame();
	focused->eventHandler().sendContextMenuEvent(*pev);

	ContextMenu *m = priv->page->contextMenuController().contextMenu();
	if (!m)
		return;

	Vector<Fl_Menu_Item> items;
	const unsigned max = m->items().size();
	items.reserveCapacity(max);

	unsigned i;
	for (i = 0; i < max; i++) {
		const ContextMenuItem &cur = m->items()[i];

		additem(items, cur);

		// Just one level of recursion for now.
		if (cur.type() == SubmenuType) {
			unsigned k;
			unsigned kmax = cur.subMenuItems().size();
			for (k = 0; k < kmax; k++) {
				const ContextMenuItem &kcur = cur.subMenuItems()[k];

				additem(items, kcur);

				ASSERT(kcur.type() != SubmenuType);
			}
			Fl_Menu_Item end;
			memset(&end, 0, sizeof(Fl_Menu_Item));
			items.append(end);
		}
	}
	Fl_Menu_Item end;
	memset(&end, 0, sizeof(Fl_Menu_Item));
	items.append(end);

	// Show
	const Fl_Menu_Item *res = items[0].popup(Fl::event_x(), Fl::event_y());

	const unsigned newmax = items.size();
	for (i = 0; i < newmax; i++) {
		free((char *) items[i].text);
	}

	if (res) {
		ContextMenuItem sel(*(ContextMenuItem *) res->user_data_);
		if (sel.type() == SubmenuType) return;
		priv->page->contextMenuController().contextMenuItemSelected(&sel);
	}
}

void webview::download(const char * const url, const char * const suggestedname,
			const void *preq) {

	static const char *prevdir = NULL;
	const char *dir = prevdir;
	if (!dir)
		dir = downloaddirfunc ? downloaddirfunc() : "/tmp";
	char *fullpath = (char *) dir;

	if (suggestedname) {
		asprintf(&fullpath, "%s/%s", dir, suggestedname);
	}

	Fl_File_Chooser c(fullpath, NULL,
				Fl_File_Chooser::CREATE, "Save as");
	c.show();
	while (c.shown())
		Fl::wait();

	if (fullpath != dir)
		free(fullpath);

	if (!c.value())
		return;

	free((char *) prevdir);
	prevdir = strdup(c.directory());

	// Must not exist, or if exists, be a file
	struct stat st;
	if (stat(c.value(), &st) == 0) {
		if (!S_ISREG(st.st_mode)) {
			fl_alert("Selected path exists, and is not a file");
			return;
		}

		fl_message_title("Overwrite?");
		int ret = fl_choice("File '%s' exists, overwrite?", fl_no, fl_yes, NULL,
			c.value());
		if (ret == 0)
			return;
		unlink(c.value());
	}

	// Perms check
	int fd = open(c.value(), O_CREAT | O_WRONLY, 0644);
	if (fd < 0) {
		fl_alert("Failed to create file (insufficient permissions?)");
		return;
	}
	close(fd);

	const ResourceRequest *req = (const ResourceRequest *) preq;

	// All good.
	priv->downloads.push_back(new ::download(url, c.value(), req));
	if (newdownloadfunc)
		newdownloadfunc();
}

unsigned webview::numDownloads() const {
	return priv->downloads.size();
}

void webview::stopDownload(const unsigned i) {
	if (i >= priv->downloads.size())
		return;
	priv->downloads[i]->stop();
}

void webview::removeDownload(const unsigned i) {
	if (i >= priv->downloads.size())
		return;
	delete priv->downloads[i];
	priv->downloads.erase(priv->downloads.begin() + i);
}

bool webview::downloadFinished(const unsigned i) const {
	if (i >= priv->downloads.size())
		return false;
	return priv->downloads[i]->isFinished();
}

bool webview::downloadFailed(const unsigned i) const {
	if (i >= priv->downloads.size())
		return false;
	return priv->downloads[i]->isFailed();
}

void webview::downloadStats(const unsigned i, time_t *start, long long *size,
			long long *received, const char **name,
			const char **url) const {
	if (i >= priv->downloads.size())
		return;

	priv->downloads[i]->getStats(start, size, received);
	*name = priv->downloads[i]->file;
	*url = priv->downloads[i]->url;
}

const char *webview::statusbar() const {
	return priv->statusbartext;
}

const char *webview::title() const {
	return priv->title;
}

const char *webview::url() const {
	return priv->url;
}

void webview::resize(int x, int y, int w, int h) {
	Fl_Widget::resize(x, y, w, h);
	resize();
}

void webview::titleChangedCB(void (*func)()) {
	priv->titleChanged = func;
}

void webview::loadStateChangedCB(void (*func)(webview *)) {
	priv->loadStateChanged = func;
}

void webview::progressChangedCB(void (*func)(webview *, float)) {
	priv->progressChanged = func;
}

void webview::faviconChangedCB(void (*func)(webview *)) {
	priv->faviconChanged = func;
}

void webview::statusChangedCB(void (*func)(webview *)) {
	priv->statusChanged = func;
}

void webview::historyAddCB(void (*func)(const char *url, const char *title,
				const time_t when)) {
	priv->historyAdd = func;
}

void webview::siteChangingCB(void (*func)(webview *, const char *url)) {
	priv->siteChanging = func;
}

void webview::errorCB(void (*func)(webview *, const char *err)) {
	priv->error = func;
}

void webview::resourceStateChangedCB(void (*func)(unsigned long id, bool finished)) {
	priv->resourceStateChanged = func;
}

void webview::back() {
	if (!canBack())
		return;
	priv->page->backForward().goBackOrForward(-1);
}

void webview::fwd() {
	if (!canFwd())
		return;
	priv->page->backForward().goBackOrForward(1);
}

void webview::prev() {
	if (!canBack())
		return;
	const unsigned backpages = priv->page->backForward().backCount();
	priv->page->backForward().goBackOrForward(-backpages);
}

void webview::next() {

	// Primary check - the HTML pagination code
	RefPtr<NodeList> pagination =
		priv->page->mainFrame().document()->getElementsByTagName("link");
	unsigned max = pagination->length();
	unsigned i, t;

	for (i = 0; i < max; i++) {
		HTMLLinkElement * const e = toHTMLLinkElement(pagination->item(i));
		if (e->href().isEmpty()) continue;
		if (equalIgnoringCase(e->rel(), "next")) {
			// Perfect match.
			load(e->href().string().utf8().data());
			return;
		}
	}

	// No matches, parse links
	RefPtr<NodeList> links = priv->page->mainFrame().document()->getElementsByTagName("a");
	max = links->length();

	const char targets[][10] = {
		"thread",
		"page"
	};
	const unsigned targetcount = sizeof(targets) / sizeof(targets[0]);

	std::vector<HTMLAnchorElement *> candidates;
	candidates.reserve(100);
	for (i = 0; i < max; i++) {
		HTMLAnchorElement * const e = toHTMLAnchorElement(links->item(i));
		if (!e->isLiveLink())
			continue;

		const String &text = e->text();
		if (!text.contains("next", false))
			continue;

		candidates.push_back(e);
	}

	max = candidates.size();
	for (t = 0; t < targetcount; t++) {
		for (i = 0; i < max; i++) {
			const String &text = candidates[i]->text();
			if (text.contains(targets[t], false)) {
				load(candidates[i]->href().string().utf8().data());
				return;
			}
		}
	}

	if (candidates.size() == 0)
		return;

	load(candidates[0]->href().string().utf8().data());
}

void webview::stop() {
	priv->page->mainFrame().loader().stopAllLoaders();
}

void webview::refresh() {
	priv->page->mainFrame().loader().reload();
}

bool webview::canBack() const {
	return priv->page->backForward().canGoBackOrForward(-1);
}

bool webview::canFwd() const {
	return priv->page->backForward().canGoBackOrForward(1);
}

bool webview::isLoading() const {
	const FrameLoader &l = priv->page->mainFrame().loader();

	return l.isLoading() || l.subframeIsLoading();
}

void webview::undo() {
	Frame * const focused = &priv->page->focusController().focusedOrMainFrame();
	focused->editor().command("Undo").execute();

}

void webview::redo() {
	Frame * const focused = &priv->page->focusController().focusedOrMainFrame();
	focused->editor().command("Redo").execute();
}

void webview::selectAll() {
	Frame * const focused = &priv->page->focusController().focusedOrMainFrame();
	focused->editor().command("SelectAll").execute();
}

void webview::cut() {
	Frame * const focused = &priv->page->focusController().focusedOrMainFrame();
	focused->editor().command("Cut").execute();
}

void webview::copy() {
	Frame * const focused = &priv->page->focusController().focusedOrMainFrame();
	focused->editor().command("Copy").execute();
}

void webview::paste() {
	Frame * const focused = &priv->page->focusController().focusedOrMainFrame();
	focused->editor().command("Paste").execute();
}

bool webview::find(const char *what, bool caseSensitive, bool forward) {
	if (!what)
		return false;

	const FindOptions opts = (caseSensitive ? 0 : CaseInsensitive) |
				(forward ? 0 : Backwards) | WrapAround;
	return priv->page->findString(String::fromUTF8(what), opts);
}

unsigned webview::countFound(const char *what, bool caseSensitive) {
	if (!what)
		return false;

	const FindOptions opts = caseSensitive ? 0 : CaseInsensitive;
	return priv->page->countFindMatches(String::fromUTF8(what), opts, UINT_MAX);
}

void webview::snapshot(const char *where) {

	Frame * const f = &priv->page->mainFrame();
	const unsigned cw = f->view()->contentsSize().width();
	const unsigned ch = f->view()->contentsSize().height();

	const unsigned oldw = w();
	const unsigned oldh = h();
	// Need to fool windowRect
	size(cw, ch);

	cairo_surface_t *surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, cw, ch);
	cairo_t *cc = cairo_create(surf);
	cairo_surface_destroy(surf);
	GraphicsContext *gc = new GraphicsContext(cc);

	f->view()->updateLayoutAndStyleIfNeededRecursive();
	f->view()->paint(gc, IntRect(0, 0, cw, ch));

	const cairo_status_t ret = cairo_surface_write_to_png(surf, where);
	cairo_destroy(cc);
	delete gc;
	size(oldw, oldh);
	if (ret == CAIRO_STATUS_SUCCESS)
		return;

	fl_alert("%s", cairo_status_to_string(ret));
}

char *webview::focusedSource() const {

	Frame * const focused = &priv->page->focusController().focusedOrMainFrame();
	if (!focused->document() || !focused->document()->isHTMLDocument())
		return NULL;

	if (focused->view() && focused->view()->layoutPending())
		focused->view()->layout();

	const String src = createMarkup(*focused->document());
	return strdup(src.utf8().data());
}

void webview::executeJS(const char *str) {

	if (!str)
		return;

	Frame * const f = &priv->page->mainFrame();
	if (!f)
		return;

	f->script().executeScript(String::fromUTF8(str), true);
}

// Settings

void webview::setBool(const SettingBool item, const bool val) {

	Settings &set = priv->page->mainFrame().settings();

	switch (item) {
		case WK_SETTING_JS:
			set.setScriptEnabled(val);
		break;
		case WK_SETTING_CSS:
			set.setAuthorAndUserStylesEnabled(val);
		break;
		case WK_SETTING_IMG:
			set.setImagesEnabled(val);
		break;
		case WK_SETTING_LOCALSTORAGE:
			set.setLocalStorageEnabled(val);
		break;
		case WK_SETTING_QUIET_JS_DIALOGS:
			priv->quietdiags = val;
		break;
	}
}

bool webview::getBool(const SettingBool item) const {

	Settings &set = priv->page->mainFrame().settings();

	switch (item) {
		case WK_SETTING_JS:
			return set.isScriptEnabled();
		break;
		case WK_SETTING_CSS:
			return set.authorAndUserStylesEnabled();
		break;
		case WK_SETTING_IMG:
			return set.areImagesEnabled();
		break;
		case WK_SETTING_LOCALSTORAGE:
			return set.localStorageEnabled();
		break;
		case WK_SETTING_QUIET_JS_DIALOGS:
			return priv->quietdiags;
		break;
	}

	fprintf(stderr, "Error, tried to fetch unknown bool setting %u\n",
		item);
	return false;
}

void webview::setDouble(const SettingDouble item, const double val) {

	//Settings &set = priv->page->mainFrame().settings();

	switch (item) {
		case WK_SETTING_ZOOM:
			priv->page->mainFrame().setPageZoomFactor(val);
		break;
	}
}

double webview::getDouble(const SettingDouble item) const {

	//Settings &set = priv->page->mainFrame().settings();

	switch (item) {
		case WK_SETTING_ZOOM:
			return priv->page->mainFrame().pageZoomFactor();
		break;
	}

	fprintf(stderr, "Error, tried to fetch unknown double setting %u\n",
		item);
	return 0;
}

void webview::setInt(const SettingInt item, const int val) {

	Settings &set = priv->page->mainFrame().settings();

	switch (item) {
		case WK_SETTING_FONT_SIZE:
			set.setDefaultFontSize(val);
		break;
		case WK_SETTING_FIXED_SIZE:
			return set.setDefaultFixedFontSize(val);
		break;
		case WK_SETTING_MINIMUM_FONT_SIZE:
			return set.setMinimumFontSize(val);
		break;
	}
}

int webview::getInt(const SettingInt item) const {

	Settings &set = priv->page->mainFrame().settings();

	switch (item) {
		case WK_SETTING_FONT_SIZE:
			return set.defaultFontSize();
		break;
		case WK_SETTING_FIXED_SIZE:
			return set.defaultFixedFontSize();
		break;
		case WK_SETTING_MINIMUM_FONT_SIZE:
			return set.minimumFontSize();
		break;
	}

	fprintf(stderr, "Error, tried to fetch unknown int setting %u\n",
		item);
	return 0;
}

void webview::setChar(const SettingChar item, const char *val) {

	Settings &set = priv->page->mainFrame().settings();

	switch (item) {
		case WK_SETTING_DEFAULT_FONT:
			set.setStandardFontFamily(val);
		break;
		case WK_SETTING_FIXED_FONT:
			set.setFixedFontFamily(val);
		break;
		case WK_SETTING_USER_CSS:
			if (val) {
				String base = "file://";
				base.append(String::fromUTF8(val));
				set.setUserStyleSheetLocation(URL(URL(), base));
			} else {
				set.setUserStyleSheetLocation(blankURL());
			}
		break;
	}
}

const char *webview::getChar(const SettingChar item) const {

	Settings &set = priv->page->mainFrame().settings();

	switch (item) {
		case WK_SETTING_DEFAULT_FONT:
			return strdup(set.standardFontFamily().string().utf8().data());
		break;
		case WK_SETTING_FIXED_FONT:
			return strdup(set.fixedFontFamily().string().utf8().data());
		break;
		case WK_SETTING_USER_CSS:
			return strdup(set.userStyleSheetLocation().string().utf8().data());
		break;
	}

	fprintf(stderr, "Error, tried to fetch unknown char setting %u\n",
		item);
	return NULL;
}

typedef void (*eventfunc)(const char *name, const char *id,
			const char *cssclass, const char *value);

class FlEventListener: public EventListener {
public:
	FlEventListener(Element *elem, eventfunc f):
				EventListener(FlEventListenerType),
				m_elem(elem), m_func(f) {
	}

	void handleEvent(ScriptExecutionContext*, Event*) override {
		String valuestr;
		if (is<HTMLInputElement>(*m_elem))
			valuestr = toHTMLInputElement(m_elem)->value();
		else
			valuestr = m_elem->getAttribute("value").string();

		m_func(m_elem->getAttribute("name").string().utf8().data(),
			m_elem->getAttribute("id").string().utf8().data(),
			m_elem->getAttribute("class").string().utf8().data(),
			valuestr.utf8().data());
	}

	bool operator==(const EventListener &other) {
		if (other.type() == FlEventListenerType) {
			const FlEventListener &e = (const FlEventListener &) other;
			return m_elem == e.m_elem && m_func == e.m_func;
		}
		return false;
	}

private:
	Element *m_elem;
	eventfunc m_func;
};

void webview::bindEvent(const char *element, const char *type, const char *event,
			void (*func)(const char *name, const char *id,
					const char *cssclass, const char *value),
			const bool capture) {

	RefPtr<NodeList> elem = priv->page->mainFrame().document()->getElementsByTagName(element);
	unsigned max = elem->length();
	unsigned i;

	for (i = 0; i < max; i++) {
		Node *n = elem->item(i);
		Element *e = toElement(n);

		if (type) {
			if (!is<HTMLInputElement>(*n))
				continue;

			const CString &typestr = e->getAttribute("type").string().utf8();
			const char *hastype = typestr.data();

			if (strcmp(hastype, type))
				continue;
		}

		RefPtr<FlEventListener> l(adoptRef(new FlEventListener(e, func)));
		e->addEventListener(event, l.release(), capture);
	}
}

const char *webview::getValue(const char *element, const char *type, const char *cssclass) {

	RefPtr<NodeList> elem = priv->page->mainFrame().document()->getElementsByTagName(element);
	unsigned max = elem->length();
	unsigned i;

	for (i = 0; i < max; i++) {
		Node *n = elem->item(i);
		Element *e = toElement(n);

		if (type) {
			if (!is<HTMLInputElement>(*n))
				continue;

			const CString &typestr = e->getAttribute("type").string().utf8();
			const char *hastype = typestr.data();

			if (strcmp(hastype, type))
				continue;
		}

		if (cssclass) {
			const CString &classstr = e->getAttribute("class").string().utf8();
			const char *hasclass = classstr.data();

			if (strcmp(cssclass, hasclass))
				continue;
		}

		if (is<HTMLInputElement>(*n))
			return strdup(toHTMLInputElement(e)->value().utf8().data());
		else
			return strdup(e->getAttribute("value").string().utf8().data());
	}

	return NULL;
}

void webview::emulateClick(const char *element, const char *type, const char *cssclass) {

	RefPtr<NodeList> elem = priv->page->mainFrame().document()->getElementsByTagName(element);
	unsigned max = elem->length();
	unsigned i;

	for (i = 0; i < max; i++) {
		Node *n = elem->item(i);
		Element *e = toElement(n);

		if (type) {
			if (!is<HTMLInputElement>(*n))
				continue;

			const CString &typestr = e->getAttribute("type").string().utf8();
			const char *hastype = typestr.data();

			if (strcmp(hastype, type))
				continue;
		}

		if (cssclass) {
			const CString &classstr = e->getAttribute("class").string().utf8();
			const char *hasclass = classstr.data();

			if (strcmp(cssclass, hasclass))
				continue;
		}

		e->dispatchSimulatedClick(nullptr, SendNoEvents, DoNotShowPressedLook);
		return;
	}
}

unsigned webview::getLinkDetails(const char *cssclass, char **hrefs, char **texts,
					const unsigned allocated) {

	RefPtr<NodeList> elem = priv->page->mainFrame().document()->getElementsByTagName("a");
	unsigned max = elem->length();
	unsigned i;
	unsigned cur = 0;

	for (i = 0; i < max; i++) {
		Node *n = elem->item(i);
		HTMLAnchorElement *e = toHTMLAnchorElement(n);

		if (cssclass) {
			const CString &classstr = e->getAttribute("class").string().utf8();
			const char *hasclass = classstr.data();

			if (strcmp(cssclass, hasclass))
				continue;
		}

		if (cur >= allocated)
			return cur;

		hrefs[cur] = strdup(e->href().string().utf8().data());
		texts[cur] = strdup(e->text().utf8().data());
		cur++;
	}

	return cur;
}

bool webview::isNoGui() const {
	return noGUI;
}
