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

#ifndef LEGEND_DETECTION_H
#define LEGEND_DETECTION_H

#include "engines/advancedDetector.h"

namespace Legend {

enum GameId {
	GAME_XANTH,
	GAME_SHANNARA,
	GAME_DEATH_GATE
};

enum LegendDebugChannels {
	kDebugGraphics = 1 << 0,
	kDebugPath     = 1 << 1,
	kDebugScan     = 1 << 2,
	kDebugFilePath = 1 << 3,
	kDebugScript   = 1 << 4
};

struct LegendGameDescription {
	ADGameDescription _desc;
	GameId _gameId;
};

extern const PlainGameDescriptor GAME_NAMES[];

extern const LegendGameDescription GAME_DESCRIPTIONS[];

} // namespace Legend

class LegendMetaEngineDetection : public AdvancedMetaEngineDetection {
	mutable Common::String _gameid;
	mutable Common::String _extra;
	mutable Common::String _filename;
	mutable Common::String _md5;

	static const DebugChannelDef debugFlagList[];

public:
	LegendMetaEngineDetection();
	~LegendMetaEngineDetection() override {}

	const char *getEngineId() const override {
		return "legend";
	}

	const char *getName() const override {
		return "FreeLegend";
	}

	const char *getOriginalCopyright() const override {
		return "Legend (C)";
	}

	const DebugChannelDef *getDebugChannels() const override {
		return debugFlagList;
	}

	ADDetectedGame fallbackDetect(const FileMap &allFiles, const Common::FSList &fslist, ADDetectedGameExtraInfo **extra = nullptr) const override;

	bool canPlayUnknownVariants() const override {
		return true;
	}
};

#endif
