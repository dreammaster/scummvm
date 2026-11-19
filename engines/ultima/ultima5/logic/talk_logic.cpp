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

#include "common/util.h"
#include "ultima/ultima5/logic/talk_logic.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

// The parts of what a person says, in the order they come in the script. The
// things they can be asked about follow, a word and what they say about it
constexpr int PART_NAME = 0;
constexpr int PART_DESCRIPTION = 1;
constexpr int PART_MET = 2;
constexpr int PART_JOB = 3;
constexpr int PART_BYE = 4;
constexpr int PART_FIRST_KEYWORD = 5;

constexpr int KEYWORD_NAME = 0;
constexpr int KEYWORD_JOB = 1;
constexpr int KEYWORD_WORK = 2;
constexpr int KEYWORD_BYE = 3;
constexpr int KEYWORD_THANK = 4;

constexpr byte MARK_END_OF_ENTRIES = 0x90;
constexpr byte MARK_END_OF_LABELS = 0x9F;
constexpr byte FIRST_LABEL = 0x91;

// Codes in the script
constexpr byte CODE_AVATAR = 0x81;
constexpr byte CODE_END = 0x82;
constexpr byte CODE_PAUSE = 0x83;
constexpr byte CODE_JOIN = 0x84;
constexpr byte CODE_GOLD = 0x85;
constexpr byte CODE_ITEM = 0x86;
constexpr byte CODE_OR = 0x87;
constexpr byte CODE_ASK_NAME = 0x88;
constexpr byte CODE_KARMA_UP = 0x89;
constexpr byte CODE_KARMA_DOWN = 0x8A;
constexpr byte CODE_CALL_GUARDS = 0x8B;
constexpr byte CODE_IF_MET = 0x8C;
constexpr byte CODE_NEWLINE = 0x8D;
constexpr byte CODE_RUNES = 0x8E;
constexpr byte CODE_WAIT = 0x8F;
constexpr byte CODE_IF_KARMA = 0xFE;
constexpr byte CODE_INTEREST = 0xFF;
constexpr byte NEWLINE_CHAR = 0x8A;
constexpr byte QUOTE = 0xA2;
constexpr byte SPACE = 0xA0;
constexpr byte NO_LABEL = 0xFF;

// Codes for what is changed in the party's belongings
constexpr byte FIRST_NON_ITEM = 0x40;

constexpr int WORD_BUFFER_SIZE = 0x10;
constexpr int LINE_WRAP = 0x12;
constexpr int LAST_COLUMN = 0xF;
constexpr int INTEREST_MAX = 15;
constexpr int RESPONSE_MAX = 15;
constexpr int MAX_PARTY = 6;
constexpr int FIRST_JOINER = 1;
constexpr int LAST_JOINER = 0xF;
constexpr int NAME_MATCH = 3;
constexpr int NAME_ASKED = 4;
constexpr int KARMA_MAX = 99;
constexpr int ITEM_MAX = 99;
constexpr int GOLD_MAX = 9999;
constexpr int GOLD_DIGIT_BASE = 0x14D0;

constexpr byte DIALOG_AFRAID = 0xFD;
constexpr byte DIALOG_VERMIN = 0xFE;
constexpr byte DIALOG_GUARD = 0xFF;
constexpr byte FIRST_MERCHANT = 0x80;
constexpr byte ACTOR_GUARD = 0x70;
constexpr byte ACTOR_BEGGAR = 0x6C;
constexpr byte SCHEDULE_FOLLOWS_NEAR = 4;
constexpr byte SCHEDULE_WANDERS_NEAR = 1;
constexpr int CHARITY_MAP = 5;
constexpr int BLACKTHORN_MAP = 0x12;
constexpr int TRIBUTE_PER_MEMBER = 10;
constexpr int BEGGAR_TURNS = 100;

TalkLogic::TalkLogic(Views::MessageLog &log) : _log(log) {
	memset(_script, 0, sizeof(_script));
	memset(_stack, 0, sizeof(_stack));
	memset(_word, 0, sizeof(_word));
	memset(_args, 0, sizeof(_args));
}

