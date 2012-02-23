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

#include "engines/ags/ags.h"
#include "engines/ags/constants.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/gamestate.h"

namespace AGS {

GameState::GameState(AGSEngine *vm) : _vm(vm) {
	_wantSpeech = -2;

	_recording = 0;
	_playback = 0;

	_takeoverData = 0;

	_gameStep = 0;
}

void GameState::init() {
	// init_game_settings
	_score = 0;
	_sierraInvColor = 7;
	_talkAnimSpeed = 5;

	_invItemWidth = 40;
	_invItemHeight = 22;

	_messageTime = (uint32)-1;
	_disabledUserInterface = 0;
	_gscriptTimer = (uint32)-1;

	_debugMode = _vm->getGameOption(OPT_DEBUGMODE);

	_invTop = 0;
	_invNumDisplayed = 0;
	_invNumOrder = 0;

	_textSpeed = 15;
	_textMinDisplayTimeMs = 1000;
	_ignoreUserInputAfterTextTimeoutMs = 500;
	_ignoreUserInputUntilTime = 0;

	_lipsyncSpeed = 15;
	_closeMouthSpeechTime = 10;
	_disableAntialiasing = 0;

	_roomTintLevel = 0;
	_roomTintLight = 255;

	_textSpeedModifier = 0;

	_textAlign = SCALIGN_LEFT;
	// Make the default alignment to the right with right-to-left text
	if (_vm->getGameOption(OPT_RIGHTLEFTWRITE))
		_textAlign = SCALIGN_RIGHT;

	_speechBubbleWidth = _vm->getFixedPixelSize(100);
	_bgFrame = 0;
	_bgFrameLocked = 0;
	_bgAnimDelay = 0;
	_animBackgroundSpeed = 0;

	_silentMIDI = 0;
	_currentMusicRepeating = 0;

	_skipUntilCharStops = (uint32)-1;
	_getLocNameLastTime = (uint32)-1;
	_getLocNameSaveCursor = (uint32)-1;
	_restoreCursorModeTo = (uint32)-1;
	_restoreCursorImageTo = (uint32)-1;

	_groundLevelAreasDisabled = 0;
	_nextScreenTransition = (uint32)-1;
	_temporarilyTurnedOffCharacter = (uint16)-1;
	_invBackwardsCompatibility = 0;
	_gammaAdjustment = 100;
	_doOnceTokens.clear();
	_musicQueue.clear();

	_shakeLength = 0;
	_waitCounter = 0;
	_keySkipWait = 0;

	_curMusicNumber = (uint32)-1;
	_musicRepeat = 1;
	_musicMasterVolume = 160;
	_digitalMasterVolume = 100;

	_screenFlipped = 0;
	_offsetsLocked = 0;

	_cantSkipSpeech = userToInternalSkipSpeech(_vm->getGameOption(OPT_NOSKIPTEXT));

	_soundVolume = 255;
	_speechVolume = 255;

	_normalFont = 0;
	_speechFont = 1;
	_speechTextShadow = 16;

	_screenTint = (uint32)-1;

	_badParsedWord.clear();

	_swapPortraitSide = 0;
	_swapPortraitLastChar = (uint32)-1;

	_inConversation = 0;
	_skipDisplay = 3;
	_noMultiLoopRepeat = 0;
	_inCutscene = 0;
	_fastForward = 0;

	// FIXME: _totalScore = _vm->_gameFile->_totalScore

	_roomScriptFinished = 0;
	_noTextBgWhenVoice = 0;
	_maxDialogOptionWidth = _vm->getFixedPixelSize(180);

	_noHiColorFadeIn = 0;
	_bgSpeechGameSpeed = 0;
	_bgSpeechStayOnDisplay = 0;

	_unfactorSpeechFromTextLength = 0;

	_mp3LoopBeforeEnd = 70;
	_speechMusicDrop = 60;

	_roomChanges = 0;
	_checkInteractionOnly = 0;

	_replayHotkey = 318; // alt+r

	_dialogOptionsX = 0;
	_dialogOptionsY = 0;

	_minDialogOptionWidth = 0;
	_disableDialogParser = 0;
	_ambientSoundsPersist = 0;
	_screenIsFadedOut = 0;
	_playerOnRegion = 0;

	_topBarBackColor = 8;
	_topBarTextColor = 16;
	_topBarBorderColor = 8;
	_topBarBorderWidth = 1;
	_topBarYPos = 25;
	_topBarFont = (uint32)-1;

	_screenshotWidth = 160;
	_screenshotHeight = 100;

	_speechTextAlign = SCALIGN_CENTRE;

	_autoUseWalkToPoints = 1;
	_inventoryGreysOut = 0;
	_skipSpeechSpecificKey = 0;
	_abortKey = 324; // alt+x

	_fadeToRed = 0;
	_fadeToGreen = 0;
	_fadeToBlue = 0;

	_showSingleDialogOption = 0;
	_keepScreenDuringInstantTransition = 0;
	_readDialogOptionColor = (uint32)-1;
	_narratorSpeech = _vm->_gameFile->_playerChar;

	_crossfadingOutChannel = 0;

	_speechTextWindowGUI = _vm->getGameOption(OPT_TWCUSTOM);
	if (_speechTextWindowGUI == 0)
		_speechTextWindowGUI = (uint32)-1;

	_gameName = _vm->_gameFile->_gameName;

	_lastParserEntry.clear();

	_followChangeRoomTimer = 150;

	// FIXME: _rawModified
	_gameSpeedModifier = 0;

	/* FIXME if (debugFlags & DBG_DEBUGMODE)
		_debugMode = 1; */

	// FIXME: global
	// _guiDisabledStyle = convertGuiDisabledStyle(_vm->getGameOption(OPT_DISABLEOFF);

	// FIXME: _walkableAreasOn -> 1, MAX_WALK_AREAS+1

	_scriptTimers.resize(MAX_TIMERS);
	for (uint i = 0; i < _scriptTimers.size(); ++i)
		_scriptTimers[i] = 0;

	// FIXME: _defaultAudioTypeVolumes -> -1, MAX_AUDIO_TYPES

	// reset graphical script variables (for compatibility with older games)
	// FIXME: _globalVars -> 0, MAXGLOBALVARS
	// FIXME: _globalStrings -> "", MAXGLOBALSTRINGS
	// FIXME: _lastSoundPlayed -> -1, MAX_SOUND_CHANNELS

	// FIXME: make sure the init_translation and update_invorder stuff is done somewhere
}

void GameState::setNormalFont(uint fontId) {
	if (fontId >= _vm->_gameFile->_fonts.size())
		error("setNormalFont: %d is too high (only %d fonts)", fontId, _vm->_gameFile->_fonts.size());

	_normalFont = fontId;
}

void GameState::setSpeechFont(uint fontId) {
	if (fontId >= _vm->_gameFile->_fonts.size())
		error("setSpeechFont: %d is too high (only %d fonts)", fontId, _vm->_gameFile->_fonts.size());

	_speechFont = fontId;
}

uint32 GameState::userToInternalSkipSpeech(uint mode) {
	switch (mode) {
	case 0:
		// click mouse or key to skip
		return SKIP_AUTOTIMER | SKIP_KEYPRESS | SKIP_MOUSECLICK;
	case 1:
		// key only
		return SKIP_AUTOTIMER | SKIP_KEYPRESS;
	case 2:
		// can't skip at all
		return SKIP_AUTOTIMER;
	case 3:
		// only on keypress, no auto timer
		return SKIP_KEYPRESS | SKIP_MOUSECLICK;
	case 4:
		// mouse only
		return SKIP_AUTOTIMER | SKIP_MOUSECLICK;
	default:
		error("userToInternalSkipSpeech: invalid skip speech mode %d", mode);
	}
}

} // End of namespace AGS
