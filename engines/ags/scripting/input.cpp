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
#include "engines/ags/gamefile.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"
#include "common/events.h"

namespace AGS {

// import void ProcessClick(int x, int y, CursorMode)
// Performs default processing of a mouse click at the specified co-ordinates.
RuntimeValue Script_ProcessClick(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;
	uint32 cursorMode = params[2]._value;

	uint locId;
	uint locType = vm->getLocationType(Common::Point(x, y), locId);
	if (locType == 0) {
		// click on nothing -> hotspot 0
		// TODO: why not just pass allowHotspot0 to getLocationType?
		locId = 0;
		locType = LOCTYPE_HOTSPOT;
	}

	// FIXME: adjust by offset

	if (cursorMode == MODE_WALK && !vm->getGameOption(OPT_NOWALKMODE)) {
		// FIXME
		warning("ProcessClick unimplemented (walk mode)");
		return RuntimeValue();
	}

	vm->_state->_usedMode = cursorMode;

	switch (locType) {
	case LOCTYPE_CHAR:
		// FIXME
		warning("ProcessClick unimplemented");
		break;
	case LOCTYPE_OBJ:
		// FIXME
		warning("ProcessClick unimplemented");
		break;
	case LOCTYPE_HOTSPOT:
		// FIXME
		warning("ProcessClick unimplemented");
		break;
	default:
		error("IsInteractionAvailable: internal error (unknown loctype %d)", locType);
	}

	return RuntimeValue();
}

// Mouse: import static void ChangeModeGraphic(CursorMode, int slot)
// Changes the sprite for the specified mouse cursor.
RuntimeValue Script_Mouse_ChangeModeGraphic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;
	uint slot = params[1]._value;

	if (cursormode >= vm->_gameFile->_cursors.size())
		error("ChangeModeGraphic: mode %d is too high (only %d cursors)", cursormode, vm->_gameFile->_cursors.size());

	if (cursormode == MODE_USE && !vm->getGameOption(OPT_FIXEDINVCURSOR))
		error("ChangeModeGraphic: should not be used on the Inventory cursor when the cursor is linked to the active inventory item");

	// TODO: check sanity of slot

	vm->_gameFile->_cursors[cursormode]._pic = slot;

	if (vm->getCursorMode() == cursormode)
		vm->_graphics->setMouseCursor(cursormode);

	return RuntimeValue();
}

// Mouse: import static void ChangeModeHotspot(CursorMode, int x, int y)
// Changes the active hotspot for the specified mouse cursor.
RuntimeValue Script_Mouse_ChangeModeHotspot(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;
	UNUSED(cursormode);
	int x = params[1]._signedValue;
	UNUSED(x);
	int y = params[2]._signedValue;
	UNUSED(y);

	// FIXME
	error("Mouse::ChangeModeHotspot unimplemented");

	return RuntimeValue();
}

// Mouse: import static void ChangeModeView(CursorMode, int view)
// Changes the view used to animate the specified mouse cursor.
RuntimeValue Script_Mouse_ChangeModeView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;
	UNUSED(cursormode);
	int view = params[1]._signedValue;
	UNUSED(view);

	// FIXME
	error("Mouse::ChangeModeView unimplemented");

	return RuntimeValue();
}

// Mouse: import static void DisableMode(CursorMode)
// Disables the specified cursor mode.
RuntimeValue Script_Mouse_DisableMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;

	if (cursormode >= vm->_gameFile->_cursors.size())
		error("Mouse::DisableMode: mode %d is too high (only %d cursors)", cursormode, vm->_gameFile->_cursors.size());

	vm->disableCursorMode(cursormode);

	return RuntimeValue();
}

// Mouse: import static void EnableMode(CursorMode)
// Re-enables the specified cursor mode.
RuntimeValue Script_Mouse_EnableMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;

	if (cursormode >= vm->_gameFile->_cursors.size())
		error("Mouse::EnableMode: mode %d is too high (only %d cursors)", cursormode, vm->_gameFile->_cursors.size());

	vm->enableCursorMode(cursormode);

	return RuntimeValue();
}

// Mouse: import static int GetModeGraphic(CursorMode)
// Gets the sprite used for the specified mouse cursor.
RuntimeValue Script_Mouse_GetModeGraphic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;
	UNUSED(cursormode);

	// FIXME
	error("Mouse::GetModeGraphic unimplemented");

	return RuntimeValue();
}

