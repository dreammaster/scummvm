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
#include "ultima/ultima6/script/u6/intro.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

int Intro::waitInput() {
	int input;

	do {
		canvasUpdate();
		input = inputPoll();
	} while (input == INPUT_NONE && !engineShouldQuit());

	return input;
}

void Intro::introExit() {
	fadeOut();
	_cutscene->hide_sprites();
	_cutscene->load_palette("palettes.int", 0);
}

bool Intro::introWait() {
	if (shouldExit(waitInput())) {
		introExit();
		return false;
	}

	return true;
}

void Intro::swayGargs(CSSprite *sprite, int &idx) {
	static const int MOVEMENT[12] = { 1, 1, 1, -1, -1, -1, -1, -1, -1, 1, 1, 1 };

	if (randomRange(0, 3) == 0)
		return;

	idx = (idx == 12) ? 1 : idx + 1;
	sprite->x += MOVEMENT[idx - 1];
}

void Intro::moongateRotatePalette() {
	if (_palCounter == 3) {
		_cutscene->rotate_palette(232, 8);
		_palCounter = 0;
	} else {
		++_palCounter;
	}
}

void Intro::setScroll(int block, const char *text, uint16 startX, uint16 width, uint16 x, uint16 y, int scrollX,
		int scrollY, bool show) {
	CSImage *image = loadImage("blocks.shp", block);
	printText(image, text, startX, width, x, y, 0x3e);
	_scroll->image = image;

	if (scrollX >= 0) {
		_scroll->x = scrollX;
		_scroll->y = scrollY;
	}

	if (show)
		_scroll->visible = true;
}

bool Intro::swayWait(bool rotate, bool doubleUpdate) {
	int input;

	do {
		swayGargs(_story._gargsLeft, _leftIdx);
		swayGargs(_story._gargsRight, _rightIdx);
		if (rotate)
			moongateRotatePalette();
		canvasUpdate();
		input = inputPoll();

		if (input != INPUT_NONE) {
			if (shouldExit(input)) {
				introExit();
				return false;
			}

			return true;
		}

		if (doubleUpdate)
			canvasUpdate();
	} while (!engineShouldQuit());

	introExit();
	return false;
}

