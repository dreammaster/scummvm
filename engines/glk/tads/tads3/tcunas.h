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

#ifndef TADS3_TCUNAS_H
#define TADS3_TCUNAS_H

/* TADS 3 Compiler Unassembler */

#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/tcgen.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/*
 *   byte-code source for unassembler 
 */
class CTcUnasSrc
{
public:
    virtual ~CTcUnasSrc() { }
    
    /* 
     *   read the next byte; returns zero on success, non-zero at the end
     *   of the byte stream 
     */
    virtual int next_byte(char *ch) = 0;

    /* get the current offset */
    virtual ulong get_ofs() const = 0;
};

/*
 *   code stream implementation of byte code source
 */
class CTcUnasSrcCodeStr: public CTcUnasSrc
{
public:
    CTcUnasSrcCodeStr(CTcCodeStream *str)
    {
        /* remember our underlying code stream */
        str_ = str;

        /* start at the first byte of the code stream */
        ofs_ = 0;
    }

    /* read from the code stream */
    int next_byte(char *ch)
    {
        /* if there's anything left, return the byte and bump the pointer */
        if (ofs_ < str_->get_ofs())
        {
            *ch = str_->get_byte_at(ofs_);
            ++ofs_;
            return 0;
        }

        /* indicate end of file */
        return 1;
    }

    /* get the current offset */
    ulong get_ofs() const
    {
        return ofs_;
    }

protected:
    /* underlying code stream object */
    CTcCodeStream *str_;

    /* current read offset in code stream */
    ulong ofs_;
};

/* ------------------------------------------------------------------------ */
/*
 *   output stream for unassembler 
 */
class CTcUnasOut
{
public:
    virtual ~CTcUnasOut() { }

    /* write a line of text to the output, printf-style */
    virtual void print(const char *fmt, ...) = 0;
};

/*
 *   stdio implementation of output stream - writes data to standard
 *   output 
 */
class CTcUnasOutStdio: public CTcUnasOut
{
public:
    void print(const char *fmt, ...)
    {
        va_list va;

        /* display the data on the standard output */
        va_start(va, fmt);
        vprintf(fmt, va);
        va_end(va);
    }
};

/*
 *   Text file (osfildef) implementation of output stream.  The file handle
 *   is managed by the caller.  
 */
class CTcUnasOutFile: public CTcUnasOut
{
public:
    CTcUnasOutFile(osfildef *fp) { fp_ = fp; }

    void print(const char *fmt, ...)
    {
        char buf[1024];
        va_list va;

        /* format the text */
        va_start(va, fmt);
        t3vsprintf(buf, sizeof(buf), fmt, va);
        va_end(va);

        /* write the formatted text to the file */
        os_fprintz(fp_, buf);
    }

protected:
    /* our file handle */
    osfildef *fp_;
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
