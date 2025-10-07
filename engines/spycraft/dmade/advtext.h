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

#ifndef SPYCRAFT_DMADE_ADVTEXT_H
#define SPYCRAFT_DMADE_ADVTEXT_H

#include "spycraft/dmade/advback.h"
#include "spycraft/dmade/advsprite.h"
#include "spycraft/dmade/advres.h"

namespace Spycraft {

/* MADE External */

// text colors
#define TEXT_COLOR_BLACK    madeRGB(0, 0, 0)
#define TEXT_COLOR_RED      madeRGB(255, 0, 0)
#define TEXT_COLOR_GREEN    madeRGB(0, 255, 0)
#define TEXT_COLOR_BLUE     madeRGB(0, 0, 255)
#define TEXT_COLOR_WHITE    madeRGB(255, 255, 255)

// font ids
#define FNT_SYSTEM			0
#define FNT_TIMES			1
#define FNT_HELVETICA		2
#define FNT_COURIER			3
#define FNT_CHICAGO			4
#define FNT_GARAMOND		5
#define FNT_GENEVA			6
#define FNT_PALATINO		7
#define FNT_SYMBOL			8
#define FNT_MONACO			9
#define FNT_ZCHANCERY		10
#define FNT_ZDINGBATS		11

// Style types
#define	STYL_PLAIN			0
#define	STYL_BOLD			(1 << 0)
#define	STYL_ITALIC			(1 << 1)
#define	STYL_UNDERLINE		(1 << 2)

#define	NO_OUTLINE_COLOR	0xFFFFFFFF

typedef int16 textNum;			// magic number of text item
typedef uint32 styleField;		// bitfield describing current style options

/*********************** Text Functions **********************/

//	Function:	sfxGetTextSprite
//
//	Purpose:	Returns text sprite pointer of given textNum ID
//
//	Parameters:	textNum ID

extern Sprite *sfxGetTextSprite(textNum text_num);


//	Function:	sfxSetTextFont
//
//	Purpose:	Sets the text font, size, and style
//
//	Parameters:	font_num - Can be one of these values:
//					FNT_TIMES
//					FNT_HELVETICA
//					FNT_COURIER
//					FNT_CHICAGO
//					FNT_MONACO
//					FNT_GENEVA
//					FNT_PALATINO
//					FNT_SYMBOL
//					FNT_ZCHANCERY
//					FNT_ZDINGBATS
//				style_field - can be either
//					STYL_PLAIN
//						or can have any combination of
//					STYL_BOLD
//					STYL_ITALIC
//					STYL_UNDERLINE					
//						set.
//
//	Returns:	void

extern void sfxSetTextFont(int16 font_num, int16 pt_size, styleField style_field);


//	Function:	sfxSetTextSize
//
//	Purpose:	Sets the current font size
//
//	Parameters:	pt_size - size of the font, in points
//
//	Returns:	void

extern void sfxSetTextSize(int16 pt_size);


//	Function:	sfxSetTextStyle
//
//	Purpose:	Sets the current font style as specified in a bitfield
//
//	Parameters:	style_field - can be either
//					STYL_PLAIN
//						or can have any combination of
//					STYL_BOLD
//					STYL_ITALIC
//					STYL_UNDERLINE					
//						set.
//
//	Returns:	void

extern void sfxSetTextStyle(styleField style_field);

//	Function:	sfxChangeTextColor
//
//	Purpose:	Changes the color of an existing line of text.
//
//	Parameters:	text_num - magic number of text line 
//	           	color_num - number of the color in the universal palette.
//
//	Returns:	void

extern void sfxChangeTextColor(textNum text_num, uint32 color_num);

//	Function:	sfxSetTextColor
//
//	Purpose:	Sets the text color.
//
//	Parameters:	color_num - number of the color in the universal palette.
//
//	Returns:	void

extern void sfxSetTextColor(uint32 color_num);


//	Function:	sfxSetTextOutlineColor
//
//	Purpose:	Sets the text outline color.
//
//	Parameters:	color_num - number of the color in the universal palette. NO_OUTLINE_COLOR
//					to turn outline colors off.
//
//	Returns:	void

extern void sfxSetTextOutlineColor(uint32 color_num);


//	Function:	sfxSetTextXY
//
//	Purpose:	Sets the coordinates for text writes.
//
//	Parameters:	baseline_pt - lower left corner of text field
//
//	Returns:	void

extern void sfxSetTextXY(MADEPt *baseline_pt);


//	Function:	sfxSetTextClip
//
//	Purpose:	Sets the clip rect. Any text outside this rectangle is not drawn.
//
//	Parameters:	clip_rect - pointer to the clip rect
//
//	Returns:	void

extern void sfxSetTextClip(MADERect *clip_rect);


//	Function:	sfxGetTextFont
//
//	Purpose:	Returns the current font
//
//	Parameters:	void
//
//	Returns:	the number of the current font

extern int16 sfxGetTextFont(void);


//	Function:	sfxGetTextSize
//
//	Purpose:	Returns the current font size, in points
//
//	Parameters:	void
//
//	Returns:	the point size of the current font

extern int16 sfxGetTextSize(void);


//	Function:	sfxGetTextStyle
//
//	Purpose:	Returns the current font style in a bitfield
//
//	Parameters:	void
//
//	Returns:	a bitfield of the current style

extern styleField sfxGetTextStyle(void);


//	Function:	sfxGetTextColor
//
//	Purpose:	Returns the current text color.
//
//	Parameters:	void
//
//	Returns:	number of the color in the universal palette.

extern int32 sfxGetTextColor(void);


//	Function:	sfxGetTextOutlineColor
//
//	Purpose:	Returns the current text outline color.
//
//	Parameters:	void
//
//	Returns:	current color of the outline, NO_OUTLINE_COLOR if outline is off

extern int32 sfxGetTextOutlineColor(void);


//	Function:	sfxGetTextXY
//
//	Purpose:	Returns the current coordinates for text writes.
//
//	Parameters:	baseline_pt - a pointer to the structure to be filled in
//
//	Returns:	the current baseline in baseline_pt

extern void sfxGetTextXY(MADEPt *baseline_pt);


//	Function:	sfxGetTextClip
//
//	Purpose:	Gets the current clip rect.
//
//	Parameters:	none
//
//	Returns:	the clip rectangle in text_rect

extern void sfxGetTextClip(MADERect *text_rect);


//	Function:	sfxGetTextWidth
//
//	Purpose:	Returns the onscreen width of a string when drawn
//
//	Parameters:	text_ptr - pointer to the string to draw
//
//	Returns:	the width of the text in virtual coordinates

extern uint16 sfxGetTextWidth(const char *text_ptr);


//	Function:	sfxGetTextHeight
//
//	Purpose:	Returns the onscreen Height of currently selected font
//
//	Parameters:	void
//
//	Returns:	the Height of the text in virtual coordinates

extern uint16 sfxGetTextHeight(void);

//	Function:	sfxDrawText
//
//	Purpose:	Draws text in the current font and style.
//
//	Parameters 	buffer_num - magic number of background text belongs to
//	           	channel_num - channel in requested background
//	           	format - same as printf in the standard C libs.
//
//	Returns:	textNum - magic number of text line

extern textNum sfxDrawText(bufferNum buffer_num, channelNum channel_num,
	const char *format, ...);

//	Function:	sfxKillText
//
//	Purpose:	Erases text line
//
//	Parameters 	text_num - magic number of text line
//
//	Returns:	void

extern void sfxKillText(textNum text_num);


//	Function:	sfxDrawTextRsrc
//
//	Purpose:	Draws text from the project file in the current font and style.
//
//	Parameters 	buffer_num - magic number of background text belongs to
//	           	channel_num - channel in requested background
//	           	text_num - number of the text in the project file
//
//	Returns:	textNum - magic number of text item

extern textNum sfxDrawTextRsrc(bufferNum buffer_num, channelNum channel_num,
	rsrcID text_num);


//	Function:	sfxPlaceTextRsrc
//
//	Purpose:	Draws text from the project file in the 
//				specified font, style, and location.
//
//	Parameters 	buffer_num - magic number of background text belongs to
//	           	channel_num - channel in requested background
//	           	text_num - number of the text resource in the project.
//				baseline - pointer to the current baseline point
//				font_num - number of the font to use
//				pt_size - the size of the font, in points
//				style_field - bitfield of the style to draw the text in
//				color_num - number of the text color in the universal palette
//				outline_color_num - number of the outline text color in the
//									universal palette. NO_OUTLINE_COLOR for
//									no outline color.
//
//	Returns:	textNum - magic number of text item

extern textNum sfxPlaceTextRsrc(bufferNum buffer_num, channelNum channel_num,
	rsrcID text_num, MADEPt *baseline, int16 font_num,
	int16 pt_size, styleField style_field,
	int32 color_num, int32 outline_color_num);

//	Function:	sfxGetTextPtr
//
//	Purpose:	gets the contents of a text resource from the project file
//				as a pointer.
//
//	Parameters:	text_num - number of the text resource in the project.
//				buffer - the address of a char*. In other words, you would
//						declare a pointer
//								char *my_ptr;
//
//						and then you would call this function
//								sfxGetTextPtr(rsrc_num, &my_ptr)
//
//	Returns:	the size of the buffer, in bytes, and puts a pointer
//				to the first byte of the buffer in buffer, else an error,
//				and buffer is null
//
//									NOTE:
//				To deallocate the pointer returned by this call, use
//				sfxKillBuffer, described below.
//

extern MADEErr32 sfxGetTextPtr(rsrcID text_num, char **buffer);

extern void sfxAddText2Pic(textNum text_num);

/* MADE Interal */

extern void InitText();
extern void CleanText();
extern void FreeTextSlots(int bufferNum);

} // namespace Spycraft

#endif
