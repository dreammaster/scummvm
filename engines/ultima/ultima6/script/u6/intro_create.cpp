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
#include "ultima/ultima6/script/u6/intro.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

namespace {

#include "ultima/ultima6/script/u6/intro_gypsy_data.h"

const int MAX_NAME_LENGTH = 13;

}

void Intro::bubbleEffect(CSImage *image) {
	if (!image || _bubbleCount == 0)
		return;

	uint8 *data = image->shp->get_data();
	uint16 w, h;
	image->shp->get_size(&w, &h);

	for (int i = 0; i < w * h; ++i) {
		if (data[i] != 0xff)
			data[i] = _bubbleColors[NUVIE_RAND() % _bubbleCount];
	}
}

void Intro::gypsyUpdateBubbles(CSImage *bubbleImage) {
	if (_bubbleCounter == 0) {
		bubbleEffect(bubbleImage);
		_bubbleCounter = 3;
	} else {
		--_bubbleCounter;
	}
}

int Intro::gypsyAbSelect(int question) {
	int input;

	do {
		canvasUpdate();
		input = inputPoll();

		if (input != INPUT_NONE) {
			if (input == 'A' || input == 'a' || (input == Common::KEYCODE_RETURN && _highlightIndex == 0))
				return GYPSY_A_LOOKUP[question - 105] + 1;

			if (input == 'B' || input == 'b' || input == Common::KEYCODE_RETURN)
				return GYPSY_B_LOOKUP[question - 105] + 1;

			if (input == INPUT_KEY_RIGHT && _highlightIndex == 0) {
				_highlightIndex = 1;
				_abHighlight->x += 17;
			} else if (input == INPUT_KEY_LEFT && _highlightIndex == 1) {
				_highlightIndex = 0;
				_abHighlight->x -= 17;
			} else if (input == INPUT_MOUSE_CLICK) {
				int y = mouseY();
				if (y > 173 && y < 190) {
					int x = mouseX();
					if (x > 278 && x < 296)
						return GYPSY_A_LOOKUP[question - 105] + 1;
					else if (x > 296 && x < 313)
						return GYPSY_B_LOOKUP[question - 105] + 1;
				}
			}
		}

		if (_gypsy._jarLiquid->visible)
			gypsyUpdateBubbles(_gypsy._jarLiquid->image);
	} while (!engineShouldQuit());

	return GYPSY_A_LOOKUP[question - 105] + 1;
}

void Intro::shuffleQuestionTable(int shuffleLen) {
	int c = randomRange(0, shuffleLen * shuffleLen - 1) + shuffleLen;

	for (int i = 0; i <= c; ++i) {
		int j = randomRange(1, shuffleLen) - 1;
		int k = randomRange(1, shuffleLen) - 1;

		SWAP(_questionTable[j], _questionTable[k]);
	}
}

void Intro::gypsyStartPouring(int vialNum, int vialLevel) {
	Gypsy &g = _gypsy;

	if (vialLevel <= 3 && vialLevel > 0) {
		g._pour->visible = true;
		int index = (3 - vialLevel) * 8 + vialNum - 1;
		int img1 = GYPSY_POUR_IMG[index];

		if (vialNum > 6)
			img1 += 9;
		else
			img1 += 0x42;

		g._pour->image = gypsyImg(img1);
		g._pour->y = GYPSY_POUR_Y[3 - vialLevel] - 20;
		g._pour->x = GYPSY_POUR_X[index];
	}

	++g._jarLevel;

	g._jarLiquid->visible = true;
	g._jarLiquid->image = gypsyImg(8 + g._jarLevel);
	g._jarLiquid->y -= 1;

	static const uint8 VIAL_COLORS[8] = { 239, 14, 231, 103, 228, 5, 15, 219 };

	if (_bubbleCount != 8)
		_bubbleColors[_bubbleCount++] = VIAL_COLORS[vialNum - 1];

	_bubbleCounter = 0;
}

void Intro::gypsyStopPouring() {
	_gypsy._pour->visible = false;
}

