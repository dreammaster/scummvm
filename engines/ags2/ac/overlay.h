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

#ifndef AGS2_AC_OVERLAY_H
#define AGS2_AC_OVERLAY_H

#include "ags2/ac/dynamic.h"

namespace AGS2 {

struct ScreenOverlay {
	IDriverDependantBitmap *bmp;
	block pic;
	int type, x, y, timeout;
	int bgSpeechForChar;
	int associatedOverlayHandle;
	bool hasAlphaChannel;
	bool positionRelativeToScreen;
};

struct ScriptOverlay : AGSCCDynamicObject {
	int overlayId;
	int borderWidth;
	int borderHeight;
	int isBackgroundSpeech;

	virtual int Dispose(const char *address, bool force);
	virtual const char *GetType();
	virtual int Serialize(const char *address, char *buffer, int bufsize);
	virtual void Unserialize(int index, const char *serializedData, int dataSize);
	void Remove();
	ScriptOverlay();
};

extern void remove_screen_overlay_index(int cc);
extern void remove_screen_overlay(int type);
extern int find_overlay_of_type(int typ);
extern int add_screen_overlay(int x, int y, int type, block piccy, bool alphaChannel = false);
extern void get_overlay_position(int overlayidx, int *x, int *y);
extern void draw_screen_overlay();

extern void RemoveOverlay(int ovrid);
extern void Overlay_Remove(ScriptOverlay *sco);
extern int CreateGraphicOverlay(int xx, int yy, int slott, int trans);
extern int CreateTextOverlayCore(int xx, int yy, int wii, int fontid, int clr, const char *tex, int allowShrink);
extern int CreateTextOverlay(int xx, int yy, int wii, int fontid, int clr, char *texx, ...);
extern void SetTextOverlay(int ovrid, int xx, int yy, int wii, int fontid, int clr, char *texx, ...);
extern void Overlay_SetText(ScriptOverlay *scover, int wii, int fontid, int clr, char *texx, ...);
extern int Overlay_GetX(ScriptOverlay *scover);
extern void Overlay_SetX(ScriptOverlay *scover, int newx);
extern int Overlay_GetY(ScriptOverlay *scover);
extern void Overlay_SetY(ScriptOverlay *scover, int newy);
extern void MoveOverlay(int ovrid, int newx, int newy);
extern int IsOverlayValid(int ovrid);
extern int Overlay_GetValid(ScriptOverlay *scover);
extern ScriptOverlay *Overlay_CreateGraphical(int x, int y, int slot, int transparent);
extern ScriptOverlay *Overlay_CreateTextual(int x, int y, int width, int font, int colour, const char *text, ...);
extern int DisplaySpeechBackground(int charid, char *speel);

} // namespace AGS2

#endif
