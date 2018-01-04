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

#ifndef LEGEND_REGION_H
#define LEGEND_REGION_H

#include "common/array.h"
#include "common/rect.h"

namespace Legend {
namespace Gfx {

class Region : public Common::Rect {
public:
	int _val1;
	int _val2;
public:
	Region() : Common::Rect(), _val1(0), _val2(0) {}
	Region(const Common::Rect &r) : Common::Rect(r), _val1(0), _val2(0) {}
};

class Regions : public Common::Array<Region> {
public:
	explicit Regions() : Common::Array<Region>() {}
	Regions(size_type count) : Common::Array<Region>(count) {}
	Regions(size_type count, const Region &value) : Common::Array<Region>(count, value) {}
	Regions(const Regions &array) : Common::Array<Region>(array) {}
	Regions(const Region *array, size_type n) : Common::Array<Region>(array, n) {}
	Regions(const Common::Rect *array, size_type n);

	/**
	 * Add a region to the list
	 */
	int add(const Region &r) {
		push_back(r);
		return size() - 1;
	}

	/**
	 * Add a region to the list
	 */
	int add(const Common::Rect &r) { 
		push_back(r);
		return size() - 1;
	}

	/**
	 * Returns the index of a region that contains a given point
	 * @returns		Index of match, or -1 for none
	 */
	int indexOf(const Common::Point &pt);
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