void Intro::gypsyVialAnimLiquid(int imgNum, int vialNum, int vialLevel, int handX, int handY) {
	Gypsy &g = _gypsy;

	if (vialLevel == 3)
		vialLevel = 2;

	if (vialLevel <= 0) {
		g._vialLiquid[vialNum - 1]->visible = false;
		return;
	}

	int si = 0;
	if (imgNum == 0xf)
		return;

	if (imgNum > 0xf) {
		if (imgNum == 0x12) {
			si = 1;
		} else if (imgNum > 0x12) {
			if (imgNum == 0x25)
				return;
			if (imgNum == 0x2e)
				si = 2;
		} else if (imgNum == 0x10) {
			si = 2;
		} else if (imgNum == 0x11) {
			si = 0;
		}
	} else {
		imgNum -= 9;
		if (imgNum == 0)
			si = 0;
		else if (imgNum == 1)
			si = 1;
		else if (imgNum >= 2 && imgNum <= 5)
			si = 3;
	}

	int imgOffset = (vialNum > 6 && si != 3) ? 9 : 0x42;

	if (vialLevel > 0 && vialLevel < 3) {
		int imgIdx = GYPSY_VIAL_LIQUID[(vialLevel - 1) * 8 + si * 24 + vialNum - 1];
		CSSprite *liquid = g._vialLiquid[vialNum - 1];
		liquid->image = gypsyImg(imgIdx + imgOffset);

		liquid->y = handY + GYPSY_HAND_Y[si * 3 + vialLevel - 1] - 20;

		static const int HAND_X_ADJUST[8] = { 10, 10, 10, 10, -13, -13, -13, -13 };
		liquid->x = handX + GYPSY_HAND_X[si * 24 + (vialLevel - 1) * 8 + vialNum - 1] + HAND_X_ADJUST[vialNum - 1];
	}
}

