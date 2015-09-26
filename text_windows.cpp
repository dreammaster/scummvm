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
#include "aesop/rtmsg.h"

namespace Aesop {

Font::Font() {
	_data = nullptr;
	_charHeight = 0;
}

Font::~Font() {
	clear();
}

int Font::charWidth(int c) {
	return READ_LE_UINT32(_chars[c]);
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

AesopEngine *TextWindow::_vm;

TextWindow::TextWindow() {
	_window = 0;
	_hTab = _vTab = 0;
	_delay = 0;
	_continueFunction = nullptr;
	_justify = J_LEFT;
	_refresh = 0;

	for (int idx = 0; idx < 256; ++idx)
		_lookaside[idx] = idx;
}

void TextWindow::init(AesopEngine *vm) {
	_vm = vm;
}

Pane &TextWindow::window() {
	return _vm->_screen->panes(_window);
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

void TextWindow::sPrint(const Common::String &format, const Parameters &params) {
	Resources &res = *_vm->_resources;
	Screen &screen = *_vm->_screen;
	unsigned char c;
	const byte *p;
	int paramIndex = 0;

	selectTextWindow();

	p = (const byte *)format.c_str();

	// Iterate across the format string
	while ((c = *p++) != '\0') {
		if (c != '%') {
			// Print a character
			screen.print(APP, "%c", Parameters(c));
			continue;
		}

		switch (c = (*p++)) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			screen.remapFontColor(15, text_colors[c - '0']);
			break;

		case 'd': {
			LONG v = params[paramIndex++];
			Common::String sVal = Common::String::format("%d", v);
			screen.print(APP, "%s", Parameters(sVal.c_str()));
			break;
		}

		case 'u': {
			uint v = params[paramIndex++];
			Common::String sVal = Common::String::format("%u", v);
			screen.print(APP, "%s", Parameters(sVal.c_str()));
			break;
		}

		case 'x':
		case 'X':  {
			uint v = params[paramIndex++];
			Common::String sVal = Common::String::format("%x", v);
			screen.print(APP, "%s", Parameters(sVal.c_str()));
			break;
		}

		case 's': {
			uint resourceId = params[paramIndex++];
			HRES str = res.get_resource_handle(resourceId, DA_DEFAULT);

			res.lock(str);
			const char *s = (const char *)res.addr(str);

			switch (READ_LE_UINT16(s)) {
			case ':S':
				screen.print(APP, "%s", Parameter(&s[2]));
				break;

			default:
				error(MSG_SRRV);
			}

			res.unlock(str);
			break;
		}

		case 'a': {
			const char *s = params[paramIndex++];
			screen.print(APP, s, Parameter());
			break;
		}

		case 'c': {
			int c = params[paramIndex++] & 0xff;
			screen.print(APP, "%c", Parameter(c));
			break;
		}

		default:
			break;
		}
	}

	screen.printBuffer(0);

	if (_refresh != -1)
		screen.refreshWindow(_window, _refresh);
}

void TextWindow::crout() {
	Parameters params;
	params.push_back(Parameter("\n"));
	sPrint("%s", params);
}

} // End of namespace Aesop

