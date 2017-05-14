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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common/language.h"
#include "file.h"
#include "early_data.h"
#include "gateway_data.h"
#include "xanth_data.h"

/**
 * Format of the access.dat file that will be created:
 * 4 Bytes - Magic string 'SVTN' to identify valid data file
 * 2 bytes - Version number
 *
 * Following is a series of index entries with the following fields:
 * 4 bytes - offset in file of entry
 * 4 bytes - size of entry in the file
 * ASCIIZ  - name of the resource
 */

#define VERSION_NUMBER 1
#define HEADER_SIZE 0x400

Common::File inputFile, outputFile;
uint headerOffset = 6;

void NORETURN_PRE error(const char *s, ...) {
	printf("%s\n", s);
	exit(1);
}

void writeEntryHeader(const char *name, uint size) {
	assert(headerOffset < HEADER_SIZE);
	outputFile.seek(headerOffset);
	outputFile.writeLong(outputFile.size());
	outputFile.writeLong(size);
	outputFile.writeString(name);

	headerOffset += 8 + strlen(name) + 1;

	// Move back to prepare for writing the contents of the resource
	outputFile.seek(0, SEEK_END);
}

void writeFinalEntryHeader() {
	assert(headerOffset <= (HEADER_SIZE - 8));
	outputFile.seek(headerOffset);
	outputFile.writeLong(0);
	outputFile.writeLong(0);
}

void writeStringList(const char *name, const char **strings, int count) {
	// First calculate the needed size of the resource
	int size = 0;
	for (int idx = 0; idx < count; ++idx)
		size += strings[idx] ? strlen(strings[idx]) + 1 : 1;

	// Write out the header
	writeEntryHeader(name, size);

	// Write out the strings
	for (int idx = 0; idx < count; ++idx)
		outputFile.writeString(strings[idx]);
}

void writeEntry(const char *name, uint fileOffset, uint size) {
	writeEntryHeader(name, size);
	inputFile.seek(fileOffset);
	outputFile.write(inputFile, size);
}

void writeHeaderIndex() {
	// Write out magic string
	const char *MAGIC_STR = "LGND";
	outputFile.write(MAGIC_STR, 4);

	// Write out version number
	outputFile.writeWord(VERSION_NUMBER);

	// Write out a full empty header
	byte headers[HEADER_SIZE - 6];
	memset(&headers[0], HEADER_SIZE - 6, 0);
	outputFile.write(headers, HEADER_SIZE - 6);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Format: %s xanth.exe\n", argv[0]);
		exit(0);
	}

	if (!outputFile.open("legend.dat", Common::kFileWriteMode)) {
		error("Could not create legend.dat");
	}

	writeHeaderIndex();
	writeEarlyGamesData();

	for (int fileIndex = 1; fileIndex < argc; ++fileIndex) {
		if (!inputFile.open(argv[fileIndex])) {
			error("Could not open input file - %s", argv[fileIndex]);
		}

		if (inputFile.size() == XANTH_FILESIZE) {
			writeXanthData();
		} else if (inputFile.size() == GATEWAY_FILESIZE) {
			writeGatewayData();
		} else {
			printf("Unknown Legend game specified\n");
			exit(0);
		}

		inputFile.close();
	}

	writeFinalEntryHeader();
	outputFile.close();
	return 0;
}
