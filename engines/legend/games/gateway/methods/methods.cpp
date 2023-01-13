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

#include "legend/games/gateway/methods/methods.h"

namespace Legend {
namespace Early {
namespace Gateway {
namespace Methods {

#define INIT10(PREFIX) \
	&_method##PREFIX##0, &_method##PREFIX##1, &_method##PREFIX##2, &_method##PREFIX##3, &_method##PREFIX##4, &_method##PREFIX##5, \
	&_method##PREFIX##6, &_method##PREFIX##7, &_method##PREFIX##8, &_method##PREFIX##9
#define INIT100(PREFIX) \
	INIT10(PREFIX##0), INIT10(PREFIX##1), INIT10(PREFIX##2), \
	INIT10(PREFIX##3), INIT10(PREFIX##4), INIT10(PREFIX##5), \
	INIT10(PREFIX##6), INIT10(PREFIX##7), INIT10(PREFIX##8), \
	INIT10(PREFIX##9)

#define INIT001 \
	&_method001, &_method002, &_method003, &_method004, &_method005, \
	&_method006, &_method007, &_method008, &_method009
#define INIT001_100 \
	INIT001, INIT10(01), INIT10(02), INIT10(03), INIT10(04), INIT10(05), \
	INIT10(06), INIT10(07), INIT10(08), INIT10(09)

Methods::Methods() : _METHODS{
	INIT001_100, INIT100(1), INIT100(2), INIT100(3), INIT100(4),
	INIT100(5), INIT100(6),
	INIT10(70), INIT10(71), INIT10(72),
	&_method730, &_method731, &_method732, &_method733, &_method734
} {
}

int Methods::call(MethodsEnum index, int param) {
	if (index == 0 || index > METHODS_COUNT)
		// No method specified
		return 0;

	// Call the appropriate method
	return _METHODS[index]->call(param);
}

} // namespace Methods
} // namespace Gateway
} // namespace Early
} // namespace Legend
