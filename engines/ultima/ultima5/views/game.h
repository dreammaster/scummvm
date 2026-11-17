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

#include "ultima/shared/engine/view.h"
#include "ultima/ultima5/gfx/game_frame.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

/**
 * The main game screen: the party, the status and the messages around the map
 */
class Game : public Shared::View {
private:
	Gfx::GameFrame _frame;
	int _cursorFrame = 0;

public:
	Game() : Shared::View("Game") {}
	~Game() override {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	void timeout() override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
