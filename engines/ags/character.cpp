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

#include "engines/ags/character.h"

namespace AGS {

uint32 Character::readUint32(uint offset) {
	switch (offset) {
	case 0:
		return _defView;
	case 4:
		return _talkView;
	case 8:
		return _view;
	case 12:
		return _room;
	case 16:
		return _prevRoom;
	case 20:
		return _x;
	case 24:
		return _y;
	case 28:
		return _wait;
	case 32:
		return _flags;
	case 40:
		return _idleView;
	case 52:
		return _activeInv;
	case 56:
		return _talkColor;
	case 60:
		return _thinkView;
	case 76:
		return _z;
	case 80:
		return _walkWait;
	case 96:
		return _indexId;
	default:
		error("Character::readUint32: offset %d is invalid", offset);
	}
}

bool Character::writeUint32(uint offset, uint value) {
	// FIXME

	return false;
}

} // End of namespace AGS
