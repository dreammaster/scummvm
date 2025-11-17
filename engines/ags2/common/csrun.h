/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef AGS2_COMMON_CSRUN_H
#define AGS2_COMMON_CSRUN_H

#include "ags2/common/cscomp.h"

namespace AGS2 {

extern void nullfree(void *data);
extern void ccAddExternalSymbol(char *namof, void *addrof);
extern void ccRemoveExternalSymbol(char *namof);
extern void ccRemoveAllSymbols();
extern void *ccGetSymbolAddress(char *namof);
extern ccInstance *ccGetCurrentInstance();

/**
 * If a while loop does this many iterations without the NofityScriptAlive function
 * getting called, the script aborts. Set to 0 to disable.
 */
extern void ccSetScriptAliveTimer(int numloop);
extern void ccNotifyScriptStillAlive();
extern ccInstance *ccCreateInstanceEx(ccScript *scri, ccInstance *joined);

/**
 * Changes all pointer variables(ie.strings) to have the relative address, to allow
 * the data segment to be saved to disk
 */
extern void ccFlattenGlobalData(ccInstance *cinst);

/**
 * Restores the pointers after a save
 */
extern void ccUnFlattenGlobalData(ccInstance *cinst);
extern ccInstance *ccCreateInstance(ccScript *scri);
extern ccInstance *ccForkInstance(ccInstance *instoff);
extern void ccFreeInstance(ccInstance *cinst);

/**
 * Get a pointer to a variable or function exported by the script
 */
extern char *ccGetSymbolAddr(ccInstance *inst, const char *symname);

extern void ccSetStringClassImpl(ICCStringClass *theClass);
extern long ccRegisterManagedObject(const void *object, ICCDynamicObject *callback);
extern long ccRegisterUnserializedObject(int index, const void *object, ICCDynamicObject *callback);
extern int ccUnRegisterManagedObject(const void *object);
extern void ccAttemptDisposeObject(long handle);
extern void ccUnregisterAllObjects();
extern void ccSerializeAllObjects(Common::WriteStream *output);
extern int ccUnserializeAllObjects(Common::ReadStream *input, ICCObjectReader *callback);
extern long ccGetObjectHandleFromAddress(const char *address);
extern const char *ccGetObjectAddressFromHandle(long handle);
extern int ccAddObjectReference(long handle);
extern int ccReleaseObjectReference(long handle);
void ccSetDebugHook(new_line_hook_type jibble);

/**
 * Parm list is backwards(last arg is parms[0])
 */
extern int call_function(long addr, int numparm, long *parms, int offset);
extern int cc_run_code(ccInstance *inst, long curpc);
extern int ccCallInstance(ccInstance *inst, char *funcname, long numargs, ...);
extern void ccAbortInstance(ccInstance *cinst);
extern void ccAbortAndDestroyInstance(ccInstance *inst);
extern void freadstring(char **strptr, Common::ReadStream *iii);
extern long fget_long(Common::ReadStream *iii);
extern ccScript *fread_script(Common::ReadStream *ooo);

} // namespace AGS2

#endif
