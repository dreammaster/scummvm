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

#ifndef AGS_GAMESTATE_H
#define AGS_GAMESTATE_H

#include "engines/ags/scriptobj.h"

namespace AGS {

class GameState : public ScriptObject {
public:
	GameState(class AGSEngine *vm);
	void init();

	const char *getObjectTypeName() { return "GameState"; }
	uint32 readUint32(uint offset);
	bool writeUint32(uint offset, uint value);

	void setNormalFont(uint fontId);
	void setSpeechFont(uint fontId);

	uint32 userToInternalSkipSpeech(uint mode);

	// player's current score
	uint32 _score;
	// set by ProcessClick to last cursor mode used
	uint32 _usedMode;
	// >0 while in cutscenes/etc
	uint32 _disabledUserInterface;
	// obsolete
	uint32 _gscriptTimer;
	// whether we're in debug mode
	uint32 _debugMode;

	// obsolete
	Common::Array<uint32> _globalVars;

	// time left for auto-remove messages
	int32 _messageTime;
	// inventory item last used
	uint32 _usedInv;

	uint32 _invTop;
	uint32 _invNumDisplayed;
	uint32 _invNumOrder;
	uint32 _invNumInLine;

	// how quickly text is removed
	uint32 _textSpeed;
	// background used to paint default inv window
	uint32 _sierraInvColor;
	// animation speed of talking anims
	uint32 _talkAnimSpeed;
	// set by SetInvDimensions
	uint32 _invItemWidth;
	uint32 _invItemHeight;
	// color of outline fonts (default black)
	uint32 _speechTextShadow;
	// sierra-style speech swap sides
	uint32 _swapPortraitSide;
	// textwindow used for sierra-style speech
	uint32 _speechTextWindowGUI;
	// delay before moving following characters into new room
	uint32 _followChangeRoomTimer;
	// maximum possible score
	uint32 _totalScore;
	// how the user can skip normal Display windows
	uint32 _skipDisplay;
	// for backwards compatibility
	uint32 _noMultiLoopRepeat;
	// on_call finished in room
	uint32 _roomScriptFinished;
	// inv item they clicked on
	uint32 _usedInvOn;
	// no textwindow bgrnd when voice speech is used
	uint32 _noTextBgWhenVoice;

	// min/max width of dialog options text window
	uint32 _maxDialogOptionWidth;
	uint32 _minDialogOptionWidth;

	// fade out but instant in for hi-color
	uint32 _noHiColorFadeIn;
	// is background speech relative to game speed
	uint32 _bgSpeechGameSpeed;
	// whether to remove bg speech when DisplaySpeech is used
	uint32 _bgSpeechStayOnDisplay;
	// remove "&10" when calculating time for text to stay
	uint32 _unfactorSpeechFromTextLength;
	// loop this time before end of track (ms)
	uint32 _mp3LoopBeforeEnd;
	// how much to drop music volume by when speech is played
	uint32 _speechMusicDrop;
	// we are between a StartCutscene and EndCutscene
	uint32 _inCutscene;
	// player has elected to skip cutscene
	uint32 _fastForward;
	// width of current room (320-res co-ordinates)
	uint32 _roomWidth;
	// height of current room (320-res co-ordinates)
	uint32 _roomHeight;

	uint32 _gameSpeedModifier;
	uint32 _scoreSound;

	// value passed to RunAGSGame in previous game
	uint32 _takeoverData;

	uint32 _replayHotkey;
	uint32 _dialogOptionsX;
	uint32 _dialogOptionsY;

	uint32 _narratorSpeech;
	uint32 _ambientSoundsPersist;
	uint32 _lipsyncSpeed;
	uint32 _closeMouthSpeechTime;

	uint32 _disableAntialiasing;
	uint32 _textSpeedModifier;
	uint32 _textAlign;
	uint32 _speechBubbleWidth;
	uint32 _disableDialogParser;

	// the setting for this room
	uint32 _animBackgroundSpeed;

	uint32 _topBarBackColor;
	uint32 _topBarTextColor;
	uint32 _topBarBorderColor;
	uint32 _topBarBorderWidth;
	uint32 _topBarYPos;
	uint32 _topBarFont;

	uint32 _screenshotWidth;
	uint32 _screenshotHeight;

	uint32 _speechTextAlign;
	uint32 _autoUseWalkToPoints;
	uint32 _inventoryGreysOut;
	uint32 _skipSpeechSpecificKey;
	uint32 _abortKey;

