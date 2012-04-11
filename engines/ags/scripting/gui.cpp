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
 * You may also modify/distribute the code in this file under that license.
 */

#include "engines/ags/scripting/scripting.h"
#include "engines/ags/gui.h"
#include "engines/ags/character.h"
#include "engines/ags/constants.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"
#include "engines/ags/room.h"
#include "engines/ags/sprites.h"

namespace AGS {

static GUIControl *getGUIControl(const char *funcName, AGSEngine *vm, uint guiId, uint objectId) {
	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("%s: GUI %d is too high (only have %d)", funcName, guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];
	if (objectId >= group->_controls.size())
		error("%s: Control %d is too high (only have %d)", funcName, objectId, group->_controls.size());
	return group->_controls[objectId];
}

// import void DisableInterface()
// Disables the player interface and activates the Wait cursor.
RuntimeValue Script_DisableInterface(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	vm->_state->_disabledUserInterface++;
	vm->invalidateGUI();
	vm->_graphics->setMouseCursor(CURS_WAIT);

	return RuntimeValue();
}

// import void EnableInterface()
// Re-enables the player interface.
RuntimeValue Script_EnableInterface(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	vm->invalidateGUI();
	vm->_state->_disabledUserInterface--;
	if (!vm->_state->_disabledUserInterface) {
		vm->setDefaultCursor();
	}

	return RuntimeValue();
}

// import int IsInterfaceEnabled()
// Checks whether the player interface is currently enabled.
RuntimeValue Script_IsInterfaceEnabled(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return (vm->_state->_disabledUserInterface > 0) ? 0 : 1;
}

// import void SetTextWindowGUI (int gui)
// Changes the GUI used to render standard game text windows.
RuntimeValue Script_SetTextWindowGUI(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int guiId = params[0]._signedValue;

	if (guiId < -1 || (uint)guiId >= vm->_gameFile->_guiGroups.size())
		error("SetTextWindowGUI: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	if (guiId != -1 && !vm->_gameFile->_guiGroups[guiId]->isTextWindow())
		error("SetTextWindowGUI: GUI %d is not a text window", guiId);

	if (vm->_state->_speechTextWindowGUI == vm->getGameOption(OPT_TWCUSTOM))
		vm->_state->_speechTextWindowGUI = guiId;
	vm->_gameFile->_options[OPT_TWCUSTOM] = guiId;

	return RuntimeValue();
}

// import int FindGUIID(const string)
// Return the ID of the GUI with the provided name.
// Internal function used only in automatically-generated code.
RuntimeValue Script_FindGUIID(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *string = (ScriptString *)params[0]._object;
	Common::String name = string->getString();

	const Common::Array<GUIGroup *> &groups = vm->_gameFile->_guiGroups;
	for (uint i = 0; i < groups.size(); ++i) {
		// check for exact match
		if (groups[i]->_name == name)
			return i;

		// check for old-style match
		if (groups[i]->_name.empty() || groups[i]->_name[0] != 'g')
			continue;
		if (groups[i]->_name.equalsIgnoreCase('g' + name))
			return i;
	}

	error("FindGUIID: Couldn't find a match for GUI '%s'", name.c_str());
}

// import void SetInvDimensions(int width, int height)
// Obsolete GUI function.
RuntimeValue Script_SetInvDimensions(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint width = params[0]._value;
	uint height = params[1]._value;

	vm->_state->_invItemWidth = width;
	vm->_state->_invItemHeight = height;
	vm->_state->_invNumDisplayed = 0;

	// backwards compatibility
	for (uint i = 0; i < vm->_gameFile->_guiInvControls.size(); ++i) {
		vm->_gameFile->_guiInvControls[i]->_itemWidth = width;
		vm->_gameFile->_guiInvControls[i]->_itemHeight = height;
		vm->_gameFile->_guiInvControls[i]->resized();
	}

	vm->invalidateGUI();

	return RuntimeValue();
}

// import int GetGUIAt (int x, int y)
// Returns the ID of the topmost enabled and clickable GUI at the given screen coordinates, or -1 if there isn't one.
RuntimeValue Script_GetGUIAt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;

	return vm->getGUIAt(Common::Point(x, y));
}

// import int GetGUIObjectAt (int x, int y)
// Returns the ID of the visible control at the given screen coordinates, or -1 if there isn't one.
// (Note that this only works when GetGUIAt returns the GUI with the control in it.)
RuntimeValue Script_GetGUIObjectAt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;

	uint guiId = vm->getGUIAt(Common::Point(x, y));
	if (guiId == (uint)-1)
		return 0;
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	vm->multiplyUpCoordinates(x, y);
	GUIControl *control = group->getControlAt(Common::Point(x - group->_x, y - group->_y), false);
	if (!control)
		return -1;
	return control->_id;
}

