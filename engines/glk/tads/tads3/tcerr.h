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

#ifndef TADS3_TCERR_H
#define TADS3_TCERR_H

/* TADS 3 Compiler Error Management */

#include "glk/tads/tads3/vmerr.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   Error Severity Levels.  
 */
enum tc_severity_t
{
    /* information only - not an error */
    TC_SEV_INFO,

    /* 
     *   Pedantic warning - doesn't prevent compilation from succeeding,
     *   and doesn't even necessarily indicate anything is wrong.  We use
     *   a separate severity level for these because some users will want
     *   to be able to filter these out.  
     */
    TC_SEV_PEDANTIC,

    /* 
     *   warning - does not prevent compilation from succeeding, but
     *   indicates a potential problem 
     */
    TC_SEV_WARNING,

    /* 
     *   error - compilation cannot be completed successfully, although it
     *   may be possible to continue with compilation to the extent
     *   necessary to check for any additional errors 
     */
    TC_SEV_ERROR,

    /* fatal - compilation must be immediately aborted */
    TC_SEV_FATAL,

    /* internal error - compilation must be immediately aborted */
    TC_SEV_INTERNAL
};


/* English version of message array */
extern const err_msg_t tc_messages_english[];
extern size_t tc_message_count_english;

/* error message array */
extern const err_msg_t *tc_messages;
extern size_t tc_message_count;

/* look up a message */
const char *tcerr_get_msg(int msgnum, int verbose);

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
