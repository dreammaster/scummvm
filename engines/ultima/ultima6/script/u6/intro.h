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

#ifndef NUVIE_SCRIPT_U6_INTRO_H
#define NUVIE_SCRIPT_U6_INTRO_H

#include "common/str.h"
#include "ultima/ultima6/script/common/cutscene_script.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

/** The Ultima VI intro: the opening story, character creation with the gypsy, the transfer from earlier games and the main menu */
class Intro : public CutsceneScript {
private:
	ImageTable _images;
	int _palCounter = 0;

	// Opening story
	CSSprite *_clock[4] = { nullptr, nullptr, nullptr, nullptr };

	struct Lounge {
		CSSprite *_bg0 = nullptr;
		CSSprite *_bg = nullptr;
		CSSprite *_bg1 = nullptr;
		CSSprite *_avatar = nullptr;
		CSSprite *_tv[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
		CSSprite *_finger = nullptr;
		CSImage *_tvStatic = nullptr;
	} _lounge;

	int _tvBaseX = 0xe5;
	int _tvBaseY = 0x32;
	int _tvLoopPos = 0;
	int _tvLoopCnt = 0;
	uint _tvCurPos = 0;
	uint _tvCurProgram = 2;
	int _tvNewsImage = 0;
	int _tvPledgeCounter = 0;
	int _tvPledgeImage = 37;
	int _tvRoadOffset = 0xe;
	bool _tvUpdate = true;
	uint8 _tvYOff[54];

	struct Window {
		CSSprite *_sky = nullptr;
		CSSprite *_cloud1 = nullptr;
		CSSprite *_cloud2 = nullptr;
		CSSprite *_clouds[5];
		CSSprite *_lightning = nullptr;
		CSSprite *_ground = nullptr;
		CSSprite *_trees = nullptr;
		CSSprite *_strike = nullptr;
		CSSprite *_rain[101];
		CSSprite *_frame = nullptr;
		CSSprite *_window = nullptr;
		CSSprite *_doorLeft = nullptr;
		CSSprite *_doorRight = nullptr;
		int _cloudX = -400;
		int _flash = 0;
		int _drops = 0;
		int _rainDelay = 20;
		int _lightningCounter = 0;
		int _lightningX = 0;
		int _lightningY = 0;
	} _window;

	struct Stones {
		CSSprite *_bg = nullptr;
		CSSprite *_stoneCover = nullptr;
		CSSprite *_gateCover = nullptr;
		CSSprite *_hand = nullptr;
		CSSprite *_moonGate = nullptr;
		CSSprite *_avatar = nullptr;
	} _stones;

	CSImage *img(int idx) const {
		return _images[idx][0];
	}
	bool shouldExit(int input) const;
	void fadeOutSprite(CSSprite *sprite, int speed = 3);
	void fadeInSprite(CSSprite *sprite, int speed = 3);

	void loadClock();
	void displayClock();
	void loadLounge();
	void displayTvSprite(int spriteIdx, int imageNum);
	void displayTv();
	void displayLounge();
	void scrollLounge();
	void hideLounge();
	bool loungeSequence();

	void loadWindow();
	void hideWindow();
	void displayWindow();
	bool windowUpdate();
	bool windowSequence();

	void stonesRotatePalette();
	bool stonesUpdate();
	bool stonesShakeMoongate();
	bool stonesSequence();

	void playOpening();

	// The story of the Avatar's capture and rescue
	struct Story {
		CSSprite *_bg = nullptr;
		CSSprite *_moongate = nullptr;
		CSSprite *_gargsLeft = nullptr;
		CSSprite *_gargsRight = nullptr;
		CSSprite *_gargBody = nullptr;
		CSSprite *_gargHead = nullptr;
		CSSprite *_iolo = nullptr;
		CSSprite *_shamino = nullptr;
		CSSprite *_dupre = nullptr;
		CSSprite *_avatar = nullptr;
		CSSprite *_alter = nullptr;
		CSSprite *_ropes = nullptr;
	} _story;

	CSSprite *_scroll = nullptr;
	int _leftIdx = 1;
	int _rightIdx = 6;

	int waitInput();
	void introExit();
	bool introWait();
	void swayGargs(CSSprite *sprite, int &idx);
	void moongateRotatePalette();
	void setScroll(int block, const char *text, uint16 startX, uint16 width, uint16 x, uint16 y, int scrollX = -1,
		int scrollY = -1, bool show = false);
	bool swayWait(bool rotate, bool doubleUpdate);
	bool story();

	// Character creation with the gypsy
	struct Gypsy {
		CSSprite *_arm = nullptr;
		CSSprite *_pour = nullptr;
		CSSprite *_jarLiquid = nullptr;
		CSSprite *_jar = nullptr;
		CSSprite *_hand = nullptr;
		CSSprite *_vialLiquid[8];
		CSSprite *_vial[8];
		int _jarLevel = 0;
		int _vialLevel[8];
	} _gypsy;

	ImageTable _gypsyImages;
	uint8 _bubbleColors[8];
	int _bubbleCount = 0;
	int _bubbleCounter = 0;
	int _highlightIndex = 0;
	CSSprite *_abHighlight = nullptr;
	int _questionTable[8];
	int _str = 0xf;
	int _dex = 0xf;
	int _int = 0xf;

	CSImage *gypsyImg(int idx) const {
		return _gypsyImages[idx][0];
	}
	void bubbleEffect(CSImage *image);
	void gypsyUpdateBubbles(CSImage *bubbleImage);
	int gypsyAbSelect(int question);
	void shuffleQuestionTable(int shuffleLen);
	void gypsyStartPouring(int vialNum, int vialLevel);
	void gypsyStopPouring();
	void gypsyVialAnimLiquid(int imgNum, int vialNum, int vialLevel, int handX, int handY);
	void gypsyVialAnim(int vial);
	void gypsyAskQuestions(int numQuestions);
	bool collectNameString(CSSprite *name);
	CSSprite *newHighlight(int w, int x, int y, bool visible);
	bool createCharacter();

	// Transferring a character from an earlier game, the credits and the main menu
	CSSprite *_menuTitle = nullptr;
	CSSprite *_menuSubtitle = nullptr;
	CSSprite *_menuMenu = nullptr;
	int _menuIdx = 0;

	void centreString(CSImage *image, const char *text, int y, uint8 colour);
	void printTransferFieldNames(CSImage *image);
	void printOldSaveField(CSImage *image, const Common::String &text, int yOffset);
	void printNewSaveField(CSImage *image, const Common::String &text, int yOffset);
	bool clickedOnButton(const CSSprite *button) const;
	bool transferPage1();
	bool transferShowStats(TransferSaveData &oldSave, TransferSaveData &newSave);
	bool transferPortrait(TransferSaveData &newSave, int &portrait);
	void transferNoSaveFound();
	bool transfer();
	void ackHeader(CSImage *image);
	void acknowledgements();
	void mainMenuSetPal(int idx);
	void mainMenuLoad();
	void selectedIntro();
	bool selectedCreateCharacter();
	bool selectedTransfer();
	void selectedAcknowledgments();
	/** Returns true if the player chose to quit, or false to journey onward */
	bool mainMenu();

public:
	void play();
};

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
