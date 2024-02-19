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

namespace Yendor {

const PlainGameDescriptor yendorGames[] = {
	{"yendor", "Yendor"},
	{"yendor2", "Yendorian Tales: Book I - Chapter 2"},
	{0, 0}};

const ADGameDescription gameDescriptions[] = {
	{"yendor2",
	 nullptr,
	 AD_ENTRY1s("world.dat", "9ad9b61e17974206cd51b0edd4daff5b", 1761397),
	 Common::EN_ANY,
	 Common::kPlatformDOS,
	 ADGF_UNSTABLE,
	 GUIO1(GUIO_NONE)},

	AD_TABLE_END_MARKER};

} // End of namespace Yendor
