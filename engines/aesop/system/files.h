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

#ifndef AESOP_SYSTEM_FILES_H
#define AESOP_SYSTEM_FILES_H

#include "common/stream.h"

namespace Aesop {

#define O_RDONLY 1
#define O_BINARY 2
#define O_CREAT 4
#define O_RDWR 8
#define O_TRUNC 16
#define O_APPEND 32

#define ATF_WRITE 1
#define ATF_READ 2

extern Common::Stream *open(const char *filename, int flags);
extern void close(Common::Stream *&file);
extern size_t read(Common::Stream *file, void *buffer, size_t size);
extern size_t write(Common::Stream *file, const void *buffer, size_t size);
extern int32 lseek(Common::Stream *file, off_t offset, int whence);
extern size_t fsize(Common::Stream *file);
extern int32 tell(Common::Stream *file);
extern void fprintStr(Common::Stream *file, const char *msg, ...);

extern void *FILE_read(const char *filename, void *dest);

} // namespace Aesop

#endif
