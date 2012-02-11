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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

// Base stuff
#include "common/error.h"
#include "common/random.h"
#include "common/events.h"

#include "engines/advancedDetector.h"
#include "engines/util.h"

// Audio
#include "audio/mixer.h"

// AGS subsystems
#include "ags/ags.h"
#include "ags/constants.h"
#include "ags/resourceman.h"
#include "ags/gamefile.h"

namespace AGS {

AGSEngine::AGSEngine(OSystem *syst, const AGSGameDescription *gameDesc) :
	Engine(syst), _gameDescription(gameDesc), _engineStartTime(0), _playTime(0),
	_width(0), _height(0), _resourceMan(0), _forceLetterbox(false) {

	_rnd = new Common::RandomSource("ags");
}

AGSEngine::~AGSEngine() {
	delete _gameFile;
	delete _resourceMan;

	delete _rnd;
}

bool AGS::AGSEngine::hasFeature(EngineFeature f) const {
	return f == kSupportsReturnToLauncher;
}

const char *AGSEngine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Language AGSEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

Common::Platform AGSEngine::getPlatform() const {
	return _gameDescription->desc.platform;
}

bool AGSEngine::isDemo() const {
	return (_gameDescription->desc.flags & ADGF_DEMO) != 0;
}

const ADGameFileDescription *AGSEngine::getGameFiles() const {
	return _gameDescription->desc.filesDescriptions;
}

Common::Error AGSEngine::run() {
	if (!init())
		return Common::kUnknownError;

	return Common::kNoError;
}

Common::String AGSEngine::getMasterArchive() const {
	const ADGameFileDescription *gameFiles = getGameFiles();

	if (gameFiles[0].fileName)
		return gameFiles[0].fileName;

	return "ac2game.dat";
}

bool AGSEngine::init() {
	// Open the archive file
	_resourceMan = new ResourceManager();
	if (!_resourceMan->init(getMasterArchive()))
		return false;

	// Load the game file
	_gameFile = new GameFile();
	if (!_gameFile->init(*_resourceMan))
		return false;

	// Init graphics

	if (!getScreenSize())
		return false;

	if (!initGraphics())
		return false;

	syncSoundSettings();

	_engineStartTime = g_system->getMillis();

	return true;
}

bool AGSEngine::getScreenSize() {
	_baseWidth = 320;
	_baseHeight = 200;
	_textMultiply = 2;

	switch (_gameFile->_defaultResolution) {
	case 0:
		_textMultiply = 1;
		break;
	case 1:
	case 2:
		_width = 640;
		_height = 480;
		break;
	case 3:
	case 4:
		_width = 640;
		_height = 400;
		break;
	case 5:
		_baseWidth = 400;
		_baseHeight = 300;
		break;
	default:
		_baseWidth = 512;
		_baseHeight = 384;
		break;
	}

	if (_gameFile->_defaultResolution >= 5) {
		_width = _baseWidth * 2;
		_height = _baseHeight * 2;
		_gameFile->_options[OPT_LETTERBOX] = 0;
		_forceLetterbox = false;
	}

	return true;
}

bool AGSEngine::initGraphics() {
	if ((_width == 0) || (_height == 0))
		return false;

	::initGraphics(_width, _height, 0);

	return true;
}

void AGSEngine::pauseEngineIntern(bool pause) {
	_mixer->pauseAll(pause);
}

} // End of namespace AGS
