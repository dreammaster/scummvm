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

#ifndef SPYCRAFT_LOGIC_95300_H
#define SPYCRAFT_LOGIC_95300_H

#include "spycraft/game/goggles.h"

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"

namespace Spycraft {

class Rm95300 : public Room {
public:
	int 		number;
	Rm95300();
	void init(void);
};
class Rm95301 : public Room {
public:

	int 		number;
	Rm95301();
	void init(void);
};
class Rm95302 : public Room {
public:

	int 		number;
	Rm95302();
	void init(void);
};
class Rm95303 : public Room {
public:

	int 		number;
	Rm95303();
	void init(void);
};
class Rm95304 : public Room {
public:

	int 		number;
	Rm95304();
	void init(void);
};
class Rm95305 : public Room {
public:

	int 		number;
	Rm95305();
	void init(void);
};
class Rm95306 : public Room {
public:

	int 		number;
	Rm95306();
	void init(void);
};
class Rm95307 : public Room {
public:

	int 		number;
	Rm95307();
	void init(void);
};
class Rm95308 : public Room {
public:

	int 		number;
	Rm95308();
	void init(void);
};
class Rm95309 : public Room {
public:

	int 		number;
	Rm95309();
	void init(void);
};
class Rm95310 : public Room {
public:

	int 		number;
	Rm95310();
	void init(void);
};
class Rm95311 : public Room {
public:

	int 		number;
	Rm95311();
	void init(void);
};
class Rm95312 : public Room {
public:

	int 		number;
	Rm95312();
	void init(void);
};
class Rm95313 : public Room {
public:

	int 		number;
	Rm95313();
	void init(void);
};
class Rm95314 : public Room {
public:

	int 		number;
	Rm95314();
	void init(void);
};
class Rm95315 : public Room {
public:

	int 		number;
	Rm95315();
	void init(void);
};
class Rm95316 : public Room {
public:

	int 		number;
	Rm95316();
	void init(void);
};
class Rm95317 : public Room {
public:

	int 		number;
	Rm95317();
	void init(void);
};
class Rm95318 : public Room {
public:

	int 		number;
	Rm95318();
	void init(void);
};
/***************************
*  EXITS
***************************/

class SouthExit95300 : public ExitFeature {
public:
	Arrow *arrow;
	SouthExit95300();
	void respond(void);
	int doVerb(int);
};
class NorthExit95300 : public ExitFeature {
public:
	Arrow *arrow;
	NorthExit95300();
	void respond(void);
	int doVerb(int);
};
class NorthEastExit95300 : public ExitFeature {
public:
	Arrow *arrow;
	NorthEastExit95300();
	void respond(void);
	int doVerb(int);
};
class NorthWestExit95300 : public ExitFeature {
public:
	Arrow *arrow;
	NorthWestExit95300();
	void respond(void);
	int doVerb(int);
};
class SouthEastExit95300 : public ExitFeature {
public:
	Arrow *arrow;
	SouthEastExit95300();
	void respond(void);
	int doVerb(int);
};
class SouthWestExit95300 : public ExitFeature {
public:
	Arrow *arrow;
	SouthWestExit95300();
	void respond(void);
	int doVerb(int);
};

} // namespace Spycraft

#endif
