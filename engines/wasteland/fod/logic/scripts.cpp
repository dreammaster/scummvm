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

#include "wasteland/fod/logic/scripts.h"
#include "wasteland/fod/fod.h"
#include "wasteland/fod/sound.h"
#include "wasteland/fod/data/map.h"
#include "wasteland/core/algorithms.h"

namespace Wasteland {
namespace FOD {
namespace Logic {

enum { BYTE = 1, WORD = 2};
static const byte PARAMS1[] = { 0 };
static const byte PARAMS2[] = { BYTE, 0 };
static const byte PARAMS3[] = { BYTE, BYTE, 0 };
static const byte PARAMS4[] = { WORD, 0 };
static const byte PARAMS5[] = { BYTE, BYTE, BYTE, 0 };
static const byte PARAMS6[] = { BYTE, WORD, 0 };
static const byte PARAMS7[] = { BYTE, BYTE, BYTE, BYTE, 0 };
static const byte PARAMS8[] = { BYTE, BYTE, WORD, 0 };
static const byte PARAMS9[] = { WORD, WORD, 0 };
static const byte PARAMS10[] = { BYTE, BYTE, BYTE, WORD, 0 };
static const byte PARAMS11[] = { BYTE, BYTE, BYTE, WORD, WORD, 0 };
static const byte *const PARAMS[11] = {
	PARAMS1, PARAMS2, PARAMS3, PARAMS4, PARAMS5, PARAMS6,
	PARAMS7, PARAMS8, PARAMS9, PARAMS10, PARAMS11
};

const char *const OPCODE_NAMES[76] = {
	"HealParty", nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr,

	nullptr, nullptr, "MoveParty", nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr,

	nullptr, nullptr, nullptr, "Message", nullptr,
	nullptr, nullptr, nullptr, "OverrideTile1", "OverrideTile2",

	"OverrideTile3", "Sound", nullptr, "NOP1", nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr,

	nullptr, "DamageParty", nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr,

	nullptr, "NOP2", "MovePerson", nullptr, nullptr,
	nullptr, nullptr, "MessageDialog", nullptr, nullptr,

	nullptr, nullptr, nullptr, nullptr, "SetAffliction",
	nullptr, nullptr, nullptr, nullptr, nullptr,

	nullptr, "DamagePartyIgnoreAC", nullptr, nullptr, nullptr,
	"MovePerson2"
};

const Scripts::OpcodeFunction Scripts::OPCODE_FUNCTIONS[76] = {
	&Scripts::opcode00_healOrDamageParty,
	&Scripts::opcode01,
	&Scripts::opcode02,
	&Scripts::opcode03,
	&Scripts::opcode04,
	&Scripts::opcode05,
	&Scripts::opcode06,
	&Scripts::opcode07,
	&Scripts::opcode08,
	&Scripts::opcode09,

	&Scripts::opcode10,
	&Scripts::opcode11,
	&Scripts::opcode12_MoveParty,
	&Scripts::opcode13,
	&Scripts::opcode14,
	&Scripts::opcode15,
	&Scripts::opcode16,
	&Scripts::opcode17,
	&Scripts::opcode18,
	&Scripts::opcode19,

	&Scripts::opcode20,
	&Scripts::opcode21,
	&Scripts::opcode22,
	&Scripts::opcode23_Message,
	&Scripts::opcode24,
	&Scripts::opcode25,
	&Scripts::opcode26,
	&Scripts::opcode27,
	&Scripts::opcode28_setTileOverride,
	&Scripts::opcode28_setTileOverride,

	&Scripts::opcode28_setTileOverride,
	&Scripts::opcode31_Sound,
	&Scripts::opcode32,
	&Scripts::opcodeNOP,
	&Scripts::opcode34,
	&Scripts::opcode35,
	&Scripts::opcode36,
	&Scripts::opcode37,
	&Scripts::opcode38,
	&Scripts::opcode39,

	&Scripts::opcode40,
	&Scripts::opcode00_healOrDamageParty,
	&Scripts::opcode42,
	&Scripts::opcode43,
	&Scripts::opcode44,
	&Scripts::opcode45,
	&Scripts::opcode46,
	&Scripts::opcode47,
	&Scripts::opcode48,
	&Scripts::opcode49,

	&Scripts::opcode50,
	&Scripts::opcodeNOP,
	&Scripts::opcode52_MovePerson,
	&Scripts::opcode53,
	&Scripts::opcode54,
	&Scripts::opcode55,
	&Scripts::opcode56,
	&Scripts::opcode57_messageDialog,
	&Scripts::opcode58,
	&Scripts::opcode59,

	&Scripts::opcode60,
	&Scripts::opcode61,
	&Scripts::opcode62,
	&Scripts::opcode63,
	&Scripts::opcode64_setAffliction,
	&Scripts::opcode65,
	&Scripts::opcode66,
	&Scripts::opcode67,
	&Scripts::opcode68_End,
	&Scripts::opcode69,

	&Scripts::opcode17,
	&Scripts::opcode00_healOrDamageParty,
	&Scripts::opcode72,
	&Scripts::opcode73,
	&Scripts::opcode74,
	&Scripts::opcode52_MovePerson
};

void Scripts::execute(const uint16 *idPtr, int action, int charNum, ScriptDoneCallback doneCallback) {
	assert(idPtr && doneCallback);
	assert(_state == kScriptIdle);

	_idPtr = idPtr;
	_actionNum = action;
	_charNum = charNum;
	_partyNum = 0;
	_doneCallback = doneCallback;

	executeLoop();
}

void Scripts::resume(const ScriptResumeParams &params) {
	assert(_state == kScriptPaused);
	_resumeParams = params;

	executeLoop();
}

void Scripts::executeLoop() {
	bool firstTime = _state == kScriptPaused;

	while (firstTime || (_actionNum != -1 && _actionNum != 8)) {
		firstTime = false;
		executeScript();

		// if (_redrawFlag && !_actionNum) g_engine->findView("Game")->redraw();
	}

	if (_state == kScriptBreak)
		_doneCallback();
}

bool Scripts::executeScript() {
	const auto &disk = g_engine->_disk;
	uint16 id = *_idPtr;
//	int charNum = _charNum;
	int partyNum = _partyNum;
	int action = _actionNum;
//	int arg4 = 0;
	uint flags;

	if (_state == kScriptPaused)
		goto resumeScript;

	_scriptVal1 = _scriptVal2 = _scriptVal3 = 0;
	_actionNum = -1;
	_newPartyNum = partyNum;

	//int local1 = 0, local2 = 0, local3 = 0;
//	if (action == 4) local1 = arg4;

	if (id == 0xffff)
		return false;

	// Check whether the script handles the particular action
	_scriptP = &disk._scripts[READ_LE_UINT16(&disk._scripts[id * 2])];
	flags = READ_LE_UINT16(_scriptP);
	if (!(flags & (1 << action)))
		return false;

	_val2 = 1;
	_scriptResult = false;
	setPartyMember(partyNum);

	if (!_redrawFlag && action == 6)
		error("TODO: action 6");
	if (_redrawFlag)
		_newPartyNum = g_engine->_disk1._party.getMemberByStatus(1);

	_scriptCurrP = _scriptP + 4;
	_scriptNextP = _scriptP + READ_LE_UINT16(_scriptP + 2);
	_state = kScriptRunning;

	do {
		if (_state == kScriptBreak || _scriptCurrP >= _scriptNextP) {
			_state = kScriptBreak;
			_val2 = 0;
			break;
		}

resumeScript:
		if (_state == kScriptPaused) {
			_params._isResuming = true;
			_state = kScriptRunning;
		} else {
			_params = readParams(_scriptCurrP, action);
			if (_params._action != action)
				continue;
		}

		int opcode = _params._opcode;
		if (opcode > kOpcodeLast) {
			warning("Out of range opcode");
			continue;
		}

		if (gDebugLevel > 0 && !_params._isResuming)
			dumpOpcode();

		(this->*OPCODE_FUNCTIONS[opcode])();
	} while (_state == kScriptRunning);

	return _scriptResult;
}

void Scripts::scriptDone() {

}

void Scripts::setPartyMember(int partyNum) {
	_partyMember = &g_engine->_disk1._party[partyNum];
}

Scripts::OpcodeParams Scripts::readParams(const byte *&scriptP, int action) {
	OpcodeParams result;
	int nextVal = *scriptP++;
	result._action = nextVal & 0xf;
	int paramsType = nextVal >> 4;

	if (!action) {
		result._opcode = (Opcode)*scriptP++;
		result._paramCount = 0;

		int *dest = result._params;
		// Iterate through loading parameters
		for (const byte *paramP = PARAMS[paramsType]; *paramP; ++paramP) {
			result._paramCount++;
			if (*paramP == BYTE) {
				*dest++ = *scriptP++;
			} else {
				*dest++ = READ_LE_INT16(scriptP);
				scriptP += 2;
			}
		}
	} else {
		// Skip
		static const byte ARRAY[47] = {
			4, 2, 1, 1, 3, 3, 2, 2, 0, 3, 2, 2, 4, 2, 0, 1,
			1, 3, 1, 4, 3, 2, 0, 1, 3, 2, 4, 2, 5, 7, 5, 1,
			2, 0, 3, 2, 2, 1, 0, 1, 0, 4, 2, 2, 2, 1, 0
		};

		int offset = ARRAY[*scriptP] + 1;
		scriptP += offset;	
	}

	return result;
}

void Scripts::dumpOpcode() {
	Common::String line = OPCODE_NAMES[_params._opcode] ?
		Common::String::format("%s(", OPCODE_NAMES[_params._opcode]) :
		Common::String::format("%d(", _params._opcode);

	if (_params._opcode == kOpcodeMessage) {
		line += Common::String::format("%s)", getMessage(_params[0]));
	} else {
		for (int i = 0; i < _params._paramCount; ++i) {
			if (i > 0)
				line += ", ";
			line += Common::String::format("%d", _params[i]);
		}
		line += ")";
	}

	debugC(kDebugScript, "%s", line.c_str());
}

const char *Scripts::getMessage(int msgNum) {
	uint msgOffset = READ_LE_UINT16(_scriptP + 2);
	const char *msg = (const char *)&_scriptP[msgOffset];
	for (int i = 0; i < msgNum; ++i)
		msg += strlen(msg) + 1;

	return msg;
}

void Scripts::opcode00_healOrDamageParty() {
	auto &disk1 = g_engine->_disk1;
	auto &party = disk1._party;
	party._ignoreMemberAC = _params._opcode == kOpcodeDamagePartyIgnoreAC;

	g_engine->_moveMessage = getMessage(_params[0]);
	if (!*g_engine->_moveMessage)
		g_engine->_moveMessage = nullptr;

	party._conDamaging = _params._opcode != kOpcodeHealParty;
	party._conMin = _params[1];
	party._conMax = _params[2];
	g_engine->_moveVal3 = _params[3];

	party.healOrDamageParty();
}

void Scripts::opcode01() { error("Unimplemented opcode"); }

void Scripts::opcode02() { error("Unimplemented opcode"); }

void Scripts::opcode03() {
	_scriptVal1 = _params[0];
	_scriptVal2 = 0;
}

void Scripts::opcode04() { error("Unimplemented opcode"); }

void Scripts::opcode05() { error("Unimplemented opcode"); }

void Scripts::opcode06() { error("Unimplemented opcode"); }

void Scripts::opcode07() { error("Unimplemented opcode"); }

void Scripts::opcode08() { error("Unimplemented opcode"); }

void Scripts::opcode09() { error("Unimplemented opcode"); }

void Scripts::opcode10() { error("Unimplemented opcode"); }

void Scripts::opcode11() { error("Unimplemented opcode"); }

void Scripts::opcode12_MoveParty() {
	auto &disk1 = g_engine->_disk1;
	auto &disk = g_engine->_disk;
	int isYes = true;
	int mapId;
	bool mapFlag = false;
	int mapIndex;

	if (_params._isResuming) {
		isYes = _resumeParams._intValue != 0;
	} else if (!_params[3]) {
		_state = kScriptPaused;
		Views::Dialogs::LeaveMap::show();
		return;
	}

	if (isYes) {
		switch (_params[2]) {
		case 0:
			if (disk1._mapIndex) {
				disk1.save();

				disk1._mapIndex--;
				disk.loadMap(disk1._maps[disk1._mapIndex]);
				disk1.moveTo(disk1._mapsX[disk1._mapIndex], disk1._mapsY[disk1._mapIndex]);
			}
			break;

		case 1:
			disk1.moveTo(_params[0], _params[1]);
			break;

		default:
			disk1.save();
			disk1._mapsX[disk1._mapIndex] = disk1._mapPosX;
			disk1._mapsY[disk1._mapIndex] = disk1._mapPosY;

			mapId = _params[2] - 1;
			mapFlag = false;
			for (mapIndex = 0; mapIndex <= disk1._mapIndex && !mapFlag; ++mapIndex) {
				mapFlag = disk1._maps[mapIndex] == mapId;
			}

			if (mapFlag) {
				disk1._mapIndex = mapIndex;
			} else {
				disk1._maps[++disk1._mapIndex] = mapId;
			}

			disk1._mapsX[disk1._mapIndex] = _params[0];
			disk1._mapsY[disk1._mapIndex] = _params[1];
			disk.loadMap(mapId);
			disk1.moveTo(disk1._mapsX[disk1._mapIndex], disk1._mapsY[disk1._mapIndex]);
			break;
		}
	}

	_val2 = 0;
	_scriptResult = true;
	_state = kScriptBreak;
}

void Scripts::opcode13() { error("Unimplemented opcode"); }

void Scripts::opcode14() { error("Unimplemented opcode"); }

void Scripts::opcode15() { error("Unimplemented opcode"); }

void Scripts::opcode16() { error("Unimplemented opcode"); }

void Scripts::opcode17() { error("Unimplemented opcode"); }

void Scripts::opcode18() { error("Unimplemented opcode"); }

void Scripts::opcode19() {
	int mapId;
	int zero = 0;

	if (_params[0] < 12) {
		mapId = (_params[0] < 6) ? _params[2] : _scriptVal3;

		switch (_params[0] % 6) {
		case 0:

		default:
			break;
		}
	} else if (_params[0] >= 15) {

	} else {

	}

	error("Unimplemented opcode");
}

void Scripts::opcode20() { error("Unimplemented opcode"); }

void Scripts::opcode21() { error("Unimplemented opcode"); }

void Scripts::opcode22() { error("Unimplemented opcode"); }

void Scripts::opcode23_Message() {
	const char *msg = getMessage(_params[0]);
	g_engine->send("Game", GameMessage("INFO", msg));
}

void Scripts::opcode24() { error("Unimplemented opcode"); }

void Scripts::opcode25() {
	g_engine->_disk._map._tiles[0][0].proc2(kOpcode25, _params[0], 0);

	error("Unimplemented opcode");
}

void Scripts::opcode26() {
	g_engine->_disk._map._tiles[0][0].proc2(kOpcode26, _params[0], _params[1]);
}

void Scripts::opcode27() {
	g_engine->_disk._map._tiles[0][0].proc2(kOpcode27, _params[0], 0);
}

void Scripts::opcode28_setTileOverride() {
	int val = (_params._opcode != kOpcode29) ? 0 : _params[4];
	g_engine->_disk1.setTileOverride(_params._opcode, _params[2], _params[0],
		_params[1], _params[3], val);
}

void Scripts::opcode31_Sound() {
	Sound::playSound(_params[0]);
}

void Scripts::opcode32() {
	_scriptVal1 = g_engine->getRandomNumber(_params[0], _params[1]);
	_scriptVal2 = 0;
}

void Scripts::opcode34() { error("Unimplemented opcode"); }

void Scripts::opcode35() { error("Unimplemented opcode"); }

void Scripts::opcode36() { error("Unimplemented opcode"); }

void Scripts::opcode37() { error("Unimplemented opcode"); }

void Scripts::opcode38() { error("Unimplemented opcode"); }

void Scripts::opcode39() { error("Unimplemented opcode"); }

void Scripts::opcode40() { error("Unimplemented opcode"); }

void Scripts::opcode42() { error("Unimplemented opcode"); }

void Scripts::opcode43() {
	const auto &party = g_engine->_disk1._party;
	bool flag = g_engine->_disk1._partyFlags[_params[0]] & Data::PARTYFLAG_1;

	if (flag) {
		int idx;
		for (idx = 0; idx < party._count && _params[0] != party[idx]._field49; ++idx) {
		}
		assert(idx < party._count);

		const auto &member = party[idx];
		flag = member._con > 0;
	}

	error("Unimplemented opcode");
}

void Scripts::opcode44() { error("Unimplemented opcode"); }

void Scripts::opcode45() { error("Unimplemented opcode"); }

void Scripts::opcode46() {
	const auto &disk1 = g_engine->_disk1;
	_scriptVal1 = disk1._timeHours * 100 + disk1._timeMinutes;
	_scriptVal2 = 0;
}

void Scripts::opcode47() { error("Unimplemented opcode"); }

void Scripts::opcode48() { error("Unimplemented opcode"); }

void Scripts::opcode49() { error("Unimplemented opcode"); }

void Scripts::opcode50() { error("Unimplemented opcode"); }

void Scripts::opcode52_MovePerson() {
	int charNum;
	Data::Map &map = g_engine->_disk._map;
	Data::Map::MapPerson *person = map.findPersonById(_params._params[0], &charNum);

	if (person && !(g_engine->_disk1._partyFlags[person->_id] & 2)) {
		if (_params._opcode == kOpcodeMovePerson) {
			// Remove character from old position
			map.updateTileForeground(person->_mapX, person->_mapY, 0);

			// Move them to new position
			person->_mapX = _params[1];
			person->_mapY = _params[2];
			map.updateTile(person->_diff, person->_mapX, person->_mapY, person->_charNum);
		}

		person->_oldX = -1;
		map.flagMap();

		if (_params[3] == 1 || _params._opcode == kOpcodeMovePerson2) {
			if (_params[3] == 1)
				person->_field50 |= 0x80;

			_idPtr = &person->_talkId;
			_actionNum = 6;
			_partyNum = _newPartyNum;
			_redrawFlag = false;

			_state = kScriptBreak;
		}
	}
}

void Scripts::opcode53() { error("Unimplemented opcode"); }

void Scripts::opcode54() { error("Unimplemented opcode"); }

void Scripts::opcode55() { error("Unimplemented opcode"); }

void Scripts::opcode56() { error("Unimplemented opcode"); }

void Scripts::opcode57_messageDialog() {
	_state = kScriptPaused;

	const char *msg = getMessage(_params[0]);
	g_engine->send("MessageDialog", GameMessage("SHOW", msg));
}

void Scripts::opcode58() { error("Unimplemented opcode"); }

void Scripts::opcode59() { error("Unimplemented opcode"); }

void Scripts::opcode60() {
	_scriptVal1 = g_engine->_disk1._timeWeekday;
	_scriptVal2 = 0;
}

void Scripts::opcode61() { error("Unimplemented opcode"); }

void Scripts::opcode62() { error("Unimplemented opcode"); }

void Scripts::opcode63() { error("Unimplemented opcode"); }

void Scripts::opcode64_setAffliction() {
	int partyNum = NONZERO(_params[0], _newPartyNum);
	auto &member = g_engine->_disk1._party[partyNum];
	member._afflicted |= 1 << _params[1];
}

void Scripts::opcode65() { error("Unimplemented opcode"); }

void Scripts::opcode66() { error("Unimplemented opcode"); }

void Scripts::opcode67() { error("Unimplemented opcode"); }

void Scripts::opcode68_End() {
	_state = kScriptBreak;
}

void Scripts::opcode69() { error("Unimplemented opcode"); }

void Scripts::opcode72() { error("Unimplemented opcode"); }

void Scripts::opcode73() { error("Unimplemented opcode"); }

void Scripts::opcode74() { error("Unimplemented opcode"); }

} // namespace Logic
} // namespace FOD
} // namespace Wasteland
