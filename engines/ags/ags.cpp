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

// Base stuff
#include "common/error.h"
#include "common/random.h"
#include "common/stack.h"
#include "common/events.h"

#include "engines/advancedDetector.h"

// Audio
#include "audio/mixer.h"

// AGS subsystems
#include "ags/ags.h"
#include "ags/audio.h"
#include "ags/constants.h"
#include "ags/gamefile.h"
#include "ags/gamestate.h"
#include "ags/graphics.h"
#include "ags/resourceman.h"
#include "ags/room.h"
#include "ags/script.h"
#include "ags/scripting/scripting.h"
#include "ags/sprites.h"

namespace AGS {

#define REP_EXEC_NAME "repeatedly_execute"

const char *kGameDataNameV2 = "ac2game.dta";
const char *kGameDataNameV3 = "game28.dta";

// wrapper class for object/hotspot/region objects, see createGlobalScript
struct RoomObjectState {
	~RoomObjectState() {
		delete _objectObject;
		delete _hotspotObject;
		delete _regionObject;
	}

	Common::Array<RoomObject *> _invalidObjects;
	ScriptObjectArray<RoomObject *> *_objectObject;
	Common::Array<RoomHotspot> _invalidHotspots;
	ScriptObjectArray<RoomHotspot> *_hotspotObject;
	Common::Array<RoomRegion> _invalidRegions;
	ScriptObjectArray<RoomRegion> *_regionObject;
};

AGSEngine::AGSEngine(OSystem *syst, const AGSGameDescription *gameDesc) :
	Engine(syst), _gameDescription(gameDesc), _engineStartTime(0), _playTime(0),
	_resourceMan(0), _needsUpdate(true), _guiNeedsUpdate(true), _poppedInterface((uint)-1),
	_startingRoom(0xffffffff), _displayedRoom(0xffffffff),
	_gameScript(NULL), _gameScriptFork(NULL), _dialogScriptsScript(NULL), _roomScript(NULL), _roomScriptFork(NULL),
	_scriptMouseObject(NULL), _gameStateGlobalsObject(NULL), _saveGameIndexObject(NULL), _scriptSystemObject(NULL),
	_roomObjectState(NULL),
	_currentRoom(NULL), _framesPerSecond(40), _lastFrameTime(0),
	_inNewRoomState(kNewRoomStateNone), _newRoomStateWas(kNewRoomStateNone), _inEntersScreenCounter(0),
	_leavesScreenRoomId(-1),
	_newRoomPos(0), _newRoomX(SCR_NO_VALUE), _newRoomY(SCR_NO_VALUE),
	_blockingUntil(kUntilNothing), _insideProcessEvent(false) {

	_rnd = new Common::RandomSource("ags");
	_scriptState = new GlobalScriptState();
	_state = new GameState(this);
}

AGSEngine::~AGSEngine() {
	for (uint i = 0; i < _scriptModules.size(); ++i)
		delete _scriptModules[i];
	for (uint i = 0; i < _scriptModuleForks.size(); ++i)
		delete _scriptModuleForks[i];
	delete _gameScript;
	delete _gameScriptFork;
	delete _dialogScriptsScript;
	delete _roomScript;
	delete _roomScriptFork;

	delete _currentRoom;
	for (Common::HashMap<uint, Room *>::iterator i = _loadedRooms.begin(); i != _loadedRooms.end(); ++i)
		if (i->_value != _currentRoom)
			delete i->_value;

	delete _scriptState;

	for (uint i = 0; i < _characters.size(); ++i) {
		assert(_characters[i]->getRefCount() == 1);
		_characters[i]->DecRef();
	}

	delete _scriptMouseObject;
	delete _gameStateGlobalsObject;
	delete _saveGameIndexObject;
	delete _scriptSystemObject;
	delete _roomObjectState;

	delete _graphics;
	delete _sprites;

	delete _state;

	delete _audio;
	delete _gameFile;
	delete _resourceMan;

	delete _rnd;
}

bool AGS::AGSEngine::hasFeature(EngineFeature f) const {
	return f == kSupportsReturnToLauncher;
}

const char *AGSEngine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Language AGSEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

Common::Platform AGSEngine::getPlatform() const {
	return _gameDescription->desc.platform;
}

bool AGSEngine::isDemo() const {
	return (_gameDescription->desc.flags & ADGF_DEMO) != 0;
}

const ADGameFileDescription *AGSEngine::getGameFiles() const {
	return _gameDescription->desc.filesDescriptions;
}

const char *AGSEngine::getDetectedGameFile() const {
	return _gameDescription->filename;
}

Common::Error AGSEngine::run() {
	if (!init())
		return Common::kUnknownError;

	// TODO: check for recording/playback?
	if (_displayedRoom == 0xffffffff)
		startNewGame();

	while (!shouldQuit()) {
		mainGameLoop();

		// FIXME: load new game if needed
	}

	return Common::kNoError;
}

bool AGSEngine::mainGameLoop() {
	if (_displayedRoom == 0xffffffff)
		error("mainGameLoop() called before a room was loaded, did game_start try blocking?");

	tickGame(true);

	// FIXME: location updates

	// FIXME: cursor updates

	// update blocking status
	if (_blockingUntil != kUntilNothing) {
		_blockingUntil = checkBlockingUntil();
		if (_blockingUntil == kUntilNothing) {
			// done blocking
			setDefaultCursor();
			invalidateGUI();
			_state->_disabledUserInterface--;

			// original had a few different FOR_ types here, but only FOR_EXITLOOP is used now
			return false;
		}
	}

	return true;
}

// This is called from all over the place, and does all of the per-frame work.
// Similar to 'mainloop' in original code.
void AGSEngine::tickGame(bool checkControls) {
	debug(9, "tickGame");

	uint gameEventQueueSizeAtStartOfFunction = _queuedGameEvents.size();

	if (_inEntersScreenCounter) {
		if (_displayedRoom == _startingRoom)
			error("script tried blocking in the Player Enters Screen event - use After Fadein instead");
		warning("script blocking in Player Enters Screen - use After Fadein instead");
	}

	// FIXME: check no_blocking_functions

	if (_state->_noHiColorFadeIn && getGameOption(OPT_FADETYPE) == FADE_NORMAL)
		_state->_screenIsFadedOut = 0;

	// FIXME: updateGUIDisabledStatus();

	if (_inNewRoomState == kNewRoomStateNone) {
		// Run the room and game script repeatedly_execute
		// FIXME: use repExecAlways on run_function_on_non_blocking_thread
		for (uint i = 0; i < _scriptModules.size(); ++i) {
			runScriptFunction(_scriptModuleForks[i], "repeatedly_execute_always", Common::Array<uint>());
		}
		runScriptFunction(_gameScriptFork, "repeatedly_execute_always", Common::Array<uint>());
		runScriptFunction(_roomScriptFork, "repeatedly_execute_always", Common::Array<uint>());

		queueGameEvent(kEventTextScript, kTextScriptRepeatedlyExecute);
		queueGameEvent(kEventRunEventBlock, kEventBlockRoom, 0, kRoomEventTick);
	}
	// run this immediately to make sure it gets done before fade-in
	// (player enters screen)
	checkNewRoom();

	if (!(_state->_groundLevelAreasDisabled & GLED_INTERACTION)) {
		// FIXME: check hotspots

		// if in a Wait loop which is no longer valid (probably
		// because the Region interaction did a NewRoom), abort
		// the rest of the loop
		if (_blockingUntil != kUntilNothing && checkBlockingUntil() == kUntilNothing) {
			// cancel the Rep Exec and Stands on Hotspot events that
			// we just added -- otherwise the event queue gets huge
			_queuedGameEvents.resize(gameEventQueueSizeAtStartOfFunction);
			return;
		}
	}

	// FIXME: mouse_on_iface=-1;
	// FIXME: checkDebugKeys();

	// FIXME: check if _inNewRoomState is 0
	updateEvents(checkControls);
	if (shouldQuit())
		return;
	// FIXME: check if inventory action changed room

	/* if (!paused) */
		updateStuff();

	// FIXME: a whole bunch of update stuff

	if (!_state->_fastForward) {
		_graphics->draw();

		// FIXME: hotspot stuff
	}

	// FIXME: a whole bunch of update stuff

	_newRoomStateWas = _inNewRoomState;
	if (_inNewRoomState != kNewRoomStateNone)
		queueGameEvent(kEventAfterFadeIn, 0, 0, 0);
	_inNewRoomState = kNewRoomStateNone;
	processAllGameEvents();
	if (_newRoomStateWas && !_inNewRoomState) {
		// we're in a new room, and the room wasn't changed again;
		// we should queue some Enters Screen scripts
		if (_newRoomStateWas == kNewRoomStateFirstTime)
			queueGameEvent(kEventRunEventBlock, kEventBlockRoom, 0, kRoomEventFirstTimeEntersScreen);
		if (_newRoomStateWas != kNewRoomStateSavedGame)
			queueGameEvent(kEventRunEventBlock, kEventBlockRoom, 0, kRoomEventEnterAfterFadeIn);
	}

	// FIXME: maintain background

	_loopCounter++;
	if (_state->_waitCounter)
		_state->_waitCounter--;
	if (_state->_shakeLength)
		_state->_shakeLength--;

	// FIXME

	if (_state->_fastForward)
		return;

	// If we're running faster than the target rate, sleep for a bit.
	uint32 time = _system->getMillis();
	if (time < _lastFrameTime + (1000 / _framesPerSecond))
		_system->delayMillis((1000 / _framesPerSecond) - time + _lastFrameTime);
	_lastFrameTime = _system->getMillis();
}

// note that this is NOT equivalent to the original's update_events
void AGSEngine::updateEvents(bool checkControls) {
	Common::Event event;

	uint numEventsWas = _queuedGameEvents.size();

	uint buttonClicked = 0;

	while (_eventMan->pollEvent(event)) {
		// FIXME: if !checkControls, put these in a queue

		switch (event.type) {
		case Common::EVENT_LBUTTONDOWN:
			// TODO
			buttonClicked = kMouseLeft;
			break;
		case Common::EVENT_RBUTTONDOWN:
			buttonClicked = kMouseRight;
			if (_state->_inCutscene == kSkipESCOrRightButton)
				startSkippingCutscene();
			break;
		case Common::EVENT_MBUTTONDOWN:
			buttonClicked = kMouseMiddle;
			break;

		case Common::EVENT_LBUTTONUP:
			// TODO
			break;

		// run mouse wheel scripts
		case Common::EVENT_WHEELDOWN:
			queueGameEvent(kEventTextScript, kTextScriptOnMouseClick, kMouseWheelSouth);
			break;
		case Common::EVENT_WHEELUP:
			queueGameEvent(kEventTextScript, kTextScriptOnMouseClick, kMouseWheelNorth);
			break;

		case Common::EVENT_KEYDOWN:
			// FIXME: keypresses
			break;

		default:
			break;
		}
	}

	int activeGUI = -1;

	Common::Point mousePos = _system->getEventManager()->getMousePos();

	if (getGameOption(OPT_DISABLEOFF) != 3 /* FIXME: || !_allButtonsDisabled */) {
		for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
			// FIXME: use draw order instead!
			GUIGroup *group = _gameFile->_guiGroups[i];

			// store the mouseover GUI for later use
			if (group->isMouseOver(mousePos))
				activeGUI = i;

			// check for enabled popup-on-y-pos GUIs which need activation
			if (group->_popup != POPUP_MOUSEY)
				continue;
			if (!group->_enabled)
				continue;
			if (group->_popupYP <= (uint)mousePos.y)
				continue;

			// don't display when skipping a cutscene
			if (_state->_fastForward)
				continue;
			// FIXME: break if is complete overlay

			// no need to do it more than once
			if (_poppedInterface == i)
				continue;

			_graphics->setMouseCursor(CURS_ARROW);
			group->setVisible(true);
			_poppedInterface = i;
			// FIXME: pauseGame();
			break;
		}
	}

