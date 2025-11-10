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

#include "ags2/metaengine.h"
#include "ags2/detection.h"
#include "ags2/achievements_tables.h"
#include "ags2/ags2.h"
#include "ags2/globals.h"
#include "ags2/shared/util/directory.h"
#include "ags2/shared/util/file_stream.h"
#include "ags2/engine/ac/rich_game_media.h"
#include "ags2/engine/game/savegame.h"
#include "common/memstream.h"
#include "common/savefile.h"
#include "common/config-manager.h"
#include "engines/achievements.h"
#include "image/bmp.h"

const char *AGS2MetaEngine::getName() const {
	return "ags2";
}

Common::Error AGS2MetaEngine::createInstance(OSystem *syst, Engine **engine, const AGS2::AGS2GameDescription *gd) const {
	*engine = new AGS2::AGSEngine(syst, gd);
	return Common::kNoError;
}

SaveStateList AGS2MetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringArray filenames;
	Common::String pattern(getSavegameFilePattern(target));

	filenames = saveFileMan->listSavefiles(pattern);

	int maxSlot = getMaximumSaveSlot();
	SaveStateList saveList;
	for (Common::StringArray::const_iterator file = filenames.begin(); file != filenames.end(); ++file) {
		Common::String filename = Common::String::format("%s%s",
		                          ::AGS2::AGS::Shared::SAVE_FOLDER_PREFIX, file->c_str());

		::AGS2::AGS::Shared::FileStream saveFile(filename, ::AGS2::AGS::Shared::kFile_Open,
		        ::AGS2::AGS::Shared::kFile_Read);
		if (saveFile.IsValid()) {
			AGS2::RICH_GAME_MEDIA_HEADER rich_media_header;
			rich_media_header.ReadFromFile(&saveFile);

			if (rich_media_header.dwMagicNumber == RM_MAGICNUMBER) {
				int slotNum = atoi(file->c_str() + file->size() - 3);
				if (slotNum > maxSlot)
					continue;

				SaveStateDescriptor desc(this, slotNum, rich_media_header.getSaveName());
				saveList.push_back(desc);
			}
		}
	}

	// Sort saves based on slot number.
	Common::sort(saveList.begin(), saveList.end(), SaveStateDescriptorSlotComparator());
	return saveList;
}

bool AGS2MetaEngine::hasFeature(MetaEngineFeature f) const {
	return
	    (f == kSupportsListSaves) ||
	    (f == kSupportsDeleteSave) ||
	    (f == kSavesSupportMetaInfo) ||
	    (f == kSavesSupportThumbnail) ||
	    (f == kSupportsLoadingDuringStartup);
}

Common::String AGS2MetaEngine::getSavegameFile(int saveGameIdx, const char *target) const {
	if (saveGameIdx == kSavegameFilePattern) {
		// Pattern requested
		return Common::String::format("%s.###", target == nullptr ? getName() : target);
	} else {
		// Specific filename requested
		return Common::String::format("%s.%03d", target == nullptr ? getName() : target, saveGameIdx);
	}
}

SaveStateDescriptor AGS2MetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	Common::String filename = Common::String::format("%s%s",
	                          ::AGS2::AGS::Shared::SAVE_FOLDER_PREFIX,
	                          getSavegameFile(slot, target).c_str());

	::AGS2::AGS::Shared::FileStream saveFile(filename, ::AGS2::AGS::Shared::kFile_Open,
	        ::AGS2::AGS::Shared::kFile_Read);
	if (saveFile.IsValid()) {
		AGS2::RICH_GAME_MEDIA_HEADER rich_media_header;
		rich_media_header.ReadFromFile(&saveFile);

		if (rich_media_header.dwMagicNumber == RM_MAGICNUMBER) {
			SaveStateDescriptor desc(this, slot, rich_media_header.getSaveName());

			// Thumbnail handling
			if (rich_media_header.dwThumbnailOffsetLowerDword != 0 &&
			        rich_media_header.dwThumbnailSize != 0) {
				// Read in the thumbnail data
				byte *thumbData = (byte *)malloc(rich_media_header.dwThumbnailSize);
				saveFile.Seek(rich_media_header.dwThumbnailOffsetLowerDword,
				              AGS2::AGS::Shared::kSeekCurrent);
				saveFile.Read(thumbData, rich_media_header.dwThumbnailSize);
				Common::MemoryReadStream thumbStream(thumbData,
				                                     rich_media_header.dwThumbnailSize, DisposeAfterUse::YES);

				// Decode the thumbnail
				Image::BitmapDecoder decoder;
				if (decoder.loadStream(thumbStream)) {
					const Graphics::Surface *src = decoder.getSurface();
					const Graphics::Palette &pal = decoder.getPalette();
					Graphics::Surface *dest;

					if (src->w == 160 && src->h == 100) {
						dest = src->convertTo(g_system->getOverlayFormat(), pal.data(), pal.size());
					} else {
						Graphics::Surface *temp = src->convertTo(g_system->getOverlayFormat(), pal.data(), pal.size());
						dest = temp->scale(160, 100);
						temp->free();
						delete temp;
					}

					desc.setThumbnail(dest);
				}
			}

			return desc;
		}
	}

	return SaveStateDescriptor();
}

bool AGS2MetaEngine::removeSaveState(const char *target, int slot) const {
	return g_system->getSavefileManager()->removeSavefile(getSavegameFile(slot, target));
}

int AGS2MetaEngine::getAutosaveSlot() const {
	if (!g_engine || !_G(noScummAutosave))
		return 0;
	else
		return -1;
}

const Common::AchievementDescriptionList* AGS2MetaEngine::getAchievementDescriptionList() const {
	return AGS::achievementDescriptionList;
}

Common::StringArray AGS2MetaEngine::getGameTranslations(const Common::String &domain) {
	Common::Path path = ConfMan.getPath("path", domain);
	Common::FSDirectory dir(path);
	Common::ArchiveMemberList traFileList;
	dir.listMatchingMembers(traFileList, "*.tra");
	Common::StringArray traFileNames;

	for (Common::ArchiveMemberList::iterator iter = traFileList.begin(); iter != traFileList.end(); ++iter) {
		Common::String traFileName = (*iter)->getName();
		traFileName.erase(traFileName.size() - 4); // remove .tra extension
		traFileNames.push_back(traFileName);
	}

	return traFileNames;
}

#if PLUGIN_ENABLED_DYNAMIC(AGS2)
REGISTER_PLUGIN_DYNAMIC(AGS2, PLUGIN_TYPE_ENGINE, AGS2MetaEngine);
#else
REGISTER_PLUGIN_STATIC(AGS2, PLUGIN_TYPE_ENGINE, AGS2MetaEngine);
#endif
