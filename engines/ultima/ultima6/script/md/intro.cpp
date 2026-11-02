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

#include "ultima/ultima6/conf/configuration.h"
#include "ultima/ultima6/script/md/intro.h"
#include "ultima/ultima6/script/md/md_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

bool Intro::waitForExit(int cycles) {
	pollForKeyOrButton(cycles);
	return _shouldExit;
}

void Intro::originFxSequence() {
	ImageTable images;
	if (!loadImages("title.lzc", images))
		return;

	_cutscene->load_palette("strax.pal", 0);

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

	musicPlay("strx_mus.lzc", 0);
	fadeIn();

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
	if (pollForKeyOrButton(135))
		return;

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

	if (pollForEsc(45))
		return;

	fireworks(images, logo);
	fadeOut();
}

void Intro::showLogos() {
	ImageTable images;
	if (!loadImages("logos.lzc", images))
		return;

	CSSprite *sprite = newSprite(images[0][0], 0, 0, true);
	musicPlay("mdd_mus.lzc", 0);
	fadeIn();

	if (waitForExit(195))
		return;

	sprite->image = images[1][0];
	fadeIn();
	if (waitForExit(195))
		return;

	sprite->image = images[2][0];
	sprite->x = 15;
	musicPlay("mdd_mus.lzc", 1);
	fadeIn();
	pollForKeyOrButton(175);
	sprite->visible = false;
}

void Intro::flashEffect(CSImage *image, int textDelay) {
	_cutscene->hide_sprites();

	newSprite(image, 0, 0, true);
	CSSprite *bg = newSprite(newImage(320, 200, 15), 0, 0, true);

	if (pollForKeyOrButton(6))
		return;

	const int speed = 10;
	for (int i = 0; i <= 0xff; i += speed) {
		setOpacity(bg, 0xff - i);
		canvasUpdate();
	}

	pollForKeyOrButton(textDelay);
}

void Intro::showFairGround() {
	ImageTable images;
	if (!loadImages("fair.lzc", images))
		return;

	_cutscene->hide_sprites();

	newSprite(images[0][0], 0, 24, true);
	CSSprite *buildings = newSprite(images[1][0], 0, 24, true);
	CSSprite *buildings1 = newSprite(images[1][1], 320, 24, true);
	CSSprite *wheel = newSprite(images[2][0], 0, 24, true);
	CSSprite *column = newSprite(images[3][0], 190, 24, true);
	CSSprite *podium = newSprite(images[4][0], 240, 24, true);
	CSSprite *people = newSprite(images[5][0], 0, 24, true);
	CSSprite *people1 = newSprite(images[5][1], 320, 24, true);

	for (int i = 0; i < 10; ++i) {
		wheel->image = images[2][i % 5];
		if (pollForKeyOrButton(7))
			return;
	}

	for (int i = 0; i < 80; ++i) {
		buildings->x = 0 - i;
		buildings1->x = 320 - i;
		wheel->x = 0 - i * 2;
		wheel->image = images[2][i % 5];
		column->x = 190 - i * 2;
		podium->x = 240 - i * 2;
		people->x = 0 - i * 3;
		people1->x = 320 - i * 3;
		if (pollForKeyOrButton(7))
			return;
	}

	pollForKeyOrButton(15);
}

void Intro::showLowell() {
	flashEffect(loadImage("credits.lzc", 0));

	_cutscene->hide_sprites();

	ImageTable images;
	if (!loadImages("lowell.lzc", images))
		return;

	newSprite(images[0][0], 0, 24, true);
	CSSprite *lowell = newSprite(images[0][1], 0, 24, true);

	fadeIn();

	for (int i = 1; i <= 20; ++i) {
		lowell->image = images[0][i];
		if (pollForKeyOrButton(5))
			return;
	}

	flashEffect(loadImage("credits.lzc", 1));

	pollForKeyOrButton(70);
}

void Intro::showFuse() {
	_cutscene->hide_sprites();

	ImageTable images;
	if (!loadImages("fuse.lzc", images))
		return;

	newSprite(images[0][0], 0, 24, true);
	CSSprite *fuse = newSprite(images[0][1], 0, 24, true);

	musicPlay("mdd_mus.lzc", 2);
	fadeIn(4);

	for (int i = 1; i <= 28; ++i) {
		fuse->image = images[0][i];
		if (pollForKeyOrButton(2))
			return;
	}

	flashEffect(loadImage("credits.lzc", 2));
}

