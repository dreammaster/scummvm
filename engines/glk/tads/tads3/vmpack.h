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

#ifndef TADS3_VMPACK_H
#define TADS3_VMPACK_H

/* Binary data stream pack/unpack
 *
 * Converts between a binary data stream and TADS types, using a list of type codes
 * to drive the conversion.  This is a convenient way of reading and writing structured
 * binary files.  Our implementation uses generic data streams, so it can work with various
 * underlying data sources; in particular, we provide File and ByteArray interfaces.
*/

#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/vmtype.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

struct CVmPackPos;
struct CVmPackArgs;
struct CVmPackGroup;
struct CVmPackType;
class CVmDataSource;
class CVmObjList;

class CVmPack {
public:
    /*
     *   Pack data from stack arguments into the given stream.  'arg_index'
     *   is the index in the stack of the format string argument; the data
     *   value arguments come after it on the stack.  'argc' is the total
     *   number of arguments, including the format string.  
     */
    static void pack(VMG_ int arg_index, int argc, 
                     CVmDataSource *dst);

    /*
     *   Unpack data from the given stream into a list.  
     */
    static void unpack(VMG_ vm_val_t *retval, const char *fmt, size_t fmtlen,
                       CVmDataSource *src);


    /* write padding bytes */
    static void write_padding(CVmDataSource *dst, char ch, long cnt);
    static void write_padding(CVmDataSource *dst, char ch1, char ch2, long cnt);

protected:
    /* 
     *   Pack a group.  pack() calls this to start packing the main string,
     *   and we recurse for each parenthesized group we find.  
     */
    static void pack_group(VMG_ CVmPackPos *pos,
                           CVmPackArgs *args,
                           CVmPackGroup *group,
                           int list_per_iter);

    /* pack a subgroup */
    static void pack_subgroup(VMG_ CVmPackPos *p,
                              CVmPackArgs *args,
                              CVmPackGroup *parent,
                              CVmPackType *prefix_count);

    /* pack an item or iterated list of items */
    static void pack_item(VMG_ CVmPackGroup *group,
                          CVmPackArgs *args,
                          CVmPackType *typ,
                          CVmPackType *prefix_count);

    /* pack a single item */
    static void pack_one_item(VMG_ CVmPackGroup *group,
                              CVmPackArgs *args,
                              CVmPackType *t,
                              CVmPackType *prefix_count);

    /* unpack a group */
    static void unpack_group(VMG_ CVmPackPos *p,
                             CVmObjList *retlst, int *retcnt,
                             CVmPackGroup *group, int list_per_iter);

    /* unpack a subgroup */
    static void unpack_subgroup(VMG_ CVmPackPos *p,
                                CVmObjList *retlst, int &retcnt,
                                CVmPackGroup *group,
                                const int *prefix_count);

    /* create a sublist for unpacking a "[ ]!" group */
    static CVmObjList *create_group_sublist(
        VMG_ CVmObjList *parent_list, int *parent_cnt);

    /* unpack an iterated item */
    static void unpack_iter_item(VMG_ CVmPackPos *p,
                                 CVmObjList *retlst, int &retcnt,
                                 CVmPackGroup *group,
                                 const int *prefix_count);

    /* unpack an item into the list */
    static void unpack_into_list(VMG_ CVmObjList *retlst, int &retcnt,
                                 CVmDataSource *src,
                                 const CVmPackType *t,
                                 const CVmPackGroup *group);

    /* unpack a single item */
    static void unpack_item(VMG_ vm_val_t *val,
                            CVmDataSource *src,
                            const CVmPackType *t,
                            const CVmPackGroup *group);

    /* parse a type code; increments the pointer past the parsed input */
    static void parse_type(CVmPackPos *p, CVmPackType *info,
                           const CVmPackGroup *group);

    /* 
     *   parse the suffix modifiers for a group, given a pointer to the
     *   opening parenthesis of the group 
     */
    static void parse_group_mods(const CVmPackPos *p,
                                 CVmPackType *gt);

    /* find the close parenthesis/bracket of a group */
    static void skip_group(CVmPackPos *p, wchar_t close_paren);

    /* parse and skip a group and its modifiers */
    static void skip_group_mods(CVmPackPos *p, CVmPackType *gt);

    /* parse suffix modifiers */
    static void parse_mods(CVmPackPos *p, CVmPackType *t);
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
