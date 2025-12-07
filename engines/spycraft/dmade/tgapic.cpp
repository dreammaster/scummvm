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

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/tgapic.h"
#include "spycraft/dmade/advport.h"
#include "spycraft/dmade/advback.h"
#include "spycraft/dmade/advfile.h"

namespace Spycraft {

extern int scene_width;
extern int scene_height;
extern int pixFormat;

void Format565 ( Viewport *port );

#if 0
#include "tga.h"

static TGA tgaHeader;

GamePalette tgaPalette;

/* claim, info, and decode must be called in order for the same picture */
int ClaimTGA(char *picData, long picSize)
{

	if (ReadTgaHeader(&tgaHeader, picData, picSize) < 0)
		return false;
	if ((tgaHeader.Head.PixelDepth != 8) && (tgaHeader.Head.PixelDepth != 16))
		return false;
	if ((tgaHeader.Head.ImageType != 1) && (tgaHeader.Head.ImageType != 2))
		return false;
	return true;
}

void InfoTGA(char *picData, long picSize, uint16 *width, uint16 *height, uint16 *colors)
{
	*width = tgaHeader.Head.ImageWidth;
	*height = tgaHeader.Head.ImageHeight;
	*colors = tgaHeader.Head.PixelDepth;
}

void DecodeTGA(char *picData, long picSize, Viewport *port )
{
	char *pixelData, *decodedBuffer;
	int r, c;
	uint16 colorTable[256], *dst16;
	int realHeight = tgaHeader.Head.ImageHeight;

	/* this is pretty much a hack to deal with a specific kind
	   of targa. it should be made more general
	*/
	/* bottom up targa */
	decodedBuffer = (BYTE *)port->ptr + ((realHeight - 1) * port->rowBytes);
	pixelData = picData + tgaHeader.TgaDataOffset;
	if (tgaHeader.Head.PixelDepth == port->colors) {
		// no depth conversion
		for (r = 0; r < tgaHeader.Head.ImageHeight; r++) {
			memcpy(decodedBuffer, pixelData, port->rowBytes);
			decodedBuffer -= port->rowBytes;
			pixelData += port->rowBytes;
		}	
		if (tgaHeader.Head.PixelDepth == 8)
			memcpy(tgaPalette, tgaHeader.Cmap.CmapData, sizeof(GamePalette));
	} 
	else {
		if (tgaHeader.Head.PixelDepth == 8) {
			// convert 8 to 16
			memcpy(tgaPalette, tgaHeader.Cmap.CmapData, sizeof(GamePalette));
			for (r = 0; r < 256; r++)
				colorTable[r] =
					((uint16)tgaPalette[r].red >> 3) << 10 |
					((uint16)tgaPalette[r].green >> 3) << 5 |
					((uint16)tgaPalette[r].blue >> 3);
				for (r = 0; r < tgaHeader.Head.ImageHeight; r++) {
					dst16 = (uint16 *)decodedBuffer;
					for (c = 0; c < tgaHeader.Head.ImageWidth; c++)
						*dst16++ = colorTable[*pixelData++];
					decodedBuffer -= port->rowBytes;
				}
		} else {
			// convert 16 to 8
		}
	}

   /* FOR 565 FORMAT */
   if ( ( port->colors == 16 ) && pixFormat ) {
      Format565 ( port );
   }
}

Viewport *MakePort(int width, int height, uint16 colors)
{
	Viewport *aPort;
	long bitmapSize;

	/* ALLOCATE A VIEWPORT */
	aPort = (Viewport *)AllocPtr ( sizeof ( Viewport ) );
	if ( aPort == NULL ) {
		MessageBox ( NULL, "Failed to allocate a viewport", "", MB_OK );
		exit ( 1 );
	}

	bitmapSize = (long)width * height;
	if ( colors != 8 )
		bitmapSize *= 2;

	/* ALLOCATE BIT DATA */
	aPort->ptr = AllocPtr ( bitmapSize );
	aPort->width = width;
	aPort->rowBytes = width * (colors == 8 ? 1 : 2);
	aPort->height = height;
	aPort->origX = 0;
	aPort->origY = 0;
	aPort->colors = colors;

	if ( aPort->ptr == NULL ) {
		FreePtr ( aPort );
		aPort = NULL;
	}

	return aPort;
}
  
Viewport *OpenTGAFile ( char *filename )
{
	int hFile;
	char *picData;
	int32 picSize;
	uint16 width, height, colors;
	Viewport *port;

	picSize = sfxFileSize ( filename );
	hFile = sfxOpenFile ( filename, MADE_FILE_READ );
	if ( hFile == -1 )	{
		 
		ASSERT(false, __ERR_FILE_OPEN_FAIL);

	picData = AllocPtr ( picSize );

	if ( picData == NULL ) {
		sfxCloseFile ( hFile );
		return ( NULL );
	}

	sfxReadFile ( hFile, picData, picSize );
	sfxCloseFile ( hFile );
	
	if ( !ClaimTGA ( picData, picSize ) )
		ASSERT ( false, __ERR_INVALID_TGA );

	InfoTGA ( picData, picSize, &width, &height, &colors);
	port = MakePort ( width, height, colors );

	if ( port == NULL ) {
		FreeTGA ( &tgaHeader, picData );
		return ( NULL );
	}

	DecodeTGA ( picData, picSize, port );
	FreeTGA ( &tgaHeader, picData );

	return ( port);
}

Viewport *OpenPICFile ( char *filename, int widthNeed, int heightNeed )
{
	int hFile;
	char *picData;
	int32 picSize;
	uint16 width, height, colors;
	Viewport *port;

	picSize = sfxFileSize ( filename );
	hFile = sfxOpenFile ( filename, MADE_FILE_READ );
	if ( hFile == -1 ) 
		ASSERT ( false, __ERR_FILE_OPEN_FAIL );

	picData = AllocPtr ( picSize );
	if ( picData == NULL ) {
		sfxCloseFile ( hFile );
		return ( NULL );
	}
	
	sfxReadFile ( hFile, picData, picSize );
	sfxCloseFile ( hFile );
	
	if ( !ClaimTGA ( picData, picSize ) )
		ASSERT ( false, __ERR_INVALID_TGA );

	InfoTGA ( picData, picSize, &width, &height, &colors);
	port = MakePort ( widthNeed, heightNeed, colors );
	if ( port == NULL ) {
		FreeTGA ( &tgaHeader, picData );
		return ( NULL );	
	}

	DecodeTGA ( picData, picSize, port );
	FreeTGA ( &tgaHeader, picData );

	return ( port);
}
#endif

Viewport *OpenPic ( DcmpStream dStream )
{
	Viewport *port;
	struct {
		char junk[8];
		uint16 x_org, y_org;
		uint16 w, h;
		uint8 depth, desc;
	} tgaHeader;
	int i;
	char *line;

	ReadDcmpStream(dStream, &tgaHeader, sizeof(tgaHeader));
	ASSERT(tgaHeader.depth == 16, __ERR_INVALID_TGA);
	ASSERT(tgaHeader.w == scene_width, __ERR_INVALID_TGA);
	ASSERT(tgaHeader.h == scene_height, __ERR_INVALID_TGA);
	ASSERT(tgaHeader.junk[0] == 0, __ERR_INVALID_TGA);	// no image id field
	ASSERT(tgaHeader.junk[2] == 2, __ERR_INVALID_TGA);	// true color
	port = AllocPort(scene_width, scene_height, 16);
	if (port == NULL)
		return NULL;
	line = ((char *)port->ptr + port->rowBytes * (scene_height - 1));
	for (i = 0; i < scene_height; i++) {
		ReadDcmpStream(dStream, line, port->rowBytes);
		line -= port->rowBytes;
	}
	if (pixFormat)
		Format565(port);
	return (port);
}

} // namespace Spycraft
