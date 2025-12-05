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

#ifndef SPYCRAFT_LOGIC_79000_H
#define SPYCRAFT_LOGIC_79000_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/globals.h"

namespace Spycraft {

class Rm79000 : public Room
{
public:
	Rm79000();
	void init(void);
	int handleEvent(MADEEventStamp *);
};

class SouthExit79000 : public ExitFeature
{
public:
	SouthExit79000();
	void respond(void);
	int doVerb(int);
};

/* Scripts */
class Script79000 : public Script
{
public:
	Script79000() {
		name = "Script79000";
	};
	void changeState(int);
};

/* Dialogs */
class DachaDialog : public Dialog
{
public:
	int	playedLoop;	// flag
	DachaDialog();
	virtual void cue(void);
};

class DachaD1_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD2_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD3_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD8_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD9_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD10_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD14_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD15_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD18_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD19_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD20_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD21_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaD22_79000 : public DachaDialog
{
public:
	void cue(void);
};

class DachaDExit_79000 : public DachaDialog
{
public:
	void cue(void);
};

/* DItems */
class CutDItem : public DItem
{
public:
	void	doit(void);	// cuts to next movie immediately
};

class GrabShootDItem : public CutDItem
{
public:
	int  grabbedGun;
	int	 shootLoop;
	GrabShootDItem(int whoToShootLoop);
	void doit();
};

class NewsFlash_16_79000 : public DItem
{
public:
	void	doit(void);
};

} // namespace Spycraft

#endif

