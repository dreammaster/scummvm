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

#ifndef SPYCRAFT_LOGIC_95000_H
#define SPYCRAFT_LOGIC_95000_H

#include "spycraft/game/goggles.h"

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"

namespace Spycraft {

class Rm95000 : public Room {
public:
	int number = 0;

	Rm95000();
	void init(void);
};

class Rm95001 : public Room {
public:
	int 		number;
	Rm95001();
	void init(void);
};
class Rm95002 : public Room {
public:
	int 		number;
	Rm95002();
	void init(void);
};
class Rm95003 : public Room {
public:
	int 		number;
	Rm95003();
	void init(void);
};
class Rm95004 : public Room {
public:
	int 		number;
	Rm95004();
	void init(void);
};
class Rm95005 : public Room {
public:
	int 		number;
	Rm95005();
	void init(void);
};
class Rm95006 : public Room {
public:
	int 		number;
	Rm95006();
	void init(void);
};
class Rm95007 : public Room {
public:
	int 		number;
	Rm95007();
	void init(void);
};
class Rm95008 : public Room {
public:
	int 		number;
	Rm95008();
	void init(void);
};
class Rm95009 : public Room {
public:
	int 		number;
	Rm95009();
	void init(void);
};
class Rm95010 : public Room {
public:
	int 		number;
	Rm95010();
	void init(void);
};
class Rm95011 : public Room {
public:
	int 		number;
	Rm95011();
	void init(void);
};
class Rm95012 : public Room {
public:
	int 		number;
	Rm95012();
	void init(void);
};
class Rm95013 : public Room {
public:
	int 		number;
	Rm95013();
	void init(void);
};
class Rm95014 : public Room {
public:
	int 		number;
	Rm95014();
	void init(void);
};
class Rm95015 : public Room {
public:
	int 		number;
	Rm95015();
	void init(void);
};
class Rm95016 : public Room {
public:
	int 		number;
	Rm95016();
	void init(void);
};
class Rm95017 : public Room {
public:
	int 		number;
	Rm95017();
	void init(void);
};
class Rm95018 : public Room {
public:
	int 		number;
	Rm95018();
	void init(void);
};
/***************************
*  EXITS
***************************/

class SouthExit95000 : public ExitFeature {
public:
	Arrow *arrow;
	SouthExit95000();
	void respond(void);
	int doVerb(int);
};
class NorthExit95000 : public ExitFeature {
public:
	Arrow *arrow;
	NorthExit95000();
	void respond(void);
	int doVerb(int);
};
class NorthEastExit95000 : public ExitFeature {
public:
	Arrow *arrow;
	NorthEastExit95000();
	void respond(void);
	int doVerb(int);
};
class NorthWestExit95000 : public ExitFeature {
public:
	Arrow *arrow;
	NorthWestExit95000();
	void respond(void);
	int doVerb(int);
};
class SouthEastExit95000 : public ExitFeature {
public:
	Arrow *arrow;
	SouthEastExit95000();
	void respond(void);
	int doVerb(int);
};
class SouthWestExit95000 : public ExitFeature {
public:
	Arrow *arrow;
	SouthWestExit95000();
	void respond(void);
	int doVerb(int);
};
class WPScript : public Script {
public:
	WPScript() {
		name = "wpscript";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif
