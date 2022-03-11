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

#ifndef LEGEND_GRAPHICS_H
#define LEGEND_GRAPHICS_H

#include "graphics/screen.h"
#include "legend/memory.h"

namespace Legend {

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

enum gxResult {
	GX_SUCCESS = 0,
	GX_20 = -20,
	GX_28 = -28
};

typedef byte Palette[PALETTE_SIZE];
typedef byte *PalettePtr;

struct Display {
	void *_ptr = nullptr;
	int _pitch = 0;
	int _field6 = 0;
	MemoryBlock *_pixels = nullptr;
	int _size = 0;
};

extern gxResult gxClearVirtual(Display *gx, int color);
extern void gxDirtyDisplay(int minY, int maxY);
extern gxResult gxClearDisplay(int color);
extern byte *gxVideoAddr(int x, int y);
extern gxResult gxDestroyVirtual(Display *gx);
extern void gxSetClipRegion(int x1, int y1, int x2, int y2);
extern void reset_clip_region();
extern void set_palette_range(const byte *pal, int start, int end);
extern void set_palette(const Palette &pal);
extern void black_palette();

} // namespace Legend

#endif
