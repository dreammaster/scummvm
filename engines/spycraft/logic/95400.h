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

#ifndef SPYCRAFT_LOGIC_95400_H
#define SPYCRAFT_LOGIC_95400_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"

namespace Spycraft {

class Rm95400 : public Room
{
public:
	int 		number;
	Rm95400();
	View *mark;
	void init(void);
};
class Rm95401 : public Room
{
public:

	int 		number;
	Rm95401();
	View *mark;
	void init(void);
};
class Rm95402 : public Room
{
public:

	int 		number;
	Rm95402();
	View *mark;
	void init(void);
};
class Rm95403 : public Room
{
public:

	int 		number;
	Rm95403();
	View *mark;
	void init(void);
};
class Rm95404 : public Room
{
public:

	int 		number;
	Rm95404();
	View *mark;
	void init(void);
};
class Rm95405 : public Room
{
public:

	int 		number;
	Rm95405();
	View *mark;
	void init(void);
};
class Rm95406 : public Room
{
public:

	int 		number;
	Rm95406();
	View *mark;
	void init(void);
};
class Rm95407 : public Room
{
public:

	int 		number;
	Rm95407();
	View *mark;
	void init(void);
};
class Rm95408 : public Room
{
public:

	int 		number;
	Rm95408();
	View *mark;
	void init(void);
};
class Rm95409 : public Room
{
public:

	int 		number;
	Rm95409();
	View *mark;
	void init(void);
};
class Rm95410 : public Room
{
public:

	int 		number;
	Rm95410();
	View *mark;
	void init(void);
};
class Rm95411 : public Room
{
public:

	int 		number;
	Rm95411();
	View *mark;
	void init(void);
};
class Rm95412 : public Room
{
public:

	int 		number;
	Rm95412();
	View *mark;
	void init(void);
};
class Rm95413 : public Room
{
public:

	int 		number;
	Rm95413();
	View *mark;
	void init(void);
};
class Rm95414 : public Room
{
public:

	int 		number;
	Rm95414();
	View *mark;
	void init(void);
};
class Rm95415 : public Room
{
public:

	int 		number;
	Rm95415();
	View *mark;
	void init(void);
};
class Rm95416 : public Room
{
public:

	int 		number;
	Rm95416();
	View *mark;
	void init(void);
};
class Rm95417 : public Room
{
public:

	int 		number;
	Rm95417();
	View *mark;
	void init(void);
};
class Rm95418 : public Room
{
public:

	int 		number;
	Rm95418();
	View *mark;
	void init(void);
};
class Rm95419 : public Room
{
public:

	int 		number;
	Rm95419();
	View *mark;
	void init(void);
};

/***************************
*  EXITS
***************************/

class SouthExit95400 : public ExitFeature
{
public:
	SouthExit95400();
	void respond(void);
	int doVerb(int);
};
class NorthExit95400 : public ExitFeature
{
public:
	NorthExit95400();
	void respond(void);
	int doVerb(int);
};
class NorthEastExit95400 : public ExitFeature
{
public:
	NorthEastExit95400();
	void respond(void);
	int doVerb(int);
};
class NorthWestExit95400 : public ExitFeature
{
public:
	NorthWestExit95400();
	void respond(void);
	int doVerb(int);
};
class SouthEastExit95400 : public ExitFeature
{
public:
	SouthEastExit95400();
	void respond(void);
	int doVerb(int);
};
class SouthWestExit95400 : public ExitFeature
{
public:
	SouthWestExit95400();
	void respond(void);
	int doVerb(int);
};

} // namespace Spycraft

#endif
