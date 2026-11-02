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
#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/game_clock.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/common/cutscene_script.h"
#include "ultima/ultima6/script/md/md_scripts.h"
#include "ultima/ultima6/usecode/usecode.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

namespace {

enum {
	UI_STYLE_ORIG = 0
};

/** The picture shown when looking at an obelisk */
class ObeliskView : public CutsceneScript {
public:
	void show(int imageIdx);
};

void ObeliskView::show(int imageIdx) {
	bool originalStyle = Game::get_game()->get_game_style() == UI_STYLE_ORIG;

	_cutscene->moveToFront();
	_cutscene->hide_sprites();
	_cutscene->set_screen_opacity(0xff);
	_cutscene->set_update_interval(25);
	_cutscene->enable_game_palette_rotation(true);

	CSSprite *obelisk = newSprite(nullptr, 184, 0, true);

	if (originalStyle) {
		_cutscene->set_solid_bg(false);
	} else {
		CSSprite *text = newSprite(nullptr, 8, 164, true);
		text->text_align = 2;
		text->text_color = 15;
		text->text = "Obelisk.";
		obelisk->x = 96;
		obelisk->y = 41;
	}

	obelisk->image = loadImage("mdream.lzc", imageIdx);

	waitForInput();

	_cutscene->set_solid_bg(true);
	_cutscene->enable_game_palette_rotation(false);
	_cutscene->Hide();
}

} // End of anonymous namespace

void MDScripts::lookPocketwatch(Obj *obj) {
	printl("THE_TIME_IS");

	GameClock *clock = Game::get_game()->get_clock();
	int hour = clock->get_hour();
	int minute = clock->get_minute();
	bool am = hour < 12;

	if (hour > 12)
		hour -= 12;

	printfl(am ? "TIME_AM" : "TIME_PM", hour, minute);
}

void MDScripts::lookCannon(Obj *obj) {
	if (obj->quality != 0)
		printl("THE_CANNON_WILL_FIRE_STEEL_CANNON_BALLS");
}

void MDScripts::lookBarrow(Obj *obj) {
	if (obj->qty == 0) {
		printl("IT_IS_EMPTY");
		return;
	}

	const char *material = "";
	switch (obj->quality) {
	case 1:
		material = _lang.get("DIRT");
		break;
	case 2:
		material = _lang.get("ROCK");
		break;
	case 3:
		material = _lang.get("IRON_ORE");
		break;
	case 4:
		material = _lang.get("COAL");
		break;
	default:
		break;
	}

	if (obj->qty == 1)
		printfl("IT_HAS_1_LOAD_OF", material);
	else
		printfl("IT_HAS_N_LOADS_OF", (int)obj->qty, material);
}

Common::String MDScripts::getLatLongString(int x, int y) {
	int lat = (int)(((y - 512) * 240) / 1024.0);
	int longitude = (int)(((x - 512) * 360) / 1024.0);
	const char *latStr = "N";
	const char *longStr = "W";

	if (lat > 0)
		latStr = "S";
	else if (lat == 0)
		latStr = " ";

	if (longitude == 180 || longitude == -180 || longitude == 0)
		longStr = " ";
	else if (longitude < 0)
		longStr = "E";

	return Common::String::format("%d%s %d%s", ABS(lat), latStr, 180 - ABS(longitude), longStr);
}

void MDScripts::lookMarkerFlag(Obj *obj) {
	static const char *const NAMES[] = {
		"Coprates Chasma", "Arsia Mons", "Pavonis Mons", "Ascraeus Mons", "Alber Tholus",
		"Elysium Mons", "Hecates Tholus", "Terra Sirenum", "Noctis Labyrinthus",
		"Deuteronicus Mensae", "Syrtis Major Planum", "Olympus Mons"
	};

	if (obj->z != 0)
		return;

	if (obj->quality > 12)
		printl("AARGH");
	else if (obj->quality != 0)
		print(Common::String(NAMES[obj->quality - 1]) + " " + getLatLongString(obj->x, obj->y) + "\n");
}

void MDScripts::lookBrokenStrap(Obj *obj) {
	Actor *spector = Game::get_game()->get_actor_manager()->get_actor(2);

	spector->set_flag(6);
	actorTalk(spector);
}

void MDScripts::lookMetalWoman(Obj *obj) {
	if (obj->quality != 0)
		printl("IT_HAS_A_HEARTSTONE");
}

