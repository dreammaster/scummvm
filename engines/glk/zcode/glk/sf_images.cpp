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

#include "glk/zcode/glk/sf_frotz.h"
#include "glk/zcode/blorb/blorblow.h"

namespace Glk {
namespace ZCode {

static byte toLinear[256];
static byte fromLinear[256];
extern bool m_adaptiveMode;

ulong sf_blend(int a, ulong s, ulong d)
{
	ulong r;
	r = fromLinear[(toLinear[s & 0xff] * a +
			toLinear[d & 0xff] * (256 - a)) >> 8];
	s >>= 8;
	d >>= 8;
	r |= (fromLinear
	      [(toLinear[s & 0xff] * a +
		toLinear[d & 0xff] * (256 - a)) >> 8]) << 8;
	s >>= 8;
	d >>= 8;
	r |= (fromLinear
	      [(toLinear[s & 0xff] * a +
		toLinear[d & 0xff] * (256 - a)) >> 8]) << 16;
	return r;
}


/* Set the screen gamma and build gamma correction tables */
void sf_setgamma(double gamma)
{
	int i;

	m_gamma = gamma;
	for (i = 0; i < 256; i++)
		toLinear[i] = (int)((pow(i / 255.0, gamma) * 255.0) + 0.5);
	gamma = 1.0 / gamma;
	for (i = 0; i < 256; i++)
		fromLinear[i] = (int)((pow(i / 255.0, gamma) * 255.0) + 0.5);
}

/****************************************************************************
 * Loader for PNG images
 ****************************************************************************
 */

typedef struct {
	byte *gfxData;
	unsigned long offset;
} PNGData;

typedef void *png_structp;
typedef byte *png_bytep;
typedef size_t png_size_t;

#ifdef TODO
static void readPNGData(png_structp png_ptr, png_bytep data, png_size_t length) {
	PNGData *pngData = (PNGData *) png_get_io_ptr(png_ptr);
	memmove(data, pngData->gfxData + pngData->offset, length);
	pngData->offset += length;
}
#endif


static int loadpng(byte * data, int length, sf_picture * graphic)
{
#ifdef TODO
	png_bytep *rowPointers = nullptr;
	png_structp png_ptr = nullptr;
	png_infop info_ptr = nullptr;
	png_infop end_info = nullptr;
	PNGData pngData;
	png_uint_32 width, height;
	int color_type, size, bit_depth;
	double gamma;

	graphic->pixels = nullptr;
	graphic->width = graphic->height = 0;

	if (!png_check_sig(data, 8))
		return 0;

	png_ptr = png_create_read_struct
	    (PNG_LIBPNG_VER_STRING, (png_voidp) nullptr, nullptr, nullptr);
	if (!png_ptr)
		return 0;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr,
					(png_infopp) nullptr, (png_infopp) nullptr);
		return 0;
	}

