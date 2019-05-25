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

#ifndef TADS3_VMPREINI_H
#define TADS3_VMPREINI_H

/* Preinit */

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   Run preinitialization.  Loads the image file, invokes its main
 *   entrypoint, and saves the new file. 
 */
void vm_run_preinit(class CVmFile *origfp, const char *orig_image_fname,
                    class CVmFile *newfp, class CVmHostIfc *hostifc,
                    class CVmMainClientIfc *clientifc,
                    const char *const *argv, int argc,
                    class CVmRuntimeSymbols *global_symtab,
                    class CVmRuntimeSymbols *macros);

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
