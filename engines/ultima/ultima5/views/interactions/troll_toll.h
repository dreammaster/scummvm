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

#ifndef ULTIMA5_VIEWS_INTERACTIONS_TROLL_TOLL_H
#define ULTIMA5_VIEWS_INTERACTIONS_TROLL_TOLL_H

#include "ultima/ultima5/views/interactions/interaction.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

/**
 * The trolls under a bridge, who the party sneaks past, or else pays a toll to or fights
 */
class TrollToll : public Interaction {
private:
	bool _asking = false;
	int _toll = 0;
	Interaction *_next = nullptr;

	void fight();

public:
	TrollToll(MessageLog &log);
	~TrollToll() override;

	/**
	 * Returns true if it was over as it began
	 */
	bool done() const {
		return !_asking;
	}

	Interaction *takeNext() override {
		Interaction *next = _next;
		_next = nullptr;
		return next;
	}

	bool keypress(const KeypressMessage &msg) override;
};

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
