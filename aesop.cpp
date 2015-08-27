/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "aesop/aesop.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"

namespace Aesop {

AesopEngine *_vm;

AesopEngine::AesopEngine(OSystem *syst, const AesopGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc), _randomSource("Aesop") {
	_vm = this;
}

AesopEngine::~AesopEngine() {
}

void AesopEngine::initialize() {
	DebugMan.addDebugChannel(kDebugLevelScript, "scripts", "Script debug level");
}

Common::Error AesopEngine::run() {
	// Initialize the engine
	initialize();

	return Common::kNoError;
}

int AesopEngine::getRandomNumber(int max) {
	return _randomSource.getRandomNumber(max);
}

} // End of namespace Aesop