bool TalkLogic::skipOne(byte first, byte second) {
	while (_pos < Data::TALK_RECORD_SIZE) {
		const byte ch = _script[_pos++];
		if (ch == first)
			return true;
		if (ch == second)
			return false;
	}

	return false;
}

void TalkLogic::selectNth(int index) {
	_pos = 0;
	for (; index > 0; --index)
		skipOne(0, MARK_END_OF_ENTRIES);
}

void TalkLogic::startString(int pos, After after) {
	_pos = pos;
	_after = after;
	_stackSize = 0;
	_phase = PHASE_STRING;
}

void TalkLogic::startNth(int index, After after) {
	selectNth(index);
	startString(_pos, after);
}

bool TalkLogic::findLabel() {
	_pos = 0;
	while (_pos < Data::TALK_RECORD_SIZE && _script[_pos] != _label)
		skipOne(MARK_END_OF_ENTRIES, MARK_END_OF_LABELS);

	return _pos < Data::TALK_RECORD_SIZE;
}

int TalkLogic::findWord(const byte *needle, const Common::String &haystack) {
	const int needleLength = strlen((const char *)needle);
	const int haystackLength = haystack.size();

	if (needleLength > haystackLength)
		return -1;

	const int maxStart = haystackLength - needleLength;
	int pos = 0;
	int k;

	do {
		for (k = 0; k < needleLength; ++k) {
			int a = needle[k] & 0x7F;
			int b = (byte)haystack[pos + k] & 0x7F;

			if (b > 0x60)
				b &= 0x5F;
			if (a > 0x60)
				a &= 0x5F;
			if (a != b)
				break;
		}

		if (k == needleLength)
			return pos;

		pos += k + 1;
	} while (pos <= maxStart);

	return -1;
}

// True if what was typed has the word in it, as a word or the start of one
bool TalkLogic::matches(const byte *needle) const {
	const int pos = findWord(needle, _line);
	return pos != -1 && (pos == 0 || _line[pos - 1] == ' ');
}

bool TalkLogic::findCustomKeyword() {
	for (_keywordIndex = 0;; ++_keywordIndex) {
		_pos = 0;

		// Each thing to be asked about is a word then what's said
		for (int i = _keywordIndex * 2 + PART_FIRST_KEYWORD; i > 0; --i) {
			if (!skipOne(0, MARK_END_OF_ENTRIES))
				return false;
		}

		if (matches(&_script[_pos]))
			return true;
	}
}

bool TalkLogic::findLabelKeyword() {
	findLabel();
	skipOne(0, MARK_END_OF_LABELS);
	skipOne(0, MARK_END_OF_LABELS);

	for (;;) {
		if (_pos >= Data::TALK_RECORD_SIZE)
			return false;

		if (matches(&_script[_pos]))
			return true;

		if (!skipOne(0, MARK_END_OF_ENTRIES) || !skipOne(0, MARK_END_OF_ENTRIES))
			return false;
	}
}

void TalkLogic::printWord() {
	if (_wordLength == 0)
		return;

	for (int i = 0; i < _wordLength; ++i) {
		const byte ch = _word[i] & 0x7F;

		if (_log.column() != 0 || ch != ' ') {
			if (_lastColumn < LAST_COLUMN || ch != '\n') {
				if (ch != '\n')
					_lastColumn = _log.column();

				_log.printChar(ch);
			} else if (ch == '\n') {
				_lastColumn = 0;
			}
		}
	}

	_wordLength = 0;
}

void TalkLogic::buildOrPrint(byte ch) {
	if (_wordLength != WORD_BUFFER_SIZE) {
		_word[_wordLength++] = ch;

		if (ch != NEWLINE_CHAR && ch != SPACE)
			return;

		if (_log.column() + _wordLength >= LINE_WRAP)
			_log.printChar('\n');
	}

	printWord();
}

void TalkLogic::printQuote() {
	processChar(QUOTE);
}

void TalkLogic::printNewline() {
	processChar(CODE_NEWLINE);
}

