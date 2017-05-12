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

 // Disable symbol overrides so that we can use system headers.
#define FORBIDDEN_SYMBOL_ALLOW_ALL

// HACK to allow building with the SDL backend on MinGW
// see bug #1800764 "TOOLS: MinGW tools building broken"
#ifdef main
#undef main
#endif // main

#include "file.h"
#include "create_legend_dat.h"

void writeCursor(uint fileOffset) {
	// Hotspot x,y - 0,0 for all four Xanth cursors
	outputFile.writeWord(0);
	outputFile.writeWord(0);

	// Write out the lines of the cursor's pixel and transparency data
	inputFile.seek(fileOffset);
	for (int idx = 0; idx < 32; ++idx)
		outputFile.writeWord(inputFile.readWord());
}

void writeCursors() {
	writeEntryHeader("XANTH/CURSORS", 4 * (2 + 2 + 64));

	writeCursor(0x3fe10);	// Big arrow
	writeCursor(0x29B20);	// Small arrow
	writeCursor(0x29B60);	// Tick arrow
	writeCursor(0x29BA0);	// Hourglass arrow

	// Reuse the small arrow cursor for the earlier games
	writeEntryHeader("EARLY/CURSORS", 1 * (2 + 2 + 64));
	writeCursor(0x29B20);	// Small arrow
}

void writeConversationData() {
	writeEntry("XANTH/CONVERSATIONS/1", 0x32508, 1036);
	writeEntry("XANTH/CONVERSATIONS/2", 0x33944, 1300);
	writeEntry("XANTH/CONVERSATIONS/3", 0x353f0, 400);
	writeEntry("XANTH/CONVERSATIONS/LOOKUPS", 0x35580, 400 * 6);
}

void writeXanthData() {
	writeCursors();
	writeConversationData();
}