	if (_poppedInterface != (uint)-1) {
		// remove the popped interface if the mouse has moved away
		GUIGroup *group = _gameFile->_guiGroups[_poppedInterface];
		if (mousePos.y >= group->_y + (int)group->_height)
			removePopupInterface(_poppedInterface);
	}

	// FIXME: check for mouse clicks on GUIs

	if (buttonClicked) {
		if (_state->_inCutscene == kSkipMouseClick || _state->_inCutscene == kSkipAnyKeyOrMouseClick)
			startSkippingCutscene();

		if (_state->_fastForward) {
			// do nothing
		} else if (_state->_waitCounter > 0 && _state->_keySkipWait > 1) {
			// skip wait
			_state->_waitCounter = -1;
		} else {
			// FIXME: the rest
			queueGameEvent(kEventTextScript, kTextScriptOnMouseClick, buttonClicked);
		}
	}

	// walking off edges only happens when none of the below are the case:
	// * if walking off edges is disabled
	if (!(_state->_groundLevelAreasDisabled & GLED_INTERACTION))
		return;
	// * if events have been queued above
	if (numEventsWas != _queuedGameEvents.size())
		return;
	// * if we're moving between rooms, or still in Player Enters Screen
	if (_inNewRoomState != kNewRoomStateNone || _newRoomStateWas != kNewRoomStateNone)
		return;
	// * if the game is paused
	// FIXME
	// * if the GUI is disabled (in wait mode)
	// FIXME

	// work out which edge the player is beyond, if any
	bool edgesActivated[4] = { false, false, false, false };
	if (_playerChar->_x <= _currentRoom->_boundary.left)
		edgesActivated[0] = true;
	else if (_playerChar->_x >= _currentRoom->_boundary.right)
		edgesActivated[1] = true;
	if (_playerChar->_y >= _currentRoom->_boundary.bottom)
		edgesActivated[2] = true;
	else if (_playerChar->_y <= _currentRoom->_boundary.top)
		edgesActivated[3] = true;

	if (_state->_enteredEdge <= 3) {
		if (!edgesActivated[_state->_enteredEdge]) {
			// once the player is no longer outside the edge, forget the stored edge
			_state->_enteredEdge = (uint)-10;
		} else {
			// don't run an edge activation script more than once
			// (original says "if we are walking in from off-screen, don't activate edges")
			edgesActivated[_state->_enteredEdge] = false;
		}
	}

	// run the script for any activated edges
	for (uint i = 0; i < 4; ++i)
		if (edgesActivated[i])
			queueGameEvent(kEventRunEventBlock, kEventBlockRoom, 0, i);
}

void AGSEngine::updateStuff() {
	// This updates the game state when *unpaused*.
	// (GUI updates are handled elsewhere, since they also run when paused.)

	if (_state->_gscriptTimer)
		_state->_gscriptTimer--;

	for (uint i = 0; i < _state->_scriptTimers.size(); ++i) {
		if (_state->_scriptTimers[i] > 1)
			_state->_scriptTimers[i]--;
	}

	for (uint i = 0; i < _currentRoom->_objects.size(); ++i) {
		_currentRoom->_objects[i]->update();
	}

	// FIXME: shadow areas

	// FIXME: character updates

	// FIXME: overlay timers

	// FIXME: speech updates
}

void AGSEngine::startNewGame() {
	setCursorMode(MODE_WALK);
	// FIXME: filter->setMousePosition(160, 100);
	// FIXME: newMusic(0);

	// run startup scripts
	for (uint i = 0; i < _scriptModules.size(); ++i)
		runTextScript(_scriptModules[i], "game_start");
	runTextScript(_gameScript, "game_start");

	// FIXME: setRestartPoint() to make an autosave

	if (_displayedRoom == 0xffffffff) {
		// FIXME: currentFadeOutEffect();
		loadNewRoom(_playerChar->_room, _playerChar);

		// loadNewRoom updates _prevRoom, reset it
		_playerChar->_prevRoom = 0xffffffff;
	}

	firstRoomInitialization();
}

void AGSEngine::setupPlayerCharacter(uint32 charId) {
	_gameFile->_playerChar = charId;
	_playerChar = _characters[charId];
}

class ScriptMouseObject : public ScriptObject {
public:
	ScriptMouseObject(AGSEngine *vm) : _vm(vm) { }
	const char *getObjectTypeName() { return "ScriptMouseObject"; }

	uint32 readUint32(uint offset) {
		switch (offset) {
		case 0:
			return _vm->divideDownCoordinate(_vm->_system->getEventManager()->getMousePos().x);
		case 4:
			return _vm->divideDownCoordinate(_vm->_system->getEventManager()->getMousePos().y);
		default:
			error("ScriptMouseObject::readUint32: offset %d is invalid", offset);
		}
	}

protected:
	AGSEngine *_vm;
};

class ScriptGameStateGlobalsObject : public ScriptObject {
public:
	ScriptGameStateGlobalsObject(AGSEngine *vm) : _vm(vm) { }
	const char *getObjectTypeName() { return "ScriptGameStateGlobalsObject"; }

	uint32 readUint32(uint offset) {
		if (offset % 4 != 0)
			error("ScriptGameStateGlobalsObject::readUint32: offset %d is invalid", offset);
		offset /= 4;
		if (offset >= _vm->_state->_globalVars.size())
			error("ScriptGameStateGlobalsObject::readUint32: index %d is too high (only %d globals)",
				offset, _vm->_state->_globalVars.size());
		return _vm->_state->_globalVars[offset];
	}

protected:
	AGSEngine *_vm;
};

class ScriptSaveGameIndexObject : public ScriptObject {
public:
	ScriptSaveGameIndexObject(AGSEngine *vm) : _vm(vm) { }
	const char *getObjectTypeName() { return "ScriptSaveGameIndexObject"; }

	// FIXME

protected:
	AGSEngine *_vm;
};

class ScriptSystemObject : public ScriptObject {
public:
	ScriptSystemObject(AGSEngine *vm) : _vm(vm) { }
	const char *getObjectTypeName() { return "ScriptSystemObject"; }

	uint32 readUint32(uint offset) {
		switch (offset) {
		case 0:
			// width
			return 0; // FIXME
		case 4:
			// height
			return 0; // FIXME
		case 8:
			// coldepth
			return 32; // FIXME
		case 12:
			// os
			return 0; // FIXME
		case 16:
			// windowed
			return 0; // FIXME
		case 20:
			// vsync
			return _vm->_graphics->_vsync ? 1 : 0;
		case 24:
			// viewport_width
			return 0; // FIXME
		case 28:
			// viewport_height
			return 0; // FIXME
		default:
			error("ScriptSystemObject::readUint32: offset %d is invalid", offset);
		}
	}

protected:
	AGSEngine *_vm;
};

