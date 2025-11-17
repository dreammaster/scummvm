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

#ifndef AGS2_COMMON_TREE_MAP_H
#define AGS2_COMMON_TREE_MAP_H

#include "common/scummsys.h"

namespace AGS2 {

/**
 * Binary tree structure for holding translations, allows fast access
 */
struct TreeMap {
	TreeMap *left, *right;
	char *text;
	char *translation;

	TreeMap() {
		left = nullptr;
		right = nullptr;
		text = nullptr;
		translation = nullptr;
	}

	char *findValue(const char *sKey) {
		if (text == nullptr)
			return nullptr;

		if (strcmp(sKey, text) == 0)
			return translation;
		//debug_log("Compare: '%s' with '%s'", sKey, text);

		if (strcmp(sKey, text) < 0) {
			if (left == nullptr)
				return nullptr;
			return left->findValue(sKey);
		} else {
			if (right == nullptr)
				return nullptr;
			return right->findValue(sKey);
		}
	}

	void addText(const char *ntx, char *trans) {
		if ((ntx == nullptr) || (ntx[0] == 0) ||
			((text != nullptr) && (strcmp(ntx, text) == 0)))
			// don't add if it's an empty string or if it's already here
			return;

		if (text == nullptr) {
			text = (char *)malloc(strlen(ntx) + 1);
			translation = (char *)malloc(strlen(trans) + 1);
			if (translation == nullptr)
				quit("load_translation: out of memory");
			Common::strcpy_s(text, 3000, ntx);
			Common::strcpy_s(translation, 3000, trans);
		} else if (strcmp(ntx, text) < 0) {
			// Earlier in alphabet, add to left
			if (left == nullptr)
				left = new TreeMap();

			left->addText(ntx, trans);
		} else if (strcmp(ntx, text) > 0) {
			// Later in alphabet, add to right
			if (right == nullptr)
				right = new TreeMap();

			right->addText(ntx, trans);
		}
	}

	void clear() {
		if (left) {
			left->clear();
			delete left;
		}
		if (right) {
			right->clear();
			delete right;
		}
		if (text)
			free(text);
		if (translation)
			free(translation);
		left = nullptr;
		right = nullptr;
		text = nullptr;
		translation = nullptr;
	}

	~TreeMap() {
		clear();
	}
};

} // namespace AGS2

#endif
