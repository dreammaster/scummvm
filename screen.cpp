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

#include "aesop/aesop.h"
#include "aesop/screen.h"
#include "aesop/shapes.h"
#include "aesop/shared.h"
#include "common/system.h"
#include "common/memstream.h"
#include "engines/util.h"
#include "graphics/palette.h"

namespace Aesop {

//
// Fixed-palette color ranges for magical auras, poison effects,
// petrification effects, damage effects, etc.
//

#define BLU_BEG 0x55
#define BLU_NUM 11
#define GRN_BEG 0x70
#define GRN_NUM 11
#define GRY_BEG 0x67
#define GRY_NUM 9
#define RED_BEG 0x22
#define RED_NUM 9
#define BRN_BEG 0x96
#define BRN_NUM 7
#define FIX_WHT 0x0b

#define  FADE_INTERVALS 20


static byte F_fade[11][256];        // fixed      00-AF (also initializes B0-FF)
static byte W_fade[11][16];         // wallset    B0-BF
static byte M1_fade[11][32];        // monster #1 C0-DF
static byte M2_fade[11][32];        // monster #2 E0-FF
static byte M1_gry[32];
static byte M1_wht[32];
static byte M2_gry[32];
static byte M2_wht[32];
static byte M1_blu[32];
static byte M2_blu[32];
static byte M1_grn[32];
static byte M2_grn[32];
static byte M1_brn[32];
static byte M2_brn[32];
static byte F_grn[256];
static byte F_blu[256];
static byte F_red[256];
static byte F_gry[256];

const int first_color[5] = { 0x00, 0xb0, 0xc0, 0xe0, 0xb0 };
const int num_colors[5] = { 256, 16, 32, 32, 80 };

byte *fade_tables[5][16] = {
	{
		F_fade[0], F_fade[1], F_fade[2], F_fade[3], F_fade[4], F_fade[5], F_fade[6], F_fade[7],
		F_fade[8], F_fade[9], F_fade[10], F_blu, F_grn, F_red, F_gry, NULL
	},

	{
		W_fade[0], W_fade[1], W_fade[2], W_fade[3], W_fade[4], W_fade[5], W_fade[6], W_fade[7],
		W_fade[8], W_fade[9], W_fade[10], NULL,  NULL,  NULL,  NULL,  NULL
	},

	{
		M1_fade[0], M1_fade[1], M1_fade[2], M1_fade[3], M1_fade[4], M1_fade[5], M1_fade[6], M1_fade[7],
		M1_fade[8], M1_fade[9], M1_fade[10], M1_gry, M1_wht, M1_grn, M1_blu, M1_brn
	},

	{
		M2_fade[0], M2_fade[1], M2_fade[2], M2_fade[3], M2_fade[4], M2_fade[5], M2_fade[6], M2_fade[7],
		M2_fade[8], M2_fade[9], M2_fade[10], M2_gry, M2_wht, M2_grn, M2_blu, M2_brn
	},

	{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

byte blu_inten[BLU_NUM];
byte grn_inten[GRN_NUM];
byte gry_inten[GRY_NUM];
byte red_inten[RED_NUM];
byte brn_inten[BRN_NUM];

const byte text_colors[9] = { DK_GRN, LT_GRN, YEL, LT_RED, DK_RED, BLK, WHT, WHT, WHT };

/*----------------------------------------------------------------*/

Screen::Screen(AesopEngine *vm): Window(Common::Point(AESOP_SCREEN_WIDTH, AESOP_SCREEN_HEIGHT)), _vm(vm) {
	// Initialize the graphics mode
	initGraphics(320, 200, false);
	_bitmapBuffer.create(320, 200, Graphics::PixelFormat::createFormatCLUT8());
	_active = true;
	_lastPage = PAGE1;

	// Add the screen itself as the first window
	_windows.push_back(this);

	// Set up a second page
	_windows.push_back(new Window(Common::Point(AESOP_SCREEN_WIDTH, AESOP_SCREEN_HEIGHT)));

	_windows[PAGE2]->wipeWindow(NO_COLOR);
	_txtBuf[sizeof(_txtBuf) - 1] = 0x69;    // constants for integrity checking
	_txtBuf[sizeof(_txtBuf) - 2] = 0x77;
}

Screen::~Screen() {
	// Delete all the windows except for the first, which is the screen itself, and is already being freed
	for (uint idx = 1; idx < _windows.size(); ++idx)
		delete _windows[idx];

	for (uint idx = 0; idx < _panes.size(); ++idx)
		delete _panes[idx];

	_bitmapBuffer.free();
}

void Screen::update() {
	// Merge the dirty rects
	mergeDirtyRects();

	// Loop through copying dirty areas to the physical screen
	Common::List<Common::Rect>::iterator i;
	for (i = _dirtyRects.begin(); i != _dirtyRects.end(); ++i) {
		const Common::Rect &r = *i;
		const byte *srcP = (const byte *)_surface.getBasePtr(r.left, r.top);
		g_system->copyRectToScreen(srcP, _surface.pitch, r.left, r.top,
		                           r.width(), r.height());
	}

	// Signal the physical screen to update
	g_system->updateScreen();
	_dirtyRects.clear();
}

void Screen::mergeDirtyRects() {
	Common::List<Common::Rect>::iterator rOuter, rInner;

	// Process the dirty rect list to find any rects to merge
	for (rOuter = _dirtyRects.begin(); rOuter != _dirtyRects.end(); ++rOuter) {
		rInner = rOuter;
		while (++rInner != _dirtyRects.end()) {

			if ((*rOuter).intersects(*rInner)) {
				// these two rectangles overlap or
				// are next to each other - merge them

				unionRectangle(*rOuter, *rOuter, *rInner);

				// remove the inner rect from the list
				_dirtyRects.erase(rInner);

				// move back to beginning of list
				rInner = rOuter;
			}
		}
	}
}

bool Screen::unionRectangle(Common::Rect &destRect, const Common::Rect &src1, const Common::Rect &src2) {
	destRect = src1;
	destRect.extend(src2);

	return !destRect.isEmpty();
}

void Screen::addDirtyRect(const Common::Rect &r) {
	_dirtyRects.push_back(r);
}

uint Screen::assignWindow(const Common::Rect &r) {
	assert(r.left == 0 && r.top == 0);

	for (uint idx = 0; idx < _windows.size(); ++idx) {
		if (_windows[idx] == nullptr) {
			// Found a previously cleared window we can replace
			_windows[idx] = new Window(Common::Point(r.right + 1, r.bottom + 1));

			return idx;
		}
	}

	// No freed windows so replace, so add new window at the end of the list
	assert(_windows.size() < MAX_WINDOWS);
	_windows.push_back(new Window(Common::Point(r.right + 1, r.bottom + 1)));
	return (int)_windows.size() - 1;
}

uint Screen::assignWindow(int owner, const Common::Rect &r) {
	uint win = assignWindow(r);
	_windows[win]->_owner = owner;
	return win;
}

uint Screen::assignSubWindow(uint wnd, const Common::Rect &r) {
	for (uint idx = 0; idx < _panes.size(); ++idx) {
		if (_panes[idx] == nullptr) {
			// Found a previously cleared window we can replace
			_panes[idx] = new Pane(_windows[wnd], r);

			return idx + MAX_WINDOWS;
		}
	}

	// No freed windows so replace, so add new window at the end of the list
	assert(_panes.size() < MAX_PANES);
	_panes.push_back(new Pane(_windows[wnd], r));
	return MAX_WINDOWS + (int)_panes.size() - 1;
}

uint Screen::assignSubWindow(int owner, uint wnd, const Common::Rect &r) {
	uint win = assignSubWindow(wnd, r);
	_panes[win - MAX_WINDOWS]->_owner = owner;
	return win;
}

void Screen::releaseWindow(uint wnd) {
	if (wnd < MAX_WINDOWS) {
		// Delete the window
		delete _windows[wnd];
		_windows[wnd] = nullptr;

		// Handle removing any cleared windows at the end of the array
		while (_windows.size() > 0 && _windows[_windows.size() - 1] == nullptr) {
			_windows.remove_at(_windows.size() - 1);
		}
	} else {
		// Delete the pane
		delete _panes[wnd - MAX_WINDOWS];
		_panes[wnd - MAX_WINDOWS] = nullptr;

		// Handle removing any cleared panes at the end of the array
		while (_panes.size() > 0 && _panes[_panes.size() - 1] == nullptr) {
			_panes.remove_at(_panes.size() - 1);
		}
	}
}


void Screen::releaseOwnedWindows(int owner) {
	if (owner == -1) {
		for (int idx = PAGE2 + 1; idx < (int)_windows.size(); ++idx) {
			if ((_windows[idx]->_owner != -1) && (_windows[idx]->_owner < NUM_ENTITIES)) {
				_windows[idx]->_owner = -1;
				releaseWindow(idx);
			}
		}
	} else {
		for (int idx = PAGE2 + 1; idx < (int)_windows.size(); ++idx) {
			if (_windows[idx]->_owner == owner) {
				_windows[idx]->_owner = -1;
				releaseWindow(idx);
			}
		}
	}
}

void Screen::refreshWindow(uint src, uint target) {
	// Original seems only to have called a mouse cursor refresh here, so it's not needed
}

Pane &Screen::panes(uint idx) {
	return (idx < MAX_WINDOWS) ? *_windows[idx] : *_panes[idx - MAX_WINDOWS];
}

Window &Screen::windows(uint idx) {
	return *_windows[idx];
}

TextWindow &Screen::textWindows(uint idx) {
	assert(idx < MAX_TEXT_WINDOWS);
	return _textWindows[idx];
}

void Screen::setPalette(uint region, uint resource) {
	Resources &res = *_vm->_resources;
	HRES handle;
	PAL_RES pal;
	int idx, j, k, n, m, dm, d;
	const byte *fade;

	handle = res.get_resource_handle(resource, DA_DEFAULT);
	res.lock(handle);

	Common::MemoryReadStream stream((const byte *)res.addr(handle), res.size(handle));
	pal.load(stream);

	if ((region == PAL_FIXED) || (region == PAL_WALLS) ||
	        (region == PAL_M1) || (region == PAL_M2)) {
		for (int idx = 0; idx < 11; ++idx) {
			fade = &pal._fade[idx][0];

			for (j = 0; j < pal._nColors; ++j)
				fade_tables[region][idx][j] = first_color[region] + fade[j];
		}
	}

	const byte *rgbP = &pal._rgb[0];

	for (idx = 0; idx < pal._nColors; ++idx)
		writePalette(idx + first_color[region], &pal._rgb[idx * 3]);

	switch (region) {
	case PAL_FIXED:
		for (n = 0, idx = BLU_BEG; n < BLU_NUM; n++, ++idx)
			blu_inten[n] = *(rgbP + idx * 3) + *(rgbP + idx * 3 + 1) + *(rgbP + idx * 3 + 2);
		for (n = 0, idx = RED_BEG; n < RED_NUM; n++, ++idx)
			red_inten[n] = *(rgbP + idx * 3) + *(rgbP + idx * 3 + 1) + *(rgbP + idx * 3 + 2);
		for (n = 0, idx = GRN_BEG; n < GRN_NUM; n++, ++idx)
			grn_inten[n] = *(rgbP + idx * 3) + *(rgbP + idx * 3 + 1) + *(rgbP + idx * 3 + 2);
		for (n = 0, idx = GRY_BEG; n < GRY_NUM; n++, ++idx)
			gry_inten[n] = *(rgbP + idx * 3) + *(rgbP + idx * 3 + 1) + *(rgbP + idx * 3 + 2);
		for (n = 0, idx = BRN_BEG; n < BRN_NUM; n++, ++idx)
			brn_inten[n] = *(rgbP + idx * 3) + *(rgbP + idx * 3 + 1) + *(rgbP + idx * 3 + 2);

		// f = first_color[region];
		n = num_colors[region];
		for (idx = 0; idx < n; ++idx) {
			j = *(rgbP + idx * 3) + *(rgbP + idx * 3 + 1) + *(rgbP + idx * 3 + 2);

			m = 0;
			dm = 32767;
			for (k = 0; k < BLU_NUM; ++k) {
				d = ABS(j - blu_inten[k]);
				if (d < dm) {
					dm = d;
					m = k;
				}
			}

			F_blu[idx] = BLU_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < GRN_NUM; ++k) {
				d = ABS(j - grn_inten[k]);
				if (d < dm) {
					dm = d;
					m = k;
				}
			}

			F_grn[idx] = GRN_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < RED_NUM; ++k) {
				d = ABS(j - red_inten[k]);
				if (d < dm) {
					dm = d;
					m = k;
				}
			}

			F_red[idx] = RED_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < GRY_NUM; k++) {
				d = abs(j - gry_inten[k]);
				if (d < dm) {
					dm = d;
					m = k;
				}
			}

			F_gry[idx] = GRY_BEG + m;
		}
		break;

	case PAL_M1:
	case PAL_M2:
		n = num_colors[region];
		for (idx = 0; idx < n; ++idx) {
			j = *(rgbP + idx * 3) + *(rgbP + idx * 3 + 1) + *(rgbP + idx * 3 + 2);

			m = 0;
			dm = 32767;
			for (k = 0; k < BRN_NUM; ++k) {
				d = abs(j - brn_inten[k]);
				if (d < dm) {
					dm = d;
					m = k;
				}
			}

			if (region == PAL_M1)
				M1_brn[idx] = BRN_BEG + m;
			else
				M2_brn[idx] = BRN_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < GRY_NUM; ++k) {
				d = ABS(j - gry_inten[k]);
				if (d < dm) {
					dm = d;
					m = k;
				}
			}

			if (region == PAL_M1)
				M1_gry[idx] = GRY_BEG + m;
			else
				M2_gry[idx] = GRY_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < GRN_NUM; ++k) {
				d = ABS(j - grn_inten[k]);
				if (d < dm) {
					dm = d;
					m = k;
				}
			}

