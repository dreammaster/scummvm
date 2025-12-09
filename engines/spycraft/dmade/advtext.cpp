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

#include "spycraft/dmade/advback.h"
#include "spycraft/dmade/advbits.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advport.h"
#include "spycraft/dmade/advsprite.h"
#include "spycraft/dmade/advtext.h"
#include "spycraft/dmade/winmade.h"
#include "spycraft/vars.h"

namespace Spycraft {

//#define _UNICODE

#define MAX_TEXT_IDS 200
#define MAX_TEXT_STRING_LENGTH 128

struct TextID {
	bufferNum background;
	Sprite *text;
};

extern int pixFormat;

void Format565(Viewport *);
unsigned short Format565P(unsigned short);

TextID text_ids[MAX_TEXT_IDS];

static LOGFONT logfont;                /* Windows structure with font info */
HFONT hFont = NULL;                    /* Current Font*/
int16 CurrentFontID = 0; 				/* Currently Selected Font ID */
unsigned char curTextColor[3];
unsigned char curTextOutlineColor[3];
short cur_user_text_x = 0;
short cur_user_text_y = 0;

BYTE *tgtextbuffer = NULL;
char    outlinetextdemo = 0;
char	  shifttextdemo = 0;
unsigned  shifttextcolor = 15;
uint32 outlinetextcolor = 0;
uint32 textcolor = 0;
styleField textstyle = 0L;
int16 pointsize = 12;

extern bool cacheValide;


void InitText() {
	short i;

	for (i = 0; i < MAX_TEXT_IDS; i++)
		text_ids[i].background = -1;

	for (i = 0; i < 3; i++) {
		curTextColor[i] = 0;
		curTextOutlineColor[i] = 255;
	}

	textcolor = RGB(0, 0, 0);

}

void CleanText() {
	if (hFont)
		DeleteObject(hFont);
}

void FreeTextSlots(bufferNum background_num) {
	int i;

	for (i = 0; i < MAX_TEXT_IDS; i++) {
		if (text_ids[i].background == background_num)
			text_ids[i].background = -1;
	}
}

textNum get_next_text_slot() {
	textNum i;

	// find an empty slot 
	for (i = 0; i < MAX_TEXT_IDS; i++) {
		if (text_ids[i].background == -1)
			return i;
	}

	return -1;
}


short tgcharheight() {
	TEXTMETRIC tm;
	int charheight;

	GetTextMetrics(hSrcDC, &tm);
	charheight = tm.tmHeight + tm.tmExternalLeading;
	return((short)charheight);
}

Sprite *sfxGetTextSprite(textNum text_num) {

	if (text_num < 0 || text_num > MAX_TEXT_IDS - 1)
		ADV_ASSERT(false, __ERR_CODING);

	if (text_ids[text_num].background == -1)
		ADV_ASSERT(false, __ERR_CODING);

	return(text_ids[text_num].text);
}

void sfxKillText(textNum text_num) {
	Sprite *text;

	if (text_num < 0 || text_num > MAX_TEXT_IDS - 1)
		ADV_ASSERT(false, __ERR_CODING);

	if (text_ids[text_num].background == -1)
		ADV_ASSERT(false, __ERR_CODING);

	// erase the text sprite on next update
	if (cacheValide) {
		text = text_ids[text_num].text;
		sfxKillSprite(text);
	}
	// free text slot
	text_ids[text_num].background = -1;

}

textNum _tgtextwrite(bufferNum display_buffer, channelNum channel_num,
		const char *format, ...) {
	textNum text_num;
	va_list argptr;


	va_start(argptr, format);
	Common::vsprintf_s((char *)tgtextbuffer, 256, format, argptr);
	va_end(argptr);

	text_num = sfxDrawText(display_buffer, channel_num, (const char *)tgtextbuffer);
	return (text_num);
}

textNum sfxDrawText(bufferNum display_buffer, channelNum channel_num,
		const char *format, ...) {
	textNum slot;
	Background *background;
	Viewport *textPort;
	WinViewport *winTextPort;
	Sprite *sprite;
	TextType *text;
	HBITMAP hBitmap;
	char *save_buffer;
	MADEPt text_xy;
	uint16 width, height;
	unsigned char red, green, blue;
	SRect textRect;

	va_list va;
	va_start(va, format);
	Common::String outtext = Common::String::vformat(format, va);
	va_end(va);

	if (outtext.empty())
		return -1;

	if (CurrentFontID < 0)
		ADV_ASSERT(false, __ERR_CODING);

	background = backgrounds[display_buffer];
	if (background == NULL)
		ADV_ASSERT(false, __ERR_CODING);

	slot = get_next_text_slot();
	if (slot == -1)
		ADV_ASSERT(false, __ERR_CODING);

	height = sfxGetTextHeight();
	width = sfxGetTextWidth(outtext.c_str());
	if (width == 0)
		ADV_ASSERT(false, __ERR_CODING);

	SRect_Init(&textRect, 0, 0, width - 1, height - 1);

	// make a text viewport
	textPort = (Viewport *)AllocPtr((DWORD)sizeof(Viewport));
	if (textPort == NULL)
		ADV_ASSERT(false, __ERR_PORT_ALLOC_FAIL);

	// set viewport elements
	textPort->origX = 0;
	textPort->origY = 0;
	textPort->width = width;
	textPort->height = height;
	textPort->colors = screen_colors;

	textPort->ptr = AllocPtr((DWORD)(width * height * (screen_colors == 8 ? 1 : 2)));
	if (textPort->ptr == NULL)
		ADV_ASSERT(false, __ERR_PORT_ALLOC_FAIL);

	textPort->rowBytes = width * (screen_colors == 8 ? 1 : 2);


	text = (TextType *)AllocPtr((DWORD)sizeof(TextType));
	if (text == NULL) {
		FreePtr(textPort);
		ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);
	}

