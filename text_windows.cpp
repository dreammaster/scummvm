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

#include "aesop/aesop.h"
#include "aesop/text_windows.h"

namespace Aesop {

Font::Font() {
	_data = nullptr;
	_charHeight = 0;
}

Font::~Font() {
	clear();
}

int Font::charWidth(int c) {
	error("TODO");
}

void Font::load(Common::SeekableReadStream &s) {
	_version = s.readUint32LE();
	int charCount = s.readUint32LE();
	_charHeight = s.readUint32LE();
	_fontBackground = s.readUint32LE();

	int dataSize = s.size() - 16 - charCount * 4;
	_data = new byte[dataSize];

	// Load in each charcater offset
	for (int idx = 0; idx < charCount; ++idx) {
		uint offset = s.readUint32LE();
		_chars.push_back(&_data[offset - 16]);
	}

	// Read in the character data
	s.read(_data, dataSize);
}

void Font::clear() {
	delete[] _data;
	_data = nullptr;
	_chars.clear();
	_fontBackground = 0;
	_charHeight = 0;
}

/*----------------------------------------------------------------*/

TextWindow::TextWindow() {
	_window = 0;
	_hTab = _vTab = 0;
	_delay = 0;
	_continueFunction = nullptr;
	_txtBuf = nullptr;
	_txtPnt = nullptr;
	_justify = J_LEFT;
	_refresh = 0;

	for (int idx = 0; idx < 256; ++idx)
		_lookaside[idx] = idx;
}

void TextWindow::setPosition(const Common::Point &pt) {
	_hTab = pt.x;
	_vTab = pt.y;
}

void TextWindow::home() {
	selectTextWindow();

	error("TODO");
}

void TextWindow::textColor(byte current, byte newColor) {
	selectTextWindow();
	remapFontColor(current, newColor);
}

void TextWindow::remapFontColor(byte current, byte newColor) {
	_lookaside[current] = newColor;
}

void TextWindow::selectTextWindow() {
	_vm->_screen->_twptr = this;
}

void TextWindow::refreshWindow(int wnd) {
	_refresh = wnd;
}

uint TextWindow::charWidth(int ch) {
	ULONG w;
	selectTextWindow();

	error("TODO");

	return w;
}


uint TextWindow::fontHeight() {
	return _font._charHeight;
}

void TextWindow::vsprint(const char *format, va_list argptr) {
	error("vsprint");
}

void TextWindow::print(const char *format, ...) {
	error("print");
}

void TextWindow::sprint(const char *format, ...) {
	error("sprint");
}

void TextWindow::crout() {
	sprint("%s", "\n");
}

} // End of namespace Aesop

