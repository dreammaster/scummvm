/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

//
//  Runtime heap/resource class
//

#ifndef AESOP_OBJECTS_H
#define AESOP_OBJECTS_H

namespace Aesop {

#define NUM_OBJECTS   2048 // Up to 2,048 objects in Eye III universe
#define NUM_ENTITIES  2000 // Up to 2,000 physical entities in Eye III universe

class AesopEngine;

class Objects {
private:
	AesopEngine *_vm;
	HRES _objList[NUM_OBJECTS];
public:
	Objects(AesopEngine *vm);

	/**
	 * Return the handle of a given object
	 */
	const HRES &operator[](int idx) { 
		assert(idx < NUM_OBJECTS);
		return _objList[idx];
	}

	/**
	 * Return index of first free object list entry within given range, or -1 if none available
	 */
	int findFreeEntry(int min, int end);

	/**
	 * Create an object at a given objlist index
	 */
	void createSOPinstance(uint name, int index);

	/**
	 * Create a SOP object and return its object list index
	 * @returns		-1 if no space in list
	 */
	int createObject(uint name);

	/**
	 * Create a SOP program object at specified object list index
	 *
	 * Dynamically assign an unused program object index if specified index == -1
	 */
	int createProgram(int index, uint name);

	/**
	 * Delete SOP object from object list
	 *
	 * Cancel any notifications for object, and release any windows allocated by object
	 */
	int destroyObject(int index);

	/**
	 * Thrash resource cache (for diagnostic purposes)
	 */
	void thrashCache();

	/**
	 * Flush resource cache (for performance-tuning purposes)
	 */
	uint flushCache(uint goal);

	/**
	 * Dump object's static context to text file
	 */
	void dumpStaticContext(uint index, TextFile *TF);
};

} // End of namespace Aesop

#endif
