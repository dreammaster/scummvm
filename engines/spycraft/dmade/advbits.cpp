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

#include "common/textconsole.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advbits.h"
#include "spycraft/dmade/advrect.h"
#include "spycraft/dmade/advport.h"

namespace Spycraft {

extern int scene_width;
extern int pixFormat;

void DrawLines(Viewport *dest, Sprite *sprite, SRect *spriteRect);

void DrawBits(Viewport *dest, Viewport *src, SRect *rect) {
	int i;
	int l, r, b, t, w, h;
	byte *address, *start;
	int skipAddress, skipStart;

	/* CLIPPING RECT */
	if (rect->left < 0)
		l = 0;
	else
		l = rect->left;

	if (rect->right > (dest->width - 1))
		r = dest->width - 1;
	else
		r = rect->right;

	if (rect->top < 0)
		t = 0;
	else
		t = rect->top;

	if (rect->bottom > (dest->height - 1))
		b = dest->height - 1;
	else
		b = rect->bottom;

	/* FIGURE OUT RECT SIZE */
	w = r - l + 1;
	h = b - t + 1;

	/* COPY BITS */
	if (dest->colors == 16) {
		/* express l and w in bytes */
		l = l << 1;
		w = w << 1;
	}
	skipAddress = src->rowBytes;
	skipStart = dest->rowBytes;
	address = (byte *)src->ptr + t * skipAddress + l;
	start = (byte *)dest->ptr + t * skipStart + l;
	for (i = 0; i < h; i++) {
		memcpy(start, address, w);
		address += skipAddress;
		start += skipStart;
	}
}

void MakeScaleTableI(uint *table, int oldLen, int newLen) {
	int i;
	int limit;
	int ratio, iterator;
#define FIXED_FACTOR      16

	iterator = 0;
	ratio = (oldLen << FIXED_FACTOR) / newLen;
	limit = (oldLen > newLen) ? oldLen : newLen;

	for (i = 0; i < limit; i++) {
		table[i] = iterator >> FIXED_FACTOR;
		iterator += ratio;
	}
}

void DrawScaleSpriteMask(Viewport *dest, Sprite *sprite, SRect *spriteRect) {
	int i, j, skipAddress, skipStart, *colorIndex;
	byte *address, *startAddress, *start;
	int l, r, t, b, w, h, lp, tp;
	int skipColor = sprite->skipColor;

	/* CLIPPING SPRITE RECT */
	if (spriteRect) {
		l = spriteRect->left;
		t = spriteRect->top;
		r = spriteRect->right;
		b = spriteRect->bottom;
	} else {
		l = sprite->rect.left;
		t = sprite->rect.top;
		r = sprite->rect.right;
		b = sprite->rect.bottom;
	}

	/* CLIPPING OF RECTS */
	if (l < 0)
		l = 0;

	if (r > (dest->width - 1))
		r = dest->width - 1;

	if (t < 0)
		t = 0;

	if (b > (dest->height - 1))
		b = dest->height - 1;

	/* GET THE OVERALL DIMENSION */
	w = r - l + 1;
	h = b - t + 1;

	/* MAKE SCALE TABLE */
	if (sprite->state & SPRITE_SCALE_TABLE) {
		sprite->state &= ~SPRITE_SCALE_TABLE;
		MakeScaleTableI(sprite->scaleTableX, sprite->port->width, sprite->rect.width);
		MakeScaleTableI(sprite->scaleTableY, sprite->port->height, sprite->rect.height);
	}

	lp = abs(sprite->rect.left - l);
	tp = abs(sprite->rect.top - t);

	skipAddress = sprite->port->rowBytes;
	skipStart = dest->rowBytes;

	if (sprite->colorIndex) {
		colorIndex = sprite->colorIndex;
		if (dest->colors == 8) {
			byte pix;
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr + t * skipStart + l;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					t = (sprite->scaleTableX)[lp + j];
					pix = colorIndex[((byte *)address)[t]];
					if (pix != skipColor)
						((byte *)start)[j] = pix;
				}
				start += skipStart;
			}
		} else {
			uint16 pix;
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr + t * skipStart + (l << 1);
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					t = sprite->scaleTableX[lp + j];
					pix = colorIndex[((byte *)address)[t]];
					if (pix != skipColor)
						((uint16 *)start)[j] = pix;
				}
				start += skipStart;
			}
		}
	} else {
		if (dest->colors == 8) {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr + t * skipStart + l;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					t = (sprite->scaleTableX)[lp + j];
					if (((byte *)address)[t] != skipColor)
						((byte *)start)[j] = ((byte *)address)[t];
				}
				start += skipStart;
			}
		} else {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr + t * skipStart + (l << 1);
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					t = sprite->scaleTableX[lp + j];
					if (((uint16 *)address)[t] != skipColor)
						((uint16 *)start)[j] = ((uint16 *)address)[t];
				}
				start += skipStart;
			}
		}
	}

	/* DRAW LINES... */
	if (sprite->drawList)
		DrawLines(dest, sprite, spriteRect);
}

