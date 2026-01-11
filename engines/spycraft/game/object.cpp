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

#define DO_DEBUG

#include "spycraft/game/dump.h"
#include "spycraft/game/object.h"
#include "spycraft/game/script.h"
#include "spycraft/game/list.h"

namespace Spycraft {

extern unsigned long gameTime;

extern int _object_allocated;
extern int _static_objects;

Object::Object() {
	name = "O";
	script = NULL;
	//	animation = NULL;
	flag = OBJECT_LOCALIZED;

	/*	if ( IsStatic ( this ) ) {
			_static_objects ++;
		}
		else	{*/
	_object_allocated++;

#ifdef _DUMP_FILE
	//if (debugFile) 
	//	debugFile->write ( "%s [STATIC] %p is consructed at %d \n", name, this, gameTime  );
		//debugFile->writeObj ( this );
#endif

#ifdef _MONON
	MonoOut("%s is constructed", name);
#endif

	//	}

}

Object::~Object() {
	if (script)
		setScript(NULL);

	//	if ( animation )
			//setAnim ( NULL );

	/*	if ( IsStatic ( this ) ) {
			_static_objects --;

	#ifdef _DUMP_FILE
			time_t theTime;
			time ( &theTime );
		//	debugFile->write ( "%s [STATIC] %p is destructed at %d /n", name, this, gameTime  );
	#endif

	#ifdef _MONON
			MonoOut ( "%s (static) is destructed", name );
	#endif

		}
		else {*/
	_object_allocated--;

	/*#ifdef _DUMP_FILE
			debugFile->deleteObj ( this );
	#endif*/

	//#ifdef _DUMP_FILE 
		//	debugFile->write ( "%s %p is destructed at %d \n", name, this, gameTime );
	//#endif

#ifdef _MONON
	MonoOut("%s is destructed", name);
#endif
	/*	if (_heapchk() != _HEAPOK)	{
			int i = 0;
			sfxPrintf("heap error");
		}*/
		//	}
}

#ifndef DOS4GW 

void *Object::operator new (size_t n) {
	return AllocPtr(n);
}

void Object::operator delete (void *obj) {
	FreePtr(obj);
}

#endif

void Object::doit() {
	if (script)
		script->doit();
}

void Object::setScript(Script *theScript) {
	if (script) {
		delete script;
		script = NULL;
	}

	if (theScript) {
		script = theScript;
		script->init(this, NULL, NULL);
	} else {
		script = NULL;
	}
}

void Object::setScript(Script *theScript, Object *whoToCue) {
	if (script)
		delete script;

	if (theScript) {
		script = theScript;
		script->init(this, whoToCue, NULL);
	} else {
		script = NULL;
	}
}

void Object::setScript(Script *theScript, Object *whoToCue, void *reg) {
	if (script)
		delete script;

	if (theScript) {
		script = theScript;
		script->init(this, whoToCue, reg);
	} else {
		script = NULL;
	}
}

void Object::dispose() {
	deathList->add(this);
}

} //  namespace Spycraft
