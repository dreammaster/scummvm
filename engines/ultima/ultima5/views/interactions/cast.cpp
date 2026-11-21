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

#include "ultima/ultima5/views/interactions/cast.h"
#include "ultima/ultima5/data/inventory_data.h"
#include "ultima/ultima5/data/spell_data.h"
#include "ultima/ultima5/views/interactions/gem_view.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int KEY_SPACE = ' ';
constexpr int MAX_WORDS = 4;
constexpr int LAST_COLUMN_FOR_WORDS = 13;
constexpr int WINDOW_TOP = 1;
constexpr int FRAME_BOTTOM = 9;
constexpr int GRID_FIRST_ROW = 1;
constexpr int HIGHLIGHT_WIDTH = 15;
constexpr byte GLYPH_CHECK = 0x0F;
constexpr int REAGENT_COUNT = 8;
constexpr int MAX_DIGITS = 2;

SpellEntry::Result SpellEntry::handleKey(const KeypressMessage &msg, MessageLog &log) {
	const int ch = toupper(msg.ascii);

	if (ch >= 'A' && ch <= 'Z') {
		// There are no words for J and O
		if (ch != 'J' && ch != 'O' && _count < MAX_WORDS) {
			const char *word = Data::SPELL_SYLLABLES[ch - 'A'];
			const int length = strlen(word);

			if (log.column() + length > LAST_COLUMN_FOR_WORDS)
				log.printChar('\n');

			log.print(word);
			log.printChar(' ');

			_letters[_count] = ch;
			_width[_count++] = length + 1;
		}

		return PENDING;
	}

	switch (msg.keycode) {
	case Common::KEYCODE_BACKSPACE:
		if (_count != 0)
			log.backspace(_width[--_count]);
		return PENDING;

	case Common::KEYCODE_ESCAPE:
		_count = 0;
		break;

	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
	case Common::KEYCODE_SPACE:
		break;

	default:
		return PENDING;
	}

	if (_count != 0)
		log.printChar('\n');

	return DONE;
}

int SpellEntry::spell() const {
	return SpellLogic::identify(_letters, _count);
}

Cast::Cast(MessageLog &log) : Interaction(log), _logic(log) {
	switch (_actor.begin(_log)) {
	case ActingMemberChooser::PENDING:
		_state = STATE_WHO;
		break;

	case ActingMemberChooser::CHOSEN:
		askSpell();
		break;

	default:
		_state = STATE_DONE;
		break;
	}
}

Cast::~Cast() {
	delete _next;
}

void Cast::askSpell() {
	_log.print("Spell name:\n:");
	_state = STATE_ENTRY;
}

bool Cast::apply(SpellLogic::Need need) {
	switch (need) {
	case SpellLogic::NEED_DIRECTION:
		_state = STATE_DIRECTION;
		return false;

	case SpellLogic::NEED_MEMBER:
		_members.reset();
		_state = STATE_MEMBER;
		return false;

	case SpellLogic::NEED_KEY:
		_state = STATE_KEY;
		return false;

	case SpellLogic::NEED_VIEW:
		_next = new GemView(_log);
		break;

	default:
		break;
	}

	_state = STATE_DONE;
	return true;
}

bool Cast::keypress(const KeypressMessage &msg) {
	switch (_state) {
	case STATE_WHO:
		if (_actor.handleKey(msg, _log) == ActingMemberChooser::PENDING)
			return false;

		if (_actor.who() == -1) {
			_state = STATE_DONE;
			return true;
		}

		askSpell();
		return false;

	case STATE_ENTRY: {
		if (_entry.handleKey(msg, _log) == SpellEntry::PENDING)
			return false;

		const int spell = _entry.spell();

		if (spell == SpellLogic::SPELL_NONE) {
			_log.print("None!\n");
			_state = STATE_DONE;
			return true;
		}

		if (spell == SpellLogic::SPELL_UNKNOWN) {
			_log.print("No effect!\n");
			_state = STATE_DONE;
			return true;
		}

		return apply(_logic.cast(_actor.who(), spell));
	}

	case STATE_DIRECTION: {
		const int key = originalKey(msg);

		if ((key >= 1 && key <= 4) || key == KEY_SPACE)
			return apply(_logic.directionChosen(key == KEY_SPACE ? 0 : key));

		return false;
	}

	case STATE_MEMBER: {
		const PartyMemberChooser::Result result = _members.handleKey(msg);
		if (result == PartyMemberChooser::PENDING)
			return false;

		return apply(_logic.memberChosen(result == PartyMemberChooser::CHOSEN ? _members.index() : -1));
	}

	case STATE_KEY:
		return apply(_logic.keyChosen(msg.ascii));

	default:
		return true;
	}
}

void Cast::draw(View &view) {
	_actor.draw(view);

	if (_state == STATE_MEMBER)
		_members.draw(view);
}

