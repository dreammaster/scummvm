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
#include "ultima/ultima6/core/game_clock.h"
#include "ultima/ultima6/script/common/cutscene_script.h"
#include "ultima/ultima6/script/u6/u6_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

namespace {

const int START_YEAR = 161;
const int START_MONTH = 7;
const int START_DAY = 4;

const char *const NUM_STRINGS[30] = {
	"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve",
	"thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty", "twenty-one",
	"twenty-two", "twenty-three", "twenty-four", "twenty-five", "twenty-six", "twenty-seven", "twenty-eight",
	"twenty-nine"
};

class Ending : public CutsceneScript {
private:
	CSSprite *_scroll = nullptr;
	CSSprite *_starField = nullptr;
	int _palCounter = 0;

	void stonesRotatePalette() {
		if (_palCounter == 4) {
			_cutscene->rotate_palette(16, 16);
			_palCounter = 0;
		} else {
			++_palCounter;
		}
	}

	void updateStarField() {
		_starField->image->updateEffect();
	}

	/** Waits for any input, returning false if the engine is quitting */
	bool waitForKey() {
		int input;

		do {
			if (engineShouldQuit()) {
				_shouldExit = true;
				return false;
			}

			canvasUpdate();
			input = inputPoll();
		} while (input == INPUT_NONE);

		return true;
	}

	/** Waits for input while the stones' palette rotates and the stars move */
	bool rotateAndWait() {
		int input;

		do {
			if (engineShouldQuit()) {
				_shouldExit = true;
				return false;
			}

			updateStarField();
			stonesRotatePalette();
			canvasUpdate();
			input = inputPoll();
		} while (input == INPUT_NONE);

		return true;
	}

	/** Waits for input while the stars move */
	bool updateStarFieldAndWait() {
		int input;

		do {
			if (engineShouldQuit()) {
				_shouldExit = true;
				return false;
			}

			updateStarField();
			canvasUpdate();
			input = inputPoll();
		} while (input == INPUT_NONE);

		return true;
	}

	/** Puts the text in the scroll at the bottom of the screen */
	void say(int imageIdx, const char *text, uint16 startX, uint16 width, uint16 x, uint16 y, int scrollX, int scrollY) {
		CSImage *image = loadImage("end.shp", imageIdx);
		printText(image, text, startX, width, x, y, 0x3e);
		_scroll->image = image;
		_scroll->x = scrollX;
		_scroll->y = scrollY;
	}

public:
	void play();
};

void Ending::play() {
	_cutscene->set_screen_opacity(0xff);
	showMouseCursor(false);

	ImageTable images;
	if (!loadImages("end.shp", images))
		return;

	musicPlay("brit.m", 0);

	_cutscene->load_palette("endpal.lbm", 0);
	_cutscene->set_update_interval(25);
	newSprite(images[7][0], 0, 0, true); // background

	setTransparency(images[0][0], 0);
	_starField = newSprite(newStarField(114, 94), 106, 0, false);
	setClip(_starField, 106, 0, 107, 84);
	updateStarField();

	CSSprite *codex = newSprite(images[1][0], 0x97, 0x1d, false);
	CSSprite *codexOpened = newSprite(images[2][0], 0x85, 0x17, false);
	CSSprite *wallTransparent = newSprite(images[0][0], 106, 0, false);
	CSSprite *wall = newSprite(images[4][0], 106, 0, true);
	CSSprite *moongate = newSprite(images[3][0], 9, 0x7d, true);
	setClip(moongate, 0, 0, 320, 0x7e);

	CSSprite *characters = newSprite(images[8][0], 0, 0, false);
	setClip(characters, 0, 0, 160, 200);

	CSSprite *characters1 = newSprite(images[9][0], 0, 0, false);
	setClip(characters1, 0, 0, 160, 200);

	CSSprite *blueLens = newSprite(images[5][0], 0x49, 0x57, true);
	CSSprite *redLens = newSprite(images[6][0], 0xda, 0x57, true);

	CSImage *scrollImage = loadImage("end.shp", 0xb);
	printText(scrollImage, "A glowing portal springs from the floor!", 7, 303, 34, 13, 0x3e);
	_scroll = newSprite(scrollImage, 1, 0xa0, true);

	int input;
	int i;

	for (i = 0x7d; i >= -0xc; --i) {
		stonesRotatePalette();
		moongate->y = i;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			moongate->y = -0xc;
			break;
		}
	}

	if (!rotateAndWait())
		return;

	characters->visible = true;

	say(0xa, "From its crimson depths Lord British emerges, trailed by the mage Nystul. Anguish and disbelief prevail on the royal seer's face, but Lord British directs his stony gaze at you and speaks as if to a wayward child.",
		8, 303, 7, 13, 0x1, 0x85);

