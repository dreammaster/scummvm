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

//////////////////////////////////////////////////////////////////////////////////
// CDXImageTGA.h: interface for the CDXImageTGA class.
//
//A Targa file is a header, followed by an identification string, followed by
//a color map, followed by the data for the image. Both the identification
//string and the color-map can be of zero length.
//////////////////////////////////////////////////////////////////////////////////

#ifndef DUNGEONCRAFT_CDX_CDXIMAGETGA
#define DUNGEONCRAFT_CDX_CDXIMAGETGA

#include "cdximagebase.h"

namespace DungeonCraft {
namespace CDX {

#define	TGA_NO_IMAGE	0		// No image data included in file
#define	TGA_UNCOMP_CM	1		// Uncompressed, Color-Mapped (VDA/D 
								// and Targa M-8 images)             
#define	TGA_UNCOMP_RGB	2		// Uncompressed, RGB images (eg: ICB 
								// Targa 16, 24 and 32)              
#define	TGA_UNCOMP_BW	3		// Uncompressed, B/W images (eg:     
								// Targa 8 and Targa M-8 images)     
#define	TGA_RL_CM		9		// Run-length, Color-Mapped (VDA/D   
								// and Targa M-8 images)             
#define	TGA_RL_RGB		10		// Run-length, RGB images (eg: ICB   
								// Targa 16, 24 and 32)              
#define	TGA_RL_BW		11		// Run-length, B/W images (eg: Targa 
								// 8 and Targa M-8)                  
#define	TGA_COMP_CM		32		// Compressed Color-Mapped (VDA/D)   
								// data using Huffman, Delta, and    
								// run length encoding               
#define	TGA_COMP_CM_4	33		// Compressed Color-Mapped (VDA/D)   
								// data using Huffman, Delta, and    
								// run length encoding in 4 passes   

#define	IDB_ATTRIBUTES	0x0f	// How many attrib bits per pixel    
								// ie: 1 for T16, 8 for T32          
#define	IDB_ORIGIN		0x20	// Origin in top left corner bit     
								// else its in bottom left corner    
#define	IDB_INTERLEAVE	0xc0	// Interleave bits as defined below  
#define	IDB_NON_INT		0x00	// Non-Interlaced                    
#define	IDB_2_WAY		0x40	// 2 way (even/odd) interleaving     
#define	IDB_4_WAY		0x80	// 4 way interleaving (eg: AT&T PC)  


#pragma pack(1)

struct _tgaHeader 
{
	unsigned char     cIDLength;
	unsigned char     cColorMapType;
	unsigned char     cType;
	unsigned short    iFirstColourMapEntry;
	unsigned short    iColourMapLength;
	unsigned char     cColourMapEntrySize;
	unsigned short    iImageXOrigin;
	unsigned short    iImageYOrigin;
	unsigned short    iWidth;
	unsigned short    iHeight;
	unsigned char     cBPP;
	unsigned char     cDescriptorBits;
};

struct _tgaFooter 
{
    unsigned long   extarea_offset;
    unsigned long   devdir_offset;
    unsigned char   signature[16];
    unsigned char   period;         // always be "."
    unsigned char   null;           // (0x00)
};

struct _tgaStruct 
{
	_tgaHeader    header;
	char          *ID;
	_tgaFooter    footer;
	unsigned char *color_map;
};

#pragma pack()

/////////////////////////////////////////////////////////////////////////


class CDXImageTGA : public  CDXImageBase
{
public:
	CDXImageTGA(void);
	virtual ~CDXImageTGA(void);

    CDX_LPDIRECTDRAWSURFACE GetImage(CDX_LPDIRECTDRAW lpDD, LONG lSize, CHAR* lpCache, BYTE memType);
	BOOL Validate(const CHAR *head, const int headsize, const CHAR *foot, const int footsize);
	void t16_t24(BYTE *dest, const BYTE *src, int n);
	void t32_t24(BYTE *dest, const BYTE *src, int n);

private:
	_tgaStruct	m_tgaInfo;
};

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
