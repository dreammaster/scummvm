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

namespace AGS2 {

const PlainGameDescriptor AGS2Games[] = {
	{ "AGS2", "AGS Early Games" },
	{ "robblanc1", "Rob Blanc 1: Better Days of a Defender Of the Universe" },
	{ "qfg45", "Quest for Glory 4 1/2: So You Thought You Were a Hero?" },
	{ 0, 0 }
};

const AGS2GameDescription gameDescriptions[] = {
	{
		{
			"qfg45",
			nullptr,
			AD_ENTRY1s("qfg.001", "dadc5de95ba5662fca06957497eaf24d", 1050254),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_UNSTABLE,
			GUIO1(GUIO_NONE)
		},
		"2.30.221"
	},

	{
		{
			"robblanc1",
			"Remastered",
			AD_ENTRY1s("ac2game.dat", "29c2ced2f2e6ad764e4249b4e4c45bba", 920415),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_UNSTABLE,
			GUIO1(GUIO_NONE)
		},
		"2.40.325"
	},

	AD_TABLE_END_MARKER
};

} // namespace AGS2
