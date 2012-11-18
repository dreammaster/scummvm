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
 * You may also modify/distribute the code in this file under that license.
 */

#include "engines/ags/ags.h"
#include "engines/ags/character.h"
#include "engines/ags/constants.h"
#include "engines/ags/drawable.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"
#include "engines/ags/gui.h"
#include "engines/ags/overlay.h"
#include "engines/ags/room.h"
#include "engines/ags/sprites.h"

#include "common/array.h"
#include "common/events.h"

#include "engines/util.h"

#include "graphics/font.h"
#include "graphics/fonts/ttf.h"
#include "graphics/palette.h"

namespace AGS {

class WFNFont : public Graphics::Font {
	struct WFNFontGlyph {
		uint16 width, height;
		byte *data;
	};

public:
	WFNFont(Common::SeekableReadStream *stream) : _maxCharWidth(0), _maxCharHeight(0) {
		const char *WFN_FILE_SIGNATURE = "WGT Font File  ";

		char buffer[16];
		stream->read(buffer, 15);
		buffer[15] = '\0';
		if (Common::String(buffer) != WFN_FILE_SIGNATURE)
			error("bad WFN font signature");
		uint16 tableOffset = stream->readUint16LE();

		_glyphs.resize(128);
		for (uint i = 0; i < 128; ++i) {
			stream->seek(tableOffset + i*2);
			uint16 charOffset = stream->readUint16LE();
			stream->seek(charOffset);

			_glyphs[i].width = stream->readUint16LE();
			if (_glyphs[i].width > _maxCharWidth)
				_maxCharWidth = _glyphs[i].width;
			_glyphs[i].height = stream->readUint16LE();
			if (_glyphs[i].height > _maxCharHeight)
				_maxCharHeight = _glyphs[i].height;

			uint32 dataSize = _glyphs[i].height * (((_glyphs[i].width - 1) / 8) + 1);
			_glyphs[i].data = new byte[dataSize];
			stream->read(_glyphs[i].data, dataSize);
		}
	}

	~WFNFont() {
		for (uint i = 0; i < _glyphs.size(); ++i)
			delete[] _glyphs[i].data;
	}

	int getFontHeight() const { return _maxCharHeight; }
	int getMaxCharWidth() const { return _maxCharWidth; }
	int getCharWidth(uint32 chr) const { return _glyphs[chr].width; }

	void drawChar(Graphics::Surface *surface, uint32 chr, int x, int y, uint32 color) const {
		if (chr >= 128)
			chr = '?';

		uint32 dataWidth = ((_glyphs[chr].width - 1) / 8) + 1;

		byte data = 0;
		for (uint chrY = 0; chrY < _glyphs[chr].height; ++chrY) {
			int destY = (int)chrY + y;
			if (destY < 0 || destY >= surface->h)
				continue;

			byte *src = _glyphs[chr].data + (chrY * dataWidth);
			for (uint chrX = 0; chrX < _glyphs[chr].width; ++chrX) {
				if (!(chrX % 8))
					data = *src++;
				else
					data <<= 1;

				int destX = (int)chrX + x;
				if (destX < 0 || destX >= surface->w)
					continue;

				if (!(data & 0x80))
					continue;

				void *dest = surface->getBasePtr(destX, destY);
				// FIXME: we should obey the configured text multiply factor here
				switch (surface->format.bytesPerPixel) {
				case 1:
					*(byte *)dest = color;
					break;
				case 2:
					*(uint16 *)dest = color;
					break;
				case 4:
					*(uint32 *)dest = color;
					break;
				}
			}
		}
	}

protected:
	uint _maxCharWidth, _maxCharHeight;

	Common::Array<WFNFontGlyph> _glyphs;
};

class CursorDrawable : public Drawable {
public:
	CursorDrawable(AGSEngine *vm) : _vm(vm), _mouseFrame(0), _mouseDelay(0), _currentCursor(0xffffffff), _cursorSprite(NULL) { }

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

	virtual int getDrawOrder() const { return 0; }

