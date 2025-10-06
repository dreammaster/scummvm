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

#ifndef SPYCRAFT_GAME_PRINT_H
#define SPYCRAFT_GAME_PRINT_H

#include "spycraft/game/made.h"
#include "spycraft/game/object.h"

namespace Spycraft {

class Print : public Object {
public:
	int16 font = 0;
	int16 size = 0;
	styleField style = 0;
	int32 fore_color = 0;
	int32 back_color = 0;
	int32 high_color = 0;
	int height = 0;
	int width = 0;
	int x = 0;
	int y = 0;
	char *myString = nullptr;
	int priority = 0;
	rsrcID textNum = 0;
	bufferNum textBuffer = 0;

	Print();
	bool init(const char *format);
	bool init(void);
	bool highlight(bool hiOrLow);
	virtual void doit(void);
	void erase(void);
	void addToPic(void);
	void dispose(void);
};

} // namespace Spycraft

#endif
