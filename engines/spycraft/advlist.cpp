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

#include "spycraft/advlib.h"
#include "spycraft/advlist.h"

namespace Spycraft {

ArrayList *ArrayList_Alloc() {
	ArrayList *list;

	list = (ArrayList *)AllocPtr(sizeof(ArrayList));

	if (list != nullptr) {
		list->size = 0;
		list->elements = (void **)AllocPtr(sizeof(void *) * DEF_ARRAY_SIZE);
		if (list->elements == nullptr) {
			FreePtr(list);
			return nullptr;
		}
	}
	list->limit = DEF_ARRAY_SIZE;
	return list;
}

ArrayList *ArrayList_Calloc(int theLimit) {
	ArrayList *list;

	list = (ArrayList *)AllocPtr(sizeof(ArrayList));

	if (list != nullptr) {
		list->size = 0;
		list->elements = (void **)AllocPtr(sizeof(void *) * theLimit);
		if (list->elements == nullptr) {
			FreePtr(list);
			return nullptr;
		}
	}
	list->limit = theLimit;
	return list;
}

void ArrayList_Free(ArrayList *list, void (*freeFn)(void *obj)) {
	int i;

	if (list) {
		for (i = 0; i < list->size; i++) {
			if (list->elements[i]) {
				if (freeFn)
					(*freeFn)(list->elements[i]);
				else
					FreePtr(list->elements[i]);
			}
		}
		FreePtr(list->elements);
		FreePtr(list);
	}
}

void ArrayList_Add(ArrayList *list, void *obj, int (*sort)(void *in, void *out)) {
	int i, j;

	if (list->size >= list->limit) {
		ADV_ASSERT(false, __ERR_EXCEED_LISTSIZE);
	} else {
		if (sort) {
			for (i = 0; i < list->size; i++) {
				if ((*sort)(list->elements[i], obj)) {
					break;
				}
			}

			for (j = list->size; j > i; j--) {
				list->elements[j] = list->elements[j - 1];
			}

			list->elements[i] = obj;
			list->size++;
		} else {
			list->elements[list->size++] = obj;
		}
	}
}

int ArrayList_Del(ArrayList *list, void *obj, void (*freeFn)(void *)) {
	int i;
	bool found = false;

	for (i = 0; i < list->size; i++) {
		if (list->elements[i] == obj) {
			if (freeFn)
				(*freeFn)(list->elements[i]);
			else
				FreePtr(list->elements[i]);
			list->elements[i] = nullptr;
			found = true;
			break;
		}
	}
	if (found) {
		while (i < list->size - 1) {
			list->elements[i] = list->elements[i + 1];
			i++;
		}
		list->size--;
	}
	return found;
}

int ArrayList_Unlink(ArrayList *list, void *obj) {
	int i;
	bool found = false;

	for (i = 0; i < list->size; i++) {
		if (list->elements[i] == obj) {
			list->elements[i] = nullptr;
			found = true;
			break;
		}
	}
	if (found) {
		while (i < list->size - 1) {
			list->elements[i] = list->elements[i + 1];
			i++;
		}
		list->size--;
	}

	return found;
}

void ArrayList_Release(ArrayList *list) {
	if (list) {
		list->size = 0;
	}
}

} // namespace Spycraft
