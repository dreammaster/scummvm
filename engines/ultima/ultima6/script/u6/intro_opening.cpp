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
#include "common/system.h"
#include "ultima/ultima6/script/u6/intro.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

namespace {

struct TvProgram {
	const uint8 *_data;
	uint _size;
};

const uint8 TV_BREATHING[] = { 0x82, 0x82, 0x80, 0x3, 0x2, 0x8A, 0x2, 0x8A, 0x1, 0x8A, 0x1, 0x8A, 0x0, 0x8A, 0x0, 0x8A, 0x1, 0x8A, 0x1, 0x81 };
const uint8 TV_INTESTINES[] = { 0x82, 0x82, 0x80, 0x28, 0x3, 0x8B, 0x81 };
const uint8 TV_VACUUM[] = { 0x82, 0x82, 0x80, 0x4, 0x4, 0x81, 0x80, 0x4, 0x8, 0x81, 0x80, 0x4, 0x9, 0x81, 0x80, 0x4, 0x0A, 0x81, 0x80, 0x4, 0x0B, 0x81, 0x80, 0x4, 0x0C, 0x81 };
const uint8 TV_ANCHOR_MAN[] = { 0x82, 0x82, 0x87, 0x80, 0x46, 0x0F, 0x86, 0x84, 0x9, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x81 };
const uint8 TV_ENDLESS_ROAD[] = { 0x82, 0x82, 0x80, 0x32, 0x83, 0x81 };
const uint8 TV_ROCK_BAND[] = {
	0x82, 0x82, 0x80, 0x5, 0x27, 0x8A, 0x28, 0x8A, 0x29, 0x81, 0x80, 0x6, 0x2A, 0x8A, 0x2A, 0x8A, 0x2B, 0x8A, 0x2B, 0x8A,
	0x2C, 0x8A, 0x2C, 0x8A, 0x2D, 0x8A, 0x2D, 0x81, 0x80, 0x0A, 0x2E, 0x8A, 0x2F, 0x8A, 0x30, 0x8A, 0x84, 0x9, 0x2E, 0x2E,
	0x2E, 0x2F, 0x2F, 0x2F, 0x30, 0x30, 0x30, 0x8A, 0x2E, 0x8A, 0x2F, 0x8A, 0x30, 0x81
};
const uint8 TV_PLEDGE_NOW[] = {
	0x82, 0x82, 0x80, 0x55, 0x16, 0x17, 0x84, 0x0C, 0x13, 0x13, 0x13, 0x13, 0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15,
	0x88, 0x81, 0x80, 0x0F, 0x16, 0x84, 0x2, 0x1A, 0x1B, 0x89, 0x88, 0x81, 0x80, 0x3, 0x16, 0x1A, 0x89, 0x88, 0x81, 0x80,
	0x3, 0x16, 0x1C, 0x88, 0x81, 0x80, 0x3, 0x16, 0x1D, 0x88, 0x81, 0x80, 0x3, 0x16, 0x1E, 0x88, 0x81, 0x80, 0x3, 0x16,
	0x23, 0x88, 0x81, 0x80, 0x3, 0x16, 0x24, 0x88, 0x81, 0x80, 0x32, 0x16, 0x88, 0x81
};

const TvProgram TV_PROGRAMS[7] = {
	{ TV_BREATHING, ARRAYSIZE(TV_BREATHING) },
	{ TV_INTESTINES, ARRAYSIZE(TV_INTESTINES) },
	{ TV_VACUUM, ARRAYSIZE(TV_VACUUM) },
	{ TV_ANCHOR_MAN, ARRAYSIZE(TV_ANCHOR_MAN) },
	{ TV_ENDLESS_ROAD, ARRAYSIZE(TV_ENDLESS_ROAD) },
	{ TV_ROCK_BAND, ARRAYSIZE(TV_ROCK_BAND) },
	{ TV_PLEDGE_NOW, ARRAYSIZE(TV_PLEDGE_NOW) }
};

const int TV_NEWS_IMAGES[9] = { 0x5, 0x6, 0x7, 0x0D, 0x0E, 0x18, 0x19, 0x1F, 0x20 };

const uint8 TV_X_OFF[54] = {
	0x0, 0x0, 0x0, 0x0, 0x0, 0x1F, 0x1F, 0x1F, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1F, 0x1F,
	0x0, 0x9, 0x9, 0x9, 0x0C, 0x0C, 0x0C, 0x0, 0x4, 0x1F, 0x1F, 0x4, 0x0, 0x4,
	0x4, 0x4, 0x1F, 0x1F, 0x0, 0x0, 0x6, 0x6, 0x8, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

const uint8 TV_Y_OFF[54] = {
	0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x2, 0x0, 0x7, 0x7,
	0x7, 0x3, 0x3, 0x3, 0x0, 0x2, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x2, 0x0, 0x0, 0x3,
	0x8, 0x1D, 0x1C, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0
};

} // End of anonymous namespace

bool Intro::shouldExit(int input) const {
	return engineShouldQuit() || input == Common::KEYCODE_ESCAPE;
}

void Intro::fadeOutSprite(CSSprite *sprite, int speed) {
	for (int i = 0xff; i >= 0; i -= speed) {
		setOpacity(sprite, i);
		canvasUpdate();

		if (engineShouldQuit())
			break;
	}
}

void Intro::fadeInSprite(CSSprite *sprite, int speed) {
	for (int i = 0; i <= 0xff; i += speed) {
		setOpacity(sprite, i);
		canvasUpdate();

		if (engineShouldQuit())
			break;
	}
}

void Intro::loadClock() {
	_clock[0] = newSprite(nullptr, 0xdd, 0x14, true);
	_clock[1] = newSprite(nullptr, 0xdd + 4, 0x14, true);
	_clock[2] = newSprite(nullptr, 0xdd + 0xa, 0x14, true);
	_clock[3] = newSprite(nullptr, 0xdd + 0xe, 0x14, true);
}

void Intro::displayClock() {
	TimeDate time;
	g_system->getTimeAndDate(time);
	int hour = time.tm_hour;
	int minute = time.tm_min;

	if (hour > 12)
		hour -= 12;

	_clock[0]->image = (hour < 10) ? img(12) : img(3);

	if (hour >= 10)
		hour -= 10;

	_clock[1]->image = img(hour + 2);
	_clock[2]->image = img(minute / 10 + 2);
	_clock[3]->image = img(minute % 10 + 2);
}

void Intro::loadLounge() {
	_lounge._bg0 = newSprite(img(15), 210, 0, true);
	_lounge._bg = newSprite(img(0), 0, 0, true);
	_lounge._bg1 = newSprite(img(1), 320, 0, true);
	_lounge._avatar = newSprite(img(0xd), 0, 63, true);

	for (int i = 0; i < 5; ++i) {
		_lounge._tv[i] = newSprite(nullptr, _tvBaseX, _tvBaseY, true);
		setClip(_lounge._tv[i], _tvBaseX, _tvBaseY, 57, 37);
	}

	_lounge._finger = newSprite(img(0xe), 143, 91, true);
	_lounge._tvStatic = newImage(57, 37);

	memcpy(_tvYOff, TV_Y_OFF, sizeof(_tvYOff));
	loadClock();
}

void Intro::displayTvSprite(int spriteIdx, int imageNum) {
	if (spriteIdx >= 5)
		return;

	CSSprite *sprite = _lounge._tv[spriteIdx];
	sprite->image = img(0x10 + imageNum);
	sprite->x = _tvBaseX + TV_X_OFF[imageNum];
	sprite->y = _tvBaseY + _tvYOff[imageNum];
	setClip(sprite, MAX(_tvBaseX, 0), _tvBaseY, 57, 37);
	sprite->visible = true;
}

void Intro::displayTv() {
	bool done = false;
	int spriteIdx = 0;

	for (int i = 0; i < 5; ++i)
		_lounge._tv[i]->visible = false;

	_lounge._finger->visible = false;

	while (!done) {
		const TvProgram &program = TV_PROGRAMS[_tvCurProgram];
		int item = program._data[_tvCurPos];

		if (item < 0x80) {
			displayTvSprite(spriteIdx, item);
			++spriteIdx;
		} else if (item == 0x82) {
			// Static
			if (spriteIdx < 5) {
				CSSprite *sprite = _lounge._tv[spriteIdx];
				sprite->image = _lounge._tvStatic;

				uint8 *data = sprite->image->shp->get_data();
				uint16 w, h;
				sprite->image->shp->get_size(&w, &h);
				memset(data, 16, w * h);
				for (int i = 0; i < 1000; ++i)
					data[NUVIE_RAND() % (w * h)] = 0;

				sprite->x = _tvBaseX;
				sprite->y = _tvBaseY;
				setClip(sprite, _tvBaseX, _tvBaseY, 57, 37);
				sprite->visible = true;
			}

			_lounge._finger->visible = true;
			done = true;
		} else if (item == 0x80) {
			// Loop start
			++_tvCurPos;
			_tvLoopCnt = program._data[_tvCurPos];
			_tvLoopPos = _tvCurPos;
		} else if (item == 0x81) {
			// Loop end
			if (_tvLoopCnt > 0) {
				_tvCurPos = _tvLoopPos;
				--_tvLoopCnt;
			}

			done = true;
		} else if (item == 0x83) {
			// Endless road
			--_tvRoadOffset;
			if (_tvRoadOffset == 0)
				_tvRoadOffset = 0xe;

			_tvYOff[0x22] = 0x15 - _tvRoadOffset;
			displayTvSprite(spriteIdx, 0x22);
			++spriteIdx;
			_tvYOff[0x22] = 0x24 - _tvRoadOffset;
			displayTvSprite(spriteIdx, 0x22);
			++spriteIdx;
			displayTvSprite(spriteIdx, 0x21);
			++spriteIdx;
		} else if (item == 0x84) {
			// Select a random image from a list
			int randLen = program._data[_tvCurPos + 1];
			item = program._data[_tvCurPos + randomRange(1, randLen) + 1];
			displayTvSprite(spriteIdx, item);
			++spriteIdx;
			_tvCurPos += 1 + randLen;
		} else if (item == 0x86) {
			// Display the news image
			displayTvSprite(spriteIdx, _tvNewsImage);
			++spriteIdx;
		} else if (item == 0x87) {
			// Select a news image
			_tvNewsImage = TV_NEWS_IMAGES[randomRange(0, 8)];
		} else if (item == 0x88) {
			// Pledge now!
			++_tvPledgeCounter;
			if (_tvPledgeCounter % 4 == 0) {
				displayTvSprite(spriteIdx, _tvPledgeImage);
				++spriteIdx;
			}

			if (_tvPledgeCounter == 16) {
				_tvPledgeImage = (_tvPledgeImage == 37) ? 38 : 37;
				_tvPledgeCounter = 0;
			}
		} else if (item == 0x89) {
			displayTvSprite(spriteIdx, randomRange(50, 52));
			++spriteIdx;
		} else if (item == 0x8a) {
			done = true;
		} else if (item == 0x8b) {
			_cutscene->rotate_palette(0xe0, 5);
		}

		++_tvCurPos;
		if (_tvCurPos >= program._size) {
			++_tvCurProgram;
			_tvCurPos = 0;

			if (_tvCurProgram >= ARRAYSIZE(TV_PROGRAMS))
				_tvCurProgram = 0;
		}
	}
}

void Intro::displayLounge() {
	displayClock();

	if (_tvUpdate) {
		displayTv();
		_tvUpdate = false;
	} else {
		_tvUpdate = true;
	}
}

void Intro::scrollLounge() {
	_lounge._bg->x -= 1;
	_lounge._bg1->x -= 1;
	_tvBaseX -= 1;
	_lounge._avatar->x -= 2;
	_lounge._finger->x -= 2;

	if (_lounge._bg1->x < 166 && _lounge._bg1->x % 2 == 0)
		_lounge._bg0->x -= 1;

	for (int i = 0; i < 4; ++i)
		_clock[i]->x -= 1;
}

void Intro::hideLounge() {
	// The original also tried to hide the TV with a table assignment, which did nothing
	_lounge._bg0->visible = false;
	_lounge._bg->visible = false;
	_lounge._bg1->visible = false;
	_lounge._avatar->visible = false;
}

bool Intro::loungeSequence() {
	loadLounge();

	_cutscene->load_palette("palettes.int", 1);

	CSImage *scrollImage = loadImage("blocks.shp", 1);
	CSSprite *scroll = newSprite(scrollImage, 1, 0x9f, true);

	uint16 x = 8, y = 10;
	printWrapped(scrollImage, "Upon your world, five seasons have passed since your ", 8, 312, x, y, 0x3e);
	printWrapped(scrollImage, "triumphant homecoming from Britannia.", 8, 312, x, y, 0x3e);

	int input = inputPoll();

	while (input == INPUT_NONE && !engineShouldQuit()) {
		displayLounge();
		canvasUpdate();
		input = inputPoll();
	}

	if (shouldExit(input))
		return false;

	scrollImage = loadImage("blocks.shp", 2);
	x = 7;
	y = 8;
	printWrapped(scrollImage, "You have traded the Avatar's life of peril and adventure ", 7, 310, x, y, 0x3e);
	printWrapped(scrollImage, "for the lonely serenity of a world at peace. But ", 7, 310, x, y, 0x3e);
	printWrapped(scrollImage, "television supermen cannot take the place of friends ", 7, 310, x, y, 0x3e);
	printWrapped(scrollImage, "who died at your side!", 7, 310, x, y, 0x3e);

	scroll->image = scrollImage;
	scroll->x = 1;
	scroll->y = 0x98;

	input = inputPoll();

	while (input == INPUT_NONE && !engineShouldQuit()) {
		displayLounge();
		canvasUpdate();
		input = inputPoll();
	}

	if (shouldExit(input))
		return false;

	scrollImage = loadImage("blocks.shp", 0);
	printText(scrollImage, "Outside, a chill wind rises...", 39, 200, 39, 8, 0x3e);

	scroll->image = scrollImage;
	scroll->x = 0x21;
	scroll->y = 0x9d;

	for (int i = 0; i < 320; ++i) {
		scrollLounge();
		displayLounge();
		canvasUpdate();
		input = inputPoll();
		if (shouldExit(input))
			return false;
	}

	input = inputPoll();
	while (input == INPUT_NONE && !engineShouldQuit()) {
		canvasUpdate();
		input = inputPoll();
	}

	if (shouldExit(input))
		return false;

	hideLounge();
	scroll->visible = false;

	return true;
}

void Intro::loadWindow() {
	Window &w = _window;
	w._cloudX = -400;

	w._sky = newSprite(img(1), 0, 0, true);
	w._cloud1 = newSprite(img(0), w._cloudX, -5, true);
	w._cloud2 = newSprite(img(0), w._cloudX, -5, true);

	for (int i = 0; i < 5; ++i)
		w._clouds[i] = newSprite(img(randomRange(2, 3)), randomRange(0, 320) - 260, randomRange(0, 30), true);

	w._lightning = newSprite(nullptr, 0, 0, false);
	w._ground = newSprite(img(10), 0, 0x4c, true);
	w._trees = newSprite(img(8), 0, 0, true);

	w._strike = newSprite(img(randomRange(19, 23)), 158, 114, false);

	for (int i = 0; i <= 100; ++i)
		w._rain[i] = newSprite(img(randomRange(4, 7)), randomRange(0, 320), randomRange(0, 200), false);

	w._frame = newSprite(img(28), 0, 0, true);
	w._window = newSprite(img(26), 0x39, 0, true);
	w._doorLeft = newSprite(img(24), 320, 0, true);
	w._doorRight = newSprite(img(25), 573, 0, true);

	w._flash = 0;
	w._drops = 0;
	w._rainDelay = 20;
	w._lightningCounter = 0;
}

void Intro::hideWindow() {
	Window &w = _window;

	w._sky->visible = false;
	w._cloud1->visible = false;
	w._cloud2->visible = false;

	for (int i = 0; i < 5; ++i)
		w._clouds[i]->visible = false;

	w._lightning->visible = false;
	w._ground->visible = false;
	w._trees->visible = false;
	w._strike->visible = false;

	for (int i = 0; i <= 100; ++i)
		w._rain[i]->visible = false;

	w._frame->visible = false;
	w._window->visible = false;
	w._doorLeft->visible = false;
	w._doorRight->visible = false;
}

void Intro::displayWindow() {
	Window &w = _window;

	for (int i = 0; i < 5; ++i) {
		CSSprite *cloud = w._clouds[i];

		if (cloud->x > 320) {
			cloud->x = randomRange(0, 320) - 320;
			cloud->y = randomRange(0, 30);
		}

		cloud->x += 2;
	}

	++w._cloudX;
	if (w._cloudX == 320)
		w._cloudX = 0;

	w._cloud1->x = w._cloudX;
	w._cloud2->x = w._cloudX - 320;

	if (randomRange(0, 6) == 0 && w._lightningCounter == 0) {
		w._lightningCounter = randomRange(1, 4);
		w._lightning->image = img(randomRange(11, 18));
		w._lightning->visible = true;
		w._lightningX = randomRange(-5, 320);
		w._lightningY = randomRange(-5, 200);
	}

	if (w._lightningCounter > 0) {
		w._lightning->x = w._lightningX + randomRange(0, 3);
		w._lightning->y = w._lightningY + randomRange(0, 3);
	}

	if ((w._lightningCounter > 0 && randomRange(0, 3) == 0) || w._strike->visible) {
		_cutscene->set_palette_entry(0x58, 0x40, 0x94, 0xfc);
		_cutscene->set_palette_entry(0x5a, 0x40, 0x94, 0xfc);
		_cutscene->set_palette_entry(0x5c, 0x40, 0x94, 0xfc);
	} else {
		_cutscene->set_palette_entry(0x58, 0x20, 0xa4, 0x80);
		_cutscene->set_palette_entry(0x5a, 0x14, 0x8c, 0x74);
		_cutscene->set_palette_entry(0x5c, 0x0c, 0x74, 0x68);
	}

	if (randomRange(0, 1) == 0)
		w._strike->image = img(randomRange(19, 23));

	if (w._flash > 0) {
		--w._flash;
	} else if (randomRange(0, 5) == 0 || w._strike->visible) {
		w._window->image = img(27);
		w._flash = randomRange(1, 5);
	} else {
		w._window->image = img(26);
	}

	if (w._drops < 100) {
		if (randomRange(0, w._rainDelay) == 0) {
			w._rainDelay -= 2;
			if (w._rainDelay < 1)
				w._rainDelay = 1;

			int i = w._drops;
			w._rain[i]->visible = true;
			w._rain[i]->image = img(randomRange(4, 7));
			w._rain[i]->y = -4;
			w._rain[i]->x = randomRange(0, 320);
			w._drops = i + 1;
		}
	}

	for (int i = 0; i <= w._drops; ++i) {
		CSSprite *rain = w._rain[i];

		if (rain->visible) {
			rain->x += 2;
			rain->y += 8;
			if (rain->x > 320 || rain->y > 200)
				rain->visible = false;
		} else {
			rain->visible = true;
			rain->image = img(randomRange(4, 7));
			rain->y = -4;
			rain->x = randomRange(0, 320);
		}
	}

	if (w._lightningCounter > 0)
		--w._lightningCounter;
}

bool Intro::windowUpdate() {
	int input = inputPoll();

	while (input == INPUT_NONE && !engineShouldQuit()) {
		displayWindow();
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE)
			break;
		canvasUpdate();
	}

