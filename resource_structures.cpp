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

#include "common/scummsys.h"
#include "common/file.h"
#include "common/memstream.h"
#include "aesop/aesop.h"
#include "aesop/resource_structures.h"

namespace Aesop {

// user field = resource name if 0, file offset if 1
#define FAST_LOCK       1
// AESOP resource cache size (800K)
#define MAX_RES_SIZE    800000

#undef FARPROC
typedef void(*FARPROC)();

/*----------------------------------------------------------------*/

RF_file_hdr::RF_file_hdr() {
	Common::fill(&_signature[0], &_signature[16], 0);
	_fileSize = 0;
	_lostSpace = 0;
	_FOB = 0;
	_createTime = 0;
	_modifyTime = 0;
}

void RF_file_hdr::load(Common::SeekableReadStream &s) {
	s.read(&_signature[0], 16);
	_fileSize = s.readUint32LE();
	_lostSpace = s.readUint32LE();
	_FOB = s.readUint32LE();
	_createTime = s.readUint32LE();
	_modifyTime = s.readUint32LE();
}

/*----------------------------------------------------------------*/

OD_block::OD_block() {
	_next = 0;
	Common::fill(&_flags[0], &_flags[OD_SIZE], 0);
	Common::fill(&_index[0], &_index[OD_SIZE], 0);
}

void OD_block::load(Common::SeekableReadStream &s) {
	_next = s.readUint32LE();
	for (int idx = 0; idx < OD_SIZE; ++idx)
		_flags[idx] = s.readByte();
	for (int idx = 0; idx < OD_SIZE; ++idx)
		_index[idx] = s.readUint32LE();
}

/*----------------------------------------------------------------*/

RF_entry_hdr::RF_entry_hdr() {
	_timestamp = 0;
	_dataAttrib = 0;
	_dataSize = 0;
}

void RF_entry_hdr::load(Common::SeekableReadStream &s) {
	_timestamp = s.readUint32LE();
	_dataAttrib = s.readUint32LE();
	_dataSize = s.readUint32LE();
}

/*----------------------------------------------------------------*/

PRG_HDR::PRG_HDR() {
	_staticSize = 0;
	_imports = 0;
	_exports = 0;
	_parent = 0;
}

void PRG_HDR::load(Common::SeekableReadStream &s) {
	_staticSize = s.readUint16LE();
	_imports = s.readUint32LE();
	_exports = s.readUint32LE();
	_parent = s.readUint32LE();
}

/*----------------------------------------------------------------*/

THDR::THDR() {
	_mvList = 0;
	_sdList = 0;
	_nPrgs = 0;
	_iSize = 0;
	_useCount = 0;
}

/*----------------------------------------------------------------*/

void PAL_RES::load(Common::SeekableReadStream &s) {
	_nColors = s.readUint16LE();
	assert(_nColors <= PALETTE_COUNT);
	int rgbOffset = s.readUint16LE();

	// Get the fade offsets
	int fadeOffsets[11];
	for (int fadeNum = 0; fadeNum < 11; ++fadeNum)
		fadeOffsets[fadeNum] = s.readUint16LE();

	// Load in the palette RGB data
	assert(s.pos() == rgbOffset);
	_rgb.resize(_nColors * 3);
	s.read(&_rgb[0], _nColors * 3);
	for (int idx = 0; idx < _nColors * 3; ++idx)
		_rgb[idx] = VGA_COLOR_TRANS((int)_rgb[idx]);

	// Load in the fade tables
	for (int fadeNum = 0; fadeNum < 11; ++fadeNum) {
		assert(s.pos() == fadeOffsets[fadeNum]);
		_fade[fadeNum].resize(_nColors);
		s.read(&_fade[fadeNum][0], _nColors);
	}
}

/*----------------------------------------------------------------*/

void CDESC::load(Common::SeekableReadStream &s) {
	slot = s.readUint16LE();
	name = s.readUint32LE();
	size = s.readUint16LE();
}

void CDESC::save(Common::WriteStream &s) {
	s.writeUint16LE(slot);
	s.writeUint32LE(name);
	s.writeUint16LE(size);
}

} // End of namespace Aesop

