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

#include "common/debug.h"
#include "engines/ags/scripting/scripting.h"

namespace AGS {

// Game: import static bool ChangeTranslation(const string newTranslationFileName)
// Changes the active translation.
RuntimeValue Script_Game_ChangeTranslation(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *newTranslationFileName = (ScriptString *)params[0]._object;
	UNUSED(newTranslationFileName);

	// FIXME
	error("Game::ChangeTranslation unimplemented");

	return RuntimeValue();
}

// Game: import static bool DoOnceOnly(const string token)
// Returns true the first time this command is called with this token.
RuntimeValue Script_Game_DoOnceOnly(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *token = (ScriptString *)params[0]._object;
	UNUSED(token);

	// FIXME
	error("Game::DoOnceOnly unimplemented");

	return RuntimeValue();
}

// Game: import static int GetColorFromRGB(int red, int green, int blue)
// Gets the AGS Colour Number for the specified RGB colour.
RuntimeValue Script_Game_GetColorFromRGB(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int red = params[0]._signedValue;
	UNUSED(red);
	int green = params[1]._signedValue;
	UNUSED(green);
	int blue = params[2]._signedValue;
	UNUSED(blue);

	// FIXME
	error("Game::GetColorFromRGB unimplemented");

	return RuntimeValue();
}

// Game: import static int GetFrameCountForLoop(int view, int loop)
// Gets the number of frames in the specified view loop.
RuntimeValue Script_Game_GetFrameCountForLoop(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int view = params[0]._signedValue;
	UNUSED(view);
	int loop = params[1]._signedValue;
	UNUSED(loop);

	// FIXME
	error("Game::GetFrameCountForLoop unimplemented");

	return RuntimeValue();
}

// Game: import static String GetLocationName(int x, int y)
// Gets the name of whatever is on the screen at (x,y)
RuntimeValue Script_Game_GetLocationName(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("Game::GetLocationName unimplemented");

	return RuntimeValue();
}

// Game: import static int GetLoopCountForView(int view)
// Gets the number of loops in the specified view.
RuntimeValue Script_Game_GetLoopCountForView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int view = params[0]._signedValue;
	UNUSED(view);

	// FIXME
	error("Game::GetLoopCountForView unimplemented");

	return RuntimeValue();
}

// Game: import static int GetMODPattern()
// Returns the current pattern/track number if the current music is MOD or XM.
RuntimeValue Script_Game_GetMODPattern(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::GetMODPattern unimplemented");

	return RuntimeValue();
}

// Game: import static bool GetRunNextSettingForLoop(int view, int loop)
// Gets whether the "Run next loop after this" setting is checked for the specified loop.
RuntimeValue Script_Game_GetRunNextSettingForLoop(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int view = params[0]._signedValue;
	UNUSED(view);
	int loop = params[1]._signedValue;
	UNUSED(loop);

	// FIXME
	error("Game::GetRunNextSettingForLoop unimplemented");

	return RuntimeValue();
}

// Game: import static String GetSaveSlotDescription(int saveSlot)
// Gets the description of the specified save game slot.
RuntimeValue Script_Game_GetSaveSlotDescription(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int saveSlot = params[0]._signedValue;
	UNUSED(saveSlot);

	// FIXME
	error("Game::GetSaveSlotDescription unimplemented");

	return RuntimeValue();
}

// Game: import static ViewFrame* GetViewFrame(int view, int loop, int frame)
// Gets the ViewFrame instance for the specified view frame.
RuntimeValue Script_Game_GetViewFrame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int view = params[0]._signedValue;
	UNUSED(view);
	int loop = params[1]._signedValue;
	UNUSED(loop);
	int frame = params[2]._signedValue;
	UNUSED(frame);

	// FIXME
	error("Game::GetViewFrame unimplemented");

	return RuntimeValue();
}

// Game: import static String InputBox(const string prompt)
// Prompts the user to type in a string, and returns the text that they type in.
RuntimeValue Script_Game_InputBox(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *prompt = (ScriptString *)params[0]._object;
	UNUSED(prompt);

	// FIXME
	error("Game::InputBox unimplemented");

	return RuntimeValue();
}