// import void InterfaceOn(int gui)
// Enable the specified GUI.
RuntimeValue Script_InterfaceOn(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("InterfaceOn: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	group->interfaceOn();

	return RuntimeValue();
}

// import void InterfaceOff(int gui)
// Disable the specified GUI.
RuntimeValue Script_InterfaceOff(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("InterfaceOff: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	group->interfaceOff();

	return RuntimeValue();
}

// import void SetGUIPosition(int gui, int x, int y)
// Set the position of the specified GUI.
RuntimeValue Script_SetGUIPosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	int x = params[1]._signedValue;
	int y = params[2]._signedValue;

	if (vm->getCurrentRoom() && (x >= vm->getCurrentRoom()->_width || y >= vm->getCurrentRoom()->_height))
		error("SetGUIPosition: (%d, %d) is outside room boundaries", x, y);

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("SetGUIPosition: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	group->_x = vm->multiplyUpCoordinate(x);
	group->_y = vm->multiplyUpCoordinate(y);

	return RuntimeValue();
}

// import void SetGUISize(int gui, int width, int height)
// Set the size of the specified GUI.
RuntimeValue Script_SetGUISize(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	int width = params[1]._signedValue;
	int height = params[2]._signedValue;

	if (width < 0 || width > vm->_graphics->_baseWidth || height < 0 || height > vm->_graphics->_baseHeight)
		error("SetGUISize: Tried resizing to an invalid size (%dx%d)", width, height);

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("SetGUISize: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	vm->multiplyUpCoordinates(width, height);
	group->setSize(width, height);

	return RuntimeValue();
}

// import void CentreGUI(int gui)
// Move the specified GUI to the centre of the screen.
RuntimeValue Script_CentreGUI(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("CentreGUI: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	group->_x = vm->_graphics->_width / 2 - group->_width / 2;
	group->_y = vm->_graphics->_height / 2 - group->_height / 2;

	return RuntimeValue();
}

// import int IsGUIOn (int gui)
// Returns whether the specified GUI is displayed.
RuntimeValue Script_IsGUIOn(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("IsGUIOn: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	return (group->_visible ? 1 : 0);
}

// import void SetGUIBackgroundPic (int gui, int spriteSlot)
// Set the background picture of the specified GUI.
RuntimeValue Script_SetGUIBackgroundPic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint spriteSlot = params[1]._signedValue;

	if (spriteSlot >= vm->getSprites()->getSpriteCount())
		error("SetGUIBackgroundPic: sprite %d is too high (only have %d sprites)",
			spriteSlot, vm->getSprites()->getSpriteCount());

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("SetGUIBackgroundPic: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	group->setBackgroundPicture(spriteSlot);

	return RuntimeValue();
}

// import void SetGUITransparency(int gui, int amount)
// Obsolete GUI function.
RuntimeValue Script_SetGUITransparency(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint amount = params[1]._value;

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("SetGUITransparency: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	if (amount > 100)
		error("SetGUITransparency: transparency value must be between 0 and 100, but got %d", amount);

	group->setTransparency(amount);

	return RuntimeValue();
}

// import void SetGUIClickable(int gui, int clickable)
// Set whether clicks are enabled on the specified GUI.
RuntimeValue Script_SetGUIClickable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	int clickable = params[1]._signedValue;

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("SetGUIClickable: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	if (clickable)
		group->_flags &= ~GUIF_NOCLICK;
	else
		group->_flags |= GUIF_NOCLICK;

	return RuntimeValue();
}

// import void SetGUIZOrder(int gui, int z)
// Set the z-order value of the specified GUI.
RuntimeValue Script_SetGUIZOrder(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint z = params[1]._value;

	if (guiId >= vm->_gameFile->_guiGroups.size())
		error("SetGUIZOrder: GUI %d is too high (only have %d)", guiId, vm->_gameFile->_guiGroups.size());
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	group->setZOrder(z);

	return RuntimeValue();
}

// import void SetGUIObjectEnabled(int gui, int object, int enable)
// Undocumented.
RuntimeValue Script_SetGUIObjectEnabled(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	uint enable = params[2]._value;

	GUIControl *control = getGUIControl("SetGUIObjectEnabled", vm, guiId, objectId);
	control->setEnabled((bool)enable);

	return RuntimeValue();
}

// import void SetGUIObjectPosition(int gui, int object, int x, int y)
// Undocumented.
RuntimeValue Script_SetGUIObjectPosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	int x = params[2]._signedValue;
	int y = params[3]._signedValue;

	GUIControl *control = getGUIControl("SetGUIObjectPosition", vm, guiId, objectId);

	control->_x = vm->multiplyUpCoordinate(x);
	control->_y = vm->multiplyUpCoordinate(y);

	control->_parent->controlPositionsChanged();
	control->_parent->invalidate();

	return RuntimeValue();
}

// import void SetGUIObjectSize(int gui, int object, int width, int height)
// Undocumented.
RuntimeValue Script_SetGUIObjectSize(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	uint width = params[2]._value;
	uint height = params[3]._value;

	if (width < 2 || height < 2)
		error("SetGUIObjectSize: new size %dx%d is too small (must be at least 2x2)", width, height);

	GUIControl *control = getGUIControl("SetGUIObjectSize", vm, guiId, objectId);
	control->resize(vm->multiplyUpCoordinate(width), vm->multiplyUpCoordinate(height));

	return RuntimeValue();
}

// import void SetLabelColor(int gui, int object, int colour)
// Undocumented.
RuntimeValue Script_SetLabelColor(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	uint colour = params[2]._value;

	GUIControl *control = getGUIControl("SetLabelColor", vm, guiId, objectId);
	if (!control->isOfType(sotGUILabel))
		error("SetLabelColor: Control %d isn't a label.", objectId);
	GUILabel *label = (GUILabel *)control;

	label->setColor(colour);
	return RuntimeValue();
}

// import void SetLabelText(int gui, int object, const string text)
// Set the text of the specified label.
RuntimeValue Script_SetLabelText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	ScriptString *text = (ScriptString *)params[2]._object;

	GUIControl *control = getGUIControl("SetLabelText", vm, guiId, objectId);
	if (!control->isOfType(sotGUILabel))
		error("SetLabelText: Control %d isn't a label.", objectId);
	GUILabel *label = (GUILabel *)control;

	label->setText(vm->getTranslation(text->getString()));
	return RuntimeValue();
}

// import void SetLabelFont(int gui, int object, FontType)
// Undocumented.
RuntimeValue Script_SetLabelFont(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	uint32 fontId = params[2]._value;

	GUIControl *control = getGUIControl("SetLabelFont", vm, guiId, objectId);
	if (!control->isOfType(sotGUILabel))
		error("SetLabelFont: Control %d isn't a label.", objectId);
	GUILabel *label = (GUILabel *)control;

	if (fontId >= vm->_gameFile->_fonts.size())
		error("SetLabelFont: font %d is invalid (only %d fonts)", fontId, vm->_gameFile->_fonts.size());

	label->setFont(fontId);
	return RuntimeValue();
}

// import void SetButtonText(int gui, int object, const string text)
// Undocumented.
RuntimeValue Script_SetButtonText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	ScriptString *text = (ScriptString *)params[2]._object;

	GUIControl *control = getGUIControl("SetButtonText", vm, guiId, objectId);
	if (!control->isOfType(sotGUIButton))
		error("SetButtonText: Control %d isn't a button.", objectId);
	GUIButton *button = (GUIButton *)control;

	button->setText(text->getString());

	return RuntimeValue();
}

// import void SetButtonPic(int gui, int object, int which, int spriteSlot)
// Change the specified button's picture.
// The 'which' parameter is 1 to change the normal picture, 2 for the mouse-over or 3 for the pushed picture.
// You can specify a slot of -1 to disable the mouse-over and pushed pictures.
RuntimeValue Script_SetButtonPic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	uint picType = params[2]._value;
	uint spriteSlot = params[3]._value;

	GUIControl *control = getGUIControl("SetButtonPic", vm, guiId, objectId);
	if (!control->isOfType(sotGUIButton))
		error("SetButtonPic: Control %d isn't a button.", objectId);
	GUIButton *button = (GUIButton *)control;

	switch (picType) {
	case 1:
		button->setNormalGraphic(spriteSlot);
		break;
	case 2:
		button->setMouseOverGraphic(spriteSlot);
		break;
	case 3:
		button->setPushedGraphic(spriteSlot);
		break;
	default:
		error("SetButtonPic: Picture type %d is invalid.", picType);
	}

	return RuntimeValue();
}

// import int GetButtonPic(int gui, int object, int which)
// Returns the button picture of the specified type (see SetButtonPic).
RuntimeValue Script_GetButtonPic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	uint picType = params[2]._value;

	GUIControl *control = getGUIControl("GetButtonPic", vm, guiId, objectId);
	if (!control->isOfType(sotGUIButton))
		error("GetButtonPic: Control %d isn't a button.", objectId);
	GUIButton *button = (GUIButton *)control;

	switch (picType) {
	case 1:
		return button->getNormalGraphic();
	case 2:
		return button->getMouseOverGraphic();
	case 3:
		return button->getPushedGraphic();
	}

	error("GetButtonPic: Picture type %d is invalid.", picType);
}

// import void AnimateButton(int gui, int object, int view, int loop, int delay, int repeat)
// Undocumented.
RuntimeValue Script_AnimateButton(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	int view = params[2]._signedValue;
	UNUSED(view);
	int loop = params[3]._signedValue;
	UNUSED(loop);
	int delay = params[4]._signedValue;
	UNUSED(delay);
	int repeat = params[5]._signedValue;
	UNUSED(repeat);

	GUIControl *control = getGUIControl("AnimateButton", vm, guiId, objectId);
	if (!control->isOfType(sotGUIButton))
		error("AnimateButton: Control %d isn't a button.", objectId);
	GUIButton *button = (GUIButton *)control;

	// FIXME
	UNUSED(button);
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
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	uint32 fonttype = params[2]._value;

	if (fonttype >= vm->_gameFile->_fonts.size())
		error("SetTextBoxFont: font %d is invalid (only %d fonts)", fonttype, vm->_gameFile->_fonts.size());

	GUIControl *control = getGUIControl("SetTextBoxFont", vm, guiId, objectId);
	if (!control->isOfType(sotGUITextBox))
		error("SetTextBoxFont: Control %d isn't a textbox.", objectId);
	GUITextBox *textbox = (GUITextBox *)control;

	textbox->setFont(fonttype);

	return RuntimeValue();
}

// import void GetTextBoxText(int gui, int object, string buffer)
// Undocumented.
RuntimeValue Script_GetTextBoxText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	ScriptString *buffer = (ScriptString *)params[2]._object;

	GUIControl *control = getGUIControl("GetTextBoxText", vm, guiId, objectId);
	if (!control->isOfType(sotGUITextBox))
		error("GetTextBoxText: Control %d isn't a textbox.", objectId);
	GUITextBox *textbox = (GUITextBox *)control;

	buffer->setString(textbox->getText());

	return RuntimeValue();
}

