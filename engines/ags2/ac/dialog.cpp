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

#include "ags2/ac/dialog.h"
#include "ags2/ac/acruntime.h"
#include "ags2/vars.h"

namespace AGS2 {

#define MSG_RESTORE      984
#define MSG_CANCEL       985    // "Cancel"
#define MSG_SELECTLOAD   986    // "Select game to restore"
#define MSG_SAVEBUTTON   987    // "Save"
#define MSG_SAVEDIALOG   988    // "Save game name:"
#define MSG_REPLACE      989    // "Replace"
#define MSG_MUSTREPLACE  990    // "The folder is full. you must replace"
#define MSG_REPLACEWITH1 991    // "Replace:"
#define MSG_REPLACEWITH2 992    // "With:"
#define MSG_QUITBUTTON   993    // "Quit"
#define MSG_PLAYBUTTON   994    // "Play"
#define MSG_QUITDIALOG   995    // "Do you want to quit?"

#if 0
struct GameSetup {
	int digicard, midicard;
	int mod_player;
	int textheight;
};

extern void quit(char *);
extern int mousex, mousey;
extern int sgsiglen;
extern GameSetup usetup;
extern char ignore_bounds;      // Ignore mouse bounding rectangle while in dialog
extern volatile int timerloop;
#endif

extern char saveGameSuffix[21];

char buff[200];
int myscrnwid = 320, myscrnhit = 200;
IDriverDependantBitmap *dialogBmp = NULL;
int windowPosX, windowPosY, windowPosWidth, windowPosHeight;
block windowBuffer = NULL;

#define domouse rec_domouse

/*#define COL251 26
#define COL252 28
#define COL253 29
#define COL254 27
#define COL255 24*/
#define COL253 15
#define COL254 7
#define COL255 8

void __my_wbutt(int x1, int y1, int x2, int y2) {
	wsetcolor(COL254);            //wsetcolor(15);
	wbar(x1, y1, x2, y2);
	wsetcolor(0);
	wrectangle(x1, y1, x2, y2);
}

#define wbutt __my_wbutt

#define _export
#ifdef WINAPI
#undef WINAPI
#endif
#define WINAPI
#define mbutrelease !rec_misbuttondown
#define TEXT_HT usetup.textheight

//  =========  DEFINES  ========
// Control types
#define CNT_PUSHBUTTON 0x001
#define CNT_LISTBOX    0x002
#define CNT_LABEL      0x003
#define CNT_TEXTBOX    0x004
// Control properties
#define CNF_DEFAULT    0x100
#define CNF_CANCEL     0x200

// Dialog messages
#define CM_COMMAND   1
#define CM_KEYPRESS  2
#define CM_SELCHANGE 3
// System messages
#define SM_SAVEGAME  100
#define SM_LOADGAME  101
#define SM_QUIT      102
// System messages (to ADVEN)
#define SM_SETTRANSFERMEM 120
#define SM_GETINIVALUE    121
// System messages (to driver)
#define SM_QUERYQUIT 110
#define SM_KEYPRESS  111
#define SM_TIMER     112
// ListBox messages
#define CLB_ADDITEM   1
#define CLB_CLEAR     2
#define CLB_GETCURSEL 3
#define CLB_GETTEXT   4
#define CLB_SETTEXT   5
#define CLB_SETCURSEL 6
// TextBox messages
#define CTB_GETTEXT   1
#define CTB_SETTEXT   2

void refresh_screen() {
	blit(abuf, windowBuffer, windowPosX, windowPosY, 0, 0, windowPosWidth, windowPosHeight);
	gfxDriver->UpdateDDBFromBitmap(dialogBmp, windowBuffer, false);

	render_graphics(dialogBmp, windowPosX, windowPosY);

	// Copy it back, because the mouse will have been drawn on top
	blit(windowBuffer, abuf, 0, 0, windowPosX, windowPosY, windowPosWidth, windowPosHeight);
}

//  =========  STRUCTS  ========
struct DisplayProperties
{
	int width;
	int height;
	int colors;
	int textheight;
};

struct CSCIMessage
{
	int code;
	int id;
	int wParam;
};

#define CTB_KEYPRESS 91

struct OnScreenWindow
{
	block buffer;
	int x, y;
	int oldtop;
};

struct NewControl {
	int x, y, wid, hit, state, typeandflags, wlevel;
	char visible, enabled;        // not implemented
	char needredraw;
	virtual void draw() = 0;
	virtual int pressedon() = 0;
	virtual int processmessage(int, int, long) = 0;

	NewControl(int xx, int yy, int wi, int hi) {
		x = xx;
		y = yy;
		wid = wi;
		hit = hi;
		state = 0;
		visible = 1;
		enabled = 1;
		needredraw = 1;
	};
	NewControl() {
		visible = 1;
		enabled = 1;
	}
	virtual ~NewControl() {}

	int mouseisinarea() {
		if (topwindowhandle != wlevel)
			return 0;

		if ((mousex > x) & (mousex < x + wid) & (mousey > y) & (mousey < y + hit))
			return 1;

		return 0;
	}
	void drawifneeded() {
		if (topwindowhandle != wlevel)
			return;
		if (needredraw) {
			needredraw = 0;
			draw();
		}
	}
	void drawandmouse() {
		//    domouse(2);
		draw();
		//  domouse(1);
	}
};

#ifdef DJGPP
#pragma warn -inl
#endif
struct PushButton : public NewControl {
	char text[50];
	PushButton(int xx, int yy, int wi, int hi, const char *tex) {                             //wlevel=2;
		x = xx;
		y = yy;
		wid = wi;
		hit = hi + 1;               //hit=hi;
		state = 0;
		Common::strcpy_s(text, tex);
	};

	void draw() {
		wtextcolor(0);
		wsetcolor(COL254);
		wbar(x, y, x + wid, y + hit);
		if (state == 0)
			wsetcolor(pushbuttondarkcolor);
		else
			wsetcolor(pushbuttonlightcolor);

		wrectangle(x, y, x + wid, y + hit);
		if (state == 0)
			wsetcolor(pushbuttonlightcolor);
		else
			wsetcolor(pushbuttondarkcolor);

		wline(x, y, x + wid - 1, y);
		wline(x, y, x, y + hit - 1);
		wouttextxy(x + (wid / 2 - wgettextwidth(text, cbuttfont) / 2), y + 2, cbuttfont, text);
		if (typeandflags & CNF_DEFAULT)
			wsetcolor(0);
		else
			wsetcolor(windowbackgroundcolor);

		wrectangle(x - 1, y - 1, x + wid + 1, y + hit + 1);
	}

	int pressedon() {
		int wasstat;
		while (mbutrelease(LEFT) == 0) {
			timerloop = 0;
			wasstat = state;
			next_iteration();
			state = mouseisinarea();
			// stop mp3 skipping if button held down
			update_polled_stuff();
			if (wasstat != state) {
				//        domouse(2);
				draw();
				//domouse(1);
			}

			//      domouse(0);

			refresh_screen();

			while (timerloop == 0);
		}
		wasstat = state;
		state = 0;
		//    domouse(2);
		draw();
		//  domouse(1);
		return wasstat;
	}

	int processmessage(int mcode, int wParam, long lParam) {
		return -1;                  // doesn't support messages
	}
};

#define MAXLISTITEM 300
int smcode = 0;
struct MyListBox :public NewControl
{
	int items, topitem, numonscreen, selected;
	char *itemnames[MAXLISTITEM];
	MyListBox(int xx, int yy, int wii, int hii) {
		x = xx;
		y = yy;
		wid = wii;
		hit = hii;
		hit -= (hit - 4) % TEXT_HT; // resize to multiple of text height
		numonscreen = (hit - 4) / TEXT_HT;
		items = 0;
		topitem = 0;
		selected = -1;
	}

	void clearlist() {
		for (int kk = 0; kk < items; kk++)
			free(itemnames[kk]);

		items = 0;
	}