			if (region == PAL_M1)
				M1_grn[idx] = GRN_BEG + m;
			else
				M2_grn[idx] = GRN_BEG + m;

			m = 0;
			dm = 32767;
			for (k = 0; k < BLU_NUM; ++k) {
				d = abs(j - blu_inten[k]);
				if (d < dm) {
					dm = d;
					m = k;
				}
			}

			if (region == PAL_M1)
				M1_blu[idx] = BLU_BEG + m;
			else
				M2_blu[idx] = BLU_BEG + m;

			if (region == PAL_M1)
				M1_wht[idx] = FIX_WHT;
			else
				M2_wht[idx] = FIX_WHT;
		}
		break;
	}

	res.unlock(handle);
	update();
}

void Screen::writePalette(byte index, const byte *rgb) {
	g_system->getPaletteManager()->setPalette(rgb, index, 1);
}

void Screen::writePalette(byte index, uint value) {
	byte rgb[3];

	rgb[0] = value >> 12;
	rgb[1] = (value >> 6) & 63;
	rgb[2] = value & 63;

	writePalette(index, rgb);
}

void Screen::readPalette(byte index, byte *rgb) {
	g_system->getPaletteManager()->grabPalette(rgb, index, 1);
}

uint Screen::readPalette(byte index) {
	byte rgb[3];
	readPalette(index, rgb);
	return ((uint)rgb[0] << 12) | ((ULONG)rgb[1] << 6) | ((ULONG)rgb[2]);
}

