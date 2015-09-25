/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef AESOP_WINDOWS_H
#define AESOP_WINDOWS_H

#include "common/rect.h"
#include "common/array.h"
#include "graphics/surface.h"
#include "aesop/interpreter.h"

namespace Aesop {

extern byte *fade_tables[5][16];
extern const byte text_colors[9];
extern const int first_color[5];
extern const int num_colors[5];

#define NTW                 32
#define PALETTE_COUNT       256
#define PALETTE_SIZE        (256 * 3)
#define VGA_COLOR_TRANS(x) ((x) * 255 / 63)

enum {
    NO_COLOR = 0
};

enum LineDraw { LD_DRAW = 0, LD_TRANSLATE = 1, LD_EXECUTE = 3 };
enum PaneScroll { PS_NOWRAP = 0, PS_WRAP = 1 };
enum ShapeTransform {
    ST_XLAT = 0,        // Use shape_lookaside() table
    ST_REUSE = 1        // Use buffer contents from prior call
};
enum Justification { J_LEFT = 0, J_RIGHT = 1, J_CENTER = 2, J_FILL = 3 };
enum { PAGE1 = 0, PAGE2 = 1 };
enum PrintOperation {
    WND = 0,        // output directly to window
    BUF = 1,        // output to start of buffer
    APP = 2         // append to end of buffer
};

class AesopEngine;
class Window;

class Pane : public Common::Rect {
public:
	Window *_window;
	int _owner;
public:
	Pane();
	Pane(Window *window);
	Pane(Window *window, const Common::Rect &bounds);
	virtual ~Pane() {}

	/**
	 * Get the left bound of the pane
	 */
	int getLeft() const;

	/**
	 * Get the top bound of the pane
	 */
	int getTop() const;

	/**
	 * Get the right bound of the pane
	 */
	int getRight() const;

	/**
	 * Get the bottom bound of the pane
	 */
	int getBottom() const;

	/**
	 * Set the left bound of the pane
	 */
	void setLeft(int x1);

	/**
	 * Set the top bound of the pane
	 */
	void setTop(int y1);

	/**
	 * Set the right bound of the pane
	 */
	void setRight(int x2);

	/**
	 * Set the bottom bound of the pane
	 */
	void setBottom(int y2);

	/**
	 * Wipe the pane with a specified color
	 */
	void paneWipe(byte color);

	/**
	 * Returns number of colors used in the specified window
	 */
	int colorScan(uint32 *colors);

	bool testOverlap(const Common::Point &pt, byte *shapes, int shape_num);

	void scroll(const Common::Point &delta, PaneScroll flags, byte background);

	int drawCharacter(const Common::Point &pt, void *font, int character, byte *color_translate);
public:
	/**
	 * Read a pixel
	 */
	virtual byte readDot(const Common::Point &pt);

	/**
	 * Draw a pixel
	 */
	virtual void drawDot(const Common::Point &pt, byte color);

	/**
	 * Draw a line
	 */
	virtual void drawLine(const Common::Point &src, const Common::Point &dest, byte color);

	/**
	 * Draw a rectangle with a given color
	 */
	virtual void drawRect(const Common::Rect &r, byte color);

	/**
	 * Fill a rectangular area with a given color
	 */
	virtual void fillRect(const Common::Rect &r, byte color);

	/**
	 * Hash the specified area
	 */
	virtual void hashRect(const Common::Rect &r, byte color);

	/**
	 * Return a specified sub-area for updating
	 */
	virtual Graphics::Surface getArea(const Common::Rect &r);

	/**
	 * Return an address for a pixel within the given pane.
	 */
	virtual byte *getBasePtr(int x, int y);

	/**
	 * Copy an area from one surfaec to another
	 */
	virtual void blitFrom(Pane &src, const Common::Point &destPos, const Common::Rect &srcRect);

	/**
	 * Copy one surface onto another
	 */
	virtual void blitFrom(Pane &src);
};

class Window : public Pane {
	friend class Pane;
protected:
	Graphics::Surface _surface;

	/**
	 * Called by any graphic routine to signify an area of the window that has been updated.
	 * Used for performance blitting on the screen window to only update affected areas
	 */
	virtual void addDirtyRect(const Common::Rect &r) {}
public:
	Window();
	Window(const Common::Point &size);
	virtual ~Window();

	/**
	 * Wipe the window with a specified color
	 */
	void wipeWindow(byte color);
	
	/**
	 * Copy a specified area of the given window to the screen
	 */
	void refresh(const Common::Rect &r);

	void fade(const byte *palette, int intervals);
public:
	/**
	 * Read a pixel
	 */
	virtual byte readDot(const Common::Point &pt);

	/**
	 * Draw a pixel
	 */
	virtual void drawDot(const Common::Point &pt, byte color);

	/**
	 * Draw a line
	 */
	virtual void drawLine(const Common::Point &src, const Common::Point &dest, byte color);

	/**
	 * Draw a rectangle
	 */
	virtual void drawRect(const Common::Rect &r, byte color);

	/**
	 * Fill a rectangular area with a given color
	 */
	virtual void fillRect(const Common::Rect &r, byte color);

	/**
	 * Hash the specified area
	 */
	virtual void hashRect(const Common::Rect &r, byte color);

	/**
	 * Return a specified sub-area for updating
	 */
	virtual Graphics::Surface getArea(const Common::Rect &r);

	/**
	 * Return an address for a pixel within the given pane.
	 */
	virtual byte *getBasePtr(int x, int y);
};

class Font {
public:
	byte font_background;
	int char_height;
public:
	int charWidth(int c);
};

class TextWindow {
public:
	int window;
	int htab, vtab;
	Font *font;
	int delay;
	int (*continueFunction)(int);
	char *txtbuf;
	char *txtpnt;
	int justify;
	byte lookaside[256];
	int _refresh;
public:
	TextWindow();

	/**
	 * Set the text position in the window
	 */
	void setPosition(const Common::Point &pt);

	int getX() const {
		return htab;
	}

	int getY() const {
		return vtab;
	}

	void home();

	void textColor(byte current, byte newColor);

	void remapFontColor(byte current, byte newColor);

	void selectTextWindow();

	void refreshWindow(int wnd);

	uint charWidth(int ch);

	uint fontHeight();

	void vsprint(const char *format, va_list argptr);

	void print(const char *format, ...);

	void sprint(const char *format, ...);

	void crout();
};

} // End of namespace Aesop

#endif

