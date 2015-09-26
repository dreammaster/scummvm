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

#ifndef AESOP_SCREEN_H
#define AESOP_SCREEN_H

#include "common/rect.h"
#include "common/array.h"
#include "graphics/surface.h"
#include "aesop/interpreter.h"
#include "aesop/windows.h"
#include "aesop/text_windows.h"

namespace Aesop {

extern byte *fade_tables[5][16];
extern const byte text_colors[9];
extern const int first_color[5];
extern const int num_colors[5];

#define AESOP_SCREEN_WIDTH  320
#define AESOP_SCREEN_HEIGHT 200
#define MAX_WINDOWS         50
#define MAX_PANES           250
#define MAX_TEXT_WINDOWS	32

class Screen: public Window {
private:
	AesopEngine *_vm;
	bool _active;
	int _lastPage;
	Common::Point _lastPos;
	Common::List<Common::Rect> _dirtyRects;

	/**
	 * Merge together overlapping dirty rects
	 */
	void mergeDirtyRects();

	/**
	 * Returns the union of two dirty area rectangles
	 */
	bool unionRectangle(Common::Rect &destRect, const Common::Rect &src1, const Common::Rect &src2);
protected:
	/**
	 * Add an area to the screen's dirty rect list
	 */
	virtual void addDirtyRect(const Common::Rect &r);
public:
	Common::Array<Window *> _windows;
	Common::Array<Pane *> _panes;
	TextWindow _textWindows[MAX_TEXT_WINDOWS];
	Graphics::Surface _bitmapBuffer;
	TextWindow *_twptr;
	char _strBuf[256];              // used for string resource buffering
	char _txtBuf[2402];             // used for word-wrapped text
public:
	Screen(AesopEngine *vm);
	virtual ~Screen();

	/**
	 * Update the physical screen
	 */
	void update();

	/**
	 * Sets up a new window, allocating space for it
	 */
	uint assignWindow(const Common::Rect &r);

	/**
	 * Sets up a new window, and assigns an owner to it
	 */
	uint assignWindow(int owner, const Common::Rect &r);

	/**
	 * Create a new pane which references a sub-section of an existing window
	 */
	uint assignSubWindow(uint wnd, const Common::Rect &r);

	/**
	 * Create a new pane and assign an owner
	 */
	uint assignSubWindow(int owner, uint wnd, const Common::Rect &r);

	/**
	 * Release a previously allocated window or pane
	 */
	void releaseWindow(uint wnd);

	/**
	 * Release all windows assigned to owner, where owner refers to any program or entity object.
	 *
	 * If owner = -1, release all assigned windows except PAGE1, PAGE2, and windows assigned
	 * to program objects
	 */
	void releaseOwnedWindows(int owner);

	/**
	 * Returns the specified pane
	 */
	Pane &panes(uint idx);

	/**
	 * Returns the specified window
	 */
	Window &windows(uint idx);

	/**
	 * Returns the specified text window
	 */
	TextWindow &textWindows(uint idx);

	/**
	 * Set a palette color range from a palette resource file
	 *
	 * Copy "fade tables" from resource file; dynamically build
	 * special shade tables for blue/green/red/gray/white/etc. effects
	 */
	void setPalette(uint region, uint resource);

	/**
	 * Set a palette entry from the passed RGB triplet
	 */
	void writePalette(byte index, const byte *rgb);

	/**
	 * Set a palette entry from an encoded RGB value
	 */
	void writePalette(byte index, uint value);

	/**
	 * Read a palette entry and return it's RGB value
	 */
	uint readPalette(byte index);

	/**
	 * Read a palette entry and save it in the passed rgb triplet
	 */
	void readPalette(byte index, byte *rgb);

	/**
	 * Do a pixel transition from one window to another
	 */
	void pixelFade(int srcWnd, int destWnd, int intervals);

	void colorFade(int srcWnd, int destWnd);

	void lightFade(uint srcWnd, byte color);

	void selectTextWindow(TextWindow *tw);

	int charWidth(int ch);

	void home();

	void remapFontColor(byte current, byte newColor);

	void print(PrintOperation operation, const char *format, ...);

	void printBuffer(int linenum);

	void cout(int c);

	/**
	 * Set the window for a given text window
	 */
	void setTextWindow(uint wndNum, uint wnd);

	/**
	 * Establish font name and justification mode for a given text window
	 *
	 * The font is guaranteed to remain locked in memory until another textStyle()
	 * call is issued for the window
	 */
	void textStyle(uint wndnum, uint font, uint justify);

	void drawDot(uint page, const Common::Point &pos, byte color);

	void drawLine(uint page, const Common::Point &src, const Common::Point &dest, byte color);

	void lineTo(Common::Array<Parameter> args);

	void solidBarGraph(const Common::Rect &r, uint32 lb_border, uint32 tr_border, uint32 bkgnd,
	                   uint32 grn, uint32 yel, uint32 red, int val, int min, int crit, int max);

	void drawBitmap(int page, uint table, int number, const Common::Point &pt,
		uint scale, uint flip, uint fadeTableNum, uint fadeLevelNum);
};

typedef int FIXED16;

extern void VFX_shape_draw(Pane *pane, void *shape_table, LONG shape_number, LONG hotX, LONG hotY);
extern int VFX_shape_resolution(byte *data, int shape_num);

} // End of namespace Aesop

#endif

