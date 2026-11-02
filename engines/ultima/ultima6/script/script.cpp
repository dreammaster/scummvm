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

#include "backends/keymapper/keymapper.h"
#include "common/system.h"
#include "ultima/ultima6/core/nuvie_defs.h"
#include "ultima/ultima6/gui/widgets/console.h"
#include "ultima/ultima6/script/script.h"
#include "ultima/ultima6/script/script_cutscene.h"
#include "ultima/ultima6/script/common/game_scripts.h"
#include "ultima/ultima6/script/md/md_scripts.h"
#include "ultima/ultima6/script/se/se_scripts.h"
#include "ultima/ultima6/script/u6/u6_scripts.h"

namespace Ultima {
namespace Ultima6 {

Script *Script::script = nullptr;

Script::Script(Configuration *cfg, GUI *gui, SoundManager *sm, nuvie_game_t type) {
	config = cfg;
	gametype = type;
	script = this;
	soundManager = sm;

	if (type == NUVIE_GAME_SE)
		_scripts = new Scripts::SE::SEScripts(cfg);
	else if (type == NUVIE_GAME_MD)
		_scripts = new Scripts::MD::MDScripts(cfg);
	else
		_scripts = new Scripts::U6::U6Scripts(cfg);

	init_cutscene(cfg, gui, sm);
}

Script::~Script() {
	delete _scripts;
}

bool Script::play_intro() {
	return play_cutscene(true);
}

bool Script::play_ending() {
	return play_cutscene(false);
}

bool Script::play_cutscene(bool intro) {
	ConsoleHide();

	// FIXME: For now we disable the keymapper during cutscenes so input works correctly
	// (e.g. for character name entry or skipping the intro)
	g_system->getEventManager()->getKeymapper()->setEnabled(false);
	bool result = intro ? _scripts->playIntro() : _scripts->playEnding();
	g_system->getEventManager()->getKeymapper()->setEnabled(true);

	return result;
}

MovementStatus Script::call_player_before_move_action(sint16 *rel_x, sint16 *rel_y) {
	return _scripts->playerBeforeMoveAction(rel_x, rel_y);
}

bool Script::call_player_post_move_action(bool didMove) {
	return _scripts->playerPostMoveAction(didMove);
}

bool Script::call_player_pass() {
	return _scripts->playerPass();
}

bool Script::call_actor_update_all() {
	return _scripts->actorUpdateAll();
}

bool Script::call_actor_init(Actor *actor, ActorAlignment alignment) {
	return _scripts->actorInit(actor, alignment);
}

bool Script::call_actor_attack(Actor *actor, MapCoord location, Obj *weapon, Actor *foe) {
	return _scripts->actorAttack(actor, location, weapon, foe);
}

bool Script::call_load_game(NuvieIO *objlist) {
	return _scripts->loadGame(objlist);
}

bool Script::call_save_game(NuvieIO *objlist) {
	return _scripts->saveGame(objlist);
}

bool Script::call_actor_map_dmg(Actor *actor, MapCoord location) {
	return _scripts->actorMapDmg(actor, location);
}

bool Script::call_actor_tile_dmg(Actor *actor, uint16 tile_num) {
	return _scripts->actorTileDmg(actor, tile_num);
}

bool Script::call_actor_hit(Actor *actor, uint8 dmg, bool display_hit_msg) {
	return _scripts->actorHit(actor, dmg, display_hit_msg);
}

bool Script::call_look_obj(Obj *obj) {
	return _scripts->lookObj(obj);
}

int Script::call_obj_get_readiable_location(Obj *obj) {
	return _scripts->objGetReadiableLocation(obj);
}

uint8 Script::actor_get_max_magic_points(const Actor *actor) {
	return _scripts->actorGetMaxMagicPoints(actor);
}

bool Script::call_actor_get_obj(Actor *actor, Obj *obj, Obj *container) {
	return _scripts->actorGetObj(actor, obj, container);
}

bool Script::call_actor_subtract_movement_points(Actor *actor, uint8 points) {
	return _scripts->actorSubtractMovementPoints(actor, points);
}

bool Script::call_actor_resurrect(Actor *actor) {
	return _scripts->actorResurrect(actor);
}

uint8 Script::call_actor_str_adj(Actor *actor) {
	return _scripts->actorStrAdj(actor);
}

uint8 Script::call_actor_dex_adj(Actor *actor) {
	return _scripts->actorDexAdj(actor);
}

uint8 Script::call_actor_int_adj(Actor *actor) {
	return _scripts->actorIntAdj(actor);
}

bool Script::call_use_keg(Obj *obj) {
	return _scripts->useKeg(obj);
}

bool Script::call_has_usecode(Obj *obj, UseCodeEvent usecode_type) {
	return _scripts->hasUsecode(obj, usecode_type);
}

ScriptThread *Script::call_use_obj(Obj *obj, Actor *actor) {
	return _scripts->useObj(obj, actor);
}

bool Script::call_ready_obj(Obj *obj, Actor *actor) {
	return _scripts->readyObj(obj, actor);
}

bool Script::call_move_obj(Obj *obj, sint16 rel_x, sint16 rel_y) {
	return _scripts->moveObj(obj, rel_x, rel_y);
}

bool Script::call_handle_alt_code(uint16 altcode) {
	return _scripts->handleAltCode(altcode);
}

ScriptThread *Script::call_magic_cast(const char *invocation) {
	return _scripts->magicCastThread(invocation);
}

bool Script::call_magic_get_spell_list(Spell **spell_list) {
	return _scripts->magicGetSpellList(spell_list);
}

bool Script::call_actor_use_effect(Obj *effect_obj, Actor *actor) {
	return _scripts->actorUseEffect(effect_obj, actor);
}

bool Script::call_can_get_obj_override(Obj *obj) const {
	return _scripts->canGetObjOverride(obj);
}

bool Script::call_out_of_ammo(Actor *attacker, Obj *weapon, bool print_message) {
	return _scripts->outOfAmmo(attacker, weapon, print_message);
}

bool Script::call_is_avatar_dead() {
	return _scripts->isAvatarDead();
}

bool Script::call_is_ranged_select(UseCodeType operation) {
	return _scripts->isRangedSelect(operation);
}

bool Script::call_moonstone_set_loc(uint8 phase, MapCoord location) {
	return _scripts->moonstoneSetLoc(phase, location);
}

MapCoord Script::call_moonstone_get_loc(uint8 phase) {
	return _scripts->moonstoneGetLoc(phase);
}

bool Script::call_update_moongates(bool visible) {
	return _scripts->updateMoongates(visible);
}

bool Script::call_advance_time(uint16 minutes) {
	return _scripts->advanceTime(minutes);
}

bool Script::call_set_g_show_stealing(bool stealing) {
	return _scripts->setGShowStealing(stealing);
}

uint8 Script::call_get_combat_range(uint16 absx, uint16 absy) {
	return _scripts->getCombatRange(absx, absy);
}

uint8 Script::call_get_weapon_range(uint16 obj_n) {
	return _scripts->getWeaponRange(obj_n);
}

uint8 Script::call_play_midgame_sequence(uint16 seq_num) {
	return _scripts->playMidgameSequence(seq_num);
}

bool Script::call_talk_script(uint8 script_number) {
	return _scripts->talkScript(script_number);
}

bool Script::call_talk_to_obj(Obj *obj) {
	return _scripts->talkToObj(obj);
}

bool Script::call_talk_to_actor(Actor *actor) {
	return _scripts->talkToActor(actor);
}

bool Script::call_is_container_obj(uint16 obj_n) {
	return _scripts->isContainerObj(obj_n);
}

uint8 Script::call_get_portrait_number(Actor *actor) {
	return _scripts->getPortraitNumber(actor);
}

ScriptThread *Script::call_player_attack_thread() {
	return _scripts->playerAttackThread();
}

bool Script::call_player_attack() {
	return _scripts->playerAttack();
}

uint16 Script::call_get_tile_to_object_mapping(uint16 tile_n) {
	return _scripts->getTileToObjectMapping(tile_n);
}

bool Script::call_is_tile_object(uint16 obj_n) {
	return _scripts->isTileObject(obj_n);
}

} // End of namespace Ultima6
} // End of namespace Ultima