	save_buffer = (char *)AllocPtr((DWORD)(outtext.c_str() + 1));
	if (save_buffer == NULL) {
		FreePtr(text);
		FreePtr(textPort);
		ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);
	}

	// allocate text sprite

	sfxGetTextXY(&text_xy);
	sprite = sfxCreateSprite(display_buffer, text_xy.x, text_xy.y,
		512, 512, textPort);
	if (sprite == NULL) {
		FreePtr(save_buffer);
		FreePtr(text);
		FreePtr(textPort);
		ADV_ASSERT(false, __ERR_SPRITE_CREATE_FAIL);
	}

	// assign text structure to sprite
	sprite->text = text;

	// fill in necessary info for sprite

	sfxSpriteChannel(sprite, channel_num);

	// set skip color to black, unless textcolor is black
	if (textcolor != 0)
		sprite->skipColor = Format565P((unsigned short)0x0);
	else
		sprite->skipColor = Format565P((unsigned short)0x0001);

	// save original text string, style, color, and pointsize in text sprite

	Common::strcpy_s(save_buffer, 256, outtext.c_str());
	sprite->text->text_string = save_buffer;
	sprite->text->textcolor = textcolor;
	sprite->text->textstyle = textstyle;
	sprite->text->pointsize = pointsize;
	sprite->text->fontID = CurrentFontID;

	sprite->changed = true;
	SRect_Copy(&sprite->oldRect, &sprite->rect);

	// select Spare Port's bitmap into Source DC

	winTextPort = (WinViewport *)SparePort;
	hBitmap = SelectObject(hSrcDC, winTextPort->hBitmap);

	// set background mode to transparent

	SetBkMode(hSrcDC, TRANSPARENT);

	// set required write area of Spare Port's bitmap to text transparent color
	if (textcolor != 0)
		memset(winTextPort->vp.ptr, 0x0,
			scene_width * height * (screen_colors == 8 ? 1 : 2));
	else {
		int i;
		WORD *ptr = (WORD *)winTextPort->vp.ptr;
		for (i = 0; i < scene_width * height; i++) {
			*ptr++ = 0x01;
		}
	}

	// set text color
	if (screen_colors == 8) {
		red = background->palette[textcolor].red;
		green = background->palette[textcolor].green;
		blue = background->palette[textcolor].blue;
		SetTextColor(hSrcDC, RGB(red, green, blue));
	} else
		SetTextColor(hSrcDC, textcolor);

	TextOut(hSrcDC, 0, 0, outtext.c_str(), outtext.size());

	// copy text from SparePort bitmap to text sprite's bitmap
	Port2Buffer16(textPort->ptr, SparePort, &textRect);

	// FOR DIRECT DRAW
	if (pixFormat)
		Format565(textPort);

	// set write area of Spare Port's bitmap to black
	memset(winTextPort->vp.ptr, 0x0,
		scene_width * height * (screen_colors == 8 ? 1 : 2));

	// select previous object back into hSrcDC
	SelectObject(hSrcDC, hBitmap);

	text_ids[slot].background = display_buffer;
	text_ids[slot].text = sprite;

	return (slot);
}

