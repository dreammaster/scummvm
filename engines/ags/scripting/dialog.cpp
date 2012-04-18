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
#include "engines/ags/constants.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/gamestate.h"

namespace AGS {

enum DialogOptionsState {
	kDialogOptionOff = 0,
	kDialogOptionOn = 1,
	kDialogOptionOffForever = 2
};

// import void SetDialogOption(int topic, int option, DialogOptionState)
// Undocumented.
RuntimeValue Script_SetDialogOption(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint topic = params[0]._value;
	uint option = params[1]._value;
	uint32 state = params[2]._value;

	// TODO: duplicates most of Dialog::SetOptionState

	if (topic >= vm->_gameFile->_dialogs.size())
		error("SetDialogOption: topic %d is too high (only have %d)", topic, vm->_gameFile->_dialogs.size());

	DialogTopic *dialogTopic = &vm->_gameFile->_dialogs[topic];

	option--;
	if (option >= dialogTopic->_options.size())
		error("SetDialogOption: option %d is too high (only have %d)", option, dialogTopic->_options.size());

	dialogTopic->_options[option]._flags &= ~DFLG_ON;
	if (!(dialogTopic->_options[option]._flags & DFLG_OFFPERM)) {
		if (state == kDialogOptionOn)
			dialogTopic->_options[option]._flags |= DFLG_ON;
		else if (state == kDialogOptionOffForever)
			dialogTopic->_options[option]._flags |= DFLG_OFFPERM;
	}

	return RuntimeValue();
}

// import DialogOptionState GetDialogOption(int topic, int option)
// Undocumented.
RuntimeValue Script_GetDialogOption(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint topic = params[0]._value;
	uint option = params[1]._value;

	// TODO: duplicates most of Dialog::GetOptionState

	if (topic >= vm->_gameFile->_dialogs.size())
		error("GetDialogOption: topic %d is too high (only have %d)", topic, vm->_gameFile->_dialogs.size());

	DialogTopic *dialogTopic = &vm->_gameFile->_dialogs[topic];

	option--;
	if (option >= dialogTopic->_options.size())
		error("GetDialogOption: option %d is too high (only have %d)", option, dialogTopic->_options.size());

	if (dialogTopic->_options[option]._flags & DFLG_OFFPERM)
		return kDialogOptionOffForever;
	else if (dialogTopic->_options[option]._flags & DFLG_ON)
		return kDialogOptionOn;
	else
		return kDialogOptionOff;
}

// import void RunDialog(int topic)
// Undocumented.
RuntimeValue Script_RunDialog(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint topic = params[0]._value;

	vm->runDialogId(topic);

	return RuntimeValue();
}

// import void StopDialog()
// From within dialog_request, tells AGS not to return to the dialog after this function ends.
RuntimeValue Script_StopDialog(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	if (vm->_state->_stopDialogAtEnd == DIALOG_NONE)
		warning("StopDialog was called while not in a dialog");
	else
		vm->_state->_stopDialogAtEnd = DIALOG_STOP;

	return RuntimeValue();
}

// Dialog: import int DisplayOptions(DialogOptionSayStyle = eSayUseOptionSetting)
// Displays the options for this dialog and returns which one the player selected.
RuntimeValue Script_Dialog_DisplayOptions(AGSEngine *vm, DialogTopic *self, const Common::Array<RuntimeValue> &params) {
	uint32 dialogoptionsaystyle = params[0]._value;
	UNUSED(dialogoptionsaystyle);

	// FIXME
	error("Dialog::DisplayOptions unimplemented");

	return RuntimeValue();
}

// Dialog: import DialogOptionState GetOptionState(int option)
// Gets the enabled state for the specified option in this dialog.
RuntimeValue Script_Dialog_GetOptionState(AGSEngine *vm, DialogTopic *self, const Common::Array<RuntimeValue> &params) {
	uint option = params[0]._value;

	option--;
	if (option >= self->_options.size())
		error("Dialog::GetOptionState: option %d is too high (only have %d)", option, self->_options.size());

	if (self->_options[option]._flags & DFLG_OFFPERM)
		return kDialogOptionOffForever;
	else if (self->_options[option]._flags & DFLG_ON)
		return kDialogOptionOn;
	else
		return kDialogOptionOff;
}

// Dialog: import String GetOptionText(int option)
// Gets the text of the specified option in this dialog.
RuntimeValue Script_Dialog_GetOptionText(AGSEngine *vm, DialogTopic *self, const Common::Array<RuntimeValue> &params) {
	uint option = params[0]._value;

	option--;
	if (option >= self->_options.size())
		error("Dialog::GetOptionText: option %d is too high (only have %d)", option, self->_options.size());

	RuntimeValue ret = new ScriptMutableString(self->_options[option]._name);
	ret._object->DecRef();
	return ret;
}

// Dialog: import bool HasOptionBeenChosen(int option)
// Checks whether the player has chosen this option before.
RuntimeValue Script_Dialog_HasOptionBeenChosen(AGSEngine *vm, DialogTopic *self, const Common::Array<RuntimeValue> &params) {
	uint option = params[0]._value;

	option--;
	if (option >= self->_options.size())
		error("Dialog::HasOptionBeenChosen: option %d is too high (only have %d)", option, self->_options.size());

	return (self->_options[option]._flags & DFLG_HASBEENCHOSEN) ? 1 : 0;
}

// Dialog: import void SetOptionState(int option, DialogOptionState)
// Sets the enabled state of the specified option in this dialog.
RuntimeValue Script_Dialog_SetOptionState(AGSEngine *vm, DialogTopic *self, const Common::Array<RuntimeValue> &params) {
	uint option = params[0]._value;
	uint32 state = params[1]._value;

	option--;
	if (option >= self->_options.size())
		error("Dialog::SetOptionState: option %d is too high (only have %d)", option, self->_options.size());

	self->_options[option]._flags &= ~DFLG_ON;
	if (!(self->_options[option]._flags & DFLG_OFFPERM)) {
		if (state == kDialogOptionOn)
			self->_options[option]._flags |= DFLG_ON;
		else if (state == kDialogOptionOffForever)
			self->_options[option]._flags |= DFLG_OFFPERM;
	}

	return RuntimeValue();
}

// Dialog: import void Start()
// Runs the dialog interactively.
RuntimeValue Script_Dialog_Start(AGSEngine *vm, DialogTopic *self, const Common::Array<RuntimeValue> &params) {
	vm->runDialogId(self->_id);

	return RuntimeValue();
}

// Dialog: readonly import attribute int ID
// Gets the dialog's ID number for interoperating with legacy code.
RuntimeValue Script_Dialog_get_ID(AGSEngine *vm, DialogTopic *self, const Common::Array<RuntimeValue> &params) {
	return self->_id;
}

// Dialog: readonly import attribute int OptionCount
// Gets the number of options that this dialog has.
RuntimeValue Script_Dialog_get_OptionCount(AGSEngine *vm, DialogTopic *self, const Common::Array<RuntimeValue> &params) {
	return self->_options.size();
}

// Dialog: readonly import attribute bool ShowTextParser
// Gets whether this dialog allows the player to type in text.
RuntimeValue Script_Dialog_get_ShowTextParser(AGSEngine *vm, DialogTopic *self, const Common::Array<RuntimeValue> &params) {
	return (self->_flags & DTFLG_SHOWPARSER) ? 1 : 0;
}

// DialogOptionsRenderingInfo: import attribute int ActiveOptionID
// The option that the mouse is currently positioned over
RuntimeValue Script_DialogOptionsRenderingInfo_get_ActiveOptionID(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_ActiveOptionID unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int ActiveOptionID
// The option that the mouse is currently positioned over
RuntimeValue Script_DialogOptionsRenderingInfo_set_ActiveOptionID(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("DialogOptionsRenderingInfo::set_ActiveOptionID unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: readonly import attribute Dialog* DialogToRender
// The dialog that is to have its options rendered
RuntimeValue Script_DialogOptionsRenderingInfo_get_DialogToRender(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_DialogToRender unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int Height
// The height of the dialog options
RuntimeValue Script_DialogOptionsRenderingInfo_get_Height(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_Height unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int Height
// The height of the dialog options
RuntimeValue Script_DialogOptionsRenderingInfo_set_Height(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("DialogOptionsRenderingInfo::set_Height unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int ParserTextBoxWidth
// The width of the text box for typing parser input, if enabled
RuntimeValue Script_DialogOptionsRenderingInfo_get_ParserTextBoxWidth(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_ParserTextBoxWidth unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int ParserTextBoxWidth
// The width of the text box for typing parser input, if enabled
RuntimeValue Script_DialogOptionsRenderingInfo_set_ParserTextBoxWidth(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("DialogOptionsRenderingInfo::set_ParserTextBoxWidth unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int ParserTextBoxX
// The X co-ordinate of the top-left corner of the text box for typing input
RuntimeValue Script_DialogOptionsRenderingInfo_get_ParserTextBoxX(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_ParserTextBoxX unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int ParserTextBoxX
// The X co-ordinate of the top-left corner of the text box for typing input
RuntimeValue Script_DialogOptionsRenderingInfo_set_ParserTextBoxX(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("DialogOptionsRenderingInfo::set_ParserTextBoxX unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int ParserTextBoxY
// The Y co-ordinate of the top-left corner of the text box for typing input
RuntimeValue Script_DialogOptionsRenderingInfo_get_ParserTextBoxY(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_ParserTextBoxY unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int ParserTextBoxY
// The Y co-ordinate of the top-left corner of the text box for typing input
RuntimeValue Script_DialogOptionsRenderingInfo_set_ParserTextBoxY(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("DialogOptionsRenderingInfo::set_ParserTextBoxY unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: readonly import attribute DrawingSurface* Surface
// The surface that the dialog options need to be rendered to
RuntimeValue Script_DialogOptionsRenderingInfo_get_Surface(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_Surface unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int Width
// The width of the dialog options
RuntimeValue Script_DialogOptionsRenderingInfo_get_Width(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_Width unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int Width
// The width of the dialog options
RuntimeValue Script_DialogOptionsRenderingInfo_set_Width(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("DialogOptionsRenderingInfo::set_Width unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int X
// The X co-ordinate of the top-left corner of the dialog options
RuntimeValue Script_DialogOptionsRenderingInfo_get_X(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_X unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int X
// The X co-ordinate of the top-left corner of the dialog options
RuntimeValue Script_DialogOptionsRenderingInfo_set_X(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("DialogOptionsRenderingInfo::set_X unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int Y
// The Y co-ordinate of the top-left corner of the dialog options
RuntimeValue Script_DialogOptionsRenderingInfo_get_Y(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("DialogOptionsRenderingInfo::get_Y unimplemented");

	return RuntimeValue();
}

// DialogOptionsRenderingInfo: import attribute int Y
// The Y co-ordinate of the top-left corner of the dialog options
RuntimeValue Script_DialogOptionsRenderingInfo_set_Y(AGSEngine *vm, DialogOptionsRenderingInfo *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("DialogOptionsRenderingInfo::set_Y unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "SetDialogOption", (ScriptAPIFunction *)&Script_SetDialogOption, "iii", sotNone },
	{ "GetDialogOption", (ScriptAPIFunction *)&Script_GetDialogOption, "ii", sotNone },
	{ "RunDialog", (ScriptAPIFunction *)&Script_RunDialog, "i", sotNone },
	{ "StopDialog", (ScriptAPIFunction *)&Script_StopDialog, "", sotNone },
	{ "Dialog::DisplayOptions^1", (ScriptAPIFunction *)&Script_Dialog_DisplayOptions, "i", sotDialog },
	{ "Dialog::GetOptionState^1", (ScriptAPIFunction *)&Script_Dialog_GetOptionState, "i", sotDialog },
	{ "Dialog::GetOptionText^1", (ScriptAPIFunction *)&Script_Dialog_GetOptionText, "i", sotDialog },
	{ "Dialog::HasOptionBeenChosen^1", (ScriptAPIFunction *)&Script_Dialog_HasOptionBeenChosen, "i", sotDialog },
	{ "Dialog::SetOptionState^2", (ScriptAPIFunction *)&Script_Dialog_SetOptionState, "ii", sotDialog },
	{ "Dialog::Start^0", (ScriptAPIFunction *)&Script_Dialog_Start, "", sotDialog },
	{ "Dialog::get_ID", (ScriptAPIFunction *)&Script_Dialog_get_ID, "", sotDialog },
	{ "Dialog::get_OptionCount", (ScriptAPIFunction *)&Script_Dialog_get_OptionCount, "", sotDialog },
	{ "Dialog::get_ShowTextParser", (ScriptAPIFunction *)&Script_Dialog_get_ShowTextParser, "", sotDialog },
	{ "DialogOptionsRenderingInfo::get_ActiveOptionID", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_ActiveOptionID, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::set_ActiveOptionID", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_set_ActiveOptionID, "i", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::get_DialogToRender", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_DialogToRender, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::get_Height", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_Height, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::set_Height", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_set_Height, "i", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::get_ParserTextBoxWidth", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_ParserTextBoxWidth, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::set_ParserTextBoxWidth", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_set_ParserTextBoxWidth, "i", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::get_ParserTextBoxX", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_ParserTextBoxX, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::set_ParserTextBoxX", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_set_ParserTextBoxX, "i", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::get_ParserTextBoxY", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_ParserTextBoxY, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::set_ParserTextBoxY", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_set_ParserTextBoxY, "i", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::get_Surface", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_Surface, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::get_Width", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_Width, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::set_Width", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_set_Width, "i", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::get_X", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_X, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::set_X", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_set_X, "i", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::get_Y", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_get_Y, "", sotDialogOptionsRenderingInfo },
	{ "DialogOptionsRenderingInfo::set_Y", (ScriptAPIFunction *)&Script_DialogOptionsRenderingInfo_set_Y, "i", sotDialogOptionsRenderingInfo },
};

void addDialogSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
