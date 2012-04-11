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
#include "common/debug-channels.h"
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
	Engine(syst), _gameDescription(gameDesc), _engineStartTime(0), _playTime(0), _pauseGameCounter(0),
	_resourceMan(0), _needsUpdate(true), _guiNeedsUpdate(true), _backgroundNeedsUpdate(false),
	_poppedInterface((uint)-1), _clickWasOnGUI(0), _mouseOnGUI((uint)-1),
	_startingRoom(0xffffffff), _displayedRoom(0xffffffff),
	_gameScript(NULL), _gameScriptFork(NULL), _dialogScriptsScript(NULL), _roomScript(NULL), _roomScriptFork(NULL),
	_scriptPlayerObject(NULL),
	_scriptMouseObject(NULL), _gameStateGlobalsObject(NULL), _saveGameIndexObject(NULL), _scriptSystemObject(NULL),
	_roomObjectState(NULL),
	_eventClaimed(EVENT_NONE),
	_currentRoom(NULL), _framesPerSecond(40), _lastFrameTime(0),
	_inNewRoomState(kNewRoomStateNone), _newRoomStateWas(kNewRoomStateNone), _inEntersScreenCounter(0),
	_leavesScreenRoomId(-1),
	_newRoomPos(0), _newRoomX(SCR_NO_VALUE), _newRoomY(SCR_NO_VALUE),
	_blockingUntil(kUntilNothing), _insideProcessEvent(false),
	_completeOverlayCount(0), _textOverlayCount(0),
	_lastTranslationSourceTextLength((uint)-1), _lipsyncLoopsPerCharacter((uint)-1),
	_lipsyncTextOffset((uint)-1), _saidText(false), _saidSpeechLine(false),
	_faceTalkingOverlayIndex((uint)-1) {

	DebugMan.addDebugChannel(kDebugLevelGame, "Game", "AGS runtime debugging");

	_rnd = new Common::RandomSource("ags");
	_scriptState = new GlobalScriptState();
	_state = new GameState(this);
}

// plugins
// TODO: migrate to something more modular
void initSnowRain(AGSEngine *vm);
void shutdownSnowRain();

AGSEngine::~AGSEngine() {
	shutdownSnowRain();

	delete _roomScriptFork;
	delete _roomScript;
	delete _gameScriptFork;
	delete _gameScript;
	delete _dialogScriptsScript;
	for (uint i = 0; i < _scriptModuleForks.size(); ++i)
		delete _scriptModuleForks[i];
	for (uint i = 0; i < _scriptModules.size(); ++i)
		delete _scriptModules[i];

	delete _currentRoom;
	for (Common::HashMap<uint, Room *>::iterator i = _loadedRooms.begin(); i != _loadedRooms.end(); ++i)
		if (i->_value != _currentRoom)
			delete i->_value;

	// un-export all the global objects
	_scriptState->removeImport("character");
	_scriptState->removeImport("player");
	_scriptState->removeImport("gui");
	_scriptState->removeImport("inventory");
	_scriptState->removeImport("mouse");
	_scriptState->removeImport("game");
	_scriptState->removeImport("gs_globals");
	_scriptState->removeImport("savegameindex");
	_scriptState->removeImport("system");
	_scriptState->removeImport("object");
	_scriptState->removeImport("hotspot");
	_scriptState->removeImport("region");
	_scriptState->removeImport("dialog");
	for (uint i = 0; i < _gameFile->_dialogs.size(); ++i)
		_scriptState->removeImport(_gameFile->_dialogs[i]._name);
	for (uint i = 0; i < _characters.size(); ++i) {
		Character *charInfo = _characters[i];
		_scriptState->removeImport(charInfo->_scriptName);
	}
	for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
		GUIGroup &group = *_gameFile->_guiGroups[i];
		_scriptState->removeImport(group._name);
		for (uint j = 0; j < group._controls.size(); ++j) {
			_scriptState->removeImport(group._controls[j]->_scriptName);
		}
	}
	for (uint i = 0; i < _gameFile->_invItemInfo.size(); ++i) {
		InventoryItem &invItem = _gameFile->_invItemInfo[i];
		_scriptState->removeImport(invItem._scriptName);
	}
	_audio->deregisterScriptObjects();

	delete _scriptState;

	delete _scriptPlayerObject;
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

	for (uint i = 0; i < _characters.size(); ++i) {
		assert(_characters[i]->getRefCount() == 1);
		_characters[i]->DecRef();
	}

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

void AGSEngine::pauseGame() {
	_pauseGameCounter++;
	debugC(kDebugLevelGame, "game paused, pause level now %d", _pauseGameCounter);
}

