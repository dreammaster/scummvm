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

#ifndef AGS2_COMMON_CSCOMP_H
#define AGS2_COMMON_CSCOMP_H

#include "common/stream.h"
#include "ags2/common/fmem.h"

namespace AGS2 {

#define SCOM_VERSION 89
#define SCOM_VERSIONSTR "0.89"

struct ccScript;
struct ccInstance;

// ********* SCRIPT COMPILATION FUNCTIONS **************
// add a script that will be compiled as a header into every compilation
// 'name' is the name of the header, used in error reports
// (only the pointer is stored so don't free the memory)
extern int ccAddDefaultHeader(char *script, char *name);
// don't compile any headers into the compilation
extern void ccRemoveDefaultHeaders(void);

// define a macro which will affect all compilations
void ccDefineMacro(const char *macro, const char *definition);
// clear all predefined macros
void ccClearAllMacros();

// set version for use with #ifversion macros
void ccSetSoftwareVersion(const char *version);

// compile the script supplied, returns NULL on failure
extern ccScript *ccCompileText(const char *script, const char *scriptName);
// write the script to disk (after compiling)
extern void fwrite_script(ccScript *, Common::WriteStream *);
// free the memory occupied by the script - do NOT attempt to run the
// script after calling this function
extern void ccFreeScript(ccScript *);

// ************ SCRIPT LOADING AND RUNNING FUNCTIONS ************
// read back a script written with fwrite_script
extern ccScript *fread_script(Common::SeekableReadStream *);

// give the script access to a variable or function in your program
extern void ccAddExternalSymbol(const char *, void *);
// remove the script access to a variable or function in your program
extern void ccRemoveExternalSymbol(char *);
// create a runnable instance of the supplied script
extern ccInstance *ccCreateInstance(ccScript *);
// create a runnable instance of the same script, sharing global memory
extern ccInstance *ccForkInstance(ccInstance *);
// free the memory associated with the instance
extern void ccFreeInstance(ccInstance *);
// get the address of an exported variable in the script
extern char *ccGetSymbolAddr(ccInstance *, const char *);
// call an exported function in the script (3rd arg is number of params)
extern int ccCallInstance(ccInstance *, const char *, long, ...);
// specifies that when the current function returns to the script, it
// will stop and return from CallInstance
extern void ccAbortInstance(ccInstance *);
// aborts instance, then frees the memory later when it is done with
extern void ccAbortAndDestroyInstance(ccInstance *);
// returns the currently executing instance, or NULL if none
extern ccInstance *ccGetCurrentInstance(void);
// removes all external symbols, allowing you to start from scratch
extern void ccRemoveAllSymbols();
// Set the number of while loop iterations that aborts the script
extern void ccSetScriptAliveTimer(int);
// reset the current while loop counter
extern void ccNotifyScriptStillAlive();

// OBJECT-BASED SCRIPTING RUNTIME FUNCTIONS
// interface 
struct ICCDynamicObject {
	virtual ~ICCDynamicObject() {}