void DrawScaleSprite(Viewport *dest, Sprite *sprite, SRect *spriteRect) {
	int i, j, skipAddress, skipStart, *colorIndex;
	byte *address, *startAddress, *start;
	int l, r, t, b, w, h, lp, tp;

	/* CLIPPING SPRITE RECT */
	if (spriteRect) {
		l = spriteRect->left;
		t = spriteRect->top;
		r = spriteRect->right;
		b = spriteRect->bottom;
	} else {
		l = sprite->rect.left;
		t = sprite->rect.top;
		r = sprite->rect.right;
		b = sprite->rect.bottom;
	}

	/* CLIPPING OF RECTS */
	if (l < 0)
		l = 0;

	if (r > (dest->width - 1))
		r = dest->width - 1;

	if (t < 0)
		t = 0;

	if (b > (dest->height - 1))
		b = dest->height - 1;

	/* GET THE OVERALL DIMENSION */
	w = r - l + 1;
	h = b - t + 1;

	/* MAKE SCALE TABLE */
	if (sprite->state & SPRITE_SCALE_TABLE) {
		sprite->state &= ~SPRITE_SCALE_TABLE;
		MakeScaleTableI(sprite->scaleTableX, sprite->port->width, sprite->rect.width);
		MakeScaleTableI(sprite->scaleTableY, sprite->port->height, sprite->rect.height);
	}

	lp = abs(sprite->rect.left - l);
	tp = abs(sprite->rect.top - t);

	skipAddress = sprite->port->rowBytes;
	skipStart = dest->rowBytes;

	if (sprite->colorIndex) {
		colorIndex = sprite->colorIndex;
		if (dest->colors == 8) {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr + t * skipStart + l;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					((byte *)start)[j] = ((byte *)address)[sprite->scaleTableX[lp + j]];
				}
				start += skipStart;
			}
		} else {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr + t * skipStart + (l << 1);
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					((uint16 *)start)[j] = colorIndex[((byte *)address)[sprite->scaleTableX[lp + j]]];
				}
				start += skipStart;
			}
		}
	} else {
		if (dest->colors == 8) {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr + t * skipStart + l;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					((byte *)start)[j] = ((byte *)address)[sprite->scaleTableX[lp + j]];
				}
				start += skipStart;
			}
		} else {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr + t * skipStart + (l << 1);
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					((uint16 *)start)[j] = ((uint16 *)address)[sprite->scaleTableX[lp + j]];
				}
				start += skipStart;
			}
		}
	}

	/* DRAW LINES... */
	if (sprite->drawList)
		DrawLines(dest, sprite, spriteRect);
}

#define GET_R16(C)         	((C>>10) & 0x1f)
#define GET_G16(C)         	((C>>5) & 0x1f)
#define GET_B16(C)         	(C & 0x1f)

#define GET_R16X(C)         	((C>>11) & 0x1f)
#define GET_G16X(C)         	((C>>6) & 0x1f)
#define GET_B16X(C)         	(C & 0x1f)

#define MK_COLOR16X(R,G,B)		( ( R<<11 ) | (G<<6) | B )

