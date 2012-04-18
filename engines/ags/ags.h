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

#ifndef AGS_AGS_H
#define AGS_AGS_H

#include "common/rect.h"
#include "common/system.h"
#include "engines/engine.h"
#include "engines/game.h"
#include "ags/detection.h"

// for RuntimeValue
#include "engines/ags/script.h"

struct ADGameFileDescription;

namespace Common {
	class RandomSource;
	class String;
}

namespace AGS {

struct AGSGameDescription;

class AGSGraphics;
class GameFile;
class ResourceManager;
class Room;
class ScriptObject;
class SpriteSet;
class Character;
class ccInstance;

struct PendingScript {
	Common::String name;
	bool isGameScript;
	Common::Array<RuntimeValue> params;
};

// actions which can't be run while scripts are running, and so must be queued until the script is done
enum PostScriptActionType {
	kPSANewRoom,
	kPSAInvScreen,
	kPSARestoreGame,
	kPSARestoreGameDialog,
	kPSARunAGSGame,
	kPSARunDialog,
	kPSARestartGame,
	kPSASaveGame,
	kPSASaveGameDialog
};

struct PostScriptAction {
	PostScriptActionType type;
	uint data;
	Common::String name;
};

class ExecutingScript {
public:
	ExecutingScript(ccInstance *instance);

	void queueAction(PostScriptActionType type, uint data, const Common::String &name);
	// run_another
	void queueScript(const Common::String &name, bool isGameScript, const Common::Array<RuntimeValue> &params);

	Common::Array<PendingScript> _pendingScripts;
	Common::Array<PostScriptAction> _pendingActions;

protected:
	ccInstance *_instance;
};

enum NewRoomState {
	kNewRoomStateNone = 0,
	kNewRoomStateNew = 1,		// new room
	kNewRoomStateFirstTime = 2,	// first time in new room
	kNewRoomStateSavedGame = 3	// new room due to loading saved game
};

enum GameEventType {
	kEventTextScript = 1,
	kEventRunEventBlock = 2,
	kEventAfterFadeIn = 3,
	kEventInterfaceClick = 4,
	kEventNewRoom = 5
};

enum {
	kEventBlockHotspot = 1,
	kEventBlockRoom = 2
};

enum {
	kTextScriptNone = 0,
	kTextScriptRepeatedlyExecute = 1,
	kTextScriptOnKeyPress = 2,
	kTextScriptOnMouseClick = 3
};

// kTextScriptOnMouseClick parameters
enum {
	kMouseLeft = 1,
	kMouseRight = 2,
	kMouseMiddle = 3,
	kMouseLeftInv = 5,
	kMouseRightInv = 6,
	kMouseMiddleInv = 7,
	kMouseWheelNorth = 8,
	kMouseWheelSouth = 9
};

enum {
	kRoomEventLeftEdge = 0,
	kRoomEventRightEdge = 1,
	kRoomEventBottomEdge = 2,
	kRoomEventTopEdge = 3,
	kRoomEventFirstTimeEntersScreen = 4,
	kRoomEventEntersScreen = 5,
	kRoomEventTick = 6,
	kRoomEventEnterAfterFadeIn = 7,
	kRoomEventPlayerLeavesScreen = 8
};

enum {
	kHotspotEventStandsOn = 0,
	kRegionEventStandsOn = 0,
	kRegionEventWalksOnto = 1,
	kRegionEventWalksOff = 2,
	kGeneralEventClick = 4,
	kHotspotEventClickOnHotspot = 5,
	kHotspotEventMouseMovesOver = 6
};

enum BlockUntilType {
	kUntilNothing = 0,
	kUntilNoTextOverlay,
	kUntilMessageDone,
	kUntilWaitDone,
	kUntilCharAnimDone,
	kUntilCharWalkDone,
	kUntilObjMoveDone,
	kUntilObjCycleDone
};

enum CutsceneSkipType {
	kSkipESCOnly = 1,
	kSkipAnyKey = 2,
	kSkipMouseClick = 3,
	kSkipAnyKeyOrMouseClick = 4,
	kSkipESCOrRightButton = 5
};

struct GameEvent {
	GameEventType type;

