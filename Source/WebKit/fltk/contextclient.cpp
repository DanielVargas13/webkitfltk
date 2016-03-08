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
#include "contextclient.h"
#include "webviewpriv.h"

#include <ContextMenu.h>
#include <ContextMenuController.h>
#include <DragController.h>
#include <HitTestResult.h>
#include <NotImplemented.h>

extern void (*bgtabfunc)(const char*);
extern webview *(*popupfunc)(const char *);
extern void (*persitesettingsfunc)(const char*);

using namespace WebCore;

FlContextMenuClient::FlContextMenuClient(webview *inview) {
	view = inview;
}

void FlContextMenuClient::contextMenuDestroyed() {
	delete this;
}

enum {
	ctxOpenInBGTab = ContextMenuItemBaseApplicationTag + 1,
	ctxTineye,
	ctxViewSource,
	ctxPerSiteSettings,
};

void FlContextMenuClient::contextMenuItemSelected(ContextMenuItem *it,
	const ContextMenu*) {

	const HitTestResult &hit = view->priv->page->contextMenuController().hitTestResult();
	Frame* frame = hit.innerNonSharedNode()->document().frame();

	const Element * const hitelem = hit.URLElement();

	switch ((unsigned) it->action()) {
		case ctxOpenInBGTab:
			if (hitelem && isDraggableLink(*hitelem) && bgtabfunc) {
				bgtabfunc(hit.absoluteLinkURL().string().utf8().data());
			}
		break;
		case ctxTineye:
			if (!hit.absoluteImageURL().isEmpty() &&
				!hit.absoluteImageURL().protocol().contains("file") &&
				popupfunc) {
				char tmp[1024];
				const String enc = encodeWithURLEscapeSequences(hit.absoluteImageURL().string());
				snprintf(tmp, 1024, "http://www.tineye.com"
					"/search/?url=%s", enc.ascii().data());

				popupfunc(tmp);
			}
		break;
		case ctxViewSource:
			if (frame) {
				char *src = view->focusedSource();
				if (src && popupfunc) {
					webview *newview = popupfunc("");
					if (newview)
						newview->loadString(src, "text/plain");
					free(src);
				}
			}
		break;
		case ctxPerSiteSettings:
			if (frame && persitesettingsfunc) {
				persitesettingsfunc(view->url());
			}
		break;
		default:
			printf("Unknown action %u\n", it->action());
	}
}

void FlContextMenuClient::downloadURL(const URL &url) {
	view->download(url.string().utf8().data(),
			url.lastPathComponent().utf8().data());
}

void FlContextMenuClient::searchWithGoogle(const Frame*) {
	notImplemented();
}

void FlContextMenuClient::lookUpInDictionary(Frame*) {
	notImplemented();
}

bool FlContextMenuClient::isSpeaking() {
	notImplemented();
	return false;
}

void FlContextMenuClient::speak(const WTF::String&) {
	notImplemented();
}

void FlContextMenuClient::stopSpeaking() {
	notImplemented();
}

std::unique_ptr<ContextMenu> FlContextMenuClient::customizeMenu(std::unique_ptr<ContextMenu> m) {

	Vector<ContextMenuItem> newitems;

	const HitTestResult &hit = view->priv->page->contextMenuController().hitTestResult();

	unsigned i;
	for (i = 0; i < m->items().size(); i++) {
		const ContextMenuItem &cur = m->items()[i];

		switch (cur.action()) {
			// Remove some unsupported menu items
			case ContextMenuItemTagSpellingMenu:
			case ContextMenuItemTagSearchWeb:
			case ContextMenuItemTagStartSpeaking:
			case ContextMenuItemTagStopSpeaking:
			case ContextMenuItemTagOpenMediaInNewWindow:
			case ContextMenuItemTagToggleMediaControls:
			case ContextMenuItemTagToggleMediaLoop:
			case ContextMenuItemTagEnterVideoFullscreen:
			case ContextMenuItemTagMediaPlayPause:
			case ContextMenuItemTagMediaMute:
			case ContextMenuItemTagToggleVideoFullscreen:
			break;
			case ContextMenuItemTagOpenLinkInNewWindow:
			{
				newitems.append(cur);
				ContextMenuItem c(ActionType,
						(ContextMenuAction) ctxOpenInBGTab,
						"Open Link in Background Tab",
						true, false);
				newitems.append(c);
			}
			break;
			case ContextMenuItemTagReload:
			{
				newitems.append(cur);

				if (popupfunc) {
					ContextMenuItem c(ActionType,
							(ContextMenuAction) ctxViewSource,
							"View frame source",
							true, false);
					newitems.append(c);
				}

				if (persitesettingsfunc) {
					ContextMenuItem c(ActionType,
							(ContextMenuAction) ctxPerSiteSettings,
							"Edit per-site settings",
							true, false);
					newitems.append(c);
				}
			}
			break;
			default:
				newitems.append(cur);
			break;
		}
	}

	if (!hit.absoluteImageURL().isEmpty() &&
		!hit.absoluteImageURL().protocol().contains("file")) {
		ContextMenuItem c(ActionType,
				(ContextMenuAction) ctxTineye,
				"Reverse Search Image on TinEye",
				true, false);
		newitems.append(c);
	}

	m->setItems(newitems);

	return m;
}

ContextMenuItem shareMenuItem(const HitTestResult&) {
	// We do not support sharing.
	return ContextMenuItem();
}
