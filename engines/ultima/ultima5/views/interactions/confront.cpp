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

#include "ultima/ultima5/views/interactions/confront.h"
#include "ultima/ultima5/logic/attack_logic.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/views/interactions/combat.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

constexpr int BLACKTHORN_MAP = 0x12;
constexpr int DIALOG_VERMIN = 0xFE;
constexpr byte TILE_GUARD = 0x70;
constexpr byte TILE_FIRST_ENEMY = 0x40;

Confront::Confront(MessageLog &log, int npc, Kind kind) : Interaction(log), _logic(log), _npc(npc) {
	const Data::Savegame &sg = _G(savegame);
	const Data::NpcState state = sg.getNpcState(npc);

	switch (kind) {
	case KIND_ATTACK:
		if (state._dialog == DIALOG_VERMIN) {
			_log.printChar('\n');
			AttackLogic(_log).begone(npc);
		} else if (sg.getActor(state._object)._tile != TILE_GUARD) {
			fight();
		} else {
			arrest();
		}
		break;

	case KIND_TALK:
		if (state._dialog != 0) {
			if (_logic.start(npc))
				talked();
			else
				_state = STATE_TALK;
		}
		break;

	case KIND_ARREST:
		arrest();
		break;
	}
}

Confront::~Confront() {
	delete _next;
}

void Confront::talked() {
	_state = STATE_DONE;

	if (_logic.refused())
		arrest();
}

void Confront::arrest() {
	_state = STATE_DONE;

	if (_G(savegame)._mapId == BLACKTHORN_MAP)
		return;

	_log.print("\n\"Thou art under arrest!\"\n\n\"Wilt thou come quietly?\"\n\n:");
	_state = STATE_ARREST;
}

void Confront::fight() {
	Data::Savegame &sg = _G(savegame);
	const Data::NpcState state = sg.getNpcState(_npc);

	_state = STATE_DONE;

	if (sg.getActor(state._object)._tile < TILE_FIRST_ENEMY) {
		NpcLogic::despawn(sg, _npc);
		return;
	}

	_log.print("\nAttacked!\n");
	sg.setNpcKilled(_npc);
	_next = Combat::forObject(_log, state._object, _npc);
}

bool Confront::keypress(const KeypressMessage &msg) {
	if (_state == STATE_TALK) {
		if (_logic.keypress(msg))
			talked();

		return _state == STATE_DONE;
	}

	if (_state != STATE_ARREST)
		return true;

	const int key = toupper(msg.ascii);
	if (key != 'Y' && key != 'N')
		return false;

	if (key == 'Y') {
		_log.print("Yes\n\nThe guard strikes thee unconscious!\n\nThou dost awaken to...\n");
		AttackLogic::imprison();
		_state = STATE_DONE;
	} else {
		_log.print("No\n\n\"Then defend thyself, rogue!\"\n");
		AttackLogic::callGuards();
		fight();
	}

	return true;
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
