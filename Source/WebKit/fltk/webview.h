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

#ifndef webview_h
#define webview_h

#include <FL/Fl_Widget.H>

class privatewebview;

enum SettingBool {
	WK_SETTING_JS = 0,
	WK_SETTING_CSS,
	WK_SETTING_IMG,
	WK_SETTING_LOCALSTORAGE,
};

enum SettingDouble {
	WK_SETTING_ZOOM = 0,
};

enum SettingInt {
	WK_SETTING_FONT_SIZE = 0,
	WK_SETTING_FIXED_SIZE,
	WK_SETTING_MINIMUM_FONT_SIZE,
};

enum SettingChar {
	WK_SETTING_DEFAULT_FONT = 0,
	WK_SETTING_FIXED_FONT,
	WK_SETTING_USER_CSS,
};

class webview: public Fl_Widget {
public:
	webview(int x, int y, int w, int h, bool noGui = false);
	~webview();

	void draw() override;
	void drawWeb();

	int handle(int) override;

	void resize();
	void resize(int x, int y, int w, int h) override;

	void load(const char *);
	void loadString(const char * const str, const char * const mime = NULL,
			const char * const enc = NULL, const char * const baseurl = NULL,
			const void * const frame = NULL);

	void show() override;
	void hide() override;

	void download(const char *url, const char *suggestedname = NULL,
			const void *req = NULL);

	const char *statusbar() const;

	const char *title() const;
	const char *url() const;

	void snapshot(const char *);

	// Return the malloced source code of the focused frame
	char *focusedSource() const;

	void executeJS(const char *);

	// Download handling
	unsigned numDownloads() const;
	void stopDownload(const unsigned);
	void removeDownload(const unsigned);
	bool downloadFinished(const unsigned) const;
	bool downloadFailed(const unsigned) const;
	void downloadStats(const unsigned, time_t *start, long long *size,
				long long *received,
				const char **name, const char **url) const;

	privatewebview *priv;

	// History
	void back();
	void fwd();
	bool canBack() const;
	bool canFwd() const;
	void prev(); // All the way back
	void next(); // Next page, if any (find the link called "next")

	void stop();
	void refresh();
	bool isLoading() const;

	// Editing
	void undo();
	void redo();
	void selectAll();
	void copy();
	void cut();
	void paste();
	bool find(const char *what, bool caseSensitive = false, bool forward = true);
	unsigned countFound(const char *what, bool caseSensitive = false);

	// Settings
	void setBool(const SettingBool, const bool);
	bool getBool(const SettingBool) const;

	void setDouble(const SettingDouble, const double);
	double getDouble(const SettingDouble) const;

	void setInt(const SettingInt, const int);
	int getInt(const SettingInt) const;

	void setChar(const SettingChar, const char *);
	const char *getChar(const SettingChar) const;

	// Callbacks
	void titleChangedCB(void (*func)());
	void loadStateChangedCB(void (*func)(webview *));
	void progressChangedCB(void (*func)(webview *, float));
	void faviconChangedCB(void (*func)(webview *));
	void statusChangedCB(void (*func)(webview *));
	void historyAddCB(void (*func)(const char *url, const char *title, const time_t when));
	void siteChangingCB(void (*func)(webview *, const char *url));
	void errorCB(void (*error)(webview *, const char *err));
	void resourceStateChangedCB(void (*resourceStateChanged)(unsigned long id, bool finished));

	// Bind a callback to element action. Call after loading has finished.
	void bindEvent(const char *element, const char *type, const char *event,
			void (*func)(const char *name, const char *id,
			const char *cssclass, const char *value),
			const bool capture = false);
	// Get the malloced value of an element with optional type and css class
	const char *getValue(const char *element, const char *type = NULL,
				const char *cssclass = NULL);


	bool isNoGui() const;
private:
	void handlecontextmenu(void *);
	bool noGUI;
};

#endif
