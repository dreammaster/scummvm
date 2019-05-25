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

#ifndef TADS3_RESLOAD_H
#define TADS3_RESLOAD_H

/* Resource loader */

#include "glk/tads/tads3/t3std.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   Resource loader.  
 */

class CResLoader
{
public:
    /* create the loader */
    CResLoader();

    /* 
     *   create the loader with a root directory - we'll look for external
     *   files under this directory 
     */
    CResLoader(const char *root_dir);

    /* set the executable file name */
    void set_exe_filename(const char *exe_filename)
    {
        /* discard the old executable filename, if there is one */
        lib_free_str(exe_filename_);

        /* remember the new path */
        exe_filename_ = lib_copy_str(exe_filename);
    }

    /* delete */
    ~CResLoader();

    /* 
     *   Load a resource given a resource path.
     *   
     *   We'll start by looking for a file in the local file system matching
     *   the name 'respath'.  We interpret 'respath' as a relative URL; we'll
     *   convert it to local system conventions before looking for the local
     *   file.  The path is relative to the root resource directory, which is
     *   the directory specified when the resource loader was created.
     *   
     *   If we fail to find a local file matching the given name, we'll look
     *   for a T3 resource library file with name 'deflib', if that argument
     *   is non-null.  'deflib' is given as a URL to a file relative to the
     *   root resource directory, and should be specified WITHOUT the default
     *   ".t3r" suffix - we'll add that automatically using the appropriate
     *   local filename conventions.  If we can find this resource library,
     *   we'll look for 'respath' within the library.  Note that 'deflib' is
     *   given in URL notation - we'll convert this to local path conventions
     *   before attempting to locate the file.
     *   
     *   If we still can't find the file, and 'exerestype' is non-null, we'll
     *   look in the executable file for a resource of the given type (this
     *   should be a four-byte identifier, as used with MAKETRX).  This
     *   allows resources of different types to be bundled into the
     *   executable file.  Note that resources bundled into the executable
     *   must use the standard T3 resource-image file format.  
     */
    osfildef *open_res_file(const char *respath,
                            const char *deflib,
                            const char *exerestype);

protected:
    /*
     *   Load a resource from the executable file.  If we find the resource,
     *   returns a file handle with its seek position set to the start of
     *   the resource data.  If we can't find the resource in the
     *   executable, returns null.  
     */
    osfildef *open_exe_res(const char *respath, const char *restype);

    /*
     *   Load a resource from a resource library.  If we find the resource,
     *   returns a file handle with its seek position set to the start of the
     *   resource data.  If we can't find the library or we can't find the
     *   resource in the library, returns null. 
     */
    osfildef *open_lib_res(const char *libfile, const char *respath);
    
    /* root directory for external file searches */
    char *root_dir_;

    /* path to executable file */
    char *exe_filename_;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
