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

#ifndef AESOP_SHAPES_H
#define AESOP_SHAPES_H

#include "common/array.h"
#include "common/rect.h"
#include "aesop/screen.h"

namespace Aesop {

enum Mirror { NO_MIRROR = 0, X_MIRROR = 1, Y_MIRROR = 2, XY_MIRROR = 3 };

struct ShapeFrame: public Common::Rect {
private:
	const byte *_data;
public:
	ShapeFrame();
	ShapeFrame(const byte *dataPtr);

	/**
	 * Draw the shape onto a pane
	 */
	void draw(Pane &pane, const Common::Point &pt);

	/**
	 * Draw the shape onto a pane with resizing
	 */
	void transform(Pane &pane, const Common::Point &pt, const byte *pixels, int rot, 
		int xScale, int yScale, uint flags);

	/**
	 * Returns the bounds of the shape encoded into a 32-bit value
	 */
	uint32 getBounds() const;

	int visibleBitmapRect(int x1, int y1, int mirror, uint16 *bounds);

	/**
	 * This function traces a shape and returns a smallest rectangle which
	 * will hold the visible portion of the shape.
	 *
	 * @param hostpot	Specifies the location where the shape is to be drawn.
	 * The shape's hot spot will end up at the specified location.
	 * @param mirror	Holds flags to mirror the shape in X, Y or both.
	 * @param rectangle	Must point to an array of at least 4 ints which
	 * will receive the rectangle coordinates.
	 */
	void visibleRectangle(int hotX, int hotY, int mirror, int *rectangle);
};

class Shapes: public Common::Array<ShapeFrame> {
public:
	Shapes(const byte *data);
	Shapes() {}

	/**
	 * Draw a bitmap onto the specified window.
	 *
	 * @param wnd			The window to draw to
	 * @param pt			Draw position
	 * @param mirror		Whether to flip the image
	 * @param scale			Scaling factor
	 * @param fade_table	Fade table
	 * @param shape_num		Shape number to draw
	 * @returns	 0: OK
	 *			-1: Bad window
	 *			-2: Bad pane
	 *			-3: Shape off pane
	 *			-4: Null shape
	 */
	void drawBitmap(int wnd, const Common::Point &pt, int mirror, int scale,
		byte *fade_table, int shape_num);

	/**
	 * Fill a 4-element array of words with the coordinates of the rectangle
	 * enclosing the visible part of a bitmap
	 *
	 * Return 0 if bitmap empty
	 * Determines array offset in instance, in case resource load causes
	 * instance to move in memory
	 *
	 * WARNING: The array must not be of automatic or external scope!
	 */
	static uint visibleBitmapRect(int x, int y, uint flip, uint table, uint number, uint16 *array);

	static uint getBitmapHeight(uint table, uint number);
};

} // End of namespace Aesop

#endif
