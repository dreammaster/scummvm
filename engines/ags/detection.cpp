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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 * You may also modify/distribute the code in this file under that license.
 */

#include "base/plugins.h"
#include "common/debug.h"
#include "engines/advancedDetector.h"
#include "ags/detection_tables.h"
#include "ags/resourceman.h"
#include "ags/constants.h"

namespace AGS {

static char s_fallbackFilenameBuffer[51];
static char s_fallbackTitleBuffer[51];

}

class AGSMetaEngineDetection : public AdvancedMetaEngineDetection {
public:
	AGSMetaEngineDetection() : AdvancedMetaEngineDetection(AGS::GAME_DESCRIPTIONS, sizeof(AGS::AGSGameDescription), AGS::GAMES) {
		_maxScanDepth = 3;
	}

	const char *getEngineId() const override {
		return "ags";
	}

	const char *getName() const override {
		return "AGS";
	}

	const char *getOriginalCopyright() const override {
		return "Copyright (C) 1999-2011 Chris Jones";
	}

	ADDetectedGame fallbackDetect(const FileMap &allFiles, const Common::FSList &fslist) const override;
};

ADDetectedGame AGSMetaEngineDetection::fallbackDetect(const FileMap &allFiles, const Common::FSList &fslist) const {
	// Set the default values for the fallback descriptor's ADGameDescription part.
	AGS::AGSGameDescription &gd = AGS::g_fallbackDesc;

	gd.desc.language = Common::UNK_LANG;
	gd.desc.platform = Common::kPlatformDOS;
	gd.desc.flags = ADGF_NO_FLAGS;

	for (Common::FSList::const_iterator file = fslist.begin(); file != fslist.end(); ++file) {
		if (file->isDirectory())
			continue;

		Common::String filename = file->getName();
		debug(4, "Checking file %s", filename.c_str());

		// Looking for either the Windows engine with the game data attached
		// or a standalone data file
		filename.toLowercase();
		if (!filename.hasSuffix(".exe") && !filename.hasSuffix(".ags"))
			continue;

		SearchMan.clear();
		SearchMan.addDirectory(file->getParent().getName(), file->getParent());
		AGS::ResourceManager resourceManager;
		if (!resourceManager.init(filename)) {
			debug(4, "File %s failed ResourceManager init", filename.c_str());
			continue;
		}

		Common::SeekableReadStream *dta = resourceManager.getFile(AGS::kGameDataNameV2);
		if (!dta) {
			// if no 2.x data, try 3.x
			debug(4, "No V2 game data. Trying V3...");
			dta = resourceManager.getFile(AGS::kGameDataNameV3);
			if (!dta) {
				debug(3, "AGS detection couldn't find game data file in '%s'", filename.c_str());
				continue;
			}
		}
		dta->skip(30 + 4); // signature + version
		uint32 versionStringLength = dta->readUint32LE();
		dta->skip(versionStringLength);

		char gameTitle[51];
		dta->read(gameTitle, 50);
		gameTitle[50] = '\0';
		delete dta;

		strncpy(AGS::s_fallbackTitleBuffer, gameTitle, 50);
		AGS::s_fallbackTitleBuffer[50] = '\0';
		gd.title = AGS::s_fallbackTitleBuffer;

		strncpy(AGS::s_fallbackFilenameBuffer, filename.c_str(), 20);
		AGS::s_fallbackFilenameBuffer[20] = '\0';
		gd.filename = AGS::s_fallbackFilenameBuffer;

		return ADDetectedGame(&gd.desc);
	}

	return ADDetectedGame();
}

REGISTER_PLUGIN_STATIC(AGS_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, AGSMetaEngineDetection);