void AGSEngine::createGlobalScript() {
	assert(_scriptModules.empty());

	_scriptMouseObject = new ScriptMouseObject(this);
	_scriptState->addSystemObjectImport("mouse", _scriptMouseObject);
	_scriptState->addSystemObjectImport("game", _state);
	_gameStateGlobalsObject = new ScriptGameStateGlobalsObject(this);
	_scriptState->addSystemObjectImport("gs_globals", _gameStateGlobalsObject);
	_saveGameIndexObject = new ScriptSaveGameIndexObject(this);
	_scriptState->addSystemObjectImport("savegameindex", _saveGameIndexObject);
	_scriptSystemObject = new ScriptSystemObject(this);
	_scriptState->addSystemObjectImport("system", _scriptSystemObject);

	// Some games import these from their global scripts (the original engine points them to
	// a global array of wrapper objects), so we initialise them with empty arrays for now,
	// and replace them later.
	_roomObjectState = new RoomObjectState;
	_roomObjectState->_objectObject = new ScriptObjectArray<RoomObject *>(&_roomObjectState->_invalidObjects, 8, "RoomObject");
	_scriptState->addSystemObjectImport("object", _roomObjectState->_objectObject);
	_roomObjectState->_hotspotObject = new ScriptObjectArray<RoomHotspot>(&_roomObjectState->_invalidHotspots, 8, "RoomHotspot");
	_scriptState->addSystemObjectImport("hotspot", _roomObjectState->_hotspotObject);
	_roomObjectState->_regionObject = new ScriptObjectArray<RoomRegion>(&_roomObjectState->_invalidRegions, 8, "RoomRegion");
	_scriptState->addSystemObjectImport("region", _roomObjectState->_regionObject);

	_scriptState->addSystemObjectImport("dialog", new ScriptObjectArray<DialogTopic>(&_gameFile->_dialogs, 8, "DialogTopic"));
	for (uint i = 0; i < _gameFile->_dialogs.size(); ++i)
		_scriptState->addSystemObjectImport(_gameFile->_dialogs[i]._name, &_gameFile->_dialogs[i]);

	_audio->registerScriptObjects();

	for (uint i = 0; i < _gameFile->_scriptModules.size(); ++i) {
		debug(3, "creating instance for script module %d", i);
		// create an instance for the script module
		_scriptModules.push_back(new ccInstance(this, _gameFile->_scriptModules[i], true));
		// fork an instance for repeatedly_execute_always to run in
		_scriptModuleForks.push_back(new ccInstance(this, _gameFile->_scriptModules[i], true, _scriptModules[i]));
	}

	debug(3, "creating instance for game script");
	// create an instance for the game script
	_gameScript = new ccInstance(this, _gameFile->_gameScript, true);
	// fork an instance for repeatedly_execute_always to run in
	_gameScriptFork = new ccInstance(this, _gameFile->_gameScript, true, _gameScript);

	if (_gameFile->_dialogScriptsScript) {
		debug(3, "creating instance for dialog scripts");
		// create an instance for the 3.1.1+ dialog scripts if present
		_dialogScriptsScript = new ccInstance(this, _gameFile->_dialogScriptsScript, true);
	}
}

void AGSEngine::firstRoomInitialization() {
	_startingRoom = _displayedRoom;
	_loopCounter = 0;
}

void AGSEngine::loadNewRoom(uint32 id, Character *forChar) {
	debug(2, "loading new room %d", id);

	assert(!_currentRoom);

	_state->_roomChanges++;
	_displayedRoom = id;

	Common::String filename = Common::String::format("room%d.crm", id);
	Common::SeekableReadStream *stream = getFile(filename);
	if ((!stream) && (id == 0)) {
		filename = "intro.crm";
		stream = getFile(filename);
	}
	if (!stream)
		error("failed to open room file for room %d", id);

	_currentRoom = new Room(this, stream);
	// FIXME: discard room if we shouldn't keep it
	_loadedRooms[id] = _currentRoom;

	_state->_roomWidth = _currentRoom->_width;
	_state->_roomHeight = _currentRoom->_height;
	_state->_animBackgroundSpeed = _currentRoom->_backgroundSceneAnimSpeed;
	_state->_bgAnimDelay = _currentRoom->_backgroundSceneAnimSpeed;

	_graphics->newRoomPalette();

	// (note: convertCoordinatesToLowRes call was here, now in Room)

	// FIXME

	_inNewRoomState = kNewRoomStateNew;

	// FIXME

	_inNewRoomState = kNewRoomStateFirstTime;

	// FIXME

	_roomObjectState->_objectObject->setArray(&_currentRoom->_objects);
	for (uint i = 0; i < _currentRoom->_objects.size(); ++i) {
		RoomObject *obj = _currentRoom->_objects[i];
		if (obj->_scriptName.empty())
			continue;
		_scriptState->addSystemObjectImport(obj->_scriptName, obj);
	}

	_roomObjectState->_hotspotObject->setArray(&_currentRoom->_hotspots);
	for (uint i = 0; i < _currentRoom->_hotspots.size(); ++i) {
		RoomHotspot &hotspot = _currentRoom->_hotspots[i];
		if (hotspot._scriptName.empty())
			continue;
		_scriptState->addSystemObjectImport(hotspot._scriptName, &hotspot);
	}

	_roomObjectState->_regionObject->setArray(&_currentRoom->_regions);

	// FIXME

	// compile_room_script
	delete _roomScript;
	delete _roomScriptFork;
	_roomScript = new ccInstance(this, _currentRoom->_compiledScript);
	_roomScriptFork = new ccInstance(this, _currentRoom->_compiledScript, false, _roomScript);
	// FIXME: optimization stuff

	// FIXME

	invalidateGUI();
}

void AGSEngine::unloadOldRoom() {
	assert(_currentRoom);

	// FIXME: discard room if we shouldn't keep it
	_currentRoom->unload();
	_currentRoom = NULL;

	_roomObjectState->_objectObject->setArray(&_roomObjectState->_invalidObjects);
	_roomObjectState->_hotspotObject->setArray(&_roomObjectState->_invalidHotspots);
	_roomObjectState->_regionObject->setArray(&_roomObjectState->_invalidRegions);
	// FIXME: remove old exported objects

	// FIXME: a lot of unimplemented stuff
	warning("AGSEngine::unloadOldRoom() unimplemented");
}

void AGSEngine::checkNewRoom() {
	// we only care if we're in a new room, and it's not from a restored game
	if (_inNewRoomState == kNewRoomStateNone || _inNewRoomState == kNewRoomStateSavedGame)
		return;

	// make sure that any script calls don't re-call Enters Screen
	NewRoomState newRoomWas = _inNewRoomState;
	_inNewRoomState = kNewRoomStateNone;

	_state->_disabledUserInterface++;
	runGameEventNow(kEventRunEventBlock, kEventBlockRoom, 0, kRoomEventEntersScreen);
	_state->_disabledUserInterface--;

	_inNewRoomState = newRoomWas;
}

void AGSEngine::scheduleNewRoom(uint roomId) {
	if (_displayedRoom == 0xffffffff) {
		// called from game_start; change the room where the game will start
		_playerChar->_room = roomId;
	}

	debug(1, "Room change requested to room %d", roomId);

	endSkippingUntilCharStops();
	// FIXME
	// can_run_delayed_command();
	if (_state->_stopDialogAtEnd != DIALOG_NONE) {
		if (_state->_stopDialogAtEnd == DIALOG_RUNNING)
			_state->_stopDialogAtEnd = DIALOG_NEWROOM + roomId;
		else
			error("scheduleNewRoom: two NewRoom/RunDialog/StopDialog requests within dialog");
		return;
	}

	if (_leavesScreenRoomId >= 0) {
		// NewRoom called from the Player Leaves Screen event -- just
		// change which room it will go to
		_leavesScreenRoomId = roomId;
	} else if (_inEntersScreenCounter) {
		queueGameEvent(kEventNewRoom, roomId);
		return;
	// FIXME
	//} else if (in_inv_screen) {
	//	inv_screen_newroom = nrnum;
	//	return;
	} else if (_runningScripts.empty()) { // in_graph_script was consulted here
		newRoom(roomId);
		return;
	} else {
		_runningScripts.back().queueAction(kPSANewRoom, roomId, "NewRoom");
		// we might be within a MoveCharacterBlocking -- the room
		// change should abort it
		if (0 < _playerChar->_walking && _playerChar->_walking < TURNING_AROUND) {
			// FIXME
			// nasty hack - make sure it doesn't move the character
			// to a walkable area
			//mls[playerchar->walking].direct = 1;
			//StopMoving(game.playercharacter);
	  }
	}
	// in_graph_script was consulted here
}

/** Changes the current room number and loads a new room from disk */
void AGSEngine::newRoom(uint roomId) {
	warning("AGSEngine::newRoom not implemented");
	endSkippingUntilCharStops();

	debug(1, "Room change requested to room %d", roomId);

	// we are currently running Leaves Screen scripts
	_leavesScreenRoomId = roomId;

	// player leaves screen event
	_eventBlockBaseName = "room";
	if (_currentRoom->_interaction)
		runInteractionEvent(_currentRoom->_interaction, kRoomEventPlayerLeavesScreen);
	else
		runInteractionScript(&_currentRoom->_interactionScripts, kRoomEventPlayerLeavesScreen);

	// Run the global OnRoomLeave event
	// TODO: run_on_event(GE_LEAVE_ROOM, _displayedRoom)

	// TODO: RunPluginHooks(AGSE_LEAVEROOM, _displayedRoom)

	// update the new room number if it has been altered by OnLeave scripts
	roomId = _leavesScreenRoomId;
	_leavesScreenRoomId = -1;

	if (_playerChar->_following >= 0 &&
	    _characters[_playerChar->_following]->_room != roomId) {
		// the player character is following another character,
		// who is not in the new room. therefore, abort the follow
		_playerChar->_following = -1;
	}

	// change rooms
	unloadOldRoom();

	loadNewRoom(roomId, _playerChar);
}

