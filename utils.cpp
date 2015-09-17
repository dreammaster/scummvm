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

#include "common/scummsys.h"
#include "aesop/aesop.h"
#include "aesop/utils.h"
#include "aesop/shared.h"

namespace Aesop {

int ascnum(const char *string) {
	LONG i, j, len, base, neg, chr;
	LONG total;

	while (aesop_isspace(*string)) string++;

	neg = 0;
	switch (*string) {
	case '-':
		neg = 1;
		string++;
		break;
	case '+':
		string++;
		break;
	}

	if (*string == '\'')
		return (LONG)(*(string + 1));

	switch (READ_LE_UINT16(string)) {
	case 'x0':
		base = 16;
		string += 2;
		break;
	case 'b0':
		base = 2;
		string += 2;
		break;
	default:
		base = 10;
		break;
	}

	if (base == 10) {
		if (aesop_isdigit(*string))
			return neg ? -atol((const char *)string) : atol((const char *)string);
		else
			return -1;
	}
	total = 0L;
	len = strlen((const char *)string);

	for (i = 0; i < len; i++) {
		chr = toupper(string[i]);

		for (j = 0; j < base; j++)
			if (chr == "0123456789ABCDEF"[j]) {
				total = (total * base) + j;
				break;
			}

		if (j == base) return -1;
	}

	return total;
}

bool aesop_isspace(const char c) {
	return c == ' ' || c == '\t';
}

extern bool aesop_isalpha(const char c) {
	return toupper(c) >= 'A' && toupper(c) <= 'Z';
}

bool aesop_isdigit(const char c) {
	return c >= '0' && c <= '9';
}

bool aesop_isnumprefix(const char c) {
	return (c == '-') || (c >= '0' && c <= '9');
}

bool aesop_isalnum(const char c) {
	return aesop_isalpha(c) || aesop_isdigit(c);
}

bool aesop_ispunct(const char c) {
	return !aesop_isalnum(c);
}

void dprint(LONG argcnt, const char *format, ...) {
	va_list argptr;

	va_start(argptr, format);

	_vm->_screen->textWindows(0).textColor(15, DK_RED);
	_vm->_screen->textWindows(0).textColor(0, 255);
	_vm->_screen->textWindows(0).vsprint(format, argptr);

	va_end(argptr);
}

void aprint(LONG argcnt, const char *format, ...) {
	error("TODO: aprint with Paramater array");
	/*
	va_list args;
	char buffer[100];

	va_start(args, format);
	vsnprintf(buffer, 100, format, args);
	va_end(args);

	debug(buffer);
	*/
}


} // End of namespace Aesop