bool TalkLogic::processChar(byte ch) {
	if (_pending != 0)
		return processArgument(ch);

	_argCount = 0;
	if (ch == QUOTE && _lastChar == QUOTE)
		return false;

	Data::Savegame &sg = _G(savegame);

	switch (ch) {
	case CODE_AVATAR:
		printAvatarName();
		break;

	case CODE_END:
		return true;

	case CODE_PAUSE:
		// The original waits a moment here
		return false;

	case CODE_JOIN:
		return join();

	case CODE_OR: {
		// What follows is another way of saying it, which is said then the rest carries on
		const int saved = _pos;

		++_pos;
		while (_pos < Data::TALK_RECORD_SIZE && _script[_pos] == 0)
			++_pos;
		while (_pos < Data::TALK_RECORD_SIZE && _script[_pos++] != 0) {
		}

		if (_stackSize < ARRAYSIZE(_stack))
			_stack[_stackSize++] = saved;
		return false;
	}

	case CODE_ASK_NAME:
		askName();
		return false;

	case CODE_KARMA_UP:
		sg._karma = MIN<int>(sg._karma + 1, KARMA_MAX);
		return false;

	case CODE_KARMA_DOWN:
		if (sg._karma > 0)
			--sg._karma;
		return false;

	case CODE_CALL_GUARDS:
		// The guards and others of the town turn hostile
		return false;

	case CODE_RUNES:
		_charMask ^= 0x80;
		return false;

	case CODE_INTEREST:
		printWord();
		_stackSize = 0;
		promptInterest();
		return false;

	case CODE_WAIT:
		_keyAction = KEY_RESUME;
		_phase = PHASE_KEY;
		return false;

	case CODE_IF_KARMA:
	case CODE_GOLD:
	case CODE_ITEM:
	case CODE_IF_MET:
		_pending = ch;
		return false;

	default:
		if (ch >= FIRST_LABEL && ch <= MARK_END_OF_LABELS) {
			_label = ch;
			startLabel();
			return false;
		}

		if (ch < CODE_AVATAR) {
			// A word that has a byte of its own
			buildOrPrint(SPACE);

			const char *word = Data::TALK_WORDS[ch - 1];
			for (; *word; ++word)
				buildOrPrint((byte)*word | 0x80);

			if (*Data::TALK_WORDS[ch - 1] == 0) {
				buildOrPrint(ch);
				return false;
			}

			_spaceNext = true;
		} else {
			if (ch == CODE_NEWLINE)
				ch = NEWLINE_CHAR;

			ch &= _charMask;
			_lastChar = ch;

			if (_spaceNext)
				buildOrPrint(SPACE);

			buildOrPrint(ch);
			_spaceNext = false;
		}
		break;
	}

	return false;
}

bool TalkLogic::processArgument(byte ch) {
	Data::Savegame &sg = _G(savegame);

	switch (_pending) {
	case CODE_GOLD:
		_args[_argCount++] = ch;
		if (_argCount != 3)
			return false;

		return payGold();

	case CODE_ITEM:
		addItem(ch & 0x7F);
		break;

	case CODE_IF_MET:
		_argCount = _pending = 0;

		if (!sg.isNpcMet(_npc))
			return false;

		_stackSize = 0;
		if (ch == NO_LABEL) {
			promptInterest();
		} else {
			_label = ch;
			startLabel();
		}
		return false;

	case CODE_IF_KARMA:
		_args[_argCount++] = ch;
		if (_argCount != 2)
			return false;

		_argCount = _pending = 0;
		if (sg._karma < _args[0])
			return false;

		_label = _args[1];
		startLabel();
		return false;

	default:
		break;
	}

	_argCount = _pending = 0;
	return false;
}

void TalkLogic::printAvatarName() {
	const Data::Savegame &sg = _G(savegame);

	for (const char *name = sg._party[0]._name; *name; ++name)
		processChar((byte)*name | 0x80);
}

