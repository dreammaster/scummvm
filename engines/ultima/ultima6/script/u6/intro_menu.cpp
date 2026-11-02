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
#include "ultima/ultima6/script/u6/u6_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

namespace {

const uint8 MENU_PALETTE[9][3] = {
	{ 232, 96, 0 }, { 236, 128, 0 }, { 244, 164, 0 }, { 248, 200, 0 }, { 252, 252, 84 },
	{ 248, 200, 0 }, { 244, 164, 0 }, { 236, 128, 0 }, { 232, 96, 0 }
};

const uint8 MENU_PALETTE_IDX[5] = { 14, 33, 34, 35, 36 };

const char *const SONG_NAMES[12] = {
	"ultima.m", "bootup.m", "intro.m", "create.m", "forest.m", "hornpipe.m", "engage.m", "stones.m", "dungeon.m",
	"brit.m", "gargoyle.m", "end.m"
};

int updateU4Stat(int stat) {
	if (stat > 9) {
		if (stat < 30)
			stat = (stat - 9) / 2 + 10;
		else
			stat = (stat - 30) / 4 + 20;
	}

	return stat;
}

} // End of anonymous namespace

void Intro::centreString(CSImage *image, const char *text, int y, uint8 colour) {
	int x = (imageWidth(image) - _cutscene->get_text_width(text)) / 2;
	_cutscene->print_text_raw(image, text, x, y, colour);
}

void Intro::printTransferFieldNames(CSImage *image) {
	static const char *const NAMES[9] = { "Name", "Sex", "Class", "Str", "Dex", "Int", "Magic", "Exp", "Level" };

	for (int i = 0; i < 9; ++i) {
		int x = 60 - _cutscene->get_text_width(NAMES[i]);
		_cutscene->print_text_raw(image, NAMES[i], x, 21 + i * 8, 0x3d);
	}
}

void Intro::printOldSaveField(CSImage *image, const Common::String &text, int yOffset) {
	_cutscene->print_text_raw(image, text.c_str(), 65, 21 + yOffset * 8, 2);
}

void Intro::printNewSaveField(CSImage *image, const Common::String &text, int yOffset) {
	_cutscene->print_text_raw(image, text.c_str(), 146, 21 + yOffset * 8, 9);
}

bool Intro::clickedOnButton(const CSSprite *button) const {
	int x = mouseX();
	int y = mouseY();
	uint16 w, h;
	button->image->shp->get_size(&w, &h);

	return x >= button->x && x < button->x + w && y >= button->y && y < button->y + h;
}

bool Intro::transferPage1() {
	loadImages("vellum1.shp", _images);
	CSSprite *bg = newSprite(img(0), 0x10, 0x50, true);

	centreString(bg->image, "TRANSFER A CHARACTER", 9, 0x3d);
	drawLine(bg->image, 9, 18, 274, 18, 0x3d);

	centreString(bg->image, "Enter the letter of the", 40, 0x3d);
	centreString(bg->image, "floppy/hard drive containing your", 40 + 9, 0x3d);
	centreString(bg->image, "Ultima V Britannia", 40 + 18, 0x3d);
	centreString(bg->image, "or Ultima IV Player disk....", 40 + 27, 0x3d);

	CSSprite *letters[12];
	int letterWidth = imageWidth(img(5));

	for (int i = 0; i <= 10; ++i)
		letters[i] = newSprite(img(5 + i), 30 + i * (letterWidth + 1), 174, true);

	letters[11] = newSprite(img(4), 33 + 11 * (letterWidth + 1), 174, true); // The esc-abort button

	bool exiting = false;
	int input;

	do {
		canvasUpdate();
		input = inputPoll();

		if (input != INPUT_NONE) {
			if (shouldExit(input))
				exiting = true;

			if (input == INPUT_MOUSE_CLICK && clickedOnButton(letters[11]))
				exiting = true;
		}
	} while (input == INPUT_NONE && !engineShouldQuit());

	for (int i = 0; i < 12; ++i)
		letters[i]->visible = false;

	bg->visible = false;

	return !exiting;
}