// import void SetTextBoxText(int gui, int object, const string text)
// Undocumented.
RuntimeValue Script_SetTextBoxText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint guiId = params[0]._value;
	uint objectId = params[1]._value;
	ScriptString *text = (ScriptString *)params[2]._object;
	UNUSED(text);

	GUIControl *control = getGUIControl("SetTextBoxText", vm, guiId, objectId);
	if (!control->isOfType(sotGUITextBox))
		error("SetTextBoxText: Control %d isn't a textbox.", objectId);
	GUITextBox *textbox = (GUITextBox *)control;

	textbox->setText(text->getString());

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
	int y = params[1]._signedValue;

	uint guiId = vm->getGUIAt(Common::Point(x, y));
	if (guiId == (uint)-1)
		return 0;
	GUIGroup *group = vm->_gameFile->_guiGroups[guiId];

	vm->multiplyUpCoordinates(x, y);
	GUIControl *control = group->getControlAt(Common::Point(x - group->_x, y - group->_y), false);
	if (!control)
		return 0;
	return control;
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
	int y = params[1]._signedValue;

	self->_x = vm->multiplyUpCoordinate(x);
	self->_y = vm->multiplyUpCoordinate(y);

	self->_parent->controlPositionsChanged();
	self->_parent->invalidate();

	return RuntimeValue();
}

// GUIControl: import void SetSize(int width, int height)
// Changes the control to the specified size.
RuntimeValue Script_GUIControl_SetSize(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	uint width = params[0]._value;
	uint height = params[1]._value;

	if (width < 2 || height < 2)
		error("GUIControl::SetSize: new size %dx%d is too small (must be at least 2x2)", width, height);

	self->resize(vm->multiplyUpCoordinate(width), vm->multiplyUpCoordinate(height));

	return RuntimeValue();
}

// GUIControl: readonly import attribute Button* AsButton
// If this control is a button, returns the Button interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsButton(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	if (self->isOfType(sotGUIButton))
		return self;
	else
		return 0;
}

// GUIControl: readonly import attribute InvWindow* AsInvWindow
// If this control is a inventory window, returns the InvWindow interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsInvWindow(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	if (self->isOfType(sotGUIInvWindow))
		return self;
	else
		return 0;
}

// GUIControl: readonly import attribute Label* AsLabel
// If this control is a label, returns the Label interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsLabel(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	if (self->isOfType(sotGUILabel))
		return self;
	else
		return 0;
}

// GUIControl: readonly import attribute ListBox* AsListBox
// If this control is a list box, returns the ListBox interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsListBox(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	if (self->isOfType(sotGUIListBox))
		return self;
	else
		return 0;
}

// GUIControl: readonly import attribute Slider* AsSlider
// If this control is a slider, returns the Slider interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsSlider(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	if (self->isOfType(sotGUISlider))
		return self;
	else
		return 0;
}

// GUIControl: readonly import attribute TextBox* AsTextBox
// If this control is a text box, returns the TextBox interface; otherwise null.
RuntimeValue Script_GUIControl_get_AsTextBox(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	if (self->isOfType(sotGUITextBox))
		return self;
	else
		return 0;
}

// GUIControl: import attribute bool Clickable
// Gets/sets whether this control can be clicked on or whether clicks pass straight through it.
RuntimeValue Script_GUIControl_get_Clickable(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	return self->isClickable() ? 1 : 0;
}

// GUIControl: import attribute bool Clickable
// Gets/sets whether this control can be clicked on or whether clicks pass straight through it.
RuntimeValue Script_GUIControl_set_Clickable(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	self->setClickable((bool)value);

	return RuntimeValue();
}

// GUIControl: import attribute bool Enabled
// Gets/sets whether this control is currently enabled.
RuntimeValue Script_GUIControl_get_Enabled(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	return self->isDisabled() ? 0 : 1;
}

// GUIControl: import attribute bool Enabled
// Gets/sets whether this control is currently enabled.
RuntimeValue Script_GUIControl_set_Enabled(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	self->setEnabled((bool)value);

	return RuntimeValue();
}

// GUIControl: import attribute int Height
// Gets/sets the height of the control.
RuntimeValue Script_GUIControl_get_Height(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(self->_height);
}

// GUIControl: import attribute int Height
// Gets/sets the height of the control.
RuntimeValue Script_GUIControl_set_Height(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->resize(self->_width, vm->multiplyUpCoordinate(value));

	return RuntimeValue();
}

