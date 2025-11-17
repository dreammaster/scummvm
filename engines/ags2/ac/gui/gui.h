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

#ifndef AGS2_AC_GUI_GUI_H
#define AGS2_AC_GUI_GUI_H

#include "common/stream.h"
#include "ags2/common/dynamic_array.h"
#include "ags2/ac/gui/button.h"
#include "ags2/ac/gui/gui_object.h"
#include "ags2/ac/gui/interface.h"
#include "ags2/ac/gui/inv.h"
#include "ags2/ac/gui/label.h"
#include "ags2/ac/gui/listbox.h"
#include "ags2/ac/gui/main.h"
#include "ags2/ac/gui/slider.h"
#include "ags2/ac/gui/textbox.h"

namespace AGS2 {

#define THIS_IS_THE_ENGINE

#define GUIF_NOCLICK    1
#define MOVER_MOUSEDOWNLOCKED -4000

#ifndef WOUTTEXT_REVERSE
#define WOUTTEXT_REVERSE wouttext_outline
#endif

#define GUIMAGIC          0xcafebeef
//#define MAX_OBJ_EACH_TYPE 251



#ifdef THIS_IS_THE_ENGINE
extern void wouttext_outline(int, int, int, char *);
inline void check_font(int *fontnum) {}
#else

#define wouttext_outline(a, b, c, d) wouttextxy(a, b, c, d)
extern void check_font(int *fontnum);
#endif

} // namespace AGS2

#endif
