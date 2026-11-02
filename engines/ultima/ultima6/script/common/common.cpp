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
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/party.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/core/tile_manager.h"
#include "ultima/ultima6/files/tmx_map.h"
#include "ultima/ultima6/gui/widgets/msg_scroll.h"
#include "ultima/ultima6/script/common/cutscene_script.h"
#include "ultima/ultima6/script/common/game_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {

enum {
	UI_STYLE_ORIG = 0
};

namespace {

/** Parses a whole string as a number, the way tonumber() does */
bool parseNumber(const Common::String &str, int base, int &value) {
	if (str.empty())
		return false;

	char *end;
	value = (int)strtol(str.c_str(), &end, base);
	return *end == '\0';
}

class MidgameSequence : public CutsceneScript {
public:
	uint8 play(uint16 seqNum);
};

uint8 MidgameSequence::play(uint16 seqNum) {
	Game *game = Game::get_game();
	bool originalStyle = game->get_game_style() == UI_STYLE_ORIG;

	_cutscene->moveToFront();
	_cutscene->hide_sprites();
	_cutscene->set_screen_opacity(0xff);
	_cutscene->set_update_interval(25);
	_cutscene->enable_game_palette_rotation(true);

	CSSprite *bg = newSprite(nullptr, 8, 16, true);
	CSSprite *avatar = newSprite(nullptr, 8, 16, false);
	CSSprite *textSprite = nullptr;

	if (originalStyle) {
		_cutscene->set_solid_bg(false);
	} else {
		textSprite = newSprite(nullptr, 8, 160, true);
		textSprite->text_align = 2;
		textSprite->text_color = 15;
		bg->x = 80;
		bg->y = 12;
		avatar->x = 80;
		avatar->y = 12;
	}

	Common::Array<CSMidGameData> data = _cutscene->load_midgame_file(
		Common::String::format("midgame%x.lzc", seqNum).c_str());

	for (uint i = 0; i < data.size(); ++i)
		adoptImages(data[i].images);

	for (uint i = 0; i < data.size(); ++i) {
		const CSMidGameData &frame = data[i];

		if (!frame.images.empty())
			bg->image = frame.images[0];

		if (frame.images.size() > 1) {
			uint gender = game->get_player()->get_gender();
			uint last = frame.images.size() - 1;
			avatar->image = frame.images[MIN(1 + gender, last)];
			avatar->visible = true;
		} else {
			avatar->visible = false;
		}

		for (uint j = 0; j < frame.text.size(); ++j) {
			if (frame.text[j] == "*END*")
				continue;

			if (originalStyle) {
				game->get_scroll()->clear_scroll();
				game->get_scroll()->display_string(frame.text[j]);
			} else {
				textSprite->text = frame.text[j];
			}

			waitForInput();
		}
	}

	if (originalStyle)
		game->get_scroll()->clear_scroll();

	_cutscene->set_solid_bg(true);
	_cutscene->enable_game_palette_rotation(false);
	_cutscene->Hide();
	return 0;
}

} // End of anonymous namespace

uint8 GameScripts::playMidgameSequence(uint16 seqNum) {
	MidgameSequence sequence;
	return sequence.play(seqNum);
}

Common::String GameScripts::inputSelect(const char *allowedChars, bool canEscape) const {
	TextInputEffect *inputEffect = new TextInputEffect(allowedChars, canEscape);
	AsyncEffect *effect = new AsyncEffect(inputEffect);
	effect->run(EFFECT_PROCESS_GUI_INPUT);

	return inputEffect->get_input();
}

int GameScripts::inputSelectInteger(bool canEscape) const {
	return (int)strtol(inputSelect(nullptr, canEscape).c_str(), nullptr, 10);
}

bool GameScripts::inputShouldProceed() const {
	Common::String input = inputSelect("yn", true);
	print("\n");

	return input != "N" && input != "n";
}

bool GameScripts::handleAltCode(uint16 altCode) {
	switch (altCode) {
	case 242:
		altCodeSetActorTalkFlag();
		break;
	case 250:
		altCodeCreateObject();
		break;
	case 913:
		altCodeExportTmxMapFiles();
		break;
	case 914:
		altCodeExportTileset();
		break;
	case 999:
		altCodeFindObjsOnMap();
		break;
	default:
		break;
	}

	return true;
}

