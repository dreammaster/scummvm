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

#ifndef TADS3_VMSAVE_H
#define TADS3_VMSAVE_H

/* Save/restore VM state
 *
 * Saves/restores VM state to/from a file.
 */

#include "glk/tads/tads3/vmglob.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   Save/restore API.  (This class is all static methods, so it's never
 *   instatiated; it's really just a namespace.)
 */
class CVmSaveFile
{
public:
    /* 
     *   Save state to a file.  Writes the state information to the given
     *   open file stream.
     *   
     *   'metadata' is an optional LookupTable object containing
     *   string->string associations.  We'll write each key/value pair to the
     *   save file in the metadata header section.  This section allows the
     *   interpreter and external tools to display user-readable information
     *   on the saved game.  For example, you could store things like the
     *   current location, chapter number, score, etc - things that would
     *   help the user identify the context of the saved game when looking
     *   for the one he/she wishes to restore.  
     */
    static void save(VMG_ class CVmFile *fp,
                     class CVmObjLookupTable *metadata);

    /* 
     *   given a saved state file, read the name of the image file that
     *   created it 
     */
    static int restore_get_image(osfildef *fp,
                                 char *fname_buf, size_t fname_buf_len);

    /* 
     *   Restore state from a file.  Returns zero on success, or a
     *   VMERR_xxx code indicating the problem on failure.  
     */
    static int restore(VMG_ class CVmFile *fp);

    /* reset the VM to the initial image file state */
    static void reset(VMG0_);

protected:
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