void Intro::showFlash() {
	_cutscene->hide_sprites();

	ImageTable fair;
	if (!loadImages("fair.lzc", fair))
		return;

	newSprite(fair[0][0], 0, 24, true);
	newSprite(fair[1][0], -80, 24, true);
	newSprite(fair[1][1], 240, 24, true);
	newSprite(fair[3][0], 30, 24, true);
	newSprite(fair[4][0], 80, 24, true);

	musicPlay("mdd_mus.lzc", 3);

	ImageTable flash;
	if (!loadImages("flash.lzc", flash))
		return;

	fadeIn(8);
	playSfx(12); // FIXME this might not be the correct sfx for the explosion
	CSSprite *s = newSprite(flash[0][0], 80, 24, true);

	for (int i = 0; i < 10; ++i) {
		s->image = flash[0][i];
		if (pollForKeyOrButton(7))
			return;
	}
	for (int i = 0; i < 9; ++i) {
		s->image = flash[1][i];
		if (pollForKeyOrButton(6))
			return;
	}

	flashEffect(loadImage("credits.lzc", 3), 215);
}

void Intro::showFlight() {
	_cutscene->hide_sprites();

	ImageTable images;
	if (!loadImages("flight.lzc", images))
		return;

	newSprite(images[0][0], 0, 24, true);
	musicPlay("mdd_mus.lzc", 4);
	fadeIn(12);
	CSSprite *capsule = createSprite(images[1][0], 270, 140);

	for (int i = 0; i <= 12; ++i) {
		capsule->image->setScale(1 + i * i + (i * 2));
		capsule->x = 270 - i * 4 * i;
		capsule->y = 140 - i * 2 * i;
		if (pollForKeyOrButton(6))
			return;
	}

	if (pollForKeyOrButton(98))
		return;
	musicStop();
	pollForKeyOrButton(8);
}

void Intro::showCabin() {
	_cutscene->hide_sprites();

	ImageTable images;
	if (!loadImages("cabin.lzc", images))
		return;

	newSprite(images[0][0], 0, 24, true);
	CSSprite *cloud = newSprite(images[1][0], 116, 28, true);
	CSSprite *cloud1 = newSprite(images[1][0], 222, 53, true);
	CSSprite *man = createSprite(images[2][0], 0, 50);
	CSSprite *silhouette = createSprite(images[3][0], -900, -150);
	CSSprite *hat1 = createSprite(images[4][0], 62, -150);
	CSSprite *hat2 = createSprite(images[5][0], 62, 0);
	CSSprite *hat3 = createSprite(images[6][0], 62, 0);

	musicPlay("mdd_mus.lzc", 5);

	// FIXME rotate the sprites
	for (int i = 0; i <= 30; ++i) {
		cloud->image = images[1][(i + 2) % 15];
		cloud1->image = images[1][i % 15];
		silhouette->x = i * 64 - 900;
		silhouette->y = i * 16 - 150;
		man->x = 0 - (i * 4);
		man->y = i * i * 2 + 50;
		man->image->setScale(100 + i * 32);
		hat1->x = i * 32 + 62;
		hat1->y = (i > 6) ? i * 4 + 15 : i * 8 + 15;

		hat2->x = i * 32 - 150;
		hat2->y = i * 8;

		hat3->x = i * 32 - 500;
		hat3->y = i * 4;

		if (pollForKeyOrButton(8))
			return;
	}

	flashEffect(loadImage("credits.lzc", 4), 250);
}

void Intro::showMarsFlash() {
	_cutscene->hide_sprites();

	ImageTable images;
	if (!loadImages("mars.lzc", images))
		return;

	musicPlay("mdd_mus.lzc", 6);

	newSprite(images[0][0], 0, 24, true);
	CSSprite *flash = newSprite(images[0][1], 0, 24, true);

	fadeIn(12);

	if (!pollForKeyOrButton(60)) {
		for (int i = -1; i <= 1; ++i) {
			for (int j = 0; j <= 2; ++j) {
				for (int k = -1; k <= 2; ++k) {
					flash->image = images[0][3 - ABS(k)];

					int delay = (i == 0 && k == 0) ? 10 : 3;
					if (pollForKeyOrButton(delay)) {
						flashEffect(loadImage("credits.lzc", 5));
						return;
					}
				}

				if (pollForKeyOrButton(5)) {
					flashEffect(loadImage("credits.lzc", 5));
					return;
				}
			}

			if (pollForKeyOrButton(30)) {
				flashEffect(loadImage("credits.lzc", 5));
				return;
			}
		}
	}

	pollForKeyOrButton(40);

	flashEffect(loadImage("credits.lzc", 5));
}

