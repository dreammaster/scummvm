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

#ifndef WASTELAND_WASTELAND1_VARS_H
#define WASTELAND_WASTELAND1_VARS_H

#include "wasteland/wasteland1/data/saved.h"
#include "wasteland/wasteland1/gfx/pics.h"

namespace Wasteland {
namespace Wasteland1 {

struct Vars {
public:
	Data::Saved _saved;
	Gfx::PicsDecoder _pics;
	Data::CurrentParty _party;
	Data::PartyMember *&_currentChar;
	Data::ItemDetailsArray _itemDetails;

public:
	Vars();
	~Vars();
	void load();
};

extern Vars *g_vars;

} // namespace Wasteland1
} // namespace Wasteland

#endif