void Intro::gypsyVialAnim(int vial) {
	Gypsy &g = _gypsy;
	int v = vial - 1;
	int *vialLevel = g._vialLevel;

	static const int ARM_IMG[8] = { 0, 0, 0, 0, 5, 5, 5, 5 };
	static const int ARM_IMG1[8] = { 1, 1, 1, 1, 4, 4, 4, 4 };
	static const int ARM_IMG2[8] = { 2, 2, 2, 2, 3, 3, 3, 3 };

	static const int HAND_IMG[8] = { 12, 12, 12, 12, 13, 13, 13, 13 };
	static const int HAND_IMG1[8] = { 9, 9, 9, 9, 17, 17, 17, 17 };
	static const int HAND_IMG2[8] = { 10, 10, 10, 10, 18, 18, 18, 18 };
	static const int HAND_IMG3[8] = { 16, 16, 16, 16, 46, 46, 46, 46 };
	static const int HAND_IMG4[8] = { 15, 15, 15, 15, 37, 37, 37, 37 };

	static const int VIAL_X_OFFSET[8] = { 41, 62, 83, 104, 200, 221, 242, 263 };
	static const int ARM_X_OFFSET[8] = { 93, 93, 93, 93, 172, 172, 172, 172 };

	static const int HAND_X_OFFSET[8] = { 29, 50, 71, 92, 202, 223, 244, 264 };
	static const int HAND_X_OFFSET1[8] = { 107, 107, 107, 107, 170, 170, 170, 170 };
	static const int HAND_X_OFFSET2[8] = { 109, 109, 109, 109, 168, 168, 168, 168 };
	static const int HAND_X_OFFSET3[8] = { 112, 112, 112, 112, 165, 165, 165, 165 };
	static const int HAND_X_OFFSET4[8] = { 10, 10, 10, 10, -13, -13, -13, -13 };

	int handImgNum = 0;

	for (int idx = 0; idx <= 16; ++idx) {
		bool shouldUpdate = true;

		if (idx == 0) {
			handImgNum = 0xf;
			shouldUpdate = false;
		} else if (idx == 1 || idx == 15) {
			g._arm->visible = true;
			g._hand->visible = true;
			g._vial[v]->visible = false;
			g._arm->image = gypsyImg(1 + ARM_IMG[v]);
			g._arm->x = VIAL_X_OFFSET[v] - 6;
			g._arm->y = 21;

			handImgNum = HAND_IMG[v];
			g._hand->image = gypsyImg(9 + handImgNum);
			g._hand->x = VIAL_X_OFFSET[v];
			g._hand->y = 66;
			g._hand->visible = true;
		} else if (idx == 2 || idx == 14) {
			g._arm->image = gypsyImg(1 + ARM_IMG1[v]);
			g._arm->x = VIAL_X_OFFSET[v] - 6;
			g._arm->y = 21;

			handImgNum = HAND_IMG[v];
			g._hand->image = gypsyImg(9 + handImgNum);
			g._hand->x = VIAL_X_OFFSET[v];
			g._hand->y = 29;
		} else if (idx == 3 || idx == 13) {
			g._arm->image = gypsyImg(1 + ARM_IMG1[v]);
			g._arm->x = VIAL_X_OFFSET[v] - 6;
			g._arm->y = 21;

			handImgNum = HAND_IMG1[v];
			g._hand->image = gypsyImg(9 + handImgNum);
			g._hand->x = HAND_X_OFFSET[v];
			g._hand->y = 25;
		} else if (idx == 4 || idx == 12) {
			g._arm->image = gypsyImg(1 + ARM_IMG2[v]);
			g._arm->x = ARM_X_OFFSET[v];
			g._arm->y = 21;

			handImgNum = HAND_IMG1[v];
			g._hand->image = gypsyImg(9 + handImgNum);
			g._hand->x = HAND_X_OFFSET1[v];
			g._hand->y = 20;
		} else if (idx == 5 || idx == 11) {
			g._arm->image = gypsyImg(1 + ARM_IMG2[v]);
			g._arm->x = ARM_X_OFFSET[v];
			g._arm->y = 21;

			handImgNum = HAND_IMG2[v];
			g._hand->image = gypsyImg(9 + handImgNum);
			g._hand->x = HAND_X_OFFSET1[v];
			g._hand->y = 20;

			if (idx == 11) {
				--vialLevel[v];
				if (vialLevel[v] == 2)
					gypsyStopPouring();
			}

			if (vialLevel[v] == 3)
				gypsyStartPouring(vial, vialLevel[v]);
		} else if (idx == 6 || idx == 10) {
			if (vialLevel[v] > 2)
				shouldUpdate = false;

			g._arm->image = gypsyImg(1 + ARM_IMG2[v]);
			g._arm->x = ARM_X_OFFSET[v];
			g._arm->y = 21;

			handImgNum = HAND_IMG3[v];
			g._hand->image = gypsyImg(9 + handImgNum);
			g._hand->x = HAND_X_OFFSET2[v];
			g._hand->y = 21;

			if (vialLevel[v] == 2) {
				if (idx == 6)
					gypsyStartPouring(vial, vialLevel[v]);
				else if (idx == 10)
					gypsyStopPouring();
			}
		} else if (idx == 7 || idx == 8 || idx == 9) {
			if (vialLevel[v] > 1)
				shouldUpdate = false;

			g._arm->image = gypsyImg(1 + ARM_IMG2[v]);
			g._arm->x = ARM_X_OFFSET[v];
			g._arm->y = 21;

			handImgNum = HAND_IMG4[v];
			g._hand->image = gypsyImg(9 + handImgNum);
			g._hand->x = HAND_X_OFFSET3[v];
			g._hand->y = 13;

			if (vialLevel[v] == 1) {
				if (idx == 7) {
					gypsyStartPouring(vial, vialLevel[v]);
					g._vialLiquid[v]->visible = false;
				} else if (idx == 9) {
					gypsyStopPouring();
				}
			}
		}

		if (shouldUpdate) {
			gypsyVialAnimLiquid(handImgNum, vial, vialLevel[v], g._hand->x - HAND_X_OFFSET4[v], g._hand->y + 20);

			for (int j = 0; j < 8; ++j) {
				if (g._jarLevel > 0)
					gypsyUpdateBubbles(g._jarLiquid->image);
				canvasUpdate();
				inputPoll();
			}
		}
	}

	g._hand->visible = false;
	g._arm->visible = false;

	g._vial[v]->visible = true;
}

void Intro::gypsyAskQuestions(int numQuestions) {
	static const int STRENGTH_ADJUSTMENT[8] = { 0, 0, 2, 0, 1, 1, 1, 0 };
	static const int DEX_ADJUSTMENT[8] = { 0, 2, 0, 1, 1, 0, 1, 0 };
	static const int INT_ADJUSTMENT[8] = { 2, 0, 0, 1, 0, 1, 1, 0 };

	for (int i = 0; i < numQuestions && !engineShouldQuit(); ++i) {
		int q = GYPSY_QUESTIONS[_questionTable[i * 2] * 8 + _questionTable[i * 2 + 1]];

		CSImage *scrollImage = loadImage("blocks.shp", 3);
		_scroll->image = scrollImage;
		printText(scrollImage, GYPSY_QUESTION_TEXT[q - 105], 7, 303, 8, 9, 0x3e);

		int vial = gypsyAbSelect(q);

		gypsyVialAnim(vial);

		_str += STRENGTH_ADJUSTMENT[vial - 1];
		_dex += DEX_ADJUSTMENT[vial - 1];
		_int += INT_ADJUSTMENT[vial - 1];

		_questionTable[i] = vial - 1;
	}
}

