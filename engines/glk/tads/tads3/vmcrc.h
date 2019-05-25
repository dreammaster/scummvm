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

#ifndef TADS3_VMCRC_H
#define TADS3_VMCRC_H

/* compute a CRC-32 checksum of a data stream */

namespace Glk {
namespace TADS {
namespace TADS3 {

class CVmCRC32
{
public:
    CVmCRC32()
    {
        /* start with zero in the accumulator */
        acc_ = 0;
    }

    /* add the given buffer into the checksum */
    void scan_bytes(const void *ptr, size_t len);

    /* retrieve the current checksum value */
    unsigned long get_crc_val() const { return acc_; }

protected:
    /* the checksum accumulator */
    unsigned long acc_;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
