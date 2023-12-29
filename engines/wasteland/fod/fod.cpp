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

#include "wasteland/fod/fod.h"
#include "wasteland/fod/views/views.h"

namespace Wasteland {
namespace FOD {

FountainOfDreamsEngine *g_engine;

FountainOfDreamsEngine::FountainOfDreamsEngine(OSystem *syst, const ADGameDescription *gameDesc) :
		Wasteland::Engine(syst, gameDesc) {
	g_engine = this;
}

FountainOfDreamsEngine::~FountainOfDreamsEngine() {
	g_engine = nullptr;
	delete _views;
}

void FountainOfDreamsEngine::setup() {
	_views = new Views::Views();
	addView("Title");
}

} // namespace FOD
} // namespace Wasteland
