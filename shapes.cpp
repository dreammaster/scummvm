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

#include "common/scummsys.h"
#include "aesop/aesop.h"
#include "aesop/shapes.h"

namespace Aesop {

void VFX_fixed_mul(FIXED16 M1, FIXED16 M2, FIXED16 *result) {
	error("TODO");
}

void VFX_shape_lookaside(UBYTE *table) {
	error("TODO");
}

/*----------------------------------------------------------------*/

ShapeFrame::ShapeFrame() : Common::Rect(), _data(nullptr) {
}

ShapeFrame::ShapeFrame(const byte *dataPtr): Common::Rect(), _data(dataPtr) {
	left = (int16)READ_LE_UINT16(_data + 8);
	top = (int16)READ_LE_UINT16(_data + 12);
	right = (int16)READ_LE_UINT16(_data + 16);
	bottom = (int16)READ_LE_UINT16(_data + 20);
}

void ShapeFrame::draw(Pane &pane, const Common::Point &pt) {
	// Get the drawing bounds, and adjust them by the drawing position
	Common::Rect drawBounds = *this;
	drawBounds.translate(pt.x, pt.y);

	// Clip the drawing bounds to the given pane
	Common::Rect clippedBounds = drawBounds;
	clippedBounds.clip(pane);
	if (!clippedBounds.isValidRect())
		return;

	// Get the area of the pane that we'll be drawing to
	Graphics::Surface surface = pane.getArea(clippedBounds);

	// Translate both rects back relative to (0,0), since the drwaing area we've retrieved
	// has it's top left at the starting draw position
	drawBounds.translate(-clippedBounds.left, -clippedBounds.top);
	clippedBounds.moveTo(0, 0);

	// Main drawing loop
	const byte *srcP = _data + 24;
	byte cmd, val;
	bool bit0;
	int xp, yp;

	for (yp = drawBounds.top; yp <= drawBounds.bottom; ++yp) {
		byte *destP = (byte *)surface.getBasePtr(drawBounds.left, yp);

		for (xp = drawBounds.left; xp <= drawBounds.right; ) {
			cmd = *srcP++;
			bit0 = cmd & 1;
			cmd >>= 1;

			if (!bit0 && cmd != 0) {
				// Run length packet
				val = *srcP++;
				while (cmd-- > 0) {
					if (clippedBounds.contains(xp, yp))
						*destP = val;
					++xp;
					++destP;
				}
			} else if (cmd != 0) {
				// String Packet - move len bytes from source
				while (cmd-- > 0) {
					val = *srcP++;
					if (clippedBounds.contains(xp, yp))
						*destP = val;
					++xp;
					++destP;
				}
			} else if (!bit0) {
				// End Packet - end of line reached
				break;
			} else {
				// Skip Packet - skip over bytes in dest surface
				val = *srcP++;
				while (val-- > 0) {
					++xp;
					++destP;
				}
			}
		}

		assert(xp <= (drawBounds.right + 1));
	}
}

void ShapeFrame::transform(Pane &pane, const Common::Point &pt, const byte *pixels, int rot, 
	int xScale, int yScale, uint flags) {

}

uint32 ShapeFrame::bounds() const {
	return 0;
}

/*----------------------------------------------------------------*/

Shapes::Shapes(const byte *data) {
	int numShapes = READ_LE_UINT32(data + 4);
	assert(numShapes < 1024);

	clear();
	for (int idx = 0; idx < numShapes; ++idx) {
		const byte *shapePtr = data + READ_LE_UINT32(data + (idx + 1) * 8);
		push_back(ShapeFrame(shapePtr));
	}
}

void Shapes::drawBitmap(int wnd, const Common::Point &pt, int mirror, int scale,
	byte *fade_table, int shape_num) {
	Pane &pane = _vm->_screen->panes(wnd);
	int xp = pt.x - pane.left;
	int yp = pt.y - pane.top;
	int x_scale, y_scale;
	int flags;
	int xs, ys;

	ShapeFrame &frame = (*this)[shape_num];

	if ((scale == 0) && (mirror == NO_MIRROR)) {
		frame.draw(pane, Common::Point(xp, yp));
	} else {
		x_scale = ((scale) ? (scale << 8) : 0x10000);
		y_scale = ((scale) ? (scale << 8) : 0x10000);

		if (x_scale != 0x10000 || y_scale != 0x10000) {
			VFX_fixed_mul(frame.bounds() & 0xffff0000, 0x10000 - x_scale, &xs);
			VFX_fixed_mul(frame.bounds() << 16, 0x10000 - y_scale, &ys);

			if (mirror & X_MIRROR) xs = -xs;
			if (mirror & Y_MIRROR) ys = -ys;

			xp += xs >> 17;
			yp += ys >> 17;
		}

		switch (mirror) {
		case X_MIRROR:
			x_scale = -x_scale;
			xp += (frame.bounds() >> 16);
			break;
		case Y_MIRROR:
			y_scale = -y_scale;
			yp += (WORD)frame.bounds();
			break;
		case XY_MIRROR:
			x_scale = -x_scale;
			y_scale = -y_scale;
			xp += (frame.bounds() >> 16) - 1;
			yp += ((WORD)frame.bounds()) - 1;
			break;
		case NO_MIRROR:
		default:
			break;
		}

		if (fade_table != NULL && scale != 0) {
			VFX_shape_lookaside(fade_table);
			flags = ST_XLAT;
		} else {
			flags = 0;
		}

		frame.transform(pane, Common::Point(xp, yp), (const byte *)_vm->_screen->_bitmapBuffer.getPixels(),
			0, x_scale, y_scale, flags);
	}
}

} // End of namespace Aesop

