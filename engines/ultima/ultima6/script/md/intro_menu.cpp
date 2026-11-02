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

#include "common/keyboard.h"
#include "ultima/ultima6/conf/configuration.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/md/intro.h"
#include "ultima/ultima6/script/md/md_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

static const int MAX_NAME_LENGTH = 13;

/** One step of Dr Freud's interview: the line to show, how he looks and what input ends it */
struct CreationRow {
	int _text; // Negative for lines said by the player, 0 ends a question
	int _eyeSprite;
	bool _canMovePen;
	int _action; // -1 any key, -2 a or b, -3 a, b or c, -4 the player's name
};

const CreationRow CREATION_TABLE[] = {
	{ -1, 1, false, -1 },
	{ -2, 1, false, -1 },
	{ 3, 2, false, -1 },
	{ 4, 3, true, -1 },
	{ 5, 2, false, -4 },
	{ 6, 3, true, -1 },
	{ 7, 1, false, -2 },
	{ 0, 2, false, -1 },
	{ 8, 3, true, -1 },
	{ 9, 1, false, -1 },
	{ 0, 2, false, -1 },
	{ 10, 3, true, -1 },
	{ 11, 1, false, -1 },
	{ 0, 2, false, -1 },
	{ 12, 2, false, -1 },
	{ 13, 1, false, -1 },
	{ 14, 1, false, -3 },
	{ 0, 2, false, -1 },
	{ 15, 3, true, -1 },
	{ 0, 2, false, -1 },
	{ 16, 3, true, -1 },
	{ 17, 1, false, -2 },
	{ 0, 2, false, -1 },
	{ 18, 3, true, -1 },
	{ 19, 2, false, -2 },
	{ 0, 2, false, -1 },
	{ 20, 3, true, -1 },
	{ 21, 2, false, -2 },
	{ 0, 2, false, -1 },
	{ 22, 3, true, -2 },
	{ 0, 1, false, -1 },
	{ 23, 3, true, -1 },
	{ 24, 2, false, -1 },
	{ 0, 2, false, -1 },
	{ 25, 3, true, -1 },
	{ 26, 3, true, -1 },
	{ 0, 2, false, -1 },
	{ 27, 3, true, -1 },
	{ 28, 1, false, -2 },
	{ 0, 2, false, -1 },
	{ 29, 3, true, -1 },
	{ 30, 2, false, -1 },
	{ 0, 2, false, -1 },
	{ 31, 3, true, -1 },
	{ 32, 2, false, -1 },
	{ 0, 2, false, -1 },
	{ 33, 3, true, -1 },
	{ 34, 1, false, -2 },
	{ 0, 2, false, -1 },
	{ 35, 3, true, -1 },
	{ 36, 1, false, -1 },
	{ 0, 2, false, -1 },
	{ 37, 3, true, -1 },
	{ 38, 2, false, -1 },
	{ 0, 2, false, -1 },
	{ 39, 3, true, -1 },
	{ 40, 2, false, -1 },
	{ 0, 2, false, -1 },
	{ 41, 3, true, -1 },
	{ 42, 2, false, -1 },
	{ 0, 2, false, -1 },
	{ 43, 3, true, -1 },
	{ 44, 1, false, -2 },
	{ 0, 2, false, -1 },
	{ 45, 3, true, -1 },
	{ 0, 2, false, -1 },
	{ 46, 3, true, -1 },
	{ 47, 1, false, -1 },
	{ 0, 2, false, -1 },
	{ 48, 3, true, -1 },
	{ 49, 1, false, -1 },
	{ 0, 2, false, -1 },
	{ 50, 3, true, -1 },
	{ 51, 2, false, -2 },
	{ 0, 2, false, -1 },
	{ 52, 3, true, -1 },
	{ 53, 2, false, -1 },
	{ 0, 2, false, -1 },
	{ 54, 1, false, -1 },
	{ 55, 3, true, -1 },
	{ 0, 2, false, -1 },
	{ 56, 2, false, -1 },
	{ 57, 3, true, -1 },
	{ 0, 2, false, -1 },
	{ 58, 3, true, -1 },
	{ 59, 2, false, -2 },
	{ 0, 2, false, -1 },
	{ 60, 3, true, -1 },
	{ 61, 1, false, -1 },
	{ 0, 2, false, -1 },
	{ 62, 3, true, -1 },
	{ 63, 1, false, -1 },
	{ 0, 2, false, -1 }
};

} // End of anonymous namespace

