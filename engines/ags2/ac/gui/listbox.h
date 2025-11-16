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

#ifndef AGS2_AC_GUI_LISTBOX_H
#define AGS2_AC_GUI_LISTBOX_H

#include "ags2/ac/gui/gui_object.h"

namespace AGS2 {

#define MAX_LISTBOX_ITEMS 200

#define GLF_NOBORDER     1
#define GLF_NOARROWS     2
#define GLF_SGINDEXVALID 4

struct GUIListBox : public GUIObject {
	char *items[MAX_LISTBOX_ITEMS];
	short saveGameIndex[MAX_LISTBOX_ITEMS];
	int numItems, selected, topItem, mousexp, mouseyp;
	int rowheight, num_items_fit;
	int font, textcol, backcol, exflags;
	int selectedbgcol;
	int alignment, reserved1;
	virtual void WriteToFile(Common::WriteStream *ooo);
	virtual void ReadFromFile(Common::SeekableReadStream *, int);
	int  AddItem(const char *toadd);
	int  InsertItem(int index, const char *toadd);
	void SetItemText(int index, const char *newtext);
	void RemoveItem(int index);
	void Clear();
	void Draw() override;
	int  IsInRightMargin(int x);
	int  GetIndexFromCoordinates(int x, int y);
	void ChangeFont(int newFont);
	virtual int MouseDown() override;

	void MouseMove(int nx, int ny) override {
		mousexp = nx - x;
		mouseyp = ny - y;
	}

	void MouseOver() override {
	}

	void MouseLeave() override {
	}

	void MouseUp() override {
	}

	void KeyPress(int kp) override {
	}

	void Resized() override;

	void reset() {
		GUIObject::init();
		mousexp = 0;
		mouseyp = 0;
		activated = 0;
		numItems = 0;
		topItem = 0;
		selected = 0;
		font = 0;
		textcol = 0;
		selectedbgcol = 16;
		backcol = 7;
		exflags = 0;
		numSupportedEvents = 1;
		supportedEvents[0] = "SelectionChanged";
		supportedEventArgs[0] = "GUIControl *control";
	}

	GUIListBox() {
		reset();
	}
};

} // namespace AGS2

#endif