	uint32 data1, data2, data3;
	uint32 playerId;
};

#define LOCTYPE_HOTSPOT 1
#define LOCTYPE_CHAR 2
#define LOCTYPE_OBJ  3

class AGSEngine : public Engine {
public:
	AGSEngine(OSystem *syst, const AGSGameDescription *gameDesc);
	~AGSEngine();

	void initGame(const AGSGameDescription *gd);

	void pauseGame();
	void unpauseGame();
	bool isGamePaused() { return _pauseGameCounter > 0; }

	bool isDemo() const;

	uint32 getGameFileVersion() const;
	uint32 getGUIVersion() const;
	uint32 getGameUniqueID() const;
	Common::String getMasterArchive() const;

	Common::SeekableReadStream *getFile(const Common::String &filename) const;
	ResourceManager *getResourceManager() { return _resourceMan; }
	SpriteSet *getSprites() { return _sprites; }

	uint32 getOperatingSystem() const;

	void setDefaultCursor();
	uint32 findNextEnabledCursor(uint32 startWith);
	void setNextCursor();
	void setCursorMode(uint32 newMode);
	uint32 getCursorMode() { return _cursorMode; }
	void enableCursorMode(uint cursorId);
	void disableCursorMode(uint cursorId);
	void updateInvCursor(uint itemId);

	void updateInvOrder();

	bool _guiDisabledState;
	uint _guiDisabledStyle;

	void resortGUIs();
	uint getGUIAt(const Common::Point &pos);
	void removePopupInterface(uint guiId);
	uint convertGUIDisabledStyle(uint style);
	void updateGUIDisabledStatus();

	uint getCharacterAt(const Common::Point &pos, int &charYPos);

	uint getInventoryItemAt(const Common::Point &pos);

	Common::String getLocationName(const Common::Point &pos);
	uint getLocationType(const Common::Point &pos, bool throughGUI = false, bool allowHotspot0 = false);
	uint getLocationType(const Common::Point &pos, uint &id, bool throughGUI = false, bool allowHotspot0 = false);

	struct ViewLoopNew *getViewLoop(uint view, uint loop);
	class ViewFrame *getViewFrame(uint view, uint loop, uint frame);
	void checkViewFrame(uint view, uint loop, uint frame);

	struct ScriptImport resolveImport(const Common::String &name, bool mustSucceed = true);
	class GlobalScriptState *getScriptState();

	Common::RandomSource *getRandomSource() { return _rnd; }

	uint32 getGameSpeed();
	void setGameSpeed(uint32 speed);
	uint getLoopCounter() const { return _loopCounter; }

	byte getGameOption(uint index);

	Common::String getTranslation(const Common::String &text);
	Common::String replaceMacroTokens(const Common::String &text);
	uint getTextDisplayTime(const Common::String &text, bool canBeRelative = false);

	// resolution system functions
	uint getFixedPixelSize(uint pixels);
	int convertToLowRes(int coord);
	int convertBackToHighRes(int coord);
	int multiplyUpCoordinate(int coords);
	void multiplyUpCoordinates(int32 &x, int32 &y);
	void multiplyUpCoordinatesRoundUp(int32 &x, int32 &y);
	int divideDownCoordinate(int coord);
	int divideDownCoordinateRoundUp(int coord);

	void updateViewport();

	void blockUntil(BlockUntilType untilType, uint untilId = 0);

	void skipUntilCharacterStops(uint charId);
	void endSkippingUntilCharStops();
	void startSkippableCutscene();
	void startSkippingCutscene();
	void stopFastForwarding();

	void runDialogId(uint dialogId);
	int showDialogOptions(uint dialogId, uint sayChosenOption);

	Common::Array<class ScreenOverlay *> _overlays;
	uint _completeOverlayCount, _textOverlayCount;

	uint createTextOverlayCore(int x, int y, uint width, uint fontId, int color, const Common::String &text, int allowShrink);
	uint createTextOverlay(int x, int y, uint width, uint fontId, int color, const Common::String &text);
	uint addScreenOverlay(int x, int y, uint type, const Graphics::Surface &surface, bool alphaChannel);
	void updateOverlayTimers();
	uint findOverlayOfType(uint type);
	void removeScreenOverlay(uint type);
	void removeScreenOverlayIndex(uint index);