// GUIControl: readonly import attribute int ID
// Gets the ID number of the control within its owning GUI.
RuntimeValue Script_GUIControl_get_ID(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	return self->_id;
}

// GUIControl: readonly import attribute GUI* OwningGUI
// Gets the GUI that this control is placed onto.
RuntimeValue Script_GUIControl_get_OwningGUI(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	return self->_parent;
}

// GUIControl: import attribute bool Visible
// Gets/sets whether this control is currently visible.
RuntimeValue Script_GUIControl_get_Visible(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	return self->isVisible() ? 1 : 0;
}

// GUIControl: import attribute bool Visible
// Gets/sets whether this control is currently visible.
RuntimeValue Script_GUIControl_set_Visible(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	bool val = value ? 1 : 0;
	if (val == self->isVisible())
		return RuntimeValue();

	if (val)
		self->show();
	else
		self->hide();
	self->_parent->controlPositionsChanged();
	self->_parent->invalidate();

	return RuntimeValue();
}

// GUIControl: import attribute int Width
// Gets/sets the width of the control.
RuntimeValue Script_GUIControl_get_Width(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(self->_width);
}

// GUIControl: import attribute int Width
// Gets/sets the width of the control.
RuntimeValue Script_GUIControl_set_Width(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->resize(vm->multiplyUpCoordinate(value), self->_height);

	return RuntimeValue();
}

// GUIControl: import attribute int X
// Gets/sets the X position of the control's top-left corner.
RuntimeValue Script_GUIControl_get_X(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(self->_x);
}

// GUIControl: import attribute int X
// Gets/sets the X position of the control's top-left corner.
RuntimeValue Script_GUIControl_set_X(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	self->_x = vm->multiplyUpCoordinate(value);

	self->_parent->controlPositionsChanged();
	self->_parent->invalidate();

	return RuntimeValue();
}

// GUIControl: import attribute int Y
// Gets/sets the Y position of the control's top-left corner.
RuntimeValue Script_GUIControl_get_Y(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(self->_y);
}

// GUIControl: import attribute int Y
// Gets/sets the Y position of the control's top-left corner.
RuntimeValue Script_GUIControl_set_Y(AGSEngine *vm, GUIControl *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	self->_y = vm->multiplyUpCoordinate(value);

	self->_parent->controlPositionsChanged();
	self->_parent->invalidate();

	return RuntimeValue();
}

// Label: import void GetText(string buffer)
// Undocumented.
RuntimeValue Script_Label_GetText(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;

	buffer->setString(self->getText());

	return RuntimeValue();
}

// Label: import void SetText(const string text)
// Undocumented.
RuntimeValue Script_Label_SetText(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;

	self->setText(text->getString());

	return RuntimeValue();
}

// Label: import attribute FontType Font
// Gets/sets the font that is used to draw the label text.
RuntimeValue Script_Label_get_Font(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	return self->getFont();
}

// Label: import attribute FontType Font
// Gets/sets the font that is used to draw the label text.
RuntimeValue Script_Label_set_Font(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	if (value >= vm->_gameFile->_fonts.size())
		error("Label::set_Font: font %d is invalid (only %d fonts)", value, vm->_gameFile->_fonts.size());

	self->setFont(value);

	return RuntimeValue();
}

// Label: import attribute String Text
// Gets/sets the text that is shown on the label.
RuntimeValue Script_Label_get_Text(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	RuntimeValue ret = new ScriptMutableString(self->getText());
	ret._object->DecRef();
	return ret;
}

// Label: import attribute String Text
// Gets/sets the text that is shown on the label.
RuntimeValue Script_Label_set_Text(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *value = (ScriptString *)params[0]._object;

	self->setText(value->getString());

	return RuntimeValue();
}

// Label: import attribute int TextColor
// Gets/sets the colour in which the label text is drawn.
RuntimeValue Script_Label_get_TextColor(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	return self->getColor();
}

// Label: import attribute int TextColor
// Gets/sets the colour in which the label text is drawn.
RuntimeValue Script_Label_set_TextColor(AGSEngine *vm, GUILabel *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->setColor(value);

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

	buffer->setString(self->getText());

	return RuntimeValue();
}

// Button: import void SetText(const string text)
// Undocumented.
RuntimeValue Script_Button_SetText(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;

	self->setText(text->getString());

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
	return self->getFont();
}

// Button: import attribute FontType Font
// Gets/sets the font used to display text on the button.
RuntimeValue Script_Button_set_Font(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	if (value >= vm->_gameFile->_fonts.size())
		error("Button::set_Font: font %d is invalid (only %d fonts)", value, vm->_gameFile->_fonts.size());

	self->setFont(value);

	return RuntimeValue();
}

// Button: readonly import attribute int Graphic
// Gets the currently displayed sprite number.
RuntimeValue Script_Button_get_Graphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	return self->getDisplayedGraphic();
}

// Button: import attribute int MouseOverGraphic
// Gets/sets the image that is shown when the player moves the mouse over the button (-1 if none)
RuntimeValue Script_Button_get_MouseOverGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	return self->getMouseOverGraphic();
}

// Button: import attribute int MouseOverGraphic
// Gets/sets the image that is shown when the player moves the mouse over the button (-1 if none)
RuntimeValue Script_Button_set_MouseOverGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->setMouseOverGraphic(value);

	return RuntimeValue();
}

// Button: import attribute int NormalGraphic
// Gets/sets the image that is shown when the button is not pressed or mouse-overed
RuntimeValue Script_Button_get_NormalGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	return self->getNormalGraphic();
}

// Button: import attribute int NormalGraphic
// Gets/sets the image that is shown when the button is not pressed or mouse-overed
RuntimeValue Script_Button_set_NormalGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->setNormalGraphic(value);

	return RuntimeValue();
}

// Button: import attribute int PushedGraphic
// Gets/sets the image that is shown when the button is pressed
RuntimeValue Script_Button_get_PushedGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	return self->getPushedGraphic();
}

// Button: import attribute int PushedGraphic
// Gets/sets the image that is shown when the button is pressed
RuntimeValue Script_Button_set_PushedGraphic(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->setPushedGraphic(value);

	return RuntimeValue();
}

// Button: import attribute int TextColor
// Gets/sets the colour in which the button text is drawn.
RuntimeValue Script_Button_get_TextColor(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	return self->getTextColor();
}

// Button: import attribute int TextColor
// Gets/sets the colour in which the button text is drawn.
RuntimeValue Script_Button_set_TextColor(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->setTextColor(value);

	return RuntimeValue();
}

// Button: import attribute String Text
// Gets/sets the text to be drawn on the button.
RuntimeValue Script_Button_get_Text(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	RuntimeValue ret = new ScriptMutableString(self->getText());
	ret._object->DecRef();
	return ret;
}

