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

RuntimeValue Script_AddInventory(AGSEngine *vm, const Common::Array<RuntimeValue> &params) {
	// FIXME
	warning("AddInventory unimplemented");

	return RuntimeValue();
}

void addCharacterSystemScripting(GlobalScriptState *state) {
	// Character functions
	state->addSystemFunctionImport("Character::AddInventory^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::AddWaypoint^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::Animate^5", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::ChangeRoom^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::ChangeRoomAutoPosition^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::ChangeView^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::FaceCharacter^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::FaceLocation^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::FaceObject^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::FollowCharacter^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::GetProperty^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::GetPropertyText^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::GetTextProperty^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::HasInventory^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::IsCollidingWithChar^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::IsCollidingWithObject^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::LockView^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::LockViewAligned^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::LockViewFrame^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::LockViewOffset^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::LoseInventory^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::Move^4", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::PlaceOnWalkableArea^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::RemoveTint^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::RunInteraction^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::Say^101", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::SayAt^4", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::SayBackground^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::SetAsPlayer^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::SetIdleView^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::SetWalkSpeed^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::StopMoving^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::Think^101", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::Tint^5", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::UnlockView^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::Walk^4", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::WalkStraight^3", &Script_UnimplementedStub);
	// Character static functions
	state->addSystemFunctionImport("Character::GetAtScreenXY^2", &Script_UnimplementedStub);
	// Character getter/setter functions
	state->addSystemFunctionImport("Character::get_ActiveInventory", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_ActiveInventory", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Animating", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_AnimationSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_AnimationSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Baseline", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Baseline", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_BlinkInterval", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_BlinkInterval", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_BlinkView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_BlinkView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_BlinkWhileThinking", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_BlinkWhileThinking", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_BlockingHeight", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_BlockingHeight", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_BlockingWidth", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_BlockingWidth", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Clickable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Clickable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_DiagonalLoops", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_DiagonalLoops", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Frame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Frame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_HasExplicitTint", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_ID", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_IdleView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::geti_InventoryQuantity", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::seti_InventoryQuantity", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_IgnoreLighting", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_IgnoreLighting", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_IgnoreScaling", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_IgnoreScaling", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_IgnoreWalkbehinds", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_IgnoreWalkbehinds", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Loop", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Loop", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_ManualScaling", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_ManualScaling", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_MovementLinkedToAnimation", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_MovementLinkedToAnimation", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Moving", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Name", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Name", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_NormalView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_PreviousRoom", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Room", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_ScaleMoveSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_ScaleMoveSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_ScaleVolume", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_ScaleVolume", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Scaling", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Scaling", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Solid", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Solid", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Speaking", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_SpeakingFrame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_SpeechAnimationDelay", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_SpeechAnimationDelay", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_SpeechColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_SpeechColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_SpeechView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_SpeechView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_ThinkView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_ThinkView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Transparency", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Transparency", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_TurnBeforeWalking", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_TurnBeforeWalking", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_View", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_WalkSpeedX", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_WalkSpeedY", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_X", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_X", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Y", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Y", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_Z", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_Z", &Script_UnimplementedStub);
	// .. and lower-case forms
	state->addSystemFunctionImport("Character::get_x", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_x", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_y", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_y", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::get_z", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Character::set_z", &Script_UnimplementedStub);

	// old-style compatibility stubs
	state->addSystemFunctionImport("AddInventory", &Script_AddInventory);
	state->addSystemFunctionImport("AddInventoryToCharacter", &Script_UnimplementedStub);
	state->addSystemFunctionImport("AnimateCharacter", &Script_UnimplementedStub);
	state->addSystemFunctionImport("AnimateCharacterEx", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ChangeCharacterView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetCharacterBlinkView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetCharacterFrame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetCharacterSpeechView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetCharacterSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetCharacterSpeedEx", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetCharacterView", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetCharacterViewEx", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetCharacterViewOffset", &Script_UnimplementedStub);

	// character-related functions
	state->addSystemFunctionImport("GetPlayerCharacter", &Script_UnimplementedStub);
	state->addSystemFunctionImport("NewRoom", &Script_UnimplementedStub);
	state->addSystemFunctionImport("NewRoomEx", &Script_UnimplementedStub);
	state->addSystemFunctionImport("NewRoomNPC", &Script_UnimplementedStub);
}

} // End of namespace AGS