	~MyListBox() {
		clearlist();
	}

#define ARROWWIDTH 8
	void draw() {
		wsetcolor(windowbackgroundcolor);
		wbar(x, y, x + wid, y + hit);
		wsetcolor(0);
		wrectangle(x, y, x + wid, y + hit);

		int widwas = wid;
		wid -= ARROWWIDTH;
		wline(x + wid, y, x + wid, y + hit);        // draw the up/down arrows
		wline(x + wid, y + hit / 2, x + widwas, y + hit / 2);

		int xmidd = x + wid + (widwas - wid) / 2;
		if (topitem < 1)
			wsetcolor(7);

		wline(xmidd, y + 2, xmidd, y + 10); // up arrow
		wline(xmidd - 1, y + 3, xmidd + 1, y + 3);
		wline(xmidd - 2, y + 4, xmidd + 2, y + 4);
		wsetcolor(0);
		if (topitem + numonscreen >= items)
			wsetcolor(7);

		wline(xmidd, y + hit - 10, xmidd, y + hit - 3);     // down arrow
		wline(xmidd - 1, y + hit - 4, xmidd + 1, y + hit - 4);
		wline(xmidd - 2, y + hit - 5, xmidd + 2, y + hit - 5);
		wsetcolor(0);

		for (int tt = 0; tt < numonscreen; tt++) {
			int inum = tt + topitem;
			if (inum >= items)
				break;

			int thisypos = y + 2 + tt * TEXT_HT;
			if (inum == selected) {
				wsetcolor(0);
				wbar(x, thisypos, x + wid, thisypos + TEXT_HT - 1);
				wtextcolor(7);
			} else
				wtextcolor(0);

			wouttextxy(x + 2, thisypos, cbuttfont, itemnames[inum]);
		}
		wid = widwas;
	}

	int pressedon() {
		if (mousex > x + wid - ARROWWIDTH) {
			if ((mousey - y < hit / 2) & (topitem > 0))
				topitem--;
			else if ((mousey - y > hit / 2) & (topitem + numonscreen < items))
				topitem++;

		} else {
			selected = ((mousey - y) - 2) / TEXT_HT + topitem;
			if (selected >= items)
				selected = items - 1;

		}

		//    domouse(2);
		draw();
		//  domouse(1);
		smcode = CM_SELCHANGE;
		return 0;
	}

	void additem(char *texx) {
		if (items >= MAXLISTITEM)
			quit("!CSCIUSER16: Too many items added to listbox");
		itemnames[items] = scumm_strdup(texx);
		items++;
		needredraw = 1;
	}

	int processmessage(int mcode, int wParam, long lParam) {
		if (mcode == CLB_ADDITEM) {
			additem((char *)lParam);
		} else if (mcode == CLB_CLEAR)
			clearlist();
		else if (mcode == CLB_GETCURSEL)
			return selected;
		else if (mcode == CLB_SETCURSEL)
		{
			selected = wParam;

			if ((selected < topitem) && (selected >= 0))
				topitem = selected;

			if (topitem + numonscreen <= selected)
				topitem = (selected + 1) - numonscreen;
		} else if (mcode == CLB_GETTEXT)
			Common::strcpy_s((char *)lParam, STD_BUFFER_SIZE, itemnames[wParam]);
		else if (mcode == CLB_SETTEXT) {
			if (wParam < items)
				free(itemnames[wParam]);

			char *newstri = (char *)lParam;
			itemnames[wParam] = (char *)malloc(strlen(newstri) + 2);
			Common::strcpy_s(itemnames[wParam], strlen(newstri) + 2, newstri);

		} else if (mcode == CTB_KEYPRESS) {
			if ((wParam == 380) && (selected < items - 1))
				selected++;

			if ((wParam == 372) && (selected > 0))
				selected--;

			if (wParam == 373)
				selected -= (numonscreen - 1);

			if (wParam == 381)
				selected += (numonscreen - 1);

			if ((selected < 0) && (items > 0))
				selected = 0;

			if (selected >= items)
				selected = items - 1;

			if ((selected < topitem) & (selected >= 0))
				topitem = selected;

			if (topitem + numonscreen <= selected)
				topitem = (selected + 1) - numonscreen;

			drawandmouse();
			smcode = CM_SELCHANGE;
		} else
			return -1;

		return 0;
	}
};

struct MyLabel : public NewControl {
	char text[150];

	MyLabel(int xx, int yy, int wii, const char *tee) {
		Common::strcpy_s(text, tee);
		text[149] = 0;
		x = xx;
		y = yy;
		wid = wii;
		hit = TEXT_HT;
	}

	void draw() {
		int cyp = y;
		char *teptr = &text[0];
		wtextcolor(0);

		break_up_text_into_lines(wid, acdialog_font, teptr);
		for (int ee = 0; ee < numlines; ee++) {
			wouttext_outline(x, cyp, acdialog_font, lines[ee]);
			cyp += TEXT_HT;
		}
		/*
		* int lastspac = 0, curofs = 0;
			while (1) {
			  if ((teptr[curofs] == ' ') | (teptr[curofs] == 0)) {
				int itwas = teptr[curofs];
				teptr[curofs] = 0;
				if (wgettextwidth(teptr, cbuttfont) > wid) {
				  teptr[curofs] = itwas;
				  teptr[lastspac] = 0;
				  wouttextxy(x, cyp, cbuttfont, teptr);
				  teptr[lastspac] = ' ';
				  teptr += lastspac + 1;
				  curofs = 0;
				  cyp += TEXT_HT;
				} else
				  teptr[curofs] = itwas;

				lastspac = curofs;
			  }

			  if (teptr[curofs] == 0)
				break;

			  curofs++;
			}
			wouttextxy(x, cyp, cbuttfont, teptr);*/
	}

	int pressedon() {
		return 0;
	}

	int processmessage(int mcode, int wParam, long lParam) {
		return -1;                  // doesn't support messages
	}
};

#define TEXTBOX_MAXLEN 49
struct MyTextBox :public NewControl {
	char text[TEXTBOX_MAXLEN + 1];

	MyTextBox(int xx, int yy, int wii, const char *tee) {
		x = xx;
		y = yy;
		wid = wii;
		if (tee != NULL)
			Common::strcpy_s(text, tee);
		else
			text[0] = 0;

		hit = TEXT_HT + 1;
	}

	void draw() {
		wsetcolor(windowbackgroundcolor);
		wbar(x, y, x + wid, y + hit);
		wsetcolor(0);
		wrectangle(x, y, x + wid, y + hit);
		wtextcolor(0);
		wouttextxy(x + 2, y + 1, cbuttfont, text);

		char tbu[2] = "_";
		wouttextxy(x + 2 + wgettextwidth(text, cbuttfont), y + 1, cbuttfont, tbu);
	}

	int pressedon() {
		return 0;
	}

