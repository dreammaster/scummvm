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

// This implements the interface of Andreas Vogel's ags_flashlight.dll plugin.

#include "engines/ags/scripting/scripting.h"

namespace AGS {

// import void SetFlashlightTint(int redTint, int greenTint, int blueTint)
// Set the tint of the flashlight, from -31 to 31 (0 is neutral).
RuntimeValue Script_SetFlashlightTint(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int redTint = params[0]._signedValue;
	UNUSED(redTint);
	int greenTint = params[1]._signedValue;
	UNUSED(greenTint);
	int blueTint = params[2]._signedValue;
	UNUSED(blueTint);

	// FIXME
	error("SetFlashlightTint unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightTintRed()
// Get the red component of the flashlight tint.
RuntimeValue Script_GetFlashlightTintRed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightTintRed unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightTintGreen()
// Get the green component of the flashlight tint.
RuntimeValue Script_GetFlashlightTintGreen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightTintGreen unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightTintBlue()
// Get the blue component of the flashlight tint.
RuntimeValue Script_GetFlashlightTintBlue(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightTintBlue unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightMinLightLevel()
// Get the darkest light level of the flashlight.
RuntimeValue Script_GetFlashlightMinLightLevel(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightMinLightLevel unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightMaxLightLevel()
// Get the brightest light level of the flashlight.
RuntimeValue Script_GetFlashlightMaxLightLevel(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightMaxLightLevel unimplemented");

	return RuntimeValue();
}

// import void SetFlashlightDarkness(int lightLevel)
// Set the light level of the darkness around the flashlight.
RuntimeValue Script_SetFlashlightDarkness(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int lightLevel = params[0]._signedValue;
	UNUSED(lightLevel);

	// FIXME
	error("SetFlashlightDarkness unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightDarkness()
// Get the light level of the darkness around the flashlight.
RuntimeValue Script_GetFlashlightDarkness(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightDarkness unimplemented");

	return RuntimeValue();
}

// import void SetFlashlightDarknessSize(int size)
// Set the radius of the darkness around the flashlight.
RuntimeValue Script_SetFlashlightDarknessSize(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int size = params[0]._signedValue;
	UNUSED(size);

	// FIXME
	error("SetFlashlightDarknessSize unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightDarknessSize()
// Get the radius of the darkness around the flashlight.
RuntimeValue Script_GetFlashlightDarknessSize(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightDarknessSize unimplemented");

	return RuntimeValue();
}

// import void SetFlashlightBrightness(int lightLevel)
// Set the light level of the brightness inside the flashlight.
RuntimeValue Script_SetFlashlightBrightness(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int lightLevel = params[0]._signedValue;
	UNUSED(lightLevel);

	// FIXME
	error("SetFlashlightBrightness unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightBrightness()
// Get the light level of the brightness inside the flashlight.
RuntimeValue Script_GetFlashlightBrightness(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightBrightness unimplemented");

	return RuntimeValue();
}

// import void SetFlashlightBrightnessSize(int size)
// Set the radius of the brightness inside the flashlight.
RuntimeValue Script_SetFlashlightBrightnessSize(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int size = params[0]._signedValue;
	UNUSED(size);

	// FIXME
	error("SetFlashlightBrightnessSize unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightBrightnessSize()
// Get the radius of the brightness inside the flashlight.
RuntimeValue Script_GetFlashlightBrightnessSize(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightBrightnessSize unimplemented");

	return RuntimeValue();
}

// import void SetFlashlightPosition(int x, int y)
// Set the position of the flashlight.
RuntimeValue Script_SetFlashlightPosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("SetFlashlightPosition unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightPositionX()
// Get the X position of the flashlight.
RuntimeValue Script_GetFlashlightPositionX(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightPositionX unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightPositionY()
// Get the Y position of the flashlight.
RuntimeValue Script_GetFlashlightPositionY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightPositionY unimplemented");

	return RuntimeValue();
}

// import void SetFlashlightFollowMouse(int on)
// Set whether the flashlight follows the mouse cursor.
RuntimeValue Script_SetFlashlightFollowMouse(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int on = params[0]._signedValue;
	UNUSED(on);

	// FIXME
	error("SetFlashlightFollowMouse unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightFollowMouse()
// Get whether the flashlight follows the mouse cursor.
RuntimeValue Script_GetFlashlightFollowMouse(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightFollowMouse unimplemented");

	return RuntimeValue();
}

// import void SetFlashlightFollowCharacter(int characterId, int dx, int dy, int horz, int vert)
// Set the flashlight to follow a character, with an offset and direction.
RuntimeValue Script_SetFlashlightFollowCharacter(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int characterId = params[0]._signedValue;
	UNUSED(characterId);
	int dx = params[1]._signedValue;
	UNUSED(dx);
	int dy = params[2]._signedValue;
	UNUSED(dy);
	int horz = params[3]._signedValue;
	UNUSED(horz);
	int vert = params[4]._signedValue;
	UNUSED(vert);

	// FIXME
	error("SetFlashlightFollowCharacter unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightFollowCharacter()
// Get the character the flashlight is following.
RuntimeValue Script_GetFlashlightFollowCharacter(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightFollowCharacter unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightCharacterDX()
// Get the X offset for the flashlight when following a character.
RuntimeValue Script_GetFlashlightCharacterDX(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightCharacterDX unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightCharacterDY()
// Get the Y offset for the flashlight when following a character.
RuntimeValue Script_GetFlashlightCharacterDY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightCharacterDY unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightCharacterHorz()
// Get the horizontal direction for the flashlight when following a character.
RuntimeValue Script_GetFlashlightCharacterHorz(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightCharacterHorz unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightCharacterVert()
// Get the vertical direction for the flashlight when following a character.
RuntimeValue Script_GetFlashlightCharacterVert(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightCharacterVert unimplemented");

	return RuntimeValue();
}

// import void SetFlashlightMask(int spriteSlot)
// Set the sprite used as a mask for the lighting, or -1 for none.
RuntimeValue Script_SetFlashlightMask(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int spriteSlot = params[0]._signedValue;
	UNUSED(spriteSlot);

	// FIXME
	error("SetFlashlightMask unimplemented");

	return RuntimeValue();
}

// import int GetFlashlightMask()
// Get the sprite used as a mask for the lighting.
RuntimeValue Script_GetFlashlightMask(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetFlashlightMask unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "SetFlashlightTint", (ScriptAPIFunction *)&Script_SetFlashlightTint, "iii", sotNone },
	{ "GetFlashlightTintRed", (ScriptAPIFunction *)&Script_GetFlashlightTintRed, "", sotNone },
	{ "GetFlashlightTintGreen", (ScriptAPIFunction *)&Script_GetFlashlightTintGreen, "", sotNone },
	{ "GetFlashlightTintBlue", (ScriptAPIFunction *)&Script_GetFlashlightTintBlue, "", sotNone },
	{ "GetFlashlightMinLightLevel", (ScriptAPIFunction *)&Script_GetFlashlightMinLightLevel, "", sotNone },
	{ "GetFlashlightMaxLightLevel", (ScriptAPIFunction *)&Script_GetFlashlightMaxLightLevel, "", sotNone },
	{ "SetFlashlightDarkness", (ScriptAPIFunction *)&Script_SetFlashlightDarkness, "i", sotNone },
	{ "GetFlashlightDarkness", (ScriptAPIFunction *)&Script_GetFlashlightDarkness, "", sotNone },
	{ "SetFlashlightDarknessSize", (ScriptAPIFunction *)&Script_SetFlashlightDarknessSize, "i", sotNone },
	{ "GetFlashlightDarknessSize", (ScriptAPIFunction *)&Script_GetFlashlightDarknessSize, "", sotNone },
	{ "SetFlashlightBrightness", (ScriptAPIFunction *)&Script_SetFlashlightBrightness, "i", sotNone },
	{ "GetFlashlightBrightness", (ScriptAPIFunction *)&Script_GetFlashlightBrightness, "", sotNone },
	{ "SetFlashlightBrightnessSize", (ScriptAPIFunction *)&Script_SetFlashlightBrightnessSize, "i", sotNone },
	{ "GetFlashlightBrightnessSize", (ScriptAPIFunction *)&Script_GetFlashlightBrightnessSize, "", sotNone },
	{ "SetFlashlightPosition", (ScriptAPIFunction *)&Script_SetFlashlightPosition, "ii", sotNone },
	{ "GetFlashlightPositionX", (ScriptAPIFunction *)&Script_GetFlashlightPositionX, "", sotNone },
	{ "GetFlashlightPositionY", (ScriptAPIFunction *)&Script_GetFlashlightPositionY, "", sotNone },
	{ "SetFlashlightFollowMouse", (ScriptAPIFunction *)&Script_SetFlashlightFollowMouse, "i", sotNone },
	{ "GetFlashlightFollowMouse", (ScriptAPIFunction *)&Script_GetFlashlightFollowMouse, "", sotNone },
	{ "SetFlashlightFollowCharacter", (ScriptAPIFunction *)&Script_SetFlashlightFollowCharacter, "iiiii", sotNone },
	{ "GetFlashlightFollowCharacter", (ScriptAPIFunction *)&Script_GetFlashlightFollowCharacter, "", sotNone },
	{ "GetFlashlightCharacterDX", (ScriptAPIFunction *)&Script_GetFlashlightCharacterDX, "", sotNone },
	{ "GetFlashlightCharacterDY", (ScriptAPIFunction *)&Script_GetFlashlightCharacterDY, "", sotNone },
	{ "GetFlashlightCharacterHorz", (ScriptAPIFunction *)&Script_GetFlashlightCharacterHorz, "", sotNone },
	{ "GetFlashlightCharacterVert", (ScriptAPIFunction *)&Script_GetFlashlightCharacterVert, "", sotNone },
	{ "SetFlashlightMask", (ScriptAPIFunction *)&Script_SetFlashlightMask, "i", sotNone },
	{ "GetFlashlightMask", (ScriptAPIFunction *)&Script_GetFlashlightMask, "", sotNone },
};

void addFlashlightSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
