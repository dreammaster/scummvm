/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 * You may also modify/distribute the code in this file under that license.
 */

#include "engines/ags/scripting/scripting.h"
#include "engines/ags/constants.h"
#include "engines/ags/drawingsurface.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"
#include "engines/ags/room.h"
#include "engines/ags/sprites.h"
#include "graphics/surface.h"

namespace AGS {

// ViewFrame: readonly import attribute bool Flipped
// Gets whether this frame is flipped.
RuntimeValue Script_ViewFrame_get_Flipped(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	return (self->_flags & VFLG_FLIPSPRITE) ? 1 : 0;
}

// ViewFrame: readonly import attribute int Frame
// Gets the frame number of this frame.
RuntimeValue Script_ViewFrame_get_Frame(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ViewFrame::get_Frame unimplemented");

	return RuntimeValue();
}

// ViewFrame: import attribute int Graphic
// Gets/sets the sprite that is displayed by this frame.
RuntimeValue Script_ViewFrame_get_Graphic(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	return self->_pic;
}

// ViewFrame: import attribute int Graphic
// Gets/sets the sprite that is displayed by this frame.
RuntimeValue Script_ViewFrame_set_Graphic(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("ViewFrame::set_Graphic unimplemented");

	return RuntimeValue();
}

// ViewFrame: import attribute AudioClip* LinkedAudio
// Gets/sets the audio that is played when this frame comes around.
RuntimeValue Script_ViewFrame_get_LinkedAudio(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ViewFrame::get_LinkedAudio unimplemented");

	return RuntimeValue();
}

// ViewFrame: import attribute AudioClip* LinkedAudio
// Gets/sets the audio that is played when this frame comes around.
RuntimeValue Script_ViewFrame_set_LinkedAudio(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	if (!params[0]._object->isOfType(sotAudioClip))
		error("ViewFrame::set_LinkedAudio got incorrect object type (expected a AudioClip) for parameter 1");
	AudioClip *value = (AudioClip *)params[0]._object;
	UNUSED(value);

	// FIXME
	error("ViewFrame::set_LinkedAudio unimplemented");

	return RuntimeValue();
}

// ViewFrame: readonly import attribute int Loop
// Gets the loop number of this frame.
RuntimeValue Script_ViewFrame_get_Loop(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ViewFrame::get_Loop unimplemented");

	return RuntimeValue();
}

// ViewFrame: import attribute int Sound
// Gets/sets the sound that is played when this frame comes around.
RuntimeValue Script_ViewFrame_get_Sound(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ViewFrame::get_Sound unimplemented");

	return RuntimeValue();
}

// ViewFrame: import attribute int Sound
// Gets/sets the sound that is played when this frame comes around.
RuntimeValue Script_ViewFrame_set_Sound(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("ViewFrame::set_Sound unimplemented");

	return RuntimeValue();
}

// ViewFrame: readonly import attribute int Speed
// Gets the delay of this frame.
RuntimeValue Script_ViewFrame_get_Speed(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ViewFrame::get_Speed unimplemented");

	return RuntimeValue();
}

// ViewFrame: readonly import attribute int View
// Gets the view number that this frame is part of.
RuntimeValue Script_ViewFrame_get_View(AGSEngine *vm, ViewFrame *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ViewFrame::get_View unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import void Clear(int colour=-SCR_NO_VALUE)
// Clears the surface to the specified colour, or transparent if you do not specify a colour.
RuntimeValue Script_DrawingSurface_Clear(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int colour = params[0]._signedValue;
	UNUSED(colour);

	// FIXME
	error("DrawingSurface::Clear unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import DrawingSurface* CreateCopy()
// Creates a copy of the surface.
RuntimeValue Script_DrawingSurface_CreateCopy(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	DrawingSurface *surface = new DrawingSurface(vm);
	surface->_type = dstDynamicSurface;

	Graphics::Surface *source = self->startDrawing();
	surface->_surface = new Graphics::Surface();
	surface->_surface->copyFrom(*source);
	self->finishedDrawing(true);

	RuntimeValue ret = surface;
	ret._object->DecRef();
	return ret;
}

// DrawingSurface: import void DrawCircle(int x, int y, int radius)
// Draws a circle onto the surface with its centre at (x,y).
RuntimeValue Script_DrawingSurface_DrawCircle(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int radius = params[2]._signedValue;
	UNUSED(radius);

	// FIXME
	error("DrawingSurface::DrawCircle unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import void DrawImage(int x, int y, int spriteSlot, int transparency=0, int width=SCR_NO_VALUE, int height=SCR_NO_VALUE)
// Draws a sprite onto the surface with its top-left corner at (x,y).
RuntimeValue Script_DrawingSurface_DrawImage(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;
	uint spriteSlot = params[2]._signedValue;
	uint transparency = params[3]._signedValue;
	uint width = params[4]._value;
	uint height = params[5]._value;

	if (spriteSlot >= vm->getSprites()->getSpriteCount())
		error("DrawingSurface::DrawImage: sprite %d is too high (only have %d sprites)",
			spriteSlot, vm->getSprites()->getSpriteCount());
	if (transparency > 100)
		error("DrawingSurface::DrawImage: transparency value must be between 0 and 100, but got %d", transparency);

	if (transparency == 100)
		return RuntimeValue();
	if (transparency > 0)
		transparency = ((100 - transparency) * 255) / 100;

	if (width != SCR_NO_VALUE) {
		// resize requested

		// FIXME
		UNUSED(height);
		error("DrawingSurface::DrawImage: resizing unimplemented");
	}

	// FIXME: adjust coordinates

	Graphics::Surface *dest = self->startDrawing();
	Sprite *source = vm->getSprites()->getSprite(spriteSlot);
	if (dest->format != source->_surface->format)
		error("DrawingSurface::DrawImage: Incompatible formats (source is %dBpp, dest is %dBpp)",
			source->_surface->format.bytesPerPixel, dest->format.bytesPerPixel);

	vm->_graphics->blit(source->_surface, dest, Common::Point(x, y), transparency);
	// FIXME: free sprite

	self->finishedDrawing();

	return RuntimeValue();
}

// DrawingSurface: import void DrawLine(int x1, int y1, int x2, int y2, int thickness=1)
// Draws a straight line between the two points on the surface.
RuntimeValue Script_DrawingSurface_DrawLine(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x1 = params[0]._signedValue;
	UNUSED(x1);
	int y1 = params[1]._signedValue;
	UNUSED(y1);
	int x2 = params[2]._signedValue;
	UNUSED(x2);
	int y2 = params[3]._signedValue;
	UNUSED(y2);
	int thickness = params[4]._signedValue;
	UNUSED(thickness);

	// FIXME
	error("DrawingSurface::DrawLine unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import void DrawMessageWrapped(int x, int y, int width, FontType, int messageNumber)
// Draws a message from the Room Message Editor, wrapping at the specified width.
RuntimeValue Script_DrawingSurface_DrawMessageWrapped(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int width = params[2]._signedValue;
	UNUSED(width);
	uint32 fonttype = params[3]._value;
	UNUSED(fonttype);
	int messageNumber = params[4]._signedValue;
	UNUSED(messageNumber);

	// FIXME
	error("DrawingSurface::DrawMessageWrapped unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import void DrawPixel(int x, int y)
// Changes the colour of a single pixel on the surface.
RuntimeValue Script_DrawingSurface_DrawPixel(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("DrawingSurface::DrawPixel unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import void DrawRectangle(int x1, int y1, int x2, int y2)
// Draws a filled rectangle to the surface.
RuntimeValue Script_DrawingSurface_DrawRectangle(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x1 = params[0]._signedValue;
	UNUSED(x1);
	int y1 = params[1]._signedValue;
	UNUSED(y1);
	int x2 = params[2]._signedValue;
	UNUSED(x2);
	int y2 = params[3]._signedValue;
	UNUSED(y2);

	// FIXME
	error("DrawingSurface::DrawRectangle unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import void DrawString(int x, int y, FontType, const string text, ...)
// Draws the specified text to the surface.
RuntimeValue Script_DrawingSurface_DrawString(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	uint32 fonttype = params[2]._value;
	UNUSED(fonttype);
	ScriptString *text = (ScriptString *)params[3]._object;
	UNUSED(text);

	// FIXME
	error("DrawingSurface::DrawString unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import void DrawStringWrapped(int x, int y, int width, FontType, Alignment, const string text)
// Draws the text to the surface, wrapping it at the specified width.
RuntimeValue Script_DrawingSurface_DrawStringWrapped(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int width = params[2]._signedValue;
	UNUSED(width);
	uint32 fonttype = params[3]._value;
	UNUSED(fonttype);
	uint32 alignment = params[4]._value;
	UNUSED(alignment);
	ScriptString *text = (ScriptString *)params[5]._object;
	UNUSED(text);

	// FIXME
	error("DrawingSurface::DrawStringWrapped unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import void DrawSurface(DrawingSurface *surfaceToDraw, int transparency=0)
// Draws the specified surface onto this surface.
RuntimeValue Script_DrawingSurface_DrawSurface(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	if (!params[0]._object->isOfType(sotDrawingSurface))
		error("DrawingSurface::DrawSurface got incorrect object type (expected a DrawingSurface) for parameter 1");
	DrawingSurface *surfaceToDraw = (DrawingSurface *)params[0]._object;
	uint transparency = params[1]._value;

	if (transparency > 100)
		error("DrawingSurface::DrawSurface: transparency value must be between 0 and 100, but got %d", transparency);

	// Original error()s out here instead.
	if (transparency == 100)
		return RuntimeValue();

	Graphics::Surface *dest = self->startDrawing();
	Graphics::Surface *src = surfaceToDraw->startDrawing();

	if (dest == src)
		error("DrawingSurface::DrawSurface: attempted to draw a surface onto itself");

	if (transparency > 0)
		transparency = ((100 - transparency) * 25) / 10;

	// TODO: remove this limitation?
	if (src->format.bytesPerPixel == 1)
		error("DrawingSurface::DrawSurface: 8bpp surfaces cannot be drawn transparently");

	vm->_graphics->blit(src, dest, Common::Point(0, 0), transparency);

	self->finishedDrawing();
	surfaceToDraw->finishedDrawing(true);

	return RuntimeValue();
}

// DrawingSurface: import void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
// Draws a filled triangle onto the surface.
RuntimeValue Script_DrawingSurface_DrawTriangle(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x1 = params[0]._signedValue;
	UNUSED(x1);
	int y1 = params[1]._signedValue;
	UNUSED(y1);
	int x2 = params[2]._signedValue;
	UNUSED(x2);
	int y2 = params[3]._signedValue;
	UNUSED(y2);
	int x3 = params[4]._signedValue;
	UNUSED(x3);
	int y3 = params[5]._signedValue;
	UNUSED(y3);

	// FIXME
	error("DrawingSurface::DrawTriangle unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import int GetPixel(int x, int y)
// Gets the colour of a single pixel on the surface.
RuntimeValue Script_DrawingSurface_GetPixel(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("DrawingSurface::GetPixel unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import void Release()
// Tells AGS that you have finished drawing onto the surface.
RuntimeValue Script_DrawingSurface_Release(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	self->release();

	return RuntimeValue();
}

// DrawingSurface: import attribute int DrawingColor
// Gets/sets the current AGS Colour Number that will be used for drawing onto this surface.
RuntimeValue Script_DrawingSurface_get_DrawingColor(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DrawingSurface::get_DrawingColor unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import attribute int DrawingColor
// Gets/sets the current AGS Colour Number that will be used for drawing onto this surface.
RuntimeValue Script_DrawingSurface_set_DrawingColor(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("DrawingSurface::set_DrawingColor unimplemented");

	return RuntimeValue();
}

// DrawingSurface: readonly import attribute int Height
// Gets the height of this surface.
RuntimeValue Script_DrawingSurface_get_Height(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DrawingSurface::get_Height unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import attribute bool UseHighResCoordinates
// Determines whether you use high-res or low-res co-ordinates for drawing onto this surface.
RuntimeValue Script_DrawingSurface_get_UseHighResCoordinates(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DrawingSurface::get_UseHighResCoordinates unimplemented");

	return RuntimeValue();
}

// DrawingSurface: import attribute bool UseHighResCoordinates
// Determines whether you use high-res or low-res co-ordinates for drawing onto this surface.
RuntimeValue Script_DrawingSurface_set_UseHighResCoordinates(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("DrawingSurface::set_UseHighResCoordinates unimplemented");

	return RuntimeValue();
}

// DrawingSurface: readonly import attribute int Width
// Gets the width of the surface.
RuntimeValue Script_DrawingSurface_get_Width(AGSEngine *vm, DrawingSurface *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DrawingSurface::get_Width unimplemented");

	return RuntimeValue();
}

// import void Display(const string message, ...)
// Displays the text in a standard text window.
RuntimeValue Script_Display(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *message = (ScriptString *)params[0]._object;

	Common::String string = vm->getTranslation(message->getString());

	Common::Array<RuntimeValue> values = params;
	values.remove_at(0);
	string = vm->formatString(string, values);

	vm->display(string);

	return RuntimeValue();
}

// import void DisplayAt(int x, int y, int width, const string message, ...)
// Displays the text in a standard text window at the specified location.
RuntimeValue Script_DisplayAt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int width = params[2]._signedValue;
	UNUSED(width);
	ScriptString *message = (ScriptString *)params[3]._object;
	UNUSED(message);

	// FIXME
	error("DisplayAt unimplemented");

	return RuntimeValue();
}

// import void DisplayAtY (int y, const string message)
// Displays the text in a standard text window at the specified y-coordinate.
RuntimeValue Script_DisplayAtY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int y = params[0]._signedValue;
	ScriptString *message = (ScriptString *)params[1]._object;

	vm->displayAtY(y, message->getString());

	return RuntimeValue();
}

// import void DisplayMessage(int messageNumber)
// Displays a message from the Room Message Editor.
RuntimeValue Script_DisplayMessage(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int messageNumber = params[0]._signedValue;
	UNUSED(messageNumber);

	// FIXME
	error("DisplayMessage unimplemented");

	return RuntimeValue();
}

// import void DisplayMessageAtY(int messageNumber, int y)
// Displays a message from the Room Message Editor at the specified y-coordinate.
RuntimeValue Script_DisplayMessageAtY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int messageNumber = params[0]._signedValue;
	UNUSED(messageNumber);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("DisplayMessageAtY unimplemented");

	return RuntimeValue();
}

// import void DisplayTopBar(int y, int textColor, int backColor, const string title, const string text, ...)
// Displays a message in a text window with a title, used for speech in SCI0 games.
RuntimeValue Script_DisplayTopBar(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int y = params[0]._signedValue;
	UNUSED(y);
	int textColor = params[1]._signedValue;
	UNUSED(textColor);
	int backColor = params[2]._signedValue;
	UNUSED(backColor);
	ScriptString *title = (ScriptString *)params[3]._object;
	UNUSED(title);
	ScriptString *text = (ScriptString *)params[4]._object;
	UNUSED(text);

	// FIXME
	error("DisplayTopBar unimplemented");

	return RuntimeValue();
}

// import void DisplayMessageBar(int y, int textColor, int backColor, const string title, int message)
// Displays a Room Message Editor message in a text window with a title, used for speech in SCI0 games.
RuntimeValue Script_DisplayMessageBar(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int y = params[0]._signedValue;
	UNUSED(y);
	int textColor = params[1]._signedValue;
	UNUSED(textColor);
	int backColor = params[2]._signedValue;
	UNUSED(backColor);
	ScriptString *title = (ScriptString *)params[3]._object;
	UNUSED(title);
	int message = params[4]._signedValue;
	UNUSED(message);

	// FIXME
	error("DisplayMessageBar unimplemented");

	return RuntimeValue();
}

// import void SetViewport(int x, int y)
// Locks the viewport to stop the screen scrolling automatically.
RuntimeValue Script_SetViewport(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	debugC(kDebugLevelGame, "viewport under script control, now %d,%d", x, y);

	vm->_graphics->_viewportX = vm->multiplyUpCoordinate(x);
	vm->_graphics->_viewportY = vm->multiplyUpCoordinate(y);
	vm->_graphics->checkViewportCoords();
	vm->_state->_offsetsLocked = 1;

	return RuntimeValue();
}

// import void ReleaseViewport()
// Allows AGS to scroll the screen automatically to follow the player character.
RuntimeValue Script_ReleaseViewport(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	debugC(kDebugLevelGame, "viewport released back to engine control");

	vm->_state->_offsetsLocked = 0;

	return RuntimeValue();
}

// import int GetViewportX()
// Gets the current X offset of the scrolled viewport.
RuntimeValue Script_GetViewportX(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(vm->_graphics->_viewportX);
}

// import int GetViewportY()
// Gets the current Y offset of the scrolled viewport.
RuntimeValue Script_GetViewportY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(vm->_graphics->_viewportY);
}

// import int CreateGraphicOverlay(int x, int y, int slot, bool transparent)
// Undocumented.
RuntimeValue Script_CreateGraphicOverlay(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int slot = params[2]._signedValue;
	UNUSED(slot);
	uint32 transparent = params[3]._value;
	UNUSED(transparent);

	// FIXME
	error("CreateGraphicOverlay unimplemented");

	return RuntimeValue();
}

// import int CreateTextOverlay(int x, int y, int width, FontType, int colour, const string text, ...)
// Undocumented.
RuntimeValue Script_CreateTextOverlay(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int width = params[2]._signedValue;
	UNUSED(width);
	uint32 fonttype = params[3]._value;
	UNUSED(fonttype);
	int colour = params[4]._signedValue;
	UNUSED(colour);
	ScriptString *text = (ScriptString *)params[5]._object;
	UNUSED(text);

	// FIXME
	error("CreateTextOverlay unimplemented");

	return RuntimeValue();
}

// import void SetTextOverlay(int overlayID, int x, int y, int width, FontType, int colour, const string text, ...)
// Undocumented.
RuntimeValue Script_SetTextOverlay(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int overlayID = params[0]._signedValue;
	UNUSED(overlayID);
	int x = params[1]._signedValue;
	UNUSED(x);
	int y = params[2]._signedValue;
	UNUSED(y);
	int width = params[3]._signedValue;
	UNUSED(width);
	uint32 fonttype = params[4]._value;
	UNUSED(fonttype);
	int colour = params[5]._signedValue;
	UNUSED(colour);
	ScriptString *text = (ScriptString *)params[6]._object;
	UNUSED(text);

	// FIXME
	error("SetTextOverlay unimplemented");

	return RuntimeValue();
}

// import void RemoveOverlay(int overlayID)
// Undocumented.
RuntimeValue Script_RemoveOverlay(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int overlayID = params[0]._signedValue;
	UNUSED(overlayID);

	// FIXME
	error("RemoveOverlay unimplemented");

	return RuntimeValue();
}

// import int MoveOverlay(int overlayID, int x, int y)
// Undocumented.
RuntimeValue Script_MoveOverlay(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int overlayID = params[0]._signedValue;
	UNUSED(overlayID);
	int x = params[1]._signedValue;
	UNUSED(x);
	int y = params[2]._signedValue;
	UNUSED(y);

	// FIXME
	error("MoveOverlay unimplemented");

	return RuntimeValue();
}

// import int IsOverlayValid(int overlayID)
// Undocumented.
RuntimeValue Script_IsOverlayValid(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint overlayID = params[0]._value;

	return (vm->findOverlayOfType(overlayID) != (uint)-1) ? 1 : 0;
}

// import int LoadImageFile(const string filename)
// Undocumented.
RuntimeValue Script_LoadImageFile(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);

	// FIXME
	error("LoadImageFile unimplemented");

	return RuntimeValue();
}

// import void DeleteSprite(int spriteSlot)
// Undocumented.
RuntimeValue Script_DeleteSprite(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int spriteSlot = params[0]._signedValue;
	UNUSED(spriteSlot);

	// FIXME
	error("DeleteSprite unimplemented");

	return RuntimeValue();
}

// import void SetSpeechFont(FontType)
// Undocumented.
RuntimeValue Script_SetSpeechFont(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 fonttype = params[0]._value;
	UNUSED(fonttype);

	vm->_state->setSpeechFont(fonttype);

	return RuntimeValue();
}

// import void SetNormalFont(FontType)
// Undocumented.
RuntimeValue Script_SetNormalFont(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 fonttype = params[0]._value;

	vm->_state->setNormalFont(fonttype);

	return RuntimeValue();
}

// import void RawClearScreen (int colour)
// Obsolete raw draw stuff.
RuntimeValue Script_RawClearScreen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int colour = params[0]._signedValue;
	UNUSED(colour);

	// FIXME
	error("RawClearScreen unimplemented");

	return RuntimeValue();
}

// import void RawDrawCircle (int x, int y, int radius)
// Obsolete raw draw stuff.
RuntimeValue Script_RawDrawCircle(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int radius = params[2]._signedValue;
	UNUSED(radius);

	// FIXME
	error("RawDrawCircle unimplemented");

	return RuntimeValue();
}

// import void RawDrawImage (int x, int y, int spriteSlot)
// Obsolete raw draw stuff.
RuntimeValue Script_RawDrawImage(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int spriteSlot = params[2]._signedValue;
	UNUSED(spriteSlot);

	// FIXME
	error("RawDrawImage unimplemented");

	return RuntimeValue();
}

// import void RawDrawImageOffset(int x, int y, int spriteSlot)
// Obsolete raw draw stuff.
RuntimeValue Script_RawDrawImageOffset(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int spriteSlot = params[2]._signedValue;
	UNUSED(spriteSlot);

	// FIXME
	error("RawDrawImageOffset unimplemented");

	return RuntimeValue();
}

// import void RawDrawImageResized(int x, int y, int spriteSlot, int width, int height)
// Obsolete raw draw stuff.
RuntimeValue Script_RawDrawImageResized(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int spriteSlot = params[2]._signedValue;
	UNUSED(spriteSlot);
	int width = params[3]._signedValue;
	UNUSED(width);
	int height = params[4]._signedValue;
	UNUSED(height);

	// FIXME
	error("RawDrawImageResized unimplemented");

	return RuntimeValue();
}

// import void RawDrawImageTransparent(int x, int y, int spriteSlot, int transparency)
// Obsolete raw draw stuff.
RuntimeValue Script_RawDrawImageTransparent(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int spriteSlot = params[2]._signedValue;
	UNUSED(spriteSlot);
	int transparency = params[3]._signedValue;
	UNUSED(transparency);

	// FIXME
	error("RawDrawImageTransparent unimplemented");

	return RuntimeValue();
}

// import void RawDrawLine (int x1, int y1, int x2, int y2)
// Obsolete raw draw stuff.
RuntimeValue Script_RawDrawLine(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x1 = params[0]._signedValue;
	UNUSED(x1);
	int y1 = params[1]._signedValue;
	UNUSED(y1);
	int x2 = params[2]._signedValue;
	UNUSED(x2);
	int y2 = params[3]._signedValue;
	UNUSED(y2);

	// FIXME
	error("RawDrawLine unimplemented");

	return RuntimeValue();
}

// import void RawDrawRectangle (int x1, int y1, int x2, int y2)
// Obsolete raw draw stuff.
RuntimeValue Script_RawDrawRectangle(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x1 = params[0]._signedValue;
	UNUSED(x1);
	int y1 = params[1]._signedValue;
	UNUSED(y1);
	int x2 = params[2]._signedValue;
	UNUSED(x2);
	int y2 = params[3]._signedValue;
	UNUSED(y2);

	// FIXME
	error("RawDrawRectangle unimplemented");

	return RuntimeValue();
}

// import void RawDrawTriangle (int x1, int y1, int x2, int y2, int x3, int y3)
// Obsolete raw draw stuff.
RuntimeValue Script_RawDrawTriangle(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x1 = params[0]._signedValue;
	UNUSED(x1);
	int y1 = params[1]._signedValue;
	UNUSED(y1);
	int x2 = params[2]._signedValue;
	UNUSED(x2);
	int y2 = params[3]._signedValue;
	UNUSED(y2);
	int x3 = params[4]._signedValue;
	UNUSED(x3);
	int y3 = params[5]._signedValue;
	UNUSED(y3);

	// FIXME
	error("RawDrawTriangle unimplemented");

	return RuntimeValue();
}

// import void RawPrint (int x, int y, const string message, ...)
// Obsolete raw draw stuff.
RuntimeValue Script_RawPrint(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	ScriptString *message = (ScriptString *)params[2]._object;
	UNUSED(message);

	// FIXME
	error("RawPrint unimplemented");

	return RuntimeValue();
}

// import void RawPrintMessageWrapped (int x, int y, int width, FontType, int messageNumber)
// Obsolete raw draw stuff.
RuntimeValue Script_RawPrintMessageWrapped(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int width = params[2]._signedValue;
	UNUSED(width);
	uint32 fonttype = params[3]._value;
	UNUSED(fonttype);
	int messageNumber = params[4]._signedValue;
	UNUSED(messageNumber);

	// FIXME
	error("RawPrintMessageWrapped unimplemented");

	return RuntimeValue();
}

// import void RawSetColor(int colour)
// Obsolete raw draw stuff.
RuntimeValue Script_RawSetColor(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int colour = params[0]._signedValue;
	UNUSED(colour);

	// FIXME
	error("RawSetColor unimplemented");

	return RuntimeValue();
}

// import void RawSetColorRGB(int red, int green, int blue)
// Obsolete raw draw stuff.
RuntimeValue Script_RawSetColorRGB(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int red = params[0]._signedValue;
	UNUSED(red);
	int green = params[1]._signedValue;
	UNUSED(green);
	int blue = params[2]._signedValue;
	UNUSED(blue);

	// FIXME
	error("RawSetColorRGB unimplemented");

	return RuntimeValue();
}

// import void RawDrawFrameTransparent (int frame, int transparency)
// Obsolete raw draw stuff.
RuntimeValue Script_RawDrawFrameTransparent(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int frame = params[0]._signedValue;
	UNUSED(frame);
	int transparency = params[1]._signedValue;
	UNUSED(transparency);

	// FIXME
	error("RawDrawFrameTransparent unimplemented");

	return RuntimeValue();
}

// import void RawSaveScreen ()
// Obsolete raw draw stuff.
RuntimeValue Script_RawSaveScreen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("RawSaveScreen unimplemented");

	return RuntimeValue();
}

// import void RawRestoreScreen ()
// Obsolete raw draw stuff.
RuntimeValue Script_RawRestoreScreen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("RawRestoreScreen unimplemented");

	return RuntimeValue();
}

// import void RawRestoreScreenTinted(int red, int green, int blue, int opacity)
// Obsolete raw draw stuff.
RuntimeValue Script_RawRestoreScreenTinted(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int red = params[0]._signedValue;
	UNUSED(red);
	int green = params[1]._signedValue;
	UNUSED(green);
	int blue = params[2]._signedValue;
	UNUSED(blue);
	int opacity = params[3]._signedValue;
	UNUSED(opacity);

	// FIXME
	error("RawRestoreScreenTinted unimplemented");

	return RuntimeValue();
}

// import int GetTextWidth(const string text, FontType)
// Gets the width of the specified text in the specified font
RuntimeValue Script_GetTextWidth(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);
	uint32 fonttype = params[1]._value;
	UNUSED(fonttype);

	// FIXME
	error("GetTextWidth unimplemented");

	return RuntimeValue();
}

// import int GetTextHeight(const string text, FontType, int width)
// Gets the height of the specified text in the specified font when wrapped at the specified width
RuntimeValue Script_GetTextHeight(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);
	uint32 fonttype = params[1]._value;
	UNUSED(fonttype);
	int width = params[2]._signedValue;
	UNUSED(width);

	// FIXME
	error("GetTextHeight unimplemented");

	return RuntimeValue();
}

// Overlay: import static Overlay* CreateGraphical(int x, int y, int slot, bool transparent)
// Creates an overlay that displays a sprite.
RuntimeValue Script_Overlay_CreateGraphical(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int slot = params[2]._signedValue;
	UNUSED(slot);
	uint32 transparent = params[3]._value;
	UNUSED(transparent);

	// FIXME
	error("Overlay::CreateGraphical unimplemented");

	return RuntimeValue();
}

// Overlay: import static Overlay* CreateTextual(int x, int y, int width, FontType, int colour, const string text, ...)
// Creates an overlay that displays some text.
RuntimeValue Script_Overlay_CreateTextual(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int width = params[2]._signedValue;
	UNUSED(width);
	uint32 fonttype = params[3]._value;
	UNUSED(fonttype);
	int colour = params[4]._signedValue;
	UNUSED(colour);
	ScriptString *text = (ScriptString *)params[5]._object;
	UNUSED(text);

	// FIXME
	error("Overlay::CreateTextual unimplemented");

	return RuntimeValue();
}

// Overlay: import void SetText(int width, FontType, int colour, const string text, ...)
// Changes the text on the overlay.
RuntimeValue Script_Overlay_SetText(AGSEngine *vm, Overlay *self, const Common::Array<RuntimeValue> &params) {
	int width = params[0]._signedValue;
	UNUSED(width);
	uint32 fonttype = params[1]._value;
	UNUSED(fonttype);
	int colour = params[2]._signedValue;
	UNUSED(colour);
	ScriptString *text = (ScriptString *)params[3]._object;
	UNUSED(text);

	// FIXME
	error("Overlay::SetText unimplemented");

	return RuntimeValue();
}

// Overlay: import void Remove()
// Removes the overlay from the screen.
RuntimeValue Script_Overlay_Remove(AGSEngine *vm, Overlay *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Overlay::Remove unimplemented");

	return RuntimeValue();
}

// Overlay: readonly import attribute bool Valid
// Checks whether this overlay is currently valid.
RuntimeValue Script_Overlay_get_Valid(AGSEngine *vm, Overlay *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Overlay::get_Valid unimplemented");

	return RuntimeValue();
}

// Overlay: import attribute int X
// Gets/sets the X position on the screen where this overlay is displayed.
RuntimeValue Script_Overlay_get_X(AGSEngine *vm, Overlay *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Overlay::get_X unimplemented");

	return RuntimeValue();
}

// Overlay: import attribute int X
// Gets/sets the X position on the screen where this overlay is displayed.
RuntimeValue Script_Overlay_set_X(AGSEngine *vm, Overlay *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Overlay::set_X unimplemented");

	return RuntimeValue();
}

// Overlay: import attribute int Y
// Gets/sets the Y position on the screen where this overlay is displayed.
RuntimeValue Script_Overlay_get_Y(AGSEngine *vm, Overlay *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Overlay::get_Y unimplemented");

	return RuntimeValue();
}

// Overlay: import attribute int Y
// Gets/sets the Y position on the screen where this overlay is displayed.
RuntimeValue Script_Overlay_set_Y(AGSEngine *vm, Overlay *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Overlay::set_Y unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import static DynamicSprite* Create(int width, int height, bool hasAlphaChannel=false)
// Creates a blank dynamic sprite of the specified size.
RuntimeValue Script_DynamicSprite_Create(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int width = params[0]._signedValue;
	UNUSED(width);
	int height = params[1]._signedValue;
	UNUSED(height);
	uint32 hasAlphaChannel = params[2]._value;
	UNUSED(hasAlphaChannel);

	// FIXME
	error("DynamicSprite::Create unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import static DynamicSprite* CreateFromBackground(int frame=SCR_NO_VALUE, int x=SCR_NO_VALUE, int y=SCR_NO_VALUE, int width=SCR_NO_VALUE, int height=SCR_NO_VALUE)
// Creates a dynamic sprite as a copy of a room background.
RuntimeValue Script_DynamicSprite_CreateFromBackground(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int frame = params[0]._signedValue;
	UNUSED(frame);
	int x = params[1]._signedValue;
	UNUSED(x);
	int y = params[2]._signedValue;
	UNUSED(y);
	int width = params[3]._signedValue;
	UNUSED(width);
	int height = params[4]._signedValue;
	UNUSED(height);

	// FIXME
	error("DynamicSprite::CreateFromBackground unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import static DynamicSprite* CreateFromDrawingSurface(DrawingSurface* surface, int x, int y, int width, int height)
// Creates a dynamic sprite as a copy of a drawing surface.
RuntimeValue Script_DynamicSprite_CreateFromDrawingSurface(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	if (!params[0]._object->isOfType(sotDrawingSurface))
		error("DynamicSprite::CreateFromDrawingSurface got incorrect object type (expected a DrawingSurface) for parameter 1");
	DrawingSurface *surface = (DrawingSurface *)params[0]._object;
	UNUSED(surface);
	int x = params[1]._signedValue;
	UNUSED(x);
	int y = params[2]._signedValue;
	UNUSED(y);
	int width = params[3]._signedValue;
	UNUSED(width);
	int height = params[4]._signedValue;
	UNUSED(height);

	// FIXME
	error("DynamicSprite::CreateFromDrawingSurface unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import static DynamicSprite* CreateFromExistingSprite(int slot)
// Creates a dynamic sprite as a copy of an existing sprite (non-alpha version for backwards compatibility).
RuntimeValue Script_DynamicSprite_CreateFromExistingSprite_Old(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int slot = params[0]._signedValue;
	UNUSED(slot);

	// FIXME
	error("DynamicSprite::CreateFromExistingSprite_Old unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import static DynamicSprite* CreateFromExistingSprite(int slot, bool preserveAlphaChannel=0)
// Creates a dynamic sprite as a copy of an existing sprite.
RuntimeValue Script_DynamicSprite_CreateFromExistingSprite(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int slot = params[0]._signedValue;
	UNUSED(slot);
	uint32 preserveAlphaChannel = params[1]._value;
	UNUSED(preserveAlphaChannel);

	// FIXME
	error("DynamicSprite::CreateFromExistingSprite unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import static DynamicSprite* CreateFromFile(const string filename)
// Creates a dynamic sprite from a BMP or PCX file.
RuntimeValue Script_DynamicSprite_CreateFromFile(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);

	// FIXME
	error("DynamicSprite::CreateFromFile unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import static DynamicSprite* CreateFromSaveGame(int slot, int width, int height)
// Creates a dynamic sprite from a save game screenshot.
RuntimeValue Script_DynamicSprite_CreateFromSaveGame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int slot = params[0]._signedValue;
	UNUSED(slot);
	int width = params[1]._signedValue;
	UNUSED(width);
	int height = params[2]._signedValue;
	UNUSED(height);

	// FIXME
	error("DynamicSprite::CreateFromSaveGame unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import static DynamicSprite* CreateFromScreenShot(int width=0, int height=0)
// Creates a dynamic sprite as a copy of the current screen.
RuntimeValue Script_DynamicSprite_CreateFromScreenShot(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int width = params[0]._signedValue;
	UNUSED(width);
	int height = params[1]._signedValue;
	UNUSED(height);

	// FIXME
	error("DynamicSprite::CreateFromScreenShot unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import void ChangeCanvasSize(int width, int height, int x, int y)
// Enlarges the size of the sprite, but does not resize the image.
RuntimeValue Script_DynamicSprite_ChangeCanvasSize(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	int width = params[0]._signedValue;
	UNUSED(width);
	int height = params[1]._signedValue;
	UNUSED(height);
	int x = params[2]._signedValue;
	UNUSED(x);
	int y = params[3]._signedValue;
	UNUSED(y);

	// FIXME
	error("DynamicSprite::ChangeCanvasSize unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import void CopyTransparencyMask(int fromSpriteSlot)
// Copies the transparency mask and/or alpha channel from the specified sprite onto this dynamic sprite.
RuntimeValue Script_DynamicSprite_CopyTransparencyMask(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	int fromSpriteSlot = params[0]._signedValue;
	UNUSED(fromSpriteSlot);

	// FIXME
	error("DynamicSprite::CopyTransparencyMask unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import void Crop(int x, int y, int width, int height)
// Reduces the size of the sprite, but does not resize the image.
RuntimeValue Script_DynamicSprite_Crop(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);
	int width = params[2]._signedValue;
	UNUSED(width);
	int height = params[3]._signedValue;
	UNUSED(height);

	// FIXME
	error("DynamicSprite::Crop unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import void Delete()
// Deletes the dynamic sprite from memory when you no longer need it.
RuntimeValue Script_DynamicSprite_Delete(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DynamicSprite::Delete unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import void Flip(eFlipDirection)
// Flips the sprite in the specified direction.
RuntimeValue Script_DynamicSprite_Flip(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	uint32 eflipdirection = params[0]._value;
	UNUSED(eflipdirection);

	// FIXME
	error("DynamicSprite::Flip unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import DrawingSurface* GetDrawingSurface()
// Gets a drawing surface that can be used to manually draw onto the sprite.
RuntimeValue Script_DynamicSprite_GetDrawingSurface(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DynamicSprite::GetDrawingSurface unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import void Resize(int width, int height)
// Resizes the sprite.
RuntimeValue Script_DynamicSprite_Resize(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	int width = params[0]._signedValue;
	UNUSED(width);
	int height = params[1]._signedValue;
	UNUSED(height);

	// FIXME
	error("DynamicSprite::Resize unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import void Rotate(int angle, int width=SCR_NO_VALUE, int height=SCR_NO_VALUE)
// Rotates the sprite by the specified number of degrees.
RuntimeValue Script_DynamicSprite_Rotate(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	int angle = params[0]._signedValue;
	UNUSED(angle);
	int width = params[1]._signedValue;
	UNUSED(width);
	int height = params[2]._signedValue;
	UNUSED(height);

	// FIXME
	error("DynamicSprite::Rotate unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import int SaveToFile(const string filename)
// Saves the sprite to a BMP or PCX file.
RuntimeValue Script_DynamicSprite_SaveToFile(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);

	// FIXME
	error("DynamicSprite::SaveToFile unimplemented");

	return RuntimeValue();
}

// DynamicSprite: import void Tint(int red, int green, int blue, int saturation, int luminance)
// Permanently tints the sprite to the specified colour.
RuntimeValue Script_DynamicSprite_Tint(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	int red = params[0]._signedValue;
	UNUSED(red);
	int green = params[1]._signedValue;
	UNUSED(green);
	int blue = params[2]._signedValue;
	UNUSED(blue);
	int saturation = params[3]._signedValue;
	UNUSED(saturation);
	int luminance = params[4]._signedValue;
	UNUSED(luminance);

	// FIXME
	error("DynamicSprite::Tint unimplemented");

	return RuntimeValue();
}

// DynamicSprite: readonly import attribute int ColorDepth
// Gets the colour depth of this sprite.
RuntimeValue Script_DynamicSprite_get_ColorDepth(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DynamicSprite::get_ColorDepth unimplemented");

	return RuntimeValue();
}

// DynamicSprite: readonly import attribute int Graphic
// Gets the sprite number of this dynamic sprite, which you can use to display it in the game.
RuntimeValue Script_DynamicSprite_get_Graphic(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DynamicSprite::get_Graphic unimplemented");

	return RuntimeValue();
}

// DynamicSprite: readonly import attribute int Height
// Gets the height of this sprite.
RuntimeValue Script_DynamicSprite_get_Height(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DynamicSprite::get_Height unimplemented");

	return RuntimeValue();
}

// DynamicSprite: readonly import attribute int Width
// Gets the width of this sprite.
RuntimeValue Script_DynamicSprite_get_Width(AGSEngine *vm, DynamicSprite *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DynamicSprite::get_Width unimplemented");

	return RuntimeValue();
}

// import void FadeIn(int speed)
// Fades the screen in from black to the normal palette.
RuntimeValue Script_FadeIn(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int speed = params[0]._signedValue;
	UNUSED(speed);

	// FIXME
	error("FadeIn unimplemented");

	return RuntimeValue();
}

// import void FadeOut(int speed)
// Fades the screen out to the current fade colour.
RuntimeValue Script_FadeOut(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int speed = params[0]._signedValue;
	UNUSED(speed);

	// FIXME
	error("FadeOut unimplemented");

	return RuntimeValue();
}

// import void CyclePalette(int start, int end)
// Cycles the palette entries between start and end. (8-bit games only)
RuntimeValue Script_CyclePalette(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int start = params[0]._signedValue;
	UNUSED(start);
	int end = params[1]._signedValue;
	UNUSED(end);

	// FIXME
	error("CyclePalette unimplemented");

	return RuntimeValue();
}

// import void SetPalRGB(int slot, int r, int g, int b)
// Changes the RGB colour of a palette slot. (8-bit games only)
RuntimeValue Script_SetPalRGB(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int slot = params[0]._signedValue;
	UNUSED(slot);
	int r = params[1]._signedValue;
	UNUSED(r);
	int g = params[2]._signedValue;
	UNUSED(g);
	int b = params[3]._signedValue;
	UNUSED(b);

	// FIXME
	error("SetPalRGB unimplemented");

	return RuntimeValue();
}

// import void UpdatePalette()
// Updates the screen with manual changes to the palette. (8-bit games only)
RuntimeValue Script_UpdatePalette(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("UpdatePalette unimplemented");

	return RuntimeValue();
}

// import void TintScreen (int red, int green, int blue)
// Tints the whole screen to the specified colour.
RuntimeValue Script_TintScreen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int red = params[0]._signedValue;
	UNUSED(red);
	int green = params[1]._signedValue;
	UNUSED(green);
	int blue = params[2]._signedValue;
	UNUSED(blue);

	// FIXME
	error("TintScreen unimplemented");

	return RuntimeValue();
}

// import void SetAmbientTint(int red, int green, int blue, int saturation, int luminance)
// Sets an ambient tint that affects all objects and characters in the room.
RuntimeValue Script_SetAmbientTint(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int red = params[0]._signedValue;
	UNUSED(red);
	int green = params[1]._signedValue;
	UNUSED(green);
	int blue = params[2]._signedValue;
	UNUSED(blue);
	int saturation = params[3]._signedValue;
	UNUSED(saturation);
	int luminance = params[4]._signedValue;
	UNUSED(luminance);

	// FIXME
	error("SetAmbientTint unimplemented");

	return RuntimeValue();
}

// import void SetBackgroundFrame(int frame)
// Locks the current room to the specified background.
RuntimeValue Script_SetBackgroundFrame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint frame = params[0]._value;

	if (frame == (uint)-1) {
		vm->_state->_bgFrameLocked = 0;
		return RuntimeValue();
	}

	if (frame >= vm->getCurrentRoom()->_backgroundScenes.size())
		error("SetBackgroundFrame: frame %d is too high (only have %d backgrounds)",
			frame, vm->getCurrentRoom()->_backgroundScenes.size());

	vm->_state->_bgFrameLocked = 1;
	if (vm->_state->_bgFrame == frame)
		return RuntimeValue();

	vm->_state->_bgFrame = frame;
	vm->invalidateBackground();

	return RuntimeValue();
}

// import int GetBackgroundFrame()
// Gets the current background frame number.
RuntimeValue Script_GetBackgroundFrame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetBackgroundFrame unimplemented");

	return RuntimeValue();
}

// import void ShakeScreen(int amount)
// Shakes the screen by the specified amount.
RuntimeValue Script_ShakeScreen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int amount = params[0]._signedValue;
	UNUSED(amount);

	// FIXME
	error("ShakeScreen unimplemented");

	return RuntimeValue();
}

// import void ShakeScreenBackground(int delay, int amount, int length)
// Shakes the screen but does not pause the game while it does so.
RuntimeValue Script_ShakeScreenBackground(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint delay = params[0]._value;
	uint amount = params[1]._value;
	uint length = params[2]._value;

	if (delay < 2)
		error("ShakeScreenBackground: invalid delay parameter %d", delay);

	vm->_state->_shakeAmount = amount;
	vm->_state->_shakeDelay = delay;
	vm->_state->_shakeLength = length;

	return RuntimeValue();
}

// import void SetScreenTransition(TransitionStyle)
// Changes the room transition style.
RuntimeValue Script_SetScreenTransition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint transitionStyle = params[0]._value;

	if (transitionStyle > FADE_LAST)
		error("SetScreenTransition: invalid transition type %d", transitionStyle);

	vm->_state->_fadeEffect = transitionStyle;

	return RuntimeValue();
}

// import void SetNextScreenTransition(TransitionStyle)
// Changes the room transition style for the next room change only.
RuntimeValue Script_SetNextScreenTransition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 transitionStyle = params[0]._value;

	if (transitionStyle > FADE_LAST)
		error("SetScreenTransition: invalid transition type %d", transitionStyle);

	vm->_state->_nextScreenTransition = transitionStyle;

	return RuntimeValue();
}

// import void SetFadeColor(int red, int green, int blue)
// Changes the colour to which the screen fades out with a FadeOut call.
RuntimeValue Script_SetFadeColor(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int red = params[0]._signedValue;
	UNUSED(red);
	int green = params[1]._signedValue;
	UNUSED(green);
	int blue = params[2]._signedValue;
	UNUSED(blue);

	// FIXME
	error("SetFadeColor unimplemented");

	return RuntimeValue();
}

// import int SaveScreenShot(const string filename)
// Saves a screenshot of the current game position to a file.
RuntimeValue Script_SaveScreenShot(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);

	// FIXME
	error("SaveScreenShot unimplemented");

	return RuntimeValue();
}

// import void FlipScreen(int way)
// Undocumented.
RuntimeValue Script_FlipScreen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int way = params[0]._signedValue;
	UNUSED(way);

	// FIXME
	error("FlipScreen unimplemented");

	return RuntimeValue();
}

// import void SetFrameSound (int view, int loop, int frame, int sound)
// Undocumented.
RuntimeValue Script_SetFrameSound(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int view = params[0]._signedValue;
	UNUSED(view);
	int loop = params[1]._signedValue;
	UNUSED(loop);
	int frame = params[2]._signedValue;
	UNUSED(frame);
	int sound = params[3]._signedValue;
	UNUSED(sound);

	// FIXME
	error("SetFrameSound unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "ViewFrame::get_Flipped", (ScriptAPIFunction *)&Script_ViewFrame_get_Flipped, "", sotViewFrame },
	{ "ViewFrame::get_Frame", (ScriptAPIFunction *)&Script_ViewFrame_get_Frame, "", sotViewFrame },
	{ "ViewFrame::get_Graphic", (ScriptAPIFunction *)&Script_ViewFrame_get_Graphic, "", sotViewFrame },
	{ "ViewFrame::set_Graphic", (ScriptAPIFunction *)&Script_ViewFrame_set_Graphic, "i", sotViewFrame },
	{ "ViewFrame::get_LinkedAudio", (ScriptAPIFunction *)&Script_ViewFrame_get_LinkedAudio, "", sotViewFrame },
	{ "ViewFrame::set_LinkedAudio", (ScriptAPIFunction *)&Script_ViewFrame_set_LinkedAudio, "o", sotViewFrame },
	{ "ViewFrame::get_Loop", (ScriptAPIFunction *)&Script_ViewFrame_get_Loop, "", sotViewFrame },
	{ "ViewFrame::get_Sound", (ScriptAPIFunction *)&Script_ViewFrame_get_Sound, "", sotViewFrame },
	{ "ViewFrame::set_Sound", (ScriptAPIFunction *)&Script_ViewFrame_set_Sound, "i", sotViewFrame },
	{ "ViewFrame::get_Speed", (ScriptAPIFunction *)&Script_ViewFrame_get_Speed, "", sotViewFrame },
	{ "ViewFrame::get_View", (ScriptAPIFunction *)&Script_ViewFrame_get_View, "", sotViewFrame },
	{ "DrawingSurface::Clear^1", (ScriptAPIFunction *)&Script_DrawingSurface_Clear, "i", sotDrawingSurface },
	{ "DrawingSurface::CreateCopy^0", (ScriptAPIFunction *)&Script_DrawingSurface_CreateCopy, "", sotDrawingSurface },
	{ "DrawingSurface::DrawCircle^3", (ScriptAPIFunction *)&Script_DrawingSurface_DrawCircle, "iii", sotDrawingSurface },
	{ "DrawingSurface::DrawImage^6", (ScriptAPIFunction *)&Script_DrawingSurface_DrawImage, "iiiiii", sotDrawingSurface },
	{ "DrawingSurface::DrawLine^5", (ScriptAPIFunction *)&Script_DrawingSurface_DrawLine, "iiiii", sotDrawingSurface },
	{ "DrawingSurface::DrawMessageWrapped^5", (ScriptAPIFunction *)&Script_DrawingSurface_DrawMessageWrapped, "iiiii", sotDrawingSurface },
	{ "DrawingSurface::DrawPixel^2", (ScriptAPIFunction *)&Script_DrawingSurface_DrawPixel, "ii", sotDrawingSurface },
	{ "DrawingSurface::DrawRectangle^4", (ScriptAPIFunction *)&Script_DrawingSurface_DrawRectangle, "iiii", sotDrawingSurface },
	{ "DrawingSurface::DrawString^104", (ScriptAPIFunction *)&Script_DrawingSurface_DrawString, "iiis.", sotDrawingSurface },
	{ "DrawingSurface::DrawStringWrapped^6", (ScriptAPIFunction *)&Script_DrawingSurface_DrawStringWrapped, "iiiiis", sotDrawingSurface },
	{ "DrawingSurface::DrawSurface^2", (ScriptAPIFunction *)&Script_DrawingSurface_DrawSurface, "oi", sotDrawingSurface },
	{ "DrawingSurface::DrawTriangle^6", (ScriptAPIFunction *)&Script_DrawingSurface_DrawTriangle, "iiiiii", sotDrawingSurface },
	{ "DrawingSurface::GetPixel^2", (ScriptAPIFunction *)&Script_DrawingSurface_GetPixel, "ii", sotDrawingSurface },
	{ "DrawingSurface::Release^0", (ScriptAPIFunction *)&Script_DrawingSurface_Release, "", sotDrawingSurface },
	{ "DrawingSurface::get_DrawingColor", (ScriptAPIFunction *)&Script_DrawingSurface_get_DrawingColor, "", sotDrawingSurface },
	{ "DrawingSurface::set_DrawingColor", (ScriptAPIFunction *)&Script_DrawingSurface_set_DrawingColor, "i", sotDrawingSurface },
	{ "DrawingSurface::get_Height", (ScriptAPIFunction *)&Script_DrawingSurface_get_Height, "", sotDrawingSurface },
	{ "DrawingSurface::get_UseHighResCoordinates", (ScriptAPIFunction *)&Script_DrawingSurface_get_UseHighResCoordinates, "", sotDrawingSurface },
	{ "DrawingSurface::set_UseHighResCoordinates", (ScriptAPIFunction *)&Script_DrawingSurface_set_UseHighResCoordinates, "i", sotDrawingSurface },
	{ "DrawingSurface::get_Width", (ScriptAPIFunction *)&Script_DrawingSurface_get_Width, "", sotDrawingSurface },
	{ "Display", (ScriptAPIFunction *)&Script_Display, "s.", sotNone },
	{ "DisplayAt", (ScriptAPIFunction *)&Script_DisplayAt, "iiis.", sotNone },
	{ "DisplayAtY", (ScriptAPIFunction *)&Script_DisplayAtY, "is", sotNone },
	{ "DisplayMessage", (ScriptAPIFunction *)&Script_DisplayMessage, "i", sotNone },
	{ "DisplayMessageAtY", (ScriptAPIFunction *)&Script_DisplayMessageAtY, "ii", sotNone },
	{ "DisplayTopBar", (ScriptAPIFunction *)&Script_DisplayTopBar, "iiiss.", sotNone },
	{ "DisplayMessageBar", (ScriptAPIFunction *)&Script_DisplayMessageBar, "iiisi", sotNone },
	{ "SetViewport", (ScriptAPIFunction *)&Script_SetViewport, "ii", sotNone },
	{ "ReleaseViewport", (ScriptAPIFunction *)&Script_ReleaseViewport, "", sotNone },
	{ "GetViewportX", (ScriptAPIFunction *)&Script_GetViewportX, "", sotNone },
	{ "GetViewportY", (ScriptAPIFunction *)&Script_GetViewportY, "", sotNone },
	{ "CreateGraphicOverlay", (ScriptAPIFunction *)&Script_CreateGraphicOverlay, "iiii", sotNone },
	{ "CreateTextOverlay", (ScriptAPIFunction *)&Script_CreateTextOverlay, "iiiiis.", sotNone },
	{ "SetTextOverlay", (ScriptAPIFunction *)&Script_SetTextOverlay, "iiiiiis.", sotNone },
	{ "RemoveOverlay", (ScriptAPIFunction *)&Script_RemoveOverlay, "i", sotNone },
	{ "MoveOverlay", (ScriptAPIFunction *)&Script_MoveOverlay, "iii", sotNone },
	{ "IsOverlayValid", (ScriptAPIFunction *)&Script_IsOverlayValid, "i", sotNone },
	{ "LoadImageFile", (ScriptAPIFunction *)&Script_LoadImageFile, "s", sotNone },
	{ "DeleteSprite", (ScriptAPIFunction *)&Script_DeleteSprite, "i", sotNone },
	{ "SetSpeechFont", (ScriptAPIFunction *)&Script_SetSpeechFont, "i", sotNone },
	{ "SetNormalFont", (ScriptAPIFunction *)&Script_SetNormalFont, "i", sotNone },
	{ "RawClearScreen", (ScriptAPIFunction *)&Script_RawClearScreen, "i", sotNone },
	{ "RawDrawCircle", (ScriptAPIFunction *)&Script_RawDrawCircle, "iii", sotNone },
	{ "RawDrawImage", (ScriptAPIFunction *)&Script_RawDrawImage, "iii", sotNone },
	{ "RawDrawImageOffset", (ScriptAPIFunction *)&Script_RawDrawImageOffset, "iii", sotNone },
	{ "RawDrawImageResized", (ScriptAPIFunction *)&Script_RawDrawImageResized, "iiiii", sotNone },
	{ "RawDrawImageTransparent", (ScriptAPIFunction *)&Script_RawDrawImageTransparent, "iiii", sotNone },
	{ "RawDrawLine", (ScriptAPIFunction *)&Script_RawDrawLine, "iiii", sotNone },
	{ "RawDrawRectangle", (ScriptAPIFunction *)&Script_RawDrawRectangle, "iiii", sotNone },
	{ "RawDrawTriangle", (ScriptAPIFunction *)&Script_RawDrawTriangle, "iiiiii", sotNone },
	{ "RawPrint", (ScriptAPIFunction *)&Script_RawPrint, "iis.", sotNone },
	{ "RawPrintMessageWrapped", (ScriptAPIFunction *)&Script_RawPrintMessageWrapped, "iiiii", sotNone },
	{ "RawSetColor", (ScriptAPIFunction *)&Script_RawSetColor, "i", sotNone },
	{ "RawSetColorRGB", (ScriptAPIFunction *)&Script_RawSetColorRGB, "iii", sotNone },
	{ "RawDrawFrameTransparent", (ScriptAPIFunction *)&Script_RawDrawFrameTransparent, "ii", sotNone },
	{ "RawSaveScreen", (ScriptAPIFunction *)&Script_RawSaveScreen, "", sotNone },
	{ "RawRestoreScreen", (ScriptAPIFunction *)&Script_RawRestoreScreen, "", sotNone },
	{ "RawRestoreScreenTinted", (ScriptAPIFunction *)&Script_RawRestoreScreenTinted, "iiii", sotNone },
	{ "GetTextWidth", (ScriptAPIFunction *)&Script_GetTextWidth, "si", sotNone },
	{ "GetTextHeight", (ScriptAPIFunction *)&Script_GetTextHeight, "sii", sotNone },
	{ "Overlay::CreateGraphical^4", (ScriptAPIFunction *)&Script_Overlay_CreateGraphical, "iiii", sotNone },
	{ "Overlay::CreateTextual^106", (ScriptAPIFunction *)&Script_Overlay_CreateTextual, "iiiiis", sotNone },
	{ "Overlay::SetText^104", (ScriptAPIFunction *)&Script_Overlay_SetText, "iiis", sotOverlay },
	{ "Overlay::Remove^0", (ScriptAPIFunction *)&Script_Overlay_Remove, "", sotOverlay },
	{ "Overlay::get_Valid", (ScriptAPIFunction *)&Script_Overlay_get_Valid, "", sotOverlay },
	{ "Overlay::get_X", (ScriptAPIFunction *)&Script_Overlay_get_X, "", sotOverlay },
	{ "Overlay::set_X", (ScriptAPIFunction *)&Script_Overlay_set_X, "i", sotOverlay },
	{ "Overlay::get_Y", (ScriptAPIFunction *)&Script_Overlay_get_Y, "", sotOverlay },
	{ "Overlay::set_Y", (ScriptAPIFunction *)&Script_Overlay_set_Y, "i", sotOverlay },
	{ "DynamicSprite::Create^3", (ScriptAPIFunction *)&Script_DynamicSprite_Create, "iii", sotNone },
	{ "DynamicSprite::CreateFromBackground^5", (ScriptAPIFunction *)&Script_DynamicSprite_CreateFromBackground, "iiiii", sotNone },
	{ "DynamicSprite::CreateFromDrawingSurface^5", (ScriptAPIFunction *)&Script_DynamicSprite_CreateFromDrawingSurface, "oiiii", sotNone },
	{ "DynamicSprite::CreateFromExistingSprite^1", (ScriptAPIFunction *)&Script_DynamicSprite_CreateFromExistingSprite_Old, "i", sotNone },
	{ "DynamicSprite::CreateFromExistingSprite^2", (ScriptAPIFunction *)&Script_DynamicSprite_CreateFromExistingSprite, "ii", sotNone },
	{ "DynamicSprite::CreateFromFile^1", (ScriptAPIFunction *)&Script_DynamicSprite_CreateFromFile, "s", sotNone },
	{ "DynamicSprite::CreateFromSaveGame^3", (ScriptAPIFunction *)&Script_DynamicSprite_CreateFromSaveGame, "iii", sotNone },
	{ "DynamicSprite::CreateFromScreenShot^2", (ScriptAPIFunction *)&Script_DynamicSprite_CreateFromScreenShot, "ii", sotNone },
	{ "DynamicSprite::ChangeCanvasSize^4", (ScriptAPIFunction *)&Script_DynamicSprite_ChangeCanvasSize, "iiii", sotDynamicSprite },
	{ "DynamicSprite::CopyTransparencyMask^1", (ScriptAPIFunction *)&Script_DynamicSprite_CopyTransparencyMask, "i", sotDynamicSprite },
	{ "DynamicSprite::Crop^4", (ScriptAPIFunction *)&Script_DynamicSprite_Crop, "iiii", sotDynamicSprite },
	{ "DynamicSprite::Delete^0", (ScriptAPIFunction *)&Script_DynamicSprite_Delete, "", sotDynamicSprite },
	{ "DynamicSprite::Flip^1", (ScriptAPIFunction *)&Script_DynamicSprite_Flip, "i", sotDynamicSprite },
	{ "DynamicSprite::GetDrawingSurface^0", (ScriptAPIFunction *)&Script_DynamicSprite_GetDrawingSurface, "", sotDynamicSprite },
	{ "DynamicSprite::Resize^2", (ScriptAPIFunction *)&Script_DynamicSprite_Resize, "ii", sotDynamicSprite },
	{ "DynamicSprite::Rotate^3", (ScriptAPIFunction *)&Script_DynamicSprite_Rotate, "iii", sotDynamicSprite },
	{ "DynamicSprite::SaveToFile^1", (ScriptAPIFunction *)&Script_DynamicSprite_SaveToFile, "s", sotDynamicSprite },
	{ "DynamicSprite::Tint^5", (ScriptAPIFunction *)&Script_DynamicSprite_Tint, "iiiii", sotDynamicSprite },
	{ "DynamicSprite::get_ColorDepth", (ScriptAPIFunction *)&Script_DynamicSprite_get_ColorDepth, "", sotDynamicSprite },
	{ "DynamicSprite::get_Graphic", (ScriptAPIFunction *)&Script_DynamicSprite_get_Graphic, "", sotDynamicSprite },
	{ "DynamicSprite::get_Height", (ScriptAPIFunction *)&Script_DynamicSprite_get_Height, "", sotDynamicSprite },
	{ "DynamicSprite::get_Width", (ScriptAPIFunction *)&Script_DynamicSprite_get_Width, "", sotDynamicSprite },
	{ "FadeIn", (ScriptAPIFunction *)&Script_FadeIn, "i", sotNone },
	{ "FadeOut", (ScriptAPIFunction *)&Script_FadeOut, "i", sotNone },
	{ "CyclePalette", (ScriptAPIFunction *)&Script_CyclePalette, "ii", sotNone },
	{ "SetPalRGB", (ScriptAPIFunction *)&Script_SetPalRGB, "iiii", sotNone },
	{ "UpdatePalette", (ScriptAPIFunction *)&Script_UpdatePalette, "", sotNone },
	{ "TintScreen", (ScriptAPIFunction *)&Script_TintScreen, "iii", sotNone },
	{ "SetAmbientTint", (ScriptAPIFunction *)&Script_SetAmbientTint, "iiiii", sotNone },
	{ "SetBackgroundFrame", (ScriptAPIFunction *)&Script_SetBackgroundFrame, "i", sotNone },
	{ "GetBackgroundFrame", (ScriptAPIFunction *)&Script_GetBackgroundFrame, "", sotNone },
	{ "ShakeScreen", (ScriptAPIFunction *)&Script_ShakeScreen, "i", sotNone },
	{ "ShakeScreenBackground", (ScriptAPIFunction *)&Script_ShakeScreenBackground, "iii", sotNone },
	{ "SetScreenTransition", (ScriptAPIFunction *)&Script_SetScreenTransition, "i", sotNone },
	{ "SetNextScreenTransition", (ScriptAPIFunction *)&Script_SetNextScreenTransition, "i", sotNone },
	{ "SetFadeColor", (ScriptAPIFunction *)&Script_SetFadeColor, "iii", sotNone },
	{ "SaveScreenShot", (ScriptAPIFunction *)&Script_SaveScreenShot, "s", sotNone },
	{ "FlipScreen", (ScriptAPIFunction *)&Script_FlipScreen, "i", sotNone },
	{ "SetFrameSound", (ScriptAPIFunction *)&Script_SetFrameSound, "iiii", sotNone },
};

void addGraphicsSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
