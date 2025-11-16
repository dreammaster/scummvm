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

#include "ags2/ac/resolution.h"
#include "ags2/vars.h"

namespace AGS2 {

// Multiplies up the number of pixels depending on the current 
// resolution, to give a relatively fixed size at any game res
int get_fixed_pixel_size(int pixels) {
	return pixels * current_screen_resolution_multiplier;
}

int convert_to_low_res(int coord) {
	if (game.options[OPT_NATIVECOORDINATES] == 0)
		return coord;
	else
		return coord / current_screen_resolution_multiplier;
}

int convert_back_to_high_res(int coord) {
	if (game.options[OPT_NATIVECOORDINATES] == 0)
		return coord;
	else
		return coord * current_screen_resolution_multiplier;
}

int multiply_up_coordinate(int coord) {
	if (game.options[OPT_NATIVECOORDINATES] == 0)
		return coord * current_screen_resolution_multiplier;
	else
		return coord;
}

void multiply_up_coordinates(int *x, int *y) {
	if (game.options[OPT_NATIVECOORDINATES] == 0)
	{
		x[0] *= current_screen_resolution_multiplier;
		y[0] *= current_screen_resolution_multiplier;
	}
}

void multiply_up_coordinates_round_up(int *x, int *y) {
	if (game.options[OPT_NATIVECOORDINATES] == 0)
	{
		x[0] = x[0] * current_screen_resolution_multiplier + (current_screen_resolution_multiplier - 1);
		y[0] = y[0] * current_screen_resolution_multiplier + (current_screen_resolution_multiplier - 1);
	}
}

int divide_down_coordinate(int coord) {
	if (game.options[OPT_NATIVECOORDINATES] == 0)
		return coord / current_screen_resolution_multiplier;
	else
		return coord;
}

int divide_down_coordinate_round_up(int coord) {
	if (game.options[OPT_NATIVECOORDINATES] == 0)
		return (coord / current_screen_resolution_multiplier) + (current_screen_resolution_multiplier - 1);
	else
		return coord;
}

} // namespace AGS2
