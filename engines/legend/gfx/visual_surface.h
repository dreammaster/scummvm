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

#ifndef LEGEND_VISUAL_SURFACE_H
#define LEGEND_VISUAL_SURFACE_H

#include "graphics/managed_surface.h"
#include "legend/core/named_item.h"
#include "legend/gfx/region.h"
#include "legend/gfx/screen.h"

namespace Legend {
namespace Gfx {

/**
 * Font related details
 */
struct FontDetails {
	int _fontNumber;
	Common::Point _fontCenter;
	int _horizSpacings;
	bool _allowsPendingText;
	Common::String _pendingText;
	Common::Point _writePos;
	int _fgColor, _bgColor;

	FontDetails() : _fontNumber(1), _horizSpacings(-1), _allowsPendingText(false),
		_fgColor(0), _bgColor(0) {}
	void reset();
};

class VisualSurface : public Graphics::ManagedSurface {
private:
	FontDetails &_font;
	const Common::Rect &_bounds;

	/**
	 * Moves font output to a new line, prompting for waiting if necessary
	 */
	bool newLine();
public:
	VisualSurface(const Graphics::ManagedSurface &src,
		FontDetails &fontDetails, const Common::Rect &bounds);

	/**
	 * Loads a font to use for the item
	 */
	Font *loadFont(int fontNumber);

	/**
	 * Gets the surface's current font
	 */
	Font *getFont();

	/**
	 * Set the position for writing text in screen-coordinates
	 * @remarks		All text is clipped to the bounds of the visual item
	 */
	void setTextPos(const Common::Point &pt) {
		_font._writePos = Common::Point(pt.x - _bounds.left, pt.y - _bounds.top);
	}

	/**
	 * Sets the postion or writing text relative to the top-left corner of the visual item's bounds
	 */
	void setRelativeTextPos(const Common::Point &pt) { _font._writePos = pt; }

	/**
	 * Set the font color
	 */
	void setFontColor(int fgColor, int bgColor = -1);

	/**
	 * Writes a string
	 * @param text	Text to display
	 * @returns		Number of characters written
	 */
	int writeString(const String &text);

	/**
	 * Writes a string
	 */
	void writeString(const Common::Point &pt, const String &msg);

	/**
	 * Get the width of a string
	 */
	int stringWidth(const String &msg);

	/**
	 * Return the font center
	 */
	Common::Point getFontCenter() const { return _font._fontCenter; }

	/**
	 * Fill the surface with a given color
	 */
	void fill(byte color) { Graphics::ManagedSurface::clear(color); }

	/**
	 * Custom blitFrom version that takes in a Region. Since regions are defined
	 * in terms of overall screen co-ordinates, and the visual surface's top left
	 * is (0,0), converts from screen to local coordintaes.
	 */
	void blitFrom(const Graphics::Surface &src, const Region &r);

	void blitFrom(const Graphics::Surface &src) {
		Graphics::ManagedSurface::blitFrom(src);
	 }
	 void blitFrom(const Graphics::Surface &src, const Common::Point &destPos) {
		 Graphics::ManagedSurface::blitFrom(src, destPos);
	 }
	 void blitFrom(const Graphics::Surface &src, const Common::Rect &srcRect,
			const Common::Point &destPos) {
		Graphics::ManagedSurface::blitFrom(src, srcRect, destPos);
	}
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