void MDScripts::lookCoveredMartianSeed(Obj *obj) {
	if (obj->frame_n >= 4)
		return;

	if (obj->quality == 15)
		printl("IT_IS_GROWING");
	else if (obj->quality == 16)
		printl("IT_IS_RIPE");
	else
		printl("IT_IS_NOT_GROWING");
}

void MDScripts::lookSprayerSystem(Obj *obj) {
	uint8 quality = obj->quality;
	Actor *actor = Game::get_game()->get_actor_manager()->get_actor(0x3e);

	if (quality & 1) {
		if (actor->get_flag(5)) {
			if (quality & 2)
				printl("IT_IS_READY_TO_USE");
			else
				printl("THE_RUBBER_IN_THE_SPRAYER_SYSTEM_IS_NOT_FLESH_COLORED");
		} else {
			printl("IT_IS_READY_TO_USE");
		}
	} else {
		printl("THERE_IS_NO_RUBBER_IN_THE_SPRAYER_SYSTEM");
		if (actor->get_flag(5) && !(quality & 2))
			printl("IT_IS_ALSO_NOT_FLESH_COLORED");
	}
}

void MDScripts::lookCabinet(Obj *obj) {
	if (obj->quality == 0 || obj->quality > 7)
		printl("YOU_CANNOT_DECIPHER_WHAT_IT_CONTROLS");
	else
		printl(Common::String::format("CABINET_%d", obj->quality).c_str());
}

void MDScripts::lookTrackingMotor(Obj *obj) {
	if ((obj->quality & 1) && obj->is_on_map())
		printl("IT_APPEARS_TO_BE_LOOSE");

	if (obj->quality & 2)
		printl("IT_APPEARS_TO_BE_BROKEN");
}

void MDScripts::lookPanel(Obj *obj) {
	switch (obj->qty) {
	case 0:
		if (obj->frame_n == 0)
			printl("THESE_APPEAR_TO_BE_MECHANICAL_CONTROLS");
		else if (obj->frame_n == 1)
			printl("THESE_APPEAR_TO_BE_VALVE_CONTROLS");
		else if (obj->frame_n == 2)
			printl("THESE_APPEAR_TO_BE_ELECTRICAL_CONTROLS");
		break;
	case 4:
		printl("IT_APPEARS_TO_CONTROL_A_DREAM_MACHINE");
		break;
	case 5:
		printl("IT_APPEARS_TO_CONTROL_A_SPRAY_SYSTEM");
		break;
	case 6:
		printl("IT_APPEARS_TO_CONTROL_THE_RUBY_LENS_SYSTEM");
		break;
	case 7:
		printl("IT_APPEARS_TO_CONTROL_THE_CISTERN_VALVES");
		break;
	default:
		printl("YOU_CANNOT_DECIPHER_ITS_PURPOSE");
		break;
	}

	if ((obj->quality & 1) && obj->is_on_map())
		printl("THE_PANEL_IS_LOOSE");

	if (obj->quality & 2)
		printl("THE_PANEL_IS_BROKEN");
}

void MDScripts::printNumberOfCharges(int qty) {
	if (qty == 1)
		printl("AND_HAS_1_CHARGE_LEFT");
	else
		printfl("AND_HAS_N_CHARGES_LEFT", qty);
}

void MDScripts::lookPortableSprayer(Obj *obj) {
	printfl("IT_IS_LOADED_WITH", tileDescription(obj->quality == 0 ? 649 : 640));
	printNumberOfCharges(obj->qty);
}

const char *MDScripts::getWeaponModeString(const Obj *obj) {
	if (obj->quality == 0)
		return _lang.get("COMBINATION");
	if (obj->quality == 1)
		return _lang.get("RIFLE");

	return _lang.get("SHOTGUN");
}

void MDScripts::lookRayGun(Obj *obj) {
	printfl("IT_IS_SET_TO", getWeaponModeString(obj));
	printNumberOfCharges(obj->qty);
}

void MDScripts::lookBelgianCombine(Obj *obj) {
	printfl("IT_IS_SET_TO", getWeaponModeString(obj));
	print(".\n");
}

void MDScripts::lookSwitchBar(Obj *obj) {
	if (obj->quality == 1)
		printl("IT_SEEMS_TO_BE_LOOSE");
	else if (obj->quality == 2)
		printl("IT_IS_NOT_INSTALLED");
}