	return shouldExit(input);
}

bool Intro::windowSequence() {
	Window &w = _window;
	loadImages("intro_2.shp", _images);

	loadWindow();

	_cutscene->load_palette("palettes.int", 2);

	int i = 0;
	int input;
	while (i < 20) {
		displayWindow();
		canvasUpdate();
		input = inputPoll();
		if (shouldExit(input))
			return false;

		++i;
		canvasUpdate();
	}

	CSImage *scrollImage = loadImage("blocks.shp", 1);
	CSSprite *scroll = newSprite(scrollImage, 1, 0x98, true);

	printText(scrollImage, "...and in moments, the storm is upon you.", 8, 312, 36, 14, 0x3e);

	if (windowUpdate())
		return false;

	uint16 x, y;
	scrollImage = loadImage("blocks.shp", 1);
	x = 8;
	y = 10;
	printWrapped(scrollImage, "Tongues of lightning lash the sky, conducting an unceasing ", 8, 310, x, y, 0x3e);
	printWrapped(scrollImage, "crescendo of thunder....", 8, 310, x, y, 0x3e);
	scroll->image = scrollImage;

	if (windowUpdate())
		return false;

	scrollImage = loadImage("blocks.shp", 1);
	x = 8;
	y = 10;
	printWrapped(scrollImage, "In a cataclysm of sound and light, a bolt of searing ", 8, 310, x, y, 0x3e);
	printWrapped(scrollImage, "blue fire strikes the earth!", 8, 310, x, y, 0x3e);
	scroll->image = scrollImage;

	w._strike->visible = true;

	if (windowUpdate())
		return false;

	scrollImage = loadImage("blocks.shp", 1);
	printText(scrollImage, "Lightning among the stones!", 8, 310, 73, 10, 0x3e);
	printText(scrollImage, "Is this a sign from distant Britannia?", 8, 310, 41, 18, 0x3e);
	scroll->image = scrollImage;

	// Scroll the window
	i = 0;
	while (i < 320) {
		displayWindow();
		canvasUpdate();
		input = inputPoll();
		if (shouldExit(input))
			return false;

		canvasUpdate();

		w._window->x -= 2;
		w._frame->x -= 2;
		w._doorLeft->x -= 2;
		w._doorRight->x -= 2;
		i += 2;
		if (i > 160 && w._strike->visible)
			w._strike->visible = false;
	}

	if (windowUpdate())
		return false;

	scroll->visible = false;
	i = 0;
	while (i < 68) {
		displayWindow();
		canvasUpdate();
		input = inputPoll();
		if (shouldExit(input))
			return false;

		canvasUpdate();
		input = inputPoll();

		w._doorLeft->x -= 1;
		w._doorRight->x += 1;
		++i;
	}

	scrollImage = loadImage("blocks.shp", 2);
	x = 8;
	y = 12;
	printWrapped(scrollImage, "You bolt from your house, stumbling, running blind in the", 7, 310, x, y, 0x3e);
	printWrapped(scrollImage, " storm. Into the forest, down the path, through the ", 7, 310, x, y, 0x3e);
	printWrapped(scrollImage, "rain... to the stones.", 7, 310, x, y, 0x3e);
	scroll->image = scrollImage;
	scroll->visible = true;

	if (windowUpdate())
		return false;

	scroll->visible = false;
	return true;
}

void Intro::stonesRotatePalette() {
	if (_palCounter == 4) {
		_cutscene->rotate_palette(144, 16);
		_palCounter = 0;
	} else {
		++_palCounter;
	}
}

bool Intro::stonesUpdate() {
	int input = inputPoll();

	while (input == INPUT_NONE && !engineShouldQuit()) {
		stonesRotatePalette();
		canvasUpdate();
		input = inputPoll();
	}

	return shouldExit(input);
}

bool Intro::stonesShakeMoongate() {
	int input = inputPoll();

	while (input == INPUT_NONE && !engineShouldQuit()) {
		stonesRotatePalette();
		_stones._moonGate->x = 0x7c + randomRange(0, 1);
		_stones._moonGate->y = 5 + randomRange(0, 3);
		canvasUpdate();
		stonesRotatePalette();
		_stones._moonGate->x = 0x7c + randomRange(0, 1);
		_stones._moonGate->y = 5 + randomRange(0, 3);
		canvasUpdate();
		input = inputPoll();
	}

	return shouldExit(input);
}

bool Intro::stonesSequence() {
	Stones &s = _stones;
	loadImages("intro_3.shp", _images);

	_cutscene->load_palette("palettes.int", 3);

	s._bg = newSprite(img(0), 0, 0, true);
	s._stoneCover = newSprite(img(3), 0x96, 0x64, false);
	s._gateCover = newSprite(img(4), 0x5e, 0x66, false);
	s._hand = newSprite(img(1), 0xbd, 0xc7, false);
	s._moonGate = newSprite(img(2), 0x7c, 0x64, false);
	setClip(s._moonGate, 0, 0, 320, 0x66);
	s._avatar = newSprite(img(7), -2, 0x12, false);

	CSImage *scrollImage = loadImage("blocks.shp", 2);
	CSSprite *scroll = newSprite(scrollImage, 1, 0xc, true);

	uint16 x = 7, y = 8;
	printWrapped(scrollImage, "Near the stones, the smell of damp, blasted earth hangs ", 7, 303, x, y, 0x3e);
	printWrapped(scrollImage, "in the air. In a frozen moment of lightning-struck ", 7, 303, x, y, 0x3e);
	printWrapped(scrollImage, "daylight, you glimpse a tiny obsidian stone in the ", 7, 303, x, y, 0x3e);
	printWrapped(scrollImage, "midst of the circle!", 7, 303, x, y, 0x3e);

	fadeIn();

	if (stonesUpdate())
		return false;

	s._stoneCover->visible = true;
	s._hand->visible = true;

	scrollImage = loadImage("blocks.shp", 0);
	printText(scrollImage, "Wondering, you pick it up....", 8, 234, 0x2a, 8, 0x3e);
	scroll->image = scrollImage;
	scroll->x = 0x21;
	scroll->y = 0x1e;

	int input;
	int i;
	for (i = 0xc7; i >= 0x54; i -= 2) {
		s._hand->y = i;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE && shouldExit(input))
			return false;
	}

	if (stonesUpdate())
		return false;

	s._bg->image = img(5);
	s._stoneCover->visible = false;
	s._gateCover->visible = true;

	scrollImage = loadImage("blocks.shp", 1);
	x = 7;
	y = 10;
	printWrapped(scrollImage, "...and from the heart of the stones, a softly glowing door ", 7, 303, x, y, 0x3e);
	printWrapped(scrollImage, "ascends in silence!", 7, 303, x, y, 0x3e);
	scroll->image = scrollImage;
	scroll->x = 0x1;
	scroll->y = 0xa0;

	s._moonGate->visible = true;

	for (i = 0x64; i >= 0x5; --i) {
		stonesRotatePalette();
		s._moonGate->y = i;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE && shouldExit(input))
			return false;
	}

	for (i = 0x54; i <= 0xc7; i += 2) {
		stonesRotatePalette();
		s._hand->y = i;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE && shouldExit(input))
			return false;
	}

	s._hand->visible = false;

	if (stonesUpdate())
		return false;

	s._hand->image = img(6);
	s._hand->x = 0x9b;
	s._hand->visible = true;
	scroll->visible = false;

	for (i = 0xc7; i >= 0x44; i -= 2) {
		stonesRotatePalette();
		s._hand->y = i;
		s._bg->y -= 1;
		s._gateCover->y -= 1;
		s._moonGate->y -= 1;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE && shouldExit(input))
			return false;
	}

	scrollImage = loadImage("blocks.shp", 2);
	x = 7;
	y = 8;
	printWrapped(scrollImage, "Exultant memories wash over you as you clutch the stone. ", 7, 303, x, y, 0x3e);
	printWrapped(scrollImage, "When last you saw an orb such as this, it was cast down ", 7, 303, x, y, 0x3e);
	printWrapped(scrollImage, "by Lord British to banish the tyrant Blackthorn!", 7, 303, x, y, 0x3e);
	scroll->image = scrollImage;
	scroll->x = 0x1;
	scroll->y = 0x98;
	scroll->visible = true;

	if (stonesUpdate())
		return false;

	scroll->visible = false;

	for (i = 0x44; i <= 0xc7; i += 2) {
		stonesRotatePalette();
		s._hand->y = i;
		s._bg->y += 1;
		s._gateCover->y += 1;
		s._moonGate->y += 1;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE && shouldExit(input))
			return false;
	}

	s._hand->visible = false;

	scrollImage = loadImage("blocks.shp", 2);
	printText(scrollImage, "But your joy soon gives way to apprehension.", 7, 303, 16, 8, 0x3e);
	printText(scrollImage, "The gate to Britannia has always been blue...", 7, 303, 18, 24, 0x3e);
	printText(scrollImage, "as blue as the morning sky.", 7, 303, 76, 32, 0x3e);
	scroll->image = scrollImage;
	scroll->visible = true;

	if (stonesUpdate())
		return false;

	scrollImage = loadImage("blocks.shp", 1);
	x = 7;
	y = 10;
	printWrapped(scrollImage, "Abruptly, the portal quivers and begins to sink ", 7, 303, x, y, 0x3e);
	printWrapped(scrollImage, "into the ground.  Its crimson light wanes!", 7, 303, x, y, 0x3e);
	scroll->image = scrollImage;
	scroll->x = 0x1;
	scroll->y = 0xa0;

	if (stonesShakeMoongate())
		return false;

	scrollImage = loadImage("blocks.shp", 1);
	printText(scrollImage, "Desperation makes the decision an easy one.", 7, 303, 22, 14, 0x3e);
	scroll->image = scrollImage;
	scroll->x = 0x1;
	scroll->y = 0xa0;

	if (stonesShakeMoongate())
		return false;

	scroll->visible = false;

	s._avatar->visible = true;

	_cutscene->set_palette_entry(0x19, 0, 0, 0);

	for (i = 0; i < 20; ++i) {
		s._avatar->image = img(7 + i);

		for (int j = 0; j < 5; ++j) {
			canvasUpdate();
			stonesRotatePalette();
			s._moonGate->x = 0x7c + randomRange(0, 1);
			s._moonGate->y = 5 + randomRange(0, 3);
		}

		s._avatar->y -= 3;

		input = inputPoll();
		if (input != INPUT_NONE && shouldExit(input))
			return false;
	}

	for (i = 0xff; i >= 0; i -= 3) {
		canvasUpdate();
		stonesRotatePalette();
		s._moonGate->x = 0x7c + randomRange(0, 1);
		s._moonGate->y = 5 + randomRange(0, 3);

		setOpacity(s._avatar, i);

		input = inputPoll();
		if (input != INPUT_NONE && shouldExit(input))
			return false;
	}

	_cutscene->set_palette_entry(0x19, 0x74, 0x74, 0x74);

	s._moonGate->x = 0x7c;

	for (i = 0x5; i <= 0x64; ++i) {
		stonesRotatePalette();
		s._moonGate->y = i;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE && shouldExit(input))
			return false;
	}

	s._bg->image = img(0);
	s._moonGate->visible = false;
	s._gateCover->visible = false;
	s._stoneCover->visible = true;

	if (stonesUpdate())
		return false;

	return true;
}