void DrawGlassMask(Viewport *dest, Sprite *sprite, SRect *spriteRect) {
	int i, j, skipAddress, skipStart;
	byte *address, *start;
	int l, r, t, b, w, h, lp, tp;
	uint16 st;
	uint16 skipColor = sprite->skipColor;
	int dc = sprite->glassTag->intensity;
	int *colorIndex = sprite->colorIndex;

	/* CLIPPING SPRITE RECT */
	if (spriteRect) {
		l = spriteRect->left;
		t = spriteRect->top;
		r = spriteRect->right;
		b = spriteRect->bottom;
	} else {
		l = sprite->rect.left;
		t = sprite->rect.top;
		r = sprite->rect.right;
		b = sprite->rect.bottom;
	}

	/* CLIPPING OF RECTS */
	if (l < 0)
		l = 0;

	if (r > (dest->width - 1))
		r = dest->width - 1;

	if (t < 0)
		t = 0;

	if (b > (dest->height - 1))
		b = dest->height - 1;

	/* GET THE OVERALL DIMENSION */
	w = r - l + 1;
	h = b - t + 1;

	lp = abs(sprite->rect.left - l);
	tp = abs(sprite->rect.top - t);

	skipAddress = sprite->port->rowBytes;
	skipStart = dest->rowBytes;

	/* COPY BITS NOW */
	if (dest->colors == 8) {
		return;
	} else if (colorIndex) {
		address = (byte *)sprite->port->ptr + tp * skipAddress + lp;
		start = (byte *)dest->ptr + t * skipStart + (l << 1);

		switch (sprite->glassTag->type) {
		case SPRITE_GLASS_GRAY:
			for (i = 0; i < h; i++) {
				for (j = 0; j < w; j++) {
					if (colorIndex[((byte *)address)[j]] != skipColor) {
						int dr, dg, db;
						st = ((uint16 *)start)[j];
						/*							if ( pixFormat ) {
												 dr = ( ( st >> 11 ) & 0x1f ) - dc;
											  dg = ( ( st >> 6 ) & 0x1f ) - dc;
											   db = ( st & 0x1f ) - dc;
													}
													else {
						*/                     	dr = ((st >> 10) & 0x1f) - dc;
						dg = ((st >> 5) & 0x1f) - dc;
						db = (st & 0x1f) - dc;
						//							}
						if (dr < 0)
							dr = 0;
						if (dg < 0)
							dg = 0;
						if (db < 0)
							db = 0;
						//							if ( pixFormat )
						//	                     ((uint16 *)start)[j] = MK_COLOR16X ( dr, dg, db ); 
						//							else
						((uint16 *)start)[j] = MK_COLOR16(dr, dg, db);
					}
				}
				address += skipAddress;
				start += skipStart;
			}
			break;

		case SPRITE_GLASS_TINT:
			st = sprite->glassTag->color;
			for (i = 0; i < h; i++) {
				for (j = 0; j < w; j++) {
					if (colorIndex[((byte *)address)[j]] != skipColor) {
						int sc = ((uint16 *)start)[j];
						/*							if ( pixFormat ) {
												t = ( ( GET_R16X ( sc ) + GET_G16X (sc ) + GET_B16X ( sc ) ) << 8 )
													 / 93;
												((uint16 *)start)[j] = MK_COLOR16X (
																( GET_R16X ( st ) * t ) >> 8,
															 ( GET_G16X ( st ) * t ) >> 8,
																GET_B16X ( st ) * t >> 8 );
													}
													else {
						*/	                     t = ((GET_R16(sc) + GET_G16(sc) + GET_B16(sc)) << 8)
	/ 93;

						((uint16 *)start)[j] = MK_COLOR16(
							(GET_R16(st) * t) >> 8,
							(GET_G16(st) * t) >> 8,
							GET_B16(st) * t >> 8);
						//  						 	}
					}
				}
				address += skipAddress;
				start += skipStart;
			}

			break;

		case SPRITE_GLASS_LIGHT:
			for (i = 0; i < h; i++) {
				for (j = 0; j < w; j++) {
					int pix = colorIndex[((byte *)address)[j]];
					if (pix != skipColor) {
						int xr, xg, xb;
						int yr, yg, yb;
						int sc = ((uint16 *)start)[j];
						/*							if ( pixFormat ) {
														xr = GET_R16X ( pix );
														xg = GET_G16X ( pix );
														xb = GET_B16X ( pix );
														yr = GET_R16X ( sc );
														yg = GET_G16X ( sc );
														yb = GET_B16X ( sc );
												((uint16 *)start)[j] = MK_COLOR16X (
																		( xr + yr ) >> 1,
																		( xg + yg ) >> 1,
																		( xb + yb ) >> 1 );
													}
													else {
						*/								xr = GET_R16(pix);
						xg = GET_G16(pix);
						xb = GET_B16(pix);
						yr = GET_R16(sc);
						yg = GET_G16(sc);
						yb = GET_B16(sc);

						((uint16 *)start)[j] = MK_COLOR16(
							(xr + yr) >> 1,
							(xg + yg) >> 1,
							(xb + yb) >> 1);
						//							}
					}
				}
				address += skipAddress;
				start += skipStart;
			}
			break;
		}
	} else {
		address = (byte *)sprite->port->ptr + tp * skipAddress + (lp << 1);
		start = (byte *)dest->ptr + t * skipStart + (l << 1);

		switch (sprite->glassTag->type) {
		case SPRITE_GLASS_GRAY:
			for (i = 0; i < h; i++) {
				for (j = 0; j < w; j++) {
					if (((uint16 *)address)[j] != skipColor) {
						int dr, dg, db;
						st = ((uint16 *)start)[j];
						/*							if ( pixFormat ) {
												dr = ( ( st >> 11 ) & 0x1f ) - dc;
												dg = ( ( st >> 6 ) & 0x1f ) - dc;
												db = ( st & 0x1f ) - dc;
													}
													else {
						*/                     	dr = ((st >> 10) & 0x1f) - dc;
						dg = ((st >> 5) & 0x1f) - dc;
						db = (st & 0x1f) - dc;
						//							}
						if (dr < 0)
							dr = 0;
						if (dg < 0)
							dg = 0;
						if (db < 0)
							db = 0;
						/*							if ( pixFormat )
												((uint16 *)start)[j] = MK_COLOR16X ( dr, dg, db );
													else
						*/	                    	((uint16 *)start)[j] = MK_COLOR16(dr, dg, db);
					}
				}
				address += skipAddress;
				start += skipStart;
			}
			break;

		case SPRITE_GLASS_TINT:
			st = sprite->glassTag->color;
			for (i = 0; i < h; i++) {
				for (j = 0; j < w; j++) {
					if (((uint16 *)address)[j] != skipColor) {
						int sc = ((uint16 *)start)[j];
						/*							if ( pixFormat ) {
												t = ( ( GET_R16X ( sc ) + GET_G16X (sc ) + GET_B16X ( sc ) ) << 8 )
													 / 93;
											   ((uint16 *)start)[j] = MK_COLOR16X (
															( GET_R16X ( st ) * t ) >> 8,
															( GET_G16X ( st ) * t ) >> 8,
															  GET_B16X ( st ) * t >> 8 );
													}
												else {
						*/							  	t = ((GET_R16(sc) + GET_G16(sc) + GET_B16(sc)) << 8)
	/ 93;
						((uint16 *)start)[j] = MK_COLOR16(
							(GET_R16(st) * t) >> 8,
							(GET_G16(st) * t) >> 8,
							GET_B16(st) * t >> 8);
						//							}
					}
				}
				address += skipAddress;
				start += skipStart;
			}

			break;

		case SPRITE_GLASS_LIGHT:
			for (i = 0; i < h; i++) {
				for (j = 0; j < w; j++) {
					unsigned int pix = ((uint16 *)address)[j];
					if (pix != skipColor) {
						uint16 xr, xg, xb;
						uint16 yr, yg, yb;
						uint16 sc = ((uint16 *)start)[j];
						/*							if ( pixFormat ) {
														xr = GET_R16X ( pix );
														xg = GET_G16X ( pix );
														xb = GET_B16X ( pix );
														yr = GET_R16X ( sc );
														yg = GET_G16X ( sc );
														yb = GET_B16X ( sc );
												 ((uint16 *)start)[j] = MK_COLOR16X (
																( xr + yr ) >> 1,
																( xg + yg ) >> 1,
																( xb + yb ) >> 1 );
													}
													else {
						*/								xr = GET_R16(pix);
						xg = GET_G16(pix);
						xb = GET_B16(pix);
						yr = GET_R16(sc);
						yg = GET_G16(sc);
						yb = GET_B16(sc);
						((uint16 *)start)[j] = MK_COLOR16(
							(xr + yr) >> 1,
							(xg + yg) >> 1,
							(xb + yb) >> 1);
						//							}
					}
				}
				address += skipAddress;
				start += skipStart;
			}
			break;
		}
	}
}

