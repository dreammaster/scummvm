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

// This implements the interface of Scorpiorus's ags_snowrain.dll plugin.

/* Based on JJS's PSP port of the AGS engine, which is licensed under the
 * Artistic License 2.0.
 * You may also modify/distribute the code in this file under that license.
 */

#include "engines/ags/scripting/scripting.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/graphics.h"
#include "engines/ags/sprites.h"
#include "common/random.h"

namespace AGS {

namespace SnowRain {

struct Particle {
	float x, y;
	uint alpha;
	float speed;
	int maxY;
	uint type; // 0-4
	int drift;
	float driftSpeed, driftOffset;
};

struct View {
	uint viewId, loopId;
	bool isDefault;
	Graphics::Surface *surface;
};

inline float signum(float x) { return (x > 0) ? 1 : -1; }

class Weather {
public:
	Weather(AGSEngine *vm, bool isSnow = false) : _vm(vm), _isSnow(isSnow) {
		setDriftRange(10, 100);
		setDriftSpeed(10, 120);

		setTransparency(0, 0);
		setWindSpeed(0);
		setBaseline(0, 200);

		if (_isSnow)
			setFallSpeed(10, 70);
		else
			setFallSpeed(100, 300);

		_viewsInitialized = false;

		_views.resize(5);
		for (uint i = 0; i < _views.size(); ++i) {
			_views[i].isDefault = true;
			_views[i].viewId = (uint)-1;
			_views[i].loopId = (uint)-1;
			_views[i].surface = NULL;
		}

		setAmount(0);
	}

	~Weather() {
	}

	void update(bool withDrift = false) {
		if (_targetAmount > _amount)
			_amount++;
		else if (_targetAmount < _amount)
			_amount--;

		if (!reinitViews())
			return;

		Common::RandomSource *rnd = _vm->getRandomSource();
		const uint screenWidth = _vm->_graphics->_width;
		const uint screenHeight = _vm->_graphics->_height;

		for (uint i = 0; i < _amount * 2; ++i) {
			Particle &p = _particles[i];

			p.y += p.speed;
			float drift = p.drift * sin((float)(p.y + p.driftOffset) * p.driftSpeed * 2.0f * M_PI / 360.0f);

			if (!withDrift || signum(_windSpeed) == signum(drift))
				p.x += _windSpeed;
			else
				p.x += _windSpeed / 4;

			if (p.x < 0)
				p.x += screenWidth;
			else if (p.x > screenWidth - 1)
				p.x -= screenWidth;

			if (p.y > p.maxY) {
				// reset to the top

				p.x = rnd->getRandomNumber(screenWidth - 1);
				p.y = -1.0f * rnd->getRandomNumber(screenHeight - 1);
				p.alpha = rnd->getRandomNumber(_deltaAlpha - 1) + _minAlpha;
				p.speed = (rnd->getRandomNumber(_deltaFallSpeed - 1) + _minFallSpeed) / 50.0f;
				p.maxY = rnd->getRandomNumber(_deltaBaseline - 1) + _topBaseline;
				if (withDrift) {
					p.drift = rnd->getRandomNumber(_deltaDrift - 1) + _minDrift;
					p.driftSpeed = (rnd->getRandomNumber(_deltaDriftSpeed - 1) + _minDriftSpeed) / 50.0f;
				}
			} else if (p.y > 0 && p.alpha > 0) {
				// draw a sprite for this flake
				// TODO: icky, also almost certainly wrong
				_vm->_graphics->internalDraw(_views[p.type].surface, Common::Point(p.x, p.y), p.alpha);
			}
		}
	}

	bool reinitViews() {
		return true; // FIXME
	}

	bool isActive() {
		return (_amount > 0 || _targetAmount != _amount);
	}

	void enterRoom() {
		_amount = _targetAmount;
	}

	void initParticles() {
		_particles.resize(2000);

		Common::RandomSource *rnd = _vm->getRandomSource();
		const uint screenWidth = _vm->_graphics->_width;
		const uint screenHeight = _vm->_graphics->_height;

		for (uint i = 0; i < _particles.size(); ++i) {
			Particle &p = _particles[i];

			// TODO: this is very similar to update() above
			p.type = rnd->getRandomNumber(4);
			p.x = rnd->getRandomNumber(screenWidth - 1);
			p.y = rnd->getRandomNumber(screenHeight * 2 - 1) - screenHeight;
			p.alpha = rnd->getRandomNumber(_deltaAlpha - 1) + _minAlpha;
			p.speed = (rnd->getRandomNumber(_deltaFallSpeed - 1) + _minFallSpeed) / 50.0f;
			p.maxY = rnd->getRandomNumber(_deltaBaseline - 1) + _topBaseline;
			p.drift = rnd->getRandomNumber(_deltaDrift - 1) + _minDrift;
			p.driftSpeed = (rnd->getRandomNumber(_deltaDriftSpeed - 1) + _minDriftSpeed) / 50.0f;
			p.driftOffset = rnd->getRandomNumber(100 - 1);
		}
	}

