/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 */

#include "engines/ags/scripting/scripting.h"
#include "engines/ags/gui.h"

namespace AGS {

// import void DisableInterface()
// Disables the player interface and activates the Wait cursor.
RuntimeValue Script_DisableInterface(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DisableInterface unimplemented");

	return RuntimeValue();
}

// import void EnableInterface()
// Re-enables the player interface.
RuntimeValue Script_EnableInterface(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("EnableInterface unimplemented");

	return RuntimeValue();
}

// import int IsInterfaceEnabled()
// Checks whether the player interface is currently enabled.
RuntimeValue Script_IsInterfaceEnabled(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("IsInterfaceEnabled unimplemented");

	return RuntimeValue();
}

// import void SetTextWindowGUI (int gui)
// Changes the GUI used to render standard game text windows.
RuntimeValue Script_SetTextWindowGUI(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);

	// FIXME
	error("SetTextWindowGUI unimplemented");

	return RuntimeValue();
}

// import int FindGUIID(const string)
// Undocumented.
RuntimeValue Script_FindGUIID(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *string = (ScriptString *)params[0]._object;
	UNUSED(string);

	// FIXME
	error("FindGUIID unimplemented");

	return RuntimeValue();
}

// import void SetInvDimensions(int width, int height)
// Obsolete GUI function.
RuntimeValue Script_SetInvDimensions(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int width = params[0]._signedValue;
	UNUSED(width);
	int height = params[1]._signedValue;
	UNUSED(height);

	// FIXME
	error("SetInvDimensions unimplemented");

	return RuntimeValue();
}

// import int GetGUIAt (int x, int y)
// Obsolete GUI function.
RuntimeValue Script_GetGUIAt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("GetGUIAt unimplemented");

	return RuntimeValue();
}

// import int GetGUIObjectAt (int x, int y)
// Obsolete GUI function.
RuntimeValue Script_GetGUIObjectAt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("GetGUIObjectAt unimplemented");

	return RuntimeValue();
}

// import void InterfaceOn(int gui)
// Obsolete GUI function.
RuntimeValue Script_InterfaceOn(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);

	// FIXME
	error("InterfaceOn unimplemented");

	return RuntimeValue();
}

// import void InterfaceOff(int gui)
// Obsolete GUI function.
RuntimeValue Script_InterfaceOff(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);

	// FIXME
	error("InterfaceOff unimplemented");

	return RuntimeValue();
}

// import void SetGUIPosition(int gui, int x, int y)
// Obsolete GUI function.
RuntimeValue Script_SetGUIPosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int x = params[1]._signedValue;
	UNUSED(x);
	int y = params[2]._signedValue;
	UNUSED(y);

	// FIXME
	error("SetGUIPosition unimplemented");

	return RuntimeValue();
}

// import void SetGUISize(int gui, int width, int height)
// Obsolete GUI function.
RuntimeValue Script_SetGUISize(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int width = params[1]._signedValue;
	UNUSED(width);
	int height = params[2]._signedValue;
	UNUSED(height);

	// FIXME
	error("SetGUISize unimplemented");

	return RuntimeValue();
}

// import void CentreGUI(int gui)
// Obsolete GUI function.
RuntimeValue Script_CentreGUI(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);

	// FIXME
	error("CentreGUI unimplemented");

	return RuntimeValue();
}

// import int IsGUIOn (int gui)
// Obsolete GUI function.
RuntimeValue Script_IsGUIOn(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);

	// FIXME
	error("IsGUIOn unimplemented");

	return RuntimeValue();
}

// import void SetGUIBackgroundPic (int gui, int spriteSlot)
// Obsolete GUI function.
RuntimeValue Script_SetGUIBackgroundPic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int spriteSlot = params[1]._signedValue;
	UNUSED(spriteSlot);

	// FIXME
	error("SetGUIBackgroundPic unimplemented");

	return RuntimeValue();
}

// import void SetGUITransparency(int gui, int amount)
// Obsolete GUI function.
RuntimeValue Script_SetGUITransparency(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int amount = params[1]._signedValue;
	UNUSED(amount);

	// FIXME
	error("SetGUITransparency unimplemented");

	return RuntimeValue();
}

// import void SetGUIClickable(int gui, int clickable)
// Obsolete GUI function.
RuntimeValue Script_SetGUIClickable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int clickable = params[1]._signedValue;
	UNUSED(clickable);

	// FIXME
	error("SetGUIClickable unimplemented");

	return RuntimeValue();
}

// import void SetGUIZOrder(int gui, int z)
// Obsolete GUI function.
RuntimeValue Script_SetGUIZOrder(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int z = params[1]._signedValue;
	UNUSED(z);

	// FIXME
	error("SetGUIZOrder unimplemented");

	return RuntimeValue();
}

// import void SetGUIObjectEnabled(int gui, int object, int enable)
// Undocumented.
RuntimeValue Script_SetGUIObjectEnabled(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int enable = params[2]._signedValue;
	UNUSED(enable);

	// FIXME
	error("SetGUIObjectEnabled unimplemented");

	return RuntimeValue();
}

// import void SetGUIObjectPosition(int gui, int object, int x, int y)
// Undocumented.
RuntimeValue Script_SetGUIObjectPosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int x = params[2]._signedValue;
	UNUSED(x);
	int y = params[3]._signedValue;
	UNUSED(y);

	// FIXME
	error("SetGUIObjectPosition unimplemented");

	return RuntimeValue();
}

// import void SetGUIObjectSize(int gui, int object, int width, int height)
// Undocumented.
RuntimeValue Script_SetGUIObjectSize(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int width = params[2]._signedValue;
	UNUSED(width);
	int height = params[3]._signedValue;
	UNUSED(height);

	// FIXME
	error("SetGUIObjectSize unimplemented");

	return RuntimeValue();
}

// import void SetLabelColor(int gui, int object, int colour)
// Undocumented.
RuntimeValue Script_SetLabelColor(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int colour = params[2]._signedValue;
	UNUSED(colour);

	// FIXME
	error("SetLabelColor unimplemented");

	return RuntimeValue();
}

// import void SetLabelText(int gui, int object, const string text)
// Undocumented.
RuntimeValue Script_SetLabelText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	ScriptString *text = (ScriptString *)params[2]._object;
	UNUSED(text);

	// FIXME
	error("SetLabelText unimplemented");

	return RuntimeValue();
}

// import void SetLabelFont(int gui, int object, FontType)
// Undocumented.
RuntimeValue Script_SetLabelFont(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	uint32 fonttype = params[2]._value;
	UNUSED(fonttype);

	// FIXME
	error("SetLabelFont unimplemented");

	return RuntimeValue();
}

// import void SetButtonText(int gui, int object, const string text)
// Undocumented.
RuntimeValue Script_SetButtonText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	ScriptString *text = (ScriptString *)params[2]._object;
	UNUSED(text);

	// FIXME
	error("SetButtonText unimplemented");

	return RuntimeValue();
}

// import void SetButtonPic(int gui, int object, int which, int spriteSlot)
// Undocumented.
RuntimeValue Script_SetButtonPic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int which = params[2]._signedValue;
	UNUSED(which);
	int spriteSlot = params[3]._signedValue;
	UNUSED(spriteSlot);

	// FIXME
	error("SetButtonPic unimplemented");

	return RuntimeValue();
}

// import int GetButtonPic(int gui, int object, int which)
// Undocumented.
RuntimeValue Script_GetButtonPic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int which = params[2]._signedValue;
	UNUSED(which);

	// FIXME
	error("GetButtonPic unimplemented");

	return RuntimeValue();
}

// import void AnimateButton(int gui, int object, int view, int loop, int delay, int repeat)
// Undocumented.
RuntimeValue Script_AnimateButton(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int view = params[2]._signedValue;
	UNUSED(view);
	int loop = params[3]._signedValue;
	UNUSED(loop);
	int delay = params[4]._signedValue;
	UNUSED(delay);
	int repeat = params[5]._signedValue;
	UNUSED(repeat);

	// FIXME
	error("AnimateButton unimplemented");

	return RuntimeValue();
}

// import void SetSliderValue(int gui, int object, int value)
// Undocumented.
RuntimeValue Script_SetSliderValue(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int value = params[2]._signedValue;
	UNUSED(value);

	// FIXME
	error("SetSliderValue unimplemented");

	return RuntimeValue();
}

