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

#ifndef WASTELAND_WASTELAND1_DATA_TEXT_H
#define WASTELAND_WASTELAND1_DATA_TEXT_H

#include "wasteland/core/serializer.h"
#include "wasteland/wasteland1/core/array1.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Data {

extern const char *const SKILL_NAMES[];
extern const char *const ITEM_NAMES[];
extern const char *const NATIONALITIES[];
extern const char *const SEX[];

/**
 * In some cases, item names are encoded with three \n characters,
 * indicating the suffixes to use for singular vs plural amounts.
 */
extern Common::String getItemText(uint index, bool isPlural = false);

// TODO: Remove me
extern const char *const ORIG_TEXT_STRINGS[168];

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland

#endif
