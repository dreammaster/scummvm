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

#define WGT2ALLEGRO_NOFUNCTIONS
#include "ags2/common/compress.h"
#include "ags2/common/wgt2allg.h"
#include "ags2/lib/allegro/color.h"
#include "ags2/common/misc.h"

namespace AGS2 {

typedef unsigned char *__block;

extern long cliboffset(char *);
extern char lib_file_name[13];
extern void domouse(int);
extern block wnewblock(int, int, int, int);

#ifndef __CJONES_H
long csavecompressed(char *, __block, color[256], long = 0);
long cloadcompressed(char *, __block, color *, long = 0);
#endif

void cpackbitl(unsigned char *line, int size, Common::WriteStream *outfile) {
	int cnt = 0;                  // bytes encoded

	while (cnt < size) {
		int i = cnt;
		int j = i + 1;
		int jmax = i + 126;
		if (jmax >= size)
			jmax = size - 1;

		if (i == size - 1) {        //................last byte alone
			outfile->writeByte(0);
			outfile->writeByte(line[i]);
			cnt++;

		} else if (line[i] == line[j]) {    //....run
			while ((j < jmax) && (line[j] == line[j + 1]))
				j++;

			outfile->writeByte(i - j);
			outfile->writeByte(line[i]);
			cnt += j - i + 1;

		} else {                    //.............................sequence
			while ((j < jmax) && (line[j] != line[j + 1]))
				j++;

			outfile->writeByte(j - i);
			outfile->write(line + i, j - i + 1);
			cnt += j - i + 1;

		}
	} // end while
}

void cpackbitl16(unsigned short *line, int size, Common::WriteStream *outfile) {
	int cnt = 0;                  // bytes encoded

	while (cnt < size) {
		int i = cnt;
		int j = i + 1;
		int jmax = i + 126;
		if (jmax >= size)
			jmax = size - 1;

		if (i == size - 1) {        //................last byte alone
			outfile->writeByte(0);
			outfile->writeSint16LE(line[i]);
			cnt++;

		} else if (line[i] == line[j]) {    //....run
			while ((j < jmax) && (line[j] == line[j + 1]))
				j++;

			outfile->writeByte(i - j);
			outfile->writeSint16LE(line[i]);
			cnt += j - i + 1;

		} else {                    //.............................sequence
			while ((j < jmax) && (line[j] != line[j + 1]))
				j++;

			outfile->writeByte(j - i);
			outfile->write(line + i, (j - i + 1) * 2);
			cnt += j - i + 1;

		}
	} // end while
}

void cpackbitl32(unsigned long *line, int size, Common::WriteStream *outfile) {
	int cnt = 0;                  // bytes encoded

	while (cnt < size) {
		int i = cnt;
		int j = i + 1;
		int jmax = i + 126;
		if (jmax >= size)
			jmax = size - 1;

		if (i == size - 1) {        //................last byte alone
			outfile->writeByte(0);
			outfile->writeUint32LE(line[i]);
			cnt++;

		} else if (line[i] == line[j]) {    //....run
			while ((j < jmax) && (line[j] == line[j + 1]))
				j++;

			outfile->writeByte(i - j);
			outfile->writeUint32LE(line[i]);
			cnt += j - i + 1;

		} else {                    //.............................sequence
			while ((j < jmax) && (line[j] != line[j + 1]))
				j++;

			outfile->writeByte(j - i);
			outfile->write(line + i, (j - i + 1) * 4);
			cnt += j - i + 1;
		}
	} // end while
}


long csavecompressed(char *finam, __block tobesaved, color pala[256], long exto) {
	Common::WriteStream *outpt;

	if (exto > 0) {
		outpt = ci_fopen_w(finam, "a+b");
		dynamic_cast<Common::SeekableWriteStream *>(outpt)->seek(exto, SEEK_SET);
	} else {
		outpt = ci_fopen_w(finam, "wb");
	}

	int widt, hit;
	long ofes;
	widt = *tobesaved++;
	widt += (*tobesaved++) * 256;
	hit = *tobesaved++;
	hit += (*tobesaved++) * 256;
	outpt->writeUint16LE(widt);
	outpt->writeUint16LE(hit);

	unsigned char *ress = (unsigned char *)malloc(widt + 1);
	int ww;

	for (ww = 0; ww < hit; ww++) {
		for (int ss = 0; ss < widt; ss++)
			(*ress++) = (*tobesaved++);

		ress -= widt;
		cpackbitl(ress, widt, outpt);
	}

	for (ww = 0; ww < 256; ww++) {
		outpt->writeByte(pala[ww].r);
		outpt->writeByte(pala[ww].g);
		outpt->writeByte(pala[ww].b);
	}

	ofes = outpt->pos();
	delete outpt;
	free(ress);
	return ofes;
}

int cunpackbitl(unsigned char *line, int size, Common::SeekableReadStream *infile) {
	int n = 0;                    // number of bytes decoded

	while (n < size && !infile->eos()) {
		int8 ix = infile->readSByte();     // get index byte

		int8 cx = ix;
		if (cx == -128)
			cx = 0;

		if (cx < 0) {                //.............run
			int i = 1 - cx;
			unsigned char ch = infile->readByte();
			while (i--) {
				// test for buffer overflow
				if (n >= size)
					return -1;

				line[n++] = ch;
			}
		} else {                     //.....................seq
			int i = cx + 1;
			while (i--) {
				// test for buffer overflow
				if (n >= size)
					return -1;

				line[n++] = infile->readByte();
			}
		}
	}

	return 0;
}

int cunpackbitl16(unsigned short *line, int size, Common::SeekableReadStream *infile) {
	int n = 0;                    // number of bytes decoded

	while (n < size && !infile->eos()) {
		int ix = infile->readSByte();     // get index byte

		char cx = ix;
		if (cx == -128)
			cx = 0;

		if (cx < 0) {                //.............run
			int i = 1 - cx;
			unsigned short ch = infile->readSint16LE();
			while (i--) {
				// test for buffer overflow
				if (n >= size)
					return -1;

				line[n++] = ch;
			}
		} else {                     //.....................seq
			int i = cx + 1;
			while (i--) {
				// test for buffer overflow
				if (n >= size)
					return -1;

				line[n++] = getshort(infile);
			}
		}
	}

	return 0;
}

int cunpackbitl32(unsigned long *line, int size, Common::SeekableReadStream *infile) {
	int n = 0;                    // number of bytes decoded

	while (n < size && !infile->eos()) {
		int ix = infile->readSByte();     // get index byte

		char cx = ix;
		if (cx == -128)
			cx = 0;

		if (cx < 0) {                //.............run
			int i = 1 - cx;
			unsigned long ch = infile->readUint32LE();
			while (i--) {
				// test for buffer overflow
				if (n >= size)
					return -1;

				line[n++] = ch;
			}
		} else {                     //.....................seq
			int i = cx + 1;
			while (i--) {
				// test for buffer overflow
				if (n >= size)
					return -1;

				line[n++] = infile->readUint32LE();
			}
		}
	}

	return 0;
}

} // namespace AGS2
