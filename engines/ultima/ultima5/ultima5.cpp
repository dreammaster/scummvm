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

#include "common/system.h"
#include "common/translation.h"
#include "engines/util.h"
#include "gui/error.h"
#include "ultima/shared/engine/graphics_options.h"
#include "ultima/ultima5/ultima5.h"
#include "ultima/ultima5/console.h"
#include "ultima/ultima5/data/data.h"
#include "ultima/ultima5/gfx/charset.h"
#include "ultima/ultima5/metaengine.h"
#include "ultima/ultima5/views/views.h"

namespace Ultima {
namespace Ultima5 {

constexpr int SAVEGAME_VERSION = 1;

Ultima5Engine *g_engine;

// The renderer stored in the configuration, falling back to EGA if it is missing or stale
static Data::RenderMode configuredRenderMode() {
	const int mode = Shared::configuredGraphicsMode(Data::RENDER_EGA);
	return (mode >= 0 && mode < Data::RENDER_MODE_COUNT) ? (Data::RenderMode)mode : Data::RENDER_EGA;
}

Ultima5Engine::Ultima5Engine(OSystem *syst, const Ultima::UltimaGameDescription *gameDesc) :
		Engine(syst), Shared::Events("ultima5"), _gameDescription(gameDesc) {
	g_engine = this;
	_renderMode = configuredRenderMode();
}

Ultima5Engine::~Ultima5Engine() {
}

uint32 Ultima5Engine::getFeatures() const {
	return _gameDescription->desc.flags;
}

Common::String Ultima5Engine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Error Ultima5Engine::run() {
	MetaEngine::setKeybindingMode(KBMODE_MINIMAL);

	// Initialize 320x200 graphics mode
	initGraphics(320, 200);
	_charset.reset(new Gfx::CharSet());
	Data::setPalette(_renderMode);

	if (!_tileset.load(_renderMode) || !_lookNames.load()) {
		GUIErrorMessage(_("Could not load the game data"));
		return Common::kNoError;
	}

	// Set the engine's debugger console
	setDebugger(new Console());

	Views::Views views;
	addView("Title");
	runGame();
	(void)views;		// Suppress any warnings of unused local

	return Common::kNoError;
}

void Ultima5Engine::returnToMainMenu() {
	replaceView("Title", true);
}

void Ultima5Engine::applyGameSettings() {
	const Data::RenderMode mode = configuredRenderMode();
	if (mode == _renderMode)
		return;

	if (!_tileset.load(mode))
		return;

	_renderMode = mode;
	Data::setPalette(_renderMode);

	if (focusedView())
		focusedView()->redraw();
}

bool Ultima5Engine::canSaveGameStateCurrently(Common::U32String *) {
	return dynamic_cast<Views::Game *>(focusedView()) != nullptr;
}

bool Ultima5Engine::canLoadGameStateCurrently(Common::U32String *) {
	return true;
}

Common::Error Ultima5Engine::syncGame(Common::Serializer &s) {
	s.syncVersion(SAVEGAME_VERSION);
	_savegame.synchronize(s);

	if (s.isLoading()) {
		Views::MessageLog *log = dynamic_cast<Views::MessageLog *>(findView("MessageLog"));
		if (log)
			log->clear();
		_world.unload();
		replaceView(_savegame.hasActiveGame() ? "Game" : "Title", true);
	}

	return Common::kNoError;
}

bool Ultima5Engine::loadWorld() {
	const bool underworld = _savegame._mapLevel >= Data::MAP_LEVEL_UNDERGROUND;
	return _world.isLoaded(underworld) || _world.load(underworld, _savegame);
}

Graphics::Font *Ultima5Engine::createFont() const {
	// A copy, as every surface made for a view asks for one
	return new Gfx::CharSet(*_charset);
}

} // namespace Ultima5
} // namespace Ultima
