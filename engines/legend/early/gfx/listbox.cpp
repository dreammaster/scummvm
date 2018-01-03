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

#include "legend/early/gfx/listbox.h"
#include "legend/early/gfx/screen.h"
#include "legend/legend.h"

namespace Legend {
namespace Early {

BEGIN_MESSAGE_MAP(Listbox, Gfx::VisualItem)
	ON_MESSAGE(ShowMsg)
	ON_MESSAGE(FrameMsg)
END_MESSAGE_MAP()

void Listbox::init() {
	_lines.clear();
	_topVisible = 0;
	_xOffset = 0;
	_upPressed = _downPressed = false;
	_thumbUp = _thumbDown = nullptr;
	_thumbUpPressed = _thumbDownPressed = nullptr;
	_thumbnail = nullptr;
}

Listbox::~Listbox() {
	delete _thumbUp;
	delete _thumbDown;
	delete _thumbnail;
	delete _thumbUpPressed;
	delete _thumbDownPressed;
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


bool Listbox::ShowMsg(CShowMsg &msg) {
	// Load the images for the listbox scrollbar
	const int LISTBOX_PIC = 17 | 0x8000;
	Gfx::PicFile &pic = *g_vm->_picFile;
	_thumbUp = pic.load(LISTBOX_PIC);
	_thumbDown = pic.load(LISTBOX_PIC + 1);
	_thumbnail = pic.load(LISTBOX_PIC + 2);
	_thumbUpPressed = pic.load(LISTBOX_PIC + 3);
	_thumbDownPressed = pic.load(LISTBOX_PIC + 4);

	return Gfx::VisualItem::ShowMsg(msg);
}

bool Listbox::FrameMsg(CFrameMsg &msg) {
	// TODO
	return true;
}

void Listbox::draw() {
	if (!_isDirty)
		return;
	Gfx::VisualItem::draw();

	// Get a drawing surface
	Gfx::VisualSurface s = getSurface();

	// Fill the background with white
	s.fill(LIGHT_GRAY);// WHITE);
/*
	// Draw the thumb up/down buttons
	s.blitFrom(_upPressed ? *_thumbUpPressed : *_thumbUp,
		Common::Point(s.w - _thumbUp->w, 0));
	s.blitFrom(_downPressed ? *_thumbDownPressed : *_thumbDown,
		Common::Point(s.w - _thumbUp->w, s.h - _thumbDown->h));
*/
}

} // End of namespace Early
} // End of namespace Legend
