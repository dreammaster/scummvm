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

#include "ags2/ac/room.h"
#include "ags2/ac/acruntime.h"

namespace AGS2 {

#define SCRIPT_CONFIG_VERSION 1

long getlong(Common::SeekableReadStream *iii) {
	return iii->readSint32LE();
}

void save_script_configuration(Common::WriteStream *iii) {
	quit("ScriptEdit: run-time version can't save");
}

void load_script_configuration(Common::SeekableReadStream *iii) {
	int aa;
	if (getlong(iii) != SCRIPT_CONFIG_VERSION)
		quit("ScriptEdit: invliad config version");

	int numvarnames = getlong(iii);
	for (aa = 0; aa < numvarnames; aa++) {
		int lenoft = iii->readByte();
		iii->seek(lenoft, SEEK_CUR);
	}
}

void save_graphical_scripts(Common::WriteStream *fff, roomstruct *rss) {
	quit("ScriptEdit: run-time version can't save");
}

static const char *scripttempn = "~acsc%d.tmp";
extern int route_script_link();

void load_graphical_scripts(Common::SeekableReadStream *iii, roomstruct *rst) {
#ifdef TODO
	long ct;
	Common::File te;

	if (route_script_link()) {
		error("STOP IT.");
	}

	while (1) {
		ct = iii->readSint32LE();
		if ((ct == -1) || iii->eos())
			break;

		long lee = iii->readSint32LE();
		char thisscn[20];
		Common::sprintf_s(thisscn, scripttempn, ct);

		te.open(thisscn);

		char *scnf = (char *)malloc(lee);
		// MACPORT FIX: swap size and nmemb
		iii->read(scnf, lee);
		fwrite(scnf, sizeof(char), lee, te);
		te.close();

		free(scnf);
	}
#else
	error("TODO");
#endif
}

} // namespace AGS2
