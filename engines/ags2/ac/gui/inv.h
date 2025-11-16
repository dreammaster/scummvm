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

#ifndef AGS2_AC_GUI_INV_H
#define AGS2_AC_GUI_INV_H

#include "ags2/ac/gui/gui_object.h"

namespace AGS2 {

struct GUIInv : public GUIObject {
	int isover;
	int charId;   // whose inventory (-1 = current player)
	int itemWidth, itemHeight;
	int topIndex;

	int itemsPerLine, numLines;  // not persisted

	virtual void WriteToFile(Common::WriteStream *ooo) override {
		GUIObject::WriteToFile(ooo);
		putw(charId, ooo);
		putw(itemWidth, ooo);
		putw(itemHeight, ooo);
		putw(topIndex, ooo);
	}

	virtual void ReadFromFile(Common::SeekableReadStream *ooo, int version) override {
		GUIObject::ReadFromFile(ooo, version);
		if (version >= 109) {
			charId = getw(ooo);
			itemWidth = getw(ooo);
			itemHeight = getw(ooo);
			topIndex = getw(ooo);
		} else {
			charId = -1;
			itemWidth = 40;
			itemHeight = 22;
			topIndex = 0;
		}
		// ensure that some items are visible
		if (itemWidth > wid)
			itemWidth = wid;
		if (itemHeight > hit)
			itemHeight = hit;
		CalculateNumCells();
	}

	void CalculateNumCells();

	virtual void Resized() override {
		CalculateNumCells();
	}

	int CharToDisplay();

	void Draw() override;

	void MouseMove(int nx, int ny) override {
	}

	void MouseOver() override {
		isover = 1;
	}

	void MouseLeave() override {
		isover = 0;
	}

	void MouseUp() override {
		if (isover)
			activated = 1;
	}

	void KeyPress(int kp) override {
	}

	GUIInv() {
		isover = 0;
		numSupportedEvents = 0;
		charId = -1;
		itemWidth = 40;
		itemHeight = 22;
		topIndex = 0;
		CalculateNumCells();
	}
};

} // namespace AGS2

#endif
