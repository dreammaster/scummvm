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
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"

namespace AGS {

// import void Debug(int command, int data)
// Performs various debugging commands.
RuntimeValue Script_Debug(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int command = params[0]._signedValue;
	UNUSED(command);
	int data = params[1]._signedValue;
	UNUSED(data);

	// FIXME
	error("Debug unimplemented");

	return RuntimeValue();
}

// import int RunAGSGame(const string filename, int mode, int data)
// Transfers gameplay into a separate AGS game.
RuntimeValue Script_RunAGSGame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);
	int mode = params[1]._signedValue;
	UNUSED(mode);
	int data = params[2]._signedValue;
	UNUSED(data);

	// FIXME
	error("RunAGSGame unimplemented");

	return RuntimeValue();
}

// import int InventoryScreen()
// Undocumented.
RuntimeValue Script_InventoryScreen(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InventoryScreen unimplemented");

	return RuntimeValue();
}

// import int AreThingsOverlapping(int thing1, int thing2)
// Determines whether two objects or characters are overlapping each other.
RuntimeValue Script_AreThingsOverlapping(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int thing1 = params[0]._signedValue;
	UNUSED(thing1);
	int thing2 = params[1]._signedValue;
	UNUSED(thing2);

	// FIXME
	error("AreThingsOverlapping unimplemented");

	return RuntimeValue();
}

// import void SetTimer(int timerID, int timeout)
// Starts a timer, which will expire after the specified number of game loops.
RuntimeValue Script_SetTimer(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint timerID = params[0]._value;
	int timeout = params[1]._signedValue;

	if (timerID >= vm->_state->_scriptTimers.size())
		error("SetTimer: timer %d is too high (only have %d)", timerID, vm->_state->_scriptTimers.size());

	if (timeout < 0)
		error("SetTimer: timeout %d is negative", timeout);

	vm->_state->_scriptTimers[timerID] = timeout;

	return RuntimeValue();
}

// import bool IsTimerExpired(int timerID)
// Returns true the first time this is called after the timer expires.
RuntimeValue Script_IsTimerExpired(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint timerID = params[0]._value;

	if (timerID >= vm->_state->_scriptTimers.size())
		error("SetTimer: timer %d is too high (only have %d)", timerID, vm->_state->_scriptTimers.size());

	if (vm->_state->_scriptTimers[timerID] == 1) {
		// The timer has expired; reset it.
		vm->_state->_scriptTimers[timerID] = 0;
		return 1;
	}

	return 0;
}

// import void SetMultitaskingMode (int mode)
// Sets whether the game can continue to run in the background if the player switches to another application.
RuntimeValue Script_SetMultitaskingMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int mode = params[0]._signedValue;
	UNUSED(mode);

	// FIXME
	error("SetMultitaskingMode unimplemented");

	return RuntimeValue();
}

// import int IsInteractionAvailable (int x, int y, CursorMode)
// Checks whether an event handler is registered to handle clicking at the specified location on the screen.
RuntimeValue Script_IsInteractionAvailable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;
	uint32 cursorMode = params[2]._value;

	if (cursorMode == MODE_WALK && !vm->getGameOption(OPT_NOWALKMODE))
		return 1;

	uint locId;
	uint locType = vm->getLocationType(Common::Point(x, y), locId, true);
	if (locType == 0) {
		// click on nothing -> hotspot 0
		// TODO: why not just pass allowHotspot0 to getLocationType?
		locId = 0;
		locType = LOCTYPE_HOTSPOT;
	}

	// TODO: maybe we don't need to do this at all
	x += vm->divideDownCoordinate(vm->_graphics->_viewportX);
	y += vm->divideDownCoordinate(vm->_graphics->_viewportY);

	bool ret;

	switch (locType) {
	case LOCTYPE_CHAR:
		ret = vm->runCharacterInteraction(locId, cursorMode, true);
		break;
	case LOCTYPE_OBJ:
		ret = vm->runObjectInteraction(locId, cursorMode, true);
		break;
	case LOCTYPE_HOTSPOT:
		ret = vm->runHotspotInteraction(locId, cursorMode, true);
		break;
	default:
		error("IsInteractionAvailable: internal error (unknown loctype %d)", locType);
	}

	return ret ? 1 : 0;
}

// import void Wait(int waitLoops)
// Blocks the script for the specified number of game loops.
RuntimeValue Script_Wait(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint waitLoops = params[0]._value;

	if (!waitLoops)
		error("Wait: must wait at least one loop");

	vm->_state->_waitCounter = waitLoops;
	vm->_state->_keySkipWait = 0;

	vm->blockUntil(kUntilWaitDone);

	return RuntimeValue();
}

// import void SkipUntilCharacterStops(CHARID)
// Fast-forwards the game until the specified character finishes moving.
RuntimeValue Script_SkipUntilCharacterStops(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 charid = params[0]._value;

	vm->skipUntilCharacterStops(charid);

	return RuntimeValue();
}

