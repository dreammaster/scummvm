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

#include "common/textconsole.h"
#include "ags2/common/lzw.h"

namespace AGS2 {

#if 0
int insert(int, int);
void _delete(int);
void lzwcompress(FILE *, FILE *);
void lzwexpand(FILE *, FILE *);
#endif

#define N 4096
#define F 16
#define THRESHOLD 3
#define min(xx,yy) ((yy<xx) ? yy : xx)

#define dad (node+1)
#define lson (node+1+N)
#define rson (node+1+N+N)
#define root (node+1+N+N+N)
#define NIL -1

static char *lzbuffer;
static int *node;
static int pos;
static int expand_to_mem = 0;
static unsigned char *membfptr = NULL;

static int insert(int i, int run) {
	int c = 0, j, k, l, n, match;
	int *p;

	k = l = 1;
	match = THRESHOLD - 1;
	p = &root[(unsigned char)lzbuffer[i]];
	lson[i] = rson[i] = NIL;
	while ((j = *p) != NIL) {
		for (n = min(k, l); n < run && (c = (lzbuffer[j + n] - lzbuffer[i + n])) == 0; n++);

		if (n > match) {
			match = n;
			pos = j;
		}

		if (c < 0) {
			p = &lson[j];
			k = n;
		} else if (c > 0) {
			p = &rson[j];
			l = n;
		} else {
			dad[j] = NIL;
			dad[lson[j]] = lson + i - node;
			dad[rson[j]] = rson + i - node;
			lson[i] = lson[j];
			rson[i] = rson[j];
			break;
		}
	}

	dad[i] = p - node;
	*p = i;
	return match;
}

static void _delete(int z) {
	int j;

	if (dad[z] != NIL) {
		if (rson[z] == NIL)
			j = lson[z];
		else if (lson[z] == NIL)
			j = rson[z];
		else {
			j = lson[z];
			if (rson[j] != NIL) {
				do {
					j = rson[j];
				} while (rson[j] != NIL);

				node[dad[j]] = lson[j];
				dad[lson[j]] = dad[j];
				lson[j] = lson[z];
				dad[lson[z]] = lson + j - node;
			}

			rson[j] = rson[z];
			dad[rson[z]] = rson + j - node;
		}

		dad[j] = dad[z];
		node[dad[z]] = j;
		dad[z] = NIL;
	}
}

void lzwcompress(Common::SeekableReadStream *f, Common::WriteStream *out) {
	int ch, i, run, len, match, size, mask;
	char buf[17];

	lzbuffer = (char *)malloc(N + F + (N + 1 + N + N + 256) * sizeof(int));       // 28.5 k !
	if (lzbuffer == NULL) {
		error("unable to compress: out of memory");
	}

	node = (int *)(lzbuffer + N + F);
	for (i = 0; i < 256; i++)
		root[i] = NIL;

	for (i = NIL; i < N; i++)
		dad[i] = NIL;

	size = mask = 1;
	buf[0] = 0;
	i = N - F - F;

	for (len = 0; len < F && !f->eos(); ++len) {
		ch = f->readByte();
		lzbuffer[i + F] = ch;
		i = (i + 1) & (N - 1);
	}

	run = len;

	do {
		ch = f->readByte();
		if (i >= N - F) {
			_delete(i + F - N);
			lzbuffer[i + F] = lzbuffer[i + F - N] = ch;
		} else {
			_delete(i + F);
			lzbuffer[i + F] = ch;
		}

		match = insert(i, run);
		if (ch == -1) {
			run--;
			len--;
		}

		if (len++ >= run) {
			if (match >= THRESHOLD) {
				buf[0] |= mask;
				// possible fix: change int* to short* ??
				*(short *)(buf + size) = ((match - 3) << 12) | ((i - pos - 1) & (N - 1));
				size += 2;
				len -= match;
			} else {
				buf[size++] = lzbuffer[i];
				len--;
			}

			if (!((mask += mask) & 0xFF)) {
				out->write(buf, size);
				outbytes += size;
				size = mask = 1;
				buf[0] = 0;
			}
		}
		i = (i + 1) & (N - 1);
	} while (len > 0);

	if (size > 1) {
		out->write(buf, size);
		outbytes += size;
	}

	free(lzbuffer);
}

static void myputc(int ccc, Common::WriteStream *ooo) {
	if (maxsize > 0) {
		putbytes++;
		if (putbytes > maxsize)
			return;
	}

	outbytes++;
	if (expand_to_mem) {
		membfptr[0] = ccc;
		membfptr++;
	} else {
		ooo->writeByte(ccc);
	}
}

#undef putc
#define putc myputc

void lzwexpand(Common::SeekableReadStream *f, Common::WriteStream *out) {
	int bits, ch, i, j, len, mask;
	char *buf;
	putbytes = 0;

	buf = (char *)malloc(N);
	if (buf == NULL) {
		error("compress.cpp: unable to decompress: insufficient memory");
	}
	i = N - F;

	// this end condition just checks for EOF, which is no good to us
	while ((bits = f->readByte()) != -1) {
		for (mask = 0x01; mask & 0xFF; mask <<= 1) {
			if (bits & mask) {
				j = f->readSint16LE();

				len = ((j >> 12) & 15) + 3;
				j = (i - j - 1) & (N - 1);

				while (len--) {
					putc(buf[i] = buf[j], out);
					j = (j + 1) & (N - 1);
					i = (i + 1) & (N - 1);
				}
			} else {
				ch = f->readByte();
				putc(buf[i] = ch, out);
				i = (i + 1) & (N - 1);
			}

			if ((putbytes >= maxsize) && (maxsize > 0))
				break;

			if (f->eos() && (maxsize > 0))
				error("Read error decompressing image - file is corrupt");
		}                           // end for mask

		if ((putbytes >= maxsize) && (maxsize > 0))
			break;
	}

	free(buf);
	expand_to_mem = 0;
}

unsigned char *lzwexpand_to_mem(Common::SeekableReadStream *ii) {
	unsigned char *membuff = (unsigned char *)malloc(maxsize + 10);
	expand_to_mem = 1;
	membfptr = membuff;
	lzwexpand(ii, NULL);
	return membuff;
}

} // namespace AGS2