	int processmessage(int mcode, int wParam, long lParam) {
		if (mcode == CTB_SETTEXT) {
			Common::strcpy_s(text, (char *)lParam);
			needredraw = 1;
		} else if (mcode == CTB_GETTEXT)
			Common::strcpy_s((char *)lParam, STD_BUFFER_SIZE, text);
		else if (mcode == CTB_KEYPRESS) {
			if (wParam == 8) {
				if (text[0] != 0)
					text[strlen(text) - 1] = 0;

				drawandmouse();
			} else if (strlen(text) >= TEXTBOX_MAXLEN - 1)
				;
			else if (wgettextwidth(text, cbuttfont) >= wid - 5)
				;
			else if (wParam > 127)
				;  // font only has 128 chars
			else {
				text[strlen(text) + 1] = 0;
				text[strlen(text)] = wParam;
				drawandmouse();
			}
		} else
			return -1;

		return 0;
	}
};

#ifdef DJGPP
#pragma warn +inl
#endif

#define MAXCONTROLS 20
#define MAXSCREENWINDOWS 5
NewControl *vobjs[MAXCONTROLS];
OnScreenWindow oswi[MAXSCREENWINDOWS];

int CSCIGetVersion() {
	return 0x0100;
}

void multiply_up_to_game_res(int *x, int *y) {
	x[0] = get_fixed_pixel_size(x[0]);
	y[0] = get_fixed_pixel_size(y[0]);
}

void multiply_up(int *x1, int *y1, int *x2, int *y2) {
	multiply_up_to_game_res(x1, y1);
	multiply_up_to_game_res(x2, y2);

	// adjust for 800x600
	if ((GetBaseWidth() == 400) || (GetBaseWidth() == 800)) {
		x1[0] = (x1[0] * 5) / 4;
		x2[0] = (x2[0] * 5) / 4;
		y1[0] = (y1[0] * 3) / 2;
		y2[0] = (y2[0] * 3) / 2;
	} else if (GetBaseWidth() == 1024)
	{
		x1[0] = (x1[0] * 16) / 10;
		x2[0] = (x2[0] * 16) / 10;
		y1[0] = (y1[0] * 384) / 200;
		y2[0] = (y2[0] * 384) / 200;
	}
}

int windowcount = 0, curswas = 0;
int CSCIDrawWindow(int xx, int yy, int wid, int hit) {
	ignore_bounds++;
	multiply_up(&xx, &yy, &wid, &hit);
	int drawit = -1;
	for (int aa = 0; aa < MAXSCREENWINDOWS; aa++) {
		if (oswi[aa].buffer == NULL) {
			drawit = aa;
			break;
		}
	}

	if (drawit < 0)
		quit("Too many windows created.");

	windowcount++;
	//  domouse(2);
	xx -= 2;
	yy -= 2;
	wid += 4;
	hit += 4;
	oswi[drawit].buffer = wnewblock(xx, yy, xx + wid, yy + hit);
	oswi[drawit].x = xx;
	oswi[drawit].y = yy;
	wbutt(xx + 1, yy + 1, xx + wid - 1, yy + hit - 1);    // wbutt goes outside its area
	//  domouse(1);
	oswi[drawit].oldtop = topwindowhandle;
	topwindowhandle = drawit;
	windowPosX = xx;
	windowPosY = yy;
	windowPosWidth = wid;
	windowPosHeight = hit;
	return drawit;
}

void CSCIEraseWindow(int handl) {
	//  domouse(2);
	ignore_bounds--;
	topwindowhandle = oswi[handl].oldtop;
	wputblock(oswi[handl].x, oswi[handl].y, oswi[handl].buffer, 0);
	wfreeblock(oswi[handl].buffer);
	//  domouse(1);
	oswi[handl].buffer = NULL;
	windowcount--;
}

int controlid = 0;
int checkcontrols() {
	smcode = 0;
	for (int kk = 0; kk < MAXCONTROLS; kk++) {
		if (vobjs[kk] != NULL) {
			if (vobjs[kk]->mouseisinarea()) {
				controlid = kk;
				return vobjs[kk]->pressedon();
			}
		}
	}
	return 0;
}

int finddefaultcontrol(int flagmask) {
	for (int ff = 0; ff < MAXCONTROLS; ff++) {
		if (vobjs[ff] == NULL)
			continue;

		if (vobjs[ff]->wlevel != topwindowhandle)
			continue;

		if (vobjs[ff]->typeandflags & flagmask)
			return ff;
	}

	return -1;
}

int CSCIWaitMessage(CSCIMessage *cscim) {
	next_iteration();
	wtexttransparent(TEXTFG);
	for (int uu = 0; uu < MAXCONTROLS; uu++) {
		if (vobjs[uu] != NULL) {
			//      domouse(2);
			vobjs[uu]->drawifneeded();
			//      domouse(1);
		}
	}

	windowBuffer = create_bitmap_ex(bitmap_color_depth(abuf), windowPosWidth, windowPosHeight);
	windowBuffer = gfxDriver->ConvertBitmapToSupportedColourDepth(windowBuffer);
	dialogBmp = gfxDriver->CreateDDBFromBitmap(windowBuffer, false, true);

	while (1) {
		timerloop = 0;
		next_iteration();
		refresh_screen();

		cscim->id = -1;
		cscim->code = 0;
		smcode = 0;
		if (kbhit()) {
			int keywas = getch();
			if (keywas == 0)
				keywas = getch() + 300;

			if (keywas == 13) {
				cscim->id = finddefaultcontrol(CNF_DEFAULT);
				cscim->code = CM_COMMAND;
			} else if (keywas == 27) {
				cscim->id = finddefaultcontrol(CNF_CANCEL);
				cscim->code = CM_COMMAND;
			} else if ((keywas < 32) && (keywas != 8));
			else if ((keywas >= 372) & (keywas <= 381) & (finddefaultcontrol(CNT_LISTBOX) >= 0))
				vobjs[finddefaultcontrol(CNT_LISTBOX)]->processmessage(CTB_KEYPRESS, keywas, 0);
			else if (finddefaultcontrol(CNT_TEXTBOX) >= 0)
				vobjs[finddefaultcontrol(CNT_TEXTBOX)]->processmessage(CTB_KEYPRESS, keywas, 0);

			if (cscim->id < 0) {
				cscim->code = CM_KEYPRESS;
				cscim->wParam = keywas;
			}
		}

		if (rec_mgetbutton() != NONE) {
			if (checkcontrols()) {
				cscim->id = controlid;
				cscim->code = CM_COMMAND;
			}
		}

		if (smcode) {
			cscim->code = smcode;
			cscim->id = controlid;
		}

		if (cscim->code > 0)
			break;

		update_polled_stuff_and_crossfade();
		while (timerloop == 0);
	}

	gfxDriver->DestroyDDB(dialogBmp);
	dialogBmp = NULL;
	destroy_bitmap(windowBuffer);
	windowBuffer = NULL;
	return 0;
}

int CSCICreateControl(int typeandflags, int xx, int yy, int wii, int hii, const char *title) {
	multiply_up(&xx, &yy, &wii, &hii);
	int usec = -1;
	for (int hh = 1; hh < MAXCONTROLS; hh++) {
		if (vobjs[hh] == NULL) {
			usec = hh;
			break;
		}
	}

	if (usec < 0)
		quit("Too many controls created");

	int type = typeandflags & 0x00ff;     // 256 control types
	if (type == CNT_PUSHBUTTON) {
		if (wii == -1)
			wii = wgettextwidth(title, cbuttfont) + 20;

		vobjs[usec] = new PushButton(xx, yy, wii, hii, title);

	} else if (type == CNT_LISTBOX) {
		vobjs[usec] = new MyListBox(xx, yy, wii, hii);
	} else if (type == CNT_LABEL) {
		vobjs[usec] = new MyLabel(xx, yy, wii, title);
	} else if (type == CNT_TEXTBOX) {
		vobjs[usec] = new MyTextBox(xx, yy, wii, title);
	} else
		quit("Unknown control type requested");

	vobjs[usec]->typeandflags = typeandflags;
	wtexttransparent(TEXTFG);
	vobjs[usec]->wlevel = topwindowhandle;
	//  domouse(2);
	vobjs[usec]->draw();
	//  domouse(1);
	return usec;
}

void CSCIDeleteControl(int haa) {
	delete vobjs[haa];
	vobjs[haa] = NULL;
}

int CSCISendControlMessage(int haa, int mess, int wPar, long lPar) {
	if (vobjs[haa] == NULL)
		return -1;
	return vobjs[haa]->processmessage(mess, wPar, lPar);
}

DisplayProperties dispp;
char *lpTemp, *lpTemp2;
char bufTemp[260], buffer2[260];
int numsaves = 0, toomanygames;
int filenumbers[MAXSAVEGAMES];
unsigned long filedates[MAXSAVEGAMES];
extern void get_save_game_path(int slotNum, char *buffer);
extern char saveGameDirectory[260];

void preparesavegamelist(int ctrllist) {
#ifdef TODO
	numsaves = 0;
	toomanygames = 0;
	al_ffblk ffb;
	int bufix = 0;
	char curdir[255];
	_getcwd(curdir, 255);

	char searchPath[260];
	Common::sprintf_s(searchPath, "%s""agssave.*%s", saveGameDirectory, saveGameSuffix);

	int don = al_findfirst(searchPath, &ffb, -1);
	while (!don) {
		bufix = 0;
		if (numsaves >= MAXSAVEGAMES) {
			toomanygames = 1;
			break;
		}

		// only list games .000 to .099 (to allow higher slots for other purposes)
		if (strstr(ffb.name, ".0") == NULL) {
			don = al_findnext(&ffb);
			continue;
		}

		const char *numberExtension = strstr(ffb.name, ".0") + 1;
		int sgNumber = atoi(numberExtension);

		char thisGamePath[260];
		get_save_game_path(sgNumber, thisGamePath);

		// get description
		load_game(sgNumber, buff, NULL);

		CSCISendControlMessage(ctrllist, CLB_ADDITEM, 0, (long)&buff[0]);
		// Select the first item
		CSCISendControlMessage(ctrllist, CLB_SETCURSEL, 0, 0);
		filenumbers[numsaves] = sgNumber;
		filedates[numsaves] = (long int)ffb.time;
		numsaves++;
		don = al_findnext(&ffb);
	}

	al_findclose(&ffb);
	if (numsaves >= MAXSAVEGAMES)
		toomanygames = 1;

	for (int nn = 0; nn < numsaves - 1; nn++) {
		for (int kk = 0; kk < numsaves - 1; kk++) { // Date order the games
			if (filedates[kk] < filedates[kk + 1]) {  // swap them round
				CSCISendControlMessage(ctrllist, CLB_GETTEXT, kk, (long)&buff[0]);
				CSCISendControlMessage(ctrllist, CLB_GETTEXT, kk + 1, (long)&buffer2[0]);
				CSCISendControlMessage(ctrllist, CLB_SETTEXT, kk + 1, (long)&buff[0]);
				CSCISendControlMessage(ctrllist, CLB_SETTEXT, kk, (long)&buffer2[0]);
				int numtem = filenumbers[kk];
				filenumbers[kk] = filenumbers[kk + 1];
				filenumbers[kk + 1] = numtem;
				long numted = filedates[kk];
				filedates[kk] = filedates[kk + 1];
				filedates[kk + 1] = numted;
			}
		}
	}
#else
	error("TODO: preparesavegamelist");
#endif
}

int loadgamedialog() {
	int boxleft = myscrnwid / 2 - 100;
	int boxtop = myscrnhit / 2 - 60;
	int buttonhit = usetup.textheight + 5;
	int handl = CSCIDrawWindow(boxleft, boxtop, 200, 120);
	int ctrlok =
		CSCICreateControl(CNT_PUSHBUTTON | CNF_DEFAULT, boxleft + 135, boxtop + 5, 60, 10, get_global_message(MSG_RESTORE));
	int ctrlcancel =
		CSCICreateControl(CNT_PUSHBUTTON | CNF_CANCEL, boxleft + 135, boxtop + 5 + buttonhit, 60, 10,
			get_global_message(MSG_CANCEL));
	int ctrllist = CSCICreateControl(CNT_LISTBOX, boxleft + 10, boxtop + 30, 120, 80, NULL);
	int ctrltex1 = CSCICreateControl(CNT_LABEL, boxleft + 10, boxtop + 5, 120, 0, get_global_message(MSG_SELECTLOAD));
	CSCISendControlMessage(ctrllist, CLB_CLEAR, 0, 0);

	preparesavegamelist(ctrllist);
	CSCIMessage mes;
	lpTemp = NULL;
	int toret = -1;
	while (1) {
		CSCIWaitMessage(&mes);      //printf("mess: %d, id %d ",mes.code,mes.id);
		if (mes.code == CM_COMMAND) {
			if (mes.id == ctrlok) {
				int cursel = CSCISendControlMessage(ctrllist, CLB_GETCURSEL, 0, 0);
				if ((cursel >= numsaves) | (cursel < 0))
					lpTemp = NULL;
				else {
					toret = filenumbers[cursel];
					get_save_game_path(toret, bufTemp);
					lpTemp = &bufTemp[0];
				}
			} else if (mes.id == ctrlcancel) {
				lpTemp = NULL;
			}

			break;
		}
	}

	CSCIDeleteControl(ctrltex1);
	CSCIDeleteControl(ctrllist);
	CSCIDeleteControl(ctrlok);
	CSCIDeleteControl(ctrlcancel);
	CSCIEraseWindow(handl);
	return toret;
}

void enterstringwindow(char *prompttext, char *stouse) {
	int boxleft = 60, boxtop = 80;
	int wantCancel = 0;
	if (prompttext[0] == '!') {
		wantCancel = 1;
		prompttext++;
	}
	int handl = CSCIDrawWindow(boxleft, boxtop, 200, 40);
	int ctrlok = CSCICreateControl(CNT_PUSHBUTTON | CNF_DEFAULT, boxleft + 135, boxtop + 5, 60, 10, "OK");
	int ctrlcancel = -1;
	if (wantCancel)
		ctrlcancel = CSCICreateControl(CNT_PUSHBUTTON | CNF_CANCEL, boxleft + 135, boxtop + 20, 60, 10, get_global_message(MSG_CANCEL));
	int ctrltbox = CSCICreateControl(CNT_TEXTBOX, boxleft + 10, boxtop + 29, 120, 0, NULL);
	int ctrltex1 = CSCICreateControl(CNT_LABEL, boxleft + 10, boxtop + 5, 120, 0, prompttext);
	CSCIMessage mes;

	while (1) {
		CSCIWaitMessage(&mes);
		if (mes.code == CM_COMMAND) {
			if (mes.id == ctrlcancel)
				buffer2[0] = 0;
			else
				CSCISendControlMessage(ctrltbox, CTB_GETTEXT, 0, (long)&buffer2[0]);
			break;
		}
	}

	CSCIDeleteControl(ctrltex1);
	CSCIDeleteControl(ctrltbox);
	CSCIDeleteControl(ctrlok);
	if (wantCancel)
		CSCIDeleteControl(ctrlcancel);
	CSCIEraseWindow(handl);
	Common::strcpy_s(stouse, STD_BUFFER_SIZE, buffer2);
}

int enternumberwindow(char *prompttext) {
	char ourbuf[200];
	enterstringwindow(prompttext, ourbuf);
	if (ourbuf[0] == 0)
		return -9999;
	return atoi(ourbuf);
}

int roomSelectorWindow(int currentRoom, int numRooms, int *roomNumbers, char **roomNames) {
	char labeltext[200];
	Common::strcpy_s(labeltext, get_global_message(MSG_SAVEDIALOG));
	int boxleft = myscrnwid / 2 - 120;
	int boxtop = myscrnhit / 2 - 80;
	//int buttonhit = usetup.textheight + 5;
	int labeltop = boxtop + 5;
	int handl = CSCIDrawWindow(boxleft, boxtop, 240, 160);
	int ctrllist = CSCICreateControl(CNT_LISTBOX, boxleft + 10, boxtop + 40, 220, 100, NULL);
	int ctrlcancel =
		CSCICreateControl(CNT_PUSHBUTTON | CNF_CANCEL, boxleft + 80, boxtop + 145, 60, 10, "Cancel");

	CSCISendControlMessage(ctrllist, CLB_CLEAR, 0, 0);    // clear the list box
	for (int aa = 0; aa < numRooms; aa++) {
		Common::sprintf_s(buff, "%3d %s", roomNumbers[aa], roomNames[aa]);
		CSCISendControlMessage(ctrllist, CLB_ADDITEM, 0, (long)&buff[0]);
		if (roomNumbers[aa] == currentRoom)
		{
			CSCISendControlMessage(ctrllist, CLB_SETCURSEL, aa, 0);
		}
	}

	int ctrlok = CSCICreateControl(CNT_PUSHBUTTON | CNF_DEFAULT, boxleft + 10, boxtop + 145, 60, 10, "OK");
	int ctrltex1 = CSCICreateControl(CNT_LABEL, boxleft + 10, labeltop, 180, 0, "Choose which room to go to:");
	CSCIMessage mes;

	lpTemp = NULL;
	//Common::sprintf_s(buffer2, "%d", currentRoom);
	Common::sprintf_s(buffer2, "");

	int ctrltbox = CSCICreateControl(CNT_TEXTBOX, boxleft + 10, boxtop + 29, 120, 0, NULL);
	CSCISendControlMessage(ctrltbox, CTB_SETTEXT, 0, (long)&buffer2[0]);

	int toret = -1;
	while (1) {
		CSCIWaitMessage(&mes);      //printf("mess: %d, id %d ",mes.code,mes.id);
		if (mes.code == CM_COMMAND)
		{
			if (mes.id == ctrlok)
			{
				CSCISendControlMessage(ctrltbox, CTB_GETTEXT, 0, (long)&buffer2[0]);
				if (Common::isDigit(buffer2[0]))
				{
					toret = atoi(buffer2);
				}
			} else if (mes.id == ctrlcancel)
			{
			}
			break;
		} else if (mes.code == CM_SELCHANGE)
		{
			int cursel = CSCISendControlMessage(ctrllist, CLB_GETCURSEL, 0, 0);
			if (cursel >= 0)
			{
				Common::sprintf_s(buffer2, "%d", roomNumbers[cursel]);
				CSCISendControlMessage(ctrltbox, CTB_SETTEXT, 0, (long)&buffer2[0]);
			}
		}
	}

	CSCIDeleteControl(ctrltbox);
	CSCIDeleteControl(ctrltex1);
	CSCIDeleteControl(ctrllist);
	CSCIDeleteControl(ctrlok);
	CSCIDeleteControl(ctrlcancel);
	CSCIEraseWindow(handl);
	return toret;
}

int savegamedialog() {
	char okbuttontext[50];
	Common::strcpy_s(okbuttontext, get_global_message(MSG_SAVEBUTTON));
	char labeltext[200];
	Common::strcpy_s(labeltext, get_global_message(MSG_SAVEDIALOG));
	int boxleft = myscrnwid / 2 - 100;
	int boxtop = myscrnhit / 2 - 60;
	int buttonhit = usetup.textheight + 5;
	int labeltop = boxtop + 5;
	int handl = CSCIDrawWindow(boxleft, boxtop, 200, 120);
	int ctrlcancel =
		CSCICreateControl(CNT_PUSHBUTTON | CNF_CANCEL, boxleft + 135, boxtop + 5 + buttonhit, 60, 10,
			get_global_message(MSG_CANCEL));
	int ctrllist = CSCICreateControl(CNT_LISTBOX, boxleft + 10, boxtop + 40, 120, 80, NULL);
	int ctrltbox = 0;

	CSCISendControlMessage(ctrllist, CLB_CLEAR, 0, 0);    // clear the list box
	preparesavegamelist(ctrllist);
	if (toomanygames) {
		Common::strcpy_s(okbuttontext, get_global_message(MSG_REPLACE));
		Common::strcpy_s(labeltext, get_global_message(MSG_MUSTREPLACE));
		labeltop = boxtop + 2;
	} else
		ctrltbox = CSCICreateControl(CNT_TEXTBOX, boxleft + 10, boxtop + 29, 120, 0, NULL);

	int ctrlok = CSCICreateControl(CNT_PUSHBUTTON | CNF_DEFAULT, boxleft + 135, boxtop + 5, 60, 10, okbuttontext);
	int ctrltex1 = CSCICreateControl(CNT_LABEL, boxleft + 10, labeltop, 120, 0, labeltext);
	CSCIMessage mes;

	lpTemp = NULL;
	if (numsaves > 0)
		CSCISendControlMessage(ctrllist, CLB_GETTEXT, 0, (long)&buffer2[0]);
	else
		buffer2[0] = 0;

	CSCISendControlMessage(ctrltbox, CTB_SETTEXT, 0, (long)&buffer2[0]);

	int toret = -1;
	while (1) {
		CSCIWaitMessage(&mes);      //printf("mess: %d, id %d ",mes.code,mes.id);
		if (mes.code == CM_COMMAND) {
			if (mes.id == ctrlok) {
				int cursell = CSCISendControlMessage(ctrllist, CLB_GETCURSEL, 0, 0);
				CSCISendControlMessage(ctrltbox, CTB_GETTEXT, 0, (long)&buffer2[0]);

				if (numsaves > 0)
					CSCISendControlMessage(ctrllist, CLB_GETTEXT, cursell, (long)&bufTemp[0]);
				else
					Common::strcpy_s(bufTemp, "_NOSAVEGAMENAME");

				if (toomanygames) {
					int nwhand = CSCIDrawWindow(boxleft + 5, boxtop + 20, 190, 65);
					int lbl1 =
						CSCICreateControl(CNT_LABEL, boxleft + 20, boxtop + 25, 160, 0, get_global_message(MSG_REPLACEWITH1));
					int lbl2 = CSCICreateControl(CNT_LABEL, boxleft + 30, boxtop + 34, 160, 0, bufTemp);
					int lbl3 =
						CSCICreateControl(CNT_LABEL, boxleft + 20, boxtop + 45, 160, 0, get_global_message(MSG_REPLACEWITH2));
					int txt1 = CSCICreateControl(CNT_TEXTBOX, boxleft + 20, boxtop + 55, 160, 0, bufTemp);
					int btnOk =
						CSCICreateControl(CNT_PUSHBUTTON | CNF_DEFAULT, boxleft + 30, boxtop + 70, 60, 10,
							get_global_message(MSG_REPLACE));
					int btnCancel =
						CSCICreateControl(CNT_PUSHBUTTON | CNF_CANCEL, boxleft + 100, boxtop + 70, 60, 10,
							get_global_message(MSG_CANCEL));

					CSCIMessage cmes;
					do {
						CSCIWaitMessage(&cmes);
					} while (cmes.code != CM_COMMAND);

					CSCISendControlMessage(txt1, CTB_GETTEXT, 0, (long)&buffer2[0]);
					CSCIDeleteControl(btnCancel);
					CSCIDeleteControl(btnOk);
					CSCIDeleteControl(txt1);
					CSCIDeleteControl(lbl3);
					CSCIDeleteControl(lbl2);
					CSCIDeleteControl(lbl1);
					CSCIEraseWindow(nwhand);
					bufTemp[0] = 0;

					if (cmes.id == btnCancel) {
						lpTemp = NULL;
						break;
					} else
						toret = filenumbers[cursell];

				} else if (strcmp(buffer2, bufTemp) != 0) {     // create a new game (description different)
					int highestnum = 0;
					for (int pp = 0; pp < numsaves; pp++) {
						if (filenumbers[pp] > highestnum)
							highestnum = filenumbers[pp];
					}

					if (highestnum > 90)
						quit("Save game directory overflow");

					toret = highestnum + 1;
					get_save_game_path(toret, bufTemp);
				} else {
					toret = filenumbers[cursell];
					bufTemp[0] = 0;
				}

				if (bufTemp[0] == 0)
					get_save_game_path(toret, bufTemp);

				lpTemp = &bufTemp[0];
				lpTemp2 = &buffer2[0];
			} else if (mes.id == ctrlcancel) {
				lpTemp = NULL;
			}
			break;
		} else if (mes.code == CM_SELCHANGE) {
			int cursel = CSCISendControlMessage(ctrllist, CLB_GETCURSEL, 0, 0);
			if (cursel >= 0) {
				CSCISendControlMessage(ctrllist, CLB_GETTEXT, cursel, (long)&buffer2[0]);
				CSCISendControlMessage(ctrltbox, CTB_SETTEXT, 0, (long)&buffer2[0]);
			}
		}
	}

	CSCIDeleteControl(ctrltbox);
	CSCIDeleteControl(ctrltex1);
	CSCIDeleteControl(ctrllist);
	CSCIDeleteControl(ctrlok);
	CSCIDeleteControl(ctrlcancel);
	CSCIEraseWindow(handl);
	return toret;
}

CSCIMessage smes;
int myscimessagebox(char *lpprompt, char *btn1, char *btn2) {
	int windl = CSCIDrawWindow(80, 80, 240 - 80, 120 - 80);
	int lbl1 = CSCICreateControl(CNT_LABEL, 90, 85, 150, 0, lpprompt);
	int btflag = CNT_PUSHBUTTON;

	if (btn2 == NULL)
		btflag |= CNF_DEFAULT | CNF_CANCEL;
	else
		btflag |= CNF_DEFAULT;

	int btnQuit = CSCICreateControl(btflag, 90, 105, 60, 10, btn1);
	int btnPlay = 0;

	if (btn2 != NULL)
		btnPlay = CSCICreateControl(CNT_PUSHBUTTON | CNF_CANCEL, 165, 105, 60, 10, btn2);

	smes.code = 0;

	do {
		CSCIWaitMessage(&smes);
	} while (smes.code != CM_COMMAND);

	if (btnPlay)
		CSCIDeleteControl(btnPlay);

	CSCIDeleteControl(btnQuit);
	CSCIDeleteControl(lbl1);
	CSCIEraseWindow(windl);

	if (smes.id == btnQuit)
		return 1;

	return 0;
}

int quitdialog() {
	char quitbut[50], playbut[50];
	Common::strcpy_s(quitbut, get_global_message(MSG_QUITBUTTON));
	Common::strcpy_s(playbut, get_global_message(MSG_PLAYBUTTON));
	return myscimessagebox(get_global_message(MSG_QUITDIALOG), quitbut, playbut);
}

#define GET_OPTIONS_HEIGHT {\
  needheight = 0;\
  for (ww=0;ww<numdisp;ww++) {\
    break_up_text_into_lines(areawid-(8+bullet_wid),usingfont,get_translation(dtop->optionnames[disporder[ww]]));\
    needheight += (numlines * txthit) + multiply_up_coordinate(game.options[OPT_DIALOGGAP]);\
  }\
  if (parserInput) needheight += parserInput->hit + multiply_up_coordinate(game.options[OPT_DIALOGGAP]);\
 }


