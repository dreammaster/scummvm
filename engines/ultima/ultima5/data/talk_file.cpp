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

#include "common/endian.h"
#include "common/file.h"
#include "common/util.h"
#include "ultima/ultima5/data/talk_data.h"
#include "ultima/ultima5/data/location_tables.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int TALK_INDEX_SIZE = 0x200;

static const char *const TALK_FILES[] = { "towne.tlk", "dwelling.tlk", "castle.tlk", "keep.tlk" };

bool loadTalkRecord(int mapId, int dialog, byte *record) {
	const int kind = (mapId - 1) / LOCATION_FILE_LOCATIONS;
	if (kind < 0 || kind >= ARRAYSIZE(TALK_FILES))
		return false;

	Common::File f;
	if (!f.open(TALK_FILES[kind]))
		return false;

	// The file starts with a count, then a pair for each person of
	// who they are and where what they say is
	byte index[TALK_INDEX_SIZE] = {};
	f.read(index, sizeof(index));

	const int count = READ_LE_INT16(index);
	int offset = -1;
	for (int i = 0; i < count && 2 + i * 4 + 3 < TALK_INDEX_SIZE; ++i) {
		if (READ_LE_UINT16(&index[2 + i * 4]) == dialog) {
			offset = READ_LE_UINT16(&index[4 + i * 4]);
			break;
		}
	}

	if (offset < 0 || !f.seek(offset))
		return false;

	memset(record, 0, TALK_RECORD_SIZE);
	f.read(record, TALK_RECORD_SIZE);
	return true;
}

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
