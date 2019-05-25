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

#ifndef TADS3_VMCOLL_H
#define TADS3_VMCOLL_H

/* T3 Collection base class
 *
 * A Collection is the base class for List, Array, and other objects providing a collection
 * of objects that can be iterated via an Iterator.
 *
 * Collection is an abstract base class: it cannot be instantiated, and thus has no image-file
 * or state-file representation.
*/

#include "glk/tads/tads3/vmtype.h"
#include "glk/tads/tads3/vmobj.h"
#include "glk/tads/tads3/vmglob.h"
#include "glk/tads/tads3/vmerr.h"
#include "glk/tads/tads3/vmerrnum.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

class CVmObjCollection: public CVmObject
{
    friend class CVmMetaclassCollection;
    
public:
    /* metaclass registration object */
    static class CVmMetaclass *metaclass_reg_;
    class CVmMetaclass *get_metaclass_reg() const { return metaclass_reg_; }

    /* am I of the given metaclass? */
    virtual int is_of_metaclass(class CVmMetaclass *meta) const
    {
        /* try my own metaclass and my base class */
        return (meta == metaclass_reg_
                || CVmObject::is_of_metaclass(meta));
    }

    /* 
     *   call a static property - we don't have any of our own, so simply
     *   "inherit" the base class handling 
     */
    static int call_stat_prop(VMG_ vm_val_t *result,
                              const uchar **pc_ptr, uint *argc,
                              vm_prop_id_t prop)
        { return CVmObject::call_stat_prop(vmg_ result, pc_ptr, argc, prop); }

    /* get a property */
    int get_prop(VMG_ vm_prop_id_t prop, vm_val_t *retval,
                 vm_obj_id_t self, vm_obj_id_t *source_obj, uint *argc);

    /* 
     *   constant value property evaluator - this allows us to evaluate a
     *   property for an object value or for a constant value using the
     *   same code 
     */
    int const_get_coll_prop(VMG_ vm_prop_id_t prop, vm_val_t *retval,
                            const vm_val_t *self_val, vm_obj_id_t *src_obj,
                            uint *argc);

protected:
    /* 
     *   Create an iterator for this collection.  Fills in *retval with a
     *   reference to the new iterator object.  This iterator must refer
     *   to an immutable snapshot of the collection.  
     */
    virtual void new_iterator(VMG_ vm_val_t *retval,
                              const vm_val_t *self_val) = 0;

    /* 
     *   Create a "live" iterator for this collection.  Fills in *retval
     *   with a reference to the new iterator object.  This iterator must
     *   refer to the original "live" collection.  
     */
    virtual void new_live_iterator(VMG_ vm_val_t *retval,
                                   const vm_val_t *self_val) = 0;

    /* property evaluator - undefined property */
    int getp_undef(VMG_ vm_val_t *, const vm_val_t *, uint *)
        { return FALSE; }
    
    /* property evaluator - create iterator */
    int getp_create_iter(VMG_ vm_val_t *retval, const vm_val_t *self_val,
                         uint *argc);

    /* property evaluator - create live iterator */
    int getp_create_live_iter(VMG_ vm_val_t *retval,
                              const vm_val_t *self_val, uint *argc);

    /* property evaluation function table */
    static int (CVmObjCollection::*func_table_[])(VMG_ vm_val_t *retval,
        const vm_val_t *self_val, uint *argc);
};


/* ------------------------------------------------------------------------ */
/*
 *   Registration table object 
 */
class CVmMetaclassCollection: public CVmMetaclass
{
public:
    /* get the global name */
    const char *get_meta_name() const { return "collection/030000"; }

    /* create from image file */
    void create_for_image_load(VMG_ vm_obj_id_t id)
        { err_throw(VMERR_BAD_STATIC_NEW); }

    /* create from restoring from saved state */
    void create_for_restore(VMG_ vm_obj_id_t id)
        { err_throw(VMERR_BAD_STATIC_NEW); }

    /* create dynamically using stack arguments */
    vm_obj_id_t create_from_stack(VMG_ const uchar **pc_ptr, uint argc)
    {
        err_throw(VMERR_BAD_DYNAMIC_NEW);
        AFTER_ERR_THROW(return VM_INVALID_OBJ;)
    }

    /* call a static property */
    int call_stat_prop(VMG_ vm_val_t *result,
                       const uchar **pc_ptr, uint *argc,
                       vm_prop_id_t prop)
    {
        return CVmObjCollection::
            call_stat_prop(vmg_ result, pc_ptr, argc, prop);
    }
};


/*
 *   Register the class 
 */
VM_REGISTER_METACLASS(CVmObjCollection)

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
