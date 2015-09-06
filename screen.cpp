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
#include "aesop/shared.h"
#include "common/system.h"
#include "engines/util.h"
#include "graphics/palette.h"

namespace Aesop {

static byte F_fade[11][256];		// fixed      00-AF (also initializes B0-FF)
static byte W_fade[11][16];			// wallset    B0-BF
static byte M1_fade[11][32];		// monster #1 C0-DF
static byte M2_fade[11][32];		// monster #2 E0-FF
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

const int first_color[5] = { 0x00,0xb0,0xc0,0xe0,0xb0 };
const int num_colors[5] = { 256, 16, 32, 32, 80 };

byte *fade_tables[5][16] = { 
	{ F_fade[0],F_fade[1],F_fade[2],F_fade[3], F_fade[4],F_fade[5],F_fade[6],F_fade[7],
            F_fade[8],F_fade[9],F_fade[10], F_blu,F_grn,F_red,F_gry, NULL },

	{ W_fade[0],W_fade[1],W_fade[2],W_fade[3], W_fade[4],W_fade[5],W_fade[6],W_fade[7],
            W_fade[8],W_fade[9],W_fade[10], NULL,  NULL,  NULL,  NULL,  NULL },

	{ M1_fade[0],M1_fade[1],M1_fade[2],M1_fade[3], M1_fade[4],M1_fade[5],M1_fade[6],M1_fade[7],
            M1_fade[8],M1_fade[9],M1_fade[10], M1_gry, M1_wht, M1_grn, M1_blu, M1_brn },

	{ M2_fade[0],M2_fade[1],M2_fade[2],M2_fade[3], M2_fade[4], M2_fade[5], M2_fade[6], M2_fade[7],
            M2_fade[8], M2_fade[9], M2_fade[10], M2_gry, M2_wht, M2_grn, M2_blu, M2_brn },

	{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

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

byte blu_inten[BLU_NUM];
byte grn_inten[GRN_NUM];
byte gry_inten[GRY_NUM];
byte red_inten[RED_NUM];
byte brn_inten[BRN_NUM];

const byte text_colors[9] = { DK_GRN, LT_GRN, YEL, LT_RED, DK_RED, BLK, WHT, WHT, WHT };

/*----------------------------------------------------------------*/

Pane::Pane() : _window(nullptr), _owner(-1), Common::Rect() {
}

Pane::Pane(Window *window) : _owner(-1), Common::Rect() {
	_window = window;
	left = window->left;
	top = window->top;
	right = window->right;
	bottom = window->bottom;
}

Pane::Pane(Window *window, const Common::Rect &bounds) : _owner(-1), Common::Rect() {
	_window = window;
	left = bounds.left;
	top = bounds.top;
	right = bounds.right;
	bottom = bounds.bottom;
}

void Pane::clear() {
	_window = nullptr;
	left = top = right = bottom = 0;
	_owner = -1;
}

void Pane::paneCopy(Pane &src, const Common::Point &srcPos, const Common::Point &destPos, LONG fill) {
	warning("TODO: paneCopy");
}

int Pane::getLeft() const {
	return _window ? _window->left : 0;
}

int Pane::getTop() const {
	return _window ? _window->top : 0;
}

int Pane::getRight() const {
	return _window ? _window->right : 0;
}

int Pane::getBottom() const {
	return _window ? _window->bottom : 0;
}

void Pane::setLeft(int x1) {
	left = x1;
}

void Pane::setTop(int y1) {
	top = y1;
}

void Pane::setRight(int x2) {
	right = x2;
}

void Pane::setBottom(int y2) {
	bottom = y2;
}

void Pane::paneWipe(byte color) {
	fillRect(*this, color);
}

int Pane::colorScan(uint32 *colors) {
	error("VFX_color_scan");
}

byte Pane::readDot(const Common::Point &pt) {
	Common::Point p(pt.x - left, pt.y - top);
	return readDot(p);
}

void Pane::drawDot(const Common::Point &pt, byte color) {
	Common::Point p(pt.x - left, pt.y - top);

	_window->drawDot(p, color);
}

void Pane::drawLine(const Common::Point &src, const Common::Point &dest, byte color) {
	_window->drawLine(Common::Point(src.x - left, src.y - top),
		Common::Point(dest.x - left, dest.y - top), color);
}

void Pane::drawRect(const Common::Rect &r, byte color) {
	Common::Rect bounds = r;
	bounds.translate(-left, -top);
	_window->drawRect(bounds, color);
}

void Pane::fillRect(const Common::Rect &r, byte color) {
	Common::Rect bounds = r;
	bounds.translate(-left, -top);
	_window->fillRect(bounds, color);
}

void Pane::hashRect(const Common::Rect &r, byte color) {
	Common::Rect bounds = r;
	bounds.translate(-left, -top);
	_window->hashRect(bounds, color);
}

bool Pane::testOverlap(const Common::Point &pt, byte *shapes, int shape_num) {
	int x2 = (pt.x + (VFX_shape_resolution(shapes, shape_num) >> 16));
	int y2 = (pt.y + (VFX_shape_resolution(shapes, shape_num)));

	return ((pt.x <= right) && (x2 >= left) && (pt.y <= bottom) && (y2 >= top));
}

void Pane::scroll(const Common::Point &delta, PaneScroll flags, byte background) {
	error("TODO");
}

int Pane::drawCharacter(const Common::Point &pt, void *font, int character, byte *color_translate) {
	error("TODO");
}

/*----------------------------------------------------------------*/

Window::Window(): Pane() {
}

Window::Window(const Common::Rect &bounds): Pane(this) {
	// Copy passed bounds
	left = bounds.left;
	top = bounds.top;
	right = bounds.right;
	bottom = bounds.bottom;

	// Create the surface
	_surface.create(bounds.width(), bounds.height(), Graphics::PixelFormat::createFormatCLUT8());
}

void Window::clear() {
	Pane::clear();
	_surface.free();
}

void Window::wipeWindow(byte color) {
	paneWipe(color);
}

byte Window::readDot(const Common::Point &pt) {
	const byte *destP = (const byte *)_surface.getBasePtr(pt.x, pt.y);
	return *destP;
}

void Window::drawDot(const Common::Point &pt, byte color) {
	byte *destP = (byte *)_surface.getBasePtr(pt.x, pt.y);
	*destP = color;

	addDirtyRect(Common::Rect(pt.x, pt.y, pt.x, pt.y));
}

void Window::drawLine(const Common::Point &src, const Common::Point &dest, byte color) {
	error("TODO: drawLine");
}

void Window::drawRect(const Common::Rect &r, byte color) {
	_surface.hLine(r.left, r.top, r.right, color);
	_surface.hLine(r.left, r.bottom, r.right, color);
	_surface.vLine(r.left, r.top + 1, r.bottom, color);
	_surface.vLine(r.left, r.top + 1, r.bottom, color);

	addDirtyRect(r);
}

void Window::fillRect(const Common::Rect &r, byte color) {
	_surface.fillRect(Common::Rect(r.left, r.top, r.right + 1, r.bottom + 1), color);
	addDirtyRect(r);
}

void Window::hashRect(const Common::Rect &r, byte color) {
	error("TODO: hashRect");
}

void Window::refresh(const Common::Rect &r) {
	error("TODO: VFX_window_refresh");
}

void Window::fade(const byte *palette, int intervals) {
	error("TODO: VFX_window_fade");
}

/*----------------------------------------------------------------*/

int Font::charWidth(int c) {
	error("TODO");
}

/*----------------------------------------------------------------*/

TextWindow::TextWindow() {
	window = 0;
	htab = vtab = 0;
	font = (Font *)-1;
	delay = 0;
	continueFunction = nullptr;
	txtbuf = nullptr;
	txtpnt = nullptr;
	justify = J_LEFT;
	_refresh = 0;

	for (int idx = 0; idx < 256; ++idx)
		lookaside[idx] = idx;
}

void TextWindow::setPosition(const Common::Point &pt) {
	htab = pt.x;
	vtab = pt.y;
}

void TextWindow::home() {
	HRES hfont = (HRES)font;
	font = (Font *)Resources::addr(hfont);

	selectTextWindow();
	
	error("TODO");

	font = (Font *)hfont;
}

void TextWindow::textColor(byte current, byte newColor) {
	HRES hfont = (HRES)font;
	font = (Font *)Resources::addr(hfont);

	selectTextWindow();
	remapFontColor(current, newColor);

	font = (Font *)hfont;
}

void TextWindow::remapFontColor(byte current, byte newColor) {
	lookaside[current] = newColor;
}

void TextWindow::selectTextWindow() {
	_vm->_screen->_twptr = this;
}

void TextWindow::refreshWindow(int wnd) {
	_refresh = wnd;
}

uint TextWindow::charWidth(int ch) {
	HRES hfont;
	ULONG w;

	hfont = (HRES)font;
	font = (Font *)Resources::addr(hfont);

	selectTextWindow();

	error("TODO");

	font = (Font *)hfont;

	return w;
}


uint TextWindow::fontHeight() {
	HRES hfont = (HRES)font;

	return ((Font *)(Resources::addr(hfont)))->char_height;
}

void TextWindow::vsprint(const char *format, va_list argptr) {
	error("vsprint");
}

void TextWindow::print(const char *format, ...) {
	error("print");
}

void TextWindow::sprint(const char *format, ...) {
	error("sprint");
}

void TextWindow::crout() {
	sprint("%s", "\n");
}

/*----------------------------------------------------------------*/

Screen::Screen(AesopEngine *vm): Window(Common::Rect(0, 0, AESOP_SCREEN_WIDTH, AESOP_SCREEN_HEIGHT)), _vm(vm) {
	// Initialize the graphics mode
	initGraphics(320, 200, false);
	_bitmapBuffer.create(320, 200, Graphics::PixelFormat::createFormatCLUT8());
	_active = true;
	_lastPage = PAGE1;

	// Add the screen itself as the first window
	_windows.push_back(*this);
		
	// Set up a second page
	_windows.push_back(Window(Common::Rect(0, 0, AESOP_SCREEN_WIDTH, AESOP_SCREEN_HEIGHT)));

	_windows[PAGE2].wipeWindow(NO_COLOR);
	txtbuf[sizeof(txtbuf) - 1] = 0x69;    // constants for integrity checking
	txtbuf[sizeof(txtbuf) - 2] = 0x77;
}

Screen::~Screen() {
	for (uint idx = 0; idx < _windows.size(); ++idx)
		_windows[idx].clear();

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

void Screen::copyWindow(uint src, uint dest) {
	_panes[dest].paneCopy(_panes[src], Common::Point(), Common::Point(), NO_COLOR);
}

uint Screen::assignWindow(const Common::Rect &r) {
	for (uint idx = 0; idx < _windows.size(); ++idx) {
		if (_windows[idx].isEmpty()) {
			// Found a previously cleared window we can replace
			_windows.remove_at(idx);
			_windows.insert_at(idx, Window(r));

			return idx;
		}
	}

	// No freed windows so replace, so add new window at the end of the list
	assert(_windows.size() < MAX_WINDOWS);
	_windows.push_back(Window(r));
	return (int)_windows.size() - 1;
}

uint Screen::assignWindow(int owner, const Common::Rect &r) {
	uint win = assignWindow(r);
	_windows[win]._owner = owner;
	return win;
}

uint Screen::assignSubWindow(uint wnd, const Common::Rect &r) {
	for (uint idx = 0; idx < _panes.size(); ++idx) {
		if (_panes[idx].isEmpty()) {
			// Found a previously cleared window we can replace
			_panes.remove_at(idx);
			_panes.insert_at(idx, Pane(&_windows[wnd], r));

			return idx + MAX_WINDOWS;
		}
	}

	// No freed windows so replace, so add new window at the end of the list
	assert(_panes.size() < MAX_PANES);
	_panes.push_back(Pane(&_windows[wnd], r));
	return (int)_panes.size() - 1;

}

uint Screen::assignSubWindow(int owner, uint wnd, const Common::Rect &r) {
	uint win = assignSubWindow(wnd, r);
	_windows[win]._owner = owner;
	return win;
}

void Screen::releaseWindow(uint wnd) {
	// Clear the window
	_windows[wnd].clear();

	// Handle removing any cleared windows at the end of the array
	while (_windows.size() > 0 && _windows[_windows.size() - 1].isEmpty()) {
		_windows.remove_at(_windows.size() - 1);
	}
}


void Screen::releaseOwnedWindows(int owner) {
	if (owner == -1) {
		for (int idx = PAGE2 + 1; idx < (int)_windows.size(); ++idx) {
			if ((_windows[idx]._owner != -1) && (_windows[idx]._owner < NUM_ENTITIES)) {
				_windows[idx]._owner = -1;
				releaseWindow(idx);
			}
		}
	} else {
		for (int idx = PAGE2 + 1; idx < 256; ++idx) {
			if (_windows[idx]._owner == owner) {
				_windows[idx]._owner = -1;
				releaseWindow(idx);
			}
		}
	}
}

Pane &Screen::panes(uint idx) {
	return (idx < MAX_WINDOWS) ? _windows[idx] : _panes[idx - MAX_WINDOWS];
}

Window &Screen::windows(uint idx) {
	return _windows[idx];
}

TextWindow &Screen::textWindows(uint idx) {
	return _tw[idx];
}

void Screen::setPalette(uint region, uint resource) {
	Resources &res = *_vm->_resources;
	HRES handle;
	PAL_HDR *PHDR;
	const byte *rgbP;
	int idx, j, k, n, m, dm, d;
	const byte *fade;

	handle = res.get_resource_handle(resource, DA_DEFAULT);
	res.lock(handle);

	PHDR = (PAL_HDR *)res.addr(handle);

	if ((region == PAL_FIXED) || (region == PAL_WALLS) ||
			(region == PAL_M1) || (region == PAL_M2)) {
		for (int idx = 0; idx < 11; ++idx) {
			fade = (const byte *)add_ptr(PHDR, PHDR->fade[idx]);

			for (j = 0; j < PHDR->ncolors; ++j)
				fade_tables[region][idx][j] = first_color[region] + fade[j];
		}
	}

	rgbP = (const byte *)add_ptr(PHDR, PHDR->RGB);

	for (idx = 0; idx < PHDR->ncolors; ++idx)
		writePalette(idx + first_color[region], rgbP + idx * 3);

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
	Window &srcWin = _windows[srcWnd];
	Window &destWin = _windows[destWnd];

	int numColors = _windows[srcWnd].colorScan(colors);
	for (int idx = 0; idx < PALETTE_COUNT; ++idx)
		readPalette(idx, palette + idx * 3);

	// Set the colors
	byte pixel = _windows[destWnd].readDot(Common::Point(0, 0));
	rgb[0] = palette[pixel * 3];
	rgb[1] = palette[pixel * 3 + 2];
	rgb[2] = palette[pixel * 3 + 2];

	for (int idx = 0; idx < numColors; ++idx)
		writePalette(idx, rgb);

	destWin.paneCopy(srcWin, Common::Point(0, 0), Common::Point(0, 0), NO_COLOR);
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
	return _twptr->font->charWidth(ch);
}

void Screen::home(void) {
	_windows[_twptr->window].paneWipe(_twptr->font->font_background);

	_twptr->htab = _windows[_twptr->window].left;
	_twptr->vtab = _windows[_twptr->window].top;
}

void Screen::remapFontColor(byte current, byte newColor) {
	_twptr->remapFontColor(current, newColor);
}

void Screen::print(PrintOperation operation, const char *format, ...) {
	va_list  arglist;
	LONG     cw;

	va_start(arglist, format);

	if (operation == BUF) {
		cw = vsprintf(_twptr->txtbuf, format, arglist);
		_twptr->txtpnt = _twptr->txtbuf + cw;
	} else if (operation == APP) {
		cw = vsprintf(_twptr->txtpnt, format, arglist);
		_twptr->txtpnt += cw;
	}
}

void Screen::printBuffer(int linenum) {
	error("TODO: GIL2VFXA_print_buffer");
}

void Screen::cout(int c) {
	int cvtab, nvtab, htab;

	if (c == '\n') {
		htab = _twptr->htab = _windows[_twptr->window].left;    // Carriage Return

		cvtab = _twptr->vtab - _windows[_twptr->window].top;
		cvtab += _twptr->font->char_height;

		nvtab = cvtab + _twptr->font->char_height;

		if (nvtab > _windows[_twptr->window].bottom - _windows[_twptr->window].top)
		{
			if (_twptr->continueFunction != NULL)
			{
				if (((*_twptr->continueFunction)(_twptr->htab)) == 0)
				{
					_twptr->htab = htab;
					return;
				}
			}
			_twptr->htab = htab;

			_windows[_twptr->window].scroll(Common::Point(0, -_twptr->font->char_height), PS_NOWRAP, 
				_twptr->font->font_background);
		} else {
			_twptr->vtab += _twptr->font->char_height;
		}
	} else if (c == '\r') {
		_twptr->htab = _windows[_twptr->window].left;    // Carriage Return
	} else {
		_twptr->htab += _windows[_twptr->window].drawCharacter(
			Common::Point(_twptr->htab - _windows[_twptr->window].left, _twptr->vtab - _windows[_twptr->window].top),
			_twptr->font, c, _twptr->lookaside);
	}

}

void Screen::setTextWindow(uint wndNum, uint wnd) {
	_tw[wndNum].window = wnd;
}

void Screen::textStyle(uint wndnum, uint font, uint justify) {
	Resources &res = *_vm->_resources;
	HRES hfont;
	Font *vfont;

	_tw[wndnum].justify = justify;

	hfont = (HRES)_tw[wndnum].font;

	if (hfont != HRES_NULL)
	{
		res.unlock(hfont);
	}

	_tw[wndnum].font = (Font *)res.get_resource_handle(font, DA_DEFAULT);

	res.lock((HRES)_tw[wndnum].font);

	vfont = (Font *)res.addr((HRES)_tw[wndnum].font);

	_tw[wndnum].lookaside[vfont->font_background] = 255;
}


void Screen::drawDot(uint page, const Common::Point &pos, byte color) {
	_panes[page].drawDot(pos, color);

	_lastPage = page;
	_lastPos = pos;
}

void Screen::drawLine(uint page, const Common::Point &src, const Common::Point &dest, byte color) {
	_panes[page].drawLine(src, dest, color);
	_lastPage = page;
	_lastPos = dest;
}

void Screen::lineTo(Common::Array<Parameter> args) {
	for (uint i = 0; i < args.size(); i += 3) {
		Common::Point destPt(args[i * 3 + 0], args[i * 3 + 1]);
		byte cc = args[i * 3 + 2];

		_panes[_lastPage].drawLine(_lastPos, destPt, cc);
		_lastPos = destPt;
	}
}


void Screen::solidBarGraph(const Common::Rect &r, uint32 lb_border, uint32 tr_border, uint32 bkgnd,
		uint32 grn, uint32 yel, uint32 red, int val, int min, int crit, int max) {
	LONG btop, bbtm, blft, brgt, grayx; LONG range, point, width;
	LONG color;

	_windows[PAGE2].drawLine(Common::Point(r.left, r.top), Common::Point(r.left, r.bottom), lb_border);
	_windows[PAGE2].drawLine(Common::Point(r.left, r.bottom), Common::Point(r.right, r.bottom), lb_border);
	_windows[PAGE2].drawLine(Common::Point(r.right, r.bottom - 1), Common::Point(r.right, r.top), tr_border);
	_windows[PAGE2].drawLine(Common::Point(r.right, r.top), Common::Point(r.left + 1, r.top), tr_border);

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
		_windows[PAGE2].fillRect(Common::Rect(grayx, btop, brgt, bbtm), bkgnd);

	if (val <= crit)
		color = red;
	else
		color = (val * 3 >= max) ? grn : yel;

	if ((val != min) && (grayx == blft))
		grayx = blft + 1;

	if (grayx != blft)
		_windows[PAGE2].fillRect(Common::Rect(blft, btop, grayx, bbtm), color);
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

LONG GIL2VFX_get_bitmap_width(void *shape_table, LONG shape_num)
{
	return (VFX_shape_bounds(shape_table, shape_num) >> 16);
}

LONG GIL2VFX_get_bitmap_height(void *shape_table, LONG shape_num)
{
	return ((LONG)(WORD)VFX_shape_bounds(shape_table, shape_num));
}

void VFX_shape_visible_rectangle(void *shape_table,
	LONG shape_number, LONG hotX, LONG hotY,
	LONG mirror, LONG *rectangle) {
	error("TODO: VFX_shape_visible_rectangle");
}

void VFX_window_fade(Window *buffer, byte *palette, LONG intervals) {
	error("TODO");
}

void VFX_fixed_mul(FIXED16 M1, FIXED16 M2, FIXED16 *result) {
	error("TODO");
}

void VFX_shape_lookaside(UBYTE *table) {
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

LONG GIL2VFX_visible_bitmap_rect(LONG x1, LONG y1, LONG mirror,
	UBYTE *shapes, LONG shape_num, WORD *bounds) {
	LONG rectangle[4];
	LONG bm_width, bm_height;

	VFX_shape_visible_rectangle(shapes, shape_num, x1, y1, mirror, rectangle);

	if (mirror)
	{
		bm_width = VFX_shape_bounds(shapes, shape_num);
		bm_height = (LONG)(WORD)bm_width;
		bm_width = bm_width >> 16;
	}


	if (mirror & X_MIRROR)
	{
		bounds[0] = (WORD)(bm_width + rectangle[0]);
		bounds[2] = (WORD)(bm_width + rectangle[2]);
	}
	else
	{
		bounds[0] = (WORD)rectangle[0];
		bounds[2] = (WORD)rectangle[2];
	}

	if (mirror & Y_MIRROR)
	{
		bounds[1] = (WORD)(bm_height + rectangle[1]);
		bounds[3] = (WORD)(bm_height + rectangle[3]);
	}
	else
	{
		bounds[1] = (WORD)rectangle[1];
		bounds[3] = (WORD)rectangle[3];
	}

	Pane &pane0 = _vm->_screen->panes(0);
	bounds[0] = (bounds[0] > pane0.left) ? bounds[0] : (WORD)pane0.left;
	bounds[1] = (bounds[1] > pane0.top) ? bounds[1] : (WORD)pane0.top;

	bounds[2] = (bounds[2] < pane0.right) ? bounds[2] : (WORD)pane0.right;
	bounds[3] = (bounds[3] < pane0.bottom) ? bounds[3] : (WORD)pane0.bottom;

	if ((bounds[0] > bounds[2]) || (bounds[1] > bounds[3]))
		return 0;            // Nothing visible
	else
		return 1;
}

void GIL2VFX_draw_bitmap(int wnd, int x, int y, int mirror, int scale,
		byte *fade_table, byte *shapes, int shape_num) {
	Pane &pane = _vm->_screen->panes(wnd);
	int xp = x - pane.left;
	int yp = y - pane.top;
	int x_scale, y_scale;
	int flags;
	int xs, ys;

//	if (gil2vfx_active != true) return;

	if ((scale == 0) && (mirror == NO_MIRROR))
	{
		VFX_shape_draw(&pane, shapes, shape_num, xp, yp);
	}
	else
	{
		x_scale = ((scale) ? (scale << 8) : 0x10000);
		y_scale = ((scale) ? (scale << 8) : 0x10000);

		if (x_scale != 0x10000 || y_scale != 0x10000)
		{
			VFX_fixed_mul(VFX_shape_bounds(shapes, shape_num) & 0xffff0000,
				0x10000 - x_scale, &xs);
			VFX_fixed_mul(VFX_shape_bounds(shapes, shape_num) << 16,
				0x10000 - y_scale, &ys);

			if (mirror & X_MIRROR) xs = -xs;
			if (mirror & Y_MIRROR) ys = -ys;

			xp += xs >> 17;
			yp += ys >> 17;
		}

		switch (mirror)
		{
		case X_MIRROR:
			x_scale = -x_scale;
			xp += (VFX_shape_bounds(shapes, shape_num) >> 16);
			break;
		case Y_MIRROR:
			y_scale = -y_scale;
			yp += ((LONG)(WORD)VFX_shape_bounds(shapes, shape_num));
			break;
		case XY_MIRROR:
			x_scale = -x_scale;
			y_scale = -y_scale;
			xp += (VFX_shape_bounds(shapes, shape_num) >> 16) - 1;
			yp += ((LONG)(WORD)VFX_shape_bounds(shapes, shape_num)) - 1;
			break;
		case NO_MIRROR:
		default:
			break;
		}

		if (fade_table != NULL && scale != 0)
		{
			VFX_shape_lookaside(fade_table);
			flags = ST_XLAT;
		}
		else
			flags = 0;

		VFX_shape_transform(&pane, shapes, shape_num, xp, yp,
			_vm->_screen->_bitmapBuffer.getPixels(), 0, x_scale, y_scale, flags);
	}
}


void GIL2VFX_refresh_window(ULONG source, ULONG target)
{
	//mouse_pane_refresh(&panes[source], &panes[target]);
}

void VFX_shape_draw(Pane *pane, void *shape_table, LONG shape_number, LONG hotX, LONG hotY) {
	error("VFX_shape_draw");
}


} // End of namespace Aesop