// 'setevent' in original
void AGSEngine::queueGameEvent(GameEventType type, uint data1, uint data2, uint data3) {
	GameEvent ev;
	ev.type = type;
	ev.data1 = data1;
	ev.data2 = data2;
	ev.data3 = data3;
	ev.playerId = _gameFile->_playerChar;
	_queuedGameEvents.push_back(ev);
}

// 'runevent_now' in original
void AGSEngine::runGameEventNow(GameEventType type, uint data1, uint data2, uint data3) {
	GameEvent ev;
	ev.type = type;
	ev.data1 = data1;
	ev.data2 = data2;
	ev.data3 = data3;
	ev.playerId = _gameFile->_playerChar;
	processGameEvent(ev);
}

// 'process_event' in original
void AGSEngine::processGameEvent(const GameEvent &event) {
	switch (event.type) {
	case kEventTextScript:
		{

		Common::String textScriptName;
		switch (event.data1) {
		case kTextScriptRepeatedlyExecute:
			textScriptName = REP_EXEC_NAME;
			break;
		case kTextScriptOnKeyPress:
			textScriptName = "on_key_press";
			break;
		case kTextScriptOnMouseClick:
			textScriptName = "on_mouse_click";
			break;
		default:
			error("processGameEvent: can't do kEventTextScript for script %d", event.data1);
		}

		if ((int)event.data2 > -1000) {
			Common::Array<uint32> params;
			params.push_back(event.data2);
			if (_runningScripts.size())
				_runningScripts.back().queueScript("!" + textScriptName, event.data2);
			else
				runTextScript(_gameScript, textScriptName, params);
		} else {
			if (_runningScripts.size())
				_runningScripts.back().queueScript(textScriptName);
			else
				runTextScript(_gameScript, textScriptName);
		}

		break;
		}
	case kEventRunEventBlock:
		{
		// save old state (to cope with nested calls)
		Common::String oldBaseName = _eventBlockBaseName;
		uint oldEventBlockId = _eventBlockId;

		NewInteraction *interaction = NULL;
		InteractionScript *scripts = NULL;

		switch (event.data1) {
		case kEventBlockHotspot:
			debug(7, "running hotspot interaction: event %d", event.data3);
			_eventBlockBaseName = "hotspot%d";
			_eventBlockId = event.data2;

			// 2.x vs 3.x
			if (_currentRoom->_hotspots[event.data2]._interaction)
				interaction = _currentRoom->_hotspots[event.data2]._interaction;
			else
				scripts = &_currentRoom->_hotspots[event.data2]._interactionScripts;
			break;
		case kEventBlockRoom:
			debug(7, "running room interaction: event %d", event.data3);
			_eventBlockBaseName = "room";

			if (event.data3 == kRoomEventEntersScreen)
				_inEntersScreenCounter++;

			// 2.x vs 3.x
			if (_currentRoom->_interaction)
				interaction = _currentRoom->_interaction;
			else
				scripts = &_currentRoom->_interactionScripts;
			break;
		default:
			error("processGameEvent: unknown event block type %d", event.data1);
		}

		if (scripts) {
			// 3.x script
			runInteractionScript(scripts, event.data3);
		} else if (interaction) {
			// 2.x interaction
			runInteractionEvent(interaction, event.data3);
		}

		if (event.data1 == kEventBlockRoom && event.data3 == kRoomEventEntersScreen) {
			_inEntersScreenCounter--;
		}

		// restore state
		_eventBlockBaseName = oldBaseName;
		_eventBlockId = oldEventBlockId;

		break;
		}
	case kEventAfterFadeIn:
		_state->_screenIsFadedOut = 0;

		uint ourTransition;
		ourTransition = _state->_fadeEffect;
		if (_state->_nextScreenTransition != (uint)-1) {
			// a one-off transition was selected, so use it
			ourTransition = _state->_nextScreenTransition;
			_state->_nextScreenTransition = (uint)-1;
		}

		if (_state->_fastForward)
			return;

		warning("processGameEvent: can't do kEventAfterFadeIn yet"); // FIXME
		// FIXME
		break;
	case kEventInterfaceClick:
		error("processGameEvent: can't do kEventInterfaceClick yet"); // FIXME
		break;
	case kEventNewRoom:
		error("processGameEvent: can't do kEventNewRoom yet"); // FIXME
		break;
	default:
		error("processGameEvent: unknown event type %d", event.type);
	}
}

// 'processallevents' in original
void AGSEngine::processAllGameEvents() {
	if (_insideProcessEvent)
		return;

	_insideProcessEvent = true;

	// make a copy of the events - if processing an event includes
	// a blocking function it will continue to the next game loop
	// and wipe the event list
	Common::Array<GameEvent> events = _queuedGameEvents;

	uint roomWas = _state->_roomChanges;

	for (uint i = 0; i < events.size(); ++i) {
		processGameEvent(events[i]);

		// if the room changed, discard all other events
		if (shouldQuit() || roomWas != _state->_roomChanges)
			break;
	}

	// this is inside the 'update_events' caller in original
	_queuedGameEvents.clear();
	_insideProcessEvent = false;
}

// run a 3.x-style interaction script
// returns true if a room change occurred
bool AGSEngine::runInteractionScript(InteractionScript *scripts, uint eventId, uint checkFallback, bool isInventory) {
	if (eventId >= scripts->_eventScriptNames.size() || scripts->_eventScriptNames[eventId].empty()) {
		// no response for this event

		// if there is a fallback, stop now (caller can run that instead)
		if (checkFallback != (uint)-1 && checkFallback < scripts->_eventScriptNames.size()
			&& !scripts->_eventScriptNames[checkFallback].empty())
			return false;

		runUnhandledEvent(eventId);
		return false;
	}

	if (_state->_checkInteractionOnly) {
		_state->_checkInteractionOnly = 2;

		return true;
	}

	uint roomWas = _state->_roomChanges;

	if (_eventBlockBaseName.contains("character") || _eventBlockBaseName.contains("inventory")) {
		// global script (character or inventory)
		if (_runningScripts.size())
			_runningScripts.back().queueScript(scripts->_eventScriptNames[eventId]);
		else
			runTextScript(_gameScript, scripts->_eventScriptNames[eventId]);
	} else {
		// room script
		if (_runningScripts.size())
			_runningScripts.back().queueScript("|" + scripts->_eventScriptNames[eventId]);
		else
			runTextScript(_roomScript, scripts->_eventScriptNames[eventId]);
	}

	if (roomWas != _state->_roomChanges)
		return false;

	return true;
}

// run a 2.x-style interaction event
// returns true if the NewInteraction has been invalidated (e.g. a room change occurred)
bool AGSEngine::runInteractionEvent(NewInteraction *interaction, uint eventId, uint checkFallback, bool isInventory) {
	if (!interaction->hasResponseFor(eventId)) {
		// no response for this event

		// if there is a fallback, stop now (caller can run that instead)
		if (checkFallback != (uint)-1 && interaction->hasResponseFor(checkFallback))
			return false;

		runUnhandledEvent(eventId);

		return false;
	}

	if (_state->_checkInteractionOnly) {
		_state->_checkInteractionOnly = 2;

		return true;
	}

	uint commandsRunCount = 0;
	bool ret = runInteractionCommandList(interaction->_events[eventId], commandsRunCount);

	// a failed inventory interaction
	if ((commandsRunCount == 0) && isInventory)
		runUnhandledEvent(eventId);

	return ret;
}

enum {
	kActionDoNothing = 0,
	kActionRunScript = 1,
	kActionAddScoreOnce = 2,
	kActionAddScore = 3,
	kActionDisplayMessage = 4,
	kActionPlayMusic = 5,
	kActionStopMusic = 6,
	kActionPlaySound = 7,
	kActionPlayFlic = 8,
	kActionRunDialog = 9,
	kActionEnableDialogOption = 10,
	kActionDisableDialogOption = 11,
	kActionGoToScreen = 12,
	kActionAddInventory = 13,
	kActionMoveObject = 14,
	kActionObjectOff = 15,
	kActionObjectOn = 16,
	kActionSetObjectView = 17,
	kActionAnimateObject = 18,
	kActionMoveCharacter = 19,
	kActionIfInventoryItemUsed = 20,
	kActionIfHasInventoryItem = 21,
	kActionIfCharacterMoving = 22,
	kActionIfVariablesEqual = 23,
	kActionStopCharacterWalking = 24,
	kActionGoToScreenAtCoordinates = 25,
	kActionMoveNPCToRoom = 26,
	kActionSetCharacterView = 27,
	kActionReleaseCharacterView = 28,
	kActionFollowCharacter = 29,
	kActionStopFollowing = 30,
	kActionDisableHotspot = 31,
	kActionEnableHotspot = 32,
	kActionSetVariableValue = 33,
	kActionRunAnimation = 34,
	kActionQuickAnimation = 35,
	kActionSetIdleAnimation = 36,
	kActionDisableIdleAnimation = 37,
	kActionLoseInventoryItem = 38,
	kActionShowGUI = 39,
	kActionHideGUI = 40,
	kActionStopRunningCommands = 41,
	kActionFaceLocation = 42,
	kActionPauseCommands = 43,
	kActionChangeCharacterView = 44,
	kActionIfPlayerCharacterIs = 45,
	kActionIfCursorModeIs = 46,
	kActionIfPlayerHasBeenToRoom = 47
};

