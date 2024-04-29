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

#include "krondor/metaengine.h"
#include "krondor/detection.h"
#include "krondor/krondor.h"

namespace Krondor {

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

} // End of namespace Krondor

const char *KrondorMetaEngine::getName() const {
	return "krondor";
}

const ADExtraGuiOptionsMap *KrondorMetaEngine::getAdvancedExtraGuiOptions() const {
	return Krondor::optionsList;
}

Common::Error KrondorMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	*engine = new Krondor::KrondorEngine(syst, desc);
	return Common::kNoError;
}

bool KrondorMetaEngine::hasFeature(MetaEngineFeature f) const {
	return checkExtendedSaves(f) ||
		(f == kSupportsLoadingDuringStartup);
}

#if PLUGIN_ENABLED_DYNAMIC(KRONDOR)
REGISTER_PLUGIN_DYNAMIC(KRONDOR, PLUGIN_TYPE_ENGINE, KrondorMetaEngine);
#else
REGISTER_PLUGIN_STATIC(KRONDOR, PLUGIN_TYPE_ENGINE, KrondorMetaEngine);
#endif
