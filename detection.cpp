/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
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

#include "aesop/aesop.h"
#include "common/system.h"
#include "common/translation.h"
#include "engines/advancedDetector.h"

namespace Aesop {

struct AesopGameDescription {
	ADGameDescription desc;

	GameType gameID;
};

GameType AesopEngine::getGameID() const {
	return _gameDescription->gameID;
}

} // End of namespace Aesop

static const PlainGameDescriptor AesopGames[] = {
	{ "eob3", "Eye of the Beholder 3" },
	{ "dungeonhack", "Dungeon Hack" },
	{0, 0}
};

#include "aesop/detection_tables.h"

class AesopMetaEngine : public AdvancedMetaEngine {
public:
	AesopMetaEngine() : AdvancedMetaEngine(Aesop::gameDescriptions, sizeof(Aesop::AesopGameDescription),
		AesopGames) {}

	virtual const char *getName() const {
		return "Aesop Engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "Aesop Engine (C)";
	}

	/**
	 * Creates an instance of the game engine
	 */
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;

	/**
	 * Returns a list of features the game's MetaEngine support
	 */
	virtual bool hasFeature(MetaEngineFeature f) const;

	/**
	 * Return a list of savegames
	 */
	virtual SaveStateList listSaves(const char *target) const;

	/**
	 * Returns the maximum number of allowed save slots
	 */
	virtual int getMaximumSaveSlot() const;

	/**
	 * Deletes a savegame in the specified slot
	 */
	virtual void removeSaveState(const char *target, int slot) const;

	/**
	 * Given a specified savegame slot, returns extended information for the save
	 */
	SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const;
};

bool AesopMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const Aesop::AesopGameDescription *gd = (const Aesop::AesopGameDescription *)desc;
	if (gd) {
		*engine = new Aesop::AesopEngine(syst, gd);
	}
	return gd != 0;
}

bool AesopMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		(f == kSupportsListSaves) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSupportsDeleteSave) ||
		(f == kSavesSupportMetaInfo) ||
		(f == kSavesSupportThumbnail);
}

bool Aesop::AesopEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsRTL) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

SaveStateList AesopMetaEngine::listSaves(const char *target) const {
	// todo
	return SaveStateList();
}

int AesopMetaEngine::getMaximumSaveSlot() const {
	return 0;
}

void AesopMetaEngine::removeSaveState(const char *target, int slot) const {
	// TODO
}

SaveStateDescriptor AesopMetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	// TODO
	return SaveStateDescriptor();
}


#if PLUGIN_ENABLED_DYNAMIC(AESOP)
	REGISTER_PLUGIN_DYNAMIC(AESOP, PLUGIN_TYPE_ENGINE, AesopMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(AESOP, PLUGIN_TYPE_ENGINE, AesopMetaEngine);
#endif
