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
  tcprsnl.cpp - TADS 3 Compiler Parser - "no linker" module
Function
  This module can be linked with programs that don't require any linker
  functionality, such as for debugger or interpreter-with-eval builds.  The
  module provides dummy entrypoints for functions and methods that would
  normally be linked from tcprsimg.cpp, so that we don't have to drag in that
  whole file (and its dependencies) for configurations that don't need its
  functionality.
Notes
  
Modified
  04/30/99 MJRoberts  - Creation
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "glk/tads/os_glk.h"
#include "glk/tads/tads3/t3std.h"
#include "glk/tads/tads3/tcprs.h"
#include "glk/tads/tads3/tctarg.h"
#include "glk/tads/tads3/tcgen.h"
#include "glk/tads/tads3/vmhash.h"
#include "glk/tads/tads3/tcmain.h"
#include "glk/tads/tads3/vmfile.h"
#include "glk/tads/tads3/tcmake.h"

void CTcSymObjBase::apply_internal_fixups()
{
    assert(FALSE);
}

void CTcSymObj::delete_prop_from_mod_base(tctarg_prop_id_t)
{
    assert(FALSE);
}

void CTcSymObj::mark_compiled_as_class()
{
    assert(FALSE);
}

int CTcSymbolBase::load_from_obj_file(CVmFile *, const textchar_t *,
                                      tctarg_obj_id_t *,
                                      tctarg_prop_id_t *,
                                      ulong *)
{
    assert(FALSE);
    return 0;
}

void CTcMake::write_build_config_to_sym_file(CVmFile *fp)
{
    assert(FALSE);
}