void Port2Buffer16(void *dest, Viewport *src, SRect *r) {
	int i, t;
	const byte *srcPtr = (const byte *)((long)src->ptr + (long)((r->left + r->top * scene_width) * 2));
	byte *destPtr = (byte *)dest;

	t = r->width << 1;
	for (i = 0; i < r->height; i++) {
		memcpy(destPtr, srcPtr, t);
		destPtr += r->width;
		srcPtr += scene_width;
	}
}

inline void GetSpriteClipRect(Viewport *dest, Sprite *sprite, SRect *spriteRect,
	SRect *clipRect) {
	/* CLIPPING SPRITE RECT */
	if (spriteRect) {
		clipRect->left = spriteRect->left;
		clipRect->top = spriteRect->top;
		clipRect->right = spriteRect->right;
		clipRect->bottom = spriteRect->bottom;
	} else {
		clipRect->left = sprite->rect.left;
		clipRect->top = sprite->rect.top;
		clipRect->right = sprite->rect.right;
		clipRect->bottom = sprite->rect.bottom;
	}

	/* CLIPPING OF RECTS */
	if (clipRect->left < 0)
		clipRect->left = 0;

	if (clipRect->right > (dest->width - 1))
		clipRect->right = dest->width - 1;

	if (clipRect->top < 0)
		clipRect->top = 0;

	if (clipRect->bottom > (dest->height - 1))
		clipRect->bottom = dest->height - 1;

	/* GET THE OVERALL DIMENSION */
	clipRect->width = clipRect->right - clipRect->left + 1;
	clipRect->height = clipRect->bottom - clipRect->top + 1;
}

