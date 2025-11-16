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

#include "ags2/ac/actimer.h"
#include "ags2/lib/allegro/timer.h"
#include "ags2/vars.h"

namespace AGS2 {

int run_interaction_commandlist(NewInteractionCommandList *nicl, int *timesrun, int *cmdsrun) {
	int i;

	if (nicl == NULL)
		return -1;

	for (i = 0; i < nicl->numCommands; i++) {
		cmdsrun[0]++;
		int room_was = play.room_changes;

		switch (nicl->command[i].type) {
		case 0:  // Do nothing
			break;
		case 1:  // Run script
		{
			TempEip tempip(4001);
			UPDATE_MP3
				if ((strstr(evblockbasename, "character") != 0) || (strstr(evblockbasename, "inventory") != 0)) {
					// Character or Inventory (global script)
					char *torun = make_ts_func_name(evblockbasename, evblocknum, nicl->command[i].data[0].val);
					// we are already inside the mouseclick event of the script, can't nest calls
					if (inside_script)
						curscript->run_another(torun, 0, 0);
					else run_text_script(gameinst, torun);
				} else {
					// Other (room script)
					if (inside_script) {
						char funcName[60];
						strcpy(funcName, "|");
						strcat(funcName, make_ts_func_name(evblockbasename, evblocknum, nicl->command[i].data[0].val));
						curscript->run_another(funcName, 0, 0);
					} else
						run_text_script(roominst, make_ts_func_name(evblockbasename, evblocknum, nicl->command[i].data[0].val));
				}
			UPDATE_MP3
				break;
		}
		case 2:  // Add score (first time)
			if (timesrun[0] > 0)
				break;
			timesrun[0]++;
		case 3:  // Add score
			GiveScore(IPARAM1);
			break;
		case 4:  // Display Message
			/*        if (comprdata<0)
					  display_message_aschar=evb->data[ss];*/
			DisplayMessage(IPARAM1);
			break;
		case 5:  // Play Music
			PlayMusicResetQueue(IPARAM1);
			break;
		case 6:  // Stop Music
			stopmusic();
			break;
		case 7:  // Play Sound
			play_sound(IPARAM1);
			break;
		case 8:  // Play Flic
			play_flc_file(IPARAM1, IPARAM2);
			break;
		case 9:  // Run Dialog
		{
			int room_was = play.room_changes;
			RunDialog(IPARAM1);
			// if they changed room within the dialog script,
			// the interaction command list is no longer valid
			if (room_was != play.room_changes)
				return -1;
		}
		break;
		case 10: // Enable Dialog Option
			SetDialogOption(IPARAM1, IPARAM2, 1);
			break;
		case 11: // Disable Dialog Option
			SetDialogOption(IPARAM1, IPARAM2, 0);
			break;
		case 12: // Go To Screen
			Character_ChangeRoomAutoPosition(playerchar, IPARAM1, IPARAM2);
			return -1;
		case 13: // Add Inventory
			add_inventory(IPARAM1);
			break;
		case 14: // Move Object
			MoveObject(IPARAM1, IPARAM2, IPARAM3, IPARAM4);
			// if they want to wait until finished, do so
			if (IPARAM5)
				do_main_cycle(UNTIL_MOVEEND, (int)&objs[IPARAM1].moving);
			break;
		case 15: // Object Off
			ObjectOff(IPARAM1);
			break;
		case 16: // Object On
			ObjectOn(IPARAM1);
			break;
		case 17: // Set Object View
			SetObjectView(IPARAM1, IPARAM2);
			break;
		case 18: // Animate Object
			AnimateObject(IPARAM1, IPARAM2, IPARAM3, IPARAM4);
			break;
		case 19: // Move Character
			if (IPARAM4)
				MoveCharacterBlocking(IPARAM1, IPARAM2, IPARAM3, 0);
			else
				MoveCharacter(IPARAM1, IPARAM2, IPARAM3);
			break;
		case 20: // If Inventory Item was used
			if (play.usedinv == IPARAM1) {
				if (game.options[OPT_NOLOSEINV] == 0)
					lose_inventory(play.usedinv);
				if (run_interaction_commandlist(nicl->command[i].get_child_list(), timesrun, cmdsrun))
					return -1;
			} else
				cmdsrun[0]--;
			break;
		case 21: // if player has inventory item
			if (playerchar->inv[IPARAM1] > 0)
				if (run_interaction_commandlist(nicl->command[i].get_child_list(), timesrun, cmdsrun))
					return -1;
			break;
		case 22: // if a character is moving
			if (game.chars[IPARAM1].walking)
				if (run_interaction_commandlist(nicl->command[i].get_child_list(), timesrun, cmdsrun))
					return -1;
			break;
		case 23: // if two variables are equal
			if (IPARAM1 == IPARAM2)
				if (run_interaction_commandlist(nicl->command[i].get_child_list(), timesrun, cmdsrun))
					return -1;
			break;
		case 24: // Stop character walking
			StopMoving(IPARAM1);
			break;
		case 25: // Go to screen at specific co-ordinates
			NewRoomEx(IPARAM1, IPARAM2, IPARAM3);
			return -1;
		case 26: // Move NPC to different room
			if (!is_valid_character(IPARAM1))
				quit("!Move NPC to different room: invalid character specified");
			game.chars[IPARAM1].room = IPARAM2;
			break;
		case 27: // Set character view
			SetCharacterView(IPARAM1, IPARAM2);
			break;
		case 28: // Release character view
			ReleaseCharacterView(IPARAM1);
			break;
		case 29: // Follow character
			FollowCharacter(IPARAM1, IPARAM2);
			break;
		case 30: // Stop following
			FollowCharacter(IPARAM1, -1);
			break;
		case 31: // Disable hotspot
			DisableHotspot(IPARAM1);
			break;
		case 32: // Enable hotspot
			EnableHotspot(IPARAM1);
			break;
		case 33: // Set variable value
			get_interaction_variable(nicl->command[i].data[0].val)->value = IPARAM2;
			break;
		case 34: // Run animation
			scAnimateCharacter(IPARAM1, IPARAM2, IPARAM3, 0);
			do_main_cycle(UNTIL_SHORTIS0, (int)&game.chars[IPARAM1].animating);
			break;
		case 35: // Quick animation
			SetCharacterView(IPARAM1, IPARAM2);
			scAnimateCharacter(IPARAM1, IPARAM3, IPARAM4, 0);
			do_main_cycle(UNTIL_SHORTIS0, (int)&game.chars[IPARAM1].animating);
			ReleaseCharacterView(IPARAM1);
			break;
		case 36: // Set idle animation
			SetCharacterIdle(IPARAM1, IPARAM2, IPARAM3);
			break;
		case 37: // Disable idle animation
			SetCharacterIdle(IPARAM1, -1, -1);
			break;
		case 38: // Lose inventory item
			lose_inventory(IPARAM1);
			break;
		case 39: // Show GUI
			InterfaceOn(IPARAM1);
			break;
		case 40: // Hide GUI
			InterfaceOff(IPARAM1);
			break;
		case 41: // Stop running more commands
			return -1;
		case 42: // Face location
			FaceLocation(IPARAM1, IPARAM2, IPARAM3);
			break;
		case 43: // Pause command processor
			scrWait(IPARAM1);
			break;
		case 44: // Change character view
			ChangeCharacterView(IPARAM1, IPARAM2);
			break;
		case 45: // If player character is
			if (GetPlayerCharacter() == IPARAM1)
				if (run_interaction_commandlist(nicl->command[i].get_child_list(), timesrun, cmdsrun))
					return -1;
			break;
		case 46: // if cursor mode is
			if (GetCursorMode() == IPARAM1)
				if (run_interaction_commandlist(nicl->command[i].get_child_list(), timesrun, cmdsrun))
					return -1;
			break;
		case 47: // if player has been to room
			if (HasBeenToRoom(IPARAM1))
				if (run_interaction_commandlist(nicl->command[i].get_child_list(), timesrun, cmdsrun))
					return -1;
			break;
		default:
			quit("unknown new interaction command");
			break;
		}

		// if the room changed within the action, nicl is no longer valid
		if (room_was != play.room_changes)
			return -1;
	}
	return 0;

}

void run_unhandled_event(int evnt) {
	if (play.check_interaction_only)
		return;

	int evtype = 0;
	if (strnicmp(evblockbasename, "hotspot", 7) == 0) evtype = 1;
	else if (strnicmp(evblockbasename, "object", 6) == 0) evtype = 2;
	else if (strnicmp(evblockbasename, "character", 9) == 0) evtype = 3;
	else if (strnicmp(evblockbasename, "inventory", 9) == 0) evtype = 5;
	else if (strnicmp(evblockbasename, "region", 6) == 0)
		return;  // no unhandled_events for regions

	// clicked Hotspot 0, so change the type code
	if ((evtype == 1) & (evblocknum == 0) & (evnt != 0) & (evnt != 5) & (evnt != 6))
		evtype = 4;
	if ((evtype == 1) & ((evnt == 0) | (evnt == 5) | (evnt == 6)))
		;  // character stands on hotspot, mouse moves over hotspot, any click
	else if ((evtype == 2) & (evnt == 4));  // any click on object
	else if ((evtype == 3) & (evnt == 4));  // any click on character
	else if (evtype > 0) {
		can_run_delayed_command();

		if (inside_script)
			curscript->run_another("#unhandled_event", evtype, evnt);
		else
			run_text_script_2iparam(gameinst, "unhandled_event", evtype, evnt);
	}

}

int run_interaction_event(NewInteraction *nint, int evnt, int chkAny, int isInv) {

	if ((nint->response[evnt] == NULL) || (nint->response[evnt]->numCommands == 0)) {
		// no response defined for this event
		// If there is a response for "Any Click", then abort now so as to
		// run that instead
		if (chkAny < 0);
		else if ((nint->response[chkAny] != NULL) && (nint->response[chkAny]->numCommands > 0))
			return 0;

		// Otherwise, run unhandled_event
		run_unhandled_event(evnt);

		return 0;
	}

	if (play.check_interaction_only) {
		play.check_interaction_only = 2;
		return -1;
	}

	int cmdsrun = 0, retval = 0;
	// Right, so there were some commands defined in response to the event.
	retval = run_interaction_commandlist(nint->response[evnt], &nint->timesRun[evnt], &cmdsrun);

	// An inventory interaction, but the wrong item was used
	if ((isInv) && (cmdsrun == 0))
		run_unhandled_event(evnt);

	return retval;
}

int run_interaction_script(InteractionScripts *nint, int evnt, int chkAny, int isInv) {
	if ((nint->scriptFuncNames[evnt] == NULL) || (nint->scriptFuncNames[evnt][0] == 0)) {
		// no response defined for this event
		// If there is a response for "Any Click", then abort now so as to
		// run that instead
		if (chkAny < 0);
		else if ((nint->scriptFuncNames[chkAny] != NULL) && (nint->scriptFuncNames[chkAny][0] != 0))
			return 0;

		// Otherwise, run unhandled_event
		run_unhandled_event(evnt);

		return 0;
	}

	if (play.check_interaction_only) {
		play.check_interaction_only = 2;
		return -1;
	}

	int room_was = play.room_changes;

	UPDATE_MP3
		if ((strstr(evblockbasename, "character") != 0) || (strstr(evblockbasename, "inventory") != 0)) {
			// Character or Inventory (global script)
			if (inside_script)
				curscript->run_another(nint->scriptFuncNames[evnt], 0, 0);
			else run_text_script(gameinst, nint->scriptFuncNames[evnt]);
		} else {
			// Other (room script)
			if (inside_script) {
				char funcName[60];
				sprintf(funcName, "|%s", nint->scriptFuncNames[evnt]);
				curscript->run_another(funcName, 0, 0);
			} else
				run_text_script(roominst, nint->scriptFuncNames[evnt]);
		}
	UPDATE_MP3

		int retval = 0;
	// if the room changed within the action
	if (room_was != play.room_changes)
		retval = -1;

	return retval;
}

int run_dialog_request(int parmtr) {
	play.stop_dialog_at_end = DIALOG_RUNNING;
	run_text_script_iparam(gameinst, "dialog_request", parmtr);

	if (play.stop_dialog_at_end == DIALOG_STOP) {
		play.stop_dialog_at_end = DIALOG_NONE;
		return -2;
	}
	if (play.stop_dialog_at_end >= DIALOG_NEWTOPIC) {
		int tval = play.stop_dialog_at_end - DIALOG_NEWTOPIC;
		play.stop_dialog_at_end = DIALOG_NONE;
		return tval;
	}
	if (play.stop_dialog_at_end >= DIALOG_NEWROOM) {
		int roomnum = play.stop_dialog_at_end - DIALOG_NEWROOM;
		play.stop_dialog_at_end = DIALOG_NONE;
		NewRoom(roomnum);
		return -2;
	}
	play.stop_dialog_at_end = DIALOG_NONE;
	return -1;
}

int run_dialog_script(DialogTopic *dtpp, int dialogID, int offse, int optionIndex) {
	said_speech_line = 0;
	int result;

	char funcName[100];
	sprintf(funcName, "_run_dialog%d", dialogID);
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

} // namespace AGS2
