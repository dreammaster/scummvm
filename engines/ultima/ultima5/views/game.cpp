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

#include "ultima/ultima5/views/game.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/logic/location_logic.h"
#include "ultima/ultima5/logic/npc_movement.h"
#include "ultima/ultima5/logic/party_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/logic/transport_logic.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/logic/monster_logic.h"
#include "ultima/ultima5/views/interactions/attack.h"
#include "ultima/ultima5/views/interactions/camp.h"
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/views/interactions/combat.h"
#include "ultima/ultima5/views/interactions/confront.h"
#include "ultima/ultima5/views/interactions/dungeon_climb.h"
#include "ultima/ultima5/views/interactions/cast.h"
#include "ultima/ultima5/data/shrine_data.h"
#include "ultima/ultima5/views/interactions/death.h"
#include "ultima/ultima5/views/interactions/dungeon_item.h"
#include "ultima/ultima5/views/interactions/gem_view.h"
#include "ultima/ultima5/views/interactions/object_command.h"
#include "ultima/ultima5/views/interactions/shrine.h"
#include "ultima/ultima5/views/interactions/troll_toll.h"
#include "ultima/ultima5/views/interactions/yell.h"
#include "ultima/ultima5/views/interactions/leave_location.h"
#include "ultima/ultima5/views/interactions/look.h"
#include "ultima/ultima5/views/interactions/new_order.h"
#include "ultima/ultima5/views/interactions/ready.h"
#include "ultima/ultima5/views/interactions/talk.h"
#include "ultima/ultima5/views/interactions/use.h"
#include "ultima/ultima5/views/interactions/ztats.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

// The id of the map of Britannia and the underworld
constexpr byte OUTDOOR_MAP = 0;
constexpr int AMBIENT_RANGE = 0x33;
constexpr byte TILE_CLOCK = 0xFA;
constexpr byte TILE_WATERFALL = 0xD4;
constexpr byte TILE_FOUNTAIN = 0xD8;
constexpr byte TILE_HARPSICHORD = 0x8D;
constexpr int CASTLE_MAP = 0x11;
constexpr int CASTLE_UPPER_LEVEL = 2;
constexpr int HIDDEN_DOOR_X = 0x11;
constexpr int HIDDEN_DOOR_Y = 0x0D;
constexpr byte HIDDEN_DOOR_TOGGLE = 0x0B;
constexpr byte ASK_TO_ATTACK = 0x61;

// How long a command takes
constexpr int MINUTES_PER_TURN = 2;
constexpr int FRAMES_PER_SAIL_STEP = 6;
constexpr byte TILE_ROUGH_SEAS = 1;
constexpr byte TILE_CAVE = 0x16;
constexpr byte TILE_MINE = 0x17;
constexpr byte TILE_DUNGEON = 0x18;
constexpr byte TILE_POISON_GROUND = 4;
constexpr byte TILE_LAVA = 0x8F;
constexpr byte TILE_FIREPLACE = 0xBC;
constexpr int EARTHQUAKE_ROLL = 0x69;
constexpr byte TILE_BED = 0xAB;
constexpr byte TILE_SKIFF = 0x28;
constexpr byte SHIP_FIRST = 0x20;
constexpr int SLEEP_TURN_LIMIT = 200;
constexpr byte TILE_TROLL_BRIDGE = 0x6A;
constexpr byte PARTY_ON_FOOT = 0x1C;
constexpr int MOON_CELLS = 12;
constexpr int MOON_COLUMN = 6;
constexpr int SUN_HOUR = 17;
constexpr int TRAMMEL_HOUR = 8;
constexpr int FELUCCA_HOUR = 2;
constexpr int HOURS_IN_DAY = 24;
constexpr byte COLOR_YELLOW = 14;
constexpr byte COLOR_LIGHT_GRAY = 7;
constexpr byte CGA_WHITE = 3;
constexpr byte TILE_MOONGATE = 0xDC;
constexpr int MOONGATE_SHRINE_MINUTES = 10;
constexpr byte TILE_SHRINE = 0x19;
constexpr byte TILE_CODEX = 0x11;
constexpr byte TILE_CARPET = 0x14;
constexpr int MINUTES_PER_LOCATION_TURN = 1;
constexpr int FIRST_NIGHT_HOUR = 20;
constexpr int FIRST_DAY_HOUR = 5;

// Maps from the first dungeon up are not out in the open, and neither is a certain one
constexpr byte FIRST_DUNGEON_MAP = 0x21;
constexpr byte NO_WIND_MAP = 0x19;

// The windows on the right of the screen, in cells
constexpr int PANEL_COLUMN = 24;
constexpr int PANEL_WIDTH = 15;
constexpr int VISIBLE_PARTY_MEMBERS = 6;
constexpr int NAME_WIDTH = 9;
constexpr int STATUS_ROW = 8;
constexpr int DATE_ROW = 9;
constexpr int ICON_COLUMN = 30;
constexpr int ICON_ROW = 7;
constexpr int GOLD_COLUMN = 8;

// The message window, in pixels
constexpr int LOG_LEFT = 192;
constexpr int LOG_TOP = 88;
constexpr int LOG_BOTTOM = 192;

constexpr byte GLYPH_ACTIVE_MARKER = 0x1A;
constexpr byte GLYPH_CORNER_TOP_LEFT = 0x7B;
constexpr byte GLYPH_CORNER_TOP_RIGHT = 0x7C;
constexpr byte GLYPH_CORNER_BOTTOM_LEFT = 0x7D;

// The wind caption is in the bottom border, and is shown in place of the
// blank border below ground
constexpr int WIND_COLUMN = 6;
constexpr int WIND_ROW = 23;
static const char *const WIND_NAMES[] = { "Calm ", "North", "South", "East ", "West " };