	void updateSpeech();

	void display(const Common::String &text) { displayAtY(-1, text); }
	void displayAtY(int y, const Common::String &text);
	void displayAt(int x, int y, int width, Common::String text, int blocking, int asSpeech = 0, bool isThought = false,
		int allowShrink = 0, bool overlayPositionFixed = false);
	uint displayMain(int x, int y, int width, const Common::String &text, int blocking, int usingFont,
		int asSpeech, bool isThought, int allowShrink, bool overlayPositionFixed);
	void displaySpeech(Common::String text, uint charId, int x = -1, int y = -1, int width = -1, bool isThought = false);
	void displaySpeechCore(const Common::String &text, uint charId);
	void displaySpeechAt(int x, int y, int width, uint charId, const Common::String &text);
	uint displaySpeechBackground(uint charId, const Common::String &text);

	void invalidateScreen() { _needsUpdate = true; }
	void invalidateGUI() { _guiNeedsUpdate = true; }
	void invalidateBackground() { _backgroundNeedsUpdate = true; }

	uint getCurrentRoomId() { return _displayedRoom; }
	Room *getCurrentRoom() { return _currentRoom; }
	bool inEntersScreen() { return _inEntersScreenCounter > 0; }
	void scheduleNewRoom(uint roomId);

	GameFile *_gameFile;
	class GameState *_state;
	class AGSAudio *_audio;
	AGSGraphics *_graphics;

	void setAsPlayerChar(uint charId);
	Character *getPlayerChar() { return _playerChar; }
	Common::Array<Character *> _characters;

	/** Prepend 'TARGET-' to the given filename. */
	Common::String wrapFilename(const Common::String &name) const { return _targetName + "-" + name; };
	Graphics::Surface *getWalkableMaskFor(uint charId);

	void addInventory(uint itemId);
	void loseInventory(uint itemId);
	void setActiveInventory(uint itemId);

	Common::String formatString(const Common::String &string, const Common::Array<RuntimeValue> &values);

	int32 _newRoomPos;
	int32 _newRoomX;
	int32 _newRoomY;

	// for CallRoomScript
	void queueCustomRoomScript(uint32 param);

	void runOnEvent(uint32 p1, uint32 p2);

	void claimEvent();

	bool playSpeech(uint charId, uint speechId);
	void stopSpeech();

	int getIntProperty(const Common::String &name, const Common::StringMap &properties);
	Common::String getStringProperty(const Common::String &name, const Common::StringMap &properties);

	bool runCharacterInteraction(uint charId, uint mode, bool checkOnly = false);
	bool runInventoryInteraction(uint itemId, uint mode, bool checkOnly = false);
	bool runHotspotInteraction(uint hotspotId, uint mode, bool checkOnly = false);
	bool runObjectInteraction(uint objectId, uint mode, bool checkOnly = false);
	void runRegionInteraction(uint regionId, uint mode);

private:
	const AGSGameDescription *_gameDescription;

	Common::RandomSource *_rnd;

	uint32 _engineStartTime;
	uint32 _playTime;
	uint32 _loopCounter;
	uint32 _framesPerSecond;
	uint32 _lastFrameTime;

	uint _pauseGameCounter;

	ResourceManager *_resourceMan;
	SpriteSet *_sprites;

	bool _needsUpdate, _guiNeedsUpdate;
	bool _backgroundNeedsUpdate;
	uint32 _cursorMode;
	uint _poppedInterface;
	// wasongui, mouse_on_iface
	uint _clickWasOnGUI, _mouseOnGUI;

	uint32 _startingRoom;
	uint32 _displayedRoom;
	Room *_currentRoom;
	Common::HashMap<uint, Room *> _loadedRooms;

	// new room state (this frame)
	NewRoomState _inNewRoomState;
	// new room state (from last time it was not None)
	NewRoomState _newRoomStateWas;

	int _leavesScreenRoomId;

	void queueOrRunTextScript(ccInstance *instance, const Common::String &name, uint32 p1);
	void queueOrRunTextScript(ccInstance *instance, const Common::String &name, uint32 p1, uint32 p2);
	void queueOrRunTextScript(ccInstance *instance, const Common::String &name, const Common::Array<RuntimeValue> &params = Common::Array<RuntimeValue>());

