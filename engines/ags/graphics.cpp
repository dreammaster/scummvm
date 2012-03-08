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
#include "engines/ags/drawable.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"
#include "engines/ags/room.h"
#include "engines/ags/sprites.h"

#include "common/array.h"
#include "common/events.h"

#include "engines/util.h"

#include "graphics/palette.h"

namespace AGS {

class CursorDrawable : public Drawable {
public:
	CursorDrawable(AGSEngine *vm) : _vm(vm), _mouseFrame(0), _mouseDelay(0), _currentCursor(0xffffffff) { }

	void setMouseCursor(uint32 cursor) {
		assert(cursor < _vm->_gameFile->_cursors.size());

		const MouseCursor &cursorInfo = _vm->_gameFile->_cursors[cursor];

		setCursorGraphic(cursorInfo._pic);
		/* FIXME if (_dottedMouseCursor) {
			delete _dottedMouseCursor;
			_dottedMouseCursor = NULL;
		} */

		if ((cursor == MODE_USE) && cursorInfo._pic && (_vm->_gameFile->_hotDot || _vm->_gameFile->_invHotDotSprite)) {
			// create a copy of cursor with the hotspot dot onto it, if needed
			/* FIXME: duplicate active mouse cursor onto _dottedMouseCursor */

			if (_vm->_gameFile->_invHotDotSprite) {
				// FIXME: draw invHotDotSprite centered on hotspot
			} else {
				// FIXME: draw pixel
				if (_vm->_gameFile->_hotDotOuter) {
					// FIXME: draw pixels
				}
			}

			// FIXME: replace active mouse cursor with _dottedMouseCursor
			updateCachedMouseCursor();
		}

		setHotspot(cursorInfo._hotspotX, cursorInfo._hotspotY);

		if (cursor != _currentCursor) {
			_currentCursor = cursor;
			_mouseFrame = 0;
			_mouseDelay = 0;
		}
	}

	void setCursorGraphic(uint32 spriteId) {
		_cursorSprite = _vm->getSprites()->getSprite(spriteId);

		if (!spriteId || !_cursorSprite) {
			// FIXME
		}

		_alphaBlendCursor = (bool)(_vm->_gameFile->_spriteFlags[spriteId] & SPF_ALPHACHANNEL);

		updateCachedMouseCursor();
	}

	void setHotspot(uint32 x, uint32 y) {
		_hotspotX = x;
		_hotspotY = y;
	}

	void updateCachedMouseCursor() {
		// FIXME: set the mouse cursor
	}

	void tick() {
		assert(_currentCursor < _vm->_gameFile->_cursors.size());
		const MouseCursor &cursorInfo = _vm->_gameFile->_cursors[_currentCursor];

		if (cursorInfo._view < 0)
			return;

		if (cursorInfo._flags & MCF_ANIMMOVE) {
			// FIXME
		}

		if (cursorInfo._flags & MCF_HOTSPOT) {
			// FIXME
		}

		if (_mouseDelay > 0) {
			_mouseDelay--;
			return;
		}

		// TODO: make sure this is sanity-checked at load time
		const ViewStruct &view = _vm->_gameFile->_views[cursorInfo._view];
		if (view._loops.empty())
			error("cursor %d is using view %d which has no loops", _currentCursor, cursorInfo._view);
		if (view._loops[0]._frames.empty())
			error("cursor %d is using view %d which has no frames in loop 0", _currentCursor, cursorInfo._view);

		_mouseFrame++;
		if (_mouseFrame >= view._loops[0]._frames.size())
			_mouseFrame = 0;
		setCursorGraphic(view._loops[0]._frames[_mouseFrame]._pic);
		_mouseDelay = view._loops[0]._frames[_mouseFrame]._speed + 5;

		_vm->checkViewFrame(cursorInfo._view, 0, _mouseFrame);
	}

	virtual Common::Point getDrawPos() {
		return _vm->_system->getEventManager()->getMousePos() - Common::Point(_hotspotX, _hotspotY);
	}

	virtual int getDrawOrder() { return 0; }

	virtual const Graphics::Surface *getDrawSurface() {
		return _cursorSprite;
	}

	// FIXME
	virtual uint getDrawWidth() { return 0; }
	virtual uint getDrawHeight() { return 0; }
	virtual uint getDrawTransparency() { return 0; }
	virtual bool isDrawVerticallyMirrored() { return false; }
	virtual int getDrawLightLevel() { return 0; }
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) { }

	uint32 _currentCursor;

protected:
	AGSEngine *_vm;

	Graphics::Surface *_cursorSprite;
	bool _alphaBlendCursor;

	uint32 _mouseFrame, _mouseDelay;
	uint32 _hotspotX, _hotspotY;
};

AGSGraphics::AGSGraphics(AGSEngine *vm) : _vm(vm), _width(0), _height(0), _forceLetterbox(false) {
	_cursorObj = new CursorDrawable(_vm);
}

AGSGraphics::~AGSGraphics() {
	_backBuffer.free();

	delete _cursorObj;
}

bool AGSGraphics::getScreenSize() {
	_width = _baseWidth = 320;
	_height = _baseHeight = 200;
	_textMultiply = 2;

	switch (_vm->_gameFile->_defaultResolution) {
	case 0:
		_textMultiply = 1;
		break;
	case 1:
	case 2:
		//_width = 640;
		//_height = 480;
		break;
	case 3:
	case 4:
		//_width = 640;
		//_height = 400;
		break;
	case 5:
		_baseWidth = 400;
		_baseHeight = 300;
		break;
	default:
		_baseWidth = 512;
		_baseHeight = 384;
		break;
	}

	if (_vm->_gameFile->_defaultResolution >= 5) {
		_width = _baseWidth * 2;
		_height = _baseHeight * 2;
		_vm->_gameFile->_options[OPT_LETTERBOX] = 0;
		_forceLetterbox = false;
	}

	_screenResolutionMultiplier = _width / _baseWidth;

	return true;
}

