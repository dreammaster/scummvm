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

#ifndef AGS_GUI_H
#define AGS_GUI_H

#include "common/array.h"
#include "common/stream.h"

namespace AGS {

#define MAX_OBJS_ON_GUI 30
#define GOBJ_BUTTON     1
#define GOBJ_LABEL      2
#define GOBJ_INVENTORY  3
#define GOBJ_SLIDER     4
#define GOBJ_TEXTBOX    5
#define GOBJ_LISTBOX    6
#define GUI_TEXTWINDOW  0x05    // set vtext[0] to this to signify text window
#define GUIF_NOCLICK    1
#define MOVER_MOUSEDOWNLOCKED -4000

#define GUIDIS_GREYOUT   1
#define GUIDIS_BLACKOUT  2
#define GUIDIS_UNCHANGED 4
#define GUIDIS_GUIOFF  0x80

// GUIObject
#define GUIF_DEFAULT  1
#define GUIF_CANCEL   2
#define GUIF_DISABLED 4
#define GUIF_TABSTOP  8
#define GUIF_INVISIBLE 0x10
#define GUIF_CLIP     0x20
#define GUIF_NOCLICKS 0x40
#define GUIF_DELETED  0x8000
#define BASEGOBJ_SIZE 7
#define GALIGN_LEFT   0
#define GALIGN_RIGHT  1
#define GALIGN_CENTRE 2
#define MAX_GUIOBJ_SCRIPTNAME_LEN 25
#define MAX_GUIOBJ_EVENTS 10
#define MAX_GUIOBJ_EVENTHANDLER_LEN 30

// GUITextBox
#define GTF_NOBORDER  1

// GUIListBox
#define GLF_NOBORDER     1
#define GLF_NOARROWS     2
#define GLF_SGINDEXVALID 4

// GUIButton
#define GBUT_ALIGN_TOPMIDDLE    0
#define GBUT_ALIGN_TOPLEFT      1
#define GBUT_ALIGN_TOPRIGHT     2
#define GBUT_ALIGN_MIDDLELEFT   3
#define GBUT_ALIGN_CENTRED      4
#define GBUT_ALIGN_MIDDLERIGHT  5
#define GBUT_ALIGN_BOTTOMLEFT   6
#define GBUT_ALIGN_BOTTOMMIDDLE 7
#define GBUT_ALIGN_BOTTOMRIGHT  8

class AGSEngine;

class GUIObject {
public:
	GUIObject(AGSEngine *vm) : _vm(vm) { }
	virtual ~GUIObject() { }

	uint32 _id;
	uint32 _objectId;
	uint32 _flags;

	uint32 _x, _y;
	uint32 _width, _height;
	uint32 _zorder;
	uint32 _activated;

	Common::String _scriptName;
	Common::Array<Common::String> _eventHandlers;

	// FIXME: virtual functions

protected:
	virtual void readFrom(Common::SeekableReadStream *dta);
	virtual uint32 getMaxNumEvents() = 0;

	AGSEngine *_vm;

	Common::Array<Common::String> _supportedEvents;
	Common::Array<Common::String> _supportedEventArgs;
};

class GUISlider : public GUIObject {
public:
	GUISlider(AGSEngine *vm) : GUIObject(vm) { }
	void readFrom(Common::SeekableReadStream *dta);

	uint32 _min, _max;
	uint32 _value;
	uint32 _mousePressed;
	uint32 _handlePic;
	uint32 _handleOffset;
	uint32 _bgImage;

protected:
	uint32 getMaxNumEvents() { return 1; }

	// The following variables are not persisted on disk
	// Cached (x1, x2, y1, y2) co-ordinates of slider handle
	uint32 _cachedHandleTLX, _cachedHandleBRX;
	uint32 _cachedHandleTLY, _cachedHandleBRY;
};

class GUILabel : public GUIObject {
public:
	GUILabel(AGSEngine *vm) : GUIObject(vm) { }
	void readFrom(Common::SeekableReadStream *dta);

	uint32 _font;
	uint32 _textColor;
	uint32 _align;

protected:
	uint32 getMaxNumEvents() { return 0; }

	Common::String _text;
};

class GUITextBox : public GUIObject {
public:
	GUITextBox(AGSEngine *vm) : GUIObject(vm) { }
	void readFrom(Common::SeekableReadStream *dta);

	Common::String _text;
	uint32 _font;
	uint32 _textColor;
	uint32 _exFlags;

protected:
	uint32 getMaxNumEvents() { return 1; }
};

class GUIListBox : public GUIObject {
public:
	GUIListBox(AGSEngine *vm) : GUIObject(vm) { }
	void readFrom(Common::SeekableReadStream *dta);

	Common::Array<Common::String> _items;
	Common::Array<uint16> _itemSaveGameIndexes;

	uint32 _selected;
	uint32 _topItem;
	uint32 _mouseXP, _mouseYP;

	uint32 _rowHeight;
	uint32 _numItemsFit;

	uint32 _font;
	uint32 _textColor;
	uint32 _backColor;
	uint32 _exFlags;
	uint32 _selectedBgColor;
	uint32 _alignment;

protected:
	uint32 getMaxNumEvents() { return 1; }
};

class GUIInvControl : public GUIObject {
public:
	GUIInvControl(AGSEngine *vm) : GUIObject(vm) { }
	void readFrom(Common::SeekableReadStream *dta);

	uint32 _charId; // whose inventory? (-1 = current player)
	uint32 _itemWidth, _itemHeight;
	uint32 _topIndex;

	// not persisted
	uint32 _isOver;
	uint32 _itemsPerLine, _numLines;

protected:
	uint32 getMaxNumEvents() { return 1; }
};

class GUIButton : public GUIObject {
public:
	GUIButton(AGSEngine *vm) : GUIObject(vm) { }
	void readFrom(Common::SeekableReadStream *dta);

	Common::String _text;

	uint32 _pic, _overPic, _pushedPic;
	uint32 _isPushed, _isOver;

	uint32 _font;
	uint32 _textColor;

	uint32 _leftClick, _rightClick;
	uint32 _leftClickData, _rightClickData;
	uint32 _textAlignment;

	// not persisted
	uint32 _usePic;

protected:
	uint32 getMaxNumEvents() { return 1; }
};

struct GUIGroup {
	char _vText[4]; // ??? - for compatibility
	Common::String _name;
	Common::String _clickEventHandler;

	uint32 _x, _y, _width, _height;

	uint32 _focus; // which object has the focus

	uint32 _popup; // // when it pops up (POPUP_NONE, POPUP_MOUSEY, POPUP_SCRIPT)
	uint32 _popupYP; // // popup when mousey < this

	uint32 _bgColor, _bgPic, _fgColor;

	uint32 _mouseOver;
	uint32 _mouseWasX, _mouseWasY;
	uint32 _mouseDownOn;

	uint32 _highlightObj;
	uint32 _flags;
	uint32 _transparency;
	uint32 _zorder;
	uint32 _id;
	uint32 _on;

	Common::Array<GUIObject *> _objects;
	Common::Array<uint32> _objectRefPtrs; // for re-building objs array
	Common::Array<uint16> _objectDrawOrder;
};

} // End of namespace AGS

#endif // AGS_GUI_H
