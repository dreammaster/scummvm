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

#ifndef SPYCRAFT_ADVDCMP_H
#define SPYCRAFT_ADVDCMP_H

namespace Spycraft {

/* MADE Internal */

typedef struct _DcmpInfo *DcmpStream;

DcmpStream OpenDcmpStream(int file, int size, int c_size, uint8 compressor);
int ReadDcmpStream(DcmpStream dStream, void *buffer, int size);
uint16 ReadWordDcmpStream(DcmpStream dStream);
uint32 ReadLongDcmpStream(DcmpStream dStream);
void CloseDcmpStream(DcmpStream dStream);

} // namespace Spycraft

#endif