void Screen::pixelFade(int srcWnd, int destWnd, int intervals) {
	error("TODO: pixelFade");
}

void Screen::colorFade(int srcWnd, int destWnd) {
	byte palette[PALETTE_SIZE], rgb[3];
	uint32 colors[PALETTE_COUNT];
	Window &srcWin = *_windows[srcWnd];
	Window &destWin = *_windows[destWnd];

	int numColors = srcWin.colorScan(colors);
	for (int idx = 0; idx < PALETTE_COUNT; ++idx)
		readPalette(idx, palette + idx * 3);

	// Set all the colors used by the screen to the current background color,
	// with the background palette index being chosen by reading the first pixel
	byte pixel = destWin.readDot(Common::Point(0, 0));
	rgb[0] = palette[pixel * 3];
	rgb[1] = palette[pixel * 3 + 2];
	rgb[2] = palette[pixel * 3 + 2];

	for (int idx = 0; idx < numColors; ++idx)
		writePalette(colors[idx], rgb);

	destWin.blitFrom(srcWin);
	destWin.refresh(Common::Rect(0, 0, AESOP_SCREEN_WIDTH - 1, AESOP_SCREEN_HEIGHT - 1));
	destWin.fade(palette, FADE_INTERVALS);
}

void Screen::lightFade(uint srcWnd, byte color) {
	byte palette[PALETTE_SIZE], rgb[3];
	readPalette(color, rgb);

	for (int idx = 0; idx < PALETTE_COUNT; ++idx) {
		palette[idx * 3] = rgb[0];
		palette[idx * 3 + 1] = rgb[1];
		palette[idx * 3 + 2] = rgb[2];
	}

	panes(srcWnd)._window->fade(palette, FADE_INTERVALS);
}

