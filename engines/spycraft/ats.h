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

#ifndef SPYCRAFT_ATS_H
#define SPYCRAFT_ATS_H

#include "common/stream.h"
#include "spycraft/advport.h"
#include "spycraft/advdcmp.h"

namespace Spycraft {

struct Reel {
	int numFrames;
	Viewport **frames;
};

struct ATS {
	int	numReels;
	int *atsPalette;
	Reel **reels;
};

#define sfxAnimFrame(ats, loop, frame) ((ats)->reels[loop]->frames[frame])
#define sfxAnimLoops(ats) ((ats)->numReels)
#define sfxAnimFrames(ats, loop) ((ats)->reels[loop]->numFrames)

extern ATS *OpenATS(DcmpStream dStream);

extern ATS *CreateATS(int numLoops);
extern int CreateReel(ATS *ats, int reelIndex, int numFrames);
extern void DestroyATS(ATS *ats);
extern void ReportErr(const char *sz, ...);
extern void WriteWord(Common::WriteStream *fp, short v);
extern void WriteDWord(Common::WriteStream *fp, long v);
extern void WriteATSHeader(Common::WriteStream *fp, int numLoop, int color, int verNum, void (*writeExtra)(Common::WriteStream *fp));
extern void WriteATSFrame(Common::WriteStream *fp, Viewport *port);
extern short ReadWord(Common::ReadStream *fp);
extern long ReadDWord(Common::ReadStream *fp);
extern void WriteATSFile(ATS *ats, const char *filename, int *thePalette);

} // namespace Spycraft

#endif
