/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
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

#include "glk/zcode/glk/sf_frotz.h"
#include "common/textconsole.h"

namespace Glk {
namespace ZCode {

/* REG_EIP does not exist on 64bit CPU */
#if defined(__amd64__) || defined (__x86_64__)
#define _PROG_COUNTER REG_RIP
#else
#define _PROG_COUNTER REG_EIP
#endif

#ifdef TODO
static struct {
	int sig;
	const char *name;
} NAMES[] = {
	{SIGSEGV, "SIGSEGV"},
	{SIGFPE, "SIGFPE"},
	{SIGILL, "SIGILL"},
	{0, nullptr}
};
#endif

#ifdef TODO
static const char *getsigname(int s) {
	int i = 0;
	while (NAMES[i].name) {
		if (NAMES[i].sig == s)
			return NAMES[i].name;
		i++;
	}
	return nullptr;
}
#endif

void sf_installhandlers() {
	// No implementation
}

} // End of namespace ZCode
} // End of namespace Glk
