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

#ifndef SPYCRAFT_DMADE_TGA_H
#define SPYCRAFT_DMADE_TGA_H

namespace Spycraft {

typedef char CHAR;
typedef short SHORT;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

/*
**  TGA File Header
*/
typedef struct _TgaHeader
{
	BYTE   IdLength;            /* Image ID Field Length      */
	BYTE   CmapType;            /* Color Map Type             */
	BYTE   ImageType;           /* Image Type                 */
	/*
	** Color Map Specification
	*/
	WORD   CmapIndex;           /* First Entry Index          */
	WORD   CmapLength;          /* Color Map Length           */
	BYTE   CmapEntrySize;       /* Color Map Entry Size       */
	/*
	** Image Specification
	*/
	WORD   X_Origin;            /* X-origin of Image          */
	WORD   Y_Origin;            /* Y-origin of Image          */
	WORD   ImageWidth;          /* Image Width                */
	WORD   ImageHeight;         /* Image Height               */
	BYTE   PixelDepth;          /* Pixel Depth                */
	BYTE   ImagDesc;            /* Image Descriptor           */
} TGAHEADER;


/*
**  Image/Color Map Data
*/
typedef struct _TgaColorMap
{
	CHAR *IdField;             /* Image ID Field             */
	BYTE *CmapData;            /* Color Map Data             */
} TGACMAP;


/*
**  Developer Area Tag Structure
*/
typedef struct _TgaTag
{
	WORD    TagNumber;           /* Number of the Tag          */
	DWORD   TagOffset;           /* Offset of the Tag Data     */
	DWORD   TagSize;             /* Size of the Tag Data       */
	BYTE *TagData;             /* Pointer to the Tag Data    */
	struct _TgaTag *Next;        /* Link to next Tag           */
} TGATAG;


/*
**  Developer Area
*/
typedef struct _TgaDeveloper
{
	WORD    NumberOfTags;     /* Number of Tags in Directory  */
	TGATAG *TagList;          /* Link to list of Tags         */
} TGADEVELOPER;


/*
**  Extension Area
*/
typedef struct _TgaExtension
{
	WORD   Size;                /* Extension Size             */
	CHAR   AuthorName[41];      /* Author Name                */
	CHAR   AuthorComment[324];  /* Author Comment             */
	WORD   StampMonth;          /* Date/Time Stamp: Month     */
	WORD   StampDay;            /* Date/Time Stamp: Day       */
	WORD   StampYear;           /* Date/Time Stamp: Year      */
	WORD   StampHour;           /* Date/Time Stamp: Hour      */
	WORD   StampMinute;         /* Date/Time Stamp: Minute    */
	WORD   StampSecond;         /* Date/Time Stamp: Second    */
	CHAR   JobName[41];         /* Job Name/ID                */
	WORD   JobHour;             /* Job Time: Hours            */
	WORD   JobMinute;           /* Job Time: Minutes          */
	WORD   JobSecond;           /* Job Time: Seconds          */
	CHAR   SoftwareId[41];      /* Software ID                */
	WORD   VersionNumber;       /* Software Version Number    */
	BYTE   VersionLetter;       /* Software Version Letter    */
	DWORD  KeyColor;            /* Key Color                  */
	WORD   PixelNumerator;      /* Pixel Aspect Ratio Numerator   */
	WORD   PixelDenominator;    /* Pixel Aspect Ratio Denominator */
	WORD   GammaNumerator;      /* Gamma Value                */
	WORD   GammaDenominator;    /* Gamma Value                */
	DWORD  ColorOffset;         /* Color Correction Offset    */
	DWORD  StampOffset;         /* Postage Stamp Offset       */
	DWORD  ScanOffset;          /* Scan Line Table Offset     */
	BYTE   AttributesType;      /* Attributes Types           */
	DWORD *ScanLineTable;       /* Scan Line Table            */
	BYTE   StampWidth;          /* Width of postage stamp image */
	BYTE   StampHeight;         /* Height of postage stamp image */
	BYTE *StampImage;          /* Postage Stamp Image        */
	WORD   ColorTable[1024];    /* Color Correction Table     */
} TGAEXTENSION;


/*
**  TGA File Footer
*/
typedef struct _TgaFooter
{
	DWORD ExtensionOffset;      /* Extension Area Offset      */
	DWORD DeveloperOffset;      /* Developer Directory Offset */
	CHAR  Signature[18];        /* Signature, dot, and NULL   */
} TGAFOOTER;


/*
**  Complete TGA File Format
*/
typedef struct _TgaFormat
{
	TGAHEADER      Head;            /* Header Area                          */
	TGACMAP        Cmap;            /* Image/Color Map Area                 */
	TGADEVELOPER   Developer;       /* Developer Area                       */
	TGAEXTENSION   Extension;       /* Extension Area                       */
	TGAFOOTER      Foot;            /* File Footer Area                     */
	bool           NewTgaFormat;    /* true if file is v2.0 TGA format      */
	DWORD          TgaDataOffset;   /* Offset of the image data in the file */
} TGA;

#define TGASIGNATURE   "TRUEVISION-XFILE.\0"
#define BYTESPERPIXEL   ((tgaHead.Head.PixelDepth + 7) >> 3)

/*
**  Function prototypes
*/
SHORT ReadTgaHeader(TGA *, CHAR *, DWORD);
void  FreeTGA(TGA *, CHAR *);

} // namespace Spycraft

#endif
