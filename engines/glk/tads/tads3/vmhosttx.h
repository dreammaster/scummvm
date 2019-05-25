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

#ifndef TADS3_VMHOSTTX_H
#define TADS3_VMHOSTTX_H

/* Host interface base class for text-only implementations
 *
 * Provides a base class for text-only T3 VM Host Interface implementations.
 * In particular, this class provides a simple resource manager implementation,
 * which is usually needed in text-only interpreter systems because there is no
 * larger host application (such as HTML TADS) that provides its own resource manager.
 *
 * Our resource manager implementation simply stores the names of the embedded resources
 * in a hash table.
 */

#include "glk/tads/tads3/vmhost.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

class CVmHostIfcText: public CVmHostIfc
{
public:
    CVmHostIfcText();
    ~CVmHostIfcText();

    /* set the image file name */
    virtual void set_image_name(const char *fname);

    /* set the resource directory */
    virtual void set_res_dir(const char *dir);

    /* add a resource file */
    virtual int add_resfile(const char *fname);

    /* we do support external resource files */
    virtual int can_add_resfiles() { return TRUE; }

    /* add a resource */
    virtual void add_resource(unsigned long ofs, unsigned long siz,
                              const char *resname, size_t resnamelen,
                              int fileno);

    /* add a resource file link */
    virtual void add_resource(const char *fname, size_t fnamelen,
                              const char *resname, size_t resnamelen);

    /* find a resource */
    virtual osfildef *find_resource(const char *resname, size_t resname_len,
                                    unsigned long *res_size);

    /* determine if a resource exists */
    virtual int resfile_exists(const char *resname, size_t resnamelen);

protected:
    /* resource map hash table */
    class CVmHashTable *restab_;

    /* array of external resource bundle filenames */
    char **ext_;
    size_t ext_cnt_;
    size_t ext_max_;

    /* resource root directory */
    char *res_dir_;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
