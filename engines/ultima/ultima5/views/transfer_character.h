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

#ifndef ULTIMA5_VIEWS_TRANSFER_CHARACTER_H
#define ULTIMA5_VIEWS_TRANSFER_CHARACTER_H

#include "common/str.h"
#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/logic/transfer_logic.h"
#include "ultima/ultima5/views/title_base.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

/**
 * Brings the character of an Ultima IV game over to be the Avatar, converting the name, sex, class,
 * experience and attributes a step at a time
 */
class TransferCharacter : public TitleBase {
private:
	enum Phase {
		PHASE_MISSING,
		PHASE_BAD,
		PHASE_FOUND,
		PHASE_KEEP_NAME,
		PHASE_NEW_NAME,
		PHASE_KEEP_SEX,
		PHASE_CONVERT,
		PHASE_SAVED
	};

	TransferLogic _logic;
	Data::Savegame _newGame;
	Phase _phase = PHASE_MISSING;
	int _step = 0;
	Common::String _name;
	int _cursorFrame = 0;

	void drawTable();
	void drawNote(const Common::String &text);
	void showStep();
	void advance();
	void finish();

public:
	TransferCharacter() : TitleBase("TransferCharacter") {}
	~TransferCharacter() override {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	void timeout() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