Common::String makeTextScriptName(const Common::String &base, uint id, uint val) {
	Common::String string = Common::String::format(base.c_str(), id);
	string += Common::String::format("_%c", 'a' + val);
	return string;
}

bool AGSEngine::runInteractionCommandList(NewInteractionEvent &event, uint &commandsRunCount) {
	assert(event._response);

	const Common::Array<NewInteractionCommand> &commands = event._response->_commands;

	for (uint i = 0; i < commands.size(); ++i) {
		commandsRunCount++;
		uint roomWas = _state->_roomChanges;

		debug(6, "runInteractionCommandList: action %d", commands[i]._type);

		switch (commands[i]._type) {
		case kActionDoNothing:
			break;
		case kActionRunScript:
			if (_eventBlockBaseName.contains("character") || _eventBlockBaseName.contains("inventory")) {
				// global script (character or inventory)
				Common::String name = makeTextScriptName(_eventBlockBaseName, _eventBlockId, commands[i]._args[0]._val);
				if (_runningScripts.size())
					_runningScripts.back().queueScript(name);
				else
					runTextScript(_gameScript, name);
			} else {
				// room script
				// FIXME: bounds check?
				Common::String name = makeTextScriptName(_eventBlockBaseName, _eventBlockId, commands[i]._args[0]._val);
				if (_runningScripts.size())
					_runningScripts.back().queueScript("|" + name);
				else
					runTextScript(_roomScript, name);
			}
			break;
		case kActionAddScoreOnce:
			if (event._response->_timesRun)
				break;
			event._response->_timesRun++;
			// fallthrough
		case kActionAddScore:
			// FIXME
			break;
		case kActionDisplayMessage:
			// FIXME
			break;
		case kActionPlayMusic:
			// FIXME
			break;
		case kActionStopMusic:
			// FIXME
			break;
		case kActionPlaySound:
			// FIXME
			break;
		case kActionPlayFlic:
			// FIXME
			break;
		case kActionRunDialog:
			// FIXME
			break;
		case kActionEnableDialogOption:
			// FIXME
			break;
		case kActionDisableDialogOption:
			// FIXME
			break;
		case kActionGoToScreen:
			// FIXME
			break;
		case kActionAddInventory:
			// FIXME
			break;
		case kActionMoveObject:
			// FIXME
			break;
		case kActionObjectOff:
			// FIXME
			break;
		case kActionObjectOn:
			// FIXME
			break;
		case kActionSetObjectView:
			// FIXME
			break;
		case kActionAnimateObject:
			// FIXME
			break;
		case kActionMoveCharacter:
			// FIXME
			break;
		case kActionIfInventoryItemUsed:
			// FIXME
			break;
		case kActionIfHasInventoryItem:
			// FIXME
			break;
		case kActionIfCharacterMoving:
			// FIXME
			break;
		case kActionIfVariablesEqual:
			// FIXME
			break;
		case kActionStopCharacterWalking:
			// FIXME
			break;
		case kActionGoToScreenAtCoordinates:
			// FIXME
			break;
		case kActionMoveNPCToRoom:
			// FIXME
			break;
		case kActionSetCharacterView:
			// FIXME
			break;
		case kActionReleaseCharacterView:
			// FIXME
			break;
		case kActionFollowCharacter:
			// FIXME
			break;
		case kActionStopFollowing:
			// FIXME
			break;
		case kActionDisableHotspot:
			// FIXME
			break;
		case kActionEnableHotspot:
			// FIXME
			break;
		case kActionSetVariableValue:
			// FIXME
			break;
		case kActionRunAnimation:
			// FIXME
			break;
		case kActionQuickAnimation:
			// FIXME
			break;
		case kActionSetIdleAnimation:
			// FIXME
			break;
		case kActionDisableIdleAnimation:
			// FIXME
			break;
		case kActionLoseInventoryItem:
			// FIXME
			break;
		case kActionShowGUI:
			// FIXME
			break;
		case kActionHideGUI:
			// FIXME
			break;
		case kActionStopRunningCommands:
			return true;
		case kActionFaceLocation:
			// FIXME
			break;
		case kActionPauseCommands:
			// FIXME
			break;
		case kActionChangeCharacterView:
			// FIXME
			break;
		case kActionIfPlayerCharacterIs:
			// FIXME
			break;
		case kActionIfCursorModeIs:
			// FIXME
			break;
		case kActionIfPlayerHasBeenToRoom:
			// FIXME
			break;
		default:
			error("runInteractionEvent: unknown action %d", commands[i]._type);
		}

		// return true if the room changed from under us (the interaction is no longer valid)
		if (shouldQuit() || roomWas != _state->_roomChanges)
			return true;
	}

	return false;
}

void AGSEngine::runUnhandledEvent(uint eventId) {
	// FIXME
}

uint32 AGSEngine::getGameFileVersion() const {
	return _gameFile->_version;
}

uint32 AGSEngine::getGUIVersion() const {
	return _gameFile->_guiVersion;
}

uint32 AGSEngine::getGameUniqueID() const {
	return _gameFile->_uniqueID;
}

Common::SeekableReadStream *AGSEngine::getFile(const Common::String &filename) const {
	return _resourceMan->getFile(filename);
}

Common::String AGSEngine::getMasterArchive() const {
	const ADGameFileDescription *gameFiles = getGameFiles();
	const char *gameFile = getDetectedGameFile();

	if (gameFiles[0].fileName)
		return gameFiles[0].fileName;
	else if (gameFile[0])
		return gameFile;

	return "ac2game.dat";
}

bool AGSEngine::init() {
	// Open the archive file
	_resourceMan = new ResourceManager();
	if (!_resourceMan->init(getMasterArchive()))
		return false;

	// Open any present audio archives
	_audio = new AGSAudio(this);

	// Load the game file
	_gameFile = new GameFile(this);
	if (!_gameFile->init())
		return false;

	// init_game_settings
	_graphics = new AGSGraphics(this);
	_graphics->initPalette();
	if (!_graphics->initGraphics())
		return false;

	adjustSizesForResolution();

	_state->init();

	addSystemScripting(this);

	// FIXME: don't leak all these!
	_scriptState->addSystemObjectImport("character", new ScriptObjectArray<Character *>(&_characters, 780, "Character"));
	_scriptState->addSystemObjectImport("player", _characters[_gameFile->_playerChar]);
	for (uint i = 0; i < _characters.size(); ++i) {
		Character *charInfo = _characters[i];
		if (charInfo->_scriptName.empty())
			continue;
		_scriptState->addSystemObjectImport(charInfo->_scriptName, charInfo);
	}
	_scriptState->addSystemObjectImport("gui", new ScriptObjectArray<GUIGroup *>(&_gameFile->_guiGroups, 8, "GUI"));
	for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
		GUIGroup &group = *_gameFile->_guiGroups[i];
		if (group._name.empty())
			continue;
		_scriptState->addSystemObjectImport(group._name, &group);
		for (uint j = 0; j < group._controls.size(); ++j) {
			if (group._controls[j]->_scriptName.empty())
				continue;
			_scriptState->addSystemObjectImport(group._controls[j]->_scriptName, group._controls[j]);
		}
	}
	_scriptState->addSystemObjectImport("inventory", new ScriptObjectArray<InventoryItem>(&_gameFile->_invItemInfo, 68, "InventoryItem"));
	for (uint i = 0; i < _gameFile->_invItemInfo.size(); ++i) {
		InventoryItem &invItem = _gameFile->_invItemInfo[i];
		if (invItem._scriptName.empty())
			continue;
		_scriptState->addSystemObjectImport(invItem._scriptName, &invItem);
	}

	_graphics->loadFonts();

	for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
		GUIGroup *group = _gameFile->_guiGroups[i];
		if (group->_popup == POPUP_NONE || group->_popup == POPUP_NOAUTOREM)
			group->setVisible(true);
		else
			group->setVisible(false);
	}

	// TODO: wtexttransparent(TEXTFG);
	// TODO: fade_effect=OPT_FADETYPE

	// FIXME: register audio script objects
	// FIXME: other script objects
	setupPlayerCharacter(_gameFile->_playerChar);

	// TODO: start plugins

	// TODO: scripty bits
	createGlobalScript();

	Common::SeekableReadStream *spritesStream = getFile("acsprset.spr");
	if (!spritesStream)
		return false;
	_sprites = new SpriteSet(this, spritesStream);

	syncSoundSettings();

	_engineStartTime = g_system->getMillis();

	return true;
}

