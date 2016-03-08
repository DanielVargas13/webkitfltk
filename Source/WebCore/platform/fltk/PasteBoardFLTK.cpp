/*
 *  Copyright (C) 2007 Holger Hans Peter Freyther
 *  Copyright (C) 2007 Alp Toker <alp@atoker.com>
 *  Copyright (C) 2008 INdT - Instituto Nokia de Tecnologia
 *  Copyright (C) 2009-2010 ProFUSION embedded systems
 *  Copyright (C) 2009-2010 Samsung Electronics
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "Pasteboard.h"

#include "NotImplemented.h"
#include <wtf/text/CString.h>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/x.H>
#include <X11/Xlib.h>

extern Window fl_message_window;

class paster_t: public Fl_Widget {
public:
	paster_t(): Fl_Widget(0, 0, 0, 0), text(NULL) {}

	int handle(int e) {
		text = NULL;
		if (e == FL_PASTE) {
			text = Fl::event_text();
			return 1;
		}

		return Fl_Widget::handle(e);
	}

	void draw() {}

	const char *text;
};

namespace WebCore {

Pasteboard::Pasteboard()
{
}

void Pasteboard::writePlainText(const String &str, SmartReplaceOption)
{
    const CString &cstr = str.utf8();
    Fl::copy(cstr.data(), cstr.length());
}

void Pasteboard::clear()
{
    notImplemented();
}

bool Pasteboard::canSmartReplace()
{
    notImplemented();
    return false;
}

std::unique_ptr<Pasteboard> Pasteboard::createForCopyAndPaste()
{
    return std::make_unique<Pasteboard>();
}

std::unique_ptr<Pasteboard> Pasteboard::createPrivate()
{
    return createForCopyAndPaste();
}

#if ENABLE(DRAG_SUPPORT)
std::unique_ptr<Pasteboard> Pasteboard::createForDragAndDrop()
{
    return createForCopyAndPaste();
}

std::unique_ptr<Pasteboard> Pasteboard::createForDragAndDrop(const DragData&)
{
    return createForCopyAndPaste();
}
#endif

bool Pasteboard::hasData()
{
    return false;
}

void Pasteboard::clear(const String&)
{
    clear();
}

void Pasteboard::read(PasteboardPlainText &text)
{
    static paster_t *paster = NULL;
    if (!paster) {
        Fl_Group * const oldgroup = Fl_Group::current();
        Fl_Group::current(NULL);

        paster = new paster_t;

        Fl_Group::current(oldgroup);
    }

    const Window owner = XGetSelectionOwner(fl_display, XA_PRIMARY);
    if (owner == None) {
        text.text = "";
        return;
    } else if (owner == fl_message_window) {
        Fl::paste(*paster, 0, Fl::clipboard_plain_text);
        if (paster->text)
            text.text = String::fromUTF8(paster->text);
        else
            text.text = "";
        return;
    }

    XConvertSelection(fl_display, XA_PRIMARY, XA_STRING, XA_STRING, fl_window, CurrentTime);
    XFlush(fl_display);

    unsigned i;
    Atom target = None;
    for (i = 0; i < 10; i++) {
        XEvent ev;
        XNextEvent(fl_display, &ev);
        if (ev.type == SelectionNotify) {
            target = ev.xselection.property;
            if (target == None) return;
            break;
        }
    }

    Atom type;
    int fmt;
    unsigned long num, bytes = 0, dummy;
    unsigned char *data;
    XGetWindowProperty(fl_display, fl_window, XA_STRING, 0, 0, 0, AnyPropertyType,
                       &type, &fmt, &num, &bytes, &data);
    if (bytes) {
        int res = XGetWindowProperty(fl_display, fl_window, XA_STRING, 0, bytes, 0,
                                     AnyPropertyType, &type, &fmt, &num, &dummy, &data);
        if (res == Success) {
            text.text = String::fromUTF8(data);
            free(data);
        }
    }
}

String Pasteboard::readString(const String&)
{
    PasteboardPlainText t;
    read(t);
    return t.text;
}

void Pasteboard::write(const PasteboardURL &url)
{
    writePlainText(url.url.string(), CanSmartReplace);
}

void Pasteboard::writeString(const String &type, const String &data)
{
    writePlainText(data, CanSmartReplace);
}

Vector<String> Pasteboard::types()
{
    Vector<String> types;

    types.append(ASCIILiteral("text/plain"));
    types.append(ASCIILiteral("Text"));
    types.append(ASCIILiteral("text"));

    return types;
}

Vector<String> Pasteboard::readFilenames()
{
    Vector<String> names;

    names.append(readString(""));

    return names;
}

#if ENABLE(DRAG_SUPPORT)
void Pasteboard::setDragImage(DragImageRef, const IntPoint&)
{
    notImplemented();
}
#endif

void Pasteboard::writePasteboard(const Pasteboard&)
{
    notImplemented();
}

}
