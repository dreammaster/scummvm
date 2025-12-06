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

#ifndef SPYCRAFT_GAME_ONYXHUNT_H
#define SPYCRAFT_GAME_ONYXHUNT_H

#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/web.h"

namespace Spycraft {

class OnyxHuntOne : public IntrPuzzPage
{
public:
	OnyxHuntOne();
	//	void saveState();
	void dispose();
};

class OnyxBackOne : public IntrPuzzView
{
public:
	OnyxBackOne();
};

class BatraFeature : public IntrPuzzFeature
{
public:
	bool cursorset;

	BatraFeature();
	int handleEvent(MADEEventStamp *);
};

class PatelFeature : public IntrPuzzFeature
{
public:
	bool cursorset;

	PatelFeature();
	int handleEvent(MADEEventStamp *);
};

class MehtaFeature : public IntrPuzzFeature
{
public:
	bool cursorset;

	MehtaFeature();
	int handleEvent(MADEEventStamp *);
};

class BulsaraFeature : public IntrPuzzFeature
{
public:
	bool cursorset;

	BulsaraFeature();
	int handleEvent(MADEEventStamp *);
};

class OnyxHuntTwo : public IntrPuzzPage
{
public:
	OnyxHuntTwo();
	//	void saveState();
	void dispose();
};

class OnyxBackTwo : public IntrPuzzView
{
public:
	OnyxBackTwo();
};

class DateFeature : public IntrPuzzFeature
{
public:
	bool cursorset;

	DateFeature();
	int handleEvent(MADEEventStamp *);
};

class LocationFeature : public IntrPuzzFeature
{
public:
	bool cursorset;

	LocationFeature();
	int handleEvent(MADEEventStamp *);
};

class OnyxOneSendBut : public WebReport
{
public:
	OnyxOneSendBut();
	virtual void activate();
	void cue();
};

class OnyxTwoSendBut : public WebReport
{
public:
	OnyxTwoSendBut();
	virtual void activate();
	void cue();
};

} // namespace Spycraft

#endif
