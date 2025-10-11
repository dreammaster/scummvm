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

#ifndef SPYCRAFT_DMADE_ADVEFF_H
#define SPYCRAFT_DMADE_ADVEFF_H

#include "spycraft/dmade/advrect.h"
#include "spycraft/dmade/advsprite.h"

namespace Spycraft {

enum {
	ADV_ZOOMBOX_STEP,
	ADV_ZOOMBOX_DELAY,
	ADV_ZOOMBOX_START_WIDTH,
	ADV_ZOOMBOX_START_HEIGHT,
	ADV_ZOOMBOX_END_WIDTH,
	ADV_ZOOMBOX_END_HEIGHT
};

enum {
	ADV_ZEFF_STEP,
	ADV_ZEFF_DELAY,
	ADV_ZEFF_END_WIDTH,
	ADV_ZEFF_END_HEIGHT
};

#define __SFX_LPFILTER_RED			0x0001
#define __SFX_LPFILTER_GREEN		0x0002
#define __SFX_LPFILTER_BLUE		0x0004

extern void sfxZoomBoxInfo(int type, int value);
extern void sfxZoomBoxUp(int end_x, int end_y);
extern void sfxZoomBoxDown(int start_x, int start_y);

extern void sfxZoomSpriteEffect(Sprite *sprite);
extern void sfxZoomEffectInfo(int type, int value);
extern void sfxZoomEffectRect(SRect *rect);
extern void sfxZoomEffect(int theX, int theY);

void sfxLPFilter(Viewport *port, int y, int *table, int size, int flag);

} // namespace Spycraft

#endif
