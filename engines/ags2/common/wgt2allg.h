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

#ifndef AGS2_COMMON_WGT2ALLG_H
#define AGS2_COMMON_WGT2ALLG_H

#include "ags2/lib/allegro.h"

namespace AGS2 {

#define _WGT45_
#define WINDOWS_VERSION

typedef BITMAP *block;
extern const char *spindexid;
extern const char *spindexfilename;

#if (WGTMAP_SIZE == 1)
typedef unsigned char *wgtmap;
#else
typedef int16 *wgtmap;
#endif

#define color RGB
#define TEXTFG    0
#define TEXTBG    1

#define fpos_t unsigned long

struct time {
	int ti_hund, ti_sec, ti_min, ti_hour;
};

#define is_ttf(fontptr)  (fontptr[0] == 'T')

#define GFX_VGA GFX_DIRECTX

extern void vga256();
extern void wsetscreen(block nss);
extern void wsetrgb(int coll, int r, int g, int b, color *pall);
extern int wloadpalette(char *filnam, color *pall);
extern void wcolrotate(unsigned char start, unsigned char finish, int dir, color *pall);
extern block wnewblock(int x1, int y1, int x2, int y2);
extern short getshort(Common::SeekableReadStream *fff);
extern void putshort(short num, Common::WriteStream *fff);
extern block wloadblock(const char *fill);
extern int wloadsprites(color *pall, char *filnam, block *sarray, int strt, int eend);
extern void wputblock(int xx, int yy, block bll, int xray);
extern void __my_setcolor(int *ctset, int newcol);
extern void wsetcolor(int nval);
extern int get_col8_lookup(int nval);
extern void wremap(color *pal1, block picc, color *pal2);
extern void wremapall(color *pal1, block picc, color *pal2);
extern void readheader();
extern void findfile(char *filnam);
extern int checkpassword(char *passw);

extern void gettime(struct time *tpt);
extern long wtimer(struct time tt1, struct time tt2);
extern void wcopyscreen(int x1, int y1, int x2, int y2, block src, int dx, int dy, block dest);

#define tx    abuf->cl
#define ty    abuf->ct
//#define bx abuf->cr   // can't do this because of REGS.bx
#define by    abuf->cb
#define kbdon key

#define installkbd()          install_keyboard()
#define uninstallkbd()        remove_keyboard()
#define wallocblock(wii,hii)  create_bitmap(wii,hii)
#define wbar(x1, y1, x2, y2)  rectfill(abuf, x1, y1, x2, y2, currentcolor)
#define wclip(x1, y1, x2, y2) set_clip(abuf, x1, y1, x2, y2)
#define wcls(coll)            clear_to_color(abuf,coll)

#define wfade_in(from, to, speed, pal)  fade_in_range(pal, 5 /* 64 - speed * 7 */, from, to)
#define wfade_out(from, to, speed, pal) fade_out_range(5, from, to)
#define wfastputpixel(x1, y1)           _putpixel(abuf, x1, y1, currentcolor)
#define wfreeblock(bll)                 destroy_bitmap(bll)
#define wgetblockheight(bll)            bll->h
#define wgetblockwidth(bll)             bll->w
#define wgetpixel(xx, yy)               getpixel(abuf, xx, yy)
#define whline(x1, x2, yy)              hline(abuf, x1, yy, x2, currentcolor)
#define wline(x1, y1, x2, y2)           line(abuf,x1,y1,x2,y2,currentcolor)
#define wloadpcx256(fnm,pall)           load_pcx( fnm, pall)
#define wnormscreen()                   abuf = screen
#define wputpixel(x1, y1)               putpixel(abuf, x1, y1, currentcolor)
#define wreadpalette(from, to, dd)      get_palette_range(dd, from, to)
#define wrectangle(x1, y1, x2, y2)      rect(abuf, x1, y1, x2, y2, currentcolor)
#define wregionfill(xx, yy)             floodfill(abuf, xx, yy, currentcolor)
#define wretrace()                      vsync()
#define setlib(lll)                     csetlib(lll, "")
#define wsetpalette(from, to, pall)     set_palette_range(pall, from, to, 0)
#define vgadetected()                   1

#ifndef WGT2ALLEGRO_NOFUNCTIONS
extern void wbutt(int x1, int y1, int x2, int y2);
#endif

// now define the wvesa_xxx to the normal names, since we use SVGA normally
#define wvesa_bar       wbar
#define wvesa_clip      wclip
#define wvesa_cls       wcls
#define wvesa_outtextxy wouttextxy
#define wvesa_rectangle wrectangle

#define XRAY    1
#define NORMAL  0

struct IMouseGetPosCallback {
public:
	virtual void AdjustPosition(int *x, int *y) = 0;
};

// Font/text rendering
extern void init_font_renderer();
extern void shutdown_font_renderer();
extern bool wloadfont_size(int fontNumber, int fontSize);
extern void wfreefont(int fontNumber);
extern void wouttextxy(int, int, int fontNumber, const char *);
extern void wgtprintf(int, int, int fontNumber, char *, ...);
extern int wgettextheight(const char *, int fontNumber);
extern int wgettextwidth(const char *, int fontNumber);
extern void wtextcolor(int);
extern int textcol;
extern int wtext_multiply;
extern void ensure_text_valid_for_font(char *text, int fontnum);
extern void adjust_y_coordinate_for_text(int *ypos, int fontnum);
extern void wtexttransparent(int);

} // namespace AGS2

#endif
