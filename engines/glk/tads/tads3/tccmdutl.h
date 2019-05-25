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

#ifndef TADS3_TCCMDUTL_H
#define TADS3_TCCMDUTL_H

/* TADS 3 Compiler command-line parsing utilities
 *
 * Defines some utility functions for command-line parsing.
 */

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   Command utilities class.  We use a class to group these functions into
 *   a class namespace
 */
class CTcCommandUtil
{
public:
    /* get an option argument */
    static char *get_opt_arg(int argc, char **argv, int *curarg, int optlen);

    /* parse an options file */
    static int parse_opt_file(osfildef *fp, char **argv,
                              class CTcOptFileHelper *helper);
};

/*
 *   Helper interface for parse_opt_file helper.  An implementation of this
 *   interface must be provided to parse_opt_file().
 */
class CTcOptFileHelper
{
public:
    /* 
     *   Allocate an option string.  Allocates the given number of bytes of
     *   memory.  Strings allocated with this must be freed with
     *   free_opt_file_str().  
     */
    virtual char *alloc_opt_file_str(size_t len) = 0;

    /* free a string allocated with alloc_opt_file_str() */
    virtual void free_opt_file_str(char *str) = 0;

    /* 
     *   Process a comment line.  We call this for each line that we find
     *   starting with "#" and for each blank line.
     */
    virtual void process_comment_line(const char *str) = 0;

    /* process a non-comment line */
    virtual void process_non_comment_line(const char *str) = 0;

    /* 
     *   Process a configuration line.  Once we see a configuration flag (a
     *   line reading "[Config:xxx]"), we'll process all subsequent text in
     *   the file through this function.
     *   
     *   'config_id' is the 'xxx' value in the [Config:xxx] configuration
     *   flag line that started the section.  'is_id_line' is true if we're
     *   processing the "[Config:xxx]" line itself, false if it's any other
     *   line within the [Config:xxx] section.
     *   
     *   Note that this routine receives ALL lines within a configuration
     *   section, including comment lines.  Once we're inside a configuration
     *   block, the entire contents are opaque to the generic processor,
     *   since even the comment format is up to the configuration section's
     *   owner to define.  
     */
    virtual void process_config_line(const char *config_id,
                                     const char *str,
                                     int is_id_line) = 0;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
