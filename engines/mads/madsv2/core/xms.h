/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MADS_CORE_XMS_H
#define MADS_CORE_XMS_H

#include "common/scummsys.h"

namespace MADS {
namespace MADSV2 {

#define XMS_MAX_UMB     16              /* Max UMB blocks to allocate */

#define XMS             byte *      /* XMS fake pointer type      */

extern int   xms_exists;           /* Flag if we've got some XMS memory  */
extern int   xms_disabled;         /* Flag to disable XMS altogether     */
extern word  xms_version;          /* XMS driver version #               */
extern dword xms_controller;       /* XMS controller address             */
extern word  xms_chain_seg;        /* First UMB MCB segment for us       */

extern word xms_umb_list[XMS_MAX_UMB];
extern word xms_umb_mark;

/* xms_1.c */
int xms_detect(void);

/* xms_2.c */
long         xms_umb_get_avail(void);

/* xms_3.c */
void *xms_umb_get(long mem_to_get);
void         xms_umb_free(void *mem_to_free);

/* xms_4.c */
void         xms_umb_purge(void);

/* xms_5.c */
long         xms_get_avail(void);

/* xms_6.c */
int  xms_get(long size_in_bytes);
void xms_free(int xms_handle);

/* xms_7.c */
int          xms_copy(long copy_size,
	word source_handle, void *source,
	word dest_handle, void *dest);

} // namespace MADSV2
} // namespace MADS

#endif
