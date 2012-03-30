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

#ifndef AGS_GUI_H
#define AGS_GUI_H

#include "common/array.h"
#include "common/rect.h"
#include "common/stream.h"
#include "graphics/surface.h"

#include "engines/ags/drawable.h"
#include "engines/ags/scriptobj.h"

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

#define POPUP_NONE      0
#define POPUP_MOUSEY    1
#define POPUP_SCRIPT    2
#define POPUP_NOAUTOREM 3  // don't remove automatically during cutscene
#define POPUP_NONEINITIALLYOFF 4   // normal GUI, initially off
#define VTA_LEFT        0
#define VTA_RIGHT       1
#define VTA_CENTRE      2
#define IFLG_TEXTWINDOW 1

#define IBACT_SETMODE   1
#define IBACT_SCRIPT    2

// GUIControl
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

class GUIControl : public ScriptObject {
public:
	GUIControl(AGSEngine *vm) : _vm(vm), _parent(NULL) { }
	virtual ~GUIControl() { }
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotGUIControl); }

	class GUIGroup *_parent;
	uint32 _id;
	uint32 _flags;

	uint32 _x, _y;
	uint32 _width, _height;
	uint32 _zorder;
	bool _activated;

	Common::String _scriptName;
	Common::Array<Common::String> _eventHandlers;

	// position relative to gui
	virtual void onMouseMove(const Common::Point &pos) { }
	virtual void onMouseEnter() { }
	virtual void onMouseLeave() { }
	// button down - return true to lock focus
	virtual bool onMouseDown() { return false; }
	virtual void onMouseUp() { }
	virtual void onKeyPress(uint id) { }
	virtual void draw(Graphics::Surface *surface) = 0;

	virtual bool isOverControl(const Common::Point &pos);

	virtual void resize(uint32 width, uint32 height);
	virtual void resized() { }

	bool isDeleted() { return _flags & GUIF_DELETED; }
	bool isDisabled();
	void setEnabled(bool enabled);
	bool isVisible() { return !(_flags & GUIF_INVISIBLE); }
	void show() { _flags &= ~GUIF_INVISIBLE; }
	void hide() { _flags |= GUIF_INVISIBLE; }
	bool isClickable() { return !(_flags & GUIF_NOCLICKS); }
	void setClickable(bool value);

	virtual uint32 getMaxNumEvents() const = 0;

protected:
	virtual void readFrom(Common::SeekableReadStream *dta);

	AGSEngine *_vm;
};

class GUISlider : public GUIControl {
public:
	GUISlider(AGSEngine *vm) : GUIControl(vm) { }
	void readFrom(Common::SeekableReadStream *dta);
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotGUIControl || objectType == sotGUISlider); }
	const char *getObjectTypeName() { return "GUISlider"; }

	virtual bool isOverControl(const Common::Point &pos);

	int32 getMin() { return _min; }
	void setMin(int32 value);
	int32 getMax() { return _max; }
	void setMax(int32 value);
	int32 getValue() { return _value; }
	void setValue(int32 value);

	int32 getHandleOffset() { return _handleOffset; }
	void setHandleOffset(int32 value);

	int32 _min, _max;
	int32 _value;

	uint32 _mousePressed;
	uint32 _handlePic;
	int32 _handleOffset;
	uint32 _bgImage;

	void draw(Graphics::Surface *surface);

protected:
	uint32 getMaxNumEvents() const { return 1; }

	// The following variables are not persisted on disk
	// Cached (x1, x2, y1, y2) co-ordinates of slider handle
	uint32 _cachedHandleTLX, _cachedHandleBRX;
	uint32 _cachedHandleTLY, _cachedHandleBRY;
};

class GUILabel : public GUIControl {
public:
	GUILabel(AGSEngine *vm) : GUIControl(vm) { }
	void readFrom(Common::SeekableReadStream *dta);
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotGUIControl || objectType == sotGUILabel); }
	const char *getObjectTypeName() { return "GUILabel"; }

	uint32 getFont() { return _font; }
	void setFont(uint32 font);
	uint32 getColor() { return _textColor; }
	void setColor(uint32 color);
	uint32 getAlign() { return _align; }
	void setAlign(uint32 align);
	const Common::String &getText() const { return _text; }
	void setText(Common::String text);

	void draw(Graphics::Surface *surface);

protected:
	uint32 getMaxNumEvents() const { return 0; }

	uint32 _font;
	uint32 _textColor;
	uint32 _align;

	Common::String _text;
};

class GUITextBox : public GUIControl {
public:
	GUITextBox(AGSEngine *vm) : GUIControl(vm) { }
	void readFrom(Common::SeekableReadStream *dta);
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotGUIControl || objectType == sotGUITextBox); }
	const char *getObjectTypeName() { return "GUITextBox"; }

	uint32 getFont() { return _font; }
	void setFont(uint32 font);
	const Common::String &getText() const { return _text; }
	void setText(Common::String text);

	Common::String _text;
	uint32 _font;
	uint32 _textColor;
	uint32 _exFlags;

	void draw(Graphics::Surface *surface);

protected:
	uint32 getMaxNumEvents() const { return 1; }
};