// The map window, in pixels, and where the party is in it
constexpr int VIEW_X = 8;
constexpr int VIEW_Y = 8;
constexpr int PARTY_TILE_X = 5;
constexpr int PARTY_TILE_Y = 5;
constexpr int ACTOR_TILE_BASE = 0x100;

constexpr byte TILE_UNSEEN = 0xFF;
constexpr byte TILE_BLOCKED_VIEW = 0x87;
constexpr byte TILE_CHAIR = 0x92;
constexpr byte TILE_OBSTRUCTED_A = 0x0A;
constexpr byte TILE_OBSTRUCTED_B = 0xEC;
constexpr byte TILE_FLOOR_UNDER = 0x57;
constexpr byte TILE_FLOOR_OVER = 0x38;
constexpr byte TILE_WATER_EDGE_A = 0x6A;
constexpr byte TILE_WATER_EDGE_B = 0x6B;

constexpr byte ACTOR_SLEEPING = 0x1E;
constexpr byte ACTOR_DEAD = 0x1F;
constexpr byte ACTOR_SLEEPING_ALT = 0x1D;
constexpr byte ACTOR_REMAINS = 0xE8;
constexpr byte ACTOR_MUSICIAN = 0x5C;
constexpr byte ACTOR_MUSICIAN_FRAME_OFFSET = 8;

// Gives the tiles of the map the party is on to work out what it can see
class MapTileSource : public TileSource {
public:
	byte tileAt(int x, int y) const override {
		return g_engine->getMapTile(x, y);
	}
};

Game::Game() : View("Game"), _messages(this) {
	_messages.setBounds(Common::Rect(LOG_LEFT, LOG_TOP, 320, LOG_BOTTOM));
	delayFrames(FRAMES_PER_SAIL_STEP);
}

// The tune that a bard plays, as notes of the scale, and what they are
static bool bardAt(int x, int y) {
	const Data::Savegame &sg = _G(savegame);

	for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord actor = sg.getActor(i);
		if (actor._tile == ACTOR_MUSICIAN && actor._x == x && actor._y == y && actor._z == sg._mapLevel)
			return true;
	}

	return false;
}

static const byte BARD_TUNE[53] = {
	1, 4, 4, 0, 0, 1, 5, 5, 0, 0, 4, 9, 6, 9, 7, 4,
	6, 5, 1, 4, 0, 0, 0, 1, 5, 0, 0, 0, 4, 9, 6, 5,
	6, 4, 0, 0, 0, 5, 8, 8, 9, 5, 6, 8, 9, 5, 4, 6,
	5, 4, 3, 2, 1
};
static const uint16 BARD_NOTES[9] = { 0x0DA9, 0x0F56, 0x1136, 0x123C, 0x1478, 0x16FA, 0x1857, 0x19CA, 0x1B53 };

void Game::ambientSound() {
	const Data::Savegame &sg = _G(savegame);
	int found = 0;
	int closest = AMBIENT_RANGE;

	for (int x = sg._mapX - 5; x < sg._mapX + 6; ++x) {
		for (int y = sg._mapY - 5; y < sg._mapY + 6; ++y) {
			const int distance = (y - sg._mapY) * (y - sg._mapY) + (x - sg._mapX) * (x - sg._mapX);
			if (distance >= closest)
				continue;

			const byte tile = g_engine->getMapTile(x, y);
			if ((tile & 0xFE) == TILE_CLOCK) {
				found = 1;
			} else if ((tile & 0xFC) == TILE_WATERFALL) {
				found = 2;
			} else if ((tile & 0xFC) == TILE_FOUNTAIN) {
				found = 3;
			} else if (bardAt(x, y)) {
				found = 4;
			} else {
				continue;
			}

			closest = distance;
		}
	}

	// A clock strikes the hour, and otherwise ticks and tocks
	if (_clockHour != sg._hour) {
		_clockHour = sg._hour;
		_clockStrikes = sg._hour == 0 ? 12 : sg._hour > 12 ? sg._hour - 12 : sg._hour;
	}

	Sound &sound = g_engine->sound();
	const bool beat = _clockBeat == 0 || _clockBeat == 4;

	switch (found) {
	case 1:
		if (_clockStrikes != 0 && beat)
			sound.pulse(0xC2C, 1, 2000, 20000, -10);
		else if (_clockBeat == 0)
			sound.tone(3000, 3);
		else if (_clockBeat == 4)
			sound.tone(2000, 3);
		break;

	case 2:
		sound.noise(0x14, 0x3C, 10000);
		break;

	case 3:
		sound.noise(10, 0x1E, 25000);
		break;

	case 4:
		if (BARD_TUNE[_bardNote] != 0)
			sound.pulse(BARD_NOTES[BARD_TUNE[_bardNote] - 1], 1, 2000, 20000, -10);

		if (++_bardNote >= (int)ARRAYSIZE(BARD_TUNE))
			_bardNote = 0;
		break;

	default:
		break;
	}

	if (_clockStrikes != 0 && beat)
		--_clockStrikes;

	if (++_clockBeat > 7)
		_clockBeat = 0;
}

void Game::timeout() {
	delayFrames(FRAMES_PER_SAIL_STEP);

	if (_interaction) {
		const int result = _interaction->tick();

		if (result == Interactions::TICK_DONE)
			interactionDone();
		else if (result == Interactions::TICK_REDRAW)
			redraw();

		return;
	}

	if (_G(savegame)._mapId != OUTDOOR_MAP && !DungeonLogic::isDungeon(_G(savegame)._mapId) && _G(savegame)._icon != 'T')
		ambientSound();

	// A ship that's been set sailing goes on without being told to
	if (_G(savegame)._mapId != OUTDOOR_MAP || TransportLogic::heading() < 0)
		return;

	if (TransportLogic::sailTick(_planTick)) {
		_sailingStep = true;
		walk(static_cast<Direction>(TransportLogic::heading()));
		_sailingStep = false;
	} else {
		redraw();
	}
}

