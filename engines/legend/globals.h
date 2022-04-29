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

#ifndef LEGEND_GLOBALS_H
#define LEGEND_GLOBALS_H

#include "legend/graphics.h"
#include "legend/memory.h"

namespace Legend {

struct Globals {
	Globals();
	virtual ~Globals();

	Memory _memory;
	Palette _default_palette = { 0 };
	bool _cycling_on = false;
	int _palette_off = 0;
	Display _display;
	int _dirty_y1 = 0, _dirty_y2 = 0;
	int _gx_clipx1 = 0, _gx_clipx2 = 0;
	int _gx_clipy1 = 0, _gx_clipy2 = 0;
	int _clipx1 = 0, _clipx2 = 0;
	int _clipy1 = 0, _clipy2 = 0;
	byte *_screenPtr = nullptr;
	int _video_modeState = 0;
	int _video_pitch = 0;
	int _video_ctr = 0;
	int _grafx_mode = 0;
	bool _fontCharFlag = false;
	int _video_x2 = 0;
	int _video_saved_fg = 0, _video_saved_bg = 0;

};

extern Globals *g_globals;
#define _G(FIELD) (::Legend::g_globals->_##FIELD)

} // namespace Legend

#endif
