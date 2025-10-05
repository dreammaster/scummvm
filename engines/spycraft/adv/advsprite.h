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

#ifndef SPYCRAFT_ADVSPRITE_H
#define SPYCRAFT_ADVSPRITE_H

#include "spycraft/adv/advrect.h"
#include "spycraft/adv/advport.h"
#include "spycraft/adv/advback.h"

namespace Spycraft {

#define SCALE_TABLE_SIZE		1024
#define MK_COLOR16(R,G,B)		((R<<10)|(G<<5)|B)

/* DEFINES FOR SPRITES */
enum {
	TYPE_SPRITE,
	TYPE_MOVIE,
	TYPE_TEXT
};

/* DEFINES FOR SCALER */
#define SPRITE_CENTER_SCALEX	512
#define SPRITE_CENTER_SCALEY	512

/* DEFINES FOR GLASS SPRITES */
enum {
	SPRITE_GLASS_GRAY,
	SPRITE_GLASS_TINT,
	SPRITE_GLASS_LIGHT
};

/* DEFINES FOR DRAW INFO */
enum {
	ADV_DRAW_SCALEX,
	ADV_DRAW_SCALEY,
	ADV_DRAW_CHANNEL
};

/* DEFINES FOR SPRITE STATE */
#define SPRITE_LOCALIZED		0x80000000
#define SPRITE_MASK				0x40000000
#define SPRITE_SCALED			0x20000000
#define SPRITE_FLIPX				0x10000000
#define SPRITE_FLIPY				0x08000000
#define SPRITE_GLASS				0x04000000
#define SPRITE_FIXEDPRI			0x02000000
#define SPRITE_UNDERSCALED		0x01000000
#define SPRITE_HIDDEN			0x00800000
#define SPRITE_SCALE_TABLE		0x00400000
#define SPRITE_CUSTOM			0x00200000
#define SPRITE_DISPOSABLE		0x00100000

typedef struct spriteTAG *SpritePtr;

typedef void (*PFDraw)(Viewport *, SpritePtr, SRect *);

struct GlassTag {
	int type = 0;
	int color = 0;
	int intensity = 0;
};

struct TextType {
	char *text_string = nullptr;
	uint32 textcolor = 0;
	uint32 textstyle = 0;
	int16  pointsize = 0;
	int16  fontID = 0;
};

struct DrawPrime {
	int id = 0;
	int sx = 0;
	int sy = 0;
	int ex = 0;
	int ey = 0;
	int color = 0;
};

struct Sprite {
	int check = 0;
	int x = 0;						/* LOCATION OF SPRITE */
	int y = 0;
	int orig_x = 0;					/* REFERENCE POINT X */
	int orig_y = 0;					/* REFERENCE POINT Y */
	uint16 scaleX = 0;				/* FOR SCALING AT X-AXIS */
	uint16 scaleY = 0;				/* FOR SCALING AT Y-AXIS */
	bool changed = false;			/* USEFUL FOR RELEASE SPRITE CALL */
	bool redraw = false;			/* USEFUL FOR RESTORE BACKGROUND */
	bool destroy = false;			/* USEFUL FOR CLEAN UP DELETED SPRITE */
	uint16 channel = 0;
	uint32 state = 0;
	SRect rect;						/* CURRENT SEEN RECT */
	SRect oldRect;					/* PREVIOUS SEEN RECT */
	int skipColor;
	uint *scaleTableX = nullptr;		/* RESERVED FOR SCALING */
	uint *scaleTableY = nullptr;
	GlassTag *glassTag = nullptr;	/* TRANSLUENT EFFECT */
	TextType *text = nullptr;		/* RESERVED FOR TEXT */
	Background *back = nullptr;		/* WHICH BACKGROUND ATTACHED TO */
	PFDraw drawFunc = nullptr;		/* MADE EXTENSION */
	ArrayList *drawList = nullptr;	/* DRAW PRIMITIVES */
	int *colorIndex = nullptr;		/* REDUCED COLOR INDEX */
	Viewport *port = nullptr;
};

struct PartialSprite {
	Sprite *sprite = nullptr;
	SRect rect;
};

/* ANIMATION SYSTEM FUNCTION (EXTERNAL) */
extern void sfxUpdate(void);
extern void sfxReleaseSprites(Background *stage);
extern void sfxHideSprite(Sprite *sprite);
extern void sfxShowSprite(Sprite *sprite);
extern void sfxKillSprite(Sprite *sprite);
extern void sfxMoveSprite(Sprite *sprite, int theX, int theY);
extern void sfxSpriteOrig(Sprite *sprite, int theX, int theY);
extern void sfxSpriteChannel(Sprite *sprite, uint16 theChannel);
extern void sfxReleasePriority(Sprite *sprite);
extern int sfxIsSkip(Sprite *sprite, int theX, int theY);
extern void sfxSpriteNoMask(Sprite *sprite);
extern void sfxSpriteMask(Sprite *sprite);
extern int sfxGetSpritePixel(Sprite *sprite, int theX, int theY);
extern Sprite *sfxCreateSprite(int theBack, int theX, int theY, uint16 theScaleX, uint16 theScaleY, Viewport *port);
extern void sfxSpriteScale(Sprite *sprite, uint16 theScaleX, uint16 theScaleY);
extern void sfxInstallSpriteFn(Sprite *sprite, PFDraw);
extern void sfxRemoveSpriteFn(Sprite *sprite);
extern void sfxSpriteFrame(Sprite *sprite, Viewport *thePort);
extern void sfxSpriteAddToPic(Sprite *sprite);
extern void sfxSpriteGlass(Sprite *sprite, int type, int color, int intensity);
extern int sfxDrawLine(Sprite *sprite, int x1, int y1, int x2, int y2, int color);
extern void sfxEraseLine(Sprite *sprite, int theLine);
extern void sfxAttachATS2Sprite(Sprite *sprite, ATS *theAts, int theLoop, int theCel);
extern void sfxSpriteFlipX(Sprite *sprite);
extern void sfxSpriteFlipY(Sprite *sprite);
extern void sfxSpriteFlipXY(Sprite *sprite);
extern void sfxSetDrawInfo(int type, int value);
extern void sfxDissolveSprite(Sprite *sprite, int delay);
extern Sprite *sfxClipSprite(Sprite *sprite, int theX, int theY, SRect *theRect);

/* ANIMATION SYSTEM FUNCTION (INTERNAL) */
extern void RealizeSprites(Background *stage);
extern Sprite *AllocSprite(Background *stage, uint16 theChannel);
extern void DirectRelease(Background *stage, int dx, int dy);
extern void UpdateSprites(Background *stage);
extern bool FreeSprite(Background *stage, Sprite *theSprite);
extern void FreeSpriteFn(void *obj);
extern bool CompareSprite(void *obj1, void *obj2);
extern bool ComparePSprite(void *obj1, void *obj2);
extern void AddBackRect(ArrayList *list, Sprite *sprite);
extern void CleanSpriteList(Background *back);

} // namespace Spycraft

#endif