bool Intro::transferShowStats(TransferSaveData &oldSave, TransferSaveData &newSave) {
	loadImages("vellum1.shp", _images);
	CSSprite *bg = newSprite(img(0), 0x10, 0x50, true);

	Common::String transferGameName = "No Save";
	if (oldSave.gameType == 4)
		transferGameName = "Ultima IV";
	else if (oldSave.gameType == 5)
		transferGameName = "Ultima V";

	CSSprite *name = newSprite(nullptr, bg->x + 146, bg->y + 21, true);
	name->text = newSave.name;
	name->text_color = 9;

	CSSprite *genderSprite = newSprite(nullptr, bg->x + 146, bg->y + 21 + 8, true);
	genderSprite->text = (newSave.gender == 0) ? "Female" : "Male";
	genderSprite->text_color = 9;

	_cutscene->print_text_raw(bg->image, transferGameName.c_str(), 65, 9, 0x3d);
	_cutscene->print_text_raw(bg->image, "Ultima VI", 146, 9, 0x3d);
	drawLine(bg->image, 9, 18, 274, 18, 0x3d);

	printTransferFieldNames(bg->image);

	printOldSaveField(bg->image, oldSave.name, 0);
	if (oldSave.gender == 0)
		printOldSaveField(bg->image, "Female", 1);
	else if (oldSave.gender == 1)
		printOldSaveField(bg->image, "Male", 1);
	printOldSaveField(bg->image, oldSave.className, 2);
	printOldSaveField(bg->image, Common::String::format("%d", oldSave.str), 3);
	printOldSaveField(bg->image, Common::String::format("%d", oldSave.dex), 4);
	printOldSaveField(bg->image, Common::String::format("%d", oldSave.intelligence), 5);
	printOldSaveField(bg->image, Common::String::format("%d", oldSave.magic), 6);
	printOldSaveField(bg->image, Common::String::format("%d", oldSave.exp), 7);
	printOldSaveField(bg->image, Common::String::format("%d", oldSave.level), 8);

	printNewSaveField(bg->image, newSave.className, 2);
	printNewSaveField(bg->image, Common::String::format("%d", newSave.str), 3);
	printNewSaveField(bg->image, Common::String::format("%d", newSave.dex), 4);
	printNewSaveField(bg->image, Common::String::format("%d", newSave.intelligence), 5);
	printNewSaveField(bg->image, Common::String::format("%d", newSave.magic), 6);
	printNewSaveField(bg->image, Common::String::format("%d", newSave.exp), 7);
	printNewSaveField(bg->image, Common::String::format("%d", newSave.level), 8);

	int btnX = 27;
	CSSprite *newName = newSprite(img(2), btnX, 174, true);
	btnX += imageWidth(newName->image) + 1;
	CSSprite *newSex = newSprite(img(3), btnX, 174, true);
	btnX += imageWidth(newSex->image) + 1;
	CSSprite *continueButton = newSprite(img(16), btnX, 174, true);
	btnX += imageWidth(continueButton->image) + 1;
	CSSprite *escButton = newSprite(img(4), btnX, 174, true);

	bool exiting = false;

	for (;;) {
		canvasUpdate();
		int input = inputPoll();

		if (input == INPUT_NONE) {
			if (!engineShouldQuit())
				continue;
			input = Common::KEYCODE_ESCAPE;
		}

		bool click = (input == INPUT_MOUSE_CLICK);

		if (shouldExit(input) || (click && clickedOnButton(escButton))) {
			exiting = true;
			break;
		} else if (input == 'c' || (click && clickedOnButton(continueButton))) {
			break;
		} else if (input == 's' || (click && clickedOnButton(newSex))) {
			if (newSave.gender == 0) {
				newSave.gender = 1;
				genderSprite->text = "Male";
			} else {
				newSave.gender = 0;
				genderSprite->text = "Female";
			}
		} else if (input == 'n' || (click && clickedOnButton(newName))) {
			name->text = "";
			name->text_color = 0;
			if (!collectNameString(name))
				name->text = oldSave.name;
			name->text_color = 9;
		}
	}

	name->visible = false;
	genderSprite->visible = false;
	newName->visible = false;
	newSex->visible = false;
	continueButton->visible = false;
	escButton->visible = false;
	bg->visible = false;

	newSave.name = name->text;
	return !exiting;
}

