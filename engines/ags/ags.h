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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 */

#ifndef AGS_AGS_H
#define AGS_AGS_H

#include "common/system.h"
#include "engines/engine.h"
#include "engines/game.h"
#include "ags/detection.h"

struct ADGameFileDescription;

namespace Common {
	class RandomSource;
	class String;
}

namespace AGS {

struct AGSGameDescription;

class ResourceManager;
class GameFile;

class AGSEngine : public Engine {
public:
	AGSEngine(OSystem *syst, const AGSGameDescription *gameDesc);
	~AGSEngine();

	void initGame(const AGSGameDescription *gd);

	void pauseGame();

	bool isDemo() const;

	uint32 getGameFileVersion() const;
	uint32 getGUIVersion() const;

private:
	const AGSGameDescription *_gameDescription;

	Common::RandomSource *_rnd;

	uint32 _engineStartTime;
	uint32 _playTime;

	uint16 _width, _height;
	uint16 _baseWidth, _baseHeight;
	uint16 _textMultiply;
	bool _forceLetterbox;

	ResourceManager *_resourceMan;
	GameFile *_gameFile;

	Common::String getMasterArchive() const;

	bool init();
	bool getScreenSize();
	bool initGraphics();

	const ADGameFileDescription *getGameFiles() const;
	const char *getGameId() const;
	Common::Language getLanguage() const;
	Common::Platform getPlatform() const;

	// Engine APIs
	Common::Error run();
	bool hasFeature(EngineFeature f) const;
	void pauseEngineIntern(bool pause);
};

} // End of namespace AGS

#endif // AGS_AGS_H