void AGSEngine::unpauseGame() {
	if (_pauseGameCounter == 0)
		warning("unpauseGame: game isn't paused");
	else
		_pauseGameCounter--;
	debugC(kDebugLevelGame, "game unpaused, pause level now %d", _pauseGameCounter);
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

	// call getLocationName, to force a GUI update if the result has changed since last time
	// (without this, @overhotspot@ often doesn't get updated)
	Common::Point mousePos = _system->getEventManager()->getMousePos();
	mousePos.x = divideDownCoordinate(mousePos.x);
	mousePos.y = divideDownCoordinate(mousePos.y);
	getLocationName(mousePos);

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
			runScriptFunction(_scriptModuleForks[i], "repeatedly_execute_always");
		}
		runScriptFunction(_gameScriptFork, "repeatedly_execute_always");
		runScriptFunction(_roomScriptFork, "repeatedly_execute_always");

		queueGameEvent(kEventTextScript, kTextScriptRepeatedlyExecute);
		queueGameEvent(kEventRunEventBlock, kEventBlockRoom, 0, kRoomEventTick);
	}
	// run this immediately to make sure it gets done before fade-in
	// (player enters screen)
	checkNewRoom();

	if (!(_state->_groundLevelAreasDisabled & GLED_INTERACTION)) {
		// run the hotspot script
		uint hotspotId = _currentRoom->getHotspotAt(_playerChar->_x, _playerChar->_y);
		queueGameEvent(kEventRunEventBlock, kEventBlockHotspot, hotspotId, 0);

		// now check for regions
		uint regionId = _currentRoom->getRegionAt(_playerChar->_x, _playerChar->_y);
		uint oldRoom = _displayedRoom;
		if (regionId != _state->_playerOnRegion) {
			// set the new region global
			uint oldRegionId = _state->_playerOnRegion;
			_state->_playerOnRegion = regionId;

			// run Walks Off and Walks Onto scripts as necessary
			if (oldRegionId)
				runRegionInteraction(oldRegionId, kRegionEventWalksOff);
			if (regionId)
				runRegionInteraction(regionId, kRegionEventWalksOnto);
		}
		// run Stands On script if standing on a region
		if (_state->_playerOnRegion)
			runRegionInteraction(_state->_playerOnRegion, kRegionEventStandsOn);

		// one of the region interactions might have sent us to another room
		if (oldRoom != _displayedRoom)
			checkNewRoom();

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

	_mouseOnGUI = (uint)-1;
	// FIXME: checkDebugKeys();

	// FIXME: check if _inNewRoomState is 0
	updateEvents(checkControls);
	if (shouldQuit())
		return;
	// FIXME: check if inventory action changed room

	if (!isGamePaused())
		updateStuff();

	// FIXME: a whole bunch of update stuff

	if (!_state->_fastForward) {
		if (_backgroundNeedsUpdate) {
			_currentRoom->updateWalkBehinds();
			_backgroundNeedsUpdate = false;
		}
		if (_guiNeedsUpdate) {
			// invalidate all GUIs (e.g. when something used in a macro changed)
			for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i)
				_gameFile->_guiGroups[i]->invalidate();
			_guiNeedsUpdate = false;
		}
		updateViewport(); // FIXME: only in the absence of a complete overlay?
		_graphics->draw();

		// FIXME: hotspot stuff
	}

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

	if (_loopCounter % 5 == 0) {
		_audio->updateAmbientSoundVolume();
		_audio->updateDirectionalSoundVolume();
	}

	// TODO: replay stuff?

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
	uint mouseUp = 0;
	bool mouseMoved = false;

	while (_eventMan->pollEvent(event)) {
		// FIXME: if !checkControls, put these in a queue

		switch (event.type) {
		case Common::EVENT_MOUSEMOVE:
			mouseMoved = true;
			break;
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
			mouseUp = kMouseLeft;
			break;
		case Common::EVENT_RBUTTONUP:
			mouseUp = kMouseRight;
			break;
		// FIXME: the others

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
	bool guisTurnedOffAsDisabled = (getGameOption(OPT_DISABLEOFF) == 3 && false /* FIXME: _allButtonsDisabled */);

	if (mouseMoved && !_state->_disabledUserInterface) {
		// Notify all the GUIs, so they can do hover effects etc.
		// (this was originally a poll() in the drawing code)
		for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
			GUIGroup *group = _gameFile->_guiGroups[i];
			if (guisTurnedOffAsDisabled && group->_popup != POPUP_NOAUTOREM)
				continue;
			group->onMouseMove(mousePos);
		}
	}

	if (!guisTurnedOffAsDisabled) {
		for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
			GUIGroup *group = _gameFile->_guiGroupDrawOrder[i];

			// store the mouseover GUI for later use
			if (group->isMouseOver(mousePos))
				activeGUI = group->_id;

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
			if (_completeOverlayCount)
				continue;

			// no need to do it more than once
			if (_poppedInterface == group->_id)
				continue;

			_graphics->setMouseCursor(CURS_ARROW);
			group->setVisible(true);
			_poppedInterface = group->_id;
			pauseGame();
			break;
		}
	}

	if (_poppedInterface != (uint)-1) {
		// remove the popped interface if the mouse has moved away
		GUIGroup *group = _gameFile->_guiGroups[_poppedInterface];
		if (mousePos.y >= group->_y + (int)group->_height)
			removePopupInterface(_poppedInterface);
	}

	// FIXME: check for mouse drags on GUIs

	// FIXME: keep track of which button, only allow up when it matches last down, don't allow down if already down
	if (mouseUp) {
		GUIGroup *group = _gameFile->_guiGroups[_clickWasOnGUI];
		group->onMouseUp(mousePos);

		for (uint i = 0; i < group->_controls.size(); ++i) {
			GUIControl *control = group->_controls[i];

			if (!control->_activated)
				continue;
			control->_activated = false;

			if (_state->_disabledUserInterface)
				break;

			if (control->isOfType(sotGUIButton) || control->isOfType(sotGUISlider)
				|| control->isOfType(sotGUIListBox)) {
				// call processInterfaceClick via a game event
				queueGameEvent(kEventInterfaceClick, _clickWasOnGUI, i, mouseUp);
			} else if (control->isOfType(sotGUIInvWindow)) {
				GUIInvControl *invControl = (GUIInvControl *)control;
				uint itemId = invControl->getItemAt(mousePos - Common::Point(group->_x, group->_y) -
					Common::Point(control->_x, control->_y));
				if (itemId != (uint)-1) {
					_state->_usedInvOn = itemId;
					if (getGameOption(OPT_HANDLEINVCLICKS)) {
						// let the script handle the click: 5 is left, 6 is right
						queueGameEvent(kEventTextScript, kTextScriptOnMouseClick, mouseUp == kMouseRight ? 6 : 5);
					} else if (mouseUp == kMouseRight) {
						// right-click is always look
						runInventoryInteraction(itemId, MODE_LOOK);
					} else if (_cursorMode == MODE_HAND) {
						// set as active inventory item
						setActiveInventory(itemId);
					} else {
						// run the interaction
						runInventoryInteraction(itemId, _cursorMode);
					}
				}
			} else
				error("clicked on unknown control type");

			if (group->_popup == POPUP_MOUSEY)
				removePopupInterface(_clickWasOnGUI);

			break;
		}

		runOnEvent(GE_GUI_MOUSEUP, _clickWasOnGUI);
	}

	if (buttonClicked) {
		if (_state->_inCutscene == kSkipMouseClick || _state->_inCutscene == kSkipAnyKeyOrMouseClick)
			startSkippingCutscene();

		if (_state->_fastForward) {
			// do nothing
		} else if (_state->_waitCounter > 0 && _state->_keySkipWait > 1) {
			// skip wait
			_state->_waitCounter = -1;
		} else if (_textOverlayCount && (_state->_cantSkipSpeech & SKIP_MOUSECLICK)) {
			removeScreenOverlay(OVER_TEXTMSG);
		} else if (!_state->_disabledUserInterface && activeGUI != -1) {
			debugC(kDebugLevelGame, "mouse down over GUI %d", activeGUI);
			// FIXME: not as it is in original
			_gameFile->_guiGroups[activeGUI]->onMouseDown(mousePos);
			runOnEvent(GE_GUI_MOUSEDOWN, activeGUI);
			_clickWasOnGUI = activeGUI;
		} else if (!_state->_disabledUserInterface) {
			queueGameEvent(kEventTextScript, kTextScriptOnMouseClick, buttonClicked);
		}
	}

	// walking off edges only happens when none of the below are the case:
	// * if walking off edges is disabled
	if (_state->_groundLevelAreasDisabled & GLED_INTERACTION)
		return;
	// * if events have been queued above
	if (numEventsWas != _queuedGameEvents.size())
		return;
	// * if we're moving between rooms, or still in Player Enters Screen
	if (_inNewRoomState != kNewRoomStateNone || _newRoomStateWas != kNewRoomStateNone)
		return;
	// * if the game is paused
	if (isGamePaused())
		return;
	// * if the GUI is disabled (in wait mode)
	if (_state->_disabledUserInterface)
		return;

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

void AGSEngine::processInterfaceClick(uint guiId, uint controlId, uint mouseButtonId) {
	Common::Array<RuntimeValue> params;

	GUIGroup *group = _gameFile->_guiGroups[guiId];

	if (controlId == (uint)-1) {
		// click on GUI background
		params.push_back(group);
		params.push_back(mouseButtonId);
		runTextScript(_gameScript, group->_clickEventHandler);
		return;
	}

	uint reactionType = 0, reactionData = 0;

	GUIControl *control = group->_controls[controlId];
	if (control->isOfType(sotGUIButton)) {
		GUIButton *button = (GUIButton *)control;
		reactionType = button->_leftClick;
		reactionData = button->_leftClickData;
	} else if (control->isOfType(sotGUISlider) || control->isOfType(sotGUITextBox)
		|| control->isOfType(sotGUIListBox)) {
		reactionType = IBACT_SCRIPT;
	} else
		error("processInterfaceClick on unknown control type");

	switch (reactionType) {
	case IBACT_SETMODE:
		setCursorMode(reactionData);
		break;
	case IBACT_SCRIPT:
		if (control->getMaxNumEvents() && !control->_eventHandlers.empty()) {
			const Common::String &handler = control->_eventHandlers[0];
			if (!handler.empty() && _gameScript->exportsSymbol(handler)) {
				params.push_back(control);
				if (control->isOfType(sotGUIButton)) {
					// The original engine does something terribly overcomplicated here,
					// trying to see if there's a ',' in the hard-coded parameter list
					// for the event. We don't, since we know that only Button expects
					// a second parameter (for the Click handler).
					params.push_back(mouseButtonId);
				}
				runTextScript(_gameScript, handler, params);
				break;
			}
		}

		params.push_back(guiId);
		params.push_back(controlId);
		runTextScript(_gameScript, "interface_click", params);
	default:
		break;
	}
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

	Common::Array<uint> sheepIds;
	for (uint i = 0; i < _characters.size(); ++i)
		if (_characters[i]->update())
			sheepIds.push_back(i);

	// FIXME: sheep

	updateOverlayTimers();

	updateSpeech();
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
		_playerChar->_prevRoom = (uint)-1;
	}

	firstRoomInitialization();
}

void AGSEngine::setupPlayerCharacter(uint32 charId) {
	_gameFile->_playerChar = charId;
	_playerChar = _characters[charId];
}

class ScriptPlayerObject : public ScriptObject {
public:
	ScriptPlayerObject(AGSEngine *vm) : _vm(vm) { }
	const char *getObjectTypeName() { return "ScriptPlayerObject"; }

	ScriptObject *getObjectAt(uint32 &offset) {
		return _vm->getPlayerChar();
	}
	uint32 readUint32(uint offset) { return _vm->getPlayerChar()->readUint32(offset); }
	bool writeUint32(uint offset, uint value) { return _vm->getPlayerChar()->writeUint32(offset, value); }
	uint16 readUint16(uint offset) { return _vm->getPlayerChar()->readUint16(offset); }
	bool writeUint16(uint offset, uint16 value) { return _vm->getPlayerChar()->writeUint16(offset, value); }
	byte readByte(uint offset) { return _vm->getPlayerChar()->readByte(offset); }
	bool writeByte(uint offset, byte value) { return _vm->getPlayerChar()->writeByte(offset, value); }

protected:
	AGSEngine *_vm;
};

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
			return _vm->_graphics->_width;
		case 4:
			// height
			return _vm->_graphics->_height;
		case 8:
			// coldepth
			return 32; // FIXME
		case 12:
			// os
			return 2; // eOS_Win, FIXME
		case 16:
			// windowed
			return 0; // FIXME
		case 20:
			// vsync
			return _vm->_graphics->_vsync ? 1 : 0;
		case 24:
			// viewport_width
			return _vm->divideDownCoordinate(_vm->_graphics->_width);
		case 28:
			// viewport_height
			return _vm->divideDownCoordinate(_vm->_graphics->_height);
		default:
			error("ScriptSystemObject::readUint32: offset %d is invalid", offset);
		}
	}
	bool writeUint32(uint offset, uint value) {
		switch (offset) {
		case 20:
			_vm->_graphics->_vsync = value;
			return true;
		default:
			// Scripts really shouldn't be changing any other values.
			return false;
		}
	}

