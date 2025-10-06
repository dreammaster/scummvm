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

#ifndef SPYCRAFT_GAME_MACRO_H
#define SPYCRAFT_GAME_MACRO_H

#include "spycraft/game/dump.h"

namespace Spycraft {

enum {
	LIST_ERROR
};

#define LOG_FILE			"log"
#define PRINT_STAT		debugFile->stat ( NULL, __LINE__, __FILE__ )
#define LOG_STAT			debugFile->stat ( LOG_FILE, __LINE__, __FILE__ )
#define LOG_ERROR(E,F)	debugFile->log_error ( (E), (F), __LINE__, __FILE__ )

int IsStatic(void *);

} // namespace Spycraft

#endif

