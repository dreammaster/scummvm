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

#include "spycraft/dmade/htm.h"
#include "spycraft/dmade/advres.h"

namespace Spycraft {

extern int sizeHTM;

void *htm = NULL;
int curSize = 0;
int curHTM = 0;


// allow only one HTM at a time
void *LoadHTM(int id) {
	if (htm)
		return NULL;

	htm = (void *)sfxLoadRes(id, RES_HTM);
	if (htm) {
		curHTM = id;
		curSize = sizeHTM;
	}

	return htm;
}

int UnLoadHTM() {
	if (htm) {
		curHTM = 0;
		curSize = 0;
		htm = NULL;
		return true;
	}

	return false;
}

int GetHTMSize() {
	return curSize;
}

} // namespace Spycraft
