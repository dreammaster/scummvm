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

#include "spycraft/advlib.h"
#include "spycraft/advsprite.h"
#include "spycraft/advport.h"
#include "spycraft/advback.h"
#include "spycraft/advscreen.h"
#include "spycraft/advbits.h"
#include "spycraft/advtime.h"

namespace Spycraft {

/* FORWARD DECLARATE CLIPPING FUNCTION */
extern int SCClipping(int *, int *, int *, int *, SRect *);

static SRect __sprite_rect;
static Background *updateBack = nullptr;
static int blt2Screen = true;

/*
#define __MADE_EXT
*/

#ifdef __MADE_EXT
void (*finalCompose)( Viewport * ) = nullptr;
#endif

void sfxSpriteFrame ( Sprite *sprite, Viewport *port )
{
	if ( !sprite->redraw ) {
		sprite->redraw = true;
		SRect_Copy ( &sprite->oldRect, &sprite->rect );
	}

	sprite->changed = true;
	sprite->port = port;
	sprite->skipColor = port->skipColor;
	sprite->orig_x = port->origX;
	sprite->orig_y = port->origY;

	sprite->rect.left = sprite->x - sprite->orig_x;
	sprite->rect.top = sprite->y - sprite->orig_y;
	sprite->rect.right = sprite->rect.left + port->width - 1;
	sprite->rect.bottom = sprite->rect.top + port->height - 1;
	sprite->rect.width = port->width;
	sprite->rect.height = port->height;
}

void sfxMoveSprite ( Sprite *sprite, int theX, int theY )
{
	/* SET THE POSITION */
	sprite->x = theX;
	sprite->y = theY;

	/* SET NECESSARY FLAGS */
	sprite->changed = true;

	/* AUTO CHANNELING */
	if ( ( !( sprite->state & SPRITE_FIXEDPRI ) ) && ( sprite->channel != theY ) ) {
		sprite->channel = theY;
		ArrayList_Unlink ( sprite->back->spriteList, sprite );
		ArrayList_Add ( sprite->back->spriteList, sprite, CompareSprite );
	}

	/* SAVE THE OLD RECT */
	if ( !sprite->redraw )
	{
		sprite->redraw = true;
		SRect_Copy ( &sprite->oldRect, &sprite->rect );
	}
						
	/* SET THE NEW RECT */
	sprite->rect.left = theX - sprite->orig_x;
	sprite->rect.top = theY - sprite->orig_y;
	sprite->rect.right = sprite->rect.left + sprite->rect.width - 1;
	sprite->rect.bottom = sprite->rect.top + sprite->rect.height - 1;
}

void sfxHideSprite ( Sprite *sprite )
{
	if ( !( sprite->state & SPRITE_HIDDEN ) ) {
		sprite->changed = true;
		sprite->state |= SPRITE_HIDDEN;

		if ( !sprite->redraw ) {
			sprite->redraw = true;

			/* SAVE THE OLD RECT */
			SRect_Copy ( &sprite->oldRect, &sprite->rect );
		}
	}
}

void sfxShowSprite(Sprite *sprite) {
	if (sprite->state & SPRITE_HIDDEN) {
		if (sprite->back) {
			if (ArrayList_Unlink(sprite->back->hiddenList, sprite))
				ArrayList_Add(sprite->back->spriteList, sprite, CompareSprite);
		}
		sprite->changed = true;
		sprite->state &= ~SPRITE_HIDDEN;
	}
}

void sfxKillSprite(Sprite *sprite) {
	sprite->changed = true;
	sprite->destroy = true;

	if (sprite->state & SPRITE_HIDDEN) {
		int ok = ArrayList_Del(sprite->back->hiddenList, sprite, FreeSpriteFn);
		if (ok)
			return;
	}

	if (!sprite->redraw) {
		sprite->redraw = true;

		/* SAVE THE OLD RECT */
		SRect_Copy(&sprite->oldRect, &sprite->rect);
	}
}

void sfxSpriteNoMask(Sprite *sprite) {
	sprite->state &= ~SPRITE_MASK;
	sprite->changed = true;
}

void sfxSpriteMask(Sprite *sprite) {
	sprite->state |= SPRITE_MASK;
	sprite->changed = true;
}

Sprite *sfxCreateSprite(int theBack, int theX, int theY, uint16 theScaleX, uint16 theScaleY, Viewport *port) {
	Sprite *sprite = AllocSprite(backgrounds[theBack], (uint16)theY);

	if (sprite) {
		sprite->back = backgrounds[theBack];
		sprite->x = theX;
		sprite->y = theY;

		/* SET SPRITE RECT */
		sprite->port = port;
		sprite->skipColor = port->skipColor;
		sprite->orig_x = port->origX;
		sprite->orig_y = port->origY;
		sprite->rect.left = theX - sprite->orig_x;
		sprite->rect.top = theY - sprite->orig_y;
		sprite->rect.right = sprite->rect.left + port->width - 1;
		sprite->rect.bottom = sprite->rect.top + port->height - 1;
		sprite->rect.width = port->width;
		sprite->rect.height = port->height;

		/* SPRITE CHANNEL */
		sprite->channel = theY;

		/* SPRITE SCALING */
		if ((theScaleX != SPRITE_CENTER_SCALEX) ||
			(theScaleY != SPRITE_CENTER_SCALEY))
		{
			if (sprite->scaleTableX == nullptr) {
				sprite->scaleTableX = (uint *)AllocPtr(sizeof(int) * SCALE_TABLE_SIZE);
			}
			if (sprite->scaleTableY == nullptr) {
				sprite->scaleTableY = (uint *)AllocPtr(sizeof(int) * SCALE_TABLE_SIZE);
			}
			sprite->state |= (SPRITE_SCALED | SPRITE_SCALE_TABLE);
			sprite->scaleX = theScaleX;
			sprite->scaleY = theScaleY;
			sprite->rect.width = (port->width * theScaleX) >> 9;
			sprite->rect.height = (port->height * theScaleY) >> 9;

			sprite->orig_x = (port->origX * theScaleX) >> 9;
			sprite->orig_y = (port->origY * theScaleY) >> 9;
			sprite->rect.left = sprite->x - sprite->orig_x;
			sprite->rect.top = sprite->y - sprite->orig_y;
			sprite->rect.right = sprite->rect.left + sprite->rect.width - 1;
			sprite->rect.bottom = sprite->rect.top + sprite->rect.height - 1;

			/* COMPUTE THE RATIO */
			if ((sprite->rect.width) == 0 || (sprite->rect.height == 0)) {
				sprite->state |= SPRITE_UNDERSCALED;
			}
		}
	}

	return sprite;
}

void sfxReleasePriority(Sprite *sprite) {
	sprite->state &= ~SPRITE_FIXEDPRI;
	sprite->changed = true;

	sprite->channel = sprite->y;
	ArrayList_Unlink(sprite->back->spriteList, sprite);
	ArrayList_Add(sprite->back->spriteList, sprite, CompareSprite);

	if (!sprite->redraw) {
		sprite->redraw = true;
		SRect_Copy(&sprite->oldRect, &sprite->rect);
	}
}

void sfxSpriteOrig(Sprite *sprite, int theX, int theY) {
	sprite->changed = true;

	sprite->port->origX = theX;
	sprite->port->origY = theY;

	/* SAVE THE OLD RECT */
	if (!sprite->redraw) {
		sprite->redraw = true;
		SRect_Copy(&sprite->oldRect, &sprite->rect);
	}

	if (sprite->state & SPRITE_SCALED) {
		sprite->orig_x = (theX * sprite->scaleX) >> 9;
		sprite->orig_y = (theY * sprite->scaleY) >> 9;
	} else {
		sprite->orig_x = theX;
		sprite->orig_y = theY;
	}

	/* RECOMPUTE THE RECT */
	sprite->rect.left -= sprite->orig_x;
	sprite->rect.top -= sprite->orig_y;
	sprite->rect.right -= sprite->orig_x;
	sprite->rect.bottom -= sprite->orig_y;
}

void sfxSpriteScale(Sprite *sprite, uint16 theScaleX, uint16 theScaleY) {
	Viewport *port = sprite->port;
	int spriteX, spriteY;

	if ((theScaleX == SPRITE_CENTER_SCALEX) &&
		(theScaleY == SPRITE_CENTER_SCALEY))
	{
		if (sprite->state & SPRITE_SCALED) {
			sprite->state &= ~SPRITE_SCALED;
			sprite->changed = true;

			/* GET ABSOLUTE COORDINATES */
			spriteX = sprite->rect.left + sprite->orig_x;
			spriteY = sprite->rect.top + sprite->orig_y;

			/* RESTORE THE OLD RECT */
			if (!sprite->redraw) {
				sprite->redraw = true;
				SRect_Copy(&sprite->oldRect, &sprite->rect);
			}

			/* GET THE ORIGINAL RECT */
			sprite->rect.width = port->width;
			sprite->rect.height = port->height;
			sprite->orig_x = port->origX;
			sprite->orig_y = port->origY;
			sprite->rect.left = spriteX - port->origX;
			sprite->rect.top = spriteY - port->origY;
			sprite->rect.right = sprite->rect.left + port->width - 1;
			sprite->rect.bottom = sprite->rect.top + port->height - 1;

			sprite->scaleX = theScaleX;
			sprite->scaleY = theScaleY;
		}
	} else {
		if (sprite->scaleTableX == nullptr) {
			sprite->scaleTableX = (uint *)AllocPtr(sizeof(int) * SCALE_TABLE_SIZE);
		}
		if (sprite->scaleTableY == nullptr) {
			sprite->scaleTableY = (uint *)AllocPtr(sizeof(int) * SCALE_TABLE_SIZE);
		}
		sprite->changed = true;
		sprite->state |= (SPRITE_SCALED | SPRITE_SCALE_TABLE);
		sprite->scaleX = theScaleX;
		sprite->scaleY = theScaleY;

		/* RESTORE THE OLD RECT */
		if (!sprite->redraw) {
			sprite->redraw = true;
			SRect_Copy(&sprite->oldRect, &sprite->rect);
		}

		/* GET ABSOLUTE COORDINATES */
		spriteX = sprite->rect.left + sprite->orig_x;
		spriteY = sprite->rect.top + sprite->orig_y;

		/* COMPUTE THE NEW RECT */
		sprite->rect.width = (port->width * theScaleX) >> 9;
		sprite->rect.height = (port->height * theScaleY) >> 9;

		sprite->orig_x = (port->origX * theScaleX) >> 9;
		sprite->orig_y = (port->origY * theScaleY) >> 9;
		sprite->rect.left = spriteX - sprite->orig_x;
		sprite->rect.top = spriteY - sprite->orig_y;
		sprite->rect.right = sprite->rect.left + sprite->rect.width - 1;
		sprite->rect.bottom = sprite->rect.top + sprite->rect.height - 1;

		/* COMPUTE THE RATIO */
		if ((sprite->rect.width) == 0 || (sprite->rect.height == 0)) {
			sprite->state |= SPRITE_UNDERSCALED;
		}
	}
}

void sfxSpriteChannel(Sprite *sprite, uint16 theChannel) {
	if (sprite->back) {
		sprite->changed = true;
		sprite->channel = theChannel;
		sprite->state |= SPRITE_FIXEDPRI;

		ArrayList_Unlink(sprite->back->spriteList, sprite);
		ArrayList_Add(sprite->back->spriteList, sprite, CompareSprite);
	}
}

int sfxIsSkip(Sprite *sprite, int theX, int theY) {
	if (sprite->state & SPRITE_MASK) {
		if (sprite->port->colors == 8) {
			return (
				*(unsigned char *)((intptr)sprite->port->ptr +
					theY * sprite->rect.width + theX) == sprite->skipColor
				);
		} else {
			if (sprite->colorIndex)
				return (sprite->colorIndex[*(unsigned char *)((intptr)sprite->port->ptr +
					((theY * sprite->rect.width + theX)))]
					== sprite->skipColor
					);
			else
				return (
					*(uint16 *)((intptr)sprite->port->ptr +
						((theY * sprite->rect.width + theX) << 1)) == sprite->skipColor
					);
		}
	} else {
		return false;
	}
}

int sfxGetSpritePixel(Sprite *sprite, int theX, int theY) {
	if (sprite->port->colors == 8) {
		return (
			*(unsigned char *)((intptr)sprite->port->ptr +
				theY * sprite->rect.width + theX));
	} else {
		return (
			*(uint16 *)((intptr)sprite->port->ptr +
				((theY * sprite->rect.width + theX) << 1)));
	}
}

void sfxInstallSpriteFn(Sprite *sprite, PFDraw thePtr) {
	sprite->state |= SPRITE_CUSTOM;
	sprite->drawFunc = thePtr;
}

void sfxRemoveSpriteFn(Sprite *sprite) {
	sprite->state &= ~SPRITE_CUSTOM;
	sprite->drawFunc = nullptr;
}

inline void UpdateSprites(Background *stage) {
	int i;
	SRect *rect;
	PartialSprite *pSprite;
	ArrayList *list;

	/* DRAW THE BACKGROUND RECTS */
	list = stage->underbits;
	for (i = 0; i < list->size; i++) {
		rect = (SRect *)list->elements[i];

		/* GET BITS FROM BACKPORT */
		DrawBits(stage->animPort, stage->backPort, rect);
		AddBltRect(stage->updateRects, rect);
	}
	ArrayList_Release(list);

	/* DRAW ALL INFECTED SPRITE IN REGION LIST */
	list = stage->updateRgn;
	for (i = 0; i < list->size; i++) {
		pSprite = (PartialSprite *)list->elements[i];

#ifdef __MADE_EXT
		/* DRAW THE CUSTOM SPRITE */
		if (pSprite->sprite->drawFunc) {
			(*pSprite->sprite->drawFunc)(stage->animPort, pSprite->sprite, &pSprite->rect);
			AddBltRect(stage->updateRects, &pSprite->rect);
			return;
		}
#endif

		/* DRAW SPRITE TO THE ANIMPORT */
		if (pSprite->sprite->state & SPRITE_SCALED) {
			if (pSprite->sprite->state & SPRITE_UNDERSCALED) {
				pSprite->sprite->state &= ~SPRITE_UNDERSCALED;
			} else {
				if (pSprite->sprite->state & SPRITE_MASK)
					DrawScaleSpriteMask(stage->animPort, pSprite->sprite, &pSprite->rect);
				else
					DrawScaleSprite(stage->animPort, pSprite->sprite, &pSprite->rect);
			}
		} else if (pSprite->sprite->state & SPRITE_GLASS) {
			DrawGlassMask(stage->animPort, pSprite->sprite, &pSprite->rect);
		} else
			DrawSpriteFlip(stage->animPort, pSprite->sprite, &pSprite->rect,
				pSprite->sprite->state, pSprite->sprite->state);

		AddBltRect(stage->updateRects, &pSprite->rect);
	}
}

void DirectRelease(Background *stage, int dx, int dy) {
	int i;
	Sprite *sprite;
	PartialSprite *pSprite;
	ArrayList *list = stage->spriteList;

	updateBack = stage;

	/* UPDATE ENTIRE SCREEN */
	SRect_Init(&__sprite_rect, 0, 0, scene_width - 1, scene_height - 1);
	ArrayList_Add(stage->underbits, &__sprite_rect, nullptr);

	/* RELEASE SPRITES */
	for (i = 0; i < list->size; i++) {
		sprite = (Sprite *)list->elements[i];
		if ((sprite->destroy) || (sprite->state & SPRITE_HIDDEN)) {
			ArrayList_Add(stage->deleteList, sprite, nullptr);
		} else {
			if (sprite->state & SPRITE_LOCALIZED) {
				sprite->rect.left += dx;
				sprite->rect.top += dy;
				sprite->rect.right += dx;
				sprite->rect.bottom += dy;
			}
			if (SRect_Intersects(&stage->displayRect, &sprite->rect)) {
				sprite->changed = false;

				/* ADD TO REGION LIST */
				pSprite = (PartialSprite *)AllocPtr(sizeof(PartialSprite));
				SRect_Copy(&pSprite->rect, &sprite->rect);
				pSprite->sprite = sprite;
				ArrayList_Add(stage->updateRgn, pSprite, ComparePSprite);
			}
		}
	}

	/* UPDATE SPRITES */
	UpdateSprites(stage);
	blt2Screen = false;
	sfxUpdate();
}

void sfxReleaseSprites(Background *stage) {
	int i, j, t;
	Sprite *sprite;
	Sprite *curSprite;
	PartialSprite *pSprite;
	ArrayList *list = stage->spriteList;

	updateBack = stage;

	/* DRAWS ALL SPRITES BACK TO FRONT */
	for (i = 0; i < list->size; i++) {
		curSprite = (Sprite *)list->elements[i];

		if (curSprite->changed) {
			t = curSprite->state & SPRITE_LOCALIZED;

			/* ONLY OLD SPRITE NEEDS TO RESTORE BACKGROUND */
			if (curSprite->redraw) {
				curSprite->redraw = false;
				if (SRect_Intersects(&stage->displayRect, &curSprite->oldRect)) {

					/* RESTORE ALL CHANGED SPRITES' BACKGROUND */
					ArrayList_Add(stage->underbits, &curSprite->oldRect, nullptr);

					/* NEED TO CHECK ALL INTERSECTED RECT */
					for (j = 0; j < list->size; j++) {
						sprite = (Sprite *)list->elements[j];

						if (!sprite->changed && !sprite->destroy && (sprite != curSprite)
							&& (SRect_Intersects(&sprite->rect, &curSprite->oldRect))
							&& (!(sprite->state & SPRITE_HIDDEN))
							)
						{
							int ii;
							int ok_to_add;
							SRect out;

							/* GET THE AREA OF INTERSECTION */
							SRect_GetIntersection(&sprite->rect, &curSprite->oldRect,
								&out);
							ok_to_add = true;
							for (ii = 0; ii < stage->updateRgn->size; ii++) {
								PartialSprite *ps = (PartialSprite *)stage->updateRgn->elements[ii];
								SRect *r = &ps->rect;
								if ((ps->sprite == sprite) && SRect_Contains(r, &out)) {
									ok_to_add = false;
									break;
								}
							}
							/* ALLOCATE A PARTIAL SPRITE */
							if (ok_to_add) {
								pSprite = (PartialSprite *)AllocPtr(sizeof(PartialSprite));
								SRect_Copy(&pSprite->rect, &out);
								pSprite->sprite = sprite;
								ArrayList_Add(stage->updateRgn, pSprite, ComparePSprite);
							}
						}
					} // end FOR
				} // end IF INTERSECT
			} // end IF	REDRAW

			/* ONLY SPRITE NOT DELETED NEED TO CHECK FOR OTHER SPRITES */
			if ((curSprite->destroy) || (curSprite->state & SPRITE_HIDDEN)) {
				ArrayList_Add(stage->deleteList, curSprite, nullptr);
			} else {
				/* ONLY SPRITE CONTAINED IN CURRENT DISPLAY RECT */
				if ((!t || (t && SRect_Intersects
				(&stage->displayRect, &curSprite->rect)))
					)
				{
					/* ADD SELF TO REGION LIST */
					pSprite = (PartialSprite *)AllocPtr(sizeof(PartialSprite));
					pSprite->sprite = curSprite;

					SRect_Copy(&pSprite->rect, &curSprite->rect);

					ArrayList_Add(stage->updateRgn, pSprite, ComparePSprite);

					/* CHECK FOR INTERSECTION OF BACKGROUND RECT WITH ALL SPRITES */
					for (j = i + 1; j < list->size; j++) {

						sprite = (Sprite *)list->elements[j];

						/* ONCE INTERSECT WITH OTHER SPRITES ADD THEM TO REGION */
						if (!sprite->changed && !sprite->destroy &&
							SRect_Intersects(&sprite->rect, &curSprite->rect)
							&& (!(sprite->state & SPRITE_HIDDEN))
							)
						{
							int ii;
							int ok_to_add;
							SRect out;

							/* GET THE AREA OF INTERSECTION */
							SRect_GetIntersection(&sprite->rect, &curSprite->rect,
								&out);
							ok_to_add = true;
							for (ii = 0; ii < stage->updateRgn->size; ii++) {
								PartialSprite *ps = (PartialSprite *)stage->updateRgn->elements[ii];
								SRect *r = &ps->rect;
								if ((ps->sprite == sprite) && SRect_Contains(r, &out)) {
									ok_to_add = false;
									break;
								}
							}
							/* ADD TO REGION LIST */
							if (ok_to_add) {
								pSprite = (PartialSprite *)AllocPtr(sizeof(PartialSprite));
								SRect_Copy(&pSprite->rect, &out);
								pSprite->sprite = sprite;
								ArrayList_Add(stage->updateRgn, pSprite, ComparePSprite);
							}
						}
					} // end FOR
				} // end IF
			} // end ELSE
		} // end IF
	} // end FOR

	/* CLEAR THE CHANGE FLAG */
	for (i = 0; i < list->size; i++)
		((Sprite *)list->elements[i])->changed = false;
}

void sfxUpdate() {
	int i;
	Sprite *sprite;
	ArrayList *list;

	/* UPDATE ALL SPRITE */
	if (updateBack == nullptr)
		return;

	UpdateSprites(updateBack);

	/* UPDATE THE SCREEN */
	if (blt2Screen)
		UpdateScreen(updateBack);
	else
		blt2Screen = true;

	/* FREE REGION LIST (PARTIAL SPRITE CONTAINED IN REGION LIST) */
	list = updateBack->updateRgn;
	for (i = 0; i < list->size; i++) {
		FreePtr(list->elements[i]);
	}
	ArrayList_Release(list);

	/* FREE ALL DESTROYED SPRITE */
	list = updateBack->deleteList;
	for (i = 0; i < list->size; i++) {
		sprite = (Sprite *)list->elements[i];
		FreeSprite(updateBack, sprite);
	}

	ArrayList_Release(list);
}

Sprite *AllocSprite(Background *stage, uint16 theChannel) {
	Sprite *sprite = (Sprite *)AllocPtr(sizeof(Sprite));

	if (sprite) {

		/* FILLING SPRITE DEFAULT INFO */
		sprite->check = 99999;
		sprite->destroy = false;
		sprite->redraw = false;
		sprite->changed = true;
		sprite->channel = theChannel;
		sprite->state = SPRITE_LOCALIZED | SPRITE_MASK | SPRITE_FIXEDPRI;
		sprite->scaleX = SPRITE_CENTER_SCALEX;
		sprite->scaleY = SPRITE_CENTER_SCALEY;
		sprite->skipColor = 0;
		sprite->orig_x = 0;
		sprite->orig_y = 0;
		sprite->scaleTableX = nullptr;
		sprite->scaleTableY = nullptr;
		sprite->glassTag = nullptr;
		sprite->text = nullptr;
		sprite->drawList = nullptr;
		sprite->colorIndex = nullptr;
		sprite->back = nullptr;
		sprite->drawFunc = nullptr;

		/* INSERTING INTO SPRITELIST */
		if (stage->spriteList) {
			ArrayList_Add(stage->spriteList, sprite, CompareSprite);
		}
	}

	return sprite;
}

bool FreeSprite(Background *stage, Sprite *sprite) {
	if (!stage->spriteList)
		return false;

	if (sprite->destroy) {
		ArrayList_Del(stage->spriteList, sprite, FreeSpriteFn);
	} else {
		ArrayList_Unlink(stage->spriteList, sprite);
		ArrayList_Add(stage->hiddenList, sprite, nullptr);
	}

	return true;
}

bool CompareSprite(void *obj1, void *obj2) {
	Sprite *sp1 = (Sprite *)obj1;
	Sprite *sp2 = (Sprite *)obj2;

	if (sp1->channel > sp2->channel)
		return true;
	else
		return false;
}

bool ComparePSprite(void *obj1, void *obj2) {
	PartialSprite *sp1 = (PartialSprite *)obj1;
	PartialSprite *sp2 = (PartialSprite *)obj2;

	if (sp1->sprite->channel > sp2->sprite->channel)
		return true;
	else
		return false;
}

void FreeSpriteFn(void *obj) {
	Sprite *sprite = (Sprite *)obj;

	if (sprite->check != 99999)
		ADV_ASSERT(0, 0);

	if (sprite->port) {
		if (sprite->text) {
			FreePtr(sprite->text->text_string);
			FreePtr(sprite->text);
			sprite->text = nullptr;
			FreePort(sprite->port);
			sprite->port = nullptr;
		}
		if (sprite->state & SPRITE_DISPOSABLE) {
			if (sprite->colorIndex)
				FreePtr(sprite->colorIndex);
			FreePort(sprite->port);
			sprite->port = nullptr;
		}
	} else {
		ADV_ASSERT(0, 0);
	}
	if (sprite->scaleTableX) {
		FreePtr(sprite->scaleTableX);
		sprite->scaleTableX = nullptr;
	}
	if (sprite->scaleTableY) {
		FreePtr(sprite->scaleTableY);
		sprite->scaleTableY = nullptr;
	}
	if (sprite->glassTag) {
		FreePtr(sprite->glassTag);
		sprite->glassTag = nullptr;
	}
	if (sprite->drawList) {
		ArrayList_Free(sprite->drawList, nullptr);
		sprite->drawList = nullptr;
	}

	sprite->check = 0;
	FreePtr(sprite);
}

void RealizeSprites(Background *stage) {
	int i;
	Sprite *sprite;
	ArrayList *list = stage->spriteList;

	for (i = 0; i < list->size; i++) {
		sprite = (Sprite *)list->elements[i];

#ifdef __MADE_EXT
		/* DRAW THE CUSTOM SPRITE */
		if (sprite->drawFunc) {
			(*sprite->drawFunc)(stage->animPort, sprite, &sprite->rect);
			return;
		}
#endif

		/* DRAW SPRITE TO THE ANIMPORT */
		if (sprite->state & SPRITE_SCALED) {
			if (sprite->state & SPRITE_UNDERSCALED) {
				sprite->state &= ~SPRITE_UNDERSCALED;
			} else {
				if (sprite->state & SPRITE_MASK)
					DrawScaleSpriteMask(stage->animPort, sprite, nullptr);
				else
					DrawScaleSprite(stage->animPort, sprite, nullptr);
			}
		} else {
			DrawSpriteFlip(stage->animPort, sprite, nullptr, sprite->state, sprite->state);
		}
	}
}

void sfxSpriteAddToPic(Sprite *sprite) {
	Background *stage = sprite->back;

	/* DRAW SPRITE TO THE BACKPORT */
	if (sprite->state & SPRITE_SCALED) {
		if (sprite->state & SPRITE_UNDERSCALED) {
			sprite->state &= ~SPRITE_UNDERSCALED;
		} else {
			if (sprite->state & SPRITE_MASK)
				DrawScaleSpriteMask(stage->backPort, sprite, &sprite->rect);
			else
				DrawScaleSprite(stage->backPort, sprite, &sprite->rect);
		}
	} else {
		if (sprite->state & SPRITE_GLASS) {
			DrawGlassMask(stage->backPort, sprite, &sprite->rect);
		} else {
			DrawSpriteFlip(stage->backPort, sprite, &sprite->rect, sprite->state,
				sprite->state);
		}
	}

	sfxKillSprite(sprite);
}

void sfxSpriteGlass(Sprite *sprite, int type, int color, int intensity) {
	GlassTag *tag;

	if (sprite->glassTag == nullptr)
		tag = (GlassTag *)AllocPtr(sizeof(GlassTag));
	else
		tag = sprite->glassTag;

	tag->type = type;
	tag->color = color;
	tag->intensity = 31 * intensity / 100;

	sprite->glassTag = tag;
	sprite->state |= SPRITE_GLASS;
	sprite->changed = true;
	sprite->redraw = true;
}

int sfxDrawLine(Sprite *sprite, int x1, int y1, int x2, int y2, int color) {
	int i;
	ArrayList *list;
	DrawPrime *dp;

	/* LINE CLIPPING */
	if (!SCClipping(&x1, &y1, &x2, &y2, &sprite->rect))
		return (-1);

	dp = (DrawPrime *)AllocPtr(sizeof(DrawPrime));

	if (dp == nullptr)
		ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);

