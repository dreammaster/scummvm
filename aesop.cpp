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

#include "aesop/aesop.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"

namespace Aesop {

AesopEngine *_vm;

#define MAX_RES_SIZE 800000


AesopEngine::AesopEngine(OSystem *syst, const AesopGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc), _randomSource("Aesop") {
	_vm = this;
	_resources = nullptr;
	_linkResources = nullptr;
}

AesopEngine::~AesopEngine() {
	delete _resources;
	delete _linkResources;
}

void AesopEngine::initialize() {
	_resources = new Resources(this);

	DebugMan.addDebugChannel(kDebugLevelScript, "scripts", "Script debug level");
}

Common::Error AesopEngine::run() {
	// Initialize the engine
	initialize();

	play();

	return Common::kNoError;
}

void AesopEngine::play() {
	Common::String resName = (getGameID() == GType_EOB3) ? "eye.res" : "hack.res";
	Common::String codeName = "";



	/*

	if (RTR == NULL)
		abend(MSG_RIF, RES_name);

	init_object_list();
	init_notify_list();
	init_event_queue();

	RT_init(RTR, STK_SIZE, objlist);

	HROED = RTR_get_resource_handle(RTR, ROED, DA_TEMPORARY | DA_EVANESCENT);
	RTR_lock(RTR, HROED);
	code = ascnum(RTD_lookup(HROED, code_name));
	RTR_unlock(HROED);

	if (code == (ULONG)-1L)
		abend(MSG_SPNF);

	rtn = create_program(1, bootstrap, (ULONG)code);
	rtn = destroy_object(1, rtn);

	for (i = 0; i<RTR->nentries; i++)
	{
		ULONG f;

		f = RTR->dir[i].flags;

		if ((f & DA_FREE) && (f & DA_DISCARDED) && (!RTR->dir[i].seg))
			break;
	}

	if (envval(0, "AESOP_DIAG") == 1)
	{
		printf("Entries avail: %u\n", RTR->nentries);
		printf("       In use: %u\n\n", i);

		printf("%u names in use\n", RTR->nd_entries);
	}

	RTR_destroy(RTR, RTR_FREEBASE);
	RT_shutdown();

	mem_shutdown();
	AIL_shutdown(MSG_AIL);
	*/
}

int AesopEngine::getRandomNumber(int max) {
	return _randomSource.getRandomNumber(max);
}

} // End of namespace Aesop
