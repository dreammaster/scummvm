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
	~Globals();

	Palette _default_palette = { 0 };
	bool _cycling_on = false;
	int _palette_off = 0;
	Display _display;
	int _dirty_y1 = 0, _dirty_y2 = 0;

	// memory
	size_t _master_table_size = 0x300;
	size_t _memory_avail = 0;
	byte *_master_ptr = nullptr;
	size_t _real_avail = 0xfffff;
	byte *_real_ptr = nullptr;
	MemoryBlock *_master_table = nullptr;
	size_t _master_table_end = 0;
	int _max_handles = 0x200;
	int _next_handle_type = 0;
	void **_handle_table;
	bool _disable_compact_memory = false;
	PurgeMethod _purge_vector_tbl[20] = { nullptr };
	int _mem_type[20] = { 0 };
	int _min_memory = 0, _max_memory = 0;
	int _low_memory = 0;
};

extern Globals *g_globals;
#define _G(FIELD) (::Legend::g_globals->_##FIELD)

} // namespace Legend

#endif
