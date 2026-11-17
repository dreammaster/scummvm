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

#ifndef ULTIMA5_H
#define ULTIMA5_H

#include "common/scummsys.h"
#include "common/system.h"
#include "common/error.h"
#include "common/serializer.h"
#include "engines/engine.h"
#include "ultima/detection.h"
#include "ultima/shared/engine/events.h"
#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/gfx/message_log.h"
#include "ultima/ultima5/gfx/tileset.h"

namespace Ultima {
namespace Ultima5 {

class Ultima5Engine : public Engine, public Shared::Events {
private:
	const Ultima::UltimaGameDescription *_gameDescription;

protected:
	// Engine APIs
	Common::Error run() override;

	bool shouldQuit() const override {
		return Engine::shouldQuit();
	}

	/**
	 * Goes back to the title screen
	 */
	void returnToMainMenu() override;

	/**
	 * Applies a change to the graphics mode from the game options dialog
	 */
	void applyGameSettings() override;

public:
	Data::Savegame _savegame;
	Gfx::MessageLog _messages;
	Gfx::Tileset _tileset;

	// Which of the renderers is in use
	Data::RenderMode _renderMode = Data::RENDER_EGA;

public:
	Ultima5Engine(OSystem *syst, const Ultima::UltimaGameDescription *gameDesc);
	~Ultima5Engine() override;

	uint32 getFeatures() const;

	/**
	 * Returns the game Id
	 */
	Common::String getGameId() const;

	bool hasFeature(EngineFeature f) const override {
		return
			(f == kSupportsLoadingDuringRuntime) ||
			(f == kSupportsSavingDuringRuntime) ||
			(f == kSupportsReturnToLauncher);
	};

	bool canSaveGameStateCurrently(Common::U32String *msg = nullptr) override;
	bool canLoadGameStateCurrently(Common::U32String *msg = nullptr) override;

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
	 * Returns a font object that GfxSurface instances can use
	 */
	Graphics::Font *createFont() const override;
};

extern Ultima5Engine *g_engine;
#define _G(X) (Ultima5::g_engine->_##X)

} // namespace Ultima5
} // namespace Ultima

#endif
