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

#include "common/textconsole.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advdebug.h"
#include "spycraft/dmade/winmade.h"
#include "spycraft/spycraft.h"

namespace Spycraft {

extern int releaseVersion;
extern char *debugText;

void pAssert(int theLine, const char *theFile, int theError) {
	if (releaseVersion) {
		if (debugText) {
			error("Error 0x(%x) %s", theError, debugText);
		}
	} else {
		error("Error 0x(%x) occurred at line %d in %s", theError, theLine, theFile);
	}
}

void dbgmsg(const char *sz, ...) {
	va_list va;
	va_start(va, sz);
	Common::String str = Common::String::format(sz, va);
	va_end(va);

	warning("%s", str.c_str());
}

void ErrMsg(const char *sz, ...) {
	va_list va;
	va_start(va, sz);
	Common::String str = Common::String::format(sz, va);
	va_end(va);

	error("%s", str.c_str());
}

int sfxPrintf(const char *format, ...) {
	va_list va;
	va_start(va, format);
	Common::String str = Common::String::format(format, va);
	va_end(va);

	debug("%s", str.c_str());
	return 0;
}

int AlertMsg(const char *title, const char *sz, ...) {
	va_list va;
	va_start(va, sz);
	Common::String str = Common::String::format(sz, va);
	va_end(va);

	g_engine->guiMessage(str);
	return 0;
}

} // namespace Spycraft
