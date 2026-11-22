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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_YELL_H
#define ULTIMA5_VIEWS_INTERACTIONS_YELL_H

#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * Yelling a word, which outdoors can be a word of power that opens a way into a dungeon, or that
 * is used to meditate at the ruins of a shrine
 */
class Yell : public Interaction {
private:
	enum State {
		STATE_WORD,
		STATE_VIRTUE,
		STATE_MANTRA,
		STATE_DONE
	};

	State _state = STATE_WORD;
	LineInput _line;
	int _shrine = 0;
	int _x = 0;
	int _y = 0;
	int _mantras = 0;
	bool _focused = true;
	bool _passes = true;

	void askMantra();
	void conclude();

public:
	Yell(MessageLog &log);

	bool takesTime() const override {
		return _passes;
	}

	bool keypress(const KeypressMessage &msg) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