Mix::Mix(MessageLog &log) : Interaction(log), _logic(log) {
	const Data::Savegame &sg = _G(savegame);
	int total = 0;

	for (int i = 0; i < REAGENT_COUNT; ++i)
		total += sg._reagents[i];

	if (total == 0) {
		_log.print("No reagents owned!\n");
		return;
	}

	_log.print("For what spell?\n:");
	_state = STATE_ENTRY;
}

void Mix::showReagents() {
	const Data::Savegame &sg = _G(savegame);

	_ownedCount = 0;
	for (int i = 0; i < REAGENT_COUNT; ++i) {
		if (sg._reagents[i] != 0)
			_owned[_ownedCount++] = i;
	}

	_row = 0;
	_picked = 0;
	drawGrid();
}

// Prints the window of the reagents, with the one that's on and the ones that are picked
void Mix::drawGrid() {
	const Data::Savegame &sg = _G(savegame);

	_grid.clear();
	_grid.drawFrame(FRAME_BOTTOM);

	for (int i = 0; i < _ownedCount; ++i) {
		const int reagent = _owned[i];
		_grid.setPosition(1, GRID_FIRST_ROW + i);

		if (i == _row)
			_grid.toggleInverse();

		_grid.printChar(' ');
		_grid.printNumber(sg._reagents[reagent], 2, '0');

		if ((_picked & (0x80 >> reagent)) != 0) {
			_grid.setRune(true);
			_grid.printChar(GLYPH_CHECK);
			_grid.setRune(false);
		} else {
			_grid.printChar(' ');
		}

		_grid.print(Data::REAGENT_LIST_NAMES[reagent]);

		if (i == _row) {
			for (int pad = _grid.column(); pad < HIGHLIGHT_WIDTH; ++pad)
				_grid.printChar(' ');
			_grid.toggleInverse();
		}
	}
}

void Mix::askAmount() {
	_log.print("How much? ");
	_digits = 0;
	_amount = 0;
	_state = STATE_AMOUNT;
}

bool Mix::keypress(const KeypressMessage &msg) {
	switch (_state) {
	case STATE_ENTRY:
		if (_entry.handleKey(msg, _log) == SpellEntry::PENDING)
			return false;

		_spell = _entry.spell();
		if (_spell == SpellLogic::SPELL_NONE) {
			_log.print("\nNone!\n");
			_state = STATE_DONE;
			return true;
		}

		_log.printChar('\n');
		_log.printChar(0x1B);
		_log.printChar(',');
		_log.printChar(0x1A);
		_log.printChar(',');
		_log.printChar(0x18);
		_log.printChar(',');
		_log.printChar(0x19);
		_log.print(" to move,\nRETURN selects.\nType M to mix:");

		showReagents();
		_state = STATE_REAGENTS;
		return false;

	case STATE_REAGENTS:
		switch (msg.keycode) {
		case Common::KEYCODE_LEFT:
		case Common::KEYCODE_UP:
			if (_row > 0)
				--_row;
			break;

		case Common::KEYCODE_RIGHT:
		case Common::KEYCODE_DOWN:
			if (_row < _ownedCount - 1)
				++_row;
			break;

		case Common::KEYCODE_RETURN:
		case Common::KEYCODE_KP_ENTER:
		case Common::KEYCODE_SPACE:
			_picked ^= 0x80 >> _owned[_row];
			break;

		case Common::KEYCODE_ESCAPE:
			_log.printChar('\n');
			_state = STATE_DONE;
			return true;

		default:
			if (toupper(msg.ascii) == 'M') {
				_log.print("\n\n");
				askAmount();
				return false;
			}
			break;
		}

		drawGrid();
		return false;

	case STATE_AMOUNT:
		if (msg.keycode == Common::KEYCODE_BACKSPACE) {
			if (_digits > 0) {
				--_digits;
				_amount /= 10;
				_log.backspace(1);
			}
			return false;
		}

		if (msg.ascii >= '0' && msg.ascii <= '9') {
			if (_digits < MAX_DIGITS) {
				++_digits;
				_amount = _amount * 10 + (msg.ascii - '0');
				_log.printChar(msg.ascii);
			}
			return false;
		}

		if (msg.keycode == Common::KEYCODE_ESCAPE) {
			_amount = 0;
		} else if (msg.keycode != Common::KEYCODE_RETURN && msg.keycode != Common::KEYCODE_KP_ENTER &&
				msg.keycode != Common::KEYCODE_SPACE) {
			return false;
		}

		_log.printChar('\n');

		if (_amount != 0 && !SpellLogic::hasReagents(_picked, _amount)) {
			_log.print("Insufficient reagents!\n\n");
			askAmount();
			return false;
		}

		if (_amount > 0) {
			if (_picked != 0)
				_logic.mixSpell(_spell, _picked, _amount);
			else
				_log.print("\nNothing to mix!\n");
		}

		_state = STATE_DONE;
		return true;

	default:
		return true;
	}
}

void Mix::draw(View &view) {
	if (_state == STATE_REAGENTS || _state == STATE_AMOUNT) {
		view.drawLabel("Reagents:");
		view.drawTextGrid(_grid, WINDOW_TOP);
	}
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
