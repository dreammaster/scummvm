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

#include "legend/legend.h"
#include "common/system.h"
#include "common/translation.h"
#include "engines/advancedDetector.h"

namespace Legend {

struct LegendGameDescription {
	ADGameDescription desc;

	GameType gameID;
};

GameType LegendEngine::getGameID() const {
	return _gameDescription->gameID;
}

Common::Platform LegendEngine::getPlatform() const {
	return _gameDescription->desc.platform;
}

Common::Language LegendEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

bool LegendEngine::isLater() const {
	return getGameID() >= GType_CompanionsOfXanth;
}


} // End of namespace Legend

static const PlainGameDescriptor legendGames[] = {
	{ "gateway", "Frederik Pohl's Gateway" },
	{ "xanth", "Companions of Xanth" },
	{0, 0}
};

#include "legend/detection_tables.h"

class LegendMetaEngine : public AdvancedMetaEngine {
public:
	LegendMetaEngine() : AdvancedMetaEngine(Legend::gameDescriptions, sizeof(Legend::LegendGameDescription),
		legendGames) {}

	virtual const char *getName() const {
		return "Legend Engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "Legend Engine (C) 1990-1994 Legend Entertainment";
	}

	/**
	 * Creates an instance of the game engine
	 */
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;

	/**
	 * Returns a list of features the game's MetaEngine support
	 */
	virtual bool hasFeature(MetaEngineFeature f) const;
};

bool LegendMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
		const Legend::LegendGameDescription *gd = (const Legend::LegendGameDescription *)desc;
	*engine = new Legend::LegendEngine(syst, gd);

	return gd != 0;
}

bool LegendMetaEngine::hasFeature(MetaEngineFeature f) const {
	return 0;
}

#if PLUGIN_ENABLED_DYNAMIC(LEGEND)
	REGISTER_PLUGIN_DYNAMIC(LEGEND, PLUGIN_TYPE_ENGINE, LegendMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(LEGEND, PLUGIN_TYPE_ENGINE, LegendMetaEngine);
#endif
