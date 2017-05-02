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

#ifndef LEGEND_PROJECT_ITEM_H
#define LEGEND_PROJECT_ITEM_H

#include "common/scummsys.h"
#include "common/str.h"
#include "engines/savestate.h"
#include "graphics/surface.h"
#include "legend/core/file.h"
#include "legend/core/named_item.h"

namespace Legend {

struct LegendSavegameHeader {
	uint8 _version;
	Common::String _saveName;
	Graphics::Surface *_thumbnail;
	int _year, _month, _day;
	int _hour, _minute;
	int _totalFrames;
};


class GameManager;

class ProjectItem : public NamedItem {
	DECLARE_MESSAGE_MAP;
private:
	GameManager *_gameManager;

	/**
	 * Called at the beginning of loading a game
	 */
	void preLoad();

	/**
	 * Does post-loading processing
	 */
	void postLoad();

	/**
	 * Called when a game is about to be saved
	 */
	void preSave();

	/**
	 * Called when a game has finished being saved
	 */
	void postSave();

	/**
	 * Finds the first child instance of a given class type
	 */
	TreeItem *findChildInstance(ClassDef *classDef) const;

	/**
	 * Finds the next sibling occurance of a given class type
	 */
	TreeItem *findSiblingChildInstanceOf(ClassDef *classDef, TreeItem *startItem) const;
private:
	/**
	 * Creates a thumbnail for the current on-screen contents
	 */
	static Graphics::Surface *createThumbnail();
public:
	/**
	 * Load a list of savegames
	 */
	static SaveStateList getSavegameList(const Common::String &target);

	/**
	 * Write out the header information for a savegame
	 */
	static void writeSavegameHeader(SimpleFile *file, LegendSavegameHeader &header);

	/**
	 * Read in the header information for a savegame
	 */
	static bool readSavegameHeader(SimpleFile *file, LegendSavegameHeader &header);
public:
	CLASSDEF;
	ProjectItem();
	virtual ~ProjectItem() { destroyChildren(); }

	/**
	 * Save the data for the class to file
	 */
	virtual void save(SimpleFile *file, int indent);

	/**
	 * Load the data for the class from file
	 */
	virtual void load(SimpleFile *file);

	/**
	 * Get the game manager for the project
	 */
	virtual GameManager *getGameManager() const;

	/**
	 * Sets the game manager for the project, if not already set
	 */
	void setGameManager(GameManager *gameManager);

	/**
	 * Resets the game manager field
	 */
	void resetGameManager();

	/**
	 * Load the entire project data for a given slot Id
	 */
	void loadGame(int slotId);

	/**
	 * Save the entire project data to a given savegame slot
	 */
	void saveGame(int slotId, const Common::String &desc);

	/**
	 * Clear any currently loaded project
	 */
	void clear();
};

} // End of namespace Legend

#endif /* LEGEND_PROJECT_ITEM_H */
