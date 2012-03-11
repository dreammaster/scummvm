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

AGSEngine::AGSEngine(OSystem *syst, const AGSGameDescription *gameDesc) :
	Engine(syst), _gameDescription(gameDesc), _engineStartTime(0), _playTime(0),
	_resourceMan(0), _needsUpdate(true), _guiNeedsUpdate(true),
	_startingRoom(0xffffffff), _displayedRoom(0xffffffff),
	_gameScript(NULL), _gameScriptFork(NULL), _dialogScriptsScript(NULL), _roomScript(NULL), _roomScriptFork(NULL),
	_currentRoom(NULL), _framesPerSecond(40), _lastFrameTime(0),
	_inNewRoomState(kNewRoomStateNone), _newRoomStateWas(kNewRoomStateNone), _inEntersScreenCounter(0),
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

	for (uint i = 0; i < _characters.size(); ++i) {
		assert(_characters[i]->getRefCount() == 1);
		_characters[i]->DecRef();
	}

	delete _scriptState;

	delete _graphics;
	delete _sprites;

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
			_state->_disabledUserInterface++;

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

	if (_inEntersScreenCounter) {
		if (_displayedRoom == _startingRoom)
			error("script tried blocking in the Player Enters Screen event - use After Fadein instead");
		warning("script blocking in Player Enters Screen - use After Fadein instead");
	}

	// FIXME: check no_blocking_functions

	updateEvents();
	if (shouldQuit())
		return;

	// If we're running faster than the target rate, sleep for a bit.
	uint32 time = _system->getMillis();
	if (time < _lastFrameTime + (1000 / _framesPerSecond))
		_system->delayMillis((1000 / _framesPerSecond) - time + _lastFrameTime);
	_lastFrameTime = _system->getMillis();

	// FIXME

	if (_inNewRoomState == 0) {
		// FIXME: repExecAlways
		queueGameEvent(kEventTextScript, kTextScriptRepeatedlyExecute);
		queueGameEvent(kEventRunEventBlock, kEventBlockRoom, 0, kRoomEventTick);
	}
	checkNewRoom();

	// FIXME

	if (!_state->_fastForward) {
		_graphics->draw();
	}

	// FIXME

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
}

