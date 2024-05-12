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

#ifndef KRONDOR_CORE_VARS_H
#define KRONDOR_CORE_VARS_H

#include "krondor/res/image_resource.h"
#include "krondor/core/preferences.h"

namespace Krondor {

struct Vars {
public:
	Preferences _prefs;
	ImageResource _icons1, _icons2;

public:
	Vars();
	~Vars();

	/**
	 * Get an icon
	 */
	const Graphics::ManagedSurface *getIcon(uint index) const;
};

extern Vars *g_vars;
#define _G(FIELD) g_vars->_##FIELD

} // namespace Krondor

#endif
