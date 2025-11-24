
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

#ifndef AGS2_DATA_WORDS_DICTIONARY_H
#define AGS2_DATA_WORDS_DICTIONARY_H

#include "ags2/data/defines.h"

namespace AGS2 {

struct WordsDictionary {
	int num_words = 0;
	char **word = nullptr;
	short *wordnum = nullptr;

	void allocate_memory(int wordCount) {
		num_words = wordCount;
		if (num_words > 0)
		{
			word = (char **)malloc(wordCount * sizeof(char *));
			word[0] = (char *)malloc(wordCount * MAX_PARSER_WORD_LENGTH);
			wordnum = (short *)malloc(wordCount * sizeof(short));
			for (int i = 1; i < wordCount; i++)
			{
				word[i] = word[0] + MAX_PARSER_WORD_LENGTH * i;
			}
		}
	}

	void free_memory() {
		if (num_words > 0) {
			free(word[0]);
			free(word);
			free(wordnum);
			word = NULL;
			wordnum = NULL;
			num_words = 0;
		}
	}

	void sort();
	int find_index(const char *);
};

} // namespace AGS2

#endif