inline void FillScanLines(byte *target, byte *source,
	int height, int width, int skipTarget, int skipSource,
	int *colorIndex, int nColorDep, int nFlip, int skipColor) {
	int i, j, mask = 0;

	if (colorIndex)
		mask = 0x1;

	if (nColorDep == 8)
		mask |= 0x1 << 1;

	switch (mask) {
	case 0:
		width = width << 1;
		for (i = 0; i < height; i++) {
			memcpy(target, source, width);
			target += skipTarget;
			source += skipSource;
		}
		break;

	case 1:
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++)
				((uint16 *)target)[j] = colorIndex[source[j]];
			target += skipTarget;
			source += skipSource;
		}
		break;

	case 2:
		for (i = 0; i < height; i++) {
			memcpy(target, source, width);
			target += skipTarget;
			source += skipSource;
		}
		break;

	case 3:
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++)
				((uint16 *)target)[j] = colorIndex[source[j]];
			target += skipTarget;
			source += skipSource;
		}
		break;

	default:
		error("invalid mask");
	}
}

inline void MaskScanLines(byte *target, byte *source,
	int height, int width, int skipTarget, int skipSource,
	int *colorIndex, int nColorDep, int nFlip, int skipColor) {
	uint16 pix;
	int i, j, mask = 0;

	if (colorIndex)
		mask = 0x1;

	if (nColorDep == 8)
		mask |= 0x1 << 1;

	switch (mask) {
	case 0:
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				if (((uint16 *)source)[j] != (uint16)skipColor)
					((uint16 *)target)[j] = ((uint16 *)source)[j];
			}
			source += skipSource;
			target += skipTarget;
		}
		break;

	case 1:
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				pix = colorIndex[source[j]];
				if (pix != skipColor)
					((uint16 *)target)[j] = pix;
			}
			source += skipSource;
			target += skipTarget;
		}
		break;

	case 2:
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				if (source[j] != skipColor)
					target[j] = source[j];
			}
			source += skipSource;
			target += skipTarget;
		}
		break;

	case 3:
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				pix = colorIndex[source[j]];
				if (pix != skipColor)
					((uint16 *)target)[j] = pix;
			}
			source += skipSource;
			target += skipTarget;
		}
		break;

	default:
		error("invalid mask");
	}
}

