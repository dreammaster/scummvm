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

namespace AGS {

class ScriptInventoryArray : public ScriptArray {
public:
	ScriptObject *getObjectAt(uint32 &offset) {
		error("ScriptInventoryArray unimplemented"); // FIXME
	}
};

void addInventorySystemScripting(GlobalScriptState *state) {
	// InventoryItem functions
	state->addSystemFunctionImport("InventoryItem::GetAtScreenXY^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::IsInteractionAvailable^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::GetName^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::GetProperty^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::GetPropertyText^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::GetTextProperty^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::RunInteraction^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::SetName^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::get_CursorGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::set_CursorGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::get_Graphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::set_Graphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::get_ID", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::get_Name", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InventoryItem::set_Name", &Script_UnimplementedStub);

	// inventory functions
	state->addSystemFunctionImport("GetInvAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetInvGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetInvName", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetInvProperty", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetInvPropertyText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RunInventoryInteraction", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetInvItemName", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetInvItemPic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("UpdateInventory", &Script_UnimplementedStub);

	state->addSystemObjectImport("inventory", new ScriptInventoryArray);
}

} // End of namespace AGS
