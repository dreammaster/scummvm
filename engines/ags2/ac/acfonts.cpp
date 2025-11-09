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

#define CROOM_NOFUNCTIONS
#ifndef USE_ALFONT
#define USE_ALFONT
#endif
#define USE_CLIB
#define WGT2ALLEGRO_NOFUNCTIONS
#include "ags2/common/wgt2allg.h"
#include "ags2/ac/acroom.h"
#include "ags2/ac/acruntime.h"
#include "ags2/common/clib32.h"
#include "ags2/vars.h"

#ifdef USE_ALFONT
#include "ags2/lib/alfont/alfont.h"
#endif

namespace AGS2 {

#ifdef THIS_IS_THE_ENGINE
#define fopen clibfopen
#endif

typedef unsigned char *wgtfont;

class WFNFontRenderer : public IAGSFontRenderer {
public:
	virtual bool LoadFromDisk(int fontNumber, int fontSize);
	virtual void FreeMemory(int fontNumber);
	virtual bool SupportsExtendedCharacters(int fontNumber) {
		return false;
	}
	virtual int GetTextWidth(const char *text, int fontNumber);
	virtual int GetTextHeight(const char *text, int fontNumber);
	virtual void RenderText(const char *text, int fontNumber, BITMAP *destination, int x, int y, int colour);
	virtual void AdjustYCoordinateForFont(int *ycoord, int fontNumber);
	virtual void EnsureTextValidForFont(char *text, int fontNumber);

private:
	int printchar(int xxx, int yyy, wgtfont foo, int charr);
};

class TTFFontRenderer : public IAGSFontRenderer {
public:
	virtual bool LoadFromDisk(int fontNumber, int fontSize);
	virtual void FreeMemory(int fontNumber);
	virtual bool SupportsExtendedCharacters(int fontNumber) {
		return true;
	}
	virtual int GetTextWidth(const char *text, int fontNumber);
	virtual int GetTextHeight(const char *text, int fontNumber);
	virtual void RenderText(const char *text, int fontNumber, BITMAP *destination, int x, int y, int colour);
	virtual void AdjustYCoordinateForFont(int *ycoord, int fontNumber);
	virtual void EnsureTextValidForFont(char *text, int fontNumber);
};

extern bool ShouldAntiAliasText();
extern int our_eip;
int texttrans = 0;
int textcol;
int wtext_multiply = 1;
wgtfont fonts[MAX_FONTS];
IAGSFontRenderer *fontRenderers[MAX_FONTS];
WFNFontRenderer wfnRenderer;
TTFFontRenderer ttfRenderer;

#ifdef USE_ALFONT
ALFONT_FONT *tempttffnt;
ALFONT_FONT *get_ttf_block(wgtfont fontptr) {
	memcpy(&tempttffnt, &fontptr[4], sizeof(tempttffnt));
	return tempttffnt;
}
#endif // USE_ALFONT

#ifndef THIS_IS_THE_ENGINE
extern GameSetupStruct thisgame;
void check_font(int *fontnum) {
	// Stop roomedit crashing if they specify an invalid font number
	if (fontnum[0] >= thisgame.numfonts)
		fontnum[0] = 0;
}
#endif

void init_font_renderer() {
#ifdef USE_ALFONT
	alfont_init();
	alfont_text_mode(-1);
#endif

	for (int i = 0; i < MAX_FONTS; i++)
		fontRenderers[i] = NULL;
	wtexttransparent(TEXTFG);
}

void shutdown_font_renderer() {
#ifdef USE_ALFONT
#ifdef THIS_IS_THE_ENGINE
	our_eip = 9919;
#endif
	alfont_exit();
#endif
}

void adjust_y_coordinate_for_text(int *ypos, int fontnum) {
	fontRenderers[fontnum]->AdjustYCoordinateForFont(ypos, fontnum);
}

void ensure_text_valid_for_font(char *text, int fontnum) {
	fontRenderers[fontnum]->EnsureTextValidForFont(text, fontnum);
}

int wgettextwidth(const char *texx, int fontNumber) {
	return fontRenderers[fontNumber]->GetTextWidth(texx, fontNumber);
}

int wgettextheight(const char *text, int fontNumber) {
	return fontRenderers[fontNumber]->GetTextHeight(text, fontNumber);
}

void wouttextxy(int xxx, int yyy, int fontNumber, const char *texx) {
	if (yyy > abuf->cb)
		return;                   // each char is clipped but this speeds it up

	if (fontRenderers[fontNumber] != NULL)
	{
		fontRenderers[fontNumber]->RenderText(texx, fontNumber, abuf, xxx, yyy, textcol);
	}
}

// Loads a font from disk
bool wloadfont_size(int fontNumber, int fsize) {
	if (ttfRenderer.LoadFromDisk(fontNumber, fsize))
	{
		fontRenderers[fontNumber] = &ttfRenderer;
		return true;
	} else if (wfnRenderer.LoadFromDisk(fontNumber, fsize))
	{
		fontRenderers[fontNumber] = &wfnRenderer;
		return true;
	}

	return false;
}

void wgtprintf(int xxx, int yyy, int fontNumber, char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	Common::String msg = Common::String::vformat(fmt, ap);
	va_end(ap);
	wouttextxy(xxx, yyy, fontNumber, msg.c_str());
}

void wtextcolor(int nval) {
	__my_setcolor(&textcol, nval);
}

void wfreefont(int fontNumber) {
	if (fontRenderers[fontNumber] != NULL)
		fontRenderers[fontNumber]->FreeMemory(fontNumber);

	fontRenderers[fontNumber] = NULL;
}

void wtexttransparent(int coo) {
	texttrans = coo;
}


// **** WFN Renderer ****

const char *WFN_FILE_SIGNATURE = "WGT Font File  ";

void WFNFontRenderer::AdjustYCoordinateForFont(int *ycoord, int fontNumber) {
	// Do nothing
}

void WFNFontRenderer::EnsureTextValidForFont(char *text, int fontNumber) {
	// replace any extended characters with question marks
	while (text[0] != 0) {
		if ((unsigned char)text[0] > 126)
		{
			text[0] = '?';
		}
		text++;
	}
}

int WFNFontRenderer::GetTextWidth(const char *texx, int fontNumber) {
	wgtfont foon = fonts[fontNumber];
	short *tabaddr;
	int totlen = 0;
	unsigned int dd;

	char thisCharacter;
	for (dd = 0; dd < strlen(texx); dd++)
	{
		thisCharacter = texx[dd];
		if ((thisCharacter >= 128) || (thisCharacter < 0)) thisCharacter = '?';
#ifndef ALLEGRO_BIG_ENDIAN
		tabaddr = (short *)&foon[15];
		tabaddr = (short *)&foon[tabaddr[0]];     // get address table
		tabaddr = (short *)&foon[tabaddr[thisCharacter]];      // use table to find character
		totlen += tabaddr[0];
#else
		tabaddr = (short *)&foon[15];
		tabaddr = (short *)&foon[__short_swap_endian(tabaddr[0])];     // get address table
		tabaddr = (short *)&foon[__short_swap_endian(tabaddr[thisCharacter])];      // use table to find character
		totlen += __short_swap_endian(tabaddr[0]);
#endif
	}
	return totlen * wtext_multiply;
}

int WFNFontRenderer::GetTextHeight(const char *texx, int fontNumber) {
	short *tabaddr;
	int highest = 0;
	unsigned int dd;
	wgtfont foon = fonts[fontNumber];

	char thisCharacter;
	for (dd = 0; dd < strlen(texx); dd++)
	{
		thisCharacter = texx[dd];
		if ((thisCharacter >= 128) || (thisCharacter < 0)) thisCharacter = '?';
#ifndef ALLEGRO_BIG_ENDIAN
		tabaddr = (short *)&foon[15];
		tabaddr = (short *)&foon[tabaddr[0]];     // get address table
		tabaddr = (short *)&foon[tabaddr[thisCharacter]];      // use table to find character
		int charHeight = tabaddr[1];
#else
		tabaddr = (short *)&foon[15];
		tabaddr = (short *)&foon[__short_swap_endian(tabaddr[0])];     // get address table
		tabaddr = (short *)&foon[__short_swap_endian(tabaddr[thisCharacter])];      // use table to find character
		int charHeight = __short_swap_endian(tabaddr[1]);
#endif
		if (charHeight > highest)
			highest = charHeight;
	}
	return highest * wtext_multiply;
}

void WFNFontRenderer::RenderText(const char *text, int fontNumber, BITMAP *destination, int x, int y, int colour) {
	unsigned int ee;

#ifdef THIS_IS_THE_ENGINE
	int oldeip = our_eip;
	our_eip = 415;
#endif

	for (ee = 0; ee < strlen(text); ee++)
		x += printchar(x, y, fonts[fontNumber], text[ee]);

#ifdef THIS_IS_THE_ENGINE
	our_eip = oldeip;
#endif
}

int WFNFontRenderer::printchar(int xxx, int yyy, wgtfont foo, int charr) {
	short *tabaddr = (short *)&foo[15];
	unsigned char *actdata;
	int tt, ss, bytewid, orixp = xxx;

	if ((charr > 127) || (charr < 0))
		charr = '?';

#ifndef ALLEGRO_BIG_ENDIAN
	tabaddr = (short *)&foo[tabaddr[0]];        // get address table
	tabaddr = (short *)&foo[tabaddr[charr]];    // use table to find character
	int charWidth = tabaddr[0];
	int charHeight = tabaddr[1];
#else
	tabaddr = (short *)&foo[__short_swap_endian(tabaddr[0])];
	tabaddr = (short *)&foo[__short_swap_endian(tabaddr[charr])];
	int charWidth = __short_swap_endian(tabaddr[0]);
	int charHeight = __short_swap_endian(tabaddr[1]);
#endif
	actdata = (unsigned char *)&tabaddr[2];
	bytewid = ((charWidth - 1) / 8) + 1;

	// MACPORT FIX: switch now using charWidth and charHeight
	for (tt = 0; tt < charHeight; tt++) {
		for (ss = 0; ss < charWidth; ss++) {
			if (((actdata[tt * bytewid + (ss / 8)] & (0x80 >> (ss % 8))) != 0)) {
				if (wtext_multiply > 1) {
					rectfill(abuf, xxx + ss, yyy + tt, xxx + ss + (wtext_multiply - 1),
						yyy + tt + (wtext_multiply - 1), textcol);
				} else
				{
					putpixel(abuf, xxx + ss, yyy + tt, textcol);
				}
			}

			xxx += wtext_multiply - 1;
		}
		yyy += wtext_multiply - 1;
		xxx = orixp;
	}
	return charWidth * wtext_multiply;
}

bool WFNFontRenderer::LoadFromDisk(int fontNumber, int fontSize) {
	Common::File ffi;
	char mbuffer[16];
	long lenof;

	Common::String filnm = Common::String::format("agsfnt%d.wfn", fontNumber);
	if (!ffi.open(Common::Path(filnm))) {
		// actual font not found, try font 0 instead
		filnm = "agsfnt0.wfn";
		if (!ffi.open(Common::Path(filnm)))
			return false;
	}

	mbuffer[15] = 0;
	ffi.read(mbuffer, 15);
	if (strcmp(mbuffer, WFN_FILE_SIGNATURE) != 0)
		return false;

#ifdef THIS_IS_THE_ENGINE
	// clibfopen will have set last_opened_size
	lenof = last_opened_size;
#else
	// in the editor, we don't read from clib, only from disk
	lenof = ffi.size();
#endif

	wgtfont tempalloc = (wgtfont)malloc(lenof + 40);

	ffi.seek(0);
	ffi.read(tempalloc, lenof);
	ffi.close();

	fonts[fontNumber] = tempalloc;
	return true;
}

void WFNFontRenderer::FreeMemory(int fontNumber) {
	free(fonts[fontNumber]);
	fonts[fontNumber] = NULL;
}


// ***** TTF RENDERER *****
#ifdef USE_ALFONT

void TTFFontRenderer::AdjustYCoordinateForFont(int *ycoord, int fontNumber) {
	// TTF fonts already have space at the top, so try to remove the gap
	ycoord[0]--;
}

void TTFFontRenderer::EnsureTextValidForFont(char *text, int fontNumber) {
	// do nothing, TTF can handle all characters
}

int TTFFontRenderer::GetTextWidth(const char *text, int fontNumber) {
	return alfont_text_length(get_ttf_block(fonts[fontNumber]), text);
}

int TTFFontRenderer::GetTextHeight(const char *text, int fontNumber) {
	return alfont_text_height(get_ttf_block(fonts[fontNumber]));
}

void TTFFontRenderer::RenderText(const char *text, int fontNumber, BITMAP *destination, int x, int y, int colour) {
	if (y > destination->cb)  // optimisation
		return;

	ALFONT_FONT *alfpt = get_ttf_block(fonts[fontNumber]);
	// Y - 1 because it seems to get drawn down a bit
	if ((ShouldAntiAliasText()) && (bitmap_color_depth(abuf) > 8))
		alfont_textout_aa(abuf, alfpt, text, x, y - 1, colour);
	else
		alfont_textout(abuf, alfpt, text, x, y - 1, colour);
}

bool TTFFontRenderer::LoadFromDisk(int fontNumber, int fontSize) {
	char filnm[20];
	Common::sprintf_s(filnm, "agsfnt%d.ttf", fontNumber);

	// we read the font in manually to make it load from library file
	Common::SeekableReadStream *reader = clibfopen(filnm, "rb");
	char *membuffer;

	if (reader == NULL)
		return false;

	long lenof = clibfilesize(filnm);

	// if not in the library, get size manually
	if (lenof < 1) {
		lenof = reader->size();
	}

	membuffer = (char *)malloc(lenof);
	reader->read(membuffer, lenof);
	delete reader;

	ALFONT_FONT *alfptr = alfont_load_font_from_mem(membuffer, lenof);
	free(membuffer);

	if (alfptr == NULL)
		return false;

	if (fontSize > 0)
		alfont_set_font_size(alfptr, fontSize);

	wgtfont tempalloc = (wgtfont)malloc(20);
	Common::strcpy_s((char *)tempalloc, 20, "TTF");
	memcpy(&((char *)tempalloc)[4], &alfptr, sizeof(alfptr));
	fonts[fontNumber] = tempalloc;

	return true;
}

void TTFFontRenderer::FreeMemory(int fontNumber) {
	alfont_destroy_font(get_ttf_block(fonts[fontNumber]));
	free(fonts[fontNumber]);
	fonts[fontNumber] = NULL;
}

#endif

} // namespace AGS2
