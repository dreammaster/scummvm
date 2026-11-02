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

#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/effect.h"
#include "ultima/ultima6/core/obj.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/party.h"
#include "ultima/ultima6/actors/actor_manager.h"
#include "ultima/ultima6/gui/widgets/map_window.h"
#include "ultima/ultima6/gui/widgets/msg_scroll.h"
#include "ultima/ultima6/misc/u6_llist.h"
#include "ultima/ultima6/script/common/game_scripts.h"
#include "ultima/ultima6/views/view_manager.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {

static const uint8 COMBAT_RANGE_TABLE[64] = {
	0, 1, 2, 3, 4, 5, 6, 7,
	1, 1, 2, 3, 4, 5, 6, 7,
	2, 2, 2, 3, 4, 5, 6, 7,
	3, 3, 3, 4, 5, 6, 7, 7,
	4, 4, 4, 5, 6, 7, 7, 8,
	5, 5, 5, 6, 7, 7, 8, 8,
	6, 6, 6, 7, 7, 8, 8, 8,
	7, 7, 7, 7, 8, 8, 8, 8
};

uint16 Target::x() const {
	return _actor ? _actor->get_x() : _obj->x;
}

uint16 Target::y() const {
	return _actor ? _actor->get_y() : _obj->y;
}

uint8 Target::z() const {
	return _actor ? _actor->get_z() : _obj->z;
}

uint16 Target::objN() const {
	return _actor ? _actor->get_obj_n() : _obj->obj_n;
}

uint16 Target::frameN() const {
	return _actor ? _actor->get_frame_n() : _obj->frame_n;
}

Common::String Target::name() const {
	if (_actor)
		return _actor->get_name();

	return Game::get_game()->get_obj_manager()->get_obj_name(_obj->obj_n, _obj->frame_n);
}

GameScripts::GameScripts(Configuration *config) : _config(config) {
}

void GameScripts::print(const Common::String &str) const {
	MsgScroll *scroll = Game::get_game()->get_scroll();
	if (scroll)
		scroll->display_string(str);
	else
		::debug(1, "%s", str.c_str());
}

void GameScripts::printl(const char *code) const {
	print(_lang.get(code));
}

void GameScripts::printfl(const char *code, ...) const {
	va_list args;
	va_start(args, code);
	print(Common::String::vformat(_lang.get(code), args));
	va_end(args);
}

void GameScripts::wait(uint32 duration) const {
	Game::get_game()->get_map_window()->updateAmbience();
	Game::get_game()->get_view_manager()->update();

	AsyncEffect *e = new AsyncEffect(new TimedEffect(duration));
	e->run(EFFECT_PROCESS_GUI_INPUT);
}

void GameScripts::getContainerObjs(Obj *container, Common::Array<Obj *> &objs) {
	if (!container->container)
		return;

	for (U6Link *link = container->container->start(); link; link = link->next)
		objs.push_back((Obj *)link->data);
}

void GameScripts::getPartyMembers(Common::Array<Actor *> &members) {
	Party *party = Game::get_game()->get_party();
	ActorManager *actorManager = Game::get_game()->get_actor_manager();

	for (uint i = 0; i < party->get_party_size(); ++i)
		members.push_back(actorManager->get_actor(party->get_actor_num(i)));
}

Obj *GameScripts::newObj(uint16 objN) {
	Obj *obj = new Obj();
	obj->obj_n = objN;
	return obj;
}

int GameScripts::findObjValue(const ObjValue *table, uint count, uint16 objN, int defaultValue) {
	for (uint i = 0; i < count; ++i) {
		if (table[i].objN == objN)
			return table[i].value;
	}

	return defaultValue;
}

uint8 GameScripts::getCombatRange(uint16 absX, uint16 absY) const {
	if (absX < 8 && absY < 8)
		return COMBAT_RANGE_TABLE[absX * 8 + absY];

	return 9;
}

uint8 GameScripts::getWeaponRange(uint16 objN) const {
	uint count;
	const ObjValue *table = weaponRanges(count);

	return findObjValue(table, count, objN, 1);
}

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
