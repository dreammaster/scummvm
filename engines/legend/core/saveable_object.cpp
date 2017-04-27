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

#include "legend/core/saveable_object.h"
#include "legend/core/tree_item.h"
#include "legend/core/named_item.h"
#include "legend/core/project_item.h"

namespace Legend {

SaveableObject *ClassDef::create() {
	return new SaveableObject();
}

/*------------------------------------------------------------------------*/

SaveableObject::ClassListMap *SaveableObject::_classList;
SaveableObject::ClassDefList *SaveableObject::_classDefs;

#define DEFFN(T) SaveableObject *Function##T() { return new T(); } \
	ClassDef *T::_type
#define ADDFN(CHILD, PARENT) \
	CHILD::_type = new TypeTemplate<CHILD>(#CHILD, PARENT::_type); \
	_classDefs->push_back(CHILD::_type); \
	(*_classList)[#CHILD] = Function##CHILD

DEFFN(SaveableObject);
DEFFN(MessageTarget);
DEFFN(TreeItem);
DEFFN(NamedItem);
DEFFN(ProjectItem);

void SaveableObject::initClassList() {
	_classDefs = new ClassDefList();
	_classList = new ClassListMap();

	ADDFN(TreeItem, MessageTarget);
	ADDFN(NamedItem, TreeItem);
	ADDFN(ProjectItem, NamedItem);
}

void SaveableObject::freeClassList() {
	ClassDefList::iterator i;
	for (i = _classDefs->begin(); i != _classDefs->end(); ++i)
		delete *i;

	delete _classDefs;
	delete _classList;
}

SaveableObject *SaveableObject::createInstance(const Common::String &name) {
	return (*_classList)[name]();
}

void SaveableObject::save(SimpleFile *file, int indent) {
	file->writeNumberLine(0, indent);
}

void SaveableObject::load(SimpleFile *file) {
	file->readNumber();
}

void SaveableObject::saveHeader(SimpleFile *file, int indent) {
	file->writeClassStart(getType()->_className, indent);
}

void SaveableObject::saveFooter(SimpleFile *file, int indent) {
	file->writeClassEnd(indent);
}

bool SaveableObject::isInstanceOf(const ClassDef *classDef) const {
	for (ClassDef *def = getType(); def != nullptr; def = def->_parent) {
		if (def == classDef)
			return true;
	}

	return false;
}

} // End of namespace Legend