void AGSEngine::adjustSizesForResolution() {
	for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
		GUIGroup *group = _gameFile->_guiGroups[i];

		if (group->_width < 1)
			group->_width = 1;
		if (group->_height < 1)
			group->_height = 1;
		// "Temp fix for older games"
		if (group->_width == (uint)_graphics->_baseWidth - 1)
			group->_width = _graphics->_baseWidth;

		for (uint j = 0; j < group->_controls.size(); ++j)
			group->_controls[j]->_activated = 0;
	}

	if (getGameFileVersion() >= kAGSVer300) {
		if (getGameOption(OPT_NATIVECOORDINATES))
			return;
		if (_gameFile->_defaultResolution <= 2)
			return;

		// New 3.1 format game file, but with Use Native Coordinates off
		// Divide down the coordinates which are always stored in native form.

		for (uint i = 0; i < _characters.size(); ++i) {
			_characters[i]->_x /= 2;
			_characters[i]->_y /= 2;
		}

		for (uint i = 0; i < _gameFile->_guiInvControls.size(); ++i) {
			GUIInvControl *control = _gameFile->_guiInvControls[i];
			control->_itemWidth /= 2;
			control->_itemHeight /= 2;
		}

		return;
	}

	// Old format game file, with non-native coordinates. Multiply them all up.

	for (uint i = 0; i < _gameFile->_cursors.size(); ++i) {
		MouseCursor &cursor = _gameFile->_cursors[i];

		cursor._hotspotX = multiplyUpCoordinate(cursor._hotspotX);
		cursor._hotspotY = multiplyUpCoordinate(cursor._hotspotY);
	}

	for (uint i = 0; i < _gameFile->_invItemInfo.size(); ++i) {
		InventoryItem &item = _gameFile->_invItemInfo[i];

		item._hotspotX = multiplyUpCoordinate(item._hotspotX);
		item._hotspotY = multiplyUpCoordinate(item._hotspotY);
	}

	for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
		GUIGroup *group = _gameFile->_guiGroups[i];

		group->_x = multiplyUpCoordinate(group->_x);
		group->_y = multiplyUpCoordinate(group->_y);
		group->_width = multiplyUpCoordinate(group->_width);
		group->_height = multiplyUpCoordinate(group->_height);

		group->_popupYP = multiplyUpCoordinate(group->_popupYP);

		for (uint j = 0; j < group->_controls.size(); ++j) {
			GUIControl *control = group->_controls[j];

			control->_x = multiplyUpCoordinate(control->_x);
			control->_y = multiplyUpCoordinate(control->_y);
			control->_width = multiplyUpCoordinate(control->_width);
			control->_height = multiplyUpCoordinate(control->_height);
		}
	}
}

void AGSEngine::pauseEngineIntern(bool pause) {
	_mixer->pauseAll(pause);
}

// reset the visible cursor to the one for the current mode
void AGSEngine::setDefaultCursor() {
	_graphics->setMouseCursor(_cursorMode);
}

// TODO: rename to something which indicates it sets too?
uint AGSEngine::findNextEnabledCursor(uint32 startWith) {
	if (startWith >= _gameFile->_cursors.size())
		startWith = 0;

	// loop through all cursors until we find one we can use
	uint32 testing = startWith;
	do {
		if (!(_gameFile->_cursors[testing]._flags & MCF_DISABLED)) {
			if (testing == MODE_USE) {
				// inventory cursor - use it if the player has an active item
				// FIXME: inventory logic
				// if (_playerChar->_activeInv != 0xffffffff)
				// 	break;
			} else if (_gameFile->_cursors[testing]._flags & MCF_STANDARD) {
				// standard enabled cursor - use this one
				break;
			}
		}

		if (++testing >= _gameFile->_cursors.size())
			testing = 0;
	} while (testing != startWith);

	if (testing != startWith)
		setCursorMode(testing);

	return testing;
}

void AGSEngine::setCursorMode(uint32 newMode) {
	if (newMode >= _gameFile->_cursors.size())
		error("setCursorMode: invalid cursor mode %d (only %d cursors)", newMode, _gameFile->_cursors.size());
	invalidateGUI();

	if (_gameFile->_cursors[newMode]._flags & MCF_DISABLED) {
		findNextEnabledCursor(newMode);
		return;
	}

	if (newMode == MODE_USE) {
		// FIXME: inventory logic
		// if (_playerChar->_activeInv == 0xffffffff) {
		// 	findNextEnabledCursor(0);
		// 	return;
		// }
		// updateInvCursor(_playerChar->_activeInv);
	}

	_cursorMode = newMode;
	setDefaultCursor();

	debug(1, "cursor mode set to %d", newMode);
}

void AGSEngine::removePopupInterface(uint guiId) {
	if (_poppedInterface != guiId)
		return;

	_poppedInterface = (uint)-1;
	// FIXME: unpauseGame();

	GUIGroup *group = _gameFile->_guiGroups[guiId];
	group->setVisible(false);

	// FIXME: filter?

	if (_state->_disabledUserInterface) {
		// Only change the mouse cursor if it hasn't been specifically changed first
		if (_cursorMode == _graphics->getCurrentCursor())
			_graphics->setMouseCursor(CURS_WAIT);
	} else
		setDefaultCursor();

	// FIXME: reset mouse_on_iface
}

ViewLoopNew *AGSEngine::getViewLoop(uint view, uint loop) {
	return &_gameFile->_views[view]._loops[loop];
}

ViewFrame *AGSEngine::getViewFrame(uint view, uint loop, uint frame) {
	return &_gameFile->_views[view]._loops[loop]._frames[frame];
}

void AGSEngine::checkViewFrame(uint view, uint loop, uint frame) {
	// FIXME: check sounds for new frames
}

void AGSEngine::runTextScript(ccInstance *instance, const Common::String &name, const Common::Array<uint32> &params) {
	// first, check for special cases
	switch (params.size()) {
	case 0:
		if (name != REP_EXEC_NAME)
			break;
		// repeatedly_execute
		for (uint i = 0; i < _scriptModules.size(); ++i) {
			// FIXME: original checks whether the symbol exists first, unnecessary?
			runScriptFunction(_scriptModules[i], name, params);
		}
		break;
	}

	if (!runScriptFunction(instance, name, params)) {
		if (instance == _roomScript)
			error("failed to run room script '%s' (room %d)", name.c_str(), _displayedRoom);
	}
}

ScriptImport AGSEngine::resolveImport(const Common::String &name) {
	if (name.empty()) {
		// no such import
		ScriptImport import;
		import._type = sitInvalid;
		return import;
	}

	if (_scriptState->_imports.contains(name))
		return _scriptState->_imports[name];

	// try resolving it without the parameter count (i.e. FunctionName^3 -> FunctionName)
	if (name.size() >= 3) {
		Common::String mangledName = name;
		while (mangledName[mangledName.size() - 1] != '^' && mangledName.size() >= 2)
			mangledName.deleteLastChar();
		if (mangledName[mangledName.size() - 1] == '^') {
			mangledName.deleteLastChar();
			if (_scriptState->_imports.contains(mangledName))
				return _scriptState->_imports[mangledName];
		}
	}

	error("unresolved script import '%s'", name.c_str());
}

GlobalScriptState *AGSEngine::getScriptState() {
	return _scriptState;
}

uint32 AGSEngine::getGameSpeed() {
	return _framesPerSecond - _state->_gameSpeedModifier;
}

void AGSEngine::setGameSpeed(uint32 speed) {
	// FIXME: check for maxed out frame rate

	speed += _state->_gameSpeedModifier;
	_framesPerSecond = CLIP<uint32>(speed, 10, 1000);
}

byte AGSEngine::getGameOption(uint index) {
	return _gameFile->_options[index];
}

Common::String AGSEngine::getTranslation(const Common::String &text) {
	// FIXME: implement

	return text;
}

Common::String AGSEngine::replaceMacroTokens(const Common::String &text) {
	// FIXME: implement

	return text;
}

// Multiplies up the number of pixels depending on the current
// resolution, to give a relatively fixed size at any game res
uint AGSEngine::getFixedPixelSize(uint pixels) {
	return pixels * _graphics->_screenResolutionMultiplier;
}

int AGSEngine::convertToLowRes(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? (coord / _graphics->_screenResolutionMultiplier) : coord);
}

int AGSEngine::convertBackToHighRes(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? (coord * _graphics->_screenResolutionMultiplier) : coord);
}

int AGSEngine::multiplyUpCoordinate(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? coord : (coord * _graphics->_screenResolutionMultiplier));
}

void AGSEngine::multiplyUpCoordinates(int32 &x, int32 &y) {
	if (getGameOption(OPT_NATIVECOORDINATES))
		return;

	x *= _graphics->_screenResolutionMultiplier;
	y *= _graphics->_screenResolutionMultiplier;
}

void AGSEngine::multiplyUpCoordinatesRoundUp(int32 &x, int32 &y) {
	if (getGameOption(OPT_NATIVECOORDINATES))
		return;

	x *= _graphics->_screenResolutionMultiplier;
	x += (_graphics->_screenResolutionMultiplier - 1);
	y *= _graphics->_screenResolutionMultiplier;
	y += (_graphics->_screenResolutionMultiplier - 1);
}

int AGSEngine::divideDownCoordinate(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? coord : (coord / _graphics->_screenResolutionMultiplier));
}

int AGSEngine::divideDownCoordinateRoundUp(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? coord : (coord / _graphics->_screenResolutionMultiplier + _graphics->_screenResolutionMultiplier - 1));
}

