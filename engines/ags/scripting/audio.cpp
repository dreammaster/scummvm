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

void addAudioSystemScripting(GlobalScriptState *state) {
	// pretending-to-be-member audio functions
	state->addSystemFunctionImport("Game::GetMODPattern^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Game::StopSound^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("System::get_Volume", &Script_UnimplementedStub);
	state->addSystemFunctionImport("System::set_Volume", &Script_UnimplementedStub);

	// static audio(/video) functions
	state->addSystemFunctionImport("GetCurrentMusic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetMIDIPosition", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetMP3PosMillis", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsChannelPlaying", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsMusicPlaying", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsMusicVoxAvailable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsSoundPlaying", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsVoxAvailable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PlayAmbientSound", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PlayFlic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PlayMP3File", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PlayMusic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PlayMusicQueued", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PlaySilentMIDI", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PlaySound", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PlaySoundEx", &Script_UnimplementedStub);
	state->addSystemFunctionImport("PlayVideo", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SeekMIDIPosition", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SeekMODPattern", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SeekMP3PosMillis", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetDigitalMasterVolume", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetMusicMasterVolume", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetMusicRepeat", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetMusicVolume", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetSoundVolume", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetSpeechVolume", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StopAmbientSound", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StopChannel", &Script_UnimplementedStub);
	state->addSystemFunctionImport("StopMusic", &Script_UnimplementedStub);
}

} // End of namespace AGS