	virtual const Graphics::Surface *getDrawSurface() {
		return _cursorSprite->_surface;
	}

	// FIXME
	virtual uint getDrawWidth() { return _cursorSprite->_surface->w; }
	virtual uint getDrawHeight() { return _cursorSprite->_surface->h; }
	virtual uint getDrawTransparency() { return 0; }
	virtual bool isDrawMirrored() { return false; }
	virtual int getDrawLightLevel() { return 0; }
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) { }

	uint32 _currentCursor;

protected:
	AGSEngine *_vm;

	Sprite *_cursorSprite;
	bool _alphaBlendCursor;

	uint32 _mouseFrame, _mouseDelay;
	uint32 _hotspotX, _hotspotY;
};

AGSGraphics::AGSGraphics(AGSEngine *vm) : _vm(vm), _width(0), _height(0), _forceLetterbox(false), _vsync(false),
	_viewportX(0), _viewportY(0), _extraDrawable(NULL) {

	_cursorObj = new CursorDrawable(_vm);
}

AGSGraphics::~AGSGraphics() {
	_backBuffer.free();

	delete _cursorObj;

	for (uint i = 0; i < _fonts.size(); ++i)
		delete _fonts[i];
}

bool AGSGraphics::getScreenSize() {
	_width = _baseWidth = 320;
	_height = _baseHeight = 200;
	_textMultiply = 1;

	switch (_vm->_gameFile->_defaultResolution) {
	case 0:
	case 1:
	case 2:
		// use defaults
		break;
	case 3:
	case 4:
		_width = 640;
		_height = 400;
		_textMultiply = 2;
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
		_textMultiply = 2;
		_vm->_gameFile->_options[OPT_LETTERBOX] = 0;
		_forceLetterbox = false;
	}

	_screenResolutionMultiplier = _width / _baseWidth;

	if ((_vm->_gameFile->_defaultResolution > 2) && _vm->getGameOption(OPT_NATIVECOORDINATES)) {
		_baseWidth *= 2;
		_baseHeight *= 2;
	}

	if (_vm->getGameOption(OPT_NOSCALEFNT))
		_textMultiply = 1;

	if (_vm->getGameOption(OPT_LETTERBOX)) {
		// FIXME: do this properly
		_height = (_height * 12) / 10;
	}

	debug(2, "target resolution: %dx%d real, %dx%d base, multiplier %d %s (depth %d)",
		_width, _height, _baseWidth, _baseHeight, _screenResolutionMultiplier,
		_vm->getGameOption(OPT_NATIVECOORDINATES) ? " (native)" : "(scaled)", _vm->_gameFile->_colorDepth);

	return true;
}

Graphics::PixelFormat AGSGraphics::getPixelFormat(bool withAlpha) const {
	switch (_vm->_gameFile->_colorDepth) {
	case 1:
		// 8bpp
		return Graphics::PixelFormat::createFormatCLUT8();
	case 2:
		// 16bpp: 565
		return Graphics::PixelFormat(2, 5, 6, 5, 0, 11, 5, 0, 0);
	default:
		if (withAlpha) {
			// 32bpp: RGB8888
			return Graphics::PixelFormat(4, 8, 8, 8, 8, 16, 8, 0, 24);
		} else {
			// 24bpp: RGB888
			return Graphics::PixelFormat(4, 8, 8, 8, 0, 16, 8, 0, 0);
		}
	}
}

