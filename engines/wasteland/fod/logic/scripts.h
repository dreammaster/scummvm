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

#ifndef WASTELAND_FOD_LOGIC_SCRIPTS_H
#define WASTELAND_FOD_LOGIC_SCRIPTS_H

#include "wasteland/fod/data/party.h"

namespace Wasteland {
namespace FOD {
namespace Logic {

typedef void (*ScriptDoneCallback)();

/**
 * Current script execution state
 */
enum ScriptState {
	kScriptIdle = 0, kScriptBreak = 0, kScriptRunning = 1, kScriptPaused = 2
};

enum Opcode {
	kOpcodeHealParty = 0,
	kOpcodeMoveParty = 12,
	kOpcodeMessage = 23,
	kOpcode25 = 25,
	kOpcode26 = 26,
	kOpcode27 = 27,
	kOpcode28 = 28,
	kOpcode29 = 29,
	kOpcode30 = 30,
	kOpcodeSound = 31,
	kOpcodeNOP1 = 33,
	kOpcodeDamageParty = 41,
	kOpcodeNOP2 = 51,
	kOpcodeMovePerson = 52,
	kOpcodeMessageDialog = 57,
	kOpcodeSetAffliction = 64,
	kOpcode_End = 68,
	kOpcodeDamagePartyIgnoreAC = 71,
	kOpcodeMovePerson2 = 75,

	kOpcodeLast = 75
};


/**
 * Container for values that can be passed into the script when resuming.
 * In the original, a script could show a view mid-execution, such as
 * prompting the user for Y/N when exiting a map. Now in ScummVM, the
 * script can pause itself to show the Y/N view, which then calls a
 * callback when a key is pressed, which in turn passes the Y/N selection
 * back into the script to resume execution with.
 */
struct ScriptResumeParams {
	int _intValue = 0;

	ScriptResumeParams() {}
	ScriptResumeParams(int value) : _intValue(value) {}
};

class Scripts {
	struct OpcodeParams {
		byte _action = 0;
		Opcode _opcode = (Opcode)0;
		int _params[5];
		int _paramCount = 0;
		bool _isResuming = false;

		const int &operator[](uint i) const {
			return _params[i];
		}
	};
	typedef void(Scripts:: *OpcodeFunction)();
private:
	static const OpcodeFunction OPCODE_FUNCTIONS[76];
	ScriptResumeParams _resumeParams;
	ScriptState _state = kScriptIdle;
	ScriptDoneCallback _doneCallback = nullptr;
	const uint16 *_idPtr = nullptr;
	int _actionNum = 0;
	int _charNum = 0;
	int _partyNum = 0;
	int _newPartyNum = 0;
	bool _redrawFlag = false;
	int _val2 = 0;
	bool _scriptResult = false;
	Data::PartyMember *_partyMember = nullptr;
	const byte *_scriptP = nullptr;
	const byte *_scriptCurrP = nullptr;
	const byte *_scriptNextP = nullptr;
	OpcodeParams _params;
	int _scriptVal1 = 0;
	int _scriptVal2 = 0;
	int _scriptVal3 = 0;

	void executeLoop();
	bool executeScript();
	void scriptDone();
	void setPartyMember(int partyNum);
	OpcodeParams readParams(const byte *&scriptP, int action);
	void dumpOpcode();
	const char *getMessage(int msgNum);

public:
	void execute(const uint16 *idPtr, int action, int arg2,
		ScriptDoneCallback doneCallback);

	void resume(const ScriptResumeParams &params);

private:
	void opcodeNOP() {}
	void opcode00_healOrDamageParty();
	void opcode01();
	void opcode02();
	void opcode03();
	void opcode04();
	void opcode05();
	void opcode06();
	void opcode07();
	void opcode08();
	void opcode09();
	void opcode10();
	void opcode11();
	void opcode12_MoveParty();
	void opcode13();
	void opcode14();
	void opcode15();
	void opcode16();
	void opcode17();
	void opcode18();
	void opcode19();
	void opcode20();
	void opcode21();
	void opcode22();
	void opcode23_Message();
	void opcode24();
	void opcode25();
	void opcode26();
	void opcode27();
	void opcode28_setTileOverride();
	void opcode31_Sound();
	void opcode32();
	void opcode34();
	void opcode35();
	void opcode36();
	void opcode37();
	void opcode38();
	void opcode39();
	void opcode40();
	void opcode42();
	void opcode43();
	void opcode44();
	void opcode45();
	void opcode46();
	void opcode47();
	void opcode48();
	void opcode49();
	void opcode50();
	void opcode52_MovePerson();
	void opcode53();
	void opcode54();
	void opcode55();
	void opcode56();
	void opcode57_messageDialog();
	void opcode58();
	void opcode59();
	void opcode60();
	void opcode61();
	void opcode62();
	void opcode63();
	void opcode64_setAffliction();
	void opcode65();
	void opcode66();
	void opcode67();
	void opcode68_End();
	void opcode69();
	void opcode72();
	void opcode73();
	void opcode74();
};

} // namespace Logic
} // namespace FOD
} // namespace Wasteland

#endif
