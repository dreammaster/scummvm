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

#ifndef ULTIMA5_LOGIC_CREATION_LOGIC_H
#define ULTIMA5_LOGIC_CREATION_LOGIC_H

#include "ultima/ultima5/data/savegame.h"

namespace Ultima {
namespace Ultima5 {

constexpr int VIRTUE_COUNT = 8;
constexpr int QUESTION_COUNT = 7;

/**
 * A choice between two virtues put to the player by the gypsy
 */
struct GypsyQuestion {
	// The virtues on the left and right cards, the left having the lower number
	int _left = 0;
	int _right = 0;

	// Where the text of the question is in question.dat
	uint _textOffset = 0;
};

/**
 * Works out a new character's attributes from the gypsy's questions. The
 * virtues are paired off in 4 questions, then the 4 that were chosen in 2,
 * and the last 2 in a final one. Each choice adds to the attributes
 */
class CreationLogic {
private:
	bool _picked[VIRTUE_COUNT] = {};
	bool _rejected[VIRTUE_COUNT] = {};
	int _int = 0;
	int _dex = 0;
	int _str = 0;
	int _asked = 0;

	int pickVirtue();

public:
	/**
	 * Starts the questions, with the attributes the character begins with
	 */
	void start(const Data::Character &base);

	/**
	 * Returns true when every question has been answered
	 */
	bool isFinished() const {
		return _asked >= QUESTION_COUNT;
	}

	/**
	 * Picks the virtues for the next question
	 */
	GypsyQuestion nextQuestion();

	/**
	 * Records which of the two cards was chosen
	 */
	void answer(const GypsyQuestion &question, bool leftCard);

	/**
	 * Gives a character the attributes that the answers have led to
	 */
	void finish(Data::Character &character) const;
};

} // namespace Ultima5
} // namespace Ultima

#endif