bool Intro::collectNameString(CSSprite *name) {
	int charIndex = 0;

	for (;;) {
		canvasUpdate();
		int input = inputPoll();

		if (input == INPUT_NONE) {
			if (engineShouldQuit())
				return false;
			continue;
		}

		if (shouldExit(input))
			return false;

		Common::String text = name->text;
		int len = text.size();

		if ((input == Common::KEYCODE_BACKSPACE || input == INPUT_KEY_LEFT) && len > 0) {
			name->text = text.substr(0, len - 1);
			charIndex = (len == 1) ? 0 : (uint8)text[len - 2];
		} else if (input == Common::KEYCODE_RETURN && len > 0) {
			break;
		} else if (keyToChar(input) != 0 && len < MAX_NAME_LENGTH) {
			charIndex = input;
			name->text = text + keyToChar(input);
		} else if (input == INPUT_KEY_UP) {
			if (charIndex == 0)
				charIndex = (len > 0) ? 97 : 65;
			else if (charIndex <= 32)
				charIndex = 48;
			else if (charIndex >= 57 && charIndex < 65)
				charIndex = 65;
			else if (charIndex >= 90 && charIndex < 97)
				charIndex = 97;
			else if (charIndex >= 122)
				charIndex = 32;
			else
				++charIndex;

			if (len > 0)
				text = text.substr(0, len - 1);

			char c = keyToChar(charIndex);
			name->text = c ? text + c : text;
		} else if (input == INPUT_KEY_DOWN) {
			if (charIndex == 0)
				charIndex = (len > 0) ? 122 : 90;
			else if (charIndex == 65)
				charIndex = 57;
			else if (charIndex == 97)
				charIndex = 90;
			else if (charIndex <= 32)
				charIndex = 122;
			else if (charIndex <= 48)
				charIndex = 32;
			else
				--charIndex;

			if (len > 0)
				text = text.substr(0, len - 1);

			char c = keyToChar(charIndex);
			name->text = c ? text + c : text;
		} else if (input == INPUT_KEY_RIGHT && len < MAX_NAME_LENGTH) {
			charIndex = 97;
			name->text = text + "a";
		}
	}

	return true;
}

CSSprite *Intro::newHighlight(int w, int x, int y, bool visible) {
	CSImage *image = newImage(w, 2);
	drawLine(image, 0, 0, w + 1, 0, 248);
	drawLine(image, 0, 1, w + 1, 1, 248);
	return newSprite(image, x, y + 14, visible);
}

