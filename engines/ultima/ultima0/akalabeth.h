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

#ifndef ULTIMA_AKALABETH_H
#define ULTIMA_AKALABETH_H

#include "graphics/screen.h"
#include "ultima/shared/engine/ultima.h"
#include "ultima/ultima0/defines.h"
#include "ultima/ultima0/struct.h"
#include "ultima/ultima0/events.h"

namespace Ultima {
namespace Ultima0 {

class AkalabethEngine : public Shared::UltimaEngine, public Events {
private:
	const ADGameDescription *_gameDescription;
protected:
	// Engine APIs
	Common::Error run() override;

public:
	Graphics::Screen *_screen = nullptr;

public:
	AkalabethEngine(OSystem *syst, const Ultima::UltimaGameDescription *gameDesc);
	~AkalabethEngine() override {
		delete _screen;
	}

	/**
	 * Get a random number
	 */
	int getRandomNumber(int maxVal) {
		return Shared::UltimaEngine::getRandomNumber(maxVal);
	}
	double rnd() {
		return (double)getRandomNumber(0xffff) / 1.0;
	}

	/**
	 * Get the ellapsed time in milliseconds
	 */
	uint32 getTicks() const;


	/**
	 * Returns true if the game should quit
	 */
	bool shouldQuit() const override {
		return Shared::UltimaEngine::shouldQuit();
	}
};

extern AkalabethEngine *g_engine;
#define SHOULD_QUIT ::Ultima::Ultima0::g_engine->shouldQuit()

} // namespace Ultima0
} // namespace Ultima

#endif
