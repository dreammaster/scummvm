/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
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

#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "aesop/aesop.h"
#include "aesop/event.h"
#include "aesop/rtmsg.h"
#include "aesop/rtobject.h"
#include "aesop/rtsystem.h"
#include "aesop/stubs.h"

namespace Aesop {

AesopEngine *_vm;

#define MAX_RES_SIZE 800000


AesopEngine::AesopEngine(OSystem *syst, const AesopGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc), _randomSource("Aesop") {
	_vm = this;
	_resources = nullptr;
}

AesopEngine::~AesopEngine() {
	delete _resources;
}

void AesopEngine::initialize() {
	_interpreter = new Interpreter(this, objlist, 8192);
	_resources = new Resources(this);
	init_object_list();
	init_notify_list();
	init_event_queue();

	DebugMan.addDebugChannel(kDebugLevelScript, "scripts", "Script debug level");
}

void AesopEngine::deinitialize() {
	mem_shutdown();
	AIL_shutdown();
}

Common::Error AesopEngine::run() {
	initialize();
	play();
	deinitialize();

	return Common::kNoError;
}

void AesopEngine::play() {
	Resources &res = *_vm->_resources;
	Common::String codeName = "start";
	HRES hroed = _resources->get_resource_handle(ROED, DA_TEMPORARY | DA_EVANESCENT);
	_resources->lock(hroed);
	int code = ascnum((const char *)_interpreter->lookup(hroed, codeName));
	_resources->unlock(hroed);

	if (code == -1)
		error(MSG_SPNF);

	int rtn = create_program(1, bootstrap, (ULONG)code);
	destroy_object(1, rtn);

	for (int i = 0; i < res._nentries; i++) {
		ULONG f = res._dir[i]._flags;

		if ((f & DA_FREE) && (f & DA_DISCARDED) && (!res._dir[i]._seg))
			break;
	}
}

int AesopEngine::getRandomNumber(int max) {
	return _randomSource.getRandomNumber(max);
}

} // End of namespace Aesop
