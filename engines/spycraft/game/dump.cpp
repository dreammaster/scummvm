/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * _file distributed with this source distribution.
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

#include "spycraft/game/macro.h"
#include "spycraft/game/dump.h"
#include "spycraft/game/object.h"

namespace Spycraft {

int _object_allocated;
int _static_objects;
DebugFile *debugFile;

DebugFile::DebugFile() {
	debugFile = this;
	_object_allocated = 0;
	_static_objects = 0;
#if 0
	if (!_file.open(DEBUGFILE) || !_tableFile.open(OBJECTABLE) || !_errorFile.open(ERRORFILE))
		error("Fatal: cannot open debug files");
#endif
}

DebugFile::~DebugFile() {
	_file.close();
	_tableFile.close();
	_errorFile.close();
}

void DebugFile::write(const char *msg, ...) {
}

void DebugFile::writeObj(const void *obj) {
}

void DebugFile::deleteObj(const void *obj) {
}

void DebugFile::stat(const char *file_name, int l, const char *df) {
}

void DebugFile::log_error(int error_num, int x, int l, const char *df) {
}

} // namespace Spycraft
