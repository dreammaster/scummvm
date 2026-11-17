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

#include "ultima/ultima5/gfx/game_frame.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int PANEL_COLUMN = 24;
constexpr int PANEL_WIDTH = 15;
constexpr int NAME_WIDTH = 9;
constexpr int STATUS_ROW = 8;
constexpr int DATE_ROW = 9;
constexpr int ICON_COLUMN = 30;
constexpr int ICON_ROW = 7;
constexpr int GOLD_COLUMN = 8;
constexpr byte GLYPH_ACTIVE_MARKER = 0x1A;
constexpr byte GLYPH_CORNER_TOP_LEFT = 0x7B;
constexpr byte GLYPH_CORNER_TOP_RIGHT = 0x7C;
constexpr byte GLYPH_CORNER_BOTTOM_LEFT = 0x7D;

// The wind caption is in the bottom border, and is shown in place of the
// blank border below ground
constexpr int WIND_COLUMN = 6;
constexpr int WIND_ROW = 23;
static const char *const WIND_NAMES[] = { "Calm ", "North", "South", "East ", "West " };

// Maps from the first dungeon up are not out in the open, and neither is a certain one
constexpr byte FIRST_DUNGEON_MAP = 0x21;
constexpr byte NO_WIND_MAP = 0x19;

void GameFrame::load() {
	loadFont();
}

void GameFrame::drawFrame(Graphics::ManagedSurface &dst) const {
	dst.clear();

	dst.fillRect(Common::Rect(0, 0, 320, 7), TEXT_BLUE);
	dst.fillRect(Common::Rect(0, 185, 192, 192), TEXT_BLUE);
	dst.fillRect(Common::Rect(0, 0, 7, 192), TEXT_BLUE);
	dst.fillRect(Common::Rect(185, 0, 192, 192), TEXT_BLUE);
	dst.fillRect(Common::Rect(313, 0, 320, 88), TEXT_BLUE);
	dst.fillRect(Common::Rect(192, 80, 313, 88), TEXT_BLUE);
	dst.fillRect(Common::Rect(192, 0x39, 313, 64), TEXT_BLUE);

	drawCell(dst, 0, 0, GLYPH_CORNER_TOP_LEFT, TEXT_BLUE, TEXT_BLACK);
	drawCell(dst, 39, 0, GLYPH_CORNER_TOP_RIGHT, TEXT_BLUE, TEXT_BLACK);
	drawCell(dst, 0, 23, GLYPH_CORNER_BOTTOM_LEFT, TEXT_BLUE, TEXT_BLACK);

	// The map, the party, the status line and the messages
	dst.frameRect(Common::Rect(7, 7, 185, 185), TEXT_WHITE);
	dst.drawLine(191, 191, 191, 87, TEXT_WHITE);
	dst.drawLine(191, 87, 319, 87, TEXT_WHITE);
	dst.frameRect(Common::Rect(191, 7, 313, 57), TEXT_WHITE);
	dst.frameRect(Common::Rect(191, 63, 313, 81), TEXT_WHITE);
}

void GameFrame::drawParty(Graphics::ManagedSurface &dst, const Data::Savegame &sg) const {
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

		print(dst, PANEL_COLUMN, 1 + i, line.c_str(), TEXT_WHITE, TEXT_BLACK);
	}
}

void GameFrame::drawStatus(Graphics::ManagedSurface &dst, const Data::Savegame &sg) const {
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
	print(dst, PANEL_COLUMN, STATUS_ROW, line.c_str(), TEXT_WHITE, TEXT_BLACK);

	// Month, day and year
	line = "   ";
	if (sg._month < 10 && sg._day < 10)
		line += ' ';
	line += Common::String::format("%d-%d-%03d", sg._month, sg._day, sg._year);
	while ((int)line.size() < PANEL_WIDTH)
		line += ' ';
	print(dst, PANEL_COLUMN, DATE_ROW, line.c_str(), TEXT_WHITE, TEXT_BLACK);

	// What the party is doing, in a cap-ended label over the border, or the plain border
	if (sg._icon != 0) {
		drawCaps(dst, ICON_COLUMN * 8, ICON_ROW * 8, true);
		drawCell(dst, ICON_COLUMN + 1, ICON_ROW, sg._icon, TEXT_WHITE, TEXT_BLACK);
		drawCaps(dst, (ICON_COLUMN + 2) * 8, ICON_ROW * 8, false);
	} else {
		dst.fillRect(Common::Rect(0xBF, 0x39, 0x139, 0x3F), TEXT_BLUE);
		dst.hLine(0xC0, 0x38, 0x137, TEXT_WHITE);
		dst.hLine(0xC0, 0x3F, 0x137, TEXT_WHITE);
	}
}

void GameFrame::drawWind(Graphics::ManagedSurface &dst, const Data::Savegame &sg) const {
	if (sg._mapId >= FIRST_DUNGEON_MAP || sg._mapId == NO_WIND_MAP)
		return;

	if (sg._mapLevel < Data::MAP_LEVEL_UNDERGROUND) {
		const Common::String text = Common::String(WIND_NAMES[MIN<int>(sg._windDirection, 4)]) + " Winds";

		drawCaps(dst, WIND_COLUMN * 8, WIND_ROW * 8, true);
		print(dst, WIND_COLUMN + 1, WIND_ROW, text.c_str(), TEXT_WHITE, TEXT_BLACK);
		drawCaps(dst, (WIND_COLUMN + 1 + text.size()) * 8, WIND_ROW * 8, false);
	} else {
		dst.hLine(0x30, 0xB8, 0x98, TEXT_WHITE);
		dst.fillRect(Common::Rect(0x30, 0xB9, 0x99, 0xC0), TEXT_BLUE);
	}
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
