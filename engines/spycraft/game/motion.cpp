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

#include "spycraft/game/motion.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/user.h"

namespace Spycraft {

//extern unsigned long gameTime;

Cycle::Cycle() {
	name = "Cycle";
	lastCel = 0;
	curCel = 0;
	client = NULL;
	caller = NULL;
}
void Cycle::doit() {
	client->cel = nextCel();
}

int Cycle::nextCel() {
	if (curCel < lastCel)
		return (++curCel);
	else {
		curCel = 0;
		return 0;  // maybe chg this?
	}
}

int Cycle::prevCel() {
	if (curCel > 0)
		return (--curCel);
	else
		return curCel;  // maybe chg this?
}

void Cycle::motionCue() {
	client->cycler = 0;
	client = 0;
	if (caller) {
		cueList->add(caller);
		//caller->cue();
		caller = NULL;
		dispose();
	}
}

EndLoop::EndLoop(Prop *theClient) {
	name = "EndLoop";
	client = theClient;
	//lastCel = client->myATS->reels[client->loop]->numFrames -1;
	lastCel = sfxAnimFrames(client->myATS, client->loop) - 1;
	curCel = client->cel;
}

EndLoop::EndLoop(Prop *theClient, Object *whoToCue) {
	name = "EndLoop";
	client = theClient;
	//	lastCel = client->myATS->reels[client->loop]->numFrames - 1;
	lastCel = sfxAnimFrames(client->myATS, client->loop) - 1;
	curCel = client->cel;
	caller = whoToCue;
}

void EndLoop::doit() {
	Cycle::doit();

	if (curCel == lastCel) //&& ( ( gameTime - client->__cycle_cnt ) > (unsigned)client->cycleSpeed ) )
		motionCue();
}

Forward::Forward(Prop *theClient) {
	name = "Forward";
	client = theClient;
	//lastCel = client->myATS->reels[client->loop]->numFrames -1;
	lastCel = sfxAnimFrames(client->myATS, client->loop) - 1;
	curCel = client->cel;
}

void Forward::doit() {
	Cycle::doit();

	if (curCel == lastCel)
		curCel = 0;
}

BegLoop::BegLoop(Prop *theClient) {
	name = "BegLoop";
	client = theClient;
	//lastCel = client->myATS->reels[client->loop]->numFrames - 1;
	lastCel = sfxAnimFrames(client->myATS, client->loop) - 1;
	curCel = client->cel;
}

BegLoop::BegLoop(Prop *theClient, Object *whoToCue) {
	name = "BegLoop";
	client = theClient;
	//lastCel = client->myATS->reels[client->loop]->numFrames - 1;
	lastCel = sfxAnimFrames(client->myATS, client->loop) - 1;
	curCel = client->cel;
	caller = whoToCue;
}

void BegLoop::doit() {
	client->cel = prevCel();
	if (curCel == 0)
		motionCue();
}

} // namespace Spycraft
