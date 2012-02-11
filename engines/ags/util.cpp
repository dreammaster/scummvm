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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "common/stream.h"
#include "common/textconsole.h"

namespace AGS {

Common::String readString(Common::SeekableReadStream *dta) {
	Common::String str;
	while (true) {
		char c = (char)dta->readByte();
		if (!c)
			break;
		str += c;
	}
	return str;
}

void decryptText(uint8 *str, uint32 max) {
	static const char *kSecretPassword = "Avis Durgan";

	uint passPos = 0;
	while (max-- > 0) {
		*str -= (uint8) kSecretPassword[passPos];
		if (!*str)
			break;

		str++;

		passPos = (passPos + 1) % 11;
	}
}

Common::String decryptString(Common::SeekableReadStream *dta) {
	uint32 stringLen = dta->readUint32LE();
	if (stringLen > 5000000)
		error("invalid value in decryptString");
	byte *string = new byte[stringLen + 1];
	dta->read(string, stringLen);
	string[stringLen] = 0;
	decryptText(string, stringLen);
	Common::String newString((char *)string);
	delete[] string;
	return newString;
}

} // End of namespace AGS
