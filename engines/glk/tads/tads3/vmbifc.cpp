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
    vm_bif_entry_t *entry;
    vm_bif_desc *desc;
    
    /* get the function set */
    entry = table_[set_index];

    /* if the function set is null, we can't call the function */
    if (entry == nullptr)
        err_throw_a(VMERR_UNKNOWN_FUNC_SET, 1, ERR_TYPE_TEXTCHAR, names_[set_index]);

    /* get the function pointer */
    desc = &entry->func[func_index];

    /* if the function is null, we can't call it */
    if (desc->func == 0)
        err_throw_a(VMERR_UNAVAIL_INTRINSIC, 2,
                    ERR_TYPE_TEXTCHAR, names_[set_index],
                    ERR_TYPE_INT, func_index);

    /* call the function */
    (*desc->func)(vmg_ argc);
}

/*
 *   Handle adding a function set entry that's unresolvable at load-time 
 */
void CVmBifTable::add_entry_unresolved(VMG_ const char *func_set_id)
{
    /*
     *   Since this is the call-time resolver, allow loading of the image
     *   file even though this function set is unresolved.  Store a null
     *   entry in the function set table, and store the name of the
     *   function set - we'll need this in case the program attempts to
     *   invoke a function in this function set, so that we can generate
     *   an error containing the unresolved function set name. 
     */
    table_[count_] = 0;
    names_[count_] = lib_copy_str(func_set_id);

    /* count the new entry */
    ++count_;
}

/*
 *   Get a function's descriptor 
 */
const vm_bif_desc *CVmBifTable::get_desc(uint set_index, uint func_index)
{
    vm_bif_entry_t *entry;
    vm_bif_desc *desc;

    /* validate that the set index is in range */
    if (set_index >= count_)
        return 0;

    /* get the function set, and make sure it's valid */
    entry = table_[set_index];
    if (entry == 0)
        return 0;

    /* validate that the function index is in range */
    if (func_index > entry->func_count)
        return 0;

    /* get the descriptor, and make sure there's a function pointer */
    desc = &entry->func[func_index];
    if (desc->func == 0)
        return 0;

    /* return the descriptor */
    return desc;
}

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk
