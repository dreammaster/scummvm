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

#include "dink/metaengine.h"
#include "dink/detection.h"
#include "dink/dink.h"

const char *DinkMetaEngine::getName() const {
	return "dink";
}

Common::Error DinkMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const Dink::DinkGameDescription *gd = (const Dink::DinkGameDescription *)desc;

	*engine = new Dink::DinkEngine(syst, gd);
	return Common::kNoError;
}

bool DinkMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
	    (f == kSupportsListSaves) ||
	    (f == kSupportsDeleteSave) ||
	    (f == kSavesSupportMetaInfo) ||
	    (f == kSavesSupportThumbnail) ||
	    (f == kSupportsLoadingDuringStartup);
}

#if PLUGIN_ENABLED_DYNAMIC(DINK)
REGISTER_PLUGIN_DYNAMIC(DINK, PLUGIN_TYPE_ENGINE, DinkMetaEngine);
#else
REGISTER_PLUGIN_STATIC(DINK, PLUGIN_TYPE_ENGINE, DinkMetaEngine);
#endif
