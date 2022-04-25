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

#include "legend/xanth/cutscenes.h"
#include "legend/xanth/globals.h"
#include "legend/mouse.h"

namespace Legend {
namespace Xanth {

#define TITLE_CUTSCENE_TEXT_COUNT 10
static const CutsceneText TITLE_CUTSCENE_TEXT[10] = {
	{ 0x40C, 0xF04E0000, 0x1770, 0x64, 2, 0x13F, 6 },
	{ 0x40D, 0xF04E0001, 0x1388, 0x0A, 3, 0x0DC, 7 },
	{ 0x40E, 0xF04E0002, 0x1770, 0x64, 2, 0x13F, 6 },
	{ 0x40F, 0xF04E0003, 0x1770, 0x0A, 3, 0x0DC, 7 },
	{ 0x410, 0xF04E0004, 0x2328, 0x0A, 3, 0x0DC, 7 },
	{ 0x411, 0xF04E0005, 0x1388, 0x96, 2, 0x13F, 6 },
	{ 0x412, 0xF04E0006, 0x1F40, 0x0A, 3, 0x0DC, 7 },
	{ 0x413, 0xF04E0007, 0x1388, 0x96, 2, 0x13F, 6 },
	{ 0x414, 0xF04E0008, 0x2328, 0x0A, 3, 0x0DC, 7 },
	{ 0x415, 0xF04E0009, 0x1388, 0x64, 2, 0x13F, 6 }
};

Cutscene::Cutscene() {
	_saved = _GX(mode1);
	_GX(mode1) = MODE1_1;
	push_mouse();
}

Cutscene::~Cutscene() {
	pop_mouse();
	_GX(mode1) = _saved;
}

void DemonCutscene::showDemonConv(const CutsceneText *data) {

}

void TitleCutscene::execute() {
	showDemonConv(TITLE_CUTSCENE_TEXT);
}


void show_cutscene(int num) {
	switch (num) {
	case 0: {
		TitleCutscene c;
		c.execute();
		break;
	}
	default:
		break;
	}
}

} // namespace Xanth
} // namespace Legend
