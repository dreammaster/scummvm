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

#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/vmtype.h"
#include "glk/tads/tads3/vmerr.h"
#include "glk/tads/tads3/vmerrnum.h"
#include "glk/tads/tads3/vmglob.h"
#include "glk/tads/tads3/vmbif.h"
#include "glk/tads/tads3/vmbifreg.h"
#include "glk/tads/tads3/vmstr.h"
#include "glk/tads/tads3/vmobj.h"
#include "glk/tads/tads3/vmrun.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   Call the given function from the given function set.  
 */
void CVmBifTable::call_func(VMG_ uint set_index, uint func_index, uint argc)
{
    /* 
     *   find the function set in the registration table, get the function
     *   descriptor from the set's function table, and invoke the function
     *   pointer from the function descriptor 
     */
    (*table_[set_index]->func[func_index].func)(vmg_ argc);
}

/*
 *   Get a function's descriptor 
 */
const vm_bif_desc *CVmBifTable::get_desc(uint set_index, uint func_index)
{
    /* 
     *   find the function in the registration table, and return the function
     *   descriptor from the set's function table 
     */
    return &table_[set_index]->func[func_index];
}


/*
 *   Handle adding a function set entry that's unresolvable at load-time 
 */
void CVmBifTable::add_entry_unresolved(VMG_ const char *func_set_id)
{
    /* this is the static-link version, so an unresolved entry is an error */
    err_throw_a(VMERR_UNKNOWN_FUNC_SET, 3,
                ERR_TYPE_TEXTCHAR, func_set_id,
                ERR_TYPE_FUNCSET, func_set_id,
                ERR_TYPE_VERSION_FLAG);
}

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk
