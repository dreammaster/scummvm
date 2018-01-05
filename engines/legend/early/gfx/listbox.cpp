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
	_selectedIndex = 0; //***DEBUG*** -1;
	_dividerIndex = -1;
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
	int valsPerLine = stream->readUint32LE();

	while (stream->pos() < stream->size()) {
		String line;
		for (int idx = 0; idx < valsPerLine; ++idx) {
			uint id = stream->readUint16LE();
			
			if (id) {
				assert(id != 0xffff);
				if (idx != 0)
					line += " ";
				line += (*g_vm->_vocab)[id - 1];
			}
		}

		line.capitalize();
		_lines.push_back(line);
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

	int thumbnailY = _bounds.top + _thumbUp->h + 3;
	int scrollbarLeft = _bounds.right - _thumbUp->w - 1;

	// Set up regions for the listbox. These match the order of ListboxRegion enum
	_regions.add(Common::Rect(_bounds.left + 1, _bounds.top + 1,
		scrollbarLeft, _bounds.bottom - 1));
	_regions.add(Common::Rect(scrollbarLeft, thumbnailY,
		scrollbarLeft + _thumbnail->w, thumbnailY + _thumbnail->h));
	_regions.add(Common::Rect(scrollbarLeft, _bounds.top + _thumbUp->h + 1,
		scrollbarLeft + _thumbnail->w, _bounds.bottom - _thumbDown->h));
	_regions.add(Common::Rect(scrollbarLeft, _bounds.top + 1,
		scrollbarLeft + _thumbUp->w, _bounds.top + 1 + _thumbUp->h));
	_regions.add(Common::Rect(scrollbarLeft, _bounds.bottom - _thumbDown->h,
		scrollbarLeft + _thumbDown->w, _bounds.bottom));

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

	// Draw top line of listbox (only the top side has a line)
	Gfx::VisualSurface s = getSurface();
	s.hLine(0, 0, s.w - 1, BLACK);

	// Draw scrollbar and list of items
	drawScrollbar();
	drawItems();
}

void Listbox::drawScrollbar() {
	// Get a drawing surface
	Gfx::VisualSurface s = getSurface();

	// Draw the vertical line for the left side of the scrollbar
	int scrollbarLeft = s.w - _regions[LB_THUMB_UP].width() - 1;
	s.vLine(scrollbarLeft, 0, s.h, BLACK);
	s.vLine(s.w - 1, 0, s.h, BLACK);

	// Draw the thumb up/down buttons
	s.blitFrom(_upPressed ? *_thumbUpPressed : *_thumbUp, _regions[LB_THUMB_UP]);
	s.blitFrom(_downPressed ? *_thumbDownPressed : *_thumbDown, _regions[LB_THUMB_DOWN]);

	// Only draw the scrolling area if there are enough items to actually allow scrolling
	if (_lines.size() > numVisibleRows()) {
		// TODO: Pattern background
		s.blitFrom(*_thumbnail, _regions[LB_THUMBNAIL]);
	}
}

void Listbox::drawItems() {
	Gfx::VisualSurface s = getSurface();
	int lineHeight = s.getFont()->_lineHeight + 2;
	int scrollbarLeft = s.w - _regions[LB_THUMB_UP].width() - 1;

	for (int yp = 7, index = _topVisible; index < (int)_lines.size() && yp < s.h;
			++index, yp += lineHeight) {
		// Write out the text line
		if (index == _selectedIndex) {
			s.setFontColor(WHITE, BLACK);
			s.fillRect(Common::Rect(1, yp, scrollbarLeft - 1, yp + lineHeight), BLACK);
		} else {
			s.setFontColor(BLACK, WHITE);
		}
		s.writeString(Common::Point(8, yp + 2), _lines[index]);
	
		if (index == _dividerIndex) {
			int lineY = yp + lineHeight;

			// Draw a dashed line alternating four pixels on to off
			for (int xp = 1; xp < scrollbarLeft; xp += 8)
				s.hLine(xp, lineY, MIN(xp + 3, scrollbarLeft - 1), BLACK);
		}
	}
}

} // End of namespace Early
} // End of namespace Legend