	void setDriftRange(uint min, uint max) {
		min = CLIP<uint>(min, 0, 100);
		max = CLIP<uint>(max, 0, 100);

		min = MIN(min, max);

		_minDrift = min / 2;
		_maxDrift = max / 2;
		_deltaDrift = _maxDrift - _minDrift;

		if (_deltaDrift == 0)
			_deltaDrift = 1;
	}

	void setDriftSpeed(uint min, uint max) {
		min = CLIP<uint>(min, 0, 200);
		max = CLIP<uint>(max, 0, 200);

		min = MIN(min, max);

		_minDriftSpeed = min;
		_maxDriftSpeed = max;
		_deltaDriftSpeed = _maxDriftSpeed - _minDriftSpeed;

		if (_deltaDriftSpeed == 0)
			_deltaDriftSpeed = 1;
	}

	void changeAmount(uint amount) {
		amount = CLIP<uint>(amount, 0, 1000);

		_targetAmount = amount;
	}

	void setAmount(uint amount) {
		amount = CLIP<uint>(amount, 0, 1000);

		_amount = amount;
		_targetAmount = amount;

		initParticles();
	}

	void setView(uint type, uint view, uint loop) {
		ViewFrame *frame = _vm->getViewFrame(view - 1, loop, 0);
		Sprite *sprite = _vm->getSprites()->getSprite(frame->_pic);
		// TODO: don't just discard the sprite?

		_views[type].surface = sprite->_surface;
		_views[type].isDefault = false;
		_views[type].viewId = view;
		_views[type].loopId = loop;

		if (!_viewsInitialized)
			setDefaultView(view, loop);
	}

	void setDefaultView(uint view, uint loop) {
		ViewFrame *frame = _vm->getViewFrame(view - 1, loop, 0);
		Sprite *sprite = _vm->getSprites()->getSprite(frame->_pic);
		// TODO: don't just discard the sprite?

		for (uint i = 0; i < _views.size(); ++i) {
			if (!_views[i].isDefault)
				continue;

			_views[i].viewId = view;
			_views[i].loopId = loop;
			_views[i].surface = sprite->_surface;
		}

		_viewsInitialized = true;
	}

	void setTransparency(uint min, uint max) {
		min = CLIP<uint>(min, 0, 100);
		max = CLIP<uint>(max, 0, 100);
		min = MIN(min, max);

		// note: reversed
		_minAlpha = 255 - floor(max * 2.55f + 0.5f);
		_maxAlpha = 255 - floor(min * 2.55f + 0.5f);
		_deltaAlpha = _maxAlpha - _minAlpha;

		if (_deltaAlpha == 0)
			_deltaAlpha = 1;

		Common::RandomSource *rnd = _vm->getRandomSource();
		for (uint i = 0; i < _particles.size(); ++i)
			_particles[i].alpha = rnd->getRandomNumber(_deltaAlpha - 1) + _minAlpha;
	}

	void setBaseline(uint top, uint bottom) {
		const uint screenHeight = _vm->_graphics->_height;

		top = CLIP<uint>(top, 0, screenHeight);
		bottom = CLIP<uint>(bottom, 0, screenHeight);
		top = MIN(top, bottom);

		_topBaseline = top;
		_bottomBaseline = bottom;
		_deltaBaseline = _bottomBaseline - _topBaseline;

		if (_deltaBaseline == 0)
			_deltaBaseline = 1;
	}

	void setWindSpeed(int speed) {
		_windSpeed = (float)CLIP<int>(speed, -200, 200);
	}

	void setFallSpeed(uint min, uint max) {
		min = CLIP<uint>(min, 0, 1000);
		max = CLIP<uint>(max, 0, 1000);

		min = MIN(min, max);

		_minFallSpeed = min;
		_maxFallSpeed = max;
		_deltaFallSpeed = _maxFallSpeed - _minFallSpeed;

		if (_deltaFallSpeed == 0)
			_deltaFallSpeed = 1;
	}

protected:
	AGSEngine *_vm;

	bool _isSnow;

	uint _minDrift, _maxDrift, _deltaDrift;
	uint _minDriftSpeed, _maxDriftSpeed, _deltaDriftSpeed;
	uint _minFallSpeed, _maxFallSpeed, _deltaFallSpeed;
	uint _topBaseline, _bottomBaseline, _deltaBaseline;
	uint _minAlpha, _maxAlpha, _deltaAlpha;
	float _windSpeed;

	bool _viewsInitialized;
	Common::Array<View> _views;

