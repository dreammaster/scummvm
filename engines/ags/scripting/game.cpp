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

class ScriptGameObject : public ScriptObject {
public:
};

class ScriptSaveGameIndexesObject : public ScriptObject {
public:
};

void addGameSystemScripting(GlobalScriptState *state) {
	// Game functions
	state->addSystemFunctionImport("Game::ChangeTranslation^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::DoOnceOnly^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::GetFrameCountForLoop^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::GetLocationName^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::GetLoopCountForView^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::GetRunNextSettingForLoop^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::GetSaveSlotDescription^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::GetViewFrame^3", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::InputBox^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::SetSaveGameDirectory^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_CharacterCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_DialogCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_FileName", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_FontCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::geti_GlobalMessages", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::geti_GlobalStrings", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::seti_GlobalStrings", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_GUICount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_IgnoreUserInputAfterTextTimeoutMs", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::set_IgnoreUserInputAfterTextTimeoutMs", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_InSkippableCutscene", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_InventoryItemCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_MinimumTextDisplayTimeMs", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::set_MinimumTextDisplayTimeMs", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_MouseCursorCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_Name", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::set_Name", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_NormalFont", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::set_NormalFont", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_SkippingCutscene", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_SpeechFont", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::set_SpeechFont", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::geti_SpriteWidth", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::geti_SpriteHeight", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_TextReadingSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::set_TextReadingSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_TranslationFilename", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_UseNativeCoordinates", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::get_ViewCount", &Script_UnimplementedStub);

	// Dialog functions
	state->addSystemFunctionImport("Dialog::get_ID", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Dialog::get_OptionCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Dialog::get_ShowTextParser", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Dialog::DisplayOptions^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Dialog::GetOptionState^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Dialog::GetOptionText^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Dialog::HasOptionBeenChosen^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Dialog::SetOptionState^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Dialog::Start^0", &Script_UnimplementedStub);

	// dialog static functions
	state->addSystemFunctionImport("GetDialogOption", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RunDialog", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetDialogOption", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StopDialog", &Script_UnimplementedStub);

	// translation functions
	state->addSystemFunctionImport("GetTranslation", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetTranslationName", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsTranslationAvailable", &Script_UnimplementedStub);

	// text parser functions
	state->addSystemFunctionImport("Parser::FindWordID^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Parser::ParseText^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Parser::SaidUnknownWord^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Parser::Said^1", &Script_UnimplementedStub);

	// text parser static functions
	state->addSystemFunctionImport("ParseText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Said", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SaidUnknownWord", &Script_UnimplementedStub);

	// misc game-related functions
	state->addSystemFunctionImport("AbortGame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Debug", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DeleteSaveSlot", &Script_UnimplementedStub);
	state->addSystemFunctionImport("EndCutscene", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGameOption", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGameParameter", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGameSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGlobalInt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGlobalString", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGraphicalVariable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GiveScore", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsGamePaused", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PauseGame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("QuitGame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RestartGame", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RestoreGameDialog", &Script_UnimplementedStub);
	state->addSystemFunctionImport("RestoreGameSlot", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SaveGameDialog", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SaveGameSlot", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGameOption", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGameSpeed", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGlobalInt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGlobalString", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGraphicalVariable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetInvDimensions", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetRestartPoint", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StartCutscene", &Script_UnimplementedStub);
	state->addSystemFunctionImport("UnPauseGame", &Script_UnimplementedStub);

	state->addSystemObjectImport("game", new ScriptGameObject);
	state->addSystemObjectImport("savegameindex", new ScriptSaveGameIndexesObject);
}

} // End of namespace AGS