void sfxAddText2Pic(textNum text_num) {
	Sprite *text;

	if (text_num < 0 || text_num > MAX_TEXT_IDS - 1)
		ADV_ASSERT(false, __ERR_CODING);

	if (text_ids[text_num].background == -1)
		ADV_ASSERT(false, __ERR_CODING);

	text = text_ids[text_num].text;
	if (text)
		sfxSpriteAddToPic(text);
	else
		ADV_ASSERT(false, __ERR_CODING);

	text_ids[text_num].background = -1;
}

void sfxSetTextFont(int16 font_num, int16 pt_size, styleField style_field) {
	TEXTMETRIC tm;

	if (font_num < 0) return;

	if (hFont) DeleteObject(hFont);

	// set and save point size
	logfont.lfHeight = pt_size;
	pointsize = pt_size;

	// defaults
	logfont.lfWeight = FW_MEDIUM;
	logfont.lfUnderline = 0;
	logfont.lfItalic = 0;
	logfont.lfPitchAndFamily = DEFAULT_QUALITY | FF_DONTCARE;
	Common::strcpy_s(logfont.lfFaceName, "System");

	// specified
	if (style_field & STYL_BOLD)
		logfont.lfWeight = FW_BOLD;
	if (style_field & STYL_ITALIC)
		logfont.lfItalic = 1;
	if (style_field & STYL_UNDERLINE)
		logfont.lfUnderline = 1;

	// save font style
	textstyle = style_field;

	// set font id
	if (font_num == FNT_TIMES) {
		Common::strcpy_s(logfont.lfFaceName, "Times");
	}
	if (font_num == FNT_HELVETICA) {
		Common::strcpy_s(logfont.lfFaceName, "Helv");
		//Common::strcpy_s(logfont.lfFaceName, "Ariel");
	}
	if (font_num == FNT_SYSTEM) {
		Common::strcpy_s(logfont.lfFaceName, "System");
	}
	if (font_num == FNT_COURIER) {
		Common::strcpy_s(logfont.lfFaceName, "Courier");
	}
	if (font_num == FNT_SYMBOL) {
		Common::strcpy_s(logfont.lfFaceName, "Symbol");
	}

	hFont = CreateFontIndirect(&logfont);
	if (hFont)
	{
		int mode;
		mode = GetMapMode(hSrcDC);
		GetTextMetrics(hSrcDC, &tm);
		SelectObject(hSrcDC, hFont);
		CurrentFontID = font_num;
	} else
		CurrentFontID = -1;
}

int16 sfxGetTextFont(void) {
	return(CurrentFontID);
}

void sfxSetTextOutlineColor(uint32 color) {
	if (color < 0)
		outlinetextdemo = 0;
	else {
		if (color > 255)
			return;
		outlinetextcolor = color;
		outlinetextdemo = 1;
		shifttextdemo = 0;
	}
}

int32 sfxGetTextOutlineColor(void) {
	return(outlinetextcolor);
}

long sfxGetTextColor() {
	return textcolor;
}

void sfxSetTextColor(uint32 color_num) {
	if (screen_colors == 8 && (color_num < 0 || color_num > 255)) return;

	textcolor = color_num;   // save current text color num
}

