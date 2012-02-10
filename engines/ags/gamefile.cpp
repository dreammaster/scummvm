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

#include "common/debug.h"
#include "common/stream.h"
#include "common/textconsole.h"

#include "ags/constants.h"
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
	if (_version > kAGSVer321)
		error("AGS version %d ('%s') is not supported", _version, _versionString.c_str());

	// now we read GameSetupStruct

	// game name
	char gameName[51];
	dta->read(gameName, 50);
	gameName[50] = '\0';
	_gameName = gameName;
	dta->skip(2); // padding

	debug(1, "AGS game file for '%s' has version %d ('%s')", _gameName.c_str(), _version, _versionString.c_str());

	// options
	for (uint32 i = 0; i < 100; i++)
		_options[i] = dta->readUint32LE();
	if (_version <= kAGSVer300) {
		// from PSP branch: fix animation speed for old formats
		_options[OPT_OLDTALKANIMSPD] = 1;
	}

	// palette
	for (uint32 i = 0; i < 256; i++)
		_palUses[i] = (uint8) dta->readByte();
	for (uint32 i = 0; i < 256; i++) {
		_defPal[3 * i + 0] = dta->readByte(); // R
		_defPal[3 * i + 1] = dta->readByte(); // G
		_defPal[3 * i + 2] = dta->readByte(); // B

		dta->skip(1); // Pad
	}

	_viewCount = dta->readUint32LE();
	_charCount = dta->readUint32LE();
	_playerChars = dta->readUint32LE();

	_totalScore = dta->readSint32LE();

	_invItemCount = dta->readUint16LE();
	dta->skip(2); // padding

	_dialogCount = dta->readUint32LE();
	_dlgMsgCount = dta->readUint32LE();
	_fontCount = dta->readUint32LE();

	_colorDepth = dta->readUint32LE();

	_targetWin = dta->readUint32LE();
	_dialogBullet = dta->readUint32LE();

	_hotDot = dta->readUint16LE();
	_hotDotOuter = dta->readUint16LE();

	_uniqueID = dta->readUint32LE();

	_guiCount = dta->readUint32LE();
	_cursorCount = dta->readUint32LE();

	_defaultResolution = dta->readUint32LE();
	_defaultLipSyncFrame = dta->readUint32LE();

	_invHotDotSprite = dta->readUint32LE();

	// reserved
	dta->skip(17 * 4);

	// messages
	dta->skip(500 * 4);

	// dict
	dta->skip(4);
	// globalscript
	_globalScript = dta->readUint32LE();
	// chars
	dta->skip(4);
	// compiled_script
	_compiledScript = dta->readUint32LE();
	if (!_compiledScript)
		error("missing compiledScript");

	if (_version > kAGSVer272) {
		// FIXME: guid, save game extension/folder
		error("3.x not supported yet");
	}

	// fonts
	_fontFlags.resize(_fontCount);
	for (uint32 i = 0; i < _fontCount; ++i)
		_fontFlags[i] = dta->readByte();
	_fontOutline.resize(_fontCount);
	for (uint32 i = 0; i < _fontCount; ++i)
		_fontOutline[i] = dta->readByte();

	// TODO: PSP version fixes up fontOutlines here...

	// sprite flags
	uint32 spriteFlagCount = dta->readUint32LE();
	_spriteFlags.resize(spriteFlagCount);
	for (uint32 i = 0; i < spriteFlagCount; ++i)
		_spriteFlags[i] = dta->readByte();

	// inventory info
	_invItemInfo.resize(_invItemCount);
	for (uint32 i = 0; i < _invItemCount; ++i) {
		InventoryItemInfo &info = _invItemInfo[i];

		char invName[26];
		dta->read(invName, 25);
		invName[25] = '\0';
		info._name = invName;
		dta->skip(3); // padding

		info._pic = dta->readUint32LE();
		info._cursorPic = dta->readUint32LE();
		info._hotX = dta->readUint32LE();
		info._hotY = dta->readUint32LE();
		dta->skip(5 * 4); // reserved
		info._flags = dta->readByte();
		dta->skip(3); // padding
	}

	// cursors
	_cursors.resize(_cursorCount);
	for (uint32 i = 0; i < _cursorCount; ++i) {
		MouseCursor &cursor = _cursors[i];

		cursor._pic = dta->readUint32LE();
		cursor._hotX = dta->readUint16LE();
		cursor._hotY = dta->readUint16LE();
		cursor._view = dta->readSint16LE();

		char cursorName[11];
		dta->read(cursorName, 10);
		cursorName[10] = '\0';
		cursor._name = cursorName;

		cursor._flags = dta->readByte();
		dta->skip(3); // padding

		if (_version <= kAGSVer272) {
			// convert old value for 'not set'
			if (cursor._view == 0)
				cursor._view = -1;
		}
	}

	delete dta;

	return true;
}

} // End of namespace AGS