void Intro::playIntro() {
	showLogos();
	if (_shouldExit)
		return;

	showFairGround();
	if (_shouldExit)
		return;

	showLowell();
	if (_shouldExit)
		return;

	showFuse();
	if (_shouldExit)
		return;

	showFlash();
	if (_shouldExit)
		return;

	showFlight();
	if (_shouldExit)
		return;

	showCabin();
	if (_shouldExit)
		return;

	showMarsFlash();
}

void Intro::showHome() {
	_cutscene->hide_sprites();

	Common::Array<Common::String> text = _cutscene->load_text("scenetxt.lzc", 0);
	ImageTable images;
	if (text.size() < 15 || !loadImages("scene1.lzc", images))
		return;

	CSSprite *bg = newSprite(images[0][0], 0, 24, true);
	CSSprite *door = newSprite(images[0][1], 0, 24, true);
	CSSprite *avatar = newSprite(images[1][0], 240, 151, true);
	CSSprite *textSprite = newSprite(nullptr, 0, 160, true);
	textSprite->text = text[1];
	textSprite->text_color = 14;
	textSprite->text_align = 2;
	fadeIn();
	musicPlay("mdd_mus.lzc", 8);

	if (waitForExit(75))
		return;

	CSSprite *woman = newSprite(images[8][0], 0, 151, true);
	CSSprite *spector = createSprite(images[2][0], 60, 151);

	door->visible = false;
	avatar->image = images[1][1];
	textSprite->text = text[2];

	if (waitForExit(100))
		return;

	avatar->image = images[1][3];
	spector->image = images[2][1];
	spector->x = 80;
	textSprite->text = text[3];
	textSprite->text_color = 2;

	if (waitForExit(100))
		return;

	woman->image = images[8][1];
	textSprite->text = text[4];
	textSprite->text_color = 14;

	if (waitForExit(100))
		return;

	woman->image = images[8][2];
	spector->image = images[2][2];
	spector->x = 100;
	textSprite->text = text[5];
	textSprite->text_color = 2;

	if (waitForExit(75))
		return;

	woman->image = images[3][0];
	woman->x = -10;
	woman->y = 155;
	spector->image = images[2][3];
	spector->x = 145;
	textSprite->text = text[6];
	textSprite->text_color = 9;

	if (waitForExit(75))
		return;

	// The woman hands Spector a note
	textSprite->text = text[7];
	for (int i = 0; i <= 10; ++i) {
		if (i <= 9)
			woman->image = images[3][i];
		spector->image = images[2][i + 5];
		pollForKeyOrButton(5);
	}

	if (waitForExit(50))
		return;

	textSprite->text = text[8];
	textSprite->text_color = 2;

	if (waitForExit(100))
		return;

	// Close up on the woman
	_cutscene->hide_sprites();

	bg->image = images[7][0];
	bg->visible = true;
	textSprite->text = text[9];
	textSprite->text_color = 9;
	textSprite->visible = true;

	if (waitForExit(100))
		return;

	// Spector and the avatar review the papers
	bg->image = images[0][0];
	avatar->image = images[1][1];
	avatar->x = 270;
	avatar->visible = true;

	spector->image = images[2][16];
	spector->x = 193;
	spector->y = 161;
	spector->visible = true;

	textSprite->text = text[10];
	textSprite->text_color = 2;

	if (waitForExit(75))
		return;

	// Close up on the papers
	_cutscene->hide_sprites();
	bg->image = images[5][0];
	bg->visible = true;

	CSSprite *letter = newSprite(images[5][1], 50, 24, true);
	CSSprite *hand = newSprite(images[4][0], 235, 78, true);

	textSprite->text = text[11];
	textSprite->text_color = 14;
	textSprite->visible = true;

	if (waitForExit(100))
		return;

	hand->visible = false;
	textSprite->text = text[12];
	textSprite->text_color = 2;

	if (waitForExit(100))
		return;

	// Pick up the letter
	letter->image = images[6][0];
	letter->x = 0;

	textSprite->text = text[13];
	// FIXME need to rotate the letter sprite
	for (int i = 0; i < 20; ++i) {
		letter->y = -20 - i;
		pollForKeyOrButton(1);
	}

	letter->visible = false;
	textSprite->text = text[14];
	textSprite->text_color = 14;

	if (waitForExit(100))
		return;

	fadeOut(6);
}

