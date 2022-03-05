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

#ifndef LEGEND_LEGEND_H
#define LEGEND_LEGEND_H

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

#include "legend/detection.h"

namespace Legend {

struct Globals;
struct LegendGameDescription;

class LegendEngine : public Engine {
private:
	const LegendGameDescription *_gameDescription;
	Common::RandomSource _randomSource;

	void initialize();
protected:
	// Engine APIs
	Common::Error run() override;
public:
	Globals *_globals = nullptr;
public:
	LegendEngine(OSystem *syst, const LegendGameDescription *gameDesc);
	~LegendEngine() override;
//	void GUIError(const Common::String &msg);

	uint32 getFeatures() const;

	/**
	 * Returns the game Id
	 */
	Common::String getGameId() const;

	/**
	 * Sets the random number seed
	 */
	void setRandomNumberSeed(uint32 seed) {
		_randomSource.setSeed(seed);
	}

	bool hasFeature(EngineFeature f) const override {
		return
		    (f == kSupportsLoadingDuringRuntime) ||
		    (f == kSupportsSavingDuringRuntime) ||
		    (f == kSupportsReturnToLauncher);
	};
};

extern LegendEngine *g_engine;
#define SHOULD_QUIT ::Legend::g_engine->shouldQuit()

} // namespace Legend

#endif
