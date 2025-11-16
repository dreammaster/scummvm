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

#ifndef AGS2_AC_GUI_TEXTBOX_H
#define AGS2_AC_GUI_TEXTBOX_H

#include "ags2/ac/gui/gui_object.h"

namespace AGS2 {

#define GTF_NOBORDER  1

struct GUITextBox : public GUIObject {
	char text[200];
	int font, textcol, exflags;

	virtual void WriteToFile(Common::WriteStream *ooo) override;
	virtual void ReadFromFile(Common::SeekableReadStream *, int) override;
	void Draw() override;
	void KeyPress(int) override;

	void MouseMove(int, int) override {
	}

	void MouseOver() override {
	}

	void MouseLeave() override {
	}

	void MouseUp() override {
	}

	void reset() {
		GUIObject::init();
		font = 0;
		textcol = 0;
		text[0] = 0;
		numSupportedEvents = 1;
		supportedEvents[0] = "Activate";
		supportedEventArgs[0] = "GUIControl *control";
	}

	GUITextBox() {
		reset();
	}
};

} // namespace AGS2

#endif
