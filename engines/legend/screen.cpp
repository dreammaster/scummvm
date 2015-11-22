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

#include "legend/screen.h"
#include "legend/legend.h"
#include "common/system.h"
#include "common/file.h"
#include "common/util.h"
#include "graphics/palette.h"

namespace Legend {

Screen::Screen(LegendEngine *vm): _vm(vm), _font0(this), _font1(this), 
		_activeFont(nullptr) {
	_screenSurface.create(g_system->getWidth(), g_system->getHeight(),
		Graphics::PixelFormat::createFormatCLUT8());
	Font::init();
}

Screen::~Screen() {
}

void Screen::fillRect(const Common::Rect &r, int color) {
	_screenSurface.fillRect(r, color);
	addDirtyRect(r);
}

void Screen::update() {
	// Merge the dirty rects
	mergeDirtyRects();

	// Loop through copying dirty areas to the physical screen
	Common::List<Common::Rect>::iterator i;
	for (i = _dirtyRects.begin(); i != _dirtyRects.end(); ++i) {
		const Common::Rect &r = *i;
		const byte *srcP = (const byte *)_screenSurface.getBasePtr(r.left, r.top);
		g_system->copyRectToScreen(srcP, _screenSurface.pitch, r.left, r.top,
			r.width(), r.height());
	}

	// Signal the physical screen to update
	g_system->updateScreen();
	_dirtyRects.clear();
}

void Screen::makeAllDirty() {
	addDirtyRect(Common::Rect(0, 0, _screenSurface.w, _screenSurface.h));
}

void Screen::addDirtyRect(const Common::Rect &r) {
	_dirtyRects.push_back(r);
	assert(r.width() > 0 && r.height() > 0);
}

void Screen::mergeDirtyRects() {
	Common::List<Common::Rect>::iterator rOuter, rInner;

	// Process the dirty rect list to find any rects to merge
	for (rOuter = _dirtyRects.begin(); rOuter != _dirtyRects.end(); ++rOuter) {
		rInner = rOuter;
		while (++rInner != _dirtyRects.end()) {

			if ((*rOuter).intersects(*rInner)) {
				// these two rectangles overlap or
				// are next to each other - merge them

				unionRectangle(*rOuter, *rOuter, *rInner);

				// remove the inner rect from the list
				_dirtyRects.erase(rInner);

				// move back to beginning of list
				rInner = rOuter;
			}
		}
	}
}

bool Screen::unionRectangle(Common::Rect &destRect, const Common::Rect &src1, const Common::Rect &src2) {
	destRect = src1;
	destRect.extend(src2);

	return !destRect.isEmpty();
}

void Screen::loadFont(int fontNumber) {
	Font *fontList[2] = { &_font0, &_font1 };
	int idx;
	Common::File f;

	for (idx = 0; idx < 2; ++idx) {
		if (fontList[idx]->_counter)
			fontList[idx]->_counter++;
	}

	for (idx = 0; idx < 2; ++idx) {
		if (fontList[idx]->_sectionNum == Font::_fontSectionNum && fontList[idx]->_fontNumber == fontNumber)
			break;
	}

	if (idx < 2) {
		// Use existing loaded font
		fontList[idx]->_counter = 1;
		_activeFont = fontList[idx];
	} else {
		Font &font = *fontList[idx];
		if (fontNumber)
			f.open(_vm->_res->getFilename(FILETYPE_FNT,
				Font::_fontSectionNum * 100 + fontNumber));

		font._counter = 1;
		font._sectionNum = Font::_fontSectionNum;
		font._fontNumber = fontNumber;

		// Original used font 0 for text mode font, which we don't need
		assert(fontNumber && f.isOpen());
		font.load(f);
	}
}

} // End of namespace Legend