// Mouse: import static bool IsButtonDown(MouseButton)
// Checks whether the specified mouse button is currently pressed.
RuntimeValue Script_Mouse_IsButtonDown(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 mouseButton = params[0]._value;

	if (mouseButton < 1 || mouseButton > 3)
		error("IsButtonDown: button %d is invalid", mouseButton);

	// TODO: at the time of writing, ScummVM doesn't provide the middle mouse button here
	uint mask = 1 << (mouseButton - 1);
	bool ret = vm->getEventManager()->getButtonState() & mask;

	return (uint)ret;
}

// Mouse: import static void SaveCursorUntilItLeaves()
// Remembers the current mouse cursor and restores it when the mouse leaves the current area.
RuntimeValue Script_Mouse_SaveCursorUntilItLeaves(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Mouse::SaveCursorUntilItLeaves unimplemented");

	return RuntimeValue();
}

// Mouse: import static void SelectNextMode()
// Cycles to the next available mouse cursor.
RuntimeValue Script_Mouse_SelectNextMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Mouse::SelectNextMode unimplemented");

	return RuntimeValue();
}

// Mouse: import static void SetBounds(int left, int top, int right, int bottom)
// Restricts the mouse movement to the specified area.
RuntimeValue Script_Mouse_SetBounds(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int left = params[0]._signedValue;
	UNUSED(left);
	int top = params[1]._signedValue;
	UNUSED(top);
	int right = params[2]._signedValue;
	UNUSED(right);
	int bottom = params[3]._signedValue;
	UNUSED(bottom);

	// FIXME
	error("Mouse::SetBounds unimplemented");

	return RuntimeValue();
}

// Mouse: import static void SetPosition(int x, int y)
// Moves the mouse cursor to the specified location.
RuntimeValue Script_Mouse_SetPosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("Mouse::SetPosition unimplemented");

	return RuntimeValue();
}

// Mouse: import static void Update()
// Updates the X and Y co-ordinates to match the current mouse position.
RuntimeValue Script_Mouse_Update(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Mouse::Update unimplemented");

	return RuntimeValue();
}

// Mouse: import static void UseDefaultGraphic()
// Changes the current mouse cursor back to the default for the current mode.
RuntimeValue Script_Mouse_UseDefaultGraphic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Mouse::UseDefaultGraphic unimplemented");

	return RuntimeValue();
}

// Mouse: import static void UseModeGraphic(CursorMode)
// Changes the mouse cursor to use the graphic for a different non-active cursor mode.
RuntimeValue Script_Mouse_UseModeGraphic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;
	UNUSED(cursormode);

	// FIXME
	error("Mouse::UseModeGraphic unimplemented");

	return RuntimeValue();
}

// Mouse: import static attribute CursorMode Mode
// Gets/sets the current mouse cursor mode.
RuntimeValue Script_Mouse_get_Mode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->getCursorMode();

	return RuntimeValue();
}

// Mouse: import static attribute CursorMode Mode
// Gets/sets the current mouse cursor mode.
RuntimeValue Script_Mouse_set_Mode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	vm->setCursorMode(value);

	return RuntimeValue();
}

// Mouse: import static attribute bool Visible
// Gets/sets whether the mouse cursor is visible.
RuntimeValue Script_Mouse_get_Visible(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return (vm->_state->_mouseCursorHidden ? 0 : 1);
}

// Mouse: import static attribute bool Visible
// Gets/sets whether the mouse cursor is visible.
RuntimeValue Script_Mouse_set_Visible(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint isOn = params[0]._value;

	vm->_state->_mouseCursorHidden = isOn ? 0 : 1;

	return RuntimeValue();
}

// import void ChangeCursorGraphic(int mode, int slot)
// Mouse function.
RuntimeValue Script_ChangeCursorGraphic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int mode = params[0]._signedValue;
	UNUSED(mode);
	int slot = params[1]._signedValue;
	UNUSED(slot);

	// FIXME
	error("ChangeCursorGraphic unimplemented");

	return RuntimeValue();
}

// import void ChangeCursorHotspot(int mode, int x, int y)
// Mouse function.
RuntimeValue Script_ChangeCursorHotspot(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int mode = params[0]._signedValue;
	UNUSED(mode);
	int x = params[1]._signedValue;
	UNUSED(x);
	int y = params[2]._signedValue;
	UNUSED(y);

	// FIXME
	error("ChangeCursorHotspot unimplemented");

	return RuntimeValue();
}

// import int GetCursorMode()
// Mouse function.
RuntimeValue Script_GetCursorMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->getCursorMode();
}

// import void SetCursorMode(CursorMode)
// Mouse function.
RuntimeValue Script_SetCursorMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;

	vm->setCursorMode(cursormode);

	return RuntimeValue();
}

// import void SetNextCursorMode()
// Mouse function.
RuntimeValue Script_SetNextCursorMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("SetNextCursorMode unimplemented");

	return RuntimeValue();
}

