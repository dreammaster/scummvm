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

#ifndef AGS2_AC_ACGFX_H
#define AGS2_AC_ACGFX_H

#include "ags2/lib/allegro/surface.h"

namespace AGS2 {

extern unsigned long _myblender_color15(unsigned long x, unsigned long y, unsigned long n);
extern unsigned long _myblender_color16(unsigned long x, unsigned long y, unsigned long n);
extern unsigned long _myblender_color32(unsigned long x, unsigned long y, unsigned long n);
extern unsigned long _myblender_color15_light(unsigned long x, unsigned long y, unsigned long n);
extern unsigned long _myblender_color16_light(unsigned long x, unsigned long y, unsigned long n);
extern unsigned long _myblender_color32_light(unsigned long x, unsigned long y, unsigned long n);
extern void set_my_trans_blender(int r, int g, int b, int a);
extern void set_additive_alpha_blender();
extern void set_opaque_alpha_blender();

#define SCR_HFLIP  1
#define SCR_VFLIP  2
#define SCR_VHFLIP 3

struct GFXFilter {
public:

	virtual const char *Initialize(int width, int height, int colDepth);
	virtual void UnInitialize();
	virtual void GetRealResolution(int *wid, int *hit);
	virtual void SetMousePosition(int x, int y);
	// SetMouseArea shows the standard Windows cursor when the mouse moves outside
	// of it in windowed mode; SetMouseLimit does not
	virtual void SetMouseArea(int x1, int y1, int x2, int y2);
	virtual void SetMouseLimit(int x1, int y1, int x2, int y2);
	virtual const char *GetVersionBoxText();
	virtual const char *GetFilterID();
	virtual ~GFXFilter();
};

struct ScalingGFXFilter : public GFXFilter {
protected:
	int MULTIPLIER;
	void *mouseCallbackPtr;

	char filterName[100];
	char filterID[15];

	ScalingGFXFilter(int multiplier, bool justCheckingForSetup);

public:

	virtual const char *Initialize(int width, int height, int colDepth);
	virtual void UnInitialize();
	virtual void GetRealResolution(int *wid, int *hit);
	virtual void SetMouseArea(int x1, int y1, int x2, int y2);
	virtual void SetMouseLimit(int x1, int y1, int x2, int y2);
	virtual void SetMousePosition(int x, int y);
	virtual void AdjustPosition(int *x, int *y);
	virtual const char *GetVersionBoxText();
	virtual const char *GetFilterID();
	virtual ~ScalingGFXFilter();
};

struct AllegroGFXFilter : ScalingGFXFilter {
protected:
	BITMAP *realScreen;
	int lastBlitX;
	int lastBlitY;

public:

	AllegroGFXFilter(bool justCheckingForSetup);
	AllegroGFXFilter(int multiplier, bool justCheckingForSetup);

	virtual BITMAP *ScreenInitialized(BITMAP *screen, int fakeWidth, int fakeHeight);
	virtual BITMAP *ShutdownAndReturnRealScreen(BITMAP *currentScreen);
	virtual void RenderScreen(BITMAP *toRender, int x, int y);
	virtual void RenderScreenFlipped(BITMAP *toRender, int x, int y, int flipType);
	virtual void ClearRect(int x1, int y1, int x2, int y2, int color);
	virtual void GetCopyOfScreenIntoBitmap(BITMAP *copyBitmap);
	virtual void GetCopyOfScreenIntoBitmap(BITMAP *copyBitmap, bool copyWithYOffset);
};

struct D3DGFXFilter : ScalingGFXFilter {
protected:

public:

	D3DGFXFilter(bool justCheckingForSetup);
	D3DGFXFilter(int multiplier, bool justCheckingForSetup);

	virtual void SetSamplerStateForStandardSprite(void *direct3ddevice9);
	virtual bool NeedToColourEdgeLines();
};

GFXFilter **get_allegro_gfx_filter_list(bool checkingForSetup);
GFXFilter **get_d3d_gfx_filter_list(bool checkingForSetup);

} // namespace AGS2

#endif