bool Intro::transferPortrait(TransferSaveData &newSave, int &portrait) {
	loadImages("vellum1.shp", _images);
	CSSprite *bg = newSprite(img(0), 0x10, 0x50, true);

	centreString(bg->image, newSave.name.c_str(), 9, 0x3d);
	drawLine(bg->image, 9, 18, 274, 18, 0x3d);

	CSSprite *newSex = newSprite(img(3), 0x5e, 0x70, true);
	CSSprite *newPortrait = newSprite(img(0x12), 0x3e, 0x81, true);
	CSSprite *transferButton = newSprite(img(1), 0x56, 0x92, true);
	CSSprite *escAbort = newSprite(img(4), 0x50, 0xa3, true);

	ImageTable montage;
	loadImages("montage.shp", montage);
	CSSprite *avatar = newSprite(montage[newSave.gender * 6 + portrait][0], 0xc3, 0x76, true);

	CSSprite *sexHighlight = newHighlight(58, newSex->x, newSex->y, true);
	CSSprite *portraitHighlight = newHighlight(90, newPortrait->x, newPortrait->y, false);
	CSSprite *continueHighlight = newHighlight(66, transferButton->x, transferButton->y, false);
	CSSprite *escAbortHighlight = newHighlight(72, escAbort->x, escAbort->y, false);

	bool exiting = false;

	for (;;) {
		canvasUpdate();
		int input = inputPoll();

		if (input == INPUT_NONE) {
			if (!engineShouldQuit())
				continue;
			input = Common::KEYCODE_ESCAPE;
		}

		bool click = (input == INPUT_MOUSE_CLICK);

		if (shouldExit(input) || (click && clickedOnButton(escAbort))) {
			exiting = true;
			break;
		} else if (input == 't' || (click && clickedOnButton(transferButton))) {
			break;
		} else if (input == 's' || (click && clickedOnButton(newSex))) {
			newSave.gender = (newSave.gender == 0) ? 1 : 0;
			avatar->image = montage[newSave.gender * 6 + portrait][0];
		} else if (input == 'p' || (click && clickedOnButton(newPortrait))) {
			portrait = (portrait == 5) ? 0 : portrait + 1;
			avatar->image = montage[newSave.gender * 6 + portrait][0];
		}
	}

	if (!exiting) {
		_cutscene->set_bg_color(0);
		fadeOut();
	}

	bg->visible = false;
	avatar->visible = false;

	newSex->visible = false;
	newPortrait->visible = false;
	transferButton->visible = false;
	escAbort->visible = false;

	sexHighlight->visible = false;
	portraitHighlight->visible = false;
	continueHighlight->visible = false;
	escAbortHighlight->visible = false;

	return !exiting;
}

void Intro::transferNoSaveFound() {
	loadImages("vellum1.shp", _images);
	CSSprite *bg = newSprite(img(0), 0x10, 0x50, true);

	centreString(bg->image, "TRANSFER A CHARACTER", 9, 0x3d);
	drawLine(bg->image, 9, 18, 274, 18, 0x3d);

	centreString(bg->image, "No character found! If you wish to", 40, 0x3d);
	centreString(bg->image, "transfer a character from a hard disk", 40 + 9, 0x3d);
	centreString(bg->image, "please copy either SAVED.GAM or PARTY.SAV", 40 + 18, 0x3d);
	centreString(bg->image, "into your Ultima VI game directory.", 40 + 27, 0x3d);

	waitInput();

	bg->visible = false;
}

bool Intro::transfer() {
	if (!transferPage1())
		return false;

	TransferSaveData oldSave = _cutscene->load_transfer_save();
	TransferSaveData newSave = oldSave;
	int portrait = 0;

	if (oldSave.gameType == 0) {
		transferNoSaveFound();
		return false;
	}

	if (oldSave.gameType == 4) {
		newSave.str = updateU4Stat(newSave.str);
		newSave.dex = updateU4Stat(newSave.dex);
		newSave.intelligence = updateU4Stat(newSave.intelligence);
	}

	newSave.className = "Avatar";
	newSave.magic = newSave.intelligence;
	newSave.exp = newSave.exp / 10;
	newSave.level = 1;

	int tmpXp = oldSave.exp / 1000;
	while (tmpXp > 0) {
		++newSave.level;
		tmpXp /= 2;
	}

	if (!transferShowStats(oldSave, newSave))
		return false;

	if (!transferPortrait(newSave, portrait))
		return false;

	Configuration *config = _cutscene->get_config();
	config->set("config/newgame", true);

	config->set("config/newgamedata/name", newSave.name);
	config->set("config/newgamedata/gender", newSave.gender);
	config->set("config/newgamedata/portrait", portrait);
	config->set("config/newgamedata/str", newSave.str);
	config->set("config/newgamedata/dex", newSave.dex);
	config->set("config/newgamedata/int", newSave.intelligence);
	config->set("config/newgamedata/magic", newSave.magic);
	config->set("config/newgamedata/exp", newSave.exp);
	config->set("config/newgamedata/level", newSave.level);

	return true;
}

void Intro::ackHeader(CSImage *image) {
	printText(image, "Ultima VI", 7, 303, 115, 9, 0x3d);
	printText(image, "A Lord British Production", 7, 303, 63, 17, 0x3d);
	drawLine(image, 9, 26, 274, 26, 0x3d);
}