void Intro::playOpening() {
	setMousePointer(0);
	showMouseCursor(false);
	loadImages("intro_1.shp", _images);
	musicPlay("bootup.m", 0);

	_cutscene->load_palette("palettes.int", 0);
	_cutscene->set_update_interval(25);

	CSSprite *background = newSprite(img(0x45), 0, 0, false);
	setOpacity(background, 0);
	background->visible = true;

	musicPlay("bootup.m", 0);

	int i;
	for (i = 0; i <= 0xff; i += 3) {
		setOpacity(background, i);
		if (pollForEsc())
			return;
		canvasUpdate();
	}

	for (i = 0xff; i >= 0; i -= 3) {
		setOpacity(background, i);
		if (pollForEsc())
			return;
		canvasUpdate();
	}

	background->image = img(0x46);

	for (i = 0; i <= 0xff; i += 3) {
		setOpacity(background, i);
		if (pollForEsc())
			return;
		canvasUpdate();
	}

	for (i = 0xff; i >= 0; i -= 3) {
		setOpacity(background, i);
		if (pollForEsc())
			return;
		canvasUpdate();
	}

	background->visible = false;

	if (!loungeSequence())
		return;

	if (!windowSequence())
		return;

	fadeOut();
	hideWindow();

	stonesSequence();
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