	uint _amount, _targetAmount;
	Common::Array<Particle> _particles;
};

} // namespace SnowRain


SnowRain::Weather *g_snow = NULL, *g_rain = NULL;

void initSnowRain(AGSEngine *vm) {
	g_snow = new SnowRain::Weather(vm, true);
	g_rain = new SnowRain::Weather(vm, false);
}

void drawSnowRain() {
	if (g_rain->isActive())
		g_rain->update(false);

	if (g_snow->isActive())
		g_snow->update(true);
}

// FIXME: enterRoom() interface

void shutdownSnowRain() {
	delete g_snow;
	g_snow = NULL;
	delete g_rain;
	g_rain = NULL;
}


// import void srSetSnowDriftRange(int minValue, int maxValue)
// Set the drift range of snowflakes (0 to 100).
RuntimeValue Script_srSetSnowDriftRange(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint minValue = params[0]._value;
	uint maxValue = params[1]._value;

	g_snow->setDriftRange(minValue, maxValue);

	return RuntimeValue();
}

// import void srSetSnowDriftSpeed(int minValue, int maxValue)
// Set the drift speed of snowflakes (0 to 200).
RuntimeValue Script_srSetSnowDriftSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint minValue = params[0]._value;
	uint maxValue = params[1]._value;

	g_snow->setDriftSpeed(minValue, maxValue);

	return RuntimeValue();
}

// import void srSetSnowFallSpeed(int minValue, int maxValue)
// Set the falling speed of snowflakes (1 to 1000).
RuntimeValue Script_srSetSnowFallSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint minValue = params[0]._value;
	uint maxValue = params[1]._value;

	g_snow->setFallSpeed(minValue, maxValue);

	return RuntimeValue();
}

// import void srSetRainFallSpeed(int minValue, int maxValue)
// Undocumented.
RuntimeValue Script_srSetRainFallSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint minValue = params[0]._value;
	uint maxValue = params[1]._value;

	g_rain->setFallSpeed(minValue, maxValue);

	return RuntimeValue();
}

// import void srChangeSnowAmount(int amount)
// Set the snow amount (0 to 1000).
RuntimeValue Script_srChangeSnowAmount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint amount = params[0]._value;

	g_snow->changeAmount(amount);

	return RuntimeValue();
}

// import void srChangeRainAmount(int amount)
// Undocumented.
RuntimeValue Script_srChangeRainAmount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint amount = params[0]._value;

	g_rain->changeAmount(amount);

	return RuntimeValue();
}

// import void srSetSnowAmount(int amount)
// Instantly changes the snow amount (0 to 1000).
RuntimeValue Script_srSetSnowAmount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint amount = params[0]._value;

	g_snow->setAmount(amount);

	return RuntimeValue();
}

// import void srSetRainAmount(int amount)
// Undocumented.
RuntimeValue Script_srSetRainAmount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint amount = params[0]._value;

	g_rain->setAmount(amount);

	return RuntimeValue();
}

// import void srSetSnowBaseline(int top, int bottom)
// Set the boundaries between which the snow falls (0 to 200).
RuntimeValue Script_srSetSnowBaseline(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint top = params[0]._value;
	uint bottom = params[1]._value;

	g_snow->setBaseline(top, bottom);

	return RuntimeValue();
}

// import void srSetRainBaseline(int top, int bottom)
// Undocumented.
RuntimeValue Script_srSetRainBaseline(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint top = params[0]._value;
	uint bottom = params[1]._value;

	g_rain->setBaseline(top, bottom);

	return RuntimeValue();
}

// import void srSetBaseline(int top, int bottom)
// Set the boundaries for both snow and rain (0 to 200).
RuntimeValue Script_srSetBaseline(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint top = params[0]._value;
	uint bottom = params[1]._value;

	g_snow->setBaseline(top, bottom);
	g_rain->setBaseline(top, bottom);

	return RuntimeValue();
}

// import void srSetSnowTransparency(int minValue, int maxValue)
// Sets the transparency of snow (0 to 100).
RuntimeValue Script_srSetSnowTransparency(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint minValue = params[0]._value;
	uint maxValue = params[1]._value;

	g_snow->setTransparency(minValue, maxValue);

	return RuntimeValue();
}

// import void srSetRainTransparency(int minValue, int maxValue)
// Undocumented.
RuntimeValue Script_srSetRainTransparency(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint minValue = params[0]._value;
	uint maxValue = params[1]._value;

	g_rain->setTransparency(minValue, maxValue);

	return RuntimeValue();
}

// import void srSetSnowDefaultView(int view, int loop)
// Set the default snow view/loop, used for 'normal' flakes which haven't had srSetSnowView called.
RuntimeValue Script_srSetSnowDefaultView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint view = params[0]._value;
	uint loop = params[1]._value;

	g_snow->setDefaultView(view, loop);

	return RuntimeValue();
}