	/* CREATE DRAWLIST */
	if (sprite->drawList == nullptr) {
		sprite->drawList = ArrayList_Alloc();
		if (sprite->drawList == nullptr)
			ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);
	}

	/* ADD THE LINE */
	dp->sx = x1;
	dp->sy = y1;
	dp->ex = x2;
	dp->ey = y2;
	dp->color = color;
	ArrayList_Add(sprite->drawList, dp, nullptr);

	sprite->changed = true;

	/* RETURN THE LINE HANDLE */
	list = sprite->drawList;
	for (i = 0; i < list->size; i++) {
		if (list->elements[i] == dp) {
			dp->id = i;
			return i;
		}
	}

	return -1;
}

void sfxEraseLine(Sprite *sprite, int theLine) {
	int i;
	DrawPrime *dp = nullptr;
	ArrayList *list = sprite->drawList;

	for (i = 0; i < list->size; i++) {
		dp = (DrawPrime *)list->elements[i];
		if (theLine == dp->id)
			break;
	}

	if (sprite->drawList)
		ArrayList_Del(sprite->drawList, dp, nullptr);

	sprite->changed = true;
	if ((sprite->state & SPRITE_MASK) && !sprite->redraw) {
		sprite->redraw = true;
		SRect_Copy(&sprite->oldRect, &sprite->rect);
	}
}

