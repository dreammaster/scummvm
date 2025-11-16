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

#ifndef AGS2_AC_GUI_OBJECT_H
#define AGS2_AC_GUI_OBJECT_H

#include "common/stream.h"
#include "ags2/common/file.h"

namespace AGS2 {

#define GUIF_DEFAULT  1
#define GUIF_CANCEL   2
#define GUIF_DISABLED 4
#define GUIF_TABSTOP  8
#define GUIF_INVISIBLE 0x10
#define GUIF_CLIP     0x20
#define GUIF_NOCLICKS 0x40
#define GUIF_DELETED  0x8000
#define BASEGOBJ_SIZE 7
#define GALIGN_LEFT   0
#define GALIGN_RIGHT  1
#define GALIGN_CENTRE 2
#define MAX_GUIOBJ_SCRIPTNAME_LEN 25
#define MAX_GUIOBJ_EVENTS 10
#define MAX_GUIOBJ_EVENTHANDLER_LEN 30

#define GOBJ_BUTTON     1
#define GOBJ_LABEL      2
#define GOBJ_INVENTORY  3
#define GOBJ_SLIDER     4
#define GOBJ_TEXTBOX    5
#define GOBJ_LISTBOX    6
#define GUI_TEXTWINDOW  0x05    // set vtext[0] to this to signify text window

struct GUIObject {
	int guin, objn;    // gui and object number of this object
	unsigned int flags;
	int x, y;
	int wid, hit;
	int zorder;
	int activated;
	char scriptName[MAX_GUIOBJ_SCRIPTNAME_LEN + 1];
	char eventHandlers[MAX_GUIOBJ_EVENTS][MAX_GUIOBJ_EVENTHANDLER_LEN + 1];

	virtual void MouseMove(int nx, int ny) = 0; // x,y relative to gui
	virtual void MouseOver() = 0; // mouse moves onto object
	virtual void MouseLeave() = 0;        // mouse moves off object
	virtual int  MouseDown() { // button down - return 1 to lock focus
		return 0;
	}
	virtual void MouseUp() = 0;   // button up
	virtual void KeyPress(int) = 0;
	virtual void Draw() = 0;
	// overridable routine to determine whether the mouse is over
	// the control
	virtual int  IsOverControl(int p_x, int p_y, int p_extra) {
		if ((p_x >= x) && (p_y >= y) && (p_x < x + wid + p_extra) && (p_y < y + hit + p_extra))
			return 1;
		return 0;
	}
	// we can't just fread/fwrite inherited objects because of vtbl, so use:
	virtual void WriteToFile(Common::WriteStream *ooo);
	virtual void ReadFromFile(Common::SeekableReadStream *, int);
	// called when the control is resized
	virtual void Resized() {
	}
	virtual int  GetNumEvents() {
		return numSupportedEvents;
	}
	virtual const char *GetEventName(int idx) {
		if ((idx < 0) || (idx >= numSupportedEvents))
			return NULL;
		return supportedEvents[idx];
	}
	virtual const char *GetEventArgs(int idx) {
		if ((idx < 0) || (idx >= numSupportedEvents))
			return NULL;
		return supportedEventArgs[idx];
	}
	void init();

	int IsDeleted() {
		return flags & GUIF_DELETED;
	}
	int IsDisabled();
	void Enable() {
		flags &= ~GUIF_DISABLED;
	}
	void Disable() {
		flags |= GUIF_DISABLED;
	}
	int IsVisible() {
		if (flags & GUIF_INVISIBLE)
			return 0;
		return 1;
	}
	void Show() {
		flags &= ~GUIF_INVISIBLE;
	}
	void Hide() {
		flags |= GUIF_INVISIBLE;
	}
	int IsClickable() {
#ifdef THIS_IS_THE_ENGINE
		return !(flags & GUIF_NOCLICKS);
#else  // make sure the button can be selected in the editor
		return 1;
#endif
	}
	void SetClickable(bool newValue) {
		flags &= ~GUIF_NOCLICKS;
		if (!newValue)
			flags |= GUIF_NOCLICKS;
	}

protected:
	const char *supportedEvents[MAX_GUIOBJ_EVENTS];
	const char *supportedEventArgs[MAX_GUIOBJ_EVENTS];
	int numSupportedEvents;
};

} // namespace AGS2

#endif
