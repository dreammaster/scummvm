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

#include "engines/ags/ags.h"
#include "engines/ags/constants.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"
#include "engines/ags/room.h"
#include "engines/ags/script.h"
#include "engines/ags/sprites.h"
#include "engines/ags/util.h"

namespace AGS {

static void decompressLZSS(Common::SeekableReadStream *stream, byte *outBuf, uint32 destSize) {
	const uint32 N = 4096; // history buffer size (2^12)
	const uint32 F = 16;
	const uint32 THRESHOLD = 3; // base threshold for encoded strings

	byte buffer[N];

	uint32 bufPos = N - F;
	uint32 outPos = 0;

	while (outPos < destSize && !stream->eos()) {
		byte flagBits = stream->readByte();

		// 8 bits
		for (uint32 i = 1; (i <= 8) && (outPos < destSize); i++) {
			if ((flagBits & 1) == 1) {
				// string from history buffer
				uint16 data = stream->readUint16LE();

				uint32 length = ((data >> 12) & 0xF) + THRESHOLD;
				uint32 inPos = (bufPos - data - 1) & (N - 1);

				while ((outPos < destSize) && length--) {
					byte val = buffer[inPos];
					outBuf[outPos++] = val;
					buffer[bufPos] = val;
					inPos = (inPos + 1) & (N - 1);
					bufPos = (bufPos + 1) & (N - 1);
				}
			} else {
				// byte (to add to history buffer)
				byte val = stream->readByte();
				outBuf[outPos++] = val;
				buffer[bufPos] = val;
				bufPos = (bufPos + 1) & (N - 1);
			}

			// next bit
			flagBits = flagBits >> 1;
		}
	}
}

static Graphics::Surface readLZSSImage(Common::SeekableReadStream *stream, Graphics::PixelFormat format, byte *destPalette) {
	Graphics::Surface surf;

	stream->read(destPalette, 256 * 4);

	uint32 uncompressedSize = stream->readUint32LE();
	uint32 compressedSize = stream->readUint32LE();
	if (uncompressedSize < 8)
		error("readLZSSImage: %d uncompressed bytes is insufficient (%d compressed)", uncompressedSize, compressedSize);

	byte *buffer = new byte[uncompressedSize];
	uint32 oldPos = stream->pos();
	decompressLZSS(stream, buffer, uncompressedSize);
	if (stream->eos() || (uint32)stream->pos() != oldPos + compressedSize)
		error("readLZSSImage: failed to read %d compressed bytes of image (started at %d, got to %d, should be %d bytes)",
			compressedSize, oldPos, stream->pos(), uncompressedSize);

	uint32 widthBytes = READ_LE_UINT32(buffer);
	uint32 height = READ_LE_UINT32(buffer + 4);
	if (widthBytes * height != uncompressedSize - 8)
		error("readLZSSImage: %d bytes width and %d height doesn't match %d size", widthBytes, height, uncompressedSize);
	if (widthBytes % format.bytesPerPixel != 0)
		error("readLZSSImage: %d bytes width doesn't divide into %d bpp", widthBytes, format.bytesPerPixel);
	surf.create(widthBytes / format.bytesPerPixel, height, format);

	// FIXME: convert to correct format
	memcpy(surf.getPixels(), buffer + 8, uncompressedSize - 8);

	delete[] buffer;
	return surf;
}

static Graphics::Surface readRLEImage(Common::SeekableReadStream *stream) {
	Graphics::Surface surf;

	uint16 width = stream->readUint16LE();
	uint16 height = stream->readUint16LE();
	surf.create(width, height, Graphics::PixelFormat::createFormatCLUT8());

	for (uint i = 0; i < height; ++i)
		unpackSpriteBits(stream, (byte *)surf.getBasePtr(0, i), width);

	stream->skip(256 * 3); // skip palette

	return surf;
}

void RoomObject::setVisible(bool visible) {
	if (visible == _visible)
		return;

	// can't change visible state of merged objects
	if (_merged)
		return;

	_visible = visible;
	if (!_visible)
		stopMoving();
}

void RoomObject::setObjectFrame(uint viewId, int loopId, int frameId) {
	if (viewId < 1 || viewId > _vm->_gameFile->_views.size())
		error("RoomObject::setObjectFrame: invalid view id %d (only have %d)", viewId, _vm->_gameFile->_views.size());
	_view = viewId - 1;
	// TODO: check loopId too?

	if (loopId >= 0)
		_loop = loopId;
	if (_loop >= _vm->_gameFile->_views[_view]._loops.size())
		_loop = _vm->_gameFile->_views[_view]._loops.size() - 1;

	ViewLoopNew *loopObj = _vm->getViewLoop(_view, _loop);
	if (frameId >= 0)
		_frame = frameId;
	if (_frame >= loopObj->_frames.size())
		_frame = loopObj->_frames.size() - 1;

	if (_vm->getGameFileVersion() > kAGSVer272) {
		// Skip check on 2.x
		if (loopObj->_frames.empty())
			error("RoomObject::setObjectFrame: loop %d of view %d is empty", _loop, _view);
	}

	_cycling = 0;
	_spriteId = loopObj->_frames[_frame]._pic;
	_vm->checkViewFrame(_view, _loop, _frame);
}

void RoomObject::animate(uint loopId, uint speed, uint repeat, uint direction) {
	if (_view == (uint16)-1)
		error("RoomObject::animate: object has not been assigned a view");
	if (loopId >= _vm->_gameFile->_views[_view]._loops.size())
		error("RoomObject::animate: loop %d is too high (view %d only has %d loops)",
			loopId, _view, _vm->_gameFile->_views[_view]._loops.size());
	if (direction > 1)
		error("RoomObject::animate: invalid direction %d", direction);
	if (repeat > 2)
		error("RoomObject::animate: invalid repeat value %d", repeat);
	if (_vm->getViewLoop(_view, loopId)->_frames.empty())
		error("RoomObject::animate: no frames in loop %d of view %d", _view, loopId);

	_cycling = repeat + 1 + (direction * 10);
	_loop = loopId;
	ViewLoopNew *loop = _vm->getViewLoop(_view, _loop);

	if (direction == 0) {
		_frame = 0;
	} else {
		_frame = loop->_frames.size() - 1;
	}

	_overallSpeed = speed;
	_wait = speed + loop->_frames[_frame]._speed;
	_spriteId = loop->_frames[_frame]._pic;

	_vm->checkViewFrame(_view, _loop, _frame);
}

void RoomObject::update() {
	if (!_visible)
		return;

	// do we need to move?
	/* FIXME: if (_moving)
		runMoveList(); */

	// do we need to animate?
	if (!_cycling)
		return;
	if (_view == (uint16)-1)
		return;
	if (_wait) {
		_wait--;
		return;
	}

	if (_cycling >= ANIM_BACKWARDS) {
		// animate backwards
		if (_frame == 0) {
			// at the start of the loop, what now?
			if (_loop > 0 && _vm->getViewLoop(_view, _loop - 1)->shouldRunNextLoop()) {
				// If it's a Go-to-next-loop on the previous one, then go back
				_loop--;
				_frame = _vm->getViewLoop(_view, _loop)->_frames.size() - 1;
			} else if (_cycling % ANIM_BACKWARDS == ANIM_ONCE) {
				// leave it on the first frame
				_cycling = 0;
				_frame = 0;
			} else {
				// repeating animation
				_frame = _vm->getViewLoop(_view, _loop)->_frames.size() - 1;
			}
		} else
			_frame--;
	} else {
		// animate forwards
		ViewLoopNew *loop = _vm->getViewLoop(_view, _loop);
		if ((uint)_frame + 1 >= loop->_frames.size()) {
			// at the end of the loop, what now?
			if (loop->shouldRunNextLoop()) {
				// go to next loop thing
				if ((uint)_loop + 1 >= _vm->_gameFile->_views[_view]._loops.size())
					error("RoomObject::update: last loop %d in view %d requested to move to next loop",
						_loop, _view);
				_loop++;
				_frame = 0;
			} else if (_cycling % ANIM_BACKWARDS == ANIM_ONCE) {
				// leave it on the last frame
				_cycling = 0;
			} else {
				if (!_vm->_state->_noMultiLoopRepeat) {
					// multi-loop animation, go back to the start
					while (_loop > 0 && _vm->getViewLoop(_view, _loop - 1)->shouldRunNextLoop())
						_loop--;
				}
				if (_cycling % ANIM_BACKWARDS == ANIM_ONCERESET)
					_cycling = 0;
				_frame = 0;
			}
		} else
			_frame++;
	}

	ViewFrame *frame = _vm->getViewFrame(_view, _loop, _frame);
	_spriteId = frame->_pic;

	if (!_cycling)
		return;

	_wait = frame->_speed + _overallSpeed;
	_vm->checkViewFrame(_view, _loop, _frame);
}

void RoomObject::stopMoving() {
	_moving = false;
}

Common::Point RoomObject::getDrawPos() {
	return Common::Point(_vm->multiplyUpCoordinate(_pos.x),
		_vm->multiplyUpCoordinate(_pos.y) - _vm->getSprites()->getSpriteHeight(_spriteId));
}

int RoomObject::getDrawOrder() const {
	uint order = _baseLine;
	if (!order)
		order = _pos.y;
	return order + ((_flags & OBJF_NOWALKBEHINDS) ? _vm->getCurrentRoom()->_height : 0);
}

const Graphics::Surface *RoomObject::getDrawSurface() {
	return _vm->getSprites()->getSprite(_spriteId)->_surface; // FIXME
}

uint RoomObject::getDrawWidth() {
	return getDrawSurface()->w; // FIXME
}

uint RoomObject::getDrawHeight() {
	return getDrawSurface()->h; // FIXME
}

uint RoomObject::getDrawTransparency() {
	return 0; // FIXME
}

bool RoomObject::isDrawVerticallyMirrored() {
	return false; // FIXME
}

int RoomObject::getDrawLightLevel() {
	return 0; // FIXME
}

void RoomObject::getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) {
	// FIXME
}

