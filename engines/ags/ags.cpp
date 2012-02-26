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
#include "common/events.h"

#include "engines/advancedDetector.h"
#include "engines/util.h"

// Audio
#include "audio/mixer.h"

#include "graphics/palette.h"

// AGS subsystems
#include "ags/ags.h"
#include "ags/audio.h"
#include "ags/constants.h"
#include "ags/gamefile.h"
#include "ags/gamestate.h"
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
	_width(0), _height(0), _resourceMan(0), _forceLetterbox(false), _needsUpdate(true), _guiNeedsUpdate(true),
	_mouseFrame(0), _mouseDelay(0), _startingRoom(0xffffffff), _displayedRoom(0xffffffff),
	_gameScript(NULL), _gameScriptFork(NULL), _dialogScriptsScript(NULL), _roomScript(NULL), _roomScriptFork(NULL),
	_currentRoom(NULL), _currentCursor(0xffffffff), _framesPerSecond(40), _lastFrameTime(0),
	_inNewRoomState(kNewRoomStateNone), _newRoomStateWas(kNewRoomStateNone), _inEntersScreenCounter(0),
	_blockingUntil(kUntilNothing), _insideProcessEvent(false) {

	_rnd = new Common::RandomSource("ags");
	_scriptState = new GlobalScriptState();
	_state = new GameState(this);
}

AGSEngine::~AGSEngine() {
	delete _currentRoom;

	for (uint i = 0; i < _characters.size(); ++i)
		delete _characters[i];

	for (uint i = 0; i < _scriptModules.size(); ++i)
		delete _scriptModules[i];
	for (uint i = 0; i < _scriptModuleForks.size(); ++i)
		delete _scriptModuleForks[i];
	delete _gameScript;
	delete _gameScriptFork;
	delete _dialogScriptsScript;
	delete _roomScript;

	delete _sprites;

	delete _gameFile;
	delete _resourceMan;

	delete _scriptState;
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
		_system->delayMillis((1000 / _framesPerSecond) + time - _lastFrameTime);
	_lastFrameTime = _system->getMillis();

	// FIXME

	if (_inNewRoomState == 0) {
		// FIXME: repExecAlways
		queueGameEvent(kEventTextScript, kTextScriptRepeatedlyExecute);
		queueGameEvent(kEventRunEventBlock, kEventBlockRoom, 0, kRoomEventTick);
	}
	checkNewRoom();

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

	for (uint i = 0; i < _gameFile->_scriptModules.size(); ++i) {
		// create an instance for the script module
		_scriptModules.push_back(new ccInstance(this, _gameFile->_scriptModules[i], true));
		// fork an instance for repeatedly_execute_always to run in
		_scriptModuleForks.push_back(new ccInstance(this, _gameFile->_scriptModules[i], true, _scriptModules[i]));
	}

	// create an instance for the game script
	_gameScript = new ccInstance(this, _gameFile->_gameScript, true);
	// fork an instance for repeatedly_execute_always to run in
	_gameScriptFork = new ccInstance(this, _gameFile->_gameScript, true, _gameScript);

	if (_gameFile->_dialogScriptsScript) {
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

	const byte *roomPal = _currentRoom->_backgroundScenes[0]._palette;
	for (uint i = 0; i < 256; ++i) {
		if (_gameFile->_paletteUses[i] == PAL_BACKGROUND) {
			_palette[i * 3 + 0] = roomPal[i * 4 + 0];
			_palette[i * 3 + 1] = roomPal[i * 4 + 0];
			_palette[i * 3 + 2] = roomPal[i * 4 + 0];
		} else {
			// FIXME: patch room palette
		}
	}

	if (_gameFile->_defaultResolution > 2 && !getGameOption(OPT_NATIVECOORDINATES))
		_currentRoom->convertCoordinatesToLowRes();

	// FIXME

	_inNewRoomState = kNewRoomStateNew;

	// FIXME

	_inNewRoomState = kNewRoomStateFirstTime;

	// FIXME

	_scriptState->addSystemObjectImport("object", new ScriptObjectArray<RoomObject>(_currentRoom->_objects, 8), true);

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
			// FIXME: check inside_script
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
		// FIXME: 3.x scripts: InteractionScripts *scripts = NULL;

		switch (event.data1) {
		case kEventBlockHotspot:
			debug(7, "running hotspot interaction: event %d", event.data3);
			_eventBlockBaseName = "hotspot%d";
			_eventBlockId = event.data2;

			// FIXME: hotspotScripts (3.x)
			interaction = _currentRoom->_hotspots[event.data2]._interaction;
			break;
		case kEventBlockRoom:
			debug(7, "running room interaction: event %d", event.data3);
			_eventBlockBaseName = "room";

			if (event.data3 == kRoomEventEntersScreen)
				_inEntersScreenCounter++;

			// FIXME: roomScripts (3.x)
			interaction = _currentRoom->_interaction;
			break;
		default:
			error("processGameEvent: unknown event block type %d", event.data1);
		}

		/*if (scripts) {
			// FIXME: scripts (3.x)
		} else */ if (interaction) {
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

// returns true if the NewInteraction has been invalidated (e.g. a room change occurred)
bool AGSEngine::runInteractionEvent(struct NewInteraction *interaction, uint eventId, uint checkFallback, bool isInventory) {
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
				// FIXME
			} else {
				// FIXME: bounds check?
				runTextScript(_roomScript, makeTextScriptName(_eventBlockBaseName, _eventBlockId, commands[i]._args[0]._val));
				// FIXME
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

Graphics::PixelFormat AGSEngine::getPixelFormat() const {
	switch (_gameFile->_colorDepth) {
	case 1:
		// 8bpp
		return Graphics::PixelFormat::createFormatCLUT8();
	case 2:
		// 16bpp: 565
		return Graphics::PixelFormat(2, 5, 6, 5, 0, 11, 5, 0, 0);
	default:
		// 24bpp: RGB888
		return Graphics::PixelFormat(3, 8, 8, 8, 0, 16, 8, 0, 0);
	}
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
	for (uint i = 0; i < 256; ++i) {
		if (_gameFile->_paletteUses[i] != PAL_BACKGROUND) {
			_palette[i * 3 + 0] = _gameFile->_defaultPalette[i * 3 + 0];
			_palette[i * 3 + 1] = _gameFile->_defaultPalette[i * 3 + 1];
			_palette[i * 3 + 2] = _gameFile->_defaultPalette[i * 3 + 2];
		}
	}
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

	if (!getScreenSize())
		return false;

	if (!initGraphics())
		return false;

	syncSoundSettings();

	_engineStartTime = g_system->getMillis();

	return true;
}

bool AGSEngine::getScreenSize() {
	_baseWidth = 320;
	_baseHeight = 200;
	_textMultiply = 2;

	switch (_gameFile->_defaultResolution) {
	case 0:
		_textMultiply = 1;
		break;
	case 1:
	case 2:
		_width = 640;
		_height = 480;
		break;
	case 3:
	case 4:
		_width = 640;
		_height = 400;
		break;
	case 5:
		_baseWidth = 400;
		_baseHeight = 300;
		break;
	default:
		_baseWidth = 512;
		_baseHeight = 384;
		break;
	}

	if (_gameFile->_defaultResolution >= 5) {
		_width = _baseWidth * 2;
		_height = _baseHeight * 2;
		_gameFile->_options[OPT_LETTERBOX] = 0;
		_forceLetterbox = false;
	}

	_screenResolutionMultiplier = _width / _baseWidth;

	return true;
}

bool AGSEngine::initGraphics() {
	if ((_width == 0) || (_height == 0))
		return false;

	::initGraphics(_width, _height, 0);

	return true;
}

void AGSEngine::pauseEngineIntern(bool pause) {
	_mixer->pauseAll(pause);
}

// reset the visible cursor to the one for the current mode
void AGSEngine::setDefaultCursor() {
	setMouseCursor(_cursorMode);
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

void AGSEngine::setMouseCursor(uint32 cursor) {
	assert(cursor < _gameFile->_cursors.size());

	MouseCursor &cursorInfo = _gameFile->_cursors[cursor];

	setCursorGraphic(cursorInfo._pic);
	/* FIXME if (_dottedMouseCursor) {
		delete _dottedMouseCursor;
		_dottedMouseCursor = NULL;
	} */

	if ((cursor == MODE_USE) && cursorInfo._pic && (_gameFile->_hotDot || _gameFile->_invHotDotSprite)) {
		// create a copy of cursor with the hotspot dot onto it, if needed
		/* FIXME: duplicate active mouse cursor onto _dottedMouseCursor */

		if (_gameFile->_invHotDotSprite) {
			// FIXME: draw invHotDotSprite centered on hotspot
		} else {
			// FIXME: draw pixel
			if (_gameFile->_hotDotOuter) {
				// FIXME: draw pixels
			}
		}

		// FIXME: replace active mouse cursor with _dottedMouseCursor
		updateCachedMouseCursor();
	}

	mouseSetHotspot(cursorInfo._hotspotX, cursorInfo._hotspotY);

	if (cursor != _currentCursor) {
		_currentCursor = cursor;
		_mouseFrame = 0;
		_mouseDelay = 0;
	}
}

void AGSEngine::setCursorGraphic(uint32 spriteId) {
	_cursorSprite = _sprites->getSprite(spriteId);

	if (!spriteId || !_cursorSprite) {
		// FIXME
	}

	_alphaBlendCursor = (bool)(_gameFile->_spriteFlags[spriteId] & SPF_ALPHACHANNEL);

	updateCachedMouseCursor();
}

void AGSEngine::updateCachedMouseCursor() {
	// FIXME: set the mouse cursor
}

void AGSEngine::mouseSetHotspot(uint32 x, uint32 y) {
	// FIXME: set the hotspot!
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
	return pixels * _screenResolutionMultiplier;
}

int AGSEngine::convertToLowRes(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? (coord / _screenResolutionMultiplier) : coord);
}

int AGSEngine::convertBackToHighRes(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? (coord * _screenResolutionMultiplier) : coord);
}

int AGSEngine::multiplyUpCoordinate(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? coord : (coord * _screenResolutionMultiplier));
}

void AGSEngine::multiplyUpCoordinates(int32 &x, int32 &y) {
	if (!getGameOption(OPT_NATIVECOORDINATES))
		return;

	x *= _screenResolutionMultiplier;
	y *= _screenResolutionMultiplier;
}

void AGSEngine::multiplyUpCoordinatesRoundUp(int32 &x, int32 &y) {
	if (!getGameOption(OPT_NATIVECOORDINATES))
		return;

	x *= _screenResolutionMultiplier;
	x += (_screenResolutionMultiplier - 1);
	y *= _screenResolutionMultiplier;
	y += (_screenResolutionMultiplier - 1);
}

int AGSEngine::divideDownCoordinate(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? coord : (coord / _screenResolutionMultiplier));
}

int AGSEngine::divideDownCoordinateRoundUp(int coord) {
	return (getGameOption(OPT_NATIVECOORDINATES) ? coord : (coord / _screenResolutionMultiplier + _screenResolutionMultiplier - 1));
}

void AGSEngine::playSound(uint soundId) {
	// uint priority = 10;

	// FIXME
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
		if ((_currentCursor == _cursorMode) || (untilType == kUntilNoOverlay))
			setMouseCursor(CURS_WAIT);

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

bool AGSEngine::runScriptFunction(ccInstance *instance, const Common::String &name, const Common::Array<uint32> &params) {
	if (!prepareTextScript(instance, name))
		return false;

	instance->call(name, params);

	// FIXME: check return value

	// FIXME: post script cleanup

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

} // End of namespace AGS
