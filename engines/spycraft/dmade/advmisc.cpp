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

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advmisc.h"
#include "spycraft/dmade/advscreen.h"
#include "spycraft/dmade/advback.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advmusic.h"
#include "spycraft/dmade/advres.h"
#include "spycraft/dmade/advtime.h"
#include "spycraft/dmade/advtext.h"
#include "spycraft/dmade/advsprite.h"
#include "spycraft/dmade/advini.h"
#include "spycraft/dmade/advmovie.h"

namespace Spycraft {

/* backwards compatibility */
#ifndef NEW_RES
#define InitRes SetRESCache
#define CleanRes DestroyRESCache
#define UpdateRes()
#endif

void sfxInitMADE(void) {
	InitMem();
	InitTime();
	InitScreen();
	InitRes();
	InitText();
	InitMovie();
	InitBackgrounds();
	if (!InitSound(_SFX_22050, _SFX_STEREO, _SFX_16_BIT, _SFX_USE_CLOSEST_SETTINGS))
		ErrMsg("Can not initialize sound driver, please restart your computer.");
	if (!InitMusic())
		ErrMsg("Can not initialize music driver, please restart your computer.");
}

void sfxCleanMADE(void) {
	// these should probably be in the opposite order as in sfxInitMade
	CleanBackgrounds();
	CleanMusic();
	CleanSound();
	CleanMovie();
	CleanText();
	CleanRes();
	CleanTime();
	CleanMem();
}

void sfxUpdateMADE(int release) {
	UpdateMusic();
	UpdateSound();

	if ((curBack != -1) && release)
		sfxReleaseSprites(backgrounds[curBack]);

	sfxUpdate();
}

} // namespace Spycraft
