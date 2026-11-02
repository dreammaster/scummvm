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

#include "ultima/ultima6/actors/actor_manager.h"
#include "ultima/ultima6/core/effect.h"
#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/game_clock.h"
#include "ultima/ultima6/core/magic.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/party.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/core/weather.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/u6/u6_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

namespace {

// What a spell asks the player for before it takes effect
enum Select {
	SEL_NONE,
	SEL_LOCATION,          // select_location_with_prompt: the spell does the rest
	SEL_LOCATION_PROJ,     // select_location_with_projectile
	SEL_ACTOR,             // select_actor
	SEL_ACTOR_PROJ,        // select_actor_with_projectile
	SEL_OBJ,               // select_obj
	SEL_OBJ_PROJ,          // select_obj_with_projectile
	SEL_ACTOR_OR_OBJ,      // select_actor_or_obj
	SEL_ACTOR_OR_OBJ_PROJ  // select_actor_or_obj_with_projectile
};

struct SpellInfo {
	const char *_name;
	const char *_invocation;
	uint8 _reagents;
	uint8 _circle;
	uint8 _num;
	Select _select;
	uint16 _tile;

	uint8 spellNum() const {
		return (_circle - 1) * 16 + (_num - 1);
	}
};

const SpellInfo SPELLS[] = {
	{ "Create Food", "imy", 0x61, 1, 1, SEL_NONE, 0 },
	{ "Detect Magic", "wo", 0x82, 1, 2, SEL_OBJ, 0 },
	{ "Detect Trap", "wj", 0x82, 1, 3, SEL_OBJ, 0 },
	{ "Dispel Magic", "ajo", 0x60, 1, 4, SEL_ACTOR_PROJ, 0x17f },
	{ "Douse", "af", 0x24, 1, 5, SEL_OBJ_PROJ, 0x18b },
	{ "Harm", "am", 0x12, 1, 6, SEL_ACTOR_PROJ, 0x17f },
	{ "Heal", "im", 0x50, 1, 7, SEL_ACTOR_PROJ, 0x17f },
	{ "Help", "kl", 0x00, 1, 8, SEL_NONE, 0 },
	{ "Ignite", "if", 0x84, 1, 9, SEL_OBJ_PROJ, 0x18d },
	{ "Light", "il", 0x80, 1, 10, SEL_NONE, 0 },

	{ "Infravision", "ql", 0x82, 2, 1, SEL_NONE, 0 },
	{ "Magic Arrow", "oj", 0x84, 2, 2, SEL_ACTOR_PROJ, 0x236 },
	{ "Poison", "inp", 0x0e, 2, 3, SEL_ACTOR_PROJ, 0x17c },
	{ "Reappear", "iy", 0x1c, 2, 4, SEL_LOCATION, 0 },
	{ "Sleep", "iz", 0x16, 2, 5, SEL_ACTOR_PROJ, 0x17d },
	{ "Telekinesis", "opy", 0x0d, 2, 6, SEL_ACTOR_OR_OBJ_PROJ, 0x17f },
	{ "Trap", "ij", 0x12, 2, 7, SEL_OBJ, 0 },
	{ "Unlock Magic", "ep", 0x88, 2, 8, SEL_OBJ, 0 },
	{ "Untrap", "aj", 0x88, 2, 9, SEL_OBJ, 0 },
	{ "Vanish", "ay", 0x2c, 2, 10, SEL_OBJ_PROJ, 0x17f },

	{ "Curse", "as", 0xa2, 3, 1, SEL_ACTOR_PROJ, 0x17f },
	{ "Dispel Field", "ag", 0x84, 3, 2, SEL_OBJ_PROJ, 0x17f },
	{ "Fireball", "pf", 0x84, 3, 3, SEL_ACTOR_PROJ, 0x17e },
	{ "Great Light", "vl", 0x81, 3, 4, SEL_NONE, 0 },
	{ "Lock", "ap", 0xa8, 3, 5, SEL_OBJ, 0 },
	{ "Mass Awaken", "avz", 0x60, 3, 6, SEL_LOCATION_PROJ, 0x18b },
	{ "Mass Sleep", "vz", 0x52, 3, 7, SEL_LOCATION_PROJ, 0x18b },
	{ "Peer", "vwy", 0x03, 3, 8, SEL_NONE, 0 },
	{ "Protection", "is", 0xe0, 3, 9, SEL_ACTOR_PROJ, 0x17f },
	{ "Repel Undead", "axc", 0xa0, 3, 10, SEL_NONE, 0 },

	{ "Animate", "oy", 0x89, 4, 1, SEL_OBJ_PROJ, 0x17f },
	{ "Conjure", "kx", 0x11, 4, 2, SEL_NONE, 0 },
	{ "Disable", "avm", 0x13, 4, 3, SEL_ACTOR, 0 },
	{ "Fire Field", "ifg", 0x94, 4, 4, SEL_LOCATION_PROJ, 0x17e },
	{ "Great Heal", "vm", 0x51, 4, 5, SEL_ACTOR_PROJ, 0x17f },
	{ "Locate", "iw", 0x02, 4, 6, SEL_NONE, 0 },
	{ "Mass Dispel", "vajo", 0x60, 4, 7, SEL_LOCATION_PROJ, 0x18b },
	{ "Poison Field", "ing", 0x16, 4, 8, SEL_LOCATION_PROJ, 0x17c },
	{ "Sleep Field", "izg", 0x54, 4, 9, SEL_LOCATION_PROJ, 0x17d },
	{ "Wind Change", "rh", 0x88, 4, 10, SEL_NONE, 0 },

	{ "Energy Field", "isg", 0x15, 5, 1, SEL_LOCATION_PROJ, 0x17f },
	{ "Explosion", "vpf", 0x8d, 5, 2, SEL_LOCATION_PROJ, 0x189 },
	{ "Insect Swarm", "kbx", 0x98, 5, 3, SEL_NONE, 0 },
	{ "Invisibility", "sl", 0x0a, 5, 4, SEL_ACTOR_PROJ, 0x18b },
	{ "Lightning", "og", 0x85, 5, 5, SEL_ACTOR_PROJ, 0x188 },
	{ "Paralyze", "axp", 0x96, 5, 6, SEL_ACTOR_PROJ, 0x17f },
	{ "Pickpocket", "py", 0x1a, 5, 7, SEL_ACTOR_PROJ, 0x18b },
	{ "Reveal", "asl", 0x13, 5, 8, SEL_NONE, 0 },
	{ "Seance", "kmc", 0x9b, 5, 9, SEL_ACTOR_OR_OBJ, 0 },
	{ "X-ray", "wy", 0x81, 5, 10, SEL_NONE, 0 },

	{ "Charm", "axe", 0x16, 6, 1, SEL_ACTOR_PROJ, 0x17f },
	{ "Clone", "iqx", 0xdb, 6, 2, SEL_ACTOR_PROJ, 0x17f },
	{ "Confuse", "vq", 0x03, 6, 3, SEL_NONE, 0 },
	{ "Flame Wind", "fh", 0x89, 6, 4, SEL_LOCATION, 0 },
	{ "Hail Storm", "kdy", 0x0d, 6, 5, SEL_LOCATION, 0 },
	{ "Mass Protect", "vis", 0xe1, 6, 6, SEL_LOCATION_PROJ, 0x18b },
	{ "Negate Magic", "ao", 0xa1, 6, 7, SEL_NONE, 0 },
	{ "Poison Wind", "nh", 0x8a, 6, 8, SEL_LOCATION, 0 },
	{ "Replicate", "iqy", 0xda, 6, 9, SEL_OBJ_PROJ, 0x17f },
	{ "Web", "idp", 0x10, 6, 10, SEL_LOCATION, 0 },

	{ "Chain Bolt", "vog", 0x8d, 7, 1, SEL_ACTOR_PROJ, 0x188 },
	{ "Enchant", "ioy", 0x91, 7, 2, SEL_OBJ, 0 },
	{ "Energy Wind", "gh", 0x8b, 7, 3, SEL_LOCATION, 0 },
	{ "Fear", "qc", 0x23, 7, 4, SEL_NONE, 0 },
	{ "Gate Travel", "vrp", 0x85, 7, 5, SEL_NONE, 0 },
	{ "Kill", "ic", 0x86, 7, 6, SEL_ACTOR_PROJ, 0x18c },
	{ "Mass Curse", "vas", 0xa3, 7, 7, SEL_LOCATION_PROJ, 0x18b },
	{ "Mass Invis", "vsl", 0x0f, 7, 8, SEL_LOCATION_PROJ, 0x188 },
	{ "Wing Strike", "kox", 0x99, 7, 9, SEL_NONE, 0 },
	{ "Wizard Eye", "pow", 0x9f, 7, 10, SEL_NONE, 0 },

	{ "Armageddon", "vcbm", 0x00, 8, 1, SEL_NONE, 0 },
	{ "Death Wind", "ch", 0x8b, 8, 2, SEL_LOCATION, 0 },
	{ "Eclipse", "val", 0xab, 8, 3, SEL_NONE, 0 },
	{ "Mass Charm", "vaxe", 0x17, 8, 4, SEL_LOCATION_PROJ, 0x188 },
	{ "Mass Kill", "vc", 0x87, 8, 5, SEL_LOCATION_PROJ, 0x188 },
	{ "Resurrect", "imc", 0xf9, 8, 6, SEL_OBJ, 0 },
	{ "Slime", "vrx", 0x0b, 8, 7, SEL_NONE, 0 },
	{ "Summon", "kxc", 0x39, 8, 8, SEL_NONE, 0 },
	{ "Time Stop", "at", 0x29, 8, 9, SEL_NONE, 0 },
	{ "Tremor", "vpy", 0x89, 8, 10, SEL_NONE, 0 },

	{ "Monster Invisible", "", 0x00, 8, 17, SEL_NONE, 0 }
};

const SpellInfo *findSpell(uint8 spellNum) {
	for (uint i = 0; i < ARRAYSIZE(SPELLS); ++i) {
		if (SPELLS[i].spellNum() == spellNum)
			return &SPELLS[i];
	}

	return nullptr;
}

// Tiles of magical objects, and the potion names for detect magic
const uint16 MAGICAL_TILES[] = {
	0x207, 0x20F, 0x216, 0x22F, 0x232, 0x235, 0x238, 0x239, 0x23D, 0x248, 0x249, 0x24A, 0x24B, 0x24C, 0x24D,
	0x24E, 0x24F, 0x251, 0x252, 0x253, 0x255, 0x256, 0x257, 0x25A, 0x25B, 0x25C, 0x25D, 0x25F, 0x26E, 0x2B6,
	0x31E, 0x31F, 0x326, 0x368, 0x369, 0x36A, 0x36B, 0x36C, 0x36D, 0x36E, 0x36F, 0x37D, 0x37E, 0x37F, 0x3A8,
	0x3A9, 0x3AA, 0x3AB, 0x3AC, 0x3AD, 0x3AE, 0x3AF, 0x3FD, 0x3FE, 0x40C, 0x40D, 0x40E, 0x40F, 0x41C, 0x41D,
	0x41E, 0x41F, 0x42C, 0x42D, 0x42E, 0x42F, 0x43C, 0x43D, 0x43E, 0x43F, 0x484, 0x485, 0x48E, 0x48F, 0x6D0,
	0x6D1, 0x6D2, 0x6D3
};

const char *const POTION_NAMES[9] = {
	"an awaken", "a cure", "a heal", "a poison", "a sleep", "a protection", "an invisibility", "an xray vision",
	"an unknown"
};

const char *const SYLLABLES[26] = {
	"An", "Bet", "Corp", "Des", "Ex", "Flam", "Grav", "Hur", "In", "Jux", "Kal", "Lor", "Mani", "Nox", "Ort", "Por",
	"Quas", "Rel", "Sanct", "Tym", "Uus", "Vas", "Wis", "Xen", "Ylem", "Zu"
};

enum {
	OBJ_CHARGE = 336,
	OBJ_EFFECT = 337,
	OBJ_CHEST = 0x62,
	OBJ_STAFF = 0x4e
};

bool isDoor(uint16 objN) {
	return objN >= 297 && objN <= 300;
}

} // End of anonymous namespace

