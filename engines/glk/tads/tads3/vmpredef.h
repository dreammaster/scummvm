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

#ifndef TADS3_VMPREDEF_H
#define TADS3_VMPREDEF_H

/* Pre-defined objects and properties
 *
 * Defines a global structure containing the pre-defined objects and properties.  These values
 * are provided by the load image file's predefined value symbol table; at load time,
 * we cache these values for quick access.
*/

#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/vmtype.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   some special import names - these are exports that we might need to
 *   synthesize, so we need to know their names in the image loader code as
 *   well as in the imports table 
 */
#define VM_IMPORT_NAME_LASTPROPOBJ   "*LastPropObj"
#define VM_IMPORT_NAME_RTERRMSG      "exceptionMessage"
#define VM_IMPORT_NAME_CONSTSTR      "*ConstStrObj"
#define VM_IMPORT_NAME_CONSTLST      "*ConstLstObj"
#define VM_IMPORT_NAME_ISARGTABLE    "_isNamedArgTable"

/*
 *   pre-defined values 
 */
struct CVmPredef
{
    /*
     *   Now include the import list to generate the actual member variables
     *   for the imported symbols.  During image file load, the loader will
     *   set these members to the actual values imported from the image
     *   file.  
     */
#define VM_IMPORT_OBJ(sym, mem) vm_obj_id_t mem;
#define VM_NOIMPORT_OBJ(sym, mem) VM_IMPORT_OBJ(sym, mem)

#define VM_IMPORT_PROP(sym, mem) vm_prop_id_t mem;
#define VM_NOIMPORT_PROP(sym, mem) VM_IMPORT_PROP(sym, mem)

#define VM_IMPORT_FUNC(sym, mem) pool_ofs_t mem;

#include "glk/tads/tads3/vmimport.h"

	/**
	 * initialize
	 */
	CVmPredef() {
		// the pre-defined variables are all undefined initially
		reset();
	}

	/**
	 * Reset the predef variables to their initial undefined values
	 */
	void reset() {
		/*
		 * include the import list to generate initializations for the
		 * predef variables
		 */
#define VM_IMPORT_OBJ(sym, mem) mem = VM_INVALID_OBJ;
#define VM_NOIMPORT_OBJ(sym, mem) VM_IMPORT_OBJ(sym, mem)

#define VM_IMPORT_PROP(sym, mem) mem = VM_INVALID_PROP;
#define VM_NOIMPORT_PROP(sym, mem) VM_IMPORT_PROP(sym, mem)

#define VM_IMPORT_FUNC(sym, mem) mem = 0;

#include "glk/tads/tads3/vmimport.h"
	}
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
