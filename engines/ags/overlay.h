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

#ifndef AGS_OVERLAY_H
#define AGS_OVERLAY_H

#include "engines/ags/scriptobj.h"
#include "engines/ags/drawable.h"

#include "graphics/surface.h"

namespace AGS {

class AGSEngine;

class ScreenOverlay : public ScriptObject, public Drawable {
public:
	ScreenOverlay(AGSEngine *vm, const Common::Point &pos, uint type, const Graphics::Surface &surface, bool alphaChannel);
	~ScreenOverlay();

	bool isOfType(ScriptObjectType objectType) { return (objectType == sotOverlay); }
	const char *getObjectTypeName() { return "ScreenOverlay"; }

	uint getType() { return _type; }

	Common::Point _pos;
	uint _timeout;
	uint _bgSpeechForChar;
	bool _positionRelativeToScreen;

	virtual Common::Point getDrawPos();
	virtual int getDrawOrder() const;
	virtual const Graphics::Surface *getDrawSurface();
	virtual uint getDrawWidth();
	virtual uint getDrawHeight();
	virtual uint getDrawTransparency();
	virtual bool isDrawMirrored();
	virtual int getDrawLightLevel();
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue);

protected:
	AGSEngine *_vm;

	Graphics::Surface _surface;
	uint _type;
	bool _hasAlphaChannel;
};

} // End of namespace AGS

#endif // AGS_OVERLAY_H
