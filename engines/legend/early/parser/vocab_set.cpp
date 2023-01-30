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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/early/parser/vocab_set.h"
#include "legend/early/engine.h"

namespace Legend {
namespace Early {
namespace Parser {

void VocabSet::clear() {
	_prefixVocabId = 0;
	_val2 = 0;
	_vocabCount = 0;
	Common::fill(&_vocabIds[0], &_vocabIds[16], 0);
	_suffixVocabId = 0;
}

void VocabSet::display() const {
	if (!_suffixVocabId)
		g_engine->_vocab->display(_prefixVocabId);

	for (int idx = 0; idx < _vocabCount; ++idx) {
		g_engine->_vocab->display(_vocabIds[idx]);
		g_engine->addText(" ");
	}

	g_engine->_vocab->display(_suffixVocabId);
}

void VocabSet::whichDoYouMean(const void *items) const {
	error("TODO");
	/*
	int val1 = 0, val2 = 0;
	int logicNum = 0;

	g_engine->addText("[Which ");
	display();
	g_engine->addText(" do you mean");

	for (int idx = 0; idx < *count || (logicNum && val1 > 1); ++idx) {
		if (idx < *count || g_engine->_logics->getBit())
	}
*/
	g_engine->addText("?]\n");
}

} // namespace Parser
} // namespace Early
} // namespace Legend
