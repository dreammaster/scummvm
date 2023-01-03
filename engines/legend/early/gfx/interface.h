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

#ifndef LEGEND_EARLY_GFX_INTERFACE_H
#define LEGEND_EARLY_GFX_INTERFACE_H

#include "legend/gfx/view_element.h"
#include "legend/gfx/compass.h"
#include "legend/early/gfx/command_buttons.h"
#include "legend/early/gfx/scene.h"
#include "legend/early/gfx/listbox.h"
#include "legend/early/gfx/text_area.h"

namespace Legend {
namespace Early {
namespace Gfx {

class Interface : public Legend::Gfx::ViewElement {
private:
	CommandButtons _commandButtons;
	Legend::Gfx::Compass _compass;
	Scene _scene;
	Listbox _commands;
	Listbox _items;
	TextArea _textArea;
public:
	Interface(UIElement *parent);
	~Interface() override {}
};

} // namespace Gfx
} // namespace Early
} // namespace Legend

#endif
