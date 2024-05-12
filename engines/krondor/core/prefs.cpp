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

#include "krondor/core/prefs.h"

namespace Krondor {

void Prefs::setDefaults() {
	_stepSize = STS_LARGE;
	_turnSize = STS_MEDIUM;
	_textSpeed = TS_WAIT;
	_detail = LOD_HIGH;
	_sound = true;
	_music = true;
	_combatMusic = true;
	_cdMusic = false;
	_introduction = true;
}

void Prefs::apply() {
	// TODO
}

void Prefs::setStepSize(const StepTurnSize sz) {
	_stepSize = sz;
}

void Prefs::setTurnSize(const StepTurnSize sz) {
	_turnSize = sz;
}

void Prefs::setTextSpeed(const TextSpeed ts) {
	_textSpeed = ts;
}

void Prefs::setDetail(const LevelOfDetail lod) {
	_detail = lod;
}

bool Prefs::getSound() const {
	return _sound;
}

void Prefs::setSound(bool toggle) {
	_sound = toggle;
}

bool Prefs::getMusic() const {
	return _music;
}

void Prefs::setMusic(bool toggle) {
	_music = toggle;
}

bool Prefs::getCombatMusic() const {
	return _combatMusic;
}

void Prefs::setCombatMusic(bool toggle) {
	_combatMusic = toggle;
}

bool Prefs::getCdMusic() const {
	return _cdMusic;
}

void Prefs::setCdMusic(bool toggle) {
	_cdMusic = toggle;
}

void Prefs::setIntroduction(bool toggle) {
	_introduction = toggle;
}

} // namespace Krondor
