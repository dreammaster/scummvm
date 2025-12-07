/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advrect.h"
#include "spycraft/dmade/advport.h"
#include "spycraft/dmade/advsprite.h"
#include "spycraft/dmade/advlist.h"

namespace Spycraft {

#define SCC_OUTCODE_NONE	0x0000
#define SCC_OUTCODE_LEFT	0x0001
#define SCC_OUTCODE_RIGHT	0x0002
#define SCC_OUTCODE_BOTTOM	0x0004
#define SCC_OUTCODE_TOP		0x0008

inline int Point_Contains(SRect *rect, int x, int y) {
	return ((x >= rect->left) && (x <= rect->right) &&
		(y >= rect->top) && (y <= rect->bottom));
}

inline int CompOutCode(int x, int y, SRect *r) {
	int ret = SCC_OUTCODE_NONE;

	if (y > r->bottom)
		ret = SCC_OUTCODE_TOP;
	else if (y < r->top)
		ret = SCC_OUTCODE_BOTTOM;

	if (x > r->right)
		ret |= SCC_OUTCODE_RIGHT;
	else if (x < r->left)
		ret |= SCC_OUTCODE_LEFT;

	return (ret);
}

int SCClipping(int *x1, int *y1, int *x2, int *y2, SRect *r) {
	int x = 0, y = 0;
	int outCodeOut;
	int outCode0 = CompOutCode(*x1, *y1, r);
	int outCode1 = CompOutCode(*x2, *y2, r);

	do {
		if ((outCode0 | outCode1) == 0x0000)
			return (true);

		if (outCode0 & outCode1)
			return (false);

		if (outCode0)
			outCodeOut = outCode0;
		else
			outCodeOut = outCode1;

		if (outCodeOut & SCC_OUTCODE_TOP) {
			x = *x1 + ((*x2 - *x1) * (r->bottom - *y1)) / (*y2 - *y1);
			y = r->bottom;
		} else if (outCodeOut & SCC_OUTCODE_BOTTOM) {
			x = *x1 + ((*x2 - *x1) * (r->top - *y1)) / (*y2 - *y1);
			y = r->top;
		} else if (outCodeOut & SCC_OUTCODE_RIGHT) {
			y = *y1 + ((*y2 - *y1) * (r->right - *x1)) / (*x2 - *x1);
			x = r->right;
		} else if (outCodeOut & SCC_OUTCODE_LEFT) {
			y = *y1 + ((*y2 - *y1) * (r->left - *x1)) / (*x2 - *x1);
			x = r->left;
		}
		if (outCodeOut == outCode0) {
			*x1 = x;
			*y1 = y;
			outCode0 = CompOutCode(*x1, *y1, r);
		} else {
			*x2 = x;
			*y2 = y;
			outCode1 = CompOutCode(*x2, *y2, r);
		}
	} while (true);
}

static void Line16(int x1, int y1, int x2, int y2, SRect *rect, int color, Viewport *port) {
	int x, y, m;
	int p, c1, c2, dx, dy, t;
	int x_end, y_end;
	int ddx = x2 - x1;
	int ddy = y2 - y1;
	int width = port->width;

	t = ((ddx < 0) && !(ddx < 0 && ddy < 0)) ||
		((ddy < 0) && !(ddx < 0 && ddy < 0));

	dx = abs(ddx);
	dy = abs(ddy);

	if (dx == 0)
		m = 1;
	else
		m = dy / dx;

	/* CHECK THE SLOPE */
	if (m >= 1) {
		p = 2 * dx - dy;
		c1 = 2 * dx;
		c2 = 2 * (dx - dy);
		if (y1 > y2) {
			x = x2;
			y = y2;
			y_end = y1;
		} else {
			x = x1;
			y = y1;
			y_end = y2;
		}
		if (Point_Contains(rect, x, y))
			((unsigned short *)port->ptr)[y * width + x] = color;

		while (y < y_end) {
			y = y + 1;

			if (p < 0) {
				p = p + c1;
			} else {
				if (t)
					x = x - 1;
				else
					x = x + 1;
				p = p + c2;
			}
			if (Point_Contains(rect, x, y))
				((unsigned short *)port->ptr)[y * width + x] = color;
		}
	} else {
		p = 2 * dy - dx;
		c1 = 2 * dy;
		c2 = 2 * (dy - dx);
		if (x1 > x2) {
			x = x2;
			y = y2;
			x_end = x1;
		} else {
			x = x1;
			y = y1;
			x_end = x2;
		}
		if (Point_Contains(rect, x, y))
			((unsigned short *)port->ptr)[y * width + x] = color;

		while (x < x_end) {
			x = x + 1;

			if (p < 0) {
				p = p + c1;
			} else {
				if (t)
					y = y - 1;
				else
					y = y + 1;
				p = p + c2;
			}
			if (Point_Contains(rect, x, y))
				((unsigned short *)port->ptr)[y * width + x] = color;
		}
	}
}

static void Line8(int x1, int y1, int x2, int y2, SRect *rect, int color, Viewport *port) {
	int x, y, m;
	int p, c1, c2, dx, dy, t;
	int x_end, y_end;
	int ddx = x2 - x1;
	int ddy = y2 - y1;
	int width = port->width;

	t = ((ddx < 0) && !(ddx < 0 && ddy < 0)) ||
		((ddy < 0) && !(ddx < 0 && ddy < 0));

	dx = abs(ddx);
	dy = abs(ddy);

	if (dx == 0)
		m = 1;
	else
		m = dy / dx;

	/* CHECK THE SLOPE */
	if (m >= 1) {
		p = 2 * dx - dy;
		c1 = 2 * dx;
		c2 = 2 * (dx - dy);
		if (y1 > y2) {
			x = x2;
			y = y2;
			y_end = y1;
		} else {
			x = x1;
			y = y1;
			y_end = y2;
		}
		if (Point_Contains(rect, x, y))
			((unsigned char *)port->ptr)[y * width + x] = color;

		while (y < y_end) {
			y = y + 1;

			if (p < 0) {
				p = p + c1;
			} else {
				if (t)
					x = x - 1;
				else
					x = x + 1;
				p = p + c2;
			}
			if (Point_Contains(rect, x, y))
				((unsigned char *)port->ptr)[y * width + x] = color;
		}
	} else {
		p = 2 * dy - dx;
		c1 = 2 * dy;
		c2 = 2 * (dy - dx);
		if (x1 > x2) {
			x = x2;
			y = y2;
			x_end = x1;
		} else {
			x = x1;
			y = y1;
			x_end = x2;
		}
		if (Point_Contains(rect, x, y))
			((unsigned char *)port->ptr)[y * width + x] = color;

		while (x < x_end) {
			x = x + 1;

			if (p < 0) {
				p = p + c1;
			} else {
				if (t)
					y = y - 1;
				else
					y = y + 1;
				p = p + c2;
			}
			if (Point_Contains(rect, x, y))
				((unsigned char *)port->ptr)[y * width + x] = color;
		}
	}
}

void DrawLines(Viewport *dest, Sprite *sprite, SRect *spriteRect) {
	int i;
	int x1, y1, x2, y2;
	DrawPrime *dp;
	ArrayList *list = sprite->drawList;

	if (dest->colors == 8) {
		for (i = 0; i < list->size; i++) {
			dp = (DrawPrime *)list->elements[i];

			/* FOR CLIPPING */
			x1 = dp->sx;
			y1 = dp->sy;
			x2 = dp->ex;
			y2 = dp->ey;
			if (SCClipping(&x1, &y1, &x2, &y2, spriteRect))
				Line8(dp->sx, dp->sy, dp->ex, dp->ey, spriteRect, dp->color, dest);
		}
	} else {
		for (i = 0; i < list->size; i++) {
			dp = (DrawPrime *)list->elements[i];

			/* FOR CLIPPING */
			x1 = dp->sx;
			y1 = dp->sy;
			x2 = dp->ex;
			y2 = dp->ey;
			if (SCClipping(&x1, &y1, &x2, &y2, spriteRect))
				Line16(dp->sx, dp->sy, dp->ex, dp->ey, spriteRect, dp->color, dest);
		}
	}
}

} // namespace Spycraft
