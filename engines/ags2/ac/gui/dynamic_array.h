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

#ifndef AGS2_AC_GUI_DYNAMIC_ARRAY_H
#define AGS2_AC_GUI_DYNAMIC_ARRAY_H

#include "common/scummsys.h"

namespace AGS2 {

template <typename T> struct DynamicArray {
private:
	T defaultConstructed;
	T *data;
	int datasize;

public:

	DynamicArray() {
		data = NULL;
		datasize = 0;
	}

	~DynamicArray() {
		if (data)
			free(data);
	}

	void GrowTo(int newsize);
	void SetSizeTo(int newsize);
	T &operator[] (int index);
};

template <typename T>
void DynamicArray<T>::GrowTo(int newsize) {
	if (datasize < newsize) {
		SetSizeTo(newsize);
	}
}

template <typename T>
void DynamicArray<T>::SetSizeTo(int newsize) {
	int dsWas = datasize;
	datasize = newsize;
	if (data == NULL)
		data = (T *)calloc(sizeof(T), datasize);
	else {
		T *newdata = (T *)calloc(sizeof(T), datasize);
		if (dsWas > datasize)
			dsWas = datasize;
		memcpy(newdata, data, sizeof(T) * dsWas);
		free(data);
		data = newdata;
	}
	// "construct" the new objects by copying the default-constructed
	// object into them
	// this is necessary so that the vtables are set up correctly
	for (int qq = dsWas; qq < datasize; qq++) {
		memcpy(&data[qq], &defaultConstructed, sizeof(T));
	}
}

template <typename T>
T &DynamicArray<T>::operator[] (int index) {
	if (index < 0)
		index = 0;
	if (index >= datasize) {
		// grow it 5 bigger, so we don't have to keep reallocating
		GrowTo(index + 5);
	}
	return data[index];
}

} // namespace AGS2

#endif