/**
 * Casts a spell. Spells that need the player to pick something wait for it and carry on once it arrives.
 * A caster of null is the player, and a monster is given the target it is attacking.
 */
class U6Scripts::MagicThread : public Continuation {
private:
	enum Step {
		STEP_START = 0,
		STEP_LOCATION,
		STEP_OBJ,
		STEP_DIRECTION,
		STEP_SPELL,
		STEP_INV_OBJ,
		STEP_WIND_DIRECTION
	};

	/** What the player picked for a spell */
	struct Selection {
		MapCoord _loc;
		Actor *_actor = nullptr;
		Obj *_obj = nullptr;
	};

	U6Scripts *_scripts;
	const SpellInfo *_info;
	uint8 _spell;
	Actor *_caster;
	bool _hasTarget;
	MapCoord _target;
	Selection _sel;

	Actor *caster() const {
		return _caster ? _caster : Game::get_game()->get_player()->get_actor();
	}
	bool casterIsPlayer() const {
		return _caster == nullptr;
	}

	void print(const Common::String &text) const {
		_scripts->print(text);
	}

	void success() {
		if (casterIsPlayer()) {
			print("\nSuccess\n");
			_scripts->playSfx(SFX_SUCCESS);
		}
	}
	void noEffect() {
		if (casterIsPlayer()) {
			print("\nNo effect\n");
			_scripts->playSfx(SFX_FAILURE);
		}
	}
	void blocked() {
		if (casterIsPlayer()) {
			print("\nBlocked!\n");
			_scripts->playSfx(SFX_FAILURE);
		}
	}
	void failed() {
		if (casterIsPlayer()) {
			print("\nFailed\n");
			_scripts->playSfx(SFX_FAILURE);
		}
	}
	void notPossible() {
		if (casterIsPlayer()) {
			print("\nNot possible\n");
			_scripts->playSfx(SFX_FAILURE);
		}
	}

	void castingEffect() {
		int level = _spell / 0x10;

		_scripts->playSfx(SFX_CASTING_MAGIC_P1 + level, true);
		_scripts->playSfx(SFX_CASTING_MAGIC_P2 + level);

		AsyncEffect *effect = new AsyncEffect(new XorEffect(1700));
		effect->run();
	}

	void castingFadeEffect() {
		_scripts->fadeCasting(caster());
	}

	bool outOfRange(int x, int y) {
		if (_scripts->actorGetCombatRange(caster(), x, y) > 7) {
			print("\nout of range\n");
			return true;
		}

		return false;
	}

	bool lineBlocked(const MapCoord &loc, uint16 tile, int speed) {
		Actor *c = caster();
		int hitX, hitY, preX, preY;
		_scripts->mapLineHitCheck(c->get_x(), c->get_y(), loc.x, loc.y, loc.z, hitX, hitY, preX, preY);
		_scripts->projectile(tile, c->get_x(), c->get_y(), hitX, hitY, speed);

		return hitX != loc.x || hitY != loc.y;
	}

	Actor *actorAt(const MapCoord &loc) const {
		return mapGetActor(loc.x, loc.y, loc.z);
	}
	Obj *objAt(const MapCoord &loc) const {
		return mapGetObj(loc.x, loc.y, loc.z);
	}

	void printName(const Common::String &name) {
		print(name + "\n");
	}

	/** Finishes a spell, killing the avatar if that's what the spell did */
	uint8 finish() {
		if (_scripts->_avatarDied)
			_scripts->actorAvatarDeath();

		return NUVIE_SCRIPT_FINISHED;
	}

	uint8 begin() {
		switch (_info->_select) {
		case SEL_NONE:
			return cast();

		case SEL_OBJ:
			if (_hasTarget) {
				_sel._loc = _target;
				_sel._obj = objAt(_target);
				return selectedObj();
			}

			print("On what: ");
			return wait(NUVIE_SCRIPT_GET_OBJ, STEP_OBJ);

		default:
			break;
		}

		if (_hasTarget) {
			_location = _target;
			_resumedWithNil = false;
			return selectedLocation();
		}

		switch (_info->_select) {
		case SEL_ACTOR:
			print("On whom: ");
			break;
		case SEL_ACTOR_OR_OBJ:
		case SEL_ACTOR_PROJ:
			print("On Whom: ");
			break;
		case SEL_ACTOR_OR_OBJ_PROJ:
			print("On what: ");
			break;
		case SEL_OBJ_PROJ:
			print("On What: ");
			break;
		default:
			print("Location: ");
			break;
		}

		return wait(NUVIE_SCRIPT_GET_TARGET, STEP_LOCATION);
	}

