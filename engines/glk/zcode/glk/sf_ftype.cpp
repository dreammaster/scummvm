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
#include "common/textconsole.h"
#include "graphics/surface.h"

namespace Glk {
namespace ZCode {

// Dummy stubs
#define FT_RENDER_MODE_MONO 1
#define FT_RENDER_MODE_NORMAL 2
#define FT_GLYPH_FORMAT_BITMAP 3

struct Metrics {
	int ascender;
	int descender;
	int height;
	Metrics() : ascender(0), descender(0), height(0) {}
};
struct FaceSize {
	Metrics metrics;
};
typedef void *FT_Library;
typedef Graphics::Surface FT_Bitmap;

struct GlyphSlot {
	int format;
	FT_Bitmap bitmap;
};
typedef GlyphSlot *FT_GlyphSlot;

struct Face {
	FT_GlyphSlot glyph;
	FaceSize *size;
	Face() : glyph(nullptr), size(nullptr) {
	}
};
typedef Face *FT_Face;

void FT_Done_Face(FT_Face face) {}
void FT_Done_FreeType(FT_Library lib) {}
int FT_Init_FreeType(FT_Library *lib) {
	return 0;
}
int FT_Get_Char_Index(FT_Face face, int ch) {
	return 0;
}
int FT_Load_Glyph(FT_Face face, int gid, int v3) {
	return 0;
}
int FT_Render_Glyph(FT_GlyphSlot slot, int v2) {
	return 0;
}
int FT_New_Face(FT_Library library, const char *fname, int v3, FT_Face *v4) {
	return 0;
}
int FT_Set_Pixel_Sizes(FT_Face face, int v2, int v3) {
	return 0;
}

typedef struct {
	SFONT sfont;
	int ascent, descent, height;
	int minchar, maxchar, totglyphs;
	FT_Face face;
	char *isset;
	SF_glyph *glyphs[1];
} MYFONT;


/* destructor */
static void bdestroy(SFONT *s) {
	if (s) {
		int i;
		MYFONT *f = (MYFONT *) s;
		for (i = 0; i < f->totglyphs; i++)
			if (f->glyphs[i])
				free(f->glyphs[i]);
		free(f->isset);
		if (f->face)
			FT_Done_Face(f->face);
		free(s);
	}
}


static int bheight(SFONT *s) {
	if (s)
		return ((MYFONT *) s)->height;
	return 0;
}


static int bascent(SFONT *s) {
	if (s)
		return ((MYFONT *) s)->ascent;
	return 0;
}


static int bdescent(SFONT *s) {
	if (s)
		return ((MYFONT *) s)->descent;
	return 0;
}


static int bminchar(SFONT *s) {
	if (s)
		return ((MYFONT *) s)->minchar;
	return 0;
}


static int bmaxchar(SFONT *s) {
	if (s)
		return ((MYFONT *) s)->maxchar;
	return 0;
}


static void setglyph(MYFONT *f, FT_Face face, int ch);

static SF_glyph *getglyph(SFONT *s, word c, int allowdef) {
	if (s) {
		MYFONT *f = (MYFONT *) s;
		if (c < f->minchar || c > f->maxchar) {
			if (allowdef)
				c = 0;
			else
				return nullptr;
		}
		if (!f->isset[c])
			setglyph(f, f->face, c);
		return f->glyphs[c];
	}
	return nullptr;
}


static int hasglyph(SFONT *fo, word c, int allowdef) {
	return (getglyph(fo, c, allowdef) != nullptr);
}


static int inited = 0, initerr = 0;
static FT_Library library;

static void libfinish() {
	if (!inited)
		return;
	FT_Done_FreeType(library);
	inited = 0;
}


static void libinit() {
	if (initerr)
		return;
	if (inited)
		return;
	initerr = FT_Init_FreeType(&library);   /* initialize library */
	/* error handling omitted */
	if (initerr)
		error("FT_Init_FreeType: error %d\n", initerr);
	else {
		inited = 1;
		atexit(libfinish);
	}
}


static MYFONT *makefont(int totglyphs) {
	MYFONT *res;
	res = (MYFONT *)calloc(1, sizeof(MYFONT) + totglyphs * sizeof(SF_glyph *));
	if (!res)
		return nullptr;
	res->isset = (char *)calloc(1, totglyphs);
	if (!res->isset) {
		free(res);
		return nullptr;
	}
	res->sfont.destroy = bdestroy;
	res->sfont.height = bheight;
	res->sfont.ascent = bascent;
	res->sfont.descent = bdescent;
	res->sfont.minchar = bminchar;
	res->sfont.maxchar = bmaxchar;
	res->sfont.hasglyph = hasglyph;
	res->sfont.getglyph = getglyph;
	res->totglyphs = totglyphs;
	res->maxchar = totglyphs - 1;
	return res;
}


#define MAXUNI 0xffff

static void setglyph(MYFONT *f, FT_Face face, int ch) {
#ifdef TODO
	int err, gid = FT_Get_Char_Index(face, ch);
	int mode = FT_RENDER_MODE_MONO;
	SF_glyph *res;
	FT_GlyphSlot slot = face->glyph;
	int i, j, nbypr;
	FT_Bitmap *bitmap;

	f->isset[ch] = 1;
	if (!gid)
		return;

	if (m_aafonts)
		mode = FT_RENDER_MODE_NORMAL;

	err = FT_Load_Glyph(face, gid, 0);
	if (err)
		return;
	if (slot->format != FT_GLYPH_FORMAT_BITMAP) {
		err = FT_Render_Glyph(slot, mode);
		if (err)
			return;
	}
	bitmap = &slot->bitmap;
	nbypr = m_aafonts ? bitmap->w : (bitmap->w + 7) / 8;
	res = (SF_glyph *)calloc(1, sizeof(SF_glyph) + nbypr * bitmap->h);
	if (!res)
		return;
	for (i = 0; i < bitmap->h; i++) {
		for (j = 0; j < nbypr; j++) {
			res->bitmap[i * nbypr + j] =
			    bitmap->buffer[i * bitmap->pitch + j];
		}
	}

	res->w = bitmap->w;
	res->h = bitmap->h;
	res->dx = slot->advance.x / 64;
	res->xof = slot->bitmap_left;
	res->yof = slot->bitmap_top - bitmap->h + 1;

	f->glyphs[ch] = res;
#else
	error("TODO: setglyph");
#endif
}


static SFONT *loadftype(char *fname, int size, SFONT *like, int *err) {
	MYFONT *res;
	FT_Face face;
	int i;

	*err = 0;
	if (!fname) {
		*err = -8;
		return nullptr;
	}
	libinit();
	if (initerr) {
		*err = -99;
		return nullptr;
	}

	res = makefont(MAXUNI + 1);
	if (!res) {
		*err = -3;
		return nullptr;
	}

	*err = FT_New_Face(library, fname, 0, &face);   /* create face object */
	if (*err) {
		res->sfont.destroy(&res->sfont);
		return nullptr;
	}
	res->face = face;

	if (like) {
		SF_glyph *zero = like->getglyph(like, '0', TRUE);
		*err = FT_Set_Pixel_Sizes(face, zero->dx, like->height(like));
	} else
		*err = FT_Set_Pixel_Sizes(face, size, size);
	if (*err) {
		res->sfont.destroy(&res->sfont);
		return nullptr;
	}

	res->ascent = face->size->metrics.ascender / 64;
	res->descent = -face->size->metrics.descender / 64;
	res->height = face->size->metrics.height / 64;

	res->sfont.antialiased = m_aafonts;
	res->minchar = 32;
	setglyph(res, face, 0);
	for (i = 32; i < 127; i++)
		setglyph(res, face, i);
	for (i = 0xa0; i < 256; i++)
		setglyph(res, face, i);
	setglyph(res, face, 0x152);
	setglyph(res, face, 0x153);

	return (SFONT *) res;
}


#ifdef WIN32
#define SYSFONTS "c:/windows/fonts"
#else
#define SYSFONTS "/usr/share/fonts/freetype"
#endif

SFONT *sf_loadftype(char *fspec, SFONT *like, int *err) {
	char buf[FILENAME_MAX], *fn, *at;// , *fenv;
	int size = DEFSIZE, fnlen = -1;

	at = strchr(fspec, '@');
	if (at) {
		fnlen = at - fspec;
		size = atoi(at + 1);
	}

	fn = sf_searchfile(fspec, fnlen, buf, "");
	if (!fn)
		fn = sf_searchfile(fspec, fnlen, buf, "./");
	if (!fn)
		if (m_fontdir)
			fn = sf_searchfile(fspec, fnlen, buf, m_fontdir);
	if (!fn)
		fn = sf_searchfile(fspec, fnlen, buf, SYSFONTS);
	if (!fn) {
#ifdef TODO
		fenv = getenv("FONTS");
		if (fenv)
			fn = sf_searchfile(fspec, fnlen, buf, fenv);
#endif
	}

	if (!fn)
		return nullptr;

	return loadftype(fn, size, like, err);
}

/****************************************/

void sf_initloader() {
	ttfontloader = sf_loadftype;
}

} // End of namespace ZCode
} // End of namespace Glk
