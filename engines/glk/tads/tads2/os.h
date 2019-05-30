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

/* Portable interfaces to OS-specific functions
 *
 * This file defines interfaces to certain functions that must be called
 * from portable code, but which must have system-specific implementations.
 */

#ifndef GLK_TADS_TADS2_OS
#define GLK_TADS_TADS2_OS

#include "common/system.h"
#include "glk/tads/os_frob_tads.h"
#include "glk/tads/os_glk.h"
#include "glk/tads/tads2/lib.h"
#include "glk/tads/tads2/appctx.h"

namespace Glk {
namespace TADS {
namespace TADS2 {


/* ------------------------------------------------------------------------ */
/*
 *   A note on character sets:
 *   
 *   Except where noted, all character strings passed to and from the
 *   osxxx functions defined herein use the local operating system
 *   representation.  On a Windows machine localized to Eastern Europe,
 *   for example, the character strings passed to and from the osxxx
 *   functions would use single-byte characters in the Windows code page
 *   1250 representation.
 *   
 *   Callers that use multiple character sets must implement mappings to
 *   and from the local character set when calling the osxxx functions.
 *   The osxxx implementations are thus free to ignore any issues related
 *   to character set conversion or mapping.
 *   
 *   The osxxx implementations are specifically not permitted to use
 *   double-byte Unicode as the native character set, nor any other
 *   character set where a null byte could appear as part of a non-null
 *   character.  In particular, callers may assume that null-terminated
 *   strings passed to and from the osxxx functions contain no embedded
 *   null bytes.  Multi-byte character sets (i.e., character sets with
 *   mixed single-byte and double-byte characters) may be used as long as
 *   a null byte is never part of any multi-byte character, since this
 *   would guarantee that a null byte could always be taken as a null
 *   character without knowledge of the encoding or context.  
 */

/* ------------------------------------------------------------------------ */
/*
 *   "Far" Pointers.  Most platforms can ignore this.  For platforms with
 *   mixed-mode addressing models, where pointers of different sizes can
 *   be used within a single program and hence some pointers require
 *   qualification to indicate that they use a non-default addressing
 *   model, the keyword OSFAR should be defined to the appropriate
 *   compiler-specific extension keyword.
 *   
 *   If you don't know what I'm talking about here, you should just ignore
 *   it, because your platform probably doesn't have anything this
 *   sinister.  As of this writing, this applies only to MS-DOS, and then
 *   only to 16-bit implementations that must interact with other 16-bit
 *   programs via dynamic linking or other mechanisms.  
 */


/* ------------------------------------------------------------------------ */
/*
 *   ANSI C99 exact-size integer types.
 *   
 *   C99 defines a set of integer types with exact bit sizes, named intXX_t
 *   for a signed integer with XX bits, and uintXX_t for unsigned.  XX can be
 *   8, 16, 32, or 64.  TADS uses the 16- and 32-bit sizes, so each platform
 *   is responsible for defining the following types:
 *   
 *.    int16_t   - a signed integer type storing EXACTLY 16 bits
 *.    uint16_t  - an unsigned integer type storing EXACTLY 16 bits
 *.    int32_t   - a signed integer type storing EXACTLY 32 bits
 *.    uint32_t  - an unsigned integer type storing EXACTLY 32 bits
 *   
 *   Many modern compilers provide definitions for these types via the
 *   standard header stdint.h.  Where stdint.h is provided, the platform code
 *   can merely #include <stdint.h>.
 *   
 *   For compilers where stdint.h isn't available, you must provide suitable
 *   typedefs.  Note that the types must be defined with the exact bit sizes
 *   specified; it's not sufficient to use a bigger type, because we depend
 *   in some cases on overflow and sign extension behavior at the specific
 *   bit size.
 */


/* ------------------------------------------------------------------------ */
/* 
 *   OS main entrypoint 
 */
int os0main(int oargc, char **oargv,
            int (*mainfn)(int, char **, char *), 
            const char *before, const char *config);

/* 
 *   new-style OS main entrypoint - takes an application container context 
 */
int os0main2(int oargc, char **oargv,
             int (*mainfn)(int, char **, appctxdef *, char *),
             const char *before, const char *config,
             appctxdef *appctx);


/* open the error message file for reading */
osfildef *oserrop(const char *arg0);

/* ------------------------------------------------------------------------ */
/*
 *   Special "switch" statement optimization flags.  These definitions are
 *   OPTIONAL - if a platform doesn't provide these definitions, suitable
 *   code that's fully portable will be used.
 *   
 *   On some compilers, the performance of a "switch" statement can be
 *   improved by fully populating the switch with all possible "case"
 *   values.  When the compiler can safely assume that every possible "case"
 *   value is specifically called out in the switch, the compiler can
 *   generate somewhat faster code by omitting any range check for the
 *   controlling expression of the switch: a range check is unnecessary
 *   because the compiler knows that the value can never be outside the
 *   "case" table.
 *   
 *   This type of optimization doesn't apply to all compilers.  When a given
 *   platform's compiler can be coerced to produce faster "switch"
 *   statements, though, there might be some benefit in defining these
 *   symbols according to local platform rules.
 *   
 *   First, #define OS_FILL_OUT_CASE_TABLES if you want this type of switch
 *   optimization at all.  This symbol is merely a flag, so it doesn't need
 *   a value - #defining it is enough to activate the special code.  If you
 *   don't define this symbol, then the code that cares about this will
 *   simply generate ordinary switches, leaving holes in the case table and
 *   using "default:" to cover them.  If the platform's osxxx.h header does
 *   #define OS_FILL_OUT_CASE_TABLES, then the portable code will know to
 *   fill out case tables with all possible alternatives where it's possible
 *   and potentially beneficial to do so.
 *   
 *   Second, if you #define OS_FILL_OUT_CASE_TABLES, you MUST ALSO #define
 *   OS_IMPOSSIBLE_DEFAULT_CASE.  The value for this symbol must be some
 *   code to insert into a "switch" statement at the point where a
 *   "default:" case would normally go.  On some compilers, special
 *   extensions allow the program to explicitly indicate within a switch
 *   that all possible cases are covered, so that the compiler doesn't have
 *   to be relied upon to infer this for itself (which would be impossible
 *   for it to do in many cases anyway).  You can use an empty definition
 *   for this symbol if your compiler doesn't have any special construct for
 *   indicating a fully-populated case table.
 *   
 *   Note that *most* switch statements in portable code won't care one way
 *   or the other about these definitions.  There's a time/space trade-off
 *   in fully populating a switch's case table, so only the most
 *   time-critical code will bother trying.  
 */



/* ------------------------------------------------------------------------ */
/*
 *   TADS 2 swapping configuration.  Define OS_DEFAULT_SWAP_ENABLED to 0
 *   if you want swapping turned off, 1 if you want it turned on.  If we
 *   haven't defined a default swapping mode yet, turn swapping on by
 *   default.  
 */
#ifndef OS_DEFAULT_SWAP_ENABLED
# define OS_DEFAULT_SWAP_ENABLED   1
#endif

/*
 *   If the system "long description" (for the banner) isn't defined, make
 *   it the same as the platform ID string.  
 */
#ifndef OS_SYSTEM_LDESC
# define OS_SYSTEM_LDESC  OS_SYSTEM_NAME
#endif

/*
 *   TADS 2 Usage Lines
 *   
 *   If the "usage" lines (i.e., the descriptive lines of text describing
 *   how to run the various programs) haven't been otherwise defined,
 *   define defaults for them here.  Some platforms use names other than
 *   tc, tr, and tdb for the tools (for example, on Unix they're usually
 *   tadsc, tadsr, and tadsdb), so the usage lines should be adjusted
 *   accordingly; simply define them earlier than this point (in this file
 *   or in one of the files included by this file, such as osunixt.h) for
 *   non-default text.  
 */
#ifndef OS_TC_USAGE
# define OS_TC_USAGE  "usage: tc [options] file"
#endif
#ifndef OS_TR_USAGE
# define OS_TR_USAGE  "usage: tr [options] file"
#endif
#ifndef OS_TDB_USAGE
# define OS_TDB_USAGE  "usage: tdb [options] file"
#endif

/*
 *   Ports can define a special TDB startup message, which is displayed
 *   after the copyright/version banner.  If it's not defined at this
 *   point, define it to an empty string.  
 */
#ifndef OS_TDB_STARTUP_MSG
# define OS_TDB_STARTUP_MSG ""
#endif

/*
 *   If a system patch sub-level isn't defined, define it here as zero.
 *   The patch sub-level is used on some systems where a number of ports
 *   are derived from a base port (for example, a large number of ports
 *   are based on the generic Unix port).  For platforms like the Mac,
 *   where the porting work applies only to that one platform, this
 *   sub-level isn't meaningful.
 */
#ifndef OS_SYSTEM_PATCHSUBLVL
# define OS_SYSTEM_PATCHSUBLVL  "0"
#endif

} // End of namespace TADS2
} // End of namespace TADS
} // End of namespace Glk

#endif