void sfxSpriteFlipX(Sprite *sprite) {
	if (!sprite->redraw) {
		sprite->redraw = true;
		SRect_Copy(&sprite->oldRect, &sprite->rect);
	}

	if (sprite->state & SPRITE_FLIPX)
		sprite->state &= ~SPRITE_FLIPX;
	else
		sprite->state |= SPRITE_FLIPX;

	sprite->changed = true;
}

void sfxSpriteFlipY(Sprite *sprite) {
	if (!sprite->redraw) {
		sprite->redraw = true;
		SRect_Copy(&sprite->oldRect, &sprite->rect);
	}

	if (sprite->state & SPRITE_FLIPY)
		sprite->state &= ~SPRITE_FLIPY;
	else
		sprite->state |= SPRITE_FLIPY;

	sprite->changed = true;
}

void sfxSpriteFlipXY(Sprite *sprite) {
	if (!sprite->redraw) {
		sprite->redraw = true;
		SRect_Copy(&sprite->oldRect, &sprite->rect);
	}

	if (sprite->state & SPRITE_FLIPX)
		sprite->state &= ~SPRITE_FLIPX;
	else
		sprite->state |= SPRITE_FLIPX;

	if (sprite->state & SPRITE_FLIPY)
		sprite->state &= ~SPRITE_FLIPY;
	else
		sprite->state |= SPRITE_FLIPY;

	sprite->changed = true;
}

