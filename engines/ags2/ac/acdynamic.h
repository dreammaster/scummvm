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

#ifndef AGS2_AC_ACDYNAMIC_H
#define AGS2_AC_ACDYNAMIC_H

#include "ags2/common/cscomp.h"

namespace AGS2 {

struct AGSCCDynamicObject : ICCDynamicObject {
public:
	// default implementation
	virtual int Dispose(const char *address, bool force);

	virtual void Unserialize(int index, const char *serializedData, int dataSize) = 0;

protected:
	int bytesSoFar;
	int totalBytes;
	char *serbuffer;

	void StartSerialize(char *sbuffer);
	void SerializeInt(int val);
	int  EndSerialize();
	void StartUnserialize(const char *sbuffer, int pTotalBytes);
	int  UnserializeInt();

};

} // namespace AGS2

#endif
