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

#ifndef AGS2_AC_GUI_BUTTON_H
#define AGS2_AC_GUI_BUTTON_H

#include "ags2/ac/gui/gui_object.h"

namespace AGS2 {

#define GBUT_ALIGN_TOPMIDDLE    0
#define GBUT_ALIGN_TOPLEFT      1
#define GBUT_ALIGN_TOPRIGHT     2
#define GBUT_ALIGN_MIDDLELEFT   3 
#define GBUT_ALIGN_CENTRED      4
#define GBUT_ALIGN_MIDDLERIGHT  5
#define GBUT_ALIGN_BOTTOMLEFT   6
#define GBUT_ALIGN_BOTTOMMIDDLE 7
#define GBUT_ALIGN_BOTTOMRIGHT  8

#define MAX_ANIMATING_BUTTONS 15

struct GUIButton : public GUIObject {
	char text[50];
	int pic, overpic, pushedpic;
	int usepic, ispushed, isover;
	int font, textcol;
	int leftclick, rightclick;
	int lclickdata, rclickdata;
	int textAlignment, reserved1;

	void WriteToFile(Common::WriteStream *ooo) override;
	void ReadFromFile(Common::SeekableReadStream *ooo, int);
	void Draw();
	void MouseUp();

	void MouseOver() override {
		if (ispushed)
			usepic = pushedpic;
		else
			usepic = overpic;

		isover = 1;
	}

	void MouseLeave() override {
		usepic = pic;
		isover = 0;
	}

	int MouseDown() override {
		if (pushedpic > 0)
			usepic = pushedpic;

		ispushed = 1;
		return 0;
	}

	void KeyPress(int keycode) override {
	}

	void reset() {
		GUIObject::init();
		usepic = -1;
		pic = -1;
		overpic = -1;
		pushedpic = -1;
		ispushed = 0;
		isover = 0;
		text[0] = 0;
		font = 0;
		textcol = 0;
		leftclick = 2;
		rightclick = 0;
		activated = 0;
		numSupportedEvents = 1;
		supportedEvents[0] = "Click";
		supportedEventArgs[0] = "GUIControl *control, MouseButton button";
	}

	GUIButton() {
		reset();
	}
};

extern void Button_Animate(GUIButton *butt, int view, int loop, int speed, int repeat);
extern const char *Button_GetText_New(GUIButton *butt);
extern void Button_GetText(GUIButton *butt, char *buffer);
extern void Button_SetText(GUIButton *butt, const char *newtx);
extern void Button_SetFont(GUIButton *butt, int newFont);
extern int Button_GetFont(GUIButton *butt);
extern int Button_GetClipImage(GUIButton *butt);
extern void Button_SetClipImage(GUIButton *butt, int newval);
extern int Button_GetGraphic(GUIButton *butt);
extern int Button_GetMouseOverGraphic(GUIButton *butt);
extern void Button_SetMouseOverGraphic(GUIButton *guil, int slotn);
extern int Button_GetNormalGraphic(GUIButton *butt);
extern void Button_SetNormalGraphic(GUIButton *guil, int slotn);
extern int Button_GetPushedGraphic(GUIButton *butt);
extern void Button_SetPushedGraphic(GUIButton *guil, int slotn);
extern int Button_GetTextColor(GUIButton *butt);
extern void Button_SetTextColor(GUIButton *butt, int newcol);
extern void SetButtonText(int guin, int objn, char *newtx);
extern void AnimateButton(int guin, int objn, int view, int loop, int speed, int repeat);
extern int GetButtonPic(int guin, int objn, int ptype);
extern void SetButtonPic(int guin, int objn, int ptype, int slotn);

// returns 1 if animation finished
extern int UpdateAnimatingButton(int bu);
extern void StopButtonAnimation(int idxn);
extern void FindAndRemoveButtonAnimation(int guin, int objn);

} // namespace AGS2

#endif
