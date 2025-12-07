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
#include "spycraft/dmade/tga.h"

namespace Spycraft {

static char *tgaStart, *tgaCurrent;

/* get byte from TGA data stream */
static BYTE GetByte(void) {
	return *tgaCurrent++;
}

/* get word from TGA data stream */
static WORD GetWord(void) {
	WORD val;
	CHAR *p;

#if 0
	p = (CHAR *)&val + sizeof(val);
	*--p = *tgaCurrent++;
	*--p = *tgaCurrent++;
#else
	p = (CHAR *)&val;
	*p++ = *tgaCurrent++;
	*p++ = *tgaCurrent++;
#endif
	return val;
}

/* get long word from TGA data stream */
static DWORD GetDword(void) {
	DWORD val;
	CHAR *p;

#if 0
	p = (CHAR *)&val + sizeof(val);
	*--p = *tgaCurrent++;
	*--p = *tgaCurrent++;
	*--p = *tgaCurrent++;
	*--p = *tgaCurrent++;
#else
	p = (CHAR *)&val;
	*p++ = *tgaCurrent++;
	*p++ = *tgaCurrent++;
	*p++ = *tgaCurrent++;
	*p++ = *tgaCurrent++;
#endif
	return val;
}

/*
**  Read the information from a TGA file info a TGAHEADER structure.
**
**  The TGAHEADER structure (defined in the file TGA.H) contains all of the
**  information found in both version 1.0 and 2.0 of the TGA format except
**  for the actual image data.  Image data is not stored because of its
**  typically large size.
**  TgaHead - Pointer to TGA header structure
**  TgaData - Pointer to TGA image data stream
**  TagSize - Size of TGA image data 
**  Returns: A negative value if a memory allocation error occured,
**           otherwise 0 if no errors occur.
*/
SHORT ReadTgaHeader(TGA *TgaHead, CHAR *TgaData, DWORD TgaSize) {
	WORD    i, j;           /* Loop counters                                */
	WORD    cmapsize;       /* Size of the color map in bytes               */
	WORD    stampsize;      /* Size of the postage stamp image in bytes     */
	TGATAG *tag;            /* TGA tag structure                            */
	TGATAG *head;           /* Head of TGA tag linked list                  */
	TGATAG *current_tag = nullptr;    /* Current node of linked list                  */

	tgaCurrent = tgaStart = TgaData;

#if 0
	GetWord = GetLittleWord;    /* Read using little-endian byte order */
	GetDword = GetLittleDword;
#endif

	/*
	** Read the TGA header (Version 1.0 and 2.0).
	*/
	TgaHead->Head.IdLength = GetByte();
	TgaHead->Head.CmapType = GetByte();
	TgaHead->Head.ImageType = GetByte();
	TgaHead->Head.CmapIndex = GetWord();
	TgaHead->Head.CmapLength = GetWord();
	TgaHead->Head.CmapEntrySize = GetByte();
	TgaHead->Head.X_Origin = GetWord();
	TgaHead->Head.Y_Origin = GetWord();
	TgaHead->Head.ImageWidth = GetWord();
	TgaHead->Head.ImageHeight = GetWord();
	TgaHead->Head.PixelDepth = GetByte();
	TgaHead->Head.ImagDesc = GetByte();

	/* Read the Image ID field. */
	if (TgaHead->Head.IdLength)
	{
		/* Allocate memory for the Id Field data */
		if ((TgaHead->Cmap.IdField =
			(CHAR *)AllocPtr(TgaHead->Head.IdLength)) ==
			(CHAR *)NULL)
		{
			return(-1);     /* Failed to allocate memory */
		}

		/* Read the Image ID data */
		/* Do not assume that it is a NULL-terminated string */
		for (i = 0; i < TgaHead->Head.IdLength; i++)
			TgaHead->Cmap.IdField[i] = (CHAR)GetByte();
	}

	/*
	** Read the color map data (Version 1.0 and 2.0).
	*/
	if (TgaHead->Head.CmapType)
	{
		/* Determine the size of the color map */
		cmapsize = ((TgaHead->Head.CmapEntrySize + 7) >> 3) *
			TgaHead->Head.CmapLength;

		/* Allocate memory for the color map data */
		if ((TgaHead->Cmap.CmapData =
			(BYTE *)AllocPtr(cmapsize)) == (BYTE *)NULL)
		{
			return(-2);     /* Failed to allocate memory */
		}

		/* Read the color map data */
		for (i = 0; i < cmapsize; i++)
			TgaHead->Cmap.CmapData[i] = GetByte();
	}

	/*
	** Store the offset of the image data.  This field is not part of
	** the TGA format, but it helps the software locate the data when
	** reading/writing TGA files.
	*/
	TgaHead->TgaDataOffset = tgaCurrent - tgaStart;

	/*
	** Check the version of the TGA file (Versions 1.0 and 2.0).
	*/
	tgaCurrent = tgaStart + TgaSize - 26;	/* Seek to the (possible) TGA footer */

	/* Read in the (possible) offset values */
	TgaHead->Foot.ExtensionOffset = GetDword();
	TgaHead->Foot.DeveloperOffset = GetDword();

	/* Read in the (possible) signature */
	for (i = 0; i < sizeof(TgaHead->Foot.Signature); i++)
		TgaHead->Foot.Signature[i] = (CHAR)GetByte();

	/* Check if the data read is a TGA signature string */
	if (!strcmp(TgaHead->Foot.Signature, TGASIGNATURE))
		TgaHead->NewTgaFormat = true;   /* Yes.  Version 2.0 */
	else
		TgaHead->NewTgaFormat = false;  /* No.  Version 1.0 */

	/*
	** If version is 2.0 then check for developer and extension areas.
	*/
	if (TgaHead->NewTgaFormat == true)
	{
		/* Check for the presence of a developers area */
		if (TgaHead->Foot.DeveloperOffset)
		{
			/* Seek to the developer area */
			tgaCurrent = tgaStart + TgaHead->Foot.DeveloperOffset;

			/* Get the number of tags in the directory */
			TgaHead->Developer.NumberOfTags = GetWord();

			/* Read the tags and store as a singly-linked list */
			head = (TGATAG *)NULL;
			for (i = 0; i < TgaHead->Developer.NumberOfTags; i++)
			{
				/* Allocate the tag */
				if ((tag =
					(TGATAG *)AllocPtr(sizeof(TGATAG))) == (TGATAG *)NULL)
					return(-3);     /* Failed to allocate memory */

				/* Read the tag information */
				tag->TagNumber = GetWord();
				tag->TagOffset = GetDword();
				tag->TagSize = GetDword();

				/*
				** Read the tag data
				*/
				/* Seek to the tag data */
				tgaCurrent = tgaStart + tag->TagOffset;

				if ((tag->TagData =
					(BYTE *)AllocPtr(tag->TagSize)) == (BYTE *)NULL)
					return(-3);     /* Failed to allocate memory */

				/* Read the tag data */
				for (j = 0; j < tag->TagSize; j++)
					tag->TagData[j] = GetByte();

				/* Link the tag to the list */
				if (head == (TGATAG *)NULL)
				{
					head = tag;
					current_tag = head;
				} else
				{
					current_tag->Next = tag;
					tag->Next = (TGATAG *)NULL;
					current_tag = tag;
				}
			}
			TgaHead->Developer.TagList = head;  /* Assign list to structure */
		}

		/* Check for the presence of an extension area */
		if (TgaHead->Foot.ExtensionOffset)
		{
			/* Seek to the extension area */
			tgaCurrent = tgaStart + TgaHead->Foot.ExtensionOffset;

			/* Read the extension area size information */
			TgaHead->Extension.Size = GetWord();

			/* Read the extension information defined in TGA version 2.0 */
			for (i = 0; i < sizeof(TgaHead->Extension.AuthorName); i++)
				TgaHead->Extension.AuthorName[i] = (CHAR)GetByte();

			for (i = 0; i < sizeof(TgaHead->Extension.AuthorComment); i++)
				TgaHead->Extension.AuthorComment[i] = (CHAR)GetByte();

			TgaHead->Extension.StampMonth = GetWord();
			TgaHead->Extension.StampDay = GetWord();
			TgaHead->Extension.StampYear = GetWord();
			TgaHead->Extension.StampHour = GetWord();
			TgaHead->Extension.StampMinute = GetWord();
			TgaHead->Extension.StampSecond = GetWord();

			for (i = 0; i < sizeof(TgaHead->Extension.JobName); i++)
				TgaHead->Extension.JobName[i] = (CHAR)GetByte();

			TgaHead->Extension.JobHour = GetWord();
			TgaHead->Extension.JobMinute = GetWord();
			TgaHead->Extension.JobSecond = GetWord();

			for (i = 0; i < sizeof(TgaHead->Extension.SoftwareId); i++)
				TgaHead->Extension.SoftwareId[i] = (CHAR)GetByte();

			TgaHead->Extension.VersionNumber = GetWord();
			TgaHead->Extension.VersionLetter = GetByte();
			TgaHead->Extension.KeyColor = GetDword();
			TgaHead->Extension.PixelNumerator = GetWord();
			TgaHead->Extension.PixelDenominator = GetWord();
			TgaHead->Extension.GammaNumerator = GetWord();
			TgaHead->Extension.GammaDenominator = GetWord();
			TgaHead->Extension.ColorOffset = GetDword();
			TgaHead->Extension.StampOffset = GetDword();
			TgaHead->Extension.ScanOffset = GetDword();
			TgaHead->Extension.AttributesType = GetByte();

			/* Check for the presence of a scan line table */
			if (TgaHead->Extension.ScanOffset)
			{
				/* Seek to the postage scan line table */
				tgaCurrent = tgaStart + TgaHead->Extension.ScanOffset;

				/* Allocate memory.  One DWORD per line in image */
				if ((TgaHead->Extension.ScanLineTable =
					(DWORD *)AllocPtr(TgaHead->Head.ImageHeight * sizeof(DWORD))) ==
					(DWORD *)NULL)
				{
					return(-4);     /* Failed to allocate memory */
				}

				/* Read in scan line offset values */
				for (i = 0; i < TgaHead->Head.ImageHeight; i++)
					TgaHead->Extension.ScanLineTable[i] = GetDword();
			}

			/* Check for the presence of a postage stamp image*/
			if (TgaHead->Extension.StampOffset)
			{
				/* Seek to the postage stamp image */
				tgaCurrent = tgaStart + TgaHead->Extension.StampOffset;

				/* Read the size of the stamp in pixels */
				TgaHead->Extension.StampWidth = GetByte();
				TgaHead->Extension.StampHeight = GetByte();

				/* Calculate the size of the stamp in bytes */
				stampsize = TgaHead->Extension.StampWidth *
					TgaHead->Extension.StampHeight *
					((TgaHead->Head.PixelDepth + 7) >> 3);

				/* Allocate memory for the postage stamp image */
				if ((TgaHead->Extension.StampImage =
					(BYTE *)AllocPtr(stampsize)) == (BYTE *)NULL)
				{
					return(-5);     /* Failed to allocate memory */
				}

				/* Read the stamp data one byte at a time */
				for (i = 0; i < stampsize; i++)
					TgaHead->Extension.StampImage[i] = GetByte();
			}

			/* Check for the presence of a color correction table */
			if (TgaHead->Extension.ColorOffset)
			{
				/* Seek to the color correction table */
				tgaCurrent = tgaStart + TgaHead->Extension.ColorOffset;

				/* Read in the entire 1024 WORD (2048 byte) table */
				for (i = 0; i < sizeof(TgaHead->Extension.ColorTable); i++)
					TgaHead->Extension.ColorTable[i] = GetWord();
			}
		}
	}
	return(0);
}

void FreeTGA(TGA *TgaHead, char *TgaData) {
	/* Free the Image ID field. */
	if (TgaHead->Head.IdLength)
		FreePtr(TgaHead->Cmap.IdField);

	if (TgaHead->Head.CmapType)
		/* Free the color map */
		FreePtr(TgaHead->Cmap.CmapData);

	/*
	** If version is 2.0 then check for developer and extension areas.
	*/
	if (TgaHead->NewTgaFormat == true)
	{
		/* Check for the presence of a developers area */
		if (TgaHead->Foot.DeveloperOffset)
			/* Free all Tag */
			FreePtr(TgaHead->Developer.TagList);

		/* Check for the presence of an extension area */
		if (TgaHead->Foot.ExtensionOffset)
		{
			/* Check for the presence of a scan line table */
			if (TgaHead->Extension.ScanOffset)
				FreePtr(TgaHead->Extension.ScanLineTable);


			/* Free the postage stamp image */
			if (TgaHead->Extension.StampOffset)
				FreePtr(TgaHead->Extension.StampImage);
		}
	}

	FreePtr(TgaData);
}

} // namespace Spycraft
