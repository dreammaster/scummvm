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

#ifndef TADS3_VMBIFTIX_H
#define TADS3_VMBIFTIX_H

/* Function set definition - TADS I/O Extensions
 *
 * The I/O Extensions function set provides access to some UI functionality beyond
 * the basic tads-io set.
*/

namespace Glk {
namespace TADS {
namespace TADS3 {

#include "glk/tads/os_glk.h"
#include "glk/tads/tads3/vmbif.h"
#include "glk/tads/tads3/utf8.h"

class CVmBifTIOExt: public CVmBif
{
public:
    /* function vector */
    static vm_bif_desc bif_table[];

    /* show a popup menu */
    static void show_popup_menu(VMG_ uint argc);

    /* enable/disable system menu command */
    static void enable_sys_menu_cmd(VMG_ uint argc);
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif

/* ------------------------------------------------------------------------ */
/*
 *   Function set vector.  Define this only if VMBIF_DEFINE_VECTOR has been
 *   defined, so that this file can be included for the prototypes alone
 *   without defining the function vector.
 *   
 *   Note that this vector is specifically defined outside of the section of
 *   the file protected against multiple inclusion.  
 */
#ifdef VMBIF_DEFINE_VECTOR

namespace Glk {
namespace TADS {
namespace TADS3 {

/* TADS input/output extension functions */
vm_bif_desc CVmBifTIOExt::bif_table[] =
{
    { &CVmBifTIOExt::show_popup_menu, 3, 0, FALSE },
    { &CVmBifTIOExt::enable_sys_menu_cmd, 2, 0, FALSE }
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
