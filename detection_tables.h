/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
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

namespace Aesop {

static const AesopGameDescription gameDescriptions[] = {
#if 0
	{
		// Eye of the Beholder 3 - GOG.com English
		{
			"eob3",
			0,
			AD_ENTRY1s("eye.res", "0adde71f4231370fa265b4f409b1273a", 6845522),
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_UNSTABLE,
			GUIO0()
		},
		GType_EOB3,
		0
	},
#endif
	{
		// Eye of the Beholder 3 - AESOP32 Converted English
		{
			"eob3",
			"AESOP32",
			AD_ENTRY1s("eye.res", "e8e432b6c9f9ec1ae51eea44c1407750", 6845234),
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_UNSTABLE,
			GUIO0()
		},
		GType_EOB3,
		GF_AESOP32
	},

	{
		// Dungeon Hack - English
		{
			"dungeonhack",
			0,
			AD_ENTRY1s("hack.res", "67345ba1870656dd54d8c8544954d834", 7083229),
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_UNSTABLE,
			GUIO0()
		},
		GType_DungeonHack,
		0
	},

	{ AD_TABLE_END_MARKER, (GameType)0, 0 }
};

} // End of namespace Aesop
