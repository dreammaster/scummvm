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

#ifndef WASTELAND_FOD_VIEWS_GAME_GAME_H
#define WASTELAND_FOD_VIEWS_GAME_GAME_H

#include "wasteland/fod/views/core/base_view.h"
#include "wasteland/fod/logic/game.h"

namespace Wasteland {
namespace FOD {
namespace Views {

class Game : public BaseView, public FOD::Logic::Game {
	enum Mode { MODE_NORMAL };
private:
	Mode _mode;
	Common::String _infoText;

	void setMode(Mode newMode);
	void writeParty();
	void writeTime();
	void writeInfo();
	void drawMap();
	void drawTileAt(Surface &s, int mapX, int mapY, int tileId);

protected:
	void move(Logic::Direction dir);

	/**
	 * Show a message after attempting to leave the edge of a map
	 */
	void showMoveMessage(int msgNum) override;

public:
	Game();
	virtual ~Game() {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	bool msgKeypress(const KeypressMessage &msg) override;
	bool msgMouseDown(const MouseDownMessage &msg) override;
	bool msgAction(const ActionMessage &msg) override;
};

} // namespace Views
} // namespace FOD
} // namespace Wasteland

#endif
