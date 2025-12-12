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

#ifndef AESOP_LIB_VFX_H
#define AESOP_LIB_VFX_H

namespace Aesop {

#ifndef TRUE
#define TRUE -1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define SHAPE_FILE_VERSION '01.1' // 1.10 backwards for big-endian compare


//
// MetaWare support
//

#ifdef __HIGHC__
#define _CC(_REVERSE_PARMS | _NEAR_CALL)
#pragma Global_aliasing_convention("_%r");
#pragma Align_members(1)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define GIF_SCRATCH_SIZE 20526L   // Temp memory req'd for GIF decompression

	//
	// VFX_map_polygon() flags
	//

#define MP_XLAT      0x0001       // Use lookaside table (speed loss = ~9%)
#define MP_XP        0x0002       // Enable transparency (speed loss = ~6%)

//
// VFX_shape_transform() flags
//

#define ST_XLAT      0x0001       // Use shape_lookaside() table
#define ST_REUSE     0x0002       // Use buffer contents from prior call

//
// VFX_line_draw() modes
//  

#define LD_DRAW      0
#define LD_TRANSLATE 1
#define LD_EXECUTE   2

//
// VFX_pane_scroll() modes
//

#define PS_NOWRAP    0
#define PS_WRAP      1

#define NO_COLOR -1

//
// VFX_shape_visible_rectangle() mirror values
//

#define VR_NO_MIRROR 0
#define VR_X_MIRROR  1
#define VR_Y_MIRROR  2
#define VR_XY_MIRROR 3

//
// PANE_LIST.flags values
//

#define PL_FREE      0            // Free and available for assignment
#define PL_VALID     1            // Assigned; to be refreshed
#define PL_CONTAINED 2            // Contained within another pane; don't refresh

//
// VFX data structures
//

	typedef unsigned char STENCIL;

	typedef struct _window
	{
		UBYTE *buffer;
		int32     x_max;
		int32     y_max;

		STENCIL *stencil;
		UBYTE *shadow;
	}
	WINDOW;

	typedef struct _pane
	{
		WINDOW *window;
		int32 x0;
		int32 y0;
		int32 x1;
		int32 y1;
	}
	PANE;

	typedef struct _pane_list
	{
		PANE *array;
		uint32 *flags;
		int32   size;
	}
	PANE_LIST;

	typedef union
	{
		void *v;
		UBYTE *b;
		UWORD *w;
		uint32 *d;
	}
	FLEX_PTR;

	typedef struct
	{
		int32 scrn_width;
		int32 scrn_height;
		int32 bytes_per_pixel;
		int32 ncolors;
		int32 npages;
		uint32 flags;
	}
	VFX_DESC;

	typedef struct
	{
		UBYTE r;
		UBYTE g;
		UBYTE b;
	}
	RGB;

	typedef struct
	{
		UBYTE color;
		RGB   rgb;
	}
	CRGB;

	typedef struct
	{
		int32 x;
		int32 y;
	}
	POINT;

	typedef struct
	{
		int32 version;
		int32 char_count;
		int32 char_height;
		int32 font_background;
	}
	FONT;

	typedef struct            // Vertex structure used by polygon primitives
	{
		int32 x;                // Screen X
		int32 y;                // Screen Y

		FIXED16 c;             // Color/addition value used by some primitives

		FIXED16 u;             // Texture source X
		FIXED16 v;             // Texture source Y
		FIXED30 w;             // Homogeneous perspective divisor (unused by VFX3D)
	}
	SCRNVERTEX;