	uint32 _fadeToRed;
	uint32 _fadeToGreen;
	uint32 _fadeToBlue;

	uint32 _showSingleDialogOption;
	uint32 _keepScreenDuringInstantTransition;
	uint32 _readDialogOptionColor;
	uint32 _stopDialogAtEnd;

	// FIXME: none of these are used, get rid of them?
	// user is recording their moves
	uint32 _recording;
	// playing back recording
	uint32 _playback;
	// step number for matching recordings
	uint16 _gameStep;
	// random seed
	uint32 _randomSeed;

	// player's current region
	uint32 _playerOnRegion;
	// the screen is currently black
	uint32 _screenIsFadedOut;
	uint32 _checkInteractionOnly;

	// for animating backgrounds
	uint32 _bgFrame;
	uint32 _bgAnimDelay;

	// before the volume drop
	uint32 _musicVolWas;

	uint16 _waitCounter;
	uint16 _mboundX1, _mboundX2, _mboundY1, _mboundY2;

	uint32 _fadeEffect;
	uint32 _bgFrameLocked;

	Common::Array<uint32> _globalScriptVars;

	uint32 _curMusicNumber;
	uint32 _musicRepeat;
	uint32 _musicMasterVolume;
	uint32 _digitalMasterVolume;

	Common::Array<byte> _walkableAreasOn;

	uint16 _screenFlipped;
	uint16 _offsetsLocked;

	uint32 _enteredAtX, _enteredAtY;
	uint32 _enteredEdge;

	// speech mode; negative if disabled
	int32 _wantSpeech;

	uint32 _cantSkipSpeech;

	// script timers; they count down to 1, once per frame, after being set
	Common::Array<uint32> _scriptTimers;

	uint32 _soundVolume;
	uint32 _speechVolume;

	uint32 _normalFont;
	uint32 _speechFont;

	uint32 _keySkipWait;
	uint32 _swapPortraitLastChar;
	uint32 _separateMusicLib;
	uint32 _inConversation;
	uint32 _screenTint;

	Common::Array<uint16> _parsedWords;
	Common::String _badParsedWord;

	uint32 _rawColor;
	Common::Array<uint32> _rawModified;

	Common::Array<uint16> _fileNumbers;

	uint32 _roomChanges;
	uint32 _mouseCursorHidden;

	uint32 _silentMIDI;
	uint32 _silentMIDIChannel;
	// remember what the loop flag was when this music started
	uint32 _currentMusicRepeating;

	uint32 _shakeDelay, _shakeAmount, _shakeLength;

	uint32 _roomTintRed, _roomTintGreen, _roomTintBlue, _roomTintLevel, _roomTintLight;

	uint32 _endCutsceneMusic;
	uint32 _skipUntilCharStops;
	uint32 _getLocNameLastTime;
	uint32 _getLocNameSaveCursor;
	uint32 _restoreCursorModeTo;
	uint32 _restoreCursorImageTo;

	Common::Array<uint16> _musicQueue;
	// TODO: Common::Array<QueuedAudioItem> _newMusicQueue;

	uint16 _crossfadingOutChannel;
	uint16 _crossfadeStep;
	uint16 _crossfadeOutVolumePerStep;
	uint16 _crossfadeInitialVolumeOut;
	uint16 _crossfadingInChannel;
	uint16 _crossfadeInVolumePerStep;
	uint16 _crossfadeFinalVolumeIn;

	Common::String _takeoverFrom;
	Common::String _playMp3FileName;
	Common::Array<Common::String> _globalStrings;
	Common::String _lastParserEntry;
	Common::String _gameName;

	uint32 _groundLevelAreasDisabled;
	uint32 _nextScreenTransition;
	uint32 _gammaAdjustment;

	// Hide Player Character ticked
	uint16 _temporarilyTurnedOffCharacter;

	uint16 _invBackwardsCompatibility;

	Common::Array<uint32> _guiDrawOrder;

	Common::Array<Common::String> _doOnceTokens;

	uint32 _textMinDisplayTimeMs;
	uint32 _ignoreUserInputAfterTextTimeoutMs;
	uint32 _ignoreUserInputUntilTime;

	Common::Array<uint32> _defaultAudioTypeVolumes;

protected:
	AGSEngine *_vm;
};

} // End of namespace AGS

#endif // AGS_GAMESTATE_H
