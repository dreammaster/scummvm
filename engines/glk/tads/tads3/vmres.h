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

#ifndef TADS3_VMRES_H
#define TADS3_VMRES_H

/* Resource object implementation
 *
 * A resource is a named binary byte stream stored within the image file.  To the VM, resources are opaque;
 * the VM merely maintains the resource name table, and provides access to the byte stream to the user program.
 */

#include "glk/tads/tads3/t3std.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

class CVmResource
{
    friend class CVmImageLoader;
    
public:
    CVmResource(long seek_pos, uint32_t len, size_t name_len);
    ~CVmResource();

    /* get the seek position */
    long get_seek_pos() const { return seek_pos_; }

    /* get the length of the byte stream */
    uint32_t get_len() const { return len_; }

    /* get my name */
    const char *get_name() const { return name_; }

    /* get/set next resource in list */
    CVmResource *get_next() const { return nxt_; }
    void set_next(CVmResource *nxt) { nxt_ = nxt; }

private:
    /* get my name buffer */
    char *get_name_buf() const { return name_; }

    /* seek position in image file of my binary data */
    long seek_pos_;

    /* length in bytes of my binary data stream */
    uint32_t len_;

    /* name string */
    char *name_;

    /* next resource in list */
    CVmResource *nxt_;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