void Game::draw() {
	if (!g_engine->loadMap())
		error("Could not load the map");

	drawFrame();

	if (DungeonLogic::isDungeon(_G(savegame)._mapId)) {
		drawDungeonFrame();
		_scene.draw(*this);
	} else {
		drawMap();
		drawMoons();
	}

	drawParty();
	drawStatus();
	drawWind();

	if (_interaction)
		_interaction->draw(*this);

	// The music is that of the interaction there is, or else that of where the party is
	int track = _interaction ? _interaction->musicTrack() : Interactions::MUSIC_AUTO;
	if (track == Interactions::MUSIC_AUTO)
		track = Music::trackForGameState(_G(savegame));

	if (track != Interactions::MUSIC_KEEP)
		g_engine->cueMusic(track);
}

void Game::drawDungeonFrame() {
	static const char *const DIRECTIONS[] = { "North", " East", "South", " West" };
	const Data::Savegame &sg = _G(savegame);
	auto s = getSurface();

	// The level and the way the party faces are set in the borders
	s.fillRect(Common::Rect(0x28, 0, 0x99, 8), COLOR_BLUE);
	s.fillRect(Common::Rect(0x30, 0xB9, 0x99, 0xC0), COLOR_BLUE);
	s.hLine(0x28, 7, 0x98, COLOR_WHITE);
	s.hLine(0x30, 0xB8, 0x98, COLOR_WHITE);

	print(10, 0, Common::String::format("L %d", sg._mapLevel + 1), COLOR_WHITE, COLOR_BLUE);
	print(6, 23, Common::String("Dir: ") + DIRECTIONS[DungeonLogic::facing() & 3], COLOR_WHITE, COLOR_BLUE);
}

void Game::drawFrame() {
	auto s = getSurface();
	s.clear();

	s.fillRect(Common::Rect(0, 0, 320, 7), COLOR_BLUE);
	s.fillRect(Common::Rect(0, 185, 192, 192), COLOR_BLUE);
	s.fillRect(Common::Rect(0, 0, 7, 192), COLOR_BLUE);
	s.fillRect(Common::Rect(185, 0, 192, 192), COLOR_BLUE);
	s.fillRect(Common::Rect(313, 0, 320, 88), COLOR_BLUE);
	s.fillRect(Common::Rect(192, 80, 313, 88), COLOR_BLUE);
	s.fillRect(Common::Rect(192, 0x39, 313, 64), COLOR_BLUE);

	drawCell(0, 0, GLYPH_CORNER_TOP_LEFT, COLOR_BLUE, COLOR_BLACK);
	drawCell(39, 0, GLYPH_CORNER_TOP_RIGHT, COLOR_BLUE, COLOR_BLACK);
	drawCell(0, 23, GLYPH_CORNER_BOTTOM_LEFT, COLOR_BLUE, COLOR_BLACK);

	// The map, the party, the status line and the messages
	s.frameRect(Common::Rect(7, 7, 185, 185), COLOR_WHITE);
	s.drawLine(191, 191, 191, 87, COLOR_WHITE);
	s.drawLine(191, 87, 319, 87, COLOR_WHITE);
	s.frameRect(Common::Rect(191, 7, 313, 57), COLOR_WHITE);
	s.frameRect(Common::Rect(191, 63, 313, 81), COLOR_WHITE);
}

void Game::drawParty() {
	const Data::Savegame &sg = _G(savegame);

	for (int i = 0; i < VISIBLE_PARTY_MEMBERS; ++i) {
		Common::String line;

		if (i < sg._partySize) {
			const Data::Character &c = sg._party[i];
			line = c._name;
			while ((int)line.size() < NAME_WIDTH)
				line += ' ';

			const bool active = i == sg._activeChar && c._status != Data::STATUS_DEAD &&
				c._status != Data::STATUS_SLEEP;
			line += active ? (char)GLYPH_ACTIVE_MARKER : ' ';
			line += Common::String::format("%4d", c._hp);
			line += (char)c._status;
		}

		while ((int)line.size() < PANEL_WIDTH)
			line += ' ';

		print(PANEL_COLUMN, 1 + i, line, COLOR_WHITE, COLOR_BLACK);
	}
}

void Game::drawStatus() {
	const Data::Savegame &sg = _G(savegame);

	// Food, then the gold lined up to its right
	Common::String food = Common::String::format("F:%d", sg._food);
	while ((int)food.size() < GOLD_COLUMN)
		food += ' ';

	Common::String gold;
	if (sg._gold < 1000)
		gold += ' ';
	if (sg._gold < 100)
		gold += ' ';
	if (sg._gold < 10)
		gold += ' ';
	gold += Common::String::format(" G:%d", sg._gold);

	Common::String line = food + gold;
	while ((int)line.size() < PANEL_WIDTH)
		line += ' ';
	print(PANEL_COLUMN, STATUS_ROW, line, COLOR_WHITE, COLOR_BLACK);

	// Month, day and year
	line = "   ";
	if (sg._month < 10 && sg._day < 10)
		line += ' ';
	line += Common::String::format("%d-%d-%03d", sg._month, sg._day, sg._year);
	while ((int)line.size() < PANEL_WIDTH)
		line += ' ';
	print(PANEL_COLUMN, DATE_ROW, line, COLOR_WHITE, COLOR_BLACK);

	// What the party is doing, in a cap-ended label over the border, or the plain border
	if (sg._icon != 0) {
		drawCaps(ICON_COLUMN * 8, ICON_ROW * 8, true);
		drawCell(ICON_COLUMN + 1, ICON_ROW, sg._icon, COLOR_WHITE, COLOR_BLACK);
		drawCaps((ICON_COLUMN + 2) * 8, ICON_ROW * 8, false);
	} else {
		auto s = getSurface();
		s.fillRect(Common::Rect(0xBF, 0x39, 0x139, 0x3F), COLOR_BLUE);
		s.hLine(0xC0, 0x38, 0x137, COLOR_WHITE);
		s.hLine(0xC0, 0x3F, 0x137, COLOR_WHITE);
	}
}

