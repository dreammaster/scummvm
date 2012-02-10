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

#include "ags/gamefile.h"
#include "ags/resourceman.h"

namespace AGS {

GameFile::GameFile() {
}

GameFile::~GameFile() {
}

void GameFile::readVersion(Common::SeekableReadStream &dta) {
	_version = dta.readUint32LE();

	uint32 versionStringLength = dta.readUint32LE();

	byte *versionString = new byte[versionStringLength + 1];
	dta.read(versionString, versionStringLength);
	versionString[versionStringLength] = '\0';

	_versionString = (const char *) versionString;

	delete[] versionString;
}

bool GameFile::init(const ResourceManager &resMan) {
	Common::SeekableReadStream *dta = resMan.getFile("ac2game.dta");
	if (!dta)
		return false;

	dta->skip(30); // "Adventure Creator Game File v2"

	readVersion(*dta);

	char gameName[51];
	dta->read(gameName, 50);
	gameName[50] = '\0';

	_gameName = gameName;

	for (uint32 i = 0; i < 100; i++)
		_options[i] = (int32) dta->readUint32LE();

	for (uint32 i = 0; i < 256; i++)
		_palUses[i] = (uint8) dta->readByte();
	for (uint32 i = 0; i < 256; i++) {
		_defPal[3 * i + 0] = dta->readByte(); // R
		_defPal[3 * i + 1] = dta->readByte(); // G
		_defPal[3 * i + 2] = dta->readByte(); // B

		dta->skip(1); // Pad
	}

	_viewCount   = (int32) dta->readUint32LE();
	_charCount   = (int32) dta->readUint32LE();
	_playerChars = (int32) dta->readUint32LE();

	warning("%d, %d, %d", _viewCount, _charCount, _playerChars);
	_totalScore = (int32) dta->readUint32LE();

	_invItemCount = (int16) dta->readUint16LE();

	_dialogCount = (int32) dta->readUint32LE();
	_dlgMsgCount = (int32) dta->readUint32LE();

	_fontCount = (int32) dta->readUint32LE();

	_colorDepth = (int32) dta->readUint32LE();

	_targetWin    = (int32) dta->readUint32LE();
	_dialogBullet = (int32) dta->readUint32LE();

	_hotDot      = dta->readUint16LE();
	_hotDotOuter = dta->readUint16LE();

	_uniqueID = (int32) dta->readUint32LE();

	_guiCount    = (int32) dta->readUint32LE();
	_cursorCount = (int32) dta->readUint32LE();

	_defaultResolution   = (int32) dta->readUint32LE();
	_defaultLipSyncFrame = (int32) dta->readUint32LE();

	_invHotDotSprite = (int32) dta->readUint32LE();

	dta->skip(17 * 4); // Reserved

	dta->skip(500 * 4); // messages

	dta->skip(4); // dict
	dta->skip(4); // globalscript
	dta->skip(4); // chars
	dta->skip(4); // compiled_script

	delete dta;

	return true;
}

} // End of namespace AGS
