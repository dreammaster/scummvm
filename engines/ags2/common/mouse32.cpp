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
#include "ags2/common/mouse32.h"
#include "ags2/vars.h"

namespace AGS2 {

static const int MB_ARRAY[3] = { 1,2,4 };
static const int BLUE_ = 0x7100, GREEN_ = 0x7200, GR_BL_ = 0x7300, RED_ = 0x7400,
	MAGENTA_ = 0x7500, BROWN_ = 0x7600, L_GREY_ = 0x7700, D_GREY_ = 0x7800,
	L_BLUE_ = 0x7900, L_GREEN_ = 0x7a00, L_GR_BL_ = 0x7b00, L_RED_ = 0x7c00,
	L_MAGENTA_ = 0x7d00, YELLOW_ = 0x7e00, WHITE_ = 0x7f00;
static int aa;
static bool mouseturnedon = false;
static int hotxwas = 0, hotywas = 0;
//static block savebk, mousecurs[MAXCURSORS];
//block ignore_mouseoff_bitmap = NULL;

void mgetgraphpos() {

	if (disable_mgetgraphpos)
		return;

	/*  if (bk-mousex==1) { mousex/=2; mousex--; }
	  else if (mousex-bk==1) { mousex/=2; mousex++; }
	  else*/
	if (vesa_xres == 320) mousex /= 2;
	if (vesa_xres == 960) mousex = (mousex * 3) / 2;
	if (vesa_yres == 240) mousey = (mousey * 12) / 10;
	if (vesa_yres == 480) mousey = (mousey * 24) / 10;
	if (vesa_yres == 400) mousey *= 2;
	if (vesa_yres == 600) mousey *= 3;
	//  mousex+=hotx; mousey+=hoty;
	if (mousex >= vesa_xres) mousex = vesa_xres - 1;

	if (ignore_bounds)
		return;

	if (mousex < boundx1) mousex = boundx1;
	if (mousey < boundy1) mousey = boundy1;
	if (mousex >= boundx2) mousex = boundx2 - 1;
	if (mousey >= boundy2) mousey = boundy2 - 1;
}

void msetcursorlimit(int x1, int y1, int x2, int y2) {
	// like graphconfine, but don't actually pass it to the driver
	// - stops the Windows cursor showing when out of the area
	boundx1 = x1;
	boundy1 = y1;
	boundx2 = x2;
	boundy2 = y2;
}

void domouse(int str) {
	/* TO USE THIS ROUTINE YOU MUST LOAD A MOUSE CURSOR USING mloadcursor.
	.
	.  YOU MUST ALSO REMEMBER TO CALL mfreemem AT THE END OF THE PROGRAM. */
	/*  short *sstr=(short*)&mousecurs[currentcursor][0];
	  int poow=(short)sstr[0],pooh=(short)sstr[1];*/
	int poow = wgetblockwidth(mousecurs[currentcursor]);
	int pooh = wgetblockheight(mousecurs[currentcursor]);
	int smx = mousex - hotxwas, smy = mousey - hotywas;
	//  mousex-=hotx; mousey-=hoty;
	mgetgraphpos();
	mousex -= hotx; mousey -= hoty;
	if (mousex + poow >= vesa_xres) poow = vesa_xres - mousex;
	if (mousey + pooh >= vesa_yres) pooh = vesa_yres - mousey;
	wclip(0, 0, vesa_xres - 1, vesa_yres - 1);
	if ((str == 0) & (mouseturnedon == true)) {
		if ((mousex != smx) | (mousey != smy)) { // the mouse has moved
			wputblock(smx, smy, savebk, 0); wfreeblock(savebk);
			savebk = wnewblock(mousex, mousey, mousex + poow, mousey + pooh);
			wputblock(mousex, mousey, mousecurs[currentcursor], 1);
		}
	} else if ((str == 1) & (mouseturnedon == false)) {
		// the mouse is just being turned on
		savebk = wnewblock(mousex, mousey, mousex + poow, mousey + pooh);
		wputblock(mousex, mousey, mousecurs[currentcursor], 1);  mouseturnedon = true;
	} else if ((str == 2) & (mouseturnedon == true)) { // the mouse is being turned off
		//    if (abuf != ignore_mouseoff_bitmap)
		wputblock(smx, smy, savebk, 0);
		wfreeblock(savebk);
		mouseturnedon = false;
	}
	mousex += hotx; mousey += hoty;
	hotxwas = hotx; hotywas = hoty;
}

int ismouseinbox(int lf, int tp, int rt, int bt) {
	if ((mousex >= lf) & (mousex <= rt) & (mousey >= tp) & (mousey <= bt)) return true;
	else return false;
}

void mfreemem() {
	for (int re = 0; re < numcurso; re++) {
		if (mousecurs[re] != NULL) wfreeblock(mousecurs[re]);
	}
}

void mnewcursor(char cursno) {
	domouse(2); currentcursor = cursno; domouse(1);
}

void mloadwcursor(char *namm) {
	color dummypal[256];
	if (wloadsprites(&dummypal[0], namm, mousecurs, 0, MAXCURSORS)) {
		error("C_Load_wCursor: Error reading mouse cursor file\n");
	}
	/*  color dummypal[256]; int f; FILE*ou;
	  if (cliboffset(namm)>0) ou=fopen(lib_file_name,"rb");
	  else ou=fopen(namm,"rb");
	  fseek(ou,((cliboffset(namm)>0) ? cliboffset(namm) : 0),SEEK_SET);
	  if (ferror(ou)) { textmode(C80);  printf("Mouse Cursor File Not Found\n");
		exit(1); }
	  int vers=fgetc(ou);
	  for (f=0;f<782;f++) fgetc(ou);
	  if (vers>=4) numcurso=getw(ou)+1;
	  else { textmode(C80); printf("Version 4.0 or later sprite file required.\n"); exit(3); }
	  if (ferror(ou)) {  textmode(C80);  printf("I/O error.");    exit(2); }
	  if (numcurso>MAXCURSORS) numcurso=MAXCURSORS;

	  int hhht,wwwd,b;
	  for (int za=0;za<numcurso;za++) {
		if (getw(ou)==0) { continue; }
		wwwd=getw(ou); hhht=getw(ou);
		wsetcolor(0); wbar(0,0,wwwd,hhht);
		for (b=0;b<hhht;b++) {
		  for (f=0;f<wwwd;f++) { wsetcolor(fgetc(ou));
		wputpixel(f,b);  }
		  }
		mousecurs[za]=wnewblock(0,0,wwwd-1,hhht-1);
		}
	//  wloadsprites(dummypal,namm,mousecurs,0,numcurso);*/
}

int mgetbutton() {
#ifdef TODO
	t.r_ax = 0x0005;
	t.r_bx = 0x0000;
	intr(0x33, &t);
	if (t.r_bx <= 0) {
		t.r_ax = 0x0005;
		t.r_bx = 0x0001;
		intr(0x33, &t);
		if (t.r_bx > 0) {
			aa = RIGHT; goto retn;
		}
		aa = NONE;
		goto retn;
	}
	t.r_ax = 0x0005;
	t.r_bx = 0x0001;
	intr(0x33, &t);
	if (t.r_bx <= 0) {
		aa = LEFT; goto retn;
	}
	aa = MIDDLE;
retn:
	return aa;
#else
	error("TODO: mgetbutton");
#endif
}

int mbutrelease(int buno) {
#ifdef TODO
	aa = false;
	t.r_ax = 0x0006;
	t.r_bx = buno;
	intr(0x33, &t);
	if (t.r_bx > 0) aa = true;
	else aa = false;
	return aa;
#else
	error("TODO: mbutrelease");
#endif
}

int misbuttondown(int buno) {
#ifdef TODO
	int tmpvr;
	t.r_ax = 3;
	intr(0x33, &t);
	tmpvr = t.r_bx;
	if (tmpvr & MB_ARRAY[buno]) return true;
	return false;
#else
	error("TODO: misbuttondown");
#endif
}
int mgetbuttonmask() {
#ifdef TODO
	t.r_ax = 3;
	intr(0x33, &t);
	return t.r_bx;
#else
	error("TODO: mgetbuttonmask");
#endif
}

void mconfine(int x1, int y1, int x2, int y2) {
#if 0
	t.r_ax = 0x0008;
	xp = (y1 - 1) * 8;
	yp = (y2 - 1) * 8;
	intr(0x33, &t);
	t.r_ax = 0x0007;
	xp = (x1 - 1) * 8;
	yp = (x2 - 1) * 8;
	intr(0x33, &t);
#endif
}

void mgraphconfine(int x1, int y1, int x2, int y2) {
#if 0
	t.r_ax = 0x0008;  // vertical restriction
	xp = y1;
	yp = y2;
	intr(0x33, &t);
	t.r_ax = 0x0007;  // horizontal restriction
	xp = x1 * 2;    // for 320 x 200
	yp = x2 * 2;
	intr(0x33, &t);
#endif
}

void mgetpos() {
#ifdef TODO
	t.r_ax = 0x0003;
	intr(0x33, &t);     // returns in 1-80, 1-25
	mousex = xp / 8 + 1;
	mousey = yp / 8 + 1;
#else
	error("TODO: mgetpos");
#endif
}

void msetpos(int xa, int ya) {
	g_system->warpMouse(xa, ya);
}

void msetgraphpos(int xa, int ya) {
	if ((xa < vesa_xres) & (ya < vesa_yres) & (ya > 0) & (xa > 0)) {
		int xp = xa * 2;
		int yp = ya;
		if (vesa_xres == 640) xp = xa;
		if (vesa_xres == 960) xp = (xa / 3) * 2;
		if (vesa_yres == 240) yp = (ya * 10) / 12;
		if (vesa_yres == 400) yp /= 2;
		if (vesa_yres == 600) yp /= 3;

		g_system->warpMouse(xp, yp);
	}
}

void mchangestyle(int nsty, int colo = 0x7700) {
#if 0
	t.r_ax = 0x0a;            // nsty is ASCII value of character to change to
	t.r_bx = 0;               // colo is 7_00, where _ is color no in hex.
	xp = colo;
	yp = nsty;
	intr(0x33, &t);
#endif
}

void mshow() {
	g_system->showMouse(true);
}

void mhide() {
	g_system->showMouse(false);
}

void msethotspot(int xx, int yy) {
	hotx = xx;
	hoty = yy;
}

} // namespace AGS2
