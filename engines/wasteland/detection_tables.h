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

namespace Wasteland {

const PlainGameDescriptor wastelandGames[] = {
	{ "wasteland", "Wasteland" },
	{ "fod", "Fountain of Dreams" },
	{ 0, 0 }
};

const WastelandGameDescription gameDescriptions[] = {
	{
		{
			"fod",
			nullptr,
			AD_ENTRY1s("dmap", "f267ac8b1916373a8437f4c6cfe73c7a", 138956),
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_UNSTABLE,
			GUIO1(GUIO_NONE)
		},
		GAMETYPE_FOD
	},

	{ AD_TABLE_END_MARKER, 0 }
};

} // End of namespace Wasteland
