/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software{} you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation{} either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program{} if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/* blorblib.c: Blorb file reader library, version 1.0.2.
    Designed by Andrew Plotkin <erkyrath@eblong.com>
    http://www.eblong.com/zarf/blorb/index.html

    This is portable code to read a Blorb file. Add it to your
    interpreter, #include "blorb.h", and you're ready to go.
*/

#include "engines/glk/zcode/blorb/blorb.h"
#include "engines/glk/zcode/blorb/blorblow.h"

namespace Glk {
namespace ZCode {

static int lib_inited = FALSE;

static bb_err_t bb_initialize_map(bb_map_t *map);
static bb_err_t bb_initialize(void);
static int sortsplot(const void *p1, const void *p2);
static int bigendian;

static uint16 bb_native2(uint16 v) {
	if (bigendian) return v;
	return ((((uint16)(v) >> 8) & 0x00ff)
	        | (((uint16)(v) << 8) & 0xff00));
}

static uint32 bb_native4(uint32 v) {
	if (bigendian) return v;
	return ((((uint32)(v) >> 24) & 0x000000ff)
	        | (((uint32)(v) >>  8) & 0x0000ff00)
	        | (((uint32)(v) <<  8) & 0x00ff0000)
	        | (((uint32)(v) << 24) & 0xff000000));
}

/* Do some one-time startup tests. */
static bb_err_t bb_initialize() {
	int i = 1;
	char *p = (char *)&i;
	union {
		uint32 val;
		unsigned char ch[4];
	} test;
	uint32 val;

	if (p[0] == 1) bigendian = 0;
	else bigendian = 1;

	if (sizeof(uint32) != 4 || sizeof(uint16) != 2)
		return bb_err_CompileTime; /* Basic types are the wrong size. */

	test.ch[0] = 0x13;
	test.ch[1] = 0x57;
	test.ch[2] = 0x9a;
	test.ch[3] = 0xce;
	val = test.val;
	if (bb_native4(val) != 0x13579ace)
		return bb_err_CompileTime; /* Wrong endianness. */

	return bb_err_None;
}

bb_err_t bb_create_map(FILE *file, bb_map_t **newmap) {
	bb_err_t err;
	bb_map_t *map;
	size_t readlen;
	uint32 nextpos, totallength;
	bb_chunkdesc_t *chunks;
	int chunks_size, numchunks;
	uint32 buffer[4];

	*newmap = nullptr;

	if (!lib_inited) {
		err = bb_initialize();
		if (err)
			return err;
		lib_inited = TRUE;
	}

	/* First, chew through the file and index the chunks. */

	err = fseek(file, 0, 0);
	if (err)
		return bb_err_Read;

	readlen = fread(buffer, sizeof(uint32), 3, file);
	if (readlen != 3)
		return bb_err_Read;

	if (bb_native4(buffer[0]) != bb_ID_FORM)
		return bb_err_Format;
	if (bb_native4(buffer[2]) != bb_ID_IFRS)
		return bb_err_Format;

	totallength = bb_native4(buffer[1]) + 8;
	nextpos = 12;

	chunks_size = 8;
	numchunks = 0;
	chunks = (bb_chunkdesc_t *)malloc(sizeof(bb_chunkdesc_t) * chunks_size);

	while (nextpos < totallength) {
		uint32 type, len;
		int chunum;
		bb_chunkdesc_t *chu;

		err = fseek(file, nextpos, 0);
		if (err)
			return bb_err_Read;

		readlen = fread(buffer, sizeof(uint32), 2, file);
		if (readlen != 2)
			return bb_err_Read;

		type = bb_native4(buffer[0]);
		len = bb_native4(buffer[1]);

		if (numchunks >= chunks_size) {
			chunks_size *= 2;
			chunks = (bb_chunkdesc_t *)realloc(chunks,
			                                   sizeof(bb_chunkdesc_t) * chunks_size);
		}

		chunum = numchunks;
		chu = &(chunks[chunum]);
		numchunks++;

		chu->type = type;
		chu->startpos = nextpos;
		if (type == bb_ID_FORM) {
			chu->datpos = nextpos;
			chu->len = len + 8;
		} else {
			chu->datpos = nextpos + 8;
			chu->len = len;
		}
		chu->ptr = nullptr;
		chu->auxdatnum = -1;

		nextpos = nextpos + len + 8;
		if (nextpos & 1)
			nextpos++;

		if (nextpos > totallength)
			return bb_err_Format;
	}

	/* The basic IFF structure seems to be ok, and we have a list of
	    chunks. Now we allocate the map structure itself. */

	map = (bb_map_t *)malloc(sizeof(bb_map_t));
	if (!map) {
		free(chunks);
		return bb_err_Alloc;
	}

	map->inited = bb_Inited_Magic;
	map->file = file;
	map->chunks = chunks;
	map->numchunks = numchunks;
	map->resources = nullptr;
	map->ressorted = nullptr;
	map->numresources = 0;
	map->releasenum = 0;
	map->zheader = nullptr;
	map->resolution = nullptr;
	map->palettechunk = -1;
	map->palette = nullptr;
	map->auxsound = nullptr;
	map->auxpict = nullptr;

	/* Now we do everything else involved in loading the Blorb file,
	    such as building resource lists. */

	err = bb_initialize_map(map);
	if (err) {
		bb_destroy_map(map);
		return err;
	}

	*newmap = map;
	return bb_err_None;
}

static bb_err_t bb_initialize_map(bb_map_t *map) {
	/* It is important that the map structure be kept valid during this
	    function. If this returns an error, bb_destroy_map() will be called. */

	int ix, jx;
	bb_result_t chunkres;
	bb_err_t err;
	uint32 *ptr;
	uint32 len;
	uint32 val;
	int numres;
	int gotindex = FALSE;

	for (ix = 0; ix < map->numchunks; ix++) {
		bb_chunkdesc_t *chu = &map->chunks[ix];

		switch (chu->type) {

		case bb_ID_RIdx:
			/* Resource index chunk: build the resource list and sort it. */

			if (gotindex)
				return bb_err_Format; /* duplicate index chunk */
			err = bb_load_chunk_by_number(map, bb_method_Memory,
			                              &chunkres, ix);
			if (err)
				return err;

			ptr = (uint32 *)chunkres.data.ptr;
			len = chunkres.length;
			val = ptr[0];
			numres = bb_native4(val);

			if (numres) {
				int ix2;
				bb_resdesc_t *resources;
				bb_resdesc_t **ressorted;

				if (len != (unsigned int) numres * 12 + 4)
					return bb_err_Format; /* bad length field */

				resources = (bb_resdesc_t *)malloc(numres * sizeof(bb_resdesc_t));
				ressorted = (bb_resdesc_t **)malloc(numres * sizeof(bb_resdesc_t *));
				if (!ressorted || !resources)
					return bb_err_Alloc;

				ix2 = 0;
				for (jx = 0; jx < numres; jx++) {
					bb_resdesc_t *res = &(resources[jx]);
					uint32 respos;

					val = ptr[1 + jx * 3];
					res->usage = bb_native4(val);
					val = ptr[2 + jx * 3];
					res->resnum = bb_native4(val);
					val = ptr[3 + jx * 3];
					respos = bb_native4(val);

					while (ix2 < map->numchunks && map->chunks[ix2].startpos < respos)
						ix2++;

					if (ix2 >= map->numchunks || map->chunks[ix2].startpos != respos)
						return bb_err_Format; /* start pos does not match a real chunk */

					res->chunknum = ix2;

					ressorted[jx] = res;
				}

				/* Sort a resource list (actually a list of pointers to structures
				    in map->resources.) This makes it easy to find resources by
				    usage and resource number. */
				qsort(ressorted, numres, sizeof(bb_resdesc_t *),
				      &sortsplot);

				map->numresources = numres;
				map->resources = resources;
				map->ressorted = ressorted;
			}

			bb_unload_chunk(map, ix);
			gotindex = TRUE;
			break;

		case bb_ID_RelN:
			/* Release number chunk: Get the release number. */

			err = bb_load_chunk_by_number(map, bb_method_Memory,
			                              &chunkres, ix);
			if (err)
				return err;

			if (chunkres.length != 2)
				return bb_err_Format;

			{
				uint16 v = *((uint16 *)chunkres.data.ptr);
				map->releasenum = bb_native2(v);
			}

			bb_unload_chunk(map, ix);
			break;

		case bb_ID_IFhd:
			/* Z-header chunk: Get the header info. */

			err = bb_load_chunk_by_number(map, bb_method_Memory,
			                              &chunkres, ix);
			if (err)
				return err;

			if (chunkres.length < 13)
				return bb_err_Format;

			{
				uint16 v;
				bb_zheader_t *head = (bb_zheader_t *)malloc(sizeof(bb_zheader_t));
				if (!head)
					return bb_err_Alloc;

				v = ((uint16 *)(chunkres.data.ptr))[0];
				head->releasenum = bb_native2(v);

				v = ((uint16 *)(chunkres.data.ptr))[4];
				head->checksum = bb_native2(v);

				for (jx = 0; jx < 6; jx++) {
					head->serialnum[jx] = ((char *)(chunkres.data.ptr))[2 + jx];
				}

				map->zheader = head;
			}

			bb_unload_chunk(map, ix);
			break;

		case bb_ID_Reso:
			/* Resolution chunk: Get the window size data, and resolution
			    ratios for images. */

			err = bb_load_chunk_by_number(map, bb_method_Memory,
			                              &chunkres, ix);
			if (err)
				return err;

			if (chunkres.length < 24)
				return bb_err_Format;

			ptr = (uint32 *)chunkres.data.ptr;
			len = chunkres.length;

			{
				bb_resolution_t *reso = (bb_resolution_t *)malloc(sizeof(bb_resolution_t));
				if (!reso)
					return bb_err_Alloc;

				reso->px = bb_native4(ptr[0]);
				reso->py = bb_native4(ptr[1]);
				reso->minx = bb_native4(ptr[2]);
				reso->miny = bb_native4(ptr[3]);
				reso->maxx = bb_native4(ptr[4]);
				reso->maxy = bb_native4(ptr[5]);

				map->resolution = reso;
			}

			ptr += 6;
			len -= 6 * 4;

			len = len / 28;

			if (len) {
				bb_aux_pict_t *aux = (bb_aux_pict_t *)malloc(len * sizeof(bb_aux_pict_t));

				for (jx = 0; (unsigned int) jx < len; jx++, ptr += 7) {
					bb_result_t res;

					err = bb_load_resource(map, bb_method_DontLoad, &res,
					                       bb_ID_Pict, bb_native4(ptr[0]));
					if (!err) {
						bb_chunkdesc_t *chu = &(map->chunks[res.chunknum]);
						if (chu->auxdatnum != -1)
							return bb_err_Format; /* two image entries for this resource */
						chu->auxdatnum = jx;
						aux[jx].ratnum = bb_native4(ptr[1]);
						aux[jx].ratden = bb_native4(ptr[2]);
						aux[jx].minnum = bb_native4(ptr[3]);
						aux[jx].minden = bb_native4(ptr[4]);
						aux[jx].maxnum = bb_native4(ptr[5]);
						aux[jx].maxden = bb_native4(ptr[6]);
					}
				}

				map->auxpict = aux;
			}

			bb_unload_chunk(map, ix);
			break;

		case bb_ID_Loop:
			/* Looping chunk: Get looping data for sounds. */

			err = bb_load_chunk_by_number(map, bb_method_Memory,
			                              &chunkres, ix);
			if (err)
				return err;

			ptr = (uint32 *)chunkres.data.ptr;
			len = chunkres.length;

			len = len / 8;

			if (len) {
				bb_aux_sound_t *aux = (bb_aux_sound_t *)malloc(len * sizeof(bb_aux_sound_t));

				for (jx = 0; (unsigned int) jx < len; jx++, ptr += 2) {
					bb_result_t res;

					err = bb_load_resource(map, bb_method_DontLoad, &res,
					                       bb_ID_Snd, bb_native4(ptr[0]));
					if (!err) {
						bb_chunkdesc_t *chu = &(map->chunks[res.chunknum]);
						if (chu->auxdatnum != -1)
							return bb_err_Format; /* two looping entries for this resource */
						chu->auxdatnum = jx;
						aux[jx].repeats = bb_native4(ptr[1]);
					}
				}

				map->auxsound = aux;
			}

			bb_unload_chunk(map, ix);
			break;

		case bb_ID_Plte:
			/* Palette chunk: Don't get the palette info now, since it may
			    be large and the interpreter may not care. But remember
			    the chunk number in case the interpreter asks later. */

			map->palettechunk = ix;
			break;
		}
	}

	return bb_err_None;
}

bb_err_t bb_destroy_map(bb_map_t *map) {
	int ix;

	if (!map || !map->chunks || map->inited != bb_Inited_Magic)
		return bb_err_NotAMap;

	for (ix = 0; ix < map->numchunks; ix++) {
		bb_chunkdesc_t *chu = &(map->chunks[ix]);
		if (chu->ptr) {
			free(chu->ptr);
			chu->ptr = nullptr;
		}
	}

	if (map->chunks) {
		free(map->chunks);
		map->chunks = nullptr;
	}

	map->numchunks = 0;

	if (map->resources) {
		free(map->resources);
		map->resources = nullptr;
	}

	if (map->ressorted) {
		free(map->ressorted);
		map->ressorted = nullptr;
	}

	map->numresources = 0;

	if (map->zheader) {
		free(map->zheader);
		map->zheader = nullptr;
	}

	if (map->resolution) {
		free(map->resolution);
		map->resolution = nullptr;
	}

	if (map->palette) {
		if (!map->palette->isdirect && map->palette->data.table.colors) {
			free(map->palette->data.table.colors);
			map->palette->data.table.colors = nullptr;
		}
		free(map->palette);
		map->palette = nullptr;
	}

	if (map->auxsound) {
		free(map->auxsound);
		map->auxsound = nullptr;
	}

	if (map->auxpict) {
		free(map->auxpict);
		map->auxpict = nullptr;
	}

	map->file = nullptr;
	map->inited = 0;

	free(map);

	return bb_err_None;
}

/* Turn a four-byte constant into a string. This returns a static buffer,
    so if you call it twice, the old value gets overwritten. */
char *bb_id_to_string(uint32 id) {
	static char buf[5];
	buf[0] = (id >> 24) & 0xff;
	buf[1] = (id >> 16) & 0xff;
	buf[2] = (id >> 8) & 0xff;
	buf[3] = (id) & 0xff;
	buf[4] = '\0';
	return buf;
}

/* Turn an error code into a string describing the error. */
const char *bb_err_to_string(bb_err_t err) {
	switch (err) {
	case bb_err_None:
		return "ok";
	case bb_err_CompileTime:
		return "library compiled wrong";
	case bb_err_Alloc:
		return "cannot allocate memory";
	case bb_err_Read:
		return "cannot read from file";
	case bb_err_NotAMap:
		return "map structure is bad";
	case bb_err_Format:
		return "bad format in Blorb file";
	case bb_err_NotFound:
		return "data not found";
	default:
		return "unknown error";
	}
}

/* This is used for binary searching and quicksorting the resource pointer list. */
static int sortsplot(const void *p1, const void *p2) {
	bb_resdesc_t *v1 = *(bb_resdesc_t **)p1;
	bb_resdesc_t *v2 = *(bb_resdesc_t **)p2;
	if (v1->usage < v2->usage)
		return -1;
	if (v1->usage > v2->usage)
		return 1;
	return v1->resnum - v2->resnum;
}

bb_err_t bb_load_chunk_by_type(bb_map_t *map, int method, bb_result_t *res,
                               uint32 type, int count) {
	int ix;

	for (ix = 0; ix < map->numchunks; ix++) {
		if (map->chunks[ix].type == type) {
			if (count == 0)
				break;
			count--;
		}
	}

	if (ix >= map->numchunks) {
		return bb_err_NotFound;
	}

	return bb_load_chunk_by_number(map, method, res, ix);
}

bb_err_t bb_load_chunk_by_number(bb_map_t *map, int method, bb_result_t *res,
                                 int chunknum) {
	bb_chunkdesc_t *chu;

	if (chunknum < 0 || chunknum >= map->numchunks)
		return bb_err_NotFound;

	chu = &(map->chunks[chunknum]);

	switch (method) {

	case bb_method_DontLoad:
		/* do nothing */
		break;

	case bb_method_FilePos:
		res->data.startpos = chu->datpos;
		break;

	case bb_method_Memory:
		if (!chu->ptr) {
			bb_err_t err;
			size_t readlen;
			void *dat = malloc(chu->len);

			if (!dat)
				return bb_err_Alloc;

			err = fseek(map->file, chu->datpos, 0);
			if (err)
				return bb_err_Read;

			readlen = fread(dat, 1, chu->len, map->file);
			if (readlen != chu->len)
				return bb_err_Read;

			chu->ptr = dat;
		}
		res->data.ptr = chu->ptr;
		break;
	}

	res->chunknum = chunknum;
	res->length = chu->len;

	return bb_err_None;
}

bb_err_t bb_load_resource(bb_map_t *map, int method, bb_result_t *res,
                          uint32 usage, int resnum) {
	bb_resdesc_t sample;
	bb_resdesc_t *ptr;
	bb_resdesc_t **found;

	sample.usage = usage;
	sample.resnum = resnum;
	ptr = &sample;

	found = (bb_resdesc_t **)bsearch(&ptr, map->ressorted, map->numresources, sizeof(bb_resdesc_t *),
	                                 &sortsplot);

	if (!found)
		return bb_err_NotFound;

	return bb_load_chunk_by_number(map, method, res, (*found)->chunknum);
}

bb_err_t bb_unload_chunk(bb_map_t *map, int chunknum) {
	bb_chunkdesc_t *chu;

	if (chunknum < 0 || chunknum >= map->numchunks)
		return bb_err_NotFound;

	chu = &(map->chunks[chunknum]);

	if (chu->ptr) {
		free(chu->ptr);
		chu->ptr = nullptr;
	}

	return bb_err_None;
}

bb_err_t bb_count_resources(bb_map_t *map, uint32 usage,
                            int *num, int *min, int *max) {
	int ix;
	int count, minval, maxval, val;

	count = 0;
	minval = 0;
	maxval = 0;

	for (ix = 0; ix < map->numresources; ix++) {
		if (map->resources[ix].usage == usage) {
			val = map->resources[ix].resnum;
			if (count == 0) {
				count++;
				minval = val;
				maxval = val;
			} else {
				count++;
				if (val < minval)
					minval = val;
				if (val > maxval)
					maxval = val;
			}
		}
	}

	if (num)
		*num = count;
	if (min)
		*min = minval;
	if (max)
		*max = maxval;

	return bb_err_None;
}

uint16 bb_get_release_num(bb_map_t *map) {
	return map->releasenum;
}

bb_zheader_t *bb_get_zheader(bb_map_t *map) {
	return map->zheader;
}

bb_resolution_t *bb_get_resolution(bb_map_t *map) {
	return map->resolution;
}

bb_err_t bb_get_palette(bb_map_t *map, bb_palette_t **res) {
	int ix;
	bb_err_t err;

	if (res)
		*res = nullptr;

	if (map->palettechunk < 0) {
		return bb_err_None;
	}

	if (!map->palette) {
		bb_result_t chunkres;
		bb_palette_t *pal;
		unsigned char *ptr;

		pal = (bb_palette_t *)malloc(sizeof(bb_palette_t));
		if (!pal)
			return bb_err_Alloc;

		err = bb_load_chunk_by_number(map, bb_method_Memory, &chunkres,
		                              map->palettechunk);
		if (err)
			return err;

		ptr = (unsigned char *)chunkres.data.ptr;

		if (chunkres.length == 1) {
			int val = ptr[0];
			if (val != 16 && val != 32)
				return bb_err_Format;
			pal->isdirect = TRUE;
			pal->data.depth = val;
		} else {
			int size = chunkres.length / 3;
			bb_color_t *colors = (bb_color_t *)malloc(size * sizeof(bb_color_t));
			if (!colors)
				return bb_err_Alloc;
			if (size < 1 || size > 256)
				return bb_err_Format;
			for (ix = 0; ix < size; ix++) {
				colors[ix].red   = ptr[ix * 3 + 0];
				colors[ix].green = ptr[ix * 3 + 1];
				colors[ix].blue  = ptr[ix * 3 + 2];
			}
			pal->isdirect = FALSE;
			pal->data.table.numcolors = size;
			pal->data.table.colors = colors;
		}

		bb_unload_chunk(map, map->palettechunk);
		map->palette = pal;
	}

	if (res)
		*res = map->palette;
	return bb_err_None;
}

bb_err_t bb_load_resource_pict(bb_map_t *map, int method, bb_result_t *res,
                               int resnum, bb_aux_pict_t **auxdata) {
	bb_err_t err;

	if (auxdata)
		*auxdata = nullptr;

	err = bb_load_resource(map, method, res, bb_ID_Pict, resnum);
	if (err)
		return err;

	if (auxdata) {
		bb_chunkdesc_t *chu = &(map->chunks[res->chunknum]);
		if (chu->auxdatnum >= 0 && map->auxpict) {
			*auxdata = &(map->auxpict[chu->auxdatnum]);
		}
	}

	return bb_err_None;
}

bb_err_t bb_load_resource_snd(bb_map_t *map, int method, bb_result_t *res,
                              int resnum, bb_aux_sound_t **auxdata) {
	bb_err_t err;

	if (auxdata)
		*auxdata = nullptr;

	/*    err = bb_load_resource(map, method, res, bb_ID_Pict, resnum); */
	err = bb_load_resource(map, method, res, bb_ID_Snd, resnum);
	if (err)
		return err;

	if (auxdata) {
		bb_chunkdesc_t *chu = &(map->chunks[res->chunknum]);
		if (chu->auxdatnum >= 0 && map->auxsound) {
			*auxdata = &(map->auxsound[chu->auxdatnum]);
		}
	}

	return bb_err_None;
}

} // End of namespace ZCode
} // End of namespace Glk
