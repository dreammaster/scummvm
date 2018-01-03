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

#ifndef LEGEND_EARLY_SCENE_H
#define LEGEND_EARLY_SCENE_H

#include "legend/gfx/visual_container.h"
#include "legend/gfx/compass.h"
#include "legend/early/gfx/command_buttons.h"
#include "legend/early/gfx/scene_picture.h"
#include "legend/early/gfx/listbox.h"
#include "legend/early/gfx/text_area.h"

namespace Legend {
namespace Early {

class Scene : public Gfx::VisualContainer {
	DECLARE_MESSAGE_MAP;
private:
	CommandButtons _commandButtons;
	Gfx::Compass _compass;
	ScenePicture _scenePicture;
	Listbox _commands;
	Listbox _items;
	TextArea _textArea;
public:
	CLASSDEF;
	Scene();
	virtual ~Scene() {}
};

} // End of namespace Early
} // End of namespace Legend

#endif