#define ROOM_FILE_VERSION kAGSRoomVer303x // 29

#define MAXOBJNAMELEN 30

#define BLOCKTYPE_MAIN        1
#define BLOCKTYPE_SCRIPT      2
#define BLOCKTYPE_COMPSCRIPT  3
#define BLOCKTYPE_COMPSCRIPT2 4
#define BLOCKTYPE_OBJECTNAMES 5
#define BLOCKTYPE_ANIMBKGRND  6
#define BLOCKTYPE_COMPSCRIPT3 7     // new CSCOMP script instead of SeeR
#define BLOCKTYPE_PROPERTIES  8
#define BLOCKTYPE_OBJECTSCRIPTNAMES 9
#define BLOCKTYPE_EOF         0xff

Room::Room(AGSEngine *vm, Common::SeekableReadStream *dta) : _vm(vm), _compiledScript(NULL), _interaction(NULL) {
	_backgroundSceneAnimSpeed = 5;
	// FIXME: copy main background scene palette

	_loaded = false;
	readData(dta);
	_loaded = true;
}

void Room::loadFrom(Common::SeekableReadStream *dta) {
	assert(!_loaded);
	_loaded = true;
	readData(dta);
}

void Room::unload() {
	assert(_loaded);
	_loaded = false;

	for (uint i = 0; i < _objects.size(); ++i)
		_objects[i]->_moving = 0;

	// we want to get rid of anything consuming a lot of memory!
	// the original saves only the contents of RoomStatus:
	//   objects, interactions, flags, script global state,
	//   hotspot/region enabled state, walkBehindBaselines
	//   and the interaction variable values.
	// anything else can go.

	// all graphics can go
	for (uint i = 0; i < _backgroundScenes.size(); ++i)
		_backgroundScenes[i]._scene.free();
	for (uint i = 0; i < _walkBehinds.size(); ++i)
		_walkBehinds[i]._surface.free();
	_backgroundScenes.clear();
	_walkableMask.free();
	_walkBehindMask.free();
	_hotspotMask.free();
	_regionsMask.free();

	// messages, options, animations, etc
	_messages.clear();
	_options.clear();
	_anims.clear();
	_shadingInfo.clear();
	_properties.clear();
	_shadingInfo.clear();
	_wallPoints.clear();
	_walkAreas.clear();

	_script.clear();

	// hotspots, objects
	for (uint i = 0; i < _hotspots.size(); ++i)
		_hotspots[i]._properties.clear();
	for (uint i = 0; i < _objects.size(); ++i) {
		_objects[i]->_properties.clear();
		// TODO: kill graphics, etc?
	}
}

