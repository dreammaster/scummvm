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

#ifndef WIZARDRY_DETECTION_H
#define WIZARDRY_DETECTION_H

#include "engines/advancedDetector.h"

namespace Wizardry {

enum WizardryDebugChannels {
	kDebugGraphics = 1,
	kDebugPath,
	kDebugScan,
	kDebugFilePath,
	kDebugScript,
};

enum WizardryVersion {
	kWizardryNone, kWizardry1V1, kWizardry1V2
};

struct WizardryGameDescription {
	AD_GAME_DESCRIPTION_HELPERS(desc);
	ADGameDescription desc;
	WizardryVersion version;
	bool uncompressed;
};

extern const PlainGameDescriptor wizardryGames[];

extern const WizardryGameDescription gameDescriptions[];

#define GAMEOPTION_ORIGINAL_SAVELOAD GUIO_GAMEOPTIONS1

} // End of namespace Wizardry

class WizardryMetaEngineDetection : public AdvancedMetaEngineDetection<Wizardry::WizardryGameDescription> {
	static const DebugChannelDef debugFlagList[];

public:
	WizardryMetaEngineDetection();
	~WizardryMetaEngineDetection() override {}

	const char *getName() const override {
		return "wizardry";
	}

	const char *getEngineName() const override {
		return "Wizardry";
	}

	const char *getOriginalCopyright() const override {
		return "Wizardry (C)";
	}

	const DebugChannelDef *getDebugChannels() const override {
		return debugFlagList;
	}
};

#endif // WIZARDRY_DETECTION_H