Common::String Intro::insertPlayerName(const Common::String &text) const {
	Common::String output;
	Common::String remaining = text;

	for (;;) {
		const char *found = strstr(remaining.c_str(), "$P");
		if (!found)
			break;

		uint pos = found - remaining.c_str();
		output += remaining.substr(0, pos) + _playerName;
		remaining = remaining.substr(pos + 2);
	}

	return output + remaining;
}

bool Intro::collectPlayerName() {
	Common::String nameText = _nameSprite->text;
	int len = nameText.size();

	_nameSprite->visible = true;

	int input = pollForInput();
	if (input != INPUT_NONE) {
		if (_shouldExit)
			return false;

		Common::String trimmed = nameText;
		trimmed.trim();

		if ((input == Common::KEYCODE_BACKSPACE || input == INPUT_KEY_LEFT) && len > 0) {
			_nameSprite->text = nameText.substr(0, len - 1);
			_charIndex = (len == 1) ? 0 : (byte)nameText[len - 2];
			--len;
		} else if ((input == Common::KEYCODE_RETURN || input == Common::KEYCODE_KP_ENTER) && !trimmed.empty()) {
			_nameSprite->visible = false;
			_cursorSprite->visible = false;
			_playerName = trimmed;
			return true;
		} else if (input == INPUT_KEY_UP) {
			if (_charIndex == 0)
				_charIndex = (len > 0) ? Common::KEYCODE_a : 65;
			else if (_charIndex <= 32)
				_charIndex = 48;
			else if (_charIndex >= 57 && _charIndex < 65)
				_charIndex = 65;
			else if (_charIndex >= 90 && _charIndex < 97)
				_charIndex = 97;
			else if (_charIndex >= 122)
				_charIndex = 32;
			else
				++_charIndex;

			if (keyToChar(_charIndex)) {
				if (len > 0)
					nameText = nameText.substr(0, len - 1);
				_nameSprite->text = nameText + keyToChar(_charIndex);
			}
		} else if (input == INPUT_KEY_DOWN) {
			if (_charIndex == 0)
				_charIndex = (len > 0) ? 122 : 90;
			else if (_charIndex == 65)
				_charIndex = 57;
			else if (_charIndex == 97)
				_charIndex = 90;
			else if (_charIndex <= 32)
				_charIndex = 122;
			else if (_charIndex <= 48)
				_charIndex = 32;
			else
				--_charIndex;

			if (keyToChar(_charIndex)) {
				if (len > 0)
					nameText = nameText.substr(0, len - 1);
				_nameSprite->text = nameText + keyToChar(_charIndex);
			}
		} else if (input == INPUT_KEY_RIGHT && len < MAX_NAME_LENGTH) {
			_charIndex = Common::KEYCODE_a;
			_nameSprite->text = nameText + "a";
		} else if (keyToChar(input) && len < MAX_NAME_LENGTH) {
			_nameSprite->text = nameText + keyToChar(input);
			++len;
		}
	}

	if (len == MAX_NAME_LENGTH) {
		_cursorSprite->visible = false;
	} else {
		_cursorSprite->x = 120 + _cutscene->get_text_width(_nameSprite->text.c_str());

		if (_cursorTimer == 0) {
			_cursorTimer = 8;
			_cursorSprite->visible = !_cursorSprite->visible;
		} else {
			--_cursorTimer;
		}
	}

	return false;
}