int write_dialog_options(int dlgxp, int curyp, int numdisp, int mouseison, int areawid,
	int bullet_wid, int usingfont, DialogTopic *dtop, char *disporder, short *dispyp,
	int txthit, int utextcol) {
	int ww;

	for (ww = 0; ww < numdisp; ww++) {

		if ((dtop->optionflags[disporder[ww]] & DFLG_HASBEENCHOSEN) &&
			(play.read_dialog_option_colour >= 0)) {
			// 'read' colour
			wtextcolor(play.read_dialog_option_colour);
		} else {
			// 'unread' colour
			wtextcolor(playerchar->talkcolor);
		}

		if (mouseison == ww) {
			if (textcol == get_col8_lookup(utextcol))
				wtextcolor(13); // the normal colour is the same as highlight col
			else wtextcolor(utextcol);
		}

		break_up_text_into_lines(areawid - (8 + bullet_wid), usingfont, get_translation(dtop->optionnames[disporder[ww]]));
		dispyp[ww] = curyp;
		if (game.dialog_bullet > 0)
			wputblock(dlgxp, curyp, spriteset[game.dialog_bullet], 1);
		int cc;
		if (game.options[OPT_DIALOGNUMBERED]) {
			char tempbfr[20];
			int actualpicwid = 0;
			if (game.dialog_bullet > 0)
				actualpicwid = spritewidth[game.dialog_bullet] + 3;

			Common::sprintf_s(tempbfr, "%d.", ww + 1);
			wouttext_outline(dlgxp + actualpicwid, curyp, usingfont, tempbfr);
		}
		for (cc = 0; cc < numlines; cc++) {
			wouttext_outline(dlgxp + ((cc == 0) ? 0 : 9) + bullet_wid, curyp, usingfont, lines[cc]);
			curyp += txthit;
		}
		if (ww < numdisp - 1)
			curyp += multiply_up_coordinate(game.options[OPT_DIALOGGAP]);
	}
	return curyp;
}