// Game: import static bool IsAudioPlaying(AudioType audioType=SCR_NO_VALUE)
// Gets whether any audio (of this type) is currently playing.
RuntimeValue Script_Game_IsAudioPlaying(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 audioType = params[0]._value;
	UNUSED(audioType);

	// FIXME
	error("Game::IsAudioPlaying unimplemented");

	return RuntimeValue();
}

// Game: import static void SetAudioTypeSpeechVolumeDrop(AudioType, int volumeDrop)
// Changes the volume drop applied to this audio type when speech is played
RuntimeValue Script_Game_SetAudioTypeSpeechVolumeDrop(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 audiotype = params[0]._value;
	UNUSED(audiotype);
	int volumeDrop = params[1]._signedValue;
	UNUSED(volumeDrop);

	// FIXME
	error("Game::SetAudioTypeSpeechVolumeDrop unimplemented");

	return RuntimeValue();
}

// Game: import static void SetAudioTypeVolume(AudioType, int volume, ChangeVolumeType)
// Changes the default volume of audio clips of the specified type.
RuntimeValue Script_Game_SetAudioTypeVolume(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 audiotype = params[0]._value;
	UNUSED(audiotype);
	int volume = params[1]._signedValue;
	UNUSED(volume);
	uint32 changevolumetype = params[2]._value;
	UNUSED(changevolumetype);

	// FIXME
	error("Game::SetAudioTypeVolume unimplemented");

	return RuntimeValue();
}

// Game: import static bool SetSaveGameDirectory(const string directory)
// Sets the directory where AGS will save and load saved games.
RuntimeValue Script_Game_SetSaveGameDirectory(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *directory = (ScriptString *)params[0]._object;
	UNUSED(directory);

	// FIXME
	error("Game::SetSaveGameDirectory unimplemented");

	return RuntimeValue();
}

// Game: import static void StopAudio(AudioType audioType=SCR_NO_VALUE)
// Stops all currently playing audio (optionally of the specified type).
RuntimeValue Script_Game_StopAudio(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 audioType = params[0]._value;
	UNUSED(audioType);

	// FIXME
	error("Game::StopAudio unimplemented");

	return RuntimeValue();
}

