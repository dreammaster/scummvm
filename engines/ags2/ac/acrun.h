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

#ifndef AGS2_AC_ACRUN_H
#define AGS2_AC_ACRUN_H

#include "common/scummsys.h"

namespace AGS2 {

#define RUN_DIALOG_STOP_DIALOG   -2
#define RUN_DIALOG_GOTO_PREVIOUS -4

// the 'cmdsrun' parameter counts how many commands are run.
// if a 'Inv Item Was Used' check does not pass, it doesn't count
// so cmdsrun remains 0 if no inventory items matched
extern int run_interaction_commandlist(NewInteractionCommandList *nicl, int *timesrun, int *cmdsrun);

extern void run_unhandled_event(int evnt);

// Returns 0 normally, or -1 to indicate that the NewInteraction has
// become invalid and don't run another interaction on it
// (eg. a room change occured)
extern int run_interaction_event(NewInteraction *nint, int evnt, int chkAny, int isInv);

// Returns 0 normally, or -1 to indicate that the NewInteraction has
// become invalid and don't run another interaction on it
// (eg. a room change occured)
extern int run_interaction_script(InteractionScripts *nint, int evnt, int chkAny, int isInv);

extern int run_dialog_request(int parmtr);

extern int run_dialog_script(DialogTopic *dtpp, int dialogID, int offse, int optionIndex);

} // namespace AGS2

#endif
