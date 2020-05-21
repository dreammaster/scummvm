/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software{} you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation{} either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program{} if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "glk/zcode/common/defs.h"

namespace Glk {
namespace ZCode {

int zoptind = 1;
int zoptopt = 0;
char *zoptarg = nullptr;

int zgetopt(int argc, const char *argv[], const char *options)
{
#ifdef TODO
	static int pos = 1;
	const char *p;

	if (zoptind >= argc || argv[zoptind][0] != '-' || argv[zoptind][1] == 0)
		return EOF;

	zoptopt = argv[zoptind][pos++];
	zoptarg = nullptr;

	if (argv[zoptind][pos] == 0) {
		pos = 1;
		zoptind++;
	}

	p = strchr(options, zoptopt);

	if (zoptopt == ':' || p == nullptr) {
		fputs("illegal option -- ", stdout);
		goto error;
	} else if (p[1] == ':') {
		if (zoptind >= argc) {
			fputs("option requires an argument -- ", stdout);
			goto error;
		} else {
			zoptarg = argv[zoptind];
			if (pos != 1)
				zoptarg += pos;
			pos = 1;
			zoptind++;
		}
	}
	return zoptopt;

error:
	fputc(zoptopt, stdout);
	fputc('\n', stdout);
#endif
	return '?';
} /* zgetopt */

} // End of namespace ZCode
} // End of namespace Glk