struct DrawInfo {
	int scaleX;
	int scaleY;
	int channel;
} ;

DrawInfo drawInfo = { SPRITE_CENTER_SCALEX, SPRITE_CENTER_SCALEY, -1 };

void sfxSetDrawInfo(int type, int value) {
	switch (type) {
	case ADV_DRAW_SCALEX:
		drawInfo.scaleX = value;
		break;

	case ADV_DRAW_SCALEY:
		drawInfo.scaleY = value;
		break;

	case ADV_DRAW_CHANNEL:
		drawInfo.channel = value;
		break;
	}
}

Sprite *sfxClipSprite(Sprite *sprite, int theX, int theY, SRect *theRect) {
	int needScale;
	Viewport *port;
	SRect aRect;
	int cw, ch;
	int l, t, r, b, w, h, sw = 0, sh = 0;
	Sprite *ret = AllocSprite(sprite->back, sprite->channel);

	if (ret) {
		l = theRect->left;
		t = theRect->top;
		r = theRect->right;
		b = theRect->bottom;

		/* CLIPPING OF RECTS */
		if (l < 0)
			l = 0;

		if (r > (sprite->port->width - 1))
			r = sprite->port->width - 1;

		if (t < 0)
			t = 0;

		if (b > (sprite->port->height - 1))
			b = sprite->port->height - 1;

		/* GET THE OVERALL DIMENSION */
		w = r - l + 1;
		h = b - t + 1;

		ret->x = theX;
		ret->y = theY;

		ret->rect.left = 0;
		ret->rect.top = 0;
		ret->rect.right = w - 1;
		ret->rect.bottom = h - 1;

		ret->rect.width = w;
		ret->rect.height = h;

		/*  COPY COLOR TABLE */
		if (sprite->colorIndex) {
			int *cTable = (int *)AllocPtr(256 * sizeof(int));
			ADV_ASSERT(cTable, __ERR_MEM_ALLOC_FAIL);
			memcpy(cTable, sprite->colorIndex, 256 * sizeof(int));
			ret->colorIndex = cTable;
		}

		if ((sprite->scaleX != SPRITE_CENTER_SCALEX) ||
			(sprite->scaleY != SPRITE_CENTER_SCALEY))
			return (nullptr);

		if ((drawInfo.scaleX != SPRITE_CENTER_SCALEX) ||
			(drawInfo.scaleY != SPRITE_CENTER_SCALEY))
		{
			cw = sprite->port->width - w;
			ch = sprite->port->height - h;
			needScale = true;
			sprite->scaleTableX = (uint *)AllocPtr(sizeof(int) * SCALE_TABLE_SIZE);
			sprite->scaleTableY = (uint *)AllocPtr(sizeof(int) * SCALE_TABLE_SIZE);
			sprite->state |= SPRITE_SCALE_TABLE;
			w = (w * drawInfo.scaleX) >> 9;
			h = (h * drawInfo.scaleY) >> 9;
			ret->rect.width = w;
			ret->rect.height = h;
			ret->rect.right = w - 1;
			ret->rect.bottom = h - 1;
			sw = ((sprite->port->width + cw) * drawInfo.scaleX) >> 9;
			sh = ((sprite->port->height + ch) * drawInfo.scaleY) >> 9;
		} else
			needScale = false;

		port = AllocPort(w, h, sprite->port->colors);
		ADV_ASSERT(port, __ERR_PORT_ALLOC_FAIL);
		port->skipColor = sprite->port->skipColor;

		ret->back = sprite->back;
		ret->changed = true;
		ret->port = port;

		if (needScale) {
			r = l + w - 1;
			b = t + h - 1;
			SRect_Init(&aRect, l, t, r, b);
			if (sprite->state & SPRITE_MASK)
				CopyScaleSpriteMask(port, sprite, &aRect, sw, sh);
			else
				CopyScaleSprite(port, sprite, &aRect, sw, sh);

			sprite->state &= ~SPRITE_SCALE_TABLE;
			if (sprite->scaleTableX) {
				FreePtr(sprite->scaleTableX);
				sprite->scaleTableX = nullptr;
			}

			if (sprite->scaleTableY) {
				FreePtr(sprite->scaleTableY);
				sprite->scaleTableY = nullptr;
			}
		} else {
			SRect_Init(&aRect, l, t, r, b);
			CopySpriteFlip(port, sprite, &aRect, sprite->state, sprite->state);
		}

		if (drawInfo.channel != -1)
			ret->channel = drawInfo.channel;
		else
			ret->channel = sprite->channel;

		ret->state = SPRITE_LOCALIZED | SPRITE_MASK |
			SPRITE_FIXEDPRI | SPRITE_DISPOSABLE;

		if (!(sprite->state & SPRITE_MASK))
			ret->state &= ~SPRITE_MASK;

		/* CLEAN UP DEFAULTS */
		drawInfo.scaleX = SPRITE_CENTER_SCALEX;
		drawInfo.scaleY = SPRITE_CENTER_SCALEY;
		drawInfo.channel = -1;

		sfxMoveSprite(ret, theX, theY);
	}

	return ret;
}

