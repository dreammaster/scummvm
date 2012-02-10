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

namespace AGS {

class ResourceManager;

class GameFile {
public:
	GameFile();
	~GameFile();

	bool init(const ResourceManager &resMan);

private:
	uint32 _version;

	Common::String _versionString;
	Common::String _gameName;

	int32 _options[100];

	uint8 _palUses[256];
	byte  _defPal[768];

	int32 _viewCount;
	int32 _charCount;
	int32 _playerChars;

	int32 _totalScore;

	int16 _invItemCount;

	int32 _dialogCount;
	int32 _dlgMsgCount;

	int32 _fontCount;

	int32 _colorDepth;

	int32 _targetWin;
	int32 _dialogBullet;

	uint16 _hotDot;
	uint16 _hotDotOuter;

	int32 _uniqueID;

	int32 _guiCount;
	int32 _cursorCount;

	int32 _defaultResolution;
	int32 _defaultLipSyncFrame;

	int32 _invHotDotSprite;



	void readVersion(Common::SeekableReadStream &dta);
};

} // End of namespace AGS

#endif // AGS_GAMEFILE_H