void draw_gui_for_dialog_options(GUIMain *guib, int dlgxp, int dlgyp) {
	if (guib->bgcol != 0) {
		wsetcolor(guib->bgcol);
		wbar(dlgxp, dlgyp, dlgxp + guib->wid, dlgyp + guib->hit);
	}
	if (guib->bgpic > 0)
		put_sprite_256(dlgxp, dlgyp, spriteset[guib->bgpic]);

	wsetcolor(0);
}

bool get_custom_dialog_options_dimensions(int dlgnum) {
	ccDialogOptionsRendering.Reset();
	ccDialogOptionsRendering.dialogID = dlgnum;

	getDialogOptionsDimensionsFunc.param1 = &ccDialogOptionsRendering;
	run_function_on_non_blocking_thread(&getDialogOptionsDimensionsFunc);

	if ((ccDialogOptionsRendering.width > 0) &&
		(ccDialogOptionsRendering.height > 0))
	{
		return true;
	}
	return false;
}

int show_dialog_options(int dlgnum, int sayChosenOption, bool runGameLoopsInBackground) {
	int dlgxp, dlgyp = get_fixed_pixel_size(160);
	int usingfont = FONT_NORMAL;
	int txthit = wgetfontheight(usingfont);
	int oldCursor = cur_cursor;
	int bullet_wid = 0, needheight = 0;
	IDriverDependantBitmap *ddb = NULL;
	BITMAP *subBitmap = NULL;
	GUITextBox *parserInput = NULL;
	DialogTopic *dtop = NULL;

	if ((dlgnum < 0) || (dlgnum >= game.numdialog))
		quit("!RunDialog: invalid dialog number specified");

	can_run_delayed_command();

	play.in_conversation++;

	update_polled_stuff();

	if (game.dialog_bullet > 0)
		bullet_wid = spritewidth[game.dialog_bullet] + 3;

	// numbered options, leave space for the numbers
	if (game.options[OPT_DIALOGNUMBERED])
		bullet_wid += wgettextwidth_compensate("9. ", usingfont);

	said_text = 0;

	update_polled_stuff();

	block tempScrn = create_bitmap_ex(final_col_dep, screen->w, screen->h);

	set_mouse_cursor(CURS_ARROW);

	dtop = &dialog[dlgnum];

	int ww, chose = -1, numdisp = 0;

	//get_real_screen();
	wsetscreen(virtual_screen);

	char disporder[MAXTOPICOPTIONS];
	short dispyp[MAXTOPICOPTIONS];
	int parserActivated = 0;
	if ((dtop->topicFlags & DTFLG_SHOWPARSER) && (play.disable_dialog_parser == 0)) {
		parserInput = new GUITextBox();
		parserInput->hit = txthit + get_fixed_pixel_size(4);
		parserInput->exflags = 0;
		parserInput->font = usingfont;
	}

	wtexttransparent(TEXTFG);
	numdisp = 0;
	for (ww = 0; ww < dtop->numoptions; ww++) {
		if ((dtop->optionflags[ww] & DFLG_ON) == 0) continue;
		ensure_text_valid_for_font(dtop->optionnames[ww], usingfont);
		disporder[numdisp] = ww;
		numdisp++;
	}
	if (numdisp < 1) quit("!DoDialog: all options have been turned off");
	// Don't display the options if there is only one and the parser
	// is not enabled.
	if ((numdisp > 1) || (parserInput != NULL) || (play.show_single_dialog_option)) {
		wsetcolor(0); //wbar(0,dlgyp-1,scrnwid-1,dlgyp+numdisp*txthit+1);
		int areawid = 0, is_textwindow = 0;
		int forecol = 14, savedwid;

		int mouseison = -1, curyp = 0;
		int mousewason = -10;
		int dirtyx = 0, dirtyy = 0;
		int dirtywidth = virtual_screen->w, dirtyheight = virtual_screen->h;
		bool usingCustomRendering = false;

		dlgxp = 1;
		if (get_custom_dialog_options_dimensions(dlgnum))
		{
			usingCustomRendering = true;
			dirtyx = multiply_up_coordinate(ccDialogOptionsRendering.x);
			dirtyy = multiply_up_coordinate(ccDialogOptionsRendering.y);
			dirtywidth = multiply_up_coordinate(ccDialogOptionsRendering.width);
			dirtyheight = multiply_up_coordinate(ccDialogOptionsRendering.height);
		} else if (game.options[OPT_DIALOGIFACE] > 0)
		{
			GUIMain *guib = &guis[game.options[OPT_DIALOGIFACE]];
			if (guib->is_textwindow()) {
				// text-window, so do the QFG4-style speech options
				is_textwindow = 1;
				forecol = guib->fgcol;
			} else {
				dlgxp = guib->x;
				dlgyp = guib->y;
				draw_gui_for_dialog_options(guib, dlgxp, dlgyp);

				dirtyx = dlgxp;
				dirtyy = dlgyp;
				dirtywidth = guib->wid;
				dirtyheight = guib->hit;

				areawid = guib->wid - 5;

				GET_OPTIONS_HEIGHT

					if (game.options[OPT_DIALOGUPWARDS]) {
						// They want the options upwards from the bottom
						dlgyp = (guib->y + guib->hit) - needheight;
					}

			}
		} else {
			//dlgyp=(scrnhit-numdisp*txthit)-1;
			areawid = scrnwid - 5;
			GET_OPTIONS_HEIGHT
				dlgyp = scrnhit - needheight;
			wbar(0, dlgyp - 1, scrnwid - 1, scrnhit - 1);

			dirtyx = 0;
			dirtyy = dlgyp - 1;
			dirtywidth = scrnwid;
			dirtyheight = scrnhit - dirtyy;
		}
		if (!is_textwindow)
			areawid -= multiply_up_coordinate(play.dialog_options_x) * 2;

		int orixp = dlgxp, oriyp = dlgyp;
		int wantRefresh = 0;
		mouseison = -10;

		update_polled_stuff();
		//blit(virtual_screen, tempScrn, 0, 0, 0, 0, screen->w, screen->h);
		if (!play.mouse_cursor_hidden)
			domouse(1);
		update_polled_stuff();

redraw_options:

		wantRefresh = 1;

		if (usingCustomRendering)
		{
			tempScrn = recycle_bitmap(tempScrn, final_col_dep,
				multiply_up_coordinate(ccDialogOptionsRendering.width),
				multiply_up_coordinate(ccDialogOptionsRendering.height));
		}

		clear_to_color(tempScrn, bitmap_mask_color(tempScrn));
		wsetscreen(tempScrn);

		dlgxp = orixp;
		dlgyp = oriyp;
		// lengthy drawing to screen, so lock it for speed
		//acquire_screen();

		if (usingCustomRendering)
		{
			ccDialogOptionsRendering.surfaceToRenderTo = dialogOptionsRenderingSurface;
			ccDialogOptionsRendering.surfaceAccessed = false;
			dialogOptionsRenderingSurface->linkedBitmapOnly = tempScrn;
			dialogOptionsRenderingSurface->hasAlphaChannel = false;

			renderDialogOptionsFunc.param1 = &ccDialogOptionsRendering;
			run_function_on_non_blocking_thread(&renderDialogOptionsFunc);

			if (!ccDialogOptionsRendering.surfaceAccessed)
				quit("!dialog_options_get_dimensions was implemented, but no dialog_options_render function drew anything to the surface");

			if (parserInput)
			{
				parserInput->x = multiply_up_coordinate(ccDialogOptionsRendering.parserTextboxX);
				curyp = multiply_up_coordinate(ccDialogOptionsRendering.parserTextboxY);
				areawid = multiply_up_coordinate(ccDialogOptionsRendering.parserTextboxWidth);
				if (areawid == 0)
					areawid = tempScrn->w;
			}
		} else if (is_textwindow) {
			// text window behind the options
			areawid = multiply_up_coordinate(play.max_dialogoption_width);
			int biggest = 0;
			for (ww = 0; ww < numdisp; ww++) {
				break_up_text_into_lines(areawid - (8 + bullet_wid), usingfont, get_translation(dtop->optionnames[disporder[ww]]));
				if (longestline > biggest)
					biggest = longestline;
			}
			if (biggest < areawid - (12 + bullet_wid))
				areawid = biggest + (12 + bullet_wid);

			if (areawid < multiply_up_coordinate(play.min_dialogoption_width)) {
				areawid = multiply_up_coordinate(play.min_dialogoption_width);
				if (play.min_dialogoption_width > play.max_dialogoption_width)
					quit("!game.min_dialogoption_width is larger than game.max_dialogoption_width");
			}

			GET_OPTIONS_HEIGHT

				savedwid = areawid;
			int txoffs = 0, tyoffs = 0, yspos = scrnhit / 2 - needheight / 2;
			int xspos = scrnwid / 2 - areawid / 2;
			// shift window to the right if QG4-style full-screen pic
			if ((game.options[OPT_SPEECHTYPE] == 3) && (said_text > 0))
				xspos = (scrnwid - areawid) - get_fixed_pixel_size(10);

			// needs to draw the right text window, not the default
			push_screen();
			draw_text_window(&txoffs, &tyoffs, &xspos, &yspos, &areawid, needheight, game.options[OPT_DIALOGIFACE]);
			pop_screen();
			// snice draw_text_window incrases the width, restore it
			areawid = savedwid;
			//wnormscreen();

			dirtyx = xspos;
			dirtyy = yspos;
			dirtywidth = screenop->w;
			dirtyheight = screenop->h;

			wputblock(xspos, yspos, screenop, 1);
			wfreeblock(screenop); screenop = NULL;

			// Ignore the dialog_options_x/y offsets when using a text window
			txoffs += xspos;
			tyoffs += yspos;
			dlgyp = tyoffs;
			curyp = write_dialog_options(txoffs, tyoffs, numdisp, mouseison, areawid, bullet_wid, usingfont, dtop, disporder, dispyp, txthit, forecol);
			if (parserInput)
				parserInput->x = txoffs;
		} else {

			if (wantRefresh) {
				// redraw the black background so that anti-alias
				// fonts don't re-alias themselves
				if (game.options[OPT_DIALOGIFACE] == 0) {
					wsetcolor(16);
					wbar(0, dlgyp - 1, scrnwid - 1, scrnhit - 1);
				} else {
					GUIMain *guib = &guis[game.options[OPT_DIALOGIFACE]];
					if (!guib->is_textwindow())
						draw_gui_for_dialog_options(guib, dlgxp, dlgyp);
				}
			}

			dirtyx = 0;
			dirtywidth = scrnwid;

			if (game.options[OPT_DIALOGIFACE] > 0)
			{
				// the whole GUI area should be marked dirty in order
				// to ensure it gets drawn
				GUIMain *guib = &guis[game.options[OPT_DIALOGIFACE]];
				dirtyheight = guib->hit;
				dirtyy = dlgyp;
			} else
			{
				dirtyy = dlgyp - 1;
				dirtyheight = needheight + 1;
			}

			dlgxp += multiply_up_coordinate(play.dialog_options_x);
			dlgyp += multiply_up_coordinate(play.dialog_options_y);

			// if they use a negative dialog_options_y, make sure the
			// area gets marked as dirty
			if (dlgyp < dirtyy)
				dirtyy = dlgyp;

			//curyp = dlgyp + 1;
			curyp = dlgyp;
			curyp = write_dialog_options(dlgxp, curyp, numdisp, mouseison, areawid, bullet_wid, usingfont, dtop, disporder, dispyp, txthit, forecol);

			/*if (curyp > scrnhit) {
			  dlgyp = scrnhit - (curyp - dlgyp);
			  wbar(0,dlgyp-1,scrnwid-1,scrnhit-1);
			  goto redraw_options;
			}*/
			if (parserInput)
				parserInput->x = dlgxp;
		}

		if (parserInput) {
			// Set up the text box, if present
			parserInput->y = curyp + multiply_up_coordinate(game.options[OPT_DIALOGGAP]);
			parserInput->wid = areawid - get_fixed_pixel_size(10);
			parserInput->textcol = playerchar->talkcolor;
			if (mouseison == DLG_OPTION_PARSER)
				parserInput->textcol = forecol;

			if (game.dialog_bullet)  // the parser X will get moved in a second
				wputblock(parserInput->x, parserInput->y, spriteset[game.dialog_bullet], 1);

			parserInput->wid -= bullet_wid;
			parserInput->x += bullet_wid;

			parserInput->Draw();
			parserInput->activated = 0;
		}

		wantRefresh = 0;
		wsetscreen(virtual_screen);

		update_polled_stuff();

		subBitmap = recycle_bitmap(subBitmap, bitmap_color_depth(tempScrn), dirtywidth, dirtyheight);
		subBitmap = gfxDriver->ConvertBitmapToSupportedColourDepth(subBitmap);

		update_polled_stuff();

		if (usingCustomRendering)
		{
			blit(tempScrn, subBitmap, 0, 0, 0, 0, tempScrn->w, tempScrn->h);
			invalidate_rect(dirtyx, dirtyy, dirtyx + subBitmap->w, dirtyy + subBitmap->h);
		} else
		{
			blit(tempScrn, subBitmap, dirtyx, dirtyy, 0, 0, dirtywidth, dirtyheight);
		}

		if ((ddb != NULL) &&
			((ddb->GetWidth() != dirtywidth) ||
				(ddb->GetHeight() != dirtyheight)))
		{
			gfxDriver->DestroyDDB(ddb);
			ddb = NULL;
		}
		if (ddb == NULL)
			ddb = gfxDriver->CreateDDBFromBitmap(subBitmap, false, false);
		else
			gfxDriver->UpdateDDBFromBitmap(ddb, subBitmap, false);

		render_graphics(ddb, dirtyx, dirtyy);

		while (1) {

			if (runGameLoopsInBackground)
			{
				play.disabled_user_interface++;
				mainloop(false, ddb, dirtyx, dirtyy);
				play.disabled_user_interface--;
			} else
			{
				timerloop = 0;
				NEXT_ITERATION();

				render_graphics(ddb, dirtyx, dirtyy);

				update_polled_stuff_and_crossfade();
			}

			if (kbhit()) {
				int gkey = getch();
				if (parserInput) {
					wantRefresh = 1;
					// type into the parser 
					if ((gkey == 361) || ((gkey == ' ') && (strlen(parserInput->text) == 0))) {
						// write previous contents into textbox (F3 or Space when box is empty)
						for (unsigned int i = strlen(parserInput->text); i < strlen(play.lastParserEntry); i++) {
							parserInput->KeyPress(play.lastParserEntry[i]);
						}
						//domouse(2);
						goto redraw_options;
					} else if ((gkey >= 32) || (gkey == 13) || (gkey == 8)) {
						parserInput->KeyPress(gkey);
						if (!parserInput->activated) {
							//domouse(2);
							goto redraw_options;
						}
					}
				}
				// Allow selection of options by keyboard shortcuts
				else if ((gkey >= '1') && (gkey <= '9')) {
					gkey -= '1';
					if (gkey < numdisp) {
						chose = disporder[gkey];
						break;
					}
				}
			}
			mousewason = mouseison;
			mouseison = -1;
			if (usingCustomRendering)
			{
				if ((mousex >= dirtyx) && (mousey >= dirtyy) &&
					(mousex < dirtyx + tempScrn->w) &&
					(mousey < dirtyy + tempScrn->h))
				{
					getDialogOptionUnderCursorFunc.param1 = &ccDialogOptionsRendering;
					run_function_on_non_blocking_thread(&getDialogOptionUnderCursorFunc);

					if (!getDialogOptionUnderCursorFunc.atLeastOneImplementationExists)
						quit("!The script function dialog_options_get_active is not implemented. It must be present to use a custom dialogue system.");

					mouseison = ccDialogOptionsRendering.activeOptionID;
				} else
				{
					ccDialogOptionsRendering.activeOptionID = -1;
				}
			} else if ((mousey <= dlgyp) || (mousey > curyp));
			else {
				mouseison = numdisp - 1;
				for (ww = 0; ww < numdisp; ww++) {
					if (mousey < dispyp[ww]) {
						mouseison = ww - 1; break;
					}
				}
				if ((mouseison < 0) | (mouseison >= numdisp)) mouseison = -1;
			}

			if (parserInput != NULL) {
				int relativeMousey = mousey;
				if (usingCustomRendering)
					relativeMousey -= dirtyy;

				if ((relativeMousey > parserInput->y) &&
					(relativeMousey < parserInput->y + parserInput->hit))
					mouseison = DLG_OPTION_PARSER;

				if (parserInput->activated)
					parserActivated = 1;
			}

			int mouseButtonPressed = mgetbutton();

			if (mouseButtonPressed != NONE) {
				if (mouseison < 0)
				{
					if (usingCustomRendering)
					{
						runDialogOptionMouseClickHandlerFunc.param1 = &ccDialogOptionsRendering;
						runDialogOptionMouseClickHandlerFunc.param2 = (void *)(mouseButtonPressed + 1);
						run_function_on_non_blocking_thread(&runDialogOptionMouseClickHandlerFunc);

						if (runDialogOptionMouseClickHandlerFunc.atLeastOneImplementationExists)
							goto redraw_options;
					}
					continue;
				}
				if (mouseison == DLG_OPTION_PARSER) {
					// they clicked the text box
					parserActivated = 1;
				} else if (usingCustomRendering)
				{
					chose = mouseison;
					break;
				} else {
					chose = disporder[mouseison];
					break;
				}
			}

			if (usingCustomRendering)
			{
				int mouseWheelTurn = check_mouse_wheel();
				if (mouseWheelTurn != 0)
				{
					runDialogOptionMouseClickHandlerFunc.param1 = &ccDialogOptionsRendering;
					runDialogOptionMouseClickHandlerFunc.param2 = (void *)((mouseWheelTurn < 0) ? 9 : 8);
					run_function_on_non_blocking_thread(&runDialogOptionMouseClickHandlerFunc);

					if (runDialogOptionMouseClickHandlerFunc.atLeastOneImplementationExists)
						goto redraw_options;

					continue;
				}
			}

			if (parserActivated) {
				// They have selected a custom parser-based option
				if (parserInput->text[0] != 0) {
					chose = DLG_OPTION_PARSER;
					break;
				} else {
					parserActivated = 0;
					parserInput->activated = 0;
				}
			}
			if (mousewason != mouseison) {
				//domouse(2);
				goto redraw_options;
			}
			while ((timerloop == 0) && (play.fast_forward == 0)) {
				update_polled_stuff();
				platform->YieldCPU();
			}

		}
		if (!play.mouse_cursor_hidden)
			domouse(2);
	} else
		chose = disporder[0];  // only one choice, so select it

	while (kbhit()) getch(); // empty keyboard buffer
	//leave_real_screen();
	construct_virtual_screen(true);

	if (parserActivated)
	{
		Common::strcpy_s(play.lastParserEntry, parserInput->text);
		ParseText(parserInput->text);
		chose = CHOSE_TEXTPARSER;
	}

	if (parserInput) {
		delete parserInput;
		parserInput = NULL;
	}

	if (ddb != NULL)
		gfxDriver->DestroyDDB(ddb);
	if (subBitmap != NULL)
		destroy_bitmap(subBitmap);

	set_mouse_cursor(oldCursor);
	// In case it's the QFG4 style dialog, remove the black screen
	play.in_conversation--;
	remove_screen_overlay(OVER_COMPLETE);

	wfreeblock(tempScrn);

	if (chose != CHOSE_TEXTPARSER)
	{
		dtop->optionflags[chose] |= DFLG_HASBEENCHOSEN;

		bool sayTheOption = false;
		if (sayChosenOption == SAYCHOSEN_YES)
		{
			sayTheOption = true;
		} else if (sayChosenOption == SAYCHOSEN_USEFLAG)
		{
			sayTheOption = ((dtop->optionflags[chose] & DFLG_NOREPEAT) == 0);
		}

		if (sayTheOption)
			DisplaySpeech(get_translation(dtop->optionnames[chose]), game.playercharacter);
	}

	return chose;
}

