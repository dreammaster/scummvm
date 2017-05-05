/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/static_data.h"

namespace Legend {
namespace Early {

const HelpMessage HELP_TEXT[38] = {
	{ "Function Keys:", nullptr },
	{ "<F1>", "this display" },
	{ "<F2>", "menu mode" },
	{ "<F3>", "half screen mode" },
	{ "<F4>", "text mode" },
	{ "<F5>", "save game" },
	{ "<F6>", "map mode" },
	{ "<F7>", "load game" },
	{ "<F8>", "look mode" },
	{ "<F9>", "status mode" },
	{ "<F10>", "inventory mode" },
	{ nullptr, nullptr },
	{ "Special Keys when TYping:", nullptr },
	{ "<Tab>", "change to Mouse Mode" },
	{ "<Backspace>", "delete last character" },
	{ "<Esc>", "erases all typing" },
	{ "<Enter>", "execute command" },
	{ nullptr, nullptr },
	{ "Mouse Commands on the Pictures:" },
	{ "Single-click:", "\"look at\" object" },
	{ "Double-click:", "do 'obvious' thing to object" },
	{ nullptr, nullptr },
	{ "Mouse Commands on the Menus:", nullptr },
	{ "Single-click:", "select word" },
	{ "Double-click:", "execute comand" },
	{ nullptr, nullptr },
	{ "Menu Control Keys:", "nullptr" },
	{ "<Tab>", "change to Typing Mode" },
	{ "<PgUp>", "move one page up in the menu" },
	{ "<PgDn>", "move one page down in the menu" },
	{ "<Home>", "move to the top of the menu" },
	{ "<End>", "move to the bottom of the menu" },
	{ "<Shift><Letter>", "move to the word" },
	{ "<Up>, <Down>", "move up or down in the meu" },
	{ "<Left>, <Right>", "select the other menu" },
	{ "<Backspace>", "delete last word" },
	{ "<Space>", "select word from list" },
	{ "<Enter>", "execute command" },
/*	{ nullptr, nullptr },
	{ "Mouse Emulator COntrol Keys", nullptr },
	{ "<Tab>", "change to Command Menu" },
	{ "<Arrow Keys>", "move mouse cursor" },
	{ "<Space> or <Enter>", "\"click mouse button\"" },
	{ "<Shift><Enter>", "double-click mouse button" }*/
};

} // End of namespace Early
} // End of namespace Legend