	/** select_obj, once the object is known */
	uint8 selectedObj() {
		Obj *obj = _sel._obj;

		if (!_hasTarget) {
			if (!obj) {
				print("nothing\n");
			} else {
				printName(Game::get_game()->get_obj_manager()->get_obj_name(obj));
				if (obj->is_on_map() && outOfRange(obj->x, obj->y)) {
					_sel._obj = nullptr;
				}
			}
		}

		return cast();
	}

	/** Works out what was selected from the location, as the select_* functions did */
	uint8 selectedLocation() {
		bool haveLoc = !_resumedWithNil;
		MapCoord loc = _location;
		Actor *c = caster();
		bool isPlayer = casterIsPlayer();
		Selection &sel = _sel;
		sel = Selection();
		sel._loc = loc;

		switch (_info->_select) {
		case SEL_LOCATION:
			return cast();

		case SEL_LOCATION_PROJ:
			castingFadeEffect();
			if (!haveLoc)
				return endNoEffect();
			if (outOfRange(loc.x, loc.y))
				return finish();
			if (lineBlocked(loc, _info->_tile, 4)) {
				blocked();
				return finish();
			}
			return cast();

		case SEL_ACTOR: {
			Actor *actor = haveLoc ? actorAt(loc) : nullptr;
			if (!actor) {
				print("nothing\n");
			} else {
				printName(actor->get_name());
				if (outOfRange(actor->get_x(), actor->get_y()))
					actor = nullptr;
			}

			sel._actor = actor;
			return cast();
		}

		case SEL_ACTOR_OR_OBJ: {
			Actor *actor = haveLoc ? actorAt(loc) : nullptr;
			Obj *obj = nullptr;

			if (!actor) {
				obj = haveLoc ? objAt(loc) : nullptr;
				if (isPlayer)
					print(obj ? Common::String(Game::get_game()->get_obj_manager()->get_obj_name(obj)) + "\n" : Common::String("nothing\n"));
			} else if (isPlayer) {
				printName(actor->get_name());
			}

			castingFadeEffect();
			if (!haveLoc || (!actor && !obj)) {
				noEffect();
				return finish();
			}
			if ((!obj || obj->is_on_map()) && outOfRange(loc.x, loc.y))
				return finish();

			sel._actor = actor;
			sel._obj = obj;
			return cast();
		}

		case SEL_ACTOR_PROJ: {
			Actor *actor = haveLoc ? actorAt(loc) : nullptr;

			if (!actor) {
				Obj *obj = haveLoc ? objAt(loc) : nullptr;
				if (isPlayer)
					print(obj ? Common::String(Game::get_game()->get_obj_manager()->get_obj_name(obj)) + "\n" : Common::String("nothing\n"));
			} else if (isPlayer) {
				printName(actor->get_name());
			}

			castingFadeEffect();
			if (!haveLoc)
				return endNoEffect();
			if (outOfRange(loc.x, loc.y))
				return finish();
			if (lineBlocked(loc, _info->_tile, 4)) {
				blocked();
				return finish();
			}
			if (!actor)
				return endNoEffect();

			sel._actor = actor;
			return cast();
		}

		case SEL_OBJ_PROJ: {
			Obj *obj = haveLoc ? objAt(loc) : nullptr;
			if (!obj)
				print("nothing\n");
			else
				printName(Game::get_game()->get_obj_manager()->get_obj_name(obj));

			castingFadeEffect();
			if (!haveLoc)
				return endNoEffect();
			if (outOfRange(loc.x, loc.y))
				return finish();
			if (lineBlocked(loc, _info->_tile, 4)) {
				blocked();
				return finish();
			}
			if (!obj)
				return endNoEffect();

			sel._obj = obj;
			return cast();
		}

		case SEL_ACTOR_OR_OBJ_PROJ: {
			Actor *actor = haveLoc ? actorAt(loc) : nullptr;
			Obj *obj = nullptr;
			if (!actor)
				obj = haveLoc ? objAt(loc) : nullptr;

			if (actor)
				print(actor->get_name());
			else if (obj)
				print(Game::get_game()->get_obj_manager()->get_obj_name(obj));
			else
				print("nothing");
			print("\n");

			castingFadeEffect();
			if (!haveLoc)
				return endNoEffect();
			if (outOfRange(loc.x, loc.y))
				return finish();
			if (lineBlocked(loc, _info->_tile, 4)) {
				blocked();
				return finish();
			}
			if (!actor && !obj)
				return endNoEffect();

			sel._actor = actor;
			sel._obj = obj;
			return cast();
		}

		default:
			break;
		}

		(void)c;
		return finish();
	}

	uint8 endNoEffect() {
		noEffect();
		return finish();
	}

	// Spell effects

	void useObj(Obj *obj) {
		Game::get_game()->get_usecode()->use_obj(obj, Game::get_game()->get_player()->get_actor());
	}

	bool removeEnchantments(Actor *actor) {
		bool result = false;
		_scripts->fadeActorBlue(actor);

		if (actor->is_sleeping()) {
			actor->set_asleep(false);
			result = true;
		}

		if (actor->is_poisoned()) {
			actor->set_poisoned(false);
			result = true;
		}

		// The original script misspelled the paralysis check, so paralysis is never removed

		if (_scripts->actorRemoveCharm(actor))
			result = true;

		return result;
	}

	void gainExp(int exp) {
		if (exp != 0)
			caster()->set_exp(caster()->get_exp() + exp);
	}

	void hitActorBySpell(Actor *actor, int damage) {
		gainExp(_scripts->actorHitTarget(Target(actor), damage, nullptr));
		_scripts->actorHitMsg(actor);
		_scripts->actorYellForHelp(caster(), Target(actor), 1);
	}

	/** Does something to every actor an explosion hits */
	void explosionAt(const MapCoord &loc, uint16 tile, Common::Array<Actor *> &actors) {
		Common::Array<MapEntity> hits;
		_scripts->explosion(tile, loc.x, loc.y, hits);

		for (uint i = 0; i < hits.size(); ++i) {
			if (hits[i].entity_type == ENT_ACTOR)
				actors.push_back(hits[i].actor);
		}
	}

	bool findEffectChild(Obj *container, Obj **found = nullptr) {
		Common::Array<Obj *> children;
		getContainerObjs(container, children);

		for (uint i = 0; i < children.size(); ++i) {
			if (children[i]->obj_n == OBJ_EFFECT) {
				if (found)
					*found = children[i];
				return true;
			}
		}

		return false;
	}

	void placeField(const MapCoord &loc, uint16 fieldObjN) {
		Game *game = Game::get_game();

		if (game->get_game_map()->can_put_obj(loc.x, loc.y, loc.z) &&
				!game->get_game_map()->is_water(loc.x, loc.y, loc.z, false)) {
			Obj *field = newObj(fieldObjN);
			field->set_temporary(true);
			field->x = loc.x;
			field->y = loc.y;
			field->z = loc.z;

			if (game->get_obj_manager()->moveto_map(field, loc)) {
				_scripts->fadeObjIn(field);
				success();
			} else {
				delete_obj(field);
				notPossible();
			}
		} else {
			notPossible();
		}
	}

	void areaActors(Common::Array<Actor *> &actors) {
		Actor *c = caster();

		for (int x = (int)c->get_x() - 5; x <= (int)c->get_x() + 5; ++x) {
			for (int y = (int)c->get_y() - 5; y <= (int)c->get_y() + 5; ++y) {
				Actor *actor = mapGetActor(x, y, c->get_z());
				if (actor)
					actors.push_back(actor);
			}
		}
	}

	bool findRandomSpot(int tries, int &x, int &y) {
		Actor *c = caster();

		for (int i = 0; i < tries; ++i) {
			x = randomRange(0, 10) + c->get_x() - 5;
			y = randomRange(0, 10) + c->get_y() - 5;

			if (Game::get_game()->get_actor_manager()->can_put_actor(MapCoord(x, y, c->get_z())))
				return true;
		}

		return false;
	}

