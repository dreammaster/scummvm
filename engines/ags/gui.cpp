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
#include "engines/ags/gamefile.h"
#include "engines/ags/graphics.h"
#include "engines/ags/gui.h"
#include "engines/ags/util.h"

#include "graphics/font.h"

namespace AGS {

bool GUIControl::isOverControl(const Common::Point &pos, uint extra) {
	Common::Rect r(_x, _y, _x + _width + extra, _y + _height + extra);
	return r.contains(pos);
}

void GUIControl::resize(uint32 width, uint32 height) {
	_width = width;
	_height = height;
}

bool GUIControl::isDisabled() {
	if (_flags & GUIF_DISABLED)
		return true;

	// FIXME: global button disabling

	return false;
}

void GUIControl::setClickable(bool value) {
	if (value)
		_flags &= ~GUIF_NOCLICKS;
	else
		_flags |= GUIF_NOCLICKS;
}

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

void GUISlider::setMin(int32 value) {
	if (_min == value)
		return;

	_min = value;
	_parent->invalidate();
}

void GUISlider::setMax(int32 value) {
	if (_max == value)
		return;

	_max = value;
	_parent->invalidate();
}

void GUISlider::setValue(int32 value) {
	if (_value == value)
		return;

	_value = value;
	_parent->invalidate();
}

void GUISlider::setHandleOffset(int32 value) {
	if (_handleOffset == value)
		return;

	_handleOffset = value;
	_parent->invalidate();
}

void GUISlider::draw(Graphics::Surface *surface) {
	warning("GUISlider::draw unimplemented");
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

void GUILabel::setFont(uint32 font) {
	if (_font == font)
		return;

	assert(font < _vm->_gameFile->_fonts.size());

	_font = font;
	_parent->invalidate();
}

void GUILabel::setColor(uint32 color) {
	if (_textColor == color)
		return;

	_textColor = color;
	_parent->invalidate();
}

void GUILabel::setAlign(uint32 align) {
	if (_align == align)
		return;

	assert(align < 3);

	_align = align;
	_parent->invalidate();
}

void GUILabel::setText(Common::String text) {
	if (_text == text)
		return;

	_text = text;
	_parent->invalidate();
}

void GUILabel::draw(Graphics::Surface *surface) {
	Common::String text = _vm->replaceMacroTokens(_vm->getTranslation(_text));
	uint32 color = _vm->_graphics->resolveHardcodedColor(_textColor);
	Graphics::Font *font = _vm->_graphics->getFont(_font);

	Common::Array<Common::String> lines;
	font->wordWrapText(text, _width, lines);

	uint y = 0;
	for (uint i = 0; i < lines.size(); ++i) {
		// TODO: fix align elsewhere?
		Graphics::TextAlign align;
		switch (_align) {
		case GALIGN_LEFT:
			align = Graphics::kTextAlignLeft;
			break;
		case GALIGN_CENTRE:
			align = Graphics::kTextAlignCenter;
			break;
		case GALIGN_RIGHT:
			align = Graphics::kTextAlignRight;
			break;
		default:
			error("GUILabel::draw: invalid alignment %d", _align);
		}
		// FIXME: draw with outline
		font->drawString(surface, lines[i], _x, _y + y, _width, color, align);
		y += font->getFontHeight() + 1;
		if (y > _height)
			return;
	}
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

void GUITextBox::draw(Graphics::Surface *surface) {
	error("GUITextBox::draw unimplemented");
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

void GUIListBox::draw(Graphics::Surface *surface) {
	error("GUIListBox::draw unimplemented");
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

void GUIInvControl::draw(Graphics::Surface *surface) {
	error("GUIInvControl::draw unimplemented");
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

void GUIButton::setNormalGraphic(uint32 pic) {
	if (_pic == pic)
		return;

	_pic = pic;
	if ((!_isOver || (int)_overPic < 1) && !_isPushed)
		_usePic = pic;

	// FIXME: resize self to size of sprite

	_parent->invalidate();
	stopAnimation();
}

void GUIButton::setMouseOverGraphic(uint32 pic) {
	if (_overPic == pic)
		return;

	_overPic = pic;
	if (_isOver && !_isPushed)
		_usePic = pic;

	_parent->invalidate();
	stopAnimation();
}

void GUIButton::setPushedGraphic(uint32 pic) {
	if (_pushedPic == pic)
		return;

	_pushedPic = pic;
	if (_isPushed)
		_usePic = pic;

	_parent->invalidate();
	stopAnimation();
}

void GUIButton::setText(Common::String text) {
	if (_text == text)
		return;

	_text = text;
	_parent->invalidate();
}

void GUIButton::stopAnimation() {
	// FIXME
}

void GUIButton::draw(Graphics::Surface *surface) {
	bool drawDisabled = isDisabled();

	// FIXME: disabled style

	if ((int)_usePic <= 0 || drawDisabled)
		_usePic = _pic;

	// FIXME: disabled style

	// First, we draw the graphical bits.
	if ((int)_usePic > 0 && (int)_pic > 0) {
		// graphical button

		// FIXME
	} else if (_text.size()) {
		// text button

		// FIXME
	}

	// Then, we try drawing the text (if any).
	if (_text.empty())
		return;
	// Don't print text of (INV) (INVSHR) (INVNS)
	if (_text.hasPrefix("(IN"))
		return;
	// Don't print the text if there's a graphic and it hasn't been named
	if (_text == "New Button" && (int)_usePic > 0 && (int)_pic > 0)
		return;

	Common::String text = _vm->getTranslation(_text);

	int useX = _x, useY = _y;

	// move the text a bit while pushed
	if (_isPushed && _isOver) {
		useX++;
		useY++;
	}

	Graphics::Font *font = _vm->_graphics->getFont(_font);
	uint fontHeight = font->getFontHeight();

	Graphics::TextAlign align;
	switch (_textAlignment) {
	case GBUT_ALIGN_TOPMIDDLE:
		align = Graphics::kTextAlignCenter;
		useY += 2;
		break;
	case GBUT_ALIGN_TOPLEFT:
		align = Graphics::kTextAlignLeft;
		useX += 2;
		useY += 2;
		break;
	case GBUT_ALIGN_TOPRIGHT:
		align = Graphics::kTextAlignRight;
		useX -= 2;
		useY += 2;
		break;
	case GBUT_ALIGN_MIDDLELEFT:
		align = Graphics::kTextAlignLeft;
		useX += 2;
		useY += (_height / 2) - ((fontHeight + 1) / 2);
		break;
	case GBUT_ALIGN_CENTRED:
		align = Graphics::kTextAlignCenter;
		useX -= 2;
		useY += (_height / 2) - ((fontHeight + 1) / 2);
		break;
	case GBUT_ALIGN_MIDDLERIGHT:
		align = Graphics::kTextAlignRight;
		useX -= 2;
		useY += (_height / 2) - ((fontHeight + 1) / 2);
		break;
	case GBUT_ALIGN_BOTTOMLEFT:
		align = Graphics::kTextAlignLeft;
		useX += 2;
		useY += _height - fontHeight - 2;
		break;
	case GBUT_ALIGN_BOTTOMMIDDLE:
		align = Graphics::kTextAlignCenter;
		useY += _height - fontHeight - 2;
		break;
	case GBUT_ALIGN_BOTTOMRIGHT:
		align = Graphics::kTextAlignRight;
		useX -= 2;
		useY += _height - fontHeight - 2;
		break;
	}

	uint32 color = _vm->_graphics->resolveHardcodedColor(_textColor);
	if (drawDisabled)
		color = _vm->_graphics->resolveHardcodedColor(8);
	// TODO: is this right?
	font->drawString(surface, text, useX, useY, _width, color, align);
}

GUIGroup::GUIGroup(AGSEngine *vm) : _vm(vm), _width(0), _height(0), _needsUpdate(true) {
}

GUIGroup::~GUIGroup() {
	_surface.free();
}

void GUIGroup::setSize(uint32 width, uint32 height) {
	if (_surface.getPixels() && width == _width && height == _height)
		return;

	_width = width;
	_height = height;
	_surface.free();

	if (!_on)
		return;

	_surface.create(width, height, _vm->_graphics->getPixelFormat());

	invalidate();
}

void GUIGroup::setBackgroundPicture(uint32 pic) {
	if (pic == _bgPic)
		return;

	_bgPic = pic;
	invalidate();
}

void GUIGroup::invalidate() {
	_needsUpdate = true;
}

void GUIGroup::controlPositionsChanged() {
	// force it to re-check for which control is under the mouse
	_mouseWasX = -1;
	_mouseWasY = -1;
}

bool GUIGroup::isMouseOver(const Common::Point &pos) {
	if (_on < 1)
		return false;

	if (_flags & GUIF_NOCLICK)
		return false;

	return (pos.x >= _x && pos.y >= _y && pos.x <= _x + (int)_width && pos.y <= _y + (int)_height);
}

void GUIGroup::setVisible(bool visible) {
	if ((bool)_on == visible)
		return;

	_on = visible ? 1 : 0;
	if (!_on)
		_surface.free();
	else
		setSize(_width, _height);
}

const Graphics::Surface *GUIGroup::getDrawSurface() {
	assert(_surface.getPixels());

	if (_needsUpdate)
		draw();

	return &_surface;
}

void GUIGroup::draw() {
	// clear the surface, filling with either transparency or the background color
	uint32 bgColor;
	if (_bgColor != 0)
		bgColor = _vm->_graphics->resolveHardcodedColor(_bgColor);
	else
		bgColor = _vm->_graphics->getTransparentColor();
	_surface.fillRect(Common::Rect(0, 0, _width, _height), bgColor);

	if (_bgColor != _fgColor) {
		// draw the border
		// FIXME
		warning("ignoring border");
	}

	if ((int)_bgPic > 0) {
		// draw the background picture
		// FIXME
		warning("ignoring bgpic");
	}

	for (uint i = 0; i < _controls.size(); ++i) {
		uint16 controlId = _controlDrawOrder[i];
		GUIControl *control = _controls[controlId];

		// only visible controls should be drawn
		if (!control->isVisible())
			continue;

		// if disabled controls shouldn't be drawn, don't draw them
		// FIXME: check vm->_guiDisabledStyle == GUIDIS_BLACKOUT too
		if (control->isDisabled())
			continue;

		// FIXME
		control->draw(&_surface);

		// FIXME: highlighting
		// _surface.frameRect(Common::Rect(control->_x, control->_y, control->_x + control->_width, control->_y + control->_height), _vm->_graphics->resolveHardcodedColor(14));
	}

	_needsUpdate = false;
}

struct GUIZOrderLess {
	bool operator()(const GUIControl *a, const GUIControl *b) const {
		return a->_zorder < b->_zorder;
	}
};

void GUIGroup::sortControls() {
	Common::Array<GUIControl *> controls = _controls;

	Common::sort(controls.begin(), controls.end(), GUIZOrderLess());

	_controlDrawOrder.resize(controls.size());
	for (uint i = 0; i < controls.size(); ++i)
		_controlDrawOrder[i] = _controls[i]->_id;
}

} // End of namespace AGS