// Game: import static void StopSound(bool includeAmbientSounds=false)
// Stops all currently playing sound effects.
RuntimeValue Script_Game_StopSound(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 includeAmbientSounds = params[0]._value;
	UNUSED(includeAmbientSounds);

	// FIXME
	error("Game::StopSound unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute int CharacterCount
// Gets the number of characters in the game.
RuntimeValue Script_Game_get_CharacterCount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_CharacterCount unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute int DialogCount
// Gets the number of dialogs in the game.
RuntimeValue Script_Game_get_DialogCount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_DialogCount unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute String FileName
// Gets the name of the game EXE file.
RuntimeValue Script_Game_get_FileName(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_FileName unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute int FontCount
// Gets the number of fonts in the game.
RuntimeValue Script_Game_get_FontCount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_FontCount unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute String GlobalMessages[]
// Accesses the legacy Global Messages, from AGS 2.x
RuntimeValue Script_Game_geti_GlobalMessages(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);

	// FIXME
	error("Game::geti_GlobalMessages unimplemented");

	return RuntimeValue();
}

// Game: import static attribute String GlobalStrings[]
// Accesses the global strings collection. This is obsolete.
RuntimeValue Script_Game_geti_GlobalStrings(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);

	// FIXME
	error("Game::geti_GlobalStrings unimplemented");

	return RuntimeValue();
}

// Game: import static attribute String GlobalStrings[]
// Accesses the global strings collection. This is obsolete.
RuntimeValue Script_Game_seti_GlobalStrings(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);
	ScriptString *value = (ScriptString *)params[1]._object;
	UNUSED(value);

	// FIXME
	error("Game::seti_GlobalStrings unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute int GUICount
// Gets the number of GUIs in the game.
RuntimeValue Script_Game_get_GUICount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_GUICount unimplemented");

	return RuntimeValue();
}

// Game: import static attribute int IgnoreUserInputAfterTextTimeoutMs
// Gets/sets the time for which user input is ignored after some text is automatically removed
RuntimeValue Script_Game_get_IgnoreUserInputAfterTextTimeoutMs(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_IgnoreUserInputAfterTextTimeoutMs unimplemented");

	return RuntimeValue();
}

// Game: import static attribute int IgnoreUserInputAfterTextTimeoutMs
// Gets/sets the time for which user input is ignored after some text is automatically removed
RuntimeValue Script_Game_set_IgnoreUserInputAfterTextTimeoutMs(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Game::set_IgnoreUserInputAfterTextTimeoutMs unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute bool InSkippableCutscene
// Checks whether the game is currently in the middle of a skippable cutscene.
RuntimeValue Script_Game_get_InSkippableCutscene(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_InSkippableCutscene unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute int InventoryItemCount
// Gets the number of inventory items in the game.
RuntimeValue Script_Game_get_InventoryItemCount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_InventoryItemCount unimplemented");

	return RuntimeValue();
}

// Game: import static attribute int MinimumTextDisplayTimeMs
// Gets/sets the minimum time that a piece of speech text stays on screen (in milliseconds)
RuntimeValue Script_Game_get_MinimumTextDisplayTimeMs(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_MinimumTextDisplayTimeMs unimplemented");

	return RuntimeValue();
}

// Game: import static attribute int MinimumTextDisplayTimeMs
// Gets/sets the minimum time that a piece of speech text stays on screen (in milliseconds)
RuntimeValue Script_Game_set_MinimumTextDisplayTimeMs(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Game::set_MinimumTextDisplayTimeMs unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute int MouseCursorCount
// Gets the number of mouse cursors in the game.
RuntimeValue Script_Game_get_MouseCursorCount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_MouseCursorCount unimplemented");

	return RuntimeValue();
}

// Game: import static attribute String Name
// Gets/sets the game name.
RuntimeValue Script_Game_get_Name(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_Name unimplemented");

	return RuntimeValue();
}

// Game: import static attribute String Name
// Gets/sets the game name.
RuntimeValue Script_Game_set_Name(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *value = (ScriptString *)params[0]._object;
	UNUSED(value);

	// FIXME
	error("Game::set_Name unimplemented");

	return RuntimeValue();
}

// Game: import static attribute FontType NormalFont
// Gets/sets the normal font used for displaying text.
RuntimeValue Script_Game_get_NormalFont(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_NormalFont unimplemented");

	return RuntimeValue();
}

// Game: import static attribute FontType NormalFont
// Gets/sets the normal font used for displaying text.
RuntimeValue Script_Game_set_NormalFont(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("Game::set_NormalFont unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute bool SkippingCutscene
// Checks whether the game is currently skipping over a cutscene.
RuntimeValue Script_Game_get_SkippingCutscene(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_SkippingCutscene unimplemented");

	return RuntimeValue();
}

// Game: import static attribute FontType SpeechFont
// Gets/sets the font used for displaying speech text.
RuntimeValue Script_Game_get_SpeechFont(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_SpeechFont unimplemented");

	return RuntimeValue();
}

// Game: import static attribute FontType SpeechFont
// Gets/sets the font used for displaying speech text.
RuntimeValue Script_Game_set_SpeechFont(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("Game::set_SpeechFont unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute int SpriteHeight[]
// Gets the height of the specified sprite.
RuntimeValue Script_Game_geti_SpriteHeight(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);

	// FIXME
	error("Game::geti_SpriteHeight unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute int SpriteWidth[]
// Gets the width of the specified sprite.
RuntimeValue Script_Game_geti_SpriteWidth(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int index = params[0]._signedValue;
	UNUSED(index);

	// FIXME
	error("Game::geti_SpriteWidth unimplemented");

	return RuntimeValue();
}

// Game: import static attribute int TextReadingSpeed
// Gets/sets how fast speech text is removed from the screen.
RuntimeValue Script_Game_get_TextReadingSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_TextReadingSpeed unimplemented");

	return RuntimeValue();
}

// Game: import static attribute int TextReadingSpeed
// Gets/sets how fast speech text is removed from the screen.
RuntimeValue Script_Game_set_TextReadingSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Game::set_TextReadingSpeed unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute String TranslationFilename
// Gets name of the currently active translation.
RuntimeValue Script_Game_get_TranslationFilename(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_TranslationFilename unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute bool UseNativeCoordinates
// Gets whether the game is using native co-ordinates.
RuntimeValue Script_Game_get_UseNativeCoordinates(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_UseNativeCoordinates unimplemented");

	return RuntimeValue();
}

// Game: readonly import static attribute int ViewCount
// Gets the number of views in the game.
RuntimeValue Script_Game_get_ViewCount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Game::get_ViewCount unimplemented");

	return RuntimeValue();
}

// import void AbortGame(const string message, ...)
// Exits the game with an error message.
RuntimeValue Script_AbortGame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *message = (ScriptString *)params[0]._object;
	UNUSED(message);

	// FIXME
	error("AbortGame unimplemented");

	return RuntimeValue();
}

// import void QuitGame(int promptUser)
// Quits the game, optionally showing a confirmation dialog.
RuntimeValue Script_QuitGame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int promptUser = params[0]._signedValue;
	UNUSED(promptUser);

	// FIXME
	error("QuitGame unimplemented");

	return RuntimeValue();
}

// import void SetGameSpeed(int framesPerSecond)
// Changes the current game speed.
RuntimeValue Script_SetGameSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 framesPerSecond = params[0]._value;

	debug(3, "SetGameSpeed: %d fps", framesPerSecond);
	vm->setGameSpeed(framesPerSecond);

	return RuntimeValue();
}

// import int GetGameSpeed()
// Gets the current game speed.
RuntimeValue Script_GetGameSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return vm->getGameSpeed();
}

// import int SetGameOption(int option, int value)
// Changes a game option; see the manual for details.
RuntimeValue Script_SetGameOption(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int option = params[0]._signedValue;
	UNUSED(option);
	int value = params[1]._signedValue;
	UNUSED(value);

	// FIXME
	error("SetGameOption unimplemented");

	return RuntimeValue();
}

// import int GetGameOption(int option)
// Gets the current value of a game option.
RuntimeValue Script_GetGameOption(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int option = params[0]._signedValue;
	UNUSED(option);

	// FIXME
	error("GetGameOption unimplemented");

	return RuntimeValue();
}

// import string GetTranslation (const string originalText)
// Gets the translated version of the specified text.
RuntimeValue Script_GetTranslation(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *originalText = (ScriptString *)params[0]._object;
	UNUSED(originalText);

	// FIXME
	error("GetTranslation unimplemented");

	return RuntimeValue();
}

// import int IsTranslationAvailable ()
// Checks if a translation is currently in use.
RuntimeValue Script_IsTranslationAvailable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("IsTranslationAvailable unimplemented");

	return RuntimeValue();
}

// import void RestoreGameDialog()
// Displays the default built-in Restore Game dialog.
RuntimeValue Script_RestoreGameDialog(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("RestoreGameDialog unimplemented");

	return RuntimeValue();
}

// import void SaveGameDialog()
// Displays the default built-in Save Game dialog.
RuntimeValue Script_SaveGameDialog(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("SaveGameDialog unimplemented");

	return RuntimeValue();
}

// import void RestartGame()
// Restarts the game from the restart point.
RuntimeValue Script_RestartGame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("RestartGame unimplemented");

	return RuntimeValue();
}

