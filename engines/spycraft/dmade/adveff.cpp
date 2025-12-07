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

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/adveff.h"
#include "spycraft/dmade/advbits.h"
#include "spycraft/dmade/advback.h"
#include "spycraft/dmade/advrect.h"
#include "spycraft/dmade/advtime.h"
#include "spycraft/dmade/advprime.h"

namespace Spycraft {

#ifndef DIRECTX

typedef struct {
	int color;
	int step;
	int delay;
	int start_width;
	int start_height;
	int end_width;
	int end_height;
} ZoomBoxInfo;

#define ZBI_DEF_START_WIDTH	1
#define ZBI_DEF_START_HEIGHT	1
#define ZBI_DEF_END_WIDTH		500
#define ZBI_DEF_END_HEIGHT		350

extern int scene_width;
extern int scene_height;
static ZoomBoxInfo zbi = { 0x7fff, 10, 20, -1, -1, -1, -1 };

static void ResetZoomBoxInfo() {
	zbi.color = 0x7fff;
	zbi.step = 10;
	zbi.delay = 20;
	zbi.start_width = -1;
	zbi.start_height = -1;
	zbi.end_width = -1;
	zbi.end_height = -1;
}

void sfxZoomBoxUp(int start_x, int start_y) {
	unsigned int oldTime;
	int sx, sy, sw, sh;
	int xStep, yStep;
	int wStep, hStep;
	int oldSX = 0, oldSY = 0;
	int oldSW = 0, oldSH = 0;

	/* CHECK FOR DEFAULTS */
	if (zbi.start_width == -1)
		zbi.start_width = ZBI_DEF_START_WIDTH;

	if (zbi.start_height == -1)
		zbi.start_height = ZBI_DEF_START_HEIGHT;

	if (zbi.end_width == -1)
		zbi.end_width = ZBI_DEF_END_WIDTH;

	if (zbi.end_height == -1)
		zbi.end_height = ZBI_DEF_END_HEIGHT;

	/* SET UP */
	sx = start_x;
	sy = start_y;
	sw = zbi.start_width;
	sh = zbi.start_height;
	oldSX = -1;
	xStep = (abs(zbi.end_width - scene_width) / 2 - start_x) / zbi.step;
	yStep = (abs(zbi.end_height - scene_height) / 2 - start_y) / zbi.step;
	wStep = (zbi.end_width - zbi.start_width) / zbi.step;
	hStep = (zbi.end_height - zbi.start_height) / zbi.step;

	/* PROCESS IT */
	while (true) {
		if (sw >= zbi.end_width)
			break;

		if (oldSX != 1)
			sfxMDDrawRECT(oldSX, oldSY, oldSW, oldSH, zbi.color);

		sfxMDDrawRECT(sx, sy, sw, sh, zbi.color);

		oldSX = sx;
		oldSY = sy;
		oldSW = sw;
		oldSH = sh;

		sx += xStep;
		sy += yStep;
		sw += wStep;
		sh += hStep;

		oldTime = sfxGetTime();
		while ((sfxGetTime() - oldTime) < (unsigned int)zbi.delay);
	}

	/* CLEAN UP */
	sfxMDDrawRECT(oldSX, oldSY, oldSW, oldSH, zbi.color);
	ResetZoomBoxInfo();
}

void sfxZoomBoxDown(int end_x, int end_y) {
	unsigned int oldTime;
	int sx, sy, sw, sh;
	int xStep, yStep;
	int wStep, hStep;
	int oldSX = 0, oldSY = 0;
	int oldSW = 0, oldSH = 0;

	/* CHECK FOR DEFAULTS */
	if (zbi.start_width == -1)
		zbi.start_width = ZBI_DEF_END_WIDTH;

	if (zbi.start_height == -1)
		zbi.start_height = ZBI_DEF_END_HEIGHT;

	if (zbi.end_width == -1)
		zbi.end_width = ZBI_DEF_START_WIDTH;

	if (zbi.end_height == -1)
		zbi.end_height = ZBI_DEF_START_HEIGHT;

	/* SET UP */
	sx = abs(zbi.start_width - scene_width) / 2;
	sy = abs(zbi.start_height - scene_height) / 2;
	sw = zbi.start_width;
	sh = zbi.start_height;
	oldSX = -1;
	xStep = (end_x - sx) / zbi.step;
	yStep = (end_y - sy) / zbi.step;
	wStep = (zbi.start_width - zbi.end_width) / zbi.step;
	hStep = (zbi.start_height - zbi.end_height) / zbi.step;

	/* PROCESS IT */
	while (true) {
		if (sw <= zbi.end_width)
			break;

		if (oldSX != 1)
			sfxMDDrawRECT(oldSX, oldSY, oldSW, oldSH, zbi.color);

		sfxMDDrawRECT(sx, sy, sw, sh, zbi.color);

		oldSX = sx;
		oldSY = sy;
		oldSW = sw;
		oldSH = sh;

		sx += xStep;
		sy += yStep;
		sw -= wStep;
		sh -= hStep;

		oldTime = sfxGetTime();
		while ((sfxGetTime() - oldTime) < (unsigned int)zbi.delay);
	}

	/* CLEAN UP */
	sfxMDDrawRECT(oldSX, oldSY, oldSW, oldSH, zbi.color);
	ResetZoomBoxInfo();
}

void sfxZoomBoxInfo(int type, int value) {
	switch (type) {
	case ADV_ZOOMBOX_STEP:
		zbi.step = value;
		break;

	case ADV_ZOOMBOX_DELAY:
		zbi.delay = value;
		break;

	case ADV_ZOOMBOX_START_WIDTH:
		zbi.start_width = value;
		break;

	case ADV_ZOOMBOX_START_HEIGHT:
		zbi.start_width = value;
		break;

	case ADV_ZOOMBOX_END_WIDTH:
		zbi.start_width = value;
		break;

	case ADV_ZOOMBOX_END_HEIGHT:
		zbi.start_width = value;
		break;
	}
}

typedef struct {
	int step;
	int delay;
	int end_width;
	int end_height;
	Sprite *sprite;
	SRect *clipRect;
} ZoomEffectInfo;

static ZoomEffectInfo zei = { 10, 5, -1, -1, NULL, NULL };

static void ResetZEMInfo() {
	zei.step = 10;
	zei.delay = 5;
	zei.end_width = -1;
	zei.end_height = -1;
	zei.sprite = NULL;
	zei.clipRect = NULL;
}

void sfxZoomEffectRect(SRect *rect) {
	zei.clipRect = rect;
}

void sfxZoomSpriteEffect(Sprite *sprite) {
	zei.sprite = sprite;
}

void sfxZoomEffectInfo(int type, int value) {
	switch (type) {
	case ADV_ZEFF_STEP:
		zei.step = value;
		break;

	case ADV_ZEFF_DELAY:
		zei.delay = value;
		break;

	case ADV_ZEFF_END_WIDTH:
		zei.end_width = value;
		break;

	case ADV_ZEFF_END_HEIGHT:
		zei.end_height = value;
		break;
	}
}

#define ZEI_DEFAULT_END_WIDTH		360

void sfxZoomEffect(int theX, int theY) {
	SRect rect;
	int xOff, yOff;
	int xStep, yStep;
	int wStep, hStep;
	int sWidth, sHeight;
	unsigned int oldTime;
	Viewport *port;
	Background *back = backgrounds[curBack];

	/* CHECK WITH DEFAULT VALUES */
	if (zei.end_width == -1)
		zei.end_width = ZEI_DEFAULT_END_WIDTH;

	if (zei.end_height == -1)
		zei.end_height = zei.end_width * scene_height / scene_width;

	/* INITIAL VALUES */
	sWidth = scene_width - zei.step;
	sHeight = scene_height - zei.step;
	xOff = abs(sWidth - scene_width) / 2;
	yOff = abs(sHeight - scene_height) / 2;

	/* COMPUTE STEPS */
	wStep = (sWidth - zei.end_width) / zei.step;
	hStep = (sHeight - zei.end_height) / zei.step;
	xStep = (abs(zei.end_width / 2 - theX) - xOff) / zei.step;
	yStep = (abs(zei.end_height / 2 - theY) - yOff) / zei.step;

	/* SELECT PORT */
	if (zei.sprite)
		port = zei.sprite->port;
	else
		port = back->backPort;

	while ((sWidth > zei.end_width)) {
		SRect_Init(&rect, xOff, yOff, sWidth - 1 + xOff, sHeight + yOff - 1);
		DrawScaleBits(back->animPort, port, &rect, zei.clipRect);
		sfxMDBitBlt();
		xOff += xStep;
		yOff += yStep;
		sWidth -= wStep;
		sHeight -= hStep;
		oldTime = sfxGetTime();
		while ((sfxGetTime() - oldTime) < (unsigned int)zei.delay);
	}
}

#endif

inline void ClipRGB16(int *c) {
	if (*c > 0x1f)
		*c = 0x1f;

	if (*c < 0)
		*c = 0;
}

extern int pixFormat;

inline void LPFilter16(Viewport *port, int y, int *table, int size, int flag) {
	int i, r, g, b, index;
	unsigned short *stPtr = (unsigned short *)port->ptr;
	unsigned short *p = stPtr + y * port->width;

	if (pixFormat) {
		for (i = 0; i < port->width; i++) {
			r = (p[i] >> 11) & 0x1f;
			g = (p[i] >> 6) & 0x1f;
			b = p[i] & 0x1f;
			index = i % size;

			if (flag & __SFX_LPFILTER_RED) {
				r += (r * table[index] / 100);
				ClipRGB16(&r);
			}

			if (flag & __SFX_LPFILTER_GREEN) {
				g += (g * table[index] / 100);
				if (g > 0x3f)
					g = 0x3f;
				if (g < 0)
					g = 0;
			}

			if (flag & __SFX_LPFILTER_BLUE) {
				b += (b * table[index] / 100);
				ClipRGB16(&b);
			}

			p[i] = (r << 11) | (g << 6) | b;

		}
	} else {
		for (i = 0; i < port->width; i++) {
			r = (p[i] >> 10) & 0x1f;
			g = (p[i] >> 5) & 0x1f;
			b = p[i] & 0x1f;
			index = i % size;

			if (flag & __SFX_LPFILTER_RED) {
				r += (r * table[index] / 100);
				ClipRGB16(&r);
			}

			if (flag & __SFX_LPFILTER_GREEN) {
				g += (g * table[index] / 100);
				ClipRGB16(&g);
			}

			if (flag & __SFX_LPFILTER_BLUE) {
				b += (b * table[index] / 100);
				ClipRGB16(&b);
			}

			p[i] = (r << 10) | (g << 5) | b;

		}
	}
}

void sfxLPFilter(Viewport *port, int y, int *table, int size, int flag) {
	ASSERT(port->colors == 16, __ERR_CODING);
	LPFilter16(port, y, table, size, flag);
}

} // namespace Spycraft
