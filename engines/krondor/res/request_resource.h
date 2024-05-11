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

#ifndef KRONDOR_RES_REQUEST_RESOURCE_H
#define KRONDOR_RES_REQUEST_RESOURCE_H

#include "common/array.h"
#include "krondor/res/resource.h"
#include "krondor/res/rect.h"

namespace Krondor {

enum RequestType {
	REQ_USERDEFINED = 0,
	REQ_IMAGEBUTTON = 3,
	REQ_CHOICE = 4,
	REQ_TEXTBUTTON = 6
};

struct RequestData {
	uint _widget = 0;
	int _action = 0;
	bool _visible = 0;
	Rect _bounds;
	int _teleport = 0;
	int _image = 0;
	int _group = 0;
	Common::String _label;
};

class RequestResource : public Resource {
public:
	bool _popup = false;
	Rect _bounds;
	int _xoff = 0;
	int _yoff = 0;
	Common::Array<RequestData> _data;

public:
	RequestResource() : Resource() {}
	RequestResource(const Common::String &name) : Resource() {
		load(name);
	}
	~RequestResource() override {}

	void clear();
	void load(const Common::String &name);
};

} // namespace Krondor

#endif
