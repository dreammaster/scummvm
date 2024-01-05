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

#include "common/algorithm.h"
#include "common/file.h"
#include "wasteland/fod/data/archetypes.h"

namespace Wasteland {
namespace FOD {
namespace Data {

void Profession::load(Common::SeekableReadStream &src) {
	byte buf[128];
	src.read(buf, 128);

	buf[15] = '\0';
	_name = Common::String((const char *)buf);
	Common::copy(&buf[0x14], &buf[0x1b], _attributes);
	_attributePoints = buf[0x33];
	Common::copy(&buf[0x38], &buf[0x48], _activeSkills);
	Common::copy(&buf[0x48], &buf[0x58], _passiveSkills);
	_unkMin = READ_LE_UINT16(buf + 0x70);
	_unkMin = READ_LE_UINT16(buf + 0x72);
	_field7B = buf[0x7b];
}

bool Archetypes::load() {
	Common::File f;

	if (!f.open("ARCHTYPE"))
		return false;

	// Read in the professions
	f.skip(4);
	f.seek(f.readUint16LE());

	for (int i = 0; i < PROFESSIONS_COUNT; ++i)
		_professions[i].load(f);

	return true;
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
