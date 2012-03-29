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
	Common::Array<uint32> params;
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
	void queueScript(const Common::String &name, bool isGameScript, const Common::Array<uint32> &params);

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

enum BlockUntilType {
	kUntilNothing = 0,
	kUntilNoOverlay,
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

	void setDefaultCursor();
	uint32 findNextEnabledCursor(uint32 startWith);
	void setCursorMode(uint32 newMode);
	uint32 getCursorMode() { return _cursorMode; }

	uint getGUIAt(const Common::Point &pos);
	void removePopupInterface(uint guiId);

	struct ViewLoopNew *getViewLoop(uint view, uint loop);
	class ViewFrame *getViewFrame(uint view, uint loop, uint frame);
	void checkViewFrame(uint view, uint loop, uint frame);

	void queueOrRunTextScript(ccInstance *instance, const Common::String &name, uint32 p1);
	void queueOrRunTextScript(ccInstance *instance, const Common::String &name, uint32 p1, uint32 p2);
	void queueOrRunTextScript(ccInstance *instance, const Common::String &name, const Common::Array<uint32> &params = Common::Array<uint32>());

	void runTextScript(ccInstance *instance, const Common::String &name,
		const Common::Array<uint32> &params = Common::Array<uint32>());

	struct ScriptImport resolveImport(const Common::String &name);
	class GlobalScriptState *getScriptState();

	Common::RandomSource *getRandomSource() { return _rnd; }

	uint32 getGameSpeed();
	void setGameSpeed(uint32 speed);

	byte getGameOption(uint index);

	Common::String getTranslation(const Common::String &text);
	Common::String replaceMacroTokens(const Common::String &text);

	// resolution system functions
	uint getFixedPixelSize(uint pixels);
	int convertToLowRes(int coord);
	int convertBackToHighRes(int coord);
	int multiplyUpCoordinate(int coords);
	void multiplyUpCoordinates(int32 &x, int32 &y);
	void multiplyUpCoordinatesRoundUp(int32 &x, int32 &y);
	int divideDownCoordinate(int coord);
	int divideDownCoordinateRoundUp(int coord);

	void blockUntil(BlockUntilType untilType, uint untilId = 0);

	void skipUntilCharacterStops(uint charId);
	void endSkippingUntilCharStops();
	void startSkippableCutscene();
	void startSkippingCutscene();
	void stopFastForwarding();

	void runDialogId(uint dialogId);
	int showDialogOptions(uint dialogId, uint sayChosenOption);

	void display(const Common::String &text) { displayAtY(-1, text); }
	void displayAtY(int y, const Common::String &text);
	void displayAt(int x, int y, int width, const Common::String &text, int blocking, int asSpeech = 0, bool isThought = false,
		int allowShrink = 0, bool overlayPositionFixed = false);
	void displayMain(int x, int y, int width, const Common::String &text, int blocking, int usingFont,
		int asSpeech, bool isThought, int allowShrink, bool overlayPositionFixed);
	void displaySpeech(const Common::String &text, uint charId, int x = -1, int y = -1, int width = -1, bool isThought = false);
	void displaySpeechAt(int x, int y, int width, uint charId, const Common::String &text);

	void invalidateScreen() { _needsUpdate = true; }
	void invalidateGUI() { _guiNeedsUpdate = true; }
	void invalidateBackground() { _backgroundNeedsUpdate = true; }

	Room *getCurrentRoom() { return _currentRoom; }
	void scheduleNewRoom(uint roomId);

	GameFile *_gameFile;
	class GameState *_state;
	class AGSAudio *_audio;
	AGSGraphics *_graphics;

	Character *getPlayerChar() { return _playerChar; }
	Common::Array<Character *> _characters;

	/** Prepend 'TARGET-' to the given filename. */
	Common::String wrapFilename(const Common::String &name) const { return _targetName + "-" + name; };

	int32 _newRoomPos;
	int32 _newRoomX;
	int32 _newRoomY;

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

	uint32 _startingRoom;
	uint32 _displayedRoom;
	Room *_currentRoom;
	Common::HashMap<uint, Room *> _loadedRooms;

	// new room state (this frame)
	NewRoomState _inNewRoomState;
	// new room state (from last time it was not None)
	NewRoomState _newRoomStateWas;

	int _leavesScreenRoomId;

	Common::Array<GameEvent> _queuedGameEvents;
	void queueGameEvent(GameEventType type, uint data1 = 0, uint data2 = (uint)-1000, uint data3 = 0);
	void runGameEventNow(GameEventType type, uint data1 = 0, uint data2 = (uint)-1000, uint data3 = 0);
	void processGameEvent(const GameEvent &event);
	void processAllGameEvents();
	bool runInteractionScript(struct InteractionScript *scripts, uint eventId, uint fallback = (uint)-1, bool isInventory = false);
	bool runInteractionEvent(struct NewInteraction *interaction, uint eventId, uint fallback = (uint)-1, bool isInventory = false);
	bool runInteractionCommandList(struct NewInteractionEvent &event, uint &commandsRunCount);
	void runUnhandledEvent(uint eventId);

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

	ScriptObject *_scriptMouseObject;
	ScriptObject *_gameStateGlobalsObject;
	ScriptObject *_saveGameIndexObject;
	ScriptObject *_scriptSystemObject;

	BlockUntilType _blockingUntil;
	uint _blockingUntilId;

	bool _saidSpeechLine;

	bool init();
	void adjustSizesForResolution();

	bool mainGameLoop();
	void tickGame(bool checkControls = false);
	void updateEvents(bool checkControls);
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
	int runDialogScript(struct DialogTopic &topic, uint dialogId, uint offset, uint optionId);
	int runDialogRequest(uint request);

	bool runScriptFunction(ccInstance *instance, const Common::String &name, const Common::Array<uint32> &params);
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
