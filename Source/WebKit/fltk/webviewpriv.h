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

#ifndef webviewpriv_h
#define webviewpriv_h

#include "chromeclient.h"
#include "contextclient.h"
#include "download.h"
#include "dragclient.h"
#include "editorclient.h"
#include "inspectorclient.h"
#include "frameclient.h"
#include "progressclient.h"

#include <EventHandler.h>
#include <GraphicsContext.h>
#include <Page.h>
#include <wtf/text/CString.h>

#include <time.h>
#include <unordered_set>
#include <vector>

typedef unsigned long Pixmap;

class privatewebview {
public:
	WebCore::Page *page;

	cairo_t *cairo;
	cairo_surface_t *cairosurf;
	WebCore::GraphicsContext *gc;
	Pixmap cairopix;

	Fl_Window *window;
	unsigned depth;
	unsigned w, h;

	int clipx, clipy, clipw, cliph;

	struct timespec lastdraw;

	bool editing;

	const char *statusbartext;
	const char *title;
	const char *url;
	bool hoveringlink;

	bool quietdiags;

	std::vector<download *> downloads;

	std::unordered_set<int> pressedkeys;

	// Callbacks
	void (*titleChanged)();
	void (*loadStateChanged)(webview *);
	void (*progressChanged)(webview *, float);
	void (*faviconChanged)(webview *);
	void (*statusChanged)(webview *);
	void (*historyAdd)(const char *url, const char *title, const time_t when);
	void (*siteChanging)(webview *, const char *url);
	void (*error)(webview *, const char *err);
	void (*resourceStateChanged)(unsigned long id, bool finished);
};

#endif
