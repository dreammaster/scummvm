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

#ifndef MADSV2_ENGINE_H
#define MADSV2_ENGINE_H

#include "common/events.h"
#include "common/stack.h"
#include "common/random.h"
#include "graphics/screen.h"
#include "mads/mads.h"
#include "mads/core/sound.h"

namespace MADS {

namespace MADSV2 {

class MADSV2Engine : public MADSEngine {
protected:
	Graphics::Screen *_screen = nullptr;
	Common::Stack<Common::Event> _keyEvents;
	uint32 _nextFrameTime = 0;

	void pollEvents();

public:
	MADS::SoundManager *_soundManager = nullptr;

public:
	MADSV2Engine(OSystem *syst, const MADSGameDescription *gameDesc);
	~MADSV2Engine() override;

	Graphics::Screen *getScreen() const {
		return _screen;
	}

	bool hasPendingKey();
	int getKey();
	void flushKeys();

	/**
	 * Get the elapsed time in milliseconds
	 */
	uint32 getMillis();

	/* Callback routines in game-specific MAIN module */
	int  main_cheating_key(int mykey) {
		return 0;
	}
	int  main_normal_key(int mykey) {
		return 0;
	}
	int  main_copy_verify() {
		return 0;
	}

	virtual void main_cold_data_init() = 0;
	virtual void global_init_code() = 0;
	virtual void section_music(int section_num) = 0;
	void game_control_loop() {}

	virtual void global_section_constructor() = 0;
};

extern MADSV2Engine *g_engine;

} // namespace MADSV2
} // namespace MADS

#endif
