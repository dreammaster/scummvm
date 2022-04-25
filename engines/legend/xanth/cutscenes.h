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

#ifndef LEGEND_XANTH_CUTSCENES_H
#define LEGEND_XANTH_CUTSCENES_H

#include "common/scummsys.h"
#include "legend/xanth/globals.h"

namespace Legend {
namespace Xanth {

struct CutsceneText {
	uint16 _field0;
	uint32 _message;
	uint16 _field6;
	uint16 _field8;
	int16 _x1;
	int16 _x2;
	int16 _yp;
};

class Cutscene {
private:
	Mode1 _saved;
public:
	Cutscene();
	virtual ~Cutscene();
};

class DemonCutscene : public Cutscene {
protected:
	void showDemonConv(const CutsceneText *data);
};

class TitleCutscene : public DemonCutscene {
public:
	void execute();
};

extern void show_cutscene(int num);

} // namespasce Xanth
} // namespace Legend

#endif
