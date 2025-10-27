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

#ifndef AGS2_COMMON_COMPRESS_H
#define AGS2_COMMON_COMPRESS_H

#include "common/stream.h"

namespace AGS2 {

typedef unsigned char /* far */ *__block;

extern long csavecompressed(const char *finam, __block tobesaved, color pala[256], long exto);

extern void cpackbitl(unsigned char *line, int size, Common::WriteStream *outfile);
extern void cpackbitl16(unsigned short *line, int size, Common::WriteStream *outfile);
extern void cpackbitl32(unsigned long *line, int size, Common::WriteStream *outfile);
extern int  cunpackbitl(unsigned char *line, int size, Common::SeekableReadStream *infile);
extern int  cunpackbitl16(unsigned short *line, int size, Common::SeekableReadStream *infile);
extern int  cunpackbitl32(unsigned long *line, int size, Common::SeekableReadStream *infile);

} // namespace AGS2

#endif
