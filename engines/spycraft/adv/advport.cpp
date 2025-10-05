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

#include "spycraft/adv/advlib.h"
#include "spycraft/adv/advport.h"

namespace Spycraft {

Viewport *AllocPort(int width, int height, int colors) {
	Viewport *port;
	int rowBytes;

	port = (Viewport *)AllocPtr(sizeof(Viewport));
	if (port == NULL)
		return NULL;

	rowBytes = width;
	if (colors == 16)
		rowBytes *= 2;
	port->width = width;
	port->height = height;
	port->colors = colors;
	port->rowBytes = (uint16)rowBytes;
	port->origX = 0;
	port->origY = 0;
	port->ptr = AllocPtr(rowBytes * height);
	if (port->ptr == NULL) {
		FreePtr(port);
		return NULL;
	}

	return port;
}

void FreePort(Viewport *port) {
	if (port->ptr != NULL)
		FreePtr(port->ptr);

	FreePtr(port);
}

} // namespace Spycraft
