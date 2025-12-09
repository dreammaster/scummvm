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

#include "common/system.h"
#include "graphics/managed_surface.h"
#include "spycraft/spycraft.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/dirscreen.h"
#include "spycraft/dmade/winmade.h"
#include "spycraft/dmade/advrect.h"
#include "spycraft/dmade/advlist.h"
#include "spycraft/dmade/winport.h"
#include "spycraft/dmade/advback.h"
#include "spycraft/dmade/advbits.h"
#include "spycraft/vars.h"

namespace Spycraft {

#define DO_OPTIMIZE
#define DO_EDGECLIP
#define DO_STRIPING
#define DO_BOUNDS

#define _USEASM

static int have_set_dimensions = 0;

static int	Rect_EnlargeEdgeOverlap(SRect *r1, SRect *r2);
static int	Rect_ClipEdgeOverlap(SRect *r1, SRect *r2);

extern int offsetX;
extern int offsetY;

void sfxSetSpritelistSize(int listsize) {
	if (listsize >= DEFAULT_SPRITELIST_SIZE)
		spritelist_size = listsize;
}

void sfxSetGameDimensions(uint16 width, uint16 height, int listsize) {
	if (!have_set_dimensions) {
		scene_width = width;
		scene_height = height;
		spritelist_size = listsize;
		have_set_dimensions = 1;
	}
}

enum {
	__SFX_USE_555,
	__SFX_USE_565
};

typedef struct {
	Graphics::ManagedSurface *_surface;
	int _pixelFormat;
} DirectScreen;

int pixFormat = 0;
int dx_rowBytes;
DirectScreen *lpCRT = NULL;

unsigned short Format565P(unsigned short x) {
	return (x);
}

void Format565(Viewport *port) {
}

void *LockPtr() {
	return lpCRT->_surface->getPixels();
}

void UnlockPtr(void *p) {
	lpCRT->_surface->markAllDirty();
}

void InitScreen() {
	hGameDC = GetDC(hGameWnd);
	hSrcDC = CreateCompatibleDC(hGameDC);
	screen_colors = 16;

	have_set_dimensions = true;

	/* Init lpCRT structure */
	lpCRT = (DirectScreen *)AllocPtr(sizeof(DirectScreen));
	ADV_ASSERT(lpCRT, __ERR_WIN_API_FAIL);

	/* CREATE PRIMARY SURFACE */
	lpCRT->_surface = new Graphics::ManagedSurface(screen_width, screen_height,
		g_system->getScreenFormat());
	lpCRT->_pixelFormat = __SFX_USE_565;

	dx_rowBytes = lpCRT->_surface->pitch;

	/* PAINT INTIAL SCREEN TO BLACK */
	g_engine->_screen->clear();

	surfaceOK = true;
}

void CleanScreen() {
	ReleaseDC(hGameWnd, hGameDC);
	DeleteDC(hSrcDC);

	/* RELEASE DIRECT DRAW SURFACE */
	delete lpCRT->_surface;
}

void SetScreenPort(Viewport *port) {
	SelectObject(hSrcDC, ((WinViewport *)port)->hBitmap);
}

/* These three factors determine if it is better to blit a union of two	*/
/* rectangles, instead of bliting two rectangles.  If the number of		*/
/* pixels in the union is not MUCH-MORE-THAN sum of the pixels in the	*/
/* two original rectangles, then it is better to blit the union.		*/

#define	SETUP_TIME	(15400)
#define	LINE_TIME	(109)
#define 	PIXEL_TIME	(16)
int		setupT = SETUP_TIME;		// So external proc can set these
int		lineT = LINE_TIME;
int		pixelT = PIXEL_TIME;
#define BLITCOST(r) (setupT + ((r)->height)*(lineT + pixelT * ((r)->width)))

#ifdef DO_FILEDUMP //-----------------------------------------------------
#define TRACE_FILE_NAME	"c:\\trace"
HFILE	hTrace = HFILE_ERROR;
void OpenTraceFile() {
	if (hTrace == HFILE_ERROR)
	{
		if ((hTrace = _lopen(TRACE_FILE_NAME, OF_READWRITE)) != HFILE_ERROR)
		{
			_lclose(hTrace);
			DeleteFile(TRACE_FILE_NAME);
		}
		hTrace = _lcreat(TRACE_FILE_NAME, 0);
	}
}

void TraceRect(char *msg, int idx, SRect *r) {
	char	buffer[256];
	if (hTrace == HFILE_ERROR) return;
	if (*msg)
		sprintf(buffer, "%14s[%d]: %3d-%3d %3d-%3d\r\n", msg, idx,
			r->left, r->right, r->top, r->bottom);
	else
		sprintf(buffer, "\r\n");
	_lwrite(hTrace, buffer, strlen(buffer));
}
#define OPEN_TRACE_FILE() if (hTrace == HFILE_ERROR) OpenTraceFile();
#define TRACE_RECT(msg, idx, rect) TraceRect(msg, idx, rect);
#define DUMP_LIST {int j;for(j=0;j<list->size;j++){TRACE_RECT("DUMP",j,(SRect *)(list->elements)[j]);}}

#else
#define OPEN_TRACE_FILE()
#define TRACE_RECT(msg, idx, rect)
#define DUMP_LIST
#endif //DO_FILEDUMP -----------------------------------------------------

void AddBltRect(ArrayList *list, SRect *rect) {
	SRect	tmpRect, *r;
	int		rectCost, i;
#ifdef DO_FILEDUMP //-----------------------------------------------------
	static	int	nCalls;
#endif

#ifndef DO_OPTIMIZE
	OPEN_TRACE_FILE()
		TRACE_RECT("Next In", list->size, rect)
		ArrayList_Add(list, rect, NULL);
	return;
#endif

	/* If just starting with an empty list just add						*/

	if (list->size <= 0) {
		OPEN_TRACE_FILE()
			TRACE_RECT("......First In", nCalls = 0, rect)
			ArrayList_Add(list, rect, NULL);
		return;
	}

	TRACE_RECT("+++Next In", ++nCalls, rect)

		/* 0. See if new is fully contained inside old in the list			*/

		for (i = 0; i < list->size; i++)
		{
			r = (SRect *)(list->elements)[i];
			if (r->left <= rect->left && r->right >= rect->right &&
				r->top <= rect->top && r->bottom >= rect->bottom)
			{
				TRACE_RECT("Inside", i, r)
					return;
			}
		}

	/* 2. Remove (clip) strips fully contained in another (or new) rect	*/

#ifdef DO_EDGECLIP
	for (i = 0; i < list->size; i++) {
		r = (SRect *)(list->elements)[i];
		if (SRect_Intersects(r, rect))
			if (Rect_ClipEdgeOverlap(r, rect) ||
				Rect_ClipEdgeOverlap(rect, r))
			{
				TRACE_RECT("ClipTo", i, r)

					if (rect->width <= 0 || rect->height <= 0)
						return;

				if (r->width <= 0 || r->height <= 0)
				{
					TRACE_RECT("Unlink", i, r)
						ArrayList_Unlink(list, r);
				}
			}
	}
#endif //DO_EDGECLIP

	/* 3. Enlarge one, then remove strip from other so that result		*/
	/* 	  is two non-intersecting rects with smaller total area			*/

#ifdef DO_STRIPING
	for (i = 0; i < list->size; i++) {
		r = (SRect *)(list->elements)[i];
		if (SRect_Intersects(r, rect))
			if (Rect_EnlargeEdgeOverlap(r, rect)) {
				TRACE_RECT("StripeAgainst", i, r)
					TRACE_RECT("to create", nCalls, rect)
			}
	}
#endif //DO_STRIPING

	/* 4. Combine with an an existing rectangle into a larger one		*/
	/* (???? later on may want to try this against every rectangle and	*/
	/* pick the one with the smallest resulting union.					*/

	rectCost = BLITCOST(rect);
	for (i = 0; i < list->size; i++) {
		r = (SRect *)(list->elements)[i];
		SRect_Union(r, rect, &tmpRect);
		/* If new area not much larger then old, then resize old rect	*/
		if (rectCost + BLITCOST(r) >= BLITCOST(&tmpRect))
		{
			TRACE_RECT("Replace", i, r)
				TRACE_RECT("With", i, &tmpRect)
				r->left = tmpRect.left;
			r->right = tmpRect.right;
			r->top = tmpRect.top;
			r->bottom = tmpRect.bottom;
			r->width = tmpRect.width;
			r->height = tmpRect.height;
			return;
		}
	}

	/* Add what's left over from the original (or all) to the list		*/

	if (rect->width > 0 && rect->height > 0)
	{
		TRACE_RECT("AddToList", list->size, rect)
			ArrayList_Add(list, rect, NULL);
	}
}

static int	Rect_EnlargeEdgeOverlap(SRect *r1, SRect *r2) {
	/* Make one rectangle wider, then clip the other against it so that	*/
	/* the resulting two rectangles do not intersect but cover same 	*/
	/* the pixels as before												*/

	int	oldArea = r1->width * r1->height +
		r2->width * r2->height;
	int	leftArea, rightArea;
	SRect *rTmp;

	if (r2->top < r1->top)				/* Make r1 to be "above" r2		*/
	{
		rTmp = r1; r1 = r2; r2 = rTmp;
	}

	if (r1->top < r2->top &&			/* Don't use if edge overlap	*/
		r1->bottom > r2->top && r1->bottom < r2->bottom)
	{
		if (r1->left < r2->left &&
			r1->right > r2->left && r1->right < r2->right)
		{
			leftArea =
				r1->width * (r2->top - r1->top) +
				r2->height * (r2->right - r1->left + 1);
			rightArea =
				r1->height * (r2->right - r1->left + 1) +
				r2->width * (r2->bottom - r1->bottom);
			if (leftArea < rightArea && leftArea < oldArea)
			{
				/* EXTEND R2 LEFT	CLIP R1 ON BOTTOM						*/
				r2->left = r1->left;
				r2->width = r2->right - r2->left + 1;
				r1->bottom = r2->top - 1;
				r1->height = r1->bottom - r1->top + 1;
			} else if (rightArea < leftArea && rightArea < oldArea)
			{
				/* EXTEND R1 RIGHT CLIP R2 ON TOP						*/
				r1->right = r2->right;
				r1->width = r1->right - r1->left + 1;
				r2->top = r1->bottom + 1;
				r2->height = r2->bottom - r2->top + 1;
			}
		} else if (r1->right > r2->right &&
			r1->left > r2->left && r1->left < r2->right)
		{
			leftArea =
				r1->height * (r1->right - r2->left + 1) +
				r2->width * (r2->bottom - r1->bottom);
			rightArea =
				r1->width * (r2->top - r1->top) +
				r2->height * (r1->right - r2->left + 1);
			if (leftArea < rightArea && leftArea < oldArea)
			{
				/* EXTEND R1 LEFT	CLIP R2 ON TOP							*/
				r1->left = r2->left;
				r1->width = r1->right - r1->left + 1;
				r2->top = r1->bottom + 1;
				r2->height = r2->bottom - r2->top + 1;
			} else if (rightArea < leftArea && rightArea < oldArea)
			{
				/* EXTEND R2 RIGHT CLIP R1 ON BOTTOM					*/
				r2->right = r1->right;
				r2->width = r2->right - r2->left + 1;
				r1->bottom = r2->top - 1;
				r1->height = r1->bottom - r1->top + 1;
			}
		} else	// Should not happen
			return(0);
	}

	else		// Should not happen
		return(0);


	return(1);
}

static int	Rect_ClipEdgeOverlap(SRect *r1, SRect *r2) {
	/* If r2 is placed over exactly one edge of r1, then clip r2		*/

	if (r1->left <= r2->left && r1->right >= r2->right)	/* R2 on TOP	*/
	{
		if (r2->top <= r1->top &&
			r2->bottom <= r1->bottom && r2->bottom >= r1->top)
		{
			r2->bottom = r1->top - 1;
			r2->height = r2->bottom - r2->top + 1;
			if (r2->height < 0) r2->height = 0;
			return(1);
		}

		if (r2->bottom >= r1->bottom &&					/* R2 on BOTTOM	*/
			r2->top <= r1->bottom && r2->top >= r1->top)
		{
			r2->top = r1->bottom + 1;
			r2->height = r2->bottom - r2->top + 1;
			if (r2->height < 0) r2->height = 0;
			return(1);
		}
	}

	if (r1->top <= r2->top && r1->bottom >= r2->bottom)	/* R2 on LEFT	*/
	{
		if (r2->left <= r1->left &&
			r2->right <= r1->right && r2->right >= r1->left)
		{
			r2->right = r1->left - 1;
			r2->width = r2->right - r2->left + 1;
			if (r2->width < 0) r2->width = 0;
			return(1);
		}

		if (r2->right >= r1->right &&					/* R2 on RIGHT	*/
			r2->left >= r1->left && r2->left <= r1->right)
		{
			r2->left = r1->right + 1;
			r2->width = r2->right - r2->left + 1;
			if (r2->width < 0) r2->width = 0;
			return(1);
		}
	}
	return(0);
}

void UpdateScreen(Background *stage) {
	int i, totalCost;
	SRect *r, rBoundingBox, jBox;

	ArrayList *list = stage->updateRects;

	if (!list->size)
		return;

	if (list->size == 1)			/* If only one rect, draw it now	*/
	{
		BitBlt(hGameDC,
			((SRect *)(list->elements)[0])->left + offsetX,
			((SRect *)(list->elements)[0])->top + offsetY,
			((SRect *)(list->elements)[0])->width,
			((SRect *)(list->elements)[0])->height, hSrcDC,
			((SRect *)(list->elements)[0])->left,
			((SRect *)(list->elements)[0])->top, SRCCOPY);

		/*
		MDBlt ( ((SRect *)(list->elements)[0])->left+offsetX,
					((SRect *)(list->elements)[0])->top+offsetY,
					((SRect *)(list->elements)[0])->width,
					((SRect *)(list->elements)[0])->height,
					((SRect *)(list->elements)[0])->left,
					((SRect *)(list->elements)[0])->top );
		*/
		TRACE_RECT("BlitOne", 0, (SRect *)(list->elements)[0])
	}

	else							/* If more than one, traverse list	*/
	{
#ifdef DO_OPTIMIZE
#ifdef DO_BOUNDS
		/* Compute cost of blitting each rect separately				*/
		r = (SRect *)(list->elements)[0];
		rBoundingBox.top = r->top;
		rBoundingBox.left = r->left;
		rBoundingBox.bottom = r->bottom;
		rBoundingBox.right = r->right;
		rBoundingBox.width = r->width;
		rBoundingBox.height = r->height;
		totalCost = BLITCOST(r);
		for (i = 1; i < list->size; i++)
		{
			r = (SRect *)(list->elements)[i];
			totalCost += BLITCOST(r);
			SRect_Union(r, &rBoundingBox, &jBox);
		}

#endif //DO_BOUNDS
#endif //not DO_OPTIMIZE

		/* If cost of blitting bounding box is much larger cost of	*/
		/* blitting rects, then blit each rectangle separately. 	*/
		for (i = 0; i < list->size; i++) {
			r = (SRect *)(list->elements)[i];
#ifdef DO_ALIGN
			r->left = r->left & ~3;
			r->right = (r->right + 3) & ~3
#endif //DO_ALIGN

				BitBlt(hGameDC, r->left + offsetX, r->top + offsetY, r->width, r->height,
					hSrcDC, r->left, r->top, SRCCOPY);

			/*
			MDBlt ( r->left+offsetX, r->top+offsetY, r->width, r->height, r->left, r->top );
			*/
			TRACE_RECT("BlitNext", i, r);

		}
	}
	ArrayList_Release(list);
	TRACE_RECT("", 0, 0)
}

void UpdatePalette(GamePalette *palette) {
}

/*
void UpdateScreenRect ( Viewport *port, SRect *rect )
{
   MDBlt ( rect->left+offsetX, rect->top+offsetY, rect->width, rect->height,
		   rect->left, rect->top );
}

void UpdateScreenRectEx ( Viewport *port, SRect *rect )
{
   MDBlt ( rect->left, rect->top, rect->width, rect->height,
		   rect->left-offsetX, rect->top-offsetY );
}
*/

void AddScreen2Pic() {
	SRect rect;
	HBITMAP hbitmap;
	Background *back = backgrounds[curBack];
	WinViewport *vp = (WinViewport *)back->animPort;

	hbitmap = SelectObject(hSrcDC, vp->hBitmap);
	BitBlt(hSrcDC, 0, 16, 512, 384,
		hGameDC, offsetX, offsetY + 16, SRCCOPY);

	SelectObject(hSrcDC, hbitmap);
	SRect_Init(&rect, 0, 16, 511, 399);
	DrawBits(back->backPort, (Viewport *)vp, &rect);

}

void AddScreen2PicEx(int dx, int dy, int w, int h) {
	SRect rect;
	HBITMAP hbitmap;
	Background *back = backgrounds[curBack];
	WinViewport *vp = (WinViewport *)back->animPort;

	hbitmap = SelectObject(hSrcDC, vp->hBitmap);
	BitBlt(hSrcDC, dx, dy, w, h,
		hGameDC, offsetX + dx, offsetY + dy, SRCCOPY);

	SelectObject(hSrcDC, hbitmap);
	SRect_Init(&rect, dx, dx, dx + w - 1, dy + h - 1);
	DrawBits(back->backPort, (Viewport *)vp, &rect);
}

void UpdateScreenRect(Viewport *port, SRect *rect) {
	WinViewport *winPort;
	HBITMAP hBitmap;

	winPort = (WinViewport *)port;
	hBitmap = SelectObject(hSrcDC, winPort->hBitmap);
	BitBlt(hGameDC, rect->left + offsetX, rect->top + offsetY, rect->width, rect->height,
		hSrcDC, rect->left, rect->top, SRCCOPY);
	SelectObject(hSrcDC, hBitmap);
}

void UpdateScreenRectEx(Viewport *port, SRect *rect) {
	WinViewport *winPort;
	HBITMAP hBitmap;

	winPort = (WinViewport *)port;
	hBitmap = SelectObject(hSrcDC, winPort->hBitmap);
	BitBlt(hGameDC, rect->left, rect->top, rect->width, rect->height,
		hSrcDC, rect->left - offsetX, rect->top - offsetY, SRCCOPY);
	SelectObject(hSrcDC, hBitmap);
}

} // namespace Spycraft