	end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) nullptr);
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		if (rowPointers)
			free(rowPointers);
		if (graphic->pixels) {
			free(graphic->pixels);
			graphic->pixels = nullptr;
		}
		return 0;
	}

	pngData.gfxData = data;
	pngData.offset = 8;
	png_set_read_fn(png_ptr, &pngData, readPNGData);

	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	graphic->width = width;
	graphic->height = height;
	graphic->usespalette = FALSE;

	if (png_get_gAMA(png_ptr, info_ptr, &gamma))
		png_set_gamma(png_ptr, m_gamma, gamma);

	if (m_adaptiveMode && (color_type == PNG_COLOR_TYPE_PALETTE) && bit_depth <= 4) {
		graphic->usespalette = TRUE;
		png_set_packing(png_ptr);

		/* Check for transparency.  In practice, the transparent
		 * color will always be color 0.
		 */
		png_bytep trans;
		int num_trans;
		png_color_16p trans_values;

		if (png_get_tRNS
		    (png_ptr, info_ptr, &trans, &num_trans, &trans_values)
		    && num_trans >= 1)
			graphic->transparentcolor = trans[0];

		size = width * height;
		graphic->pixels = (byte *) malloc(size);

		rowPointers = malloc(sizeof(png_bytep) * height);
		for (int i = 0; i < (int)height; i++)
			rowPointers[i] = graphic->pixels + (width * i);
		png_read_image(png_ptr, rowPointers);

		/* Get the palette after reading the image, so that the gamma
		 * correction is applied.
		 */
		png_colorp palette;
		int num_palette;
		if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
			graphic->palette_entries = num_palette;
			for (int i = 0; i < num_palette; i++) {
				ulong color =
				    palette[i].red | (palette[i].
						      green << 8) | (palette[i].
								     blue <<
								     16);
				graphic->palette[i] = color;
			}
		}
	} else {
		if (graphic->adaptive)
			os_fatal("Non-paletted graphics cannot be adaptive");
		if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8)
			png_set_palette_to_rgb(png_ptr);
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		if (png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png_ptr);

		if (bit_depth == 16)
			png_set_strip_16(png_ptr);
		if (bit_depth < 8)
			png_set_packing(png_ptr);
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png_ptr);

		png_set_filler(png_ptr,0xff,PNG_FILLER_AFTER);
		size = width*height*4;
		graphic->pixels = (byte *) malloc(size);
		rowPointers = malloc(sizeof(png_bytep) * height);
		for (int i = 0; i < (int)height; i++)
			rowPointers[i] = graphic->pixels + (width * i * 4);
		png_read_image(png_ptr, rowPointers);
	}

	/* Reading done. */
	png_read_end(png_ptr, end_info);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	if (rowPointers)
		free(rowPointers);

	return 1;
#else
	return 0;
#endif
}

/****************************************************************************
 * Loader for JPEG images
 ****************************************************************************
 */
#ifdef TODO
/* Error Handling */
struct JPEGErrorInfo {
	struct jpeg_error_mgr base;
	jmp_buf errorJump;
};


static void errorJPEGExit(j_common_ptr cinfo)
{
	struct JPEGErrorInfo *error = (struct JPEGErrorInfo *)cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(error->errorJump, 1);
}


static void outputJPEGMessage(j_common_ptr cinfo)
{
	char buffer[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message) (cinfo, buffer);
}


/* Memory Data Source */
static void memJPEGInit(j_decompress_ptr unused)
{
	/* Nothing here */
}


static int memJPEGFillInput(j_decompress_ptr unused)
{
	return 0;
}


static void memJPEGSkipInput(j_decompress_ptr cinfo, long num_bytes)
{
	if (num_bytes > 0) {
		if (num_bytes > (long)cinfo->src->bytes_in_buffer)
			num_bytes = (long)cinfo->src->bytes_in_buffer;

		cinfo->src->next_input_byte += num_bytes;
		cinfo->src->bytes_in_buffer -= num_bytes;
	}
}


static void memJPEGTerm(j_decompress_ptr unused)
{
	/* Nothing here */
}

#endif

static int loadjpeg(byte * data, int length, sf_picture * graphic) {
#ifdef TODO
	struct jpeg_decompress_struct info;
	struct JPEGErrorInfo error;
	int width, height, size;
	JSAMPARRAY buffer;

	graphic->pixels = nullptr;
	graphic->width = graphic->height = 0;

	info.err = jpeg_std_error(&(error.base));
	error.base.error_exit = errorJPEGExit;
	error.base.output_message = outputJPEGMessage;
	if (setjmp(error.errorJump)) {
		jpeg_destroy_decompress(&info);
		if (graphic->pixels) {
			free(graphic->pixels);
			graphic->pixels = nullptr;
		}
		return 0;
	}

	jpeg_create_decompress(&info);

	info.src = (struct jpeg_source_mgr *)(info.mem->alloc_small)
	    ((j_common_ptr) (&info), JPOOL_PERMANENT,
	     sizeof(struct jpeg_source_mgr));
	info.src->init_source = memJPEGInit;
	info.src->fill_input_buffer = memJPEGFillInput;
	info.src->skip_input_data = memJPEGSkipInput;
	info.src->resync_to_restart = jpeg_resync_to_restart;
	info.src->term_source = memJPEGTerm;
	info.src->bytes_in_buffer = length;
	info.src->next_input_byte = data;

	jpeg_read_header(&info, TRUE);
	jpeg_calc_output_dimensions(&info);
	width = info.output_width;
	height = info.output_height;

	graphic->width = width;
	graphic->height = height;
	size = width * height * 4;
	graphic->pixels = (byte *) malloc(size);

	/* Force RGB output */
	info.out_color_space = JCS_RGB;

	/* Get an output buffer */
	buffer = (*info.mem->alloc_sarray)
	    ((j_common_ptr) & info, JPOOL_IMAGE, width * 3, 1);

	jpeg_start_decompress(&info);
	while ((int)info.output_scanline < height) {
		byte *pixelRow;
		int i;
		jpeg_read_scanlines(&info, buffer, 1);

		pixelRow = graphic->pixels +
		    (width * (info.output_scanline - 1) * 4);
		for (i = 0; i < width; i++) {
			pixelRow[(i * 4) + 0] = (*buffer)[(i * 3) + 0];
			pixelRow[(i * 4) + 1] = (*buffer)[(i * 3) + 1];
			pixelRow[(i * 4) + 2] = (*buffer)[(i * 3) + 2];
			pixelRow[(i * 4) + 3] = 0xFF;
		}
	}
	jpeg_finish_decompress(&info);
	jpeg_destroy_decompress(&info);

	return 1;
#else
	return 0;
#endif
}

