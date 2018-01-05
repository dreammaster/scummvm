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

#ifndef LEGEND_GATEWAY_PROJECT_ITEM_H
#define LEGEND_GATEWAY_PROJECT_ITEM_H

#include "legend/core/project_item.h"
#include "legend/early/gfx/help_screen.h"
#include "legend/early/gfx/interface.h"
#include "legend/early/gateway/part_title.h"

namespace Legend {
namespace Early {
namespace Gateway {

class GatewayProjectItem : public ProjectItem {
	DECLARE_MESSAGE_MAP;
private:
	PartTitle _partTitleScreen;
	Interface _interface;
	Early::HelpScreen _helpScreen;
public:
	CLASSDEF;
	GatewayProjectItem();
};

} // End of namespace Gateway
} // End of namespace Early
} // End of namespace Legend

#endif