void Intro::acknowledgements() {
	CSSprite *bg = newSprite(loadImage("vellum1.shp", 0), 0x10, 0x50, true);

	ackHeader(bg->image);

	printText(bg->image, "Produced by", 7, 303, 106, 32, 0xC);
	printText(bg->image, "Richard Garriott and Warren Spector", 7, 303, 28, 40, 0x48);

	printText(bg->image, "Executive Producer", 7, 303, 82, 56, 0xC);
	printText(bg->image, "Dallas Snell", 7, 303, 106, 64, 0x48);

	printText(bg->image, "Programming", 7, 303, 104, 80, 0xC);
	printText(bg->image, "Cheryl Chen    John Miles", 7, 303, 67, 88, 0x48);
	printText(bg->image, "Herman Miller    Gary Scott Smith", 7, 303, 40, 96, 0x48);

	fadeInSprite(bg);

	waitInput();

	bg->image = loadImage("vellum1.shp", 0);

	ackHeader(bg->image);

	printText(bg->image, "Writing", 7, 303, 120, 47, 0xC);
	printText(bg->image, "Stephen Beeman    Dr. Cat    'Manda Dee", 7, 303, 25, 55, 0x48);
	printText(bg->image, "Richard Garriott    Greg Malone", 7, 303, 46, 63, 0x48);
	printText(bg->image, "John Miles    Herman Miller", 7, 303, 61, 71, 0x48);
	printText(bg->image, "Todd Porter    Warren Spector", 7, 303, 50, 79, 0x48);

	waitInput();

	bg->image = loadImage("vellum1.shp", 0);

	ackHeader(bg->image);

	printText(bg->image, "Art", 7, 303, 132, 31, 0xC);
	printText(bg->image, "Keith Berdak    Daniel Bourbonnais", 7, 303, 37, 39, 0x48);
	printText(bg->image, "Jeff Dee    'Manda Dee", 7, 303, 75, 47, 0x48);
	printText(bg->image, "Glen Johnson    Denis Loubet", 7, 303, 56, 55, 0x48);

	printText(bg->image, "Music", 7, 303, 126, 71, 0xC);
	printText(bg->image, "Ken Arnold    Iolo Fitzowen", 7, 303, 61, 79, 0x48);
	printText(bg->image, "Herman Miller    Todd Porter", 7, 303, 56, 87, 0x48);

	waitInput();

	bg->image = loadImage("vellum1.shp", 0);

	ackHeader(bg->image);

	printText(bg->image, "Quality Assurance", 7, 303, 87, 31, 0xC);
	printText(bg->image, "Paul Malone    Mike Romero", 7, 303, 62, 39, 0x48);

	printText(bg->image, "Additional Support", 7, 303, 84, 63, 0xC);
	printText(bg->image, "Michelle Caddel    Melanie Fleming", 7, 303, 39, 71, 0x48);
	printText(bg->image, "Alan Gardner    Jeff Hillhouse", 7, 303, 51, 79, 0x48);
	printText(bg->image, "Sherry Hunter    Steve Muchow", 7, 303, 49, 87, 0x48);
	printText(bg->image, "Cheryl Neeld", 7, 303, 104, 95, 0x48);

	waitInput();

	fadeOutSprite(bg, 4);

	bg->visible = false;
}

void Intro::mainMenuSetPal(int idx) {
	for (int i = 0; i < 5; ++i) {
		const uint8 *colour = MENU_PALETTE[4 + i - idx];
		_cutscene->set_palette_entry(MENU_PALETTE_IDX[i], colour[0], colour[1], colour[2]);
	}
}

void Intro::mainMenuLoad() {
	if (engineShouldQuit())
		return;

	musicPlay("ultima.m", 0);

	_cutscene->load_palette("palettes.int", 0);

	ImageTable titles;
	loadImages("titles.shp", titles);
	_menuTitle = newSprite(titles[0][0], 0x13, 0, true);
	_menuSubtitle = newSprite(titles[1][0], 0x3b, 0x2f, false);

	_menuMenu = newSprite(loadImage("mainmenu.shp", 0), 0x31, 0x53, false);

	fadeIn();

	_menuSubtitle->visible = true;
	_menuMenu->visible = true;

	fadeInSprite(_menuMenu);
	showMouseCursor(true);
}

void Intro::selectedIntro() {
	showMouseCursor(false);
	mainMenuSetPal(0);
	fadeOut();
	_cutscene->hide_sprites();
	story();
	musicPlay("ultima.m", 0);
	_menuTitle->visible = true;
	fadeIn();
	_menuSubtitle->visible = true;
	_menuMenu->visible = true;

	fadeInSprite(_menuMenu);
	showMouseCursor(true);
}