void DrawSpriteFlip(Viewport *dest, Sprite *sprite, SRect *spriteRect,
	int nFlip, int nMask) {
	int skipAddress, skipStart, *colorIndex;
	byte *address, *start;
	SRect clipRect;
	int lp, tp;
	int skipColor = sprite->skipColor;

	GetSpriteClipRect(dest, sprite, spriteRect, &clipRect);

	if (nFlip & SPRITE_FLIPX)
		lp = abs(sprite->rect.left - (clipRect.right - sprite->rect.width));
	else
		lp = abs(sprite->rect.left - clipRect.left);

	if (nFlip & SPRITE_FLIPY)
		tp = abs(sprite->rect.top - (clipRect.bottom - sprite->rect.height));
	else
		tp = abs(sprite->rect.top - clipRect.top);

	skipAddress = sprite->port->rowBytes;
	skipStart = dest->rowBytes;

	/* COPY BITS NOW */
	if (sprite->colorIndex) {
		colorIndex = sprite->colorIndex;
		if (dest->colors == 8) {
			address = (byte *)sprite->port->ptr + tp * skipAddress + lp;
			start = (byte *)dest->ptr + clipRect.top * skipStart + clipRect.left;
			if (nMask & SPRITE_MASK)
				MaskScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, colorIndex, 8,
					nFlip, skipColor);
			else
				FillScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, colorIndex, 8, nFlip,
					skipColor);
		} else {
			address = (byte *)sprite->port->ptr + tp * skipAddress + lp;
			start = (byte *)dest->ptr + clipRect.top * skipStart +
				(clipRect.left << 1);
			if (nMask & SPRITE_MASK)
				MaskScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, colorIndex, 8, nFlip,
					skipColor);
			else
				FillScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, colorIndex, 8, nFlip,
					skipColor);
		}
	} else {
		if (dest->colors == 8) {
			address = (byte *)sprite->port->ptr + tp * skipAddress + lp;
			start = (byte *)dest->ptr + clipRect.top * skipStart + clipRect.left;
			if (nMask & SPRITE_MASK)
				MaskScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, sprite->colorIndex,
					dest->colors, nFlip, skipColor);
			else
				FillScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, sprite->colorIndex,
					dest->colors, nFlip, skipColor);
		} else {
			address = (byte *)sprite->port->ptr + tp * skipAddress + (lp << 1);
			start = (byte *)dest->ptr + clipRect.top * skipStart + (clipRect.left << 1);
			if (nMask & SPRITE_MASK)
				MaskScanLines(start, address, clipRect.height,
					clipRect.width, skipStart, skipAddress,
					sprite->colorIndex, dest->colors, nFlip, skipColor);
			else
				FillScanLines(start, address, clipRect.height,
					clipRect.width, skipStart, skipAddress,
					sprite->colorIndex, dest->colors, nFlip, skipColor);
		}
	}

	/* DRAW LINES... */
	if (sprite->drawList)
		DrawLines(dest, sprite, spriteRect);
}

inline void CopyScanLines(byte *ucTarget, byte *ucSource,
	int nHeight, int nWidth, int skipTarget, int skipSource,
	int *colorIndex, int nColorDep, int nFlip, int skipColor) {
	byte *source, *target;
	int i, j, k;

	source = ucSource;
	target = ucTarget;

	if (nFlip & SPRITE_FLIPY) {
		if (nFlip & SPRITE_FLIPX)
			i = 0;
		else
			i = 1;

		if (nColorDep == 16)
			source += (nHeight - i) * nWidth * 2;
		else
			source += (nHeight - i) * nWidth;
	}

	for (i = 0; i < nHeight; i++) {
		if ((nFlip & SPRITE_FLIPX) && (nFlip & SPRITE_FLIPY)) {
			if (colorIndex) {
				for (j = 0, k = nWidth - 2; j < nWidth; j++, k--)
					((byte *)target)[k] = ((byte *)source)[j];
			} else {
				for (j = 0, k = nWidth - 1; j < nWidth; j++, k--) {
					if (k >= 1)
						((uint16 *)target)[k - 1] = ((uint16 *)source)[j];
				}
			}
			source -= skipSource;
			target += skipTarget;
		}  // End of FLIPX & FLIPY condition
		else if (nFlip & SPRITE_FLIPY) {
			if (colorIndex) {
				for (j = 0; j < nWidth; j++) {
					((byte *)target)[j] = ((byte *)source)[j];
				}
			} else {
				memcpy(target, source, nWidth * 2);
			}
			source -= skipSource;
			target += skipTarget;
		} // End of FLIPY condition
		else {
			if (!(nFlip & SPRITE_FLIPX) && !(nFlip & SPRITE_FLIPY)
				&& colorIndex) {
				for (j = 0; j < nWidth; j++) {
					((byte *)target)[j] = ((byte *)source)[j];
				}
			} else if (!(nFlip & SPRITE_FLIPX) && !(nFlip & SPRITE_FLIPY)) {
				memcpy(target, source, nWidth * 2);
			} else if ((nFlip & SPRITE_FLIPX) && colorIndex) {
				for (j = 0, k = nWidth - 2; j < nWidth; j++, k--) {
					((byte *)target)[k] = ((byte *)source)[j];
				}
			} else if (nFlip & SPRITE_FLIPX) {
				for (j = 0, k = nWidth - 1; j < nWidth; j++, k--) {
					if (k >= 1)
						((uint16 *)target)[k - 1] = ((uint16 *)source)[j];
				}
			}
			source += skipSource;
			target += skipTarget;
		} // End of FLIPX and NO-FLIP condition
	} // End of for (i) loop
}