void Intro::showMap() {
	static const struct {
		int _x, _y;
	} CROSSHAIRS[12] = {
		{ 190, 80 }, { 180, 82 }, { 170, 84 }, { 162, 81 }, { 154, 75 }, { 145, 73 },
		{ 135, 73 }, { 125, 74 }, { 115, 76 }, { 105, 78 }, { 95, 81 }, { 86, 86 }
	};

	_cutscene->hide_sprites();

	CSSprite *bg = newSprite(loadImage("scene2.lzc", 0), 0, 24, true);
	fadeIn(6);

	if (waitForExit(50))
		return;

	for (int i = 0; i < 12; ++i) {
		int x = CROSSHAIRS[i]._x;
		int y = CROSSHAIRS[i]._y;

		drawLine(bg->image, x - 1, y - 24 - 5, x + 3, y - 24 - 5, 9);
		drawLine(bg->image, x + 1, y - 1 - 24 - 5, x + 1, y + 1 - 24 - 5, 9);

		if (waitForExit(20))
			return;
	}

	pollForKeyOrButton(50);

	fadeOut(6);
}

void Intro::showHike() {
	_cutscene->hide_sprites();

	ImageTable images;
	if (!loadImages("scene2.lzc", images))
		return;

	newSprite(images[1][0], 0, 24, true);
	CSSprite *avatar = newSprite(images[3][0], -15, 148, true);
	CSSprite *spector = newSprite(images[4][0], -45, 149, true);
	newSprite(images[2][0], 0, 24, true);

	fadeIn(6);

	for (int i = 0; i < 95; ++i) {
		avatar->image = images[3][i % 12];
		avatar->x = i * 4 - 15;
		spector->image = images[4][(i + 4) % 12];
		spector->x = i * 4 - 45;

		if (waitForExit(3))
			return;
	}

	fadeOut(6);
}