// import void SaveGameSlot(int slot, const string description)
// Saves the current game position to the specified slot.
RuntimeValue Script_SaveGameSlot(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int slot = params[0]._signedValue;
	UNUSED(slot);
	ScriptString *description = (ScriptString *)params[1]._object;
	UNUSED(description);

	// FIXME
	error("SaveGameSlot unimplemented");

	return RuntimeValue();
}

// import void RestoreGameSlot(int slot)
// Restores the game saved to the specified game slot.
RuntimeValue Script_RestoreGameSlot(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int slot = params[0]._signedValue;
	UNUSED(slot);

	// FIXME
	error("RestoreGameSlot unimplemented");

	return RuntimeValue();
}

// import void DeleteSaveSlot(int slot)
// Deletes the specified save game.
RuntimeValue Script_DeleteSaveSlot(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int slot = params[0]._signedValue;
	UNUSED(slot);

	// FIXME
	error("DeleteSaveSlot unimplemented");

	return RuntimeValue();
}

// import void SetRestartPoint()
// Sets this as the point at which the game will be restarted.
RuntimeValue Script_SetRestartPoint(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("SetRestartPoint unimplemented");

	return RuntimeValue();
}

// import int IsGamePaused()
// Returns whether the game is currently paused.
RuntimeValue Script_IsGamePaused(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("IsGamePaused unimplemented");

	return RuntimeValue();
}

