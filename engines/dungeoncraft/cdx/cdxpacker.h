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

#define N			4096/* size of ring buffer */
#define F			60	/* upper limit for match_length */
#define THRESHOLD	2   /* encode string into position and length
						   if match_length is greater than this */
#define NIL			N	/* index for root of binary search trees */
#define M   15

#define Q1  (1UL << M)
#define Q2  (2 * Q1)
#define Q3  (3 * Q1)
#define Q4  (4 * Q1)
#define MAX_CUM (Q1 - 1)

#define N_CHAR  (256 - THRESHOLD + F)

typedef void (* EXPANDPROC)(char ch, DWORD pos, DWORD width, DWORD heigth, void *data );
typedef int (* PACKPROC)(DWORD pos, DWORD width, DWORD heigth, void *data );

class CDXAPI CDXPacker
{
public:
	int FilePtrInfo( FILE *in, int &w, int &h, BYTE &b );
	int FileInfo( char *in, int &w, int &h, BYTE &b );
	BOOL PackFile( char *in, char *out );
	BOOL PackRam( BYTE *in, DWORD w, DWORD h, BYTE d, char *out );
	BOOL PackTo( DWORD w, DWORD h, BYTE d, PACKPROC lpProc, void *data, char *out );
	BOOL ExpandToFile( char *in, char *out );
	BYTE *ExpandToRam( char *in );
	BYTE *ExpandToRam( FILE *in );
	BOOL ExpandTo( char *in, EXPANDPROC lpProc, void *data );
	BOOL ExpandTo( FILE *in, EXPANDPROC lpProc, void *data );
	BOOL ExpandSurface( char *in, CDXSurface *lpDDS );
	BOOL ExpandSurface( FILE *in, CDXSurface *lpDDS );

	DWORD textsize, codesize, width, height;
	BYTE bpp;

private:
	void Init( int comp );
	void StartModel(void);
	void UpdateModel(int sym);

	int SpecWrite( BYTE ch );
	int SpecRead( void );
	BOOL ReadHeader( void );
	BOOL WriteHeader( void );

	/* Expand routinez */
	int GetBit(void);
	int BinarySearchSym(WORD x);
	int BinarySearchPos(WORD x);
	void StartDecode(void);
	int DecodeChar(void);
	int DecodePosition(void);
	BOOL Decode(void);

	/* Compress Routinez */
	BOOL PutBit( int bit );
	void FlushBitBuffer( void );
	void InitTree(void);
	void InsertNode(int r);
	void DeleteNode(int p);
	void Output(int bit);
	void EncodeChar(int ch);
	void EncodePosition(int position);
	void EncodeEnd(void);
	BOOL Encode(void);

	EXPANDPROC m_lpEProc;
	PACKPROC m_lpPProc;
	void *m_data;
	DWORD pos, mode;

	BYTE *ramptr;

	BYTE text_buf[N + F - 1];
	int	match_position, match_length, shifts,
		lson[N + 1], rson[N + 257], dad[N + 1],
		char_to_sym[N_CHAR], sym_to_char[N_CHAR + 1];
	WORD	sym_freq[N_CHAR + 1],
			sym_cum[N_CHAR + 1],
			position_cum[N + 1],
			buffer, mask;

	DWORD  low, high, value;
	FILE *fin, *fout;
};

void DibBlit( CDXSurface *lpDDS, int dw, int dh, char *data, int sw, int sh, int bpp  );