// don't return until the provided blocking condition is satisfied
// this is similar to 'do_main_cycle' in original.
void AGSEngine::blockUntil(BlockUntilType untilType, uint untilId) {
	debug(2, "blocking (type %d, %d)", untilType, untilId);

	endSkippingUntilCharStops();

	// save old state (to cope with nested calls)
	BlockUntilType oldType = _blockingUntil;
	uint oldId = _blockingUntilId;

	// main_loop_until:
	_state->_disabledUserInterface++;
	invalidateGUI();
	// only update the mouse cursor if it's speech, or if it hasn't been specifically changed first
	if (_cursorMode != CURS_WAIT)
		if ((_graphics->getCurrentCursor() == _cursorMode) || (untilType == kUntilNoOverlay))
			_graphics->setMouseCursor(CURS_WAIT);

	_blockingUntil = untilType;
	_blockingUntilId = untilId;

	while (mainGameLoop() && !shouldQuit()) { }

	_blockingUntil = oldType;
	_blockingUntilId = oldId;

	debug(2, "done blocking (type %d, %d)", untilType, untilId);
}

// 'wait_loop_still_valid' in original
BlockUntilType AGSEngine::checkBlockingUntil() {
	if (_blockingUntil == kUntilNothing)
		error("checkBlockingUntil called, but game wasn't blocking");

	switch (_blockingUntil) {
	case kUntilNoOverlay:
		error("checkBlockingUntil unfinished"); // FIXME
		break;
	case kUntilMessageDone:
		error("checkBlockingUntil unfinished"); // FIXME
		break;
	case kUntilWaitDone:
		if (_state->_waitCounter == 0)
			return kUntilNothing;
		break;
	case kUntilCharAnimDone:
		error("checkBlockingUntil unfinished"); // FIXME
		break;
	case kUntilCharWalkDone:
		error("checkBlockingUntil unfinished"); // FIXME
		break;
	case kUntilObjMoveDone:
		error("checkBlockingUntil unfinished"); // FIXME
		break;
	case kUntilObjCycleDone:
		if (_currentRoom->_objects[_blockingUntilId]->_cycling == 0)
			return kUntilNothing;
		break;
	default:
		error("checkBlockingUntil: invalid blocking type %d", _blockingUntil);
	}

	return _blockingUntil;
}

void AGSEngine::skipUntilCharacterStops(uint charId) {
	// FIXME
}

void AGSEngine::endSkippingUntilCharStops() {
	if (_state->_skipUntilCharStops == (uint)-1)
		return;

	stopFastForwarding();
	_state->_skipUntilCharStops = (uint)-1;
}

// 'initialize_skippable_cutscene' in original
void AGSEngine::startSkippableCutscene() {
	_state->_endCutsceneMusic = (uint)-1;
}

void AGSEngine::startSkippingCutscene() {
	_state->_fastForward = 1;

	if (_poppedInterface != (uint)-1)
		removePopupInterface(_poppedInterface);

	// FIXME: remove text message, if any
}

void AGSEngine::stopFastForwarding() {
	_state->_fastForward = 0;
	// FIXME: setpal


	/* FIXME if (_state->_endCutsceneMusic != (uint)-1)
		newMusic(_state->_endCutsceneMusic); */
	// FIXME: restore actual volume of sounds
	// FIXME: updateMusicVolume();
}

#define CHOSE_TEXTPARSER -3053
#define SAYCHOSEN_USEFLAG 1
#define SAYCHOSEN_YES 2
#define SAYCHOSEN_NO  3

#define RUN_DIALOG_STAY          -1
#define RUN_DIALOG_STOP_DIALOG   -2
#define RUN_DIALOG_GOTO_PREVIOUS -4

void AGSEngine::runDialogId(uint dialogId) {
	if (dialogId >= _gameFile->_dialogs.size())
		error("runDialogId: dialog %d invalid (only have %d dialogs)", dialogId, _gameFile->_dialogs.size());

	// FIXME: can_run_delayed_command

	if (_state->_stopDialogAtEnd != DIALOG_NONE) {
		if (_state->_stopDialogAtEnd != DIALOG_RUNNING)
			error("runDialogId: already-running dialog was in state %d", _state->_stopDialogAtEnd);
		_state->_stopDialogAtEnd = DIALOG_NEWTOPIC + dialogId;
		return;
	}

	if (_runningScripts.size())
		_runningScripts.back().queueAction(kPSARunDialog, dialogId, "RunDialog");
	else
		doConversation(dialogId);
}

int AGSEngine::showDialogOptions(uint dialogId, uint sayChosenOption) {
	if (dialogId >= _gameFile->_dialogs.size())
		error("showDialogOptions: dialog %d invalid (only have %d dialogs)", dialogId, _gameFile->_dialogs.size());

	// FIXME: can_run_delayed_command

	// FIXME

	while (!shouldQuit()) {
		if ((bool)getGameOption(OPT_RUNGAMEDLGOPTS)) {
			_state->_disabledUserInterface++;
			// FIXME: pass alternative display info
			tickGame();
			_state->_disabledUserInterface--;
		} else {
			_state->_gameStep++;
			// FIXME: rendering/polling stuff
		}

		// FIXME
	}

	// FIXME
	return 0;
}

void AGSEngine::doConversation(uint dialogId) {
	endSkippingUntilCharStops();

	// AGS 2.x always makes the mouse cursor visible when displaying a dialog.
	if (getGameFileVersion() <= kAGSVer272)
		_state->_mouseCursorHidden = 0;

	Common::Stack<uint> previousTopics;
	uint currDialogId = dialogId;
	DialogTopic &currDialogTopic = _gameFile->_dialogs[dialogId];

	// run the dialog startup script
	int result = runDialogScript(currDialogTopic, currDialogId, currDialogTopic._startupEntryPoint, 0);
	if ((result == RUN_DIALOG_STOP_DIALOG) || (result == RUN_DIALOG_GOTO_PREVIOUS)) {
		// 'stop' or 'goto-previous' from startup script
		// FIXME: remove_screen_overlay(OVER_COMPLETE);
		_state->_inConversation--;
		return;
	} else if (result >= 0) {
		currDialogId = (uint)result;
	}

	while (result != RUN_DIALOG_STOP_DIALOG && !shouldQuit()) {
		if (currDialogId >= _gameFile->_dialogs.size())
			error("doConversation: new dialog was too high (%d), only have %d dialogs",
				currDialogId, _gameFile->_dialogs.size());

		currDialogTopic = _gameFile->_dialogs[dialogId];

		if (currDialogId != dialogId) {
			// dialog topic changed, so play the startup script for the new topic
			dialogId = currDialogId;
			result = runDialogScript(currDialogTopic, currDialogId, currDialogTopic._startupEntryPoint, 0);
		} else {
			int chose = showDialogOptions(currDialogId, SAYCHOSEN_USEFLAG);

			if (chose == CHOSE_TEXTPARSER) {
				_saidSpeechLine = false;
				result = runDialogRequest(currDialogId);
				if (_saidSpeechLine) {
					// FIXME: original futzes with the screen for close-up face here
				}
			} else {
				result = runDialogScript(currDialogTopic, currDialogId, currDialogTopic._options[chose]._entryPoint, chose + 1);
			}
		}

		if (result == RUN_DIALOG_GOTO_PREVIOUS) {
			if (previousTopics.empty()) {
				// goto-previous on first topic -- end dialog
				result = RUN_DIALOG_STOP_DIALOG;
			} else {
				result = (int)previousTopics.pop();
			}
		}
		if (result >= 0) {
			// another topic change
			previousTopics.push(currDialogId);
			currDialogId = (uint)result;
		}
	}
}

// TODO: move this into DialogTopic itself?
static void getDialogScriptParameters(DialogTopic &topic, uint &pos, uint16 *param1, uint16 *param2 = NULL) {
	const Common::Array<byte> &code = topic._code;
	if (pos + 3 > code.size())
		error("getDialogScriptParameters: %d is off end of script (size %d)", pos, code.size());
	pos++;
	*param1 = READ_LE_UINT16(&code[pos]);
	pos += 2;
	if (param2) {
		if (pos + 2 > code.size())
			error("getDialogScriptParameters: %d is off end of script (size %d)", pos, code.size());
		*param2 = READ_LE_UINT16(&code[pos]);
		pos += 2;
	}
}

