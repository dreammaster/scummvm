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

void VisualItem::init() {
	_isDirty = true;
	_fontNumber = -1;
	_fontHorizSpacings = -1;
	_fontDetailsFlags1 = 0;
}

bool VisualItem::ShowMsg(CShowMsg &msg) {
	// When a view is shown, mark it to be redrawn
	_isDirty = true;
	Font *font = Font::getActiveFont();
	_fontNumber = font ? font->_fontNumber : -1;

	return false;
}

bool VisualItem::HideMsg(CHideMsg &msg) {
	// When view is hidden, mark it as not dirty
	_isDirty = false;
	return false;
}

Graphics::ManagedSurface VisualItem::getSurface() const {
	assert(!_bounds.isEmpty());
	return Graphics::ManagedSurface(Screen::get(), _bounds);
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

void VisualItem::changeView(const Common::String &name) {
	getGameManager()->changeView(name);
}

Font *VisualItem::loadFont(int fontNumber) {
	init();
	Font *font = Font::loadFont(fontNumber);
	assert(font);

	_fontNumber = font->_fontNumber;
	_fontCenter = Common::Point(font->_xCenter, font->_yCenter);
	return font;
}

void VisualItem::setFontColor(int fgColor, int bgColor) {
	Font::setColor(fgColor, bgColor);
}

void VisualItem::writeString(const Common::String &msg) {
	// Get the surface area the visual item covers
	Graphics::ManagedSurface surface = getSurface();

	// Ensure the correct font is active
	Font *font = Font::_activeFont;
	if (Font::_activeFont->_fontNumber != _fontNumber) {
		assert(_fontNumber > 0);
		loadFont(_fontNumber);
	}

	// Write out the text
	font->writeString(surface, _fontWritePos, msg);
}

} // End of namespace Gfx
} // End of namespace Legend
