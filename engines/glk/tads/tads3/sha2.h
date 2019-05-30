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

#ifndef TADS3_SHA2_H
#define TADS3_SHA2_H

#include "common/scummsys.h"

namespace Glk {
namespace TADS {
namespace TADS3 {

/*  Defines for suffixes to 32 and 64 bit unsigned numeric values   */

#define sfx_lo(x,y) x##y
#define sfx_hi(x,y) sfx_lo(x,y)
#define n_u32(p)    sfx_hi(0x##p,s_u32)
#define n_u64(p)    sfx_hi(0x##p,s_u64)

/* define an unsigned 32-bit type */

#if UINT_MAX == 0xffffffff
  typedef   unsigned int     sha2_32t;
  #define s_u32    u
#elif ULONG_MAX == 0xffffffff
  typedef   unsigned long    sha2_32t;
  #define s_u32   ul
#else
#error Please define sha2_32t as an unsigned 32 bit type in sha2.h
#endif

/* define an unsigned 64-bit type */

#if defined( _MSC_VER )
  typedef unsigned __int64   sha2_64t;
  #define s_u64 ui64
#elif ULONG_MAX == 0xffffffffffffffff
  typedef unsigned long      sha2_64t;
  #define s_u64   ul
#elif ULONG_MAX == 0xffffffff
  typedef unsigned long long sha2_64t;   /* a somewhat dangerous guess */
  #define s_u64  ull
#else
#error Please define sha2_64t as an unsigned 64 bit type in sha2.h
#endif

//#if defined(__cplusplus)
//extern "C"
//{
//#endif

#define SHA256_DIGEST_SIZE  32
#define SHA384_DIGEST_SIZE  48
#define SHA512_DIGEST_SIZE  64

#define SHA256_BLOCK_SIZE   64
#define SHA384_BLOCK_SIZE  128
#define SHA512_BLOCK_SIZE  128

#define SHA2_DIGEST_SIZE        SHA256_DIGEST_SIZE
#define SHA2_MAX_DIGEST_SIZE    SHA512_DIGEST_SIZE

#define SHA2_GOOD   0
#define SHA2_BAD    1

/* type to hold the SHA256 context                              */

struct sha256_ctx {
	sha2_32t count[2];
	sha2_32t hash[8];
	sha2_32t wbuf[16];
};

/* type to hold the SHA384/512 context                  */

struct sha512_ctx {
	sha2_64t count[2];
	sha2_64t hash[8];
	sha2_64t wbuf[16];
};

typedef sha512_ctx  sha384_ctx;

/* type to hold a SHA2 context (256/384/512)  */

typedef struct
{   union
    {   sha256_ctx  ctx256[1];
        sha512_ctx  ctx512[1];
    } uu[1];
    sha2_32t    sha2_len;
} sha2_ctx;

void sha256_compile(sha256_ctx ctx[1]);
void sha512_compile(sha512_ctx ctx[1]);

void sha256_begin(sha256_ctx ctx[1]);
void sha256_hash(const unsigned char data[], unsigned long len, sha256_ctx ctx[1]);
void sha256_end(unsigned char hval[], sha256_ctx ctx[1]);
void sha256(unsigned char hval[], const unsigned char data[], unsigned long len);

/* 
 *   Generate a printable version of a hash for a given buffer.  'hash' is an
 *   array of at least 65 characters to receive the hash string.  It's fine
 *   to pass in the same buffer for both 'hash' and 'data', as long as it's
 *   big enough (>=65 characters). [mjr] 
 */
void sha256_ez(char *hash, const char *data, size_t data_len);

/*
 *   Generate a printable version of a hash for a given data source. [mjr]
 */
void sha256_datasrc(char *hash, class CVmDataSource *src, unsigned long len);

/* 
 *   printf-style hash construction: format the string given by 'fmt' and the
 *   subsequent arguments, and hash the result 
 */
void sha256_ezf(char *hash, const char *fmt, ...);

void sha384_begin(sha384_ctx ctx[1]);
#define sha384_hash sha512_hash
void sha384_end(unsigned char hval[], sha384_ctx ctx[1]);
void sha384(unsigned char hval[], const unsigned char data[], unsigned long len); 

void sha512_begin(sha512_ctx ctx[1]);
void sha512_hash(const unsigned char data[], unsigned long len, sha512_ctx ctx[1]);
void sha512_end(unsigned char hval[], sha512_ctx ctx[1]);
void sha512(unsigned char hval[], const unsigned char data[], unsigned long len); 

int sha2_begin(unsigned long size, sha2_ctx ctx[1]);
void sha2_hash(const unsigned char data[], unsigned long len, sha2_ctx ctx[1]);
void sha2_end(unsigned char hval[], sha2_ctx ctx[1]);
int sha2(unsigned char hval[], unsigned long size, const unsigned char data[], unsigned long len); 

//#if defined(__cplusplus)
//}
//#endif

} // End of namespace TADS3
} // End of namespace TADS
} // End of namespace Glk

#endif
