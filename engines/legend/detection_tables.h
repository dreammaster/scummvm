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

namespace Legend {

static const LegendGameDescription gameDescriptions[] = {
	{
		// Gateway
		{
			"gateway",
			0,
			AD_ENTRY1s("gate_000.rgn", "33c250bf306b7cbbd3dd71b6029b8784", 768),
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		GType_Gateway
	},

	{
		// Companions of Xanth
		{
			"xanth",
			0,
			AD_ENTRY1s("xanth_00.pic", "17f720a5648a8fc98d81f7f86750414c", 60113),
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		GType_CompanionsOfXanth
	},

	{ AD_TABLE_END_MARKER, (GameType)0 }
};

} // End of namespace Legend
