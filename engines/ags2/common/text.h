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

#ifndef AGS2_COMMON_TEXT_H
#define AGS2_COMMON_TEXT_H

#include "common/str.h"

namespace AGS2 {

extern void split_lines_rightleft(char *todis, int wii, int fonnt);
extern char *reverse_text(const char *text);
extern void wouttext_reverseifnecessary(int x, int y, int font, char *text);
extern void break_up_text_into_lines(int wii, int fonnt, char *todis);

} // namespace AGS2

#endif
