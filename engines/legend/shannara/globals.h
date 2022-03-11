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

#ifndef LEGEND_SHANNARA_GLOBALS_H
#define LEGEND_SHANNARA_GLOBALS_H

#include "legend/globals.h"
#include "legend/file.h"
#include "legend/graphics.h"

namespace Legend {
namespace Shannara {

struct QHeader;
struct QInfo;

struct ShannaraGlobals : public ::Legend::Globals {
	ShannaraGlobals();
	virtual ~ShannaraGlobals();

	PalettePtr _qpalette = nullptr;
	PalettePtr _new_palette = nullptr;
	byte *_qbuffer = nullptr;
	byte *_qbuffer_orig = nullptr;
	byte *_video_buffer = nullptr;
	size_t _qaudio_buffer_size = 0;
	int _q_y1 = 0, _q_y2 = 0;
	QHeader *_qheader;
	QInfo *_qinfo;
	Stream *_q_fp = nullptr;
	byte _next_qprefix[6] = { 0 };
	void *_qframe = nullptr;
	int _next_qframe = 0;

	void *_composite_addr_tbl = nullptr;
	void *_composite_buffer = nullptr;
	int _qloop2 = 0;
};

#define _GS(FIELD) (((::Legend::Shannara::ShannaraGlobals *)::Legend::g_globals)->_##FIELD)

} // namespasce Shannara
} // namespace Legend

#endif
