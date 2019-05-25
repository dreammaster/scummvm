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

#ifndef TADS3_TCGLOB_H
#define TADS3_TCGLOB_H

/* TADS 3 Compiler globals
 *
 * The TADS 3 Compiler uses a number of static variables that are shared by several subsystems.
 * We define these variables as global variables for quick access, and to minimize the number
 * of parameters that are passed around among subsystems.
 */

namespace Glk {
namespace TADS {
namespace TADS3 {

/*
 *   If we're not explicitly defining the storage for the globals, define
 *   them as external - this lets everyone pick up external declarations
 *   for all of the globals simply by including this file.  
 */
#ifndef TADS3_TC_GLOB_DECLARE
#define TC_GLOB_DECLARE extern
#endif

/* host system interface */
TC_GLOB_DECLARE class CTcHostIfc *G_hostifc;

/* main compiler driver */
TC_GLOB_DECLARE class CTcMain *G_tcmain;

/* the parser */
TC_GLOB_DECLARE class CTcParser *G_prs;

/* parse tree node list memory manager */
TC_GLOB_DECLARE class CTcPrsMem *G_prsmem;

/* the tokenizer */
TC_GLOB_DECLARE class CTcTokenizer *G_tok;

/* 
 *   Current code stream - this points to the currently active code stream
 *   object.  The active code stream can vary according to what kind of
 *   code we're generating. 
 */
TC_GLOB_DECLARE class CTcCodeStream *G_cs;

/* primary generated code stream - for all normal code */
TC_GLOB_DECLARE class CTcCodeStream *G_cs_main;

/* static initializer code stream */
TC_GLOB_DECLARE class CTcCodeStream *G_cs_static;

/* local variable name stream */
TC_GLOB_DECLARE class CTcDataStream *G_lcl_stream;

/* generated data (constant) stream */
TC_GLOB_DECLARE class CTcDataStream *G_ds;

/* TADS-Object metaclass data stream */
TC_GLOB_DECLARE class CTcDataStream *G_os;

/* Dictionary metaclass data stream */
TC_GLOB_DECLARE class CTcDataStream *G_dict_stream;

/* GrammarProduction metaclass data stream */
TC_GLOB_DECLARE class CTcDataStream *G_gramprod_stream;

/* BigNumber metaclass data stream */
TC_GLOB_DECLARE class CTcDataStream *G_bignum_stream;

/* RexPattern metaclass data stream */
TC_GLOB_DECLARE class CTcDataStream *G_rexpat_stream;

/* IntrinsicClass metaclass data stream */
TC_GLOB_DECLARE class CTcDataStream *G_int_class_stream;

/* intrinsic class modifier metaclass data stream */
TC_GLOB_DECLARE class CTcDataStream *G_icmod_stream;

/* static initializer obj.prop ID stream */
TC_GLOB_DECLARE class CTcDataStream *G_static_init_id_stream;

/* target-specific code generator class */
TC_GLOB_DECLARE class CTcGenTarg *G_cg;

/* 
 *   Run-time metaclass table.  When we're doing dynamic compilation, the
 *   interpreter will set this to its live metaclass table for the loaded
 *   program. 
 */
TC_GLOB_DECLARE class CVmMetaTable *G_metaclass_tab;

/* 
 *   object ID fixup list head, and flag indicating whether to keep object
 *   fixups 
 */
TC_GLOB_DECLARE struct CTcIdFixup *G_objfixup;
TC_GLOB_DECLARE int G_keep_objfixups;

/* 
 *   property ID fixup list head, and flag indicating whether to keep
 *   property ID fixups 
 */
TC_GLOB_DECLARE struct CTcIdFixup *G_propfixup;
TC_GLOB_DECLARE int G_keep_propfixups;

/*
 *   enumerator ID fixup list head, and flag indicating whether to keep
 *   enumerator fixups 
 */
TC_GLOB_DECLARE struct CTcIdFixup *G_enumfixup;
TC_GLOB_DECLARE int G_keep_enumfixups;


/* 
 *   Debug mode - if this is true, we're compiling for debugging, so we
 *   must generate additional symbolic information for the debugger. 
 */
TC_GLOB_DECLARE int G_debug;

/* disassembly output stream, if disassembly display is desired */
TC_GLOB_DECLARE class CTcUnasOut *G_disasm_out;

/*
 *   Image file sizes.  For the normal compiler, these are fixed quantities
 *   based on the current VM version we're targeting.  For the dynamic
 *   compiler, these are adjusted to match the data from the loaded image
 *   file; this is necessary to ensure that code we generate matches static
 *   code loaded from the image.  
 */
struct tc_image_info
{
    /* size of the method header */
    int mhdr;

    /* size of the exception table entry */
    int exc_entry;

    /* debugger table header size */
    int dbg_hdr;

    /* debugger line table entry size */
    int dbg_line;

    /* debugger frame record size */
    int dbg_frame;

    /* local symbol header size */
    int lcl_hdr;

    /* debug record format version ID */
    int dbg_fmt_vsn;
};
TC_GLOB_DECLARE tc_image_info G_sizes;

/*
 *   Compiler interface to the live VM.  In dynamic compilation mode, the VM
 *   supplies the compiler with this object to provide the compiler with
 *   access to VM resources.  
 */
TC_GLOB_DECLARE class CTcVMIfc *G_vmifc;

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
