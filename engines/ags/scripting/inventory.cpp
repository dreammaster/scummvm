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
#include "engines/ags/gamefile.h"
#include "engines/ags/inventory.h"

namespace AGS {

// import int GetInvAt(int x,int y)
// Undocumented.
RuntimeValue Script_GetInvAt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;

	return vm->getInventoryItemAt(Common::Point(x, y));
}

// import int GetInvProperty(int invItem, const string property)
// Obsolete inventory function.
RuntimeValue Script_GetInvProperty(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int invItem = params[0]._signedValue;
	UNUSED(invItem);
	ScriptString *property = (ScriptString *)params[1]._object;
	UNUSED(property);

	// FIXME
	error("GetInvProperty unimplemented");

	return RuntimeValue();
}

// import void GetInvPropertyText(int invItem, const string property, string buffer)
// Obsolete inventory function.
RuntimeValue Script_GetInvPropertyText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int invItem = params[0]._signedValue;
	UNUSED(invItem);
	ScriptString *property = (ScriptString *)params[1]._object;
	UNUSED(property);
	ScriptString *buffer = (ScriptString *)params[2]._object;
	UNUSED(buffer);

	// FIXME
	error("GetInvPropertyText unimplemented");

	return RuntimeValue();
}

// import void GetInvName(int item, string buffer)
// Obsolete inventory function.
RuntimeValue Script_GetInvName(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint item = params[0]._value;
	ScriptString *buffer = (ScriptString *)params[1]._object;

	if (item >= vm->_gameFile->_invItemInfo.size())
		error("GetInvName: item %d is too high (only have %d)", item, vm->_gameFile->_invItemInfo.size());

	buffer->setString(vm->getTranslation(vm->_gameFile->_invItemInfo[item]._name));

	return RuntimeValue();
}

// import int GetInvGraphic(int item)
// Obsolete inventory function.
RuntimeValue Script_GetInvGraphic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int item = params[0]._signedValue;
	UNUSED(item);

	// FIXME
	error("GetInvGraphic unimplemented");

	return RuntimeValue();
}

// import void SetInvItemPic(int item, int spriteSlot)
// Obsolete inventory function.
RuntimeValue Script_SetInvItemPic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int item = params[0]._signedValue;
	UNUSED(item);
	int spriteSlot = params[1]._signedValue;
	UNUSED(spriteSlot);

	// FIXME
	error("SetInvItemPic unimplemented");

	return RuntimeValue();
}

// import void SetInvItemName(int item, const string name)
// Obsolete inventory function.
RuntimeValue Script_SetInvItemName(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int item = params[0]._signedValue;
	UNUSED(item);
	ScriptString *name = (ScriptString *)params[1]._object;
	UNUSED(name);

	// FIXME
	error("SetInvItemName unimplemented");

	return RuntimeValue();
}

// import int IsInventoryInteractionAvailable (int item, CursorMode)
// Obsolete inventory function.
RuntimeValue Script_IsInventoryInteractionAvailable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint item = params[0]._value;
	uint32 cursormode = params[1]._value;

	if (item >= vm->_gameFile->_invItemInfo.size())
		error("IsInventoryInteractionAvailable: item %d is too high (only have %d)", item, vm->_gameFile->_invItemInfo.size());

	return vm->runInventoryInteraction(item, cursormode, true) ? 1 : 0;
}

// import void RunInventoryInteraction (int item, CursorMode)
// Obsolete inventory function.
RuntimeValue Script_RunInventoryInteraction(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint item = params[0]._value;
	uint32 cursormode = params[1]._value;

	if (item >= vm->_gameFile->_invItemInfo.size())
		error("RunInventoryInteraction: item %d is too high (only have %d)", item, vm->_gameFile->_invItemInfo.size());

	vm->runInventoryInteraction(item, cursormode);

	return RuntimeValue();
}

// import void UpdateInventory()
// Refreshes the on-screen inventory display.
RuntimeValue Script_UpdateInventory(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("UpdateInventory unimplemented");

	return RuntimeValue();
}