// import void StartCutscene(CutsceneSkipType)
// Specifies the start of a skippable cutscene.
RuntimeValue Script_StartCutscene(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 cutsceneSkipType = params[0]._value;

	if (vm->_state->_inCutscene)
		error("StartCutscene: already in cutscene");
	if (cutsceneSkipType < 1 || cutsceneSkipType > 5)
		error("StartCutscene: skip type %d is invalid", cutsceneSkipType);

	vm->endSkippingUntilCharStops();

	vm->_state->_inCutscene = cutsceneSkipType;
	vm->startSkippableCutscene();

	return RuntimeValue();
}

// import int EndCutscene()
// Specifies the end of a skippable cutscene.
RuntimeValue Script_EndCutscene(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	if (vm->_state->_inCutscene == 0)
		error("EndCutscene: not in a cutscene");

	vm->_state->_inCutscene = 0;

	// store the 'did the player skip it' return value
	uint ret = vm->_state->_fastForward;

	// stop fast-forwarding and force a screen redraw
	vm->stopFastForwarding();
	vm->invalidateScreen();

	return ret;
}

// import void ClaimEvent()
// Prevents further event handlers running for this event.
RuntimeValue Script_ClaimEvent(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	vm->claimEvent();

	return RuntimeValue();
}

// import int ShellExecute(const string operation, const string file, const string parameters, ShowCommand showCommand = SW_SHOW);
// From the ags_shell.dll plugin.
RuntimeValue Script_ShellExecute(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *operation = (ScriptString *)params[0]._object;
	ScriptString *file = (ScriptString *)params[1]._object;
	ScriptString *parameters = (ScriptString *)params[2]._object;
	uint showCommand = params[3]._value;

	warning("attempted to call ShellExecute('%s', '%s', '%s', %d)", operation->getString().c_str(), file->getString().c_str(),
		parameters->getString().c_str(), showCommand);

	return RuntimeValue();
}

// System: readonly import static attribute bool CapsLock
// Gets whether Caps Lock is currently on.
RuntimeValue Script_System_get_CapsLock(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("System::get_CapsLock unimplemented");

	return RuntimeValue();
}

// System: readonly import static attribute int ColorDepth
// Gets the colour depth that the game is running at.
RuntimeValue Script_System_get_ColorDepth(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->_graphics->getPixelFormat().bytesPerPixel * 8;
}

// System: import static attribute int Gamma
// Gets/sets the gamma correction level.
RuntimeValue Script_System_get_Gamma(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->_state->_gammaAdjustment;
}

// System: import static attribute int Gamma
// Gets/sets the gamma correction level.
RuntimeValue Script_System_set_Gamma(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	if (value > 200)
		error("System::set_Gamma: value must be between 0-200 (not %d)", value);

	if (vm->_state->_gammaAdjustment != value) {
		vm->_state->_gammaAdjustment = value;
		debugC(kDebugLevelGame, "gamma control set to %d", value);

		// ScummVM doesn't support gamma.
	}

	return RuntimeValue();
}

// System: readonly import static attribute bool HardwareAcceleration
// Gets whether the game is running with 3D Hardware Acceleration.
RuntimeValue Script_System_get_HardwareAcceleration(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("System::get_HardwareAcceleration unimplemented");

	return RuntimeValue();
}

// System: readonly import static attribute bool NumLock
// Gets whether Num Lock is currently on.
RuntimeValue Script_System_get_NumLock(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("System::get_NumLock unimplemented");

	return RuntimeValue();
}

// System: readonly import static attribute eOperatingSystem OperatingSystem
// Gets which operating system the game is running on.
RuntimeValue Script_System_get_OperatingSystem(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->getOperatingSystem();
}

// System: readonly import static attribute int ScreenHeight
// Gets the screen height of the current resolution.
RuntimeValue Script_System_get_ScreenHeight(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->_graphics->_height;
}

// System: readonly import static attribute int ScreenWidth
// Gets the screen width of the current resolution.
RuntimeValue Script_System_get_ScreenWidth(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->_graphics->_width;
}

// System: readonly import static attribute bool ScrollLock
// Gets whether Scroll Lock is currently on.
RuntimeValue Script_System_get_ScrollLock(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("System::get_ScrollLock unimplemented");

	return RuntimeValue();
}

// System: readonly import static attribute bool SupportsGammaControl
// Gets whether the player's system supports gamma adjustment.
RuntimeValue Script_System_get_SupportsGammaControl(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// ScummVM doesn't support gamma (yet).
	return 0;
}

// System: readonly import static attribute String Version
// Gets the AGS engine version number.
RuntimeValue Script_System_get_Version(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("System::get_Version unimplemented");

	return RuntimeValue();
}

// System: readonly import static attribute int ViewportHeight
// Gets the height of the visible area in which the game is displayed.
RuntimeValue Script_System_get_ViewportHeight(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(vm->_graphics->_height);
}

// System: readonly import static attribute int ViewportWidth
// Gets the width of the visible area in which the game is displayed.
RuntimeValue Script_System_get_ViewportWidth(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(vm->_graphics->_width);
}

