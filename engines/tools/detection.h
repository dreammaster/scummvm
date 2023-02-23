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

#ifndef TOOLS_DETECTION_H
#define TOOLS_DETECTION_H

#include "engines/advancedDetector.h"

namespace Tools {

enum ToolsDebugChannels {
	kDebugGraphics = 1 << 0,
	kDebugPath     = 1 << 1,
	kDebugScan     = 1 << 2,
	kDebugFilePath = 1 << 3,
	kDebugScript   = 1 << 4,
};

extern const PlainGameDescriptor toolsGames[];

extern const ADGameDescription gameDescriptions[];

#define GAMEOPTION_ORIGINAL_SAVELOAD GUIO_GAMEOPTIONS1

} // End of namespace Tools

class ToolsMetaEngineDetection : public AdvancedMetaEngineDetection {
	static const DebugChannelDef debugFlagList[];

public:
	ToolsMetaEngineDetection();
	~ToolsMetaEngineDetection() override {}

	const char *getName() const override {
		return "tools";
	}

	const char *getEngineName() const override {
		return "Tools";
	}

	const char *getOriginalCopyright() const override {
		return "Tools (C)";
	}

	const DebugChannelDef *getDebugChannels() const override {
		return debugFlagList;
	}
};

#endif // TOOLS_DETECTION_H
