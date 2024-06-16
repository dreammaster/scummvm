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

#include "common/engine_data.h"
#include "wasteland/gfx/surface.h"
#include "wasteland/wasteland1/wasteland1.h"
#include "wasteland/wasteland1/gfx/cursors.h"
#include "wasteland/wasteland1/gfx/color_font.h"
#include "wasteland/wasteland1/gfx/mono_font.h"

namespace Wasteland {
namespace Wasteland1 {

Wasteland1Engine *g_engine;

Wasteland1Engine::Wasteland1Engine(OSystem *syst, const WastelandGameDescription *gameDesc) :
		Wasteland::Engine(syst, gameDesc) {
	g_engine = this;
}

Wasteland1Engine::~Wasteland1Engine() {
	g_engine = nullptr;
	delete _views;
}

void Wasteland1Engine::initializePath(const Common::FSNode &gamePath) {
	Engine::initializePath(gamePath);
	SearchMan.addDirectory("data", gamePath.getChild("rom").getChild("data"), 0, 3);
}

void Wasteland1Engine::setup() {
	// Initialise engine data for the game
	Common::U32String errMsg;
	if (!Common::load_engine_data("wasteland.dat", "wasteland1", 1, 0, errMsg)) {
		Common::String msg(errMsg);
		error("%s", msg.c_str());
	}

	// Setup game views
	_views = new Views::Views();
	addView("Roster");

	Surface::setupPalette();

	auto monoFont = new Gfx::MonoFont();
	monoFont->load();
	_fonts.push_back(monoFont);
	auto colorFont = new Gfx::ColorFont();
	colorFont->load();
	_fonts.push_back(colorFont);

	Gfx::Cursors::load(_cursors);
	setCursor(0);

	_gameArchive = new GameArchive();
	SearchMan.add("Game", _gameArchive);
}

} // namespace Wasteland1
} // namespace Wasteland
