/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NUVIE_SCRIPT_MD_INTRO_H
#define NUVIE_SCRIPT_MD_INTRO_H

#include "common/str.h"
#include "ultima/ultima6/script/common/cutscene_script.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

/** The Martian Dreams intro: the title sequence, the story so far, character creation and the main menu */
class Intro : public CutsceneScript {
private:
	enum FreudState {
		FREUD_STARING = 0,
		FREUD_WRITING = 1
	};

	/** Dr Freud interviewing the player during character creation */
	struct Freud {
		CSSprite *_eyesSprite = nullptr;
		int _eyesSpriteIdx = 4;
		int _blinkTimer = 0;
		CSSprite *_penSprite = nullptr;
		int _penXOff = 0;
		int _penYOff = 0;
		FreudState _state = FREUD_STARING;
		int _timer = 0;
		bool _canMovePen = false;
		const ImageTable *_images = nullptr;
		Common::Array<Common::String> _textTbl;
	};

	enum {
		ANSWER_NONE = -1
	};

	// Character creation state
	Common::String _playerName;
	int _genderAnswer = ANSWER_NONE;
	int _avatarStr = 0;
	int _avatarDex = 0;
	int _avatarInt = 0;
	CSSprite *_nameSprite = nullptr;
	CSSprite *_cursorSprite = nullptr;
	int _cursorTimer = 0;
	int _charIndex = 0;

	// Main menu state
	int _menuIdx = 0;
	CSSprite *_menuCursorSprite = nullptr;
	CSSprite *_menuBgSprite = nullptr;
	bool _mouseCursorVisible = false;

	/** Waits for the time to pass or input, returning true if the player asked to leave the intro */
	bool waitForExit(int cycles);

	void originFxSequence();
	void showLogos();
	void flashEffect(CSImage *image, int textDelay = 300);
	void showFairGround();
	void showLowell();
	void showFuse();
	void showFlash();
	void showFlight();
	void showCabin();
	void showMarsFlash();
	void playIntro();

	void showHome();
	void showMap();
	void showHike();
	void showLabPresentDay();
	void showLab1895();
	void runIntroduction();

	Common::String insertPlayerName(const Common::String &text) const;
	bool collectPlayerName();
	void updateFreud(Freud &freud);
	int askQuestion(int questionIdx, CSSprite *text, Freud &freud);
	void question1AnswerA(CSSprite *text, Freud &freud, int randHigh, int randLow);
	void question1AnswerB(CSSprite *text, Freud &freud, int randHigh, int randLow);
	void question1AnswerC(CSSprite *text, Freud &freud, int randHigh, int randLow);
	bool createCharacter();

	void updateMenuCursor();
	void setMenuCursorPos(int newPos);
	void showMenuCursor();
	void hideMenuCursor();
	bool executeMenuItem(int cursorPos = -1);

	/** Returns true if the player chose to quit, or false to journey onward */
	bool mainMenu();

public:
	void play();
};

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
