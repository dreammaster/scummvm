/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
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

//
//  Runtime heap/resource class
//

#ifndef AESOP_RES_STRUCTURES_H
#define AESOP_RES_STRUCTURES_H

#include "common/file.h"
#include "aesop/defs.h"

namespace Aesop {

enum {
    DA_FIXED        = 0x00000001U,   // Entry attribute equates
    DA_MOVEABLE     = 0x00000000U,   // (only low word preserved in cache)
    DA_PRECIOUS     = 0x00000020U,
    DA_DISCARDABLE  = 0x00000010U,
    DA_TEMPORARY    = 0x00000000U,

    DA_EVANESCENT   = 0x00000040U,   // Special runtime attribute equates
    DA_DISCARDED    = 0x00000100U,
    DA_FREE         = 0x00000200U,
    DA_DEFAULT      = 0xFFFFFFFFU,

    DA_PLACEHOLDER  = 0x10000000L
};

#define SA_UNUSED      0x00000001   // Storage attribute flag equates
#define SA_DELETED     0x00000002

#define ROED 0                      // Resource Ordinal Entry Directory
#define RDES 1                      // Resource Description Directory
#define RDEP 2                      // Resource Dependency Directory
#define CRFD 3                      // Code Resource Function Directory

#define RTR_FREEBASE   0x0001U   // TRUE for destructor to free heap memory

#define SIZE_DB (DIR_BLK * sizeof(HD_entry))
#define DOS_BUFFSIZE 32768U
#define OD_SIZE        128       // # of entries/ordinal file directory block

// cached resource entry descriptor
struct HD_entry {
	void *_seg;                  // pointer to resource data

	ULONG _size;                 // size of resource in bytes
	ULONG _flags;                // DA_ flags
	ULONG _history;              // LRU counter value
	ULONG _locks;                // locking depth
	ULONG _user;                 // .RES file offset or instance object name
};

// resource file header
struct RF_file_hdr {
	BYTE  _signature[16];
	ULONG _fileSize;
	ULONG _lostSpace;
	ULONG _FOB;
	ULONG _createTime;
	ULONG _modifyTime;

	RF_file_hdr();

	/**
	 * Load header data from the passed stream
	*/
	void load(Common::SeekableReadStream &s);
};

// resource file entry header
struct RF_entry_hdr {
	ULONG _timestamp;
	ULONG _dataAttrib;
	ULONG _dataSize;

	RF_entry_hdr();

	/**
	 * Load header data from the passed stream
	*/
	void load(Common::SeekableReadStream &s);
};

struct OD_block {
	ULONG _next;
	UBYTE _flags[OD_SIZE];
	ULONG _index[OD_SIZE];

	OD_block();

	/**
	 * Load header data from the passed stream
	*/
	void load(Common::SeekableReadStream &s);
};

// name directory entry
struct ND_entry {
	ULONG OE;                     // public
	HRES thunk;                   // public
	HRES handle;                  // public
};

// External code reference entry
struct XCR_entry {
	ULONG val;
};

// External data reference entry
struct XDR_entry {
	UWORD offset;
};


// SOP program header
struct PRG_HDR {
	uint _staticSize;
	uint _imports;
	uint _exports;
	uint _parent;

	PRG_HDR();

	/**
	 * Load header data from the passed stream
	 */
	void load(Common::SeekableReadStream &s);
};

// Instance header
struct IHDR {
	HRES _thunk;

	IHDR() : _thunk(nullptr) {}
};

// Thunk header
struct THDR {
	UWORD _mvList;
	UWORD _maxMsg;
	UWORD _sdList;
	UWORD _nPrgs;
	UWORD _iSize;
	UWORD _useCount;

	THDR();
};

// Palette resource header
struct PAL_RES {
	UWORD _nColors;
	Common::Array<byte> _fade[11];
	Common::Array <byte> _rgb;

	/**
	* Load header data from the passed stream
	*/
	void load(Common::SeekableReadStream &s);
};

// static context descriptor
struct CDESC {
	UWORD slot;       // object list index
	ULONG name;       // code object name
	UWORD size;       // size of instance data (unused in text files)
};

} // End of namespace Aesop

#endif