	// when a ref count reaches 0, this is called with the address
	// of the object. Return 1 to remove the object from memory, 0 to
	// leave it
	virtual int Dispose(const char *address, bool force) = 0;
	// return the type name of the object
	virtual const char *GetType() = 0;
	// serialize the object into BUFFER (which is BUFSIZE bytes)
	// return number of bytes used
	virtual int Serialize(const char *address, char *buffer, int bufsize) = 0;
};
struct ICCObjectReader {
	virtual ~ICCObjectReader() {}
	virtual void Unserialize(int index, const char *objectType, const char *serializedData, int dataSize) = 0;
};
struct ICCStringClass {
	virtual ~ICCStringClass() {}
	virtual void *CreateString(const char *fromText) = 0;
};

// set the class that will be used for dynamic strings
extern void  ccSetStringClassImpl(ICCStringClass *theClass);
// register a memory handle for the object and allow script
// pointers to point to it
extern long  ccRegisterManagedObject(const void *object, ICCDynamicObject *);
// register a de-serialized object
extern long  ccRegisterUnserializedObject(int index, const void *object, ICCDynamicObject *);
// unregister a particular object
extern int   ccUnRegisterManagedObject(const void *object);
// remove all registered objects
extern void  ccUnregisterAllObjects();
// serialize all objects to disk
extern void  ccSerializeAllObjects(Common::WriteStream *output);
// un-serialise all objects (will remove all currently registered ones)
extern int   ccUnserializeAllObjects(Common::SeekableReadStream *input, ICCObjectReader *callback);
// dispose the object if RefCount==0
extern void  ccAttemptDisposeObject(long handle);
// translate between object handles and memory addresses
extern long  ccGetObjectHandleFromAddress(const char *address);
extern const char *ccGetObjectAddressFromHandle(long handle);

// DEBUG HOOK
typedef void (*new_line_hook_type) (ccInstance *, int);
extern void ccSetDebugHook(new_line_hook_type jibble);


// *********** MISCELLANEOUS FUNCTIONS ***************
#define SCOPT_EXPORTALL      1   // export all functions automatically
#define SCOPT_SHOWWARNINGS   2   // printf warnings to console
#define SCOPT_LINENUMBERS    4   // include line numbers in compiled code
#define SCOPT_AUTOIMPORT     8   // when creating instance, export funcs to other scripts
#define SCOPT_DEBUGRUN    0x10   // write instructions as they are procssed to log file
#define SCOPT_NOIMPORTOVERRIDE 0x20 // do not allow an import to be re-declared
#define SCOPT_LEFTTORIGHT 0x40   // left-to-right operator precedance
#define SCOPT_OLDSTRINGS  0x80   // allow old-style strings

extern void ccSetOption(int, int);
extern int ccGetOption(int);

// error reporting
extern int ccError;             // set to non-zero if error occurs
extern int ccErrorLine;         // line number of the error
extern char ccErrorString[400]; // description of the error
extern char ccErrorCallStack[400];
extern bool ccErrorIsUserError;
extern const char *ccCurScriptName; // name of currently compiling script

extern void ccGetCallStack(ccInstance *inst, char *buffer, int maxLines);

// *
// ***************** INTERNAL USE ONLY *******************
// *
extern void cc_error(const char *, ...);
extern int currentline;

extern int is_alphanum(int);
extern void cc_preprocess(const char *, const char *);
extern void preproc_startup(void);
extern void preproc_shutdown(void);

extern int ccAddObjectReference(long handle);
extern int ccReleaseObjectReference(long handle);
extern void fputstring(const char *sss, Common::WriteStream *ddd);
extern void fgetstring_limit(char *sss, Common::ReadStream *ddd, int bufsize);
extern void fgetstring(char *sss, Common::ReadStream *ddd);

#define FIXUP_GLOBALDATA  1     // code[fixup] += &globaldata[0]
#define FIXUP_FUNCTION    2     // code[fixup] += &code[0]
#define FIXUP_STRING      3     // code[fixup] += &strings[0]
#define FIXUP_IMPORT      4     // code[fixup] = &imported_thing[code[fixup]]
#define FIXUP_DATADATA    5     // globaldata[fixup] += &globaldata[0]
#define FIXUP_STACK       6     // code[fixup] += &stack[0]
#define EXPORT_FUNCTION   1
#define EXPORT_DATA       2

struct ICompareStrings {
	virtual ~ICompareStrings() {
	}

	virtual int compare(const char *left, const char *right) {
		return strcmp(left, right);
	}
};

// Binary tree structure for holding strings, allows fast access
struct ccTreeMap {
	ccTreeMap *left, *right;
	const char *text;
	int value;