void Screen::selectTextWindow(TextWindow *tw) {
	_twptr = tw;
}

int Screen::charWidth(int ch) {
	return _twptr->_font.charWidth(ch);
}

void Screen::home(void) {
	panes(_twptr->_window).paneWipe(_twptr->_font._fontBackground);
	_twptr->_hTab = panes(_twptr->_window).left;
	_twptr->_vTab = panes(_twptr->_window).top;
}

void Screen::remapFontColor(byte current, byte newColor) {
	_twptr->remapFontColor(current, newColor);
}

void Screen::setTextWindow(uint wndNum, uint wnd) {
	_textWindows[wndNum]._window = wnd;
}

void Screen::textStyle(uint wndnum, uint font, uint justify) {
	Resources &res = *_vm->_resources;
	TextWindow &tw = _textWindows[wndnum];

	tw._justify = (Justification)justify;
	tw._font.clear();

	HRES handle = res.get_resource_handle(font, DA_DEFAULT);
	Common::MemoryReadStream stream((const byte *)Resources::addr(handle), Resources::size(handle));
	tw._font.load(stream);
	res.free(handle);

	tw._lookaside[tw._font._fontBackground] = 255;
}


void Screen::drawDot(uint page, const Common::Point &pos, byte color) {
	_panes[page]->drawDot(pos, color);

	_lastPage = page;
	_lastPos = pos;
}

