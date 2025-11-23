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

#include "ags2/ac/cc.h"
#include "ags2/vars.h"

namespace AGS2 {

int CCGUIObject::Serialize(const char *address, char *buffer, int bufsize) {
	GUIObject *guio = (GUIObject *)address;
	StartSerialize(buffer);
	SerializeInt(guio->guin);
	SerializeInt(guio->objn);
	return EndSerialize();
}

void CCGUIObject::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int guinum = UnserializeInt();
	int objnum = UnserializeInt();
	ccRegisterUnserializedObject(index, guis[guinum].objs[objnum], this);
}

/*------------------------------------------------------------------*/

int CCCharacter::Serialize(const char *address, char *buffer, int bufsize) {
	CharacterInfo *chaa = (CharacterInfo *)address;
	StartSerialize(buffer);
	SerializeInt(chaa->index_id);
	return EndSerialize();
}

void CCCharacter::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int num = UnserializeInt();
	ccRegisterUnserializedObject(index, &game.chars[num], this);
}

/*------------------------------------------------------------------*/

int CCHotspot::Serialize(const char *address, char *buffer, int bufsize) {
	ScriptHotspot *shh = (ScriptHotspot *)address;
	StartSerialize(buffer);
	SerializeInt(shh->id);
	return EndSerialize();
}

void CCHotspot::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int num = UnserializeInt();
	ccRegisterUnserializedObject(index, &scrHotspot[num], this);
}

/*------------------------------------------------------------------*/

int CCRegion::Serialize(const char *address, char *buffer, int bufsize) {
	ScriptRegion *shh = (ScriptRegion *)address;
	StartSerialize(buffer);
	SerializeInt(shh->id);
	return EndSerialize();
}

void CCRegion::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int num = UnserializeInt();
	ccRegisterUnserializedObject(index, &scrRegion[num], this);
}

/*------------------------------------------------------------------*/

int CCInventory::Serialize(const char *address, char *buffer, int bufsize) {
	ScriptInvItem *shh = (ScriptInvItem *)address;
	StartSerialize(buffer);
	SerializeInt(shh->id);
	return EndSerialize();
}

void CCInventory::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int num = UnserializeInt();
	ccRegisterUnserializedObject(index, &scrInv[num], this);
}

/*------------------------------------------------------------------*/

int CCDialog::Serialize(const char *address, char *buffer, int bufsize) {
	ScriptDialog *shh = (ScriptDialog *)address;
	StartSerialize(buffer);
	SerializeInt(shh->id);
	return EndSerialize();
}

void CCDialog::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int num = UnserializeInt();
	ccRegisterUnserializedObject(index, &scrDialog[num], this);
}

/*------------------------------------------------------------------*/

int CCGUI::Serialize(const char *address, char *buffer, int bufsize) {
	ScriptGUI *shh = (ScriptGUI *)address;
	StartSerialize(buffer);
	SerializeInt(shh->id);
	return EndSerialize();
}

void CCGUI::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int num = UnserializeInt();
	ccRegisterUnserializedObject(index, &scrGui[num], this);
}

/*------------------------------------------------------------------*/

int CCObject::Serialize(const char *address, char *buffer, int bufsize) {
	ScriptObject *shh = (ScriptObject *)address;
	StartSerialize(buffer);
	SerializeInt(shh->id);
	return EndSerialize();
}

void CCObject::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int num = UnserializeInt();
	ccRegisterUnserializedObject(index, &scrObj[num], this);
}

/*------------------------------------------------------------------*/

ScriptDialogOptionsRendering::ScriptDialogOptionsRendering() {
	Reset();
}

void ScriptDialogOptionsRendering::Reset() {
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	parserTextboxX = 0;
	parserTextboxY = 0;
	parserTextboxWidth = 0;
	dialogID = 0;
	surfaceToRenderTo = NULL;
	surfaceAccessed = false;
	activeOptionID = -1;
}

int ScriptDialogOptionsRendering::Serialize(const char *address, char *buffer, int bufsize) {
	return 0;
}

