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

#ifndef SPYCRAFT_DMADE_IMPLODE_H
#define SPYCRAFT_DMADE_IMPLODE_H

namespace Spycraft {

unsigned int implode(
	unsigned int (*read_buf)(char *buf, unsigned int *size, void *param),
	void         (*write_buf)(char *buf, unsigned int *size, void *param),
	char *work_buf,
	void *param,
	unsigned int *type,
	unsigned int *dsize);


unsigned int explode(
	unsigned int (*read_buf)(char *buf, unsigned  int *size, void *param),
	void         (*write_buf)(char *buf, unsigned  int *size, void *param),
	char *work_buf,
	void *param);

unsigned long crc32(char *buffer, unsigned int *size, unsigned long *old_crc);

#define CMP_BUFFER_SIZE    36312
#define EXP_BUFFER_SIZE    12596

#define CMP_BINARY             0
#define CMP_ASCII              1

#define CMP_NO_ERROR           0
#define CMP_INVALID_DICTSIZE   1
#define CMP_INVALID_MODE       2
#define CMP_BAD_DATA           3
#define CMP_ABORT              4

} // namespace Spycraft

#endif