void Screen::drawLine(uint page, const Common::Point &src, const Common::Point &dest, byte color) {
	_panes[page]->drawLine(src, dest, color);
	_lastPage = page;
	_lastPos = dest;
}

void Screen::lineTo(Common::Array<Parameter> args) {
	for (uint i = 0; i < args.size(); i += 3) {
		Common::Point destPt(args[i * 3 + 0], args[i * 3 + 1]);
		byte cc = args[i * 3 + 2];

		_panes[_lastPage]->drawLine(_lastPos, destPt, cc);
		_lastPos = destPt;
	}
}

void Screen::solidBarGraph(const Common::Rect &r, uint32 lb_border, uint32 tr_border, uint32 bkgnd,
                           uint32 grn, uint32 yel, uint32 red, int val, int min, int crit, int max) {
	LONG btop, bbtm, blft, brgt, grayx;
	LONG range, point, width;
	LONG color;

	_windows[PAGE2]->drawLine(Common::Point(r.left, r.top), Common::Point(r.left, r.bottom), lb_border);
	_windows[PAGE2]->drawLine(Common::Point(r.left, r.bottom), Common::Point(r.right, r.bottom), lb_border);
	_windows[PAGE2]->drawLine(Common::Point(r.right, r.bottom - 1), Common::Point(r.right, r.top), tr_border);
	_windows[PAGE2]->drawLine(Common::Point(r.right, r.top), Common::Point(r.left + 1, r.top), tr_border);

	btop = r.top + 1;
	bbtm = r.bottom - 1;

	blft = r.left + 1;
	brgt = r.right - 1;

	width = brgt - blft;

	if (val > max)
		val = max;
	else if (val < min)
		val = min;

	range = max - min;
	point = val - min;

	grayx = blft + (point * width / range);

	if (grayx != brgt)
		_windows[PAGE2]->fillRect(Common::Rect(grayx, btop, brgt, bbtm), bkgnd);

	if (val <= crit)
		color = red;
	else
		color = (val * 3 >= max) ? grn : yel;

	if ((val != min) && (grayx == blft))
		grayx = blft + 1;

	if (grayx != blft)
		_windows[PAGE2]->fillRect(Common::Rect(blft, btop, grayx, bbtm), color);
}