void Game::drawMoons() {
	const Data::Savegame &sg = _G(savegame);
	auto s = getSurface();

	if (sg._mapId == NO_WIND_MAP || sg._mapLevel >= Data::MAP_LEVEL_UNDERGROUND) {
		s.fillRect(Common::Rect(0x28, 0, 0x98, 6), COLOR_BLUE);
		s.hLine(0x28, 7, 0x98, COLOR_WHITE);
		return;
	}

	// The sun and the two moons cross the sky as the hours go by
	char cells[MOON_CELLS + 1];
	memset(cells, ' ', MOON_CELLS);
	cells[MOON_CELLS] = 0;

	const int sun = SUN_HOUR - sg._hour;
	if (sun >= 0 && sun < MOON_CELLS)
		cells[sun] = '*';

	const int day = (sg._day - 1) * 2;
	int trammel = TRAMMEL_HOUR - sg._hour;
	if (trammel < -MOON_CELLS)
		trammel += HOURS_IN_DAY;
	if (trammel >= 0 && trammel < MOON_CELLS)
		cells[trammel] = '0' + Data::MOON_PHASES[day];

	int felucca = FELUCCA_HOUR - sg._hour;
	if (felucca < -MOON_CELLS)
		felucca += HOURS_IN_DAY;
	if (felucca >= 0 && felucca < MOON_CELLS)
		cells[felucca] = '0' + Data::MOON_PHASES[day + 1];

	// The colours are those of the screen mode, which has only white of those to pick from in CGA
	const bool cga = g_engine->_renderMode == Data::RENDER_CGA;
	const byte sunColor = cga ? CGA_WHITE : COLOR_YELLOW;
	const byte moonColor = cga ? CGA_WHITE : COLOR_LIGHT_GRAY;

	for (int i = 0; i < MOON_CELLS; ++i)
		drawRune(MOON_COLUMN + i, 0, cells[i], cells[i] == '*' ? sunColor : moonColor, COLOR_BLUE);
}

void Game::drawWind() {
	const Data::Savegame &sg = _G(savegame);
	if (sg._mapId >= FIRST_DUNGEON_MAP || sg._mapId == NO_WIND_MAP)
		return;

	if (sg._mapLevel < Data::MAP_LEVEL_UNDERGROUND) {
		const Common::String text = Common::String(WIND_NAMES[MIN<int>(sg._windDirection, 4)]) + " Winds";

		drawCaps(WIND_COLUMN * 8, WIND_ROW * 8, true);
		print(WIND_COLUMN + 1, WIND_ROW, text, COLOR_WHITE, COLOR_BLACK);
		drawCaps((WIND_COLUMN + 1 + text.size()) * 8, WIND_ROW * 8, false);
	} else {
		auto s = getSurface();
		s.hLine(0x30, 0xB8, 0x98, COLOR_WHITE);
		s.fillRect(Common::Rect(0x30, 0xB9, 0x99, 0xC0), COLOR_BLUE);
	}
}

// Whether an object is one that's hidden by some of the terrain it's on
static bool isHideable(byte tile) {
	return tile == 0x1C || (tile >= 0x12 && tile < 0x16) || tile >= 0x40 || (tile >= 0x28 && tile < 0x2C);
}

void Game::placeObjects(byte objects[VIEW_TILES][VIEW_TILES], byte ground[VIEW_TILES][VIEW_TILES]) {
	const Data::Savegame &sg = _G(savegame);

	for (int idx = Data::ACTOR_COUNT - 1; idx >= 0; --idx) {
		const Data::ActorRecord actor = sg.getActor(idx);
		if (actor._tile == 0)
			continue;

		// The position in the window, which can wrap around the edge of the map
		const byte vx = actor._x - (sg._mapX - PARTY_TILE_X);
		const byte vy = actor._y - (sg._mapY - PARTY_TILE_Y);
		if (actor._z != sg._mapLevel || vx >= VIEW_TILES || vy >= VIEW_TILES)
			continue;

		byte frame = actor._animTile;
		if (frame == 0)
			continue;

		const byte under = ground[vy][vx];
		if (under == TILE_UNSEEN || under == TILE_BLOCKED_VIEW)
			continue;

		if ((actor._tile & 0xFC) == ACTOR_REMAINS || actor._tile == ACTOR_SLEEPING || actor._tile == ACTOR_DEAD) {
			// Remains stay beneath anything else
			if (objects[vy][vx] == 0)
				objects[vy][vx] = frame;
			continue;
		}

		if (frame == ACTOR_SLEEPING_ALT || frame == ACTOR_SLEEPING) {
			objects[vy][vx] = frame;
			continue;
		}

		if (actor._tile == ACTOR_MUSICIAN) {
			if (under == TILE_CHAIR) {
				objects[vy][vx] = frame;
				continue;
			}

			frame -= ACTOR_MUSICIAN_FRAME_OFFSET;
		}

		if (isHideable(frame)) {
			if (under == TILE_OBSTRUCTED_B || under == TILE_OBSTRUCTED_A)
				continue;

			if (under == TILE_FLOOR_UNDER) {
				ground[vy][vx] = TILE_FLOOR_OVER;
				continue;
			}

			if (under == TILE_WATER_EDGE_A || under == TILE_WATER_EDGE_B) {
				if ((frame & 0xF0) == 0x80 || (frame & 0xFC) == 0x28)
					continue;
			}
		}

		objects[vy][vx] = frame;
	}
}

