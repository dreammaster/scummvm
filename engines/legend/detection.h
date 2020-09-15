/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
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

#ifndef LEGEND_DETECTION
#define LEGEND_DETECTION

#include "engines/advancedDetector.h"
#include "legend/detection.h"

namespace Legend {

enum GameId {
	GAME_SHANNARA
};

struct LegendGameDescription {
	ADGameDescription desc;
	GameId gameId;
	uint32 features;
};

} // End of namespace Legend

class LegendMetaEngine : public AdvancedMetaEngine {
private:
	/**
	 * Gets the game Id given a target string
	 */
	static Common::String getGameId(const char *target);
public:
	LegendMetaEngine();
	~LegendMetaEngine() override {}

	const char *getEngineId() const override {
		return "legend";
	}

	const char *getName() const override {
		return "Legend";
	}

	const char *getOriginalCopyright() const override {
		return "Legend Games (C) 1990-1995 Legend Entertainment Inc.";
	}

	bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const override;
};

#endif
