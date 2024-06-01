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

#include "wasteland/gfx/wasteland_font.h"
#include "wasteland/gfx/surface.h"
#include "wasteland/wasteland1/wasteland1.h"

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
	delete _font;
}

void Wasteland1Engine::initializePath(const Common::FSNode &gamePath) {
	Engine::initializePath(gamePath);
	SearchMan.addDirectory("data", gamePath.getChild("rom").getChild("data"), 0, 3);
}

void Wasteland1Engine::setup() {
	_views = new Views::Views();
	addView("Title");

	Gfx::Surface::setupPalette();
	auto font = new Wasteland::Gfx::WastelandFont();
	if (!font->load("colorf.fnt"))
		error("Could not load FONT");
}

} // namespace Wasteland1
} // namespace Wasteland
