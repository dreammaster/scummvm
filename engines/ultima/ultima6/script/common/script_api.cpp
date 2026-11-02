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

#include "ultima/ultima6/actors/actor.h"
#include "ultima/ultima6/core/converse.h"
#include "ultima/ultima6/core/effect.h"
#include "ultima/ultima6/files/u6_lib_n.h"
#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/map.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/core/tile_manager.h"
#include "ultima/ultima6/gui/widgets/msg_scroll.h"
#include "ultima/ultima6/misc/u6_llist.h"
#include "ultima/ultima6/script/common/game_scripts.h"
#include "ultima/ultima6/views/view_manager.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {

void GameScripts::hitAnim(int x, int y) {
	AsyncEffect *effect = new AsyncEffect(new HitEffect(MapCoord(x, y)));
	effect->run();
}

void GameScripts::projectileAnim(uint16 tileNum, int startX, int startY, int targetX, int targetY,
		int speed, bool trail, int initialTileRotation) {
	ProjectileEffect *projectile = new ProjectileEffect(tileNum, MapCoord(startX, startY),
		MapCoord(targetX, targetY), speed, trail, initialTileRotation, 0, 0);
	AsyncEffect *effect = new AsyncEffect(projectile);
	effect->run();
}

void GameScripts::projectileAnimMulti(uint16 tileNum, int startX, int startY, const Common::Array<MapCoord> &targets,
		int speed, bool trail, int initialTileRotation, Common::Array<MapEntity> &hitEntities) {
	ProjectileEffect *projectile = new ProjectileEffect(tileNum, MapCoord(startX, startY), targets, speed, trail,
		initialTileRotation);
	AsyncEffect *effect = new AsyncEffect(projectile);
	effect->run();

	hitEntities = *projectile->get_hit_entities();
}

void GameScripts::findObjs(int level, int objN, Common::Array<Obj *> &objs) {
	ObjManager *objManager = Game::get_game()->get_obj_manager();

	for (Obj *obj = objManager->find_obj(level, objN, 0, OBJ_NOMATCH_QUALITY); obj;
			obj = objManager->find_next_obj(level, obj, OBJ_NOMATCH_FRAME_N, OBJ_NOMATCH_QUALITY))
		objs.push_back(obj);
}

void GameScripts::quakeStart(int magnitude, int duration) {
	new QuakeEffect(magnitude, duration, Game::get_game()->get_player()->get_actor());
}

void GameScripts::fadeOutEffect() {
	AsyncEffect *effect = new AsyncEffect(new FadeEffect(FADE_PIXELATED, FADE_OUT));
	effect->run();
}

void GameScripts::fadeInEffect() {
	AsyncEffect *effect = new AsyncEffect(new FadeEffect(FADE_PIXELATED, FADE_IN));
	effect->run();
}

void GameScripts::displayPrompt(bool newline) {
	MsgScroll *scroll = Game::get_game()->get_scroll();
	if (!scroll->can_display_prompt())
		return;

	if (newline)
		scroll->display_string("\n");
	scroll->display_prompt();
}

void GameScripts::actorTalk(Actor *actor) {
	Game::get_game()->get_converse()->start(actor);
	Game::get_game()->update_until_converse_finished();
}

void GameScripts::displayTextInScrollGump(const Common::String &text) {
	Game *game = Game::get_game();

	if (game->is_new_style())
		game->get_view_manager()->open_scroll_gump(text.c_str(), text.size());
	else
		game->get_scroll()->message(text.c_str());
}

bool GameScripts::loadTextFromLzc(const char *filename, int idx, Common::String &text) {
	U6Lib_n lib;
	Common::Path path;

	config_get_path(Game::get_game()->get_config(), filename, path);
	if (!lib.open(path, 4, NUVIE_GAME_MD) || idx >= (int)lib.get_num_items())
		return false;

	unsigned char *buf = lib.get_item(idx, nullptr);
	if (!buf)
		return false;

	int len = lib.get_item_size(idx);
	lib.close();

	if (len < 1 || buf[len - 1] != 0) {
		free(buf);
		return false;
	}

	if (len >= 2 && buf[len - 2] == 0xff)
		buf[len - 2] = 0;

	text = (const char *)buf;
	free(buf);
	return true;
}

bool GameScripts::mapCanReachPoint(int x, int y, int x1, int y1, uint8 level) {
	LineTestResult result;

	// FIXME world wrapping for MD
	return !Game::get_game()->get_game_map()->lineTest(x, y, x1, y1, level, LT_HitMissileBoundary, result);
}

float GameScripts::objectWeight(Obj *obj) {
	float weight = Game::get_game()->get_obj_manager()->get_obj_weight(obj, OBJ_WEIGHT_INCLUDE_CONTAINER_ITEMS,
		OBJ_WEIGHT_DONT_SCALE);

	// Gets rid of the tiny fraction, then scales
	return floorf(weight) / 10;
}

static void addInventoryObjs(U6LList *list, bool recursive, Common::Array<Obj *> &objs) {
	for (U6Link *link = list->start(); link; link = link->next) {
		Obj *obj = (Obj *)link->data;
		objs.push_back(obj);

		if (recursive && obj->container)
			addInventoryObjs(obj->container, true, objs);
	}
}

void GameScripts::getInventoryObjs(Actor *actor, bool recursive, Common::Array<Obj *> &objs) {
	U6LList *inventory = actor->get_inventory_list();
	if (inventory)
		addInventoryObjs(inventory, recursive, objs);
}

void GameScripts::removeFromEngine(Obj *obj) {
	Game::get_game()->get_obj_manager()->unlink_from_engine(obj);

	if (obj->get_engine_loc() == OBJ_LOC_NONE)
		delete_obj(obj);
}

uint16 GameScripts::objOriginalTileNum(const Obj *obj) {
	Game *game = Game::get_game();
	return game->get_tile_manager()->get_original_tile(
		game->get_obj_manager()->get_obj_tile_num(obj->obj_n) + obj->frame_n)->tile_num;
}

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
