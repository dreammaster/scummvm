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

#ifndef SPYCRAFT_DMADE_GAMEBOX_H
#define SPYCRAFT_DMADE_GAMEBOX_H

namespace Spycraft {

extern int sfxGameBox(void);
extern int sfxRunProgram(char *programName);
extern void sfxEnableReplay(int replay);
extern void sfxEnableSaveGame(int saveGame);
extern void sfxEnableRestoreGame(int restoreGame);
extern void StopStreams(void);
extern void ReStartStreams(void);
extern void StopAllMusic(void);
extern void ReStartAllMusic(void);


#define GAMEBOX  	10
#define IDSAVE  	20
#define IDRESTORE 	30
#define IDVOL		50
#define IDQUIT		60
#define EXIT_TITLE  70

} // namespace Spycraft

#endif