void ScriptDialogOptionsRendering::Unserialize(int index, const char *serializedData, int dataSize) {
	ccRegisterUnserializedObject(index, this, this);
}

/*------------------------------------------------------------------*/

int ScriptDateTime::Dispose(const char *address, bool force) {
	// always dispose a DateTime
	delete this;
	return 1;
}

const char *ScriptDateTime::GetType() {
	return "DateTime";
}

int ScriptDateTime::Serialize(const char *address, char *buffer, int bufsize) {
	StartSerialize(buffer);
	SerializeInt(year);
	SerializeInt(month);
	SerializeInt(day);
	SerializeInt(hour);
	SerializeInt(minute);
	SerializeInt(second);
	SerializeInt(rawUnixTime);
	return EndSerialize();
}

void ScriptDateTime::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	year = UnserializeInt();
	month = UnserializeInt();
	day = UnserializeInt();
	hour = UnserializeInt();
	minute = UnserializeInt();
	second = UnserializeInt();
	rawUnixTime = UnserializeInt();
	ccRegisterUnserializedObject(index, this, this);
}

ScriptDateTime::ScriptDateTime() {
	year = month = day = 0;
	hour = minute = second = 0;
	rawUnixTime = 0;
}

/*------------------------------------------------------------------*/

void DrawingSurface_Release(ScriptDrawingSurface *sds) {
	if (sds->roomBackgroundNumber >= 0)
	{
		if (sds->modified)
		{
			if (sds->roomBackgroundNumber == play.bg_frame)
			{
				invalidate_screen();
				mark_current_background_dirty();
			}
			play.raw_modified[sds->roomBackgroundNumber] = 1;
		}

		sds->roomBackgroundNumber = -1;
	}
	if (sds->dynamicSpriteNumber >= 0)
	{
		if (sds->modified)
		{
			int tt;
			// force a refresh of any cached object or character images
			if (croom != NULL)
			{
				for (tt = 0; tt < croom->numobj; tt++)
				{
					if (objs[tt].num == sds->dynamicSpriteNumber)
						objcache[tt].sppic = -31999;
				}
			}
			for (tt = 0; tt < game.numcharacters; tt++)
			{
				if (charcache[tt].sppic == sds->dynamicSpriteNumber)
					charcache[tt].sppic = -31999;
			}
			for (tt = 0; tt < game.numgui; tt++)
			{
				if ((guis[tt].bgpic == sds->dynamicSpriteNumber) &&
					(guis[tt].on == 1))
				{
					guis_need_update = 1;
					break;
				}
			}
		}

		sds->dynamicSpriteNumber = -1;
	}
	if (sds->dynamicSurfaceNumber >= 0)
	{
		destroy_bitmap(dynamicallyCreatedSurfaces[sds->dynamicSurfaceNumber]);
		dynamicallyCreatedSurfaces[sds->dynamicSurfaceNumber] = NULL;
		sds->dynamicSurfaceNumber = -1;
	}
	sds->modified = 0;
}

ScriptDrawingSurface *DrawingSurface_CreateCopy(ScriptDrawingSurface *sds) {
	BITMAP *sourceBitmap = sds->GetBitmapSurface();

	for (int i = 0; i < MAX_DYNAMIC_SURFACES; i++)
	{
		if (dynamicallyCreatedSurfaces[i] == NULL)
		{
			dynamicallyCreatedSurfaces[i] = create_bitmap_ex(bitmap_color_depth(sourceBitmap), sourceBitmap->w, sourceBitmap->h);
			blit(sourceBitmap, dynamicallyCreatedSurfaces[i], 0, 0, 0, 0, sourceBitmap->w, sourceBitmap->h);
			ScriptDrawingSurface *newSurface = new ScriptDrawingSurface();
			newSurface->dynamicSurfaceNumber = i;
			newSurface->hasAlphaChannel = sds->hasAlphaChannel;
			ccRegisterManagedObject(newSurface, newSurface);
			return newSurface;
		}
	}

	quit("!DrawingSurface.CreateCopy: too many copied surfaces created");
	return NULL;
}

