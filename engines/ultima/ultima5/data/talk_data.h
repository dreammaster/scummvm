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

#ifndef ULTIMA5_DATA_TALK_DATA_H
#define ULTIMA5_DATA_TALK_DATA_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int TALK_WORD_COUNT = 128;
constexpr int TALK_KEYWORD_COUNT = 35;

// The most of what a person says, in the TLK file of the kind of location
constexpr int TALK_RECORD_SIZE = 0x400;

extern const char *const TALK_WORDS[TALK_WORD_COUNT];
extern const char *const TALK_KEYWORDS[TALK_KEYWORD_COUNT];

/**
 * Reads what a person of a location says from the TLK file of the kind of
 * location. The record is padded with zeroes if the file ends first
 * @param mapId		The location the person is in, from 1
 * @param dialog	Which of the people in the file it is
 * @param record	Filled with TALK_RECORD_SIZE bytes
 * @returns			False if the person isn't in the file
 */
bool loadTalkRecord(int mapId, int dialog, byte *record);

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
