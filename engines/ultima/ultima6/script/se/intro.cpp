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
#include "ultima/ultima6/script/common/cutscene_script.h"
#include "ultima/ultima6/script/se/se_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace SE {

namespace {

static const char *const STORY_SO_FAR_TEXT[40] = {
	"You had disturbing dreams of a faraway jungle,",
	"an engangered princess, and the black",
	"moonstone you carried away from Britannia in",
	"your last adventure there.",
	"The spirit of Lord British commanded you to",
	"find out all you could about the stone.",
	"",
	"",
	"So you took it to your old friend Dr. Rafkin,",
	"curator of the local museum of natural",
	"history, hoping that he could unravel the",
	"mystery.",
	"At Dr. Rafkin's, you met ace reporter and",
	"\"Ultimate Adventures\" correspondent Jimmy",
	"Malone.  Malone's a little too nosy to suit you...",
	"",
	"...but he kept his wits when everything went",
	"haywire, when Dr. Rafkin's experiments on your",
	"moonstone sent the whole museum lab through",
	"a bizarre black moongate!",
	"You were shocked to lift your head and see",
	"jungle around you: Jungle, and this enormous",
	"creature descending... descending upon--",
	"",
	"Her! The woman you dreamt of! A fiery tribal",
	"she-warrior of some time-lost land... You and",
	"your friends rescued her from the giant",
	"pteranodon and befriended her.",
	"She said her name was Aiela, princess of the",
	"Kurak tribe, and seemed to be quite taken with",
	"you, her most unusual rescuer.",
	"",
	"But Aiela's hated suitor, Darden the Huge,",
	"prince of the Urali tribe, arrived with his men,",
	"scattering your friends and smashing you into",
	"the earth.",
	"Now you lie broken and defeated, struggling to",
	"regain consciousness...",
	"",
	""
};

// Four lines per question; nullptr means the line is unused
static const char *const CREATE_QUESTIONS_TEXT[24] = {
	"Your chief has told you to guard the village while a battle rages nearby. Your chief fights in the battle",
	"Will you (a) obey the word of your chief, or (b) join your fellow warriors in the fight?",
	nullptr,
	nullptr,
	"You fight a warrior you hate, and knock his spear from his hands.",
	"Another blow and he will be dead.",
	"Will you (a) let him surrender, and spare him if he does, or (b) slay him where he stands?",
	nullptr,
	"One warrior borrows another's spear and fails to return it. Days later, he mislays his own spear and you find it.",
	"Do you (a) return it to him, or (b) give it to the warrior who is owed the spear?",
	nullptr,
	nullptr,
	"You have sworn to protect your chief at any cost. Yet you see him foully murder a warrior of his own tribe.",
	"The murdered man's brother asks you what you saw.",
	"Will you (a) keep your oath and protect your king, or (b) tell the truth of the matter?",
	nullptr,
	"A huge, powerful warrior stands against you and demands you give him your food.",
	"Will you (a) throw his demand in his teeth and attack him, or (b) give him your food, since it is clear he is hungry?",
	nullptr,
	nullptr,
	"Your chief gives you a pouch of gems to take to another; he has not counted them.",
	"On the path to the other village, you find a lamed warrior who cannot hunt.",
	"He could trade you a few of your gems for food, and they will not be missed.",
	"Will you (a) deliver the gems as you were charged, or (b) give the warrior a gem?"
};

// Stat raised by answering a or b, for each question: 0 strength, 1 dexterity, 2 intelligence
static const int CREATE_QUESTIONS_A[6] = { 2, 1, 1, 0, 0, 2 };
static const int CREATE_QUESTIONS_B[6] = { 0, 0, 2, 2, 1, 1 };

// Heights of the credit images, which can't be read from the sprites
static const int CREDIT_HEIGHTS[12] = { 182, 157, 100, 188, 136, 193, 183, 176, 97, 190, 190, 50 };

enum {
	SPRITE_BORDER = 1,
	SPRITE_SHAMAN = 2,
	SPRITE_SHAMAN_BORDER = 3,
	SPRITE_ARM_FIRST = 4,
	SPRITE_ARM_LAST = 9,
	SPRITE_DROP_FIRST = 10,
	SPRITE_DROP_LAST = 26,
	SPRITE_COLOUR_FIRST = 27,
	SPRITE_SCROLL = 30,
	SPRITE_NAME = 31,
	SPRITE_ANSWER_BORDER = 32,
	SPRITE_COUNT = 33
};

static const int MAX_NAME_LENGTH = 13;

class Intro : public CutsceneScript {
private:
	// Main menu sprites: the four entries, then their highlights, then their focus markers
	enum { MENU_ENTRIES = 4, MENU_HIGHLIGHT = 4, MENU_FOCUS = 8, MENU_SPRITES = 12 };

