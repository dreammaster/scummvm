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

#include "ags2/ac/acruntime.h"
#include "ags2/ac/main.h"
#include "ags2/vars.h"

namespace AGS2 {

ExecutingScript::ExecutingScript() {
	init();
}

void ExecutingScript::init() {
	inst = NULL;
	forked = 0;
	numanother = 0;
	numPostScriptActions = 0;
}

void ExecutingScript::run_another(const char *namm, int p1, int p2) {
	if (numanother < MAX_QUEUED_SCRIPTS)
		numanother++;

	int thisslot = numanother - 1;
	Common::strcpy_s(script_run_another[thisslot], namm);
	run_another_p1[thisslot] = p1;
	run_another_p2[thisslot] = p2;
}

int ExecutingScript::queue_action(PostScriptAction act, int data, const char *aname) {
	if (numPostScriptActions >= MAX_QUEUED_ACTIONS)
		quitprintf("!%s: Cannot queue action, post-script queue full", aname);

	if (numPostScriptActions > 0) {
		// if something that will terminate the room has already
		// been queued, don't allow a second thing to be queued
		switch (postScriptActions[numPostScriptActions - 1]) {
		case ePSANewRoom:
		case ePSARestoreGame:
		case ePSARestoreGameDialog:
		case ePSARunAGSGame:
		case ePSARestartGame:
			quitprintf("!%s: Cannot run this command, since there is a %s command already queued to run", aname, postScriptActionNames[numPostScriptActions - 1]);
			break;
			// MACPORT FIX 9/6/5: added default clause to remove warning
		default:
			break;
		}
	}

	postScriptActions[numPostScriptActions] = act;
	postScriptActionData[numPostScriptActions] = data;
	postScriptActionNames[numPostScriptActions] = aname;
	numPostScriptActions++;
	return numPostScriptActions - 1;
}

int prepare_text_script(ccInstance *sci, const char **tsname) {
	ccError = 0;
	if (sci == NULL) return -1;
	if (ccGetSymbolAddr(sci, tsname[0]) == NULL) {
		Common::strcpy_s(ccErrorString, "no such function in script");
		return -2;
	}
	if (sci->pc != 0) {
		Common::strcpy_s(ccErrorString, "script is already in execution");
		return -3;
	}
	scripts[num_scripts].init();
	scripts[num_scripts].inst = sci;
	/*  char tempb[300];
	  Common::sprintf_s(tempb,"Creating script instance for '%s' room %d",tsname[0],displayed_room);
	  write_log(tempb);*/
	if (sci->pc != 0) {
		//    write_log("Forking instance");
		scripts[num_scripts].inst = ccForkInstance(sci);
		if (scripts[num_scripts].inst == NULL)
			quit("unable to fork instance for secondary script");
		scripts[num_scripts].forked = 1;
	}
	curscript = &scripts[num_scripts];
	num_scripts++;
	if (num_scripts >= MAX_SCRIPT_AT_ONCE)
		quit("too many nested text script instances created");
	// in case script_run_another is the function name, take a backup
	Common::strcpy_s(scfunctionname, tsname[0]);
	tsname[0] = &scfunctionname[0];
	update_script_mouse_coords();
	inside_script++;
	//  aborted_ip=0;
	//  abort_executor=0;
	return 0;
}

void cancel_all_scripts() {
	int aa;

	for (aa = 0; aa < num_scripts; aa++) {
		if (scripts[aa].forked)
			ccAbortAndDestroyInstance(scripts[aa].inst);
		else
			ccAbortInstance(scripts[aa].inst);
		scripts[aa].numanother = 0;
	}
	num_scripts = 0;
	/*  if (gameinst!=NULL) ccAbortInstance(gameinst);
	  if (roominst!=NULL) ccAbortInstance(roominst);*/
}

void post_script_cleanup() {
	// should do any post-script stuff here, like go to new room
	if (ccError) quit(ccErrorString);
	ExecutingScript copyof = scripts[num_scripts - 1];
	//  write_log("Instance finished.");
	if (scripts[num_scripts - 1].forked)
		ccFreeInstance(scripts[num_scripts - 1].inst);
	num_scripts--;
	inside_script--;

	if (num_scripts > 0)
		curscript = &scripts[num_scripts - 1];
	else {
		curscript = NULL;
	}
	//  if (abort_executor) user_disabled_data2=aborted_ip;

	int old_room_number = displayed_room;

	// run the queued post-script actions
	for (int ii = 0; ii < copyof.numPostScriptActions; ii++) {
		int thisData = copyof.postScriptActionData[ii];

		switch (copyof.postScriptActions[ii]) {
		case ePSANewRoom:
			// only change rooms when all scripts are done
			if (num_scripts == 0) {
				new_room(thisData, playerchar);
				// don't allow any pending room scripts from the old room
				// in run_another to be executed
				return;
			} else
				curscript->queue_action(ePSANewRoom, thisData, "NewRoom");
			break;
		case ePSAInvScreen:
			invscreen();
			break;
		case ePSARestoreGame:
			cancel_all_scripts();
			load_game_and_print_error(thisData);
			return;
		case ePSARestoreGameDialog:
			restore_game_dialog();
			return;
		case ePSARunAGSGame:
			cancel_all_scripts();
			load_new_game = thisData;
			return;
		case ePSARunDialog:
			do_conversation(thisData);
			break;
		case ePSARestartGame:
			cancel_all_scripts();
			restart_game();
			return;
		case ePSASaveGame:
			save_game(thisData, copyof.postScriptSaveSlotDescription[ii]);
			break;
		case ePSASaveGameDialog:
			save_game_dialog();
			break;
		default:
			quitprintf("undefined post script action found: %d", copyof.postScriptActions[ii]);
		}
		// if the room changed in a conversation, for example, abort
		if (old_room_number != displayed_room) {
			return;
		}
	}


	int jj;
	for (jj = 0; jj < copyof.numanother; jj++) {
		old_room_number = displayed_room;
		char runnext[40];
		Common::strcpy_s(runnext, copyof.script_run_another[jj]);
		copyof.script_run_another[jj][0] = 0;
		if (runnext[0] == '#')
			run_text_script_2iparam(gameinst, &runnext[1], copyof.run_another_p1[jj], copyof.run_another_p2[jj]);
		else if (runnext[0] == '!')
			run_text_script_iparam(gameinst, &runnext[1], copyof.run_another_p1[jj]);
		else if (runnext[0] == '|')
			run_text_script(roominst, &runnext[1]);
		else if (runnext[0] == '%')
			run_text_script_2iparam(roominst, &runnext[1], copyof.run_another_p1[jj], copyof.run_another_p2[jj]);
		else if (runnext[0] == '$') {
			run_text_script_iparam(roominst, &runnext[1], copyof.run_another_p1[jj]);
			play.roomscript_finished = 1;
		} else
			run_text_script(gameinst, runnext);

		// if they've changed rooms, cancel any further pending scripts
		if ((displayed_room != old_room_number) || (load_new_game))
			break;
	}
	copyof.numanother = 0;

}

void quit_with_script_error(const char *functionName) {
	quitprintf("%sError running function '%s':\n%s", (ccErrorIsUserError ? "!" : ""), functionName, ccErrorString);
}

void _do_run_script_func_cant_block(ccInstance *forkedinst, NonBlockingScriptFunction *funcToRun, bool *hasTheFunc) {
	if (!hasTheFunc[0])
		return;

	no_blocking_functions++;
	int result = 0;

	if (funcToRun->numParameters == 0)
		result = ccCallInstance(forkedinst, (char *)funcToRun->functionName, 0);
	else if (funcToRun->numParameters == 1)
		result = ccCallInstance(forkedinst, (char *)funcToRun->functionName, 1, funcToRun->param1);
	else if (funcToRun->numParameters == 2)
		result = ccCallInstance(forkedinst, (char *)funcToRun->functionName, 2, funcToRun->param1, funcToRun->param2);
	else
		quit("_do_run_script_func_cant_block called with too many parameters");

	if (result == -2) {
		// the function doens't exist, so don't try and run it again
		hasTheFunc[0] = false;
	} else if ((result != 0) && (result != 100)) {
		quit_with_script_error(funcToRun->functionName);
	} else
	{
		funcToRun->atLeastOneImplementationExists = true;
	}
	// this might be nested, so don't disrupt blocked scripts
	ccErrorString[0] = 0;
	ccError = 0;
	no_blocking_functions--;
}

void run_function_on_non_blocking_thread(NonBlockingScriptFunction *funcToRun) {

	update_script_mouse_coords();

	int room_changes_was = play.room_changes;
	funcToRun->atLeastOneImplementationExists = false;

	// run modules
	// modules need a forkedinst for this to work
	for (int kk = 0; kk < numScriptModules; kk++) {
		_do_run_script_func_cant_block(moduleInstFork[kk], funcToRun, &funcToRun->moduleHasFunction[kk]);

		if (room_changes_was != play.room_changes)
			return;
	}

	_do_run_script_func_cant_block(gameinstFork, funcToRun, &funcToRun->globalScriptHasFunction);

	if (room_changes_was != play.room_changes)
		return;

	_do_run_script_func_cant_block(roominstFork, funcToRun, &funcToRun->roomHasFunction);
}

int run_script_function_if_exist(ccInstance *sci, const char *tsname, int numParam, int iparam, int iparam2, int iparam3) {
	int oldRestoreCount = gameHasBeenRestored;
	// First, save the current ccError state
	// This is necessary because we might be attempting
	// to run Script B, while Script A is still running in the
	// background.
	// If CallInstance here has an error, it would otherwise
	// also abort Script A because ccError is a global variable.
	int cachedCcError = ccError;
	ccError = 0;

	int toret = prepare_text_script(sci, &tsname);
	if (toret) {
		ccError = cachedCcError;
		return -18;
	}

	// Clear the error message
	ccErrorString[0] = 0;

	if (numParam == 0)
		toret = ccCallInstance(curscript->inst, tsname, numParam);
	else if (numParam == 1)
		toret = ccCallInstance(curscript->inst, tsname, numParam, iparam);
	else if (numParam == 2)
		toret = ccCallInstance(curscript->inst, tsname, numParam, iparam, iparam2);
	else if (numParam == 3)
		toret = ccCallInstance(curscript->inst, tsname, numParam, iparam, iparam2, iparam3);
	else
		quit("Too many parameters to run_script_function_if_exist");

	// 100 is if Aborted (eg. because we are LoadAGSGame'ing)
	if ((toret != 0) && (toret != -2) && (toret != 100)) {
		quit_with_script_error(tsname);
	}

	post_script_cleanup_stack++;

	if (post_script_cleanup_stack > 50)
		quitprintf("!post_script_cleanup call stack exceeded: possible recursive function call? running %s", tsname);

	post_script_cleanup();

	post_script_cleanup_stack--;

	// restore cached error state
	ccError = cachedCcError;

	// if the game has been restored, ensure that any further scripts are not run
	if ((oldRestoreCount != gameHasBeenRestored) && (eventClaimed == EVENT_INPROGRESS))
		eventClaimed = EVENT_CLAIMED;

	return toret;
}

int run_text_script(ccInstance *sci, const char *tsname) {
	if (strcmp(tsname, REP_EXEC_NAME) == 0) {
		// run module rep_execs
		int room_changes_was = play.room_changes;
		int restore_game_count_was = gameHasBeenRestored;

		for (int kk = 0; kk < numScriptModules; kk++) {
			if (moduleRepExecAddr[kk] != NULL)
				run_script_function_if_exist(moduleInst[kk], tsname, 0, 0, 0);

			if ((room_changes_was != play.room_changes) ||
				(restore_game_count_was != gameHasBeenRestored))
				return 0;
		}
	}

	int toret = run_script_function_if_exist(sci, tsname, 0, 0, 0);
	if ((toret == -18) && (sci == roominst)) {
		// functions in room script must exist
		quitprintf("prepare_script: error %d (%s) trying to run '%s'   (Room %d)", toret, ccErrorString, tsname, displayed_room);
	}
	return toret;
}

int run_claimable_event(const char *tsname, bool includeRoom, int numParams, int param1, int param2, bool *evtClaimed) {
	*evtClaimed = true;
	// Run the room script function, and if it is not claimed,
	// then run the main one
	// We need to remember the eventClaimed variable's state, in case
	// this is a nested event
	int eventClaimedOldValue = eventClaimed;
	eventClaimed = EVENT_INPROGRESS;
	int toret;

	if (includeRoom) {
		toret = run_script_function_if_exist(roominst, tsname, numParams, param1, param2);

		if (eventClaimed == EVENT_CLAIMED) {
			eventClaimed = eventClaimedOldValue;
			return toret;
		}
	}

	// run script modules
	for (int kk = 0; kk < numScriptModules; kk++) {
		toret = run_script_function_if_exist(moduleInst[kk], tsname, numParams, param1, param2);

		if (eventClaimed == EVENT_CLAIMED) {
			eventClaimed = eventClaimedOldValue;
			return toret;
		}
	}

	eventClaimed = eventClaimedOldValue;
	*evtClaimed = false;
	return 0;
}

int run_text_script_iparam(ccInstance *sci, const char *tsname, int iparam) {
	if ((strcmp(tsname, "on_key_press") == 0) || (strcmp(tsname, "on_mouse_click") == 0)) {
		bool evtClaimed;
		int toret = run_claimable_event(tsname, true, 1, iparam, 0, &evtClaimed);

		if (evtClaimed)
			return toret;
	}

	return run_script_function_if_exist(sci, tsname, 1, iparam, 0);
}

int run_text_script_2iparam(ccInstance *sci, const char *tsname, int iparam, int param2) {
	if (strcmp(tsname, "on_event") == 0) {
		bool evtClaimed;
		int toret = run_claimable_event(tsname, true, 2, iparam, param2, &evtClaimed);

		if (evtClaimed)
			return toret;
	}

	// response to a button click, better update guis
	if (scumm_strnicmp(tsname, "interface_click", 15) == 0)
		guis_need_update = 1;

	int toret = run_script_function_if_exist(sci, tsname, 2, iparam, param2);

	// tsname is no longer valid, because run_script_function_if_exist might
	// have restored a save game and freed the memory. Therefore don't 
	// attempt any strcmp's here
	tsname = NULL;

	return toret;
}

void update_script_mouse_coords() {
	scmouse.x = divide_down_coordinate(mousex);
	scmouse.y = divide_down_coordinate(mousey);
}

void RunDialog(int tum) {
	if ((tum < 0) | (tum >= game.numdialog))
		quit("!RunDialog: invalid topic number specified");

	can_run_delayed_command();

	if (play.stop_dialog_at_end != DIALOG_NONE) {
		if (play.stop_dialog_at_end == DIALOG_RUNNING)
			play.stop_dialog_at_end = DIALOG_NEWTOPIC + tum;
		else
			quit("!NewRoom: two NewRoom/RunDiaolg/StopDialog requests within dialog");
		return;
	}

	if (inside_script)
		curscript->queue_action(ePSARunDialog, tum, "RunDialog");
	else
		do_conversation(tum);
}

// check and abort game if the script is currently
// inside the rep_exec_always function
void can_run_delayed_command() {
	if (no_blocking_functions)
		quit("!This command cannot be used within non-blocking events such as " REP_EXEC_ALWAYS_NAME);
}

int run_dialog_script(DialogTopic *dtpp, int dialogID, int offse, int optionIndex) {
	said_speech_line = 0;
	int result;

	char funcName[100];
	Common::sprintf_s(funcName, "_run_dialog%d", dialogID);
	run_text_script_iparam(dialogScriptsInst, funcName, optionIndex);
	result = dialogScriptsInst->returnValue;

	if (in_new_room > 0)
		return RUN_DIALOG_STOP_DIALOG;

	if (said_speech_line > 0) {
		// the line below fixes the problem with the close-up face remaining on the
		// screen after they finish talking; however, it makes the dialog options
		// area flicker when going between topics.
		DisableInterface();
		mainloop(); // redraw the screen to make sure it looks right
		EnableInterface();
		// if we're not about to abort the dialog, switch back to arrow
		if (result != RUN_DIALOG_STOP_DIALOG)
			set_mouse_cursor(CURS_ARROW);
	}

	return result;
}

void replace_macro_tokens(const char *statusbarformat, char *cur_stb_text) {
	const char *curptr = &statusbarformat[0];
	char tmpm[3];
	const char *endat = curptr + strlen(statusbarformat);
	cur_stb_text[0] = 0;
	char tempo[STD_BUFFER_SIZE];

	while (1) {
		if (curptr[0] == 0) break;
		if (curptr >= endat) break;
		if (curptr[0] == '@') {
			const char *curptrWasAt = curptr;
			char macroname[21]; int idd = 0; curptr++;
			for (idd = 0; idd < 20; idd++) {
				if (curptr[0] == '@') {
					macroname[idd] = 0;
					curptr++;
					break;
				}
				// unterminated macro (eg. "@SCORETEXT"), so abort
				if (curptr[0] == 0)
					break;
				macroname[idd] = curptr[0];
				curptr++;
			}
			macroname[idd] = 0;
			tempo[0] = 0;
			if (scumm_stricmp(macroname, "score") == 0)
				Common::sprintf_s(tempo, "%d", play.score);
			else if (scumm_stricmp(macroname, "totalscore") == 0)
				Common::sprintf_s(tempo, "%d", MAXSCORE);
			else if (scumm_stricmp(macroname, "scoretext") == 0)
				Common::sprintf_s(tempo, "%d of %d", play.score, MAXSCORE);
			else if (scumm_stricmp(macroname, "gamename") == 0)
				Common::strcpy_s(tempo, play.game_name);
			else if (scumm_stricmp(macroname, "overhotspot") == 0) {
				// While game is in Wait mode, no overhotspot text
				if (!IsInterfaceEnabled())
					tempo[0] = 0;
				else
					GetLocationName(divide_down_coordinate(mousex), divide_down_coordinate(mousey), tempo);
			} else { // not a macro, there's just a @ in the message
				curptr = curptrWasAt + 1;
				Common::strcpy_s(tempo, "@");
			}

			Common::strcat_s(cur_stb_text, 65536, tempo);
		} else {
			tmpm[0] = curptr[0]; tmpm[1] = 0;
			Common::strcat_s(cur_stb_text, 65536, tmpm);
			curptr++;
		}
	}
}

} // namespace AGS2
