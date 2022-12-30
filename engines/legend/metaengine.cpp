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

#include "common/translation.h"

#include "legend/metaengine.h"
#include "legend/detection.h"
#include "legend/legend.h"

namespace Legend {

static const ADExtraGuiOptionsMap optionsList[] = {
	{
		GAMEOPTION_ORIGINAL_SAVELOAD,
		{
			_s("Use original save/load screens"),
			_s("Use the original save/load screens instead of the ScummVM ones"),
			"original_menus",
			false,
			0,
			0
		}
	},
	AD_EXTRA_GUI_OPTIONS_TERMINATOR
};

} // End of namespace Legend

const char *LegendMetaEngine::getName() const {
	return "legend";
}

const ADExtraGuiOptionsMap *LegendMetaEngine::getAdvancedExtraGuiOptions() const {
	return Legend::optionsList;
}

Common::Error LegendMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	*engine = new Legend::LegendEngine(syst, desc);
	return Common::kNoError;
}

bool LegendMetaEngine::hasFeature(MetaEngineFeature f) const {
	return checkExtendedSaves(f) ||
		(f == kSupportsLoadingDuringStartup);
}

#if PLUGIN_ENABLED_DYNAMIC(LEGEND)
REGISTER_PLUGIN_DYNAMIC(LEGEND, PLUGIN_TYPE_ENGINE, LegendMetaEngine);
#else
REGISTER_PLUGIN_STATIC(LEGEND, PLUGIN_TYPE_ENGINE, LegendMetaEngine);
#endif