void sfxChangeTextColor(textNum text_num, uint32 color_num) {
	Sprite *text_sprite;
	WinViewport *winTextPort;
	Background *background;
	HBITMAP hBitmap;
	unsigned char red, green, blue;
	COLORREF color = 0;
	SRect textRect;
	uint32 cur_color, cur_style;
	int16 cur_size, cur_fontnum;

	if (screen_colors == 8 && (color_num < 0 || color_num > 255)) return;

	if (text_num < 0 || text_num > MAX_TEXT_IDS - 1)
		ADV_ASSERT(false, __ERR_CODING);

	if (text_ids[text_num].background == -1)
		ADV_ASSERT(false, __ERR_CODING);

	// get pointer to text's background 
	background = backgrounds[text_ids[text_num].background];

	// get text sprite pointer
	text_sprite = text_ids[text_num].text;


	if (text_sprite->port->height > 32)
		ADV_ASSERT(0, 0);

	SRect_Init(&textRect, 0, 0, text_sprite->port->width - 1, text_sprite->port->height - 1);

	// select SparePort's bitmap into Source DC

	winTextPort = (WinViewport *)SparePort;
	hBitmap = SelectObject(hSrcDC, winTextPort->hBitmap);

	// set background mode to transparent

	SetBkMode(hSrcDC, TRANSPARENT);

	// reset text sprite's skip color
	if (textcolor != 0)
		text_sprite->skipColor = (short)0x0;
	else
		text_sprite->skipColor = (short)0x1F1F;

	// set required write area of Spare Port's bitmap to text transparent color
	if (textcolor != 0)
		memset(winTextPort->vp.ptr, 0x0,
			scene_width * text_sprite->port->height * (screen_colors == 8 ? 1 : 2));
	else
		memset(winTextPort->vp.ptr, 0x1F,
			scene_width * text_sprite->port->height * (screen_colors == 8 ? 1 : 2));

	// save current color, style, pointsize, and font.
	cur_color = textcolor;
	cur_style = textstyle;
	cur_size = pointsize;
	cur_fontnum = CurrentFontID;

	// set text color
	if (screen_colors == 8) {
		red = background->palette[color_num].red;
		green = background->palette[color_num].green;
		blue = background->palette[color_num].blue;
		SetTextColor(hSrcDC, RGB(red, green, blue));
	} else {
		color = SetTextColor(hSrcDC, color_num);
	}

	if (color == CLR_INVALID)
		ADV_ASSERT(false, __ERR_CODING);

	// save text's new color in sprite structure
	text_sprite->text->textcolor = color_num;

	// set font to this sprite's style, pointsize, and font id
	sfxSetTextFont(text_sprite->text->fontID,
		text_sprite->text->pointsize,
		text_sprite->text->textstyle);

	// draw text in new color
	TextOut(hSrcDC, 0, 0, text_sprite->text->text_string,
		strlen(text_sprite->text->text_string));

	// copy text from SparePort bitmap to text sprite's bitmap
	Port2Buffer16(text_sprite->port->ptr, SparePort, &textRect);

	// FOR DIRECT DRAW
	if (pixFormat)
		Format565(text_sprite->port);

	// set write area of Spare Port's bitmap to black
	memset(winTextPort->vp.ptr, 0x0,
		512 * text_sprite->port->height * (screen_colors == 8 ? 1 : 2));

	text_sprite->changed = true;

	SelectObject(hSrcDC, hBitmap);

	// reset font to saved, current values
	sfxSetTextFont(cur_fontnum, cur_size, cur_style);

}


void sfxSetTextStyle(styleField style_field) {
	textstyle = style_field;
}

styleField sfxGetTextStyle(void) {
	return(textstyle);
}

void sfxSetTextSize(int16 pt_size) {
	pointsize = pt_size;
	return;
}

int16 sfxGetTextSize(void) {
	return(pointsize);
}

uint16 sfxGetTextWidth(const char *text_ptr) {
	SIZE size;

	GetTextExtentPoint(hSrcDC, text_ptr, strlen(text_ptr), &size);
	return((uint16)size.cx);
}

uint16 sfxGetTextHeight(void) {
	TEXTMETRIC tm;

	GetTextMetrics(hSrcDC, &tm);
	return((uint16)(tm.tmHeight + tm.tmInternalLeading));
}

void sfxSetTextXY(MADEPt *baseline) {
	// save user's current x,y coords
	cur_user_text_x = baseline->x;
	cur_user_text_y = baseline->y;

}

void sfxGetTextXY(MADEPt *baseline_pt) {
	*(&baseline_pt->x) = cur_user_text_x;
	*(&baseline_pt->y) = cur_user_text_y;
}

} // namespace Spycraft
