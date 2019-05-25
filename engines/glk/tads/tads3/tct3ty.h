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

#ifndef TADS3_TCT3TY_H
#define TADS3_TCT3TY_H

/* T3 Target-specific Type definitions */

#include "glk/tads/tads3/vmtype.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   target type specifications 
 */

/* global object ID */
typedef vm_obj_id_t tctarg_obj_id_t;

/* global property ID */
typedef vm_prop_id_t tctarg_prop_id_t;

/* invalid object/property ID's */
const tctarg_obj_id_t TCTARG_INVALID_OBJ = VM_INVALID_OBJ;
const tctarg_prop_id_t TCTARG_INVALID_PROP = VM_INVALID_PROP;

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
