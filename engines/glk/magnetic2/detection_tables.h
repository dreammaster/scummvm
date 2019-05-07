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

#include "engines/game.h"
#include "common/gui_options.h"
#include "common/language.h"

namespace Glk {
namespace Magnetic2 {

/**
 * Game description
 */
struct MagneticGameDescription {
	const char *const _gameId;
	const char *const _extra;
	const char *const _md5;
	size_t _filesize;
	Common::Language _language;
};

const PlainGameDescriptor MAGNETIC_GAME_LIST[] = {
	{ "corruption2", "Corruption" },
	{ "fish2", "Fish!" },
	{ "guild2", "The Guild of Thieves" },
	{ "myth2", "Myth" },
	{ "pawn2", "the Pawn" },
	{ "wonderland2", "Wonderland" },

	{ nullptr, nullptr }
};

#define ENTRY0(ID, MD5, FILESIZE) { ID, nullptr, MD5, FILESIZE, Common::EN_ANY }
#define ENTRY1(ID, EXTRA, MD5, FILESIZE) { ID, EXTRA, MD5, FILESIZE, Common::EN_ANY }
#define TABLE_END_MARKER { nullptr, nullptr, nullptr, 0, Common::EN_ANY }

const MagneticGameDescription MAGNETIC_GAMES[] = {
	ENTRY0("corruption2", "313880cbe0f15bfa259ebaf228b4d0e9", 167466),
	ENTRY1("corruption2", "Collection", "6fe35b357fa0311450d3a9c809e60ba8", 177185),
	ENTRY0("fish2", "2efb8118f4cb9a36bb54646ce41a950e", 162858),
	ENTRY1("fish2", "Collection", "cfe333306597d36c8aa3fc64f6be94ba", 172517),
	ENTRY0("guild2", "bab78740d39ee5e058faf4912fdbf33d", 130858),
	ENTRY1("guild2", "Collection", "36af907a4ec9db909148f308287586f1", 141766),
	ENTRY0("myth2", "9c2a5272a9c0b1e173401ba4df32567a", 99370),
	ENTRY0("pawn2", "4a7847980f9e942acd7aa51ea12a6586", 103466),
	ENTRY0("wonderland2", "2cea8fccf42d570be8836416c2802613", 183916),
	TABLE_END_MARKER
};

} // End of namespace Magnetic2
} // End of namespace Glk