void DrawingSurface_DrawSurface(ScriptDrawingSurface *target, ScriptDrawingSurface *source, int translev) {
	if ((translev < 0) || (translev > 99))
		quit("!DrawingSurface.DrawSurface: invalid parameter (transparency must be 0-99)");

	target->StartDrawing();
	BITMAP *surfaceToDraw = source->GetBitmapSurface();

	if (surfaceToDraw == abuf)
		quit("!DrawingSurface.DrawSurface: cannot draw surface onto itself");

	if (translev == 0) {
		// just draw it over the top, no transparency
		blit(surfaceToDraw, abuf, 0, 0, 0, 0, surfaceToDraw->w, surfaceToDraw->h);
		target->FinishedDrawing();
		return;
	}

	if (bitmap_color_depth(surfaceToDraw) <= 8)
		quit("!DrawingSurface.DrawSurface: 256-colour surfaces cannot be drawn transparently");

	// Draw it transparently
	trans_mode = ((100 - translev) * 25) / 10;
	put_sprite_256(0, 0, surfaceToDraw);
	target->FinishedDrawing();
}

void DrawingSurface_DrawImage(ScriptDrawingSurface *sds, int xx, int yy, int slot, int trans, int width, int height) {
	if ((slot < 0) || (slot >= MAX_SPRITES) || (spriteset[slot] == NULL))
		quit("!DrawingSurface.DrawImage: invalid sprite slot number specified");

	if ((trans < 0) || (trans > 100))
		quit("!DrawingSurface.DrawImage: invalid transparency setting");

	// 100% transparency, don't draw anything
	if (trans == 100)
		return;

	BITMAP *sourcePic = spriteset[slot];
	bool needToFreeBitmap = false;

	if (width != SCR_NO_VALUE)
	{
		// Resize specified

		if ((width < 1) || (height < 1))
			return;

		sds->MultiplyCoordinates(&width, &height);

		// resize the sprite to the requested size
		block newPic = create_bitmap_ex(bitmap_color_depth(sourcePic), width, height);

		stretch_blit(sourcePic, newPic,
			0, 0, spritewidth[slot], spriteheight[slot],
			0, 0, width, height);

		sourcePic = newPic;
		needToFreeBitmap = true;
		update_polled_stuff();
	}

	sds->StartDrawing();
	sds->MultiplyCoordinates(&xx, &yy);

	if (bitmap_color_depth(sourcePic) != bitmap_color_depth(abuf)) {
		debug_log("RawDrawImage: Sprite %d colour depth %d-bit not same as background depth %d-bit", slot, bitmap_color_depth(spriteset[slot]), bitmap_color_depth(abuf));
	}

	if (trans > 0)
	{
		trans_mode = ((100 - trans) * 255) / 100;
	}

	draw_sprite_support_alpha(xx, yy, sourcePic, slot);

	sds->FinishedDrawing();

	if (needToFreeBitmap)
		destroy_bitmap(sourcePic);
}

ScriptDrawingSurface::ScriptDrawingSurface() {
	roomBackgroundNumber = -1;
	dynamicSpriteNumber = -1;
	dynamicSurfaceNumber = -1;
	isLinkedBitmapOnly = false;
	linkedBitmapOnly = NULL;
	currentColour = play.raw_color;
	currentColourScript = 0;
	modified = 0;
	hasAlphaChannel = 0;
	highResCoordinates = 0;

	if ((game.options[OPT_NATIVECOORDINATES] != 0) &&
		(game.default_resolution > 2))
	{
		highResCoordinates = 1;
	}
}

BITMAP *ScriptDrawingSurface::GetBitmapSurface() {
	if (roomBackgroundNumber >= 0)
		return thisroom.ebscene[roomBackgroundNumber];
	else if (dynamicSpriteNumber >= 0)
		return spriteset[dynamicSpriteNumber];
	else if (dynamicSurfaceNumber >= 0)
		return dynamicallyCreatedSurfaces[dynamicSurfaceNumber];
	else if (linkedBitmapOnly != NULL)
		return linkedBitmapOnly;
	else
		quit("!DrawingSurface: attempted to use surface after Release was called");

	return NULL;
}

