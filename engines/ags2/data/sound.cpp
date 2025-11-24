
/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ags2/data/sound.h"

namespace AGS2 {

void ScriptAudioClip::synchronize(Common::Serializer &s) {
	s.syncAsSint32LE(id);
	s.syncBytes((byte *)scriptName, 30);
	s.syncBytes((byte *)fileName, 15);
	s.syncAsByte(bundlingType);
	s.syncAsByte(type);
	s.syncAsByte(fileType);
	s.syncAsByte(defaultRepeat);
	s.syncAsSint16LE(defaultPriority);
	s.syncAsSint16LE(defaultVolume);
	s.syncAsSint32LE(reserved);
};

void AudioClipType::synchronize(Common::Serializer &s) {
	s.syncAsSint32LE(id);
	s.syncAsSint32LE(reservedChannels);
	s.syncAsSint32LE(volume_reduction_while_speech_playing);
	s.syncAsSint32LE(crossfadeSpeed);
	s.syncAsSint32LE(reservedForFuture);
}

void AmbientSound::synchronize(Common::Serializer &s) {
	s.syncAsSint32LE(channel);
	s.syncAsSint32LE(x);
	s.syncAsSint32LE(y);
	s.syncAsSint32LE(vol);
	s.syncAsSint32LE(num);
	s.syncAsSint32LE(maxdist);
}

} // namespace AGS2