void Game::drawMap() {
	Data::Savegame &sg = _G(savegame);
	WorldLogic::syncPartyObject(sg);

	byte objects[VIEW_TILES][VIEW_TILES] = {};
	byte ground[VIEW_TILES][VIEW_TILES];

	// Only what the party can see of the map is shown
	MapTileSource source;
	g_engine->_visibility.computeView(source, sg._chunkX, sg._chunkY, sg._mapX, sg._mapY, sg._light, ground);

	placeObjects(objects, ground);

	auto s = getSurface();
	for (int y = 0; y < VIEW_TILES; ++y) {
		for (int x = 0; x < VIEW_TILES; ++x) {
			const int tile = objects[y][x] != 0 ? ACTOR_TILE_BASE + objects[y][x] : ground[y][x];
			_G(tileset).drawTile(s, VIEW_X + x * Gfx::TILE_SIZE, VIEW_Y + y * Gfx::TILE_SIZE, tile);
		}
	}
}

void Game::startInteraction(Interactions::Interaction *interaction) {
	_interaction.reset(interaction);
	redraw();
}

void Game::startFight(int object) {
	AttackLogic::Fight fight;
	fight._started = true;
	fight._actor = object;
	AttackLogic(_messages).prepare(fight);

	Interactions::Combat *combat = new Interactions::Combat(_messages);
	combat->setAftermath(fight);

	if (!combat->start(fight._kind, fight._map, fight._lone) || combat->done()) {
		delete combat;
		redraw();
	} else {
		startInteraction(combat);
	}
}

bool Game::startCombat(int kind, int map, bool lone) {
	Interactions::Combat *combat = new Interactions::Combat(_messages);

	if (!combat->start(kind, map, lone)) {
		delete combat;
		return false;
	}

	if (combat->done())
		delete combat;
	else
		startInteraction(combat);

	return true;
}

void Game::endTurn() {
	Data::Savegame &sg = _G(savegame);

	// While all the party are asleep the turns go by on their own, until one wakes or something comes
	if (!_sleeping && PartyLogic::allAsleep()) {
		_sleeping = true;

		for (int i = 0; i < SLEEP_TURN_LIMIT && PartyLogic::allAsleep() && !_interaction; ++i) {
			_messages.print("Zzzzzz...\n");

			// Towns wake the sleepers with each turn already
			if (sg._mapId == OUTDOOR_MAP || DungeonLogic::isDungeon(sg._mapId))
				PartyLogic::wakeSleepers();

			endTurn();
		}

		_sleeping = false;
		return;
	}

	// When all are dead, Lord British takes them in
	if (DeathLogic::partyDead()) {
		startInteraction(new Interactions::Death(_messages));
		return;
	}

	if (DungeonLogic::isDungeon(sg._mapId)) {
		const bool timePassed = passDungeonTime();
		dungeonEvent(DungeonLogic(_messages).arrive(timePassed));
		return;
	} else if (sg._mapId == OUTDOOR_MAP) {
		TimeLogic::advance(sg, MINUTES_PER_TURN);
		TransportLogic::clearHeadingIfNotSailing();

		// Poisonous and burning ground, and the earth moving below
		const byte ground = g_engine->getMapTile(sg._mapX, sg._mapY);
		if (ground == TILE_POISON_GROUND && sg._partyTile == TILE_AVATAR)
			PartyLogic(_messages).stepOnPoison(1);
		else if (ground == TILE_LAVA)
			PartyLogic(_messages).stepOnFire();

		if (sg._mapLevel != 0 && _G(gameRandom).range(0, 0xFF) == EARTHQUAKE_ROLL) {
			_messages.print("EARTHQUAKE!\n");
			PartyLogic::hurtParty();
		}

		PartyLogic(_messages).turnUpkeep();

		// Small boats and carpets don't last in rough water
		if (g_engine->getMapTile(sg._mapX, sg._mapY) == TILE_ROUGH_SEAS &&
				((sg._partyTile & 0xFC) == TILE_SKIFF || (sg._partyTile & 0xFE) == TILE_CARPET)) {
			_messages.print("Rough seas!\n");
			TransportLogic(_messages).damage();
		}

		// A moongate takes the party to where the stone of the moons' phases lies, or at midnight to a shrine
		if (ground == TILE_MOONGATE) {
			_G(world).setTile(sg._mapX, sg._mapY, Data::TILE_GRASS);
			_G(visibility).invalidate();

			if (sg._hour == 0 && sg._minute < MOONGATE_SHRINE_MINUTES) {
				startInteraction(new Interactions::Shrine(_messages, false, false));
				return;
			}

			WorldLogic::jumpToMoonstone(sg, WorldLogic::gateMoonstone(sg));
		}

		// Trolls under a bridge may be there to be sneaked past
		if ((ground & 0xFE) == TILE_TROLL_BRIDGE && sg._partyTile == PARTY_ON_FOOT && _G(gameRandom).range(0, 7) == 0) {
			Interactions::TrollToll *toll = new Interactions::TrollToll(_messages);

			if (toll->done()) {
				delete toll;
			} else {
				startInteraction(toll);
				return;
			}
		}

		// The monsters have their turn
		const MonsterLogic::Result monsters = MonsterLogic(_messages).turn();
		if (monsters._fight) {
			startFight(monsters._actor);
			return;
		}
	} else {
		const byte hour = sg._hour;
		TimeLogic::advance(sg, MINUTES_PER_LOCATION_TURN);

		// The drawbridges and the windows change as it gets dark and light
		if (sg._hour != hour && (sg._hour == FIRST_NIGHT_HOUR || sg._hour == FIRST_DAY_HOUR))
			LocationLogic::updateForHour();

		// A door that was opened shuts again after a few turns
		if (sg._doorOpen != 0 && --sg._doorTurns == 0)
			ObjectItems::closeDoor();

		NpcMovement(sg, _G(location), _G(gameRandom)).turn();

		// What the ground does, and the turn's upkeep
		PartyLogic::wakeSleepers();

		const byte ground = g_engine->getMapTile(sg._mapX, sg._mapY);
		if (ground == TILE_POISON_GROUND && sg._partyTile == TILE_AVATAR)
			PartyLogic(_messages).stepOnPoison(0);
		else if (ground == TILE_FIREPLACE || ground == TILE_LAVA)
			PartyLogic(_messages).stepOnFire();

		PartyLogic(_messages).turnUpkeep();

		// Someone who has come up to the party speaks to it or sets upon it
		if (sg._unk1019 != 0) {
			Interactions::Confront *confront = new Interactions::Confront(_messages, sg._unk1019,
				sg._unk1018 == ASK_TO_ATTACK ? Interactions::Confront::KIND_ATTACK : Interactions::Confront::KIND_TALK);
			Interactions::Interaction *next = confront;

			if (confront->done()) {
				next = confront->takeNext();
				delete confront;
			}

			if (next) {
				startInteraction(next);
				return;
			}
		}
	}

	redraw();
}

