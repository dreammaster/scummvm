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

#ifndef AGS2_AC_SAVEGAME_H
#define AGS2_AC_SAVEGAME_H

#include "common/scummsys.h"

namespace AGS2 {

#define RESTART_POINT_SAVE_GAME_NUMBER 999
#define RM_MAXLENGTH    1024

typedef struct _RICH_GAME_MEDIA_HEADER {
private:
	template<class SRC, class DEST>
	static void uconvert(const SRC *src, DEST *dest, size_t maxSize) {
		do {
			*dest++ = *src;
		} while (*src++ != 0 && --maxSize > 1);

		*dest = '\0';
	}
public:
	int       dwMagicNumber;
	int       dwHeaderVersion;
	int       dwHeaderSize;
	int       dwThumbnailOffsetLowerDword;
	int       dwThumbnailOffsetHigherDword;
	int       dwThumbnailSize;
	unsigned char guidGameId[16];
	unsigned short szGameName[RM_MAXLENGTH];
	unsigned short szSaveName[RM_MAXLENGTH];
	unsigned short szLevelName[RM_MAXLENGTH];
	unsigned short szComments[RM_MAXLENGTH];

	void ReadFromFile(Common::SeekableReadStream *in);
	void WriteToFile(Common::WriteStream *out);

	void setSaveName(const Common::String &saveName);
	Common::String getSaveName() const;
} RICH_GAME_MEDIA_HEADER;

extern const char *load_game_errors[9];

extern void restore_after_dialog();
extern void RestoreGameSlot(int slnum);
extern void get_save_game_path(int slotNum, char *buffer);
extern void DeleteSaveSlot(int slnum);
extern int Game_SetSaveGameDirectory(const char *newFolder);
extern int GetSaveSlotDescription(int slnum, char *desbuf);
extern const char *Game_GetSaveSlotDescription(int slnum);
extern int LoadSaveSlotScreenshot(int slnum, int width, int height);
extern int load_game_and_print_error(int toload);
extern void restore_game_dialog();
extern void save_game_dialog();
extern void restart_game();
extern void SetRestartPoint();
extern int load_game(int slotn, char *descrp, int *wantShow);
extern void save_game(int slotn, const char *descript);
extern int do_game_load(const char *nametouse, int slotNumber, char *descrp, int *wantShot);
extern void save_room_data_segment();
extern int LoadImageFile(const char *filename);

} // namespace AGS2

#endif
