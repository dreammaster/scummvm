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
#include "wasteland/wasteland1/views/dialogs/title.h"
#include "wasteland/core/file.h"
#include "wasteland/wasteland1/files/vertical_xor_stream.h"
#include "wasteland/keymapping.h"
#include "wasteland/wasteland1/core/text_decoder.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Dialogs {

#define TITLE_W 288
#define TITLE_H 128

static const char *const TEXT[] = {
	"\r         Electronic Arts and          \r"
		"        Interplay Productions         \r"
		"           proudly present            \r\r",
	"              WASTELAND!              \r\r"
		"         Copyright 1986-88 by         \r"
		"        Interplay Productions.        \r",
	"\r       Written by Alan Pavlish        \r"
		"    IBM version by Michael Quarles    \r\r",
	"\rPlace : EARTH\r",
	"Year  : 1998\r",
	"Status: DEFCON 1\r\r",
	"\r Computer defense initiative activated.\r",
	"\r Diplomatic solutions to the world's\r",
	" problems fail and war erupts as some\r",
	" madmen press ahead with their insane\r dreams.\r",
	"Current condition:\r",
	"High concentrations of radiation\r",
	"produce random storms and mutations.\r",
	"Somehow life continues in the\r",
	"Wasteland!\r"
};


Title::Title() : Dialog("Title"),
		_start(this, "Start", "START", 18, 24),
		_textView("TextLines", this,
			Window(1 * FONT_W, 18 * FONT_H, 39 * FONT_W, 24 * FONT_H)) {
}

bool Title::msgAction(const ActionMessage &msg) {
	if (msg._action == KEYBIND_SELECT)
		replaceView("Roster");
	return true;
}

bool Title::msgGame(const GameMessage &msg) {
	if (msg._name == "Start")
		replaceView("Roster");
	return true;
}

void Title::draw() {
	Surface s = getSurface();
	s.clear();
	s.blitFrom(_surface, Common::Point(8, 8));

	s.setFont(1);
	drawFrame(Common::Rect(0, 0, 37, 17));
	drawFrame(Common::Rect(0, 17, 39, 24));
	s.writeChar(94, 0, 17);
	s.writeChar(95, 37, 17);

	const unsigned char WASTELAND[17] = {
		0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x64, 0x6e, 0x70,
		0x74, 0x72, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x7a
	};
	for (int i = 0; i < 17; ++i) {
		s.writeChar(WASTELAND[i], 38, i);
		s.writeChar(WASTELAND[i] + 1);
	}
}

bool Title::msgFocus(const FocusMessage &msg) {
	Dialog::msgFocus(msg);

	// Open title pic file for access
	File f("title.pic");
	VerticalXorStream xorStream(&f, TITLE_W, DisposeAfterUse::NO);

	// Load into surface
	_surface.create(TITLE_W, TITLE_H);
	byte *pDest = (byte *)_surface.getPixels();
	xorStream.read(pDest, TITLE_W * TITLE_H);

	_textNum = 0;
	setText();
	return true;
}

bool Title::msgUnfocus(const UnfocusMessage &msg) {
	Dialog::msgUnfocus(msg);
	_surface.clear();
	return true;
}

void Title::timeout() {
	setText();
}

void Title::setText() {
	switch (_textNum++) {
	case 0:
		_textView.print(TEXT[0]);
		delaySeconds(3);
		break;

	default:
		break;
	}
}

} // namespace Dialogs
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
