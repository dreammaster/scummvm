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

#ifndef MADS_CORE_TILE_H
#define MADS_CORE_TILE_H

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/color.h"

namespace MADS {
namespace MADSV2 {


#define DEFAULT_TILE_X          20
#define DEFAULT_TILE_Y          12

#define TILE_MAX_PAGES          14      /* Max EMS pages for a picture  */
/* (14 is enough for 4 screens) */
#define EMPTY_TILE              -1
#define REPEAT_TILE             0x8000

#define TILE_PICTURE            -1
#define TILE_ATTRIBUTE          0

#define TILE_MAP_SHADOW         0x0020  /* Load shadow for tile */

#define tile_ems_page           0
#define tile_ems_address        ems_page[tile_ems_page]

typedef struct {
	int num_tiles;                /* Number of tiles in resource      */
	int tile_x;                   /* Tile X size                      */
	int tile_y;                   /* Tile Y size                      */
	int compression;              /* Compression in resource file     */
	int ems_handle;               /* EMS handle of resource           */
	int num_pages;                /* Number of pages needed           */
	int tiles_per_page;           /* Tiles stored per page            */
	long chunk_size;              /* Tile size in bytes (x*y)         */
	int color_handle;             /* Color handle for loaded resource */
} TileResource;

typedef struct {
	long file_offset;
} Tile;

typedef struct {
	int tile_type;                /* Type of tile                     */
	int one_to_one;               /* One-to-one ratio with scr_orig   */
	int num_x_tiles;              /* Number of X tiles in map         */
	int num_y_tiles;              /* Number of Y tiles in map         */
	int tile_x_size;              /* Tile X size                      */
	int tile_y_size;              /* Tile Y size                      */
	int viewport_x;               /* Viewport X size                  */
	int viewport_y;               /* Viewport Y size                  */
	int orig_x_size;              /* Orig buffer X pixel size         */
	int orig_y_size;              /* Orig buffer Y pixel size         */
	int orig_x_tiles;             /* Orig buffer X tile size          */
	int orig_y_tiles;             /* Orig buffer Y tile size          */
	int total_x_size;             /* Total picture X size (pixels)    */
	int total_y_size;             /* Total picture Y size (pixels)    */

	int pan_x;                    /* Panned to pixel X value          */
	int pan_y;                    /* Panned to pixel Y value          */
	int pan_tile_x;               /* Panned to tile X value           */
	int pan_tile_y;               /* Panned to tile Y value           */
	int pan_base_x;               /* Base orig screen pixel X value   */
	int pan_base_y;               /* Base orig screen pixel Y value   */
	int pan_offset_x;             /* Panning orig offset to work X    */
	int pan_offset_y;             /* Panning orig offset to work Y    */

	TileResource *resource;       /* Resource pointer                 */
	Buffer *buffer;               /* Buffer pointer                   */

	int *map;                     /* Picture tile map pointer         */
} TileMapHeader;

extern ShadowList tile_shadow;


/* tile_1.c */
int tile_load(char *base,
	int           tile_type,
	TileResource *tile_resource,
	TileMapHeader *map,
	Buffer *picture,
	ColorListPtr  color_list,
	CycleListPtr  cycle_list,
	int           ems_handle,
	int           load_flags);

extern int tile_load_error;

/* tile_2.c */
int tile_buffer(Buffer *target,
	TileResource *tile_resource,
	TileMapHeader *map,
	int           tile_x,
	int           tile_y);

/* tile_3.c */
extern int tile_ems_available;
extern int tile_picture_handle;
extern int tile_attribute_handle;

int tile_setup(void);

/* tile_4.c */
void tile_map_free(TileMapHeader *map);

/* tile_5.c */
void tile_pan(TileMapHeader *tile_map,
	int           x,
	int           y);

/* tile_6.c */
int tile_fake_map(int tile_type,
	TileMapHeader *tile_map,
	Buffer *buffer,
	int           x,
	int           y);

} // namespace MADSV2
} // namespace MADS

#endif
