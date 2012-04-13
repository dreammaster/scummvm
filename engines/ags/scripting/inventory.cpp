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
#include "engines/ags/character.h"
#include "engines/ags/constants.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/graphics.h"
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
	uint invItem = params[0]._value;
	ScriptString *property = (ScriptString *)params[1]._object;

	if (invItem >= vm->_gameFile->_invItemInfo.size())
		error("GetInvProperty: item %d is too high (only have %d)", invItem, vm->_gameFile->_invItemInfo.size());

	return vm->getIntProperty(property->getString(), vm->_gameFile->_invItemInfo[invItem]._properties);
}

// import void GetInvPropertyText(int invItem, const string property, string buffer)
// Obsolete inventory function.
RuntimeValue Script_GetInvPropertyText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint invItem = params[0]._value;
	ScriptString *property = (ScriptString *)params[1]._object;
	ScriptString *buffer = (ScriptString *)params[2]._object;

	if (invItem >= vm->_gameFile->_invItemInfo.size())
		error("GetInvPropertyText: item %d is too high (only have %d)", invItem, vm->_gameFile->_invItemInfo.size());

	buffer->setString(vm->getStringProperty(property->getString(), vm->_gameFile->_invItemInfo[invItem]._properties));

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
	uint item = params[0]._value;
	UNUSED(item);

	if (item >= vm->_gameFile->_invItemInfo.size())
		error("GetInvGraphic: item %d is too high (only have %d)", item, vm->_gameFile->_invItemInfo.size());

	return vm->_gameFile->_invItemInfo[item]._pic;
}

// import void SetInvItemPic(int item, int spriteSlot)
// Obsolete inventory function.
RuntimeValue Script_SetInvItemPic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint item = params[0]._value;
	uint spriteSlot = params[1]._value;

	if (item >= vm->_gameFile->_invItemInfo.size())
		error("SetInvItemPic: item %d is too high (only have %d)", item, vm->_gameFile->_invItemInfo.size());

	// TODO: sanity-check spriteSlot

	InventoryItem &invItem = vm->_gameFile->_invItemInfo[item];
	if (invItem._pic == spriteSlot)
		return RuntimeValue();

	if (invItem._pic == invItem._cursorPic) {
		// Backwards compatibility -- there didn't used to be a cursorPic,
		// so if they're the same update both.

		invItem._cursorPic = spriteSlot;

		if (vm->_graphics->getCurrentCursor() == MODE_USE && vm->getPlayerChar()->_activeInv == item) {
			vm->updateInvCursor(item);
			vm->_graphics->setMouseCursor(vm->_graphics->getCurrentCursor());
		}
	}

	invItem._pic = spriteSlot;
	vm->invalidateGUI();

	return RuntimeValue();
}

// import void SetInvItemName(int item, const string name)
// Obsolete inventory function.
RuntimeValue Script_SetInvItemName(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint item = params[0]._value;
	ScriptString *name = (ScriptString *)params[1]._object;

	if (item >= vm->_gameFile->_invItemInfo.size())
		error("SetInvItemName: item %d is too high (only have %d)", item, vm->_gameFile->_invItemInfo.size());

	vm->_gameFile->_invItemInfo[item]._name = name->getString();

	// might need to redraw the GUI if it has the inv item name on it
	vm->invalidateGUI();

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
	vm->updateInvOrder();

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

	return vm->getIntProperty(property->getString(), self->_properties);
}

// InventoryItem: import String GetTextProperty(const string property)
// Gets a text custom property for this item.
RuntimeValue Script_InventoryItem_GetTextProperty(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *property = (ScriptString *)params[0]._object;

	Common::String string = vm->getStringProperty(property->getString(), self->_properties);
	RuntimeValue ret = new ScriptMutableString(string);
	ret._object->DecRef();
	return ret;
}

// InventoryItem: import int IsInteractionAvailable(CursorMode)
// Checks whether an event handler has been registered for clicking on this item in the specified cursor mode.
RuntimeValue Script_InventoryItem_IsInteractionAvailable(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;

	return vm->runInventoryInteraction(self->_id, cursormode, true) ? 1 : 0;
}

// InventoryItem: import void RunInteraction(CursorMode)
// Runs the registered event handler for this item.
RuntimeValue Script_InventoryItem_RunInteraction(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;

	vm->runInventoryInteraction(self->_id, cursormode);

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
	uint value = params[0]._signedValue;

	self->_cursorPic = value;

	if (vm->_graphics->getCurrentCursor() == MODE_USE && vm->getPlayerChar()->_activeInv == self->_id) {
		vm->updateInvCursor(self->_id);
		vm->_graphics->setMouseCursor(vm->_graphics->getCurrentCursor());
	}

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
	uint value = params[0]._value;

	// TODO: mostly the same as SetInvItemPic

	// TODO: sanity-check value

	if (self->_pic == value)
		return RuntimeValue();

	if (self->_pic == self->_cursorPic) {
		// Backwards compatibility -- there didn't used to be a cursorPic,
		// so if they're the same update both.

		self->_cursorPic = value;

		if (vm->_graphics->getCurrentCursor() == MODE_USE && vm->getPlayerChar()->_activeInv == self->_id) {
			vm->updateInvCursor(self->_id);
			vm->_graphics->setMouseCursor(vm->_graphics->getCurrentCursor());
		}
	}

	self->_pic = value;
	vm->invalidateGUI();

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
	RuntimeValue ret = new ScriptMutableString(vm->getTranslation(self->_name));
	ret._object->DecRef();
	return ret;
}

// InventoryItem: import attribute String Name
// Gets/sets the name of the inventory item.
RuntimeValue Script_InventoryItem_set_Name(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *value = (ScriptString *)params[0]._object;

	self->_name = value->getString();

	// might need to redraw the GUI if it has the inv item name on it
	vm->invalidateGUI();

	return RuntimeValue();
}

// InventoryItem: import void GetName(string buffer)
// Undocumented.
RuntimeValue Script_InventoryItem_GetName(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;

	buffer->setString(self->_name);

	return RuntimeValue();
}

// InventoryItem: import void GetPropertyText(const string property, string buffer)
// Undocumented.
RuntimeValue Script_InventoryItem_GetPropertyText(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *property = (ScriptString *)params[0]._object;
	ScriptString *buffer = (ScriptString *)params[1]._object;

	buffer->setString(vm->getStringProperty(property->getString(), self->_properties));

	return RuntimeValue();
}

// InventoryItem: import void SetName(const string newName)
// Undocumented.
RuntimeValue Script_InventoryItem_SetName(AGSEngine *vm, InventoryItem *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *newName = (ScriptString *)params[0]._object;

	self->_name = newName->getString();

	// might need to redraw the GUI if it has the inv item name on it
	vm->invalidateGUI();

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