bool TalkLogic::join() {
	Data::Savegame &sg = _G(savegame);
	const int saved = _pos;

	if (sg._partySize == MAX_PARTY) {
		_keyAction = KEY_JOIN_FULL;
		_phase = PHASE_KEY;
		return false;
	}

	selectNth(PART_NAME);

	// Who this is, from the first letters of their name
	byte name[NAME_MATCH + 1];
	memcpy(name, &_script[_pos], NAME_MATCH);
	name[NAME_MATCH] = 0;

	int member = LAST_JOINER;
	for (;;) {
		byte other[NAME_MATCH + 1];
		memcpy(other, sg._party[member]._name, NAME_MATCH);
		other[NAME_MATCH] = 0;

		bool same = true;
		for (int i = 0; i < NAME_MATCH; ++i) {
			if (toupper(name[i] & 0x7F) != toupper(other[i] & 0x7F)) {
				same = false;
				break;
			}
		}

		if (same)
			break;

		if (--member == 0) {
			buildOrPrint('"');
			buildOrPrint('\n');
			_log.print("\nSystem Error -\nNo Match!");
			_pos = saved;
			return true;
		}
	}

	sg._party[member]._mapId = 0;

	const Data::Character joiner = sg._party[member];
	sg._party[member] = sg._party[sg._partySize];
	sg._party[sg._partySize] = joiner;
	++sg._partySize;

	sg.setNpcKilled(_npc);
	NpcLogic::despawn(sg, _npc);

	printWord();
	_pos = saved;
	return true;
}

bool TalkLogic::payGold() {
	Data::Savegame &sg = _G(savegame);
	const int cost = (_args[0] & 0x7F) * 100 + (_args[1] & 0x7F) * 10 + (_args[2] & 0x7F) - GOLD_DIGIT_BASE;

	if (sg._gold >= cost) {
		sg._gold -= cost;

		// A beggar who's given to every so often is remembered for it
		const Data::NpcState state = sg.getNpcState(_npc);
		if ((sg.getActor(state._object)._tile & 0xFC) == ACTOR_BEGGAR && sg._turnCount >= BEGGAR_TURNS) {
			sg._turnCount = 0;
			sg._karma = MIN(sg._karma + 1, KARMA_MAX);

			if (sg._gold == 0)
				sg._karma = MIN(sg._karma + 2, KARMA_MAX);
		}

		_argCount = _pending = 0;
		return false;
	}

	_wordLength = 0;
	_log.printChar('"');
	_log.print("Thou hast not enough gold!");
	_log.printChar('"');
	_log.print("\n\n");

	_argCount = _pending = 0;
	_stackSize = 0;
	promptInterest();
	return false;
}

void TalkLogic::addItem(byte code) {
	Data::Savegame &sg = _G(savegame);

	if (code < FIRST_NON_ITEM) {
		sg._equipment[code] = MIN(sg._equipment[code] + 1, ITEM_MAX);
		return;
	}

	switch (code) {
	case 0x41:
		sg._food = MIN(sg._food + 1, GOLD_MAX);
		break;
	case 0x42:
		sg._gold = MIN(sg._gold + 1, GOLD_MAX);
		break;
	case 0x43:
		sg._keys = MIN(sg._keys + 1, ITEM_MAX);
		break;
	case 0x44:
		sg._gems = MIN(sg._gems + 1, ITEM_MAX);
		break;
	case 0x45:
		sg._torches = MIN(sg._torches + 1, ITEM_MAX);
		break;
	case 0x46:
		sg._grapple = MIN(sg._grapple + 1, ITEM_MAX);
		break;
	case 0x47:
		sg._carpets = MIN(sg._carpets + 1, ITEM_MAX);
		break;
	case 0x48:
		sg._sextant = 0xFF;
		break;
	case 0x49:
		sg._spyGlass = 0xFF;
		break;
	case 0x4A:
		sg._blackBadge = 0xFF;
		break;
	case 0x4B:
		sg._skullKeys = MIN(sg._skullKeys + 1, ITEM_MAX);
		break;
	default:
		break;
	}
}

void TalkLogic::askName() {
	printQuote();
	printWord();
	_log.print("What is thy name?\"\n");
	_log.print("\nYou respond-\n:");

	printWord();
	startLine(LINE_NAME, INTEREST_MAX);
}

