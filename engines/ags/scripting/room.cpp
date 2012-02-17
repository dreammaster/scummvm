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

void addRoomSystemScripting(GlobalScriptState *state) {
	// static room functions
	state->addSystemFunctionImport("CallRoomScript", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisableGroundLevelAreas", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisableRegion", &Script_UnimplementedStub);
	state->addSystemFunctionImport("EnableGroundLevelAreas", &Script_UnimplementedStub);
	state->addSystemFunctionImport("EnableRegion", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetBackgroundFrame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetHotspotAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetHotspotName", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetHotspotPointX", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetHotspotPointY", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetHotspotProperty", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetHotspotPropertyText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetObjectAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetObjectBaseline", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetObjectGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetObjectName", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetObjectProperty", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetObjectPropertyText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetObjectX", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetObjectY", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetRegionAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetRoomProperty", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetRoomPropertyText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetScalingAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetWalkableAreaAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("HasPlayerBeenInRoom", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsObjectAnimating", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsObjectMoving", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsObjectOn", &Script_UnimplementedStub);
	state->addSystemFunctionImport("MergeObject", &Script_UnimplementedStub);
	state->addSystemFunctionImport("MoveObject", &Script_UnimplementedStub);
	state->addSystemFunctionImport("MoveObjectDirect", &Script_UnimplementedStub);
	state->addSystemFunctionImport("MoveToWalkableArea", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RemoveObjectTint", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RemoveWalkableArea", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RunHotspotInteraction", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RunObjectInteraction", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RunRegionInteraction", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetAreaLightLevel", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetAreaScaling", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetBackgroundFrame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetObjectBaseline", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetObjectClickable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetObjectFrame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetObjectGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetObjectIgnoreWalkbehinds", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetObjectPosition", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetObjectTint", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetObjectTransparency", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetObjectView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetRegionTint", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetWalkBehindBase", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StopObjectMoving", &Script_UnimplementedStub);
}

} // End of namespace AGS