void GameScripts::altCodeSetActorTalkFlag() {
	print("NPC: ");

	int actorNum;
	if (!parseNumber(inputSelect(nullptr, true), 16, actorNum))
		return;

	Actor *actor = Game::get_game()->get_actor_manager()->get_actor(actorNum);
	if (!actor)
		return;

	print(Common::String("\n") + actor->get_name() + "\n");
	print("flags: \n");
	print("\nBit: ");
	uint8 bit = (uint8)inputSelectInteger(true);

	print(Common::String(" is ") + (actor->get_flag(bit) ? "on" : "off") + ".\n");
	print("New value? ");

	if (inputSelectInteger(true) == 1) {
		actor->set_flag(bit);
		print("\non\n");
	} else {
		actor->clear_flag(bit);
		print("\noff\n");
	}
}

void GameScripts::altCodeCreateObject() {
	ObjManager *objManager = Game::get_game()->get_obj_manager();

	print("Create Item:\nType:0x");
	int objN = 0;
	parseNumber(inputSelect(nullptr, true), 16, objN);

	Obj *obj = newObj(objN);
	Obj *nextObj = newObj(objN + 1);
	bool hasFrames = objManager->get_obj_tile(nextObj->obj_n, nextObj->frame_n)->tile_num -
		objManager->get_obj_tile(obj->obj_n, obj->frame_n)->tile_num > 1;
	delete_obj(nextObj);

	int value = 0;
	if (hasFrames) {
		print("\nFrame:0x");
		parseNumber(inputSelect(nullptr, true), 16, value);
		obj->frame_n = (uint8)value;
	}

	print("\nQual:0x");
	value = 0;
	parseNumber(inputSelect(nullptr, true), 16, value);
	obj->quality = (uint8)value;

	if (objManager->is_stackable(obj) || createObjectNeedsQuan(objN)) {
		print("\nQuan:0x");
		value = 0;
		parseNumber(inputSelect(nullptr, true), 16, value);
		obj->qty = (uint8)value;
	}

	if (!objManager->moveto_inventory(obj, 1))
		delete_obj(obj);

	print("\n");
}

void GameScripts::altCodeExportTmxMapFiles() {
	print("\nExport maps to savedir? ");

	if (!inputShouldProceed())
		return;

	print("saving.\n");
	wait(1);

	Game *game = Game::get_game();
	TMXMap *tmxMap = new TMXMap(game->get_tile_manager(), game->get_game_map(), game->get_obj_manager());
	bool success = tmxMap->exportTmxMapFiles("data", game->get_game_type());
	delete tmxMap;

	print(success ? "done.\n\n" : "error!!\n\n");
}

void GameScripts::altCodeExportTileset() {
	Game *game = Game::get_game();
	const char *gameTag = get_game_tag(game->get_game_type());

	print(Common::String("Exporting tileset to \"data/images/tiles/") + gameTag +
		"/custom_tiles.bmp\" in the savegame directory.\n");

	Common::Path path("data");
	build_path(path, "images", path);
	build_path(path, "tiles", path);
	build_path(path, gameTag, path);

	if (!directory_exists(path))
		mkdir_recursive(path, 0700);

	build_path(path, "custom_tiles.bmp", path);

	if (file_exists(path)) {
		print("file already exists. Overwrite? ");
		if (!inputShouldProceed())
			return;
	}

	game->get_tile_manager()->exportTilesetToBmpFile(path, false);
	print("done.\n\n");
}

void GameScripts::altCodeFindObjsOnMap() {
	Game *game = Game::get_game();

	print("Find Object\nObj_n: ");
	int objN, frameN, quality, z;
	if (!parseNumber(inputSelect(nullptr, true), 10, objN)) {
		print("Nothing.\n");
		return;
	}

	print("\nFrame: ");
	bool matchFrameN = parseNumber(inputSelect(nullptr, true), 10, frameN);
	print("\nQuality: ");
	bool matchQuality = parseNumber(inputSelect(nullptr, true), 10, quality);
	print("\nz: ");
	if (!parseNumber(inputSelect(nullptr, true), 10, z))
		z = 0;
	print("\n");

	ObjManager *objManager = game->get_obj_manager();
	Obj *obj = objManager->find_obj(z, objN, matchQuality ? quality : 0, matchQuality,
		matchFrameN ? frameN : 0, matchFrameN);

	while (obj) {
		print(Common::String::format("OBJ: (%x,%x,%x)\n", obj->x, obj->y, obj->z));
		game->get_party()->move(obj->x, obj->y, obj->z);
		print("continue? ");

		Common::String input = inputSelect("yn", false);
		print("\n");
		if (input == "N" || input == "n")
			return;

		obj = objManager->find_next_obj(z, obj, matchFrameN, matchQuality);
	}
}

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
