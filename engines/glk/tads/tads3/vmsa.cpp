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

/* Defines certain entrypoints that are not needed in a normal stand-alone
 * version of the interpreter.  The entrypoints defined here are dummy
 * implementations that should never be called.
 *
 * Certain functions are needed only in special VM versions that support,
 * for example, rebuilding of an image file after running 'preinit'.
 */

#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/vmobj.h"
#include "glk/tads/tads3/vmglob.h"
#include "glk/tads/tads3/vmdict.h"
#include "glk/tads/tads3/vmstr.h"
#include "glk/tads/tads3/vmlst.h"
#include "glk/tads/tads3/vmbignum.h"
#include "glk/tads/tads3/vmtobj.h"
#include "glk/tads/tads3/vmgram.h"
#include "glk/tads/tads3/vmmeta.h"
#include "glk/tads/tads3/vmintcls.h"
#include "glk/tads/tads3/vmiter.h"
#include "glk/tads/tads3/vmvec.h"
#include "glk/tads/tads3/vmlookup.h"
#include "glk/tads/tads3/vmbytarr.h"
#include "glk/tads/tads3/vmcset.h"
#include "glk/tads/tads3/vmfilobj.h"
#include "glk/tads/tads3/vmtmpfil.h"
#include "glk/tads/tads3/vmfilnam.h"
#include "glk/tads/tads3/vmpat.h"
#include "glk/tads/tads3/vmstrcmp.h"
#include "glk/tads/tads3/vmstrbuf.h"
#include "glk/tads/tads3/vmdynfunc.h"
#include "glk/tads/tads3/vmfref.h"
#include "glk/tads/tads3/vmdate.h"
#include "glk/tads/tads3/vmtzobj.h"


#ifdef TADSNET
#include "glk/tads/tads3/vmhttpsrv.h"
#include "glk/tads/tads3/vmhttpreq.h"
#endif

namespace Glk {
namespace TADS {
namespace TADS3 {

ulong CVmObjTads::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjTads::convert_to_const_data(VMG_ CVmConstMapper *,
                                       vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjString::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjString::reserve_const_data(VMG_ CVmConstMapper *,
                                      vm_obj_id_t)
{
    assert(FALSE);
}

void CVmObjString::convert_to_const_data(VMG_ CVmConstMapper *,
                                         vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjList::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjList::reserve_const_data(VMG_ CVmConstMapper *,
                                    vm_obj_id_t)
{
    assert(FALSE);
}

void CVmObjList::convert_to_const_data(VMG_ CVmConstMapper *,
                                       vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjDict::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjDict::convert_to_const_data(VMG_ CVmConstMapper *,
                                       vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjGramProd::rebuild_image(VMG_ char *buf, ulong buflen)
{
    assert(FALSE);
    return 0;
}

void CVmMetaTable::rebuild_image(class CVmImageWriter *)
{
}

ulong CVmObjBigNum::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjBigNum::reserve_const_data(VMG_ CVmConstMapper *,
                                      vm_obj_id_t)
{
    assert(FALSE);
}

void CVmObjBigNum::convert_to_const_data(VMG_ CVmConstMapper *,
                                         vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjClass::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjIterIdx::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjIterIdx::convert_to_const_data(VMG_ CVmConstMapper *,
                                          vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjVector::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjVector::reserve_const_data(VMG_ CVmConstMapper *, vm_obj_id_t)
{
    assert(FALSE);
}

void CVmObjVector::convert_to_const_data(VMG_ CVmConstMapper *, vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjLookupTable::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjStringBuffer::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjLookupTable::convert_to_const_data(VMG_ CVmConstMapper *,
                                              vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjIterLookupTable::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjIterLookupTable::convert_to_const_data(
    VMG_ CVmConstMapper *, vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjByteArray::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjCharSet::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjFile::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjTemporaryFile::rebuild_image(VMG_ char *buf, ulong buflen)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjFileName::rebuild_image(VMG_ char *buf, ulong buflen)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjPattern::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

void CVmObjPattern::convert_to_const_data(VMG_ CVmConstMapper *, vm_obj_id_t)
{
    assert(FALSE);
}

ulong CVmObjStrComp::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

ulong CVmDynamicFunc::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjFrameDesc::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjFrameRef::rebuild_image(VMG_ char *, ulong)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjDate::rebuild_image(VMG_ char *buf, ulong buflen)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjTimeZone::rebuild_image(VMG_ char *buf, ulong buflen)
{
    assert(FALSE);
    return 0;
}


/* ------------------------------------------------------------------------ */
/*
 *   networking support, if present
 */

#ifdef TADSNET

ulong CVmObjHTTPServer::rebuild_image(VMG_ char *buf, ulong buflen)
{
    assert(FALSE);
    return 0;
}

ulong CVmObjHTTPRequest::rebuild_image(VMG_ char *buf, ulong buflen)
{
    assert(FALSE);
    return 0;
}

#endif

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk
