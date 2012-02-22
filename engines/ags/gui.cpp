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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 */

#include "engines/ags/ags.h"
#include "engines/ags/constants.h"
#include "engines/ags/gui.h"
#include "engines/ags/util.h"

namespace AGS {

void GUIControl::readFrom(Common::SeekableReadStream *dta) {
	_flags = dta->readUint32LE();
	_x = dta->readUint32LE();
	_y = dta->readUint32LE();
	_width = dta->readUint32LE();
	_height = dta->readUint32LE();
	_zorder = dta->readUint32LE();
	_activated = dta->readUint32LE();

	if (_vm->getGUIVersion() >= 106) {
		_scriptName = readString(dta);
	}

	if (_vm->getGUIVersion() >= 108) {
		uint32 eventCount = dta->readUint32LE();
		if (eventCount > getMaxNumEvents())
			error("too many events (%d) when reading GUIControl", eventCount);

		_eventHandlers.resize(eventCount);
		for (uint i = 0; i < eventCount; ++i)
			_eventHandlers[i] = readString(dta);
	}
}

void GUISlider::readFrom(Common::SeekableReadStream *dta) {
	GUIControl::readFrom(dta);

	_min = dta->readUint32LE();
	_max = dta->readUint32LE();
	_value = dta->readUint32LE();
	_mousePressed = dta->readUint32LE();

	if (_vm->getGUIVersion() < 104) {
		_handlePic = 0xffffffff;
		_handleOffset = 0;
		_bgImage = 0;
	} else {
		_handlePic = dta->readUint32LE();
		_handleOffset = dta->readUint32LE();
		_bgImage = dta->readUint32LE();
	}
}

void GUILabel::readFrom(Common::SeekableReadStream *dta) {
	GUIControl::readFrom(dta);

	uint32 textLen = 200;
	if (_vm->getGUIVersion() >= 113) {
		textLen = dta->readUint32LE();
	}

	byte *buffer = new byte[textLen + 1];
	dta->read(buffer, textLen);
	buffer[textLen] = '\0';
	_text = (char *)buffer;
	delete[] buffer;

	_font = dta->readUint32LE();
	_textColor = dta->readUint32LE();
	_align = dta->readUint32LE();

	if (_textColor == 0)
		_textColor = 16;
}

void GUITextBox::readFrom(Common::SeekableReadStream *dta) {
	GUIControl::readFrom(dta);

	char buffer[200 + 1];
	dta->read(&buffer[0], 200);
	buffer[200] = '\0';
	_text = buffer;

	_font = dta->readUint32LE();
	_textColor = dta->readUint32LE();
	_exFlags = dta->readUint32LE();
}

void GUIListBox::readFrom(Common::SeekableReadStream *dta) {
	GUIControl::readFrom(dta);

	uint32 itemCount = dta->readUint32LE();
	_selected = dta->readUint32LE();
	_topItem = dta->readUint32LE();
	_mouseXP = dta->readUint32LE();
	_mouseYP = dta->readUint32LE();
	_rowHeight = dta->readUint32LE();
	_numItemsFit = dta->readUint32LE();
	_font = dta->readUint32LE();
	_textColor = dta->readUint32LE();
	_backColor = dta->readUint32LE();
	_exFlags = dta->readUint32LE();

	if (_textColor == 0)
		_textColor = 16;

	if (_vm->getGUIVersion() >= 112) {
		_alignment = dta->readUint32LE();
		dta->skip(4); // reserved1
	} else {
		_alignment = GALIGN_LEFT;
	}

	if (_vm->getGUIVersion() >= 107) {
		_selectedBgColor = dta->readUint32LE();
	} else {
		_selectedBgColor = _textColor;
	}

	_items.resize(itemCount);
	_itemSaveGameIndexes.resize(itemCount);
	for (uint i = 0; i < itemCount; ++i) {
		_items[i] = readString(dta);
		_itemSaveGameIndexes[i] = 0xffff;
	}

	if ((_vm->getGUIVersion() >= 114) && (_exFlags & GLF_SGINDEXVALID)) {
		for (uint i = 0; i < itemCount; ++i)
			_itemSaveGameIndexes[i] = dta->readUint16LE();
	}
}

void GUIInvControl::readFrom(Common::SeekableReadStream *dta) {
	GUIControl::readFrom(dta);

	if (_vm->getGUIVersion() >= 109) {
		_charId = dta->readUint32LE();
		_itemWidth = dta->readUint32LE();
		_itemHeight = dta->readUint32LE();
		_topIndex = dta->readUint32LE();
	} else {
		_charId = 0xffffffff;
		_itemWidth = 40;
		_itemHeight = 22;
		_topIndex = 0;
	}

	if (_vm->getGameFileVersion() >= kAGSVer270) {
		// ensure that some items are visible
		if (_itemWidth > _width)
			_itemWidth = _width;
		if (_itemHeight > _height)
			_itemHeight = _height;
	}
}

void GUIButton::readFrom(Common::SeekableReadStream *dta) {
	GUIControl::readFrom(dta);

	_pic = dta->readUint32LE();
	_overPic = dta->readUint32LE();
	_pushedPic = dta->readUint32LE();

	dta->skip(4); // _usePic
	_usePic = _pic;

	_isPushed = dta->readUint32LE();
	_isOver = dta->readUint32LE();
	_font = dta->readUint32LE();
	_textColor = dta->readUint32LE();
	_leftClick = dta->readUint32LE();
	_rightClick = dta->readUint32LE();
	_leftClickData = dta->readUint32LE();
	_rightClickData = dta->readUint32LE();

	char buffer[50 + 1];
	dta->read(&buffer[0], 50);
	buffer[50] = '\0';
	_text = buffer;

	if (_vm->getGUIVersion() >= 111) {
		_textAlignment = dta->readUint32LE();
		dta->skip(4); // reserved1
	} else {
		_textAlignment = GBUT_ALIGN_TOPMIDDLE;
	}
}

} // End of namespace AGS
