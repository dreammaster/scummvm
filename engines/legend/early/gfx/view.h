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

#ifndef LEGEND_EARLY_GFX_VIEW_H
#define LEGEND_EARLY_GFX_VIEW_H

#include "legend/gfx/view.h"

namespace Legend {
namespace Early {
namespace Gfx {

enum {
	WHITE = 0,
	LIGHT_GRAY = 1,
	DARK_GRAY = 2,
	BLACK = 15
};

class View : public Legend::Gfx::View {
public:
	View(const Common::String &name, UIElement *uiParent = nullptr) :
		Legend::Gfx::View(name, uiParent) {}
	View() : Legend::Gfx::View() {}
	View(UIElement *uiParent) : Legend::Gfx::View(uiParent) {}
	View(UIElement *uiParent, const Common::Rect &area) :
		Legend::Gfx::View(uiParent, area) {}
	View(const Common::Rect &area) : Legend::Gfx::View(area) {}
	~View() override {}
};

} // namespace Gfx
} // namespace Early
} // namespace Legend

#endif
