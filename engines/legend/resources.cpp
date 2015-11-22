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

#include "legend/resources.h"
#include "legend/legend.h"

namespace Legend {

LegendEngine *Message::_vm;

Message::Message(uint id) : _id(id), _msg(nullptr) {}

Message::Message(const char *msg) : _id(0), _msg(msg) {}

Message::operator const char *() const {
	error("TODO");
}

/*-------------------------------------------------------------------*/

Resources::Resources(LegendEngine *vm) {
	_vm = vm;
	Message::_vm = vm;

	switch (vm->getGameID()) {
	case GType_Gateway:
		_prefix = "gate";
		break;
	case GType_CompanionsOfXanth:
		_prefix = "xanth";
		break;
	default:
		break;
	}
}

Common::String Resources::getFilename(FileType fileType, int fileNumber) {
	const char *const EXTENSIONS[] = { "PIC", "RGN", "FNT", "MUS", "SAV", "SAV" };
	return Common::String::format("%s_%03d.%s", _prefix.c_str(), fileNumber,
		EXTENSIONS[fileType]);
}

} // End of namespace Legend