// Button: import attribute String Text
// Gets/sets the text to be drawn on the button.
RuntimeValue Script_Button_set_Text(AGSEngine *vm, GUIButton *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *value = (ScriptString *)params[0]._object;

	self->setText(value->getString());

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
	return self->getHandleOffset();
}

// Slider: import attribute int HandleOffset
// Gets/sets the pixel offset at which the handle is drawn.
RuntimeValue Script_Slider_set_HandleOffset(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	self->setHandleOffset(value);

	return RuntimeValue();
}

// Slider: import attribute int Max
// Gets/sets the maximum value that the slider can have.
RuntimeValue Script_Slider_get_Max(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	return self->getMax();
}

// Slider: import attribute int Max
// Gets/sets the maximum value that the slider can have.
RuntimeValue Script_Slider_set_Max(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	self->setMax(value);

	return RuntimeValue();
}

// Slider: import attribute int Min
// Gets/sets the minimum value that the slider can have.
RuntimeValue Script_Slider_get_Min(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	return self->getMin();
}

// Slider: import attribute int Min
// Gets/sets the minimum value that the slider can have.
RuntimeValue Script_Slider_set_Min(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	self->setMin(value);

	return RuntimeValue();
}

// Slider: import attribute int Value
// Gets/sets the current value of the slider.
RuntimeValue Script_Slider_get_Value(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	return self->getValue();
}

// Slider: import attribute int Value
// Gets/sets the current value of the slider.
RuntimeValue Script_Slider_set_Value(AGSEngine *vm, GUISlider *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	self->setValue(value);

	return RuntimeValue();
}

// TextBox: import void GetText(string buffer)
// Undocumented.
RuntimeValue Script_TextBox_GetText(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;

	buffer->setString(self->getText());

	return RuntimeValue();
}

// TextBox: import void SetText(const string text)
// Undocumented.
RuntimeValue Script_TextBox_SetText(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *text = (ScriptString *)params[0]._object;

	self->setText(text->getString());

	return RuntimeValue();
}

// TextBox: import attribute FontType Font
// Gets/sets the font used to draw the text in the text box.
RuntimeValue Script_TextBox_get_Font(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	return self->getFont();
}

// TextBox: import attribute FontType Font
// Gets/sets the font used to draw the text in the text box.
RuntimeValue Script_TextBox_set_Font(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	if (value >= vm->_gameFile->_fonts.size())
		error("TextBox::set_Font: font %d is invalid (only %d fonts)", value, vm->_gameFile->_fonts.size());

	self->setFont(value);

	return RuntimeValue();
}

// TextBox: import attribute String Text
// Gets/sets the text that is currently in the text box.
RuntimeValue Script_TextBox_get_Text(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	RuntimeValue ret = new ScriptMutableString(self->getText());
	ret._object->DecRef();
	return ret;
}

// TextBox: import attribute String Text
// Gets/sets the text that is currently in the text box.
RuntimeValue Script_TextBox_set_Text(AGSEngine *vm, GUITextBox *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *value = (ScriptString *)params[0]._object;

	self->setText(value->getString());

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
	if (self->_charId == (uint)-1)
		return 0;

	return vm->_characters[self->_charId];
}

// InvWindow: import attribute Character* CharacterToUse
// Gets/sets which character's inventory is displayed in this window.
RuntimeValue Script_InvWindow_set_CharacterToUse(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	if (params[0]._type == rvtInteger) {
		// null pointer
		self->_charId = (uint)-1;
	} else {
		if (!params[0]._object->isOfType(sotCharacter))
			error("InvWindow::set_CharacterToUse got incorrect object type (expected a Character) for parameter 1");
		Character *value = (Character *)params[0]._object;
		self->_charId = value->_indexId;
	}

	self->_topIndex = 0;
	self->_parent->invalidate();

	return RuntimeValue();
}

// InvWindow: readonly import attribute InventoryItem* ItemAtIndex[]
// Gets the inventory item at the specified index in the window.
RuntimeValue Script_InvWindow_geti_ItemAtIndex(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	uint index = params[0]._value;

	Character *chr = self->getCharToDisplay();
	if (index >= chr->_invOrder.size())
		return 0;

	return &vm->_gameFile->_invItemInfo[chr->_invOrder[index]];
}

// InvWindow: readonly import attribute int ItemCount
// Gets the number of inventory items currently shown in the window.
RuntimeValue Script_InvWindow_get_ItemCount(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	Character *chr = self->getCharToDisplay();

	return chr->_invOrder.size();
}

// InvWindow: import attribute int ItemHeight
// Gets the height of each row of items.
RuntimeValue Script_InvWindow_get_ItemHeight(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	return self->_itemHeight;
}

// InvWindow: import attribute int ItemHeight
// Gets the height of each row of items.
RuntimeValue Script_InvWindow_set_ItemHeight(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->_itemHeight = value;
	self->resized();

	return RuntimeValue();
}

// InvWindow: import attribute int ItemWidth
// Gets the width of each column of items.
RuntimeValue Script_InvWindow_get_ItemWidth(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	return self->_itemWidth;
}

// InvWindow: import attribute int ItemWidth
// Gets the width of each column of items.
RuntimeValue Script_InvWindow_set_ItemWidth(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->_itemWidth = value;
	self->resized();

	return RuntimeValue();
}

// InvWindow: import attribute int TopItem
// Gets the index of the first visible item in the window.
RuntimeValue Script_InvWindow_get_TopItem(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	return self->_topIndex;
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
	return self->_itemsPerLine;
}

// InvWindow: readonly import attribute int RowCount
// Gets the number of visible rows in this inventory window.
RuntimeValue Script_InvWindow_get_RowCount(AGSEngine *vm, GUIInvControl *self, const Common::Array<RuntimeValue> &params) {
	return self->_numLines;
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
	return self->getFont();
}

// ListBox: import attribute FontType Font
// Gets/sets the font used to draw the list items.
RuntimeValue Script_ListBox_set_Font(AGSEngine *vm, GUIListBox *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	if (value >= vm->_gameFile->_fonts.size())
		error("ListBox::set_Font: font %d is invalid (only %d fonts)", value, vm->_gameFile->_fonts.size());

	self->setFont(value);

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
	self->_x = vm->_graphics->_width / 2 - self->_width / 2;
	self->_y = vm->_graphics->_height / 2 - self->_height / 2;

	return RuntimeValue();
}

// GUI: import static GUI* GetAtScreenXY(int x, int y)
// Gets the topmost GUI visible on the screen at the specified co-ordinates.
RuntimeValue Script_GUI_GetAtScreenXY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;

	uint guiId = vm->getGUIAt(Common::Point(x, y));

	if (guiId == (uint)-1)
		return 0;
	else
		return vm->_gameFile->_guiGroups[guiId];
}