bool Game::walk(Direction direction) {
	if (_G(savegame)._mapId == OUTDOOR_MAP) {
		// A ship that's sailing already ignores being told to go the way it's going
		if (TransportLogic::heading() == direction && !_sailingStep)
			return true;

		if (WorldLogic(_messages).walk(direction))
			endTurn();
		else
			redraw();

		return true;
	}

	switch (LocationLogic(_messages).walk(direction)) {
	case LocationLogic::WALK_MOVED:
		endTurn();
		break;
	case LocationLogic::WALK_AT_EDGE:
		startInteraction(new Interactions::LeaveLocation(_messages));
		break;
	default:
		redraw();
		break;
	}

	return true;
}

void Game::dungeonEvent(const DungeonLogic::Event &event) {
	if (event._kind == DungeonLogic::Event::EVENT_NONE) {
		redraw();
		return;
	}

	Interactions::Combat *combat = new Interactions::Combat(_messages);
	if (combat->startDungeon(event) && !combat->done()) {
		startInteraction(combat);
		return;
	}

	Interactions::Interaction *next = combat->takeNext();
	delete combat;

	if (next)
		startInteraction(next);
	else
		redraw();
}

bool Game::passDungeonTime() {
	Data::Savegame &sg = _G(savegame);

	// A minute goes by with each command, or each other one when quick
	if (sg._icon == 'Q') {
		_quickToggle = !_quickToggle;
		if (_quickToggle)
			TimeLogic::advance(sg, 1);

		return _quickToggle;
	}

	if (sg._icon != 'T') {
		TimeLogic::advance(sg, 1);
		return true;
	}

	return false;
}

void Game::objectCommand(ObjectItems::Kind kind, const char *prompt) {
	_messages.print(prompt);

	Interactions::ObjectCommand *command = new Interactions::ObjectCommand(_messages, kind);

	if (command->done()) {
		const bool time = command->takesTime();
		delete command;

		if (time)
			endTurn();
		else
			redraw();
	} else {
		startInteraction(command);
	}
}

void Game::castSpell() {
	_messages.print("Cast...\n");

	Interactions::Cast *cast = new Interactions::Cast(_messages);

	if (cast->done()) {
		const bool time = cast->takesTime();
		delete cast;

		if (time)
			endTurn();
		else
			redraw();
	} else {
		startInteraction(cast);
	}
}

void Game::mixReagents() {
	_messages.print("Mix Reagents\n\n");

	Interactions::Mix *mix = new Interactions::Mix(_messages);

	if (mix->done()) {
		delete mix;
		endTurn();
	} else {
		startInteraction(mix);
	}
}

void Game::viewGem() {
	Data::Savegame &sg = _G(savegame);

	_messages.print("View a gem!\n");

	if (sg._gems == 0) {
		_messages.print("You have none!\n");
		endTurn();
		return;
	}

	--sg._gems;
	startInteraction(new Interactions::GemView(_messages));
}

