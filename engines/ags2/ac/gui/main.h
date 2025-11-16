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

#ifndef AGS2_AC_GUI_MAIN_H
#define AGS2_AC_GUI_MAIN_H

#include "common/serializer.h"
#include "ags2/ac/gui/gui_object.h"

namespace AGS2 {

#define MAX_OBJS_ON_GUI 30

struct GameSetupStruct;

struct GUIMain {
	char vtext[4];                // for compatibility
	char name[16];                // the name of the GUI
	char clickEventHandler[20];
	int x, y, wid, hit;
	int focus;                    // which object has the focus
	int numobjs;                  // number of objects on gui
	int popup;                    // when it pops up (POPUP_NONE, POPUP_MOUSEY, POPUP_SCRIPT)
	int popupyp;                  // popup when mousey < this
	int bgcol, bgpic, fgcol;
	int mouseover, mousewasx, mousewasy;
	int mousedownon;
	int highlightobj;
	int flags;
	int transparency;
	int zorder;
	int guiId;
	int reserved[6];
	int on;
	GUIObject *objs[MAX_OBJS_ON_GUI];
	int objrefptr[MAX_OBJS_ON_GUI];       // for re-building objs array
	short drawOrder[MAX_OBJS_ON_GUI];

	static char oNameBuffer[20];

	GUIMain();
	void init();
	const char *get_objscript_name(const char *basedOn);
	void rebuild_array();
	void resort_zorder();
	int  get_control_type(int);
	int  is_mouse_on_gui();
	void draw_blob(int xp, int yp);
	void draw_at(int xx, int yy);
	void draw();
	int  find_object_under_mouse();
	// this version allows some extra leeway in the Editor so that
	// the user can grab tiny controls
	int  find_object_under_mouse(int);
	int  find_object_under_mouse(int leeway, bool mustBeClickable);
	void poll();
	void mouse_but_down();
	void mouse_but_up();
	int  is_textwindow();
	bool send_to_back(int objNum);
	bool bring_to_front(int objNum);
	void control_positions_changed();
	bool is_alpha();

	void SetTransparencyAsPercentage(int percent) {
		// convert from % transparent to Opacity from 0-255
		if (percent == 0)
			this->transparency = 0;
		else if (percent == 100)
			this->transparency = 255;
		else
			this->transparency = ((100 - percent) * 25) / 10;
	}

	void ReadFromFile(Common::SeekableReadStream *fp, int version) {
		Common::Serializer s(fp, nullptr);
		synchronize(s);
	}
	void WriteToFile(Common::WriteStream *fp) {
		Common::Serializer s(nullptr, fp);
		synchronize(s);
	}
	void synchronize(Common::Serializer &s);
};

#define GUIDIS_GREYOUT   1
#define GUIDIS_BLACKOUT  2
#define GUIDIS_UNCHANGED 4
#define GUIDIS_GUIOFF  0x80

extern void read_gui(Common::SeekableReadStream *iii, GUIMain *guiread, GameSetupStruct *gss, GUIMain **allocate = NULL);
extern void write_gui(Common::WriteStream *ooo, GUIMain *guiwrite, GameSetupStruct *gss);

extern void removeBackslashBracket(char *lbuffer);
extern void split_lines_leftright(const char *todis, int wii, int fonnt);

} // namespace AGS2

#endif