void Intro::showLabPresentDay() {
	// Text, text colour, avatar frame (or a negative close up), Spector frame, delay
	static const int LAB_TABLE[10][5] = {
		{ 1, 2, 0, 0, 50 },
		{ 1, 2, 0, 1, 100 },
		{ 2, 14, 0, 1, 100 },
		{ 3, 2, 0, 2, 100 },
		{ 4, 2, -1, 0, 100 },
		{ 5, 14, -2, 0, 100 },
		{ 6, 2, -1, 0, 100 },
		{ 7, 14, 1, 3, 100 },
		{ 8, 2, 1, 2, 100 },
		{ 9, 14, 0, 2, 100 }
	};

	_cutscene->hide_sprites();

	ImageTable images, moongate;
	if (!loadImages("scene3.lzc", images))
		return;

	Common::Array<Common::String> text = _cutscene->load_text("scenetxt.lzc", 1);
	if (text.size() < 10 || !loadImages("moongate.lzc", moongate))
		return;

	CSSprite *textSprite = newSprite(nullptr, 0, 160, true);
	textSprite->text = text[1];
	textSprite->text_color = 2;
	textSprite->text_align = 2;

	CSSprite *bg = newSprite(images[0][0], 0, 24, true);

	CSSprite *moongateSprite = createSprite(moongate[0][0], 125, 130);
	moongateSprite->visible = false;

	CSSprite *spector = newSprite(images[3][0], 165, 137, true);
	CSSprite *tableSprite = createSprite(images[1][0], 151, 82);
	CSSprite *avatar = createSprite(images[2][0], 65, 150);

	musicPlay("mdd_mus.lzc", 9);
	fadeIn(6);

	for (int i = 0; i < 7; ++i) {
		if (LAB_TABLE[i][2] >= 0) {
			spector->image = images[3][LAB_TABLE[i][3]];
		} else {
			spector->visible = false;
			tableSprite->visible = false;
			avatar->visible = false;
			bg->image = images[4][ABS(LAB_TABLE[i][2]) - 1];
		}

		textSprite->text = text[LAB_TABLE[i][0]];
		textSprite->text_color = LAB_TABLE[i][1];
		if (waitForExit(LAB_TABLE[i][4]))
			return;
	}

	bg->image = images[0][0];
	spector->visible = true;
	avatar->visible = true;
	tableSprite->visible = true;
	tableSprite->image = images[1][1];

	spector->image = images[3][3];
	avatar->image = images[2][1];

	moongateSprite->visible = true;

	// The moongate rises up from the floor
	for (int i = 0; i <= 8; ++i) {
		moongateSprite->image = moongate[0][i];
		if (waitForExit(4))
			return;
	}

	// The avatar and Spector discuss the moongate
	moongateSprite->image = moongate[1][0];
	for (int i = 7; i < 10; ++i) {
		for (int j = 0; j < 40; ++j) {
			moongateSprite->image = moongate[1][j % 8];
			avatar->image = images[2][LAB_TABLE[i][2]];
			spector->image = images[3][LAB_TABLE[i][3]];
			textSprite->text = text[LAB_TABLE[i][0]];
			textSprite->text_color = LAB_TABLE[i][1];

			if (waitForExit(4))
				return;
		}
	}

	spector->image = moongate[4][0];
	for (int i = 0; i <= 64; ++i) {
		moongateSprite->image = moongate[1][i % 8];
		avatar->image = moongate[2][i / 2];

		if (waitForExit(4))
			return;
	}

	avatar->visible = false;

	for (int i = 0; i < 40; ++i) {
		moongateSprite->image = moongate[1][i % 8];
		if (i != 39)
			spector->image = moongate[4][i / 2];

		if (waitForExit(4))
			return;
	}

	fadeOut(6);
}