void Intro::updateFreud(Freud &freud) {
	const ImageTable &images = *freud._images;

	if (freud._blinkTimer < 200) {
		if (freud._timer == 0) {
			freud._timer = randomRange(100, 350);
			freud._state = (freud._state == FREUD_STARING) ? FREUD_WRITING : FREUD_STARING;
		} else {
			--freud._timer;
		}

		if (freud._state == FREUD_STARING) {
			freud._eyesSprite->image = freud._canMovePen ? images[0][1] : images[0][freud._eyesSpriteIdx];
		} else if (freud._state == FREUD_WRITING && freud._canMovePen) {
			freud._eyesSprite->image = images[0][freud._eyesSpriteIdx];

			if (freud._timer % 4 == 0) {
				freud._penSprite->image = images[2][randomRange(0, 6)];
				freud._penXOff += randomRange(0, 3);

				if (freud._penXOff >= 30) {
					freud._penXOff %= 30;
					freud._penYOff = (freud._penYOff + 2) % 20;
				}

				freud._penSprite->x = 128 - freud._penXOff;
				freud._penSprite->y = 83 + freud._penYOff;
			}
		}
	}

	if (freud._blinkTimer == 200) {
		freud._eyesSprite->image = images[0][(freud._eyesSpriteIdx == 2) ? 5 : 4];
	} else if (freud._blinkTimer == 215) {
		if (freud._state == FREUD_WRITING || !freud._canMovePen)
			freud._eyesSprite->image = images[0][freud._eyesSpriteIdx];
		else
			freud._eyesSprite->image = images[0][1];

		freud._blinkTimer = -1;
	}

	++freud._blinkTimer;
}

int Intro::askQuestion(int questionIdx, CSSprite *text, Freud &freud) {
	int keyInput = ANSWER_NONE;
	int textOffset = CREATION_TABLE[questionIdx]._text;

	while (textOffset != 0) {
		const CreationRow &row = CREATION_TABLE[questionIdx];

		text->text_color = (textOffset < 0) ? 4 : 6;

		uint lineIdx = ABS(textOffset);
		text->text = insertPlayerName(lineIdx < freud._textTbl.size() ? freud._textTbl[lineIdx] : Common::String());

		freud._eyesSpriteIdx = row._eyeSprite;
		freud._eyesSprite->image = (*freud._images)[0][freud._eyesSpriteIdx];
		freud._blinkTimer = 0;
		freud._state = FREUD_STARING;
		freud._canMovePen = row._canMovePen;

		bool continueLoop = true;
		while (continueLoop) {
			if (row._action == -1) {
				if (pollForInput() != INPUT_NONE)
					continueLoop = false;
			} else if (row._action == -2) {
				int input = pollForInput();
				if (input == Common::KEYCODE_a || input == Common::KEYCODE_b) {
					keyInput = input;
					continueLoop = false;
				}
			} else if (row._action == -3) {
				int input = pollForInput();
				if (input == Common::KEYCODE_a || input == Common::KEYCODE_b || input == Common::KEYCODE_c) {
					keyInput = input;
					continueLoop = false;
				}
			} else if (row._action == -4) {
				if (collectPlayerName())
					continueLoop = false;
			}

			if (_shouldExit)
				return ANSWER_NONE;

			updateFreud(freud);
			canvasUpdate();
		}

		++questionIdx;
		textOffset = CREATION_TABLE[questionIdx]._text;
	}

	return keyInput;
}