// import int GetSliderValue(int gui, int object)
// Undocumented.
RuntimeValue Script_GetSliderValue(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);

	// FIXME
	error("GetSliderValue unimplemented");

	return RuntimeValue();
}

// import void SetTextBoxFont(int gui, int object, FontType)
// Undocumented.
RuntimeValue Script_SetTextBoxFont(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	uint32 fonttype = params[2]._value;
	UNUSED(fonttype);

	// FIXME
	error("SetTextBoxFont unimplemented");

	return RuntimeValue();
}

// import void GetTextBoxText(int gui, int object, string buffer)
// Undocumented.
RuntimeValue Script_GetTextBoxText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	ScriptString *buffer = (ScriptString *)params[2]._object;
	UNUSED(buffer);

	// FIXME
	error("GetTextBoxText unimplemented");

	return RuntimeValue();
}

// import void SetTextBoxText(int gui, int object, const string text)
// Undocumented.
RuntimeValue Script_SetTextBoxText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	ScriptString *text = (ScriptString *)params[2]._object;
	UNUSED(text);

	// FIXME
	error("SetTextBoxText unimplemented");

	return RuntimeValue();
}

// import void ListBoxClear(int gui, int object)
// Undocumented.
RuntimeValue Script_ListBoxClear(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);

	// FIXME
	error("ListBoxClear unimplemented");

	return RuntimeValue();
}

// import void ListBoxAdd(int gui, int object, const string text)
// Undocumented.
RuntimeValue Script_ListBoxAdd(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	ScriptString *text = (ScriptString *)params[2]._object;
	UNUSED(text);

	// FIXME
	error("ListBoxAdd unimplemented");

	return RuntimeValue();
}

// import int ListBoxGetSelected(int gui, int object)
// Undocumented.
RuntimeValue Script_ListBoxGetSelected(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);

	// FIXME
	error("ListBoxGetSelected unimplemented");

	return RuntimeValue();
}

// import void ListBoxGetItemText(int gui, int object, int listIndex, string buffer)
// Undocumented.
RuntimeValue Script_ListBoxGetItemText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int listIndex = params[2]._signedValue;
	UNUSED(listIndex);
	ScriptString *buffer = (ScriptString *)params[3]._object;
	UNUSED(buffer);

	// FIXME
	error("ListBoxGetItemText unimplemented");

	return RuntimeValue();
}

// import void ListBoxSetSelected(int gui, int object, int listIndex)
// Undocumented.
RuntimeValue Script_ListBoxSetSelected(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int listIndex = params[2]._signedValue;
	UNUSED(listIndex);

	// FIXME
	error("ListBoxSetSelected unimplemented");

	return RuntimeValue();
}

// import void ListBoxSetTopItem (int gui, int object, int listIndex)
// Undocumented.
RuntimeValue Script_ListBoxSetTopItem(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int listIndex = params[2]._signedValue;
	UNUSED(listIndex);

	// FIXME
	error("ListBoxSetTopItem unimplemented");

	return RuntimeValue();
}

// import void ListBoxDirList (int gui, int object, const string fileMask)
// Undocumented.
RuntimeValue Script_ListBoxDirList(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	ScriptString *fileMask = (ScriptString *)params[2]._object;
	UNUSED(fileMask);

	// FIXME
	error("ListBoxDirList unimplemented");

	return RuntimeValue();
}

// import int ListBoxGetNumItems (int gui, int object)
// Undocumented.
RuntimeValue Script_ListBoxGetNumItems(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);

	// FIXME
	error("ListBoxGetNumItems unimplemented");

	return RuntimeValue();
}

// import int ListBoxSaveGameList (int gui, int object)
// Undocumented.
RuntimeValue Script_ListBoxSaveGameList(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);

	// FIXME
	error("ListBoxSaveGameList unimplemented");

	return RuntimeValue();
}

// import void ListBoxRemove (int gui, int object, int listIndex)
// Undocumented.
RuntimeValue Script_ListBoxRemove(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int gui = params[0]._signedValue;
	UNUSED(gui);
	int object = params[1]._signedValue;
	UNUSED(object);
	int listIndex = params[2]._signedValue;
	UNUSED(listIndex);

	// FIXME
	error("ListBoxRemove unimplemented");

	return RuntimeValue();
}

// GUIControl: import void BringToFront()
// Brings this control to the front of the z-order, in front of all other controls.
RuntimeValue Script_GUIControl_BringToFront(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::BringToFront unimplemented");

	return RuntimeValue();
}

// GUIControl: import static GUIControl* GetAtScreenXY(int x, int y)
// Gets the GUI Control that is visible at the specified location on the screen, or null.
RuntimeValue Script_GUIControl_GetAtScreenXY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("GUIControl::GetAtScreenXY unimplemented");

	return RuntimeValue();
}

// GUIControl: import void SendToBack()
// Sends this control to the back of the z-order, behind all other controls.
RuntimeValue Script_GUIControl_SendToBack(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::SendToBack unimplemented");

	return RuntimeValue();
}

// GUIControl: import void SetPosition(int x, int y)
// Moves the control to the specified position within the GUI.
RuntimeValue Script_GUIControl_SetPosition(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("GUIControl::SetPosition unimplemented");

	return RuntimeValue();
}

// GUIControl: import void SetSize(int width, int height)
// Changes the control to the specified size.
RuntimeValue Script_GUIControl_SetSize(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	int width = params[0]._signedValue;
	UNUSED(width);
	int height = params[1]._signedValue;
	UNUSED(height);

	// FIXME
	error("GUIControl::SetSize unimplemented");

	return RuntimeValue();
}

// GUIControl: readonly import attribute Button* AsButton
// If this control is a button, returns the Button interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsButton(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_AsButton unimplemented");

	return RuntimeValue();
}

// GUIControl: readonly import attribute InvWindow* AsInvWindow
// If this control is a inventory window, returns the InvWindow interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsInvWindow(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_AsInvWindow unimplemented");

	return RuntimeValue();
}

// GUIControl: readonly import attribute Label* AsLabel
// If this control is a label, returns the Label interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsLabel(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_AsLabel unimplemented");

	return RuntimeValue();
}

// GUIControl: readonly import attribute ListBox* AsListBox
// If this control is a list box, returns the ListBox interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsListBox(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_AsListBox unimplemented");

	return RuntimeValue();
}

// GUIControl: readonly import attribute Slider* AsSlider
// If this control is a slider, returns the Slider interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsSlider(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_AsSlider unimplemented");

	return RuntimeValue();
}

