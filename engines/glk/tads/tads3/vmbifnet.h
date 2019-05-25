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

#ifndef TADS3_VMBIFNET_H
#define TADS3_VMBIFNET_H

/* Function set definition - TADS Networking
 *
 * The networking function set provides access to the HTTP server feature, which allows
 * running the UI in a web browser.
*/

#include "glk/tads/os_glk.h"
#include "glk/tads/tads3/vmbif.h"
#include "glk/tads/tads3/utf8.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

class CVmBifNet: public CVmBif
{
public:
    /* function vector */
    static vm_bif_desc bif_table[];

    /* attach/detach - static initialization and cleanup */
    static void attach(VMG0_);
    static void detach(VMG0_);

    /* connect to the web UI */
    static void connect_ui(VMG_ uint argc);

    /* get a network event */
    static void get_event(VMG_ uint argc);

    /* get the local host name */
    static void get_hostname(VMG_ uint argc);

    /* get the local host IP address */
    static void get_host_ip(VMG_ uint argc);

    /* get the storage server URL */
    static void get_storage_url(VMG_ uint argc);

    /* get the launch host address */
    static void get_launch_host_addr(VMG_ uint argc);

    /* send a network request */
    static void send_net_request(VMG_ uint argc);
};


#endif /* VMBIFNET_H */

/* ------------------------------------------------------------------------ */
/*
 *   Function set vector.  Define this only if VMBIF_DEFINE_VECTOR has been
 *   defined, so that this file can be included for the prototypes alone
 *   without defining the function vector.
 *   
 *   Note that this vector is specifically defined outside of the section of
 *   the file protected against multiple inclusion.  
 */
#ifdef VMBIF_DEFINE_VECTOR

/* TADS input/output extension functions */
vm_bif_desc CVmBifNet::bif_table[] =
{
    { &CVmBifNet::connect_ui, 2, 0, FALSE },
    { &CVmBifNet::get_event, 0, 1, FALSE },
    { &CVmBifNet::get_hostname, 0, 0, FALSE },
    { &CVmBifNet::get_host_ip, 0, 0, FALSE },
    { &CVmBifNet::get_storage_url, 0, 0, FALSE },
    { &CVmBifNet::get_launch_host_addr, 0, 0, FALSE },
    { &CVmBifNet::send_net_request, 2, 0, TRUE }
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
