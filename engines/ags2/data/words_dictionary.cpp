
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

#include "common/str.h"
#include "ags2/data/words_dictionary.h"

namespace AGS2 {

void WordsDictionary::sort() {
	int aa, bb;
	for (aa = 0; aa < num_words; aa++) {
		for (bb = aa + 1; bb < num_words; bb++) {
			if (((wordnum[aa] == wordnum[bb]) && (scumm_stricmp(word[aa], word[bb]) > 0))
				|| (wordnum[aa] > wordnum[bb])) {
				short temp = wordnum[aa];
				char tempst[30];

				wordnum[aa] = wordnum[bb];
				wordnum[bb] = temp;
				Common::strcpy_s(tempst, word[aa]);
				Common::strcpy_s(word[aa], 3000, word[bb]);
				Common::strcpy_s(word[bb], 3000, tempst);
				bb = aa;
			}
		}
	}
}

int WordsDictionary::find_index(const char *wrem) {
	int aa;
	for (aa = 0; aa < num_words; aa++) {
		if (scumm_stricmp(wrem, word[aa]) == 0)
			return aa;
	}
	return -1;
}

} // namespace AGS2
