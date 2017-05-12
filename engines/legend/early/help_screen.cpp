/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
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

#include "legend/early/help_screen.h"
#include "legend/early/screen.h"
#include "legend/legend.h"
#include "legend/core/resources.h"

namespace Legend {
namespace Early {

BEGIN_MESSAGE_MAP(HelpScreen, VisualItem)
	ON_MESSAGE(MouseButtonDownMsg)
	ON_MESSAGE(KeyCharMsg)
END_MESSAGE_MAP()

HelpScreen::HelpScreen() : VisualItem() {
	_name = "Help";
	setBounds(Screen::get().getBounds());
}

void HelpScreen::draw() {
	if (!_isDirty)
		return;
	VisualItem::draw();

	// Fill out the background
	Graphics::ManagedSurface surface = getSurface();
	surface.fillRect(surface.getBounds(), 0);

	loadFont(1);
	setFontColor(Screen::get()._defaultTextColor);

	// Write out the help
	Common::SeekableReadStream *s = g_vm->_res->getResource("EARLY/HELP");

	for (int idx = 0; s->pos() < s->size(); ++idx) {
		Common::String msg1 = readStringFromStream(s);
		Common::String msg2 = readStringFromStream(s);

		int xp = idx < 22 ? 24 : 312;
		int yp = (idx >= 22 ? idx - 22 : idx) * Gfx::Font::_lineHeight
			+ 64 + _fontCenter.y;

		int xs;
		if (idx < 11)
			xs = 56;
		else if (idx < 18)
			xs = 112;
		else
			xs = 120;

		if (!msg2.empty()) {
			// It's a key/purpose pair
			setTextPos(Common::Point(xp + 16, yp));
			writeString(msg1);
			setTextPos(Common::Point(xp + xs, yp));
			writeString(msg2);
		} else if (!msg1.empty()) {
			// It's a section title
			setTextPos(Common::Point(xp, yp));
			writeString(msg1);
		}
	}

	delete s;
}

bool HelpScreen::MouseButtonDownMsg(CMouseButtonDownMsg &msg) {
	changeView("Scene");
	return true;
}

bool HelpScreen::KeyCharMsg(CKeyCharMsg &msg) {
	changeView("Scene");
	return true;
}

} // End of namespace Early
} // End of namespace Legend
