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

#include "spycraft/dmade/ats.h"
#include "spycraft/dmade/adverror.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advdcmp.h"
#include "spycraft/dmade/advfile.h"
#include "spycraft/dmade/advsprite.h"

namespace Spycraft {

static const char ATS_SIGNATURE[] = "ATS";

extern int pixFormat;
void Format565(Viewport *port);
unsigned short Format565P(unsigned short x);

ATS *CreateATS(int numReels) {
	int i, j;
	Reel *lpReel;
	ATS *ats = (ATS *)AllocPtr(sizeof(ATS));

	if (ats) {
		ats->atsPalette = nullptr;
		ats->numReels = numReels;
		ats->reels = (Reel **)AllocPtr(sizeof(Reel *) * numReels);

		if (ats->reels == nullptr) {
			FreePtr(ats);
			return nullptr;
		}

		for (i = 0; i < numReels; i++) {
			lpReel = (Reel *)AllocPtr(sizeof(Reel));
			if (lpReel == nullptr)
				goto fail_reel_allocate;
			ats->reels[i] = lpReel;
		}
	}

	return ats;

fail_reel_allocate:

	for (j = 0; j < i; j++)
		FreePtr(ats->reels[j]);

	FreePtr(ats);
	return nullptr;
}

int CreateReel(ATS *ats, int reelIndex, int numFrames) {
	Viewport **lpPort;

	ats->reels[reelIndex]->numFrames = numFrames;
	lpPort = (Viewport **)AllocPtr((sizeof(Viewport *)) * numFrames);
	if (lpPort == nullptr)
		return false;
	ats->reels[reelIndex]->frames = lpPort;

	return true;
}

void DestroyATS(ATS *ats) {
	int i, j;
	Viewport *port;
	int numReels, numFrames;

	if (ats->reels) {
		numReels = ats->numReels;
		for (i = 0; i < numReels; i++) {
			if (ats->reels[i]) {
				numFrames = ats->reels[i]->numFrames;
				for (j = 0; j < numFrames; j++) {
					port = ats->reels[i]->frames[j];
					if (port) {
						FreePort(port);
					}
				}
			}
			FreePtr(ats->reels[i]->frames);
			FreePtr(ats->reels[i]);
		}
		FreePtr(ats->reels);
		ats->reels = nullptr;
	}

	if (ats->atsPalette)
		FreePtr(ats->atsPalette);

	FreePtr(ats);
}

ATS *OpenATS(DcmpStream dStream) {
	int i, j, fi, fj, r, n, nL, nC, width, height;
	int8 version, colors;
	ATS *ats;
	Viewport *port;
	int bpp = 0, theColors = 0;
	int *thePalette = nullptr;
	char sig[4];

	ReadDcmpStream(dStream, sig, sizeof(sig));
	if (strncmp(sig, ATS_SIGNATURE, 3) != 0) {
		ErrMsg("Corrupt file, please reinstall Spycraft.");
		ADV_ASSERT(false, __ERR_INVALID_ATS);
	}

	/* GET VER NUMBER */
	ReadDcmpStream(dStream, &version, sizeof(version));

	/* COLOR DEPTH */
	ReadDcmpStream(dStream, &colors, sizeof(colors));
	switch (colors) {
	case 8:
		bpp = 8;
		theColors = 8;
		colors = sizeof(char);
		break;

	case 16:
		bpp = 16;
		theColors = 16;
		colors = sizeof(short);
		break;

	case 24:
		bpp = 32;
		theColors = 32;
		colors = sizeof(long);
		break;
	}

	/* EXTRA DATA */
	if (version == 1) {
		r = ReadLongDcmpStream(dStream);
	} else if (version == 2) {
		n = ReadLongDcmpStream(dStream) / sizeof(int);
		thePalette = (int *)AllocPtr(n * sizeof(int));

		if (thePalette == nullptr)
			return nullptr;

		if ((colors == 2) && pixFormat) {
			for (i = 0; i < n; i++) {
				thePalette[i] = Format565P((unsigned short)ReadLongDcmpStream(dStream));
			}
		} else {
			for (i = 0; i < n; i++) {
				thePalette[i] = ReadLongDcmpStream(dStream);
			}
		}

		/* OVERWRITE THE 16 BIT SIZE */
		colors = sizeof(char);
		bpp = 8;
	}

	/* NUMBER OF LOOP */
	nL = ReadWordDcmpStream(dStream);

	ats = CreateATS(nL);

	if (ats == nullptr)
		return nullptr;

	if (version == 1)
		ats->atsPalette = nullptr;
	else if (version == 2)
		ats->atsPalette = thePalette;

	for (i = 0, j = 0; i < nL; i++) {
		/* NUMBER OF CELS */
		nC = ReadWordDcmpStream(dStream);
		if (CreateReel(ats, i, nC) == false)
			goto label_failure1;

		for (j = 0; j < nC; j++) {
			/* GET IMAGE SIZE */
			width = ReadWordDcmpStream(dStream);
			height = ReadWordDcmpStream(dStream);

			port = AllocPort(width, height, bpp);
			if (port == nullptr)
				goto label_failure2;
			port->origX = (short)ReadWordDcmpStream(dStream);
			port->origY = (short)ReadWordDcmpStream(dStream);
			if ((theColors == 16) && pixFormat) {
				port->skipColor = Format565P(ReadWordDcmpStream(dStream));
			} else {
				port->skipColor = ReadWordDcmpStream(dStream);
			}
			n = port->rowBytes * height;
			ReadDcmpStream(dStream, port->ptr, n);
			if ((ats->atsPalette == nullptr) && (theColors == 16) && pixFormat) {
				Format565(port);
			}
			ats->reels[i]->frames[j] = port;
		}
	}
	return ats;

label_failure2:
	// First free all frames for the last partial reel, and then free the reel
	// j-th frame in i-th reel failed to allocate
	for (fj = 0; fj < j; fj++)
		port = ats->reels[i]->frames[fj];
	FreePtr(ats->reels[i]->frames);
label_failure1:
	// Get back to last completed reel
	// i-th reel either failed to allocate, or was alread freed at failure2 above
	// Free all completed reels (if any) befor the i-th reel
	for (fi = 0; fi < i; fi++)
	{
		for (fj = 0; fj < nC; fj++)
		{
			port = ats->reels[fi]->frames[fj];
			if (port)
				FreePort(port);
		}
		FreePtr(ats->reels[fi]->frames);
	}


	FreePtr(ats->reels);
	ats->reels = nullptr;

	if (ats->atsPalette)
		FreePtr(ats->atsPalette);

	FreePtr(ats);

	return nullptr;
}

void sfxAttachATS2Sprite(const SpriteSharedPtr &sprite, ATS *theATS, int theReel, int theFrame) {
	/* GET THE REDUCED PALETTE */
	sprite->colorIndex = theATS->atsPalette;

	/* GET THE FIRST FRAME */
	if (theATS->numReels <= theReel)
		theReel = sfxAnimLoops(theATS) - 1;

	if (theATS->reels[theReel]->numFrames <= theFrame)
		theFrame = sfxAnimFrames(theATS, theReel) - 1;

	sprite->port = sfxAnimFrame(theATS, theReel, theFrame);
}

} // namespace Spycraft
