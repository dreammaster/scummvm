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

#ifndef TADS3_VMSRCF_H
#define TADS3_VMSRCF_H

/* T3 VM source file table
 *
 * Maintains a list of the source file records found in the "SRCF" block in the image file
 */

#include "glk/tads/tads3/t3std.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   Source file line record 
 */
struct CVmSrcfLine
{
    /* line number in source file */
    ulong linenum;
    
    /* code address of generated code for source line */
    ulong code_addr;
};

/*
 *   Source file entry 
 */
class CVmSrcfEntry
{
public:
    CVmSrcfEntry(int orig_index, int is_orig, size_t name_len)
    {
        /* no source lines yet */
        lines_ = 0;
        lines_cnt_ = lines_alo_ = 0;

        /* set the original index */
        orig_index_ = orig_index;

        /* remember whether this is the master entry */
        is_orig_ = is_orig;

        /* allocate space for our name buffer */
        name_buf_ = (char *)t3malloc(name_len + 1);
    }

    ~CVmSrcfEntry()
    {
        /* delete our line records, if we have any */
        if (lines_ != 0)
            t3free(lines_);

        /* delete our name buffer */
        t3free(name_buf_);
    }

    /* determine if the entry is the master record */
    int is_master() const { return is_orig_; }

    /* get my name */
    const char *get_name() const { return name_buf_; }

    /* get a pointer to my name buffer */
    char *get_name_buf() { return name_buf_; }

    /* 
     *   allocate space for source line entries - this can be used to
     *   pre-allocate space if the number of line entries is known in
     *   advance 
     */
    void alloc_line_records(ulong cnt);

    /* 
     *   Add a source line entry.  Line entries must be added in ascending
     *   order of line number. 
     */
    void add_line_record(ulong linenum, ulong code_addr);

    /*
     *   Find the code address for a given source line.  Returns zero if
     *   we are unsuccessful, non-zero if successful (an executable line
     *   can never have a source address equal to zero, since if a method
     *   is at such a low address at all, its header would take up the
     *   first few bytes, putting the first executable instruction at a
     *   non-zero address).
     *   
     *   If 'exact' is true, we'll fail unless we find an exact match;
     *   otherwise, we'll return the code address for the next executable
     *   line after the given line, or the last executable line in the
     *   file if there are no more executable lines after the given line.
     *   
     *   If we find a non-exact match, we'll update *linenum to give the
     *   actual line number where we found the executable line.  
     */
    ulong find_src_addr(ulong *linenum, int exact);

private:
    /* name buffer */
    char *name_buf_;

    /* index of original entry for this filename */
    int orig_index_;

    /* flag: I'm the original entry */
    uint is_orig_ : 1;

    /* 
     *   Line records array.  For simplicity, this is simply a single
     *   array.  This could be a little constraining for 16-bit machines,
     *   but even on a 16-bit machine, this will accommodate 8000 records,
     *   which should be enough for even fairly large source files. 
     */
    CVmSrcfLine *lines_;

    /* number of line records actually in use */
    ulong lines_cnt_;

    /* number of lines allocated */
    ulong lines_alo_;
};

/*
 *   Source file table 
 */
class CVmSrcfTable
{
public:
    CVmSrcfTable();
    ~CVmSrcfTable();

    /* clear the table */
    void clear();

    /* add a new entry */
    CVmSrcfEntry *add_entry(int orig_index, size_t name_len);

    /* get an entry given the 0-based index */
    CVmSrcfEntry *get_entry(size_t idx) const
        { return (idx < list_used_ ? list_[idx] : 0); }

    /* get the number of entries */
    size_t get_count() const { return list_used_; }

private:
    /* array of entries */
    CVmSrcfEntry **list_;

    /* number of entries currently in use in the list */
    size_t list_used_;

    /* number of entries allocated in the list */
    size_t list_alloc_;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