void CopyScaleSpriteMask(Viewport *dest, Sprite *sprite, SRect *spriteRect, int newLenX, int newLenY) {
	int i, j, skipAddress, skipStart, *colorIndex;
	byte *address, *startAddress, *start;
	int l, r, t, b, w, h, lp, tp;
	int skipColor = sprite->skipColor;

	/* CLIPPING SPRITE RECT */
	l = spriteRect->left;
	t = spriteRect->top;
	r = spriteRect->right;
	b = spriteRect->bottom;

	/* GET THE OVERALL DIMENSION */
	w = dest->width;
	h = dest->height;

	/* MAKE SCALE TABLE */
	if (sprite->state & SPRITE_SCALE_TABLE) {
		sprite->state &= ~SPRITE_SCALE_TABLE;
		MakeScaleTableI(sprite->scaleTableX, sprite->port->width, newLenX);
		MakeScaleTableI(sprite->scaleTableY, sprite->port->height, newLenY);
	}

	lp = l;
	tp = t;

	skipAddress = sprite->port->rowBytes;
	skipStart = dest->rowBytes;

	if (sprite->colorIndex) {
		colorIndex = sprite->colorIndex;
		if (dest->colors == 8) {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					t = (sprite->scaleTableX)[lp + j];
					((byte *)start)[j] = ((byte *)address)[t];
				}
				start += skipStart;
			}
		} else {
			uint16 pix;
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					t = sprite->scaleTableX[lp + j];
					pix = colorIndex[((byte *)address)[t]];
					if (pix != skipColor)
						((uint16 *)start)[j] = pix;
				}
				start += skipStart;
			}
		}
	} else {
		if (dest->colors == 8) {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					t = (sprite->scaleTableX)[lp + j];
					if (((byte *)address)[t] != (byte)skipColor)
						((byte *)start)[j] = ((byte *)address)[t];
				}
				start += skipStart;
			}
		} else {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					t = sprite->scaleTableX[lp + j];
					if (((uint16 *)address)[t] != (uint16)skipColor)
						((uint16 *)start)[j] = ((uint16 *)address)[t];
				}
				start += skipStart;
			}
		}
	}
}

void CopyScaleSprite(Viewport *dest, Sprite *sprite, SRect *spriteRect, int newLenX, int newLenY) {
	int i, j, skipAddress, skipStart, *colorIndex;
	byte *address, *startAddress, *start;
	int l, r, t, b, w, h, lp, tp;

	/* CLIPPING SPRITE RECT */
	l = spriteRect->left;
	t = spriteRect->top;
	r = spriteRect->right;
	b = spriteRect->bottom;

	/* GET THE OVERALL DIMENSION */
	w = dest->width;
	h = dest->height;

	/* MAKE SCALE TABLE */
	if (sprite->state & SPRITE_SCALE_TABLE) {
		sprite->state &= ~SPRITE_SCALE_TABLE;
		MakeScaleTableI(sprite->scaleTableX, sprite->port->width, newLenX);
		MakeScaleTableI(sprite->scaleTableY, sprite->port->height, newLenY);
	}

	lp = l;
	tp = t;

	skipAddress = sprite->port->rowBytes;
	skipStart = dest->rowBytes;

	if (sprite->colorIndex) {
		colorIndex = sprite->colorIndex;
		if (dest->colors == 8) {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					((byte *)start)[j] = ((byte *)address)[sprite->scaleTableX[lp + j]];
				}
				start += skipStart;
			}
		} else {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					((uint16 *)start)[j] = colorIndex[((byte *)address)[sprite->scaleTableX[lp + j]]];
				}
				start += skipStart;
			}
		}
	} else {
		if (dest->colors == 8) {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr;
			for (i = 0; i < h; i++) {
				address = startAddress + sprite->scaleTableY[tp + i] * skipAddress;
				for (j = 0; j < w; j++) {
					((byte *)start)[j] = ((byte *)address)[sprite->scaleTableX[lp + j]];
				}
				start += skipStart;
			}
		} else {
			startAddress = (byte *)sprite->port->ptr;
			start = (byte *)dest->ptr;
			for (i = 0; i < h; i++) {
				address = startAddress + (sprite->scaleTableY[tp + i] * skipAddress);
				for (j = 0; j < w; j++) {
					((uint16 *)start)[j] = ((uint16 *)address)[sprite->scaleTableX[lp + j]];
				}
				start += skipStart;
			}
		}
	}
}

