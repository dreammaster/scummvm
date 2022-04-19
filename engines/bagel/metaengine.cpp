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

#include "base/plugins.h"

#include "engines/advancedDetector.h"

#include "bagel/bagel.h"

namespace Bagel {
const char *BagelEngine::getGameId() const { return _gameDescription->gameId; }
Common::Platform BagelEngine::getPlatform() const { return _gameDescription->platform; }
} // End of namespace Bagel

class BagelMetaEngine : public AdvancedMetaEngine {
	const char *getName() const override {
		return "bagel";
	}

	Common::Error createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const override;
	bool hasFeature(MetaEngineFeature f) const override;
};

Common::Error BagelMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	*engine = new Bagel::BagelEngine(syst, desc);
	return Common::kNoError;
}

bool BagelMetaEngine::hasFeature(MetaEngineFeature f) const {
	return false;
}

#if PLUGIN_ENABLED_DYNAMIC(BAGEL)
	REGISTER_PLUGIN_DYNAMIC(BAGEL, PLUGIN_TYPE_ENGINE, BagelMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(BAGEL, PLUGIN_TYPE_ENGINE, BagelMetaEngine);
#endif