protected:
	AGSEngine *_vm;
};

void AGSEngine::createGlobalScript() {
	assert(_scriptModules.empty());

	_scriptPlayerObject = new ScriptPlayerObject(this);
	_scriptState->addSystemObjectImport("player", _scriptPlayerObject);
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

	bool beenHere = false;
	if (_loadedRooms.contains(id)) {
		// We already have state for this room.
		_currentRoom = _loadedRooms[id];
		_currentRoom->loadFrom(stream);
		beenHere = true;
	} else {
		// This is a brand-new room.
		_currentRoom = new Room(this, stream);
		// FIXME: discard room if we shouldn't keep it
		_loadedRooms[id] = _currentRoom;
	}

	// (note: convertCoordinatesToLowRes call was here, now in Room)

	_state->_roomWidth = _currentRoom->_width;
	_state->_roomHeight = _currentRoom->_height;
	_state->_animBackgroundSpeed = _currentRoom->_backgroundSceneAnimSpeed;
	_state->_bgAnimDelay = _currentRoom->_backgroundSceneAnimSpeed;

	_graphics->newRoomPalette();

	// FIXME

	_inNewRoomState = kNewRoomStateNew;

	// FIXME

	if (!beenHere) {
		// FIXME

		_inNewRoomState = kNewRoomStateFirstTime;
	}

	// FIXME

	_roomObjectState->_objectObject->setArray(&_currentRoom->_objects);
	for (uint i = 0; i < _currentRoom->_objects.size(); ++i) {
		RoomObject *obj = _currentRoom->_objects[i];
		_scriptState->addSystemObjectImport(obj->_scriptName, obj);
	}

	_roomObjectState->_hotspotObject->setArray(&_currentRoom->_hotspots);
	for (uint i = 0; i < _currentRoom->_hotspots.size(); ++i) {
		RoomHotspot &hotspot = _currentRoom->_hotspots[i];
		_scriptState->addSystemObjectImport(hotspot._scriptName, &hotspot);
	}

	_roomObjectState->_regionObject->setArray(&_currentRoom->_regions);

	// FIXME

	if (forChar) {
		// if it's not a Restore Game

		// FIXME: following

		_graphics->_viewportX = 0;
		_graphics->_viewportY = 0;
		forChar->_prevRoom = forChar->_room;
		forChar->_room = id;

		// only stop moving if it's a new room, not a restore game
		for (uint i = 0; i < _characters.size(); ++i)
			_characters[i]->stopMoving();
	}

	// compile_room_script
	_roomScript = new ccInstance(this, _currentRoom->_compiledScript);
	_roomScriptFork = new ccInstance(this, _currentRoom->_compiledScript, false, _roomScript);
	// FIXME: optimization stuff
	// FIXME: global data

	// position character if new position was provided
	if (_newRoomX != SCR_NO_VALUE && forChar) {
		forChar->_x = _newRoomX;
		forChar->_y = _newRoomY;
	}

	_newRoomX = SCR_NO_VALUE;

	// if a destination edge was provided, try positioning character there
	if (_newRoomPos > 0 && forChar) {
		// FIXME: room entry stuff
		warning("room entry unimplemented");

		_newRoomPos = 0;
	}

	// update state according to character position when entering
	_state->_enteredEdge = (uint)-1;
	if (forChar) {
		_state->_enteredAtX = forChar->_x;
		_state->_enteredAtY = forChar->_y;
		if (forChar->_x >= _currentRoom->_boundary.right)
			_state->_enteredEdge = 1;
		else if (forChar->_x <= _currentRoom->_boundary.left)
			_state->_enteredEdge = 0;
		else if (forChar->_y >= _currentRoom->_boundary.bottom)
			_state->_enteredEdge = 2;
		else if (forChar->_y <= _currentRoom->_boundary.top)
			_state->_enteredEdge = 3;
	}

	if (_currentRoom->_options[ST_TUNE] > 0)
		{ } // FIXME: music

	if (forChar) {
		// disable/enable the player character as specified in the room options
		if (!_currentRoom->_options[ST_MANDISABLED]) {
			forChar->_on = 1;
			enableCursorMode(0);
		} else {
			forChar->_on = 0;
			disableCursorMode(0);
			// remember which character we turned off, in case they
			// use SetPlayerChracter within this room (so we re-enable
			// the correct character when leaving the room)
			_state->_temporarilyTurnedOffCharacter = _gameFile->_playerChar;
		}

		// reset the frame/view
		if (!(forChar->_flags & CHF_FIXVIEW)) {
			if (_currentRoom->_options[ST_MANVIEW] != 0)
				forChar->_view = _currentRoom->_options[ST_MANVIEW] - 1;
			else
				forChar->_view = forChar->_defView;
		}
		forChar->_frame = 0; // make him standing
	}

	// FIXME: lots and lots

	_audio->updateMusicVolume();
	updateViewport();

	// FIXME: merge objects

	_state->_gscriptTimer = (uint)-1; // avoid screw-ups with changing screens
	_state->_playerOnRegion = 0;

	// FIXME: drop input

	debugC(kDebugLevelGame, "now in room %d", _displayedRoom);
	// TODO: plugin hook
	invalidateGUI();
}

void AGSEngine::unloadOldRoom() {
	assert(_currentRoom);

	// set the global arrays back to invalid empty versions
	_roomObjectState->_objectObject->setArray(&_roomObjectState->_invalidObjects);
	_roomObjectState->_hotspotObject->setArray(&_roomObjectState->_invalidHotspots);
	_roomObjectState->_regionObject->setArray(&_roomObjectState->_invalidRegions);

	if (!_state->_ambientSoundsPersist)
		for (uint i = 0; i < 8; ++i)
			_audio->stopAmbientSound(i);

	// FIXME: save room data segment
	delete _roomScriptFork;
	_roomScriptFork = NULL;
	delete _roomScript;
	_roomScript = NULL;
	// cancel any pending room events
	_queuedGameEvents.clear();

	_state->_bgFrame = 0;
	_state->_bgFrameLocked = 0;
	_state->_offsetsLocked = 0;

	// FIXME: a lot of unimplemented stuff
	warning("AGSEngine::unloadOldRoom() unimplemented");

	_state->_swapPortraitLastChar = (uint)-1;

	// remove all the exported objects
	for (uint i = 0; i < _currentRoom->_objects.size(); ++i) {
		RoomObject *obj = _currentRoom->_objects[i];
		_scriptState->removeImport(obj->_scriptName);
	}
	for (uint i = 0; i < _currentRoom->_hotspots.size(); ++i) {
		RoomHotspot &hotspot = _currentRoom->_hotspots[i];
		_scriptState->removeImport(hotspot._scriptName);
	}

	if (_state->_temporarilyTurnedOffCharacter != (uint16)-1) {
		_characters[_state->_temporarilyTurnedOffCharacter]->_on = 1;
		_state->_temporarilyTurnedOffCharacter = (uint16)-1;
	}

	// FIXME: discard room if we shouldn't keep it
	_currentRoom->unload();
	_currentRoom = NULL;
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

// 'NewRoom' in original
void AGSEngine::scheduleNewRoom(uint roomId) {
	// FIXME: sanity-check roomId?

	if (_displayedRoom == 0xffffffff) {
		// called from game_start; change the room where the game will start
		_playerChar->_room = roomId;
	}

	debugC(kDebugLevelGame, "Room change requested to room %d", roomId);

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
			_playerChar->stopMoving();
		}
	}
	// in_graph_script was consulted here
}

