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

#ifndef MADS_CORE_SPRITE_E_H
#define MADS_CORE_SPRITE_E_H

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/pal.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/loader.h"
#include "mads/madsv2/core/sprite.h"
#include "mads/madsv2/core/sprite_h.h"

namespace MADS {
namespace MADSV2 {

int sprite_error = 0;

byte *sprite_force_memory = NULL;
long sprite_force_size;

const byte color_table[7] = { 7, 246, 247, 248, 249, 250, 251 };


SeriesPtr sprite_series_load(const char *filename, int load_flags) {
	register int count;
	int len;
	int len2;
	int found, low_color, color_pointer;
	byte *base_pointer;
	byte *sprite_marker;
	char temp_buf[80];
	char block_name[20];
	char *mark;
	long base_quantity, quantity;
	long initial_quantity;
	long total_color_size;
	long largest_block;
	long total_offset;
	FileSeries header;
	FileSpritePtr sprite = NULL;
	SeriesPtr target = NULL;
	SeriesPtr result = NULL;
	SpritePageInfoPtr  page_info;
	SpritePageTablePtr page_table;
	ColorListPtr color_list = NULL;
	/* ColorList color_list; */
	Load load_handle;

	mem_last_alloc_loader = MODULE_SPRITE_LOADER;

	load_handle.open = false;

	Common::strcpy_s(temp_buf, filename);
	if (strchr(temp_buf, '.') == NULL) {
		Common::strcat_s(temp_buf, ".SS");
	}

	Common::strcpy_s(block_name, "S$");
	mark = temp_buf;
	mads_strupr(temp_buf);
	if (*mark == '*') mark++;
	if ((*mark == 'R') && (*(mark + 1) == 'M')) {
		mark += 2;
	}
	strncat(block_name, mark, 6);


	/* Open our input file */
	if (loader_open(&load_handle, temp_buf, "rb", true)) {
		sprite_error = SS_ERR_OPENFILE;
		goto done;
	}

	/* Set default error condition */

	sprite_error = SS_ERR_READFILE;

	/* Determine length of header, and read it */

	len = sizeof(FileSeries) - sizeof(FileSprite);

	if (!loader_read(&header, len, 1, &load_handle)) goto done;

	if (header.misc_is_a_walker) load_flags |= SPRITE_LOAD_WALKER_INFO;

	/* Determine length of index record array */

	len2 = sizeof(FileSprite) * header.num_sprites;

	base_quantity = sizeof(Series) + (sizeof(Sprite) * (header.num_sprites - 1));
	if (load_flags & SPRITE_LOAD_WALKER_INFO) {
		base_quantity += sizeof(WalkerInfo);
	}
	initial_quantity = base_quantity;
	if (header.pack_by_sprite) {
		base_quantity += (SPRITE_COLOR_TABLE_SIZE + sizeof(SpritePageInfo) + (sizeof(SpritePageTable) * header.num_sprites));
	}
	quantity = base_quantity;

	if (!(load_flags & SPRITE_LOAD_HEADER_ONLY)) {
		if (!header.pack_by_sprite) {
			quantity += header.total_data_size;
		}
	}

	/* Allocate memory for entire series (target) and for the file-formatted */
	/* index record array (sprite)                                           */

	if (sprite_force_memory != NULL) {
		if (quantity <= sprite_force_size) {
			target = (SeriesPtr)sprite_force_memory;
		}
	}

	if (target == NULL)
		// TODO: Check memory alignment
		target = (SeriesPtr)mem_get_name(quantity, block_name);
	if (target == NULL) {
		sprite_error = SS_ERR_NOMOREMEMORY;
		goto done;
	}

	// TODO: Check memory alignment
	sprite = (FileSpritePtr)mem_get_name(len2, "$sp-load");
	if (sprite == NULL) {
		sprite_error = SS_ERR_NOMOREMEMORY;
		goto done;
	}

	target->walker = NULL;
	target->color_table = NULL;
	target->page_info = NULL;
	target->page_table = NULL;
	target->arena = NULL;

	/* Read the index record array */

	if (!loader_read(sprite, len2, 1, &load_handle)) {
		sprite_error = SS_ERR_READFILE;
		goto done;
	}

	/* Read the color list */

	total_color_size = load_handle.pack.strategy[load_handle.pack_list_marker].size;

	// TODO: Confirm memory alignment
	color_list = (ColorListPtr)mem_get_name(total_color_size, "$color$");
	if (color_list == NULL) {
		sprite_error = SS_ERR_NOMOREMEMORY;
		goto done;
	}

	if (!loader_read(color_list, total_color_size, 1, &load_handle)) goto done;

	/* Copy relevant header data to target header */

	target->pack_by_sprite = header.pack_by_sprite;
	target->delta_series = header.delta_series && (header.base_mode < SS_INDIVIDUAL);
	target->base_mode = header.base_mode;
	target->num_sprites = header.num_sprites;
	target->offset_x_view = header.offset_x_view;
	target->offset_y_view = header.offset_y_view;

	for (count = 0; count < 16; count++) {
		target->misc[count] = header.misc[count];
	}

	/* Copy walker information, if requested */

	if (load_flags & SPRITE_LOAD_WALKER_INFO) {
		target->walker = (WalkerInfoPtr)(((byte *)(target)) + (initial_quantity - sizeof(WalkerInfo)));
		// TODO: Mem alignment check
		target->walker = (WalkerInfoPtr)mem_normalize(target->walker);
		memcpy(target->walker, &header.walker, sizeof(WalkerInfo));
	}

	/* base_pointer points to the beginning of the memory block at which */
	/* the sprite data will be loaded.                                   */

	base_pointer = (byte *) (((byte *)target) + base_quantity);
	base_pointer = (byte *)mem_normalize(base_pointer);
	sprite_marker = base_pointer;

	/* Set up the target index record for each sprite, including a pointer */
	/* to the memory block designated for its sprite data.                 */

	for (count = 0; count < target->num_sprites; count++) {
		target->index[count].x = sprite[count].x;
		target->index[count].y = sprite[count].y;
		target->index[count].xs = sprite[count].xs;
		target->index[count].ys = sprite[count].ys;
		if (!(load_flags & SPRITE_LOAD_HEADER_ONLY) && !header.pack_by_sprite) {
			target->index[count].data = sprite_marker;
			sprite_marker = (byte *)mem_normalize(sprite_marker + sprite[count].memory_needed);
		} else {
			target->index[count].data = NULL;
		}
	}

	/* Load all of the sprite data in at the base address */

	if (!(load_flags & SPRITE_LOAD_HEADER_ONLY) && !header.pack_by_sprite) {
		if (!loader_read(base_pointer, header.total_data_size, 1, &load_handle)) goto done;
	}

	/* Perform palette allocation and color list transformation */

	if (load_flags & (SPRITE_LOAD_TRANSLATE | SPRITE_LOAD_SPINNING_OBJECT)) {
		target->color_handle = 0;
		if (load_flags & (SPRITE_LOAD_SPINNING_OBJECT)) {
			color_pointer = 0;
			for (count = 0; count < color_list->num_colors; count++) {
				found = false;
				for (low_color = 0; !found && (low_color < 4); low_color++) {
					if (memcmp(&color_list->table[count].r, &master_palette[low_color].r, sizeof(RGBcolor)) == 0) {
						found = true;
						color_list->table[count].x16 = (byte)low_color;
					}
				}
				if (!found) {
					memcpy(&master_palette[color_table[color_pointer]].r,
						&color_list->table[count].r, sizeof(RGBcolor));
					color_list->table[count].x16 = (byte)color_table[color_pointer];
					color_pointer = MIN(6, color_pointer + 1);
				}
			}
			sprite_color_translate(target, color_list);
		}
	} else {

		target->color_handle = pal_allocate(color_list, master_shadow, load_flags & PAL_MAP_MASK);
		if (target->color_handle < 0) {
			sprite_error = SS_ERR_TOOMANYCOLORS;
			goto done;
		}

		if (!(load_flags & SPRITE_LOAD_HEADER_ONLY) && !header.pack_by_sprite) {
			sprite_color_translate(target, color_list);
		}
	}

	/* If series was packed by sprite (and we are therefore expected to */
	/* stream-load it), we need to set up paging tables for the series. */
	/* This process is different depending on whether the sprite data   */
	/* is to be loaded from disk or from the EMS preload area.          */

	if (header.pack_by_sprite) {
		target->color_table = ((byte *)target) + initial_quantity;
		page_info = target->page_info = (SpritePageInfoPtr)(((byte *)target->color_table) + SPRITE_COLOR_TABLE_SIZE);
		page_table = target->page_table = (SpritePageTablePtr)(((byte *)page_info) + sizeof(SpritePageInfo));

		for (count = 0; count < color_list->num_colors; count++) {
			target->color_table[count] = color_list->table[count].x16;
		}

		page_info->packing_mode = header.compression;
		page_info->paging_source = (byte)load_handle.mode;

		if ((page_info->paging_source == LOADER_EMS) ||
			(page_info->paging_source == LOADER_XMS)) {
			page_info->ems_handle = load_handle.ems_handle;
			page_info->ems_page_marker = load_handle.ems_page_marker;
			page_info->ems_page_offset = load_handle.ems_page_offset;
			page_info->xms_handle = load_handle.xms_handle;
			page_info->xms_offset = load_handle.xms_offset;
			largest_block = 0;
			total_offset = 0;
			for (count = 0; count < target->num_sprites; count++) {
				page_table[count].file_offset = total_offset;
				page_table[count].memory_needed = sprite[count].memory_needed;
				total_offset += page_table[count].memory_needed;
				largest_block = MAX(largest_block, page_table[count].memory_needed);
			}
		} else {
			page_info->handle = load_handle.handle;
			total_offset = load_handle.handle->pos();
			page_info->base_sprite_offset = total_offset;

			largest_block = 0;
			for (count = 0; count < target->num_sprites; count++) {
				page_table[count].file_offset = sprite[count].file_offset;
				page_table[count].memory_needed = sprite[count].memory_needed;
				largest_block = MAX(largest_block, page_table[count].memory_needed);
			}
		}

		if (!(load_flags & SPRITE_LOAD_HEADER_ONLY)) {
			if (color_list != NULL) mem_free(color_list);
			color_list = NULL;

			if (sprite != NULL) mem_free(sprite);
			sprite = NULL;

			target->arena = (byte *)mem_get_name(largest_block, "$arena$");
			if (target->arena == NULL) goto done;

			memcpy(&target->misc_largest_block, &largest_block, sizeof(long));

			load_handle.open = false;   /* Hack handle so it won't get closed */
		} else {
			target->arena = NULL;
		}
	}

	result = target;

done:
	if (load_handle.open) loader_close(&load_handle);
	if (color_list != NULL) mem_free(color_list);
	if (sprite != NULL) mem_free(sprite);
	if ((target != NULL) && (target != (SeriesPtr)sprite_force_memory) && (result == NULL)) mem_free(target);

	return (result);
}
} // namespace MADSV2
} // namespace MADS

#endif