void Intro::showLab1895() {
	// The colour of each line of text
	static const int TEXT_COLOURS[27] = {
		4, 6, 7, 7, 14, 2, 2, 7, 4, 4, 7, 9, 7, 4, 7, 4, 7, 2, 7, 7, 7, 7, 7, 11, 2, 7, 7
	};

	_cutscene->hide_sprites();

	ImageTable scene4a, scene4b, moongate;
	if (!loadImages("scene4a.lzc", scene4a) || !loadImages("scene4b.lzc", scene4b) ||
			!loadImages("moongate.lzc", moongate))
		return;

	Common::Array<Common::String> text = _cutscene->load_text("scenetxt.lzc", 2);
	if (text.size() < 27)
		return;

	CSSprite *textSprite = newSprite(nullptr, 0, 160, true);
	textSprite->text = text[1];
	textSprite->text_color = 6;
	textSprite->text_align = 2;

	CSSprite *bg = newSprite(scene4a[0][0], 0, 24, true);

	CSSprite *moongateSprite = createSprite(moongate[0][0], 140, 125);
	moongateSprite->visible = false;

	CSSprite *tesla = newSprite(scene4a[6][0], 187, 125, true);
	CSSprite *spark = newSprite(scene4b[2][0], 32, 24, false);

	CSSprite *freudBody = createSprite(scene4a[4][0], 0, 74);
	CSSprite *freudHead = createSprite(scene4a[4][2], 23, 100);

	CSSprite *nellie = newSprite(scene4b[1][0], 242, 24, false);

	CSSprite *bookcase = newSprite(scene4a[1][0], 242, 24, false);

	CSSprite *blood = newSprite(scene4a[5][0], 240, 40, true);
	CSSprite *garrett = newSprite(scene4b[0][0], 270, 45, false);

	fadeIn(6);

	musicPlay("mdd_mus.lzc", 10);

	for (int i = 1; i <= 2; ++i) {
		textSprite->text = text[i];

		for (int j = 0; j <= 20; ++j) {
			int frame = 10 - ABS(j - 10);
			tesla->image = scene4a[6][(frame > 5) ? 5 : frame];

			if (j > 4 && j < 16) {
				// FIXME need spark sfx
				spark->visible = true;
				spark->image = scene4b[2][j % 10];
			} else {
				spark->visible = false;
			}

			if (waitForExit(4))
				return;
		}

		textSprite->text_color = 7;
	}

	moongateSprite->visible = true;

	// The moongate rises up from the floor
	tesla->x = 188;
	tesla->y = 128;
	tesla->image = scene4a[6][6];

	for (int i = 0; i <= 8; ++i) {
		moongateSprite->image = moongate[0][i];
		if (i == 5)
			tesla->image = scene4a[6][7];

		if (i == 2)
			blood->image = scene4a[5][1];

		if (waitForExit(4))
			return;
	}

	// The avatar walks out of the newly risen moongate
	CSSprite *avatar = createSprite(moongate[3][0], 80, 150);

	tesla->x = 180;
	tesla->y = 133;
	tesla->image = scene4a[6][8];

	for (int i = 0; i <= 68; ++i) {
		moongateSprite->image = moongate[1][i % 8];
		avatar->image = moongate[3][i / 2];
		if (i == 3)
			blood->image = scene4a[5][2];

		if (i == 4)
			freudHead->image = scene4a[4][3];

		if (i == 11) {
			textSprite->text = text[3];
			textSprite->text_color = 7;
		}

		if (waitForExit(3))
			return;
	}

	// Spector walks out of the moongate
	CSSprite *spector = createSprite(moongate[5][0], 140, 130);

	textSprite->text = text[4];
	textSprite->text_color = 14;

	for (int i = 0; i <= 24; ++i) {
		moongateSprite->image = moongate[1][i % 8];
		spector->image = moongate[5][i / 2];

		if (waitForExit(3))
			return;
	}

	// Spector's face in close up
	_cutscene->hide_sprites();

	bg->image = scene4a[9][0];
	bg->visible = true;

	CSSprite *face = newSprite(scene4a[9][1], 0, 24, true);

	textSprite->text = text[5];
	textSprite->text_color = 2;
	textSprite->visible = true;

	if (waitForExit(200))
		return;

	// Spector shows the note to Tesla
	textSprite->text = text[6];
	face->visible = false;
	bg->image = scene4a[0][0];

	tesla->visible = true;
	tesla->image = scene4a[6][9];

	blood->visible = true;

	freudHead->image = scene4a[4][2];
	freudHead->visible = true;

	freudBody->visible = true;

	avatar->visible = true;
	avatar->x = 80;
	avatar->y = 150;
	avatar->image = scene4a[2][1];

	spector->visible = true;
	spector->x = 145;
	spector->y = 140;
	spector->image = scene4a[3][1];

	if (waitForExit(200))
		return;

	// Tesla's face in close up
	_cutscene->hide_sprites();

	bg->image = scene4a[7][0];
	bg->visible = true;

	face->image = scene4a[7][1];
	face->visible = true;

	textSprite->text = text[7];
	textSprite->text_color = 7;
	textSprite->visible = true;

	if (waitForExit(200))
		return;

	// Blood in close up
	_cutscene->hide_sprites();

	bg->image = scene4a[8][0];
	bg->visible = true;

	face->visible = true;
	textSprite->visible = true;
	textSprite->text_color = 4;

	for (int i = 8; i <= 9; ++i) {
		face->image = scene4a[8][i - 7];
		textSprite->text = text[i];

		if (waitForExit(200))
			return;
	}

	// Tesla calls Mr Garrett
	textSprite->text = text[10];
	textSprite->text_color = 7;
	face->visible = false;
	bg->image = scene4a[0][0];

	tesla->visible = true;
	tesla->image = scene4a[6][10];

	blood->visible = true;

	freudHead->visible = true;
	freudBody->visible = true;

	avatar->visible = true;
	avatar->image = scene4a[2][0];

	spector->visible = true;
	spector->image = scene4a[3][0];

	if (waitForExit(200))
		return;

	// Mr Garrett's entrance
	garrett->visible = true;
	spector->x = 150;
	spector->y = 140;
	textSprite->text = text[11];
	textSprite->text_color = 9;

	for (int i = 0; i <= 31; ++i) {
		if (i == 5)
			spector->image = scene4a[3][2];
		if (i <= 10)
			avatar->image = scene4a[2][i];

		garrett->image = scene4b[0][i];

		if (waitForExit(3))
			return;
	}

	// Tesla checks the note and talks about the mission
	spector->image = scene4a[3][1];
	spector->x = 155;
	avatar->image = scene4a[2][10];
	freudHead->image = scene4a[4][1];

	for (int i = 12; i <= 20; ++i) {
		if (i == 13 || i == 16 || i == 20) {
			face->visible = true;
			tesla->visible = false;
			blood->visible = false;
			freudHead->visible = false;
			freudBody->visible = false;
			avatar->visible = false;
			spector->visible = false;
			garrett->visible = false;

			int idx = (i == 13) ? 8 : 7;
			bg->image = scene4a[idx][0];
			face->image = scene4a[idx][(i == 16) ? 2 : 1];
		} else {
			if (i == 14)
				spector->image = scene4a[3][0];

			int idx;
			if (i == 12 || i == 17)
				idx = 9;
			else if (i == 14 || i == 15)
				idx = 10;
			else
				idx = 11;

			tesla->image = scene4a[6][idx];
		}

		textSprite->text = text[i];
		textSprite->text_color = TEXT_COLOURS[i];

		if (waitForExit(200))
			return;

		face->visible = false;
		tesla->visible = true;
		blood->visible = true;
		freudHead->visible = true;
		freudBody->visible = true;
		avatar->visible = true;
		spector->visible = true;
		garrett->visible = true;
		bg->image = scene4a[0][0];
	}

	// Nellie walks in
	tesla->image = scene4a[6][10];

	textSprite->text = text[21];
	textSprite->text_color = TEXT_COLOURS[21];

	nellie->visible = true;
	nellie->y = 125;
	bookcase->visible = true;

	for (int i = 0; i <= 31; ++i) {
		if (i == 7)
			freudHead->image = scene4a[4][2];

		nellie->image = scene4b[1][i % 4];
		nellie->x = 320 - i * 3;
		if (i > 25)
			nellie->y = 125 + (i - 26) * 2;

		avatar->image = scene4a[2][CLIP(20 - i, 4, 10)];
		garrett->image = scene4b[0][CLIP(45 - i, 18, 31)];

		if (waitForExit(3))
			return;
	}

	// Nellie shakes hands with Spector and the avatar
	spector->x = 165;
	nellie->x = 227;
	nellie->y = 135;

	for (int i = 22; i <= 26; ++i) {
		if (i == 24) {
			nellie->image = scene4b[1][7];
			spector->image = scene4a[3][3];
		} else {
			nellie->image = scene4b[1][(i == 23) ? 6 : 5];
			spector->image = scene4a[3][0];
		}

		if (i == 25)
			tesla->image = scene4a[6][9];
		else if (i == 26)
			tesla->image = scene4a[6][11];
		else
			tesla->image = scene4a[6][12];

		textSprite->text = text[i];
		textSprite->text_color = TEXT_COLOURS[i];

		if (waitForExit(200))
			return;
	}
}

void Intro::runIntroduction() {
	showHome();
	if (_shouldExit)
		return;

	showMap();
	if (_shouldExit)
		return;

	showHike();
	if (_shouldExit)
		return;

	showLabPresentDay();
	if (_shouldExit)
		return;

	showLab1895();
	if (_shouldExit)
		return;

	fadeOut(6);
}

void Intro::play() {
	// These are created first so they're drawn beneath everything else
	_nameSprite = newSprite(nullptr, 120, 184, false);
	_nameSprite->text_color = 14;
	_cursorSprite = newSprite(nullptr, 120, 184, false);
	_cursorSprite->text = "_";
	_cursorSprite->text_color = 14;

	showMouseCursor(false);
	_cutscene->set_update_interval(25);
	_cutscene->set_bg_color(0);
	_cutscene->set_screen_opacity(0);

	originFxSequence();

	_cutscene->load_palette("md_title.pal", 0);
	_cutscene->hide_sprites();
	playIntro();

	if (mainMenu()) {
		hideMenuCursor();
		fadeOut(6);
		_cutscene->get_config()->set("config/quit", true);
	}

	musicStop();
	_cutscene->Hide();
}

bool MDScripts::playIntro() {
	Intro intro;
	intro.play();
	return true;
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