uint32 AGSGraphics::resolveHardcodedColor(uint32 color) const {
	// in 8bpp mode, the hardcoded colors are (thereotically) in the palette
	if (_vm->_gameFile->_colorDepth == 1)
		return color;

	// hardcoded color values are all less than 32
	if (color >= 32)
		return color;

	const uint32 hardcodedColorTable[32] = {
		// 16 system colors
		0x000000, 0x0000A0, 0x00A000, 0x00A0A0,
		0xA00000, 0xA000A0, 0xA05000, 0xA0A0A0,
		0x505050, 0x5050FF, 0x50FF50, 0x50FFFF,
		0xFF5050, 0xFF50FF, 0xFFFF50, 0xFFFFFF,
		// 16 shades of grey
		0x000000, 0x101010, 0x202020, 0x303030,
		0x404040, 0x505050, 0x606060, 0x707070,
		0x808080, 0x909090, 0xA0A0A0, 0xB0B0B0,
		0xC0C0C0, 0xD0D0D0, 0xE0E0E0, 0xF0F0F0
	};

	const Graphics::PixelFormat inFormat(3, 8, 8, 8, 0, 16, 8, 0, 0);
	const Graphics::PixelFormat outFormat = getPixelFormat();
	uint8 r, g, b;
	inFormat.colorToRGB(hardcodedColorTable[color], r, g, b);
	return outFormat.RGBToColor(r, g, b);
}

