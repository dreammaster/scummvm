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

#ifndef LEGEND_RESOURCES_H
#define LEGEND_RESOURCES_H

#include "common/str.h"

namespace Legend {

enum FileType {
	FILETYPE_PIC = 0,
	FILETYPE_RGN = 1,
	FILETYPE_FNT = 2,
	FILETYPE_MUS = 3,
	FILETYPE_SAV = 4,
	FILETYPE_SAV2 = 5,
	FILETYPE_VOC = 6,
	FILETYPE_RS = 7
};

class LegendEngine;

class Resources {
private:
	LegendEngine *_vm;
	Common::String _prefix;
public:
	Resources(LegendEngine *vm);

	/**
	 * Form a filename for a specified file type and number
	 */
	Common::String getFilename(FileType fileType, int fileNumber);
};

} // End of namespace Legend

#endif
