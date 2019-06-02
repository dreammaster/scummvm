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

#ifndef TADS3_TCTARGTY_H
#define TADS3_TCTARGTY_H

/* TADS 3 Compiler Target Selector - target types */

/* ------------------------------------------------------------------------ */
/*
 *   Target Selection - type definitions
 *   
 *   This is a target selector for the target types header.  This follows
 *   the same mechanism that tctarg.h uses - refer to that file for
 *   details.  
 */

/* 
 *   make sure TC_TARGET_DEFINED__ isn't defined, so we can use it to
 *   sense whether we defined a code generator or not 
 */
#undef TCTARGTY_TARGET_DEFINED__

/* ------------------------------------------------------------------------ */
/*
 *   T3 Virtual Machine Code Generator 
 */
#ifdef TC_TARGET_T3
#include "glk/tads/tads3/tct3ty.h"
#define TCTARGTY_TARGET_DEFINED__
#endif

/*
 *   Javascript Code Generator 
 */
#ifdef TC_TARGET_JS
#include "glk/tads/tads3/tcjsty.h"
#define TCTARGTY_TARGET_DEFINED__
#endif


/* ------------------------------------------------------------------------ */
/*
 *   ensure that a code generator was defined - if not, the compilation
 *   cannot proceed 
 */
#ifndef TADS3_TCTARGTY_TARGET_DEFINED__
#ifdef TODO
#error No code generator target is defined.  A code generator must be defined in your makefile.  See tctarg.h for details.
#endif
#endif

#endif
