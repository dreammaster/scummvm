#ifdef RCSID
static char RCSid[] =
"$Header$";
#endif

/* 
 *   Copyright (c) 1999, 2002 Michael J. Roberts.  All Rights Reserved.
 *   
 *   Please see the accompanying license file, LICENSE.TXT, for information
 *   on using and copying this software.  
 */
/*
Name
  vmsa.cpp - stand-alone interpreter entrypoints
Function
  Defines certain entrypoints that are not needed in a normal stand-alone
  version of the interpreter.  The entrypoints defined here are dummy
  implementations that should never be called.

  Certain functions are needed only in special VM versions that support,
  for example, rebuilding of an image file after running 'preinit'.
Notes
  
Modified
  07/21/99 MJRoberts  - Creation
*/

#include <assert.h>

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
