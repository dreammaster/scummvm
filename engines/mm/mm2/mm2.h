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

#ifndef MM2_H
#define MM2_H

#include "common/error.h"
#include "common/random.h"
#include "common/scummsys.h"
#include "common/serializer.h"
#include "common/system.h"
#include "engines/engine.h"
#include "mm/mm2/events.h"
#include "mm/mm.h"

namespace MM {
namespace MM2 {

class MM2Engine : public MMEngine, public Events {
protected:
	// Engine APIs
	Common::Error run() override;

	/**
	 * Returns true if the game should quit
	 */
	bool shouldQuit() const override {
		return Engine::shouldQuit();
	}

public:
	MM2Engine(OSystem *syst, const MightAndMagicGameDescription *gameDesc);
	~MM2Engine() override;

	uint32 getFeatures() const;
	bool isDemo() const override;

	/**
	 * Returns the game Id
	 */
	Common::String getGameId() const;

	bool hasFeature(EngineFeature f) const override {
		return (f == kSupportsLoadingDuringRuntime) ||
			(f == kSupportsSavingDuringRuntime) ||
			(f == kSupportsReturnToLauncher);
	};

	bool canLoadGameStateCurrently(Common::U32String *msg = nullptr) override;
	bool canSaveGameStateCurrently(Common::U32String *msg = nullptr) override;

	/**
	 * Uses a serializer to allow implementing savegame
	 * loading and saving using a single method
	 */
	Common::Error syncGame(Common::Serializer &s);

	Common::Error saveGameStream(Common::WriteStream *stream, bool isAutosave = false) override;
	Common::Error loadGameStream(Common::SeekableReadStream *stream) override;

	/**
	 * Returns a random number
	 */
	int getRandomNumber(int minNumber, int maxNumber) {
		return MMEngine::getRandomNumber(minNumber, maxNumber);
	}
	int getRandomNumber(int maxNumber) {
		return MMEngine::getRandomNumber(maxNumber);
	}
};

extern MM2Engine *g_engine;
#define SHOULD_QUIT ::MM:MM2::g_engine->shouldQuit()

} // namespace MM2
} // namespace MM

#endif
