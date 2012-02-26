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

#include "engines/ags/scripting/scripting.h"

namespace AGS {

// import void srSetSnowDriftRange(int minValue, int maxValue)
// Set the drift range of snowflakes (0 to 100).
RuntimeValue Script_srSetSnowDriftRange(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int minValue = params[0]._signedValue;
	UNUSED(minValue);
	int maxValue = params[1]._signedValue;
	UNUSED(maxValue);

	// FIXME
	error("srSetSnowDriftRange unimplemented");

	return RuntimeValue();
}

// import void srSetSnowDriftSpeed(int minValue, int maxValue)
// Set the drift speed of snowflakes (0 to 200).
RuntimeValue Script_srSetSnowDriftSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int minValue = params[0]._signedValue;
	UNUSED(minValue);
	int maxValue = params[1]._signedValue;
	UNUSED(maxValue);

	// FIXME
	error("srSetSnowDriftSpeed unimplemented");

	return RuntimeValue();
}

// import void srSetSnowFallSpeed(int minValue, int maxValue)
// Set the falling speed of snowflakes (1 to 1000).
RuntimeValue Script_srSetSnowFallSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int minValue = params[0]._signedValue;
	UNUSED(minValue);
	int maxValue = params[1]._signedValue;
	UNUSED(maxValue);

	// FIXME
	error("srSetSnowFallSpeed unimplemented");

	return RuntimeValue();
}

// import void srSetRainFallSpeed(int minValue, int maxValue)
// Undocumented.
RuntimeValue Script_srSetRainFallSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int minValue = params[0]._signedValue;
	UNUSED(minValue);
	int maxValue = params[1]._signedValue;
	UNUSED(maxValue);

	// FIXME
	error("srSetRainFallSpeed unimplemented");

	return RuntimeValue();
}

// import void srChangeSnowAmount(int amount)
// Set the snow amount (0 to 1000).
RuntimeValue Script_srChangeSnowAmount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int amount = params[0]._signedValue;
	UNUSED(amount);

	// FIXME
	error("srChangeSnowAmount unimplemented");

	return RuntimeValue();
}

// import void srChangeRainAmount(int amount)
// Undocumented.
RuntimeValue Script_srChangeRainAmount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int amount = params[0]._signedValue;
	UNUSED(amount);

	// FIXME
	error("srChangeRainAmount unimplemented");

	return RuntimeValue();
}

// import void srSetSnowAmount(int amount)
// Instantly changes the snow amount (0 to 1000).
RuntimeValue Script_srSetSnowAmount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int amount = params[0]._signedValue;
	UNUSED(amount);

	// FIXME
	error("srSetSnowAmount unimplemented");

	return RuntimeValue();
}

// import void srSetRainAmount(int amount)
// Undocumented.
RuntimeValue Script_srSetRainAmount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int amount = params[0]._signedValue;
	UNUSED(amount);

	// FIXME
	error("srSetRainAmount unimplemented");

	return RuntimeValue();
}

// import void srSetSnowBaseline(int top, int bottom)
// Set the boundaries between which the snow falls (0 to 200).
RuntimeValue Script_srSetSnowBaseline(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int top = params[0]._signedValue;
	UNUSED(top);
	int bottom = params[1]._signedValue;
	UNUSED(bottom);

	// FIXME
	error("srSetSnowBaseline unimplemented");

	return RuntimeValue();
}

// import void srSetRainBaseline(int top, int bottom)
// Undocumented.
RuntimeValue Script_srSetRainBaseline(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int top = params[0]._signedValue;
	UNUSED(top);
	int bottom = params[1]._signedValue;
	UNUSED(bottom);

	// FIXME
	error("srSetRainBaseline unimplemented");

	return RuntimeValue();
}

// import void srSetBaseline(int top, int bottom)
// Set the boundaries for both snow and rain (0 to 200).
RuntimeValue Script_srSetBaseline(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int top = params[0]._signedValue;
	UNUSED(top);
	int bottom = params[1]._signedValue;
	UNUSED(bottom);

	// FIXME
	error("srSetBaseline unimplemented");

	return RuntimeValue();
}

// import void srSetSnowTransparency(int minValue, int maxValue)
// Sets the transparency of snow (0 to 100).
RuntimeValue Script_srSetSnowTransparency(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int minValue = params[0]._signedValue;
	UNUSED(minValue);
	int maxValue = params[1]._signedValue;
	UNUSED(maxValue);

	// FIXME
	error("srSetSnowTransparency unimplemented");

	return RuntimeValue();
}

// import void srSetRainTransparency(int minValue, int maxValue)
// Undocumented.
RuntimeValue Script_srSetRainTransparency(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int minValue = params[0]._signedValue;
	UNUSED(minValue);
	int maxValue = params[1]._signedValue;
	UNUSED(maxValue);

	// FIXME
	error("srSetRainTransparency unimplemented");

	return RuntimeValue();
}

// import void srSetSnowDefaultView(int view, int loop)
// Set the default snow view/loop, used for 'normal' flakes which haven't had srSetSnowView called.
RuntimeValue Script_srSetSnowDefaultView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int view = params[0]._signedValue;
	UNUSED(view);
	int loop = params[1]._signedValue;
	UNUSED(loop);

	// FIXME
	error("srSetSnowDefaultView unimplemented");

	return RuntimeValue();
}

// import void srSetRainDefaultView(int view, int loop)
// Undocumented.
RuntimeValue Script_srSetRainDefaultView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int view = params[0]._signedValue;
	UNUSED(view);
	int loop = params[1]._signedValue;
	UNUSED(loop);

	// FIXME
	error("srSetRainDefaultView unimplemented");

	return RuntimeValue();
}

// import void srSetSnowView(int kindId, int event, int view, int loop)
// Set a special view/loop for the specified kind of flake.
RuntimeValue Script_srSetSnowView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int kindId = params[0]._signedValue;
	UNUSED(kindId);
	int event = params[1]._signedValue;
	UNUSED(event);
	int view = params[2]._signedValue;
	UNUSED(view);
	int loop = params[3]._signedValue;
	UNUSED(loop);

	// FIXME
	error("srSetSnowView unimplemented");

	return RuntimeValue();
}

// import void srSetRainView(int kindId, int event, int view, int loop)
// Undocumented.
RuntimeValue Script_srSetRainView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int kindId = params[0]._signedValue;
	UNUSED(kindId);
	int event = params[1]._signedValue;
	UNUSED(event);
	int view = params[2]._signedValue;
	UNUSED(view);
	int loop = params[3]._signedValue;
	UNUSED(loop);

	// FIXME
	error("srSetRainView unimplemented");

	return RuntimeValue();
}

// import void srSetSnowWindSpeed(int value)
// Set the wind speed for snow (-200 to 200).
RuntimeValue Script_srSetSnowWindSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("srSetSnowWindSpeed unimplemented");

	return RuntimeValue();
}

// import void srSetRainWindSpeed(int value)
// Undocumented.
RuntimeValue Script_srSetRainWindSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("srSetRainWindSpeed unimplemented");

	return RuntimeValue();
}

// import void srSetWindSpeed(int value)
// Set the wind speed for both snow and rain (-200 to 200).
RuntimeValue Script_srSetWindSpeed(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("srSetWindSpeed unimplemented");

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
