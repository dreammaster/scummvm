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

#include "legend/gfx/visual_item.h"
#include "legend/legend.h"

namespace Legend {
namespace Gfx {

BEGIN_MESSAGE_MAP(VisualItem, NamedItem)
	ON_MESSAGE(ShowMsg)
	ON_MESSAGE(HideMsg)
END_MESSAGE_MAP()

void FontDetails::reset() {
	_fontNumber = -1;
	_horizSpacings = -1;
	_flags1 = 0;
}

/*-------------------------------------------------------------------*/

VisualSurface::VisualSurface(const Graphics::ManagedSurface &src,
		FontDetails &fontDetails, const Common::Rect &bounds) :
		Graphics::ManagedSurface(src), _font(fontDetails), _bounds(bounds) {
}

Font *VisualSurface::loadFont(int fontNumber) {
	_font.reset();
	Font *font = Font::loadFont(fontNumber);
	assert(font);

	_font._fontNumber = font->_fontNumber;
	_font._fontCenter = Common::Point(font->_xCenter, font->_yCenter);
	return font;
}

void VisualSurface::setFontColor(int fgColor, int bgColor) {
	Font::setColor(fgColor, bgColor);
}

void VisualSurface::writeString(const String &msg) {
	// Ensure the correct font is active
	Font *font = Font::_activeFont;
	if (Font::_activeFont->_fontNumber != _font._fontNumber) {
		assert(_font._fontNumber > 0);
		loadFont(_font._fontNumber);
	}

	// Write out the text
	font->writeString(*this, _font._writePos, msg);
}

void VisualSurface::writeString(const Common::Point &pt, const String &msg) {
	setTextPos(pt);
	writeString(msg);
}

int VisualSurface::stringWidth(const String &msg) {
	Font *font = Font::_activeFont;
	return font->stringWidth(msg);
}

/*-------------------------------------------------------------------*/

void VisualItem::init() {
	_isDirty = true;
}

bool VisualItem::ShowMsg(CShowMsg &msg) {
	// When a view is shown, mark it to be redrawn
	_isDirty = true;
	Font *font = Font::getActiveFont();
	_fontDetails._fontNumber = font ? font->_fontNumber : -1;

	return false;
}

bool VisualItem::HideMsg(CHideMsg &msg) {
	// When view is hidden, mark it as not dirty
	_isDirty = false;
	return false;
}

VisualSurface VisualItem::getSurface() {
	Graphics::ManagedSurface src(Screen::get(), _bounds);
	return VisualSurface(src, _fontDetails, _bounds);
}

void VisualItem::setBounds(const Common::Rect &r) {
	_bounds = r;
	setDirty();
}

void VisualItem::setPosition(const Common::Point &pt) {
	_bounds.moveTo(pt);
	setDirty();
}

void VisualItem::setDirty() {
	// Flag the item as dirty
	_isDirty = true;

	// Flag any child items also as dirty, since rendering the parent
	// would cover up any areas that the sub-items render within
	for (TreeItem *treeItem = scan(this); treeItem; treeItem = treeItem->scan(this)) {
		VisualItem *item = dynamic_cast<VisualItem *>(treeItem);
		if (item)
			item->_isDirty = true;
	}
}

void VisualItem::changeView(const String &name) {
	getGameManager()->changeView(name);
}

} // End of namespace Gfx
} // End of namespace Legend
