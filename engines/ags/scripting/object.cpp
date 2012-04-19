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

#include "engines/ags/scripting/scripting.h"
#include "engines/ags/constants.h"
#include "engines/ags/room.h"

namespace AGS {

// import void MergeObject(int object)
// Obsolete function for objects.
RuntimeValue Script_MergeObject(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int object = params[0]._signedValue;
	UNUSED(object);

	// FIXME
	error("MergeObject unimplemented");

	return RuntimeValue();
}

// import void SetObjectTint(int object, int red, int green, int blue, int saturation, int luminance)
// Obsolete function for objects.
RuntimeValue Script_SetObjectTint(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int object = params[0]._signedValue;
	UNUSED(object);
	int red = params[1]._signedValue;
	UNUSED(red);
	int green = params[2]._signedValue;
	UNUSED(green);
	int blue = params[3]._signedValue;
	UNUSED(blue);
	int saturation = params[4]._signedValue;
	UNUSED(saturation);
	int luminance = params[5]._signedValue;
	UNUSED(luminance);

	// FIXME
	error("SetObjectTint unimplemented");

	return RuntimeValue();
}

// import void RemoveObjectTint(int object)
// Obsolete function for objects.
RuntimeValue Script_RemoveObjectTint(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int object = params[0]._signedValue;
	UNUSED(object);

	// FIXME
	error("RemoveObjectTint unimplemented");

	return RuntimeValue();
}

// import void StopObjectMoving(int object)
// Obsolete function for objects.
RuntimeValue Script_StopObjectMoving(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("StopObjectMoving: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->stopMoving();

	return RuntimeValue();
}

// import void RunObjectInteraction (int object, CursorMode)
// Obsolete function for objects.
RuntimeValue Script_RunObjectInteraction(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	uint32 cursormode = params[1]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("RunObjectInteraction: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->runObjectInteraction(object, cursormode);

	return RuntimeValue();
}

// import int GetObjectProperty(int object, const string property)
// Obsolete function for objects.
RuntimeValue Script_GetObjectProperty(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	ScriptString *property = (ScriptString *)params[1]._object;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("GetObjectProperty: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	return vm->getIntProperty(property->getString(), vm->getCurrentRoom()->_objects[object]->_properties);
}

// import void GetObjectPropertyText(int object, const string property, string buffer)
// Obsolete function for objects.
RuntimeValue Script_GetObjectPropertyText(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	ScriptString *property = (ScriptString *)params[1]._object;
	ScriptString *buffer = (ScriptString *)params[2]._object;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("GetObjectPropertyText: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	buffer->setString(vm->getStringProperty(property->getString(), vm->getCurrentRoom()->_objects[object]->_properties));

	return RuntimeValue();
}

// import void AnimateObject(int object, int loop, int delay, int repeat)
// Obsolete function for objects.
RuntimeValue Script_AnimateObject(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	uint loop = params[1]._value;
	uint delay = params[2]._value;
	uint repeat = params[3]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("AnimateObject: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->animate(loop, delay, repeat, 0);

	return RuntimeValue();
}

// import void AnimateObjectEx(int object, int loop, int delay, int repeat, int direction, int blocking)
// Obsolete function for objects.
RuntimeValue Script_AnimateObjectEx(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._signedValue;
	uint loop = params[1]._signedValue;
	uint delay = params[2]._signedValue;
	uint repeat = params[3]._signedValue;
	uint direction = params[4]._signedValue;
	uint blocking = params[5]._signedValue;

	vm->getCurrentRoom()->_objects[object]->animate(loop, delay, repeat, direction);

	if (blocking)
		vm->blockUntil(kUntilObjCycleDone, object);

	return RuntimeValue();
}

// import void ObjectOff(int object)
// Obsolete function for objects.
RuntimeValue Script_ObjectOff(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("ObjectOff: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->setVisible(false);

	return RuntimeValue();
}

// import void ObjectOn(int object)
// Obsolete function for objects.
RuntimeValue Script_ObjectOn(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("ObjectOn: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->setVisible(true);

	return RuntimeValue();
}

// import void SetObjectBaseline(int object, int baseline)
// Obsolete function for objects.
RuntimeValue Script_SetObjectBaseline(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	int baseline = params[1]._signedValue;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("ObjectOn: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->_baseline = baseline;

	return RuntimeValue();
}

// import int GetObjectBaseline(int object)
// Obsolete function for objects.
RuntimeValue Script_GetObjectBaseline(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("GetObjectBaseline: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	if (vm->getCurrentRoom()->_objects[object]->_baseline < 1)
		return 0;

	return vm->getCurrentRoom()->_objects[object]->_baseline;
}

// import void SetObjectFrame(int object, int view, int loop, int frame)
// Obsolete function for objects.
RuntimeValue Script_SetObjectFrame(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	uint view = params[1]._signedValue;
	int loop = params[2]._signedValue;
	int frame = params[3]._signedValue;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("SetObjectFrame: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->setObjectFrame(view, loop, frame);

	return RuntimeValue();
}

// import void SetObjectGraphic(int object, int spriteSlot)
// Obsolete function for objects.
RuntimeValue Script_SetObjectGraphic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	int spriteSlot = params[1]._signedValue;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("SetObjectGraphic: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->setGraphic(spriteSlot);

	return RuntimeValue();
}

// import void SetObjectView(int object, int view)
// Obsolete function for objects.
RuntimeValue Script_SetObjectView(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	uint view = params[1]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("SetObjectView: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->setObjectView(view);

	return RuntimeValue();
}

// import void SetObjectTransparency(int object, int amount)
// Obsolete function for objects.
RuntimeValue Script_SetObjectTransparency(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	uint amount = params[1]._value;

	if (amount > 100)
		error("SetObjectTransparency: transparency value must be between 0 and 100, but got %d", amount);

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("SetObjectTransparency: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->setTransparency(amount);

	return RuntimeValue();
}

// import void MoveObject(int object, int x, int y, int speed)
// Obsolete function for objects.
RuntimeValue Script_MoveObject(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	int x = params[1]._signedValue;
	int y = params[2]._signedValue;
	int speed = params[3]._signedValue;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("MoveObject: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->move(x, y, speed, false);

	return RuntimeValue();
}

// import void MoveObjectDirect(int object, int x, int y, int speed)
// Obsolete function for objects.
RuntimeValue Script_MoveObjectDirect(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	int x = params[1]._signedValue;
	int y = params[2]._signedValue;
	int speed = params[3]._signedValue;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("MoveObjectDirect: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	vm->getCurrentRoom()->_objects[object]->move(x, y, speed, true);

	return RuntimeValue();
}

// import void SetObjectPosition(int object, int x, int y)
// Obsolete function for objects.
RuntimeValue Script_SetObjectPosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;
	int x = params[1]._signedValue;
	int y = params[2]._signedValue;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("SetObjectPosition: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	RoomObject *obj = vm->getCurrentRoom()->_objects[object];

	if (obj->_moving > 0)
		error("SetObjectPosition: cannot set position while object is moving");

	obj->_pos.x = x;
	obj->_pos.y = y;

	return RuntimeValue();
}

// import int AreObjectsColliding(int object1, int object2)
// Obsolete function for objects.
RuntimeValue Script_AreObjectsColliding(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int object1 = params[0]._signedValue;
	UNUSED(object1);
	int object2 = params[1]._signedValue;
	UNUSED(object2);

	// FIXME
	error("AreObjectsColliding unimplemented");

	return RuntimeValue();
}

// import void GetObjectName(int object, string buffer)
// Obsolete function for objects.
RuntimeValue Script_GetObjectName(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int object = params[0]._signedValue;
	UNUSED(object);
	ScriptString *buffer = (ScriptString *)params[1]._object;
	UNUSED(buffer);

	// FIXME
	error("GetObjectName unimplemented");

	return RuntimeValue();
}

// import int GetObjectX(int object)
// Obsolete function for objects.
RuntimeValue Script_GetObjectX(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("GetObjectX: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	return vm->getCurrentRoom()->_objects[object]->_pos.x;
}

// import int GetObjectY(int object)
// Obsolete function for objects.
RuntimeValue Script_GetObjectY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("GetObjectY: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	return vm->getCurrentRoom()->_objects[object]->_pos.y;
}

// import int GetObjectGraphic(int object)
// Obsolete function for objects.
RuntimeValue Script_GetObjectGraphic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int object = params[0]._signedValue;
	UNUSED(object);

	// FIXME
	error("GetObjectGraphic unimplemented");

	return RuntimeValue();
}

// import int IsObjectAnimating(int object)
// Obsolete function for objects.
RuntimeValue Script_IsObjectAnimating(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("IsObjectAnimating: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	return vm->getCurrentRoom()->_objects[object]->_cycling ? 1 : 0;
}

// import int IsObjectMoving(int object)
// Obsolete function for objects.
RuntimeValue Script_IsObjectMoving(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("IsObjectMoving: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	return vm->getCurrentRoom()->_objects[object]->_moving ? 1 : 0;
}

// import int IsObjectOn (int object)
// Obsolete function for objects.
RuntimeValue Script_IsObjectOn(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint object = params[0]._value;

	if (object >= vm->getCurrentRoom()->_objects.size())
		error("IsObjectOn: object %d is too high (only have %d)", object, vm->getCurrentRoom()->_objects.size());

	return vm->getCurrentRoom()->_objects[object]->isVisible() ? 1 : 0;
}

// import void SetObjectClickable(int object, int clickable)
// Obsolete function for objects.
RuntimeValue Script_SetObjectClickable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint objectId = params[0]._value;
	uint clickable = params[1]._value;

	if (objectId >= vm->getCurrentRoom()->_objects.size())
		error("SetObjectClickable: object %d is too high (only have %d)", objectId, vm->getCurrentRoom()->_objects.size());

	RoomObject *object = vm->getCurrentRoom()->_objects[objectId];
	if (clickable)
		object->_flags &= ~OBJF_NOINTERACT;
	else
		object->_flags |= OBJF_NOINTERACT;

	return RuntimeValue();
}

// import void SetObjectIgnoreWalkbehinds (int object, int ignore)
// Obsolete function for objects.
RuntimeValue Script_SetObjectIgnoreWalkbehinds(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint objectId = params[0]._value;
	uint ignore = params[1]._value;

	if (objectId >= vm->getCurrentRoom()->_objects.size())
		error("SetObjectIgnoreWalkbehinds: object %d is too high (only have %d)", objectId, vm->getCurrentRoom()->_objects.size());

	RoomObject *object = vm->getCurrentRoom()->_objects[objectId];
	if (ignore)
		object->_flags |= OBJF_NOWALKBEHINDS;
	else
		object->_flags &= ~OBJF_NOWALKBEHINDS;

	return RuntimeValue();
}

// Object: import function Animate(int loop, int delay, RepeatStyle=eOnce, BlockingStyle=eBlock, Direction=eForwards)
// Animates the object using its current view.
RuntimeValue Script_Object_Animate(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	uint loop = params[0]._value;
	uint delay = params[1]._value;
	uint repeatStyle = params[2]._value;
	uint blockingStyle = params[3]._value;
	uint direction = params[4]._value;

	if (direction == FORWARDS)
		direction = 0;
	else if (direction == BACKWARDS)
		direction = 1;
	else
		error("Object::Animate: invalid direction %d", direction);

	self->animate(loop, delay, repeatStyle, direction);

	if (blockingStyle == BLOCKING)
		blockingStyle = 1;
	else if (blockingStyle == IN_BACKGROUND)
		blockingStyle = 0;
	else if (blockingStyle != 0 && blockingStyle != 1)
		error("Object::Animate: invalid blocking style %d", blockingStyle);

	if (blockingStyle)
		vm->blockUntil(kUntilObjCycleDone, self->_id);

	return RuntimeValue();
}

// Object: import static Object* GetAtScreenXY(int x, int y)
// Gets the object that is on the screen at the specified co-ordinates.
RuntimeValue Script_Object_GetAtScreenXY(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;

	uint objectId = vm->getCurrentRoom()->getObjectAt(x, y);
	if (objectId == (uint)-1)
		return 0;

	return vm->getCurrentRoom()->_objects[objectId];
}

// Object: import void GetName(string buffer)
// Undocumented.
RuntimeValue Script_Object_GetName(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *buffer = (ScriptString *)params[0]._object;
	UNUSED(buffer);

	// FIXME
	error("Object::GetName unimplemented");

	return RuntimeValue();
}

// Object: import function GetPropertyText(const string property, string buffer)
// Undocumented.
RuntimeValue Script_Object_GetPropertyText(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *property = (ScriptString *)params[0]._object;
	ScriptString *buffer = (ScriptString *)params[1]._object;

	buffer->setString(vm->getStringProperty(property->getString(), self->_properties));

	return RuntimeValue();
}

// Object: import function GetProperty(const string property)
// Gets an integer Custom Property for this object.
RuntimeValue Script_Object_GetProperty(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *property = (ScriptString *)params[0]._object;

	return vm->getIntProperty(property->getString(), self->_properties);
}

// Object: import String GetTextProperty(const string property)
// Gets a text Custom Property for this object.
RuntimeValue Script_Object_GetTextProperty(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	ScriptString *property = (ScriptString *)params[0]._object;

	Common::String string = vm->getStringProperty(property->getString(), self->_properties);
	RuntimeValue ret = new ScriptMutableString(string);
	ret._object->DecRef();
	return ret;
}

// Object: import bool IsCollidingWithObject(Object*)
// Checks whether this object is colliding with another.
RuntimeValue Script_Object_IsCollidingWithObject(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	if (!params[0]._object->isOfType(sotRoomObject))
		error("Object::IsCollidingWithObject got incorrect object type (expected a RoomObject) for parameter 1");
	RoomObject *object = (RoomObject *)params[0]._object;
	UNUSED(object);

	// FIXME
	error("Object::IsCollidingWithObject unimplemented");

	return RuntimeValue();
}

// Object: import function MergeIntoBackground()
// Merges the object's image into the room background, and disables the object.
RuntimeValue Script_Object_MergeIntoBackground(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::MergeIntoBackground unimplemented");

	return RuntimeValue();
}

// Object: import function Move(int x, int y, int speed, BlockingStyle=eNoBlock, WalkWhere=eWalkableAreas)
// Starts the object moving towards the specified co-ordinates.
RuntimeValue Script_Object_Move(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;
	int speed = params[2]._signedValue;
	uint32 blockingStyle = params[3]._value;
	uint32 walkWhere = params[4]._value;

	if (walkWhere == ANYWHERE)
		walkWhere = 1;
	else if (walkWhere == WALKABLE_AREAS)
		walkWhere = 0;
	else if (walkWhere != 0 && walkWhere != 1)
		error("Object::Move: invalid walkWhere parameter %d", walkWhere);

	self->move(x, y, speed, (bool)walkWhere);

	if (blockingStyle == BLOCKING)
		blockingStyle = 1;
	else if (blockingStyle == IN_BACKGROUND)
		blockingStyle = 0;
	else if (blockingStyle != 0 && blockingStyle != 1)
		error("Object::Move: invalid blocking style %d", blockingStyle);

	if (blockingStyle)
		vm->blockUntil(kUntilObjMoveDone, self->_id);

	return RuntimeValue();
}

// Object: import function RemoveTint()
// Removes a specific object tint, and returns the object to using the ambient room tint.
RuntimeValue Script_Object_RemoveTint(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::RemoveTint unimplemented");

	return RuntimeValue();
}

// Object: import function RunInteraction(CursorMode)
// Runs the event handler for the specified event.
RuntimeValue Script_Object_RunInteraction(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	uint32 cursormode = params[0]._value;

	vm->runObjectInteraction(self->_id, cursormode);

	return RuntimeValue();
}

// Object: import function SetPosition(int x, int y)
// Instantly moves the object to have its bottom-left at the new co-ordinates.
RuntimeValue Script_Object_SetPosition(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	int y = params[1]._signedValue;

	if (self->_moving > 0)
		error("Object::SetPosition: cannot set position while object is moving");

	self->_pos.x = x;
	self->_pos.y = y;

	return RuntimeValue();
}

// Object: import function SetView(int view, int loop=-1, int frame=-1)
// Sets the object to use the specified view, ahead of doing an animation.
RuntimeValue Script_Object_SetView(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	uint view = params[0]._value;
	int loop = params[1]._signedValue;
	int frame = params[2]._signedValue;

	self->setObjectFrame(view, loop, frame);

	return RuntimeValue();
}

// Object: import function StopAnimating()
// Stops any currently running animation on the object.
RuntimeValue Script_Object_StopAnimating(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	if (self->_cycling) {
		self->_cycling = 0;
		self->_wait = 0;
	}

	return RuntimeValue();
}

// Object: import function StopMoving()
// Stops any currently running move on the object.
RuntimeValue Script_Object_StopMoving(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::StopMoving unimplemented");

	return RuntimeValue();
}

// Object: import function Tint(int red, int green, int blue, int saturation, int luminance)
// Tints the object to the specified colour.
RuntimeValue Script_Object_Tint(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	int red = params[0]._signedValue;
	UNUSED(red);
	int green = params[1]._signedValue;
	UNUSED(green);
	int blue = params[2]._signedValue;
	UNUSED(blue);
	int saturation = params[3]._signedValue;
	UNUSED(saturation);
	int luminance = params[4]._signedValue;
	UNUSED(luminance);

	// FIXME
	error("Object::Tint unimplemented");

	return RuntimeValue();
}

// Object: readonly import attribute bool Animating
// Gets whether the object is currently animating.
RuntimeValue Script_Object_get_Animating(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	return self->_cycling ? 1 : 0;
}

// Object: import attribute int Baseline
// Gets/sets the object's baseline. This can be 0 to use the object's Y position as its baseline.
RuntimeValue Script_Object_get_Baseline(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	if (self->_baseline < 1)
		return 0;

	return self->_baseline;
}

// Object: import attribute int Baseline
// Gets/sets the object's baseline. This can be 0 to use the object's Y position as its baseline.
RuntimeValue Script_Object_set_Baseline(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	self->_baseline = value;

	return RuntimeValue();
}

// Object: import attribute int BlockingHeight
// Allows you to manually specify the blocking height of the base of the object.
RuntimeValue Script_Object_get_BlockingHeight(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::get_BlockingHeight unimplemented");

	return RuntimeValue();
}

// Object: import attribute int BlockingHeight
// Allows you to manually specify the blocking height of the base of the object.
RuntimeValue Script_Object_set_BlockingHeight(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Object::set_BlockingHeight unimplemented");

	return RuntimeValue();
}

// Object: import attribute int BlockingWidth
// Allows you to manually specify the blocking width of the base of the object.
RuntimeValue Script_Object_get_BlockingWidth(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::get_BlockingWidth unimplemented");

	return RuntimeValue();
}

// Object: import attribute int BlockingWidth
// Allows you to manually specify the blocking width of the base of the object.
RuntimeValue Script_Object_set_BlockingWidth(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("Object::set_BlockingWidth unimplemented");

	return RuntimeValue();
}

// Object: import attribute bool Clickable
// Gets/sets whether the mouse can be clicked on this object or whether it passes straight through.
RuntimeValue Script_Object_get_Clickable(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	return (self->_flags & OBJF_NOINTERACT) ? 0 : 1;
}

// Object: import attribute bool Clickable
// Gets/sets whether the mouse can be clicked on this object or whether it passes straight through.
RuntimeValue Script_Object_set_Clickable(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	if (value)
		self->_flags &= ~OBJF_NOINTERACT;
	else
		self->_flags |= OBJF_NOINTERACT;

	return RuntimeValue();
}

// Object: readonly import attribute int Frame
// Gets the current frame number during an animation.
RuntimeValue Script_Object_get_Frame(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::get_Frame unimplemented");

	return RuntimeValue();
}

// Object: import attribute int Graphic
// Gets/sets the sprite number that is currently displayed on the object.
RuntimeValue Script_Object_get_Graphic(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	return self->_spriteId;
}

// Object: import attribute int Graphic
// Gets/sets the sprite number that is currently displayed on the object.
RuntimeValue Script_Object_set_Graphic(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	self->setGraphic(value);

	return RuntimeValue();
}

// Object: readonly import attribute int ID
// Gets the object's ID number.
RuntimeValue Script_Object_get_ID(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::get_ID unimplemented");

	return RuntimeValue();
}

// Object: import attribute bool IgnoreScaling
// Gets/sets whether the object ignores walkable area scaling.
RuntimeValue Script_Object_get_IgnoreScaling(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::get_IgnoreScaling unimplemented");

	return RuntimeValue();
}

// Object: import attribute bool IgnoreScaling
// Gets/sets whether the object ignores walkable area scaling.
RuntimeValue Script_Object_set_IgnoreScaling(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("Object::set_IgnoreScaling unimplemented");

	return RuntimeValue();
}

// Object: import attribute bool IgnoreWalkbehinds
// Gets/sets whether the object ignores walk-behind areas.
RuntimeValue Script_Object_get_IgnoreWalkbehinds(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	return (self->_flags & OBJF_NOWALKBEHINDS) ? 1 : 0;
}

// Object: import attribute bool IgnoreWalkbehinds
// Gets/sets whether the object ignores walk-behind areas.
RuntimeValue Script_Object_set_IgnoreWalkbehinds(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;

	if (value)
		self->_flags |= OBJF_NOWALKBEHINDS;
	else
		self->_flags &= ~OBJF_NOWALKBEHINDS;

	return RuntimeValue();
}

// Object: readonly import attribute int Loop
// Gets the current loop number during an animation.
RuntimeValue Script_Object_get_Loop(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::get_Loop unimplemented");

	return RuntimeValue();
}

// Object: readonly import attribute bool Moving
// Gets whether the object is currently moving.
RuntimeValue Script_Object_get_Moving(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	return self->_moving ? 1 : 0;
}

// Object: readonly import attribute String Name
// Gets the object's description.
RuntimeValue Script_Object_get_Name(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::get_Name unimplemented");

	return RuntimeValue();
}

// Object: import attribute bool Solid
// Gets/sets whether other objects and characters can move through this object.
RuntimeValue Script_Object_get_Solid(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::get_Solid unimplemented");

	return RuntimeValue();
}

// Object: import attribute bool Solid
// Gets/sets whether other objects and characters can move through this object.
RuntimeValue Script_Object_set_Solid(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	uint32 value = params[0]._value;
	UNUSED(value);

	// FIXME
	error("Object::set_Solid unimplemented");

	return RuntimeValue();
}

// Object: import attribute int Transparency
// Gets/sets the object's transparency.
RuntimeValue Script_Object_get_Transparency(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	return self->getTransparency();
}

// Object: import attribute int Transparency
// Gets/sets the object's transparency.
RuntimeValue Script_Object_set_Transparency(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	uint trans = params[0]._value;

	if (trans > 100)
		error("Object::set_Transparency: transparency value must be between 0 and 100, but got %d", trans);

	self->setTransparency(trans);

	return RuntimeValue();
}

// Object: readonly import attribute int View
// Gets the current view number during an animation.
RuntimeValue Script_Object_get_View(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("Object::get_View unimplemented");

	return RuntimeValue();
}

// Object: import attribute bool Visible
// Gets/sets whether the object is currently visible.
RuntimeValue Script_Object_get_Visible(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	return self->isVisible();
}

// Object: import attribute bool Visible
// Gets/sets whether the object is currently visible.
RuntimeValue Script_Object_set_Visible(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	uint value = params[0]._value;

	self->setVisible((bool)value);

	return RuntimeValue();
}

// Object: import attribute int X
// Gets/sets the X co-ordinate of the object's bottom-left hand corner.
RuntimeValue Script_Object_get_X(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	return self->_pos.x;
}

// Object: import attribute int X
// Gets/sets the X co-ordinate of the object's bottom-left hand corner.
RuntimeValue Script_Object_set_X(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	if (self->_moving > 0)
		error("Object::set_X: cannot set position while object is moving");

	self->_pos.x = value;

	return RuntimeValue();
}

// Object: import attribute int Y
// Gets/sets the Y co-ordinate of the object's bottom-left hand corner.
RuntimeValue Script_Object_get_Y(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	return self->_pos.y;
}

// Object: import attribute int Y
// Gets/sets the Y co-ordinate of the object's bottom-left hand corner.
RuntimeValue Script_Object_set_Y(AGSEngine *vm, RoomObject *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;

	if (self->_moving > 0)
		error("Object::set_Y: cannot set position while object is moving");

	self->_pos.y = value;

	return RuntimeValue();
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "MergeObject", (ScriptAPIFunction *)&Script_MergeObject, "i", sotNone },
	{ "SetObjectTint", (ScriptAPIFunction *)&Script_SetObjectTint, "iiiiii", sotNone },
	{ "RemoveObjectTint", (ScriptAPIFunction *)&Script_RemoveObjectTint, "i", sotNone },
	{ "StopObjectMoving", (ScriptAPIFunction *)&Script_StopObjectMoving, "i", sotNone },
	{ "RunObjectInteraction", (ScriptAPIFunction *)&Script_RunObjectInteraction, "ii", sotNone },
	{ "GetObjectProperty", (ScriptAPIFunction *)&Script_GetObjectProperty, "is", sotNone },
	{ "GetObjectPropertyText", (ScriptAPIFunction *)&Script_GetObjectPropertyText, "iss", sotNone },
	{ "AnimateObject", (ScriptAPIFunction *)&Script_AnimateObject, "iiii", sotNone },
	{ "AnimateObjectEx", (ScriptAPIFunction *)&Script_AnimateObjectEx, "iiiiii", sotNone },
	{ "ObjectOff", (ScriptAPIFunction *)&Script_ObjectOff, "i", sotNone },
	{ "ObjectOn", (ScriptAPIFunction *)&Script_ObjectOn, "i", sotNone },
	{ "SetObjectBaseline", (ScriptAPIFunction *)&Script_SetObjectBaseline, "ii", sotNone },
	{ "GetObjectBaseline", (ScriptAPIFunction *)&Script_GetObjectBaseline, "i", sotNone },
	{ "SetObjectFrame", (ScriptAPIFunction *)&Script_SetObjectFrame, "iiii", sotNone },
	{ "SetObjectGraphic", (ScriptAPIFunction *)&Script_SetObjectGraphic, "ii", sotNone },
	{ "SetObjectView", (ScriptAPIFunction *)&Script_SetObjectView, "ii", sotNone },
	{ "SetObjectTransparency", (ScriptAPIFunction *)&Script_SetObjectTransparency, "ii", sotNone },
	{ "MoveObject", (ScriptAPIFunction *)&Script_MoveObject, "iiii", sotNone },
	{ "MoveObjectDirect", (ScriptAPIFunction *)&Script_MoveObjectDirect, "iiii", sotNone },
	{ "SetObjectPosition", (ScriptAPIFunction *)&Script_SetObjectPosition, "iii", sotNone },
	{ "AreObjectsColliding", (ScriptAPIFunction *)&Script_AreObjectsColliding, "ii", sotNone },
	{ "GetObjectName", (ScriptAPIFunction *)&Script_GetObjectName, "is", sotNone },
	{ "GetObjectX", (ScriptAPIFunction *)&Script_GetObjectX, "i", sotNone },
	{ "GetObjectY", (ScriptAPIFunction *)&Script_GetObjectY, "i", sotNone },
	{ "GetObjectGraphic", (ScriptAPIFunction *)&Script_GetObjectGraphic, "i", sotNone },
	{ "IsObjectAnimating", (ScriptAPIFunction *)&Script_IsObjectAnimating, "i", sotNone },
	{ "IsObjectMoving", (ScriptAPIFunction *)&Script_IsObjectMoving, "i", sotNone },
	{ "IsObjectOn", (ScriptAPIFunction *)&Script_IsObjectOn, "i", sotNone },
	{ "SetObjectClickable", (ScriptAPIFunction *)&Script_SetObjectClickable, "ii", sotNone },
	{ "SetObjectIgnoreWalkbehinds", (ScriptAPIFunction *)&Script_SetObjectIgnoreWalkbehinds, "ii", sotNone },
	{ "Object::Animate^5", (ScriptAPIFunction *)&Script_Object_Animate, "iiiii", sotRoomObject },
	{ "Object::GetAtScreenXY^2", (ScriptAPIFunction *)&Script_Object_GetAtScreenXY, "ii", sotNone },
	{ "Object::GetName^1", (ScriptAPIFunction *)&Script_Object_GetName, "s", sotRoomObject },
	{ "Object::GetPropertyText^2", (ScriptAPIFunction *)&Script_Object_GetPropertyText, "ss", sotRoomObject },
	{ "Object::GetProperty^1", (ScriptAPIFunction *)&Script_Object_GetProperty, "s", sotRoomObject },
	{ "Object::GetTextProperty^1", (ScriptAPIFunction *)&Script_Object_GetTextProperty, "s", sotRoomObject },
	{ "Object::IsCollidingWithObject^1", (ScriptAPIFunction *)&Script_Object_IsCollidingWithObject, "o", sotRoomObject },
	{ "Object::MergeIntoBackground^0", (ScriptAPIFunction *)&Script_Object_MergeIntoBackground, "", sotRoomObject },
	{ "Object::Move^5", (ScriptAPIFunction *)&Script_Object_Move, "iiiii", sotRoomObject },
	{ "Object::RemoveTint^0", (ScriptAPIFunction *)&Script_Object_RemoveTint, "", sotRoomObject },
	{ "Object::RunInteraction^1", (ScriptAPIFunction *)&Script_Object_RunInteraction, "i", sotRoomObject },
	{ "Object::SetPosition^2", (ScriptAPIFunction *)&Script_Object_SetPosition, "ii", sotRoomObject },
	{ "Object::SetView^3", (ScriptAPIFunction *)&Script_Object_SetView, "iii", sotRoomObject },
	{ "Object::StopAnimating^0", (ScriptAPIFunction *)&Script_Object_StopAnimating, "", sotRoomObject },
	{ "Object::StopMoving^0", (ScriptAPIFunction *)&Script_Object_StopMoving, "", sotRoomObject },
	{ "Object::Tint^5", (ScriptAPIFunction *)&Script_Object_Tint, "iiiii", sotRoomObject },
	{ "Object::get_Animating", (ScriptAPIFunction *)&Script_Object_get_Animating, "", sotRoomObject },
	{ "Object::get_Baseline", (ScriptAPIFunction *)&Script_Object_get_Baseline, "", sotRoomObject },
	{ "Object::set_Baseline", (ScriptAPIFunction *)&Script_Object_set_Baseline, "i", sotRoomObject },
	{ "Object::get_BlockingHeight", (ScriptAPIFunction *)&Script_Object_get_BlockingHeight, "", sotRoomObject },
	{ "Object::set_BlockingHeight", (ScriptAPIFunction *)&Script_Object_set_BlockingHeight, "i", sotRoomObject },
	{ "Object::get_BlockingWidth", (ScriptAPIFunction *)&Script_Object_get_BlockingWidth, "", sotRoomObject },
	{ "Object::set_BlockingWidth", (ScriptAPIFunction *)&Script_Object_set_BlockingWidth, "i", sotRoomObject },
	{ "Object::get_Clickable", (ScriptAPIFunction *)&Script_Object_get_Clickable, "", sotRoomObject },
	{ "Object::set_Clickable", (ScriptAPIFunction *)&Script_Object_set_Clickable, "i", sotRoomObject },
	{ "Object::get_Frame", (ScriptAPIFunction *)&Script_Object_get_Frame, "", sotRoomObject },
	{ "Object::get_Graphic", (ScriptAPIFunction *)&Script_Object_get_Graphic, "", sotRoomObject },
	{ "Object::set_Graphic", (ScriptAPIFunction *)&Script_Object_set_Graphic, "i", sotRoomObject },
	{ "Object::get_ID", (ScriptAPIFunction *)&Script_Object_get_ID, "", sotRoomObject },
	{ "Object::get_IgnoreScaling", (ScriptAPIFunction *)&Script_Object_get_IgnoreScaling, "", sotRoomObject },
	{ "Object::set_IgnoreScaling", (ScriptAPIFunction *)&Script_Object_set_IgnoreScaling, "i", sotRoomObject },
	{ "Object::get_IgnoreWalkbehinds", (ScriptAPIFunction *)&Script_Object_get_IgnoreWalkbehinds, "", sotRoomObject },
	{ "Object::set_IgnoreWalkbehinds", (ScriptAPIFunction *)&Script_Object_set_IgnoreWalkbehinds, "i", sotRoomObject },
	{ "Object::get_Loop", (ScriptAPIFunction *)&Script_Object_get_Loop, "", sotRoomObject },
	{ "Object::get_Moving", (ScriptAPIFunction *)&Script_Object_get_Moving, "", sotRoomObject },
	{ "Object::get_Name", (ScriptAPIFunction *)&Script_Object_get_Name, "", sotRoomObject },
	{ "Object::get_Solid", (ScriptAPIFunction *)&Script_Object_get_Solid, "", sotRoomObject },
	{ "Object::set_Solid", (ScriptAPIFunction *)&Script_Object_set_Solid, "i", sotRoomObject },
	{ "Object::get_Transparency", (ScriptAPIFunction *)&Script_Object_get_Transparency, "", sotRoomObject },
	{ "Object::set_Transparency", (ScriptAPIFunction *)&Script_Object_set_Transparency, "i", sotRoomObject },
	{ "Object::get_View", (ScriptAPIFunction *)&Script_Object_get_View, "", sotRoomObject },
	{ "Object::get_Visible", (ScriptAPIFunction *)&Script_Object_get_Visible, "", sotRoomObject },
	{ "Object::set_Visible", (ScriptAPIFunction *)&Script_Object_set_Visible, "i", sotRoomObject },
	{ "Object::get_X", (ScriptAPIFunction *)&Script_Object_get_X, "", sotRoomObject },
	{ "Object::set_X", (ScriptAPIFunction *)&Script_Object_set_X, "i", sotRoomObject },
	{ "Object::get_Y", (ScriptAPIFunction *)&Script_Object_get_Y, "", sotRoomObject },
	{ "Object::set_Y", (ScriptAPIFunction *)&Script_Object_set_Y, "i", sotRoomObject },
};

void addObjectSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
