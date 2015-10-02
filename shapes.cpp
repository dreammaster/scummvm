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
	if (!clippedBounds.isValidRect() || clippedBounds.isEmpty())
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

		xp = drawBounds.left;
		for (;;) {
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

		//assert(xp == 0 || xp == (drawBounds.right + 1));
	}
}

void ShapeFrame::transform(Pane &pane, const Common::Point &pt, const byte *pixels, int rot, 
		int xScale, int yScale, uint flags) {
	warning("TODO: transform");
}

uint32 ShapeFrame::getBounds() const {
	// Height is low 16-bits, Width is high 16 bits
	return READ_LE_UINT32(_data);
}

int ShapeFrame::visibleBitmapRect(int x1, int y1, int mirror, uint16 *bounds) {
	int rectangle[4];
	int bm_width, bm_height;

	visibleRectangle(x1, y1, mirror, rectangle);

	if (mirror) {
		bm_width = getBounds();
		bm_height = bm_width & 0xffff;
		bm_width = bm_width >> 16;
	}


	if (mirror & X_MIRROR) {
		bounds[0] = (WORD)(bm_width + rectangle[0]);
		bounds[2] = (WORD)(bm_width + rectangle[2]);
	} else {
		bounds[0] = (WORD)rectangle[0];
		bounds[2] = (WORD)rectangle[2];
	}

	if (mirror & Y_MIRROR) {
		bounds[1] = (WORD)(bm_height + rectangle[1]);
		bounds[3] = (WORD)(bm_height + rectangle[3]);
	} else {
		bounds[1] = (WORD)rectangle[1];
		bounds[3] = (WORD)rectangle[3];
	}

	Pane &pane0 = _vm->_screen->panes(0);
	bounds[0] = (bounds[0] > pane0.left) ? bounds[0] : (WORD)pane0.left;
	bounds[1] = (bounds[1] > pane0.top) ? bounds[1] : (WORD)pane0.top;

	bounds[2] = (bounds[2] < pane0.right) ? bounds[2] : (WORD)pane0.right;
	bounds[3] = (bounds[3] < pane0.bottom) ? bounds[3] : (WORD)pane0.bottom;

	return ((bounds[0] > bounds[2]) || (bounds[1] > bounds[3])) ? 0 : 1;
}

void ShapeFrame::visibleRectangle(int hotX, int hotY, int mirror, int *rectangle) {
	Common::Rect vpt;
	vpt.left = vpt.top = 0x7FFF;
	vpt.right = vpt.bottom = -0x7FFF;

	// Main loop to decode data and figure out the shape size
	const byte *srcP = _data + 24;
	byte cmd;
	bool bit0;
	int16 xp, yp;

	for (yp = hotY + top; yp <= hotY + bottom; ++yp) {
		xp = hotX + left;
		for (;;) {
			cmd = *srcP++;
			bit0 = cmd & 1;
			cmd >>= 1;

			if (!bit0 && cmd != 0) {
				// Run length packet
				srcP++;
				vpt.left = MIN(vpt.left, xp);
				vpt.right = MAX((int)vpt.right, xp + cmd);
				vpt.top = MIN(vpt.top, yp);
				vpt.bottom = MAX(vpt.bottom, yp);
				xp += cmd;
			} else if (cmd != 0) {
				// String Packet
				srcP += cmd;
				vpt.left = MIN(vpt.left, xp);
				vpt.right = MAX((int)vpt.right, xp + cmd);
				vpt.top = MIN(vpt.top, yp);
				vpt.bottom = MAX(vpt.bottom, yp);
				xp += cmd;
			} else if (!bit0) {
				// End Packet - end of line reached
				break;
			} else {
				// Skip Packet
				cmd = *srcP++;
				xp += cmd;
			}
		}
	}

	// Adjust for mirroring
	if (mirror & X_MIRROR) {
		int x0 = hotX * 2 - vpt.right;
		int x1 = hotX * 2 - vpt.left;
		vpt.left = x0;
		vpt.right = x1;
	}

	if (mirror & Y_MIRROR) {
		int y0 = hotY * 2 - vpt.bottom;
		int y1 = hotY * 2 - vpt.top;
		vpt.top = y0;
		vpt.bottom = y1;
	}

	// Save the calculated coordinates
	assert(vpt.isValidRect());
	*rectangle++ = vpt.left;
	*rectangle++ = vpt.top;
	*rectangle++ = vpt.right;
	*rectangle++ = vpt.bottom;
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
			VFX_fixed_mul(frame.getBounds() & 0xffff0000, 0x10000 - x_scale, &xs);
			VFX_fixed_mul(frame.getBounds() << 16, 0x10000 - y_scale, &ys);

			if (mirror & X_MIRROR) xs = -xs;
			if (mirror & Y_MIRROR) ys = -ys;

			xp += xs >> 17;
			yp += ys >> 17;
		}

		switch (mirror) {
		case X_MIRROR:
			x_scale = -x_scale;
			xp += (frame.getBounds() >> 16);
			break;
		case Y_MIRROR:
			y_scale = -y_scale;
			yp += (WORD)frame.getBounds();
			break;
		case XY_MIRROR:
			x_scale = -x_scale;
			y_scale = -y_scale;
			xp += (frame.getBounds() >> 16) - 1;
			yp += ((WORD)frame.getBounds()) - 1;
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

uint Shapes::visibleBitmapRect(int x, int y, uint flip, uint table, uint number, uint16 *array) {
	Objects &objects = *_vm->_objects;
	Resources &res = *_vm->_resources;
	HRES handle;
	uint array_offset;
	byte *new_array;
	uint16 bounds[4];
	uint result;

	array_offset = (ULONG)array - (ULONG)res.addr(objects[Interpreter::_currentThis]);

	handle = res.get_resource_handle(table, DA_DEFAULT);

	res.lock(handle);

	Shapes shapes((const byte *)res.addr(handle));
	result = shapes[number].visibleBitmapRect(x, y, flip, bounds);

	new_array = (byte *)add_ptr(res.addr(objects[Interpreter::_currentThis]), array_offset);

	far_memmove(new_array, bounds, sizeof(bounds));

	res.unlock(handle);

	return result;
}

uint Shapes::getBitmapHeight(uint table, uint number) {
	Resources &res = *_vm->_resources;
	HRES handle;
	uint h;

	handle = res.get_resource_handle(table, DA_DEFAULT);

	res.lock(handle);

	Shapes shapes((const byte *)res.addr(handle));
	h = shapes[number].height();

	res.unlock(handle);

	return h;
}

} // End of namespace Aesop

