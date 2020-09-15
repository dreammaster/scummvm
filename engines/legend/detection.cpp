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

#include "base/plugins.h"
#include "common/system.h"
#include "engines/game.h"
#include "legend/detection.h"
#include "legend/detection_tables.h"
#include "legend/shannara_demo/shannara_demo.h"

namespace Legend {

static const PlainGameDescriptor LEGEND_GAMES[] = {
	{ "shannara", "Shannara" },
	{ 0, 0 }
};

} // End of namespace Legend

LegendMetaEngine::LegendMetaEngine() : AdvancedMetaEngine(Legend::GAME_DESCRIPTIONS,
	        sizeof(Legend::LegendGameDescription), Legend::LEGEND_GAMES) {
}

bool LegendMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const Legend::LegendGameDescription *gd = (const Legend::LegendGameDescription *)desc;
	if (gd) {
		switch (gd->gameId) {
		case Legend::GAME_SHANNARA:
			*engine = new Legend::Shannara::ShannaraDemo(syst, *gd);
			break;

		default:
			error("Unsupported legend engine game specified");
		}
	}

	return gd != 0;
}

#if PLUGIN_ENABLED_DYNAMIC(LEGEND)
REGISTER_PLUGIN_DYNAMIC(LEGEND, PLUGIN_TYPE_ENGINE, LegendMetaEngine);
#else
REGISTER_PLUGIN_STATIC(LEGEND, PLUGIN_TYPE_ENGINE, LegendMetaEngine);
#endif
