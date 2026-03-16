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
#include "common/list.h"
#include "common/random.h"
#include "graphics/screen.h"
#include "mads/mads.h"

namespace MADS {
namespace MADSV2 {

class MADSV2Engine : public MADSEngine {
private:
	Graphics::Screen *_screen = nullptr;
	Common::List<Common::Event> _events;

	void pollEvents();

public:
	MADSV2Engine(OSystem *syst, const MADSGameDescription *gameDesc);
	~MADSV2Engine() override;

	Common::Error run() override;

	Graphics::Screen *getScreen() const {
		return _screen;
	}

	bool hasPendingKey();
	int getKey();
	void flushKeys();

	/* Callback routines in game-specific MAIN module */
	int  main_cheating_key(int mykey) {
		return 0;
	}
	int  main_normal_key(int mykey) {
		return 0;
	}
	int  main_copy_verify(void) {
		return 0;
	}
	void main_cold_data_init(void) {}
	void main_false_start(void) {}
	void main_global_init_code(void) {}
	void section_music(int section_num) {}
	void game_control_loop() {}
};

extern MADSV2Engine *g_engine;

} // namespace MADSV2
} // namespace MADS

#endif
