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

#include "aesop/aesop.h"
#include "aesop/rt.h"

namespace Aesop {

ULONG current_this;

void *RTD_first(void *dictionary) {
	error("TODO: Reimplement ASM as CPP code");
}

void *RTD_iterate(void *base, void *cur, BYTE **tag, BYTE **def) {
	error("TODO: Reimplement ASM as CPP code");
}

BYTE *RTD_lookup(HRES dictionary, const Common::String &key) {
	void *es32 = static_cast<HD_entry *>(dictionary)->_seg;
	int hashSize = READ_LE_UINT32(es32);

	// Calculate the hash for the key
	const char *keyP = key.c_str();
	int hashTotal = 0;
	while (*keyP)
		hashTotal += *keyP++;
	int hash = hashTotal % hashSize;

	// Get the entry, and check whether there's any resources in that slot
	byte *entryOffset = (byte *)es32 + hash * 4;
	uint32 v = READ_LE_UINT32(entryOffset + 2);
	if (!v)
		return nullptr;

	// Scan through the list of names with that hash to see if our resource is one of them
	int keySize = key.size();
	int nameLen;
	for (;;) {
		nameLen = READ_LE_UINT16(entryOffset);
		if (!nameLen)
			break;

		if (keySize == nameLen && !strncmp(keyP, (const char *)(entryOffset + 2), keySize))
			// Found a name match
			return (BYTE *)entryOffset + 2;

		entryOffset += keySize + 2;
	}

	// At this point, the key wasn't found, so return nothing
	return nullptr;
}

void RT_init(Resources *RTR, ULONG stack_size, HRES *objlist) {
	error("TODO: Reimplement ASM as CPP code");
}

void RT_shutdown(void) {
	error("TODO: Reimplement ASM as CPP code");
}

void RT_arguments(void *base, ULONG size) {
	error("TODO: Reimplement ASM as CPP code");
}

LONG RT_execute(ULONG index, ULONG msg_num, ULONG vector) {
	error("TODO: Reimplement ASM as CPP code");
}

} // End of namespace Aesop
