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

#ifndef TADS3_VMMCREG_H
#define TADS3_VMMCREG_H

/* Metaclass Registry definitions */

/* ------------------------------------------------------------------------ */
/*
 *   Define the NORMAL version of the metaclass registration macro.  This
 *   version is used on all inclusions of metaclass header files in normal
 *   contexts, and does nothing.
 *   
 *   This macro is redefined for SPECIAL versions, where metaclass header
 *   files are included in order to build certain tables, such as the
 *   central registration table.
 */
#define VM_REGISTER_METACLASS(metaclass)


#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/vmtype.h"
#include "glk/tads/tads3/vmglob.h"

/*
 *   Include all of the core headers first, to ensure that we skip
 *   everything but the registration macros on subsequent inclusions while
 *   building the tables.  (Since each header is protected against
 *   multiple inclusion, except for the registration macros, we need only
 *   include each file once up front to ensure that these definitions
 *   won't show up again.) 
 */
#include "glk/tads/tads3/vmmccore.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   Metaclass registration entry.  Each registered metaclass defines an
 *   entry like this. 
 */
struct vm_meta_reg_t
{
    /*
     *   The CVmMetaclass object that describes the metaclass 
     */
    class CVmMetaclass **meta;
};

/* ------------------------------------------------------------------------ */
/*
 *   Declare the global static table 
 */
extern vm_meta_reg_t G_meta_reg_table[];


/* ------------------------------------------------------------------------ */
/*
 *   Register metaclasses.  This must be called during VM initialization
 *   to assign each metaclass its registration index.  
 */
void vm_register_metaclasses();

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