int AGSEngine::runDialogScript(DialogTopic &topic, uint dialogId, uint offset, uint optionId) {
	_saidSpeechLine = false;

	int result = RUN_DIALOG_STAY;
	if (_dialogScriptsScript) {
		Common::Array<uint32> params;
		params.push_back(optionId);
		runTextScript(_dialogScriptsScript, Common::String::format("_run_dialog%d", dialogId), params);
		result = (int)_dialogScriptsScript->getReturnValue();
	} else {
		// old-style dialog script
		if (offset == (uint)-1)
			return result;

		uint pos = offset;
		bool scriptRunning = true;
		uint16 param1, param2;

		while (scriptRunning && !shouldQuit()) {
			if (pos + 2 > topic._code.size())
				error("runDialogScript: %d is off end of script (size %d)", pos, topic._code.size());
			byte opcode = topic._code[pos];
			switch (opcode) {
			case DCMD_SAY:
				getDialogScriptParameters(topic, pos, &param1, &param2);

				{
				if (param2 > _gameFile->_speechLines.size())
					error("DCMD_SAY: speech line %d is too high (only have %d)", param2, _gameFile->_speechLines.size());
				const Common::String &speechLine = _gameFile->_speechLines[param2];

				if (param1 == DCHAR_PLAYER)
					param1 = _gameFile->_playerChar;

				if (param1 == DCHAR_NARRATOR)
					display(getTranslation(speechLine));
				else
					displaySpeech(getTranslation(speechLine), param1);
				}

				_saidSpeechLine = true;
				break;
			case DCMD_OPTOFF:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				// FIXME
				error("DCMD_OPTOFF unimplemented");
				break;
			case DCMD_OPTON:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				// FIXME
				error("DCMD_OPTON unimplemented");
				break;
			case DCMD_RETURN:
				scriptRunning = false;
				break;
			case DCMD_STOPDIALOG:
				result = RUN_DIALOG_STOP_DIALOG;
				scriptRunning = false;
				break;
			case DCMD_OPTOFFFOREVER:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				// FIXME
				error("DCMD_OPTOFFFOREVER unimplemented");
				break;
			case DCMD_RUNTEXTSCRIPT:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				// FIXME
				error("DCMD_RUNTEXTSCRIPT unimplemented");
				break;
			case DCMD_GOTODIALOG:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				result = param1;
				scriptRunning = false;
				break;
			case DCMD_PLAYSOUND:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				_audio->playSound(param1);
				break;
			case DCMD_ADDINV:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				// FIXME
				error("DCMD_ADDINV unimplemented");
				break;
			case DCMD_SETSPCHVIEW:
				getDialogScriptParameters(topic, pos, &param1, &param2);
				// FIXME
				error("DCMD_SETSPCHVIEW unimplemented");
				break;
			case DCMD_NEWROOM:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				// FIXME
				error("DCMD_NEWROOM unimplemented");
				break;
			case DCMD_SETGLOBALINT:
				getDialogScriptParameters(topic, pos, &param1, &param2);
				// FIXME
				error("DCMD_SETGLOBALINT unimplemented");
				break;
			case DCMD_GIVESCORE:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				// FIXME
				error("DCMD_GIVESCORE unimplemented");
				break;
			case DCMD_GOTOPREVIOUS:
				result = RUN_DIALOG_GOTO_PREVIOUS;
				scriptRunning = false;
				break;
			case DCMD_LOSEINV:
				getDialogScriptParameters(topic, pos, &param1, NULL);
				// FIXME
				error("DCMD_LOSEINV unimplemented");
				break;
			case DCMD_ENDSCRIPT:
				result = RUN_DIALOG_STOP_DIALOG;
				scriptRunning = false;
				break;
			default:
				error("runDialogScript: unknown opcode %d", opcode);
			}
		}
	}

	// if there was a room change, stop the dialog
	if (_inNewRoomState != kNewRoomStateNone)
		return RUN_DIALOG_STOP_DIALOG;

	if (_saidSpeechLine) {
		// FIXME: original futzes with the screen for close-up face here
		// (see doConversation also)
	}

	return result;
}

int AGSEngine::runDialogRequest(uint request) {
	_state->_stopDialogAtEnd = DIALOG_RUNNING;

	Common::Array<uint32> params;
	params.push_back(request);
	runScriptFunction(_gameScript, "dialog_request", params);

	if (_state->_stopDialogAtEnd == DIALOG_STOP) {
		_state->_stopDialogAtEnd = DIALOG_NONE;
		return RUN_DIALOG_STOP_DIALOG;
	} else if (_state->_stopDialogAtEnd >= DIALOG_NEWTOPIC) {
		uint topicId = (uint)_state->_stopDialogAtEnd - DIALOG_NEWTOPIC;
		_state->_stopDialogAtEnd = DIALOG_NONE;
		return topicId;
	} else if (_state->_stopDialogAtEnd >= DIALOG_NEWROOM) {
		uint roomId = (uint)_state->_stopDialogAtEnd - DIALOG_NEWROOM;
		_state->_stopDialogAtEnd = DIALOG_NONE;
		scheduleNewRoom(roomId);
		error("runDialogRequest doesn't do DIALOG_NEWROOM yet");
		return RUN_DIALOG_STOP_DIALOG;
	} else {
		_state->_stopDialogAtEnd = DIALOG_NONE;
		return RUN_DIALOG_STAY;
	}
}

void AGSEngine::displayAtY(int y, const Common::String &text) {
	// FIXME: sanity-check y parameter

	// Display("") ... a bit of a stupid thing to do, so ignore it
	if (text.empty())
		return;

	if (y > 0)
		y = multiplyUpCoordinate(y);

	if (getGameOption(OPT_ALWAYSSPCH)) {
		displaySpeechAt(-1, (y > 0) ? divideDownCoordinate(y) : y, -1, _gameFile->_playerChar, text);
	} else {
		// Normal "Display" in text box

		// FIXME: some dirty stuff here

		displayAt(-1, y, _graphics->_width / 2 + _graphics->_width / 4, getTranslation(text), 1);
	}
}

void AGSEngine::displayAt(int x, int y, int width, const Common::String &text, int blocking, int asSpeech, bool isThought,
	int allowShrink, bool overlayPositionFixed) {

	bool needStopSpeech = false;

	uint usingFont = _state->_normalFont;
	if (asSpeech)
		usingFont = _state->_speechFont;

	// FIXME: auto-speech

	displayMain(x, y, width, text, blocking, usingFont, asSpeech, isThought, allowShrink, overlayPositionFixed);

	/* FIXME: if (needStopSpeech)
		stopSpeech(); */
}

void AGSEngine::displayMain(int x, int y, int width, const Common::String &text, int blocking, int usingFont,
	int asSpeech, bool isThought, int allowShrink, bool overlayPositionFixed) {
	warning("displayMain '%s' unimplemented", text.c_str());
}

void AGSEngine::displaySpeech(const Common::String &text, uint charId, int x, int y, int width, bool isThought) {
	warning("displaySpeech '%s' unimplemented", text.c_str());
}

void AGSEngine::displaySpeechAt(int x, int y, int width, uint charId, const Common::String &text) {
	multiplyUpCoordinates(x, y);
	width = multiplyUpCoordinate(width);
	displaySpeech(getTranslation(text), charId, x, y, width);
}

bool AGSEngine::runScriptFunction(ccInstance *instance, const Common::String &name, const Common::Array<uint32> &params) {
	if (!prepareTextScript(instance, name))
		return false;

	instance->call(name, params);

	// non-zero if failed, except 100 if aborted
	// TODO: original checked -2 but we don't use that, right?
	// FIXME: this is fail
	/*uint32 result = instance->getReturnValue();
	if (result != 0 && result != 100)
		error("runScriptFunction: script '%s' returned error %d", name.c_str(), result);*/

	postScriptCleanup();
	// FIXME: sabotage any running scripts in the event of restored game

	return true;
}

bool AGSEngine::prepareTextScript(ccInstance *instance, const Common::String &name) {
	if (!instance->exportsSymbol(name))
		return false;

	if (instance->isRunning()) {
		warning("script was already running, when trying to run '%s'", name.c_str());
		return false;
	}

	// FIXME: original code has code which forks an instance if it was running
	// (but it's unreachable due to the check above..)

	_runningScripts.push_back(ExecutingScript(instance));
	// FIXME: updateScriptMouseCoords();

	return true;
}

void AGSEngine::postScriptCleanup() {
	ExecutingScript wasRunning = _runningScripts.back();
	_runningScripts.pop_back();

	// TODO: original engine checks forked here (but that is never used)

	uint oldRoomNumber = _displayedRoom;

	for (uint i = 0; i < wasRunning._pendingActions.size(); ++i) {
		PostScriptAction &action = wasRunning._pendingActions[i];

		switch (action.type) {
		case kPSANewRoom:
			if (_runningScripts.empty()) {
				newRoom(action.data);
				// don't allow any pending room scripts from the old room
				// to be executed
				return;
			}
			// queue it on the next script in the call stack
			_runningScripts.back().queueAction(kPSANewRoom, action.data, "NewRoom");
			break;
		case kPSAInvScreen:
			// FIXME
			break;
		case kPSARestoreGame:
			// FIXME
			break;
		case kPSARestoreGameDialog:
			// FIXME
			break;
		case kPSARunAGSGame:
			// FIXME
			break;
		case kPSARunDialog:
			doConversation(action.data);
			break;
		case kPSARestartGame:
			// FIXME
			break;
		case kPSASaveGame:
			// FIXME
			break;
		case kPSASaveGameDialog:
			// FIXME
			break;
		default:
			error("postScriptCleanup: unknown action %d", action.type);
		}

		// if the room changed in a conversation, for example, abort
		if (oldRoomNumber != _displayedRoom)
			return;
	}

	for (uint i = 0; i < wasRunning._pendingScripts.size(); ++i) {
		PendingScript &script = wasRunning._pendingScripts[i];

		// FIXME

		// if they've changed rooms, cancel any further pending scripts
		if (oldRoomNumber != _displayedRoom /* FIXME: || _loadNewGame */)
			break;
	}
}

ExecutingScript::ExecutingScript(ccInstance *instance) : _instance(instance) {
}

void ExecutingScript::queueAction(PostScriptActionType type, uint data, const Common::String &name) {
	PostScriptAction a;
	a.type = type;
	a.data = data;
	a.name = name;
	_pendingActions.push_back(a);
}

void ExecutingScript::queueScript(const Common::String &name, uint p1, uint p2) {
	PendingScript i;
	i.name = name;
	i.p1 = p1;
	i.p2 = p2;
	_pendingScripts.push_back(i);
}

} // End of namespace AGS
