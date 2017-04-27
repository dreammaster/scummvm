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

#ifndef LEGEND_GAME_MANAGER_H
#define LEGEND_GAME_MANAGER_H

#include "common/scummsys.h"
#include "audio/mixer.h"
#include "legend/input_handler.h"
#include "legend/input_translator.h"

namespace Legend {

class ProjectItem;

struct GameState {
};

class GameManager {
private:
	int _transitionCtr;
private:
	/**
	 * Generates a message for the next game frame
	 */
	void frameMessage(void *room);
public:
	ProjectItem *_project;
	GameState _gameState;
	InputHandler _inputHandler;
	InputTranslator _inputTranslator;
public:
	GameManager(ProjectItem *project, Audio::Mixer *mixer);
	~GameManager();

	/**
	 * Save data to a save file
	 */
	void save(SimpleFile *file);

	/**
	 * Load data from a save file
	 */
	void load(SimpleFile *file);

	/**
	 * Called when a game is about to be loaded
	 */
	void preLoad();

	/**
	 * Called after loading a game has finished
	 */
	void postLoad(ProjectItem *project);

	/**
	 * Called when a game is about to be saved
	 */
	void preSave(ProjectItem *project);

	/**
	 * Called when a game has finished being saved
	 */
	void postSave();

	/**
	 * Updates the game time when the last disk access started
	 */
	void updateDiskTicksCount();

	/**
	 * Lock the input handler
	 */
	void lockInputHandler() { _inputHandler.incLockCount(); }

	/**
	 * Unlock the input handler
	 */
	void unlockInputHandler() { _inputHandler.decLockCount(); }

	/**
	 * Set default screen bounds
	 */
	void initBounds();

	/**
	 * Main frame update method for the game
	 */
	void update();

	/**
	 * Called when the view changes
	 */
	void viewChange();

	/**
	 * Returns true if no transition is currently in progress
	 */
	bool isntTransitioning() const { return !_transitionCtr; }

	/**
	 * Increments the number of active transitions
	 */
	void incTransitions() { ++_transitionCtr; }

	/**
	 * Decremenst the number of active transitions
	 */
	void decTransitions() { --_transitionCtr; }

	/**
	 * Extends the bounds of the currently affected game display area
	 * to include the passed rect
	 */
	void extendBounds(const Common::Rect &r);

	/**
	 * Return the full Id of the current view in a
	 * room.node.view tuplet form
	 */
	Common::String getFullViewName();
};

} // End of namespace Legend

#endif /* LEGEND_GAME_MANAGER_H */