	ccTreeMap();
	ccTreeMap *findNode(const char *key, ICompareStrings *comparer);
	int findValue(const char *key, ICompareStrings *comparer);
	int findValue(const char *key);
	void Clone(ccTreeMap *node);
	void removeNode();
	void removeEntry(const char *key);
	void addEntry(const char *ntx, int p_value);
	void destroyNonRecursive();
	void clear();
	~ccTreeMap();
};


struct ccScript
{
	char *globaldata;
	long globaldatasize;
	long *code;
	long codesize;
	char *strings;
	long stringssize;
	char *fixuptypes;             // global data/string area/ etc
	long *fixups;                 // code array index to fixup (in longs)
	int numfixups;
	int importsCapacity;
	char **imports;
	int numimports;
	int exportsCapacity;
	char **exports;   // names of exports
	long *export_addr;        // high byte is type; low 24-bits are offset
	int numexports;
	int instances;
	// 'sections' allow the interpreter to find out which bit
	// of the code came from header files, and which from the main file
	char **sectionNames;
	long *sectionOffsets;
	int numSections;
	int capacitySections;
};

#define SREG_SP           1     // stack pointer
#define SREG_MAR          2     // memory address register
#define SREG_AX           3     // general purpose
#define SREG_BX           4
#define SREG_CX           5
#define SREG_OP           6    // object pointer for member func calls
#define SREG_DX           7
#define CC_NUM_REGISTERS  8
#define INSTF_SHAREDATA   1
#define INSTF_ABORTED     2
#define INSTF_FREE        4
#define INSTF_RUNNING     8   // set by main code to confirm script isn't stuck
#define CC_STACK_SIZE     4000
#define MAX_CALL_STACK    100

struct ccInstance {
	long flags;
	char *globaldata;
	long globaldatasize;
	unsigned long *code;
	ccInstance *runningInst;  // might point to another instance if in far call
	long codesize;
	char *strings;
	long stringssize;
	char **exportaddr;  // real pointer to export
	char *stack;
	long stacksize;
	long registers[CC_NUM_REGISTERS];
	long pc;                        // program counter
	long line_number;               // source code line number
	ccScript *instanceof;
	long callStackLineNumber[MAX_CALL_STACK];
	long callStackAddr[MAX_CALL_STACK];
	ccInstance *callStackCodeInst[MAX_CALL_STACK];
	int  callStackSize;
	int  loadedInstanceId;
	int  returnValue;
};

// virtual CPU commands
#define SCMD_ADD          1     // reg1 += arg2
#define SCMD_SUB          2     // reg1 -= arg2
#define SCMD_REGTOREG     3     // reg2 = reg1
#define SCMD_WRITELIT     4     // m[MAR] = arg2 (copy arg1 bytes)
#define SCMD_RET          5     // return from subroutine
#define SCMD_LITTOREG     6     // set reg1 to literal value arg2
#define SCMD_MEMREAD      7     // reg1 = m[MAR]
#define SCMD_MEMWRITE     8     // m[MAR] = reg1
#define SCMD_MULREG       9     // reg1 *= reg2
#define SCMD_DIVREG       10    // reg1 /= reg2
#define SCMD_ADDREG       11    // reg1 += reg2
#define SCMD_SUBREG       12    // reg1 -= reg2
#define SCMD_BITAND       13    // bitwise  reg1 & reg2
#define SCMD_BITOR        14    // bitwise  reg1 | reg2
#define SCMD_ISEQUAL      15    // reg1 == reg2   reg1=1 if true, =0 if not
#define SCMD_NOTEQUAL     16    // reg1 != reg2
#define SCMD_GREATER      17    // reg1 > reg2
#define SCMD_LESSTHAN     18    // reg1 < reg2
#define SCMD_GTE          19    // reg1 >= reg2
#define SCMD_LTE          20    // reg1 <= reg2
#define SCMD_AND          21    // (reg1!=0) && (reg2!=0) -> reg1
#define SCMD_OR           22    // (reg1!=0) || (reg2!=0) -> reg1
#define SCMD_CALL         23    // jump to subroutine at reg1
#define SCMD_MEMREADB     24    // reg1 = m[MAR] (1 byte)
#define SCMD_MEMREADW     25    // reg1 = m[MAR] (2 bytes)
#define SCMD_MEMWRITEB    26    // m[MAR] = reg1 (1 byte)
#define SCMD_MEMWRITEW    27    // m[MAR] = reg1 (2 bytes)
#define SCMD_JZ           28    // jump if ax==0 to arg1
#define SCMD_PUSHREG      29    // m[sp]=reg1; sp++
#define SCMD_POPREG       30    // sp--; reg1=m[sp]
#define SCMD_JMP          31    // jump to arg1
#define SCMD_MUL          32    // reg1 *= arg2
#define SCMD_CALLEXT      33    // call external (imported) function reg1
#define SCMD_PUSHREAL     34    // push reg1 onto real stack
#define SCMD_SUBREALSTACK 35
#define SCMD_LINENUM      36    // debug info - source code line number
#define SCMD_CALLAS       37    // call external script function
#define SCMD_THISBASE     38    // current relative address
#define SCMD_NUMFUNCARGS  39    // number of arguments for ext func call
#define SCMD_MODREG       40    // reg1 %= reg2
#define SCMD_XORREG       41    // reg1 ^= reg2
#define SCMD_NOTREG       42    // reg1 = !reg1
#define SCMD_SHIFTLEFT    43    // reg1 = reg1 << reg2
#define SCMD_SHIFTRIGHT   44    // reg1 = reg1 >> reg2
#define SCMD_CALLOBJ      45    // next call is member function of reg1
#define SCMD_CHECKBOUNDS  46    // check reg1 is between 0 and arg2
#define SCMD_MEMWRITEPTR  47    // m[MAR] = reg1 (adjust ptr addr)
#define SCMD_MEMREADPTR   48    // reg1 = m[MAR] (adjust ptr addr)
#define SCMD_MEMZEROPTR   49    // m[MAR] = 0    (blank ptr)
#define SCMD_MEMINITPTR   50    // m[MAR] = reg1 (but don't free old one)
#define SCMD_LOADSPOFFS   51    // MAR = SP - arg1 (optimization for local var access)
#define SCMD_CHECKNULL    52    // error if MAR==0
#define SCMD_FADD         53    // reg1 += arg2 (float,int)
#define SCMD_FSUB         54    // reg1 -= arg2 (float,int)
#define SCMD_FMULREG      55    // reg1 *= reg2 (float)
#define SCMD_FDIVREG      56    // reg1 /= reg2 (float)
#define SCMD_FADDREG      57    // reg1 += reg2 (float)
#define SCMD_FSUBREG      58    // reg1 -= reg2 (float)
#define SCMD_FGREATER     59    // reg1 > reg2 (float)
#define SCMD_FLESSTHAN    60    // reg1 < reg2 (float)
#define SCMD_FGTE         61    // reg1 >= reg2 (float)
#define SCMD_FLTE         62    // reg1 <= reg2 (float)
#define SCMD_ZEROMEMORY   63    // m[MAR]..m[MAR+(arg1-1)] = 0
#define SCMD_CREATESTRING 64    // reg1 = new String(reg1)
#define SCMD_STRINGSEQUAL 65    // (char*)reg1 == (char*)reg2   reg1=1 if true, =0 if not
#define SCMD_STRINGSNOTEQ 66    // (char*)reg1 != (char*)reg2
#define SCMD_CHECKNULLREG 67    // error if reg1 == NULL
#define SCMD_LOOPCHECKOFF 68    // no loop checking for this function
#define SCMD_MEMZEROPTRND 69    // m[MAR] = 0    (blank ptr, no dispose if = ax)
#define SCMD_JNZ          70    // jump to arg1 if ax!=0
#define SCMD_DYNAMICBOUNDS 71   // check reg1 is between 0 and m[MAR-4]
#define SCMD_NEWARRAY     72    // reg1 = new array of reg1 elements, each of size arg2 (arg3=managed type?)

#define ENDFILESIG 0xbeefcafe

} // namespace AGS2

#endif