void TalkLogic::nameEntered() {
	Data::Savegame &sg = _G(savegame);
	_pending = 0;
	_argCount = 0;
	_phase = PHASE_STRING;

	if (_line.empty()) {
		_log.print("\n\n\"If you say so...");
		return;
	}

	for (int i = 0; i < sg._partySize; ++i) {
		byte name[NAME_ASKED + 1];
		memcpy(name, sg._party[i]._name, NAME_ASKED);
		name[NAME_ASKED] = 0;

		if (matches(name)) {
			sg.setNpcMet(_npc);
			_log.print("\n\n\"A pleasure!");
			return;
		}
	}

	_log.print("\n\n\"If you say so...");
}

void TalkLogic::promptInterest() {
	_inLabel = false;
	_log.print("Your interest?\n:");
	printWord();
	startLine(LINE_INTEREST, INTEREST_MAX);
}

void TalkLogic::sayBye() {
	printQuote();
	startNth(PART_BYE, AFTER_BYE);
}

void TalkLogic::interestEntered() {
	if (_line.empty()) {
		_log.print("BYE\n\n");
		sayBye();
		return;
	}

	printNewline();
	printNewline();

	if (scanKeywords() == SCAN_HANDLED)
		return;

	if (!findCustomKeyword()) {
		_log.print("\"I cannot help thee with that.");
		printQuote();
		printNewline();
		printNewline();
		promptInterest();
	} else {
		printQuote();
		startNth(_keywordIndex * 2 + PART_FIRST_KEYWORD + 1, AFTER_ANSWER);
	}
}

void TalkLogic::startLabel() {
	_inLabel = true;
	printQuote();

	if (!findLabel()) {
		finish();
		return;
	}

	startString(_pos + 1, AFTER_LABEL_QUESTION);
}

void TalkLogic::promptResponse() {
	printNewline();
	printNewline();
	_inLabel = true;
	_log.print("You respond-\n:");
	printWord();
	startLine(LINE_RESPONSE, RESPONSE_MAX);
}

void TalkLogic::responseEntered() {
	if (_line.empty()) {
		_log.print("\n\n\"What didst thou say?");
		promptResponse();
		return;
	}

	printNewline();
	printNewline();

	if (scanKeywords() == SCAN_HANDLED)
		return;

	if (!findLabelKeyword()) {
		printQuote();
		findLabel();
		skipOne(0, MARK_END_OF_LABELS);
		startString(_pos, AFTER_LABEL_ANSWER);
	} else {
		printQuote();
		skipOne(0, MARK_END_OF_ENTRIES);
		startString(_pos, AFTER_LABEL_ANSWER);
	}
}

bool TalkLogic::builtinKeyword(int key) {
	switch (key) {
	case KEYWORD_NAME:
		if (_inLabel)
			return false;

		_log.print("\"My name is ");
		startNth(PART_NAME, AFTER_BUILTIN);
		return true;

	case KEYWORD_JOB:
	case KEYWORD_WORK:
		if (_inLabel)
			return false;

		printQuote();
		startNth(PART_JOB, AFTER_BUILTIN);
		return true;

	case KEYWORD_BYE:
	case KEYWORD_THANK:
		if (_inLabel)
			return false;

		sayBye();
		return true;

	default:
		// Anything rude gets the same answer
		_log.print("\"With language like that, how did you become an Avatar?");
		printQuote();
		printNewline();
		printNewline();

		if (_inLabel)
			startLabel();
		else
			promptInterest();
		return true;
	}
}

TalkLogic::Scan TalkLogic::scanKeywords() {
	for (int key = 0; key < Data::TALK_KEYWORD_COUNT; ++key) {
		if (matches((const byte *)Data::TALK_KEYWORDS[key]) && builtinKeyword(key))
			return SCAN_HANDLED;
	}

	return SCAN_NONE;
}