	void runTextScript(ccInstance *instance, const Common::String &name,
		const Common::Array<RuntimeValue> &params = Common::Array<RuntimeValue>());

	Common::Array<GameEvent> _queuedGameEvents;
	void queueGameEvent(GameEventType type, uint data1 = 0, uint data2 = (uint)-1000, uint data3 = 0);
	void runGameEventNow(GameEventType type, uint data1 = 0, uint data2 = (uint)-1000, uint data3 = 0);
	void processGameEvent(const GameEvent &event);
	void processAllGameEvents();
	bool runInteractionScript(struct InteractionScript *scripts, uint eventId, uint fallback = (uint)-1, bool isInventory = false, bool checkOnly = false);
	bool runInteractionEvent(struct NewInteraction *interaction, uint eventId, uint fallback = (uint)-1, bool isInventory = false, bool checkOnly = false);
	uint getInteractionValue(const struct NewInteractionValue &value);
	bool runInteractionCommandList(struct NewInteractionEvent &event, struct NewInteractionCommandList *list, uint &commandsRunCount);
	void runUnhandledEvent(uint eventId);
	bool runClaimableEvent(const Common::String &name, const Common::Array<RuntimeValue> &params);

	// details of running event block, if any
	bool _insideProcessEvent;
	uint _inEntersScreenCounter;
	Common::String _eventBlockBaseName;
	uint _eventBlockId;

	Character *_playerChar;

	Common::Array<ExecutingScript> _runningScripts;

	// script instances
	ccInstance *_gameScript, *_gameScriptFork;
	Common::Array<ccInstance *> _scriptModules;
	Common::Array<ccInstance *> _scriptModuleForks;
	ccInstance *_dialogScriptsScript;
	ccInstance *_roomScript, *_roomScriptFork;

	class GlobalScriptState *_scriptState;
	struct RoomObjectState *_roomObjectState;

	ScriptObject *_scriptPlayerObject;
	ScriptObject *_scriptMouseObject;
	ScriptObject *_gameStateGlobalsObject;
	ScriptObject *_saveGameIndexObject;
	ScriptObject *_scriptSystemObject;

	uint _eventClaimed;

	BlockUntilType _blockingUntil;
	uint _blockingUntilId;

	uint _lastTranslationSourceTextLength;
	uint _lipsyncLoopsPerCharacter;
	uint _lipsyncTextOffset;
	Common::String _lipsyncText;
	bool _saidText;
	bool _saidSpeechLine;
	uint _faceTalkingOverlayIndex;

	bool init();
	void adjustSizesForResolution();

	bool mainGameLoop();
	void tickGame(bool checkControls = false);
	void updateEvents(bool checkControls);
	void processInterfaceClick(uint guiId, uint controlId, uint mouseButtonId);
	void draw();
	void updateStuff();

	void startNewGame();
	void setupPlayerCharacter(uint32 charId);
	void createGlobalScript();
	void firstRoomInitialization();
	void loadNewRoom(uint32 id, Character *forChar);
	void unloadOldRoom();
	void checkNewRoom();
	void newRoom(uint roomId);

	BlockUntilType checkBlockingUntil();

	void doConversation(uint dialogId);
	int runDialogScript(struct DialogTopic &topic, uint dialogId, uint16 offset, uint optionId);
	int runDialogRequest(uint request);

	bool runScriptFunction(ccInstance *instance, const Common::String &name, const Common::Array<RuntimeValue> &params = Common::Array<RuntimeValue>());
	bool prepareTextScript(ccInstance *instance, const Common::String &name);
	void postScriptCleanup();

	const ADGameFileDescription *getGameFiles() const;
	const char *getDetectedGameFile() const;
	const char *getGameId() const;
	Common::Language getLanguage() const;
	Common::Platform getPlatform() const;

	// Engine APIs
	Common::Error run();
	bool hasFeature(EngineFeature f) const;
	void pauseEngineIntern(bool pause);
};

} // End of namespace AGS

#endif // AGS_AGS_H