void CopySpriteFlip(Viewport *dest, Sprite *sprite, SRect *spriteRect,
	int nFlip, int nMask) {
	int skipAddress, skipStart, *colorIndex;
	byte *address, *start;
	SRect clipRect;
	int lp, tp;
	int skipColor = sprite->skipColor;

	SRect_Copy(&clipRect, spriteRect);

	lp = clipRect.left;
	tp = clipRect.top;

	skipAddress = sprite->port->rowBytes;
	skipStart = dest->rowBytes;

	/* COPY BITS NOW */
	if (sprite->colorIndex) {
		colorIndex = sprite->colorIndex;
		if (dest->colors == 8) {
			address = (byte *)sprite->port->ptr + tp * skipAddress + lp;
			start = (byte *)dest->ptr;
			CopyScanLines(start, address, clipRect.height, clipRect.width,
				skipStart, skipAddress, colorIndex, 8,
				nFlip, skipColor);
		} else {
			address = (byte *)sprite->port->ptr + tp * skipAddress + lp;
			start = (byte *)dest->ptr;
			if (nMask & SPRITE_MASK)
				MaskScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, colorIndex, 8, nFlip,
					skipColor);
			else
				FillScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, colorIndex, 8, nFlip,
					skipColor);
		}
	} else {
		if (dest->colors == 8) {
			address = (byte *)sprite->port->ptr + tp * skipAddress + lp;
			start = (byte *)dest->ptr;
			if (nMask & SPRITE_MASK)
				MaskScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, sprite->colorIndex,
					dest->colors, nFlip, skipColor);
			else
				FillScanLines(start, address, clipRect.height, clipRect.width,
					skipStart, skipAddress, sprite->colorIndex,
					dest->colors, nFlip, skipColor);
		} else {
			address = (byte *)sprite->port->ptr + tp * skipAddress + (lp << 1);
			start = (byte *)dest->ptr;
			if (nMask & SPRITE_MASK)
				MaskScanLines(start, address, clipRect.height,
					clipRect.width, skipStart, skipAddress,
					sprite->colorIndex, dest->colors, nFlip, skipColor);
			else
				FillScanLines(start, address, clipRect.height,
					clipRect.width, skipStart, skipAddress,
					sprite->colorIndex, dest->colors, nFlip, skipColor);
		}
	}
}

void DrawScaleBits(Viewport *dest, Viewport *src, SRect *rect, SRect *clipRect) {
	int i, j, skipAddress, skipStart;
	byte *address, *startAddress, *start;
	int l, r, t, b, w, h, lp, tp, dw, dh;
	uint scaleTableX[1024];
	uint scaleTableY[1024];

	l = rect->left;
	t = rect->top;
	r = rect->right;
	b = rect->bottom;

	/* CLIPPING OF RECTS */
	if (l < 0)
		l = 0;

	if (r > (dest->width - 1))
		r = dest->width - 1;

	if (t < 0)
		t = 0;

	if (b > (dest->height - 1))
		b = dest->height - 1;

	/* GET THE OVERALL DIMENSION */
	w = r - l + 1;
	h = b - t + 1;

	/* MAKE SCALE TABLE */
	MakeScaleTableI(scaleTableX, w, dest->width);
	MakeScaleTableI(scaleTableY, h, dest->height);

	lp = abs(rect->left - l);
	tp = abs(rect->top - t);

	skipAddress = src->rowBytes;
	skipStart = dest->rowBytes;

	if (dest->colors == 8) {
		if (clipRect) {
			dw = clipRect->width;
			dh = clipRect->height;
			start = (byte *)dest->ptr + clipRect->top *
				skipStart + (clipRect->left << 1);
		} else {
			dw = dest->width;
			dh = dest->height;
			start = (byte *)dest->ptr;
		}
		startAddress = (byte *)src->ptr + t * skipAddress + l;
		for (i = 0; i < dest->height; i++) {
			address = startAddress + scaleTableY[tp + i] * skipAddress;
			for (j = 0; j < dest->width; j++) {
				((byte *)start)[j] = ((byte *)address)[scaleTableX[lp + j]];
			}
			start += skipStart;
		}
	} else {
		if (clipRect) {
			dw = clipRect->width;
			dh = clipRect->height;
			start = (byte *)dest->ptr + clipRect->top *
				skipStart + (clipRect->left << 1);
		} else {
			dw = dest->width;
			dh = dest->height;
			start = (byte *)dest->ptr;
		}
		startAddress = (byte *)src->ptr + t * skipAddress + (l << 1);
		for (i = 0; i < dh; i++) {
			address = startAddress + scaleTableY[tp + i] * skipAddress;
			for (j = 0; j < dw; j++) {
				((uint16 *)start)[j] = ((uint16 *)address)[scaleTableX[lp + j]];
			}
			start += skipStart;
		}
	}
}

} // namespace Spycraft
