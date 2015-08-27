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

#ifndef AESOP_AESOP_H
#define AESOP_AESOP_H

#include "common/scummsys.h"
#include "common/array.h"
#include "common/endian.h"
#include "common/hash-str.h"
#include "common/serializer.h"
#include "common/random.h"
#include "common/savefile.h"
#include "common/util.h"
#include "engines/engine.h"

namespace Aesop {

enum {
	kDebugLevelScript      = 1 << 0,
};

enum GameType {
	GType_EOB3 = 0,
	GType_DungeonHack = 1
};

#define AESOP_SCREEN_WIDTH 320
#define AESOP_SCREEN_HEIGHT 200

struct AesopGameDescription;

class AesopEngine : public Engine {
private:
	Common::RandomSource _randomSource;

	void initialize();
public:
	const AesopGameDescription *_gameDescription;
public:
	AesopEngine(OSystem *syst, const AesopGameDescription *gameDesc);
	virtual ~AesopEngine();

	/**
	 * Main method for running the game
	 */
	virtual Common::Error run();

	GameType getGameID() const;
	bool hasFeature(EngineFeature f) const;

	int getRandomNumber(int max);
};

extern AesopEngine *_vm;

} // End of namespace Aesop

#endif
