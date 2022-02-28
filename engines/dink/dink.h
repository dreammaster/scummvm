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

#ifndef DINK_DINK_H
#define DINK_DINK_H

#include "common/scummsys.h"
#include "common/system.h"
#include "common/error.h"
#include "common/fs.h"
#include "common/random.h"
#include "common/hash-str.h"
#include "common/util.h"
#include "engines/engine.h"
#include "engines/savestate.h"
#include "graphics/surface.h"

#include "dink/detection.h"

namespace Dink {

class EventsManager;
struct DinkGameDescription;

class DinkEngine : public Engine {
private:
	const DinkGameDescription *_gameDescription;
	Common::RandomSource _randomSource;

	void initialize();
protected:
	// Engine APIs
	Common::Error run() override;
public:
	EventsManager *_events = nullptr;

public:
	DinkEngine(OSystem *syst, const DinkGameDescription *gameDesc);
	~DinkEngine() override;
//	void GUIError(const Common::String &msg);

	uint32 getFeatures() const;

	/**
	 * Returns the game Id
	 */
	Common::String getGameId() const;

	/**
	 * Returns the current list of savegames
	 */
//	SaveStateList listSaves() const;

	/**
	 * Gets a random number
	 */
	uint32 getRandomNumber(uint maxNum) {
		return _randomSource.getRandomNumber(maxNum);
	}

	/**
	 * Gets a random number
	 */
	uint32 getRandomNumber(uint minNum, uint maxNum) {
		return minNum + _randomSource.getRandomNumber(maxNum - minNum);
	}

	bool hasFeature(EngineFeature f) const override {
		return
		    (f == kSupportsLoadingDuringRuntime) ||
		    (f == kSupportsSavingDuringRuntime) ||
		    (f == kSupportsReturnToLauncher);
	};
};

extern DinkEngine *g_engine;
#define SHOULD_QUIT ::Dink::g_engine->shouldQuit()

} // namespace Dink

#endif