	uint8 windSpell(int spellNum, uint16 tile) {
		Actor *c = caster();
		const MapCoord &loc = _sel._loc;

		if (_resumedWithNil)
			return finish();

		print("\n");

		if (loc.x == c->get_x() && loc.y == c->get_y()) {
			noEffect();
			return finish();
		}

		int targetX = loc.x;
		int targetY = loc.y;
		int index = (((int)c->get_y() - targetY + 5) * 11) + ((int)c->get_x() - targetX + 5);
		int offsetXLow = 0, offsetYLow = 0, offsetXHigh = 0, offsetYHigh = 0;

		if (index >= 0 && index < 121) {
			static const int DIAGONAL_X[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
			static const int DIAGONAL_Y[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
			int low = PROJECTILE_OFFSETS[0][index];
			int high = PROJECTILE_OFFSETS[1][index];

			offsetXLow = DIAGONAL_X[low];
			offsetYLow = DIAGONAL_Y[low];
			offsetXHigh = DIAGONAL_X[high];
			offsetYHigh = DIAGONAL_Y[high];
		}

		Common::Array<MapCoord> targets;
		uint8 z = c->get_z();
		targets.push_back(MapCoord(targetX, targetY, z));
		targets.push_back(MapCoord(targetX + offsetXLow, targetY + offsetYLow, z));
		targets.push_back(MapCoord(targetX + offsetXLow * 2, targetY + offsetYLow * 2, z));
		targets.push_back(MapCoord(targetX + offsetXHigh, targetY + offsetYHigh, z));
		targets.push_back(MapCoord(targetX + offsetXHigh * 2, targetY + offsetYHigh * 2, z));

		Common::Array<MapEntity> hits;
		projectileAnimMulti(tile, c->get_x(), c->get_y(), targets, 3, true, 0, hits);

		for (uint i = 0; i < hits.size(); ++i) {
			if (hits[i].entity_type != ENT_ACTOR || hits[i].actor->get_actor_num() == c->get_actor_num())
				continue;

			Actor *actor = hits[i].actor;

			if (spellNum == 83) {
				_scripts->spellTakeFireDmg(c, actor);
			} else if (spellNum == 87) {
				_scripts->spellPoisonActor(c, actor);
			} else if (spellNum == 98) {
				hitActorBySpell(actor, randomRange(1, 0x1e));
			} else if (spellNum == 113) {
				_scripts->spellKillActor(c, actor);
			}

			if (_scripts->_avatarDied)
				break;
		}

		return finish();
	}

	uint8 chainBolt(Actor *actor) {
		Actor *c = caster();

		hitActorBySpell(actor, randomRange(1, 0x1e));
		_scripts->actorHitMsg(actor);

		for (int i = 0; i <= 6; ++i) {
			if (_scripts->_avatarDied)
				break; // Don't keep casting once the avatar is dead

			int bestDistance = 32769;
			Actor *newTarget = nullptr;

			for (int j = 1; j <= 0xff; ++j) {
				Actor *other = getActor(j);

				if (other->get_obj_n() != 0 && other->is_alive() && other->get_actor_num() != actor->get_actor_num() &&
						_scripts->actorOkToAttack(actor, other)) {
					int targetX = other->get_x();
					int targetY = other->get_y();

					if (other->get_z() == c->get_z() && targetX > (int)c->get_x() - 5 && targetX < (int)c->get_x() + 5 &&
							targetY > (int)c->get_y() - 5 && targetY < (int)c->get_y() + 5) {
						int val = (targetX - (int)actor->get_x()) * (targetX - (int)actor->get_x()) +
							(targetY - (int)actor->get_y()) * (targetY - (int)actor->get_y());

						if (val > 0 && val <= bestDistance) {
							if (val == bestDistance) {
								if (randomRange(0, 1) == 0)
									newTarget = other;
							} else {
								bestDistance = val;
								newTarget = other;
							}
						}
					}
				}
			}

			if (!newTarget)
				break;

			_scripts->projectile(0x188, actor->get_x(), actor->get_y(), newTarget->get_x(), newTarget->get_y(), 2, 0);
			actor = newTarget;
			hitActorBySpell(actor, randomRange(1, 0x1e));
		}

		return finish();
	}

	/** Runs the spell once it has what it needs. Returns what the thread should do next */
	uint8 cast() {
		Game *game = Game::get_game();
		GameClock *clock = game->get_clock();
		ObjManager *objManager = game->get_obj_manager();
		Player *player = game->get_player();
		Actor *c = caster();
		Actor *actor = _sel._actor;
		Obj *obj = _sel._obj;
		const MapCoord loc = _sel._loc;

		switch (_spell) {
		case 0: { // Create Food
			castingEffect();
			castingFadeEffect();
			Obj *food = newObj(129);
			food->qty = randomRange(1, 10);
			objManager->moveto_inventory(food, c);
			print(Common::String::format("\nCreate %d food.\n", food->qty));
			break;
		}

		case 1: { // Detect Magic
			castingFadeEffect();
			if (!obj) {
				noEffect();
				break;
			}

			_scripts->fadeObjBlue(obj);
			bool magical = false;
			for (uint i = 0; i < ARRAYSIZE(MAGICAL_TILES); ++i) {
				if (MAGICAL_TILES[i] == objTileNum(obj))
					magical = true;
			}

			if (magical) {
				if (obj->obj_n == 0x113) {
					print(Common::String::format("\nIt's %s potion.\n", POTION_NAMES[MIN<int>(obj->frame_n, 8)]));
				} else {
					print("\nIt's magical.\n");
				}
			} else {
				bool hasCharge = false;
				Common::Array<Obj *> children;
				getContainerObjs(obj, children);

				for (uint i = 0; i < children.size() && !hasCharge; ++i) {
					if (children[i]->obj_n == OBJ_CHARGE) {
						hasCharge = true;
						const SpellInfo *spell = findSpell(children[i]->quality);
						print(Common::String::format("\nIt shows a charge of %s.\n", spell ? spell->_name : "Unknown"));
					}
				}

				if (!hasCharge)
					print("\nIt's not magical.\n");
			}
			break;
		}

		case 2: { // Detect Trap
			castingFadeEffect();
			if (!obj) {
				noEffect();
				break;
			}

			_scripts->fadeObjBlue(obj);
			bool found = false;
			int diceRoll = randomRange(1, 0x1e);

			if (_scripts->actorIntAdjusted(c) >= diceRoll && findEffectChild(obj)) {
				found = true;
				print("\nIt's trapped.\n");
			}

			if (!found)
				print("\nNo trap.\n");
			break;
		}

		case 3: // Dispel Magic
			if (removeEnchantments(actor))
				success();
			else
				noEffect();
			break;

		case 4: // Douse
			if (obj->obj_n == 122 || obj->obj_n == 145 || obj->obj_n == 164 || obj->obj_n == 90 ||
					(obj->obj_n == 206 && obj->frame_n == 1)) {
				_scripts->fadeObjBlue(obj);

				if (obj->frame_n == 1 || obj->frame_n == 3) {
					if (obj->obj_n == 90)
						useObj(obj); // Use the usecode to remove the torch
					else
						obj->frame_n = obj->frame_n - 1;

					success();
					break;
				}
			}

			noEffect();
			break;

		case 5: // Harm
			if (!_scripts->spellHitActor(c, actor, 5))
				failed();
			break;

		case 6: // Heal
			_scripts->fadeActorBlue(actor);
			if (actor->is_alive()) {
				actor->set_hp(MIN<int>(actor->get_hp() + randomRange(1, 0x1e), actor->get_maxhp()));
				success();
			} else {
				noEffect();
			}
			break;

		case 7: // Help
			castingEffect();
			_scripts->playSfx(SFX_KAL_LOR, true);
			if (!_scripts->_armageddon)
				game->get_party()->resurrect_dead_members();
			_scripts->partyHeal();
			player->move(0x133, 0x160, 0, true);
			_scripts->getLBToThrone();
			fadeInEffect(); // The original fades from the current location. We fade from black
			break;

		case 8: // Ignite
			if (obj->obj_n == 122 || obj->obj_n == 145 || obj->obj_n == 164 || obj->obj_n == 90 ||
					(obj->obj_n == 206 && obj->frame_n == 0)) {
				_scripts->fadeObjBlue(obj);

				if (obj->frame_n == 0 || obj->frame_n == 2) {
					if (obj->obj_n == 90)
						useObj(obj);
					else
						obj->frame_n = obj->frame_n + 1;

					success();
					break;
				}
			} else if (obj->obj_n == 0xdf && obj->frame_n == 0) { // unlit keg
				_scripts->useKeg(obj);
				success();
				break;
			}

			noEffect();
			break;

		case 9: // Light
			castingFadeEffect();
			castingEffect();
			clock->set_timer(TIMER_LIGHT, 0x64);
			break;

		case 16: // Infravision
			castingFadeEffect();
			castingEffect();
			clock->set_timer(TIMER_INFRAVISION, 0x14);
			break;

		case 17: // Magic Arrow
			if ((((_scripts->actorDexAdjusted(actor) / 2.0f) + 0x1e) - _scripts->actorIntAdjusted(c)) / 2 <= randomRange(1, 0x1e)) {
				print("\n");
				hitActorBySpell(actor, randomRange(1, 0xa));
			} else {
				failed();
			}
			break;

		case 18: { // Poison
			int result = _scripts->spellPoisonActor(c, actor);
			if (result == 2)
				noEffect();
			else if (result == 1)
				failed();
			break;
		}

		case 19: { // Reappear
			castingFadeEffect();
			if (_resumedWithNil || _scripts->_vanishObjN == 0) {
				noEffect();
				break;
			}

			int hitX, hitY, preX, preY;
			_scripts->mapLineHitCheck(c->get_x(), c->get_y(), loc.x, loc.y, loc.z, hitX, hitY, preX, preY);
			_scripts->projectile(0x17f, c->get_x(), c->get_y(), hitX, hitY, 2);

			if (hitX != loc.x || hitY != loc.y) {
				blocked();
				break;
			}

			Obj *vanished = newObj(_scripts->_vanishObjN);
			vanished->frame_n = _scripts->_vanishFrameN;
			vanished->qty = 1;
			vanished->status = 0x21; // OK, TEMP

			if (!game->get_game_map()->can_put_obj(loc.x, loc.y, loc.z) || !objManager->moveto_map(vanished, loc)) {
				delete_obj(vanished);
				noEffect();
			} else {
				_scripts->fadeObjIn(vanished);
				_scripts->_vanishObjN = 0;
				_scripts->_vanishFrameN = 0;
				success();
			}
			break;
		}

		case 20: { // Sleep
			int result = _scripts->spellPutActorToSleep(c, actor);
			if (result == 1)
				failed();
			else if (result == 2)
				noEffect();
			break;
		}

		case 21: // Telekinesis
			return telekinesis();

		case 22: // Trap
			castingFadeEffect();
			if (!obj) {
				noEffect();
				break;
			}

			_scripts->fadeObjBlue(obj);
			if (isDoor(obj->obj_n) || obj->obj_n == OBJ_CHEST) {
				if (!findEffectChild(obj)) {
					Obj *effect = newObj(OBJ_EFFECT);
					objManager->moveto_container(effect, obj);
					success();
					break;
				}
			}

			noEffect();
			break;

		case 23: // Unlock Magic
			castingFadeEffect();
			if (!obj) {
				noEffect();
			} else if (isDoor(obj->obj_n) && obj->frame_n >= 13) { // magically locked door
				obj->frame_n = obj->frame_n - 8;
				print("\nunlocked!\n");
			} else if (obj->obj_n == OBJ_CHEST && obj->frame_n == 3) { // magically locked chest
				obj->frame_n = 1;
				print("\nunlocked!\n");
			} else {
				noEffect();
			}
			break;

		case 24: { // Untrap
			castingFadeEffect();
			if (!obj) {
				noEffect();
				break;
			}

			bool found = false;
			_scripts->fadeObjBlue(obj);

			if (isDoor(obj->obj_n) || obj->obj_n == OBJ_CHEST) {
				Obj *effect = nullptr;
				if (findEffectChild(obj, &effect)) {
					found = true;
					obj->remove(effect);
					delete_obj(effect);
				}
			}

			if (found)
				success();
			else
				noEffect();
			break;
		}

		case 25: // Vanish
			if (tileGetFlag(objTileNum(obj), 3, 3)) {
				_scripts->_vanishObjN = obj->obj_n;
				_scripts->_vanishFrameN = obj->frame_n;
				_scripts->fadeObjOut(obj);
				removeFromEngine(obj);
				success();
			} else {
				noEffect();
			}
			break;

		case 32: // Curse
			if (game->is_god_mode_enabled() && actor->is_in_party())
				break;
			actor->set_cursed(true);
			success();
			break;

		case 33: // Dispel Field
			if (obj->obj_n >= 317 && obj->obj_n <= 320) {
				hitAnim(obj->x, obj->y);
				removeFromEngine(obj);
				success();
			} else {
				noEffect();
			}
			break;

		case 34: // Fireball
			_scripts->spellTakeFireDmg(c, actor);
			break;

		case 35: // Great Light
			castingFadeEffect();
			castingEffect();
			clock->set_timer(TIMER_LIGHT, 0xff);
			break;

		case 36: // Lock
			castingFadeEffect();
			if (!obj) {
				noEffect();
			} else if (isDoor(obj->obj_n)) {
				if (obj->frame_n <= 7) {
					if (obj->frame_n <= 3) {
						print("\nCan't (Un)lock an opened door\n");
					} else if (obj->frame_n >= 5 && obj->frame_n <= 7) {
						obj->frame_n = obj->frame_n + 8;
						print("\nmagically locked!\n");
					}

					success();
				}
			} else if (obj->obj_n == OBJ_CHEST && (obj->frame_n == 1 || obj->frame_n == 2)) {
				obj->frame_n = 3;
				print("\nmagically locked!\n");
			} else {
				noEffect();
			}
			break;

		case 37: { // Mass Awaken
			Common::Array<Actor *> hit;
			explosionAt(loc, 0x17f, hit);
			for (uint i = 0; i < hit.size(); ++i)
				hit[i]->set_asleep(false);
			success();
			break;
		}

		case 38: { // Mass Sleep
			Common::Array<Actor *> hit;
			explosionAt(loc, 0x17d, hit);
			for (uint i = 0; i < hit.size(); ++i)
				_scripts->spellPutActorToSleep(c, hit[i]);
			success();
			break;
		}

		case 39: { // Peer
			castingEffect();
			uint16 x, y;
			uint8 z;
			player->get_location(&x, &y, &z);
			AsyncEffect *effect = new AsyncEffect(new PeerEffect((x - x % 8) - 18, (y - y % 8) - 18, z));
			effect->run(EFFECT_PROCESS_GUI_INPUT);
			break;
		}

		case 40: // Protection
			actor->set_protected(true);
			success();
			break;

		case 41: { // Repel Undead
			castingFadeEffect();
			Common::Array<Actor *> actors;
			areaActors(actors);

			for (uint i = 0; i < actors.size(); ++i) {
				if (statFlag(actors[i]->get_obj_n(), FLAG_REPEL_UNDEAD) && !_scripts->actorIntCheck(actors[i], c))
					actors[i]->set_worktype(WT_UNK_13);
			}
			success();
			break;
		}

		case 48: // Animate
			if (isReadiableTile(objTileNum(obj))) {
				newTempActor(obj->obj_n, obj->x, obj->y, obj->z, ACTOR_ALIGNMENT_GOOD, 8);
				removeFromEngine(obj);
				success();
			} else {
				noEffect();
			}
			break;

		case 49: { // Conjure
			castingEffect();
			castingFadeEffect();
			int i = randomRange(0, 0xf);
			uint16 objN;

			if (i <= 3)
				objN = 0x173; // troll
			else if (i <= 6)
				objN = 0x156; // giant rat
			else if (i <= 9)
				objN = 0x169; // giant spider
			else if (i <= 0xc)
				objN = 0x158; // giant bat
			else
				objN = 0x166; // snake

			int x, y;
			if (findRandomSpot(8, x, y)) {
				Actor *monster = newTempActor(objN, x, y, c->get_z());
				if (monster) {
					monster->set_alignment(c->get_alignment());
					monster->set_worktype(WT_ASSAULT);
					success();
					break;
				}
			}

			failed();
			break;
		}

		case 50: // Disable
			castingFadeEffect();
			if (!actor) {
				noEffect();
			} else if (!_scripts->spellHitActor(c, actor, 50)) {
				failed();
			}
			break;

		case 51: // Fire Field
			placeField(loc, 317);
			break;

		case 52: // Great Heal
			_scripts->fadeActorBlue(actor);
			if (actor->is_alive()) {
				actor->set_hp(actor->get_maxhp());
				success();
			} else {
				noEffect();
			}
			break;

		case 53: { // Locate
			castingFadeEffect();
			castingEffect();
			int x = c->get_x() / 8;
			int y = c->get_y() / 8;
			char lon, lat;

			if (x > 38) {
				x -= 38;
				lon = 'E';
			} else {
				x = 38 - x;
				lon = 'W';
			}

			if (y > 45) {
				y -= 45;
				lat = 'S';
			} else {
				y = 45 - y;
				lat = 'N';
			}

			print(Common::String::format("\n%d{%c, %d{%c\n", y, lat, x, lon));
			break;
		}

		case 54: { // Mass Dispel
			Common::Array<Actor *> hit;
			explosionAt(loc, 0x17f, hit);
			for (uint i = 0; i < hit.size(); ++i)
				removeEnchantments(hit[i]);
			success();
			break;
		}

		case 55: // Poison Field
			placeField(loc, 318);
			break;

		case 56: // Sleep Field
			placeField(loc, 320);
			break;

		case 57: // Wind Change
			print("Direction-");
			return wait(NUVIE_SCRIPT_GET_DIRECTION, STEP_WIND_DIRECTION);

		case 64: // Energy Field
			placeField(loc, 319);
			break;

		case 65: { // Explosion
			Common::Array<Actor *> hit;
			explosionAt(loc, 0x17e, hit);

			for (uint i = 0; i < hit.size(); ++i) {
				_scripts->spellTakeFireDmg(c, hit[i]);
				if (_scripts->_avatarDied)
					break;
			}

			_scripts->explodeSurroundingObjects(loc.x, loc.y, loc.z);
			break;
		}

		case 66: { // Insect Swarm
			castingEffect();
			castingFadeEffect();

			for (int i = 0; i < 8; ++i) {
				int x, y;
				if (findRandomSpot(8, x, y)) {
					Actor *insect = newTempActor(0x157, x, y, c->get_z());
					if (insect) {
						insect->set_alignment(c->get_alignment());
						insect->set_worktype(WT_ASSAULT);
					}
				}
			}

			success();
			break;
		}

		case 67: // Invisibility
			if (actor->is_visible() && actorCanTurnInvisible(actor->get_obj_n())) {
				_scripts->fadeActorBlue(actor);
				actor->set_invisible(true);
				success();
			} else {
				noEffect();
			}
			break;

		case 68: // Lightning
			if (casterIsPlayer())
				print("\n");
			hitActorBySpell(actor, randomRange(1, 30));
			break;

		case 69: // Paralyze
			if (game->is_god_mode_enabled() && actor->is_in_party())
				break;

			print("\n");
			hitAnim(actor->get_x(), actor->get_y());
			actor->set_paralyzed(true);
			print(Common::String::format("%s is paralyzed.\n", actor->get_name()));
			if (actor->is_in_party())
				partyUpdateLeader();
			break;

		case 70: // Pickpocket
			print("Which object:");
			return wait(NUVIE_SCRIPT_GET_INV_OBJ, STEP_INV_OBJ, actor->get_actor_num());

		case 71: { // Reveal
			castingFadeEffect();
			Common::Array<Actor *> actors;
			areaActors(actors);

			for (uint i = 0; i < actors.size(); ++i) {
				if (!actors[i]->is_visible())
					actors[i]->set_invisible(false);
			}
			success();
			break;
		}

		case 72: { // Seance
			if (!actor && !obj)
				break;

			uint16 objN = actor ? actor->get_obj_n() : obj->obj_n;
			if (objN == 0x153 || objN == 0x160 || objN == 0x155 || objN == 0x154) {
				if (actor && actor->get_actor_num() == 91) {
					actor->set_flag(7);
					actorTalk(actor);
				} else if (obj) {
					actorTalk(getActor(obj->quality));
				}
			}
			break;
		}

		case 73: { // X-ray
			castingEffect();
			castingFadeEffect();
			AsyncEffect *effect = new AsyncEffect(new XRayEffect(6000));
			effect->run();
			print("\nDone\n");
			break;
		}

		case 80: // Charm
			if (!_scripts->spellCharmActor(c, actor))
				failed();
			break;

		case 81: { // Clone
			int x, y;
			if (findRandomSpot(8, x, y)) {
				Actor *clone = newTempActor(actor->get_obj_n(), x, y, c->get_z());
				if (clone) {
					clone->set_frame_n(actor->get_frame_n());
					clone->set_alignment(actor->get_alignment());
					clone->set_worktype(actor->get_worktype() > WT_PLAYER ? actor->get_worktype() : WT_ASSAULT);
					_scripts->fadeActorIn(clone);
					success();
					break;
				}
			}

			noEffect();
			break;
		}

		case 82: { // Confuse
			castingFadeEffect();
			Common::Array<Actor *> actors;
			areaActors(actors);

			for (uint i = 0; i < actors.size(); ++i) {
				if (actors[i]->get_alignment() == ACTOR_ALIGNMENT_EVIL && !_scripts->actorIntCheck(actors[i], c)) {
					actors[i]->set_alignment(ACTOR_ALIGNMENT_GOOD);
					actors[i]->set_worktype(WT_ASSAULT);
				}
			}
			success();
			break;
		}

		case 83: // Flame Wind
			return windSpell(83, 0x18d);

		case 84: // Hail Storm
			castingFadeEffect();
			if (_resumedWithNil) {
				noEffect();
				break;
			}

			print("\n");
			{
				AsyncEffect *effect = new AsyncEffect(new HailStormEffect(loc));
				effect->run();
			}
			print("\nSuccess\n");
			break;

		case 85: { // Mass Protect
			Common::Array<Actor *> hit;
			explosionAt(loc, 0x17f, hit);
			for (uint i = 0; i < hit.size(); ++i)
				hit[i]->set_protected(true);
			success();
			break;
		}

		case 86: // Negate Magic
			castingFadeEffect();
			castingEffect();
			clock->set_timer(TIMER_STORM, 0x14);
			break;

		case 87: // Poison Wind
			return windSpell(87, 0x17c);

		case 88: // Replicate
			if (tileGetFlag(objTileNum(obj), 3, 3)) {
				Obj *copy = newObj(obj->obj_n);
				copy->frame_n = obj->frame_n;
				copy->qty = 1;
				copy->status = 0x21; // OK, TEMP

				for (int i = 0; i < 8; ++i) {
					MapCoord spot(c->get_x() + randomRange(0, 10) - 5, c->get_y() + randomRange(0, 10) - 5, c->get_z());

					if (game->get_game_map()->can_put_obj(spot.x, spot.y, spot.z) && objManager->moveto_map(copy, spot)) {
						_scripts->fadeObjIn(copy);
						success();
						return finish();
					}
				}

				delete_obj(copy);
				failed();
			} else {
				noEffect();
			}
			break;

		case 89: { // Web
			print("\n");
			castingFadeEffect();
			if (_resumedWithNil) {
				noEffect();
				break;
			}

			Common::Array<MapCoord> targets;
			for (int x = loc.x - 1; x <= (int)loc.x + 1; ++x) {
				for (int y = loc.y - 1; y <= (int)loc.y + 1; ++y)
					targets.push_back(MapCoord(x, y, c->get_z()));
			}

			Common::Array<MapEntity> hits;
			projectileAnimMulti(564, c->get_x(), c->get_y(), targets, 2, false, 0, hits);

			int hitX, hitY, preX, preY;
			_scripts->mapLineHitCheck(c->get_x(), c->get_y(), loc.x, loc.y, loc.z, hitX, hitY, preX, preY);
			if (hitX != loc.x || hitY != loc.y) {
				blocked();
				break;
			}

			for (uint i = 0; i < targets.size(); ++i) {
				if (game->get_game_map()->can_put_obj(targets[i].x, targets[i].y, targets[i].z)) {
					Obj *web = newObj(0x35);
					if (!objManager->moveto_map(web, targets[i]))
						delete_obj(web);
				}
			}

			success();
			break;
		}

		case 96: // Chain Bolt
			return chainBolt(actor);

		case 97: // Enchant
			if (!obj)
				break;

			if (obj->obj_n != OBJ_STAFF) {
				noEffect();
				break;
			}

			print("with: ");
			return wait(NUVIE_SCRIPT_GET_SPELL, STEP_SPELL);

		case 98: // Energy Wind
			return windSpell(98, 0x188);

		case 99: { // Fear
			castingFadeEffect();
			Common::Array<Actor *> actors;
			areaActors(actors);

			for (uint i = 0; i < actors.size(); ++i) {
				Actor *other = actors[i];
				if (other->get_alignment() != ACTOR_ALIGNMENT_GOOD && !_scripts->actorIntCheck(other, c)) {
					if (!statFlag(other->get_obj_n(), FLAG_UNDEAD)) // if not undead
						other->set_worktype(WT_UNK_13);
				}
			}
			success();
			break;
		}

		case 100: { // Gate Travel
			print("To phase ");
			int phase = _scripts->inputSelectInteger(true);
			print("\n");
			castingFadeEffect();

			if (phase > 0 && phase < 9 && _scripts->_moonstones[phase - 1].x != 0) {
				const MapCoord &dest = _scripts->_moonstones[phase - 1];
				player->move(dest.x, dest.y, dest.z, true);
				fadeInEffect();
			} else {
				_scripts->playSfx(SFX_FAILURE);
			}
			break;
		}

		case 101: { // Kill
			int result = _scripts->spellKillActor(c, actor);
			if (result == 2)
				noEffect();
			else if (result == 1)
				failed();
			break;
		}

		case 102: { // Mass Curse
			Common::Array<Actor *> hit;
			explosionAt(loc, 0x17f, hit);
			for (uint i = 0; i < hit.size(); ++i) {
				if (!game->is_god_mode_enabled() || !hit[i]->is_in_party())
					hit[i]->set_cursed(true);
			}
			success();
			break;
		}

		case 103: { // Mass Invisibility
			Common::Array<Actor *> hit;
			explosionAt(loc, 0x18a, hit);
			for (uint i = 0; i < hit.size(); ++i) {
				if (hit[i]->is_visible() && actorCanTurnInvisible(hit[i]->get_obj_n())) {
					_scripts->fadeActorBlue(hit[i]);
					hit[i]->set_invisible(true);
				}
			}
			success();
			break;
		}

		case 104: { // Wing Strike
			castingFadeEffect();
			MapCoord playerLoc = player->get_actor()->get_location();
			bool foesPresent = false;

			for (int i = 1; i <= 0xff; ++i) {
				Actor *other = getActor(i);

				if (other->get_obj_n() != 0 && other->is_alive() && other->is_visible() &&
						alignmentIsEvil(other->get_alignment())) {
					int targetX = other->get_x();
					int targetY = other->get_y();

					if (other->get_z() == playerLoc.z && targetX > (int)playerLoc.x - 5 && targetX < (int)playerLoc.x + 5 &&
							targetY > (int)playerLoc.y - 5 && targetY < (int)playerLoc.y + 5) {
						AsyncEffect *effect = new AsyncEffect(new WingStrikeEffect(other));
						effect->run();
						foesPresent = true;
					}

					if (_scripts->_avatarDied)
						break;
				}
			}

			if (!foesPresent) {
				print("\nNo foes.\n");
				_scripts->playSfx(SFX_FAILURE);
			}
			break;
		}

		case 105: { // Wizard Eye
			castingEffect();
			castingFadeEffect();
			AsyncEffect *effect = new AsyncEffect(new WizardEyeEffect(c->get_location(), 0x28));
			effect->run(EFFECT_PROCESS_GUI_INPUT);
			print("\nDone\n");
			break;
		}

		case 112: // Armageddon
			_scripts->setArmageddon(true);
			castingEffect();

			for (int i = 1; i <= 0xff; ++i) {
				Actor *other = getActor(i);

				// Don't kill the avatar or Lord British
				if (i != 1 && i != 5 && other->is_alive())
					other->die(false);
			}
			break;

		case 113: // Death Wind
			return windSpell(113, 0x18c);

		case 114: // Eclipse
			castingFadeEffect();
			castingEffect();
			clock->set_timer(TIMER_ECLIPSE, 0x14);
			break;

		case 115: { // Mass Charm
			Common::Array<Actor *> hit;
			explosionAt(loc, 0x18a, hit);
			for (uint i = 0; i < hit.size(); ++i)
				_scripts->spellCharmActor(c, hit[i]);
			success();
			break;
		}

		case 116: { // Mass Kill
			Common::Array<Actor *> hit;
			explosionAt(loc, 0x18c, hit);

			for (uint i = 0; i < hit.size(); ++i) {
				_scripts->spellKillActor(c, hit[i]);
				if (_scripts->_avatarDied)
					break;
			}

			_scripts->explodeSurroundingObjects(loc.x, loc.y, loc.z);
			success();
			break;
		}

		case 117: // Resurrect
			castingFadeEffect();
			if (obj && !_scripts->_armageddon) {
				_scripts->fadeObjBlue(obj);

				if (obj->obj_n == 339 || obj->obj_n == 0x162 || obj->obj_n == 0x155 || obj->obj_n == 0x154 ||
						obj->obj_n == 349 || obj->obj_n == 134) {
					if (obj->quality < 203) { // Greater than 202 are temporary actors
						if (obj->quality != 0) {
							Actor *body = getActor(obj->quality);
							MapCoord bodyLoc(obj->x, obj->y, obj->z);
							bool toss = obj->is_in_inventory() || obj->is_in_container();
							if (toss)
								bodyLoc = player->get_actor()->get_location();

							body->resurrect(bodyLoc, obj);
							if (toss) {
								ActorManager *actorManager = game->get_actor_manager();
								if (!actorManager->toss_actor(body, 2, 2))
									actorManager->toss_actor(body, 4, 4);
							}

							success();
						} else {
							print("\nThe corpse is too old!\n");
						}
					}
				} else {
					noEffect();
				}
			} else {
				noEffect();
			}
			break;

		case 118: { // Slime
			MapCoord playerLoc = player->get_actor()->get_location();
			castingFadeEffect();

			for (int x = (int)playerLoc.x - 5; x <= (int)playerLoc.x + 5; ++x) {
				for (int y = (int)playerLoc.y - 5; y <= (int)playerLoc.y + 5; ++y) {
					Actor *other = mapGetActor(x, y, playerLoc.z);

					if (other && other->get_alignment() == ACTOR_ALIGNMENT_EVIL &&
							!statFlag(other->get_obj_n(), FLAG_LIVES_IN_WATER) && !_scripts->actorIntCheck(other, c)) {
						int oldTileNum = other->get_tile()->tile_num;
						other->change_base_obj_n(0x177); // Convert the actor into a slime
						int newTileNum = other->get_tile()->tile_num;
						other->set_invisible(true);
						_scripts->fadeTile(x, y, playerLoc.z, oldTileNum, newTileNum);
						other->set_invisible(false);

						if (other->is_temp()) {
							Common::Array<Obj *> inventory;
							getInventoryObjs(other, false, inventory);
							for (uint i = 0; i < inventory.size(); ++i)
								removeInventoryObj(other, inventory[i]);
						}
					}
				}
			}

			_scripts->slimeUpdateFrames();
			break;
		}

		case 119: { // Summon
			MapCoord playerLoc = player->get_actor()->get_location();
			newTempActor(367, playerLoc.x, playerLoc.y - 1, playerLoc.z, c->get_alignment(), 8);
			break;
		}

		case 120: // Time Stop
			castingFadeEffect();
			castingEffect();
			clock->set_timer(TIMER_TIME_STOP, 0xa);
			break;

		case 121: { // Tremor
			castingFadeEffect();
			_scripts->playSfx(SFX_EARTH_QUAKE, false);
			quakeStart(1, 1024);

			for (int x = (int)c->get_x() - 5; x <= (int)c->get_x() + 5; ++x) {
				for (int y = (int)c->get_y() - 5; y <= (int)c->get_y() + 5; ++y) {
					Actor *other = mapGetActor(x, y, c->get_z());

					if (other && other->get_alignment() == ACTOR_ALIGNMENT_EVIL &&
							!statFlag(other->get_obj_n(), FLAG_IMMUNE_TO_TREMOR)) {
						gainExp(_scripts->actorHitTarget(Target(other), randomRange(1, 0x1e), nullptr));
						_scripts->actorHitMsg(other);

						if (_scripts->_avatarDied)
							break;
					}
				}
			}

			print("\nSuccess\n");
			break;
		}

		case 128: // Monster Invisible
			if (c->is_visible()) {
				_scripts->fadeActorBlue(c);
				c->set_invisible(true);
				success();
			} else {
				noEffect();
			}
			break;

		default:
			break;
		}

		return finish();
	}

	uint8 telekinesis() {
		Actor *actor = _sel._actor;
		Obj *obj = _sel._obj;

		if (actor) {
			noEffect(); // Does nothing with actors
			return finish();
		}

		_scripts->fadeObjBlue(obj);

		if (obj->obj_n == 0x120 || obj->obj_n == 0x10c) { // crank, lever
			useObj(obj);
		} else if (floorf(Game::get_game()->get_obj_manager()->get_obj_weight(obj, OBJ_WEIGHT_INCLUDE_CONTAINER_ITEMS,
				OBJ_WEIGHT_DONT_SCALE)) / 10 >= 0.01f) {
			print("Direction-");
			return wait(NUVIE_SCRIPT_GET_DIRECTION, STEP_DIRECTION);
		} else {
			print("\nNot Possible\n");
		}

		return finish();
	}

public:
	MagicThread(U6Scripts *scripts, uint8 spell, Actor *caster, const MapCoord *target) :
			_scripts(scripts), _info(findSpell(spell)), _spell(spell), _caster(caster), _hasTarget(target != nullptr) {
		if (target)
			_target = *target;
	}

protected:
	uint8 run() override {
		if (!_info)
			return NUVIE_SCRIPT_FINISHED;

		switch (_step) {
		case STEP_START:
			return begin();

		case STEP_LOCATION:
			return selectedLocation();

		case STEP_OBJ:
			_sel = Selection();
			_sel._obj = _resumedWithNil ? nullptr : _obj;
			return selectedObj();

		case STEP_DIRECTION: {
			if (_resumedWithNil) {
				print("`unknown\n");
				return finish();
			}

			static const char *const NAMES[8] = { "north", "east", "south", "west", "northeast", "southeast", "southwest", "northwest" };
			print(Common::String("`") + NAMES[_direction] + "\n");

			Obj *obj = _sel._obj;
			int newX = obj->x + dirX(_direction);
			int newY = obj->y + dirY(_direction);

			if (Game::get_game()->get_game_map()->can_put_obj(newX, newY, obj->z)) {
				Game::get_game()->get_obj_manager()->moveto_map(obj, MapCoord(newX, newY, obj->z));
			} else {
				print("\nBlocked\n");
			}

			return finish();
		}

		case STEP_WIND_DIRECTION: {
			if (_resumedWithNil)
				return finish();

			static const char *const NAMES[8] = { "north", "east", "south", "west", "northeast", "southeast", "southwest", "northwest" };
			print(Common::String("`") + NAMES[_direction] + "\n");
			Game::get_game()->get_weather()->set_wind_dir(_direction);
			return finish();
		}

		case STEP_SPELL: {
			if (_resumedWithNil) {
				print("none\n\n");
				return finish();
			}

			uint8 spellNum = _spellNum;
			Obj *staff = _sel._obj;

			if (spellNum != 0x61) { // Don't allow recursion on the enchant spell
				const SpellInfo *chosen = findSpell(spellNum);
				if (chosen) {
					for (const char *p = chosen->_invocation; *p; ++p) {
						if (p != chosen->_invocation)
							print(" ");
						print(SYLLABLES[*p - 'a']);
					}
				}
				print("\n");

				Common::Array<Obj *> children;
				getContainerObjs(staff, children);
				if (children.size() >= 10) {
					print("\nIt's full\n");
					_scripts->playSfx(SFX_FAILURE);
					return finish();
				}

				castingFadeEffect();
				Obj *charge = newObj(OBJ_CHARGE);
				charge->quality = spellNum;
				charge->set_invisible(true);
				Game::get_game()->get_obj_manager()->moveto_container(charge, staff);
			}

			return finish();
		}

		case STEP_INV_OBJ: {
			Actor *actor = _sel._actor;
			Actor *c = caster();

			if (!_resumedWithNil && _obj) {
				Obj *item = _obj;
				print(Common::String(Game::get_game()->get_obj_manager()->get_obj_name(item)) + "\n");

				if (!actor->can_carry_object(item)) {
					print("\ncan't carry!\n");
					return finish();
				}

				_scripts->projectile(objTileNum(item), actor->get_x(), actor->get_y(), c->get_x(), c->get_y(), 2);
				actor->inventory_remove_obj(item, false);
				c->inventory_add_object(item, nullptr, false);
				_scripts->playerSubtractKarma(5);
				success();
			} else {
				noEffect();
			}

			return finish();
		}

		default:
			return NUVIE_SCRIPT_FINISHED;
		}
	}
};

void U6Scripts::spellTakeFireDmg(Actor *attacker, Actor *foe) {
	const ActorStats *base = getActorStats(foe->get_obj_n());
	if (!base || base->_flags[FLAG_IMMUNE_TO_MAGIC - FLAG_CAN_TALK] == 1)
		return;

	int dmg = randomRange(1, 0x14);

	if (base->_flags[FLAG_DOUBLE_FIRE_DAMAGE - FLAG_CAN_TALK] == 1)
		dmg *= 2;

	int expGained = actorHitTarget(Target(foe), dmg, nullptr);
	if (expGained != 0)
		attacker->set_exp(attacker->get_exp() + expGained);

	actorYellForHelp(attacker, Target(foe), 1);
	actorHitMsg(foe);
}

int U6Scripts::spellPutActorToSleep(Actor *attacker, Actor *foe) {
	if (!statFlag(foe->get_obj_n(), FLAG_IMMUNE_TO_SLEEP)) {
		if (!actorIntCheck(foe, attacker)) {
			hitAnim(foe->get_x(), foe->get_y());
			actorPutToSleep(foe);
			return 0xfe;
		}

		return 1;
	}

	return 2;
}

int U6Scripts::spellPoisonActor(Actor *attacker, Actor *foe) {
	const ActorStats *base = getActorStats(foe->get_obj_n());
	if (!base || base->_flags[FLAG_IMMUNE_TO_POISON - FLAG_CAN_TALK] == 1 || foe->get_actor_num() == 0)
		return 2;

	if (((actorStrAdjusted(foe) / 2 + 0x1e - actorIntAdjusted(attacker)) / 2) <= randomRange(1, 0x1e)) {
		foe->set_poisoned(true);
		hitAnim(foe->get_x(), foe->get_y());
		return -1;
	}

	return 1;
}

bool U6Scripts::spellCharmActor(Actor *attacker, Actor *foe) {
	if (actorIntCheck(foe, attacker))
		return false;

	if (foe->is_charmed()) {
		actorRemoveCharm(foe);
	} else {
		foe->set_charmed(true);
		foe->set_old_alignment(foe->get_alignment());
		foe->set_alignment(attacker->get_alignment());
		hitAnim(foe->get_x(), foe->get_y());
		print(Common::String::format("\n%s is charmed.\n", foe->get_name()));
		if (foe->is_in_party())
			partyUpdateLeader();
	}

	return true;
}

int U6Scripts::spellKillActor(Actor *attacker, Actor *foe) {
	if (statFlag(foe->get_obj_n(), FLAG_UNDEAD))
		return 2; // Immune to corp spells

	if (actorIntCheck(foe, attacker))
		return 1;

	int expGained = actorHitTarget(Target(foe), foe->get_hp(), nullptr);
	if (expGained != 0)
		attacker->set_exp(attacker->get_exp() + expGained);

	actorYellForHelp(attacker, Target(foe), 1);
	actorHitMsg(foe);

	return -1;
}

bool U6Scripts::spellHitActor(Actor *attacker, Actor *foe, int spellNum) {
	if (actorIntCheck(foe, attacker))
		return false;

	int dmg = 0;

	if (spellNum == 5)
		dmg = randomRange(1, 0xa);
	else if (spellNum == 0x62)
		dmg = randomRange(1, 0x1e);
	else if (spellNum == 0x32)
		dmg = foe->get_hp() - 1;

	print("\n");

	int expGained = actorHitTarget(Target(foe), dmg, nullptr);
	if (expGained != 0)
		attacker->set_exp(attacker->get_exp() + expGained);

	actorHitMsg(foe);
	actorYellForHelp(attacker, Target(foe), 1);

	return true;
}

void U6Scripts::magicCastSpell(uint8 spellNum, Actor *caster, const MapCoord *target) {
	MagicThread thread(this, spellNum, caster, target);
	thread.start();

	if (_avatarDied)
		actorAvatarDeath();
}

ScriptThread *U6Scripts::magicCastThread(const char *invocation) {
	for (uint i = 0; i < ARRAYSIZE(SPELLS); ++i) {
		if (!scumm_stricmp(SPELLS[i]._invocation, invocation))
			return new MagicThread(this, SPELLS[i].spellNum(), nullptr, nullptr);
	}

	return nullptr;
}

bool U6Scripts::magicGetSpellList(Spell **spellList) {
	for (uint i = 0; i < ARRAYSIZE(SPELLS); ++i) {
		uint8 num = SPELLS[i].spellNum();

		if (!spellList[num])
			spellList[num] = new Spell(num, SPELLS[i]._name, SPELLS[i]._invocation, SPELLS[i]._reagents);
	}

	return true;
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