void Intro::question1AnswerA(CSSprite *text, Freud &freud, int randHigh, int randLow) {
	const int A = Common::KEYCODE_a;
	const int B = Common::KEYCODE_b;

	askQuestion(18, text, freud);
	int var14 = askQuestion(20, text, freud);

	if (var14 == A) {
		int var16 = askQuestion(23, text, freud);

		if (var16 == A) {
			int answer = askQuestion(29, text, freud);

			if (answer == A) {
				_avatarStr = randHigh;
				_avatarInt = randLow;
				askQuestion(_genderAnswer == A ? 31 : 34, text, freud);
			} else if (answer == B) {
				_avatarInt = randHigh;
				_avatarStr = randLow;
			}
		} else if (var16 == B) {
			int answer = askQuestion(37, text, freud);

			if (answer == A) {
				_avatarInt = randHigh;
				_avatarDex = randLow;
				askQuestion(40, text, freud);
			} else if (answer == B) {
				_avatarDex = randHigh;
				_avatarInt = randLow;
				askQuestion(43, text, freud);
			}
		}
	} else if (var14 == B) {
		int var16 = askQuestion(26, text, freud);

		if (var16 == A) {
			int answer = askQuestion(46, text, freud);

			if (answer == A) {
				_avatarStr = randHigh;
				_avatarDex = randLow;
				askQuestion(_genderAnswer == A ? 49 : 52, text, freud);
			} else if (answer == B) {
				_avatarDex = randHigh;
				_avatarStr = randLow;
				askQuestion(_genderAnswer == A ? 58 : 55, text, freud);
			}
		} else if (var16 == B) {
			int answer = askQuestion(61, text, freud);

			if (answer == A) {
				_avatarInt = randHigh;
				_avatarDex = randLow;
				askQuestion(64, text, freud);
			} else if (answer == B) {
				_avatarDex = randHigh;
				_avatarInt = randLow;
				askQuestion(66, text, freud);
			}
		}
	}
}

void Intro::question1AnswerB(CSSprite *text, Freud &freud, int randHigh, int randLow) {
	const int A = Common::KEYCODE_a;
	const int B = Common::KEYCODE_b;

	int var14 = askQuestion(26, text, freud);

	if (var14 == A) {
		int var16 = askQuestion(23, text, freud);

		if (var16 == A) {
			int answer = askQuestion(29, text, freud);

			if (answer == A) {
				_avatarStr = randHigh;
				_avatarInt = randLow;
				askQuestion(_genderAnswer == A ? 31 : 34, text, freud);
			} else if (answer == B) {
				_avatarInt = randHigh;
				_avatarStr = randLow;
			}
		} else if (var16 == B) {
			int answer = askQuestion(37, text, freud);

			if (answer == A) {
				_avatarInt = randHigh;
				_avatarDex = randLow;
				askQuestion(40, text, freud);
			} else if (answer == B) {
				_avatarDex = randHigh;
				_avatarInt = randLow;
				askQuestion(43, text, freud);
			}
		}
	} else if (var14 == B) {
		int var16 = askQuestion(20, text, freud);

		if (var16 == A) {
			int answer = askQuestion(72, text, freud);

			if (answer == A) {
				_avatarInt = randHigh;
				_avatarStr = randLow;
				askQuestion(75, text, freud);
			} else if (answer == B) {
				_avatarStr = randHigh;
				_avatarInt = randLow;
				askQuestion(78, text, freud);
			}
		} else if (var16 == B) {
			int answer = askQuestion(46, text, freud);

			if (answer == A) {
				_avatarStr = randHigh;
				_avatarDex = randLow;
				askQuestion(_genderAnswer == A ? 49 : 52, text, freud);
			} else if (answer == B) {
				_avatarDex = randHigh;
				_avatarStr = randLow;
				askQuestion(_genderAnswer == A ? 58 : 55, text, freud);
			}
		}
	}
}

