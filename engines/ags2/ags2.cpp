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

#include "common/scummsys.h"
#include "ags2/ags2.h"
#include "ags2/detection.h"
#include "ags2/console.h"
#include "ags2/ac/main.h"

namespace AGS2 {

AGS2Engine *g_engine;

AGS2Engine::AGS2Engine(OSystem *syst, const AGS2GameDescription *gameDesc) : Engine(syst),
	_gameDescription(gameDesc), _randomSource("AGS2") {
	g_engine = this;
}

AGS2Engine::~AGS2Engine() {
	delete _screen;
}

uint32 AGS2Engine::getFeatures() const {
	return _gameDescription->desc.flags;
}

const char *AGS2Engine::getFilename() const {
	return _gameDescription->desc.filesDescriptions[0].fileName;
};

Common::String AGS2Engine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Error AGS2Engine::run() {
	const char *argv[] = { nullptr };
	ags_main(1, argv);

	return Common::kNoError;
}

Common::Error AGS2Engine::syncGame(Common::Serializer &s) {
	// The Serializer has methods isLoading() and isSaving()
	// if you need to specific steps; for example setting
	// an array size after reading it's length, whereas
	// for saving it would write the existing array's length
	int dummy = 0;
	s.syncAsUint32LE(dummy);

	return Common::kNoError;
}

} // namespace AGS2
