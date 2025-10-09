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

#ifndef SPYCRAFT_DMADE_DUCK_DXL_H
#define SPYCRAFT_DMADE_DUCK_DXL_H

#include "common/scummsys.h"

namespace Spycraft {

enum BGMODE           /* sprite drawing modes
						 v1.0.2 supports NORM & NO_BACKGROUND */ {
	NORM = 0,         /* normal sprite mode, blend edges w/background */
	NO_BACKGROUND = 1,/* transparant sprite mode 1,
						 sets all background data transparant no blending */
	NORM_TRANS = 2,   /* transparant sprite mode 2,
						 blend edges (alphas) w/separate background buffer,
						 set sprite background to trans */
	RGB_OPAQUE = 3,   /* blend edges to sprColor, set background to sprColor*/
	RGB_TRANS = 4     /* blend edges w/sprColor, set background to trans */
};
enum BITDEPTH {
	DXRGBNULL = 0,
	DXRGB8 = 1,
	DXRGB16 = 2,
	DXRGB24CHAR = 3,
	DXRGB24BITMAP = 4
};

enum BLITQUALITY {
	DXBLIT_SAME = 0,		/* Blit from 16 bit to 16 bit, no stretch */
	DXBLIT_VGA,  			/* MODE_13 unstretched 8-bit output */
	DXBLIT_MODEX,   		/* mode_x interleaved memory */
	DXBLIT_STRETCH,			/* stretch, Double Pixels; leave skipped lines */
	DXBLIT_STRETCH_8BIT,	/* stretch, double pixels, leave skipped lines, convert to 8-bit*/
	DXBLIT_STRETCH_BRIGHT,  /* stretch, double pixels, double lines */
	DXBLIT_HINTERP_BRIGHT,  /* stretch, h interp, double lines */
	DXBLIT_HVINTERP,			/* stretch, h and v interp */
	DXBLIT_3X2,		  		/* 2 pixels to 3 upsample */
	DXBLIT_STRETCH_3X2		/* 2 pixels to 6 upsample */
};

#define DXRGB24 DXRGB24CHAR

enum OFFSETXY {
	DXL_ABSOLUTE = 0, DXL_RELATIVE
};   		/* 7/20 need DXL_Preface to ABSOLUTE, RELATIVE*/
enum IMAGETYPE {
	DXL_IMERR2 = -2, DXL_IMERR1 = -1, DXL_INTRAFRAME = 0, DXL_INTERFRAME, DXL_SPRITE
};

typedef struct vScreen *DXL_VSCREEN_HANDLE;
typedef struct tXImage *DXL_XIMAGE_HANDLE;
typedef struct tXAudioSource *DXL_XAUDIOSRC_HANDLE;
typedef struct tAudioBuff *DXL_AUDIODST_HANDLE;

extern int DXL_InitVideo(int maxScreens, int maxImages);
extern void DXL_ExitVideo(void);

extern DXL_VSCREEN_HANDLE DXL_CreateVScreen(byte *addr, BITDEPTH depth, int p, int h);  /* 7/24 added far*/
extern int DXL_AlterVScreen(DXL_VSCREEN_HANDLE dst, byte *addr, BITDEPTH depth, int p, int h);
extern int DXL_AlterVScreenClip(DXL_VSCREEN_HANDLE dst, int x, int y, int w, int h);
extern int DXL_AlterVScreenView(DXL_VSCREEN_HANDLE dst, int x, int y, int w, int h);
extern void DXL_DestroyVScreen(DXL_VSCREEN_HANDLE dst);

extern byte *DXL_GetVScreenAddr(DXL_VSCREEN_HANDLE src);

extern int DXL_SetBlitQuality(DXL_VSCREEN_HANDLE dst, enum BLITQUALITY bq);
enum BLITQUALITY DXL_GetBlitQuality(DXL_VSCREEN_HANDLE dst);
extern int DXL_SetBitDepth(DXL_VSCREEN_HANDLE dst, enum BITDEPTH bitdepth);

extern DXL_XIMAGE_HANDLE DXL_CreateXImage(byte *data); 		/* 7/24 removed const*/
extern DXL_XIMAGE_HANDLE DXL_CreateGenericXImage(byte *data, int height, int width, enum IMAGETYPE iType);
extern int DXL_CreateDirtyBuffer(DXL_XIMAGE_HANDLE src);		/* 7/19 added prototype*/
extern void DXL_DestroyXImage(DXL_XIMAGE_HANDLE src);
extern int DXL_IsXImageKeyFrame(DXL_XIMAGE_HANDLE src);

extern int DXL_MoveXImage(DXL_XIMAGE_HANDLE src, enum OFFSETXY mode, int x, int y);
extern int DXL_AlterXImageData(DXL_XIMAGE_HANDLE src, byte *data);	/* 7/24 removed const            */
extern int DXL_AlterXImageSpriteModes(DXL_XIMAGE_HANDLE src, int sprMode, int sprColor);

extern int DXL_GetXImageXYWH(DXL_XIMAGE_HANDLE src, int *x, int *y, int *w, int *h);
enum IMAGETYPE  DXL_GetXImageType(DXL_XIMAGE_HANDLE src);
extern int DXL_GetXImageColorDepth(DXL_XIMAGE_HANDLE src);
extern int DXL_GetHInterp(DXL_XIMAGE_HANDLE src);

extern DXL_VSCREEN_HANDLE DXL_DeriveVScreenFromXImage(DXL_XIMAGE_HANDLE src);	/* 7/24 ADDED PROTOTYPE*/

extern int DXL_dxImageToVScreen(DXL_XIMAGE_HANDLE src, DXL_VSCREEN_HANDLE dst);
extern int DXL_dxImage(DXL_XIMAGE_HANDLE src);
extern void DXL_DisplayVScreen(DXL_XIMAGE_HANDLE XIsrc, DXL_VSCREEN_HANDLE src, DXL_VSCREEN_HANDLE dst, int FroceRepaint); /* 7/24 added prototype*/

extern int DXL_dxImageToVScreen1(DXL_XIMAGE_HANDLE src, DXL_VSCREEN_HANDLE dst);
extern int DXL_dxImageToVScreen2(DXL_XIMAGE_HANDLE src, DXL_VSCREEN_HANDLE dst);

extern int DXL_AlterVScreenBackground(DXL_VSCREEN_HANDLE dst, byte *addr,
	enum BITDEPTH bd, int x, int y, int p, int h);
extern int DXL_GetXImageColorDepth(DXL_XIMAGE_HANDLE src);
extern int DXL_GetXImageDirtyXYWH(DXL_XIMAGE_HANDLE src, int *x, int *y, int *w, int *h);

#define DXL_blitXImageToVScreen(s,d) DXL_dxImageToVScreen2(s,d)

extern int DXL_InitAudio(int srcs, int dsts);
extern void DXL_ExitAudio(void);

extern DXL_XAUDIOSRC_HANDLE DXL_CreateXAudioSrc(DKWAVEFORM *wv, const byte *addr, int length);
extern void DXL_AlterXAudioData(DXL_XAUDIOSRC_HANDLE xSource, const byte *addr, int length);
extern void DXL_DestroyXAudioSrc(DXL_XAUDIOSRC_HANDLE xSource);

extern DXL_AUDIODST_HANDLE DXL_CreateAudioDst(
	void *addrL, void *addrR,
	int length, int bitDepth,
	int numChannels,
	int sampleRate);

extern void DXL_AlterAudioDst(
	DXL_AUDIODST_HANDLE dst,
	void *addrL, void *addrR,
	int length, int bitDepth,
	int numChannels,
	int sampleRate);

extern void DXL_DestroyAudioDst(DXL_AUDIODST_HANDLE dst);

extern int DXL_dxAudio(DXL_XAUDIOSRC_HANDLE src, DXL_AUDIODST_HANDLE dst, int length);

} // namespace Spycraft

#endif
