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

/* Core Metaclass Registrations */

/* 
 *   NOTE - this file is INTENTIONALLY not protected against multiple
 *   inclusion.  Because of the funny business involved in buildling the
 *   registration tables, we must include this file more than once in
 *   different configurations.  Therefore, there's no #ifndef
 *   VMMCCORE_INCLUDED test at all in this file.  
 */

/* ------------------------------------------------------------------------ */
/*
 *   Before we begin, if we're building the registration table, redefine
 *   the CENTRAL REGISTRATION BUILDER version of the metaclass
 *   registration macro.  We do this here rather than in vmmcreg.cpp to
 *   make it easier to create separate versions of vmmcreg.cpp for
 *   separate subsystems.  This core file is always included, even in
 *   special configurations, before any other metaclass headers.  
 */
#ifdef VMMCCORE_BUILD_TABLE

#ifdef VM_REGISTER_METACLASS
#undef VM_REGISTER_METACLASS
#endif
#define VM_REGISTER_METACLASS(meta_class) \
    { &meta_class::metaclass_reg_ },

#endif /* VMMCCORE_BUILD_TABLE */

/* ------------------------------------------------------------------------ */
/*
 *   Now include each header for file the core metaclasses 
 */
#include "glk/tads/tads3/vmobj.h"
#include "glk/tads/tads3/vmtobj.h"
#include "glk/tads/tads3/vmstr.h"
#include "glk/tads/tads3/vmlst.h"
#include "glk/tads/tads3/vmdict.h"
#include "glk/tads/tads3/vmgram.h"
#include "glk/tads/tads3/vmbignum.h"
#include "glk/tads/tads3/vmintcls.h"
#include "glk/tads/tads3/vmanonfn.h"
#include "glk/tads/tads3/vmcoll.h"
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

/* if networking is enabled, include the networking metaclasses */
#ifdef TADSNET
#include "glk/tads/tads3/vmmcnet.h"
#endif
