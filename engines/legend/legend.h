/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef LEGEND_H
#define LEGEND_H

#include "common/scummsys.h"
#include "common/array.h"
#include "common/endian.h"
#include "common/hash-str.h"
#include "common/serializer.h"
#include "common/random.h"
#include "common/savefile.h"
#include "common/util.h"
#include "engines/engine.h"
#include "legend/debugger.h"
#include "legend/events.h"
#include "legend/resources.h"
#include "legend/screen.h"

namespace Legend {

enum {
	kDebugLevelScript      = 1 << 0
};

enum GameType {
	GType_Spellcasting101	= 0,
	GType_Spellcasting201	= 1,
	GType_Spellcasting301	= 2,
	GType_Timequest			= 3,
	GType_Gateway			= 4,
	GType_Gateway2			= 5,
	GType_EricTheUnready	= 6,
	GType_CompanionsOfXanth	= 7,
	GType_DeathGate			= 8
};

#define LEGEND_SCREEN_WIDTH _vm->_screen->w()
#define LEGEND_SCREEN_HEIGHT _vm->_screen->h()

struct LegendGameDescription;

class Resource;

class LegendEngine : public Engine {
private:
	Common::RandomSource _randomSource;
protected:
	/**
	 * Initialize the engine
	 */
	virtual void initialize();
public:
	const LegendGameDescription *_gameDescription;
	Debugger *_debugger;
	Events *_events;
	Resources *_res;
	Screen *_screen;
public:
	LegendEngine(OSystem *syst, const LegendGameDescription *gameDesc);
	virtual ~LegendEngine();

	/**
	 * Main method for running the game
	 */
	virtual Common::Error run();

	/**
	 * Returns the Id of the game
	 */
	GameType getGameID() const;

	/**
	 * Returns true if the game is Companions of Xanth or Death Gate
	 */
	bool isLater() const;

	/**
	 * Returns the platform the game's datafiles are for
	 */
	Common::Platform getPlatform() const;

	/**
	 * Return the game's language
	 */
	Common::Language getLanguage() const;

	/**
	 * Return a random number
	 */
	int getRandomNumber(int limit) { return _randomSource.getRandomNumber(limit - 1); }
};

} // End of namespace Legend

#endif