	typedef struct
	{
		int32 x0;
		int32 y0;
		int32 x1;
		int32 y1;
	}
	RECT;

#define INT_TO_FIXED16(x)       (((long)(int)(x)) << 16)
#define DOUBLE_TO_FIXED16(x)    ((long) ((x) * 65536.0 + 0.5))
#define FIXED16_TO_DOUBLE(x)    (((double)(x)) / 65536.0)
#define FIXED16_TO_INT(x)       ((int) ((x)<0 ? -(-(x) >> 16) : (x) >> 16))
#define ROUND_FIXED16_TO_INT(x) ((int) ((x)<0 ? -((32768-(x)) >> 16) : ((x)+32768) >> 16))

#define FIXED16_TO_FIXED30(x)   ((x) << 14)
#define FIXED30_TO_FIXED16(x)   ((x) >> 14)
#define FIXED30_TO_DOUBLE(x)    (((double)x) / 1073741824.0)
#define DOUBLE_TO_FIXED30(x)    ((long) (x * 1073741824.0 + 0.5))

#define PIXELS_IN_PANE(pane)    (((pane).x1-(pane).x0+1)*((pane).y1-(pane).y0+1))
#define PIXELS_IN_PANEP(pane)   (((pane)->x1-(pane)->x0+1)*((pane)->y1-(pane)0>y0+1))

//
// Hardware-specific VFX DLL functions
//

extern VFX_DESC *(*VFX_describe_driver)(void);
extern void(*VFX_init_driver)(void);
extern void(*VFX_shutdown_driver)(void);
extern void(*VFX_wait_vblank)(void);
extern void(*VFX_wait_vblank_leading)(void);
extern void(*VFX_area_wipe)(int32 x0, int32 y0, int32 x1, int32 y1,
	int32 color);
extern void(*VFX_window_refresh)(WINDOW *target, int32 x0, int32 y0,
	int32 x1, int32 y1);
extern void(*VFX_window_read)(WINDOW *destination, int32 x0, int32 y0,
	int32 x1, int32 y1);
extern void(*VFX_pane_refresh)(PANE *target, int32 x0, int32 y0,
	int32 x1, int32 y1);
extern void(*VFX_DAC_read)(int32 color_number, RGB *triplet);
extern void(*VFX_DAC_write)(int32 color_number, RGB *triplet);
extern void(*VFX_bank_reset)(void);
extern void(*VFX_line_address)(int32 x, int32 y, UBYTE **addr, uint32 *nbytes);

//
// Device-independent VFX API functions (VFXC.C)
//

extern uint32 VFX_stencil_size(WINDOW *source, uint32 transparent_color);

extern STENCIL *VFX_stencil_construct(WINDOW *source, void *dest, uint32 transparent_color);
extern void VFX_stencil_destroy(STENCIL *stencil);

extern WINDOW *VFX_window_construct(int32 width, int32 height);
extern void VFX_window_destroy(WINDOW *window);

extern PANE *VFX_pane_construct(WINDOW *window, int32 x0, int32 y0, int32 x1, int32 y1);
extern void VFX_pane_destroy(PANE *pane);

extern PANE_LIST *VFX_pane_list_construct(int32 n_entries);
extern void VFX_pane_list_destroy(PANE_LIST *list);

extern void VFX_pane_list_clear(PANE_LIST *list);

extern int32 VFX_pane_list_add(PANE_LIST *list, PANE *target);
extern int32 VFX_pane_list_add_area(PANE_LIST *list, WINDOW *window, int32 x0, int32 y0,
	int32 x1, int32 y1);

extern void VFX_pane_list_delete_entry(PANE_LIST *list, int32 entry_num);

extern void VFX_pane_list_refresh(PANE_LIST *list);

//
// Device-independent VFX API functions (VFXA.ASM)
//

extern BYTE *VFX_driver_name(void *VFXScanDLL);

extern int32 VFX_register_driver(void *DLLbase);

extern int32 VFX_line_draw(PANE *pane, int32 x0, int32 y0,
	int32 x1, int32 y1, int32 mode, int32 parm);

extern void VFX_shape_draw(PANE *pane, void *shape_table,
	int32 shape_number, int32 hotX, int32 hotY);

extern void VFX_shape_lookaside(UBYTE *table);
extern void VFX_shape_translate_draw(PANE *pane, void *shape_table,
	int32 shape_number,
	int32 hotX, int32 hotY);

extern void VFX_shape_remap_colors(void *shape_table,
	uint32 shape_number);

void VFX_shape_visible_rectangle(void *shape_table,
	int32 shape_number, int32 hotX, int32 hotY,
	int32 mirror, int32 *rectangle);

extern int32 VFX_shape_scan(PANE *pane, uint32 transparentColor,
	int32 hotX, int32 hotY, void *buffer);
extern int32 VFX_shape_bounds(void *shape_table, int32 shape_num);
extern int32 VFX_shape_origin(void *shape_table, int32 shape_num);
extern int32 VFX_shape_resolution(void *shape_table, int32 shape_num);
extern int32 VFX_shape_minxy(void *shape_table, int32 shape_num);
extern void VFX_shape_palette(void *shape_table, int32 shape_num,
	RGB *palette);
extern int32 VFX_shape_colors(void *shape_table, int32 shape_num,
	CRGB *colors);
extern int32 VFX_shape_set_colors(void *shape_table, int32 shape_number,
	CRGB *colors);
extern int32 VFX_shape_count(void *shape_table);
extern int32 VFX_shape_list(void *shape_table, uint32 *index_list);
extern int32 VFX_shape_palette_list(void *shape_table, uint32 *index_list);

extern int32 VFX_pixel_write(PANE *pane, int32 x, int32 y, uint32 color);
extern int32 VFX_pixel_read(PANE *pane, int32 x, int32 y);

extern int32 VFX_rectangle_hash(PANE *pane, int32 x0, int32 y0,
	int32 x1, int32 y1, uint32 color);

extern int32 VFX_pane_wipe(PANE *pane, int32 color);
extern int32 VFX_pane_copy(PANE *source, int32 sx, int32 sy,
	PANE *target, int32 tx, int32 ty, int32 fill);

extern int32 VFX_pane_scroll(PANE *pane, int32 dx, int32 dy,
	int32 mode, int32 parm);

extern void VFX_ellipse_draw(PANE *pane, int32 xc, int32 yc,
	int32 width, int32 height, int32 color);
extern void VFX_ellipse_fill(PANE *pane, int32 xc, int32 yc,
	int32 width, int32 height, int32 color);

extern void VFX_point_transform(POINT *in, POINT *out, POINT *origin,
	int32 rot, int32 x_scale, int32 y_scale);

extern void VFX_Cos_Sin(int32 Angle, FIXED16 *Cos, FIXED16 *Sin);
extern void VFX_fixed_mul(FIXED16 M1, FIXED16 M2,
	FIXED16 *result);

extern int32 VFX_font_height(void *font);
extern int32 VFX_character_width(void *font, int32 character);
extern int32 VFX_character_draw(PANE *pane, int32 x, int32 y, void *font,
	int32 character, UBYTE *color_translate);
extern void VFX_string_draw(PANE *pane, int32 x, int32 y, void *font,
	char *string, UBYTE *color_translate);

extern int32 VFX_ILBM_draw(PANE *pane, UBYTE *ILBM_buffer);
extern void VFX_ILBM_palette(UBYTE *ILBM_buffer, RGB *palette);
extern int32 VFX_ILBM_resolution(UBYTE *ILBM_buffer);

extern void VFX_PCX_draw(PANE *pane, UBYTE *PCX_buffer);
extern void VFX_PCX_palette(UBYTE *PCX_buffer, int32 PCX_file_size,
	RGB *palette);
extern int32 VFX_PCX_resolution(UBYTE *PCX_buffer);

extern int32 VFX_GIF_draw(PANE *pane, UBYTE *GIF_buffer,
	void *GIF_scratch);
extern void VFX_GIF_palette(UBYTE *GIF_buffer, RGB *palette);
extern int32 VFX_GIF_resolution(UBYTE *GIF_buffer);

extern int32 VFX_pixel_fade(PANE *source, PANE *destination,
	int32 intervals, int32 rnd);

extern void VFX_window_fade(WINDOW *buffer, RGB *palette, int32 intervals);
extern int32 VFX_color_scan(PANE *pane, uint32 *colors);

extern void VFX_shape_transform(PANE *pane,
	void *shape_table, int32 shape_number, int32 hotX, int32 hotY,
	void *buffer, int32 rot, int32 x_scale, int32 y_scale, int32 flags);

//
// VFX 3D polygon functions
//

extern void VFX_flat_polygon(PANE *pane, int32 vcnt, SCRNVERTEX *vlist);

extern void VFX_Gouraud_polygon(PANE *pane, int32 vcnt,
	SCRNVERTEX *vlist);

extern void VFX_dithered_Gouraud_polygon(PANE *pane, FIXED16
	dither_amount, int32 vcnt, SCRNVERTEX *vlist);

extern void VFX_map_lookaside(UBYTE *table);

extern void VFX_map_polygon(PANE *pane, int32 vcnt, SCRNVERTEX *vlist,
	WINDOW *texture, uint32 flags);

extern void VFX_translate_polygon(PANE *pane, int32 vcnt, SCRNVERTEX *vlist,
	void *lookaside);

extern void VFX_illuminate_polygon(PANE *pane, FIXED16 dither_amount,
	int32 vcnt, SCRNVERTEX *vlist);

#ifdef __cplusplus
}
#endif

//
// MetaWare support
//

#ifdef __HIGHC__
#pragma Global_aliasing_convention();
#endif

} // namespace Aesop

#endif
