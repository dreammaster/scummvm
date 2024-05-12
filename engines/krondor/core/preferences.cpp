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

#include "krondor/core/preferences.h"

namespace Krondor {

void Preferences::setDefaults() {
	_stepSize = STS_SMALL;
	_turnSize = STS_SMALL;
	_textSpeed = TS_WAIT;
	_detail = LOD_MAX;
	_sound = true;
	_music = true;
	_combatMusic = true;
	_cdMusic = false;
	_introduction = true;
}

void Preferences::setStepSize(const StepTurnSize sz) {
	_stepSize = sz;
}

void Preferences::setTurnSize(const StepTurnSize sz) {
	_turnSize = sz;
}

void Preferences::setTextSpeed(const TextSpeed ts) {
	_textSpeed = ts;
}

void Preferences::setDetail(const LevelOfDetail lod) {
	_detail = lod;
}

bool Preferences::getSound() const {
	return _sound;
}

void Preferences::setSound(bool toggle) {
	_sound = toggle;
}

bool Preferences::getMusic() const {
	return _music;
}

void Preferences::setMusic(bool toggle) {
	_music = toggle;
}

bool Preferences::getCombatMusic() const {
	return _combatMusic;
}

void Preferences::setCombatMusic(bool toggle) {
	_combatMusic = toggle;
}

bool Preferences::getCdMusic() const {
	return _cdMusic;
}

void Preferences::setCdMusic(bool toggle) {
	_cdMusic = toggle;
}

void Preferences::setIntroduction(bool toggle) {
	_introduction = toggle;
}

} // namespace Krondor