// import int GetGraphicalVariable (const string variableName)
// Undocumented.
RuntimeValue Script_GetGraphicalVariable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *variableName = (ScriptString *)params[0]._object;
	UNUSED(variableName);

	// FIXME
	error("GetGraphicalVariable unimplemented");

	return RuntimeValue();
}

// import void SetGraphicalVariable (const string variableName, int value)
// Undocumented.
RuntimeValue Script_SetGraphicalVariable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *variableName = (ScriptString *)params[0]._object;
	UNUSED(variableName);
	int value = params[1]._signedValue;
	UNUSED(value);

	// FIXME
	error("SetGraphicalVariable unimplemented");

	return RuntimeValue();
}

// import void SetGlobalString(int stringID, const string newValue)
// Old string buffer function.
RuntimeValue Script_SetGlobalString(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int stringID = params[0]._signedValue;
	UNUSED(stringID);
	ScriptString *newValue = (ScriptString *)params[1]._object;
	UNUSED(newValue);

	// FIXME
	error("SetGlobalString unimplemented");

	return RuntimeValue();
}

// import void GetGlobalString(int stringID, string buffer)
// Old string buffer function.
RuntimeValue Script_GetGlobalString(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int stringID = params[0]._signedValue;
	UNUSED(stringID);
	ScriptString *buffer = (ScriptString *)params[1]._object;
	UNUSED(buffer);

	// FIXME
	error("GetGlobalString unimplemented");

	return RuntimeValue();
}

// import void InputBox(const string prompt, string buffer)
// Old string buffer function.
RuntimeValue Script_InputBox(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *prompt = (ScriptString *)params[0]._object;
	UNUSED(prompt);
	ScriptString *buffer = (ScriptString *)params[1]._object;
	UNUSED(buffer);

	// FIXME
	error("InputBox unimplemented");

	return RuntimeValue();
}

// import int GetTranslationName (string buffer)
// Old string buffer function.
RuntimeValue Script_GetTranslationName(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;
	UNUSED(buffer);

	// FIXME
	error("GetTranslationName unimplemented");

	return RuntimeValue();
}

// import int GetSaveSlotDescription(int slot, string buffer)
// Old string buffer function.
RuntimeValue Script_GetSaveSlotDescription(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int slot = params[0]._signedValue;
	UNUSED(slot);
	ScriptString *buffer = (ScriptString *)params[1]._object;
	UNUSED(buffer);

	// FIXME
	error("GetSaveSlotDescription unimplemented");

	return RuntimeValue();
}

// import void GetMessageText (int messageNumber, string buffer)
// Undocumented.
RuntimeValue Script_GetMessageText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int messageNumber = params[0]._signedValue;
	UNUSED(messageNumber);
	ScriptString *buffer = (ScriptString *)params[1]._object;
	UNUSED(buffer);

	// FIXME
	error("GetMessageText unimplemented");

	return RuntimeValue();
}

// import int LoadSaveSlotScreenshot(int saveSlot, int width, int height)
// Undocumented.
RuntimeValue Script_LoadSaveSlotScreenshot(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int saveSlot = params[0]._signedValue;
	UNUSED(saveSlot);
	int width = params[1]._signedValue;
	UNUSED(width);
	int height = params[2]._signedValue;
	UNUSED(height);

	// FIXME
	error("LoadSaveSlotScreenshot unimplemented");

	return RuntimeValue();
}

// import int GetGameParameter(int parameter, int data1=0, int data2=0, int data3=0)
// Undocumented.
RuntimeValue Script_GetGameParameter(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int parameter = params[0]._signedValue;
	UNUSED(parameter);
	int data1 = params[1]._signedValue;
	UNUSED(data1);
	int data2 = params[2]._signedValue;
	UNUSED(data2);
	int data3 = params[3]._signedValue;
	UNUSED(data3);

	// FIXME
	error("GetGameParameter unimplemented");

	return RuntimeValue();
}

// import void GiveScore(int points)
// Adds to the player's score and plays the score sound, if set.
RuntimeValue Script_GiveScore(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int points = params[0]._signedValue;
	UNUSED(points);

	// FIXME
	error("GiveScore unimplemented");

	return RuntimeValue();
}

