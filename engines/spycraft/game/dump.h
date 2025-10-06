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

#ifndef SPYCRAFT_GAME_DUMP_H
#define SPYCRAFT_GAME_DUMP_H

#include "common/stream.h"

namespace Spycraft {

#define DEBUGFILE	"FILEDUMP.XXX"
#define OBJECTABLE "TABLE"
#define ERRORFILE	"ERROR"

class DebugFile {
	Common::WriteStream *file;
	Common::WriteStream *table_file;
	Common::WriteStream *error_file;

public:
	DebugFile();
	~DebugFile();

	void write(const char *, ...);
	void writeObj(const void *);
	void deleteObj(void *);
	void stat(const char *, int, const char *);
	void log_error(int, int, int, const char *);
};

extern DebugFile *debugFile;

} // namespace Spycraft

#endif

