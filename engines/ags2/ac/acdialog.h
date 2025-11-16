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

#ifndef AGS2_AC_ACDIALOG_H
#define AGS2_AC_ACDIALOG_H

#include "ags2/ac/acroom.h"
#include "ags2/ac/gui/gui.h"

namespace AGS2 {

#define CHOSE_TEXTPARSER -3053
#define SAYCHOSEN_USEFLAG 1
#define SAYCHOSEN_YES 2
#define SAYCHOSEN_NO  3

#define MAX_TOPIC_HISTORY 50
#define DLG_OPTION_PARSER 99

extern int write_dialog_options(int dlgxp, int curyp, int numdisp, int mouseison, int areawid,
	int bullet_wid, int usingfont, DialogTopic *dtop, char *disporder, short *dispyp,
	int txthit, int utextcol);
extern int show_dialog_options(int dlgnum, int sayChosenOption, bool runGameLoopsInBackground);
extern void draw_gui_for_dialog_options(GUIMain *guib, int dlgxp, int dlgyp);
extern bool get_custom_dialog_options_dimensions(int dlgnum);
extern int show_dialog_options(int dlgnum, int sayChosenOption, bool runGameLoopsInBackground);
extern void do_conversation(int dlgnum);

} // namespace AGS2

#endif
