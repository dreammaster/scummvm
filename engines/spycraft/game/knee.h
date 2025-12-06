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

#ifndef SPYCRAFT_GAME_KNEE_H
#define SPYCRAFT_GAME_KNEE_H

#include "spycraft/game/globals.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/print.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"								   
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/web.h"
#include "spycraft/game/vlink.h"

namespace Spycraft {

class KneeScript : public Script {
public:
	KneeScript() {
		name = "Kneescript";
	};
	void changeState(int);
};

class KneeDrawScript : public Script {
public:
	KneeDrawScript() {
		name = "kneedrawscript";
	};
	void changeState(int);
};

class KneeBack : public IntrPuzzView {
protected:
	const char *name;

public:
	KneeBack();
};

class KneeIAButt : public Button {
protected:
	const char *name;

public:
	KneeIAButt();
	void activate(void);
};

class KneePlayButt : public Button {
protected:
	const char *name;

public:
	KneePlayButt();
	void cue(void);
	void activate(void);
};

class KneeStopButt : public Button {
protected:
	const char *name;
public:
	KneeStopButt();
	void activate(void);
};

class Knee : public IntrPuzzPage {
public:
	int playing;
	int pos;
	int out;
	WebReport *report;
	Knee();
	~Knee();
	void dispose(void);
};

extern Knee *knee;

} // namespace Spycraft

#endif
