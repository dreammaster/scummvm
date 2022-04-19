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

#ifndef BAGEL_BAGEL_H
#define BAGEL_BAGEL_H

#include "engines/engine.h"
#include "engines/advancedDetector.h"
#include "common/platform.h"

namespace Bagel {

enum BagelDebugChannels {
	kDebugGeneral = 1 << 0
};

class BagelEngine : public Engine {
public:
	BagelEngine(OSystem *syst, const ADGameDescription *gameDesc);
	~BagelEngine() override;

	Common::Error run() override;

	// Detection related functions
	const ADGameDescription *_gameDescription;
	const char *getGameId() const;
	Common::Platform getPlatform() const;
};

} // End of namespace Bagel

#endif
