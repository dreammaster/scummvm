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

#ifndef MADS_CORE_PACK_5_H
#define MADS_CORE_PACK_5_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

extern word (*pack_read_routine)(char *buffer, word *size);
extern void (*pack_write_routine)(char *buffer, word *size);

extern word pack_mode;
extern byte *pack_buffer;
extern word pack_buffer_size;

extern long pack_read_size;
extern long pack_read_count;
extern long pack_write_size;
extern long pack_write_count;

extern byte pack_zip_enabled;           /* ZIP packing enabled           */
extern byte pack_pfab_enabled;          /* PFAB packing enabled          */
extern int  pack_strategy;              /* Current packing strategy      */

/* All compression routines called through function pointers, so that */
/* we can determine at compile time which compression modules will be */
/* linked.                                                            */

extern unsigned short (*pack_implode_routine)
  (unsigned (*read_buff)(char *buffer,unsigned short *size),
   void (*write_buff)(char *buffer,unsigned short *size),
   char *work_buff,
   unsigned short int *type,
   unsigned short int *dsize);

extern unsigned (*pack_explode_routine)
  (unsigned (*read_buff)(char *buffer,unsigned short *size),
   void (*write_buff)(char *buffer,unsigned short *size),
   char *work_buff);

extern unsigned short (*pack_pFABcomp_routine)
  (unsigned (*read_buff)(char *buffer,unsigned short *size),
   void (*write_buff)(char *buffer,unsigned short *size),
   char *work_buff,
   unsigned short int *type,
   unsigned short int *dsize);

extern unsigned (*pack_pFABexp0_routine)
  (unsigned (*read_buff)(char *buffer,unsigned short *size),
   void (*write_buff)(char *buffer,unsigned short *size),
   char *work_buff);

extern unsigned (*pack_pFABexp1_routine)
  (unsigned (*read_buff)(char *buffer,unsigned short *size),
   char *write_buf,
   char *work_buff);

extern unsigned (*pack_pFABexp2_routine)
  (byte *read_buf,
   byte *write_buf,
   char *work_buff);

word pack_a_packet(int packing_flag, int explode_mode);

} // namespace MADSV2
} // namespace MADS

#endif
