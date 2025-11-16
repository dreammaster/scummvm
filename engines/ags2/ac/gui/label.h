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

#ifndef AGS2_AC_GUI_LABEL_H
#define AGS2_AC_GUI_LABEL_H

#include "ags2/ac/gui/gui_object.h"

namespace AGS2 {

#define MAX_GUILABEL_TEXT_LEN 2048

struct GUILabel : public GUIObject {
private:
	char emptyStr[1] = { '\0' };
	char *text;
	int textBufferLen;
public:
	int font, textcol, align;

	virtual void WriteToFile(Common::WriteStream *ooo);
	virtual void ReadFromFile(Common::SeekableReadStream *, int);
	void Draw();
	void printtext_align(int yy, char *teptr);
	void SetText(const char *newText);
	const char *GetText();

	void MouseMove(int, int) override {
	}

	void MouseOver() override {
	}

	void MouseLeave() override {
	}

	void MouseUp() override {
	}

	void KeyPress(int kp) override {
	}

	void reset() {
		GUIObject::init();
		align = GALIGN_LEFT;
		font = 0;
		textcol = 0;
		numSupportedEvents = 0;
		text = emptyStr;
		textBufferLen = 0;
	}

	GUILabel() {
		reset();
	}
};

} // namespace AGS2

#endif