// GUIControl: readonly import attribute TextBox* AsTextBox
// If this control is a text box, returns the TextBox interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsTextBox(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_AsTextBox unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute bool Clickable
// Gets/sets whether this control can be clicked on or whether clicks pass straight through it.
RuntimeValue Script_GUIControl_get_Clickable(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_Clickable unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute bool Clickable
// Gets/sets whether this control can be clicked on or whether clicks pass straight through it.
RuntimeValue Script_GUIControl_set_Clickable(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("GUIControl::set_Clickable unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute bool Enabled
// Gets/sets whether this control is currently enabled.
RuntimeValue Script_GUIControl_get_Enabled(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_Enabled unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute bool Enabled
// Gets/sets whether this control is currently enabled.
RuntimeValue Script_GUIControl_set_Enabled(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("GUIControl::set_Enabled unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute int Height
// Gets/sets the height of the control.
RuntimeValue Script_GUIControl_get_Height(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_Height unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute int Height
// Gets/sets the height of the control.
RuntimeValue Script_GUIControl_set_Height(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUIControl::set_Height unimplemented");

	return RuntimeValue();
}

// GUIControl: readonly import attribute int ID
// Gets the ID number of the control within its owning GUI.
RuntimeValue Script_GUIControl_get_ID(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_ID unimplemented");

	return RuntimeValue();
}

// GUIControl: readonly import attribute GUI* OwningGUI
// Gets the GUI that this control is placed onto.
RuntimeValue Script_GUIControl_get_OwningGUI(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_OwningGUI unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute bool Visible
// Gets/sets whether this control is currently visible.
RuntimeValue Script_GUIControl_get_Visible(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_Visible unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute bool Visible
// Gets/sets whether this control is currently visible.
RuntimeValue Script_GUIControl_set_Visible(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("GUIControl::set_Visible unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute int Width
// Gets/sets the width of the control.
RuntimeValue Script_GUIControl_get_Width(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_Width unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute int Width
// Gets/sets the width of the control.
RuntimeValue Script_GUIControl_set_Width(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUIControl::set_Width unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute int X
// Gets/sets the X position of the control's top-left corner.
RuntimeValue Script_GUIControl_get_X(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_X unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute int X
// Gets/sets the X position of the control's top-left corner.
RuntimeValue Script_GUIControl_set_X(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUIControl::set_X unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute int Y
// Gets/sets the Y position of the control's top-left corner.
RuntimeValue Script_GUIControl_get_Y(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUIControl::get_Y unimplemented");

	return RuntimeValue();
}

// GUIControl: import attribute int Y
// Gets/sets the Y position of the control's top-left corner.
RuntimeValue Script_GUIControl_set_Y(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUIControl::set_Y unimplemented");

	return RuntimeValue();
}

// Label: import void GetText(string buffer)
// Undocumented.
RuntimeValue Script_Label_GetText(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;
	UNUSED(buffer);

	// FIXME
	error("Label::GetText unimplemented");

	return RuntimeValue();
}

// Label: import void SetText(const string text)
// Undocumented.
RuntimeValue Script_Label_SetText(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);

	// FIXME
	error("Label::SetText unimplemented");

	return RuntimeValue();
}

// Label: import attribute FontType Font
// Gets/sets the font that is used to draw the label text.
RuntimeValue Script_Label_get_Font(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Label::get_Font unimplemented");

	return RuntimeValue();
}

// Label: import attribute FontType Font
// Gets/sets the font that is used to draw the label text.
RuntimeValue Script_Label_set_Font(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("Label::set_Font unimplemented");

	return RuntimeValue();
}

// Label: import attribute String Text
// Gets/sets the text that is shown on the label.
RuntimeValue Script_Label_get_Text(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Label::get_Text unimplemented");

	return RuntimeValue();
}

// Label: import attribute String Text
// Gets/sets the text that is shown on the label.
RuntimeValue Script_Label_set_Text(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *value = (ScriptString *)params[0]._object;
	UNUSED(value);

	// FIXME
	error("Label::set_Text unimplemented");

	return RuntimeValue();
}

// Label: import attribute int TextColor
// Gets/sets the colour in which the label text is drawn.
RuntimeValue Script_Label_get_TextColor(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Label::get_TextColor unimplemented");

	return RuntimeValue();
}

// Label: import attribute int TextColor
// Gets/sets the colour in which the label text is drawn.
RuntimeValue Script_Label_set_TextColor(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Label::set_TextColor unimplemented");

	return RuntimeValue();
}

// Button: import void Animate(int view, int loop, int delay, RepeatStyle)
// Animates the button graphic using the specified view loop.
RuntimeValue Script_Button_Animate(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	int view = params[0]._signedValue;
	UNUSED(view);
	int loop = params[1]._signedValue;
	UNUSED(loop);
	int delay = params[2]._signedValue;
	UNUSED(delay);
	uint32 repeatstyle = params[3]._value;
	UNUSED(repeatstyle);

	// FIXME
	error("Button::Animate unimplemented");

	return RuntimeValue();
}

// Button: import void GetText(string buffer)
// Undocumented.
RuntimeValue Script_Button_GetText(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;
	UNUSED(buffer);

	// FIXME
	error("Button::GetText unimplemented");

	return RuntimeValue();
}

// Button: import void SetText(const string text)
// Undocumented.
RuntimeValue Script_Button_SetText(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);

	// FIXME
	error("Button::SetText unimplemented");

	return RuntimeValue();
}

// Button: import attribute bool ClipImage
// Gets/sets whether the image is clipped to the size of the control.
RuntimeValue Script_Button_get_ClipImage(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Button::get_ClipImage unimplemented");

	return RuntimeValue();
}

// Button: import attribute bool ClipImage
// Gets/sets whether the image is clipped to the size of the control.
RuntimeValue Script_Button_set_ClipImage(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("Button::set_ClipImage unimplemented");

	return RuntimeValue();
}

// Button: import attribute FontType Font
// Gets/sets the font used to display text on the button.
RuntimeValue Script_Button_get_Font(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Button::get_Font unimplemented");

	return RuntimeValue();
}

// Button: import attribute FontType Font
// Gets/sets the font used to display text on the button.
RuntimeValue Script_Button_set_Font(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("Button::set_Font unimplemented");

	return RuntimeValue();
}

// Button: readonly import attribute int Graphic
// Gets the currently displayed sprite number.
RuntimeValue Script_Button_get_Graphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Button::get_Graphic unimplemented");

	return RuntimeValue();
}

// Button: import attribute int MouseOverGraphic
// Gets/sets the image that is shown when the player moves the mouse over the button (-1 if none)
RuntimeValue Script_Button_get_MouseOverGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Button::get_MouseOverGraphic unimplemented");

	return RuntimeValue();
}

// Button: import attribute int MouseOverGraphic
// Gets/sets the image that is shown when the player moves the mouse over the button (-1 if none)
RuntimeValue Script_Button_set_MouseOverGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Button::set_MouseOverGraphic unimplemented");

	return RuntimeValue();
}

// Button: import attribute int NormalGraphic
// Gets/sets the image that is shown when the button is not pressed or mouse-overed
RuntimeValue Script_Button_get_NormalGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Button::get_NormalGraphic unimplemented");

	return RuntimeValue();
}

// Button: import attribute int NormalGraphic
// Gets/sets the image that is shown when the button is not pressed or mouse-overed
RuntimeValue Script_Button_set_NormalGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Button::set_NormalGraphic unimplemented");

	return RuntimeValue();
}

// Button: import attribute int PushedGraphic
// Gets/sets the image that is shown when the button is pressed
RuntimeValue Script_Button_get_PushedGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Button::get_PushedGraphic unimplemented");

	return RuntimeValue();
}

// Button: import attribute int PushedGraphic
// Gets/sets the image that is shown when the button is pressed
RuntimeValue Script_Button_set_PushedGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Button::set_PushedGraphic unimplemented");

	return RuntimeValue();
}

// Button: import attribute int TextColor
// Gets/sets the colour in which the button text is drawn.
RuntimeValue Script_Button_get_TextColor(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Button::get_TextColor unimplemented");

	return RuntimeValue();
}

// Button: import attribute int TextColor
// Gets/sets the colour in which the button text is drawn.
RuntimeValue Script_Button_set_TextColor(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Button::set_TextColor unimplemented");

	return RuntimeValue();
}

// Button: import attribute String Text
// Gets/sets the text to be drawn on the button.
RuntimeValue Script_Button_get_Text(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Button::get_Text unimplemented");

	return RuntimeValue();
}

// Button: import attribute String Text
// Gets/sets the text to be drawn on the button.
RuntimeValue Script_Button_set_Text(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *value = (ScriptString *)params[0]._object;
	UNUSED(value);

	// FIXME
	error("Button::set_Text unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int BackgroundGraphic
// Gets/sets the image that is tiled to make up the background of the slider.
RuntimeValue Script_Slider_get_BackgroundGraphic(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Slider::get_BackgroundGraphic unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int BackgroundGraphic
// Gets/sets the image that is tiled to make up the background of the slider.
RuntimeValue Script_Slider_set_BackgroundGraphic(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Slider::set_BackgroundGraphic unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int HandleGraphic
// Gets/sets the image used for the 'handle' that represents the current slider position.
RuntimeValue Script_Slider_get_HandleGraphic(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Slider::get_HandleGraphic unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int HandleGraphic
// Gets/sets the image used for the 'handle' that represents the current slider position.
RuntimeValue Script_Slider_set_HandleGraphic(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Slider::set_HandleGraphic unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int HandleOffset
// Gets/sets the pixel offset at which the handle is drawn.
RuntimeValue Script_Slider_get_HandleOffset(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Slider::get_HandleOffset unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int HandleOffset
// Gets/sets the pixel offset at which the handle is drawn.
RuntimeValue Script_Slider_set_HandleOffset(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Slider::set_HandleOffset unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int Max
// Gets/sets the maximum value that the slider can have.
RuntimeValue Script_Slider_get_Max(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Slider::get_Max unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int Max
// Gets/sets the maximum value that the slider can have.
RuntimeValue Script_Slider_set_Max(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Slider::set_Max unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int Min
// Gets/sets the minimum value that the slider can have.
RuntimeValue Script_Slider_get_Min(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Slider::get_Min unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int Min
// Gets/sets the minimum value that the slider can have.
RuntimeValue Script_Slider_set_Min(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Slider::set_Min unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int Value
// Gets/sets the current value of the slider.
RuntimeValue Script_Slider_get_Value(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Slider::get_Value unimplemented");

	return RuntimeValue();
}

// Slider: import attribute int Value
// Gets/sets the current value of the slider.
RuntimeValue Script_Slider_set_Value(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Slider::set_Value unimplemented");

	return RuntimeValue();
}

// TextBox: import void GetText(string buffer)
// Undocumented.
RuntimeValue Script_TextBox_GetText(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;
	UNUSED(buffer);

	// FIXME
	error("TextBox::GetText unimplemented");

	return RuntimeValue();
}

// TextBox: import void SetText(const string text)
// Undocumented.
RuntimeValue Script_TextBox_SetText(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);

	// FIXME
	error("TextBox::SetText unimplemented");

	return RuntimeValue();
}

// TextBox: import attribute FontType Font
// Gets/sets the font used to draw the text in the text box.
RuntimeValue Script_TextBox_get_Font(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("TextBox::get_Font unimplemented");

	return RuntimeValue();
}

// TextBox: import attribute FontType Font
// Gets/sets the font used to draw the text in the text box.
RuntimeValue Script_TextBox_set_Font(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("TextBox::set_Font unimplemented");

	return RuntimeValue();
}

// TextBox: import attribute String Text
// Gets/sets the text that is currently in the text box.
RuntimeValue Script_TextBox_get_Text(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("TextBox::get_Text unimplemented");

	return RuntimeValue();
}

// TextBox: import attribute String Text
// Gets/sets the text that is currently in the text box.
RuntimeValue Script_TextBox_set_Text(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *value = (ScriptString *)params[0]._object;
	UNUSED(value);

	// FIXME
	error("TextBox::set_Text unimplemented");

	return RuntimeValue();
}

// TextBox: import attribute int TextColor
// Gets/sets the color of the text in the text box.
RuntimeValue Script_TextBox_get_TextColor(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("TextBox::get_TextColor unimplemented");

	return RuntimeValue();
}

// TextBox: import attribute int TextColor
// Gets/sets the color of the text in the text box.
RuntimeValue Script_TextBox_set_TextColor(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("TextBox::set_TextColor unimplemented");

	return RuntimeValue();
}

// InvWindow: import void ScrollDown()
// Scrolls the inventory window down one row.
RuntimeValue Script_InvWindow_ScrollDown(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InvWindow::ScrollDown unimplemented");

	return RuntimeValue();
}

// InvWindow: import void ScrollUp()
// Scrolls the inventory window up one row.
RuntimeValue Script_InvWindow_ScrollUp(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InvWindow::ScrollUp unimplemented");

	return RuntimeValue();
}

// InvWindow: import attribute Character* CharacterToUse
// Gets/sets which character's inventory is displayed in this window.
RuntimeValue Script_InvWindow_get_CharacterToUse(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InvWindow::get_CharacterToUse unimplemented");

	return RuntimeValue();
}

// InvWindow: import attribute Character* CharacterToUse
// Gets/sets which character's inventory is displayed in this window.
RuntimeValue Script_InvWindow_set_CharacterToUse(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	if (!params[0]._object->isOfType(sotCharacter))
		error("InvWindow::set_CharacterToUse got incorrect object type (expected a Character) for parameter 1");
	Character *value = (Character *)params[0]._object;
	UNUSED(value);

	// FIXME
	error("InvWindow::set_CharacterToUse unimplemented");

	return RuntimeValue();
}

// InvWindow: readonly import attribute InventoryItem* ItemAtIndex[]
// Gets the inventory item at the specified index in the window.
RuntimeValue Script_InvWindow_geti_ItemAtIndex(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);

	// FIXME
	error("InvWindow::geti_ItemAtIndex unimplemented");

	return RuntimeValue();
}

// InvWindow: readonly import attribute int ItemCount
// Gets the number of inventory items currently shown in the window.
RuntimeValue Script_InvWindow_get_ItemCount(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InvWindow::get_ItemCount unimplemented");

	return RuntimeValue();
}

// InvWindow: import attribute int ItemHeight
// Gets the height of each row of items.
RuntimeValue Script_InvWindow_get_ItemHeight(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InvWindow::get_ItemHeight unimplemented");

	return RuntimeValue();
}

// InvWindow: import attribute int ItemHeight
// Gets the height of each row of items.
RuntimeValue Script_InvWindow_set_ItemHeight(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("InvWindow::set_ItemHeight unimplemented");

	return RuntimeValue();
}

// InvWindow: import attribute int ItemWidth
// Gets the width of each column of items.
RuntimeValue Script_InvWindow_get_ItemWidth(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InvWindow::get_ItemWidth unimplemented");

	return RuntimeValue();
}

// InvWindow: import attribute int ItemWidth
// Gets the width of each column of items.
RuntimeValue Script_InvWindow_set_ItemWidth(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("InvWindow::set_ItemWidth unimplemented");

	return RuntimeValue();
}

// InvWindow: import attribute int TopItem
// Gets the index of the first visible item in the window.
RuntimeValue Script_InvWindow_get_TopItem(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InvWindow::get_TopItem unimplemented");

	return RuntimeValue();
}

// InvWindow: import attribute int TopItem
// Gets the index of the first visible item in the window.
RuntimeValue Script_InvWindow_set_TopItem(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("InvWindow::set_TopItem unimplemented");

	return RuntimeValue();
}

// InvWindow: readonly import attribute int ItemsPerRow
// Gets the number of items shown per row in this inventory window.
RuntimeValue Script_InvWindow_get_ItemsPerRow(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InvWindow::get_ItemsPerRow unimplemented");

	return RuntimeValue();
}

// InvWindow: readonly import attribute int RowCount
// Gets the number of visible rows in this inventory window.
RuntimeValue Script_InvWindow_get_RowCount(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InvWindow::get_RowCount unimplemented");

	return RuntimeValue();
}

// ListBox: import bool AddItem(const string text)
// Adds a new item to the bottom of the list with the specified text.
RuntimeValue Script_ListBox_AddItem(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;
	UNUSED(text);

	// FIXME
	error("ListBox::AddItem unimplemented");

	return RuntimeValue();
}

// ListBox: import void Clear()
// Removes all the items from the list.
RuntimeValue Script_ListBox_Clear(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::Clear unimplemented");

	return RuntimeValue();
}

// ListBox: import void FillDirList(const string fileMask)
// Fills the list box with all the filenames that match the specified file mask.
RuntimeValue Script_ListBox_FillDirList(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *fileMask = (ScriptString *)params[0]._object;
	UNUSED(fileMask);

	// FIXME
	error("ListBox::FillDirList unimplemented");

	return RuntimeValue();
}

// ListBox: import int FillSaveGameList()
// Fills the list box with all the current user's saved games.
RuntimeValue Script_ListBox_FillSaveGameList(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::FillSaveGameList unimplemented");

	return RuntimeValue();
}

// ListBox: import int GetItemAtLocation(int x, int y)
// Gets the item index at the specified screen co-ordinates, if they lie within the list box.
RuntimeValue Script_ListBox_GetItemAtLocation(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("ListBox::GetItemAtLocation unimplemented");

	return RuntimeValue();
}

// ListBox: import void GetItemText(int listIndex, string buffer)
// Undocumented.
RuntimeValue Script_ListBox_GetItemText(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int listIndex = params[0]._signedValue;
	UNUSED(listIndex);
	ScriptString *buffer = (ScriptString *)params[1]._object;
	UNUSED(buffer);

	// FIXME
	error("ListBox::GetItemText unimplemented");

	return RuntimeValue();
}

// ListBox: import void SetItemText(int listIndex, const string newText)
// Undocumented.
RuntimeValue Script_ListBox_SetItemText(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int listIndex = params[0]._signedValue;
	UNUSED(listIndex);
	ScriptString *newText = (ScriptString *)params[1]._object;
	UNUSED(newText);

	// FIXME
	error("ListBox::SetItemText unimplemented");

	return RuntimeValue();
}

// ListBox: import bool InsertItemAt(int listIndex, const string text)
// Inserts a new item before the specified index.
RuntimeValue Script_ListBox_InsertItemAt(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int listIndex = params[0]._signedValue;
	UNUSED(listIndex);
	ScriptString *text = (ScriptString *)params[1]._object;
	UNUSED(text);

	// FIXME
	error("ListBox::InsertItemAt unimplemented");

	return RuntimeValue();
}

// ListBox: import void RemoveItem(int listIndex)
// Removes the specified item from the list.
RuntimeValue Script_ListBox_RemoveItem(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int listIndex = params[0]._signedValue;
	UNUSED(listIndex);

	// FIXME
	error("ListBox::RemoveItem unimplemented");

	return RuntimeValue();
}

// ListBox: import void ScrollDown()
// Scrolls the list down one row.
RuntimeValue Script_ListBox_ScrollDown(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::ScrollDown unimplemented");

	return RuntimeValue();
}

// ListBox: import void ScrollUp()
// Scrolls the list up one row.
RuntimeValue Script_ListBox_ScrollUp(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::ScrollUp unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute FontType Font
// Gets/sets the font used to draw the list items.
RuntimeValue Script_ListBox_get_Font(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::get_Font unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute FontType Font
// Gets/sets the font used to draw the list items.
RuntimeValue Script_ListBox_set_Font(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("ListBox::set_Font unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute bool HideBorder
// Gets/sets whether the border around the list box is hidden.
RuntimeValue Script_ListBox_get_HideBorder(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::get_HideBorder unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute bool HideBorder
// Gets/sets whether the border around the list box is hidden.
RuntimeValue Script_ListBox_set_HideBorder(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("ListBox::set_HideBorder unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute bool HideScrollArrows
// Gets/sets whether the clickable scroll arrows are hidden.
RuntimeValue Script_ListBox_get_HideScrollArrows(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::get_HideScrollArrows unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute bool HideScrollArrows
// Gets/sets whether the clickable scroll arrows are hidden.
RuntimeValue Script_ListBox_set_HideScrollArrows(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("ListBox::set_HideScrollArrows unimplemented");

	return RuntimeValue();
}

// ListBox: readonly import attribute int ItemCount
// Gets the number of items currently in the list.
RuntimeValue Script_ListBox_get_ItemCount(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::get_ItemCount unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute String Items[]
// Accesses the text for the items in the list.
RuntimeValue Script_ListBox_geti_Items(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);

	// FIXME
	error("ListBox::geti_Items unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute String Items[]
// Accesses the text for the items in the list.
RuntimeValue Script_ListBox_seti_Items(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);
	ScriptString *value = (ScriptString *)params[1]._object;
	UNUSED(value);

	// FIXME
	error("ListBox::seti_Items unimplemented");

	return RuntimeValue();
}

// ListBox: readonly import attribute int RowCount
// Gets the number of visible rows that the listbox can display.
RuntimeValue Script_ListBox_get_RowCount(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::get_RowCount unimplemented");

	return RuntimeValue();
}

// ListBox: readonly import attribute int SaveGameSlots[]
// Gets the save game number that each row in the list corresponds to, after using FillSaveGameList.
RuntimeValue Script_ListBox_geti_SaveGameSlots(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);

	// FIXME
	error("ListBox::geti_SaveGameSlots unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute int SelectedIndex
// Gets/sets the currently selected item.
RuntimeValue Script_ListBox_get_SelectedIndex(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::get_SelectedIndex unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute int SelectedIndex
// Gets/sets the currently selected item.
RuntimeValue Script_ListBox_set_SelectedIndex(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("ListBox::set_SelectedIndex unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute int TopItem
// Gets/sets the first visible item in the list.
RuntimeValue Script_ListBox_get_TopItem(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("ListBox::get_TopItem unimplemented");

	return RuntimeValue();
}

// ListBox: import attribute int TopItem
// Gets/sets the first visible item in the list.
RuntimeValue Script_ListBox_set_TopItem(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("ListBox::set_TopItem unimplemented");

	return RuntimeValue();
}

// GUI: import void Centre()
// Moves the GUI to be centred on the screen.
RuntimeValue Script_GUI_Centre(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::Centre unimplemented");

	return RuntimeValue();
}

// GUI: import static GUI* GetAtScreenXY(int x, int y)
// Gets the topmost GUI visible on the screen at the specified co-ordinates.
RuntimeValue Script_GUI_GetAtScreenXY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("GUI::GetAtScreenXY unimplemented");

	return RuntimeValue();
}

// GUI: import void SetPosition(int x, int y)
// Moves the GUI to have its top-left corner at the specified position.
RuntimeValue Script_GUI_SetPosition(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("GUI::SetPosition unimplemented");

	return RuntimeValue();
}

// GUI: import void SetSize(int width, int height)
// Changes the size of the GUI.
RuntimeValue Script_GUI_SetSize(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int width = params[0]._signedValue;
	UNUSED(width);
	int height = params[1]._signedValue;
	UNUSED(height);

	// FIXME
	error("GUI::SetSize unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int BackgroundGraphic
// Gets/sets the sprite used to draw the GUI's background image.
RuntimeValue Script_GUI_get_BackgroundGraphic(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::get_BackgroundGraphic unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int BackgroundGraphic
// Gets/sets the sprite used to draw the GUI's background image.
RuntimeValue Script_GUI_set_BackgroundGraphic(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUI::set_BackgroundGraphic unimplemented");

	return RuntimeValue();
}

// GUI: import attribute bool Clickable
// Gets/sets whether the GUI can be clicked on, or whether clicks pass straight through.
RuntimeValue Script_GUI_get_Clickable(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	return ((self->_flags & GUIF_NOCLICK) == 0);
}

// GUI: import attribute bool Clickable
// Gets/sets whether the GUI can be clicked on, or whether clicks pass straight through.
RuntimeValue Script_GUI_set_Clickable(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	if (value)
		self->_flags &= ~GUIF_NOCLICK;
	else
		self->_flags |= GUIF_NOCLICK;

	return RuntimeValue();
}

// GUI: readonly import attribute GUIControl *Controls[]
// Accesses the controls that are on this GUI.
RuntimeValue Script_GUI_geti_Controls(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	uint index = params[0]._value;

	if (index >= self->_controls.size())
		error("GUI::geti_Controls: control %d is too high (only have %d controls)", index, self->_controls.size());

	return self->_controls[index];
}

// GUI: readonly import attribute int ControlCount
// Gets the number of controls on this GUI.
RuntimeValue Script_GUI_get_ControlCount(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	return self->_controls.size();
}

// GUI: import attribute int Height
// Gets/sets the height of the GUI.
RuntimeValue Script_GUI_get_Height(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::get_Height unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int Height
// Gets/sets the height of the GUI.
RuntimeValue Script_GUI_set_Height(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUI::set_Height unimplemented");

	return RuntimeValue();
}

// GUI: readonly import attribute int ID
// Gets the ID number of the GUI.
RuntimeValue Script_GUI_get_ID(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::get_ID unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int Transparency
// Gets/sets the transparency of the GUI.
RuntimeValue Script_GUI_get_Transparency(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::get_Transparency unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int Transparency
// Gets/sets the transparency of the GUI.
RuntimeValue Script_GUI_set_Transparency(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUI::set_Transparency unimplemented");

	return RuntimeValue();
}

// GUI: import attribute bool Visible
// Gets/sets whether the GUI is visible.
RuntimeValue Script_GUI_get_Visible(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::get_Visible unimplemented");

	return RuntimeValue();
}

// GUI: import attribute bool Visible
// Gets/sets whether the GUI is visible.
RuntimeValue Script_GUI_set_Visible(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("GUI::set_Visible unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int Width
// Gets/sets the width of the GUI.
RuntimeValue Script_GUI_get_Width(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::get_Width unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int Width
// Gets/sets the width of the GUI.
RuntimeValue Script_GUI_set_Width(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUI::set_Width unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int X
// Gets/sets the X co-ordinate of the GUI's top-left corner.
RuntimeValue Script_GUI_get_X(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::get_X unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int X
// Gets/sets the X co-ordinate of the GUI's top-left corner.
RuntimeValue Script_GUI_set_X(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUI::set_X unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int Y
// Gets/sets the Y co-ordinate of the GUI's top-left corner.
RuntimeValue Script_GUI_get_Y(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::get_Y unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int Y
// Gets/sets the Y co-ordinate of the GUI's top-left corner.
RuntimeValue Script_GUI_set_Y(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUI::set_Y unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int ZOrder
// Gets/sets the GUI's z-order relative to other GUIs.
RuntimeValue Script_GUI_get_ZOrder(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GUI::get_ZOrder unimplemented");

	return RuntimeValue();
}

// GUI: import attribute int ZOrder
// Gets/sets the GUI's z-order relative to other GUIs.
RuntimeValue Script_GUI_set_ZOrder(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("GUI::set_ZOrder unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "DisableInterface", (ScriptAPIFunction *)&Script_DisableInterface, "", sotNone },
	{ "EnableInterface", (ScriptAPIFunction *)&Script_EnableInterface, "", sotNone },
	{ "IsInterfaceEnabled", (ScriptAPIFunction *)&Script_IsInterfaceEnabled, "", sotNone },
	{ "SetTextWindowGUI", (ScriptAPIFunction *)&Script_SetTextWindowGUI, "i", sotNone },
	{ "FindGUIID", (ScriptAPIFunction *)&Script_FindGUIID, "s", sotNone },
	{ "SetInvDimensions", (ScriptAPIFunction *)&Script_SetInvDimensions, "ii", sotNone },
	{ "GetGUIAt", (ScriptAPIFunction *)&Script_GetGUIAt, "ii", sotNone },
	{ "GetGUIObjectAt", (ScriptAPIFunction *)&Script_GetGUIObjectAt, "ii", sotNone },
	{ "InterfaceOn", (ScriptAPIFunction *)&Script_InterfaceOn, "i", sotNone },
	{ "InterfaceOff", (ScriptAPIFunction *)&Script_InterfaceOff, "i", sotNone },
	{ "SetGUIPosition", (ScriptAPIFunction *)&Script_SetGUIPosition, "iii", sotNone },
	{ "SetGUISize", (ScriptAPIFunction *)&Script_SetGUISize, "iii", sotNone },
	{ "CentreGUI", (ScriptAPIFunction *)&Script_CentreGUI, "i", sotNone },
	{ "IsGUIOn", (ScriptAPIFunction *)&Script_IsGUIOn, "i", sotNone },
	{ "SetGUIBackgroundPic", (ScriptAPIFunction *)&Script_SetGUIBackgroundPic, "ii", sotNone },
	{ "SetGUITransparency", (ScriptAPIFunction *)&Script_SetGUITransparency, "ii", sotNone },
	{ "SetGUIClickable", (ScriptAPIFunction *)&Script_SetGUIClickable, "ii", sotNone },
	{ "SetGUIZOrder", (ScriptAPIFunction *)&Script_SetGUIZOrder, "ii", sotNone },
	{ "SetGUIObjectEnabled", (ScriptAPIFunction *)&Script_SetGUIObjectEnabled, "iii", sotNone },
	{ "SetGUIObjectPosition", (ScriptAPIFunction *)&Script_SetGUIObjectPosition, "iiii", sotNone },
	{ "SetGUIObjectSize", (ScriptAPIFunction *)&Script_SetGUIObjectSize, "iiii", sotNone },
	{ "SetLabelColor", (ScriptAPIFunction *)&Script_SetLabelColor, "iii", sotNone },
	{ "SetLabelText", (ScriptAPIFunction *)&Script_SetLabelText, "iis", sotNone },
	{ "SetLabelFont", (ScriptAPIFunction *)&Script_SetLabelFont, "iii", sotNone },
	{ "SetButtonText", (ScriptAPIFunction *)&Script_SetButtonText, "iis", sotNone },
	{ "SetButtonPic", (ScriptAPIFunction *)&Script_SetButtonPic, "iiii", sotNone },
	{ "GetButtonPic", (ScriptAPIFunction *)&Script_GetButtonPic, "iii", sotNone },
	{ "AnimateButton", (ScriptAPIFunction *)&Script_AnimateButton, "iiiiii", sotNone },
	{ "SetSliderValue", (ScriptAPIFunction *)&Script_SetSliderValue, "iii", sotNone },
	{ "GetSliderValue", (ScriptAPIFunction *)&Script_GetSliderValue, "ii", sotNone },
	{ "SetTextBoxFont", (ScriptAPIFunction *)&Script_SetTextBoxFont, "iii", sotNone },
	{ "GetTextBoxText", (ScriptAPIFunction *)&Script_GetTextBoxText, "iis", sotNone },
	{ "SetTextBoxText", (ScriptAPIFunction *)&Script_SetTextBoxText, "iis", sotNone },
	{ "ListBoxClear", (ScriptAPIFunction *)&Script_ListBoxClear, "ii", sotNone },
	{ "ListBoxAdd", (ScriptAPIFunction *)&Script_ListBoxAdd, "iis", sotNone },
	{ "ListBoxGetSelected", (ScriptAPIFunction *)&Script_ListBoxGetSelected, "ii", sotNone },
	{ "ListBoxGetItemText", (ScriptAPIFunction *)&Script_ListBoxGetItemText, "iiis", sotNone },
	{ "ListBoxSetSelected", (ScriptAPIFunction *)&Script_ListBoxSetSelected, "iii", sotNone },
	{ "ListBoxSetTopItem", (ScriptAPIFunction *)&Script_ListBoxSetTopItem, "iii", sotNone },
	{ "ListBoxDirList", (ScriptAPIFunction *)&Script_ListBoxDirList, "iis", sotNone },
	{ "ListBoxGetNumItems", (ScriptAPIFunction *)&Script_ListBoxGetNumItems, "ii", sotNone },
	{ "ListBoxSaveGameList", (ScriptAPIFunction *)&Script_ListBoxSaveGameList, "ii", sotNone },
	{ "ListBoxRemove", (ScriptAPIFunction *)&Script_ListBoxRemove, "iii", sotNone },
	{ "GUIControl::BringToFront^0", (ScriptAPIFunction *)&Script_GUIControl_BringToFront, "", sotGUIControl },
	{ "GUIControl::GetAtScreenXY^2", (ScriptAPIFunction *)&Script_GUIControl_GetAtScreenXY, "ii", sotNone },
	{ "GUIControl::SendToBack^0", (ScriptAPIFunction *)&Script_GUIControl_SendToBack, "", sotGUIControl },
	{ "GUIControl::SetPosition^2", (ScriptAPIFunction *)&Script_GUIControl_SetPosition, "ii", sotGUIControl },
	{ "GUIControl::SetSize^2", (ScriptAPIFunction *)&Script_GUIControl_SetSize, "ii", sotGUIControl },
	{ "GUIControl::get_AsButton", (ScriptAPIFunction *)&Script_GUIControl_get_AsButton, "", sotGUIControl },
	{ "GUIControl::get_AsInvWindow", (ScriptAPIFunction *)&Script_GUIControl_get_AsInvWindow, "", sotGUIControl },
	{ "GUIControl::get_AsLabel", (ScriptAPIFunction *)&Script_GUIControl_get_AsLabel, "", sotGUIControl },
	{ "GUIControl::get_AsListBox", (ScriptAPIFunction *)&Script_GUIControl_get_AsListBox, "", sotGUIControl },
	{ "GUIControl::get_AsSlider", (ScriptAPIFunction *)&Script_GUIControl_get_AsSlider, "", sotGUIControl },
	{ "GUIControl::get_AsTextBox", (ScriptAPIFunction *)&Script_GUIControl_get_AsTextBox, "", sotGUIControl },
	{ "GUIControl::get_Clickable", (ScriptAPIFunction *)&Script_GUIControl_get_Clickable, "", sotGUIControl },
	{ "GUIControl::set_Clickable", (ScriptAPIFunction *)&Script_GUIControl_set_Clickable, "i", sotGUIControl },
	{ "GUIControl::get_Enabled", (ScriptAPIFunction *)&Script_GUIControl_get_Enabled, "", sotGUIControl },
	{ "GUIControl::set_Enabled", (ScriptAPIFunction *)&Script_GUIControl_set_Enabled, "i", sotGUIControl },
	{ "GUIControl::get_Height", (ScriptAPIFunction *)&Script_GUIControl_get_Height, "", sotGUIControl },
	{ "GUIControl::set_Height", (ScriptAPIFunction *)&Script_GUIControl_set_Height, "i", sotGUIControl },
	{ "GUIControl::get_ID", (ScriptAPIFunction *)&Script_GUIControl_get_ID, "", sotGUIControl },
	{ "GUIControl::get_OwningGUI", (ScriptAPIFunction *)&Script_GUIControl_get_OwningGUI, "", sotGUIControl },
	{ "GUIControl::get_Visible", (ScriptAPIFunction *)&Script_GUIControl_get_Visible, "", sotGUIControl },
	{ "GUIControl::set_Visible", (ScriptAPIFunction *)&Script_GUIControl_set_Visible, "i", sotGUIControl },
	{ "GUIControl::get_Width", (ScriptAPIFunction *)&Script_GUIControl_get_Width, "", sotGUIControl },
	{ "GUIControl::set_Width", (ScriptAPIFunction *)&Script_GUIControl_set_Width, "i", sotGUIControl },
	{ "GUIControl::get_X", (ScriptAPIFunction *)&Script_GUIControl_get_X, "", sotGUIControl },
	{ "GUIControl::set_X", (ScriptAPIFunction *)&Script_GUIControl_set_X, "i", sotGUIControl },
	{ "GUIControl::get_Y", (ScriptAPIFunction *)&Script_GUIControl_get_Y, "", sotGUIControl },
	{ "GUIControl::set_Y", (ScriptAPIFunction *)&Script_GUIControl_set_Y, "i", sotGUIControl },
	{ "Label::GetText^1", (ScriptAPIFunction *)&Script_Label_GetText, "s", sotNone },
	{ "Label::SetText^1", (ScriptAPIFunction *)&Script_Label_SetText, "s", sotNone },
	{ "Label::get_Font", (ScriptAPIFunction *)&Script_Label_get_Font, "", sotNone },
	{ "Label::set_Font", (ScriptAPIFunction *)&Script_Label_set_Font, "i", sotNone },
	{ "Label::get_Text", (ScriptAPIFunction *)&Script_Label_get_Text, "", sotNone },
	{ "Label::set_Text", (ScriptAPIFunction *)&Script_Label_set_Text, "s", sotNone },
	{ "Label::get_TextColor", (ScriptAPIFunction *)&Script_Label_get_TextColor, "", sotNone },
	{ "Label::set_TextColor", (ScriptAPIFunction *)&Script_Label_set_TextColor, "i", sotNone },
	{ "Button::Animate^4", (ScriptAPIFunction *)&Script_Button_Animate, "iiii", sotNone },
	{ "Button::GetText^1", (ScriptAPIFunction *)&Script_Button_GetText, "s", sotNone },
	{ "Button::SetText^1", (ScriptAPIFunction *)&Script_Button_SetText, "s", sotNone },
	{ "Button::get_ClipImage", (ScriptAPIFunction *)&Script_Button_get_ClipImage, "", sotNone },
	{ "Button::set_ClipImage", (ScriptAPIFunction *)&Script_Button_set_ClipImage, "i", sotNone },
	{ "Button::get_Font", (ScriptAPIFunction *)&Script_Button_get_Font, "", sotNone },
	{ "Button::set_Font", (ScriptAPIFunction *)&Script_Button_set_Font, "i", sotNone },
	{ "Button::get_Graphic", (ScriptAPIFunction *)&Script_Button_get_Graphic, "", sotNone },
	{ "Button::get_MouseOverGraphic", (ScriptAPIFunction *)&Script_Button_get_MouseOverGraphic, "", sotNone },
	{ "Button::set_MouseOverGraphic", (ScriptAPIFunction *)&Script_Button_set_MouseOverGraphic, "i", sotNone },
	{ "Button::get_NormalGraphic", (ScriptAPIFunction *)&Script_Button_get_NormalGraphic, "", sotNone },
	{ "Button::set_NormalGraphic", (ScriptAPIFunction *)&Script_Button_set_NormalGraphic, "i", sotNone },
	{ "Button::get_PushedGraphic", (ScriptAPIFunction *)&Script_Button_get_PushedGraphic, "", sotNone },
	{ "Button::set_PushedGraphic", (ScriptAPIFunction *)&Script_Button_set_PushedGraphic, "i", sotNone },
	{ "Button::get_TextColor", (ScriptAPIFunction *)&Script_Button_get_TextColor, "", sotNone },
	{ "Button::set_TextColor", (ScriptAPIFunction *)&Script_Button_set_TextColor, "i", sotNone },
	{ "Button::get_Text", (ScriptAPIFunction *)&Script_Button_get_Text, "", sotNone },
	{ "Button::set_Text", (ScriptAPIFunction *)&Script_Button_set_Text, "s", sotNone },
	{ "Slider::get_BackgroundGraphic", (ScriptAPIFunction *)&Script_Slider_get_BackgroundGraphic, "", sotNone },
	{ "Slider::set_BackgroundGraphic", (ScriptAPIFunction *)&Script_Slider_set_BackgroundGraphic, "i", sotNone },
	{ "Slider::get_HandleGraphic", (ScriptAPIFunction *)&Script_Slider_get_HandleGraphic, "", sotNone },
	{ "Slider::set_HandleGraphic", (ScriptAPIFunction *)&Script_Slider_set_HandleGraphic, "i", sotNone },
	{ "Slider::get_HandleOffset", (ScriptAPIFunction *)&Script_Slider_get_HandleOffset, "", sotNone },
	{ "Slider::set_HandleOffset", (ScriptAPIFunction *)&Script_Slider_set_HandleOffset, "i", sotNone },
	{ "Slider::get_Max", (ScriptAPIFunction *)&Script_Slider_get_Max, "", sotNone },
	{ "Slider::set_Max", (ScriptAPIFunction *)&Script_Slider_set_Max, "i", sotNone },
	{ "Slider::get_Min", (ScriptAPIFunction *)&Script_Slider_get_Min, "", sotNone },
	{ "Slider::set_Min", (ScriptAPIFunction *)&Script_Slider_set_Min, "i", sotNone },
	{ "Slider::get_Value", (ScriptAPIFunction *)&Script_Slider_get_Value, "", sotNone },
	{ "Slider::set_Value", (ScriptAPIFunction *)&Script_Slider_set_Value, "i", sotNone },
	{ "TextBox::GetText^1", (ScriptAPIFunction *)&Script_TextBox_GetText, "s", sotNone },
	{ "TextBox::SetText^1", (ScriptAPIFunction *)&Script_TextBox_SetText, "s", sotNone },
	{ "TextBox::get_Font", (ScriptAPIFunction *)&Script_TextBox_get_Font, "", sotNone },
	{ "TextBox::set_Font", (ScriptAPIFunction *)&Script_TextBox_set_Font, "i", sotNone },
	{ "TextBox::get_Text", (ScriptAPIFunction *)&Script_TextBox_get_Text, "", sotNone },
	{ "TextBox::set_Text", (ScriptAPIFunction *)&Script_TextBox_set_Text, "s", sotNone },
	{ "TextBox::get_TextColor", (ScriptAPIFunction *)&Script_TextBox_get_TextColor, "", sotNone },
	{ "TextBox::set_TextColor", (ScriptAPIFunction *)&Script_TextBox_set_TextColor, "i", sotNone },
	{ "InvWindow::ScrollDown^0", (ScriptAPIFunction *)&Script_InvWindow_ScrollDown, "", sotNone },
	{ "InvWindow::ScrollUp^0", (ScriptAPIFunction *)&Script_InvWindow_ScrollUp, "", sotNone },
	{ "InvWindow::get_CharacterToUse", (ScriptAPIFunction *)&Script_InvWindow_get_CharacterToUse, "", sotNone },
	{ "InvWindow::set_CharacterToUse", (ScriptAPIFunction *)&Script_InvWindow_set_CharacterToUse, "o", sotNone },
	{ "InvWindow::geti_ItemAtIndex", (ScriptAPIFunction *)&Script_InvWindow_geti_ItemAtIndex, "i", sotNone },
	{ "InvWindow::get_ItemCount", (ScriptAPIFunction *)&Script_InvWindow_get_ItemCount, "", sotNone },
	{ "InvWindow::get_ItemHeight", (ScriptAPIFunction *)&Script_InvWindow_get_ItemHeight, "", sotNone },
	{ "InvWindow::set_ItemHeight", (ScriptAPIFunction *)&Script_InvWindow_set_ItemHeight, "i", sotNone },
	{ "InvWindow::get_ItemWidth", (ScriptAPIFunction *)&Script_InvWindow_get_ItemWidth, "", sotNone },
	{ "InvWindow::set_ItemWidth", (ScriptAPIFunction *)&Script_InvWindow_set_ItemWidth, "i", sotNone },
	{ "InvWindow::get_TopItem", (ScriptAPIFunction *)&Script_InvWindow_get_TopItem, "", sotNone },
	{ "InvWindow::set_TopItem", (ScriptAPIFunction *)&Script_InvWindow_set_TopItem, "i", sotNone },
	{ "InvWindow::get_ItemsPerRow", (ScriptAPIFunction *)&Script_InvWindow_get_ItemsPerRow, "", sotNone },
	{ "InvWindow::get_RowCount", (ScriptAPIFunction *)&Script_InvWindow_get_RowCount, "", sotNone },
	{ "ListBox::AddItem^1", (ScriptAPIFunction *)&Script_ListBox_AddItem, "s", sotNone },
	{ "ListBox::Clear^0", (ScriptAPIFunction *)&Script_ListBox_Clear, "", sotNone },
	{ "ListBox::FillDirList^1", (ScriptAPIFunction *)&Script_ListBox_FillDirList, "s", sotNone },
	{ "ListBox::FillSaveGameList^0", (ScriptAPIFunction *)&Script_ListBox_FillSaveGameList, "", sotNone },
	{ "ListBox::GetItemAtLocation^2", (ScriptAPIFunction *)&Script_ListBox_GetItemAtLocation, "ii", sotNone },
	{ "ListBox::GetItemText^2", (ScriptAPIFunction *)&Script_ListBox_GetItemText, "is", sotNone },
	{ "ListBox::SetItemText^2", (ScriptAPIFunction *)&Script_ListBox_SetItemText, "is", sotNone },
	{ "ListBox::InsertItemAt^2", (ScriptAPIFunction *)&Script_ListBox_InsertItemAt, "is", sotNone },
	{ "ListBox::RemoveItem^1", (ScriptAPIFunction *)&Script_ListBox_RemoveItem, "i", sotNone },
	{ "ListBox::ScrollDown^0", (ScriptAPIFunction *)&Script_ListBox_ScrollDown, "", sotNone },
	{ "ListBox::ScrollUp^0", (ScriptAPIFunction *)&Script_ListBox_ScrollUp, "", sotNone },
	{ "ListBox::get_Font", (ScriptAPIFunction *)&Script_ListBox_get_Font, "", sotNone },
	{ "ListBox::set_Font", (ScriptAPIFunction *)&Script_ListBox_set_Font, "i", sotNone },
	{ "ListBox::get_HideBorder", (ScriptAPIFunction *)&Script_ListBox_get_HideBorder, "", sotNone },
	{ "ListBox::set_HideBorder", (ScriptAPIFunction *)&Script_ListBox_set_HideBorder, "i", sotNone },
	{ "ListBox::get_HideScrollArrows", (ScriptAPIFunction *)&Script_ListBox_get_HideScrollArrows, "", sotNone },
	{ "ListBox::set_HideScrollArrows", (ScriptAPIFunction *)&Script_ListBox_set_HideScrollArrows, "i", sotNone },
	{ "ListBox::get_ItemCount", (ScriptAPIFunction *)&Script_ListBox_get_ItemCount, "", sotNone },
	{ "ListBox::geti_Items", (ScriptAPIFunction *)&Script_ListBox_geti_Items, "i", sotNone },
	{ "ListBox::seti_Items", (ScriptAPIFunction *)&Script_ListBox_seti_Items, "is", sotNone },
	{ "ListBox::get_RowCount", (ScriptAPIFunction *)&Script_ListBox_get_RowCount, "", sotNone },
	{ "ListBox::geti_SaveGameSlots", (ScriptAPIFunction *)&Script_ListBox_geti_SaveGameSlots, "i", sotNone },
	{ "ListBox::get_SelectedIndex", (ScriptAPIFunction *)&Script_ListBox_get_SelectedIndex, "", sotNone },
	{ "ListBox::set_SelectedIndex", (ScriptAPIFunction *)&Script_ListBox_set_SelectedIndex, "i", sotNone },
	{ "ListBox::get_TopItem", (ScriptAPIFunction *)&Script_ListBox_get_TopItem, "", sotNone },
	{ "ListBox::set_TopItem", (ScriptAPIFunction *)&Script_ListBox_set_TopItem, "i", sotNone },
	{ "GUI::Centre^0", (ScriptAPIFunction *)&Script_GUI_Centre, "", sotNone },
	{ "GUI::GetAtScreenXY^2", (ScriptAPIFunction *)&Script_GUI_GetAtScreenXY, "ii", sotNone },
	{ "GUI::SetPosition^2", (ScriptAPIFunction *)&Script_GUI_SetPosition, "ii", sotNone },
	{ "GUI::SetSize^2", (ScriptAPIFunction *)&Script_GUI_SetSize, "ii", sotNone },
	{ "GUI::get_BackgroundGraphic", (ScriptAPIFunction *)&Script_GUI_get_BackgroundGraphic, "", sotNone },
	{ "GUI::set_BackgroundGraphic", (ScriptAPIFunction *)&Script_GUI_set_BackgroundGraphic, "i", sotNone },
	{ "GUI::get_Clickable", (ScriptAPIFunction *)&Script_GUI_get_Clickable, "", sotNone },
	{ "GUI::set_Clickable", (ScriptAPIFunction *)&Script_GUI_set_Clickable, "i", sotNone },
	{ "GUI::geti_Controls", (ScriptAPIFunction *)&Script_GUI_geti_Controls, "i", sotNone },
	{ "GUI::get_ControlCount", (ScriptAPIFunction *)&Script_GUI_get_ControlCount, "", sotNone },
	{ "GUI::get_Height", (ScriptAPIFunction *)&Script_GUI_get_Height, "", sotNone },
	{ "GUI::set_Height", (ScriptAPIFunction *)&Script_GUI_set_Height, "i", sotNone },
	{ "GUI::get_ID", (ScriptAPIFunction *)&Script_GUI_get_ID, "", sotNone },
	{ "GUI::get_Transparency", (ScriptAPIFunction *)&Script_GUI_get_Transparency, "", sotNone },
	{ "GUI::set_Transparency", (ScriptAPIFunction *)&Script_GUI_set_Transparency, "i", sotNone },
	{ "GUI::get_Visible", (ScriptAPIFunction *)&Script_GUI_get_Visible, "", sotNone },
	{ "GUI::set_Visible", (ScriptAPIFunction *)&Script_GUI_set_Visible, "i", sotNone },
	{ "GUI::get_Width", (ScriptAPIFunction *)&Script_GUI_get_Width, "", sotNone },
	{ "GUI::set_Width", (ScriptAPIFunction *)&Script_GUI_set_Width, "i", sotNone },
	{ "GUI::get_X", (ScriptAPIFunction *)&Script_GUI_get_X, "", sotNone },
	{ "GUI::set_X", (ScriptAPIFunction *)&Script_GUI_set_X, "i", sotNone },
	{ "GUI::get_Y", (ScriptAPIFunction *)&Script_GUI_get_Y, "", sotNone },
	{ "GUI::set_Y", (ScriptAPIFunction *)&Script_GUI_set_Y, "i", sotNone },
	{ "GUI::get_ZOrder", (ScriptAPIFunction *)&Script_GUI_get_ZOrder, "", sotNone },
	{ "GUI::set_ZOrder", (ScriptAPIFunction *)&Script_GUI_set_ZOrder, "i", sotNone },
};

void addGuiSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
