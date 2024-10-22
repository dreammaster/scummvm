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
#include "ultima/ultima0/attack.h"
#include "ultima/ultima0/british.h"
#include "ultima/ultima0/ddraw.h"
#include "ultima/ultima0/dead.h"
#include "ultima/ultima0/draw.h"
#include "ultima/ultima0/dungeon.h"
#include "ultima/ultima0/globals.h"
#include "ultima/ultima0/hardware.h"
#include "ultima/ultima0/main.h"
#include "ultima/ultima0/monst.h"
#include "ultima/ultima0/move.h"
#include "ultima/ultima0/player.h"
#include "ultima/ultima0/sdw.h"
#include "ultima/ultima0/town.h"
#include "ultima/ultima0/world.h"

namespace Ultima {
namespace Ultima0 {

class AkalabethEngine : public Shared::UltimaEngine {
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
	double rnd() {
		return (double)getRandomNumber(0xffff) / 1.0;
	}

	/**
	 * Get the ellapsed time in milliseconds
	 */
	uint32 getTicks() const;
};

extern AkalabethEngine *g_ultima;

} // namespace Ultima0
} // namespace Ultima

#endif