	CSSprite *_menuSprites[MENU_SPRITES];
	int _menuIdx = 0;
	int _oldMenuIdx = 0;
	int _stats[3];

	static void destroySprite(CSSprite *sprite) {
		sprite->visible = false;
	}

	void destroySprites(CSSprite *const *sprites, uint count);
	void fadeOutSprite(CSSprite *sprite, int speed);
	void fadeOutSprites(CSSprite *const *sprites, int speed, uint count);
	void fadeInSprites(CSSprite *const *sprites, int speed, uint count);

	/** Prints wrapped text into a scroll image starting at the given y, and returns the new y */
	uint16 printScroll(CSImage *image, const char *text, uint16 y, uint16 width);

	void originFxSequence();
	void introSequence(const ImageTable &images);
	void storySoFar(const ImageTable &images);
	void shamanDropAnim(CSSprite *const *sprites, int colour);
	int processAnswer(int qIndex, int answer, CSSprite *const *sprites);
	int askQuestion(int qNum, int qOrd, CSSprite *const *sprites, CSImage *textImage,
		const ImageTable &images, uint16 textWidth);
	bool createNewCharacter(const ImageTable &images);
	void aboutTheSavageEmpire(const ImageTable &images);

	void initMainMenuSprites(const ImageTable &images);
	void setMainMenuHighlight();
	void selectedStorySoFar(const ImageTable &images);
	bool selectedCreateCharacter(const ImageTable &images);
	void selectedAboutTheSavageEmpire(const ImageTable &images);
	void journeyOnward();