bool Intro::selectedCreateCharacter() {
	mainMenuSetPal(1);
	fadeOutSprite(_menuMenu, 6);

	if (createCharacter())
		return true;

	_cutscene->load_palette("palettes.int", 0);
	_menuIdx = 0;
	mainMenuSetPal(_menuIdx);
	musicPlay("ultima.m", 0);
	fadeInSprite(_menuMenu);
	return false;
}

bool Intro::selectedTransfer() {
	mainMenuSetPal(2);
	fadeOutSprite(_menuMenu, 6);

	if (transfer())
		return true;

	_cutscene->load_palette("palettes.int", 0);
	_menuIdx = 0;
	mainMenuSetPal(_menuIdx);
	fadeInSprite(_menuMenu);
	return false;
}

void Intro::selectedAcknowledgments() {
	mainMenuSetPal(3);
	fadeOutSprite(_menuMenu, 6);
	acknowledgements();
	_cutscene->load_palette("palettes.int", 0);
	_menuIdx = 0;
	mainMenuSetPal(_menuIdx);
	fadeInSprite(_menuMenu);
}

bool Intro::mainMenu() {
	_menuTitle->visible = true;
	_menuSubtitle->visible = true;
	_menuMenu->visible = true;

	for (;;) {
		canvasUpdate();

		if (engineShouldQuit())
			return true;

		for (;;) {
			int input = inputPoll(true);
			if (input == INPUT_NONE)
				break;

			if (input == 'q') {
				return true;
			} else if (input == 'i' || (input == Common::KEYCODE_RETURN && _menuIdx == 0)) {
				selectedIntro();
			} else if (input == 'c' || (input == Common::KEYCODE_RETURN && _menuIdx == 1)) {
				if (selectedCreateCharacter())
					return false;
			} else if (input == 't' || (input == Common::KEYCODE_RETURN && _menuIdx == 2)) {
				if (selectedTransfer()) {
					story();
					return false;
				}
			} else if (input == 'a' || (input == Common::KEYCODE_RETURN && _menuIdx == 3)) {
				selectedAcknowledgments();
			} else if (input == 'j' || (input == Common::KEYCODE_RETURN && _menuIdx == 4)) {
				mainMenuSetPal(4);
				fadeOut();
				return false;
			} else if (input == INPUT_KEY_DOWN) {
				if (_menuIdx < 4) {
					++_menuIdx;
					mainMenuSetPal(_menuIdx);
				}
			} else if (input == INPUT_KEY_UP) {
				if (_menuIdx > 0) {
					--_menuIdx;
					mainMenuSetPal(_menuIdx);
				}
			} else if ((input >= '0' && input <= '9') || input == '-' || input == '=') {
				// Play music by pressing the number keys, '-' or '='
				if (input == '-')
					input = 11;
				else if (input == '0')
					input = 10;
				else if (input == '=')
					input = 12;
				else
					input -= '0';

				musicPlay(SONG_NAMES[input - 1], 0);
			} else if (input == INPUT_MOUSE_CLICK) {
				int x = mouseX();

				if (x > 56 && x < 264) {
					int y = mouseY();

					if (y > 86 && y < 108) {
						selectedIntro();
					} else if (y > 107 && y < 128) {
						if (selectedCreateCharacter())
							return false;
					} else if (y > 127 && y < 149) {
						if (selectedTransfer()) {
							story();
							return false;
						}
					} else if (y > 148 && y < 170) {
						selectedAcknowledgments();
					} else if (y > 169 && y < 196) {
						mainMenuSetPal(4);
						fadeOut();
						return false; // Journey onward
					}
				}
			} else if (input == INPUT_MOUSE_MOTION) {
				int x = mouseX();

				if (x > 56 && x < 264) {
					int oldMenuIdx = _menuIdx;
					int y = mouseY();

					if (y > 86 && y < 108)
						_menuIdx = 0;
					else if (y > 107 && y < 128)
						_menuIdx = 1;
					else if (y > 127 && y < 149)
						_menuIdx = 2;
					else if (y > 148 && y < 170)
						_menuIdx = 3;
					else if (y > 169 && y < 196)
						_menuIdx = 4;

					if (_menuIdx != oldMenuIdx)
						mainMenuSetPal(_menuIdx);
				}
			}
		}
	}
}

void Intro::play() {
	playOpening();
	fadeOut();

	_cutscene->hide_sprites();

	mainMenuLoad();

	if (mainMenu())
		_cutscene->get_config()->set("config/quit", true);

	musicStop();
	_cutscene->hide_sprites();
	_cutscene->Hide();
}

bool U6Scripts::playIntro() {
	Intro intro;
	intro.play();
	return true;
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
