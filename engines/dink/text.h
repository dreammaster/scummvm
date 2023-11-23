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

#ifndef DINK_TEXT_H
#define DINK_TEXT_H

namespace Dink {

extern void Msg(const char *fmt, ...);
extern void TRACE(const char *fmt, ...);
extern void dderror(int err);
extern void get_word(char line[300], int word, char *crap);
extern int var_figure(char h[200], int script);
extern void kill_text_owned_by(int sprite);
extern bool text_owned_by(int sprite);
extern int say_text(const char *text, int h, int script);
extern int say_text_xy(const char *text, int mx, int my, int script);

/**
 * This acts in the same way as dink_strcat except it combines
 * a string an a single character, updating the null at the end.
 */
extern void strchar(char *string, char ch);

extern void replace(const char *this1, const char *that, char *line);
extern bool compare(const char *orig, const char *comp);
//redink1 added this prototype to fix this save info declaration error thing
extern void decipher_string(char line[200], int script);
extern bool seperate_string(const char *str, int num, char liney, char *return1);
extern void strip_beginning_spaces(char *s);

} // namespace Dink

#endif
