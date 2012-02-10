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

#ifndef AGS_GAMEFILE_H
#define AGS_GAMEFILE_H

#include "common/array.h"

namespace AGS {

class ResourceManager;

struct InventoryItemInfo {
	Common::String _name;
	uint32 _pic;
	uint32 _cursorPic;
	uint32 _hotX, _hotY;
	uint8 _flags;
};

struct MouseCursor {
	uint32 _pic;
	uint16 _hotX, _hotY;
	int16 _view;
	Common::String _name;
	uint8 _flags;
};

class GameFile {
public:
	GameFile();
	~GameFile();

	bool init(const ResourceManager &resMan);

private:
	uint32 _version;

	Common::String _versionString;
	Common::String _gameName;

	uint32 _options[100];

	uint8 _palUses[256];
	byte  _defPal[768];

	uint32 _viewCount;
	uint32 _charCount;
	uint32 _playerChars;

	uint32 _totalScore;

	uint16 _invItemCount;
	Common::Array<InventoryItemInfo> _invItemInfo;

	uint32 _dialogCount;
	uint32 _dlgMsgCount;

	uint32 _fontCount;

	uint32 _colorDepth;

	uint32 _targetWin;
	uint32 _dialogBullet;

	uint16 _hotDot;
	uint16 _hotDotOuter;

	uint32 _uniqueID;

	uint32 _guiCount;
	uint32 _cursorCount;
	Common::Array<MouseCursor> _cursors;

	uint32 _defaultResolution;
	uint32 _defaultLipSyncFrame;

	uint32 _invHotDotSprite;

	uint32 _globalScript;
	uint32 _compiledScript;

	Common::Array<byte> _fontFlags;
	Common::Array<byte> _fontOutline;

	Common::Array<byte> _spriteFlags;

	void readVersion(Common::SeekableReadStream &dta);
};

} // End of namespace AGS

#endif // AGS_GAMEFILE_H
