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

#ifndef SPYCRAFT_DLGBOX_H
#define SPYCRAFT_DLGBOX_H

#include "spycraft/game/interface_puzzle.h"

namespace Spycraft {

#define DIALOGBOX_FOCUS		11

class DialogText : public TextFeature {
public:
	~DialogText();
	void init(int theX, int theY, int theWidth, int theHeight, int color);
	void dispose();
};

class DialogButton : public View {
public:
	void init(int x, int y, int pic_id, int loop_id, int cel_id);
	void init(int x, int y);
	int handleEvent(MADEEventStamp *event);
	virtual void activate();
};

class DialogFeature : public Feature {
public:
	void init(int theX, int theY, int theWidth, int theHeight);
	void dispose();
};

class DialogView : public View {
public:
	void init(int x, int y);
	void dispose();
};

class DlgBox : public View {
public:
	int origin_x;
	int origin_y;

	DlgBox();
	void init(int x, int y, int pic_id, int loop_id, int cel_id);
	void init(int x, int y);
	void dispose();
};

extern List *dialogList;
extern DlgBox *dialogbox;

} // namespace Spycraft

#endif
