/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "asylum/views/menu.h"

#include "asylum/resources/actor.h"

#include "asylum/system/config.h"
#include "asylum/system/cursor.h"
#include "asylum/system/graphics.h"
#include "asylum/system/savegame.h"
#include "asylum/system/screen.h"
#include "asylum/system/sound.h"
#include "asylum/system/text.h"
#include "asylum/system/video.h"

#include "asylum/views/scene.h"

#include "asylum/respack.h"
#include "asylum/staticres.h"

#include "asylum/asylum.h"

namespace Asylum {

MainMenu::MainMenu(AsylumEngine *vm): _vm(vm) {
	_initGame = false;

	_activeScreen   = kMenuNone;
	_soundResourceId = kResourceNone;
	_allowInteraction = false;
	_currentIcon = kMenuNone;
	_dword_4464B8 = -1;
	_dword_455C74 = 0;
	_dword_455C78 = false;
	_dword_455C80 = false;
	_dword_455D4C = false;
	_dword_455D5C = false;
	_dword_455DD8 = false;
	_dword_456288 = 0;
	_dword_4562C0 = 0;
	_dword_4562C4 = 0;
	_dword_45628C = 0;
	_needEyeCursorInit = false;

	memset(&_iconFrames, 0, sizeof(_iconFrames));
}

MainMenu::~MainMenu() {
	// Zero-out passed pointers
	_vm = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Loading and setup
//////////////////////////////////////////////////////////////////////////
void MainMenu::show() {
	getSharedData()->setFlag(kFlagSkipDrawScene, true);
	getScreen()->clear();

	// Set ourselves as the current event handler
	_vm->switchEventHandler(this);

	getSound()->stopAll();

	_activeScreen = kMenuShowCredits;
	_soundResourceId = kResourceNone;
	_musicResourceId = kResourceNone;
	_allowInteraction = false;

	_dword_4562C4 = 480;
	_dword_45628C = 0;

	setup();
}

void MainMenu::setup() {
	getScreen()->clear();
	// Original fills the screen with black

	getCursor()->hide();

	getSharedData()->setFlag(kFlag1, true);

	if (_vm->isGameFlagSet(kGameFlagFinishGame)) {
		getText()->loadFont(MAKE_RESOURCE(kResourcePackShared, 32));
		getScreen()->setPalette(MAKE_RESOURCE(kResourcePackShared, 31));
		getScreen()->setGammaLevel(MAKE_RESOURCE(kResourcePackShared, 31), 0);
		getScreen()->setupTransTables(4, MAKE_RESOURCE(kResourcePackShared, 34),
		                                 MAKE_RESOURCE(kResourcePackShared, 35),
		                                 MAKE_RESOURCE(kResourcePackShared, 36),
		                                 MAKE_RESOURCE(kResourcePackShared, 37));
		getScreen()->selectTransTable(1);

		_dword_455D4C = false;
		_dword_455D5C = false;

		getSound()->playSound(MAKE_RESOURCE(kResourcePackShared, 56), false, Config.voiceVolume);
	} else {
		getText()->loadFont(MAKE_RESOURCE(kResourcePackShared, 25));
		getScreen()->setPalette(MAKE_RESOURCE(kResourcePackShared, 26));
		getScreen()->setGammaLevel(MAKE_RESOURCE(kResourcePackShared, 26), 0);
		getScreen()->setupTransTables(4, MAKE_RESOURCE(kResourcePackShared, 27),
		                                 MAKE_RESOURCE(kResourcePackShared, 28),
		                                 MAKE_RESOURCE(kResourcePackShared, 29),
		                                 MAKE_RESOURCE(kResourcePackShared, 30));
		getScreen()->selectTransTable(1);

		getSound()->playMusic(kResourceNone, 0);
		getSound()->playMusic(MAKE_RESOURCE(kResourcePackShared, 38));
	}
}

void MainMenu::leave() {
	_activeScreen = kMenuNone;
	getCursor()->set(MAKE_RESOURCE(kResourcePackShared, 2), 0, 2);
	getText()->loadFont(kFontYellow);
}

void MainMenu::switchFont(bool condition) {
	getText()->loadFont((condition) ? kFontYellow : kFontBlue);
}

void MainMenu::showCredits() {
	getScreen()->clear();
	// Original fills the screen with black

	getCursor()->show();
	getSharedData()->setFlag(kFlag1, false);

	getText()->loadFont(kFontYellow);
	getScreen()->setPalette(MAKE_RESOURCE(kResourcePackShared, 17));
	getScreen()->setGammaLevel(MAKE_RESOURCE(kResourcePackShared, 17), 0);
	getScreen()->setupTransTables(4, MAKE_RESOURCE(kResourcePackShared, 18),
	                                 MAKE_RESOURCE(kResourcePackShared, 19),
	                                 MAKE_RESOURCE(kResourcePackShared, 20),
	                                 MAKE_RESOURCE(kResourcePackShared, 21));
	getScreen()->selectTransTable(1);

	getSound()->playMusic(kResourceNone, 0);
	getSound()->playMusic(_musicResourceId);

	if (_vm->isGameFlagSet(kGameFlagFinishGame)) {
		if (!_dword_455D4C) {
			_dword_455D4C = true;
			getSound()->stop(MAKE_RESOURCE(kResourcePackShared, 56));
		}
	}

	leave();
}

void MainMenu::listMovies() {
	warning("[MainMenu::listMovies] Not implemented!");
}

MainMenu::MenuScreen MainMenu::findMousePosition() {
	for (uint i = 0; i < ARRAYSIZE(menuRects); i++)
		if (menuRects[i].contains(getCursor()->position()))
			return (MenuScreen)i;

	return kMenuNone;
}

//////////////////////////////////////////////////////////////////////////
// Event Handler
//////////////////////////////////////////////////////////////////////////
bool MainMenu::handleEvent(const AsylumEvent &evt) {
	switch ((uint32)evt.type) {
	default:
		break;

	case EVENT_ASYLUM_INIT:
		return init();
		break;

	case EVENT_ASYLUM_UPDATE:
		return update();
		break;

	case EVENT_ASYLUM_MUSIC:
		return music();
		break;

	case Common::EVENT_KEYDOWN:
		return key(evt);
		break;

	case Common::EVENT_LBUTTONDOWN:
	case Common::EVENT_RBUTTONDOWN:
		return click(evt);
		break;
	}

	return false;
}

bool MainMenu::init() {
	// TODO: save dialog key codes into sntrm_k.txt (need to figure out why they use such thing) (address 00411CD0)

	if (_needEyeCursorInit) {
		getCursor()->set(MAKE_RESOURCE(kResourcePackShared, 3), 0, 2);
	} else {
		// Init the game if not already done
		if (!_initGame) {
			_initGame = true;

			// The original also
			//  - load the config (this is done when constructing the config object).
			//  - initialize game structures (this is done in classes constructors)

			getSaveLoad()->loadViewedMovies();

			_needEyeCursorInit = true;

			// Play start video
			getVideo()->playVideo(0);

			if (!getSaveLoad()->setup()) {
				_vm->restart();
				return true;
			}

			// The original preloads graphics

			getCursor()->show();
		}

		_dword_4562C0 = 0;
		_activeScreen = kMenuNone;
		_currentIcon = kMenuNone;
		_dword_455C74 = 0;

		getCursor()->hide();
		getCursor()->set(MAKE_RESOURCE(kResourcePackShared, 2), 0, 2);
	}

	if (_allowInteraction)
		getScene()->getActor()->update_409230();

	getScreen()->clear();
	getText()->loadFont(kFontYellow);
	getScreen()->setPalette(MAKE_RESOURCE(kResourcePackShared, 17));
	getScreen()->setGammaLevel(MAKE_RESOURCE(kResourcePackShared, 17), 0);
	getScreen()->setupTransTables(4, MAKE_RESOURCE(kResourcePackShared, 18),
	                                 MAKE_RESOURCE(kResourcePackShared, 19),
	                                 MAKE_RESOURCE(kResourcePackShared, 20),
	                                 MAKE_RESOURCE(kResourcePackShared, 21));
	getScreen()->selectTransTable(1);

	// Update the screen
	g_system->updateScreen();

	getCursor()->show();
	return true;
}

bool MainMenu::update() {
	uint32 ticks = _vm->getTick();

	if (!getSharedData()->getFlag(kFlagRedraw)) {
		// TODO original fills a rectangle with black

		// Draw background
		getScreen()->draw(kBackground, (_activeScreen == kMenuNone) ? 1 : 0, 0, 0, 0);

		uint32 frameIndex = 0;

		// Get the eye frame index
		if (!getCursor()->isHidden()) {
			Common::Point cursor = getCursor()->position();

			if (cursor.x < 230 || cursor.x > 399 || cursor.y < 199 || cursor.y > 259)
				frameIndex = eyeFrameIndex[Actor::direction(Common::Point(320, 240), cursor)];
			else if (cursor.x >= 743 && cursor.x <= 743 && cursor.y >= 587 && cursor.y <= 602)
				frameIndex = 9;
		}

		if (_activeScreen == kMenuNone) {
			// Draw eye
			getScreen()->draw(kEye, frameIndex, 0, 0, 0);

			// Find mouse position
			MenuScreen icon = findMousePosition();
			if (icon != kMenuNone) {
				// Draw icon
				getScreen()->draw(MAKE_RESOURCE(kResourcePackShared, icon + 4), _iconFrames[icon], 0, 0, 0);
				_iconFrames[icon] = (_iconFrames[icon] + 1) % GraphicResource::getFrameCount(_vm, MAKE_RESOURCE(kResourcePackShared, icon + 4));

				// Draw text
				getText()->drawCentered(menuRects[icon].left - 5, menuRects[icon].bottom + 5, menuRects[icon].width(), MAKE_RESOURCE(kResourcePackText, 1309 + icon));

				if (!_dword_455C74 || _currentIcon != icon) {
					_dword_455C74 = true;
					_currentIcon = icon;

					if (_soundResourceId
					 && getSound()->isPlaying(_soundResourceId)
					 && _soundResourceId != MAKE_RESOURCE(kResourcePackShared, icon + 44))
						getSound()->stopAll(_soundResourceId);

					if (_soundResourceId != MAKE_RESOURCE(kResourcePackShared, icon + 44) || !getSound()->isPlaying(_soundResourceId)) {
						_soundResourceId = MAKE_RESOURCE(kResourcePackShared, icon + 44);
						getSound()->playSound(_soundResourceId, false, Config.voiceVolume);
					}
				}
			} else {
				_dword_455C74 = 0;
			}
		} else {
			getScreen()->draw(kEye, frameIndex, 0, 0, 0, 3);

			// Draw icon
			getScreen()->draw(MAKE_RESOURCE(kResourcePackShared, _activeScreen + 4), _iconFrames[_activeScreen], 0, 0, 0);
			_iconFrames[_activeScreen] = (_iconFrames[_activeScreen] + 1) % GraphicResource::getFrameCount(_vm, MAKE_RESOURCE(kResourcePackShared, _activeScreen + 4));
		}

		// Update current screen
		switch (_activeScreen) {
		default:
			break;

		case kMenuNewGame:
			updateNewGame();
			break;

		case kMenuLoadGame:
			updateLoadGame();
			break;

		case kMenuSaveGame:
			updateSaveGame();
			break;

		case kMenuDeleteGame:
			updateDeleteGame();
			break;

		case kMenuViewMovies:
			updateViewMovies();
			break;

		case kMenuQuitGame:
			updateQuitGame();
			break;

		case kMenuTextOptions:
			updateTextOptions();
			break;

		case kMenuAudioOptions:
			updateAudioOptions();
			break;

		case kMenuSettings:
			updateSettings();
			break;

		case kMenuKeyboardConfig:
			updateKeyboardConfig();
			break;

		case kMenuShowCredits:
			updateShowCredits();
			break;

		case kMenuReturnToGame:
			updateReturnToGame();
			break;
		}

		// Ask for redraw
		getSharedData()->setFlag(kFlagRedraw, true);
	}

	if (ticks > getSharedData()->getNextScreenUpdate()) {
		if (getSharedData()->getFlag(kFlagRedraw)) {
			getScreen()->copyBackBufferToScreen();

			getSharedData()->setFlag(kFlagRedraw, false);
			getSharedData()->setNextScreenUpdate(ticks + 55);
		}
	}

	return true;
}

bool MainMenu::music() {
	if (_activeScreen == kMenuShowCredits
	 && _vm->isGameFlagSet(kGameFlagFinishGame)
	 && !_dword_455D5C
	 && !_dword_455D4C) {
		_dword_455D5C = true;

		getSound()->playMusic(kResourceNone, 0);
		getSound()->playMusic(MAKE_RESOURCE(kResourcePackShared, 38));

		return true;
	}

	return false;
}

bool MainMenu::key(const AsylumEvent &evt) {
	switch (_activeScreen) {
	default:
		break;

	case kMenuSaveGame:
		keySaveGame();
		break;

	case kMenuKeyboardConfig:
		keyKeyboardConfig();
		break;

	case kMenuShowCredits:
		keyShowCredits();
		break;
	}

	return true;
}

bool MainMenu::click(const AsylumEvent &evt) {
	if (evt.type == Common::EVENT_RBUTTONDOWN
	 && _activeScreen == kMenuShowCredits) {
		 clickShowCredits();
		 return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Handle clicks on sub-screens
	if (_activeScreen != kMenuNone) {
		switch (_activeScreen) {
		default:
			break;

		case kMenuNewGame:
			updateNewGame();
			break;

		case kMenuLoadGame:
			updateLoadGame();
			break;

		case kMenuSaveGame:
			updateSaveGame();
			break;

		case kMenuDeleteGame:
			updateDeleteGame();
			break;

		case kMenuViewMovies:
			updateViewMovies();
			break;

		case kMenuQuitGame:
			updateQuitGame();
			break;

		case kMenuTextOptions:
			updateTextOptions();
			break;

		case kMenuAudioOptions:
			updateAudioOptions();
			break;

		case kMenuSettings:
			updateSettings();
			break;

		case kMenuKeyboardConfig:
			updateKeyboardConfig();
			break;

		case kMenuShowCredits:
			updateShowCredits();
			break;

		case kMenuReturnToGame:
			updateReturnToGame();
			break;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Handle clicks on the main menu
	_activeScreen = findMousePosition();
	if (_activeScreen == kMenuNone)
		return true;

	getCursor()->set(MAKE_RESOURCE(kResourcePackShared, 3), 0, 2);
	getText()->loadFont(kFontYellow);

	switch (_activeScreen) {
	default:
		break;

	case kMenuSaveGame:
		_dword_455DD8 = false;
		// Fallback to next case

	case kMenuLoadGame:
		_dword_455C80 = false;
		_dword_455C78 = false;
		_dword_456288 = 0;
		_dword_4562C4 = 0;
		getSaveLoad()->loadList();
		break;

	case kMenuDeleteGame:
		_dword_455C80 = false;
		_dword_4562C4 = 0;
		getSaveLoad()->loadList();
		break;

	case kMenuViewMovies:
		_needEyeCursorInit = false;
		_dword_455C78 = false;
		_dword_456288 = 0;
		_dword_4562C4 = 0;
		listMovies();
		break;

	case kMenuKeyboardConfig:
		_dword_4464B8 = -1;
		break;

	case kMenuReturnToGame:
		if (!_allowInteraction)
			break;

		clickReturnToGame();
		break;

	case kMenuShowCredits:
		_dword_4562C4 = 480;
		_dword_45628C = 0;
		setup();
		break;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
// Update handlers
//////////////////////////////////////////////////////////////////////////
void MainMenu::updateNewGame() {
	error("[MainMenu::updateNewGame] Not implemented!");
}

void MainMenu::updateLoadGame() {
	error("[MainMenu::updateLoadGame] Not implemented!");
}

void MainMenu::updateSaveGame() {
	error("[MainMenu::updateSaveGame] Not implemented!");
}

void MainMenu::updateDeleteGame() {
	error("[MainMenu::updateDeleteGame] Not implemented!");
}

void MainMenu::updateViewMovies() {
	error("[MainMenu::updateViewMovies] Not implemented!");
}

void MainMenu::updateQuitGame() {
	error("[MainMenu::updateQuitGame] Not implemented!");
}

void MainMenu::updateTextOptions() {
	error("[MainMenu::updateTextOptions] Not implemented!");
}

void MainMenu::updateAudioOptions() {
	error("[MainMenu::updateAudioOptions] Not implemented!");
}

void MainMenu::updateSettings() {
	error("[MainMenu::updateSettings] Not implemented!");
}

void MainMenu::updateKeyboardConfig() {
	error("[MainMenu::updateKeyboardConfig] Not implemented!");
}

void MainMenu::updateReturnToGame() {
	error("[MainMenu::updateReturnToGame] Not implemented!");
}

void MainMenu::updateShowCredits() {
	error("[MainMenu::updateShowCredits] Not implemented!");
}

//////////////////////////////////////////////////////////////////////////
// Click handlers
//////////////////////////////////////////////////////////////////////////
void MainMenu::clickNewGame() {
	error("[MainMenu::clickNewGame] Not implemented!");
}

void MainMenu::clickLoadGame() {
	error("[MainMenu::clickLoadGame] Not implemented!");
}

void MainMenu::clickSaveGame() {
	error("[MainMenu::clickSaveGame] Not implemented!");
}

void MainMenu::clickDeleteGame() {
	error("[MainMenu::clickDeleteGame] Not implemented!");
}

void MainMenu::clickViewMovies() {
	error("[MainMenu::clickViewMovies] Not implemented!");
}

void MainMenu::clickQuitGame() {
	error("[MainMenu::clickQuitGame] Not implemented!");
}

void MainMenu::clickTextOptions() {
	error("[MainMenu::clickTextOptions] Not implemented!");
}

void MainMenu::clickAudioOptions() {
	error("[MainMenu::clickAudioOptions] Not implemented!");
}

void MainMenu::clickSettings() {
	error("[MainMenu::clickSettings] Not implemented!");
}

void MainMenu::clickKeyboardConfig() {
	error("[MainMenu::clickKeyboardConfig] Not implemented!");
}

void MainMenu::clickReturnToGame() {
	error("[MainMenu::clickReturnToGame] Not implemented!");
}

void MainMenu::clickShowCredits() {
	showCredits();
}

//////////////////////////////////////////////////////////////////////////
// Key handlers
//////////////////////////////////////////////////////////////////////////
void MainMenu::keySaveGame() {
	error("[MainMenu::keySaveGame] Not implemented!");
}

void MainMenu::keyKeyboardConfig() {
	error("[MainMenu::keyKeyboardConfig] Not implemented!");
}

void MainMenu::keyShowCredits() {
	showCredits();
}

//////////////////////////////////////////////////////////////////////////
// TODO REMOVE
//////////////////////////////////////////////////////////////////////////

// UpdateShowCredits
//// TODO if game finished (gameFlag=901) then show resource image 33 and music 56 and than music 40
////if (!_creditsResource)
////	_creditsResource = new GraphicResource(_vm, MAKE_RESOURCE(kResourcePackShared, 24));
////if (!_creditsFadeResource)
////	_creditsFadeResource = new GraphicResource(_vm, MAKE_RESOURCE(kResourcePackShared, 23));
////_creditsTextScroll = 0x1E0 - 30;
//// Set credits palette
//getScreen()->setPalette(MAKE_RESOURCE(kResourcePackShared, 26));
//// Stop all sounds
//getSound()->stopMusic();
//// Start playing music
//getSound()->playMusic(MAKE_RESOURCE(kResourcePackShared, 38));
//
//void MainMenu::updateMainMenu() {
//	//int32 rowId = 0;
//
//	//if (getCursor()->position().y >= 20 && getCursor()->position().y <= 20 + 48) {
//	//	rowId = 0; // Top row
//	//} else if (getCursor()->position().y >= 400 && getCursor()->position().y <= 400 + 48) {
//	//	rowId = 1; // Bottom row
//	//} else {
//	//	// No row selected
//	//	_previousActiveIcon = _activeScreen = -1;
//	//	_leftClick = false;
//	//	return;
//	//}
//
//	getText()->loadFont(kFontYellow);
//
//	// Icon animation
////	for (int32 i = 0; i <= 5; i++) {
////		int32 curX = 40 + i * 100;
////		if (getCursor()->position().x >= curX && getCursor()->position().x <= curX + 55) {
////			int32 iconNum = i + 6 * rowId;
////			_activeScreen = iconNum;
////
////			// The last 2 icons are swapped
////			if (iconNum == 11)
////				iconNum = 10;
////			else if (iconNum == 10)
////				iconNum = 11;
////
////			// Get the current icon animation
////	/*		if (!_iconResource || _activeScreen != _previousActiveIcon) {
////				delete _iconResource;
////				_iconResource = new GraphicResource(_vm, MAKE_RESOURCE(kResourcePackShared, iconNum + 4));
////			}*/
////
////	/*		GraphicFrame *iconFrame = _iconResource->getFrame(MIN<uint>(_iconResource->count() - 1, _curIconFrame));
////			_vm->screen()->copyRectToScreenWithTransparency((byte *)iconFrame->surface.pixels, iconFrame->surface.w, iconFrame->x, iconFrame->y, iconFrame->surface.w, iconFrame->surface.h);
////*/
////			//// Cycle icon frame
////			//_curIconFrame++;
////			//if (_curIconFrame >= _iconResource->count())
////			//	_curIconFrame = 0;
////
////			//// Show text
////			//getText()->drawCentered(MenuIconFixedXpos[iconNum],
////			//		iconFrame->y + 50,
////			//		getText()->getWidth(MAKE_RESOURCE(kResourcePackText, iconNum + 1309)),
////			//		MAKE_RESOURCE(kResourcePackText, iconNum + 1309));
////
////			// Play creepy voice
////			if (_activeScreen != _previousActiveIcon) {
////				getSound()->playSound(MAKE_RESOURCE(kResourcePackShared, iconNum + 44), false, Config.sfxVolume);
////				_previousActiveIcon = _activeScreen;
////			}
////
////			break;
////		}
////	}
//}
//

//
//void MainMenu::updateSubMenuNewGame() {
//	getText()->loadFont(kFontYellow);
//
//	// begin new game
//	getText()->drawCentered(10, 100, 620, MAKE_RESOURCE(kResourcePackText, 529));
//
//	// Yes
//	switchFont(getCursor()->position().x < 247 || getCursor()->position().x > 247 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 1322)) || getCursor()->position().y < 273 || getCursor()->position().y > 273 + 24);
//	getText()->setPosition(247, 273);
//	getText()->draw(MAKE_RESOURCE(kResourcePackText, 1322));
//
//	// No
//	switchFont(getCursor()->position().x < 369 || getCursor()->position().x > 369 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 1323)) || getCursor()->position().y < 273 ||	getCursor()->position().y > 273 + 24);
//	getText()->setPosition(369, 273);
//	getText()->draw(MAKE_RESOURCE(kResourcePackText, 1323));
//
//	// action
//	//if (_leftClick) {
//	//	// Yes
//	//	if (getCursor()->position().x >= 247 && getCursor()->position().x <= 247 + 24 && getCursor()->position().y >= 273 && getCursor()->position().y <= 273 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 1322))) {
//	//		_leftClick = false;
// //           // TODO closeMenu();
// //           _vm->startGame(kResourcePackTowerCells, AsylumEngine::kStartGamePlayIntro);
//	//	}
//	//	// No
//	//	if (getCursor()->position().x >= 369 && getCursor()->position().x <= 369 + 24 && getCursor()->position().y >= 273 && getCursor()->position().y <= 273 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 1323)))
//	//		exitSubMenu();
//	//}
//}
//
//void MainMenu::updateSubMenuCinematics() {
//	int32 currentCD = 1;	// FIXME: dummy value
//	getText()->drawCentered(10, 100, 620, MAKE_RESOURCE(kResourcePackText, 1352), currentCD);
//	getText()->setPosition(30, 340);
//	getText()->draw(MAKE_RESOURCE(kResourcePackText, 1353));	// Prev Page
//
//	//if (getCursor()->position().x >= 280 && getCursor()->position().x <= 400 && getCursor()->position().y >= 340 && getCursor()->position().y <= 360) {
//	//	getText()->loadFont(kFontBlue);
//	//	if (_leftClick)
//	//		exitSubMenu();
//	//} else {
//	//	getText()->loadFont(kFontYellow);
//	//}
//
//	getText()->setPosition(280, 340);
//	getText()->draw(MAKE_RESOURCE(kResourcePackText, 1355));	// Main Menu
//
//	getText()->loadFont(kFontYellow);
//
//	getText()->setPosition(500, 340);
//	getText()->draw(MAKE_RESOURCE(kResourcePackText, 1354));	// Next Page
//}
//
//void MainMenu::updateSubMenuSettings() {
//	int32 sizeMinus	= getText()->getWidth("-");
//	int32 sizePlus		= getText()->getWidth("+");
//	int32 sizeMainMenu = getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 1437));
//
//	getText()->loadFont(kFontYellow);
//	// Settings
//	getText()->drawCentered(10, 100, 620, MAKE_RESOURCE(kResourcePackText, 1432));
//
//	// gamma correction
//	getText()->drawAlignedRight(320, 150, MAKE_RESOURCE(kResourcePackText, 1433));
//	switchFont(getCursor()->position().x < 350 || getCursor()->position().x > sizeMinus + 350 || getCursor()->position().y < 150 || getCursor()->position().y > 174);
//	getText()->setPosition(350, 150);
//	getText()->draw("-");
//
//	switchFont(getCursor()->position().x < sizeMinus + 360 || getCursor()->position().x > sizeMinus + sizePlus + 360 || getCursor()->position().y < 150 || getCursor()->position().y > 174);
//	getText()->setPosition(sizeMinus + 360, 150);
//	getText()->draw("+");
//
//	getText()->setPosition(sizeMinus + sizePlus + 365, 150);
//	getText()->loadFont(kFontYellow);
//	if (Config.gammaLevel) {
//		for (int32 i = 0; i < Config.gammaLevel; i++) {
//			getText()->draw("]");
//		}
//		if (Config.gammaLevel == 8)
//			getText()->draw("*");
//	} else
//		getText()->draw(MAKE_RESOURCE(kResourcePackText, 1435));
//
//	// performance
//	getText()->loadFont(kFontYellow);
//	getText()->drawAlignedRight(320, 179, MAKE_RESOURCE(kResourcePackText, 1434));
//	switchFont(getCursor()->position().x < 350 || getCursor()->position().x > sizeMinus + 350 || getCursor()->position().y < 179 || getCursor()->position().y > 203);
//	getText()->setPosition(350, 179);
//	getText()->draw("-");
//
//	switchFont(getCursor()->position().x < sizeMinus + 360 || getCursor()->position().x > sizeMinus + sizePlus + 360 || getCursor()->position().y < 179 || getCursor()->position().y > 203);
//	getText()->setPosition(sizeMinus + 360, 179);
//	getText()->draw("+");
//
//	getText()->setPosition(sizeMinus + sizePlus + 365, 179);
//	getText()->loadFont(kFontYellow);
//	if (Config.performance == 5) {
//		getText()->draw(MAKE_RESOURCE(kResourcePackText, 1436));
//	} else {
//		for (int32 i = 5; i > Config.performance; --i) {
//			getText()->draw("]");
//		}
//		if (!Config.performance)
//			getText()->draw("*");
//	}
//
//	// back to main menu
//	switchFont(getCursor()->position().x < 300 || getCursor()->position().x > 300 + sizeMainMenu || getCursor()->position().y < 340 || getCursor()->position().y > 340 + 24);
//	getText()->setPosition(300, 340);
//	getText()->draw(MAKE_RESOURCE(kResourcePackText, 1437));
//
//	// action
//	//if (_leftClick) {
//	//	// back to main menu
//	//	if (getCursor()->position().x >= 300 && getCursor()->position().x <= 300 + sizeMainMenu && getCursor()->position().y >= 340 && getCursor()->position().y <= 340 + 24) {
//	//		// TODO: save new configurations
//	//		exitSubMenu();
//	//	}
//
//	//	// gamma level minus
//	//	if (getCursor()->position().x >= 350 && getCursor()->position().x <= sizeMinus + 350 && getCursor()->position().y >= 150 && getCursor()->position().y <= 174) {
//	//		if (Config.gammaLevel) {
//	//			Config.gammaLevel -= 1;
//	//			// TODO: setResGammaLevel(0x80010011, 0);
//	//		}
//	//	}
//	//	// gamma level plus
//	//	if (getCursor()->position().x >= sizeMinus + 360 && getCursor()->position().x <= sizeMinus + sizePlus + 360 && getCursor()->position().y >= 150 && getCursor()->position().y <= 174) {
//	//		if (Config.gammaLevel < 8) {
//	//			Config.gammaLevel += 1;
//	//			// TODO: setResGammaLevel(0x80010011, 0);
//	//		}
//	//	}
//
//	//	// performance minus
//	//	if (getCursor()->position().x >= 350 && getCursor()->position().x <= sizeMinus + 350 && getCursor()->position().y >= 179 && getCursor()->position().y <= 203) {
//	//		if (Config.performance) {
//	//			Config.performance -= 1;
//	//			// TODO: change quality settings
//	//		}
//	//	}
//	//	// performance plus
//	//	if (getCursor()->position().x >= sizeMinus + 360 && getCursor()->position().x <= sizeMinus + sizePlus + 360 && getCursor()->position().y >= 179 && getCursor()->position().y <= 203) {
//	//		if (Config.performance < 5) {
//	//			Config.performance += 1;
//	//			// TODO: change quality settings
//	//		}
//	//	}
//	//}
//}
//
//void MainMenu::updateSubMenuQuitGame() {
//	getText()->loadFont(kFontYellow);
//
//	// begin new game
//	getText()->drawCentered(10, 100, 620, MAKE_RESOURCE(kResourcePackText, 1408));
//
//	// Yes
//	switchFont(getCursor()->position().x < 247 || getCursor()->position().x > 247 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 581)) || getCursor()->position().y < 273 || getCursor()->position().y > 273 + 24);
//	getText()->setPosition(247, 273);
//	getText()->draw(MAKE_RESOURCE(kResourcePackText, 1409));
//
//	// No
//	switchFont(getCursor()->position().x < 369 || getCursor()->position().x > 369 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 582)) || getCursor()->position().y < 273 || getCursor()->position().y > 273 + 24);
//	getText()->setPosition(369, 273);
//	getText()->draw(MAKE_RESOURCE(kResourcePackText, 1410));
//
//	//// action
//	//if (_leftClick) {
//	//	// Yes
//	//	if (getCursor()->position().x >= 247 && getCursor()->position().x <= 247 + 24 && getCursor()->position().y >= 273 && getCursor()->position().y <= 273 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 1409))) {
//	//		_leftClick = false;
//
//	//		// User clicked on quit, so push a quit event
//	//		Common::Event event;
//	//		event.type = Common::EVENT_QUIT;
//	//		g_system->getEventManager()->pushEvent(event);
//	//	}
//	//	// No
//	//	if (getCursor()->position().x >= 369 && getCursor()->position().x <= 369 + 24 && getCursor()->position().y >= 273 && getCursor()->position().y <= 273 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 1410)))
//	//		exitSubMenu();
//	//}
//}
//
//void MainMenu::updateSubMenuShowCredits() {
//	//int32 posY	 = _creditsTextScroll;
//	//ResourceId resourceId = kResourceNone;
//	//int32 step	 = 0;
//	//int32 minBound = 0;
//	//int32 maxBound = 0;
//
//	////GraphicFrame *creditsFadeFrame = _creditsFadeResource->getFrame(0);
//	////_vm->screen()->copyRectToScreenWithTransparency((byte *)creditsFadeFrame->surface.pixels, creditsFadeFrame->surface.w, creditsFadeFrame->x, creditsFadeFrame->y, creditsFadeFrame->surface.w, creditsFadeFrame->surface.h);
//
//	////GraphicFrame *creditsFrame = _creditsResource->getFrame(MIN<uint>(_creditsResource->count() - 1, _creditsBgFrame));
//	////_vm->screen()->copyRectToScreenWithTransparency((byte *)creditsFrame->surface.pixels, creditsFrame->surface.w, creditsFrame->x, creditsFrame->y, creditsFrame->surface.w, creditsFrame->surface.h);
//
//	////_creditsBgFrame++;
//	////if (_creditsBgFrame >= _creditsResource->count())
//	////	_creditsBgFrame = 0;
//
//	//do {
//	//	if (posY + step >= 0) {
//	//		if (posY + step > 450)
//	//			break;
//
//	//		minBound = posY + step + 24;
//	//		if (minBound >= 0)
//	//			if (minBound < 32) {
//	//				// TODO fade side text
//	//				posY = _creditsTextScroll;
//	//			}
//
//	//		maxBound = posY + step;
//	//		if (posY + step < 480)
//	//			if (maxBound > 448) {
//	//				// TODO fade side text
//	//				posY = _creditsTextScroll;
//	//			}
//
//	//		getText()->setPosition(320, step + posY);
//	//		getText()->draw((ResourceId)(resourceId - 2147482201));
//	//		posY = _creditsTextScroll;
//	//	}
//	//	step += 24;
//	//	resourceId = (ResourceId)(resourceId + 1);
//	//} while (step < 0x21F0);
//
//	//_creditsTextScroll -= 2;
//
//	//// TODO: some palette stuffs
//
//	//// TODO: if gameFlag=901 (finished game) and already play music 56, start music 40
//
//	//if (_leftClick) {
//	//	// Restore palette
//	//	_vm->screen()->setPalette(MAKE_RESOURCE(kResourcePackShared, 17));
//	//	// Stop all sounds
//	//	_vm->sound()->stopMusic();
//	//	// Start playing music
//	//	_vm->sound()->playMusic(MAKE_RESOURCE(kResourcePackShared, 39));
//	//	exitSubMenu();
//	//}
//}
//
//void MainMenu::exitSubMenu() {
//	//_leftClick = false;
//	//_activeScreen = kMenuMain;
//
//	//// Copy the bright background to the back buffer
//	//getScreen()->draw(MAKE_RESOURCE(kResourcePackShared, 0), 1, 0, 0, 0);
//
//	//// Set the cursor
//	//getCursor()->set(MAKE_RESOURCE(kResourcePackShared, 2));
//}
//
//void MainMenu::updateSubMenuReturnToGame() {
//	getText()->loadFont(kFontYellow);
//
//	// no game loaded
//	getText()->drawCentered(10, 100, 620, MAKE_RESOURCE(kResourcePackText, 712));
//
//	// Main menu
//	switchFont(getCursor()->position().x < 285 || getCursor()->position().x > 285 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 713)) || getCursor()->position().y < 273 || getCursor()->position().y > 273 + 24);
//	getText()->setPosition(285, 273);
//	getText()->draw(MAKE_RESOURCE(kResourcePackText, 713));
//
//	// action
//	//if (_leftClick) {
//	//	// Main menu
//	//	if (getCursor()->position().x >= 285 && getCursor()->position().x <= 285 + getText()->getWidth(MAKE_RESOURCE(kResourcePackText, 713)) && getCursor()->position().y >= 273 && getCursor()->position().y <= 273 + 24)
//	//		exitSubMenu();
//	//}
//}

} // end of namespace Asylum
