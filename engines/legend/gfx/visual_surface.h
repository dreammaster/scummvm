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

enum FrameRectFlag {
	FF_UNUSED = 1, FF_BLACK_FILL = 2, FF_GRAY_CONTENT = 4, FF_WHITE_CONTENT = 8
};

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
	Common::Rect _bounds;

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
	 * Set the position for writing text
	 * @remarks		All text is clipped to the bounds of the visual item
	 */
	void setTextPos(const Common::Point &pt) { _font._writePos = pt; }

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

	/**
	 * Gets a new visual surface representing sub-area of the parent surface.
	 * This is useful when drawing needs to be clipped to an area of a surface
	 * @param r		Rect specifying sub-area of surface to return
	 */
	VisualSurface getSubArea(const Common::Rect &r);

	/**
	 * Fill a rect with a given color.
	 * @param r			Drawing area
	 * @param color		Color to use. If bit 7 is set, it uses a checkerbox
	 *		of alternating grey with the color in the remaining bits
	 */
	void fillRect(Common::Rect r, byte color);

	/**
	 * Fill the entire surface with a given color.
	 * @param color		Color to use. If bit 7 is set, it uses a checkerbox
	 *		of alternating color 10 with the color in the remaining bits
	 */
	void fill(byte color);

	/**
	 * Fills an area with a frame
	 */
	void frameRect(const Common::Rect &r, byte flags);

	/**
	 * Fill the entire surface with bevelled frame
	 */
	void frame(byte flags);
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
