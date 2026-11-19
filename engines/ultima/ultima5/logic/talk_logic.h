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

#ifndef ULTIMA5_LOGIC_TALK_LOGIC_H
#define ULTIMA5_LOGIC_TALK_LOGIC_H

#include "common/str.h"
#include "ultima/shared/engine/events.h"
#include "ultima/ultima5/data/talk_data.h"
#include "ultima/ultima5/views/message_log.h"
#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {

/**
 * A conversation with one of the people of a location. What a person says is a
 * script of text with a few codes for things that happen, which is run until
 * it needs a key or a line typed, then run on once the player has given it
 */
class TalkLogic {
private:
	enum Phase {
		PHASE_STRING,	// Running the text of the script
		PHASE_KEY,		// Waiting for any key
		PHASE_LINE,		// Reading a line
		PHASE_PAY,		// Waiting for yes or no
		PHASE_DONE
	};

	// What is done when the text being run comes to its end
	enum After {
		AFTER_DESCRIPTION,
		AFTER_CALLED,
		AFTER_MET,
		AFTER_BUILTIN,
		AFTER_BYE,
		AFTER_ANSWER,
		AFTER_LABEL_QUESTION,
		AFTER_LABEL_ANSWER
	};

	// What a line that's been read is for
	enum LineKind {
		LINE_INTEREST,
		LINE_RESPONSE,
		LINE_NAME
	};

	// What is done once a key has been pressed
	enum KeyAction {
		KEY_RESUME,
		KEY_JOIN_FULL
	};

	// What is paid for
	enum Payment {
		PAY_CHARITY,
		PAY_TRIBUTE
	};

	Views::MessageLog &_log;
	byte _script[Data::TALK_RECORD_SIZE + 8];
	int _pos = 0;
	int _npc = 0;
	int _stack[8];
	int _stackSize = 0;

	// Words are gathered up before they're printed, so lines can be broken between them
	byte _word[0x10];
	int _wordLength = 0;
	int _lastColumn = 0;
	byte _lastChar = 0;
	bool _spaceNext = false;
	byte _charMask = 0xFF;

	// The arguments of a code that takes some
	byte _pending = 0;
	int _argCount = 0;
	byte _args[3];

	Phase _phase = PHASE_DONE;
	After _after = AFTER_DESCRIPTION;
	LineKind _lineKind = LINE_INTEREST;
	KeyAction _keyAction = KEY_RESUME;
	Payment _payment = PAY_CHARITY;
	int _tribute = 0;
	bool _steal = false;
	int _shopDialog = 0;
	bool _inLabel = false;
	byte _label = 0;
	int _keywordIndex = 0;
	int _lineMax = 0;
	Common::String _line;

	/**
	 * Takes the script apart
	 */
	bool skipOne(byte first, byte second);
	void selectNth(int index);
	void startString(int pos, After after);
	void startNth(int index, After after);
	void stringEnded();
	bool findLabel();
	bool findCustomKeyword();
	bool findLabelKeyword();
	bool matches(const byte *needle) const;

	/**
	 * Printing text as it's said
	 */
	void printWord();
	void buildOrPrint(byte ch);
	void printQuote();
	void printNewline();
	bool processChar(byte ch);
	bool processArgument(byte ch);

	/**
	 * Things that the script can do
	 */
	void printAvatarName();
	bool join();
	bool payGold();
	void addItem(byte code);
	void askName();
	void nameEntered();

	/**
	 * The talk, from asking for things to be told about to the words that end it
	 */
	void promptInterest();
	void interestEntered();
	void promptResponse();
	void responseEntered();
	void startLabel();
	void sayBye();
	void answerFound(After after);
	bool builtinKeyword(int key);

	enum Scan {
		SCAN_NONE,
		SCAN_HANDLED,
		SCAN_END
	};

	Scan scanKeywords();

	void startLine(LineKind kind, int maxLength);
	void finish();
	void run();
	void stealSomething();
	void askToPay(Payment payment);

public:
	/**
	 * Finds the first place in what was typed with the start of a word in it that
	 * matches, ignoring case, or -1
	 */
	static int findWord(const byte *needle, const Common::String &haystack);

	TalkLogic(Views::MessageLog &log);

	/**
	 * Starts speaking to a person of the location the party is in
	 * @returns		True if there was nothing more to it than that
	 */
	bool start(int npc);

	/**
	 * Returns what a shopkeeper that wants to do business says, from 0x81, or zero
	 * if the person isn't one
	 */
	int shopDialog() const {
		return _shopDialog;
	}

	/**
	 * Handles a keypress
	 * @returns		True once the talk is finished
	 */
	bool keypress(const Graphics::Views::KeypressMessage &msg);
};

} // namespace Ultima5
} // namespace Ultima

#endif
