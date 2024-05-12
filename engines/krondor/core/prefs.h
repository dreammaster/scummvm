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

#ifndef KRONDOR_CORE_PREFERENCES_H
#define KRONDOR_CORE_PREFERENCES_H

#include "krondor/res/image_resource.h"

namespace Krondor {

enum StepTurnSize {
	STS_SMALL,
	STS_MEDIUM,
	STS_LARGE
};

enum TextSpeed {
	TS_WAIT,
	TS_MEDIUM,
	TS_FAST
};

enum LevelOfDetail {
	LOD_MIN,
	LOD_LOW,
	LOD_HIGH,
	LOD_MAX
};

class Prefs {
private:
	StepTurnSize _stepSize = STS_LARGE;
	StepTurnSize _turnSize = STS_MEDIUM;
	TextSpeed _textSpeed = TS_WAIT;
	LevelOfDetail _detail = LOD_HIGH;
	bool _sound = true;
	bool _music = true;
	bool _combatMusic = true;
	bool _cdMusic = false;
	bool _introduction = true;

public:
	Prefs() { setDefaults(); }
	
	void setDefaults();
	void apply();

	StepTurnSize getStepSize() const { return _stepSize; }
	void setStepSize(const StepTurnSize sz);
	StepTurnSize getTurnSize() const { return _turnSize; }
	void setTurnSize(const StepTurnSize sz);
	TextSpeed getTextSpeed() const { return _textSpeed; }
	void setTextSpeed(const TextSpeed ts);
	LevelOfDetail getDetail() const { return _detail; }
	void setDetail(const LevelOfDetail lod);
	bool getSound() const;
	void setSound(const bool toggle);
	bool getMusic() const;
	void setMusic(const bool toggle);
	bool getCombatMusic() const;
	void setCombatMusic(const bool toggle);
	bool getCdMusic() const;
	void setCdMusic(const bool toggle);
	bool getIntroduction() const { return _introduction; }
	void setIntroduction(const bool toggle);
};

} // namespace Krondor

#endif
