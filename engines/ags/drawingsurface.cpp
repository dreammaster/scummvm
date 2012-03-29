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

#include "engines/ags/drawingsurface.h"
#include "engines/ags/ags.h"
#include "engines/ags/constants.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/room.h"

#include "graphics/surface.h"

namespace AGS {

DrawingSurface::DrawingSurface(AGSEngine *vm) : _vm(vm), _type(dstInvalid), _modified(false),
	_useHighResCoordinates(false), _currentColor(vm->_state->_rawColor) {

	if (_vm->getGameOption(OPT_NATIVECOORDINATES) && (_vm->_gameFile->_defaultResolution > 2))
		_useHighResCoordinates = true;
}

DrawingSurface::~DrawingSurface() {
	release();
}

void DrawingSurface::release() {
	switch (_type) {
	case dstInvalid:
		// already released, do nothing
		break;
	case dstRoomBackground:
		if (!_modified)
			break;
		// TODO: mark as modified for saving
		if (_id == _vm->_state->_bgFrame)
			_vm->invalidateBackground();
		break;
	case dstDynamicSprite:
		// TODO: release sprite, update any caches..?
		break;
	case dstDynamicSurface:
		_surface->free();
		delete _surface;
		break;
	}

	_type = dstInvalid;
}

Graphics::Surface *DrawingSurface::startDrawing() {
	switch (_type) {
	case dstInvalid:
		error("attempt to use DrawingSurface after it was released");
	case dstRoomBackground:
		// TODO: sanity check this somewhere
		return &_vm->getCurrentRoom()->_backgroundScenes[_id]._scene;
	case dstDynamicSprite:
		// FIXME
		error("DynamicSprite not supported yet");
	case dstDynamicSurface:
		return _surface;
	default:
		error("DrawingSurface::startDrawing: internal error");
	}
}

void DrawingSurface::finishedDrawing(bool readOnly) {
	// TODO

	if (readOnly)
		return;

	_modified = true;

	// TODO: mark as workaround for Gemini Rue
	if (_type == dstRoomBackground)
		if (_id == _vm->_state->_bgFrame)
			_vm->invalidateBackground();
}

} // End of namespace AGS