/****************************************************************************
 * Loader for simple rectangles
 ****************************************************************************
 */

static int loadrect(byte * data, int length, sf_picture * graphic)
{
	graphic->width =
	    (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
	graphic->height =
	    (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
	graphic->pixels = nullptr;
	return 1;
}

/*****************************/

/* Get a picture from the Blorb resource map */
static int sf_loadpic(int picture, sf_picture * graphic)
{
	myresource res;
	int st = 0;

	/* Set whether graphic has an adaptive palette */
	graphic->adaptive = sf_IsAdaptive(picture) ? TRUE : FALSE;

	if (sf_getresource(picture, 1, bb_method_Memory, &res) == bb_err_None) {
		byte *data = (byte *) res.bbres.data.ptr;
		int length = res.bbres.length;
		unsigned int id = res.type;

		/* Look for a recognized format */
		if (id == bb_ID_PNG) {
			st = loadpng(data, length, graphic);
			if (!st)
				st = loadjpeg(data, length, graphic);
		} else if (id == bb_ID_JPEG) {
			st = loadjpeg(data, length, graphic);
			if (!st)
				st = loadpng(data, length, graphic);
		} else if (id == bb_ID_Rect)
			st = loadrect(data, length, graphic);
		sf_freeresource(&res);
	}

	if (st)
		graphic->number = picture;
	return st;
}

/*******************
 * CACHE
 */

#define MAXCACHE 1

static sf_picture cached[MAXCACHE];
static int cacheinited = 0;

static void cacheflush()
{
	int i;
	if (!cacheinited)
		return;
	for (i = 0; i < MAXCACHE; i++) {
		cached[i].number = -1;
		if (cached[i].pixels)
			free(cached[i].pixels);
		cached[i].pixels = nullptr;
	}
	cacheinited = 0;
}


static void cacheinit()
{
	int i;
	if (cacheinited)
		return;
	CLEANREG(cacheflush);
	for (i = 0; i < MAXCACHE; i++) {
		cached[i].number = -1;
		cached[i].pixels = nullptr;
	}
	cacheinited = 1;
}


static sf_picture *cachefind(int n)
{
	int i;
	for (i = 0; i < MAXCACHE; i++)
		if (cached[i].number == n)
			return (cached + i);
	if (n < 0) {
		cached[0].number = -1;
		if (cached[0].pixels)
			free(cached[0].pixels);
		cached[0].pixels = nullptr;
		return (cached + 0);
	}
	return nullptr;
}


sf_picture *sf_getpic(int num)
{
	sf_picture *res;
	cacheinit();
	res = cachefind(num);
	if (res)
		return res;
	/* not found, peek a slot */
	res = cachefind(-1);
	if (sf_loadpic(num, res))
		return res;
	return nullptr;
}

} // End of namespace ZCode
} // End of namespace Glk
