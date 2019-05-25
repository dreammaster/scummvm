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

#ifndef TADS3_VMSORT_H
#define TADS3_VMSORT_H

/* T3 VM quicksort implementation */

#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/vmglob.h"
#include "glk/tads/tads3/vmtype.h"
#include "glk/tads/tads3/vmrun.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   Quicksort data interface 
 */
class CVmQSortData
{
public:
    /* 
     *   sort a range; to sort the entire array, provide the indices of
     *   the first and last elements of the array, inclusive 
     */
    void sort(VMG_ size_t l, size_t r);
    
    /* 
     *   compare two elements by index - returns -1 if the first element
     *   is less than the second, 0 if they're equal, 1 if the first is
     *   greater than the second 
     */
    virtual int compare(VMG_ size_t idx_a, size_t idx_b) = 0;

    /* exchange two elements */
    virtual void exchange(VMG_ size_t idx_a, size_t idx_b) = 0;
};

/* ------------------------------------------------------------------------ */
/*
 *   Sorter implementation for sets of vm_val_t data 
 */
class CVmQSortVal: public CVmQSortData
{
public:
    CVmQSortVal()
    {
        compare_fn_.set_nil();
        descending_ = FALSE;
    }
    
    /* get/set an element */
    virtual void get_ele(VMG_ size_t idx, vm_val_t *val) = 0;
    virtual void set_ele(VMG_ size_t idx, const vm_val_t *val) = 0;
    
    /* compare */
    virtual int compare(VMG_ size_t idx_a, size_t idx_b);

    /* exchange */
    virtual void exchange(VMG_ size_t idx_a, size_t idx_b);

    /* 
     *   comparison function - if this is nil, we'll compare the values as
     *   ordinary values 
     */
    vm_val_t compare_fn_;

    /* flag: sort descending */
    int descending_;

    /* recursive native caller context */
    vm_rcdesc rc;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
