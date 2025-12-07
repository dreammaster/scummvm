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

#include "spycraft/game/print.h"
#include "spycraft/game/colby.h"

namespace Spycraft {

extern Colby colby;
extern int currentFocus;
Print::Print() {
	name = "Print";
	font = FNT_HELVETICA;
	style = STYL_BOLD;
	size = 18;
	fore_color = madeRGB(240, 191, 91);
	//	back_color = 0;
	high_color = madeRGB(217, 217, 217);
	x = 100;
	y = 200;
	priority = 400;
	textBuffer = -1;
	myString = NULL;
}

bool Print::init(const char *format) {
	MADEPt baseline;
	baseline.x = x;
	baseline.y = y;

	int32 curForeColor = sfxGetTextColor();
	int32 curBackColor = sfxGetTextOutlineColor();

	//sfxPrintf("x: %d, y: %d", baseline.x, baseline.y);

	sfxSetTextFont(font, size, style);

	if (curForeColor != fore_color)
		sfxSetTextColor(fore_color);

	if (curBackColor != back_color)
		sfxSetTextOutlineColor(back_color);

	sfxSetTextXY(&baseline);
	myString = format;

	textBuffer = sfxDrawText(theGame->display_buffer, priority, format);
	height = sfxGetTextHeight();
	width = sfxGetTextWidth(format);

	return(true);
}

bool Print::init(void) {
	MADEPt baseline;
	baseline.x = x;
	baseline.y = y;
	int32 curForeColor = sfxGetTextColor();
	int32 curBackColor = sfxGetTextOutlineColor();

	sfxSetTextFont(font, size, style);

	if (curForeColor != fore_color)
		sfxSetTextColor(fore_color);

	if (curBackColor != back_color)
		sfxSetTextOutlineColor(back_color);

	sfxSetTextXY(&baseline);

	if (myString) {
		textBuffer = sfxDrawText(theGame->display_buffer, priority, myString);
		height = sfxGetTextHeight();
		width = sfxGetTextWidth(myString);
	} else {
		sfxPrintf("No String attached to Print");
		return false;
	}
	return(true);
}

bool Print::highlight(bool hiOrLow) {
	if (!myString || textBuffer < 0) {
		sfxPrintf("No String attached to Print");
		return false;
	}

	if (hiOrLow)
		sfxChangeTextColor(textBuffer, high_color);
	else
		sfxChangeTextColor(textBuffer, fore_color);

	return true;
}

void Print::doit(void) {
	//  sfxPrintf("Print doit"); //DEBUG
}

void Print::erase(void) {
	if (textBuffer >= 0) {
		sfxKillText(textBuffer);
		textBuffer = -1;
		//	myString = NULL;  /may want to show myString by initing
	}
}

void Print::addToPic(void) {
	if (textBuffer >= 0)
	{
		sfxAddText2Pic(textBuffer);
		textBuffer = -1;
	}
}

void Print::dispose(void) {
	erase();
	delete this;
}

} // namespace Spycraft
