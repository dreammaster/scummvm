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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_ENDGAME_H
#define ULTIMA5_VIEWS_INTERACTIONS_ENDGAME_H

#include "common/array.h"
#include "ultima/ultima5/logic/endgame_logic.h"
#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * The end of the game: the party stands before Lord British, who asks for the box, and if it
 * is given he takes them home. Then the certificate of the quest is shown
 */
class Endgame : public Interaction {
private:
	enum State {
		STATE_GREETED,
		STATE_BOX,
		STATE_BOX_AGAIN,
		STATE_STORY,
		STATE_REFUSED,
		STATE_CERTIFICATE
	};

	// Something drawn on the map of the room
	struct Sprite {
		byte _tile = 0;
		byte _x = 0;
		byte _y = 0;
	};

	State _state = STATE_GREETED;
	byte _map[11 * 16] = {};
	Sprite _sprites[32];
	int _step = 0;
	Common::Array<EndgameLogic::Line> _certificate;

	void print(int offset);
	void askBox(int offset, State next);
	void answered(bool yes);
	void ending();
	void certificate();

public:
	Endgame(MessageLog &log);

	bool takesTime() const override {
		return false;
	}

	bool keypress(const KeypressMessage &msg) override;
	void draw(View &view) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