// import void srSetRainDefaultView(int view, int loop)
// Undocumented.
RuntimeValue Script_srSetRainDefaultView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint view = params[0]._value;
	uint loop = params[1]._value;

	g_rain->setDefaultView(view, loop);

	return RuntimeValue();
}

// import void srSetSnowView(int kindId, int event, int view, int loop)
// Set a special view/loop for the specified kind of flake.
RuntimeValue Script_srSetSnowView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint kindId = params[0]._signedValue;
	uint event = params[1]._signedValue;
	UNUSED(event);
	uint view = params[2]._signedValue;
	uint loop = params[3]._signedValue;

	if (kindId > 4)
		error("srSetSnowView: type %d is invalid (must be 0-4)", kindId);

	g_snow->setView(kindId, view, loop);

	return RuntimeValue();
}

// import void srSetRainView(int kindId, int event, int view, int loop)
// Undocumented.
RuntimeValue Script_srSetRainView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint kindId = params[0]._signedValue;
	uint event = params[1]._signedValue;
	UNUSED(event);
	uint view = params[2]._signedValue;
	uint loop = params[3]._signedValue;

	if (kindId > 4)
		error("srSetRainView: type %d is invalid (must be 0-4)", kindId);

	g_rain->setView(kindId, view, loop);

	return RuntimeValue();
}

// import void srSetSnowWindSpeed(int value)
// Set the wind speed for snow (-200 to 200).
RuntimeValue Script_srSetSnowWindSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	g_snow->setWindSpeed(value);

	return RuntimeValue();
}

// import void srSetRainWindSpeed(int value)
// Undocumented.
RuntimeValue Script_srSetRainWindSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	g_rain->setWindSpeed(value);

	return RuntimeValue();
}

// import void srSetWindSpeed(int value)
// Set the wind speed for both snow and rain (-200 to 200).
RuntimeValue Script_srSetWindSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	g_snow->setWindSpeed(value);
	g_rain->setWindSpeed(value);

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "srSetSnowDriftRange", (ScriptAPIFunction *)&Script_srSetSnowDriftRange, "ii", sotNone },
	{ "srSetSnowDriftSpeed", (ScriptAPIFunction *)&Script_srSetSnowDriftSpeed, "ii", sotNone },
	{ "srSetSnowFallSpeed", (ScriptAPIFunction *)&Script_srSetSnowFallSpeed, "ii", sotNone },
	{ "srSetRainFallSpeed", (ScriptAPIFunction *)&Script_srSetRainFallSpeed, "ii", sotNone },
	{ "srChangeSnowAmount", (ScriptAPIFunction *)&Script_srChangeSnowAmount, "i", sotNone },
	{ "srChangeRainAmount", (ScriptAPIFunction *)&Script_srChangeRainAmount, "i", sotNone },
	{ "srSetSnowAmount", (ScriptAPIFunction *)&Script_srSetSnowAmount, "i", sotNone },
	{ "srSetRainAmount", (ScriptAPIFunction *)&Script_srSetRainAmount, "i", sotNone },
	{ "srSetSnowBaseline", (ScriptAPIFunction *)&Script_srSetSnowBaseline, "ii", sotNone },
	{ "srSetRainBaseline", (ScriptAPIFunction *)&Script_srSetRainBaseline, "ii", sotNone },
	{ "srSetBaseline", (ScriptAPIFunction *)&Script_srSetBaseline, "ii", sotNone },
	{ "srSetSnowTransparency", (ScriptAPIFunction *)&Script_srSetSnowTransparency, "ii", sotNone },
	{ "srSetRainTransparency", (ScriptAPIFunction *)&Script_srSetRainTransparency, "ii", sotNone },
	{ "srSetSnowDefaultView", (ScriptAPIFunction *)&Script_srSetSnowDefaultView, "ii", sotNone },
	{ "srSetRainDefaultView", (ScriptAPIFunction *)&Script_srSetRainDefaultView, "ii", sotNone },
	{ "srSetSnowView", (ScriptAPIFunction *)&Script_srSetSnowView, "iiii", sotNone },
	{ "srSetRainView", (ScriptAPIFunction *)&Script_srSetRainView, "iiii", sotNone },
	{ "srSetSnowWindSpeed", (ScriptAPIFunction *)&Script_srSetSnowWindSpeed, "i", sotNone },
	{ "srSetRainWindSpeed", (ScriptAPIFunction *)&Script_srSetRainWindSpeed, "i", sotNone },
	{ "srSetWindSpeed", (ScriptAPIFunction *)&Script_srSetWindSpeed, "i", sotNone },
};

void addSnowRainSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