void do_conversation(int dlgnum) {
	EndSkippingUntilCharStops();

	int dlgnum_was = dlgnum;
	int previousTopics[MAX_TOPIC_HISTORY];
	int numPrevTopics = 0;
	DialogTopic *dtop = &dialog[dlgnum];

	// run the startup script
	int tocar = run_dialog_script(dtop, dlgnum, dtop->startupentrypoint, 0);
	if ((tocar == RUN_DIALOG_STOP_DIALOG) ||
		(tocar == RUN_DIALOG_GOTO_PREVIOUS))
	{
		// 'stop' or 'goto-previous' from first startup script
		remove_screen_overlay(OVER_COMPLETE);
		play.in_conversation--;
		return;
	} else if (tocar >= 0)
		dlgnum = tocar;

	while (dlgnum >= 0)
	{
		if (dlgnum >= game.numdialog)
			quit("!RunDialog: invalid dialog number specified");

		dtop = &dialog[dlgnum];

		if (dlgnum != dlgnum_was)
		{
			// dialog topic changed, so play the startup
			// script for the new topic
			tocar = run_dialog_script(dtop, dlgnum, dtop->startupentrypoint, 0);
			dlgnum_was = dlgnum;
			if (tocar == RUN_DIALOG_GOTO_PREVIOUS) {
				if (numPrevTopics < 1) {
					// goto-previous on first topic -- end dialog
					tocar = RUN_DIALOG_STOP_DIALOG;
				} else {
					tocar = previousTopics[numPrevTopics - 1];
					numPrevTopics--;
				}
			}
			if (tocar == RUN_DIALOG_STOP_DIALOG)
				break;
			else if (tocar >= 0) {
				// save the old topic number in the history
				if (numPrevTopics < MAX_TOPIC_HISTORY) {
					previousTopics[numPrevTopics] = dlgnum;
					numPrevTopics++;
				}
				dlgnum = tocar;
				continue;
			}
		}

		int chose = show_dialog_options(dlgnum, SAYCHOSEN_USEFLAG, (game.options[OPT_RUNGAMEDLGOPTS] != 0));

		if (chose == CHOSE_TEXTPARSER)
		{
			said_speech_line = 0;

			tocar = run_dialog_request(dlgnum);

			if (said_speech_line > 0) {
				// fix the problem with the close-up face remaining on screen
				DisableInterface();
				mainloop(); // redraw the screen to make sure it looks right
				EnableInterface();
				set_mouse_cursor(CURS_ARROW);
			}
		} else
		{
			tocar = run_dialog_script(dtop, dlgnum, dtop->entrypoints[chose], chose + 1);
		}

		if (tocar == RUN_DIALOG_GOTO_PREVIOUS) {
			if (numPrevTopics < 1) {
				tocar = RUN_DIALOG_STOP_DIALOG;
			} else {
				tocar = previousTopics[numPrevTopics - 1];
				numPrevTopics--;
			}
		}
		if (tocar == RUN_DIALOG_STOP_DIALOG) break;
		else if (tocar >= 0) {
			// save the old topic number in the history
			if (numPrevTopics < MAX_TOPIC_HISTORY) {
				previousTopics[numPrevTopics] = dlgnum;
				numPrevTopics++;
			}
			dlgnum = tocar;
		}

	}

}

} // namespace AGS2