class GUIListBox : public GUIControl {
public:
	GUIListBox(AGSEngine *vm) : GUIControl(vm) { }
	void readFrom(Common::SeekableReadStream *dta);
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotGUIControl || objectType == sotGUIListBox); }
	const char *getObjectTypeName() { return "GUIListBox"; }

	void resized();

	uint32 getFont() { return _font; }
	void setFont(uint32 font);

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

	void draw(Graphics::Surface *surface);

protected:
	uint32 getMaxNumEvents() const { return 1; }
};

class GUIInvControl : public GUIControl {
public:
	GUIInvControl(AGSEngine *vm) : GUIControl(vm) { }
	void readFrom(Common::SeekableReadStream *dta);
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotGUIControl || objectType == sotGUIInvWindow); }
	const char *getObjectTypeName() { return "GUIInvControl"; }

	Character *getCharToDisplay();
	void resized();

	uint32 _charId; // whose inventory? (-1 = current player)
	uint32 _itemWidth, _itemHeight;
	uint32 _topIndex;

	// not persisted
	uint32 _isOver;
	uint32 _itemsPerLine, _numLines;

	void draw(Graphics::Surface *surface);

protected:
	uint32 getMaxNumEvents() const { return 1; }
};

class GUIButton : public GUIControl {
public:
	GUIButton(AGSEngine *vm) : GUIControl(vm) { }
	void readFrom(Common::SeekableReadStream *dta);
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotGUIControl || objectType == sotGUIButton); }
	const char *getObjectTypeName() { return "GUIButton"; }

	uint32 getDisplayedGraphic();

	uint32 getNormalGraphic() { return _pic; }
	void setNormalGraphic(uint32 pic);
	uint32 getMouseOverGraphic() { return _overPic; }
	void setMouseOverGraphic(uint32 pic);
	uint32 getPushedGraphic() { return _pushedPic; }
	void setPushedGraphic(uint32 pic);

	const Common::String &getText() const { return _text; }
	void setText(Common::String text);

	uint32 getFont() { return _font; }
	void setFont(uint32 font);

	uint getTextColor() const { return _textColor; }
	void setTextColor(uint color);

	uint32 _leftClick, _rightClick;
	uint32 _leftClickData, _rightClickData;

protected:
	Common::String _text;

	uint32 _pic, _overPic, _pushedPic;
	uint32 _isPushed, _isOver;

	uint32 _font;
	uint32 _textColor;

	uint32 _textAlignment;

	// not persisted
	uint32 _usePic;

	void stopAnimation();
	void draw(Graphics::Surface *surface);

	uint32 getMaxNumEvents() const { return 1; }
};

class GUIGroup : public ScriptObject, public Drawable {
public:
	GUIGroup(AGSEngine *vm);
	~GUIGroup();

	// note: these get GLOBAL positions
	void onMouseMove(const Common::Point &pos);
	void onMouseUp(const Common::Point &pos);
	void onMouseDown(const Common::Point &pos);

	void setEnabled(bool enabled);
	void setVisible(bool visible);
	void setSize(uint32 width, uint32 height);
	void setBackgroundPicture(uint32 pic);
	void setZOrder(uint zorder);

	void setTransparency(uint val);
	uint getTransparency();

	void invalidate();
	void controlPositionsChanged();

	bool isMouseOver(const Common::Point &pos);
	GUIControl *getControlAt(const Common::Point &pos, bool mustBeClickable = true);

	bool isTextWindow() const;

	void interfaceOn();
	void interfaceOff();

	void sortControls();

	bool isOfType(ScriptObjectType objectType) { return (objectType == sotGUI); }
	const char *getObjectTypeName() { return "GUI"; }

	char _vText[4]; // ??? - for compatibility
	Common::String _name;
	Common::String _clickEventHandler;

	int32 _x, _y;
	uint32 _width, _height;

	uint32 _focus; // which object has the focus

	uint32 _popup; // // when it pops up (POPUP_NONE, POPUP_MOUSEY, POPUP_SCRIPT)
	uint32 _popupYP; // // popup when mousey < this

	uint32 _bgColor, _bgPic, _fgColor;

	int32 _mouseOver;
	int32 _mouseDownOn;

	uint32 _highlightObj;
	uint32 _flags;
	uint32 _transparency;
	uint32 _zorder;
	uint32 _id;

	// these replace _on
	bool _enabled;
	bool _visible;

	Common::Array<GUIControl *> _controls;
	Common::Array<uint32> _controlRefPtrs; // for re-building objs array
	Common::Array<uint16> _controlDrawOrder;

	virtual Common::Point getDrawPos() { return Common::Point(_x, _y); }
	virtual int getDrawOrder() const { return 0; }
	virtual uint getDrawWidth() { return _width; }
	virtual uint getDrawHeight() { return _height; }

	virtual const Graphics::Surface *getDrawSurface();

	virtual uint getDrawTransparency() { return _transparency; }
	virtual bool isDrawVerticallyMirrored() { return 0; }
	virtual int getDrawLightLevel() { return 0; }
	virtual void getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) { }

protected:
	AGSEngine *_vm;
	Graphics::Surface _surface;

	bool _needsUpdate;

	void draw();
};

} // End of namespace AGS

#endif // AGS_GUI_H