bool Game::handleDungeonCommand(const KeypressMessage &msg) {
	PartyLogic party(_messages);
	int key = 0;

	switch (msg.keycode) {
	case Common::KEYCODE_LEFT:
		key = 1;
		break;
	case Common::KEYCODE_RIGHT:
		key = 2;
		break;
	case Common::KEYCODE_UP:
		key = 3;
		break;
	case Common::KEYCODE_DOWN:
		key = 4;
		break;
	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
		key = 13;
		break;
	default:
		if (msg.ascii == '.')
			key = '.';
		break;
	}

	if (key != 0) {
		const DungeonLogic::Move move = DungeonLogic(_messages).command(key);

		const bool timePassed = passDungeonTime();
		if (move == DungeonLogic::MOVE_WALKED)
			dungeonEvent(DungeonLogic(_messages).arrive(timePassed));
		else
			redraw();

		return true;
	}

	switch (toupper(msg.ascii)) {
	case 'K': {
		Interactions::DungeonClimb *climb = new Interactions::DungeonClimb(_messages);

		if (climb->done()) {
			const bool time = climb->takesTime();
			delete climb;

			if (time)
				endTurn();
			else
				redraw();
		} else {
			startInteraction(climb);
		}
		return true;
	}

	case 'G':
	case 'O':
	case 'J':
	case 'S':
	case 'L': {
		const char ch = toupper(msg.ascii);
		_messages.print(ch == 'G' ? "" : ch == 'O' ? "Open-" : ch == 'J' ? "Jimmy-" : ch == 'S' ? "Search...\n" : "Look...\n");

		Interactions::DungeonItem *item = new Interactions::DungeonItem(_messages,
			ch == 'G' ? Interactions::DungeonItem::KIND_GET :
			ch == 'O' ? Interactions::DungeonItem::KIND_OPEN :
			ch == 'J' ? Interactions::DungeonItem::KIND_JIMMY :
			ch == 'S' ? Interactions::DungeonItem::KIND_SEARCH : Interactions::DungeonItem::KIND_LOOK);

		if (item->done()) {
			delete item;
			endTurn();
		} else {
			startInteraction(item);
		}
		return true;
	}

	case 'C':
		castSpell();
		return true;

	case 'M':
		mixReagents();
		return true;

	case 'V':
		viewGem();
		return true;

	case 'H': {
		Interactions::Camp *camp = new Interactions::Camp(_messages);

		if (camp->done()) {
			delete camp;
			redraw();
		} else {
			startInteraction(camp);
		}
		return true;
	}

	case 'Y':
		_messages.print("Yell ");
		startInteraction(new Interactions::Yell(_messages));
		return true;

	case 'F':
		_messages.print("Fire-What?\n");
		endTurn();
		return true;

	case 'I':
		party.igniteTorch();
		endTurn();
		return true;

	case 'R':
		_messages.print("Ready...\n\n");
		startInteraction(new Interactions::Ready(_messages));
		return true;

	case 'Z':
		_messages.print("Z-stats...\n");
		startInteraction(new Interactions::Ztats(_messages));
		return true;

	case 'U': {
		_messages.print("Use item\n\n");
		Interactions::Use *use = new Interactions::Use(_messages);

		if (use->done()) {
			delete use;
			endTurn();
		} else {
			startInteraction(use);
		}
		return true;
	}

	case 'N':
		_messages.print("New Order");
		startInteraction(new Interactions::NewOrder(_messages));
		return true;

	default:
		break;
	}

	if (msg.keycode == Common::KEYCODE_SPACE) {
		_messages.print("Pass\n");
		endTurn();
		return true;
	}

	return false;
}

// The notes of the harpsichord in the castle, which open the way to a hidden room when played as a tune
static const byte HARPSICHORD_TUNE[13] = { 6, 7, 8, 9, 8, 7, 8, 7, 6, 7, 6, 5, 3 };

bool Game::digitKey(int digit) {
	Data::Savegame &sg = _G(savegame);
	const int number = digit - '0';

	if (sg._mapId != OUTDOOR_MAP && !DungeonLogic::isDungeon(sg._mapId) && g_engine->getMapTile(sg._mapX, sg._mapY + 1) == TILE_HARPSICHORD) {
		static int progress = 0;

		g_engine->sound().harpsichord(number);

		if (HARPSICHORD_TUNE[progress] == number) {
			if (++progress == 13) {
				progress = 0;

				if (sg._mapId == CASTLE_MAP && sg._mapLevel == CASTLE_UPPER_LEVEL) {
					_G(location).setTile(HIDDEN_DOOR_X, HIDDEN_DOOR_Y, _G(location).getTile(HIDDEN_DOOR_X, HIDDEN_DOOR_Y) ^ HIDDEN_DOOR_TOGGLE);
					_G(visibility).invalidate();
				}
			}
		} else if (progress == 10 && number == 8) {
			progress = 3;
		} else if (progress == 11 && number == 7) {
			progress = 2;
		} else if (number == HARPSICHORD_TUNE[0]) {
			progress = 1;
		} else {
			progress = 0;
		}

		redraw();
		return true;
	}

	_messages.print("Set Active Plr:\n");

	if (number == 0) {
		_messages.print("None!\n");
		sg._activeChar = Data::NO_ACTIVE_CHARACTER;
		redraw();
		return true;
	}

	const int member = number - 1;
	if (member < sg._partySize && sg._party[member]._status != Data::STATUS_DEAD && sg._party[member]._status != Data::STATUS_SLEEP) {
		sg._activeChar = member;
		_messages.print(sg._party[member]._name);
		_messages.printChar('\n');
		redraw();
	} else {
		_messages.print("Invalid!\n");
		endTurn();
	}

	return true;
}

bool Game::handleCommand(const KeypressMessage &msg) {
	if (msg.ascii >= '0' && msg.ascii <= '9' && !(msg.flags & (Common::KBD_CTRL | Common::KBD_ALT)))
		return digitKey(msg.ascii);

	if (DungeonLogic::isDungeon(_G(savegame)._mapId))
		return handleDungeonCommand(msg);

	const bool outdoors = _G(savegame)._mapId == OUTDOOR_MAP;
	WorldLogic world(_messages);
	PartyLogic party(_messages);

	switch (msg.keycode) {
	case Common::KEYCODE_UP:
	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_DOWN:
	case Common::KEYCODE_LEFT:
		return walk(msg.keycode == Common::KEYCODE_UP ? DIR_NORTH :
			msg.keycode == Common::KEYCODE_RIGHT ? DIR_EAST :
			msg.keycode == Common::KEYCODE_DOWN ? DIR_SOUTH : DIR_WEST);

	case Common::KEYCODE_SPACE:
		world.pass();
		endTurn();
		return true;

	default:
		break;
	}

	switch (toupper(msg.ascii)) {
	case 'E':
		if (!outdoors)
			return false;

		_messages.print("Enter ");

		{
			const byte tile = _G(world).getTile(_G(savegame)._mapX, _G(savegame)._mapY);

			if (tile == TILE_SHRINE) {
				const int shrine = ShrineLogic::shrineAt(_G(savegame)._mapX, _G(savegame)._mapY);

				_messages.print("the shrine of\n");
				_messages.print(Data::SHRINE_VIRTUES[shrine]);
				_messages.printChar('\n');
				startInteraction(new Interactions::Shrine(_messages, false));
				return true;
			}

			if (tile == TILE_CODEX) {
				_messages.print("the Shrine of the Codex!\n");
				startInteraction(new Interactions::Shrine(_messages, true));
				return true;
			}
			const char *dungeon = tile == TILE_CAVE ? "cave" : tile == TILE_MINE ? "mine" : tile == TILE_DUNGEON ? "dungeon" : nullptr;

			if (dungeon) {
				DungeonLogic(_messages).enter(dungeon);
				redraw();
				return true;
			}
		}

		if (LocationLogic(_messages).enter())
			endTurn();
		else
			redraw();
		return true;

	case 'H': {
		if (!outdoors) {
			// In a town it's only in a bed
			_messages.print("Hole up- ");

			if (g_engine->getMapTile(_G(savegame)._mapX, _G(savegame)._mapY) != TILE_BED) {
				_messages.print("Only in bed!\n");
				redraw();
			} else {
				startInteraction(new Interactions::Bed(_messages));
			}
			return true;
		}

		Interactions::Camp *camp = new Interactions::Camp(_messages);

		if (camp->done()) {
			Interactions::Interaction *next = camp->takeNext();
			delete camp;

			if (next)
				startInteraction(next);
			else
				redraw();
		} else {
			startInteraction(camp);
		}
		return true;
	}

	case 'G':
		objectCommand(ObjectItems::KIND_GET, "Get-");
		return true;

	case 'J':
		objectCommand(ObjectItems::KIND_JIMMY, "Jimmy-");
		return true;

	case 'O':
		objectCommand(ObjectItems::KIND_OPEN, "Open-");
		return true;

	case 'S':
		objectCommand(ObjectItems::KIND_SEARCH, "Search-");
		return true;

	case 'K':
		objectCommand(ObjectItems::KIND_KLIMB, "Klimb-");
		return true;

	case 'P':
		objectCommand(ObjectItems::KIND_PUSH, "Push-");
		return true;

	case 'F':
		objectCommand(ObjectItems::KIND_FIRE, "Fire-");
		return true;

	case 'Y': {
		Data::Savegame &sg = _G(savegame);

		_messages.print("Yell ");

		// A ship has its sails told what to do, and otherwise a word is asked for
		if ((sg._partyTile & 0xF8) != SHIP_FIRST) {
			startInteraction(new Interactions::Yell(_messages));
			return true;
		}

		if ((sg._partyTile & 0xFC) == SHIP_FIRST) {
			_messages.print("FURL!\n");
			sg._partyTile += 4;
		} else {
			_messages.print("HOIST!\n");
			sg._partyTile -= 4;
		}

		endTurn();
		return true;
	}

	case 'C':
		castSpell();
		return true;

	case 'M':
		mixReagents();
		return true;

	case 'V':
		viewGem();
		return true;

	case 'I':
		party.igniteTorch();
		endTurn();
		return true;

	case 'L':
		_messages.print("Look-");
		startInteraction(new Interactions::Look(_messages));
		return true;

	case 'U': {
		_messages.print("Use item\n\n");
		Interactions::Use *use = new Interactions::Use(_messages);

		if (use->done()) {
			delete use;
			endTurn();
		} else {
			startInteraction(use);
		}
		return true;
	}

	case 'A': {
		_messages.print("Attack-");
		Interactions::Attack *attack = new Interactions::Attack(_messages);

		if (attack->done()) {
			delete attack;
			redraw();
		} else {
			startInteraction(attack);
		}
		return true;
	}

	case 'B':
		_messages.print("Board ");
		if (TransportLogic(_messages).board())
			endTurn();
		else
			redraw();
		return true;

	case 'X':
		_messages.print("X-it ");
		TransportLogic(_messages).getOff();
		endTurn();
		return true;

	case 'R':
		_messages.print("Ready...\n\n");
		startInteraction(new Interactions::Ready(_messages));
		return true;

	case 'Z':
		_messages.print("Z-stats...\n");
		startInteraction(new Interactions::Ztats(_messages));
		return true;

	case 'T':
		_messages.print("Talk-");
		startInteraction(new Interactions::Talk(_messages));
		return true;

	case 'N':
		_messages.print("New Order");
		startInteraction(new Interactions::NewOrder(_messages));
		return true;

	default:
		return false;
	}
}

void Game::interactionDone() {
	const bool timePasses = _interaction->takesTime();
	Interactions::Interaction *next = _interaction->takeNext();
	_interaction.reset(next);

	if (next)
		redraw();
	else if (timePasses)
		endTurn();
	else
		redraw();
}

bool Game::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode))
		return false;

	if (_interaction) {
		if (_interaction->keypress(msg)) {
			interactionDone();
		} else {
			redraw();
		}

		return true;
	}

	if (msg.keycode == Common::KEYCODE_s && (msg.flags & Common::KBD_CTRL)) {
		g_engine->sound().toggle();
		_messages.print(g_engine->sound().isOn() ? "Sound On\n" : "Sound Off\n");
		redraw();
		return true;
	}

	return handleCommand(msg);
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
