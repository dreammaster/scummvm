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

#include "common/file.h"
#include "common/savefile.h"
#include "common/str-array.h"
#include "common/system.h"
#include "graphics/scaler.h"
#include "graphics/thumbnail.h"
#include "legend/game_manager.h"
#include "legend/legend.h"
#include "legend/core/project_item.h"

namespace Legend {

#define CURRENT_SAVEGAME_VERSION 1
#define MAX_SAVEGAME_SLOTS 99
#define MINIMUM_SAVEGAME_VERSION 1

static const char *const SAVEGAME_STR = "LGND";
#define SAVEGAME_STR_SIZE 4

EMPTY_MESSAGE_MAP(ProjectItem, NamedItem);

ProjectItem::ProjectItem() : _gameManager(nullptr) {
}

void ProjectItem::save(SimpleFile *file, int indent) {
	TreeItem::save(file, indent);
}

void ProjectItem::load(SimpleFile *file) {
	TreeItem::load(file);
}

GameManager *ProjectItem::getGameManager() const {
	return _gameManager;
}

Events *ProjectItem::getEvents() const {
	return g_vm->_events;
}

void ProjectItem::setGameManager(GameManager *gameManager) {
	if (!_gameManager)
		_gameManager = gameManager;
}

void ProjectItem::resetGameManager() {
	_gameManager = nullptr;
}

void ProjectItem::loadGame(int slotId) {

}

void ProjectItem::saveGame(int slotId, const String &desc) {

}

void ProjectItem::clear() {
	TreeItem *item;
	while ((item = getFirstChild()) != nullptr)
		item->destroyAll();
}

void ProjectItem::preLoad() {

}

void ProjectItem::postLoad() {

}

void ProjectItem::preSave() {

}

void ProjectItem::postSave() {

}

SaveStateList ProjectItem::getSavegameList(const String &target) {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringArray filenames;
	Common::String saveDesc;
	Common::String pattern = String::format("%s.0??", target.c_str());
	LegendSavegameHeader header;

	filenames = saveFileMan->listSavefiles(pattern);
	sort(filenames.begin(), filenames.end());   // Sort to get the files in numerical order

	SaveStateList saveList;
	for (Common::StringArray::const_iterator file = filenames.begin(); file != filenames.end(); ++file) {
		const char *ext = strrchr(file->c_str(), '.');
		int slot = ext ? atoi(ext + 1) : -1;

		if (slot >= 0 && slot < MAX_SAVEGAME_SLOTS) {
			Common::InSaveFile *in = g_system->getSavefileManager()->openForLoading(*file);

			if (in) {
				SimpleFile f;
				f.open(in);
				if (!readSavegameHeader(&f, header))
					continue;

				saveList.push_back(SaveStateDescriptor(slot, header._saveName));

				header._thumbnail->free();
				delete header._thumbnail;
				delete in;
			}
		}
	}

	return saveList;
}

bool ProjectItem::readSavegameHeader(SimpleFile *file, LegendSavegameHeader &header) {
	char saveIdentBuffer[SAVEGAME_STR_SIZE + 1];
	header._thumbnail = nullptr;

	// Validate the header Id
	file->unsafeRead(saveIdentBuffer, SAVEGAME_STR_SIZE + 1);
	if (strncmp(saveIdentBuffer, SAVEGAME_STR, SAVEGAME_STR_SIZE)) {
		file->seek(-SAVEGAME_STR_SIZE, SEEK_CUR);
		header._saveName = "Unnamed";
		return true;
	}

	header._version = file->readByte();
	if (header._version < MINIMUM_SAVEGAME_VERSION || header._version > CURRENT_SAVEGAME_VERSION)
		return false;

	// Read in the string
	header._saveName.clear();
	char ch;
	while ((ch = (char)file->readByte()) != '\0') header._saveName += ch;

	// Get the thumbnail
	header._thumbnail = Graphics::loadThumbnail(*file);
	if (!header._thumbnail)
		return false;

	// Read in save date/time
	header._year = file->readUint16LE();
	header._month = file->readUint16LE();
	header._day = file->readUint16LE();
	header._hour = file->readUint16LE();
	header._minute = file->readUint16LE();
	header._totalFrames = file->readUint32LE();

	return true;
}

void ProjectItem::writeSavegameHeader(SimpleFile *file, LegendSavegameHeader &header) {
	// Write out a savegame header
	file->write(SAVEGAME_STR, SAVEGAME_STR_SIZE + 1);

	file->writeByte(CURRENT_SAVEGAME_VERSION);

	// Write savegame name
	file->write(header._saveName.c_str(), header._saveName.size());
	file->writeByte('\0');

	// Create a thumbnail of the screen and save it out
	Graphics::Surface *thumb = createThumbnail();
	Graphics::saveThumbnail(*file, *thumb);
	thumb->free();
	delete thumb;

	// Write out the save date/time
	TimeDate td;
	g_system->getTimeAndDate(td);
	file->writeUint16LE(td.tm_year + 1900);
	file->writeUint16LE(td.tm_mon + 1);
	file->writeUint16LE(td.tm_mday);
	file->writeUint16LE(td.tm_hour);
	file->writeUint16LE(td.tm_min);
	file->writeUint32LE(g_vm->_events->getFrameCounter());
}

Graphics::Surface *ProjectItem::createThumbnail() {
	Graphics::Surface *thumb = new Graphics::Surface();

	::createThumbnailFromScreen(thumb);
	return thumb;
}

} // End of namespace Legend
