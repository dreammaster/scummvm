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

#ifndef TADS3_VMSTRRES_H
#define TADS3_VMSTRRES_H

/* Defines the ID's for string resources.  These strings must be loaded when needed with os_get_str_rsc(),
 * since the specific mechanism by which these strings are loaded varies by operating system.
 *
 * We assign the resources sequential numbers starting at 1, to make it easier to map the resource loader
 * to an operating system mechanism where such a mechanism exists.
 */

#include "glk/tads/tads3/glk/tads/tads.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   Dialog buttons.  These provide the text for standard buttons in
 *   dialogs created with os_input_dialog().
 *   
 *   These labels can use "&" to indicate a shortcut letter, per the
 *   normal os_input_dialog() interface; for example, if the Yes button
 *   label is "&Yes", the button has the shortcut letter "Y".
 *   
 *   The text of these buttons may vary by system, since these should
 *   conform to local conventions where there are local conventions.  In
 *   addition, of course, these strings will vary by language.  
 */

/* OK and Cancel buttons */
#define VMRESID_BTN_OK       1
#define VMRESID_BTN_CANCEL   2

/* "Yes" and "No" buttons */
#define VMRESID_BTN_YES      3
#define VMRESID_BTN_NO       4

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif

