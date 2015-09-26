/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef AESOP_UTILS_H
#define AESOP_UTILS_H

#include "common/str.h"
#include "aesop/interpreter.h"

namespace Aesop {

/**
 * Convert string to number, returning -1 if not valid numeric string
 * Skip leading whitespace; handles unary -/+ operators
 *
 * Accepts binary numbers with '0b' prefix,
 * hexadecimal numbers with '0x' prefix; decimal numbers
 * handled via atol() library function for speed
 *
 * Accepts single ASCII characters with '\'' prefix
 */
extern int ascnum(const char *string);

extern bool aesop_isspace(const char c);
extern bool aesop_isalpha(const char c);
extern bool aesop_isdigit(const char c);
extern bool aesop_isnumprefix(const char c);
extern bool aesop_isalnum(const char c);
extern bool aesop_ispunct(const char c);

/**
 * Aesop version of vsprintf that accepts a Parameters array
 */
Common::String aesop_vsprintf(const Common::String &format, const Parameters &params);

} // End of namespace Aesop

#endif
