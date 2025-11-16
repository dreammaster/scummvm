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

#include "ags2/common/text.h"
#include "ags2/vars.h"

namespace AGS2 {

void split_lines_rightleft(char *todis, int wii, int fonnt) {
	// start on the last character
	char *thisline = todis + strlen(todis) - 1;
	char prevlwas = 0, *prevline = NULL;
	// work backwards
	while (thisline >= todis) {

		int needBreak = 0;
		if (thisline <= todis)
			needBreak = 1;
		// ignore \[ sequence
		else if ((thisline > todis) && (thisline[-1] == '\\')) {
		} else if (thisline[0] == '[') {
			needBreak = 1;
			thisline++;
		} else if (wgettextwidth_compensate(thisline, fonnt) >= wii) {
			// go 'back' to the nearest word
			while ((thisline[0] != ' ') && (thisline[0] != 0))
				thisline++;

			if (thisline[0] == 0)
				quit("!Single word too wide for window");

			thisline++;
			needBreak = 1;
		}

		if (needBreak) {
			Common::strcpy_s(lines[numlines], thisline);
			removeBackslashBracket(lines[numlines]);
			numlines++;
			if (prevline) {
				prevline[0] = prevlwas;
			}
			thisline--;
			prevline = thisline;
			prevlwas = prevline[0];
			prevline[0] = 0;
		}

		thisline--;
	}
	if (prevline)
		prevline[0] = prevlwas;
}

char *reverse_text(const char *text) {
	int stlen = strlen(text), rr;
	char *backwards = (char *)malloc(stlen + 1);
	for (rr = 0; rr < stlen; rr++)
		backwards[rr] = text[(stlen - rr) - 1];
	backwards[stlen] = 0;
	return backwards;
}

void wouttext_reverseifnecessary(int x, int y, int font, char *text) {
	char *backwards = NULL;
	char *otext = text;
	if (game.options[OPT_RIGHTLEFTWRITE]) {
		backwards = reverse_text(text);
		otext = backwards;
	}

	wouttext_outline(x, y, font, otext);

	if (backwards)
		free(backwards);
}

void break_up_text_into_lines(int wii, int fonnt, char *todis) {
	if (fonnt == -1)
		fonnt = play.normal_font;

	//  char sofar[100];
	if (todis[0] == '&') {
		while ((todis[0] != ' ') & (todis[0] != 0)) todis++;
		if (todis[0] == ' ') todis++;
	}
	numlines = 0;
	longestline = 0;

	// Don't attempt to display anything if the width is tiny
	if (wii < 3)
		return;

	int rr;

	if (game.options[OPT_RIGHTLEFTWRITE] == 0)
	{
		split_lines_leftright(todis, wii, fonnt);
	} else {
		// Right-to-left just means reverse the text then
		// write it as normal
		char *backwards = reverse_text(todis);
		split_lines_rightleft(backwards, wii, fonnt);
		free(backwards);
	}

	for (rr = 0; rr < numlines; rr++) {
		if (wgettextwidth_compensate(lines[rr], fonnt) > longestline)
			longestline = wgettextwidth_compensate(lines[rr], fonnt);
	}
}

} // namespace AGS2
