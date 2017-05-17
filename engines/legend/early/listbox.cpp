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

#include "legend/early/listbox.h"
#include "legend/legend.h"

namespace Legend {
namespace Early {

EMPTY_MESSAGE_MAP(Listbox, Gfx::VisualItem);

void Listbox::init() {
	_lines.clear();
	_topVisible = 0;
	_xOffset = 0;
}

void Listbox::load(const StringArray &lines) {
	init();
	_lines = lines;
}

void Listbox::load(const String &resName) {
	init();

	Common::SeekableReadStream *stream = g_vm->_res->getResource(resName);
	int valsPerLine = stream->readUint16LE();

	while (stream->pos() < stream->size()) {
		String line;
		for (int idx = 0; idx < valsPerLine; ++idx) {
			uint id = stream->readUint16LE();
			
			if (id) {
				assert(id != 0xffff);
				if (idx > 0)
					line += " ";
				// TODO: line += vocab
			}
		}
	}

	delete stream;
}


} // End of namespace Early
} // End of namespace Legend