// import void SetDefaultCursor()
// Mouse function.
RuntimeValue Script_SetDefaultCursor(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	vm->setDefaultCursor();

	return RuntimeValue();
}

// import void SetMouseCursor(CursorMode)
// Mouse function.
RuntimeValue Script_SetMouseCursor(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 newCursor = params[0]._value;

	vm->_graphics->setMouseCursor(newCursor);

	return RuntimeValue();
}

// import void SetMouseBounds(int left, int top, int right, int bottom)
// Mouse function.
RuntimeValue Script_SetMouseBounds(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int left = params[0]._signedValue;
	UNUSED(left);
	int top = params[1]._signedValue;
	UNUSED(top);
	int right = params[2]._signedValue;
	UNUSED(right);
	int bottom = params[3]._signedValue;
	UNUSED(bottom);

	// FIXME
	error("SetMouseBounds unimplemented");

	return RuntimeValue();
}

// import void SetMousePosition(int x, int y)
// Mouse function.
RuntimeValue Script_SetMousePosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("SetMousePosition unimplemented");

	return RuntimeValue();
}

// import void ShowMouseCursor()
// Mouse function.
RuntimeValue Script_ShowMouseCursor(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ShowMouseCursor unimplemented");

	return RuntimeValue();
}

// import void HideMouseCursor()
// Mouse function.
RuntimeValue Script_HideMouseCursor(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("HideMouseCursor unimplemented");

	return RuntimeValue();
}

// import void RefreshMouse()
// Mouse function.
RuntimeValue Script_RefreshMouse(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("RefreshMouse unimplemented");

	return RuntimeValue();
}

// import void DisableCursorMode(CursorMode)
// Mouse function.
RuntimeValue Script_DisableCursorMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;

	if (cursormode >= vm->_gameFile->_cursors.size())
		error("DisableCursorMode: mode %d is too high (only %d cursors)", cursormode, vm->_gameFile->_cursors.size());

	vm->disableCursorMode(cursormode);

	return RuntimeValue();
}

// import void EnableCursorMode(CursorMode)
// Mouse function.
RuntimeValue Script_EnableCursorMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;

	if (cursormode >= vm->_gameFile->_cursors.size())
		error("EnableCursorMode: mode %d is too high (only %d cursors)", cursormode, vm->_gameFile->_cursors.size());

	vm->enableCursorMode(cursormode);

	return RuntimeValue();
}

// import void SaveCursorForLocationChange()
// Mouse function.
RuntimeValue Script_SaveCursorForLocationChange(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("SaveCursorForLocationChange unimplemented");

	return RuntimeValue();
}

// import int IsButtonDown(MouseButton)
// Mouse function.
// TODO: Point this whole function to IsButtonDown instead.
RuntimeValue Script_IsButtonDown(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 mouseButton = params[0]._value;

	if (mouseButton < 1 || mouseButton > 3)
		error("Mouse::IsButtonDown: button %d is invalid", mouseButton);

	// TODO: at the time of writing, ScummVM doesn't provide the middle mouse button here
	uint mask = 1 << (mouseButton - 1);
	bool ret = vm->getEventManager()->getButtonState() & mask;

	return (uint)ret;
}

// import int WaitKey(int waitLoops)
// Blocks the script for the specified number of game loops, unless a key is pressed.
RuntimeValue Script_WaitKey(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int waitLoops = params[0]._signedValue;
	UNUSED(waitLoops);

	// FIXME
	error("WaitKey unimplemented");

	return RuntimeValue();
}

// import int WaitMouseKey(int waitLoops)
// Blocks the script for the specified number of game loops, unless a key is pressed or the mouse is clicked.
RuntimeValue Script_WaitMouseKey(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int waitLoops = params[0]._signedValue;
	UNUSED(waitLoops);

	// FIXME
	error("WaitMouseKey unimplemented");

	return RuntimeValue();
}

