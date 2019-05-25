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

/* TADS 3 Compiler Host System Interface
 *
 * The host system interface is an abstract interface to services provided by the
 * compiler host system.
 */

#ifndef TADS3_TCHOST_H
#define TADS3_TCHOST_H

#include "glk/tads/tads3/tcerr.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   Abstract Host Interface 
 */
class CTcHostIfc
{
public:
    virtual ~CTcHostIfc() { }

    /* 
     *   Display a message, taking varargs parameters.  The message is a
     *   UTF-8 string.  The message string is a printf-style format
     *   string.  This is used to display informational messages.  
     */
    void print_msg(const char *msg, ...)
    {
        va_list args;

        va_start(args, msg);
        v_print_msg(msg, args);
        va_end(args);
    }

    /* display a message with va_list-style varargs */
    virtual void v_print_msg(const char *msg, va_list args) = 0;

    /*
     *   Display a process step message.  These work the same as
     *   print_msg() and v_print_msg(), respectively, but display a
     *   message indicating a process step.  Some implementations might
     *   want to display process step messages in a special manner; for
     *   example, a GUI implementation might put the message in a status
     *   bar or similar type of display rather than intermixed with other
     *   messages.  
     */
    void print_step(const char *msg, ...)
    {
        va_list args;

        va_start(args, msg);
        v_print_step(msg, args);
        va_end(args);
    }

    /* display a process step message with va_list-style varargs */
    virtual void v_print_step(const char *msg, va_list args) = 0;

    /* 
     *   Display an error message.  These work the same way as print_msg()
     *   and v_print_msg(), respectively, but display error messages
     *   instead of informational messages.  Some implementations might
     *   want to direct the different types of messages to different
     *   places; for example, a stdio implementation may want to send
     *   print_msg messages to stdout, but print_err messages to stderr.  
     */
    void print_err(const char *msg, ...)
    {
        va_list args;

        va_start(args, msg);
        v_print_err(msg, args);
        va_end(args);
    }

    /* display an error with va_list arguments */
    virtual void v_print_err(const char *msg, va_list args) = 0;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