bool Intro::story() {
	Story &s = _story;
	ImageTable images;
	int input;
	int i;

	loadImages("intro.shp", images);

	s._bg = newSprite(images[6][0], 0, 0, true);
	s._moongate = newSprite(images[7][0], 0x78, 0x3a, false);
	s._gargsLeft = newSprite(images[3][0], -84, 0x6d, true);
	s._gargsRight = newSprite(images[4][0], 326, 0xc7, true);
	s._gargBody = newSprite(images[8][0], 0xd5, 0x8d, false);
	s._gargHead = newSprite(images[11][0], 0x123, 0x9b, false);
	s._iolo = newSprite(images[1][0], 0xa8, 0xca, false);
	s._shamino = newSprite(images[2][0], 0x44, 0x7a, false);
	s._dupre = newSprite(images[0][0], -0x20, 0x7a, false);

	s._avatar = newSprite(images[9][0], 0x31, 0x44, false);
	s._alter = newSprite(images[5][0], 0, 0x70, true);
	s._ropes = newSprite(images[12][0], 0xd2, 0x84, false);

	_cutscene->load_palette("palettes.int", 7);
	musicPlay("intro.m", 0);

	fadeIn();

	CSImage *scrollImage = loadImage("blocks.shp", 2);
	_scroll = newSprite(scrollImage, 1, 0x98, true);

	printText(scrollImage, "Dazed, you emerge from the portal to find yourself standing on a desolate plain. Nearby rests a massive rune-struck altar, shrouded in moonlit fog.", 7, 308, 8, 8, 0x3e);

	if (!introWait())
		return false;

	setScroll(2, "At first the plain is still. Then a hundred voices raise a slow, deathlike song, drawing closer and closer with each passing moment. You are seized by an urge to run...", 7, 308, 8, 8);

	if (!introWait())
		return false;

	static const int L_MOVE_X[6] = { 1, 1, 0, 1, 1, 1 };
	static const int L_MOVE_Y[6] = { 0, 0, -1, 0, 0, -1 };
	static const int R_MOVE_X[6] = { -1, -1, -1, -1, -1, -1 };
	static const int R_MOVE_Y[6] = { -1, -1, 0, -1, -1, -1 };

	_scroll->visible = false;

	for (i = 0; i < 96; ++i) {
		s._gargsLeft->x += L_MOVE_X[i % 6];
		s._gargsLeft->y += L_MOVE_Y[i % 6];

		if (i > 23) {
			s._gargsRight->x += R_MOVE_X[i % 6] * 2;
			s._gargsRight->y += R_MOVE_Y[i % 6] * 2;
		}

		input = inputPoll();
		if (input != INPUT_NONE) {
			if (shouldExit(input)) {
				introExit();
				return false;
			}

			s._gargsLeft->x = -4;
			s._gargsLeft->y = 77;
			s._gargsRight->x = 182;
			s._gargsRight->y = 79;
			break;
		}

		canvasUpdate();
		canvasUpdate();
	}

	setScroll(0, "...but you have no place to go.", 7, 308, 35, 8, 0x21, 0x9d, true);

	if (!introWait())
		return false;

	setScroll(2, "Before you can offer a protest to the creatures who surround you, scaly claws grasp your body.", 7, 308, 8, 12, 0x1, 0x98);

	if (!introWait())
		return false;

	setScroll(1, "With unearthly strength, the monsters bind you to the altar stone!", 7, 308, 11, 10, 0x1, 0xa0);

	s._avatar->visible = true;
	s._ropes->visible = true;

	if (!introWait())
		return false;

	s._gargsLeft->y += 4;
	s._gargsRight->y += 4;
	_scroll->visible = false;

	s._gargBody->visible = true;
	s._gargHead->visible = true;

	input = INPUT_NONE;
	for (i = 0; i < 23; ++i) {
		s._gargBody->x -= 3;
		s._gargBody->y -= 3;
		s._gargHead->x -= 3;
		s._gargHead->y -= 3;

		input = inputPoll();
		if (input != INPUT_NONE) {
			if (shouldExit(input)) {
				introExit();
				return false;
			}

			break;
		}

		canvasUpdate();
		canvasUpdate();
	}

	if (input == INPUT_NONE) {
		for (i = 0; i < 14; ++i) {
			s._gargBody->y -= 3;
			s._gargHead->y -= 3;

			input = inputPoll();
			if (input != INPUT_NONE) {
				if (shouldExit(input)) {
					introExit();
					return false;
				}

				break;
			}

			canvasUpdate();
			canvasUpdate();
		}
	}

	s._gargBody->x = 144;
	s._gargBody->y = 30;
	s._gargHead->x = 222;
	s._gargHead->y = 44;

	setScroll(1, "Kneeling, the hordes sway and chant as a stately winged nightmare steps forward.", 32, 262, 33, 10, 0x1, 0xa0, true);

	_leftIdx = 1;
	_rightIdx = 6;

	if (!swayWait(false, true))
		return false;

	setScroll(2, "The leader unwraps a velvet covered, brass-bound book and recites from it in a formal, stilted tongue.", 7, 308, 8, 12, 0x1, 0x98, true);

	if (!swayWait(false, true))
		return false;

	setScroll(2, "Shouts and jeers explode from the masses as the priest slams shut the book. In his hand a malignant dagger drips with moonlight.", 7, 308, 8, 12);

	if (!swayWait(false, true))
		return false;

	setScroll(1, "You close your eyes. A dying scream, certainly your own, curdles the air.", 80, 228, 16, 10, 0x1, 0xa0);

	s._moongate->visible = true;
	_palCounter = 0;

	for (i = 0x78; i >= 0x3a; --i) {
		s._moongate->y = i;
		moongateRotatePalette();
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			if (shouldExit(input)) {
				introExit();
				return false;
			}

			s._moongate->y = 0x3a;
			break;
		}
	}

	input = INPUT_NONE;
	while (input == INPUT_NONE && !engineShouldQuit()) {
		moongateRotatePalette();
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE && shouldExit(input)) {
			introExit();
			return false;
		}
	}

	for (i = 0xff; i >= 0; i -= 3) {
		moongateRotatePalette();
		_cutscene->set_screen_opacity(i);
		canvasUpdate();
		inputPoll();
	}

	_cutscene->hide_sprites();

	setScroll(1, "Catcalls, the dagger, a scream, Death....", 7, 308, 39, 14, 0x1, 0x50, true);

	fadeIn();

	if (!introWait())
		return false;

	fadeOut();

	setScroll(1, "Pandemonium. Shrieks of rage, of terror.", 7, 308, 34, 14);

	fadeIn();

	if (!introWait())
		return false;

	fadeOut();

	setScroll(1, "From the inevitable, an impossibility emerges.", 7, 308, 16, 14);

	fadeIn();

	if (!introWait())
		return false;

	fadeOut();

	setScroll(1, "You are still alive.", 7, 308, 101, 14);

	fadeIn();

	if (!introWait())
		return false;

	fadeOut();

	s._bg->visible = true;
	s._moongate->visible = true;
	s._gargsLeft->visible = true;
	s._gargsRight->visible = true;
	s._gargBody->visible = true;
	s._gargHead->visible = true;
	s._alter->visible = true;
	s._avatar->visible = true;
	s._ropes->visible = true;

	_scroll->visible = false;

	for (i = 0; i <= 0xff; i += 3) {
		swayGargs(s._gargsLeft, _leftIdx);
		swayGargs(s._gargsRight, _rightIdx);
		moongateRotatePalette();
		_cutscene->set_screen_opacity(i);
		canvasUpdate();
		inputPoll();
	}

	s._gargHead->image = images[10][0];

	setScroll(2, "Silent red light fills the darkness. There is the wooden clack of a crossbow, and a violet- fletched rose blooms in the priest's barren forehead.", 7, 308, 8, 8, 0x1, 0x98, true);

	if (!swayWait(true, false))
		return false;

	_scroll->visible = false;

	for (i = 0; i <= 42; ++i) {
		swayGargs(s._gargsLeft, _leftIdx);
		swayGargs(s._gargsRight, _rightIdx);
		moongateRotatePalette();

		int x = randomRange(-1, 2);
		s._gargBody->x += x;
		s._gargBody->y += 3;
		s._gargHead->x += x;
		s._gargHead->y += 3;

		input = inputPoll();
		if (input != INPUT_NONE) {
			if (shouldExit(input)) {
				introExit();
				return false;
			}

			s._gargBody->visible = false;
			s._gargHead->visible = false;
			break;
		}

		canvasUpdate();
	}

	s._gargBody->visible = false;
	s._gargHead->visible = false;

	s._iolo->visible = true;
	s._dupre->visible = true;
	s._shamino->visible = true;

	for (i = 0; i <= 82; ++i) {
		swayGargs(s._gargsLeft, _leftIdx);
		swayGargs(s._gargsRight, _rightIdx);
		moongateRotatePalette();

		s._dupre->x += (i > 33) ? 2 : 1;
		s._dupre->y -= 1;

		if (i > 13) {
			s._shamino->x += (i > 36) ? 2 : 1;
			s._shamino->y -= 1;
		}

		if (i > 10) {
			s._iolo->x += 1;
			s._iolo->y -= (i > 14) ? 2 : 1;
		}

		input = inputPoll();
		if (input != INPUT_NONE) {
			if (shouldExit(input)) {
				introExit();
				return false;
			}

			s._dupre->x = 99;
			s._dupre->y = 40;
			s._shamino->x = 183;
			s._shamino->y = 53;
			s._iolo->x = 240;
			s._iolo->y = 62;
			break;
		}

		canvasUpdate();
	}

	setScroll(2, "Friendly faces vault from a newborn moongate, while a rain of quarrels holds the furious mob at bay. The knight Dupre's sword flashes twice in the darkness, slicing away your bonds!", 7, 308, 8, 8, 0x1, 0x98, true);

	if (!swayWait(true, false))
		return false;

	_scroll->visible = false;
	s._ropes->visible = false;

	for (i = 0; i <= 82; ++i) {
		swayGargs(s._gargsLeft, _leftIdx);
		swayGargs(s._gargsRight, _rightIdx);
		moongateRotatePalette();

		s._dupre->x -= 1;
		s._dupre->y += 2;

		s._shamino->x -= 1;
		s._shamino->y += 2;

		s._iolo->x -= 1;
		s._iolo->y += 2;

		s._avatar->y += 1;

		input = inputPoll();
		if (input != INPUT_NONE) {
			if (shouldExit(input)) {
				introExit();
				return false;
			}

			s._dupre->visible = false;
			s._shamino->visible = false;
			s._iolo->visible = false;
			s._avatar->visible = false;
			break;
		}

		canvasUpdate();
	}

	setScroll(2, "\"Quickly, old friend! To the gate!\177 Accompanied by the swordsman Shamino and a grinning, crossbow-wielding Iolo the Bard, Dupre thrusts a spare sword into your hand.", 7, 308, 8, 8, -1, -1, true);

	if (!swayWait(true, false))
		return false;

	setScroll(2, "Snatching the fallen priest's book, Iolo dives into the redness with Shamino at his heels. The howling throng surges forward, all of one terrible mind.", 7, 308, 8, 8);

	if (!swayWait(true, false))
		return false;

	setScroll(1, "The gate wanes rapidly as you and Dupre charge through...", 130, 178, 12, 10, 0x1, 0xa0);

	for (i = 0; i <= 150; ++i) {
		swayGargs(s._gargsLeft, _leftIdx);
		swayGargs(s._gargsRight, _rightIdx);
		if (i % 2 == 0)
			s._moongate->y += 1;
		moongateRotatePalette();
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			if (shouldExit(input)) {
				introExit();
				return false;
			}

			s._moongate->visible = false;
			break;
		}
	}

	setScroll(0, "...but not rapidly enough.", 7, 308, 50, 8, 0x21, 0x9d);

	if (!swayWait(false, false))
		return false;

	setScroll(2, "From the mob's vanguard, three of the abominations scramble toward the gate. Driven by fury, the creatures hurl their bodies into the portal's last handspan of light!", 7, 308, 8, 8, 0x1, 0x98);

	if (!swayWait(false, false))
		return false;

	_scroll->visible = false;

	introExit();
	return true;
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