void Screen::drawBitmap(int page, uint table, int number, const Common::Point &pt,
	uint scale, uint flip, uint fadeTableNum, uint fadeLevelNum) {
	Resources &res = *_vm->_resources;
	HRES handle;
	static HRES last_handle = 0;
	static ULONG last_table;
	UBYTE *lookaside;

	if (table == last_table)
		handle = last_handle;
	else {
		handle = res.get_resource_handle(table, DA_DEFAULT);
		last_handle = handle;
		last_table = table;
	}

	if ((fadeLevelNum > 10) && (!scale))
		scale = 256;

	lookaside = fade_tables[fadeTableNum][fadeLevelNum] - first_color[fadeTableNum];

	res.lock(handle);

	Shapes shapes((const byte *)res.addr(handle));
	shapes.drawBitmap(page, pt, flip, scale, lookaside, number);

	res.unlock(handle);
}

/*----------------------------------------------------------------*/


void GIL2VFXA_print_buffer(Pane *pane, LONG line_number) {
	error("TODO: Convert ASM to CPP");
}

void GIL2VFX_hook_INT9(void) {
	error("TODO: Convert ASM to CPP");
}

void GIL2VFX_unhook_INT9(void) {
	error("TODO: Convert ASM to CPP");
}

LONG VFX_shape_bounds(void *shape_table, LONG shape_num) {
	return 0;
}

LONG GIL2VFX_get_bitmap_width(void *shape_table, LONG shape_num) {
	return (VFX_shape_bounds(shape_table, shape_num) >> 16);
}

LONG GIL2VFX_get_bitmap_height(void *shape_table, LONG shape_num) {
	return ((LONG)(WORD)VFX_shape_bounds(shape_table, shape_num));
}

void VFX_window_fade(Window *buffer, byte *palette, LONG intervals) {
	error("TODO");
}

void VFX_shape_transform(Pane *pane,
                         void *shape_table, LONG shape_number, LONG hotX, LONG hotY,
                         void *buffer, LONG rot, LONG x_scale, LONG y_scale, LONG flags) {
	error("TODO");
}

LONG VFX_shape_resolution(void *shape_table, LONG shape_num) {
	error("TODO");
}

int VFX_shape_resolution(byte *data, int shape_num) {
	error("TODO");
}

void GIL2VFX_refresh_window(ULONG source, ULONG target) {
	//mouse_pane_refresh(&panes[source], &panes[target]);
}

void VFX_shape_draw(Pane *pane, void *shape_table, LONG shape_number, LONG hotX, LONG hotY) {
	error("VFX_shape_draw");
}


} // End of namespace Aesop

