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

#ifndef TADS3_VMPOOLSL_H
#define TADS3_VMPOOLSL_H

/* Pool manager selector definitions
 *
 * This header makes some definitions that vary according to which type of memory pool manager
 * is selected.  This can be included without including the entire memory pool subsystem header.
 *
 * As of this writing, only the all-in-memory pool manager is supported. The swapping pool manager
 * is no longer supported and is not planned to be supported in the future.  However, we are retaining
 * the selection mechanism for now, in the event that we want to restore the swapping pool, or add
 * other pool configuration options, in the future (which seems unlikely, but still).
*/

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   Conditionally include code needed for the swapping pool manager.  When
 *   the swapping pool manager is used, code that translates memory addresses
 *   must be mindful of the fact that translating one memory address can
 *   render previously-translated addresses invalid.  Such code is
 *   unnecessary with non-swapping pool managers.
 *   
 *   To include the swapping pool manager in the build, you must #define
 *   VM_SWAPPING_POOL globally for all modules - normally, this should be
 *   done in the CFLAGS in the makefile, or with the equivalent local
 *   convention, so that every module has the symbol defined.  
 */

/* ------------------------------------------------------------------------ */
#ifdef VM_SWAPPING_POOL

/* 
 *   swapping mode - include swapping-specific code 
 */
#define VM_IF_SWAPPING_POOL(x)  x

/* the final pool manager subclass - use the swapping pool class */
#define CVmPool_CLASS CVmPoolSwap

/* ------------------------------------------------------------------------ */
#else /* VM_SWAPPING_POOL */

/* 
 *   non-swapping mode - omit swapping-specific code 
 */
#define VM_IF_SWAPPING_POOL(x)

/* 
 *   The non-swapping pool comes in two varieties.  Select the FLAT or PAGED
 *   pool, as desired.  The FLAT pool is slightly faster, but it doesn't have
 *   any dynamic memory capabilities, which are required for the debugger.  
 */
#ifdef VM_FLAT_POOL

/* select the non-swapped FLAT pool */
#define CVmPool_CLASS CVmPoolFlat

#else /* VM_FLAT_POOL */

/* select the non-swapped PAGED pool */
#define CVmPool_CLASS CVmPoolInMem

#endif /* VM_FLAT_POOL */

#endif /* VM_SWAPPING_POOL */

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
