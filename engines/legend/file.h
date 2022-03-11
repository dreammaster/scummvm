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

#ifndef LEGEND_FILE_H
#define LEGEND_FILE_H

#include "common/stream.h"

namespace Legend {

typedef Common::SeekableReadStream Stream;

extern Stream *fp_open(const char *filename, const char *mode = "rb");
extern void fp_close(Stream *fp);
extern size_t fp_size(Stream *fp);
extern void fp_set_bufsiz(Stream *fp, size_t bufSize);
extern size_t fp_read(Stream *fp, void *buf, size_t size);
extern int fp_seek(Stream *fp, int offset, int origin);

} // namespace Legend

#endif
