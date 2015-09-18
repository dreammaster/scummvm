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
#include "aesop/rtmsg.h"
#include "aesop/rtsystem.h"
#include "aesop/stubs.h"
#include "aesop/utils.h"

namespace Aesop {

AesopEngine *_vm;

#define MAX_RES_SIZE 800000

AesopEngine::AesopEngine(OSystem *syst, const AesopGameDescription *gameDesc) :
	Engine(syst), _gameDescription(gameDesc), _randomSource("Aesop") {
	_vm = this;
	_debugger = nullptr;
	_events = nullptr;
	_files = nullptr;
	_interpreter = nullptr;
	_resources = nullptr;
	_screen = nullptr;
	_memChecksum = 0;

	DebugMan.addDebugChannel(kDebugScripts, "scripts", "Script debug level");
}

AesopEngine::~AesopEngine() {
	delete _debugger;
	delete _events;
	delete _files;
	delete _interpreter;
	delete _objects;
	delete _resources;
	delete _screen;

	assert(_memChecksum == 0);
}

void AesopEngine::initialize() {
	_debugger = new Debugger(this);
	_events = new Events(this);
	_files = new Files(this);
	_interpreter = Interpreter::init(this);
	_objects = new Objects(this);
	_resources = new Resources(this);
	_screen = new Screen(this);
}

void AesopEngine::deinitialize() {
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

	int rtn = _objects->createProgram(BOOTSTRAP_OBJECT, (ULONG)code);
	_objects->destroyObject(rtn);

	for (int i = 0; i < res._nentries; i++) {
		ULONG f = res._dir[i]._flags;

		if ((f & DA_FREE) && (f & DA_DISCARDED) && (!res._dir[i]._seg))
			break;
	}
}

int AesopEngine::getRandomNumber(int max) {
	return _randomSource.getRandomNumber(max);
}

void *AesopEngine::memAlloc(uint bytes) {
	void *ptr = (void *)malloc(bytes);
	assert(ptr);

	_memChecksum ^= (uint)ptr;

	return ptr;
}

char *AesopEngine::strAlloc(const char *str) {
	char *ptr;

	ptr = (char *)memAlloc(strlen(str) + 1);
	strcpy(ptr, str);

	return ptr;
}

void AesopEngine::memFree(void *ptr) {
	_memChecksum ^= (uint)ptr;

	free(ptr);
}

} // End of namespace Aesop
