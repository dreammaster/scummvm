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

#ifndef AGS2_AC_CC_H
#define AGS2_AC_CC_H

#include "ags2/ac/gui/gui_object.h"
#include "ags2/ac/dynamic.h"
#include "ags2/lib/allegro/surface.h"

namespace AGS2 {

struct ScriptDrawingSurface;

struct CCGUIObject : AGSCCDynamicObject {
	// return the type name of the object
	const char *GetType() override {
		return "GUIObject";
	}

	/**
	 * Serialize the object into BUFFER(which is BUFSIZE bytes)
	 * return number of bytes used
	 */
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct CCCharacter : AGSCCDynamicObject {
	// return the type name of the object
	const char *GetType() override {
		return "Character";
	}

	/**
	 * Serialize the object into BUFFER(which is BUFSIZE bytes)
	 * return number of bytes used
	 */
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct CCHotspot : AGSCCDynamicObject {
	// return the type name of the object
	const char *GetType() override {
		return "Hotspot";
	}

	/**
	 * Serialize the object into BUFFER(which is BUFSIZE bytes)
	 * return number of bytes used
	 */
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct CCRegion : AGSCCDynamicObject {

	// return the type name of the object
	const char *GetType() override {
		return "Region";
	}

	/**
	 * Serialize the object into BUFFER(which is BUFSIZE bytes)
	 * return number of bytes used
	 */
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct CCInventory : AGSCCDynamicObject {
	// return the type name of the object
	const char *GetType() override {
		return "Inventory";
	}

	/**
	 * Serialize the object into BUFFER(which is BUFSIZE bytes)
	 * return number of bytes used
	 */
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct CCDialog : AGSCCDynamicObject {
	// return the type name of the object
	const char *GetType() override {
		return "Dialog";
	}

	/**
	 * Serialize the object into BUFFER(which is BUFSIZE bytes)
	 * return number of bytes used
	 */
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct CCGUI : AGSCCDynamicObject {
	// return the type name of the object
	const char *GetType() override {
		return "GUI";
	}

	/**
	 * Serialize the object into BUFFER(which is BUFSIZE bytes)
	 * return number of bytes used
	 */
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct CCObject : AGSCCDynamicObject {
	// return the type name of the object
	const char *GetType() override {
		return "Object";
	}

	/**
	 * Serialize the object into BUFFER(which is BUFSIZE bytes)
	 * return number of bytes used
	 */
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct ScriptDialogOptionsRendering : AGSCCDynamicObject {
	int x, y, width, height;
	int parserTextboxX, parserTextboxY;
	int parserTextboxWidth;
	int dialogID;
	int activeOptionID;
	ScriptDrawingSurface *surfaceToRenderTo;
	bool surfaceAccessed;

	ScriptDialogOptionsRendering();

	// return the type name of the object
	const char *GetType() override {
		return "DialogOptionsRendering";
	}

	// serialize the object into BUFFER (which is BUFSIZE bytes)
	// return number of bytes used
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;

	void Reset();
};

struct ScriptDateTime : AGSCCDynamicObject {
	int year, month, day;
	int hour, minute, second;
	int rawUnixTime;

	virtual int Dispose(const char *address, bool force);
	const char *GetType() override;
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;

	ScriptDateTime();
};

struct ScriptDrawingSurface : AGSCCDynamicObject {
	int roomBackgroundNumber;
	int dynamicSpriteNumber;
	int dynamicSurfaceNumber;
	bool isLinkedBitmapOnly;
	BITMAP *linkedBitmapOnly;
	int currentColour;
	int currentColourScript;
	int highResCoordinates;
	int modified;
	int hasAlphaChannel;
	BITMAP *abufBackup;

	virtual int Dispose(const char *address, bool force);
	const char *GetType() override;
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
	BITMAP *GetBitmapSurface();
	void StartDrawing();
	void MultiplyThickness(int *adjustValue);
	void UnMultiplyThickness(int *adjustValue);
	void MultiplyCoordinates(int *xcoord, int *ycoord);
	void FinishedDrawing();
	void FinishedDrawingReadOnly();

	ScriptDrawingSurface();
};

struct ScriptViewFrame : AGSCCDynamicObject {
	int view, loop, frame;

	virtual int Dispose(const char *address, bool force);
	const char *GetType() override;
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;

	ScriptViewFrame(int p_view, int p_loop, int p_frame);
	ScriptViewFrame();
};

struct ScriptDynamicSprite : AGSCCDynamicObject {
	int slot;

	virtual int Dispose(const char *address, bool force);
	const char *GetType() override;
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;

	ScriptDynamicSprite(int slot);
	ScriptDynamicSprite();
};

struct ScriptAudioChannel {
	int id = 0;
	int reserved = 0;
};

struct CCAudioChannel : AGSCCDynamicObject {
	const char *GetType() override;
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct CCAudioClip : AGSCCDynamicObject {
	const char *GetType() override;
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;
};

struct ScriptString : AGSCCDynamicObject, ICCStringClass {
private:
	char _emptyString[1] = { '\0' };

public:
	char *text;

	virtual int Dispose(const char *address, bool force);
	const char *GetType() override;
	int Serialize(const char *address, char *buffer, int bufsize) override;
	void Unserialize(int index, const char *serializedData, int dataSize) override;

	virtual void *CreateString(const char *fromText);

	ScriptString();
	ScriptString(const char *fromText);
};

extern const char *CreateNewScriptString(const char *fromText, bool reAllocate);

extern void DrawingSurface_Release(ScriptDrawingSurface *sds);
extern ScriptDrawingSurface *DrawingSurface_CreateCopy(ScriptDrawingSurface *sds);
extern void DrawingSurface_DrawSurface(ScriptDrawingSurface *target, ScriptDrawingSurface *source, int translev);
extern void DrawingSurface_DrawImage(ScriptDrawingSurface *sds, int xx, int yy, int slot, int trans, int width, int height);

} // namespace AGS2

#endif
