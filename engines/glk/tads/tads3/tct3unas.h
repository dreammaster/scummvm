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

#ifndef TADS3_TCT3UNAS_H
#define TADS3_TCT3UNAS_H

/* TADS 3 Compiler - T3 Unassembler
 *
 * Takes a T3 byte-code stream and disassembles it to a printable display
 */

#include "glk/tads/tads3/tcunas.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/* ------------------------------------------------------------------------ */
/* 
 *   operand types, for instruction descriptions 
 */
enum t3_oper_type_t
{
    /* 8-bit signed integer */
    T3OP_TYPE_8S,

    /* 8-bit unsigned integer */
    T3OP_TYPE_8U,

    /* 16-bit signed integer */
    T3OP_TYPE_16S,

    /* 16-bit unsigned integer */
    T3OP_TYPE_16U,

    /* 32-bit signed integer */
    T3OP_TYPE_32S,

    /* 32-bit unsigned integer */
    T3OP_TYPE_32U,

    /* string offset */
    T3OP_TYPE_STR,

    /* list offset */
    T3OP_TYPE_LIST,

    /* absolute (32-bit) code address */
    T3OP_TYPE_CODE_ABS,

    /* relative (16-bit) code address */
    T3OP_TYPE_CODE_REL,

    /* object ID */
    T3OP_TYPE_OBJ,

    /* property ID */
    T3OP_TYPE_PROP,

    /* enum ID */
    T3OP_TYPE_ENUM,

    /* context element type */
    T3OP_TYPE_CTX_ELE,

    /* in-line string */
    T3OP_TYPE_INLINE_STR
};


/* ------------------------------------------------------------------------ */
/* 
 *   Instruction information - this structure describes one T3 byte-code
 *   instruction.  
 */
struct t3_instr_info_t
{
    /* name of the instruction */
    const char *nm;

    /* number of operands */
    int op_cnt;

    /* 
     *   operand types (allow for a fixed upper limit to the number of
     *   operands) 
     */
    t3_oper_type_t op_type[3];
};


/* ------------------------------------------------------------------------ */
/*
 *   T3 Disassembler 
 */
class CTcT3Unasm
{
public:
    /* disassemble a byte stream */
    static void disasm(class CTcUnasSrc *src, class CTcUnasOut *out);

    /* show an exception table */
    static void show_exc_table(class CTcUnasSrc *src, class CTcUnasOut *out,
                               unsigned long base_ofs);

    /* get the instruction information array (read-only) */
    static const t3_instr_info_t *get_instr_info() { return instr_info; }

protected:
    /* disassemble an instruction */
    static void disasm_instr(class CTcUnasSrc *src, class CTcUnasOut *out,
                             char ch);

    /* instruction information array */
    static t3_instr_info_t instr_info[];
};

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