void MDScripts::lookLightSource(Obj *obj) {
	uint16 objN = obj->obj_n;
	uint8 quality = obj->quality;

	// Oil lamps and lanterns are fuelled, the rest have wicks
	bool fuelled = objN == 115 || objN == 117 || objN == 116 || objN == 118;

	if ((objN == 109 || objN == 110) && obj->qty > 1)
		return;

	if (quality > 30)
		printl("PLENTY_OF");
	else if (quality > 6)
		printfl("IT_HAS_N_MORE_MINUTES_OF", (int)quality);
	else if (quality > 0)
		printl("ITS_ALMOST_OUT_OF");
	else if (fuelled)
		printl("NO");
	else
		printl("PLENTY_OF");

	printl(fuelled ? "FUEL" : "WICK");
}

void MDScripts::lookDoor(Obj *obj) {
	int quality = obj->quality;
	if (quality < 128)
		return;

	printl(quality - 128 >= 64 ? "IT_IS_APPARENTLY_RUSTED_SHUT" : "IT_IS_APPARENTLY_LOCKED");
}

void MDScripts::lookObelisk(Obj *obj) {
	if (obj->quality == 0)
		return;

	ObeliskView view;
	view.show(obj->quality - 1);
}

bool MDScripts::lookUsecode(Obj *obj) {
	static const struct {
		uint16 _objN;
		ObjFunc _func;
	} LOOK_TABLE[] = {
		{ 45, &MDScripts::lookBelgianCombine },
		{ 91, &MDScripts::lookPocketwatch },
		{ 98, &MDScripts::lookPocketwatch },
		{ 109, &MDScripts::lookLightSource }, // OBJ_TORCH
		{ 110, &MDScripts::lookLightSource }, // OBJ_LIT_TORCH
		{ 111, &MDScripts::lookLightSource }, // OBJ_CANDLESTICK
		{ 112, &MDScripts::lookLightSource }, // OBJ_LIT_CANDLE
		{ 113, &MDScripts::lookLightSource }, // OBJ_CANDELABRA
		{ 114, &MDScripts::lookLightSource }, // OBJ_LIT_CANDELABRA
		{ 115, &MDScripts::lookLightSource }, // OBJ_OIL_LAMP
		{ 116, &MDScripts::lookLightSource }, // OBJ_LIT_OIL_LAMP
		{ 117, &MDScripts::lookLightSource }, // OBJ_LANTERN
		{ 118, &MDScripts::lookLightSource }, // OBJ_LIT_LANTERN
		{ 129, &MDScripts::lookPortableSprayer }, // OBJ_WEED_SPRAYER
		{ 172, &MDScripts::lookMarkerFlag },
		{ 179, &MDScripts::lookDoor }, // OBJ_CLOSED_DOOR
		{ 227, &MDScripts::lookDoor }, // OBJ_DOOR3
		{ 240, &MDScripts::lookRayGun }, // OBJ_HEAT_RAY_GUN
		{ 241, &MDScripts::lookRayGun }, // OBJ_FREEZE_RAY_GUN
		{ 251, &MDScripts::lookCoveredMartianSeed },
		{ 261, &MDScripts::lookPortableSprayer }, // OBJ_SPRAY_GUN
		{ 268, &MDScripts::lookBarrow }, // OBJ_MARTIAN_WHEEL_BARROW
		{ 276, &MDScripts::lookSprayerSystem },
		{ 287, &MDScripts::lookMetalWoman },
		{ 292, &MDScripts::lookObelisk },
		{ 314, &MDScripts::lookTrackingMotor },
		{ 333, &MDScripts::lookCannon },
		{ 410, &MDScripts::lookBarrow }, // OBJ_RAIL_CAR
		{ 411, &MDScripts::lookSwitchBar },
		{ 457, &MDScripts::lookCabinet },
		{ 458, &MDScripts::lookPanel },
		{ 460, &MDScripts::lookBrokenStrap }
	};

	for (uint i = 0; i < ARRAYSIZE(LOOK_TABLE); ++i) {
		if (LOOK_TABLE[i]._objN == obj->obj_n) {
			(this->*LOOK_TABLE[i]._func)(obj);
			return true;
		}
	}

	return false;
}

bool MDScripts::lookObj(Obj *obj) {
	Game *game = Game::get_game();

	printfl("YOU_SEE", game->get_obj_manager()->look_obj(obj, true));

	if (game->get_usecode()->look_obj(obj, game->get_player()->get_actor())) {
		print("\n");
		return false;
	}

	print(".\n\n");

	if (lookUsecode(obj))
		print("\n");

	if (isContainerObj(obj->obj_n))
		search(obj);

	return false;
}

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
