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

#ifndef TADS3_VMISAAC_H
#define TADS3_VMISAAC_H

/* TADS 3 VM ISAAC random number generator implementation */

#include "glk/tads/tads3/t3std.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* data block size */
#define ISAAC_RANDSIZL   (8)
#define ISAAC_RANDSIZ    (1<<ISAAC_RANDSIZL)

/* ISAAC RNG context */
struct isaacctx
{
    /* RNG context */
    uint32_t cnt;
    uint32_t rsl[ISAAC_RANDSIZ];
    uint32_t mem[ISAAC_RANDSIZ];
    uint32_t a;
    uint32_t b;
    uint32_t c;
};

#define isaac_rand(r) \
    ((r)->cnt-- == 0 ? \
     (isaac_gen_group(r), (r)->cnt=ISAAC_RANDSIZ-1, (r)->rsl[(r)->cnt]) : \
     (r)->rsl[(r)->cnt])

/* 
 *   Initialize.  If flag is true, then use the contents of ctx->rsl[] to
 *   initialize ctx->mm[]; otherwise, we'll use a fixed starting
 *   configuration.  
 */
void isaac_init(isaacctx *ctx, int flag);

/* 
 *   Get/set the internal state.  This allows saving and restoring the
 *   internal state of the RNG.  'get' returns the size of the byte buffer
 *   required; call with buf==0 to determine the size needed.
 *   
 *   'get' and 'set' both expect the caller to know the correct size from a
 *   size-query call to 'get'.  The size passed to 'set' must always exactly
 *   match the size returned from 'get', since anything else could corrupt
 *   the internal state.
 */
size_t isaac_get_state(isaacctx *ctx, char *buf);
void isaac_set_state(isaacctx *ctx, const char *buf);

/* generate a group of random values */
void isaac_gen_group(isaacctx *ctx);

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
