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

#include "legend/gfx/image.h"
#include "legend/legend.h"

namespace Legend {

BEGIN_MESSAGE_MAP(Image, VisualItem)
	ON_MESSAGE(ShowMsg)
	ON_MESSAGE(HideMsg)
END_MESSAGE_MAP()

void Image::setup() {
	_pic = nullptr;
	_picNum = -1;
	_frameNumber = 0;
	_active = false;
	_field1 = 0;
	_fieldA = _fieldB = 0;
}

Image::~Image() {
	delete _pic;
}

void Image::fill(int color) {
	assert(!_bounds.isEmpty());
	delete _pic;
	_pic = new Picture(_bounds.width(), _bounds.height());
	assert(_pic);

	_fieldA = _fieldB = 0;
	_active = true;
	_field1 = 0;
}

bool Image::ShowMsg(CShowMsg &msg) {
	if (_picNum != -1 && !_pic)
		loadImage();

	return VisualItem::ShowMsg(msg);
}

bool Image::HideMsg(CHideMsg &msg) {
	delete _pic;
	_pic = nullptr;
	
	return VisualItem::HideMsg(msg);
}

void Image::draw() {
	if (_isDirty && _pic) {
		// Draw the picture to the screen
		getSurface().blitFrom(*_pic);
		getScreen().setPalette();
	}

	VisualItem::draw();
}

bool Image::load(int picNumber, int frameNumber) {
	_picNum = picNumber;
	_frameNumber = frameNumber;

	bool result = loadImage();
	if (result) {
		PicFile &pf = *g_vm->_picFile;
		_bounds = Common::Rect(pf._origin.x, pf._origin.y,
			pf._origin.x + pf._currentPic._width,
			pf._origin.y + pf._currentPic._height);
	}

	return result;
}

bool Image::loadImage() {
	_fieldA = _fieldB = 0;
	if (g_vm->_picFile->open(_picNum, _frameNumber)) {
		_pic = g_vm->_picFile->load(_picNum, _frameNumber);
		assert(_pic);

		PicFile &pf = *g_vm->_picFile;
		_fieldA = pf._currentPic._fieldA;
		_fieldB = pf._currentPic._fieldB;
		_field1 = (pf._currentPic._flags & PIC_40) ? 3 : 0;

		_active = true;
		return true;
	}

	return false;
}

} // End of namespace Legend