void Dissolve16(Viewport *dest, Viewport *src,
	int x1, int y1, int x2, int y2) {
	int i, j;
	int width, height;
	uint16 *lpSrc, *lpDest;
	static int dissolve_index = 0;
	int blocks[8][8] = {
	  {6,1,3,7,5,2,0,4}, {3,7,5,2,0,4,6,1}, {2,5,1,6,3,0,4,7},
	  {0,3,7,4,1,6,2,5}, {4,0,2,5,7,3,1,6}, {1,6,4,0,2,5,7,3},
	{5,2,6,1,4,7,3,0}, {7,4,0,3,6,1,5,2} };

	width = x2 - x1;
	ADV_ASSERT((width > 0), __ERR_CODING);
	height = y2 - y1;
	ADV_ASSERT((height > 0), __ERR_CODING);

	lpSrc = (uint16 *)src->ptr + x1 + y1 * src->width;
	lpDest = (uint16 *)dest->ptr + x1 + y1 * dest->width;

	for (j = 0; j < height; j++) {
		for (i = 0; i < 8; i++) {
			if (blocks[j % 8][i] == dissolve_index) {
				while (i < width) {
					lpDest[i] = lpSrc[i];
					i += 8;
				}
				break;
			}
		}
		lpDest += dest->width;
		lpSrc += src->width;
	}

	if (dissolve_index++ == 8)
		dissolve_index = 0;
}

