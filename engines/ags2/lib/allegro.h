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

#ifndef AGS2_LIB_ALLEGRO_H
#define AGS2_LIB_ALLEGRO_H

#define ALLEGRO_H

#include "ags2/lib/allegro/alconfig.h"
#include "ags2/lib/allegro/base.h"
#include "ags2/lib/allegro/color.h"
#include "ags2/lib/allegro/config.h"
#include "ags2/lib/allegro/draw.h"
#include "ags2/lib/allegro/error.h"
#include "ags2/lib/allegro/file.h"
#include "ags2/lib/allegro/fixed.h"
#include "ags2/lib/allegro/fmaths.h"
#include "ags2/lib/allegro/gfx.h"
#include "ags2/lib/allegro/system.h"
#include "ags2/lib/allegro/unicode.h"

namespace AGS2 {

inline int install_allegro() {
	return 0;
}

inline void allegro_exit() {}

} // namespace AGS2

#endif
