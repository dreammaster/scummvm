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

#include "spycraft/game/puzztest.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/timer.h"

namespace Spycraft {

extern Timer *hurryTimer;

void HurrySterling::cue(void) {
	if (strcmp("Rm18000", curRoom->name) != 0)	// not in room
		new VLink("Where are you: Sterling, Peter R.", "557.avi", 0, 20); //We're waiting!  After KAT and PEG id 
	hurryTimer = NULL;
	delete this;
}

int	testKATAndPEG(void) {
#define GFT GameFlag.test
	if (GFT(fSolvedDubanskyKAT) && GFT(fIDPEG) && !GFT(fSetVL_Warhurst113))
	{
		new VLink("Urgent meeting with DCI: Warhurst, Eugene J.",
			"113.avi", fSaw113, 0); //Warhurst: come.	(Pz)
		new EMail("Interesting image: Seaton, Jaimie A.", 98130, fWahKa84, 0);
		GameFlag.set(fSetVL_Warhurst113);
		// Hurry up timer
		ASSERT(hurryTimer == NULL, 0); // poss problem if restored game or mult. use
		hurryTimer = new Timer;
		hurryTimer->setReal(300, new HurrySterling);  // 5 minutes

		GameFlag.set(fDCIOfficeAvailable);
		return true;
	}

	return false;
}

} // namespace Spycraft
