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

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Dialogs {

#define TITLE_W 288
#define TITLE_H 128

Title::Title() : Dialog("Title"), _start(this, "Start", "START", 18, 24) {
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

	// TODO: Write intro text
	s.setFont(0);
	s.writeString("TESTING 012345", 3, 22);
}

bool Title::msgFocus(const FocusMessage &msg) {
	Dialog::msgFocus(msg);

	// Read title in
	File f("title.pic");
	VerticalXorStream xorStream(&f, TITLE_W, DisposeAfterUse::NO);

	// Load into surface
	_surface.create(TITLE_W, TITLE_H);

	byte *pDest = (byte *)_surface.getPixels();
	for (int i = 0; i < (TITLE_W / 2 * (TITLE_H - 1)); ++i) {
		byte b = xorStream.readByte();
		*pDest++ = b >> 4;
		*pDest++ = b & 0xf;
	}

	return true;
}

bool Title::msgUnfocus(const UnfocusMessage &msg) {
	Dialog::msgUnfocus(msg);
	_surface.clear();
	return true;
}

void Title::timeout() {
}

} // namespace Dialogs
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
