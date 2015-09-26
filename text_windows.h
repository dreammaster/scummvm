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

#ifndef AESOP_TEXT_WINDOWS_H
#define AESOP_TEXT_WINDOWS_H

#include "graphics/surface.h"
#include "aesop/interpreter.h"
#include "aesop/windows.h"

namespace Aesop {

enum Justification { J_LEFT = 0, J_RIGHT = 1, J_CENTER = 2, J_FILL = 3 };
enum PrintOperation {
    WND = 0,        // output directly to window
    BUF = 1,        // output to start of buffer
    APP = 2         // append to end of buffer
};

class Window;

class Font {
private:
	byte *_data;
public:
	Common::Array<byte *>_chars;
	byte _fontBackground;
	int _charHeight;
	int _version;
public:
	Font();
	~Font();

	/**
	 * Return the width of a specified character
	 */
	int charWidth(int c);

	/**
	 * Load the data for the font
	 */
	void load(Common::SeekableReadStream &s);

	/**
	 * Clear the currently loaded font
	 */
	void clear();
};

class TextWindow {
private:
	static AesopEngine *_vm;
	bool _endOfPageReached;
	int _pendingFlag;

	/**
	 * Print a line from the buffer
	 */
	void printBuffer(int lineNum);

	/**
	 * Print a character
	 */
	void cout(char c);
public:
	int _window;
	int _hTab, _vTab;
	Font _font;
	int _delay;
	int (*_continueFunction)(int);
	Common::String _textBuffer;
	Justification _justify;
	byte _lookaside[256];
	int _refresh;
public:
	TextWindow();
	static void init(AesopEngine *vm);

	/**
	 * Get the window the text window is associated with
	 */
	Pane &window();

	/**
	 * Set the text position in the window
	 */
	void setPosition(const Common::Point &pt);

	int getX() const { return _hTab; }

	int getY() const { return _vTab; }

	void home();

	void textColor(byte current, byte newColor);

	void remapFontColor(byte current, byte newColor);

	void selectTextWindow();

	void refreshWindow(int wnd);

	uint charWidth(int ch);

	uint fontHeight();

	/**
	 * Print the specified string
	 */
	void print(PrintOperation operation, const Common::String &format, const Parameters &params);

	void vsprint(const char *format, va_list argptr);

	void print(const char *format, ...);

	void sPrint(const Common::String &format, const Parameters &params);

	void crout();
};

} // End of namespace Aesop

#endif