void ScriptDrawingSurface::MultiplyCoordinates(int *xcoord, int *ycoord) {
	if (this->highResCoordinates)
	{
		if (current_screen_resolution_multiplier == 1)
		{
			// using high-res co-ordinates but game running at low-res
			xcoord[0] /= 2;
			ycoord[0] /= 2;
		}
	} else
	{
		if (current_screen_resolution_multiplier > 1)
		{
			// using low-res co-ordinates but game running at high-res
			xcoord[0] *= 2;
			ycoord[0] *= 2;
		}
	}
}

void ScriptDrawingSurface::MultiplyThickness(int *valueToAdjust) {
	if (this->highResCoordinates)
	{
		if (current_screen_resolution_multiplier == 1)
		{
			valueToAdjust[0] /= 2;
			if (valueToAdjust[0] < 1)
				valueToAdjust[0] = 1;
		}
	} else
	{
		if (current_screen_resolution_multiplier > 1)
		{
			valueToAdjust[0] *= 2;
		}
	}
}

// convert actual co-ordinate back to what the script is expecting
void ScriptDrawingSurface::UnMultiplyThickness(int *valueToAdjust) {
	if (this->highResCoordinates)
	{
		if (current_screen_resolution_multiplier == 1)
		{
			valueToAdjust[0] *= 2;
		}
	} else
	{
		if (current_screen_resolution_multiplier > 1)
		{
			valueToAdjust[0] /= 2;
			if (valueToAdjust[0] < 1)
				valueToAdjust[0] = 1;
		}
	}
}

void ScriptDrawingSurface::StartDrawing() {
	abufBackup = abuf;
	abuf = this->GetBitmapSurface();
}

void ScriptDrawingSurface::FinishedDrawingReadOnly() {
	abuf = abufBackup;
}

void ScriptDrawingSurface::FinishedDrawing() {
	FinishedDrawingReadOnly();
	modified = 1;
}

int ScriptDrawingSurface::Dispose(const char *address, bool force) {
	// dispose the drawing surface
	DrawingSurface_Release(this);
	delete this;
	return 1;
}

const char *ScriptDrawingSurface::GetType() {
	return "DrawingSurface";
}

int ScriptDrawingSurface::Serialize(const char *address, char *buffer, int bufsize) {
	StartSerialize(buffer);
	SerializeInt(roomBackgroundNumber);
	SerializeInt(dynamicSpriteNumber);
	SerializeInt(dynamicSurfaceNumber);
	SerializeInt(currentColour);
	SerializeInt(currentColourScript);
	SerializeInt(highResCoordinates);
	SerializeInt(modified);
	SerializeInt(hasAlphaChannel);
	SerializeInt(isLinkedBitmapOnly ? 1 : 0);
	return EndSerialize();
}

void ScriptDrawingSurface::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	roomBackgroundNumber = UnserializeInt();
	dynamicSpriteNumber = UnserializeInt();
	dynamicSurfaceNumber = UnserializeInt();
	currentColour = UnserializeInt();
	currentColourScript = UnserializeInt();
	highResCoordinates = UnserializeInt();
	modified = UnserializeInt();
	hasAlphaChannel = UnserializeInt();
	isLinkedBitmapOnly = (UnserializeInt() != 0);
	ccRegisterUnserializedObject(index, this, this);
}

/*------------------------------------------------------------------*/

int ScriptViewFrame::Dispose(const char *address, bool force) {
	// always dispose a ViewFrame
	delete this;
	return 1;
}

const char *ScriptViewFrame::GetType() {
	return "ViewFrame";
}

int ScriptViewFrame::Serialize(const char *address, char *buffer, int bufsize) {
	StartSerialize(buffer);
	SerializeInt(view);
	SerializeInt(loop);
	SerializeInt(frame);
	return EndSerialize();
}

void ScriptViewFrame::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	view = UnserializeInt();
	loop = UnserializeInt();
	frame = UnserializeInt();
	ccRegisterUnserializedObject(index, this, this);
}

ScriptViewFrame::ScriptViewFrame(int p_view, int p_loop, int p_frame) {
	view = p_view;
	loop = p_loop;
	frame = p_frame;
}