void TalkLogic::stringEnded() {
	if (_stackSize > 0) {
		_pos = _stack[--_stackSize];
		return;
	}

	switch (_after) {
	case AFTER_DESCRIPTION:
		printNewline();
		printNewline();

		if (!_G(savegame).isNpcMet(_npc)) {
			if (_G(gameRandom).range(0, 1) != 0) {
				_log.print("\"I am called ");
				startNth(PART_NAME, AFTER_CALLED);
			} else {
				promptInterest();
			}
		} else {
			printQuote();
			startNth(PART_MET, AFTER_MET);
		}
		break;

	case AFTER_CALLED:
	case AFTER_MET:
		printQuote();
		printNewline();
		printNewline();
		promptInterest();
		break;

	case AFTER_BUILTIN:
		printQuote();
		printNewline();
		printNewline();

		if (_inLabel)
			startLabel();
		else
			promptInterest();
		break;

	case AFTER_BYE:
		printQuote();
		printNewline();
		finish();
		break;

	case AFTER_ANSWER:
	case AFTER_LABEL_ANSWER:
		printQuote();
		printNewline();
		printNewline();
		promptInterest();
		break;

	case AFTER_LABEL_QUESTION:
		printQuote();
		promptResponse();
		break;
	}
}

void TalkLogic::run() {
	while (_phase == PHASE_STRING) {
		if (_pos >= Data::TALK_RECORD_SIZE) {
			finish();
			return;
		}

		const byte ch = _script[_pos];
		if (ch == 0) {
			stringEnded();
			continue;
		}

		++_pos;
		if (processChar(ch)) {
			finish();
			return;
		}
	}
}

void TalkLogic::startLine(LineKind kind, int maxLength) {
	_line.clear();
	_lineKind = kind;
	_lineMax = maxLength;
	_phase = PHASE_LINE;
}

void TalkLogic::stealSomething() {
	Data::Savegame &sg = _G(savegame);
	GameRandom &random = _G(gameRandom);

	_log.print("\nSomething was stolen!\n");

	if ((sg._keys | sg._gems | sg._torches) != 0) {
		byte *const things[] = { &sg._keys, &sg._gems, &sg._torches };

		for (;;) {
			byte &thing = *things[random.range(0, 2)];
			if (thing != 0) {
				--thing;
				return;
			}
		}
	}

	for (int i = ARRAYSIZE(sg._equipment) - 1; i >= 0; --i) {
		if (sg._equipment[i] != 0) {
			--sg._equipment[i];
			return;
		}
	}

	for (int i = ARRAYSIZE(sg._potions) - 1; i >= 0; --i) {
		if (sg._potions[i] != 0) {
			--sg._potions[i];
			return;
		}
	}

	for (int i = ARRAYSIZE(sg._scrolls) - 1; i >= 0; --i) {
		if (sg._scrolls[i] != 0) {
			--sg._scrolls[i];
			return;
		}
	}

	sg._gold = MAX(0, sg._gold - random.range(1, 15));
}

void TalkLogic::finish() {
	printWord();

	if (_steal && _G(savegame)._unk03B2 == 0)
		stealSomething();

	_phase = PHASE_DONE;
}

void TalkLogic::askToPay(Payment payment) {
	_payment = payment;
	_log.print("\n\nDost thou pay?\n\n:");
	_phase = PHASE_PAY;
}

bool TalkLogic::start(int npc) {
	Data::Savegame &sg = _G(savegame);
	_npc = npc;
	_steal = false;
	_shopDialog = 0;
	_phase = PHASE_DONE;

	_log.printChar('\n');

	const Data::NpcState state = sg.getNpcState(npc);
	Data::NpcSchedule schedule = sg.getSchedule(npc);

	if (schedule._type[state._slot] == SCHEDULE_FOLLOWS_NEAR) {
		schedule._type[state._slot] = SCHEDULE_WANDERS_NEAR;
		sg.setSchedule(npc, schedule);
	} else if (sg.getActor(state._object)._tile == ACTOR_GUARD && ((state._slot & 1) == 0 || state._dialog == 0)) {
		_log.print("The guard offers\nno response!\n");
		return true;
	}

	const int dialog = state._dialog;

	if (dialog == 0) {
		_log.print("No response!\n");
		return true;
	}

	if (dialog < FIRST_MERCHANT) {
		if (!Data::loadTalkRecord(sg._mapId, dialog, _script)) {
			_log.print("No response!\n");
			return true;
		}

		_steal = true;
		_log.print("You see ");
		startNth(PART_DESCRIPTION, AFTER_DESCRIPTION);
		run();
		return _phase == PHASE_DONE;
	}

	if (dialog == DIALOG_AFRAID) {
		_log.printChar('"');
		_log.print("Don't hurt me!\nPlease go away!");
		_log.printChar('"');
		_log.printChar('\n');
		return true;
	}

	if (dialog == DIALOG_VERMIN) {
		// The person turns on the party
		_log.print("\"Begone,\nvermin!\"\n");
		return true;
	}

	if (dialog == DIALOG_GUARD) {
		if (sg._mapId == CHARITY_MAP) {
			_log.printChar('"');
			_log.print("Thou wilt give\nhalf thy gold to\ncharity!");
			_log.printChar('"');
			askToPay(PAY_CHARITY);
			return false;
		}

		if (sg._mapId != BLACKTHORN_MAP) {
			int members = 0;
			for (int i = 0; i < sg._partySize; ++i) {
				if (sg._party[i]._status != Data::STATUS_DEAD)
					members += TRIBUTE_PER_MEMBER;
			}

			_tribute = members;
			_log.print("A guard demands\na ");
			_log.print(Common::String::format("%d", members));
			_log.print(" gp tribute\nto Blackthorn!");
			askToPay(PAY_TRIBUTE);
			return false;
		}

		return true;
	}

	// The people who keep shops are talked to at their counters
	if ((state._slot & 1) != 0 && (NpcLogic::scheduleSlot(schedule, sg._hour) & 1) != 0) {
		_shopDialog = dialog;
		return true;
	}

	_log.print("A merchant says:\n\"Come see me at\nmy shoppe, ");
	_log.print("when\nit's open!\"\n");
	return true;
}

bool TalkLogic::keypress(const Graphics::Views::KeypressMessage &msg) {
	Data::Savegame &sg = _G(savegame);

	switch (_phase) {
	case PHASE_KEY:
		if (_keyAction == KEY_JOIN_FULL) {
			_log.print("\"Thou hast no room for me in thy party! ");
			_log.print("Seek me again if one of thy members doth leave\nthee.");
		}

		_phase = PHASE_STRING;
		run();
		break;

	case PHASE_LINE:
		if ((msg.keycode == Common::KEYCODE_BACKSPACE || msg.keycode == Common::KEYCODE_LEFT) && !_line.empty()) {
			_log.backspace(1);
			_line.deleteLastChar();
		} else if (msg.keycode == Common::KEYCODE_ESCAPE && !_line.empty()) {
			_log.backspace(_line.size());
			_line.clear();
		} else if (msg.keycode == Common::KEYCODE_RETURN || msg.keycode == Common::KEYCODE_KP_ENTER) {
			switch (_lineKind) {
			case LINE_INTEREST:
				interestEntered();
				break;
			case LINE_RESPONSE:
				responseEntered();
				break;
			case LINE_NAME:
				nameEntered();
				break;
			}

			if (_phase == PHASE_STRING)
				run();
		} else if (msg.ascii > 0x1F && msg.ascii < 0x80 && (int)_line.size() < _lineMax) {
			_line += (char)msg.ascii;
			_log.printChar((char)msg.ascii);
		}
		break;

	case PHASE_PAY: {
		const int ch = toupper(msg.ascii);
		if (ch != 'Y' && ch != 'N')
			break;

		if (ch == 'Y') {
			_log.print("Yes\n");

			if (_payment == PAY_CHARITY) {
				sg._gold /= 2;
			} else if (sg._gold >= _tribute) {
				sg._gold -= _tribute;
			}
		} else {
			_log.print("No!\n");
		}

		// Whoever isn't paid should turn on the party
		_phase = PHASE_DONE;
		break;
	}

	default:
		break;
	}

	return _phase == PHASE_DONE;
}

} // namespace Ultima5
} // namespace Ultima
