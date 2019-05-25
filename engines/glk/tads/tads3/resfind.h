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

#ifndef TADS3_RESFIND_H
#define TADS3_RESFIND_H

/* Find a multimedia resource in a tads 2 or tads 3 game file
 *
 * Searches a compiled tads 2 game file or a compiled tads 3 image file
 * for a multimedia resource of a given name.  The caller doesn't have to
 * know which tads version created the file; we'll sense the file type and
 * parse it accordingly.
 *
 * This implementation is independent of any tads 2 or tads 3 subsystem
 * except osifc, which it uses for portable file I/O and portable byte format
 * conversions.
 */

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   Resource locator.  This gives the seek offset and size of a resource
 *   within a larger file. 
 */
struct tads_resinfo
{
    /* seek location of start of resource */
    unsigned long seek_pos;

    /* size of resource in bytes */
    unsigned long siz;
};

/*
 *   Find a multimedia resource with the given name in the given file.  The
 *   file must be positioned at the start of the tads game file when we're
 *   invoked - this allows searching for a resource within a game file that
 *   is embedded in a larger file stream, since we don't care where within
 *   the osfildef stream the tads game file starts.
 *   
 *   Fills in the resource information structure with the seek offset and
 *   size of the resource in the file and returns true if the resource is
 *   found; returns false if a resource with the given name doesn't exist in
 *   the file.  
 */
int tads_find_resource_fp(osfildef *fp, const char *resname,
                          tads_resinfo *info);

/*
 *   Find a resource in a file, given the filename. 
 *   
 *   Fills in the resource information structure with the seek offset and
 *   size of the resource in the file and returns true if the resource is
 *   found; returns false if a resource with the given name doesn't exist in
 *   the file.  
 */
int tads_find_resource(const char *fname, const char *resname,
                       tads_resinfo *info);

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