/** Changes the current room number and loads a new room from disk */
void AGSEngine::newRoom(uint roomId) {
	endSkippingUntilCharStops();

	debugC(kDebugLevelGame, "Room change now to room %d", roomId);

	// we are currently running Leaves Screen scripts
	_leavesScreenRoomId = roomId;

	// player leaves screen event
	_eventBlockBaseName = "room";
	if (_currentRoom->_interaction)
		runInteractionEvent(_currentRoom->_interaction, kRoomEventPlayerLeavesScreen);
	else
		runInteractionScript(&_currentRoom->_interactionScripts, kRoomEventPlayerLeavesScreen);

	// Run the global OnRoomLeave event
	runOnEvent(GE_LEAVE_ROOM, _displayedRoom);

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

void AGSEngine::setAsPlayerChar(uint charId) {
	if (charId == _gameFile->_playerChar)
		return;

	setupPlayerCharacter(charId);

	debugC(kDebugLevelGame, "'%s' is now player character", _playerChar->_scriptName.c_str());

	// if we're still starting the game, return now
	if (_displayedRoom == 0xffffffff)
		return;

	if (_displayedRoom != _playerChar->_room)
		scheduleNewRoom(_playerChar->_room);
	else
		_state->_playerOnRegion = _currentRoom->getRegionAt(_playerChar->_x, _playerChar->_y);

	// FIXME: is this what's intended?
	if (_playerChar->_activeInv != (uint)-1 && _playerChar->_inventory[_playerChar->_activeInv] < 1)
		_playerChar->_activeInv = (uint)-1;

	if (_cursorMode == MODE_USE) {
		if (_playerChar->_activeInv == (uint)-1)
			setNextCursor();
		else
			setActiveInventory(_playerChar->_activeInv);
	}
}

void AGSEngine::addInventory(uint itemId) {
	if (itemId >= _gameFile->_invItemInfo.size())
		error("addInventory: itemId is too high (only %d items)", _gameFile->_invItemInfo.size());

	_playerChar->addInventory(itemId);

	_state->_invNumOrder = _playerChar->_invOrder.size();
}

void AGSEngine::loseInventory(uint itemId) {
	if (itemId >= _gameFile->_invItemInfo.size())
		error("loseInventory: itemId is too high (only %d items)", _gameFile->_invItemInfo.size());

	_playerChar->loseInventory(itemId);

	_state->_invNumOrder = _playerChar->_invOrder.size();
}

void AGSEngine::setActiveInventory(uint itemId) {
	if (itemId >= _gameFile->_invItemInfo.size())
		error("setActiveInventory: itemId is too high (only %d items)", _gameFile->_invItemInfo.size());

	_playerChar->setActiveInventory(itemId);
}

Common::String AGSEngine::formatString(const Common::String &string, const Common::Array<RuntimeValue> &values) {
	Common::String out;

	uint paramId = 0;
	for (uint i = 0; i < string.size(); ++i) {
		if (string[i] != '%') {
			out += string[i];
			continue;
		}

		++i;

		// handle %%
		if (string[i] == '%') {
			out += string[i];
			continue;
		}

		uint n = i;
		while (n < string.size()) {
			char c = string[n];
			if (c == 'd' || c == 'f' || c == 'c' || c == 's' || c == 'x' || c == 'X')
				break;
			++n;
		}
		if (n++ == string.size()) {
			// something unsupported, so just write it literally
			--i;
			out += string[i];
			continue;
		}

		if (paramId >= values.size())
			error("formatString: ran out of parameters for string '%s' (got %d, needed #%d)",
				string.c_str(), values.size(), paramId);
		const RuntimeValue &value = values[paramId++];

		// copy the format specifier and skip it
		Common::String formatSpecifier(string.c_str() + i - 1, n - i + 1);
		i = n;

		char formatType = formatSpecifier[formatSpecifier.size() - 1];
		switch (formatType) {
		case 'd':
		case 'x':
		case 'X':
		case 'c':
			if (value._type != rvtInteger)
				error("formatString: expected integer for parameter #%d for string '%s'",
					paramId - 1, string.c_str());
			out += Common::String::format(formatSpecifier.c_str(), value._value);
			break;
		case 'f':
			if (value._type != rvtFloat)
				error("formatString: expected float for parameter #%d for string '%s'",
					paramId - 1, string.c_str());
			out += Common::String::format(formatSpecifier.c_str(), value._floatValue);
			break;
		case 's':
			if (value._type != rvtSystemObject || !value._object->isOfType(sotString))
				error("formatString: expected string for parameter #%d for string '%s'",
					paramId - 1, string.c_str());
			out += Common::String::format(formatSpecifier.c_str(),
				((ScriptString *)value._object)->getString().c_str());
			break;
		default:
			error("formatString: internal error (invalid format type '%c')", formatType);
		}
	}

	if (paramId < values.size())
		error("formatString: too many parameters for string '%s' (got %d, used %d)",
			string.c_str(), values.size(), paramId);

	return out;
}

// for CallRoomScript
void AGSEngine::queueCustomRoomScript(uint32 param) {
	assert(!_runningScripts.empty());

	queueOrRunTextScript(_roomScript, "on_call", param);
}

void AGSEngine::runOnEvent(uint32 p1, uint32 p2) {
	queueOrRunTextScript(_gameScript, "on_event", p1, p2);
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

		if ((int)event.data2 > -1000)
			queueOrRunTextScript(_gameScript, textScriptName, event.data2);
		else
			queueOrRunTextScript(_gameScript, textScriptName);

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

			if (event.data3 == kRoomEventEntersScreen) {
				_inEntersScreenCounter++;
				runOnEvent(GE_ENTER_ROOM, _displayedRoom);
			}

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
		processInterfaceClick(event.data1, event.data2, event.data3);
		break;
	case kEventNewRoom:
		scheduleNewRoom(event.data1);
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
bool AGSEngine::runInteractionScript(InteractionScript *scripts, uint eventId, uint checkFallback, bool isInventory, bool checkOnly) {
	if (eventId >= scripts->_eventScriptNames.size() || scripts->_eventScriptNames[eventId].empty()) {
		// no response for this event

		// if there is a fallback, stop now (caller can run that instead)
		if (checkFallback != (uint)-1 && checkFallback < scripts->_eventScriptNames.size()
			&& !scripts->_eventScriptNames[checkFallback].empty())
			return false;

		if (!checkOnly)
			runUnhandledEvent(eventId);
		return false;
	}

	if (checkOnly)
		return true;

	uint roomWas = _state->_roomChanges;

	if (_eventBlockBaseName.contains("character") || _eventBlockBaseName.contains("inventory")) {
		// global script (character or inventory)
		queueOrRunTextScript(_gameScript, scripts->_eventScriptNames[eventId]);
	} else {
		queueOrRunTextScript(_roomScript, scripts->_eventScriptNames[eventId]);
	}

	if (roomWas != _state->_roomChanges)
		return false;

	return true;
}

// run a 2.x-style interaction event
// returns true if the NewInteraction has been invalidated (e.g. a room change occurred)
bool AGSEngine::runInteractionEvent(NewInteraction *interaction, uint eventId, uint checkFallback, bool isInventory, bool checkOnly) {
	if (!interaction->hasResponseFor(eventId)) {
		// no response for this event

		// if there is a fallback, stop now (caller can run that instead)
		if (checkFallback != (uint)-1 && interaction->hasResponseFor(checkFallback))
			return false;

		if (!checkOnly)
			runUnhandledEvent(eventId);

		return false;
	}

	if (checkOnly)
		return true;

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
				queueOrRunTextScript(_gameScript, name);
			} else {
				// room script
				// FIXME: bounds check?
				Common::String name = makeTextScriptName(_eventBlockBaseName, _eventBlockId, commands[i]._args[0]._val);
				queueOrRunTextScript(_roomScript, name);
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
	Common::String name = _eventBlockBaseName;
	name.toLowercase();

	// TODO: change the eventType to named constants
	uint eventType = 0;
	if (name.hasPrefix("hotspot")) {
		eventType = 1;

		// ignore: character stands on hotspot, mouse moves over hotspot, any click
		if (eventId == kHotspotEventStandsOn || eventId == kHotspotEventClickOnHotspot || eventId == kHotspotEventMouseMovesOver)
			return;

		// clicked on hotspot 0?
		if (_eventBlockId == 0)
			eventType = 4;
	} else if (name.hasPrefix("object")) {
		eventType = 2;

		if (eventId == kGeneralEventClick)
			return;
	} else if (name.hasPrefix("character")) {
		eventType = 3;

		if (eventId == kGeneralEventClick)
			return;
	} else if (name.hasPrefix("inventory"))
		eventType = 5;
	else if (name.hasPrefix("region"))
		return; // no unhandled events for regions
	else
		return; // nor rooms etc

	// FIXME: can_run_delayed_command

	queueOrRunTextScript(_gameScript, "unhandled_event", eventType, eventId);
}

void AGSEngine::claimEvent() {
	if (_eventClaimed == EVENT_NONE)
		error("claimEvent: no event to claim");

	_eventClaimed = EVENT_CLAIMED;
}

bool AGSEngine::runClaimableEvent(const Common::String &name, const Common::Array<RuntimeValue> &params) {
	// Run the room script function, and if it is not claimed,
	// then run the main one

	// We need to remember the eventClaimed variable's state, in case
	// this is a nested event
	uint eventClaimedOldValue = _eventClaimed;
	_eventClaimed = EVENT_INPROGRESS;

	if (_roomScript)
		runScriptFunction(_roomScript, name, params);
	if (_eventClaimed == EVENT_CLAIMED) {
		_eventClaimed = eventClaimedOldValue;
		return true;
	}

	for (uint i = 0; i < _scriptModules.size(); ++i) {
		runScriptFunction(_scriptModules[i], name, params);

		if (_eventClaimed == EVENT_CLAIMED) {
			_eventClaimed = eventClaimedOldValue;
			return true;
		}
	}

	_eventClaimed = eventClaimedOldValue;
	return false;
}

bool AGSEngine::runCharacterInteraction(uint charId, uint mode, bool checkOnly) {
	assert(charId < _characters.size());

	uint param = (uint)-1;

	switch (mode) {
	case MODE_LOOK:
		param = 0;
		break;
	case MODE_HAND:
		param = 1;
		break;
	case MODE_TALK:
		param = 2;
		break;
	case MODE_USE:
		param = 3;
		_state->_usedInv = _playerChar->_activeInv;
		break;
	case MODE_PICKUP:
		param = 5;
		break;
	case MODE_CUSTOM1:
		param = 6;
		break;
	case MODE_CUSTOM2:
		param = 7;
		break;
	}

	if (!checkOnly)
		debugC(kDebugLevelGame, "run character interaction: hotspot %d, param %d", charId, param);

	// can't use queueGameEvent, because "this ProcessClick is only executed once in a eventlist"
	Common::String oldBaseName = _eventBlockBaseName;
	uint oldEventBlockId = _eventBlockId;

	_eventBlockBaseName = "character%d";
	_eventBlockId = charId;

	bool ret = false;
	if (!_gameFile->_interactionsChar.empty()) {
		// 2.x
		if (param != (uint)-1)
			ret |= runInteractionEvent(_gameFile->_interactionsChar[charId], param,
				kGeneralEventClick, param == 3, checkOnly);
		ret |= runInteractionEvent(_gameFile->_interactionsChar[charId], kGeneralEventClick, (uint)-1, false, checkOnly);
	} else {
		// 3.x
		if (param != (uint)-1)
			ret |= runInteractionScript(&_gameFile->_charInteractionScripts[charId], param,
				kGeneralEventClick, param == 3, checkOnly);
		ret |= runInteractionScript(&_gameFile->_charInteractionScripts[charId], kGeneralEventClick, (uint)-1, false, checkOnly);
	}

	_eventBlockBaseName = oldBaseName;
	_eventBlockId = oldEventBlockId;

	return ret;

}

bool AGSEngine::runInventoryInteraction(uint itemId, uint mode, bool checkOnly) {
	// original doesn't bother preserving these, so maybe it's pointless
	Common::String oldBaseName = _eventBlockBaseName;
	uint oldEventBlockId = _eventBlockId;

	_eventBlockBaseName = "inventory%d";
	_eventBlockId = itemId;

	uint eventId;
	if (mode == MODE_LOOK)
		eventId = 0;
	else if (mode == MODE_HAND)
		eventId = 1;
	else if (mode == MODE_TALK)
		eventId = 2;
	else if (mode == MODE_USE) {
		_state->_usedInv = _playerChar->_activeInv;
		eventId = 3;
	} else
		eventId = 4;

	if (!checkOnly)
		debugC(kDebugLevelGame, "run inventory interaction: item %d, event %d", itemId, eventId);

	bool ret;
	if (!_gameFile->_interactionsInv.empty()) {
		// 2.x
		ret = runInteractionEvent(_gameFile->_interactionsInv[itemId], eventId, (uint)-1, false, checkOnly);
	} else {
		// 3.x
		ret = runInteractionScript(&_gameFile->_invInteractionScripts[itemId], eventId, (uint)-1, false, checkOnly);
	}

	_eventBlockBaseName = oldBaseName;
	_eventBlockId = oldEventBlockId;

	return ret;
}

bool AGSEngine::runHotspotInteraction(uint hotspotId, uint mode, bool checkOnly) {
	uint param = (uint)-1;

	switch (mode) {
	case MODE_TALK:
		param = 4;
		break;
	case MODE_WALK:
		param = 0;
		break;
	case MODE_LOOK:
		param = 1;
		break;
	case MODE_HAND:
		param = 2;
		break;
	case MODE_PICKUP:
		param = 7;
		break;
	case MODE_CUSTOM1:
		param = 8;
		break;
	case MODE_CUSTOM2:
		param = 9;
		break;
	case MODE_USE:
		param = 3;
		_state->_usedInv = _playerChar->_activeInv;
		break;
	}

	if (_state->_autoUseWalkToPoints && (getGameOption(OPT_WALKONLOOK) || mode != MODE_LOOK)) {
		// If we should implicitly walk to the hotspot, do so.
		if (mode != MODE_WALK && !checkOnly)
			{ } // FIXME: _playerChar->moveToHotspot(hotspotId);
	}

	if (!checkOnly)
		debugC(kDebugLevelGame, "run hotspot interaction: hotspot %d, param %d", hotspotId, param);

	// can't use queueGameEvent, because "this ProcessClick is only executed once in a eventlist"
	Common::String oldBaseName = _eventBlockBaseName;
	uint oldEventBlockId = _eventBlockId;

	_eventBlockBaseName = "hotspot%d";
	_eventBlockId = hotspotId;

	bool ret = false;
	RoomHotspot &hotspot = _currentRoom->_hotspots[hotspotId];
	if (hotspot._interaction) {
		// 2.x
		if (param != (uint)-1)
			ret |= runInteractionEvent(hotspot._interaction, param,
				kHotspotEventClickOnHotspot, param == 3, checkOnly);
		if (!ret)
			ret |= runInteractionEvent(hotspot._interaction, kHotspotEventClickOnHotspot, (uint)-1, false, checkOnly);
	} else {
		// 3.x
		if (param != (uint)-1)
			ret |= runInteractionScript(&hotspot._interactionScripts, param,
				kHotspotEventClickOnHotspot, param == 3, checkOnly);
		ret |= runInteractionScript(&hotspot._interactionScripts, kHotspotEventClickOnHotspot, (uint)-1, false, checkOnly);
	}

	_eventBlockBaseName = oldBaseName;
	_eventBlockId = oldEventBlockId;

	return ret;
}

bool AGSEngine::runObjectInteraction(uint objectId, uint mode, bool checkOnly) {
	uint param = (uint)-1;

	switch (mode) {
	case MODE_LOOK:
		param = 0;
		break;
	case MODE_HAND:
		param = 1;
		break;
	case MODE_TALK:
		param = 2;
		break;
	case MODE_PICKUP:
		param = 5;
		break;
	case MODE_CUSTOM1:
		param = 6;
		break;
	case MODE_CUSTOM2:
		param = 7;
		break;
	case MODE_USE:
		param = 3;
		_state->_usedInv = _playerChar->_activeInv;
		break;
	}

	if (!checkOnly)
		debugC(kDebugLevelGame, "run object interaction: object %d, param %d", objectId, param);

	// original doesn't bother preserving these, so maybe it's pointless
	Common::String oldBaseName = _eventBlockBaseName;
	uint oldEventBlockId = _eventBlockId;

	_eventBlockBaseName = "object%d";
	_eventBlockId = objectId;

	bool ret = false;
	RoomObject *object = _currentRoom->_objects[objectId];
	if (object->_interaction) {
		// 2.x
		if (param != (uint)-1)
			ret |= runInteractionEvent(object->_interaction, param,
				kGeneralEventClick, param == 3, checkOnly);
		if (!ret)
			ret |= runInteractionEvent(object->_interaction, kGeneralEventClick, (uint)-1, checkOnly);
	} else {
		// 3.x
		if (param != (uint)-1)
			ret |= runInteractionScript(&object->_interactionScripts, param,
				kGeneralEventClick, param == 3, checkOnly);
		if (!ret)
			ret |= runInteractionScript(&object->_interactionScripts, kGeneralEventClick, (uint)-1, checkOnly);
	}

	_eventBlockBaseName = oldBaseName;
	_eventBlockId = oldEventBlockId;

	return ret;
}

void AGSEngine::runRegionInteraction(uint regionId, uint mode) {
	assert(regionId < _currentRoom->_regions.size());
	assert(mode == kRegionEventStandsOn || mode == kRegionEventWalksOnto || mode == kRegionEventWalksOff);

	Common::String oldBaseName = _eventBlockBaseName;
	uint oldEventBlockId = _eventBlockId;

	_eventBlockBaseName = "region%d";
	_eventBlockId = regionId;

	RoomRegion &region = _currentRoom->_regions[regionId];
	if (region._interaction) {
		// 2.x
		runInteractionEvent(region._interaction, mode);
	} else {
		// 3.x
		runInteractionScript(&region._interactionScripts, mode);
	}

	_eventBlockBaseName = oldBaseName;
	_eventBlockId = oldEventBlockId;
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

	// init all the inventory controls (now that the screen size is known)
	for (uint i = 0; i < _gameFile->_guiInvControls.size(); ++i) {
		_gameFile->_guiInvControls[i]->resized();
	}

	// FIXME: the rest of init_game_settings (player inv etc)

	adjustSizesForResolution();
	resortGUIs();

	_state->init();

	addSystemScripting(this);
	initSnowRain(this);

	// FIXME: don't leak all these!
	_scriptState->addSystemObjectImport("character", new ScriptObjectArray<Character *>(&_characters, 780, "Character"));
	for (uint i = 0; i < _characters.size(); ++i) {
		Character *charInfo = _characters[i];
		_scriptState->addSystemObjectImport(charInfo->_scriptName, charInfo);
	}
	_scriptState->addSystemObjectImport("gui", new ScriptObjectArray<GUIGroup *>(&_gameFile->_guiGroups, 8, "GUI"));
	for (uint i = 0; i < _gameFile->_guiGroups.size(); ++i) {
		GUIGroup &group = *_gameFile->_guiGroups[i];
		_scriptState->addSystemObjectImport(group._name, &group);
		for (uint j = 0; j < group._controls.size(); ++j) {
			_scriptState->addSystemObjectImport(group._controls[j]->_scriptName, group._controls[j]);
		}
	}
	_scriptState->addSystemObjectImport("inventory", new ScriptObjectArray<InventoryItem>(&_gameFile->_invItemInfo, 68, "InventoryItem"));
	for (uint i = 0; i < _gameFile->_invItemInfo.size(); ++i) {
		InventoryItem &invItem = _gameFile->_invItemInfo[i];
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
			group->_controls[j]->_activated = false;
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
				if (_playerChar->_activeInv != (uint)-1)
					break;
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

void AGSEngine::setNextCursor() {
	setCursorMode(findNextEnabledCursor(_cursorMode + 1));
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
		if (_playerChar->_activeInv == (uint)-1) {
			findNextEnabledCursor(0);
			return;
		}
		updateInvCursor(_playerChar->_activeInv);
	}

	_cursorMode = newMode;
	setDefaultCursor();

	debugC(kDebugLevelGame, "cursor mode set to %d", newMode);
}

void AGSEngine::enableCursorMode(uint cursorId) {
	_gameFile->_cursors[cursorId]._flags &= ~MCF_DISABLED;

	// FIXME: enable GUI buttons

	invalidateGUI();
}

void AGSEngine::disableCursorMode(uint cursorId) {
	_gameFile->_cursors[cursorId]._flags |= MCF_DISABLED;

	// FIXME: disable GUI buttons

	if (_cursorMode == cursorId)
		findNextEnabledCursor(cursorId);

	invalidateGUI();
}

void AGSEngine::updateInvCursor(uint itemId) {
	if (getGameOption(OPT_FIXEDINVCURSOR))
		return;
	if (itemId == 0)
		return;

	uint cursorSprite = _gameFile->_invItemInfo[itemId]._cursorPic;
	// Fall back to the inventory pic if no cursor pic is defined.
	if (cursorSprite == 0)
		cursorSprite = _gameFile->_invItemInfo[itemId]._pic;

	_gameFile->_cursors[MODE_USE]._pic = cursorSprite;

	// FIXME: hotspot
}

// 'update_gui_zorder'
void AGSEngine::resortGUIs() {
	Common::Array<GUIGroup *> &groups = _gameFile->_guiGroups;
	Common::Array<GUIGroup *> &drawOrder = _gameFile->_guiGroupDrawOrder;

	drawOrder.clear();
	drawOrder.reserve(groups.size());

	// for each GUI
	for (uint i = 0; i < groups.size(); ++i) {
		// find the right place in the draw order array
		uint insertAt = drawOrder.size();
		for (uint j = 0; j < insertAt; ++j) {
			if (groups[i]->_zorder < drawOrder[j]->_zorder) {
				insertAt = j;
				break;
			}
		}
		// insert the new item
		drawOrder.insert_at(insertAt, groups[i]);
	}
}

uint AGSEngine::getGUIAt(const Common::Point &pos) {
	Common::Point p(multiplyUpCoordinate(pos.x), multiplyUpCoordinate(pos.y));

	for (int i = _gameFile->_guiGroups.size() - 1; i >= 0; --i) {
		GUIGroup *group = _gameFile->_guiGroupDrawOrder[i];
		if (!group->_visible)
			continue;
		if (group->_flags & GUIF_NOCLICK)
			continue;
		if (p.x >= group->_x && p.y >= group->_y && p.x <= group->_x + (int)group->_width && p.y <= group->_y + (int)group->_height)
			return (uint)group->_id;
	}

	return (uint)-1;
}

void AGSEngine::removePopupInterface(uint guiId) {
	if (_poppedInterface != guiId)
		return;

	_poppedInterface = (uint)-1;
	unpauseGame();

	GUIGroup *group = _gameFile->_guiGroups[guiId];
	group->setVisible(false);

	// FIXME: filter?

	if (_state->_disabledUserInterface) {
		// Only change the mouse cursor if it hasn't been specifically changed first
		if (_cursorMode == _graphics->getCurrentCursor())
			_graphics->setMouseCursor(CURS_WAIT);
	} else
		setDefaultCursor();

	if (guiId == _mouseOnGUI)
		_mouseOnGUI = (uint)-1;
	invalidateGUI();
}

uint AGSEngine::getCharacterAt(const Common::Point &pos, int &charYPos) {
	uint charId = (uint)-1;
	int bestBaseline = 0;

	for (uint i = 0; i < _characters.size(); ++i) {
		Character *chr = _characters[i];

		if (chr->_room != _displayedRoom)
			continue;
		if (!chr->_on)
			continue;
		if (chr->_flags & CHF_NOINTERACT)
			continue;
		if (chr->_view < 0)
			continue;

		// TODO: any more sanity-checks?
		ViewStruct &view = _gameFile->_views[chr->_view];
		if (chr->_loop >= view._loops.size() || chr->_frame >= view._loops[chr->_loop]._frames.size())
			continue;

		if (!chr->containsPoint(this, pos))
			continue;

		int baseline = chr->getBaseline();
		if (baseline < bestBaseline)
			continue;

		charId = i;
		bestBaseline = baseline;
	}

	charYPos = bestBaseline;
	return charId;
}

uint AGSEngine::getInventoryItemAt(const Common::Point &pos) {
	uint guiId = getGUIAt(pos);
	if (guiId == (uint)-1)
		return (uint)-1;

	GUIGroup *group = _gameFile->_guiGroups[guiId];

	GUIControl *control = group->getControlAt(pos - Common::Point(group->_x, group->_y));
	if (!control || !control->isOfType(sotGUIInvWindow))
		return (uint)-1;

	GUIInvControl *invControl = (GUIInvControl *)control;
	return invControl->getItemAt(pos - Common::Point(group->_x, group->_y) -
		Common::Point(control->_x, control->_y));
}

Common::String AGSEngine::getLocationName(const Common::Point &pos) {
	if (_displayedRoom == 0xffffffff)
		error("getLocationName called before a room was loaded");

	if (getGUIAt(pos) != (uint)-1) {
		uint itemId = getInventoryItemAt(pos);
		if (itemId != (uint)-1) {
			// over an item
			if (_state->_getLocNameLastTime != 1000 + itemId) {
				_state->_getLocNameLastTime = 1000 + itemId;
				invalidateGUI();
			}
			return getTranslation(_gameFile->_invItemInfo[itemId]._name);
		} else if (_state->_getLocNameLastTime > 1000 && _state->_getLocNameLastTime < 1000 + MAX_INV) {
			// no longer over an item
			_state->_getLocNameLastTime = (uint)-1;
			invalidateGUI();
		}
		return Common::String();
	}

	uint locId;
	uint locType = getLocationType(pos, locId);

	// don't reset the loc name if we got passed out-of-bounds coordinates
	Common::Point p = pos + Common::Point(divideDownCoordinate(_graphics->_viewportX),
		divideDownCoordinate(_graphics->_viewportY));
	if (p.x < 0 || p.x >= _currentRoom->_width || p.y < 0 || p.y >= _currentRoom->_height)
		return Common::String();

	Common::String name;

	switch (locType) {
	case 0:
		if (_state->_getLocNameLastTime) {
			_state->_getLocNameLastTime = 0;
			invalidateGUI();
		}
		break;

	case LOCTYPE_CHAR:
		if (_state->_getLocNameLastTime != 2000 + locId) {
			_state->_getLocNameLastTime = 2000 + locId;
			invalidateGUI();
		}
		name = getTranslation(_characters[locId]->_name);
		break;

	case LOCTYPE_OBJ:
		if (_state->_getLocNameLastTime != 3000 + locId) {
			_state->_getLocNameLastTime = 3000 + locId;
			invalidateGUI();
		}
		name = getTranslation(_currentRoom->_objects[locId]->_name);
		break;

	case LOCTYPE_HOTSPOT:
		if (_state->_getLocNameLastTime != locId) {
			_state->_getLocNameLastTime = locId;
			invalidateGUI();
		}
		if (locId)
			name = getTranslation(_currentRoom->_hotspots[locId]._name);
		break;

	default:
		error("internal error in getLocationName (invalid location type %d)", locType);
	}

	return name;
}

uint AGSEngine::getLocationType(const Common::Point &pos, bool throughGUI, bool allowHotspot0) {
	uint id;
	return getLocationType(pos, id, throughGUI, allowHotspot0);
}

// allowHotspot0 defines whether Hotspot 0 returns LOCTYPE_HOTSPOT
// or whether it returns 0
uint AGSEngine::getLocationType(const Common::Point &pos, uint &id, bool throughGUI, bool allowHotspot0) {
	id = 0;

	// If it's not in ProcessClick, then return 0 when over a GUI
	if (!throughGUI && getGUIAt(pos) != (uint)-1)
		return 0;

	Common::Point p = pos + Common::Point(divideDownCoordinate(_graphics->_viewportX),
		divideDownCoordinate(_graphics->_viewportY));
	if (p.x < 0 || p.y < 0 || p.x >= _currentRoom->_width || p.y >= _currentRoom->_height)
		return 0;

	// check characters, objects and walkbehinds, work out which is
	// foremost visible to the player

	int charYPos = 0;
	uint charAt = getCharacterAt(p, charYPos);
	uint hotspotAt = _currentRoom->getHotspotAt(p.x, p.y);
	// getObjectAt adjusts the parameters itself, so use the unmodified pos.
	int objectYPos = 0;
	uint objAt = _currentRoom->getObjectAt(pos.x, pos.y, objectYPos);

	p.x = multiplyUpCoordinate(p.x);
	p.y = multiplyUpCoordinate(p.y);

	// Find the walkbehind at this point, since it might obscure the object.
	assert(_currentRoom->_walkBehindMask.format.bytesPerPixel == 1);
	byte walkBehindId = *(byte *)_currentRoom->_walkBehindMask.getBasePtr(p.x, p.y);

	int walkBase = 0;
	if (walkBehindId > 0 && walkBehindId < _currentRoom->_walkBehinds.size()) {
		walkBase = _currentRoom->_walkBehinds[walkBehindId]._baseline;
		// // if it's an Ignore Walkbehinds object, then ignore the walkbehind
		if (objAt != (uint)-1 && (_currentRoom->_objects[objAt]->_flags & OBJF_NOWALKBEHINDS))
			walkBase = 0;
		if (charAt != (uint)-1 && (_characters[charAt]->_flags & CHF_NOWALKBEHINDS))
			walkBase = 0;
	}

	// There's always a match for hotspot 0, if nothing else.
	uint winner = LOCTYPE_HOTSPOT;
	if (charAt != (uint)-1 && objAt != (uint)-1) {
		if (walkBase > objectYPos && walkBase > charYPos)
			winner = LOCTYPE_HOTSPOT;
		else if (objectYPos > charYPos)
			winner = LOCTYPE_OBJ;
		else
			winner = LOCTYPE_CHAR;
	} else if (charAt != (uint)-1) {
		if (walkBase <= charYPos)
			winner = LOCTYPE_CHAR;
	} else if (objAt != (uint)-1) {
		if (walkBase <= objectYPos)
			winner = LOCTYPE_OBJ;
	}

	// Hotspot 0 doesn't win if it's not allowed to.
	if ((winner == LOCTYPE_HOTSPOT) && (!allowHotspot0) && (hotspotAt == 0))
		winner = 0;

	if (winner == LOCTYPE_HOTSPOT)
		id = hotspotAt;
	else if (winner == LOCTYPE_CHAR)
		id = charAt;
	else if (winner == LOCTYPE_OBJ)
		id = objAt;

	return winner;
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

void AGSEngine::queueOrRunTextScript(ccInstance *instance, const Common::String &name, uint32 p1) {
	Common::Array<RuntimeValue> params;
	params.push_back(p1);
	queueOrRunTextScript(instance, name, params);
}

void AGSEngine::queueOrRunTextScript(ccInstance *instance, const Common::String &name, uint32 p1, uint32 p2) {
	Common::Array<RuntimeValue> params;
	params.push_back(p1);
	params.push_back(p2);
	queueOrRunTextScript(instance, name, params);
}

void AGSEngine::queueOrRunTextScript(ccInstance *instance, const Common::String &name, const Common::Array<RuntimeValue> &params) {
	assert(instance == _gameScript || instance == _roomScript);

	if (_runningScripts.size())
		_runningScripts.back().queueScript(name, instance == _gameScript, params);
	else
		runTextScript(instance, name, params);
}

void AGSEngine::runTextScript(ccInstance *instance, const Common::String &name, const Common::Array<RuntimeValue> &params) {
	// first, check for special cases
	switch (params.size()) {
	case 0:
		if (name != REP_EXEC_NAME)
			break;
		// repeatedly_execute
		{
		uint roomWas = _state->_roomChanges;
		for (uint i = 0; i < _scriptModules.size(); ++i) {
			// FIXME: original checks whether the symbol exists first, unnecessary?
			runScriptFunction(_scriptModules[i], name, params);
			// FIXME: check restore game also
			if (roomWas != _state->_roomChanges)
				return;
		}
		break;
		}
	case 1:
		if (name != "on_key_press" && name != "on_mouse_click")
			break;
		if (runClaimableEvent(name, params))
			return;
		break;
	case 2:
		if (name != "on_event")
			break;
		if (runClaimableEvent(name, params))
			return;
		break;
	}

	if (!runScriptFunction(instance, name, params)) {
		if (instance == _roomScript)
			error("failed to run room script '%s' (room %d)", name.c_str(), _displayedRoom);
	}

	if (params.size() == 1 && instance == _roomScript)
		_state->_roomScriptFinished = 1;
}

ScriptImport AGSEngine::resolveImport(const Common::String &name, bool mustSucceed) {
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

	if (mustSucceed)
		error("unresolved script import '%s'", name.c_str());

	return ScriptImport();
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
	_lastTranslationSourceTextLength = text.size();
	if (!text.empty() && text[0] == '&' && _state->_unfactorSpeechFromTextLength) {
		// if there's an "&12 text" type line, remove "&12 " from the source
		// length
		// TODO: merge this with identical code in getTextDisplayTime
		for (uint i = 0; i < text.size(); ++i) {
			_lastTranslationSourceTextLength--;
			if (text[i] == ' ')
				break;
		}
	}

	// FIXME: implement

	return text;
}

Common::String AGSEngine::replaceMacroTokens(const Common::String &text) {
	Common::String out;

	bool hasMacro = false;
	Common::String macroName;
	for (uint i = 0; i < text.size(); ++i) {
		if (!hasMacro) {
			if (text[i] != '@')
				out += text[i];
			else
				hasMacro = true;
			continue;
		}

		if (text[i] != '@') {
			macroName += text[i];
			continue;
		}

		// try and find a macro; if we don't match one, just output as it was
		if (macroName.equalsIgnoreCase("score"))
			out += Common::String::format("%d", _state->_score);
		else if (macroName.equalsIgnoreCase("totalscore"))
			out += Common::String::format("%d", _state->_totalScore);
		else if (macroName.equalsIgnoreCase("scoretext"))
			out += Common::String::format("%d of %d", _state->_score, _state->_totalScore);
		else if (macroName.equalsIgnoreCase("gamename"))
			out += _gameFile->_gameName;
		else if (macroName.equalsIgnoreCase("overhotspot")) {
			// while the game is in wait mode, no overhotspot text
			if (!_state->_disabledUserInterface) {
				Common::Point mousePos = _system->getEventManager()->getMousePos();
				mousePos.x = divideDownCoordinate(mousePos.x);
				mousePos.y = divideDownCoordinate(mousePos.y);
				out += getLocationName(mousePos);
			}
		} else
			out += '@' + macroName;

		hasMacro = false;
		macroName.clear();
	}

	if (hasMacro)
		out += '@' + macroName;

	return out;
}

uint AGSEngine::getTextDisplayTime(const Common::String &text, bool canBeRelative) {
	uint fpsTimer = _framesPerSecond;
	uint useLen = text.size();

	// if it's background speech, make it stay relative to game speed
	if (canBeRelative && (_state->_bgSpeechGameSpeed == 1))
		fpsTimer = 40;

	if (_lastTranslationSourceTextLength != (uint)-1) {
		// sync to length of original text, to make sure any animations
		// and music sync up correctly
		useLen = _lastTranslationSourceTextLength;
		_lastTranslationSourceTextLength = (uint)-1;
	} else if (!text.empty() && text[0] == '&' && _state->_unfactorSpeechFromTextLength) {
		// if there's an "&12 text" type line, remove "&12 " from the source
		// length
		for (uint i = 0; i < text.size(); ++i) {
			useLen--;
			if (text[i] == ' ')
				break;
		}
	}

	if (!useLen)
		return 0;

	uint textSpeed = _state->_textSpeed + _state->_textSpeedModifier;
	if (textSpeed <= 0)
		error("text speed is zero; unable to display text");

	// Store how many game loops per character of text
	// This is calculated using a hard-coded 15 for the text speed,
	// so that it's always the same no matter how fast the user
	// can read.
	// TODO: Except it isn't?
	_lipsyncLoopsPerCharacter = (((useLen / _state->_lipsyncSpeed) + 1) * fpsTimer) / useLen;

	uint textDisplayTimeInMs = ((useLen / textSpeed) + 1) * 1000;
	if (textDisplayTimeInMs < _state->_textMinDisplayTimeMs)
		textDisplayTimeInMs = _state->_textMinDisplayTimeMs;

	return (textDisplayTimeInMs * fpsTimer) / 1000;
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

void AGSEngine::updateViewport() {
	if (_currentRoom->_width <= _graphics->_baseWidth && _currentRoom->_height <= _graphics->_baseHeight) {
		_graphics->_viewportX = 0;
		_graphics->_viewportY = 0;
		return;
	}

	if (!_state->_offsetsLocked) {
		int x = multiplyUpCoordinate(_playerChar->_x) - _graphics->_width/2;
		int y = multiplyUpCoordinate(_playerChar->_y) - _graphics->_height/2;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		_graphics->_viewportX = x;
		_graphics->_viewportY = y;
	}

	_graphics->checkViewportCoords();
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
		if ((_graphics->getCurrentCursor() == _cursorMode) || (untilType == kUntilNoTextOverlay))
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
	case kUntilNoTextOverlay:
		if (!_textOverlayCount)
			return kUntilNothing;
		break;
	case kUntilMessageDone:
		if (_state->_messageTime < 0)
			return kUntilNothing;
		break;
	case kUntilWaitDone:
		if (_state->_waitCounter == 0)
			return kUntilNothing;
		break;
	case kUntilCharAnimDone:
		if (_characters[_blockingUntilId]->_animating == 0)
			return kUntilNothing;
		break;
	case kUntilCharWalkDone:
		if (_characters[_blockingUntilId]->_walking == 0)
			return kUntilNothing;
		break;
	case kUntilObjMoveDone:
		if (_currentRoom->_objects[_blockingUntilId]->_moving == 0)
			return kUntilNothing;
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
	if (charId >= _characters.size())
		error("skipUntilCharacterStops: character %d is invalid", charId);

	Character *chr = _characters[charId];
	if (chr->_room != _displayedRoom)
		error("skipUntilCharacterStops: character %d isn't in current room", charId);

	// if they are not currently moving, do nothing
	if (!chr->_walking)
		return;

	if (_state->_inCutscene)
		error("skipUntilCharacterStops: can't be used within a cutscene");

	_state->_endCutsceneMusic = (uint)-1;
	_state->_fastForward = 2;
	_state->_skipUntilCharStops = charId;
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

	if (_textOverlayCount)
		removeScreenOverlay(OVER_TEXTMSG);
}

void AGSEngine::stopFastForwarding() {
	_state->_fastForward = 0;
	// FIXME: setpal


	/* FIXME if (_state->_endCutsceneMusic != (uint)-1)
		newMusic(_state->_endCutsceneMusic); */
	// FIXME: restore actual volume of sounds
	_audio->updateMusicVolume();
}

bool AGSEngine::playSpeech(uint charId, uint speechId) {
	_audio->_channels[SCHAN_SPEECH]->stop();

	// don't play speech if we're skipping a cutscene
	if (_state->_fastForward)
		return false;
	if (_state->_wantSpeech < 1)
		return false;
	if (!_audio->hasSpeechResources())
		return false;

	Common::String speechName = "NARR";
	if (charId != (uint)-1) {
		// append the first 4 characters of the script name to the filename
		Common::String scriptName = _characters[charId]->_scriptName;
		// TODO: ouch, checking for 'c' like this seems a bit drastic
		if (!scriptName.empty() && scriptName[0] == 'c')
			scriptName = scriptName.c_str() + 1;
		speechName.clear();
		for (uint i = 0; i < scriptName.size() && i < 4; ++i)
			speechName += scriptName[i];
	}

	speechName = speechName + Common::String::format("%d", speechId);

	// FIXME: voice lip sync

	if (!_audio->playSpeech(speechName)) {
		debugC(kDebugLevelGame, "failed to load speech '%s'", speechName.c_str());
		// FIXME: reset lip sync
		return false;
	}

	// change Sierra w/bgrnd  to Sierra without background when voice
	// is available (for Tierra)
	if ((getGameOption(OPT_SPEECHTYPE) == 2) && (_state->_noTextBgWhenVoice > 0)) {
		_gameFile->_options[OPT_SPEECHTYPE] = 1;
		_state->_noTextBgWhenVoice = 2;
	}

	return true;
}

void AGSEngine::stopSpeech() {
	if (!_audio->_channels[SCHAN_SPEECH]->isValid())
		return;

	// FIXME: volume
	_audio->_channels[SCHAN_SPEECH]->stop();
	// FIXME: lip line

	if (_state->_noTextBgWhenVoice == 2) {
		// set back to Sierra w/bgrnd
		_state->_noTextBgWhenVoice = 1;
		_gameFile->_options[OPT_SPEECHTYPE] = 2;
	}
}

#define PROP_TYPE_BOOL   1
#define PROP_TYPE_INT    2
#define PROP_TYPE_STRING 3

int AGSEngine::getIntProperty(const Common::String &name, const Common::StringMap &properties) {
	int propertyId = -1;
	for (uint i = 0; i < _gameFile->_schemaProperties.size(); ++i) {
		if (_gameFile->_schemaProperties[i]._name == name) {
			propertyId = i;
			break;
		}
	}
	if (propertyId == -1)
		error("script requested property '%s', which isn't present in the game schema", name.c_str());

	const CustomPropertySchemaProperty &property = _gameFile->_schemaProperties[propertyId];
	if (property._type == PROP_TYPE_STRING)
		error("script requested property '%s' as an integer, but the schema says it's a string", name.c_str());

	if (properties.contains(name))
		return atoi(properties[name].c_str());
	return atoi(property._defaultValue.c_str());
}

Common::String AGSEngine::getStringProperty(const Common::String &name, const Common::StringMap &properties) {
	int propertyId = -1;
	for (uint i = 0; i < _gameFile->_schemaProperties.size(); ++i) {
		if (_gameFile->_schemaProperties[i]._name == name) {
			propertyId = i;
			break;
		}
	}
	if (propertyId == -1)
		error("script requested property '%s', which isn't present in the game schema", name.c_str());

	const CustomPropertySchemaProperty &property = _gameFile->_schemaProperties[propertyId];
	if (property._type != PROP_TYPE_STRING)
		error("script requested property '%s' as a string, but the schema says it's an integer", name.c_str());

	if (properties.contains(name))
		return properties[name].c_str();
	return property._defaultValue;
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
	// "Fixed invisible mouse cursor in dialogs with some version 2.x games, e.g. Murder in a Wheel."
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
		Common::Array<RuntimeValue> params;
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

	Common::Array<RuntimeValue> params;
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

bool AGSEngine::runScriptFunction(ccInstance *instance, const Common::String &name, const Common::Array<RuntimeValue> &params) {
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
	// FIXME: sabotage any running scripts (set _eventClaimed) in the event of restored game

	return true;
}

bool AGSEngine::prepareTextScript(ccInstance *instance, const Common::String &name) {
	if (!instance->exportsSymbol(name))
		return false;

	if (instance->isRunning()) {
		warning("script was already running, when trying to run '%s'", name.c_str());
		return false;
	}

	// TODO: original engine has code which forks an instance if it was running
	// (but it's unreachable due to the check above..)

	_runningScripts.push_back(ExecutingScript(instance));

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

		runTextScript(script.isGameScript ? _gameScript : _roomScript, script.name, script.params);

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

void ExecutingScript::queueScript(const Common::String &name, bool isGameScript, const Common::Array<RuntimeValue> &params) {
	PendingScript i;
	i.name = name;
	i.isGameScript = isGameScript;
	i.params = params;
	_pendingScripts.push_back(i);
}

} // End of namespace AGS