Graphics::PixelFormat AGSGraphics::getPixelFormat() const {
	switch (_vm->_gameFile->_colorDepth) {
	case 1:
		// 8bpp
		return Graphics::PixelFormat::createFormatCLUT8();
	case 2:
		// 16bpp: 565
		return Graphics::PixelFormat(2, 5, 6, 5, 0, 11, 5, 0, 0);
	default:
		// 24bpp: RGB888
		return Graphics::PixelFormat(3, 8, 8, 8, 0, 16, 8, 0, 0);
	}
}

bool AGSGraphics::initGraphics() {
	if (!getScreenSize())
		return false;

	if ((_width == 0) || (_height == 0))
		return false;

	Graphics::PixelFormat format = getPixelFormat();
	::initGraphics(_width, _height, &format);
	// FIXME: check format?

	_backBuffer.create(_width, _height, format);

	return true;
}

void AGSGraphics::initPalette() {
	for (uint i = 0; i < 256; ++i) {
		if (_vm->_gameFile->_paletteUses[i] != PAL_BACKGROUND) {
			_palette[i * 3 + 0] = _vm->_gameFile->_defaultPalette[i * 3 + 0];
			_palette[i * 3 + 1] = _vm->_gameFile->_defaultPalette[i * 3 + 1];
			_palette[i * 3 + 2] = _vm->_gameFile->_defaultPalette[i * 3 + 2];
		}
	}
}

void AGSGraphics::newRoomPalette() {
	const byte *roomPal = _vm->getCurrentRoom()->_backgroundScenes[0]._palette;
	for (uint i = 0; i < 256; ++i) {
		if (_vm->_gameFile->_paletteUses[i] == PAL_BACKGROUND) {
			_palette[i * 3 + 0] = roomPal[i * 4 + 0];
			_palette[i * 3 + 1] = roomPal[i * 4 + 0];
			_palette[i * 3 + 2] = roomPal[i * 4 + 0];
		} else {
			// FIXME: patch room palette
		}
	}
}

void AGSGraphics::draw() {
	// update palette
	// TODO: be smarter
	if (_vm->_gameFile->_colorDepth == 1)
		g_system->getPaletteManager()->setPalette(_palette, 0, 256);

	/*
	 * This draws the screen. First, the current room background is drawn.
	 * Then, room walkbehinds, objects and characters are drawn, sorted
	 * by their baselines. After them come non-text overlays, then GUIs,
	 * then text overlays, and finally the cursor.
	 */
	Room *room = _vm->getCurrentRoom();

	// draw the current room background
	draw(_vm->getCurrentRoom());

	// add the walkbehinds, objects and characters to an array, then sort it
	Common::Array<Drawable *> drawables;

	for (uint i = 0; i < room->_objects.size(); ++i) {
		if (!room->_objects[i]->_on)
			continue;

		drawables.push_back(room->_objects[i]);
	}

	// FIXME

	for (uint i = 0; i < drawables.size(); ++i)
		draw(drawables[i]);

	_cursorObj->tick();
	if (!_vm->_state->_mouseCursorHidden)
		draw(_cursorObj);

	// finally, update the screen
	// FIXME: add dirty rectangling
	g_system->copyRectToScreen((byte *)_backBuffer.getPixels(), _width * _vm->_gameFile->_colorDepth, 0, 0, _width, _height);
	g_system->updateScreen();
}

void AGSGraphics::draw(Drawable *item) {
	const Common::Point pos = item->getDrawPos();
	const Graphics::Surface *surface = item->getDrawSurface();

	// FIXME: lots of things

	uint startX = 0, startY = 0;
	if (pos.x < 0)
		startX = -pos.x;
	if (pos.y < 0)
		startY = -pos.y;
	uint width = surface->w, height = surface->h;
	if (pos.x + width > _width)
		width = _width - pos.x;
	if (pos.y + height > _height)
		height = _height - pos.y;

	if (surface->format.bytesPerPixel == 1) {
		for (uint y = 0; y < height - startY; ++y) {
			byte *dest = (byte *)_backBuffer.getBasePtr(pos.x + startX, pos.y + y);
			const byte *src = (byte *)surface->getBasePtr(startX, startY + y);
			for (uint x = startX; x < width; ++x) {
				byte data = *src++;
				if (data != 0)
					*dest = data;
				dest++;
			}
		}
	} else if (surface->format.bytesPerPixel == 2) {
		for (uint y = 0; y < height - startY; ++y) {
			uint16 *dest = (uint16 *)_backBuffer.getBasePtr(pos.x + startX, pos.y + y);
			const uint16 *src = (uint16 *)surface->getBasePtr(startX, startY + y);
			for (uint x = startX; x < width; ++x) {
				uint16 data = *src++;
				if (data != 0xf81f)
					*dest = data;
				dest++;
			}
		}
	} else {
		// FIXME
		warning("blub");
	}
}

void AGSGraphics::setMouseCursor(uint32 cursor) {
	_cursorObj->setMouseCursor(cursor);
}

void AGSGraphics::mouseSetHotspot(uint32 x, uint32 y) {
	_cursorObj->setHotspot(x, y);
}

void AGSGraphics::setCursorGraphic(uint32 spriteId) {
	_cursorObj->setCursorGraphic(spriteId);
}

uint32 AGSGraphics::getCurrentCursor() {
	return _cursorObj->_currentCursor;
}

void AGSGraphics::updateCachedMouseCursor() {
	_cursorObj->updateCachedMouseCursor();
}

Drawable::Drawable() {
}

Drawable::~Drawable() {
}

} // End of namespace AGS
