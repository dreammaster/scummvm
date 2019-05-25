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

#ifndef TADS3_TCHOSTSI_H
#define TADS3_TCHOSTSI_H

/* Stdio implementation of host interface */

#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/tchost.h"
#include "glk/tads/tads3/osifcnet.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   stdio host interface 
 */
class CTcHostIfcStdio: public CTcHostIfc
{
public:
    CTcHostIfcStdio()
    {
        /* by default, show progress messages */
        show_progress_ = TRUE;

        /* there's no status prefix string yet */
        status_prefix_ = 0;
    }

    ~CTcHostIfcStdio()
    {
        /* free our status prefix string */
        lib_free_str(status_prefix_);
    }

    /* set the status message prefix string */
    void set_status_prefix(const char *str)
    {
        /* delete any former prefix string */
        lib_free_str(status_prefix_);

        /* store a copy of the string */
        status_prefix_ = lib_copy_str(str);
    }

    /* display information messages */
    void v_print_msg(const char *msg, va_list args);

    /* display a process step message */
    void v_print_step(const char *msg, va_list args);

    /* display error messages */
    void v_print_err(const char *msg, va_list args);

    /* turn status (step) messages on/off */
    void set_show_progress(int flag) { show_progress_ = flag; }

protected:
    /* 
     *   internal display routine - formats the message, converts it to
     *   the console character set and displays the result on the standard
     *   output 
     */
    void v_printf(const char *msg, va_list args);

    /* flag: show progress (step) messages */
    int show_progress_;

    /* status prefix message */
    char *status_prefix_;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
