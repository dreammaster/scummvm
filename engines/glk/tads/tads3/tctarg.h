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

#ifndef TADS3_TCTARG_H
#define TADS3_TCTARG_H

/* TADS 3 Compiler Target Selector */

#include "glk/tads/tads3/t3std.h"

/* ------------------------------------------------------------------------ */
/*
 *   Target Selection.
 *   
 *   SEE ALSO: tctargty.h - target type header selector
 *   
 *   As the parser scans the source stream, it constructs a parse tree
 *   that represent the parsed form of the source.  After the parsing
 *   pass, the parse tree contains all of the information necessary to
 *   generate code for the translation.
 *   
 *   The parse tree objects are actually the code generators.  So, as the
 *   scanner parses the source file, it must create parse tree objects for
 *   the appropriate target architecture.  However, we want to keep the
 *   scanner independent of the target architecture -- we want the same
 *   scanner to be usable for any target architecture for which we can
 *   provide a code generator.
 *   
 *   To accomplish this, we define a base class for parse tree nodes; the
 *   scanner is only interested in this base class interface.  Then, for
 *   each target architecture, we create a subclass of each parse tree
 *   node that contains the code generator for that node type for the
 *   target.
 *   
 *   However, the scanner must still know enough to create the appropriate
 *   subclass of each parse tree node.  This file contains the target
 *   selection switch that mediates the independence of the scanner from
 *   the target code generator, but still allows the scanner to create the
 *   correct type of parse tree nodes for the desired target.  For each
 *   parse tree node type that the scanner must create, we #define a
 *   generic symbol to a target-specific subclass.  The scanner uses the
 *   generic symbol, but we expand the macro when compiling the compiler
 *   to the correct target.
 *   
 *   Because the target selection is made through macros, the target
 *   architecture is fixed at compile time.  However, the same scanner
 *   source code can be compiled into multiple target compilers.  
 */

/*
 *   Before including this file, #define the appropriate target type.
 *   This should usually be done in the makefile, since this is a
 *   compile-time selection.
 *   
 *   To add a new code generator, define the appropriate subclasses in a
 *   new file.  Add a new #ifdef-#include sequence below that includes the
 *   subclass definitions for your code generator.
 *   
 *   Note that each target uses the same names for the final subclasses.
 *   We choose the target at link time when building the compiler
 *   executable, so we must compile and link only a single target
 *   architecture into a given build of the compiler.  
 */

/* 
 *   make sure TC_TARGET_DEFINED__ isn't defined, so we can use it to
 *   sense whether we defined a code generator or not
 */
#undef TCTARG_TARGET_DEFINED__


/* ------------------------------------------------------------------------ */
/*
 *   T3 Virtual Machine Code Generator 
 */
#ifdef TC_TARGET_T3
#include "glk/tads/tads3/tct3.h"
#define TCTARG_TARGET_DEFINED__
#endif

/* ------------------------------------------------------------------------ */
/*
 *   JavaScript code generator
 */
#ifdef TC_TARGET_JS
#include "glk/tads/tads3/tcjs.h"
#define TCTARG_TARGET_DEFINED__
#endif


/* ------------------------------------------------------------------------ */
/*
 *   ensure that a code generator was defined - if not, the compilation
 *   cannot proceed 
 */
#ifndef TADS3_TCTARG_TARGET_DEFINED__
#error No code generator target is defined.  A code generator must be defined in your makefile.  See tctarg.h for details.
#endif

#endif