// 'recache_walk_behinds' in original
void Room::initWalkBehinds() {
	// The original initialises the walkBehindExists/walkBehindStartY/walkBehindEndY
	// caches here too, but we don't care about those (at least, not yet).

	// First, reset the data.
	const uint NO_WALK_BEHIND = 100000;
	for (uint i = 0; i < _walkBehinds.size(); ++i) {
		_walkBehinds[i]._left = NO_WALK_BEHIND;
		_walkBehinds[i]._top = NO_WALK_BEHIND;
		_walkBehinds[i]._right = 0;
		_walkBehinds[i]._bottom = 0;

		_walkBehinds[i]._surface.free();
	}

	// Then, calculate the bounds of the walkbehinds.
	assert(_walkBehindMask.format.bytesPerPixel == 1);
	for (uint y = 0; y < _walkBehindMask.h; ++y) {
		byte *ptr = (byte *)_walkBehindMask.getBasePtr(0, y);
		for (uint x = 0; x < _walkBehindMask.w; ++x) {
			byte maskId = *(ptr++);
			if (maskId == 0 || maskId >= _walkBehinds.size())
				continue;

			_walkBehinds[maskId]._left = MIN(_walkBehinds[maskId]._left, x);
			_walkBehinds[maskId]._right = MAX(_walkBehinds[maskId]._right, x);
			_walkBehinds[maskId]._top = MIN(_walkBehinds[maskId]._top, y);
			_walkBehinds[maskId]._bottom = MAX(_walkBehinds[maskId]._bottom, y);
		}
	}

	// Finally, update the surfaces.
	updateWalkBehinds();
}

void Room::updateWalkBehinds() {
	const Graphics::Surface &background = _backgroundScenes[_vm->_state->_bgFrame]._scene;

	// walkbehind 0 is never valid
	for (uint i = 1; i < _walkBehinds.size(); ++i) {
		WalkBehind &wb = _walkBehinds[i];

		if (wb._right == 0)
			continue;

		uint width = wb._right - wb._left + 1;
		uint height = wb._bottom - wb._top + 1;
		if (!wb._surface.getPixels())
			wb._surface.create(width, height, background.format);
		else
			assert(wb._surface.w == width && wb._surface.h == height);

		// FIXME: pass format to getTransparentColor
		wb._surface.fillRect(Common::Rect(0, 0, width, height), _vm->_graphics->getTransparentColor());
		for (uint y = wb._top; y <= wb._bottom; y++) {
			for (uint x = wb._left; x <= wb._right; x++) {
				byte *ptr = (byte *)_walkBehindMask.getBasePtr(x, y);
				if (*ptr != i)
					continue;
				const void *srcPtr = background.getBasePtr(x, y);
				void *destPtr = wb._surface.getBasePtr(x - wb._left, y - wb._top);
				switch (background.format.bytesPerPixel) {
				case 1:
					*(byte *)destPtr = *(byte *)srcPtr;
					break;
				case 2:
					WRITE_UINT16(destPtr, READ_UINT16(srcPtr));
					break;
				case 4:
					WRITE_UINT32(destPtr, READ_UINT32(srcPtr));
					break;
				}
				//srcPtr = (byte *)srcPtr + background.format.bytesPerPixel;
				//destPtr = (byte *)destPtr + background.format.bytesPerPixel;
			}
		}
	}
}

uint Room::getRegionAt(int x, int y) {
	x = _vm->convertToLowRes(x);
	y = _vm->convertToLowRes(y);

	if (x >= _regionsMask.w)
		x = _regionsMask.w - 1;
	if (y >= _regionsMask.h)
		y = _regionsMask.h - 1;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	void *ptr = _regionsMask.getBasePtr(x, y);
	assert(_regionsMask.format.bytesPerPixel == 1);
	uint regionId = *(byte *)ptr;

	if (regionId >= _regions.size())
		error("An invalid pixel was found on the room region mask (colour %d, location: %d, %d)", regionId, x, y);
	if (!_regions[regionId]._enabled)
		return 0;
	return regionId;
}

void Room::readData(Common::SeekableReadStream *dta) {
	uint16 version = dta->readUint16LE();

	if (_loaded && version != _version)
		error("room version changed from under us (was %d, now %d)", _version, version);
	_version = version;

	if (_version < kAGSRoomVer241)
		error("room version %d is too old", _version);
	else if (_version > kAGSRoomVer303x)
		error("room version %d is too new", _version);

	bool seenMain = false;

	while (true) {
		if (dta->eos())
			error("ran out of data while reading room");

		byte blockType = dta->readByte();
		if (blockType == BLOCKTYPE_EOF)
			break;

		uint32 blockSize = dta->readUint32LE();
		uint32 blockEnd = dta->pos() + blockSize;

		if (blockType != BLOCKTYPE_MAIN && !seenMain)
			error("Room: expected main block, but got block type %d", blockType);

		uint32 count;
		switch (blockType) {
		case BLOCKTYPE_MAIN:
			if (seenMain)
				error("Room: second main block encountered");
			seenMain = true;
			readMainBlock(dta);
			break;
		case BLOCKTYPE_SCRIPT:
			if (!_script.empty())
				error("Room: second script encountered");

			count = dta->readUint32LE();
			while (count--)
				_script += (char)dta->readByte();
			for (uint i = 0; i < _script.size(); ++i)
				_script.setChar(_script[i] + kSecretPassword[i % 11], i);
			debug(7, "Room script: %s", _script.c_str());
			break;
		case BLOCKTYPE_COMPSCRIPT:
			// TODO
			warning("Room: ignoring old (COMPSCRIPT) compiled script");
			dta->skip(blockSize);
			break;
		case BLOCKTYPE_COMPSCRIPT2:
			// TODO
			warning("Room: ignoring old (COMPSCRIPT2) compiled script");
			dta->skip(blockSize);
			break;
		case BLOCKTYPE_COMPSCRIPT3:
			if (_compiledScript)
				error("Room: second compiled script encountered");

			_compiledScript = new ccScript();
			_compiledScript->readFrom(dta);
			break;
		case BLOCKTYPE_OBJECTNAMES:
			count = dta->readByte();
			if (count != _objects.size())
				error("Room: got %d object names for %d objects", count, _objects.size());
			for (uint i = 0; i < count; ++i) {
				char objectName[MAXOBJNAMELEN + 1];
				dta->read(objectName, MAXOBJNAMELEN);
				objectName[MAXOBJNAMELEN] = '\0';
				_objects[i]->_name = objectName;
			}
			break;
		case BLOCKTYPE_OBJECTSCRIPTNAMES:
			count = dta->readByte();
			if (count != _objects.size())
				error("Room: got %d object script names for %d objects", count, _objects.size());
			for (uint i = 0; i < count; ++i) {
				char objectName[MAX_SCRIPT_NAME_LEN + 1];
				dta->read(objectName, MAX_SCRIPT_NAME_LEN);
				objectName[MAX_SCRIPT_NAME_LEN] = '\0';
				_objects[i]->_scriptName = objectName;
				debug(7, "object %d script '%s'", i, objectName);

				if (_version <= kAGSRoomVer300 && !_objects[i]->_scriptName.empty()) {
					_objects[i]->_scriptName.toLowercase();
					_objects[i]->_scriptName.setChar(toupper(_objects[i]->_scriptName[0]), 0);
					_objects[i]->_scriptName.insertChar('o', 0);
				}
			}
			break;
		case BLOCKTYPE_ANIMBKGRND:
			{
				if (_backgroundScenes.size() != 1)
					error("Room: second set of background scenes encountered");

				byte numScenes = dta->readByte();
				debug(5, "Room: %d extra background scenes", numScenes);
				if (numScenes < 1)
					error("Room: no background scenes encountered");

				_backgroundSceneAnimSpeed = dta->readByte();

				_backgroundScenes.resize(numScenes);
				if (_version >= kAGSRoomVer255) {
					for (uint i = 0; i < _backgroundScenes.size(); ++i)
						_backgroundScenes[i]._sharedPalette = (bool)dta->readByte();
				}
				// we already read the first scene as part of the main block
				for (uint i = 1; i < _backgroundScenes.size(); ++i)
					_backgroundScenes[i]._scene = readLZSSImage(dta, _vm->_graphics->getPixelFormat(), _backgroundScenes[i]._palette);
			}
			break;
		case BLOCKTYPE_PROPERTIES:
			count = dta->readUint32LE();
			if (count != 1)
				error("Room: invalid Custom Properties block encountered (%d)", count);
			_vm->_gameFile->readProperties(dta, _properties);
			for (uint i = 0; i < _hotspots.size(); ++i)
				_vm->_gameFile->readProperties(dta, _hotspots[i]._properties);
			for (uint i = 0; i < _objects.size(); ++i)
				_vm->_gameFile->readProperties(dta, _objects[i]->_properties);
			break;
		default:
			error("Room: unknown block type %d", blockType);
		}

		if ((uint32)dta->pos() != blockEnd)
			error("Room: didn't read correct amount from block type %d (at %d, block end at %d)",
				blockType, dta->pos(), blockEnd);
	}

	// FIXME: palette sync

	if (!_loaded && _vm->_gameFile->_defaultResolution > 2) {
		if (_version < kAGSRoomVer303x && _vm->getGameOption(OPT_NATIVECOORDINATES)) {
			// coordinates are stored in non-native form, and we want them in native
			// (originally at the end of load_room)

			for (uint i = 0; i < _objects.size(); ++i) {
				_objects[i]->_pos.x *= 2;
				_objects[i]->_pos.y *= 2;
				if (_objects[i]->_baseLine != 0xffffffff)
					_objects[i]->_baseLine *= 2;
			}

			for (uint i = 0; i < _hotspots.size(); ++i) {
				_hotspots[i]._walkToPos.x *= 2;
				_hotspots[i]._walkToPos.y *= 2;
			}

			for (uint i = 0; i < _walkBehinds.size(); ++i)
				_walkBehinds[i]._baseline *= 2;

			_boundary.left *= 2;
			_boundary.top *= 2;
			_boundary.bottom *= 2;
			_boundary.right *= 2;

			_width *= 2;
			_height *= 2;
		} else if (version >= kAGSRoomVer303x && !_vm->getGameOption(OPT_NATIVECOORDINATES)) {
			// coordinates are stored in native form, and we want them in non-native
			// (originally convert_room_coordinates_to_low_res)

			for (uint i = 0; i < _objects.size(); ++i) {
				_objects[i]->_pos.x /= 2;
				_objects[i]->_pos.y /= 2;
				if (_objects[i]->_baseLine != 0xffffffff)
					_objects[i]->_baseLine /= 2;
			}

			for (uint i = 0; i < _hotspots.size(); ++i) {
				_hotspots[i]._walkToPos.x /= 2;
				_hotspots[i]._walkToPos.y /= 2;
			}

			for (uint i = 0; i < _walkBehinds.size(); ++i)
				_walkBehinds[i]._baseline /= 2;

			_boundary.left /= 2;
			_boundary.top /= 2;
			_boundary.bottom /= 2;
			_boundary.right /= 2;

			_width /= 2;
			_height /= 2;
		}
	}

	delete dta;
}

