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

#ifndef SPYCRAFT_ADVLIST_H
#define SPYCRAFT_ADVLIST_H

namespace Spycraft {

#define DEF_ARRAY_SIZE		32

struct ArrayList {
	int limit;
	int size;
	void **elements;
};

extern ArrayList *ArrayList_Alloc(void);
extern ArrayList *ArrayList_Calloc(int theLimit);
extern void ArrayList_Free(ArrayList *list, void (*freeFn)(void *obj));
extern void ArrayList_Add(ArrayList *list, void *obj, bool (*sort)(void *obj1, void *obj2));
extern int ArrayList_Del(ArrayList *list, void *obj, void (*freeFn)(void *obj));
extern int ArrayList_Unlink(ArrayList *list, void *obj);
extern void ArrayList_Release(ArrayList *list);

} // namespace Spycraft

#endif