bool Intro::createCharacter() {
	Configuration *config = _cutscene->get_config();
	Gypsy &g = _gypsy;

	musicPlay("create.m", 0);

	CSSprite *bubbles = newSprite(newImage(100, 100, 0), 110, 30, false);
	CSSprite *bg = newSprite(loadImage("vellum1.shp", 0), 0x10, 0x50, true);
	printText(bg->image, "By what name shalt thou be called?", 7, 303, 36, 24, 0x48);

	CSSprite *name = newSprite(nullptr, 0x34, 0x78, true);
	name->text = "";

	if (!collectNameString(name)) {
		bg->visible = false;
		name->visible = false;
		return false;
	}

	name->x = 0x10 + (284 - _cutscene->get_text_width(name->text.c_str())) / 2;

	printText(bg->image, "And art thou Male, or Female?", 7, 303, 52, 56, 0x48);
	CSSprite *genderSprite = newSprite(nullptr, 154, 152, true);
	genderSprite->text = "";

	int gender = 0;
	int input;
	for (;;) {
		canvasUpdate();
		input = inputPoll();

		if (input == INPUT_NONE) {
			if (engineShouldQuit())
				input = Common::KEYCODE_ESCAPE;
			else
				continue;
		}

		if (shouldExit(input)) {
			bg->visible = false;
			name->visible = false;
			genderSprite->visible = false;
			return false;
		}

		if (input == 'M' || input == 'm') {
			gender = 1;
			break;
		} else if (input == 'F' || input == 'f') {
			gender = 0;
			break;
		} else if (input == INPUT_KEY_UP || input == INPUT_KEY_DOWN) {
			if (gender == 0) {
				gender = 1;
				genderSprite->text = "M";
			} else {
				gender = 0;
				genderSprite->text = "F";
			}
		} else if (input == Common::KEYCODE_RETURN && genderSprite->text != "") {
			break;
		}
	}

	genderSprite->visible = false;
	loadImages("vellum1.shp", _images);
	bg->image = img(0);

	name->y = 0x59;

	drawLine(bg->image, 14, 19, 277, 19, 0x48);

	CSSprite *newSex = newSprite(img(3), 0x5e, 0x70, true);
	CSSprite *newPortrait = newSprite(img(0x12), 0x3e, 0x81, true);
	CSSprite *continueButton = newSprite(img(0x10), 0x56, 0x92, true);
	CSSprite *escAbort = newSprite(img(4), 0x50, 0xa3, true);

	ImageTable montage;
	loadImages("montage.shp", montage);
	int portraitNum = 0;
	CSSprite *avatar = newSprite(montage[gender * 6 + portraitNum][0], 0xc3, 0x76, true);
	int buttonIndex = 0;
	int oldButtonIndex = 0;

	CSSprite *sexHighlight = newHighlight(58, newSex->x, newSex->y, true);
	CSSprite *portraitHighlight = newHighlight(90, newPortrait->x, newPortrait->y, false);
	CSSprite *continueHighlight = newHighlight(66, continueButton->x, continueButton->y, false);
	CSSprite *escAbortHighlight = newHighlight(72, escAbort->x, escAbort->y, false);

	bool exiting = false;

	for (;;) {
		canvasUpdate();
		input = inputPoll();

		if (input == INPUT_NONE) {
			if (!engineShouldQuit())
				continue;
			input = Common::KEYCODE_ESCAPE;
		}

		if (shouldExit(input) || (input == Common::KEYCODE_RETURN && buttonIndex == 3))
			exiting = true;

		if (input == 'p' || input == 'P' || (input == Common::KEYCODE_RETURN && buttonIndex == 1)) {
			portraitNum = (portraitNum == 5) ? 0 : portraitNum + 1;
			avatar->image = montage[gender * 6 + portraitNum][0];
		} else if (input == 's' || input == 'S' || (input == Common::KEYCODE_RETURN && buttonIndex == 0)) {
			gender = (gender == 0) ? 1 : 0;
			avatar->image = montage[gender * 6 + portraitNum][0];
		} else if (input == 'c' || input == 'C' || (input == Common::KEYCODE_RETURN && buttonIndex == 2)) {
			break;
		} else if (input == INPUT_KEY_UP && buttonIndex > 0) {
			--buttonIndex;
		} else if (input == INPUT_KEY_DOWN && buttonIndex < 3) {
			++buttonIndex;
		} else if (input == INPUT_MOUSE_CLICK) {
			int x = mouseX();

			if (x < 152) {
				int y = mouseY();

				if (x > 93 && y > 111 && y < 128) {
					gender = (gender == 0) ? 1 : 0;
					avatar->image = montage[gender * 6 + portraitNum][0];
				} else if (x > 61 && y > 128 && y < 145) {
					portraitNum = (portraitNum == 5) ? 0 : portraitNum + 1;
					avatar->image = montage[gender * 6 + portraitNum][0];
				} else if (x > 85 && y > 145 && y < 162) {
					break;
				} else if (x > 79 && y > 162 && y < 179) {
					exiting = true;
				}
			}
		}

		if (buttonIndex != oldButtonIndex || exiting) {
			if (oldButtonIndex == 0)
				sexHighlight->visible = false;
			else if (oldButtonIndex == 1)
				portraitHighlight->visible = false;
			else if (oldButtonIndex == 2)
				continueHighlight->visible = false;
			else
				escAbortHighlight->visible = false;

			if (exiting) {
				bg->visible = false;
				name->visible = false;
				newSex->visible = false;
				newPortrait->visible = false;
				continueButton->visible = false;
				escAbort->visible = false;
				avatar->visible = false;
				return false;
			}

			if (buttonIndex == 0)
				sexHighlight->visible = true;
			else if (buttonIndex == 1)
				portraitHighlight->visible = true;
			else if (buttonIndex == 2)
				continueHighlight->visible = true;
			else
				escAbortHighlight->visible = true;

			oldButtonIndex = buttonIndex;
		}
	}

	showMouseCursor(false);
	fadeOut();
	_cutscene->hide_sprites();

	_cutscene->load_palette("palettes.int", 4);
	ImageTable woodsImages;
	loadImages("woods.shp", woodsImages);
	CSSprite *woods = newSprite(woodsImages[0][0], 0, 0, true);
	CSSprite *woods1 = newSprite(woodsImages[1][0], 0x140, 0, true);

	fadeIn();

	CSImage *scrollImage = loadImage("blocks.shp", 1);
	_scroll = newSprite(scrollImage, 1, 0xa0, true);

	printText(scrollImage, "\"Welcome, O Seeker!\177", 7, 303, 96, 14, 0x3e);

	waitInput();

	_scroll->visible = false;

	for (int i = 0; i <= 0xbd; i += 2) {
		woods->x -= 2;
		woods1->x -= 2;
		canvasUpdate();
		if (inputPoll() != INPUT_NONE)
			break;
	}

	woods->x = -190;
	woods1->x = 130;

	setScroll(2, "A lonely stroll along an unfamiliar forest path brings you upon a curious gypsy wagon, its exotic colors dappled in the summer shade.", 7, 303, 8, 12, 1, 0x98, true);

	waitInput();

	setScroll(2, "A woman's voice rings out with friendship, beckoning you into across the wagon's threshold and, as it happens, into another life....", 7, 303, 8, 12, 1, 0x98, true);

	waitInput();

	_scroll->visible = false;

	fadeOut();
	_cutscene->hide_sprites();
	_cutscene->load_palette("palettes.int", 5);

	loadImages("gypsy.shp", _gypsyImages);

	bg->image = gypsyImg(0);
	bg->x = 0;
	bg->y = -20;
	bg->visible = true;

	g._arm = newSprite(nullptr, 0, 0, false);
	g._pour = newSprite(nullptr, 0, 0, false);

	g._jarLevel = 0;
	g._jarLiquid = newSprite(nullptr, 0x92, 0x53, false);
	setClip(g._jarLiquid, 0, 0, 320, 0x6d);

	g._jar = newSprite(gypsyImg(16), 0x8e, 0x38, true);

	_bubbleCounter = 0;

	static const int VIAL_IMG_OFF[8] = { 2, 5, 8, 0xB, 0x18, 0x1B, 0x1E, 0x21 };
	static const int VIAL_LIQUID_X[8] = { 44, 65, 86, 107, 203, 224, 245, 266 };
	static const int VIAL_X[8] = { 41, 62, 83, 104, 200, 221, 242, 263 };

	for (int i = 0; i < 8; ++i) {
		g._vialLevel[i] = 3;
		g._vialLiquid[i] = newSprite(gypsyImg(0x42 + VIAL_IMG_OFF[i] + g._vialLevel[i] - 3), VIAL_LIQUID_X[i], 0x4d, true);
	}

	g._hand = newSprite(nullptr, 0, 0, false);

	for (int i = 0; i < 8; ++i)
		g._vial[i] = newSprite(gypsyImg(i < 4 ? 20 : 23), VIAL_X[i], 0x42, true);

	fadeIn();

	_scroll->x = 1;
	_scroll->y = 0x7c;
	_scroll->visible = true;

	scrollImage = loadImage("blocks.shp", 3);
	_scroll->image = scrollImage;
	uint16 x = 8, y = 19;
	printWrapped(scrollImage, "\"At last thou hast come to fulfill thy destiny,\177 the gypsy says. She smiles, as if in great relief.", 7, 303, x, y, 0x3e);
	x = 8;
	y += 16;
	printWrapped(scrollImage, "\"Sit before me now, and I shall pour the light of Virtue into the shadows of thy future.\177", 7, 303, x, y, 0x3e);

	waitInput();

	scrollImage = loadImage("blocks.shp", 3);
	_scroll->image = scrollImage;
	x = 8;
	y = 19;
	printWrapped(scrollImage, "On a wooden table eight bottles stand, a rainbow of bubbling liquids.", 7, 303, x, y, 0x3e);
	x = 8;
	y += 16;
	printWrapped(scrollImage, "\"Behold the Virtues of the Avatar,\177 the woman says. \"Let us begin the casting!\177", 7, 303, x, y, 0x3e);

	waitInput();
	_cutscene->set_palette_entry(19, 200, 200, 200); // Fix the mouse cursor
	_cutscene->set_palette_entry(27, 68, 68, 68);

	showMouseCursor(true);
	CSSprite *aButton = newSprite(gypsyImg(7), 0x117, 0xae, true);
	CSSprite *bButton = newSprite(gypsyImg(8), 0x128, 0xae, true);
	CSImage *highlightImage = newImage(16, 2);
	_abHighlight = newSprite(highlightImage, aButton->x, aButton->y + 13, true);
	drawLine(highlightImage, 0, 0, 17, 0, 248);
	drawLine(highlightImage, 0, 1, 17, 1, 248);

	_str = 0xf;
	_dex = 0xf;
	_int = 0xf;

	for (int i = 0; i < 8; ++i)
		_questionTable[i] = i;

	shuffleQuestionTable(8);
	gypsyAskQuestions(4);

	shuffleQuestionTable(4);

	gypsyAskQuestions(2);
	gypsyAskQuestions(1);

	aButton->visible = false;
	bButton->visible = false;
	_abHighlight->visible = false;
	showMouseCursor(false);

	scrollImage = loadImage("blocks.shp", 3);
	_scroll->image = scrollImage;
	printText(scrollImage, (Common::String("\"The path of the Avatar lies beneath thy feet, worthy ") + name->text +
		",\177 the gypsy intones. With a mysterious smile, she passes you the flask of shimmering liquids. \"Drink of these waters and go forth among our people, who shall receive thee in joy!\177").c_str(),
		7, 303, 8, 16, 0x3e);

	do {
		gypsyUpdateBubbles(g._jarLiquid->image);
		canvasUpdate();
		input = inputPoll();
	} while (input == INPUT_NONE && !engineShouldQuit());

	fadeOut();

	_cutscene->hide_sprites();
	_cutscene->load_palette("palettes.int", 6);

	bubbles->visible = true;
	bg->x = 0;
	bg->y = 0;
	bg->visible = true;
	bg->image = gypsyImg(198);
	_bubbleCounter = 0;
	gypsyUpdateBubbles(bubbles->image);

	fadeIn();

	setScroll(2, "As you drink from the flask, vertigo overwhelms you. A soothing mist obscures the gypsy's face, and you sink without fear into an untroubled sleep.", 7, 303, 8, 8, 1, 0x98, true);

	do {
		gypsyUpdateBubbles(bubbles->image);
		canvasUpdate();
		input = inputPoll();
	} while (input == INPUT_NONE && !engineShouldQuit());

	_scroll->visible = false;

	_cutscene->set_bg_color(0x75);
	fadeOut();

	bubbles->visible = false;

	bg->image = gypsyImg(199);
	bg->x = 0;
	bg->y = 0;
	bg->visible = true;

	_scroll->visible = false;

	fadeIn();

	setScroll(2, "You wake in a different time, upon another world's shore. Though the Avatar's quests bring you both triumph and tragedy, never do you stray from the path of the Eight Virtues.", 7, 303, 8, 8, -1, -1, true);

	waitInput();

	setScroll(2, "The sagas of Ultima IV and Ultima V chronicle your perilous travels, and your name and your deeds are written forever among Britannia's legends....", 7, 303, 8, 8, -1, -1, true);

	waitInput();

	setScroll(2, "Finally, tempered by your struggles against the enemies of Virtue, you are proven ready to answer the epic challenge of Ultima VI!", 7, 303, 8, 12, -1, -1, true);

	waitInput();
	_scroll->visible = false;

	_cutscene->set_bg_color(0);
	fadeOut();

	config->set("config/newgame", true);

	config->set("config/newgamedata/name", name->text);
	config->set("config/newgamedata/gender", gender);
	config->set("config/newgamedata/portrait", portraitNum);
	config->set("config/newgamedata/str", _str);
	config->set("config/newgamedata/dex", _dex);
	config->set("config/newgamedata/int", _int);

	return true;
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
