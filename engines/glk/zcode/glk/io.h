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

#ifndef GLK_ZCODE_GLK_IO_H
#define GLK_ZCODE_GLK_IO_H

#include "glk/zcode/common/defs.h"

namespace Glk {
namespace ZCode {

#undef strdup
#define strdup scumm_strdup
#undef strcasecmp
#define strcasecmp scumm_stricmp

#undef FILE
#define FILE Common::Stream
#undef fopen
extern Common::Stream *fopen(const char *filename, const char *mode);
#undef fseek
extern int fseek(Common::Stream *stream, long int offset, int origin);
#undef ftell
extern int32 ftell(Common::Stream *stream);
#undef feof
extern int feof(FILE *stream);
#undef fread
extern size_t fread(void *ptr, size_t size, size_t count, Common::Stream *stream);
#undef fgetc
extern int fgetc(Common::Stream *stream);
#undef fungetc
extern int ungetc(int character, Common::Stream *stream);
#undef fgets
extern char *fgets(char *str, int num, FILE *stream);

#undef fputc
extern int fputc(int character, Common::Stream *stream);
#undef fwrite
extern size_t fwrite(const void *ptr, size_t size, size_t count, Common::Stream *stream);
#undef fclose
extern int fclose(Common::Stream *&stream);
#undef ferror
extern bool ferror(Common::Stream *stream);

} // End of namespace ZCode
} // End of namespace Glk

#endif
