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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 * You may also modify/distribute the code in this file under that license.
 */

#ifndef AGS_DRAWINGSURFACE_H
#define AGS_DRAWINGSURFACE_H

#include "engines/ags/scriptobj.h"

namespace Graphics {
	struct Surface;
}

namespace AGS {

enum DrawingSurfaceType {
	dstInvalid,		// e.g. after Release() was called
	dstRoomBackground,	// _id is the background id
	dstDynamicSprite,	// _id is the sprite id
	dstDynamicSurface	// _surface is our surface
};

class AGSEngine;

class DrawingSurface : public ScriptObject {
public:
	DrawingSurface(AGSEngine *vm);
	~DrawingSurface();

	bool isOfType(ScriptObjectType objectType) { return (objectType == sotDrawingSurface); }
	const char *getObjectTypeName() { return "DrawingSurface"; }

	void release();

	Graphics::Surface *startDrawing();
	void finishedDrawing(bool readOnly = false);

	DrawingSurfaceType _type;
	union {
		uint _id;
		Graphics::Surface *_surface;
	};

	bool _modified;
	bool _useHighResCoordinates;
	uint32 _currentColor;

protected:
	AGSEngine *_vm;
};

} // End of namespace AGS

#endif // AGS_DRAWINGSURFACE_H
