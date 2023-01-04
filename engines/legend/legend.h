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

#ifndef LEGEND_H
#define LEGEND_H

#include "common/scummsys.h"
#include "common/system.h"
#include "common/error.h"
#include "common/fs.h"
#include "common/hash-str.h"
#include "common/random.h"
#include "common/serializer.h"
#include "common/util.h"
#include "engines/engine.h"
#include "engines/savestate.h"
#include "graphics/screen.h"

#include "legend/detection.h"
#include "legend/events.h"
#include "legend/gfx/mouse_cursor.h"
#include "legend/gfx/picture.h"
#include "legend/gfx/screen.h"
#include "legend/utils/file.h"
#include "legend/utils/resources.h"

namespace Legend {

class LegendEngine : public Engine, public Events {
private:
	const LegendGameDescription *_gameDescription;
	Common::RandomSource _randomSource;
protected:
	// Engine APIs
	Common::Error run() override;

	/**
	 * Initalize the game
	 */
	virtual bool initialize();

	/**
	 * Deinitalize the game
	 */
	virtual void deinitialize();

public:
	Gfx::MouseCursor *_mouseCursor = nullptr;
	Gfx::PicFile *_picFile = nullptr;
	Gfx::Screen *_screen = nullptr;
	Resources *_res = nullptr;

public:
	LegendEngine(OSystem *syst, const LegendGameDescription *gameDesc);
	~LegendEngine() override;

	uint32 getFeatures() const;

	/**
	 * Returns the game Id
	 */
	GameType getGameId() const;

	/**
	 * Returns the file prefix for the games data files
	 */
	String getGameFilePrefix() const;

	/**
	 * Returns whether the game is one of the later all-graphical ones
	 */
	bool isLater() const;

	/**
	 * Gets a random number
	 */
	uint32 getRandomNumber(uint maxNum) {
		return _randomSource.getRandomNumber(maxNum);
	}

	bool hasFeature(EngineFeature f) const override {
		return
		    (f == kSupportsLoadingDuringRuntime) ||
		    (f == kSupportsSavingDuringRuntime) ||
		    (f == kSupportsReturnToLauncher);
	};

	void runGame(Gfx::Screen *screen) override final {
		Events::runGame(screen);
	}
	virtual void runGame() {
		runGame(_screen);
	}


	bool canLoadGameStateCurrently() override {
		return true;
	}
	bool canSaveGameStateCurrently() override {
		return true;
	}

	/**
	 * Uses a serializer to allow implementing savegame
	 * loading and saving using a single method
	 */
	Common::Error syncGame(Common::Serializer &s);

	Common::Error saveGameStream(Common::WriteStream *stream, bool isAutosave = false) override {
		Common::Serializer s(nullptr, stream);
		return syncGame(s);
	}
	Common::Error loadGameStream(Common::SeekableReadStream *stream) override {
		Common::Serializer s(stream, nullptr);
		return syncGame(s);
	}

	/**
	 * Returns a formatted filename for a given resource type and file number
	 */
	String getFilename(FileType fileType, int fileNumber);
};

extern LegendEngine *g_engine;
#define SHOULD_QUIT ::Legend::g_engine->shouldQuit()

} // End of namespace Legend

#endif // LEGEND_H
