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

#ifndef TADS3_VMVSN_H
#define TADS3_VMVSN_H

/* VM Version Information */

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   The VM version number.  A VM program can obtain this value through
 *   the get_vm_vsn() function in the T3VM intrinsic function set.
 *   
 *   The value is encoded as a 32-bit value with the major version number
 *   in the high-order 16 bits, the minor version number in the next 8
 *   bits, and the patch release number in the low-order 8 bits.  So, the
 *   release 1.2.3 would be encoded as 0x00010203.  
 */
#define MAKE_VERSION_NUMBER(major,minor,maint) \
    (((major) << 16) | ((minor) << 8) | (maint))
#define T3VM_VSN_NUMBER  MAKE_VERSION_NUMBER(3,1,3)

/*
 *   The VM identification string 
 */
#define T3VM_IDENTIFICATION "mjr-T3"

/*
 *   The VM short version string.  This contains merely the version number,
 *   in display format.  
 */
#define T3VM_VSN_STRING "3.1.3"

/*
 *   The VM banner string.  A VM program can obtain this value through the
 *   get_vm_banner() function in the T3VM intrinsic function set. 
 */
/* copyright-date-string */
#define T3VM_BANNER_STRING \
    "T3 VM " T3VM_VSN_STRING " - Copyright 1999, 2012 Michael J. Roberts"

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
