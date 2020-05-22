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

#include "ultima/ultima2/ultima2.h"
#include "common/debug.h"
#include "common/system.h"

namespace Ultima {
namespace Ultima2 {

Ultima2Engine *g_ultima;

Ultima2Engine::Ultima2Engine(OSystem *syst, const Ultima::UltimaGameDescription *gameDesc) :
		Shared::UltimaEngine(syst, gameDesc), _saveSlotToLoad(-1) {
	g_ultima = this;
}

Ultima2Engine::~Ultima2Engine() {
}

bool Ultima2Engine::initialize() {
	if (!Shared::UltimaEngine::initialize())
		return false;

	_saveSlotToLoad = ConfMan.hasKey("save_slot") ? ConfMan.getInt("save_slot") : -1;

	return true;
}

Common::Error Ultima2Engine::run() {
	if (initialize()) {
	}

	return Common::kNoError;
}

bool Ultima2Engine::isDataRequired(Common::String &folder, int &majorVersion, int &minorVersion) {
	folder = "ultima2";
	majorVersion = 1;
	minorVersion = 0;
	return false;
}

} // End of namespace Ultima2
} // End of namespace Ultima
