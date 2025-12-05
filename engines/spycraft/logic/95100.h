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

#ifndef SPYCRAFT_LOGIC_95100_H
#define SPYCRAFT_LOGIC_95100_H

#include "spycraft/game/goggles.h"

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"

namespace Spycraft {

class seeBeacon : public View {
protected:
	char *name;
public:
	Sound *beacon;
	seeBeacon();
	void dispose(void);
	void respond(void);
	int doVerb(int);
};

class Rm95100 : public Room {
public:
	int 		number;
	Sound *beacon;
	Rm95100();
	~Rm95100();
	void init(void);
};
class Rm95101 : public Room {
public:

	int 		number;
	Sound *beacon;
	Rm95101();
	~Rm95101();
	void init(void);
};
class Rm95102 : public Room {
public:

	int 		number;
	Sound *beacon;
	Rm95102();
	~Rm95102();
	void init(void);
};
class Rm95103 : public Room {
public:

	int 		number;
	Rm95103();
	void init(void);
};
class Rm95104 : public Room {
public:
	int 		number;
	seeBeacon *beacon;
	Rm95104();
	~Rm95104();
	void init(void);
};
class Rm95105 : public Room {
public:
	int 		number;
	Rm95105();
	void init(void);
};
class Rm95106 : public Room {
public:
	int 		number;
	Sound *beacon;
	Rm95106();
	~Rm95106();
	void init(void);
};
class Rm95107 : public Room {
public:
	int 		number;
	Sound *beacon;
	Rm95107();
	~Rm95107();
	void init(void);
};
class Rm95108 : public Room {
public:
	int 		number;
	Rm95108();
	void init(void);
};
class Rm95109 : public Room {
public:
	int 		number;
	Sound *beacon;
	Rm95109();
	~Rm95109();
	void init(void);
};
class Rm95110 : public Room {
public:
	int 		number;
	Rm95110();
	void init(void);
};
class Rm95111 : public Room {
public:
	int 		number;
	Rm95111();
	void init(void);
};
class Rm95112 : public Room {
public:
	int 		number;
	Rm95112();
	void init(void);
};
class Rm95113 : public Room {
public:
	int 		number;
	Rm95113();
	void init(void);
};
class Rm95114 : public Room {
public:
	int 		number;
	Rm95114();
	void init(void);
};
class Rm95115 : public Room {
public:
	int 		number;
	Rm95115();
	void init(void);
};
class Rm95116 : public Room {
public:
	int 		number;
	Rm95116();
	void init(void);
};
class Rm95117 : public Room {
public:
	int 		number;
	Rm95117();
	void init(void);
};
class Rm95118 : public Room {
public:
	int 		number;
	Rm95118();
	void init(void);
};
/***************************
*  EXITS
***************************/
class SouthExit95100 : public ExitFeature {
public:
	Arrow *arrow;
	SouthExit95100();
	void respond(void);
	int doVerb(int);
};
class NorthExit95100 : public ExitFeature {
public:
	Arrow *arrow;
	NorthExit95100();
	void respond(void);
	int doVerb(int);
};
class NorthEastExit95100 : public ExitFeature {
public:
	Arrow *arrow;
	NorthEastExit95100();
	void respond(void);
	int doVerb(int);
};
class NorthWestExit95100 : public ExitFeature {
public:
	Arrow *arrow;
	NorthWestExit95100();
	void respond(void);
	int doVerb(int);
};
class SouthEastExit95100 : public ExitFeature {
public:
	Arrow *arrow;
	SouthEastExit95100();
	void respond(void);
	int doVerb(int);
};
class SouthWestExit95100 : public ExitFeature {
public:
	Arrow *arrow;
	SouthWestExit95100();
	void respond(void);
	int doVerb(int);
};

class CrisisScript : public Script {
public:
	CrisisScript() {
		name = "crisisscript";
	};
	void changeState(int);
};

class BleepScript : public Script {
public:
	BleepScript() {
		name = "bleepscript";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif
