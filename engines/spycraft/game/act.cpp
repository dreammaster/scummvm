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

#include "spycraft/game/act.h"

namespace Spycraft {

extern unsigned long gameTime;

//This is our mover class

MoveTo::MoveTo(int theX, int theY, Actor *theClient) {
	name = "MoveTo";
	client = theClient;
	destX = theX;
	destY = theY;
	xInc = client->xInc;
	yInc = client->yInc;
	setUpBres();
}

MoveTo::MoveTo(int theX, int theY, Actor *theClient, Object *whoToCue) {
	name = "MoveTo";
	client = theClient;
	caller = whoToCue;
	destX = theX;
	destY = theY;
	xInc = client->xInc;
	yInc = client->yInc;
	xDiff = 0;
	setUpBres();
}

void MoveTo::setUpBres() {
	yDiff = destY - client->y;
	xDiff = destX - client->x;
	error = 0;

	if (yDiff < 0) {
		yDiff = -yDiff;
		yInc = -yInc;
	}

	if (xDiff < 0) {
		xDiff = -xDiff;
		xInc = -xInc;
	}
}

void MoveTo::doit() {
	doBresen();
	client->posn(nextX, nextY);
}

void MoveTo::doBresen() {

	if (xDiff > yDiff) {
		//		nextX = client->x + client->xInc;	  								Pat:I think this fixes a small bug
		nextX = client->x + xInc;
		nextY = client->y;
		error += yDiff;
		if (error > xDiff) {
			error -= xDiff;
			//			nextY = client->y + client->yInc;  								Pat:I think this fixes a small bug
			nextY = client->y + yInc;
		}
		//		if 	(((destX - xInc) <= nextX) && ((destX + xInc) >= nextX))	{ 	Pat:I think this fixes a small bug
		if (((destX - client->xInc) <= nextX) && ((destX + client->xInc) >= nextX)) {
			nextX = destX;
			nextY = destY;
			motionCue();
		}
	} else {
		//		nextY = client->y + client->yInc;  									Pat:I think this fixes a small bug
		nextY = client->y + yInc;
		nextX = client->x;
		error += xDiff;
		if (error > yDiff) {
			error -= yDiff;
			//			nextX = client->x + client->xInc;  								Pat:I think this fixes a small bug
			nextX = client->x + xInc;
		}

		//		if 	(((destY - yInc) <= nextY) && ((destY + yInc) >= nextY))	{ 	Pat:I think this fixes a small bug
		if (((destY - client->yInc) <= nextY) && ((destY + client->yInc) >= nextY)) {
			nextY = destY;
			nextX = destX;
			motionCue();
		}
	}
}
void MoveTo::motionCue() {
	client->mover = 0;
	if (caller) {
		caller->cue();
		caller = NULL;
	}
	dispose();
}

//This is our actor class

Actor::Actor() {
	name = "Actor";
	xInc = 1;
	yInc = 1;
	moveSpeed = 6;
	mover = 0;
}

void Actor::doit() {

	Prop::doit();

	if (mover && ((gameTime - __move_cnt) > (unsigned)moveSpeed)) {
		__move_cnt = gameTime;
		mover->doit();
	}
}

void Actor::setSpeed(int theSpeed) {
	moveSpeed = theSpeed;
}

void Actor::setInc(int theXInc, int theYInc) {
	xInc = theXInc;
	yInc = theYInc;
}

Actor::~Actor() {
	if (mover) {
		delete mover;
		mover = NULL;
	}
}

void Actor::setMotion(MoveTo *theMover) {
	if (mover)
		delete mover;

	mover = theMover;
}

/* Usage example
t = new(TestActor);
t->setMotion(new MoveTo(100, 200, t));

TestActor::TestActor()
{
	view = 930;
	init(30, 30);
	setInc(1,1);
	setSpeed(0);
}
*/

} // namespace Spycraft
