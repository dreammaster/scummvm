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

#include "spycraft/adv/advrect.h"

namespace Spycraft {

int SRect_Intersects(const SRect *rect1, const SRect *rect2) {
	return (!(
		(rect1->left > rect2->right) ||
		(rect1->right < rect2->left) ||
		(rect1->top > rect2->bottom) ||
		(rect1->bottom < rect2->top)
		));
}

int SRect_Contains(const SRect *rect1, const SRect *rect2) {
	return (
		(rect1->left <= rect2->left) &&
		(rect1->right >= rect2->right) &&
		(rect1->top <= rect2->top) &&
		(rect1->bottom >= rect2->bottom)
		);
}

void SRect_Copy(SRect *dest, const SRect *src) {
	dest->left = src->left;
	dest->top = src->top;
	dest->right = src->right;
	dest->bottom = src->bottom;
	dest->width = src->width;
	dest->height = src->height;
}

void SRect_Union(const SRect *rect1, const SRect *rect2, SRect *r) {
	r->left = rect1->left < rect2->left ? rect1->left : rect2->left;
	r->right = rect1->right > rect2->right ? rect1->right : rect2->right;
	r->top = rect1->top < rect2->top ? rect1->top : rect2->top;
	r->bottom = rect1->bottom > rect2->bottom ? rect1->bottom : rect2->bottom;
	r->width = r->right - r->left + 1;
	r->height = r->bottom - r->top + 1;
}

void SRect_Init(SRect *rect, int l, int t, int r, int b) {
	rect->left = l;
	rect->top = t;
	rect->right = r;
	rect->bottom = b;
	rect->width = r - l + 1;
	rect->height = b - t + 1;
}

void SRect_GetIntersection(const SRect *r1, const SRect *r2, SRect *out) {
	if (r1->left < r2->left)
		out->left = r2->left;
	else
		out->left = r1->left;

	if (r1->right > r2->right)
		out->right = r2->right;
	else
		out->right = r1->right;

	if (r1->top < r2->top)
		out->top = r2->top;
	else
		out->top = r1->top;

	if (r1->bottom > r2->bottom)
		out->bottom = r2->bottom;
	else
		out->bottom = r1->bottom;

	out->width = out->right - out->left + 1;
	out->height = out->bottom - out->top + 1;
}

} // namespace Spycraft
