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
#include "engines/game.h"

#include "bagel/bagel.h"

static const PlainGameDescriptor plumbersGames[] = {
	{"Bagel", "Bagel"},
	{nullptr, nullptr}
};

static const DebugChannelDef debugFlagList[] = {
	{Bagel::kDebugGeneral, "general", "General debug level"},
	DEBUG_CHANNEL_END
};

namespace Bagel {

static const ADGameDescription gameDescriptions[] = {
	AD_TABLE_END_MARKER
};

} // End of namespace Bagel

class BagelMetaEngineDetection : public AdvancedMetaEngineDetection {
public:
	BagelMetaEngineDetection() : AdvancedMetaEngineDetection(Bagel::gameDescriptions, sizeof(ADGameDescription), plumbersGames) {
	}

	const char *getEngineId() const override {
		return "bagel";
	}

	const char *getName() const override {
		return "Bagel";
	}

	const char *getOriginalCopyright() const override {
		return "Bagel";
	}

	const DebugChannelDef *getDebugChannels() const override {
		return debugFlagList;
	}
};

REGISTER_PLUGIN_STATIC(BAGEL_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, BagelMetaEngineDetection);