void Intro::question1AnswerC(CSSprite *text, Freud &freud, int randHigh, int randLow) {
	const int A = Common::KEYCODE_a;
	const int B = Common::KEYCODE_b;

	askQuestion(81, text, freud);
	int var14 = askQuestion(23, text, freud);

	if (var14 == A) {
		int var16 = askQuestion(26, text, freud);

		if (var16 == A) {
			int answer = askQuestion(46, text, freud);

			if (answer == A) {
				_avatarStr = randHigh;
				_avatarDex = randLow;
				askQuestion(_genderAnswer == A ? 49 : 52, text, freud);
			} else if (answer == B) {
				_avatarDex = randHigh;
				_avatarStr = randLow;
				askQuestion(_genderAnswer == A ? 58 : 55, text, freud);
			}
		} else if (var16 == B) {
			int answer = askQuestion(37, text, freud);

			if (answer == A) {
				_avatarInt = randHigh;
				_avatarDex = randLow;
				askQuestion(40, text, freud);
			} else if (answer == B) {
				_avatarDex = randHigh;
				_avatarInt = randLow;
				askQuestion(43, text, freud);
			}
		}
	} else if (var14 == B) {
		int var16 = askQuestion(20, text, freud);

		if (var16 == A) {
			int answer = askQuestion(29, text, freud);

			if (answer == A) {
				_avatarStr = randHigh;
				_avatarInt = randLow;
				askQuestion(_genderAnswer == A ? 31 : 34, text, freud);
			} else if (answer == B) {
				_avatarInt = randHigh;
				_avatarStr = randLow;
			}
		} else if (var16 == B) {
			int answer = askQuestion(84, text, freud);

			if (answer == A) {
				_avatarDex = randHigh;
				_avatarStr = randLow;
				askQuestion(87, text, freud);
			} else if (answer == B) {
				_avatarStr = randHigh;
				_avatarDex = randLow;
				askQuestion(90, text, freud);
			}
		}
	}
}

bool Intro::createCharacter() {
	_cutscene->hide_sprites();

	ImageTable createImages;
	Common::Array<Common::String> textTbl = _cutscene->load_text("scenetxt.lzc", 5);
	if (textTbl.empty() || !loadImages("create.lzc", createImages))
		return false;

	CSSprite *text = newSprite(nullptr, 0, 160, true);
	text->text = textTbl[0];
	text->text_color = 2;
	text->text_align = 2;

	newSprite(createImages[0][0], 0, 24, true);
	CSSprite *eyesSprite = newSprite(createImages[0][4], 0, 24, true);
	CSSprite *penSprite = newSprite(createImages[2][0], 128, 83, true);
	newSprite(createImages[1][0], 89, 109, true);

	Freud freud;
	freud._eyesSprite = eyesSprite;
	freud._penSprite = penSprite;
	freud._images = &createImages;
	freud._textTbl = textTbl;

	musicPlay("mdd_mus.lzc", 7);

	int randHigh = randomRange(24, 26);
	int randLow = randomRange(22, 24);
	int statBase = randomRange(18, 22);

	_avatarStr = statBase;
	_avatarDex = statBase;
	_avatarInt = statBase;

	int gender = 0;

	_genderAnswer = askQuestion(0, text, freud);
	if (_genderAnswer == Common::KEYCODE_a) {
		gender = 0; // Male
		askQuestion(8, text, freud);
	} else if (_genderAnswer == Common::KEYCODE_b) {
		gender = 1; // Female
		askQuestion(11, text, freud);
	}

	int answer = askQuestion(14, text, freud);

	if (answer == Common::KEYCODE_a)
		question1AnswerA(text, freud, randHigh, randLow);
	else if (answer == Common::KEYCODE_b)
		question1AnswerB(text, freud, randHigh, randLow);
	else if (answer == Common::KEYCODE_c)
		question1AnswerC(text, freud, randHigh, randLow);

	if (_shouldExit)
		return false;

	askQuestion(69, text, freud);

	Configuration *config = _cutscene->get_config();
	config->set("config/newgame", true);
	config->set("config/newgamedata/name", _playerName);
	config->set("config/newgamedata/gender", gender);
	config->set("config/newgamedata/str", _avatarStr);
	config->set("config/newgamedata/dex", _avatarDex);
	config->set("config/newgamedata/int", _avatarInt);

	fadeOut(6);
	return true;
}

