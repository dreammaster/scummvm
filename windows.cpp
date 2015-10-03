/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
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

#include "aesop/aesop.h"
#include "aesop/windows.h"
#include "aesop/shapes.h"
#include "aesop/shared.h"
#include "common/system.h"
#include "common/memstream.h"
#include "engines/util.h"
#include "graphics/palette.h"

namespace Aesop {

/*----------------------------------------------------------------*/

Pane::Pane() : _window(nullptr), _owner(-1), Common::Rect() {
}

Pane::Pane(Window *window) : _owner(-1), Common::Rect() {
	_window = window;
	left = window->left;
	top = window->top;
	right = window->right;
	bottom = window->bottom;
}

Pane::Pane(Window *window, const Common::Rect &bounds) : _owner(-1), Common::Rect() {
	_window = window;
	left = bounds.left;
	top = bounds.top;
	right = bounds.right;
	bottom = bounds.bottom;
}

int Pane::getLeft() const {
	return _window ? _window->left : 0;
}

int Pane::getTop() const {
	return _window ? _window->top : 0;
}

int Pane::getRight() const {
	return _window ? _window->right : 0;
}

int Pane::getBottom() const {
	return _window ? _window->bottom : 0;
}

void Pane::setLeft(int x1) {
	left = x1;
}

void Pane::setTop(int y1) {
	top = y1;
}

void Pane::setRight(int x2) {
	right = x2;
}

void Pane::setBottom(int y2) {
	bottom = y2;
}

void Pane::paneWipe(byte color) {
	fillRect(*this, color);
}

int Pane::colorScan(uint32 *colors) {
	bool colorUsed[PALETTE_COUNT];
	Common::fill(&colorUsed[0], &colorUsed[PALETTE_COUNT], 0);
	int numColors = 0;

	// Iterate through the pane
	for (int yp = top; yp <= bottom; ++yp) {
		const byte *srcP = getBasePtr(left, yp);
		for (int xp = left; xp <= right; ++xp, ++srcP) {
			if (!colorUsed[*srcP]) {
				// Flag the color as being used, and increment the total colors count
				colorUsed[*srcP] = true;
				++numColors;

				// If an array was passed, store the palette index
				if (colors)
					*colors++ = *srcP;
			}
		}
	}

	return numColors;
}

byte Pane::readDot(const Common::Point &pt) {
	Common::Point p(pt.x - left, pt.y - top);
	return readDot(p);
}

void Pane::drawDot(const Common::Point &pt, byte color) {
	Common::Point p(pt.x - left, pt.y - top);

	_window->drawDot(p, color);
}

void Pane::drawLine(const Common::Point &src, const Common::Point &dest, byte color) {
	_window->drawLine(Common::Point(src.x - left, src.y - top),
	                  Common::Point(dest.x - left, dest.y - top), color);
}

void Pane::drawRect(const Common::Rect &r, byte color) {
	Common::Rect bounds = r;
	bounds.translate(-left, -top);
	_window->drawRect(bounds, color);
}

void Pane::fillRect(const Common::Rect &r, byte color) {
	Common::Rect bounds = r;
	bounds.translate(-left, -top);
	_window->fillRect(bounds, color);
}

void Pane::hashRect(const Common::Rect &r, byte color) {
	Common::Rect bounds = r;
	bounds.translate(-left, -top);
	_window->hashRect(bounds, color);
}

bool Pane::testOverlap(const Common::Point &pt, byte *shapes, int shape_num) {
	int x2 = (pt.x + (VFX_shape_resolution(shapes, shape_num) >> 16));
	int y2 = (pt.y + (VFX_shape_resolution(shapes, shape_num)));

	return ((pt.x <= right) && (x2 >= left) && (pt.y <= bottom) && (y2 >= top));
}

void Pane::scroll(const Common::Point &delta, PaneScroll flags, byte background) {
	error("TODO");
}

Graphics::Surface Pane::getArea(const Common::Rect &r) {
	Common::Rect bounds = r;
	bounds.translate(left, top);
	return _window->getArea(bounds);
}

byte *Pane::getBasePtr(int x, int y) {
	return _window->getBasePtr(x, y);
}

void Pane::blitFrom(Pane &src, const Common::Point &destPos, const Common::Rect &srcRect) {
	// Add a dirty rect for the area we'll be copying over
	Common::Rect destRect(destPos.x, destPos.y, destPos.x + (srcRect.right - srcRect.left),
		destPos.y + (srcRect.bottom - srcRect.top));
	_window->addDirtyRect(destRect);

	// Main blitting loop
	for (int yp = 0; yp < srcRect.bottom - srcRect.top + 1; ++yp) {
		const byte *srcP = src._window->getBasePtr(srcRect.left, srcRect.top + yp);
		byte *destP = _window->getBasePtr(destPos.x, destPos.y + yp);
		Common::copy(srcP, srcP + (srcRect.right - srcRect.left) + 1, destP);
	}
}

void Pane::blitFrom(Pane &src) {
	blitFrom(src, Common::Point(left, top), src);
}

/*----------------------------------------------------------------*/

Window::Window(): Pane() {
}

Window::~Window() {
	_surface.free();
}

Window::Window(const Common::Point &size) : Pane(this) {
	// Set up window size
	left = top = 0;
	right = size.x - 1;
	bottom = size.y - 1;

	// Create the surface
	_surface.create(size.x, size.y, Graphics::PixelFormat::createFormatCLUT8());
}

void Window::wipeWindow(byte color) {
	paneWipe(color);
}

byte Window::readDot(const Common::Point &pt) {
	const byte *destP = (const byte *)_surface.getBasePtr(pt.x, pt.y);
	return *destP;
}

void Window::drawDot(const Common::Point &pt, byte color) {
	byte *destP = (byte *)_surface.getBasePtr(pt.x, pt.y);
	*destP = color;

	addDirtyRect(Common::Rect(pt.x, pt.y, pt.x, pt.y));
}

void Window::drawLine(const Common::Point &src, const Common::Point &dest, byte color) {
	if (src.y == dest.y) {
		_surface.hLine(MIN(src.x, dest.x), src.y, MAX(src.x, dest.x), color);
	} else if (src.x == dest.x) {
		_surface.vLine(src.x, MIN(src.y, dest.y), MAX(src.y, dest.y), color);
	} else {
		error("Diagonal drawLine not yet supported");
	}

	addDirtyRect(Common::Rect(MIN(src.x, dest.x), MIN(src.x, src.y),
		MAX(src.x, dest.x), MAX(src.y, dest.y)));
}

void Window::drawRect(const Common::Rect &r, byte color) {
	_surface.hLine(r.left, r.top, r.right, color);
	_surface.hLine(r.left, r.bottom, r.right, color);
	_surface.vLine(r.left, r.top + 1, r.bottom, color);
	_surface.vLine(r.left, r.top + 1, r.bottom, color);

	addDirtyRect(r);
}

void Window::fillRect(const Common::Rect &r, byte color) {
	_surface.fillRect(Common::Rect(r.left, r.top, r.right + 1, r.bottom + 1), color);
	addDirtyRect(r);
}

void Window::hashRect(const Common::Rect &r, byte color) {
	error("TODO: hashRect");
}

void Window::refresh(const Common::Rect &r) {
	if (this == _vm->_screen)
		return;

	Common::Rect bounds = r;
	bounds.clip(Common::Rect(0, 0, AESOP_SCREEN_WIDTH - 1, AESOP_SCREEN_HEIGHT - 1));
	Graphics::Surface destSurface = _vm->_screen->getArea(bounds);

	for (int yp = bounds.top; yp <= bounds.bottom; ++yp) {
		const byte *srcP = (const byte *)getBasePtr(bounds.left, bounds.top);
		byte *destP = (byte *)_vm->_screen->getBasePtr(0, yp - bounds.top);
		Common::copy(srcP, srcP + (bounds.right - bounds.left + 1), destP);
	}
}

void Window::fade(const byte *palette, int intervals) {
	Events &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	bool colorUsed[PALETTE_COUNT];
	byte colorList[PALETTE_COUNT];
	byte colorBuffer[PALETTE_SIZE];
	byte fadeDirection[PALETTE_SIZE];
	byte colorDelta[PALETTE_SIZE];
	byte colorError[PALETTE_SIZE];
	int lastColor = -1;

	// Get the pixels in use in the window and their palette rgb values
	Common::fill(&colorUsed[0], &colorUsed[PALETTE_COUNT], false);
	Common::fill(&colorBuffer[0], &colorBuffer[PALETTE_SIZE], 0);

	for (int yp = 0; yp < _surface.h; ++yp) {
		const byte *srcP = (const byte *)_surface.getBasePtr(0, yp);
		for (int xp = 0; xp < _surface.w; ++xp, ++srcP) {
			if (!colorUsed[*srcP]) {
				colorUsed[*srcP] = true;

				++lastColor;
				colorList[lastColor] = *srcP;
				screen.readPalette(*srcP, &colorBuffer[*srcP * 3]);
			}
		}
	}

	// Compute the needed deltas
	int maxValue = 0, deltaVal = 0;
	for (int idx = lastColor; idx >= 0; --idx) {
		for (int rgbIndex = 0; rgbIndex < 3; ++rgbIndex) {
			byte col = colorList[idx];
			byte const *srcP = &colorBuffer[col * 3 + rgbIndex];
			byte const *destP = &palette[col * 3 + rgbIndex];

			fadeDirection[col * 3 + rgbIndex] = *srcP >= *destP ? (byte)-4 : 4;
			colorDelta[col * 3 + rgbIndex] = deltaVal = ABS((int)*destP - (int)*srcP) >> 2;
			maxValue = MAX(maxValue, deltaVal);
		}
	}

	if (maxValue == 0)
		return;

	// Do the actual fading
	Common::fill(&colorError[0], &colorError[lastColor], maxValue / 2);
	Common::fill(&colorError[lastColor * 3 + 3], &colorError[PALETTE_SIZE], 0);
	//int step = (intervals << 16) / maxValue;
	//int stepCount = 0x8000;

	for (int stepNum = 0; stepNum < maxValue && !_vm->shouldQuit(); ++stepNum) {
		// Figure out the intermediate palette
		for (int idx = lastColor; idx >= 0; --idx) {
			int offset = colorList[idx] * 3;

			for (int rgbNum = 0; rgbNum < 3; ++rgbNum) {
				byte rgbVal = colorError[offset + rgbNum] + colorDelta[offset + rgbNum];
				if (rgbVal >= maxValue) {
					// Move component to next rgb index
					rgbVal -= maxValue;
					colorBuffer[offset + rgbNum] += fadeDirection[offset + rgbNum];
				}

				colorError[offset + rgbNum] = rgbVal;
			}

			// Write the new palette entry
			screen.writePalette(colorList[idx], &colorBuffer[offset]);
		}

		events.delay(2);
	}
}

Graphics::Surface Window::getArea(const Common::Rect &r) {
	// Mark the desired area as dirty
	addDirtyRect(r);

	// Form a surface of the sub-area of the window
	Graphics::Surface s;
	s.setPixels(_surface.getBasePtr(r.left, r.top));
	s.format = _surface.format;
	s.pitch = _surface.pitch;
	s.w = r.width() + 1;
	s.h = r.height() + 1;

	return s;
}

byte *Window::getBasePtr(int x, int y) {
	return (byte *)_surface.getBasePtr(x, y);
}

} // End of namespace Aesop

