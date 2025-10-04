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

#ifndef SPYCRAFT_ADVERROR_H
#define SPYCRAFT_ADVERROR_H

namespace Spycraft {

enum {
	__ERR_CODING,
	__ERR_FILE_OPEN_FAIL,
	__ERR_MEM_ALLOC_FAIL,
	__ERR_PORT_ALLOC_FAIL,
	__ERR_BUFFER_OVERFLOW,
	__ERR_EXCEED_LISTSIZE,
	__ERR_INVALID_POINTER,
	__ERR_SPRITE_CREATE_FAIL,
	__ERR_RES_NOT_FOUND,
	__ERR_CANNOT_PURGE_RES,
	__ERR_DECOMPRESS_SIZE_MISMATCH,
	__ERR_INIT_SOUND_FAIL,
	__ERR_INVALID_WAVE,
	__ERR_INIT_MUSIC_FAIL,
	__ERR_WIN_API_FAIL,
	__ERR_MCI_API_FAIL,
	__ERR_HMI_API_FAIL,
	__ERR_PK_API_FAIL,
	__ERR_ILLEGAL_API,
	__ERR_PK_INTERNAL_FAIL,
	__ERR_INVALID_ATS,
	__ERR_INVALID_PPM,
	__ERR_INVALID_TGA
};

#define ADV_ASSERT(EXPR, VAL) assert(EXPR)

typedef int32 MADEErr32;		// 32 bit error type
typedef int16 MADEErr;			// error type

//	error codes
enum {
	fnfError = -43,				// File not found
	memFullError = -108,		// not enough room to allocate requested memory
	noPrjError = -10001,		// no such project
	noRsrcError = -10002,		// no such resource
	noRsrcTypeError = -10003,	// no such resource type
	fckFINoError = -10004		// miscellaneous error
};

} // namespace Spycraft

#endif
