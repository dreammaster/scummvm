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

#ifndef AGS_DRAWABLE_H
#define AGS_DRAWABLE_H

#include "common/rect.h"

namespace Graphics {
	struct Surface;
}

namespace AGS {

class Drawable {
public:
	Drawable();
	virtual ~Drawable();

	// helper functions
	bool containsPoint(class AGSEngine *vm, Common::Point point);

	// position/zorder
	virtual Common::Point getDrawPos() = 0;
	virtual int getDrawOrder() const = 0;
	virtual bool priorityIfEqual() const { return true; }

	// surface to draw
	virtual const Graphics::Surface *getDrawSurface() = 0;

	// transformations
	virtual uint getDrawWidth() = 0;
	virtual uint getDrawHeight() = 0;
	virtual uint getDrawTransparency() = 0;
	virtual bool isDrawMirrored() = 0;
	virtual int getDrawLightLevel() = 0;
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) = 0;
};

} // End of namespace AGS

#endif // AGS_DRAWABLE_H
