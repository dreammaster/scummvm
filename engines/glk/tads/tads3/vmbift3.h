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

#ifndef TADS3_VMBIFTAD_H
#define TADS3_VMBIFTAD_H

/* Function set definition - T3 VM system interface */

#include "glk/tads/tads3/vmbif.h"

#endif /* VMBIFTAD_H */

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   T3 VM function set intrinsics
 */
class CVmBifT3: public CVmBif
{
public:
    /* function vector */
    static vm_bif_desc bif_table[];

    /* run the garbage collector */
    static void run_gc(VMG_ uint argc);

    /* set the SAY instruction's handler function */
    static void set_say(VMG_ uint argc);

    /* get the VM version number */
    static void get_vm_vsn(VMG_ uint argc);

    /* get the VM identification string */
    static void get_vm_id(VMG_ uint argc);

    /* get the VM banner string */
    static void get_vm_banner(VMG_ uint argc);

    /* get the 'preinit' status - true if preinit, nil if normal */
    static void get_vm_preinit_mode(VMG_ uint argc);

    /* debug trace functions */
    static void debug_trace(VMG_ uint argc);

    /* get the global symbol table, if available */
    static void get_global_symtab(VMG_ uint argc);

    /* allocate a new property ID */
    static void alloc_new_prop(VMG_ uint argc);

    /* get a stack trace */
    static void get_stack_trace(VMG_ uint argc);

    /* look up a named argument */
    static void get_named_arg(VMG_ uint argc);

    /* get a named argument list */
    static void get_named_arg_list(VMG_ uint argc);

protected:
    /* 
     *   service routine - retrieve source information for a given code
     *   location 
     */
    static void get_source_info(VMG_ const uchar *entry_addr, ulong method_ofs,
                                vm_val_t *retval);

    /* get the local variables for a stack level */
    static void get_stack_locals(VMG_ vm_val_t *fp, const uchar *entry_addr,
                                 ulong method_ofs,
                                 vm_val_t *local_tab, vm_val_t *frameref_obj);
};

/*
 *   T3 VM Test function set - test and debug function set
 */
class CVmBifT3Test: public CVmBif
{
public:
    /* function vector */
    static vm_bif_desc bif_table[];

    /* get an instance's unique identifier number */
    static void get_obj_id(VMG_ uint argc);

    /* get an instance's garbage collection state */
    static void get_obj_gc_state(VMG_ uint argc);

    /* get the Unicode character code for the first character of a string */
    static void get_charcode(VMG_ uint argc);
};


/* ------------------------------------------------------------------------ */
/*
 *   debug_trace mode flags 
 */

/* check to see if the debugger is present */
#define T3DBG_CHECK     1

/* break into the debugger */
#define T3DBG_BREAK     2

/* write a message to the debug log */
#define T3DBG_LOG       3



/* ------------------------------------------------------------------------ */
/*
 *   Function set vector.  Define this only if VMBIF_DEFINE_VECTOR has
 *   been defined, so that this file can be included for the prototypes
 *   alone without defining the function vector.  
 */
#ifdef VMBIF_DEFINE_VECTOR

vm_bif_desc CVmBifT3::bif_table[] =
{
    { &CVmBifT3::run_gc, 0, 0, FALSE },
    { &CVmBifT3::set_say, 0, 0, FALSE },
    { &CVmBifT3::get_vm_vsn, 0, 0, FALSE },
    { &CVmBifT3::get_vm_id, 0, 0, FALSE },
    { &CVmBifT3::get_vm_banner, 0, 0, FALSE },
    { &CVmBifT3::get_vm_preinit_mode, 0, 0, FALSE },
    { &CVmBifT3::debug_trace, 1, 0, TRUE },
    { &CVmBifT3::get_global_symtab, 0, 0, FALSE },
    { &CVmBifT3::alloc_new_prop, 0, 0, FALSE },
    { &CVmBifT3::get_stack_trace, 0, 1, FALSE },
    { &CVmBifT3::get_named_arg, 1, 1, FALSE },
    { &CVmBifT3::get_named_arg_list, 0, 0, FALSE }
};

vm_bif_desc CVmBifT3Test::bif_table[] =
{
    { &CVmBifT3Test::get_obj_id, 1, 0, FALSE },
    { &CVmBifT3Test::get_obj_gc_state, 1, 0, FALSE },
    { &CVmBifT3Test::get_charcode, 1, 0, FALSE }
};

#endif /* VMBIF_DEFINE_VECTOR */

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk
