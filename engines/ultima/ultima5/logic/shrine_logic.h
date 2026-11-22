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

#ifndef ULTIMA5_LOGIC_SHRINE_LOGIC_H
#define ULTIMA5_LOGIC_SHRINE_LOGIC_H

#include "common/str.h"
#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * What the shrines of the virtues and the Codex do with the party's quests, gold and karma
 */
class ShrineLogic {
public:
	// What the party is to be told by a shrine that its meditation was right
	enum Stage {
		STAGE_ORDAIN,
		STAGE_OFFER,
		STAGE_REWARD
	};

	/**
	 * Gives the words that the shrines and the Codex say, which the game keeps in a file
	 * @param offset	Where it is, from where the messages start
	 */
	static Common::String message(int offset);

	/**
	 * Looks for the words of a virtue or a mantra in what was typed, which is how the game
	 * does it, and gives where it is or -1
	 */
	static int find(const char *needle, const char *typed);

	/**
	 * Gives which shrine is at a place, or the last shrine if there's none
	 */
	static int shrineAt(int x, int y);

	/**
	 * Tells what the shrine does when it is meditated at
	 */
	static Stage stage(int shrine);

	/**
	 * Gives the quest of the shrine to the party
	 */
	static void ordain(int shrine);

	/**
	 * Takes gold from the party as an offering, and gives karma for it
	 * @returns		False if the party hasn't that much
	 */
	static bool offer(int shrine, int hundreds);

	/**
	 * Rewards the party for a quest that is done
	 */
	static void reward(int shrine, Views::MessageLog &log);

	// What yelling a word of power came to
	enum Spoken {
		SPOKEN_NOTHING,
		SPOKEN_EFFECT,
		SPOKEN_RUINS
	};

	/**
	 * Says what a word that was yelled outdoors does, next to a dungeon's way in or to the
	 * ruins of a shrine. If it is at ruins, those are given
	 */
	static Spoken speak(const char *typed, Views::MessageLog &log, int &shrine, int &x, int &y);

	/**
	 * Says a word in the castles of the shadowlords, which may call one of them up
	 * @returns		True if one appeared
	 */
	static bool callShadowlord(const char *typed, Views::MessageLog &log);

	/**
	 * Raises the shrine at its ruins again
	 */
	static void restore(int shrine, int x, int y);

	/**
	 * Gives how many of the party that were left behind are in urns
	 */
	static int urnCount();

	/**
	 * Gives the names of those left behind, in capitals
	 */
	static Common::String urnNames();

	/**
	 * Tells which of the quests that are ordained has been learned of in the Codex first
	 * @returns		Which, or -1 if none is
	 */
	static int codexVirtue();
};

} // namespace Ultima5
} // namespace Ultima

#endif
