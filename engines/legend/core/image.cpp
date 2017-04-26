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

#include "legend/core/image.h"
#include "legend/legend.h"

namespace Legend {

Image::Image() : _pixels(nullptr), _pic(nullptr) {
	_active = false;
	_field1 = 0;
	_field2 = 0;
	_field4 = 0;
	_width = 0;
	_height = 0;
	_fieldA = _fieldB = 0;
}

Image::~Image() {
	delete _pic;
}

bool Image::load(int picNumber, int frameNumber) {
	int result;

	if (!_active) {
		_fieldA = _fieldB = 0;
		if (_vm->_picFile->open(picNumber, frameNumber)) {
			_pic = _vm->_picFile->load(picNumber, frameNumber);
			assert(_pic);

			_pixels = (const byte *)_pic->getPixels();
			
			PicFile &pf = *_vm->_picFile;
			_field2 = pf._val1;
			_field4 = pf._val2;
			_width = pf._currentPic._width;
			_height = pf._currentPic._height;
			_fieldA = pf._currentPic._fieldA;
			_fieldB = pf._currentPic._fieldB;
			_field1 = (pf._currentPic._flags & PIC_40) ? 3 : 0;

			_active = true;
			return true;
		}
	}

	return false;
}

} // End of namespace Legend