uint32 AGSGraphics::getTransparentColor(uint depth) const {
	if (depth == 0)
		depth = _vm->_gameFile->_colorDepth;

	switch (depth) {
	case 1:
		return 0;
	case 2:
		return 0xf81f;
	default:
		return 0xff00ff;
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

void AGSGraphics::loadFonts() {
	_fonts.resize(_vm->_gameFile->_fonts.size());
	for (uint i = 0; i < _fonts.size(); ++i) {
		AGSFont &font = _vm->_gameFile->_fonts[i];

		// calculate the font size (only used for TTF)
		uint fontSize = font._flags & FFLG_SIZEMASK;
		if (fontSize == 0)
			fontSize = 8;
		if (!_vm->getGameOption(OPT_NOSCALEFNT) && _vm->_gameFile->_defaultResolution > 2)
			fontSize *= 2;

		// prefer TTF
		Common::SeekableReadStream *stream = _vm->getFile(Common::String::format("agsfnt%d.ttf", i));
		if (stream) {
			bool antialias = (_vm->_gameFile->_colorDepth != 1) && _vm->getGameOption(OPT_ANTIALIASFONTS);
			antialias = false; // FIXME: AA causes color-key artifacts at present
			Graphics::TTFSizeMode fontSizeMode = Graphics::kTTFSizeModeCharacter;
			_fonts[i] = Graphics::loadTTFFont(*stream, fontSize, fontSizeMode, !antialias);
			if (!_fonts[i])
				error("loadTTFFont returned NULL for font %d", i);
			delete stream;
			continue;
		}
		// try WFN
		stream = _vm->getFile(Common::String::format("agsfnt%d.wfn", i));
		// fall back to font 0 if needed
		if (!stream)
			stream = _vm->getFile("agsfnt0.wfn");
		if (!stream)
			error("couldn't find font %d", i);
		_fonts[i] = new WFNFont(stream);
		delete stream;
	}
}

Graphics::Font *AGSGraphics::getFont(uint id) {
	if (id >= _fonts.size())
		error("game used font %d, but only %d fonts exist", id, _fonts.size());
	if (!_fonts[id])
		error("getFont: font is NULL for font %d", id);

	return _fonts[id];
}

void AGSGraphics::initPalette() {
	for (uint i = 0; i < 256; ++i) {
		if (_vm->_gameFile->_paletteUses[i] != PAL_BACKGROUND) {
			_palette[i * 3 + 0] = _vm->_gameFile->_defaultPalette[i * 3 + 0] * 4;
			_palette[i * 3 + 1] = _vm->_gameFile->_defaultPalette[i * 3 + 1] * 4;
			_palette[i * 3 + 2] = _vm->_gameFile->_defaultPalette[i * 3 + 2] * 4;
		}
	}
}

void AGSGraphics::newRoomPalette() {
	const byte *roomPal = _vm->getCurrentRoom()->_backgroundScenes[_vm->_state->_bgFrame]._palette;
	for (uint i = 0; i < 256; ++i) {
		if (_vm->_gameFile->_paletteUses[i] == PAL_BACKGROUND) {
			_palette[i * 3 + 0] = roomPal[i * 3 + 0];
			_palette[i * 3 + 1] = roomPal[i * 3 + 1];
			_palette[i * 3 + 2] = roomPal[i * 3 + 2];
		} else {
			// FIXME: patch room palette
		}
	}
}

void AGSGraphics::drawOutlinedString(uint fontId, Graphics::Surface *surface, const Common::String &text, int x, int y, uint width, uint32 color) {
	uint32 outlineColor = resolveHardcodedColor(_vm->_state->_speechTextShadow);
	if (surface->format.aShift) {
		color |= 0xff000000;
		outlineColor |= 0xff000000;
	}
	Graphics::Font *font = _fonts[fontId];

	if (_vm->_gameFile->_fonts[fontId]._outline >= 0) {
		// TODO: make sure this is sanity-checked
		Graphics::Font *outlineFont = _fonts[_vm->_gameFile->_fonts[fontId]._outline];
		outlineFont->drawString(surface, text, x, y, width, outlineColor);
	} else if (_vm->_gameFile->_fonts[fontId]._outline == FONT_OUTLINE_AUTO) {
		uint outlineDist = 1;
		// FIXME: following should also check it's not ttf
		if (!_vm->getGameOption(OPT_NOSCALEFNT) && false) {
			// // if it's a scaled up SCI font, move the outline out more
			outlineDist = _vm->getFixedPixelSize(1);
		}

		// move the text over so that it's still within the bounding rect
		x += outlineDist;
		y += outlineDist;

		font->drawString(surface, text, x - outlineDist, y, width, outlineColor);
		font->drawString(surface, text, x + outlineDist, y, width, outlineColor);
		font->drawString(surface, text, x, y + outlineDist, width, outlineColor);
		font->drawString(surface, text, x, y - outlineDist, width, outlineColor);
		font->drawString(surface, text, x - outlineDist, y - outlineDist, width, outlineColor);
		font->drawString(surface, text, x - outlineDist, y + outlineDist, width, outlineColor);
		font->drawString(surface, text, x + outlineDist, y - outlineDist, width, outlineColor);
		font->drawString(surface, text, x + outlineDist, y + outlineDist, width, outlineColor);
	}

	font->drawString(surface, text, x, y, width, color);
}

struct DrawableLess {
	bool operator()(const Drawable *a, const Drawable *b) const {
		int baseLineA = a->getDrawOrder();
		int baseLineB = b->getDrawOrder();
		if (baseLineA == baseLineB) {
			if (a->priorityIfEqual())
				return false;
			if (b->priorityIfEqual())
				return true;
		}
		return baseLineA < baseLineB;
	}
};

// plugin
// TODO: make more modular (see also gamefile, ags.cpp)
void drawSnowRain();

void AGSGraphics::draw() {
	// update palette
	// TODO: be smarter
	if (_vm->_gameFile->_colorDepth == 1)
		g_system->getPaletteManager()->setPalette(_palette, 0, 256);

	_backBuffer.fillRect(Common::Rect(0, 0, _backBuffer.w, _backBuffer.h), 0);

	/*
	 * This draws the screen. First, the current room background is drawn.
	 * Then, room walkbehinds, objects and characters are drawn, sorted
	 * by their baselines. After them come non-text overlays, then GUIs,
	 * then text overlays, and finally the cursor.
	 */
	Room *room = _vm->getCurrentRoom();

	// draw the current room background
	draw(_vm->getCurrentRoom(), true);

	// add the walkbehinds, objects and characters to an array, then sort it
	Common::Array<Drawable *> drawables;

	for (uint i = 0; i < room->_objects.size(); ++i) {
		if (!room->_objects[i]->isVisible())
			continue;

		drawables.push_back(room->_objects[i]);
	}

	for (uint i = 0; i < _vm->_characters.size(); ++i) {
		if (!_vm->_characters[i]->_on)
			continue;
		if (_vm->_characters[i]->_room != _vm->getCurrentRoomId())
			continue;

		drawables.push_back(_vm->_characters[i]);
	}

	for (uint i = 0; i < room->_walkBehinds.size(); ++i) {
		if (!room->_walkBehinds[i]._surface.getPixels())
			continue;

		drawables.push_back(&room->_walkBehinds[i]);
	}

	// TODO: need stable sort?
	Common::sort(drawables.begin(), drawables.end(), DrawableLess());

	for (uint i = 0; i < drawables.size(); ++i)
		draw(drawables[i], true);

	// TODO: make this suck less
	drawSnowRain();

	// draw overlays, except text boxes
	for (uint i = 0; i < _vm->_overlays.size(); ++i) {
		if (_vm->_overlays[i]->getType() == OVER_TEXTMSG)
			continue;

		// FIXME: draw OVER_COMPLETE in non-transparent mode
		draw(_vm->_overlays[i]);
	}

	bool guisTurnedOffAsDisabled = (_vm->_guiDisabledStyle == GUIDIS_GUIOFF) && _vm->_guiDisabledState;

	// draw GUIs
	for (uint i = 0; i < _vm->_gameFile->_guiGroups.size(); ++i) {
		GUIGroup *group = _vm->_gameFile->_guiGroupDrawOrder[i];
		if (!group->_visible)
			continue;
		if (guisTurnedOffAsDisabled && (group->_popup != POPUP_NOAUTOREM))
			continue;
		draw(group);
	}

	// draw text overlays (so that they appear over GUIs)
	for (uint i = 0; i < _vm->_overlays.size(); ++i) {
		if (_vm->_overlays[i]->getType() != OVER_TEXTMSG)
			continue;

		draw(_vm->_overlays[i]);
	}

	if (_extraDrawable)
		draw(_extraDrawable);

	_cursorObj->tick();
	if (!_vm->_state->_mouseCursorHidden)
		draw(_cursorObj);

	// finally, update the screen
	// FIXME: add dirty rectangling
	g_system->copyRectToScreen((byte *)_backBuffer.getPixels(), _width * _vm->_gameFile->_colorDepth, 0, 0, _width, _height);
	g_system->updateScreen();
}

void AGSGraphics::internalDraw(const Graphics::Surface *srcSurf, const Common::Point &pos, uint transparency) {
	blit(srcSurf, &_backBuffer, pos, transparency);
}

void AGSGraphics::draw(Drawable *item, bool useViewport) {
	uint transparency = item->getDrawTransparency();

	Common::Point pos = item->getDrawPos();
	if (useViewport)
		pos -= Common::Point(_viewportX, _viewportY);
	uint itemWidth = item->getDrawWidth();
	uint itemHeight = item->getDrawHeight();
	if (!itemWidth || !itemHeight)
		return;
	bool mirrored = item->isDrawMirrored();
	const Graphics::Surface *surface = item->getDrawSurface();

	// FIXME: lots of things
	blit(surface, &_backBuffer, pos, transparency, mirrored);
}

void AGSGraphics::blit(const Graphics::Surface *srcSurf, Graphics::Surface *destSurf, Common::Point pos, uint transparency,
	bool mirrored) {
	if (transparency == 255)
		return;

	// FIXME: fix the bounds checks for mirrored sprites

	// ignore surfaces which are entirely off-screen
	if (pos.x > destSurf->w)
		return;
	if (pos.y > destSurf->h)
		return;

	// work out if we want to start partially into a source surface,
	// because some of it is off-screen
	uint startX = 0, startY = 0;
	if (pos.x < 0) {
		startX = -pos.x;
		if (startX > srcSurf->w)
			return;
		pos.x = 0;
	}
	if (pos.y < 0) {
		startY = -pos.y;
		if (startY > srcSurf->h)
			return;
		pos.y = 0;
	}

	// work out how much of the surface we want to draw, given some
	// of it might be off-screen
	uint width = srcSurf->w - startX, height = srcSurf->h - startY;
	if (pos.x + width > destSurf->w)
		width = destSurf->w - pos.x;
	if (pos.y + height > destSurf->h)
		height = destSurf->h - pos.y;

	if (srcSurf->format.bytesPerPixel == 1) {
		for (uint y = 0; y < height; ++y) {
			byte *dest;
			if (mirrored)
				dest = (byte *)destSurf->getBasePtr(pos.x + width, pos.y + y);
			else
				dest = (byte *)destSurf->getBasePtr(pos.x, pos.y + y);
			const byte *src = (byte *)srcSurf->getBasePtr(startX, startY + y);
			for (uint x = 0; x < width; ++x) {
				byte data = *src++;
				if (data != 0)
					*dest = data;
				if (mirrored)
					dest--;
				else
					dest++;
			}
		}
	} else if (srcSurf->format.bytesPerPixel == 2) {
		uint16 transColor = (uint16)getTransparentColor();

		if (!transparency) {
			// simplified version of the loop below for the transparent==0 (opaque) case
			for (uint y = 0; y < height; ++y) {
				uint16 *dest;
				if (mirrored)
					dest = (uint16 *)destSurf->getBasePtr(pos.x + width, pos.y + y);
				else
					dest = (uint16 *)destSurf->getBasePtr(pos.x, pos.y + y);
				const uint16 *src = (uint16 *)srcSurf->getBasePtr(startX, startY + y);
				for (uint x = 0; x < width; ++x) {
					uint16 srcData = *src++;
					if (srcData != transColor)
						*dest = srcData;
					if (mirrored)
						dest--;
					else
						dest++;
				}
			}
			return;
		}

		transparency = (transparency + 1) / 8;

		for (uint y = 0; y < height; ++y) {
			uint16 *dest;
			if (mirrored)
				dest = (uint16 *)destSurf->getBasePtr(pos.x + width, pos.y + y);
			else
				dest = (uint16 *)destSurf->getBasePtr(pos.x, pos.y + y);
			const uint16 *src = (uint16 *)srcSurf->getBasePtr(startX, startY + y);
			for (uint x = 0; x < width; ++x) {
				uint16 srcData = *src++;
				if (srcData != transColor) {
					uint16 destData = *dest;

					uint32 blendDest = (destData | (destData << 16)) & 0x7E0F81F;
					uint32 blendSrc = (srcData | (srcData << 16)) & 0x7E0F81F;
					uint32 blended = (blendSrc - blendDest) * transparency / 32 + blendDest;
					blended &= 0x7E0F81F;
					*dest = (blended & 0xFFFF) | (blended >> 16);
				}
				if (mirrored)
					dest--;
				else
					dest++;
			}
		}
	} else {
		uint32 transColor = (uint32)getTransparentColor();

		if (srcSurf->format.aShift) {
			for (uint y = 0; y < height; ++y) {
				uint32 *dest;
				if (mirrored)
					dest = (uint32 *)destSurf->getBasePtr(pos.x + width, pos.y + y);
				else
					dest = (uint32 *)destSurf->getBasePtr(pos.x, pos.y + y);
				const uint32 *src = (uint32 *)srcSurf->getBasePtr(startX, startY + y);
				for (uint x = 0; x < width; ++x) {
					uint32 srcData = *src++;
					if ((srcData & 0xffffff) != transColor) {
						uint32 destData = *dest;

						transparency = srcData >> 24;
						if (destSurf->format.aShift) {
							// alpha onto alpha; additive
							uint alpha = MIN<uint>(0xff, transparency + (destData >> 24));
							*dest = (alpha << 24) | (srcData & 0xffffff);
						} else {
							// alpha onto non-alpha; blend
							if (transparency)
								transparency++;
							uint32 blendedRB = ((srcData & 0xFF00FF) - (destData & 0xFF00FF))
								* transparency / 256 + (destData & 0xFF00FF);
							uint32 blended = ((srcData & 0xFF00) - (destData & 0xFF00))
								* transparency / 256 + (destData & 0xFF00);
							*dest = (blended & 0xFF00) | (blendedRB & 0xFF00FF) | 0xFF000000;
						}
					}
					if (mirrored)
						dest--;
					else
						dest++;
				}
			}
			return;
		}

		if (!transparency) {
			// simplified version of the loop below for the transparent==0 (opaque) case
			for (uint y = 0; y < height; ++y) {
				uint32 *dest;
				if (mirrored)
					dest = (uint32 *)destSurf->getBasePtr(pos.x + width, pos.y + y);
				else
					dest = (uint32 *)destSurf->getBasePtr(pos.x, pos.y + y);
				const uint32 *src = (uint32 *)srcSurf->getBasePtr(startX, startY + y);
				for (uint x = 0; x < width; ++x) {
					uint32 srcData = *src++;
					if ((srcData & 0xffffff) != transColor)
						*dest = srcData | 0xff000000;
					if (mirrored)
						dest--;
					else
						dest++;
				}
			}
			return;
		}

		transparency = transparency + 1;

		for (uint y = 0; y < height; ++y) {
			uint32 *dest;
			if (mirrored)
				dest = (uint32 *)destSurf->getBasePtr(pos.x + width, pos.y + y);
			else
				dest = (uint32 *)destSurf->getBasePtr(pos.x, pos.y + y);
			const uint32 *src = (uint32 *)srcSurf->getBasePtr(startX, startY + y);
			for (uint x = 0; x < width; ++x) {
				uint32 srcData = *src++;
				if ((srcData & 0xffffff) != transColor) {
					uint32 destData = *dest;

					uint32 blendedRB = ((srcData & 0xFF00FF) - (destData & 0xFF00FF))
						* transparency / 256 + destData;
					uint32 blended = ((srcData & 0xFF00) - (destData & 0xFF00))
						* transparency / 256 + (destData & 0xFF00);
					*dest = (blended & 0xFF00) | (blendedRB & 0xFF00FF) | 0xFF000000;
				}
				if (mirrored)
					dest--;
				else
					dest++;
			}
		}
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

void AGSGraphics::checkViewportCoords() {
	uint roomWidth = _vm->multiplyUpCoordinate(_vm->getCurrentRoom()->_width);
	uint roomHeight = _vm->multiplyUpCoordinate(_vm->getCurrentRoom()->_height);
	if (_viewportX + _width > roomWidth)
		_viewportX = roomWidth - _width;
	if (_viewportY + _height > roomHeight)
		_viewportY = roomHeight - _height;
}

Drawable::Drawable() {
}

Drawable::~Drawable() {
}

// 'is_pos_in_sprite' in original
bool Drawable::containsPoint(AGSEngine *vm, Common::Point point) {
	Common::Point pos = getDrawPos();
	pos.x = vm->divideDownCoordinate(pos.x);
	pos.y = vm->divideDownCoordinate(pos.y);

	uint width = vm->divideDownCoordinate(getDrawWidth());
	uint height = vm->divideDownCoordinate(getDrawHeight());

	if (!Common::Rect(pos.x, pos.y, pos.x + width, pos.y + height).contains(point))
		return false;

	if (!vm->getGameOption(OPT_PIXPERFECT))
		return true;

	point.x = vm->multiplyUpCoordinate(point.x - pos.x);
	point.y = vm->multiplyUpCoordinate(point.y - pos.y);

	const Graphics::Surface *surface = getDrawSurface();

	// FIXME: stretching

	if (isDrawMirrored())
		point.x = (surface->w - 1) - point.x;

	// TODO: sanity-check it's actually in the surface?
	const void *ptr = surface->getBasePtr(point.x, point.y);
	switch (surface->format.bytesPerPixel) {
	case 1:
		if (*(byte *)ptr == vm->_graphics->getTransparentColor())
			return false;
		break;
	case 2:
		if (*(uint16 *)ptr == vm->_graphics->getTransparentColor())
			return false;
		break;
	case 4:
		if ((*(uint32 *)ptr & 0xffffff) == vm->_graphics->getTransparentColor())
			return false;
		break;
	default:
		error("Drawable::containsPoint: %dBpp not supported", surface->format.bytesPerPixel);
	}

	return true;
}

} // End of namespace AGS