// GUI: import void SetPosition(int x, int y)
// Moves the GUI to have its top-left corner at the specified position.
RuntimeValue Script_GUI_SetPosition(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;

	if (vm->getCurrentRoom() && (x >= vm->getCurrentRoom()->_width || y >= vm->getCurrentRoom()->_height))
		error("GUI::SetPosition: (%d, %d) is outside room boundaries", x, y);

	self->_x = vm->multiplyUpCoordinate(x);
	self->_y = vm->multiplyUpCoordinate(y);

	return RuntimeValue();
}

// GUI: import void SetSize(int width, int height)
// Changes the size of the GUI.
RuntimeValue Script_GUI_SetSize(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int width = params[0]._signedValue;
	int height = params[1]._signedValue;

	if (width < 0 || width > vm->_graphics->_baseWidth || height < 0 || height > vm->_graphics->_baseHeight)
		error("GUI::SetSize: Tried resizing to an invalid size (%dx%d)", width, height);

	vm->multiplyUpCoordinates(width, height);
	self->setSize(width, height);

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
	int slot = params[0]._value;

	if (slot != -1 && (uint)slot >= vm->getSprites()->getSpriteCount())
		error("GUI::set_BackgroundGraphic: sprite %d is too high (only have %d sprites)",
			slot, vm->getSprites()->getSpriteCount());

	self->setBackgroundPicture(slot);

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
	return vm->divideDownCoordinate(self->_height);
}

// GUI: import attribute int Height
// Gets/sets the height of the GUI.
RuntimeValue Script_GUI_set_Height(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	self->setSize(self->_width, vm->multiplyUpCoordinate(value));

	return RuntimeValue();
}

// GUI: readonly import attribute int ID
// Gets the ID number of the GUI.
RuntimeValue Script_GUI_get_ID(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	return self->_id;
}

// GUI: import attribute int Transparency
// Gets/sets the transparency of the GUI.
RuntimeValue Script_GUI_get_Transparency(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	return self->getTransparency();
}

// GUI: import attribute int Transparency
// Gets/sets the transparency of the GUI.
RuntimeValue Script_GUI_set_Transparency(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	uint trans = params[0]._value;

	if (trans > 100)
		error("GUI::set_Transparency: transparency value must be between 0 and 100, but got %d", trans);

	self->setTransparency(trans);

	return RuntimeValue();
}

// GUI: import attribute bool Visible
// Gets/sets whether the GUI is visible (or disabled(!), for mouse ypos popups).
RuntimeValue Script_GUI_get_Visible(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	return (self->_visible || !self->_enabled) ? 1 : 0;
}

// GUI: import attribute bool Visible
// Gets/sets whether the GUI is visible (or enabled, for mouse ypos popups).
RuntimeValue Script_GUI_set_Visible(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	if (value)
		self->interfaceOn();
	else
		self->interfaceOff();

	return RuntimeValue();
}

// GUI: import attribute int Width
// Gets/sets the width of the GUI.
RuntimeValue Script_GUI_get_Width(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(self->_width);
}

// GUI: import attribute int Width
// Gets/sets the width of the GUI.
RuntimeValue Script_GUI_set_Width(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	self->setSize(vm->multiplyUpCoordinate(value), self->_height);

	return RuntimeValue();
}

// GUI: import attribute int X
// Gets/sets the X co-ordinate of the GUI's top-left corner.
RuntimeValue Script_GUI_get_X(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(self->_x);
}

// GUI: import attribute int X
// Gets/sets the X co-ordinate of the GUI's top-left corner.
RuntimeValue Script_GUI_set_X(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	if (vm->getCurrentRoom() && value >= vm->getCurrentRoom()->_width)
		error("GUI::set_X: %d is outside room boundaries", value);

	self->_x = vm->multiplyUpCoordinate(value);

	return RuntimeValue();
}

// GUI: import attribute int Y
// Gets/sets the Y co-ordinate of the GUI's top-left corner.
RuntimeValue Script_GUI_get_Y(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	return vm->divideDownCoordinate(self->_y);
}

// GUI: import attribute int Y
// Gets/sets the Y co-ordinate of the GUI's top-left corner.
RuntimeValue Script_GUI_set_Y(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	if (vm->getCurrentRoom() && value >= vm->getCurrentRoom()->_height)
		error("GUI::set_Y: %d is outside room boundaries", value);

	self->_y = vm->multiplyUpCoordinate(value);

	return RuntimeValue();
}

// GUI: import attribute int ZOrder
// Gets/sets the GUI's z-order relative to other GUIs.
RuntimeValue Script_GUI_get_ZOrder(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	return self->_zorder;

	return RuntimeValue();
}