// System: import static attribute int Volume
// Gets/sets the audio output volume, from 0-100.
RuntimeValue Script_System_get_Volume(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("System::get_Volume unimplemented");

	return RuntimeValue();
}

// System: import static attribute int Volume
// Gets/sets the audio output volume, from 0-100.
RuntimeValue Script_System_set_Volume(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("System::set_Volume unimplemented");

	return RuntimeValue();
}

// System: import static attribute bool VSync
// Gets/sets whether waiting for the vertical sync is enabled.
RuntimeValue Script_System_get_VSync(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->_graphics->_vsync ? 1 : 0;
}

// System: import static attribute bool VSync
// Gets/sets whether waiting for the vertical sync is enabled.
RuntimeValue Script_System_set_VSync(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	vm->_graphics->_vsync = value ? 1 : 0;

	return RuntimeValue();
}

// System: readonly import static attribute bool Windowed
// Gets whether the game is running in a window.
RuntimeValue Script_System_get_Windowed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("System::get_Windowed unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "Debug", (ScriptAPIFunction *)&Script_Debug, "ii", sotNone },
	{ "RunAGSGame", (ScriptAPIFunction *)&Script_RunAGSGame, "sii", sotNone },
	{ "InventoryScreen", (ScriptAPIFunction *)&Script_InventoryScreen, "", sotNone },
	{ "AreThingsOverlapping", (ScriptAPIFunction *)&Script_AreThingsOverlapping, "ii", sotNone },
	{ "SetTimer", (ScriptAPIFunction *)&Script_SetTimer, "ii", sotNone },
	{ "IsTimerExpired", (ScriptAPIFunction *)&Script_IsTimerExpired, "i", sotNone },
	{ "SetMultitaskingMode", (ScriptAPIFunction *)&Script_SetMultitaskingMode, "i", sotNone },
	{ "IsInteractionAvailable", (ScriptAPIFunction *)&Script_IsInteractionAvailable, "iii", sotNone },
	{ "Wait", (ScriptAPIFunction *)&Script_Wait, "i", sotNone },
	{ "SkipUntilCharacterStops", (ScriptAPIFunction *)&Script_SkipUntilCharacterStops, "i", sotNone },
	{ "StartCutscene", (ScriptAPIFunction *)&Script_StartCutscene, "i", sotNone },
	{ "EndCutscene", (ScriptAPIFunction *)&Script_EndCutscene, "", sotNone },
	{ "ClaimEvent", (ScriptAPIFunction *)&Script_ClaimEvent, "", sotNone },
	{ "ShellExecute", (ScriptAPIFunction *)&Script_ShellExecute, "sssi", sotNone },
	{ "System::get_CapsLock", (ScriptAPIFunction *)&Script_System_get_CapsLock, "", sotNone },
	{ "System::get_ColorDepth", (ScriptAPIFunction *)&Script_System_get_ColorDepth, "", sotNone },
	{ "System::get_Gamma", (ScriptAPIFunction *)&Script_System_get_Gamma, "", sotNone },
	{ "System::set_Gamma", (ScriptAPIFunction *)&Script_System_set_Gamma, "i", sotNone },
	{ "System::get_HardwareAcceleration", (ScriptAPIFunction *)&Script_System_get_HardwareAcceleration, "", sotNone },
	{ "System::get_NumLock", (ScriptAPIFunction *)&Script_System_get_NumLock, "", sotNone },
	{ "System::get_OperatingSystem", (ScriptAPIFunction *)&Script_System_get_OperatingSystem, "", sotNone },
	{ "System::get_ScreenHeight", (ScriptAPIFunction *)&Script_System_get_ScreenHeight, "", sotNone },
	{ "System::get_ScreenWidth", (ScriptAPIFunction *)&Script_System_get_ScreenWidth, "", sotNone },
	{ "System::get_ScrollLock", (ScriptAPIFunction *)&Script_System_get_ScrollLock, "", sotNone },
	{ "System::get_SupportsGammaControl", (ScriptAPIFunction *)&Script_System_get_SupportsGammaControl, "", sotNone },
	{ "System::get_Version", (ScriptAPIFunction *)&Script_System_get_Version, "", sotNone },
	{ "System::get_ViewportHeight", (ScriptAPIFunction *)&Script_System_get_ViewportHeight, "", sotNone },
	{ "System::get_ViewportWidth", (ScriptAPIFunction *)&Script_System_get_ViewportWidth, "", sotNone },
	{ "System::get_Volume", (ScriptAPIFunction *)&Script_System_get_Volume, "", sotNone },
	{ "System::set_Volume", (ScriptAPIFunction *)&Script_System_set_Volume, "i", sotNone },
	{ "System::get_VSync", (ScriptAPIFunction *)&Script_System_get_VSync, "", sotNone },
	{ "System::set_VSync", (ScriptAPIFunction *)&Script_System_set_VSync, "i", sotNone },
	{ "System::get_Windowed", (ScriptAPIFunction *)&Script_System_get_Windowed, "", sotNone },
};

void addMiscSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