	if (!rotateAndWait())
		return;

	say(0xc, "\"Thou didst have just cause to burgle our Codex, I trust\177 His Majesty says. \"But for Virtue's sake...",
		8, 303, 7, 12, 0x1, 0x97);

	for (i = -0xc; i <= 0x7d; ++i) {
		stonesRotatePalette();
		moongate->y = i;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			moongate->y = 0x7d;
			break;
		}
	}

	moongate->visible = false;
	if (!waitForKey())
		return;

	say(0xb, "\"WHAT HAST THOU DONE WITH IT?\177", 8, 303, 63, 13, 0x0, 0xa0);

	if (!waitForKey())
		return;

	say(0xa, "You pick up the concave lens and pass it to the King. \"Was the book ever truly ours, Your Majesty? Was it written for Britannia alone? Thou dost no longer hold the Codex, but is its wisdom indeed lost? Look into the Vortex, and let the Codex answer for itself!\177",
		8, 303, 7, 8, 0x1, 0x85);

	if (!waitForKey())
		return;

	blueLens->visible = false;

	setClip(characters, 160, 0, 160, 200);
	characters->visible = false;

	characters1->visible = true;

	if (!waitForKey())
		return;

	say(0xc, "As Lord British holds the glass before the wall, the Codex of Ultimate Wisdom wavers into view against a myriad of swimming stars!",
		8, 303, 7, 12, 0x1, 0x97);

	_starField->visible = true;
	wallTransparent->visible = true;

	for (i = 0xff; i >= 0; i -= 3) {
		setOpacity(wall, i);
		updateStarField();
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			setOpacity(wall, 0);
			break;
		}
	}

	setOpacity(codex, 0);
	codex->visible = true;

	for (i = 0; i <= 0xff; i += 3) {
		setOpacity(codex, i);
		updateStarField();
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			setOpacity(codex, 0xff);
			break;
		}
	}

	if (!updateStarFieldAndWait())
		return;

	say(0xb, "Yet the book remains closed.", 8, 303, 70, 13, 0x0, 0xa0);

	if (!updateStarFieldAndWait())
		return;

	musicPlay("gargoyle.m", 0);
	say(0xb, "And waves of heat shimmer in the air, heralding the birth of another red gate!",
		8, 303, 7, 9, 0x0, 0x98);

	moongate->x = 0xe6;
	moongate->y = 0x7d;
	moongate->visible = true;

	for (i = 0x7d; i >= -0xc; --i) {
		updateStarField();
		stonesRotatePalette();
		moongate->y = i;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			moongate->y = -0xc;
			break;
		}
	}

	if (!rotateAndWait())
		return;

	characters->visible = true;

	say(0xa, "King Draxinusom of the Gargoyles strides forward, flanked by a small army of wingless attendants. Like Lord British, he seems to suppress his rage only through a heroic effort of will. His scaly hand grasps your shoulder, and your Amulet of Submission grows very warm.",
		8, 303, 7, 8, 0x1, 0x85);

	if (!rotateAndWait())
		return;

	say(0xb, "\"Thy time hath come, Thief,\177 he says.", 8, 303, 46, 13, 0x0, 0xa0);

	for (i = -0xc; i <= 0x7d; ++i) {
		updateStarField();
		stonesRotatePalette();
		moongate->y = i;
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			moongate->y = 0x7d;
			break;
		}
	}

	moongate->visible = false;

	if (!updateStarFieldAndWait())
		return;

	say(0xb, "Quickly you reach down to seize the convex lens...", 8, 310, 5, 13, 0x0, 0x98);

	if (!updateStarFieldAndWait())
		return;

	say(0xc, "...and you press it into the hand of the towering Gargoyle king, meeting his sunken eyes. \"Join my Lord in his search for peace, I beg thee.\177",
		8, 303, 7, 12, 0x1, 0x97);

	if (!updateStarFieldAndWait())
		return;

	characters->visible = false;
	setClip(characters1, 0, 0, 320, 200);
	redLens->visible = false;

	say(0xa, "At your urging, King Draxinusom reluctantly raises his lens to catch the light. As Lord British holds up his own lens, every eye in the room, human and Gargoyle alike, fixes upon the image of the Codex which shines upon the wall.",
		8, 303, 7, 13, 0x1, 0x85);

	if (!updateStarFieldAndWait())
		return;

	musicPlay("end.m", 0);
	say(0xa, "The ancient book opens. Both kings gaze upon its pages in spellbound silence, as the eloquence of Ultimate Wisdom is revealed in the tongues of each lord's domain. You, too, can read the answers the Codex gives...",
		8, 303, 7, 13, 0x1, 0x85);

	setOpacity(codexOpened, 0);
	codexOpened->visible = true;

	for (i = 0; i <= 0xff; i += 3) {
		setOpacity(codexOpened, i);
		updateStarField();
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			setOpacity(codexOpened, 0xff);
			break;
		}
	}

	codex->visible = false;

	if (!updateStarFieldAndWait())
		return;

	say(0xa, "...and when its wisdom is gleaned, when Lord British and King Draxinusom turn to each other as friends, hating no longer, fearing no more, you know that your mission in Britannia has ended at last.",
		8, 303, 7, 13, 0x1, 0x85);

	if (!updateStarFieldAndWait())
		return;

	_scroll->visible = false;

	for (i = 0xff; i >= 0; i -= 3) {
		setOpacity(codexOpened, i);
		updateStarField();
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			setOpacity(codexOpened, 0);
			break;
		}
	}

	codexOpened->visible = false;

	setOpacity(wall, 0);
	wall->visible = true;

	for (i = 0; i <= 0xff; i += 3) {
		setOpacity(wall, i);
		updateStarField();
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			setOpacity(wall, 0xff);
			break;
		}
	}

	_starField->visible = false;
	if (!waitForKey())
		return;

	for (i = 0xff; i >= 0; i -= 3) {
		_cutscene->set_screen_opacity(i);
		canvasUpdate();
	}

	_cutscene->hide_sprites();
	_cutscene->set_screen_opacity(0xff);

	// How long the game took, from the day it started
	GameClock *clock = Game::get_game()->get_clock();
	int currentDays = clock->get_year() * 365 + clock->get_month() * 30 + clock->get_day();
	int startDays = START_YEAR * 365 + START_MONTH * 30 + START_DAY;
	int time = MAX(currentDays - startDays, 0);

	int years = time / 365;
	int months = (time - years * 365) / 30;
	int days = time - (years * 365 + months * 30);

	Common::String line1;

	if (years > 0) {
		if (years > 29)
			line1 += Common::String::format("%d year", years);
		else
			line1 += Common::String(NUM_STRINGS[years]) + " year";

		if (years > 1)
			line1 += "s";
	}

	if (months > 0) {
		if (!line1.empty())
			line1 += ", ";

		line1 += Common::String(NUM_STRINGS[MIN(months, 29)]) + " month";
		if (months > 1)
			line1 += "s";
	}

	Common::String line2;
	if (days > 0 || line1.empty()) {
		if (!line1.empty()) {
			line1 += ",";
			line2 = "and ";
		}

		line2 += Common::String(NUM_STRINGS[MIN(days, 29)]) + " day";
		if (days != 1)
			line2 += "s";
	}

	if (line1.empty())
		line1 = "Prophet, in";
	else
		line1 = "Prophet, in " + line1;

	if ((line1 + " " + line2).size() <= 38 && !line2.empty()) {
		line1 += " " + line2;
		line2.clear();
	}

	if (line2.empty())
		line1 += ".";
	else
		line2 += ".";

	int y = 9;
	scrollImage = loadImage("end.shp", 0xa);
	printText(scrollImage, "CONGRATULATIONS", 8, 303, 107, y, 0x3e);
	y += 8;
	printText(scrollImage, "Thou hast completed Ultima VI: The False", 8, 303, 34, y, 0x3e);
	y += 8;
	printText(scrollImage, line1.c_str(), 8, 303, (318 - _cutscene->get_text_width(line1.c_str())) / 2, y, 0x3e);
	y += 8;

	if (!line2.empty()) {
		printText(scrollImage, line2.c_str(), 8, 303, (318 - _cutscene->get_text_width(line2.c_str())) / 2, y, 0x3e);
		y += 8;
	}

	printText(scrollImage, "Report thy feat unto Lord British at Origin", 8, 303, 23, y, 0x3e);
	y += 8;
	printText(scrollImage, "Systems!", 8, 303, 132, y, 0x3e);
	_scroll->image = scrollImage;
	_scroll->x = 0x1;
	_scroll->y = 0x44;

	setOpacity(_scroll, 0);
	_scroll->visible = true;

	for (i = 0; i <= 0xff; i += 3) {
		setOpacity(_scroll, i);
		canvasUpdate();
		input = inputPoll();
		if (input != INPUT_NONE) {
			setOpacity(_scroll, 0xff);
			break;
		}
	}

	waitForKey();
}

} // End of anonymous namespace

bool U6Scripts::playEnding() {
	Ending ending;
	ending.play();
	return true;
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
