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

#ifndef ULTIMA5_VIEWS_GAME_H
#define ULTIMA5_VIEWS_GAME_H

#include "common/ptr.h"
#include "ultima/ultima5/gfx/tileset.h"
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/logic/object_items.h"
#include "ultima/ultima5/logic/visibility_logic.h"
#include "ultima/ultima5/views/dungeon_scene.h"
#include "ultima/ultima5/views/interactions/interaction.h"
#include "ultima/ultima5/views/message_log.h"
#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

/**
 * The main game screen: the map, with the party, the status and the messages
 * around it
 */
class Game : public View {
private:
	MessageLog _messages;
	bool _planTick = false;
	bool _sailingStep = false;

	// Where the sounds of a town have got to: the beat of a clock, the strikes it has left, and the tune of a bard
	int _clockBeat = 0;
	int _clockStrikes = 0;
	int _clockHour = -1;
	int _bardNote = 0;
	bool _quickToggle = false;
	bool _sleeping = false;
	DungeonScene _scene;
	Common::ScopedPtr<Interactions::Interaction> _interaction;

	/**
	 * Starts a command that needs more input than the key that began it
	 */
	void startInteraction(Interactions::Interaction *interaction);

	/**
	 * Lets time go by after a command
	 */
	void endTurn();

	/**
	 * Goes on from an interaction that has finished
	 */
	void interactionDone();

	/**
	 * Makes the sound of what is near the party in a town: a clock, falling water, or a bard
	 */
	void ambientSound();

	/**
	 * Picks the party member that acts when none is asked for, or plays the harpsichord
	 */
	bool digitKey(int digit);

	/**
	 * Moves the party in the map it's in
	 */
	bool walk(Direction direction);

	/**
	 * Carries out the command of a key
	 * @returns		True if the key was a command
	 */
	bool handleCommand(const KeypressMessage &msg);

	/**
	 * Draws what is set in the borders in a dungeon
	 */
	void drawDungeonFrame();

	/**
	 * Lets time go by in a dungeon after a command
	 */
	bool passDungeonTime();

	/**
	 * Deals with what came of the party arriving somewhere in a dungeon
	 */
	void dungeonEvent(const DungeonLogic::Event &event);

	/**
	 * Carries out the command of a key in a dungeon
	 */
	bool handleDungeonCommand(const KeypressMessage &msg);

	/**
	 * Looks into a gem, which shows the map from above
	 */
	void viewGem();

	/**
	 * Casts a spell, and mixes them from reagents
	 */
	void castSpell();
	void mixReagents();

	/**
	 * Starts one of the commands that act on what is next to the party
	 */
	void objectCommand(ObjectItems::Kind kind, const char *prompt);

	/**
	 * Clears the screen and draws the blue frame and the outlines of the windows
	 */
	void drawFrame();

	/**
	 * Draws the names, hit points and condition of the first members of the party
	 */
	void drawParty();

	/**
	 * Draws the food, gold, date and the label between them
	 */
	void drawStatus();

	/**
	 * Draws the wind in the bottom border, or clears it below ground
	 */
	void drawWind();
	void drawMoons();

	/**
	 * Draws the part of the map around the party, with the party and the other
	 * objects on it
	 */
	void drawMap();

	/**
	 * Works out which objects are seen in each tile of the window
	 */
	void placeObjects(byte objects[VIEW_TILES][VIEW_TILES], byte ground[VIEW_TILES][VIEW_TILES]);

public:
	Game();
	~Game() override {}

	/**
	 * Starts a fight with an object on the map that has come to blows with the party
	 */
	void startFight(int object);

	/**
	 * Starts a fight with a kind of enemy
	 * @param kind		The kind of enemy
	 * @param map		The number of the map of the fight
	 * @param lone		Whether a lone enemy is met in places where they come one at a time
	 * @returns			False if the map of the fight couldn't be loaded
	 */
	bool startCombat(int kind, int map, bool lone);

	void draw() override;
	void timeout() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