void Intro::updateMenuCursor() {
	static const int BOX_Y[4] = { 130, 147, 164, 181 };

	_menuCursorSprite->y = BOX_Y[_menuIdx];
}

void Intro::setMenuCursorPos(int newPos) {
	_menuIdx = newPos;
	updateMenuCursor();
}

void Intro::showMenuCursor() {
	if (!_mouseCursorVisible) {
		setMousePointer(9);
		showMouseCursor(true);
		_mouseCursorVisible = true;
	}
}

void Intro::hideMenuCursor() {
	if (_mouseCursorVisible) {
		showMouseCursor(false);
		_mouseCursorVisible = false;
	}
}

bool Intro::executeMenuItem(int cursorPos) {
	if (cursorPos != -1)
		setMenuCursorPos(cursorPos);

	hideMenuCursor();

	if (_menuIdx == 0) { // The story so far
		runIntroduction();
	} else if (_menuIdx == 1) { // Create a character
		if (createCharacter())
			return true; // Starting a new game
	} else if (_menuIdx == 2) { // Journey onward
		return true;
	} else if (_menuIdx == 3) { // About Martian Dreams
		aboutMartianDreams();
	}

	_shouldExit = false;
	_cutscene->hide_sprites();
	_cutscene->set_screen_opacity(0xff);
	musicStop();
	_menuBgSprite->visible = true;
	_menuCursorSprite->visible = true;

	return false;
}

bool Intro::mainMenu() {
	_cutscene->hide_sprites();
	musicStop();
	_shouldExit = false;

	ImageTable images;
	if (!loadImages("mdmenu.lzc", images))
		return true;

	_menuBgSprite = newSprite(images[0][0], 0, 0, true);

	fadeIn();

	_menuCursorSprite = newSprite(images[0][2], 26, 0, true);
	updateMenuCursor();
	_cutscene->set_update_interval(10);

	for (;;) {
		canvasUpdate();
		int input = inputPoll(true);

		if (engineShouldQuit())
			return true;

		if (input == INPUT_NONE)
			continue;

		_cutscene->set_update_interval(25);

		if (input == Common::KEYCODE_q) {
			return true;
		} else if (input == Common::KEYCODE_RETURN || input == Common::KEYCODE_SPACE || input == Common::KEYCODE_KP_ENTER) {
			if (executeMenuItem())
				return false;
		} else if (input == Common::KEYCODE_r || input == Common::KEYCODE_i) { // Run the introduction
			executeMenuItem(0);
		} else if (input == Common::KEYCODE_c) { // Create a character
			if (executeMenuItem(1))
				return false;
		} else if (input == Common::KEYCODE_j || input == Common::KEYCODE_g) { // Journey onward, or continue the game
			executeMenuItem(2);
			return false;
		} else if (input == Common::KEYCODE_a) { // About Martian Dreams
			executeMenuItem(3);
		} else if (input == INPUT_KEY_DOWN || input == Common::KEYCODE_KP2) {
			setMenuCursorPos((_menuIdx + 1) % 4);
		} else if (input == INPUT_KEY_UP || input == Common::KEYCODE_KP8) {
			setMenuCursorPos(_menuIdx == 0 ? 3 : _menuIdx - 1);
		} else if (input == INPUT_MOUSE_MOTION || input == INPUT_MOUSE_CLICK) {
			showMenuCursor();
			int x = mouseX();
			int y = mouseY();

			if (x > 57 && x < 260 && y > 130) {
				if (y > 130 && y < 148)
					setMenuCursorPos(0);
				else if (y > 147 && y < 164)
					setMenuCursorPos(1);
				else if (y > 163 && y < 181)
					setMenuCursorPos(2);
				else if (y > 180)
					setMenuCursorPos(3);

				if (input == INPUT_MOUSE_CLICK && executeMenuItem())
					return false;
			}
		}

		_cutscene->set_update_interval(10);
	}
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
