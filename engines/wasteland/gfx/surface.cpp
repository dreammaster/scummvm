/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common/system.h"
#include "graphics/palette.h"
#include "graphics/paletteman.h"
#include "wasteland/gfx/surface.h"
#include "wasteland/engine.h"
#include "wasteland/gfx/wasteland_font.h"

namespace Wasteland {
namespace Gfx {

#define VGA_COLOR_TRANS(x) ((x)*255 / 63)

static const byte MASTER_PALETTE[16][3] = {
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x27},
	{0x03, 0x25, 0x03},
	{0x00, 0x1B, 0x1B},
	{0x20, 0x00, 0x00},
	{0x1B, 0x00, 0x1B},
	{0x20, 0x1B, 0x05},
	{0x26, 0x26, 0x26},
	{0x10, 0x10, 0x10},
	{0x00, 0x00, 0x38},
	{0x00, 0x3F, 0x00},
	{0x00, 0x3F, 0x3F},
	{0x3A, 0x20, 0x10},
	{0x3F, 0x00, 0x3F},
	{0x3F, 0x3F, 0x00},
	{0x3F, 0x3F, 0x3F}
};

void Surface::setupPalette() {
	byte p[48];
	for (int i = 0; i < 48; ++i)
		p[i] = VGA_COLOR_TRANS(*(& MASTER_PALETTE[0][0] + i));
	g_system->getPaletteManager()->setPalette(p, 0, 16);

	uint32 white = 0xffffffff;
	g_system->getPaletteManager()->setPalette((const byte *)&white, 255, 1);
	return;
	/*
	// The game sets blocks of 16 palette entries to the same color
	byte pal[3];

	for (int blockCtr = 0; blockCtr < 16; ++blockCtr) {
		const byte *src = &MASTER_PALETTE[0][blockCtr];
		for (int i = 0; i < 3; ++i, ++src)
			pal[i] = VGA_COLOR_TRANS(*src);

		for (int i = 0; i < 16; ++i)
			g_system->getPaletteManager()->setPalette(pal, blockCtr * 16 + i, 1);
	}
	*/
}

void Surface::writeString(const Common::String &str) {
	g_engine->_font->drawString(this, str, _textX * FONT_W, _textY * FONT_H,
		this->w - (_textX * FONT_W), _inverseColor ? -1 : 1);
	_textX += str.size();
}

void Surface::writeString(const Common::String &str, int x, int y) {
	_textX = x;
	_textY = y;
	writeString(str);
}

void Surface::writeCenteredString(const Common::String &str, int y) {
	int x = ((this->w / FONT_W) - str.size()) / 2;
	writeString(str, x, y);
}

void Surface::writeChar(unsigned char c) {
	g_engine->_font->drawChar(this, c, _textX * FONT_W, _textY * FONT_H, _inverseColor ? -1 : 1);
	++_textX;
}

void Surface::setTextPos(int x, int y) {
	_textX = x;
	_textY = y;
}

} // namespace Gfx
} // namespace Wasteland
