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

namespace Dink {

const PlainGameDescriptor GAME_NAMES[] = {
	{ "dink", "Dink Smallwood Game Module" },
	{ "dinksmallwood", "Dink Smallwood" },

	{ "pq", "Pilgrim's Quest" },
	{ 0, 0 }
};

#define DETECTION_ENTRY(ID, FILENAME, MD5, SIZE, LANG, PLATFORM, GUIO, FLAGS) \
	{{ ID, PLATFORM, AD_ENTRY1s(FILENAME, MD5, SIZE), LANG, \
	Common::kPlatformUnknown, FLAGS, GUIO }}

#define GAME_ENTRY(ID, FILENAME, MD5, SIZE) \
	DETECTION_ENTRY(ID, FILENAME, MD5, SIZE, Common::UNK_LANG, nullptr, GUIO1(GUIO_NOLANG), ADGF_UNSTABLE)

const DinkGameDescription GAME_DESCRIPTIONS[] = {
	GAME_ENTRY("dinksmallwood", "dink.dat", "019120cef0d2555c72afeba30a5d18ec", 11488),
	//GAME_ENTRY("pq", "pq.dmod", "98ae54ad2264db226bf697545d60a727", 35324007),

	{ AD_TABLE_END_MARKER }
};

/**
 * The fallback game descriptor used by the fallback detection code
 */
static DinkGameDescription g_fallbackDesc = {
	{
		"",
		"",
		AD_ENTRY1(0, 0),
		Common::UNK_LANG,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO1(GUIO_NOLANG)
	}
};

} // namespace Dink