// import bool IsKeyPressed(eKeyCode)
// Checks whether the specified key is currently held down.
RuntimeValue Script_IsKeyPressed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 ekeycode = params[0]._value;
	UNUSED(ekeycode);

	// FIXME
	error("IsKeyPressed unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "ProcessClick", (ScriptAPIFunction *)&Script_ProcessClick, "iii", sotNone },
	{ "Mouse::ChangeModeGraphic^2", (ScriptAPIFunction *)&Script_Mouse_ChangeModeGraphic, "ii", sotNone },
	{ "Mouse::ChangeModeHotspot^3", (ScriptAPIFunction *)&Script_Mouse_ChangeModeHotspot, "iii", sotNone },
	{ "Mouse::ChangeModeView^2", (ScriptAPIFunction *)&Script_Mouse_ChangeModeView, "ii", sotNone },
	{ "Mouse::DisableMode^1", (ScriptAPIFunction *)&Script_Mouse_DisableMode, "i", sotNone },
	{ "Mouse::EnableMode^1", (ScriptAPIFunction *)&Script_Mouse_EnableMode, "i", sotNone },
	{ "Mouse::GetModeGraphic^1", (ScriptAPIFunction *)&Script_Mouse_GetModeGraphic, "i", sotNone },
	{ "Mouse::IsButtonDown^1", (ScriptAPIFunction *)&Script_Mouse_IsButtonDown, "i", sotNone },
	{ "Mouse::SaveCursorUntilItLeaves^0", (ScriptAPIFunction *)&Script_Mouse_SaveCursorUntilItLeaves, "", sotNone },
	{ "Mouse::SelectNextMode^0", (ScriptAPIFunction *)&Script_Mouse_SelectNextMode, "", sotNone },
	{ "Mouse::SetBounds^4", (ScriptAPIFunction *)&Script_Mouse_SetBounds, "iiii", sotNone },
	{ "Mouse::SetPosition^2", (ScriptAPIFunction *)&Script_Mouse_SetPosition, "ii", sotNone },
	{ "Mouse::Update^0", (ScriptAPIFunction *)&Script_Mouse_Update, "", sotNone },
	{ "Mouse::UseDefaultGraphic^0", (ScriptAPIFunction *)&Script_Mouse_UseDefaultGraphic, "", sotNone },
	{ "Mouse::UseModeGraphic^1", (ScriptAPIFunction *)&Script_Mouse_UseModeGraphic, "i", sotNone },
	{ "Mouse::get_Mode", (ScriptAPIFunction *)&Script_Mouse_get_Mode, "", sotNone },
	{ "Mouse::set_Mode", (ScriptAPIFunction *)&Script_Mouse_set_Mode, "i", sotNone },
	{ "Mouse::get_Visible", (ScriptAPIFunction *)&Script_Mouse_get_Visible, "", sotNone },
	{ "Mouse::set_Visible", (ScriptAPIFunction *)&Script_Mouse_set_Visible, "i", sotNone },
	{ "ChangeCursorGraphic", (ScriptAPIFunction *)&Script_ChangeCursorGraphic, "ii", sotNone },
	{ "ChangeCursorHotspot", (ScriptAPIFunction *)&Script_ChangeCursorHotspot, "iii", sotNone },
	{ "GetCursorMode", (ScriptAPIFunction *)&Script_GetCursorMode, "", sotNone },
	{ "SetCursorMode", (ScriptAPIFunction *)&Script_SetCursorMode, "i", sotNone },
	{ "SetNextCursorMode", (ScriptAPIFunction *)&Script_SetNextCursorMode, "", sotNone },
	{ "SetDefaultCursor", (ScriptAPIFunction *)&Script_SetDefaultCursor, "", sotNone },
	{ "SetMouseCursor", (ScriptAPIFunction *)&Script_SetMouseCursor, "i", sotNone },
	{ "SetMouseBounds", (ScriptAPIFunction *)&Script_SetMouseBounds, "iiii", sotNone },
	{ "SetMousePosition", (ScriptAPIFunction *)&Script_SetMousePosition, "ii", sotNone },
	{ "ShowMouseCursor", (ScriptAPIFunction *)&Script_ShowMouseCursor, "", sotNone },
	{ "HideMouseCursor", (ScriptAPIFunction *)&Script_HideMouseCursor, "", sotNone },
	{ "RefreshMouse", (ScriptAPIFunction *)&Script_RefreshMouse, "", sotNone },
	{ "DisableCursorMode", (ScriptAPIFunction *)&Script_DisableCursorMode, "i", sotNone },
	{ "EnableCursorMode", (ScriptAPIFunction *)&Script_EnableCursorMode, "i", sotNone },
	{ "SaveCursorForLocationChange", (ScriptAPIFunction *)&Script_SaveCursorForLocationChange, "", sotNone },
	{ "IsButtonDown", (ScriptAPIFunction *)&Script_IsButtonDown, "i", sotNone },
	{ "WaitKey", (ScriptAPIFunction *)&Script_WaitKey, "i", sotNone },
	{ "WaitMouseKey", (ScriptAPIFunction *)&Script_WaitMouseKey, "i", sotNone },
	{ "IsKeyPressed", (ScriptAPIFunction *)&Script_IsKeyPressed, "i", sotNone },
};

void addInputSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
