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

#ifndef TADS3_VMINIT_H
#define TADS3_VMINIT_H

/* initialize and terminate a VM session
 *
 * VM initialization is configurable via the linker.  To select a configuration,
 * link in exactly one of the vmcfgxxx.cpp object files.
 */

#include "glk/tads/tads3/vmglob.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   Generic initialization function.  Client code calls this function to
 *   initialize the VM.  The actual implementation of this function is
 *   selected at link time from the several provided configurations.
 *   
 *   'charset' is the name of the display character mapping that we should
 *   use.  If 'charset' is null, we'll use the default character set
 *   obtained from the OS layer.  
 */
void vm_initialize(struct vm_globals **vmg,
                   const struct vm_init_options *opts);

/*
 *   Initialization options - this structure is passed to vm_initialize to
 *   specify the configuration.  (We pass this as a structure, rather than as
 *   individual parameters, because we internally have several different
 *   implementations of the initialization function that we choose from based
 *   on the VM link-time configuration.  Packaging these arguments as a
 *   structure generally lets us avoid changes to the various implementations
 *   when we add new configuration parameters.)  
 */
struct vm_init_options
{
    vm_init_options(class CVmHostIfc *hi, class CVmMainClientIfc *ci,
                    const char *cs = 0, const char *lcs = 0)
    {
        hostifc = hi;
        clientifc = ci;
        charset = cs;
        log_charset = lcs;
    }
    
    /* the host interface */
    class CVmHostIfc *hostifc;

    /* the client interface */
    class CVmMainClientIfc *clientifc;

    /* the display character set (or null to use the OS default) */
    const char *charset;

    /* the log file character set (or null to use the OS default) */
    const char *log_charset;
};

/*
 *   Initialization, phase 2 - just before loading the image file 
 */
void vm_init_before_load(VMG_ const char *image_filename);

/*
 *   Initialization, phase 3 - just after loading the image file 
 */
void vm_init_after_load(VMG0_);

/*
 *   Terminate the VM.  Deletes all objects created by vm_init().  This
 *   can be used to clean up memory after a program has finished
 *   executing.  
 */
void vm_terminate(struct vm_globals *vmg, class CVmMainClientIfc *clientifc);


/* ------------------------------------------------------------------------ */
/*
 *   Private interface.  Client code does not call any of the following
 *   routines; they're for use internally by the initialization mechanism
 *   only. 
 */

/*
 *   Initialize the VM with in-memory pools.  Creates all global objects
 *   necessary for the VM to run.  
 */
void vm_init_in_mem(struct vm_globals **vmg, const vm_init_options *opts);

/* initialize the VM with the "flat" memory pool manager */
void vm_init_flat(struct vm_globals **vmg, const vm_init_options *opts);

/*
 *   Initialize the VM with swapping pools with the given maximum number
 *   of pages in memory.  
 */
void vm_init_swap(struct vm_globals **vmg, size_t max_mem_pages,
                  const vm_init_options *opts);

/*
 *   Internal base initialization routine.  Clients do not call this
 *   routine directly; it's for use by vm_init_in_mem(), vm_init_swap(),
 *   and any other vm_init_xxx routines.
 */
void vm_init_base(struct vm_globals **vmg, const vm_init_options *opts);

/*
 *   Initialize debugger-related objects.  For non-debug versions, this
 *   doesn't need to do anything.  
 */
void vm_init_debugger(VMG0_);

/*
 *   Get the amount of stack space to allocate as a reserve for handling
 *   stack overflows in the debugger.  For non-debug versions, we don't
 *   normally use a reserve, since the reserve is only useful for manually
 *   recovering from stack overflows in the debugger.  
 */
size_t vm_init_stack_reserve();

/*
 *   Termination - shut down the debugger.  Notifies the debugger UI that
 *   we're terminating the executable.  
 */
void vm_terminate_debug_shutdown(VMG0_);

/*
 *   termination - delete debugger-related objects, if any were allocated
 *   in vm_init_debugger() 
 */
void vm_terminate_debug_delete(VMG0_);

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
