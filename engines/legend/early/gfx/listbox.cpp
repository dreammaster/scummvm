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

#include "legend/early/gfx/listbox.h"
#include "legend/early/gfx/screen.h"
#include "legend/early/engine.h"
#include "legend/gfx/region.h"

namespace Legend {
namespace Early {
namespace Gfx {

using Legend::Gfx::Region;

void Listbox::init() {
	_lines.clear();
	_topVisible = 0;
	_selectedIndex = -1;
	_dividerIndex = -1;
	_xOffset = 0;
	_dragOffset = 0;
	_draggingThumbnail = false;
	_itemSelected = false;
	_upPressed = _downPressed = false;
	_pressRepeatExpiry = 0;
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

uint Listbox::itemsLineHeight() const {
	return _font->_lineHeight + 2;
}

void Listbox::load(const StringArray &lines) {
	init();
	_lines = lines;
}

void Listbox::load(const String &resName) {
	init();

	Common::SeekableReadStream *stream = g_engine->_res->getResource(resName);
	int valsPerLine = stream->readUint32LE();

	while (stream->pos() < stream->size()) {
		String line;
		for (int idx = 0; idx < valsPerLine; ++idx) {
			uint id = stream->readUint16LE();

			if (id) {
				assert(id != 0xffff);
				if (idx != 0)
					line += " ";
				line += (*g_engine->_vocab)[id - 1];
			}
		}

		line.capitalize();
		_lines.push_back(line);
	}

	delete stream;
	updateThumbnail();
}

void Listbox::updateThumbnail() {
	if (!_thumbnail || _lines.size() < numVisibleRows())
		return;

	const Region &scrollRegion = _regions[LB_SCROLLBAR];
	Region &thumbRegion = _regions[LB_THUMBNAIL];

	// Figure out the new vertical position of the scrollbar thumbnail
	int yStart = scrollRegion.top;
	int yEnd = scrollRegion.bottom - _thumbnail->h;
	int newY = yStart + ((yEnd - yStart + 1) * MAX(_selectedIndex, 0)
		/ ((int)_lines.size() - 1));

	// Readjust the thumbnail's region
	thumbRegion.moveTo(thumbRegion.left, newY);
}

void Listbox::draw() {
	BoxedElement::draw();

	// Draw top line of listbox (only the top side has a line)
	Graphics::ManagedSurface s = getSurface();
	s.hLine(0, 0, s.w - 1, Gfx::BLACK);

	// Draw scrollbar and list of items
	drawScrollbar();
	drawItems();
}

void Listbox::drawScrollbar() {
	bool thumbVisible = isScrollingEnabled();

	// Get a drawing surface
	Graphics::ManagedSurface surface = getSurface();
	Graphics::ManagedSurface s(surface, Common::Rect(
		surface.w - _regions[LB_THUMB_UP].width(), 0, surface.w, surface.h));

	// Draw the scrollbar background. Either checkered if there are enough items
	// for scrolling, or completely white for a 'disabled' look
	s.clear(thumbVisible ? 0x82 : WHITE);

	// Draw the vertical lines on both sides of the scrollbar
	s.vLine(0, 0, s.h - 1, BLACK);
	s.vLine(s.w - 1, 0, s.h - 1, BLACK);

	// Draw the thumb up/down buttons
	s.blitFrom(_upPressed ? *_thumbUpPressed : *_thumbUp, _regions[LB_THUMB_UP]);
	s.blitFrom(_downPressed ? *_thumbDownPressed : *_thumbDown, _regions[LB_THUMB_DOWN]);

	if (thumbVisible)
		s.blitFrom(*_thumbnail, _regions[LB_THUMBNAIL]);
}

void Listbox::drawItems() {
	// Get a surface constrained to the area to write the items in
	Graphics::ManagedSurface surface = getSurface();
	Graphics::ManagedSurface s(surface, Common::Rect(
		0, LISTBOX_ITEMS_X,
		surface.w - _regions[LB_THUMB_UP].width() - 1, surface.h - 1
	));
	int lineHeight = itemsLineHeight();

	// Erase any previously drawn items
	s.clear(WHITE);

	for (int yp = 0, index = _topVisible; index < (int)_lines.size() && yp < s.h;
		++index, yp += lineHeight) {
		// Write out the text line
		if (index == _selectedIndex) {
			_font->setColor(WHITE, BLACK);
			s.fillRect(Common::Rect(0, yp, s.w, yp + lineHeight), BLACK);
		} else {
			_font->setColor(BLACK, WHITE);
		}
		writeString(Common::Point(8, yp + 2), _lines[index]);

		// Handle drawing the divider line, if present
		if (index == _dividerIndex) {
			int lineY = yp + lineHeight;

			// Draw a dashed line alternating four pixels on to off
			for (int xp = 1; xp < s.w; xp += 8)
				s.hLine(xp, lineY, MIN(xp + 3, s.w - 1), BLACK);
		}
	}
}

void Listbox::deltaChange(int delta) {
	if (_lines.empty())
		return;

	int topVisible = _topVisible;
	int newIndex = _selectedIndex;

	// Figure out the new index and top visible row to use after delta change
	if (delta < -1 || delta > 1) {
		int maxTopVisible = _lines.size() - numVisibleRows();
		topVisible += delta;

		if (topVisible > maxTopVisible) {
			topVisible = MAX(maxTopVisible, 0);
			newIndex = (int)_lines.size() - 1;
		} else {
			if (topVisible < 0)
				topVisible = 0;
			newIndex = topVisible;
		}
	} else {
		newIndex = CLIP(newIndex + delta, 0, (int)_lines.size() - 1);

		if (newIndex < topVisible) {
			topVisible = newIndex;
		} else {
			int minTopVisible = newIndex - numVisibleRows() + 1;
			if (topVisible < minTopVisible)
				topVisible = MAX(minTopVisible, 0);
		}
	}

	_topVisible = topVisible;
	_selectedIndex = newIndex;

	// Update the thumbnail and flag the listbox for a redraw
	updateThumbnail();
	redraw();
}

bool Listbox::msgFocus(const FocusMessage &msg) {
	// Load the images for the listbox scrollbar
	const int LISTBOX_PIC = 17 | 0x8000;
	Legend::Gfx::PicFile &pic = *g_engine->_picFile;
	_thumbUp = pic.load(LISTBOX_PIC);
	_thumbDown = pic.load(LISTBOX_PIC + 1);
	_thumbnail = pic.load(LISTBOX_PIC + 2);
	_thumbUpPressed = pic.load(LISTBOX_PIC + 3);
	_thumbDownPressed = pic.load(LISTBOX_PIC + 4);

	int thumbnailY = _bounds.top + _thumbUp->h;
	int scrollbarLeft = _bounds.right - _thumbUp->w;

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

	updateThumbnail();
	return BoxedElement::msgFocus(msg);
}

bool Listbox::tick() {
	// When the up or down arrow buttons are pressed in the scrollbar,
	// do repeated delta changes as long as the mouse button is held
	if (_upPressed || _downPressed) {
		uint currTime = g_engine->getTotalPlayTime();

		if (currTime >= _pressRepeatExpiry) {
			_pressRepeatExpiry = currTime + ARROW_REPEAT_FRAMES;
			deltaChange(_upPressed ? -1 : 1);
		}
	}

	return true;
}

bool Listbox::msgMouseWheel(const MouseWheelMessage &msg) {
	if (_selectedIndex == -1)
		_selectedIndex = _topVisible;

	deltaChange(msg._wheelUp ? -1 : 1);
	return true;
}

bool Listbox::msgMouseDown(const MouseDownMessage &msg) {
	switch (_regions.indexOf(msg._pos)) {
	case LB_ITEMS:
		if (msg._pos.y >= (_bounds.top + LISTBOX_ITEMS_Y)) {
			_itemSelected = true;
			_selectedIndex = (msg._pos.y - _bounds.top - LISTBOX_ITEMS_Y)
				/ itemsLineHeight() + _topVisible;
			redraw();
		}
		break;

	case LB_THUMB_UP:
		if (isScrollingEnabled()) {
			_upPressed = true;
			redraw();
		}
		break;

	case LB_THUMB_DOWN:
		if (isScrollingEnabled()) {
			_downPressed = true;
			redraw();
		}
		break;

	case LB_SCROLLBAR:
		// Clicked on scrollbar above or below thmbnail, so move up/down a page
		deltaChange((msg._pos.y < _regions[LB_THUMBNAIL].top) ?
			-(int)numVisibleRows() : (int)numVisibleRows());
		break;

	case LB_THUMBNAIL:
		// Clicked on thumbail
		_draggingThumbnail = true;
		_dragOffset = msg._pos.y - _regions[LB_THUMBNAIL].top;
		break;

	default:
		break;
	}

	return true;
}

bool Listbox::msgMouseUp(const MouseUpMessage &msg) {
	if (_upPressed || _downPressed) {
		// Released mouse from clicking on up/down buttons
		_upPressed = _downPressed = false;
		_pressRepeatExpiry = 0;
		redraw();
	}

	if (_draggingThumbnail) {
		// Finished dragging scrollbar thumbnail
		_draggingThumbnail = false;
		_dragOffset = 0;
	}

	if (_itemSelected) {
		// An entry was selected, generate message now that the mouse is released
		_itemSelected = false;
		GameMessage selMsg("SELECTION", _selectedIndex);
		g_events->send(selMsg);
	}

	return true;
}
/*
bool Listbox::MouseDragMsg(CMouseDragMsg &msg) {
	if (_draggingThumbnail) {
		// Figure out new position based on mouse position
		int yStart = _regions[LB_SCROLLBAR].top;
		int yEnd = _regions[LB_SCROLLBAR].bottom - _thumbnail->h;
		int newY = CLIP(msg._pos.y - _dragOffset, yStart, yEnd);

		// Determine the effective selection index
		int newIndex = (newY - yStart) * ((int)_lines.size() - 1)
			/ (yEnd - yStart + 1);

		if (newIndex != _selectedIndex)
			// Do a delta change to select that index
			deltaChange(newIndex - MAX(_selectedIndex, 0));
	}

	return true;
}
*/
} // namespace Gfx
} // namespace Early
} // namespace Legend