// import void SetVoiceMode(eVoiceMode)
// Sets whether voice and/or text are used in the game.
RuntimeValue Script_SetVoiceMode(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 evoicemode = params[0]._value;
	UNUSED(evoicemode);

	// FIXME
	error("SetVoiceMode unimplemented");

	return RuntimeValue();
}

// import void SetSkipSpeech(int skipFlag)
// Sets how the player can skip speech lines.
RuntimeValue Script_SetSkipSpeech(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int skipFlag = params[0]._signedValue;
	UNUSED(skipFlag);

	// FIXME
	error("SetSkipSpeech unimplemented");

	return RuntimeValue();
}

// import void SetSpeechStyle(eSpeechStyle)
// Changes the style in which speech is displayed.
RuntimeValue Script_SetSpeechStyle(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 espeechstyle = params[0]._value;
	UNUSED(espeechstyle);

	// FIXME
	error("SetSpeechStyle unimplemented");

	return RuntimeValue();
}

// import void PauseGame()
// Pauses the game, which stops all animations and movement.
RuntimeValue Script_PauseGame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("PauseGame unimplemented");

	return RuntimeValue();
}

// import void UnPauseGame()
// Resumes the game after it was paused earlier.
RuntimeValue Script_UnPauseGame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("UnPauseGame unimplemented");

	return RuntimeValue();
}

// import void SetGlobalInt(int globalInt, int value)
// Undocumented.
RuntimeValue Script_SetGlobalInt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int globalInt = params[0]._signedValue;
	UNUSED(globalInt);
	int value = params[1]._signedValue;
	UNUSED(value);

	// FIXME
	error("SetGlobalInt unimplemented");

	return RuntimeValue();
}

