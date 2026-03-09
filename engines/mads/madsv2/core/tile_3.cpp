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

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/ems.h"
#include "mads/madsv2/core/dialog.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/tile.h"

namespace MADS {
namespace MADSV2 {

int tile_ems_available = false;
int tile_picture_handle = -1;
int tile_attribute_handle = -1;


int tile_setup(void) {
	int error_flag = true;

	picture_map.map = NULL;
	depth_map.map = NULL;

	tile_picture_handle = ems_get_page_handle(TILE_MAX_PAGES);
	if (tile_picture_handle < 0) goto done;

	tile_attribute_handle = ems_get_page_handle(TILE_MAX_PAGES >> 1);
	if (tile_attribute_handle < 0) goto done;

	tile_ems_available = true;
	error_flag = false;

done:
	if (error_flag) {
		if (tile_picture_handle >= 0) ems_free_page_handle(tile_picture_handle);
	}

	return error_flag;
}

} // namespace MADSV2
} // namespace MADS