void AGSEngine::updateEvents() {
	Common::Event event;

	while (_eventMan->pollEvent(event)) {
		// FIXME
	}
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

void AGSEngine::createGlobalScript() {
	assert(_scriptModules.empty());

	_scriptState->addSystemObjectImport("dialog", new ScriptObjectArray<DialogTopic>(_gameFile->_dialogs, 8));
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

	delete _currentRoom;

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

	_state->_roomWidth = _currentRoom->_width;
	_state->_roomHeight = _currentRoom->_height;
	_state->_animBackgroundSpeed = _currentRoom->_backgroundSceneAnimSpeed;
	_state->_bgAnimDelay = _currentRoom->_backgroundSceneAnimSpeed;

	_graphics->newRoomPalette();

	if (_gameFile->_defaultResolution > 2 && !getGameOption(OPT_NATIVECOORDINATES))
		_currentRoom->convertCoordinatesToLowRes();

	// FIXME

	_inNewRoomState = kNewRoomStateNew;

	// FIXME

	_inNewRoomState = kNewRoomStateFirstTime;

	// FIXME

	_scriptState->addSystemObjectImport("object", new ScriptObjectArray<RoomObject *>(_currentRoom->_objects, 8), true);

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

void AGSEngine::queueGameEvent(GameEventType type, uint data1, uint data2, uint data3) {
	GameEvent ev;
	ev.type = type;
	ev.data1 = data1;
	ev.data2 = data2;
	ev.data3 = data3;
	ev.playerId = _gameFile->_playerChar;
	_queuedGameEvents.push_back(ev);
}

void AGSEngine::runGameEventNow(GameEventType type, uint data1, uint data2, uint data3) {
	GameEvent ev;
	ev.type = type;
	ev.data1 = data1;
	ev.data2 = data2;
	ev.data3 = data3;
	ev.playerId = _gameFile->_playerChar;
	processGameEvent(ev);
}

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
			error("processGameEvent: can't do kEventTextScript yet"); // FIXME
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
		warning("processGameEvent: can't do kEventAfterFadeIn yet"); // FIXME
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
		if (roomWas != _state->_roomChanges)
			break;
	}

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
		if (roomWas != _state->_roomChanges)
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
	_state->init();

	addSystemScripting(this);

	_scriptState->addSystemObjectImport("character", new ScriptObjectArray<Character *>(_characters, 780));
	_scriptState->addSystemObjectImport("player", _characters[_gameFile->_playerChar]);
	for (uint i = 0; i < _characters.size(); ++i) {
		Character *charInfo = _characters[i];
		if (charInfo->_scriptName.empty())
			continue;
		_scriptState->addSystemObjectImport(charInfo->_scriptName, charInfo);
	}
	_scriptState->addSystemObjectImport("gui", new ScriptObjectArray<GUIGroup>(_gameFile->_guiGroups, 8));
	for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
		GUIGroup &group = _gameFile->_guiGroups[i];
		if (group._name.empty())
			continue;
		_scriptState->addSystemObjectImport(group._name, &group);
		for (uint j = 0; j < group._controls.size(); ++j) {
			if (group._controls[j]->_scriptName.empty())
				continue;
			_scriptState->addSystemObjectImport(group._controls[j]->_scriptName, group._controls[j]);
		}
	}
	_scriptState->addSystemObjectImport("inventory", new ScriptObjectArray<InventoryItem>(_gameFile->_invItemInfo, 68));
	for (uint i = 0; i < _gameFile->_invItemInfo.size(); ++i) {
		InventoryItem &invItem = _gameFile->_invItemInfo[i];
		if (invItem._scriptName.empty())
			continue;
		_scriptState->addSystemObjectImport(invItem._scriptName, &invItem);
	}

	// FIXME: load fonts

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

	// Init graphics
	if (!_graphics->initGraphics())
		return false;

	syncSoundSettings();

	_engineStartTime = g_system->getMillis();

	return true;
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
	// FIXME: adjust by modifier
	return _framesPerSecond;
}

void AGSEngine::setGameSpeed(uint32 speed) {
	// FIXME: adjust by modifier
	_framesPerSecond = CLIP<uint32>(speed, 10, 1000);
}

byte AGSEngine::getGameOption(uint index) {
	return _gameFile->_options[index];
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
	if (!getGameOption(OPT_NATIVECOORDINATES))
		return;

	x *= _graphics->_screenResolutionMultiplier;
	y *= _graphics->_screenResolutionMultiplier;
}

void AGSEngine::multiplyUpCoordinatesRoundUp(int32 &x, int32 &y) {
	if (!getGameOption(OPT_NATIVECOORDINATES))
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
}

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
		error("checkBlockingUntil unfinished"); // FIXME
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

void AGSEngine::startSkippableCutscene() {
	_state->_endCutsceneMusic = (uint)-1;
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
	const Common::Array<byte> code = topic._code;
	if (pos + 3 > code.size())
		error("getDialogScriptParameters: %d is off end of script (size %d)", pos, code.size());
	pos++;
	*param1 = READ_LE_UINT16(&code[pos]);
	pos += 2;
	if (param2) {
		if (pos + 2 > code.size())
			error("getDialogScriptParameters: %d is off end of script (size %d)", pos, code.size());
		*param1 = READ_LE_UINT16(&code[pos]);
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

		while (scriptRunning) {
			if (pos + 2 > topic._code.size())
				error("runDialogScript: %d is off end of script (size %d)", pos, topic._code.size());
			byte opcode = topic._code[pos];
			switch (opcode) {
			case DCMD_SAY:
				getDialogScriptParameters(topic, pos, &param1, &param2);
				// FIXME
				error("DCMD_SAY unimplemented");
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
		// FIXME: NewRoom(roomId);
		error("runDialogRequest doesn't do DIALOG_NEWROOM yet");
		return RUN_DIALOG_STOP_DIALOG;
	} else {
		_state->_stopDialogAtEnd = DIALOG_NONE;
		return RUN_DIALOG_STAY;
	}
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

	// FIXME: post script cleanup
	_runningScripts.pop_back();
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