// import int GetGlobalInt(int globalInt)
// Undocumented.
RuntimeValue Script_GetGlobalInt(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int globalInt = params[0]._signedValue;
	UNUSED(globalInt);

	// FIXME
	error("GetGlobalInt unimplemented");

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "Game::ChangeTranslation^1", (ScriptAPIFunction *)&Script_Game_ChangeTranslation, "s", sotNone },
	{ "Game::DoOnceOnly^1", (ScriptAPIFunction *)&Script_Game_DoOnceOnly, "s", sotNone },
	{ "Game::GetColorFromRGB^3", (ScriptAPIFunction *)&Script_Game_GetColorFromRGB, "iii", sotNone },
	{ "Game::GetFrameCountForLoop^2", (ScriptAPIFunction *)&Script_Game_GetFrameCountForLoop, "ii", sotNone },
	{ "Game::GetLocationName^2", (ScriptAPIFunction *)&Script_Game_GetLocationName, "ii", sotNone },
	{ "Game::GetLoopCountForView^1", (ScriptAPIFunction *)&Script_Game_GetLoopCountForView, "i", sotNone },
	{ "Game::GetMODPattern^0", (ScriptAPIFunction *)&Script_Game_GetMODPattern, "", sotNone },
	{ "Game::GetRunNextSettingForLoop^2", (ScriptAPIFunction *)&Script_Game_GetRunNextSettingForLoop, "ii", sotNone },
	{ "Game::GetSaveSlotDescription^1", (ScriptAPIFunction *)&Script_Game_GetSaveSlotDescription, "i", sotNone },
	{ "Game::GetViewFrame^3", (ScriptAPIFunction *)&Script_Game_GetViewFrame, "iii", sotNone },
	{ "Game::InputBox^1", (ScriptAPIFunction *)&Script_Game_InputBox, "s", sotNone },
	{ "Game::IsAudioPlaying^1", (ScriptAPIFunction *)&Script_Game_IsAudioPlaying, "i", sotNone },
	{ "Game::SetAudioTypeSpeechVolumeDrop^2", (ScriptAPIFunction *)&Script_Game_SetAudioTypeSpeechVolumeDrop, "ii", sotNone },
	{ "Game::SetAudioTypeVolume^3", (ScriptAPIFunction *)&Script_Game_SetAudioTypeVolume, "iii", sotNone },
	{ "Game::SetSaveGameDirectory^1", (ScriptAPIFunction *)&Script_Game_SetSaveGameDirectory, "s", sotNone },
	{ "Game::StopAudio^1", (ScriptAPIFunction *)&Script_Game_StopAudio, "i", sotNone },
	{ "Game::StopSound^1", (ScriptAPIFunction *)&Script_Game_StopSound, "i", sotNone },
	{ "Game::get_CharacterCount", (ScriptAPIFunction *)&Script_Game_get_CharacterCount, "", sotNone },
	{ "Game::get_DialogCount", (ScriptAPIFunction *)&Script_Game_get_DialogCount, "", sotNone },
	{ "Game::get_FileName", (ScriptAPIFunction *)&Script_Game_get_FileName, "", sotNone },
	{ "Game::get_FontCount", (ScriptAPIFunction *)&Script_Game_get_FontCount, "", sotNone },
	{ "Game::geti_GlobalMessages", (ScriptAPIFunction *)&Script_Game_geti_GlobalMessages, "i", sotNone },
	{ "Game::geti_GlobalStrings", (ScriptAPIFunction *)&Script_Game_geti_GlobalStrings, "i", sotNone },
	{ "Game::seti_GlobalStrings", (ScriptAPIFunction *)&Script_Game_seti_GlobalStrings, "is", sotNone },
	{ "Game::get_GUICount", (ScriptAPIFunction *)&Script_Game_get_GUICount, "", sotNone },
	{ "Game::get_IgnoreUserInputAfterTextTimeoutMs", (ScriptAPIFunction *)&Script_Game_get_IgnoreUserInputAfterTextTimeoutMs, "", sotNone },
	{ "Game::set_IgnoreUserInputAfterTextTimeoutMs", (ScriptAPIFunction *)&Script_Game_set_IgnoreUserInputAfterTextTimeoutMs, "i", sotNone },
	{ "Game::get_InSkippableCutscene", (ScriptAPIFunction *)&Script_Game_get_InSkippableCutscene, "", sotNone },
	{ "Game::get_InventoryItemCount", (ScriptAPIFunction *)&Script_Game_get_InventoryItemCount, "", sotNone },
	{ "Game::get_MinimumTextDisplayTimeMs", (ScriptAPIFunction *)&Script_Game_get_MinimumTextDisplayTimeMs, "", sotNone },
	{ "Game::set_MinimumTextDisplayTimeMs", (ScriptAPIFunction *)&Script_Game_set_MinimumTextDisplayTimeMs, "i", sotNone },
	{ "Game::get_MouseCursorCount", (ScriptAPIFunction *)&Script_Game_get_MouseCursorCount, "", sotNone },
	{ "Game::get_Name", (ScriptAPIFunction *)&Script_Game_get_Name, "", sotNone },
	{ "Game::set_Name", (ScriptAPIFunction *)&Script_Game_set_Name, "s", sotNone },
	{ "Game::get_NormalFont", (ScriptAPIFunction *)&Script_Game_get_NormalFont, "", sotNone },
	{ "Game::set_NormalFont", (ScriptAPIFunction *)&Script_Game_set_NormalFont, "i", sotNone },
	{ "Game::get_SkippingCutscene", (ScriptAPIFunction *)&Script_Game_get_SkippingCutscene, "", sotNone },
	{ "Game::get_SpeechFont", (ScriptAPIFunction *)&Script_Game_get_SpeechFont, "", sotNone },
	{ "Game::set_SpeechFont", (ScriptAPIFunction *)&Script_Game_set_SpeechFont, "i", sotNone },
	{ "Game::geti_SpriteHeight", (ScriptAPIFunction *)&Script_Game_geti_SpriteHeight, "i", sotNone },
	{ "Game::geti_SpriteWidth", (ScriptAPIFunction *)&Script_Game_geti_SpriteWidth, "i", sotNone },
	{ "Game::get_TextReadingSpeed", (ScriptAPIFunction *)&Script_Game_get_TextReadingSpeed, "", sotNone },
	{ "Game::set_TextReadingSpeed", (ScriptAPIFunction *)&Script_Game_set_TextReadingSpeed, "i", sotNone },
	{ "Game::get_TranslationFilename", (ScriptAPIFunction *)&Script_Game_get_TranslationFilename, "", sotNone },
	{ "Game::get_UseNativeCoordinates", (ScriptAPIFunction *)&Script_Game_get_UseNativeCoordinates, "", sotNone },
	{ "Game::get_ViewCount", (ScriptAPIFunction *)&Script_Game_get_ViewCount, "", sotNone },
	{ "AbortGame", (ScriptAPIFunction *)&Script_AbortGame, "s", sotNone },
	{ "QuitGame", (ScriptAPIFunction *)&Script_QuitGame, "i", sotNone },
	{ "SetGameSpeed", (ScriptAPIFunction *)&Script_SetGameSpeed, "i", sotNone },
	{ "GetGameSpeed", (ScriptAPIFunction *)&Script_GetGameSpeed, "", sotNone },
	{ "SetGameOption", (ScriptAPIFunction *)&Script_SetGameOption, "ii", sotNone },
	{ "GetGameOption", (ScriptAPIFunction *)&Script_GetGameOption, "i", sotNone },
	{ "GetTranslation", (ScriptAPIFunction *)&Script_GetTranslation, "s", sotNone },
	{ "IsTranslationAvailable", (ScriptAPIFunction *)&Script_IsTranslationAvailable, "", sotNone },
	{ "RestoreGameDialog", (ScriptAPIFunction *)&Script_RestoreGameDialog, "", sotNone },
	{ "SaveGameDialog", (ScriptAPIFunction *)&Script_SaveGameDialog, "", sotNone },
	{ "RestartGame", (ScriptAPIFunction *)&Script_RestartGame, "", sotNone },
	{ "SaveGameSlot", (ScriptAPIFunction *)&Script_SaveGameSlot, "is", sotNone },
	{ "RestoreGameSlot", (ScriptAPIFunction *)&Script_RestoreGameSlot, "i", sotNone },
	{ "DeleteSaveSlot", (ScriptAPIFunction *)&Script_DeleteSaveSlot, "i", sotNone },
	{ "SetRestartPoint", (ScriptAPIFunction *)&Script_SetRestartPoint, "", sotNone },
	{ "IsGamePaused", (ScriptAPIFunction *)&Script_IsGamePaused, "", sotNone },
	{ "GetGraphicalVariable", (ScriptAPIFunction *)&Script_GetGraphicalVariable, "s", sotNone },
	{ "SetGraphicalVariable", (ScriptAPIFunction *)&Script_SetGraphicalVariable, "si", sotNone },
	{ "SetGlobalString", (ScriptAPIFunction *)&Script_SetGlobalString, "is", sotNone },
	{ "GetGlobalString", (ScriptAPIFunction *)&Script_GetGlobalString, "is", sotNone },
	{ "InputBox", (ScriptAPIFunction *)&Script_InputBox, "ss", sotNone },
	{ "GetTranslationName", (ScriptAPIFunction *)&Script_GetTranslationName, "s", sotNone },
	{ "GetSaveSlotDescription", (ScriptAPIFunction *)&Script_GetSaveSlotDescription, "is", sotNone },
	{ "GetMessageText", (ScriptAPIFunction *)&Script_GetMessageText, "is", sotNone },
	{ "LoadSaveSlotScreenshot", (ScriptAPIFunction *)&Script_LoadSaveSlotScreenshot, "iii", sotNone },
	{ "GetGameParameter", (ScriptAPIFunction *)&Script_GetGameParameter, "iiii", sotNone },
	{ "GiveScore", (ScriptAPIFunction *)&Script_GiveScore, "i", sotNone },
	{ "SetVoiceMode", (ScriptAPIFunction *)&Script_SetVoiceMode, "i", sotNone },
	{ "SetSkipSpeech", (ScriptAPIFunction *)&Script_SetSkipSpeech, "i", sotNone },
	{ "SetSpeechStyle", (ScriptAPIFunction *)&Script_SetSpeechStyle, "i", sotNone },
	{ "PauseGame", (ScriptAPIFunction *)&Script_PauseGame, "", sotNone },
	{ "UnPauseGame", (ScriptAPIFunction *)&Script_UnPauseGame, "", sotNone },
	{ "SetGlobalInt", (ScriptAPIFunction *)&Script_SetGlobalInt, "ii", sotNone },
	{ "GetGlobalInt", (ScriptAPIFunction *)&Script_GetGlobalInt, "i", sotNone },
};

void addGameSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
