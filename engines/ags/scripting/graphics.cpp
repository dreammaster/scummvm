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
 */

#include "engines/ags/scripting/scripting.h"

namespace AGS {

void addGraphicsSystemScripting(GlobalScriptState *state) {
	// Overlay functions
	state->addSystemFunctionImport("Overlay::CreateGraphical^4", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Overlay::CreateTextual^106", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Overlay::SetText^104", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Overlay::Remove^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Overlay::get_Valid", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Overlay::get_X", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Overlay::set_X", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Overlay::get_Y", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Overlay::set_Y", &Script_UnimplementedStub);

	// DrawingSurface functions
	state->addSystemFunctionImport("DrawingSurface::Clear^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::CreateCopy^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawCircle^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawImage^6", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawLine^5", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawMessageWrapped^5", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawPixel^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawRectangle^4", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawString^104", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawStringWrapped^6", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawSurface^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::DrawTriangle^6", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::GetPixel^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::Release^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::get_DrawingColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::set_DrawingColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::get_Height", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::get_UseHighResCoordinates", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::set_UseHighResCoordinates", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DrawingSurface::get_Width", &Script_UnimplementedStub);

	// DynamicSprite functions
	state->addSystemFunctionImport("DynamicSprite::Create^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::CreateFromBackground", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::CreateFromDrawingSurface^5", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::CreateFromExistingSprite^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::CreateFromExistingSprite^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::CreateFromFile", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::CreateFromSaveGame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::CreateFromScreenShot", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::ChangeCanvasSize^4", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::CopyTransparencyMask^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::Crop^4", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::Delete", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::Flip^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::GetDrawingSurface^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::Resize^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::Rotate^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::SaveToFile^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::Tint^5", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::get_ColorDepth", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::get_Graphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::get_Height", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DynamicSprite::get_Width", &Script_UnimplementedStub);

	// static graphics functions
	state->addSystemFunctionImport("CreateGraphicOverlay", &Script_UnimplementedStub);
	state->addSystemFunctionImport("CreateTextOverlay", &Script_UnimplementedStub);
	state->addSystemFunctionImport("CyclePalette", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DeleteSprite", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Display", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplayAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplayAtY", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplayMessage", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplayMessageAtY", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplayMessageBar", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplaySpeech", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplaySpeechAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplaySpeechBackground", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplayThought", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisplayTopBar", &Script_UnimplementedStub);
	state->addSystemFunctionImport("FadeIn", &Script_UnimplementedStub);
	state->addSystemFunctionImport("FadeOut", &Script_UnimplementedStub);
	state->addSystemFunctionImport("FlipScreen", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetViewportX", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsOverlayValid", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetViewportY", &Script_UnimplementedStub);
	state->addSystemFunctionImport("MoveOverlay", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawClearScreen", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawDrawCircle", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawDrawFrameTransparent", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawDrawImage", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawDrawImageOffset", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawDrawImageResized", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawDrawImageTransparent", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawDrawLine", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawDrawRectangle", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawDrawTriangle", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawPrint", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawPrintMessageWrapped", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawRestoreScreen", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawRestoreScreenTinted", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawSaveScreen", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawSetColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RawSetColorRGB", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SaveScreenShot", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetAmbientTint", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetFadeColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetPalRGB", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetTextOverlay", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ShakeScreen", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ShakeScreenBackground", &Script_UnimplementedStub);
	state->addSystemFunctionImport("TintScreen", &Script_UnimplementedStub);
	state->addSystemFunctionImport("UpdatePalette", &Script_UnimplementedStub);
}

} // End of namespace AGS
