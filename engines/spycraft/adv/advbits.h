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

#ifndef SPYCRAFT_ADVBITS_H
#define SPYCRAFT_ADVBITS_H

#include "spycraft/adv/advsprite.h"

namespace Spycraft {

extern void DrawBits(Viewport *dest, Viewport *src, SRect *r);
extern void DrawScaleSprite(Viewport *port, Sprite *sprite, SRect *r);
extern void DrawScaleSpriteMask(Viewport *port, Sprite *sprite, SRect *spriteRect);
extern void DrawGlassMask(Viewport *port, Sprite *sprite, SRect *spriteRect);
extern void DrawSpriteFlip(Viewport *dest, Sprite *sprite, SRect *spriteRect,
	int nFlip, int nMask);
extern void CopyScaleSprite(Viewport *port, Sprite *sprite, SRect *r, int newLenX, int newLenY);
extern void CopyScaleSpriteMask(Viewport *port, Sprite *sprite, SRect *spriteRect, int newLenX, int LenY);
extern void CopySpriteFlip(Viewport *dest, Sprite *sprite, SRect *spriteRect,
	int nFlip, int nMask);

extern void MakeScaleTableF(uint *table, int oldLen, int newLen);
extern void MakeScaleTableI(uint *table, int oldLen, int newLen);

extern void DrawScaleBits(Viewport *port, Viewport *src, SRect *r, SRect *clipRect);
extern void Port2Buffer16(void *dest, Viewport *src, SRect *r);

} // namespace Spycraft

#endif
