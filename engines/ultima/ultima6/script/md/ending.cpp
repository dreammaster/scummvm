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

#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/common/cutscene_script.h"
#include "ultima/ultima6/script/md/md_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

class Ending : public CutsceneScript {
private:
	/** Waits for input or the time to pass, returning true if the player asked to leave */
	bool wait(int cycles) {
		pollForKeyOrButton(cycles);
		return _shouldExit;
	}

public:
	void play();
};

void Ending::play() {
	int gender = Game::get_game()->get_player()->get_gender();

	ImageTable images;
	if (!loadImages("endgame.lzc", images))
		return;

	Common::Array<Common::String> text = _cutscene->load_text("scenetxt.lzc", 3);
	if (text.size() < 10)
		return;

	CSSprite *textSprite = newSprite(nullptr, 0, 160, true);
	textSprite->text_color = 4;
	textSprite->text = text[0];
	textSprite->text_align = 2;

	CSSprite *bg = newSprite(images[0][0], 0, 24, true);
	CSSprite *jack = newSprite(images[0][gender + 1], 155, 151, true);

	_cutscene->load_palette("md_title.pal", 0);
	_cutscene->set_screen_opacity(0xff);
	showMouseCursor(false);
	_cutscene->set_update_interval(25);

	musicPlay("mdd_mus.lzc", 11);
	fadeIn();

	if (wait(150))
		return;

	textSprite->text = text[1];

	if (wait(150))
		return;

	// The space capsule rockets away from Mars
	jack->visible = false;
	textSprite->text = text[2];
	bg->image = images[1][0];

	CSSprite *capsule = createSprite(images[1][1], 105, 112);
	capsule->visible = false;
	CSSprite *smoke = newSprite(images[1][2], 105, 112, false);

	for (int i = -1; i < 40; ++i) {
		bg->y = 23 + ((i + 3) % 4);
		bg->x = ((i + 2) % 5) - 2;

		if (i > -1 && i < 13) {
			capsule->visible = true;
			capsule->x = 105 + (i * i * 2) / 3;
			capsule->y = 112 - i * i;
			capsule->image->setScale(i * i + 16);

			smoke->image = images[1][2 + i / 2];
			smoke->visible = true;
		} else {
			capsule->visible = false;
			smoke->visible = false;
		}

		if (wait(3))
			return;
	}

	// The capsule lands in the ocean
	bg->image = images[2][0];
	bg->x = 0;
	bg->y = 24;

	capsule->visible = true;
	capsule->x = 170;
	capsule->y = 150;

	textSprite->text = text[3];

	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 4; ++j) {
			capsule->image = images[2][ABS(2 - j) + 1];

			if (i == 5)
				textSprite->text = text[4];

			if (wait(8))
				return;
		}
	}

	// The ticker tape parade
	_cutscene->set_screen_opacity(0);
	capsule->visible = false;

	bg->image = images[3][0];
	CSSprite *spector = newSprite(images[3][gender + 1], 190, 151, true);
	textSprite->text = text[5];

	fadeIn(6);

	if (wait(150))
		return;

	// The group photo
	_cutscene->set_screen_opacity(0);
	spector->visible = false;

	bg->image = images[4][0];

	ImageTable moongateImages;
	if (!loadImages("moongate.lzc", moongateImages))
		return;

	CSSprite *moongate = newSprite(moongateImages[1][0], 35, 135, true);
	CSSprite *group = newSprite(images[4][gender + 1], 195, 151, true);
	CSSprite *photographer = newSprite(images[4][3], 75, 151, true);

	textSprite->text = text[6];
	fadeIn(6);

	for (int i = 0; i < 80; ++i) {
		if (i == 40) {
			moongate->visible = false;
			group->visible = false;
			photographer->visible = false;
			bg->image = images[4][5];
			pollForKeyOrButton(2);
			fadeOut(6);
			bg->image = images[4][0];
			moongate->visible = true;
			group->visible = true;
			photographer->visible = true;
			_cutscene->set_screen_opacity(0xff);
			continue;
		}

		moongate->image = moongateImages[1][i % 8];
		photographer->image = (i >= 38 && i <= 42) ? images[4][4] : images[4][3];

		if (i == 41)
			textSprite->text = text[7];

		if (wait(3))
			return;
	}

	// The Martian dust storm
	moongate->visible = false;
	group->visible = false;
	photographer->visible = false;

	bg->image = images[5][0];

	CSSprite *sand = newSprite(nullptr, 240, 140, true);
	CSSprite *tree = newSprite(nullptr, 0, 140, true);

	CSSprite *dust[5];
	for (int i = 0; i < 5; ++i) {
		dust[i] = createSprite(images[6][0], 0, 24);
		dust[i]->visible = true;
	}

	textSprite->text = text[8];

	// Where each layer of dust starts and how fast it moves
	static const int DUST_START[5] = { 340, 420, 500, 580, 660 };
	static const int DUST_SPEED[5] = { 10, 20, 30, 30, 20 };

	for (int i = 0; i < 64; ++i) {
		sand->image = images[7][MIN(i / 8, 4)];

		if (i < 56)
			tree->image = images[8][MAX(i - 50, 0)];
		else
			tree->visible = false;

		for (int j = 0; j < 5; ++j) {
			dust[j]->x = DUST_START[j] - (i * DUST_SPEED[j]) - randomRange(0, 19);
			dust[j]->y = 24 - randomRange(0, 9);
		}

		if (i == 62)
			textSprite->text = text[9];

		if (wait(2))
			return;
	}

	if (wait(150))
		return;

	aboutMartianDreams();
}

} // End of anonymous namespace

bool MDScripts::playEnding() {
	Ending ending;
	ending.play();
	return true;
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
