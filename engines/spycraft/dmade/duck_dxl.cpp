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

#include "spycraft/dmade/duck_dxl.h"

namespace Spycraft {

#if 0
typedef struct vScreen *DXL_VSCREEN_HANDLE;
typedef struct tXImage *DXL_XIMAGE_HANDLE;
typedef struct tXAudioSource *DXL_XAUDIOSRC_HANDLE;
typedef struct tAudioBuff *DXL_AUDIODST_HANDLE;
#endif

int DXL_InitVideo(int maxScreens, int maxImages) {
	return 0;
}
void DXL_ExitVideo(void) {}

DXL_VSCREEN_HANDLE DXL_CreateVScreen(byte *addr, BITDEPTH depth, int p, int h) {
	return (DXL_VSCREEN_HANDLE)0;
}
int DXL_AlterVScreen(DXL_VSCREEN_HANDLE dst, byte *addr, BITDEPTH depth, int p, int h) {
	return 0;
}
int DXL_AlterVScreenClip(DXL_VSCREEN_HANDLE dst, int x, int y, int w, int h) {
	return 0;
}
int DXL_AlterVScreenView(DXL_VSCREEN_HANDLE dst, int x, int y, int w, int h) {
	return 0;
}
void DXL_DestroyVScreen(DXL_VSCREEN_HANDLE dst) {}

byte *DXL_GetVScreenAddr(DXL_VSCREEN_HANDLE src) {
	return nullptr;
}

int DXL_SetBlitQuality(DXL_VSCREEN_HANDLE dst, enum BLITQUALITY bq) {
	return 0;
}
enum BLITQUALITY DXL_GetBlitQuality(DXL_VSCREEN_HANDLE dst) {
	return (BLITQUALITY)0;
}
int DXL_SetBitDepth(DXL_VSCREEN_HANDLE dst, enum BITDEPTH bitdepth) {
	return 0;
}

DXL_XIMAGE_HANDLE DXL_CreateXImage(byte *data) {
	return (DXL_XIMAGE_HANDLE)0;
}
DXL_XIMAGE_HANDLE DXL_CreateGenericXImage(byte *data, int height, int width, enum IMAGETYPE iType) {
	return (DXL_XIMAGE_HANDLE)0;
}
int DXL_CreateDirtyBuffer(DXL_XIMAGE_HANDLE src) {
	return 0;
}
void DXL_DestroyXImage(DXL_XIMAGE_HANDLE src) {}
int DXL_IsXImageKeyFrame(DXL_XIMAGE_HANDLE src) {
	return 0;
}

int DXL_MoveXImage(DXL_XIMAGE_HANDLE src, enum OFFSETXY mode, int x, int y) {
	return 0;
}
int DXL_AlterXImageData(DXL_XIMAGE_HANDLE src, byte *data) {
	return 0;
}
int DXL_AlterXImageSpriteModes(DXL_XIMAGE_HANDLE src, int sprMode, int sprColor) {
	return 0;
}

int DXL_GetXImageXYWH(DXL_XIMAGE_HANDLE src, int *x, int *y, int *w, int *h) {
	return 0;
}

IMAGETYPE  DXL_GetXImageType(DXL_XIMAGE_HANDLE src) {
	return (IMAGETYPE)0;
}

int DXL_GetXImageColorDepth(DXL_XIMAGE_HANDLE src) {
	return 0;
}
int DXL_GetHInterp(DXL_XIMAGE_HANDLE src) {
	return 0;
}

DXL_VSCREEN_HANDLE DXL_DeriveVScreenFromXImage(DXL_XIMAGE_HANDLE src) {
	return 0;
}

int DXL_dxImageToVScreen(DXL_XIMAGE_HANDLE src, DXL_VSCREEN_HANDLE dst) {
	return 0;
}
int DXL_dxImage(DXL_XIMAGE_HANDLE src) {
	return 0;
}
void DXL_DisplayVScreen(DXL_XIMAGE_HANDLE XIsrc, DXL_VSCREEN_HANDLE src, DXL_VSCREEN_HANDLE dst, int FroceRepaint) {} /* 7/24 added prototype*/

int DXL_dxImageToVScreen1(DXL_XIMAGE_HANDLE src, DXL_VSCREEN_HANDLE dst) {
	return 0;
}
int DXL_dxImageToVScreen2(DXL_XIMAGE_HANDLE src, DXL_VSCREEN_HANDLE dst) {
	return 0;
}

int DXL_AlterVScreenBackground(DXL_VSCREEN_HANDLE dst, byte *addr,
	enum BITDEPTH bd, int x, int y, int p, int h) {
	return 0;
}
int DXL_GetXImageDirtyXYWH(DXL_XIMAGE_HANDLE src, int *x, int *y, int *w, int *h) {
	return 0;
}

int DXL_InitAudio(int srcs, int dsts) {
	return 0;
}

void DXL_ExitAudio(void) {}

DXL_XAUDIOSRC_HANDLE DXL_CreateXAudioSrc(DKWAVEFORM *wv, const byte *addr, int length) {
	return 0;
}

void DXL_AlterXAudioData(DXL_XAUDIOSRC_HANDLE xSource, const byte *addr, int length) {}
void DXL_DestroyXAudioSrc(DXL_XAUDIOSRC_HANDLE xSource) {}

DXL_AUDIODST_HANDLE DXL_CreateAudioDst(
		void *addrL, void *addrR,
		int length, int bitDepth,
		int numChannels,
		int sampleRate) {
	return (DXL_AUDIODST_HANDLE)0;
}

void DXL_AlterAudioDst(
	DXL_AUDIODST_HANDLE dst,
	void *addrL, void *addrR,
	int length, int bitDepth,
	int numChannels,
	int sampleRate) {}

void DXL_DestroyAudioDst(DXL_AUDIODST_HANDLE dst) {}

int DXL_dxAudio(DXL_XAUDIOSRC_HANDLE src, DXL_AUDIODST_HANDLE dst, int length) {
	return 0;
}

} // namespace Spycraft
