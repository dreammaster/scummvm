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

#include "ags2/metaengine.h"
#include "ags2/detection.h"
#include "ags2/ags2.h"

namespace AGS2 {

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

} // namespace AGS2

const char *AGS2MetaEngine::getName() const {
	return "AGS2";
}

const ADExtraGuiOptionsMap *AGS2MetaEngine::getAdvancedExtraGuiOptions() const {
	return AGS2::optionsList;
}

Common::Error AGS2MetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	*engine = new AGS2::AGS2Engine(syst, desc);
	return Common::kNoError;
}

bool AGS2MetaEngine::hasFeature(MetaEngineFeature f) const {
	return checkExtendedSaves(f) ||
		(f == kSupportsLoadingDuringStartup);
}

#if PLUGIN_ENABLED_DYNAMIC(AGS2)
REGISTER_PLUGIN_DYNAMIC(AGS2, PLUGIN_TYPE_ENGINE, AGS2MetaEngine);
#else
REGISTER_PLUGIN_STATIC(AGS2, PLUGIN_TYPE_ENGINE, AGS2MetaEngine);
#endif
