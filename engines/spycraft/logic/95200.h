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

#ifndef SPYCRAFT_LOGIC_95200_H
#define SPYCRAFT_LOGIC_95200_H

#include "spycraft/game/goggles.h"
#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"

namespace Spycraft {

class Rm95200 : public Room {
public:
	int 		number;
	Rm95200();
	void init(void);
};
class Rm95201 : public Room {
public:

	int 		number;
	Rm95201();
	void init(void);
};
class Rm95202 : public Room {
public:

	int 		number;
	Rm95202();
	void init(void);
};
class Rm95203 : public Room {
public:

	int 		number;
	Rm95203();
	void init(void);
};
class Rm95204 : public Room {
public:

	int 		number;
	Rm95204();
	void init(void);
};
class Rm95205 : public Room {
public:

	int 		number;
	Rm95205();
	void init(void);
};
class Rm95206 : public Room {
public:

	int 		number;
	Rm95206();

	void init(void);
};
class Rm95207 : public Room {
public:

	int 		number;
	Rm95207();

	void init(void);
};
class Rm95208 : public Room {
public:

	int 		number;
	Rm95208();

	void init(void);
};
class Rm95209 : public Room {
public:

	int 		number;
	Rm95209();

	void init(void);
};
class Rm95210 : public Room {
public:

	int 		number;
	Rm95210();

	void init(void);
};
class Rm95211 : public Room {
public:

	int 		number;
	Rm95211();

	void init(void);
};
class Rm95212 : public Room {
public:

	int 		number;
	Rm95212();

	void init(void);
};
class Rm95213 : public Room {
public:

	int 		number;
	Rm95213();

	void init(void);
};
class Rm95214 : public Room {
public:

	int 		number;
	Rm95214();

	void init(void);
};
class Rm95215 : public Room {
public:

	int 		number;
	Rm95215();

	void init(void);
};
class Rm95216 : public Room {
public:

	int 		number;
	Rm95216();

	void init(void);
};
class Rm95217 : public Room {
public:

	int 		number;
	Rm95217();

	void init(void);
};
class Rm95218 : public Room {
public:

	int 		number;
	Rm95218();

	void init(void);
};
/***************************
*  EXITS
***************************/

class SouthExit95200 : public ExitFeature {
public:
	Arrow *arrow;
	SouthExit95200();
	void respond(void);
	int doVerb(int);
};
class NorthExit95200 : public ExitFeature {
public:
	Arrow *arrow;
	NorthExit95200();
	void respond(void);
	int doVerb(int);
};
class NorthEastExit95200 : public ExitFeature {
public:
	Arrow *arrow;
	NorthEastExit95200();
	void respond(void);
	int doVerb(int);
};
class NorthWestExit95200 : public ExitFeature {
public:
	Arrow *arrow;
	NorthWestExit95200();
	void respond(void);
	int doVerb(int);
};
class SouthEastExit95200 : public ExitFeature {
public:
	Arrow *arrow;
	SouthEastExit95200();
	void respond(void);
	int doVerb(int);
};
class SouthWestExit95200 : public ExitFeature {
public:
	Arrow *arrow;
	SouthWestExit95200();
	void respond(void);
	int doVerb(int);
};

} // namespace Spycraft

#endif
