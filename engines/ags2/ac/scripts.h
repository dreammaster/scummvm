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

#ifndef AGS2_AC_SCRIPTS_H
#define AGS2_AC_SCRIPTS_H

namespace AGS2 {

#define MAX_OPEN_SCRIPT_FILES 10

#define MAX_SCRIPT_MODULES 50
#define MAX_QUEUED_SCRIPTS 4
#define MAX_QUEUED_ACTIONS 5

#define REP_EXEC_ALWAYS_NAME "repeatedly_execute_always"
#define REP_EXEC_NAME "repeatedly_execute"

#define RUN_DIALOG_STOP_DIALOG   -2
#define RUN_DIALOG_GOTO_PREVIOUS -4

// Check that a supplied buffer from a text script function was not null
#define VALIDATE_STRING(strin) if ((intptr)strin <= 4096) quit("!String argument was null: make sure you pass a string, not an int, as a buffer")

enum PostScriptAction {
	ePSANewRoom,
	ePSAInvScreen,
	ePSARestoreGame,
	ePSARestoreGameDialog,
	ePSARunAGSGame,
	ePSARunDialog,
	ePSARestartGame,
	ePSASaveGame,
	ePSASaveGameDialog
};

struct ScriptGUI {
	int id;
	GUIMain *gui;
};

struct ScriptHotspot {
	int id;
	int reserved;
};

struct ScriptRegion {
	int id;
	int reserved;
};

struct NonBlockingScriptFunction {
	const char *functionName;
	int numParameters;
	void *param1 = nullptr;
	void *param2 = nullptr;
	bool roomHasFunction = true;
	bool globalScriptHasFunction = true;
	bool moduleHasFunction[MAX_SCRIPT_MODULES] = { true };
	bool atLeastOneImplementationExists = false;

	NonBlockingScriptFunction(const char *funcName, int numParams) :
		functionName(funcName), numParameters(numParams) {
	}
};

struct ExecutingScript {
	ccInstance *inst;
	PostScriptAction postScriptActions[MAX_QUEUED_ACTIONS];
	const char *postScriptActionNames[MAX_QUEUED_ACTIONS];
	char postScriptSaveSlotDescription[MAX_QUEUED_ACTIONS][100];
	int  postScriptActionData[MAX_QUEUED_ACTIONS];
	int  numPostScriptActions;
	char script_run_another[MAX_QUEUED_SCRIPTS][30];
	int  run_another_p1[MAX_QUEUED_SCRIPTS];
	int  run_another_p2[MAX_QUEUED_SCRIPTS];
	int  numanother;
	char forked;

	int queue_action(PostScriptAction act, int data, const char *aname);
	void run_another(const char *namm, int p1, int p2);
	void init();
	ExecutingScript();
};

extern int prepare_text_script(ccInstance *sci, const char **tsname);
extern void cancel_all_scripts();
extern void post_script_cleanup();
extern void quit_with_script_error(const char *functionName);
extern void _do_run_script_func_cant_block(ccInstance *forkedinst, NonBlockingScriptFunction *funcToRun, bool *hasTheFunc);
extern void run_function_on_non_blocking_thread(NonBlockingScriptFunction *funcToRun);
extern int run_script_function_if_exist(ccInstance *sci, const char *tsname, int numParam, int iparam, int iparam2, int iparam3);
extern int run_text_script(ccInstance *sci, const char *tsname);
extern int run_claimable_event(char *tsname, bool includeRoom, int numParams, int param1, int param2, bool *eventWasClaimed);
extern int run_text_script_iparam(ccInstance *sci, const char *tsname, int iparam);
extern int run_text_script_2iparam(ccInstance *sci, const char *tsname, int iparam, int param2);
extern void update_script_mouse_coords();
extern void RunDialog(int tum);
extern void do_conversation(int dlgnu);
extern void can_run_delayed_command();
extern int run_dialog_script(DialogTopic *dtpp, int dialogID, int offse, int optionIndex);
extern void replace_macro_tokens(const char *statusbarformat, char *cur_stb_text);

} // namespace AGS2

#endif