// InventoryItem: import static InventoryItem* GetAtScreenXY(int x, int y)
// Returns the inventory item at the specified location.
RuntimeValue Script_InventoryItem_GetAtScreenXY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;

	uint invId = vm->getInventoryItemAt(Common::Point(x, y));
	if (invId == (uint)-1 || invId == 0)
		return 0;

	return &vm->_gameFile->_invItemInfo[invId];
}

// InventoryItem: import int GetProperty(const string property)
// Gets an integer custom property for this item.
RuntimeValue Script_InventoryItem_GetProperty(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *property = (ScriptString *)params[0]._object;
	UNUSED(property);

	// FIXME
	error("InventoryItem::GetProperty unimplemented");

	return RuntimeValue();
}

// InventoryItem: import String GetTextProperty(const string property)
// Gets a text custom property for this item.
RuntimeValue Script_InventoryItem_GetTextProperty(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *property = (ScriptString *)params[0]._object;
	UNUSED(property);

	// FIXME
	error("InventoryItem::GetTextProperty unimplemented");

	return RuntimeValue();
}

// InventoryItem: import int IsInteractionAvailable(CursorMode)
// Checks whether an event handler has been registered for clicking on this item in the specified cursor mode.
RuntimeValue Script_InventoryItem_IsInteractionAvailable(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;
	UNUSED(cursormode);

	// FIXME
	error("InventoryItem::IsInteractionAvailable unimplemented");

	return RuntimeValue();
}

// InventoryItem: import void RunInteraction(CursorMode)
// Runs the registered event handler for this item.
RuntimeValue Script_InventoryItem_RunInteraction(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;
	UNUSED(cursormode);

	// FIXME
	error("InventoryItem::RunInteraction unimplemented");

	return RuntimeValue();
}

// InventoryItem: import attribute int CursorGraphic
// Gets/sets the sprite used as the item's mouse cursor.
RuntimeValue Script_InventoryItem_get_CursorGraphic(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	return self->_cursorPic;
}

// InventoryItem: import attribute int CursorGraphic
// Gets/sets the sprite used as the item's mouse cursor.
RuntimeValue Script_InventoryItem_set_CursorGraphic(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("InventoryItem::set_CursorGraphic unimplemented");

	return RuntimeValue();
}

// InventoryItem: import attribute int Graphic
// Gets/sets the sprite used to display the inventory item.
RuntimeValue Script_InventoryItem_get_Graphic(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	return self->_pic;
}

// InventoryItem: import attribute int Graphic
// Gets/sets the sprite used to display the inventory item.
RuntimeValue Script_InventoryItem_set_Graphic(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("InventoryItem::set_Graphic unimplemented");

	return RuntimeValue();
}

// InventoryItem: readonly import attribute int ID
// Gets the ID number of the inventory item.
RuntimeValue Script_InventoryItem_get_ID(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	return self->_id;
}

// InventoryItem: import attribute String Name
// Gets/sets the name of the inventory item.
RuntimeValue Script_InventoryItem_get_Name(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("InventoryItem::get_Name unimplemented");

	return RuntimeValue();
}

// InventoryItem: import attribute String Name
// Gets/sets the name of the inventory item.
RuntimeValue Script_InventoryItem_set_Name(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *value = (ScriptString *)params[0]._object;
	UNUSED(value);

	// FIXME
	error("InventoryItem::set_Name unimplemented");

	return RuntimeValue();
}

// InventoryItem: import void GetName(string buffer)
// Undocumented.
RuntimeValue Script_InventoryItem_GetName(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;
	UNUSED(buffer);

	// FIXME
	error("InventoryItem::GetName unimplemented");

	return RuntimeValue();
}

// InventoryItem: import void GetPropertyText(const string property, string buffer)
// Undocumented.
RuntimeValue Script_InventoryItem_GetPropertyText(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *property = (ScriptString *)params[0]._object;
	UNUSED(property);
	ScriptString *buffer = (ScriptString *)params[1]._object;
	UNUSED(buffer);

	// FIXME
	error("InventoryItem::GetPropertyText unimplemented");

	return RuntimeValue();
}