Room::~Room() {
	delete _compiledScript;

	delete _interaction;
	for (uint i = 0; i < _hotspots.size(); ++i)
		delete _hotspots[i]._interaction;
	for (uint i = 0; i < _objects.size(); ++i)
		delete _objects[i]->_interaction;
	for (uint i = 0; i < _regions.size(); ++i)
		delete _regions[i]._interaction;

	for (uint i = 0; i < _objects.size(); ++i)
		delete _objects[i];

	for (uint i = 0; i < _backgroundScenes.size(); ++i)
		_backgroundScenes[i]._scene.free();
	for (uint i = 0; i < _walkBehinds.size(); ++i)
		_walkBehinds[i]._surface.free();
	_walkableMask.free();
	_walkBehindMask.free();
	_hotspotMask.free();
	_regionsMask.free();
}

#define NO_GAME_ID_IN_ROOM_FILE 16325
#define MAX_WALK_AREAS 15

void Room::readMainBlock(Common::SeekableReadStream *dta) {
	_bytesPerPixel = 1;
	if (_version >= kAGSRoomVer208)
		_bytesPerPixel = dta->readUint32LE();
	if (_bytesPerPixel < 1)
		_bytesPerPixel = 1;

	// walk-behind base lines
	uint16 walkBehindsCount = dta->readUint16LE();
	debug(5, "Room: %d walk-behinds", walkBehindsCount);
	if (walkBehindsCount > 16)
		error("Room: too many walk-behinds (%d)", walkBehindsCount);
	if (_loaded) {
		assert(walkBehindsCount == _walkBehinds.size());
		dta->skip(walkBehindsCount * 2);
	} else {
		_walkBehinds.resize(walkBehindsCount);
		for (uint i = 0; i < _walkBehinds.size(); ++i)
			_walkBehinds[i]._baseline = dta->readUint16LE();
	}

	// hotspots
	uint32 hotspotCount = dta->readUint32LE();
	debug(5, "Room: %d hotspots", hotspotCount);
	if (!hotspotCount)
		hotspotCount = 20;
	if (hotspotCount > 50) // MAX_HOTSPOTS: v2.62 increased from 20 to 30; v2.8 to 50
		error("Room: too many hotspots (%d)", hotspotCount);
	if (_loaded)
		assert(hotspotCount == _hotspots.size());
	else
		_hotspots.resize(hotspotCount);
	for (uint i = 0; i < _hotspots.size(); ++i) {
		_hotspots[i]._walkToPos.x = dta->readSint16LE();
		_hotspots[i]._walkToPos.y = dta->readSint16LE();
	}
	for (uint i = 0; i < _hotspots.size(); ++i) {
		if (_version >= kAGSRoomVer303) {
			_hotspots[i]._name = readString(dta);
		} else {
			char hotspotName[30 + 1];
			dta->read(hotspotName, 30);
			hotspotName[30] = '\0';
			_hotspots[i]._name = hotspotName;
			debug(7, "hotspot %d: '%s'", i, hotspotName);
		}
	}
	if (_version >= kAGSRoomVer270) {
		for (uint i = 0; i < _hotspots.size(); ++i) {
			char hotspotName[MAX_SCRIPT_NAME_LEN + 1];
			dta->read(hotspotName, MAX_SCRIPT_NAME_LEN);
			hotspotName[MAX_SCRIPT_NAME_LEN] = '\0';
			_hotspots[i]._scriptName = hotspotName;
			debug(7, "hotspot %d script '%s'", i, hotspotName);
		}
	}

	// walk area wall points
	uint32 wallPointCount = dta->readUint32LE();
	debug(5, "Room: %d wall points", wallPointCount);
	if (wallPointCount > MAX_WALK_AREAS)
		error("Room: too many wall points (%d)", wallPointCount);
	_wallPoints.resize(wallPointCount);
	for (uint i = 0; i < _wallPoints.size(); ++i) {
		_wallPoints[i].x = dta->readUint32LE();
		_wallPoints[i].y = dta->readUint32LE();
	}

	// room boundary
	_boundary.top = dta->readUint16LE();
	_boundary.bottom = dta->readUint16LE();
	_boundary.left = dta->readUint16LE();
	_boundary.right = dta->readUint16LE();

	// objects
	uint32 objectCount = dta->readUint16LE();
	debug(5, "Room: %d objects", objectCount);
	if (objectCount > 40)
		error("Room: too many objects (%d)", objectCount);
	if (_loaded)
		assert(_objects.size() == objectCount);
	else
		_objects.resize(objectCount);
	for (uint i = 0; i < _objects.size(); ++i) {
		// sprites
		if (_loaded) {
			dta->skip(10);
			continue;
		}

		_objects[i] = new RoomObject(_vm, i);

		_objects[i]->_spriteId = dta->readUint16LE();
		_objects[i]->_pos.x = dta->readUint16LE();
		_objects[i]->_pos.y = dta->readUint16LE();
		if (_version <= kAGSRoomVer300) {
			uint spriteHeight = _vm->getSprites()->getSpriteHeight(_objects[i]->_spriteId);
			_objects[i]->_pos.y += _vm->divideDownCoordinate(spriteHeight);
		}
		/*uint16 roomId = */ dta->readUint16LE();
		uint16 objectOn = dta->readUint16LE();
		switch (objectOn) {
		case 0:
			// off
			_objects[i]->_merged = false;
			_objects[i]->_visible = false;
			break;
		case 1:
			// on
			_objects[i]->_merged = false;
			_objects[i]->_visible = true;
			break;
		case 2:
			// merged into background, so disabled
			_objects[i]->_merged = true;
			_objects[i]->_visible = false;
			break;
		default:
			error("Room: invalid on status %d for object", objectOn);
		}
	}

	if (_version >= kAGSRoomVer253) {
		// local variables
		uint32 localVarCount = dta->readUint32LE();
		debug(5, "Room: %d local variables", localVarCount);
		_localVars.resize(localVarCount);
		for (uint i = 0; i < _localVars.size(); ++i)
			_localVars[i].readFrom(dta);
	}

	if (!_loaded && _version >= kAGSRoomVer241) {
		// 2.x interactions
		if (_version < kAGSRoomVer300) {
			for (uint i = 0; i < _hotspots.size(); ++i) {
				_hotspots[i]._interaction = NewInteraction::createFrom(dta);
			}
			for (uint i = 0; i < _objects.size(); ++i) {
				_objects[i]->_interaction = NewInteraction::createFrom(dta);
			}
		}

		if (_version < kAGSRoomVer300)
			_interaction = NewInteraction::createFrom(dta);

		if (_version >= kAGSRoomVer255r) {
			// regions
			uint32 regionCount = dta->readUint32LE();
			debug(5, "Room: %d regions", regionCount);
			if (regionCount > 16)
				error("Room: too many regions (%d)", regionCount);
			_regions.resize(regionCount);

			// 2.x interactions
			if (_version < kAGSRoomVer300) {
				for (uint i = 0; i < _regions.size(); ++i) {
					_regions[i]._interaction = NewInteraction::createFrom(dta);
				}
			}
		}

		if (_version >= kAGSRoomVer300) {
			// 3.x scripts

			_interactionScripts.readFrom(dta);
			for (uint i = 0; i < _hotspots.size(); ++i)
				_hotspots[i]._interactionScripts.readFrom(dta);
			for (uint i = 0; i < _objects.size(); ++i)
				_objects[i]->_interactionScripts.readFrom(dta);
			for (uint i = 0; i < _regions.size(); ++i)
				_regions[i]._interactionScripts.readFrom(dta);
		}
	}

	if (_version >= kAGSRoomVer2a) {
		if (_loaded) {
			dta->skip(_objects.size() * 4);
		} else {
			for (uint i = 0; i < _objects.size(); ++i)
				_objects[i]->_baseLine = dta->readUint32LE();
		}
		_width = dta->readUint16LE();
		_height = dta->readUint16LE();
		debug(4, "Room: width %d, height %d", _width, _height);
	}

	// object flags
	if (_version >= kAGSRoomVer262) {
		if (_loaded) {
			dta->skip(_objects.size() * 2);
		} else {
			for (uint i = 0; i < _objects.size(); ++i)
				_objects[i]->_flags = dta->readUint16LE();
		}
	}

	// (relative) resolution
	_resolution = 1;
	if (_version >= kAGSRoomVer200)
		_resolution = dta->readUint16LE();

	// walk area header
	uint32 walkAreaCount = MAX_WALK_AREAS;
	if (_version >= kAGSRoomVer240) {
		walkAreaCount = dta->readUint32LE();
		debug(5, "Room: %d walk areas", walkAreaCount);
	}
	if (walkAreaCount > MAX_WALK_AREAS + 1)
		error("Room: too many walk areas (%d)", walkAreaCount);
	_walkAreas.resize(walkAreaCount);

	// walk area zoom level
	if (_version >= kAGSRoomVer2a7) {
		for (uint i = 0; i < _walkAreas.size(); ++i)
			_walkAreas[i]._zoom = dta->readUint16LE();
	}

	// walk area light level
	if (_version >= kAGSRoomVer214) {
		for (uint i = 0; i < _walkAreas.size(); ++i)
			_walkAreas[i]._light = dta->readUint16LE();
	}

	// walk area vector scaling
	if (_version >= kAGSRoomVer251) {
		for (uint i = 0; i < _walkAreas.size(); ++i) {
			_walkAreas[i]._zoom2 = dta->readUint16LE();

			// if this is a continuously scaled area with the same zoom
			// level at both ends, change it to a normal scaled area
			if (_walkAreas[i]._zoom == _walkAreas[i]._zoom2)
				_walkAreas[i]._zoom2 = NOT_VECTOR_SCALED;
		}
		for (uint i = 0; i < _walkAreas.size(); ++i)
			_walkAreas[i]._top = dta->readUint16LE();
		for (uint i = 0; i < _walkAreas.size(); ++i)
			_walkAreas[i]._bottom = dta->readUint16LE();
	}

	dta->skip(11); // FIXME: password (+demangling)

	// options
	_options.resize(10);
	for (uint i = 0; i < 10; ++i)
		_options[i] = dta->readByte();

	// message count
	uint16 messageCount = dta->readUint16LE();
	debug(5, "Room: %d messages", messageCount);
	if (messageCount > 100)
		error("Room: too many messages (%d)", messageCount);
	_messages.resize(messageCount);

	// owning game id
	_gameId = NO_GAME_ID_IN_ROOM_FILE; // FIXME: good?
	if (_version >= kAGSRoomVer272)
		_gameId = dta->readUint32LE();
	if ((_gameId != NO_GAME_ID_IN_ROOM_FILE) && (_gameId != _vm->getGameUniqueID()))
		error("Room: room game id %d doesn't match main game id %d", _gameId, _vm->getGameUniqueID());

	// actual messages
	if (_version >= 3) {
		for (uint i = 0; i < _messages.size(); ++i) {
			_messages[i]._displayAs = dta->readByte();
			_messages[i]._flags = dta->readByte();
		}
	}

	for (uint i = 0; i < _messages.size(); ++i) {
		Common::String &text = _messages[i]._text;

		if (_version >= kAGSRoomVer261)
			text = decryptString(dta);
		else
			text = readString(dta);

		debug(7, "Room: message %d: '%s'", i, text.c_str());

		if (text.empty())
			continue;
		if (text[text.size() - 1] == (char)200) {
			// convert old-style terminator into new-style flag
			text.deleteLastChar();
			_messages[i]._flags |= MSG_DISPLAYNEXT;
		}
	}

	if (_version >= 6) {
		uint16 animationCount = dta->readUint16LE();

		if (animationCount)
			warning("Room: ignoring %d animations", animationCount);
		// FIXME: read these?
		dta->skip(animationCount * (4 + (24 * 10)));
	}

	if ((_version >= 4) && (_version < kAGSRoomVer250)) {
		// FIXME
		error("don't support pre-2.5 graphical scripts yet");
	}

	if (_version >= kAGSRoomVer114) {
		_shadingInfo.resize(16);
		for (uint i = 0; i < 16; ++i)
			_shadingInfo[i] = dta->readUint16LE();
	}

	if (_version >= kAGSRoomVer255r) {
		for (uint i = 0; i < _regions.size(); ++i)
			_regions[i]._lightLevel = dta->readUint16LE();
		for (uint i = 0; i < _regions.size(); ++i)
			_regions[i]._tintLevel = dta->readUint32LE();
	}

	BackgroundScene scene;
	_backgroundScenes.push_back(scene);
	if (_version >= 5)
		_backgroundScenes[0]._scene = readLZSSImage(dta, _vm->_graphics->getPixelFormat(), _backgroundScenes[0]._palette);
	else
		_backgroundScenes[0]._scene = readRLEImage(dta);

	_regionsMask = readRLEImage(dta);
	_walkableMask = readRLEImage(dta);
	_walkBehindMask = readRLEImage(dta);
	_hotspotMask = readRLEImage(dta);

	initWalkBehinds(); // FIXME

	if (_version < kAGSRoomVer255r) {
		// old version - copy the walkable areas to regions

		// FIXME
		error("Room: too old, fixme");
	}
}

const Graphics::Surface *Room::getDrawSurface() {
	return &_backgroundScenes[_vm->_state->_bgFrame]._scene;
}

uint Room::getDrawWidth() {
	return _backgroundScenes[_vm->_state->_bgFrame]._scene.w;
}

uint Room::getDrawHeight() {
	return _backgroundScenes[_vm->_state->_bgFrame]._scene.h;
}

} // End of namespace AGS
