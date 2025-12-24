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

#include "aesop/aesop.h"
#include "graphics/framelimiter.h"
#include "aesop/detection.h"
#include "aesop/console.h"
#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "engines/util.h"
#include "aesop/lib/ail32.h"
#include "aesop/event.h"
#include "aesop/rt.h"
#include "aesop/rtmsg.h"
#include "aesop/rtobject.h"
#include "aesop/rtres.h"
#include "aesop/rtsystem.h"

namespace Aesop {

// Maximum useful AESOP resource cache size (800K)
#define MAX_RES_SIZE    800000
// Amount of memory to reserve for AESOP interpreter stack (16K)
#define STK_SIZE        16384

AesopEngine *g_engine;
uint32 heap_size;
HRES HROED;


AesopEngine::AesopEngine(OSystem *syst, const ADGameDescription *gameDesc) : Engine(syst),
	_gameDescription(gameDesc), _randomSource("Aesop") {
	g_engine = this;
}

AesopEngine::~AesopEngine() {
	delete _screen;
}

uint32 AesopEngine::getFeatures() const {
	return _gameDescription->flags;
}

Common::String AesopEngine::getGameId() const {
	return _gameDescription->gameId;
}

Common::Error AesopEngine::run() {
	uint32 code;
	int32 rtn;

	// Initialize 320x200 paletted graphics mode
	initGraphics(320, 200);
	_screen = new Graphics::Screen();

	AIL_startup();
	mem_init();

	const char *RES_name = "eye.res";
	const char *code_name = "start";

	heap_size = MAX_RES_SIZE;
	RTR = RTR_construct(mem_alloc(heap_size), heap_size, MAX_OBJ_TYPES, RES_name);
	assert(RTR);

	init_object_list();
	init_notify_list();
	init_event_queue();

	RT_init(RTR, STK_SIZE, objlist);

	HROED = RTR_get_resource_handle(RTR, ROED, DA_TEMPORARY | DA_EVANESCENT);
	RTR_lock(RTR, HROED);
	code = ascnum((const char *)RTD_lookup(HROED, code_name));
	RTR_unlock(HROED);

	if (code == (uint32)-1L)
		abend(MSG_SPNF);

	rtn = create_program(1, bootstrap, (uint32)code);
	rtn = destroy_object(1, rtn);

	for (UWORD i = 0; i < RTR->nentries; i++) {
		uint32 f;

		f = RTR->dir[i].flags;

		if ((f & DA_FREE) && (f & DA_DISCARDED) && (!RTR->dir[i].seg))
			break;
	}

	RTR_destroy(RTR, RTR_FREEBASE);
	RT_shutdown();

	mem_shutdown();
	AIL_shutdown(MSG_AIL);

	return Common::kNoError;
}

Common::Error AesopEngine::syncGame(Common::Serializer &s) {
	// The Serializer has methods isLoading() and isSaving()
	// if you need to specific steps; for example setting
	// an array size after reading it's length, whereas
	// for saving it would write the existing array's length
	int dummy = 0;
	s.syncAsUint32LE(dummy);

	return Common::kNoError;
}

} // End of namespace Aesop