ScriptViewFrame::ScriptViewFrame() {
	view = -1;
	loop = -1;
	frame = -1;
}

/*------------------------------------------------------------------*/

int ScriptDynamicSprite::Dispose(const char *address, bool force) {
	// always dispose
	if ((slot) && (!force))
		free_dynamic_sprite(slot);

	delete this;
	return 1;
}

const char *ScriptDynamicSprite::GetType() {
	return "DynamicSprite";
}

int ScriptDynamicSprite::Serialize(const char *address, char *buffer, int bufsize) {
	StartSerialize(buffer);
	SerializeInt(slot);
	return EndSerialize();
}

void ScriptDynamicSprite::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	slot = UnserializeInt();
	ccRegisterUnserializedObject(index, this, this);
}

ScriptDynamicSprite::ScriptDynamicSprite(int theSlot) {
	slot = theSlot;
	ccRegisterManagedObject(this, this);
}

ScriptDynamicSprite::ScriptDynamicSprite() {
	slot = 0;
}

/*------------------------------------------------------------------*/

const char *CCAudioChannel::GetType() {
	return "AudioChannel";
}

int CCAudioChannel::Serialize(const char *address, char *buffer, int bufsize) {
	ScriptAudioChannel *ach = (ScriptAudioChannel *)address;
	StartSerialize(buffer);
	SerializeInt(ach->id);
	return EndSerialize();
}

void CCAudioChannel::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int id = UnserializeInt();
	ccRegisterUnserializedObject(index, &scrAudioChannel[id], this);
}

/*------------------------------------------------------------------*/

const char *CCAudioClip::GetType() {
	return "AudioClip";
}

int CCAudioClip::Serialize(const char *address, char *buffer, int bufsize) {
	ScriptAudioClip *ach = (ScriptAudioClip *)address;
	StartSerialize(buffer);
	SerializeInt(ach->id);
	return EndSerialize();
}

void CCAudioClip::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int id = UnserializeInt();
	ccRegisterUnserializedObject(index, &game.audioClips[id], this);
}

/*------------------------------------------------------------------*/

const char *CreateNewScriptString(const char *fromText, bool reAllocate) {
	ScriptString *str;
	if (reAllocate) {
		str = new ScriptString(fromText);
	} else {
		str = new ScriptString();
		str->text = (char *)fromText;
	}

	ccRegisterManagedObject(str->text, str);

	/*long handle = ccRegisterManagedObject(str->text, str);
	char buffer[1000];
	Common::sprintf_s(buffer, "String %p (handle %d) allocated: '%s'", str->text, handle, str->text);
	write_log(buffer);*/

	return str->text;
}

void *ScriptString::CreateString(const char *fromText) {
	return (void *)CreateNewScriptString(fromText);
}

int ScriptString::Dispose(const char *address, bool force) {
	// always dispose
	if (text) {
		/*    char buffer[1000];
			Common::sprintf_s(buffer, "String %p deleted: '%s'", text, text);
			write_log(buffer);*/
		free(text);
	}
	delete this;
	return 1;
}

const char *ScriptString::GetType() {
	return "String";
}

int ScriptString::Serialize(const char *address, char *buffer, int bufsize) {
	if (text == NULL)
		text = _emptyString;
	StartSerialize(buffer);
	SerializeInt(strlen(text));
	Common::strcpy_s(&serbuffer[bytesSoFar], STD_BUFFER_SIZE, text);
	bytesSoFar += strlen(text) + 1;
	return EndSerialize();
}

void ScriptString::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	int textsize = UnserializeInt();
	text = (char *)malloc(textsize + 1);
	Common::strcpy_s(text, STD_BUFFER_SIZE, &serializedData[bytesSoFar]);
	ccRegisterUnserializedObject(index, text, this);
}

ScriptString::ScriptString() {
	text = NULL;
}

ScriptString::ScriptString(const char *fromText) {
	text = (char *)malloc(strlen(fromText) + 1);
	Common::strcpy_s(text, STD_BUFFER_SIZE, fromText);
}


} // namespace AGS2
