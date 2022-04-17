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

#include "common/system.h"
#include "graphics/palette.h"
#include "legend/graphics.h"
#include "legend/globals.h"
#include "legend/legend.h"

namespace Legend {

gxResult gxClearVirtual(Display *gx, int color) {
	if (gx && gx->_pixels) {
		Common::fill((byte *)gx->_pixels->_ptr,
			(byte *)gx->_pixels->_ptr + gx->_size, color);
		return GX_SUCCESS;
	} else {
		return GX_20;
	}
}

gxResult gxClearDisplay(int color) {
	gxClearVirtual(&_G(display), color);
	gxDirtyDisplay(0, SCREEN_HEIGHT - 1);
	return GX_SUCCESS;
}

int find_rgb(byte r, byte g, byte b) {
	int palIndex = 0, minDiff = 0xffff;
	const byte *palP = _G(default_palette);

	for (int i = 0; i < PALETTE_COUNT; ++i, palP += 3) {
		int diff = ABS(palP[0] - r) + ABS(palP[1] - g) +
			ABS(palP[2] - b);

		if (diff < minDiff) {
			minDiff = diff;
			palIndex = i;
		}
	}

	return palIndex;
}

byte *gxVideoAddr(int x, int y) {
/*
	int offset = _G(display)._pitch * y + x;
	return (byte *)_G(display)._pixels->_ptr + offset;
	*/
	int videoOffset = y * _G(video_pitch) + x;
	byte *screenP = (byte *)g_engine->_screen->getPixels();

	if (_G(video_modeState)) {
		return _G(screenPtr) + videoOffset;
	} else if (_G(grafx_mode) == 5) {
		gxSetCtr(videoOffset >> 16);
		return screenP + (videoOffset & 0xffff);
	} else {
		return screenP + videoOffset;
	}
}

void gxDirtyDisplay(int minY, int maxY) {
	if (minY > maxY)
		SWAP(minY, maxY);

	if (minY < 0)
		_G(dirty_y1) = 0;
	else if (minY < _G(dirty_y1))
		_G(dirty_y1) = minY;

	if (maxY >= SCREEN_HEIGHT)
		_G(dirty_y2) = SCREEN_HEIGHT - 1;
	else if (maxY > _G(dirty_y2))
		_G(dirty_y2) = maxY;
}

gxResult gxDestroyVirtual(Display *gx) {
	if (gx) {
		if (gx->_pixels) {
			// TODO: kill_handle call for a memory block?
			//kill_handle(gx->_pixels);
			gx->_pixels = nullptr;
			return GX_SUCCESS;
		} else if (gx->_pitch == 0 && gx->_field6 == 0) {
			return GX_SUCCESS;
		}
	}

	return GX_28;
}

void gxSetClipRegion(int x1, int y1, int x2, int y2) {
	_G(gx_clipx1) = (x1 < 0 || x1 >= SCREEN_WIDTH) ? 0 : x1;
	_G(gx_clipy1) = (y1 < 0 || y1 >= SCREEN_HEIGHT) ? 0 : y1;
	_G(gx_clipx2) = (x2 < 0 || x2 >= SCREEN_WIDTH) ? SCREEN_WIDTH - 1 : x2;
	_G(gx_clipy2) = (y2 < 0 || y2 >= SCREEN_HEIGHT) ? SCREEN_HEIGHT - 1 : y2;

	_G(clipx1) = _G(gx_clipx1) >> 16;
	_G(clipy1) = _G(gx_clipy1) >> 16;
	_G(clipx2) = _G(gx_clipx2) >> 16;
	_G(clipy2) = _G(gx_clipy2) >> 16;
}

void reset_clip_region() {
	gxSetClipRegion(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
}

void set_palette_range(const byte *pal, int start, int count) {
	g_system->getPaletteManager()->setPalette(pal, start, count);
}

void set_palette(const Palette &pal) {
	set_palette_range(pal, 0, PALETTE_COUNT - 1);
}

void black_palette() {
	_G(cycling_on) = false;
	Common::fill(&_G(default_palette)[0],
		&_G(default_palette)[PALETTE_SIZE], 0);
	set_palette(_G(default_palette));
	_G(palette_off) = 1;
}

void draw_xor(int x, int y, int w, int h, int mask) {
	//byte *videoAddr = gxVideoPtr();
}

int gxSetModeState(int state) {
	int oldState = state;
	if (state != _G(video_modeState) && _G(screenPtr))
		_G(video_modeState) = state ? 1 : 0;

	return oldState;
}

int gxSetCtr(int val) {
	if (val < 0) {
		++_G(video_ctr);
		return _G(video_ctr);
	} else if (val != _G(video_ctr)) {
		_G(video_ctr) = val;
	}

	return _G(video_ctr);
}

int gxSaveColors(int fg, int bg) {
	_G(video_saved_fg) = fg;
	_G(video_saved_bg) = bg;
	return 0;
}

} // namespace Legend