void sfxDissolveSprite(Sprite *sprite, int delay) {
	int i;
	unsigned int oldTime;
	Viewport *animPort = sprite->back->animPort;
	Viewport *backPort = sprite->port;

	// NO REDRAW IS NECESSARY
	sprite->changed = false;
	sprite->redraw = false;

	for (i = 0; i < 8; i++) {
		Dissolve16(animPort, backPort, 0, 0, animPort->width, animPort->height);
		UpdateScreenRect(animPort, &sprite->rect);

		oldTime = sfxGetTime();
		while ((sfxGetTime() - oldTime) < (unsigned int)delay);
	}
}

void CleanSpriteList(Background *back) {
	int i;
	ArrayList *list = back->spriteList;

	for (i = 0; i < list->size; i++) {
		FreeSpriteFn(list->elements[i]);
		list->elements[i] = nullptr;
	}
	list->size = 0;
}

void CleanUpSprites() {
	int i;
	Sprite *sprite;
	Background *back = backgrounds[curBack];
	ArrayList *delList = back->deleteList;
	ArrayList *spriteList = back->spriteList;

	for (i = 0; i < spriteList->size; i++) {
		sprite = (Sprite *)spriteList->elements[i];
		if (sprite->destroy)
			ArrayList_Add(delList, sprite, nullptr);
	}

	for (i = 0; i < delList->size; i++) {
		sprite = (Sprite *)delList->elements[i];
		FreeSprite(back, sprite);
	}
	ArrayList_Release(delList);
}

} // namespace Spycraft
