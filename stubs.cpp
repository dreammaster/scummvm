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

#include "aesop/stubs.h"

namespace Aesop {

int int386(int intNum, REGS *inregs, REGS *outregs) {
	return 0;
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

bool aesop_isalnum(const char c) {
	return aesop_isalpha(c) || aesop_isdigit(c);
}

bool aesop_ispunct(const char c) {
	return !aesop_isalnum(c);
}

void PollMod() {
}

int kbhit() {
	return 0;
}

HTIMER AIL_register_timer(void(*fn)()) {
	return (HTIMER)0;
}
void AIL_set_timer_frequency(HTIMER timer, int freq) {}
void AIL_start_timer(HTIMER htimer) {}
void AIL_release_timer_handle(HTIMER htimer) {}
void AIL_shutdown() {}

}
