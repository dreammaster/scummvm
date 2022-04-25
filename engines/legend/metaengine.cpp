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

#include "legend/metaengine.h"
#include "legend/detection.h"
#include "legend/legend.h"
#include "legend/shannara/shannara_demo.h"

const char *LegendMetaEngine::getName() const {
	return "legend";
}

Common::Error LegendMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const Legend::LegendGameDescription *gd = (const Legend::LegendGameDescription *)desc;

	switch (gd->_gameId) {
	case Legend::GAME_XANTH:
		*engine = new Legend::Shannara::ShannaraDemo(syst, gd);
		return Common::kNoError;
	case Legend::GAME_SHANNARA:
		if (gd->_desc.flags & ADGF_DEMO) {
			*engine = new Legend::Shannara::ShannaraDemo(syst, gd);
			return Common::kNoError;
		}
		// Fall-through
	default:
		// Unknown Legend game
		return Common::kUnsupportedGameidError;
	}
}

bool LegendMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
	    (f == kSupportsListSaves) ||
	    (f == kSupportsDeleteSave) ||
	    (f == kSavesSupportMetaInfo) ||
	    (f == kSavesSupportThumbnail) ||
	    (f == kSupportsLoadingDuringStartup);
}

#if PLUGIN_ENABLED_DYNAMIC(LEGEND)
REGISTER_PLUGIN_DYNAMIC(LEGEND, PLUGIN_TYPE_ENGINE, LegendMetaEngine);
#else
REGISTER_PLUGIN_STATIC(LEGEND, PLUGIN_TYPE_ENGINE, LegendMetaEngine);
#endif
