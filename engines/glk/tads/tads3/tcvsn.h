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

/* TADS 3 Compiler version information
 *
 * Defines the version number information for the TADS 3 compiler. Note that the compiler
 * version numbers need not match the VM or interpreter version numbers; we maintain a
 * separate version numbering system for the compiler.
*/

#ifndef TADS3_TCVSN_H
#define TADS3_TCVSN_H

namespace Glk {
namespace TADS {
namespace TADS3 {

/* compiler major/minor/revision version number */
#define TC_VSN_MAJOR  3
#define TC_VSN_MINOR  1
#define TC_VSN_REV    3

/*
 *   Patch number.  This is an even finer release number that can be used for
 *   bug-fix releases or other releases that don't warrant a revision number
 *   increase, but still must be identified as separate builds.  This is only
 *   shown if it's non-zero.  
 */
#define TC_VSN_PATCH  0

/* 
 *   Development build iteration - this is a sub-sub-sub version number that
 *   we use to indicate unofficial interim builds that don't get their own
 *   release numbers.  We use this to distinguish these interim builds
 *   without incrementing the official release numbers.  This is only
 *   displayed if it's non-zero, and is displayed as a lower-case letter
 *   (1='a' 2='b', etc).  For official releases, this is set to zero.  
 */
#define TC_VSN_DEVBUILD  0

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
