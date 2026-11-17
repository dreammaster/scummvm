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

#include "common/algorithm.h"
#include "common/util.h"
#include "ultima/ultima5/logic/creation_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int ROUND_ONE_QUESTIONS = 4;
constexpr int ROUND_TWO_QUESTIONS = 2;
constexpr int MIN_STRENGTH = 0x14;

// What choosing each virtue adds to the attributes
static const byte INT_BONUS[VIRTUE_COUNT] = { 2, 0, 0, 1, 0, 1, 1, 0 };
static const byte DEX_BONUS[VIRTUE_COUNT] = { 0, 2, 0, 1, 1, 0, 1, 0 };
static const byte STR_BONUS[VIRTUE_COUNT] = { 0, 0, 2, 0, 1, 1, 1, 0 };

// The text of the question for each pair of virtues
static const uint16 TEXT_OFFSETS[VIRTUE_COUNT][VIRTUE_COUNT] = {
	{ 0x0000, 0x06C2, 0x07A7, 0x088C, 0x0960, 0x0A47, 0x0B2D, 0x0C11 },
	{ 0x06C2, 0x0000, 0x0CDA, 0x0D8F, 0x0E77, 0x0F56, 0x1039, 0x111B },
	{ 0x07A7, 0x0CDA, 0x0000, 0x11FF, 0x12DB, 0x1374, 0x1456, 0x14ED },
	{ 0x088C, 0x0D8F, 0x11FF, 0x0000, 0x15D1, 0x16AF, 0x1794, 0x185B },
	{ 0x0960, 0x0E77, 0x12DB, 0x15D1, 0x0000, 0x192F, 0x1A18, 0x1B00 },
	{ 0x0A47, 0x0F56, 0x1374, 0x16AF, 0x192F, 0x0000, 0x1BE3, 0x1CBB },
	{ 0x0B2D, 0x1039, 0x1456, 0x1794, 0x1A18, 0x1BE3, 0x0000, 0x1D87 },
	{ 0x0C11, 0x111B, 0x14ED, 0x185B, 0x1B00, 0x1CBB, 0x1D87, 0x0000 }
};

void CreationLogic::start(const Data::Character &base) {
	Common::fill(_picked, _picked + VIRTUE_COUNT, false);
	Common::fill(_rejected, _rejected + VIRTUE_COUNT, false);

	_int = base._int;
	_dex = base._dex;
	_str = base._str;
	_asked = 0;
}

int CreationLogic::pickVirtue() {
	int virtue;
	do {
		virtue = g_engine->getRandomNumber(VIRTUE_COUNT - 1);
	} while (_picked[virtue] || _rejected[virtue]);

	_picked[virtue] = true;
	return virtue;
}

GypsyQuestion CreationLogic::nextQuestion() {
	// Virtues that lost in earlier rounds stay out, and the others can be picked again
	// at the start of each round
	if (_asked == ROUND_ONE_QUESTIONS || _asked == ROUND_ONE_QUESTIONS + ROUND_TWO_QUESTIONS)
		Common::fill(_picked, _picked + VIRTUE_COUNT, false);

	const int first = pickVirtue();
	const int second = pickVirtue();

	GypsyQuestion question;
	question._left = MIN(first, second);
	question._right = MAX(first, second);
	question._textOffset = TEXT_OFFSETS[question._left][question._right];
	return question;
}

void CreationLogic::answer(const GypsyQuestion &question, bool leftCard) {
	const int chosen = leftCard ? question._left : question._right;
	const int rejected = leftCard ? question._right : question._left;

	_int += INT_BONUS[chosen];
	_dex += DEX_BONUS[chosen];
	_str += STR_BONUS[chosen];
	_rejected[rejected] = true;
	++_asked;
}

void CreationLogic::finish(Data::Character &character) const {
	character._int = character._mag = _int;
	character._dex = _dex;
	character._str = MAX(_str, MIN_STRENGTH);
}

} // namespace Ultima5
} // namespace Ultima
