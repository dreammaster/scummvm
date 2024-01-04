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

#include "wasteland/fod/data/pics.h"
#include "common/file.h"

namespace Wasteland {
namespace FOD {
namespace Data {

Pics::~Pics() {
	delete[] _borders;
}

bool Pics::load() {
	Common::File borders;
	if (!borders.open("BORDERS"))
		return false;

	// Load data for drawing on-screen borders
	borders.seek(5000);
	_borders = new byte[TEXT_W * TEXT_H];
	borders.read(_borders, TEXT_W * TEXT_H);
	borders.close();

	return true;
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