	/** Returns true if the player chose to quit, or false to journey onward */
	bool mainMenu(const ImageTable &images);

public:
	void play();
};

void Intro::destroySprites(CSSprite *const *sprites, uint count) {
	for (uint i = 0; i < count; ++i) {
		if (sprites[i])
			destroySprite(sprites[i]);
	}
}

void Intro::fadeOutSprite(CSSprite *sprite, int speed) {
	for (int i = 0xff; i >= 0; i -= speed) {
		setOpacity(sprite, i);
		canvasUpdate();
	}
}

void Intro::fadeOutSprites(CSSprite *const *sprites, int speed, uint count) {
	for (int i = 0xff; i >= 0; i -= speed) {
		for (uint j = 0; j < count; ++j) {
			setOpacity(sprites[j], i);
			canvasUpdate();
		}
	}

	for (uint j = 0; j < count; ++j) {
		setOpacity(sprites[j], 0);
		canvasUpdate();
	}
}

void Intro::fadeInSprites(CSSprite *const *sprites, int speed, uint count) {
	for (int i = 0; i <= 0xff; i += speed) {
		for (uint j = 0; j < count; ++j) {
			setOpacity(sprites[j], i);
			canvasUpdate();
		}
	}

	for (uint j = 0; j < count; ++j) {
		setOpacity(sprites[j], 0xff);
		canvasUpdate();
	}
}

uint16 Intro::printScroll(CSImage *image, const char *text, uint16 y, uint16 width) {
	uint16 x = 8;
	_cutscene->print_text(image, text, &x, &y, 8, width, 0);
	return y;
}

void Intro::originFxSequence() {
	ImageTable images;
	if (!loadImages("title.lzc", images))
		return;

	_cutscene->load_palette("savage.pal", 2);

	newSprite(images[0][0], 0, 24, true);

	CSImage *logoImage = newImage(282, 82);
	blit(logoImage, images[0][1], 0, 16);
	blit(logoImage, images[0][2], imageWidth(images[0][1]), 14);
	blit(logoImage, images[0][3], imageWidth(images[0][1]) + imageWidth(images[0][2]), 0);

	CSSprite *logo = newSprite(logoImage, 20, 70, false);

	CSSprite *planet = newSprite(images[12][0], 160, 48, true);
	setClip(planet, 0, 0, 320, 152);

	CSSprite *players[PLAYER_COUNT];
	for (uint i = 0; i < PLAYER_COUNT; ++i) {
		static const int PLAYER_X[3] = { 58, 186, 278 };
		players[i] = createPlayerSprite(images[i + 1][0], PLAYER_X[i % 3], 118 + (i / 3) * 8);
	}

	CSSprite *conductor = newSprite(images[10][0], 158, 98, true);
	setClip(conductor, 0, 24, 320, 128);

	fadeIn();

	musicPlay("music.lzc", 19);

	for (int i = 0; i <= 6; ++i) {
		conductor->image = images[10][i];
		updatePlayers(players, images);
		if (pollForKeyOrButton(4))
			return;
	}
	for (int i = 7; i <= 13; ++i) {
		conductor->image = images[10][i];
		updatePlayers(players, images);
		if (pollForKeyOrButton(4))
			return;
	}
	for (int i = 7; i <= 12; ++i) {
		conductor->image = images[10][i];
		updatePlayers(players, images);
		if (pollForKeyOrButton(4))
			return;
	}

	for (int i = 1; i <= 4; ++i) {
		for (int j = 13; j <= 15; ++j) {
			conductor->image = images[10][j];
			if (pollForKeyOrButton(1))
				return;
		}

		conductor->image = images[10][14];
		if (pollForKeyOrButton(2))
			return;
		conductor->image = images[10][13];
		if (pollForKeyOrButton(2))
			return;
		conductor->image = images[10][16];

		if (pollForKeyOrButton(1))
			return;
		playSfx(38);
	}

	for (int i = 16; i <= 20; ++i) {
		conductor->image = images[10][i];
		if (pollForKeyOrButton(4))
			return;
	}
	if (pollForKeyOrButton(200))
		return;

	playSfx(12);

	conductor->image = images[10][6];

	for (int i = 1; i <= 21; ++i) {
		conductor->y = 98 + i * 12;
		conductor->image->setScale(100 + i * 15);

		for (uint j = 0; j < PLAYER_COUNT; ++j) {
			players[j]->y += 5;
			players[j]->image->setScale(100 + i * 5);

			if (j % 3 == 0)
				players[j]->x -= 2;
			else if (j % 3 == 2)
				players[j]->x += 2;
		}

		if (pollForEsc(4))
			return;
	}

	logo->visible = true;
	logo->image->setScale(10);

	for (int i = 1; i <= 18; ++i) {
		planet->y += 6;

		logo->image->setScale(logo->image->getScale() + 5);
		logo->x = (320 - imageWidth(logo->image)) / 2;
		if (i < 10)
			logo->y -= 4;
		else
			logo->y += 1;

		if (pollForKeyOrButton(4))
			return;
	}

	fireworks(images, logo);
}

void Intro::introSequence(const ImageTable &images) {
	_cutscene->load_palette("savage.pal", 0);
	musicPlay("music.lzc", 18);

	CSSprite *logo = newSprite(images[0][0], 0, 0, true);
	fadeIn();
	if (pollForKeyOrButton(175))
		return;
	fadeOut();
	logo->image = images[0][1];
	fadeIn();
	if (pollForKeyOrButton(175))
		return;
	fadeOut();

	_cutscene->load_palette("savage.pal", 0);
	CSSprite *bg = newSprite(images[1][0], 0, 0, true);
	CSSprite *bg1 = newSprite(images[1][1], 320, 0, true);
	CSSprite *bg2 = newSprite(images[1][2], 0, 0, true);
	CSSprite *bg3 = newSprite(images[1][3], 320, 0, true);
	CSSprite *bg4 = newSprite(images[1][4], 640, 0, true);
	CSSprite *title = newSprite(images[2][0], 0, 240, true);
	CSSprite *bg5 = newSprite(images[1][5], 0, 0, true);
	CSSprite *bg6 = newSprite(images[1][6], 320, 0, true);
	CSSprite *bg7 = newSprite(images[1][7], 640, 0, true);
	_cutscene->set_screen_opacity(0xff);

	int i = 0;
	while (i < 240) {
		canvasUpdate();
		title->y -= 3;
		if (title->y < 0)
			title->y = 0;
		i += 3;
		if (pollForKeyOrButton(1))
			return;
	}
	if (pollForKeyOrButton(30))
		return;
	fadeOutSprite(title, 4);

	i = 0;
	int currentCredit = 1;
	CSSprite *creditSprite = newSprite(images[2][currentCredit], 0, 0, true);
	int creditTime = 15;
	bool creditShow = true;
	while (i < 636) {
		canvasUpdate();
		bg->x -= 1;
		bg1->x -= 1;
		bg2->x -= 2;
		bg3->x -= 2;
		bg4->x -= 2;
		bg5->x -= 3;
		bg6->x -= 3;
		bg7->x -= 3;
		i += 3;

		if (--creditTime == 0) {
			if (creditShow) {
				creditShow = false;
				creditSprite->visible = false;
				creditTime = 20;
			} else {
				creditShow = true;
				++currentCredit;
				if (currentCredit < 7) {
					creditSprite = newSprite(images[2][currentCredit], 0, 0, true);
					creditTime = 15;
				}
			}
		}

		if (pollForKeyOrButton(8))
			return;
	}
}

void Intro::storySoFar(const ImageTable &images) {
	CSSprite *prev1 = nullptr;
	CSSprite *prev2 = nullptr;

	_cutscene->load_palette("savage.pal", 1);

	for (int i = 0; i < 10; ++i) {
		CSImage *newImg = copyImage(images[5][0]);
		CSSprite *f1 = newSprite(newImg, 0, 0, true);
		CSSprite *f2 = newSprite(images[5][i + 1], 120, 45, true);

		if (prev1)
			destroySprite(prev1);
		if (prev2)
			destroySprite(prev2);
		prev1 = f1;
		prev2 = f2;

		for (int line = 0; line < 4; ++line) {
			uint16 x = 25;
			uint16 y = 136 + line * 8;
			_cutscene->print_text(newImg, STORY_SO_FAR_TEXT[i * 4 + line], &x, &y, 0, 300, 0);
		}

		waitForInput();
	}

	if (prev1)
		destroySprite(prev1);
	if (prev2)
		destroySprite(prev2);

	_cutscene->load_palette("savage.pal", 0);
}

void Intro::shamanDropAnim(CSSprite *const *sprites, int colour) {
	CSSprite *prevShaman = nullptr;

	for (int i = SPRITE_ARM_FIRST; i <= SPRITE_ARM_LAST; ++i) {
		sprites[i]->visible = true;
		if (prevShaman)
			prevShaman->visible = false;
		prevShaman = sprites[i];
		canvasUpdate();
	}

	CSSprite *prevDrop = nullptr;
	for (int i = SPRITE_DROP_FIRST; i <= SPRITE_DROP_LAST; ++i) {
		sprites[SPRITE_COLOUR_FIRST + colour]->visible = true;
		sprites[i]->visible = true;
		if (prevDrop)
			prevDrop->visible = false;
		prevDrop = sprites[i];
		canvasUpdate();
	}
	sprites[SPRITE_COLOUR_FIRST + colour]->visible = false;
	prevDrop->visible = false;

	for (int i = SPRITE_ARM_FIRST + 4; i >= SPRITE_ARM_FIRST; --i) {
		sprites[i]->visible = true;
		if (prevShaman)
			prevShaman->visible = false;
		prevShaman = sprites[i];
		canvasUpdate();
	}
	prevShaman->visible = false;
	canvasUpdate();
}

int Intro::processAnswer(int qIndex, int answer, CSSprite *const *sprites) {
	int statType = (answer == Common::KEYCODE_a) ? CREATE_QUESTIONS_A[qIndex] : CREATE_QUESTIONS_B[qIndex];

	_stats[statType] += 3 + randomRange(0, 2);

	int colour = 0;
	if (statType == 0)
		colour = 1;
	else if (statType == 1)
		colour = 2;

	shamanDropAnim(sprites, colour);
	return statType;
}

int Intro::askQuestion(int qNum, int qOrd, CSSprite *const *sprites, CSImage *textImage,
		const ImageTable &images, uint16 textWidth) {
	const char *const *lines = &CREATE_QUESTIONS_TEXT[qNum * 4];

	// Only the last question has too much text for one page
	int firstLine = 0;
	if (lines[3]) {
		blit(textImage, images[4][2], 0, 0);
		uint16 y = printScroll(textImage, lines[0], 10, textWidth);
		printScroll(textImage, lines[1], y + 20, textWidth);
		canvasUpdate();
		waitForInput();
		firstLine = 2;
	}

	blit(textImage, images[4][2], 0, 0);
	uint16 y = printScroll(textImage, lines[firstLine], 10, textWidth);
	y = printScroll(textImage, lines[firstLine + 1], y + 20, textWidth);
	if (firstLine != 2 && lines[firstLine + 2])
		printScroll(textImage, lines[firstLine + 2], y + 20, textWidth);

	const int red = 0x04;
	const int black = 0x00;
	int answer = Common::KEYCODE_a;
	int oldAnswer = 0;
	int statType = -1;
	bool needToProcessAnswer = false;
	CSSprite *aSprite = newSprite(nullptr, 56 + 170, 129 + 45, true);
	CSSprite *bSprite = newSprite(nullptr, 72 + 170, 129 + 45, true);
	aSprite->text = "a";
	bSprite->text = "b";
	aSprite->text_color = red;
	bSprite->text_color = black;

	while (statType == -1) {
		canvasUpdate();
		int input = inputPoll(true);

		if (input != INPUT_NONE) {
			if (input == Common::KEYCODE_a || input == Common::KEYCODE_b) {
				answer = input;
				needToProcessAnswer = true;
			} else if (input == INPUT_KEY_LEFT || input == Common::KEYCODE_KP4) {
				answer = Common::KEYCODE_a;
			} else if (input == INPUT_KEY_RIGHT || input == Common::KEYCODE_KP6) {
				answer = Common::KEYCODE_b;
			} else if (input == Common::KEYCODE_KP_ENTER || input == Common::KEYCODE_RETURN ||
					input == Common::KEYCODE_SPACE) {
				needToProcessAnswer = true;
			} else if (input == INPUT_MOUSE_CLICK || input == INPUT_MOUSE_MOTION) {
				int mouseY = this->mouseY();
				if (mouseY >= bSprite->y && mouseY <= bSprite->y + 7) {
					int mouseX = this->mouseX();
					if (mouseX > aSprite->x && mouseX < aSprite->x + 7) {
						answer = Common::KEYCODE_a;
						if (input == INPUT_MOUSE_CLICK)
							needToProcessAnswer = true;
					} else if (mouseX > bSprite->x && mouseX < bSprite->x + 6) {
						answer = Common::KEYCODE_b;
						if (input == INPUT_MOUSE_CLICK)
							needToProcessAnswer = true;
					}
				}
			}

			if (answer != oldAnswer) {
				oldAnswer = answer;
				if (answer == Common::KEYCODE_a) {
					aSprite->text_color = red;
					bSprite->text_color = black;
				} else {
					aSprite->text_color = black;
					bSprite->text_color = red;
				}
			}

			if (needToProcessAnswer)
				statType = processAnswer(qNum, answer, sprites);
		}
	}

	destroySprite(aSprite);
	destroySprite(bSprite);

	// The next question depends on the stat just raised and how many have been asked
	if (statType == 0) {
		if (qOrd == 0)
			return 2;
		return (qNum == 0) ? 4 : 3;
	}
	if (statType == 1) {
		if (qOrd == 0)
			return 0;
		return (qNum == 2) ? 4 : 5;
	}

	if (qOrd == 0)
		return 1;
	return (qNum == 0) ? 5 : 3;
}

bool Intro::createNewCharacter(const ImageTable &images) {
	_cutscene->load_palette("savage.pal", 1);

	CSSprite *sprites[SPRITE_COUNT] = { nullptr };

	for (int i = 0; i < 3; ++i)
		_stats[i] = 16 + randomRange(0, 2);

	sprites[SPRITE_BORDER] = newSprite(images[1][2], 0, 0, true);
	sprites[SPRITE_SHAMAN] = newSprite(images[1][0], 8, 28, true);
	for (int i = 0; i <= SPRITE_ARM_LAST - SPRITE_ARM_FIRST; ++i)
		sprites[SPRITE_ARM_FIRST + i] = newSprite(images[2][i], 8, 28, false);
	for (int i = 0; i < 3; ++i)
		sprites[SPRITE_COLOUR_FIRST + i] = newSprite(images[3][17 + i], 8, 28, false);
	for (int i = 0; i <= SPRITE_DROP_LAST - SPRITE_DROP_FIRST; ++i)
		sprites[SPRITE_DROP_FIRST + i] = newSprite(images[3][i], 8, 28, false);
	sprites[SPRITE_SHAMAN_BORDER] = newSprite(images[1][1], 8, 28, true);

	CSImage *scrollImg = copyImage(images[4][2]);
	CSImage *answerBorder = copyImage(images[4][2]);
	const uint16 textWidth = 130;
	sprites[SPRITE_ANSWER_BORDER] = newSprite(answerBorder, 170, 45, false);
	sprites[SPRITE_SCROLL] = newSprite(scrollImg, 170, 25, true);

	uint16 y = printScroll(scrollImg, "You are in a dirt-floored hut... and a tribesman with wise eyes stares down at you.", 10, textWidth);
	printScroll(scrollImg, "You feel as though you are floating. You cannot move your limbs. It is like a dream... yet you sense it is not a dream.", y + 20, textWidth);
	canvasUpdate();
	waitForInput();

	blit(scrollImg, images[4][2], 0, 0);
	printScroll(scrollImg, "The man speaks: \"Yes, warrior, you can hear. You see Intanya, shaman and healer. Intanya will heal you... but to concoct his healing potion, he must ask you questions.\"", 10, textWidth);
	canvasUpdate();
	waitForInput();

	blit(scrollImg, images[4][2], 0, 0);
	y = printScroll(scrollImg, "\"In order to heal your spirit, Intanya must know your spirit, so you must answer with truthful words.\"", 10, textWidth);
	printScroll(scrollImg, "\"Intanya will speak of deeds and choices; you must answer with the choices you would make.\"", y + 20, textWidth);
	canvasUpdate();
	waitForInput();

	blit(scrollImg, images[4][2], 0, 0);
	y = printScroll(scrollImg, "\"First, though, Intanya must know the warrior's name, for there is much power in names.", 10, textWidth);
	y = printScroll(scrollImg, "\"What does the warrior call himself?\"", y + 20, textWidth);
	canvasUpdate();

	CSSprite *name = newSprite(nullptr, 178, 25 + y + 10, true);
	sprites[SPRITE_NAME] = name;
	name->text = "";
	name->text_color = 0x00;
	int charIndex = 0;

	bool done = false;
	while (!done) {
		canvasUpdate();
		int input = inputPoll();
		if (input == INPUT_NONE)
			continue;

		if (input == Common::KEYCODE_ESCAPE) {
			destroySprites(sprites, SPRITE_COUNT);
			_cutscene->load_palette("savage.pal", 0);
			return false;
		}

		Common::String nameText = name->text;
		int len = nameText.size();

		if ((input == Common::KEYCODE_BACKSPACE || input == INPUT_KEY_LEFT) && len > 0) {
			name->text = nameText.substr(0, len - 1);
			charIndex = (len == 1) ? 0 : (byte)nameText[len - 2];
		} else if ((input == Common::KEYCODE_RETURN || input == Common::KEYCODE_KP_ENTER) && len > 0) {
			done = true;
		} else if (keyToChar(input) && len < MAX_NAME_LENGTH) {
			charIndex = input;
			name->text = nameText + keyToChar(input);
		} else if (input == INPUT_KEY_UP) {
			if (charIndex == 0)
				charIndex = (len > 0) ? Common::KEYCODE_a : 'A';
			else if (charIndex == ' ')
				charIndex = '\'';
			else if (charIndex == '\'')
				charIndex = ',';
			else if (charIndex == '.')
				charIndex = '0';
			else if (charIndex == '9')
				charIndex = 'A';
			else if (charIndex == 'Z')
				charIndex = 'a';
			else if (charIndex == 'z')
				charIndex = ' ';
			else
				++charIndex;

			if (len > 0)
				nameText = nameText.substr(0, len - 1);
			name->text = nameText + keyToChar(charIndex);
		} else if (input == INPUT_KEY_DOWN) {
			if (charIndex == 0)
				charIndex = (len > 0) ? 'z' : 'Z';
			else if (charIndex == '\'')
				charIndex = ' ';
			else if (charIndex == ',')
				charIndex = '\'';
			else if (charIndex == '0')
				charIndex = '.';
			else if (charIndex == 'A')
				charIndex = '9';
			else if (charIndex == 'a')
				charIndex = 'Z';
			else if (charIndex == ' ')
				charIndex = 'z';
			else
				--charIndex;

			if (len > 0)
				nameText = nameText.substr(0, len - 1);
			name->text = nameText + keyToChar(charIndex);
		} else if (input == INPUT_KEY_RIGHT && len < MAX_NAME_LENGTH) {
			charIndex = Common::KEYCODE_a;
			name->text = nameText + "a";
		}
	}

	name->visible = false;

	int nextQ = randomRange(0, 2);

	showMouseCursor(true);
	sprites[SPRITE_ANSWER_BORDER]->visible = true;
	// Answers are 0 red, 1 yellow, 2 blue, which is the order of the sprites in the file
	nextQ = askQuestion(nextQ, 0, sprites, scrollImg, images, textWidth);
	nextQ = askQuestion(nextQ, 1, sprites, scrollImg, images, textWidth);
	askQuestion(nextQ, 2, sprites, scrollImg, images, textWidth);

	Configuration *config = _cutscene->get_config();
	config->set("config/newgame", true);
	config->set("config/newgamedata/name", name->text);
	config->set("config/newgamedata/str", _stats[0]);
	config->set("config/newgamedata/dex", _stats[1]);
	config->set("config/newgamedata/int", _stats[2]);

	destroySprites(sprites, SPRITE_COUNT);
	_cutscene->load_palette("savage.pal", 0);
	return true;
}

void Intro::aboutTheSavageEmpire(const ImageTable &images) {
	CSSprite *sprites[12];
	int ypos = 200;

	for (int i = 0; i < 12; ++i) {
		sprites[i] = newSprite(images[6][i], 100, ypos, true);
		setClip(sprites[i], 0, 100, 320, 100);
		ypos += CREDIT_HEIGHTS[i];
	}

	int done = 0;
	while (done < ypos) {
		for (int i = 0; i < 12; ++i)
			sprites[i]->y -= 1;

		done += 1;
		if (pollForKeyOrButton(1))
			break;
	}

	destroySprites(sprites, 12);
}

void Intro::initMainMenuSprites(const ImageTable &images) {
	static const int MENU_IMAGE[MENU_ENTRIES] = { 5, 1, 9, 3 };
	static const int FOCUS_IMAGE[MENU_ENTRIES] = { 6, 2, 10, 4 };
	static const int HIGHLIGHT_IMAGE[MENU_ENTRIES] = { 13, 11, 15, 12 };

	for (int i = 0; i < MENU_ENTRIES; ++i)
		_menuSprites[i] = newSprite(images[0][MENU_IMAGE[i]], 85, 117 + i * 18, true);
	for (int i = 0; i < MENU_ENTRIES; ++i)
		_menuSprites[MENU_FOCUS + i] = newSprite(images[0][FOCUS_IMAGE[i]], 85, 117 + i * 18, i == 0);
	for (int i = 0; i < MENU_ENTRIES; ++i)
		_menuSprites[MENU_HIGHLIGHT + i] = newSprite(images[0][HIGHLIGHT_IMAGE[i]], 85, 117 + i * 18, false);
}

void Intro::setMainMenuHighlight() {
	if (_menuIdx == _oldMenuIdx)
		return;

	_menuSprites[MENU_FOCUS + _menuIdx]->visible = true;
	_menuSprites[MENU_FOCUS + _oldMenuIdx]->visible = false;
	_oldMenuIdx = _menuIdx;
}

void Intro::selectedStorySoFar(const ImageTable &images) {
	showMouseCursor(false);
	_menuSprites[0]->visible = false;
	_menuSprites[MENU_FOCUS]->visible = false;
	_menuSprites[MENU_HIGHLIGHT]->visible = true;
	canvasUpdate();
	storySoFar(images);
	_menuSprites[0]->visible = true;
	_menuSprites[MENU_FOCUS]->visible = true;
	_menuSprites[MENU_HIGHLIGHT]->visible = false;
	showMouseCursor(true);
}

bool Intro::selectedCreateCharacter(const ImageTable &images) {
	showMouseCursor(false);
	bool startNewGame = createNewCharacter(images);
	showMouseCursor(true);
	return startNewGame;
}

void Intro::selectedAboutTheSavageEmpire(const ImageTable &images) {
	showMouseCursor(false);
	_menuSprites[2]->visible = false;
	_menuSprites[MENU_FOCUS + 2]->visible = false;
	_menuSprites[MENU_HIGHLIGHT + 2]->visible = true;
	canvasUpdate();
	fadeOutSprites(_menuSprites, 50, MENU_SPRITES);
	aboutTheSavageEmpire(images);
	_menuSprites[2]->visible = true;
	_menuSprites[MENU_FOCUS + 2]->visible = true;
	_menuSprites[MENU_HIGHLIGHT + 2]->visible = false;
	fadeInSprites(_menuSprites, 50, MENU_SPRITES);
	showMouseCursor(true);
}

void Intro::journeyOnward() {
	showMouseCursor(false);
	_menuSprites[3]->visible = false;
	_menuSprites[MENU_FOCUS + 3]->visible = false;
	_menuSprites[MENU_HIGHLIGHT + 3]->visible = true;
	canvasUpdate();
}

bool Intro::mainMenu(const ImageTable &images) {
	newSprite(images[0][0], 0, 0, true);
	showMouseCursor(true);
	initMainMenuSprites(images);
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
		} else if (input == Common::KEYCODE_RETURN || input == Common::KEYCODE_SPACE ||
				input == Common::KEYCODE_KP_ENTER) {
			if (_menuIdx == 0) {
				selectedStorySoFar(images);
			} else if (_menuIdx == 1) {
				if (selectedCreateCharacter(images))
					return false;
			} else if (_menuIdx == 2) {
				selectedAboutTheSavageEmpire(images);
			} else if (_menuIdx == 3) {
				journeyOnward();
				return false;
			}
		} else if (input == Common::KEYCODE_s || input == Common::KEYCODE_t) {
			_menuIdx = 0;
			setMainMenuHighlight();
			selectedStorySoFar(images);
		} else if (input == Common::KEYCODE_c) {
			_menuIdx = 1;
			setMainMenuHighlight();
			if (selectedCreateCharacter(images))
				return false;
		} else if (input == Common::KEYCODE_a) {
			_menuIdx = 2;
			setMainMenuHighlight();
			selectedAboutTheSavageEmpire(images);
		} else if (input == Common::KEYCODE_j) {
			_menuIdx = 3;
			setMainMenuHighlight();
			journeyOnward();
			return false;
		} else if (input == INPUT_KEY_DOWN || input == Common::KEYCODE_KP2) {
			_menuIdx = (_menuIdx + 1) % MENU_ENTRIES;
			setMainMenuHighlight();
		} else if (input == INPUT_KEY_UP || input == Common::KEYCODE_KP8) {
			_menuIdx = (_menuIdx + MENU_ENTRIES - 1) % MENU_ENTRIES;
			setMainMenuHighlight();
		} else if (input == INPUT_MOUSE_CLICK || input == INPUT_MOUSE_MOTION) {
			int x = mouseX();
			if (x > 84 && x < 242) {
				int y = mouseY();
				int entry = -1;

				if (y > 118 && y < 134)
					entry = 0;
				else if (y > 133 && y < 152)
					entry = 1;
				else if (y > 151 && y < 170)
					entry = 2;
				else if (y > 169 && y < 185)
					entry = 3;

				if (input == INPUT_MOUSE_CLICK) {
					if (entry == 0) {
						selectedStorySoFar(images);
					} else if (entry == 1) {
						if (selectedCreateCharacter(images))
							return false;
					} else if (entry == 2) {
						selectedAboutTheSavageEmpire(images);
					} else if (entry == 3) {
						journeyOnward();
						return false;
					}
				} else if (entry != -1) {
					_menuIdx = entry;
					setMainMenuHighlight();
				}
			}
		}

		_cutscene->set_update_interval(10);
	}
}

void Intro::play() {
	showMouseCursor(false);
	_cutscene->set_update_interval(25);
	_cutscene->set_bg_color(0);
	_cutscene->set_screen_opacity(0);

	originFxSequence();

	ImageTable introImages, createImages;
	if (!loadImages("intro.lzc", introImages) || !loadImages("create.lzc", createImages))
		return;

	introSequence(introImages);

	if (mainMenu(createImages)) {
		fadeOut(6);
		_cutscene->get_config()->set("config/quit", true);
	}

	musicStop();
	_cutscene->hide_sprites();
	_cutscene->Hide();
}

} // End of anonymous namespace

bool SEScripts::playIntro() {
	Intro intro;
	intro.play();
	return true;
}

} // End of namespace SE
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
