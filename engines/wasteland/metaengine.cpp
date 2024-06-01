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

#include "wasteland/metaengine.h"
#include "wasteland/detection.h"
#include "wasteland/keymapping.h"
#include "wasteland/wasteland1/wasteland1.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {

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

} // End of namespace Wasteland

const char *WastelandMetaEngine::getName() const {
	return "wasteland";
}

const ADExtraGuiOptionsMap *WastelandMetaEngine::getAdvancedExtraGuiOptions() const {
	return Wasteland::optionsList;
}

Common::Error WastelandMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const auto *gd = (Wasteland::WastelandGameDescription *)desc;

	switch (gd->gameType) {
	case Wasteland::GAMETYPE_WASTELAND:
		*engine = new Wasteland::Wasteland1::Wasteland1Engine(syst, gd);
		break;
	case Wasteland::GAMETYPE_FOD:
		*engine = new Wasteland::FOD::FountainOfDreamsEngine(syst, gd);
		break;

	default:
		error("Unknown game");
		break;
	}

	return Common::kNoError;
}

bool WastelandMetaEngine::hasFeature(MetaEngineFeature f) const {
	return checkExtendedSaves(f) ||
		(f == kSupportsLoadingDuringStartup);
}

Common::KeymapArray WastelandMetaEngine::initKeymaps(const char *target) const {
	return Wasteland::Keymapping::initKeymaps();
}

#if PLUGIN_ENABLED_DYNAMIC(WASTELAND)
REGISTER_PLUGIN_DYNAMIC(WASTELAND, PLUGIN_TYPE_ENGINE, WastelandMetaEngine);
#else
REGISTER_PLUGIN_STATIC(WASTELAND, PLUGIN_TYPE_ENGINE, WastelandMetaEngine);
#endif