// GUI: import attribute int ZOrder
// Gets/sets the GUI's z-order relative to other GUIs.
RuntimeValue Script_GUI_set_ZOrder(AGSEngine *vm, GUIGroup *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->setZOrder(value);

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
	{ "Label::GetText^1", (ScriptAPIFunction *)&Script_Label_GetText, "s", sotGUILabel },
	{ "Label::SetText^1", (ScriptAPIFunction *)&Script_Label_SetText, "s", sotGUILabel },
	{ "Label::get_Font", (ScriptAPIFunction *)&Script_Label_get_Font, "", sotGUILabel },
	{ "Label::set_Font", (ScriptAPIFunction *)&Script_Label_set_Font, "i", sotGUILabel },
	{ "Label::get_Text", (ScriptAPIFunction *)&Script_Label_get_Text, "", sotGUILabel },
	{ "Label::set_Text", (ScriptAPIFunction *)&Script_Label_set_Text, "s", sotGUILabel },
	{ "Label::get_TextColor", (ScriptAPIFunction *)&Script_Label_get_TextColor, "", sotGUILabel },
	{ "Label::set_TextColor", (ScriptAPIFunction *)&Script_Label_set_TextColor, "i", sotGUILabel },
	{ "Button::Animate^4", (ScriptAPIFunction *)&Script_Button_Animate, "iiii", sotGUIButton },
	{ "Button::GetText^1", (ScriptAPIFunction *)&Script_Button_GetText, "s", sotGUIButton },
	{ "Button::SetText^1", (ScriptAPIFunction *)&Script_Button_SetText, "s", sotGUIButton },
	{ "Button::get_ClipImage", (ScriptAPIFunction *)&Script_Button_get_ClipImage, "", sotGUIButton },
	{ "Button::set_ClipImage", (ScriptAPIFunction *)&Script_Button_set_ClipImage, "i", sotGUIButton },
	{ "Button::get_Font", (ScriptAPIFunction *)&Script_Button_get_Font, "", sotGUIButton },
	{ "Button::set_Font", (ScriptAPIFunction *)&Script_Button_set_Font, "i", sotGUIButton },
	{ "Button::get_Graphic", (ScriptAPIFunction *)&Script_Button_get_Graphic, "", sotGUIButton },
	{ "Button::get_MouseOverGraphic", (ScriptAPIFunction *)&Script_Button_get_MouseOverGraphic, "", sotGUIButton },
	{ "Button::set_MouseOverGraphic", (ScriptAPIFunction *)&Script_Button_set_MouseOverGraphic, "i", sotGUIButton },
	{ "Button::get_NormalGraphic", (ScriptAPIFunction *)&Script_Button_get_NormalGraphic, "", sotGUIButton },
	{ "Button::set_NormalGraphic", (ScriptAPIFunction *)&Script_Button_set_NormalGraphic, "i", sotGUIButton },
	{ "Button::get_PushedGraphic", (ScriptAPIFunction *)&Script_Button_get_PushedGraphic, "", sotGUIButton },
	{ "Button::set_PushedGraphic", (ScriptAPIFunction *)&Script_Button_set_PushedGraphic, "i", sotGUIButton },
	{ "Button::get_TextColor", (ScriptAPIFunction *)&Script_Button_get_TextColor, "", sotGUIButton },
	{ "Button::set_TextColor", (ScriptAPIFunction *)&Script_Button_set_TextColor, "i", sotGUIButton },
	{ "Button::get_Text", (ScriptAPIFunction *)&Script_Button_get_Text, "", sotGUIButton },
	{ "Button::set_Text", (ScriptAPIFunction *)&Script_Button_set_Text, "s", sotGUIButton },
	{ "Slider::get_BackgroundGraphic", (ScriptAPIFunction *)&Script_Slider_get_BackgroundGraphic, "", sotGUISlider },
	{ "Slider::set_BackgroundGraphic", (ScriptAPIFunction *)&Script_Slider_set_BackgroundGraphic, "i", sotGUISlider },
	{ "Slider::get_HandleGraphic", (ScriptAPIFunction *)&Script_Slider_get_HandleGraphic, "", sotGUISlider },
	{ "Slider::set_HandleGraphic", (ScriptAPIFunction *)&Script_Slider_set_HandleGraphic, "i", sotGUISlider },
	{ "Slider::get_HandleOffset", (ScriptAPIFunction *)&Script_Slider_get_HandleOffset, "", sotGUISlider },
	{ "Slider::set_HandleOffset", (ScriptAPIFunction *)&Script_Slider_set_HandleOffset, "i", sotGUISlider },
	{ "Slider::get_Max", (ScriptAPIFunction *)&Script_Slider_get_Max, "", sotGUISlider },
	{ "Slider::set_Max", (ScriptAPIFunction *)&Script_Slider_set_Max, "i", sotGUISlider },
	{ "Slider::get_Min", (ScriptAPIFunction *)&Script_Slider_get_Min, "", sotGUISlider },
	{ "Slider::set_Min", (ScriptAPIFunction *)&Script_Slider_set_Min, "i", sotGUISlider },
	{ "Slider::get_Value", (ScriptAPIFunction *)&Script_Slider_get_Value, "", sotGUISlider },
	{ "Slider::set_Value", (ScriptAPIFunction *)&Script_Slider_set_Value, "i", sotGUISlider },
	{ "TextBox::GetText^1", (ScriptAPIFunction *)&Script_TextBox_GetText, "s", sotGUITextBox },
	{ "TextBox::SetText^1", (ScriptAPIFunction *)&Script_TextBox_SetText, "s", sotGUITextBox },
	{ "TextBox::get_Font", (ScriptAPIFunction *)&Script_TextBox_get_Font, "", sotGUITextBox },
	{ "TextBox::set_Font", (ScriptAPIFunction *)&Script_TextBox_set_Font, "i", sotGUITextBox },
	{ "TextBox::get_Text", (ScriptAPIFunction *)&Script_TextBox_get_Text, "", sotGUITextBox },
	{ "TextBox::set_Text", (ScriptAPIFunction *)&Script_TextBox_set_Text, "s", sotGUITextBox },
	{ "TextBox::get_TextColor", (ScriptAPIFunction *)&Script_TextBox_get_TextColor, "", sotGUITextBox },
	{ "TextBox::set_TextColor", (ScriptAPIFunction *)&Script_TextBox_set_TextColor, "i", sotGUITextBox },
	{ "InvWindow::ScrollDown^0", (ScriptAPIFunction *)&Script_InvWindow_ScrollDown, "", sotGUIInvWindow },
	{ "InvWindow::ScrollUp^0", (ScriptAPIFunction *)&Script_InvWindow_ScrollUp, "", sotGUIInvWindow },
	{ "InvWindow::get_CharacterToUse", (ScriptAPIFunction *)&Script_InvWindow_get_CharacterToUse, "", sotGUIInvWindow },
	{ "InvWindow::set_CharacterToUse", (ScriptAPIFunction *)&Script_InvWindow_set_CharacterToUse, "t", sotGUIInvWindow },
	{ "InvWindow::geti_ItemAtIndex", (ScriptAPIFunction *)&Script_InvWindow_geti_ItemAtIndex, "i", sotGUIInvWindow },
	{ "InvWindow::get_ItemCount", (ScriptAPIFunction *)&Script_InvWindow_get_ItemCount, "", sotGUIInvWindow },
	{ "InvWindow::get_ItemHeight", (ScriptAPIFunction *)&Script_InvWindow_get_ItemHeight, "", sotGUIInvWindow },
	{ "InvWindow::set_ItemHeight", (ScriptAPIFunction *)&Script_InvWindow_set_ItemHeight, "i", sotGUIInvWindow },
	{ "InvWindow::get_ItemWidth", (ScriptAPIFunction *)&Script_InvWindow_get_ItemWidth, "", sotGUIInvWindow },
	{ "InvWindow::set_ItemWidth", (ScriptAPIFunction *)&Script_InvWindow_set_ItemWidth, "i", sotGUIInvWindow },
	{ "InvWindow::get_TopItem", (ScriptAPIFunction *)&Script_InvWindow_get_TopItem, "", sotGUIInvWindow },
	{ "InvWindow::set_TopItem", (ScriptAPIFunction *)&Script_InvWindow_set_TopItem, "i", sotGUIInvWindow },
	{ "InvWindow::get_ItemsPerRow", (ScriptAPIFunction *)&Script_InvWindow_get_ItemsPerRow, "", sotGUIInvWindow },
	{ "InvWindow::get_RowCount", (ScriptAPIFunction *)&Script_InvWindow_get_RowCount, "", sotGUIInvWindow },
	{ "ListBox::AddItem^1", (ScriptAPIFunction *)&Script_ListBox_AddItem, "s", sotGUIListBox },
	{ "ListBox::Clear^0", (ScriptAPIFunction *)&Script_ListBox_Clear, "", sotGUIListBox },
	{ "ListBox::FillDirList^1", (ScriptAPIFunction *)&Script_ListBox_FillDirList, "s", sotGUIListBox },
	{ "ListBox::FillSaveGameList^0", (ScriptAPIFunction *)&Script_ListBox_FillSaveGameList, "", sotGUIListBox },
	{ "ListBox::GetItemAtLocation^2", (ScriptAPIFunction *)&Script_ListBox_GetItemAtLocation, "ii", sotGUIListBox },
	{ "ListBox::GetItemText^2", (ScriptAPIFunction *)&Script_ListBox_GetItemText, "is", sotGUIListBox },
	{ "ListBox::SetItemText^2", (ScriptAPIFunction *)&Script_ListBox_SetItemText, "is", sotGUIListBox },
	{ "ListBox::InsertItemAt^2", (ScriptAPIFunction *)&Script_ListBox_InsertItemAt, "is", sotGUIListBox },
	{ "ListBox::RemoveItem^1", (ScriptAPIFunction *)&Script_ListBox_RemoveItem, "i", sotGUIListBox },
	{ "ListBox::ScrollDown^0", (ScriptAPIFunction *)&Script_ListBox_ScrollDown, "", sotGUIListBox },
	{ "ListBox::ScrollUp^0", (ScriptAPIFunction *)&Script_ListBox_ScrollUp, "", sotGUIListBox },
	{ "ListBox::get_Font", (ScriptAPIFunction *)&Script_ListBox_get_Font, "", sotGUIListBox },
	{ "ListBox::set_Font", (ScriptAPIFunction *)&Script_ListBox_set_Font, "i", sotGUIListBox },
	{ "ListBox::get_HideBorder", (ScriptAPIFunction *)&Script_ListBox_get_HideBorder, "", sotGUIListBox },
	{ "ListBox::set_HideBorder", (ScriptAPIFunction *)&Script_ListBox_set_HideBorder, "i", sotGUIListBox },
	{ "ListBox::get_HideScrollArrows", (ScriptAPIFunction *)&Script_ListBox_get_HideScrollArrows, "", sotGUIListBox },
	{ "ListBox::set_HideScrollArrows", (ScriptAPIFunction *)&Script_ListBox_set_HideScrollArrows, "i", sotGUIListBox },
	{ "ListBox::get_ItemCount", (ScriptAPIFunction *)&Script_ListBox_get_ItemCount, "", sotGUIListBox },
	{ "ListBox::geti_Items", (ScriptAPIFunction *)&Script_ListBox_geti_Items, "i", sotGUIListBox },
	{ "ListBox::seti_Items", (ScriptAPIFunction *)&Script_ListBox_seti_Items, "is", sotGUIListBox },
	{ "ListBox::get_RowCount", (ScriptAPIFunction *)&Script_ListBox_get_RowCount, "", sotGUIListBox },
	{ "ListBox::geti_SaveGameSlots", (ScriptAPIFunction *)&Script_ListBox_geti_SaveGameSlots, "i", sotGUIListBox },
	{ "ListBox::get_SelectedIndex", (ScriptAPIFunction *)&Script_ListBox_get_SelectedIndex, "", sotGUIListBox },
	{ "ListBox::set_SelectedIndex", (ScriptAPIFunction *)&Script_ListBox_set_SelectedIndex, "i", sotGUIListBox },
	{ "ListBox::get_TopItem", (ScriptAPIFunction *)&Script_ListBox_get_TopItem, "", sotGUIListBox },
	{ "ListBox::set_TopItem", (ScriptAPIFunction *)&Script_ListBox_set_TopItem, "i", sotGUIListBox },
	{ "GUI::Centre^0", (ScriptAPIFunction *)&Script_GUI_Centre, "", sotGUI },
	{ "GUI::GetAtScreenXY^2", (ScriptAPIFunction *)&Script_GUI_GetAtScreenXY, "ii", sotNone },
	{ "GUI::SetPosition^2", (ScriptAPIFunction *)&Script_GUI_SetPosition, "ii", sotGUI },
	{ "GUI::SetSize^2", (ScriptAPIFunction *)&Script_GUI_SetSize, "ii", sotGUI },
	{ "GUI::get_BackgroundGraphic", (ScriptAPIFunction *)&Script_GUI_get_BackgroundGraphic, "", sotGUI },
	{ "GUI::set_BackgroundGraphic", (ScriptAPIFunction *)&Script_GUI_set_BackgroundGraphic, "i", sotGUI },
	{ "GUI::get_Clickable", (ScriptAPIFunction *)&Script_GUI_get_Clickable, "", sotGUI },
	{ "GUI::set_Clickable", (ScriptAPIFunction *)&Script_GUI_set_Clickable, "i", sotGUI },
	{ "GUI::geti_Controls", (ScriptAPIFunction *)&Script_GUI_geti_Controls, "i", sotGUI },
	{ "GUI::get_ControlCount", (ScriptAPIFunction *)&Script_GUI_get_ControlCount, "", sotGUI },
	{ "GUI::get_Height", (ScriptAPIFunction *)&Script_GUI_get_Height, "", sotGUI },
	{ "GUI::set_Height", (ScriptAPIFunction *)&Script_GUI_set_Height, "i", sotGUI },
	{ "GUI::get_ID", (ScriptAPIFunction *)&Script_GUI_get_ID, "", sotGUI },
	{ "GUI::get_Transparency", (ScriptAPIFunction *)&Script_GUI_get_Transparency, "", sotGUI },
	{ "GUI::set_Transparency", (ScriptAPIFunction *)&Script_GUI_set_Transparency, "i", sotGUI },
	{ "GUI::get_Visible", (ScriptAPIFunction *)&Script_GUI_get_Visible, "", sotGUI },
	{ "GUI::set_Visible", (ScriptAPIFunction *)&Script_GUI_set_Visible, "i", sotGUI },
	{ "GUI::get_Width", (ScriptAPIFunction *)&Script_GUI_get_Width, "", sotGUI },
	{ "GUI::set_Width", (ScriptAPIFunction *)&Script_GUI_set_Width, "i", sotGUI },
	{ "GUI::get_X", (ScriptAPIFunction *)&Script_GUI_get_X, "", sotGUI },
	{ "GUI::set_X", (ScriptAPIFunction *)&Script_GUI_set_X, "i", sotGUI },
	{ "GUI::get_Y", (ScriptAPIFunction *)&Script_GUI_get_Y, "", sotGUI },
	{ "GUI::set_Y", (ScriptAPIFunction *)&Script_GUI_set_Y, "i", sotGUI },
	{ "GUI::get_ZOrder", (ScriptAPIFunction *)&Script_GUI_get_ZOrder, "", sotGUI },
	{ "GUI::set_ZOrder", (ScriptAPIFunction *)&Script_GUI_set_ZOrder, "i", sotGUI },
};

void addGuiSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