// InventoryItem: import void SetName(const string newName)
// Undocumented.
RuntimeValue Script_InventoryItem_SetName(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *newName = (ScriptString *)params[0]._object;
	UNUSED(newName);

	// FIXME
	error("InventoryItem::SetName unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "GetInvAt", (ScriptAPIFunction *)&Script_GetInvAt, "ii", sotNone },
	{ "GetInvProperty", (ScriptAPIFunction *)&Script_GetInvProperty, "is", sotNone },
	{ "GetInvPropertyText", (ScriptAPIFunction *)&Script_GetInvPropertyText, "iss", sotNone },
	{ "GetInvName", (ScriptAPIFunction *)&Script_GetInvName, "is", sotNone },
	{ "GetInvGraphic", (ScriptAPIFunction *)&Script_GetInvGraphic, "i", sotNone },
	{ "SetInvItemPic", (ScriptAPIFunction *)&Script_SetInvItemPic, "ii", sotNone },
	{ "SetInvItemName", (ScriptAPIFunction *)&Script_SetInvItemName, "is", sotNone },
	{ "IsInventoryInteractionAvailable", (ScriptAPIFunction *)&Script_IsInventoryInteractionAvailable, "ii", sotNone },
	{ "RunInventoryInteraction", (ScriptAPIFunction *)&Script_RunInventoryInteraction, "ii", sotNone },
	{ "UpdateInventory", (ScriptAPIFunction *)&Script_UpdateInventory, "", sotNone },
	{ "InventoryItem::GetAtScreenXY^2", (ScriptAPIFunction *)&Script_InventoryItem_GetAtScreenXY, "ii", sotNone },
	{ "InventoryItem::GetProperty^1", (ScriptAPIFunction *)&Script_InventoryItem_GetProperty, "s", sotInventoryItem },
	{ "InventoryItem::GetTextProperty^1", (ScriptAPIFunction *)&Script_InventoryItem_GetTextProperty, "s", sotInventoryItem },
	{ "InventoryItem::IsInteractionAvailable^1", (ScriptAPIFunction *)&Script_InventoryItem_IsInteractionAvailable, "i", sotInventoryItem },
	{ "InventoryItem::RunInteraction^1", (ScriptAPIFunction *)&Script_InventoryItem_RunInteraction, "i", sotInventoryItem },
	{ "InventoryItem::get_CursorGraphic", (ScriptAPIFunction *)&Script_InventoryItem_get_CursorGraphic, "", sotInventoryItem },
	{ "InventoryItem::set_CursorGraphic", (ScriptAPIFunction *)&Script_InventoryItem_set_CursorGraphic, "i", sotInventoryItem },
	{ "InventoryItem::get_Graphic", (ScriptAPIFunction *)&Script_InventoryItem_get_Graphic, "", sotInventoryItem },
	{ "InventoryItem::set_Graphic", (ScriptAPIFunction *)&Script_InventoryItem_set_Graphic, "i", sotInventoryItem },
	{ "InventoryItem::get_ID", (ScriptAPIFunction *)&Script_InventoryItem_get_ID, "", sotInventoryItem },
	{ "InventoryItem::get_Name", (ScriptAPIFunction *)&Script_InventoryItem_get_Name, "", sotInventoryItem },
	{ "InventoryItem::set_Name", (ScriptAPIFunction *)&Script_InventoryItem_set_Name, "s", sotInventoryItem },
	{ "InventoryItem::GetName^1", (ScriptAPIFunction *)&Script_InventoryItem_GetName, "s", sotInventoryItem },
	{ "InventoryItem::GetPropertyText^2", (ScriptAPIFunction *)&Script_InventoryItem_GetPropertyText, "ss", sotInventoryItem },
	{ "InventoryItem::SetName^1", (ScriptAPIFunction *)&Script_InventoryItem_SetName, "s", sotInventoryItem },
};

void addInventorySystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
