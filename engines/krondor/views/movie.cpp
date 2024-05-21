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

#include "krondor/views/movie.h"

namespace Krondor {
namespace Views {

void Movie::play(const Common::String &name, bool repeat) {
	if (!_decoder.loadFile(Common::Path(name)))
		error("Could not load - %s", name.c_str());
	_decoder.setRepeat(repeat);

	g_events->replaceView("Movie");
}

bool Movie::msgFocus(const FocusMessage &msg) {
	View::msgFocus(msg);

	_decoder.start();
	tick();
	return true;
}

bool Movie::tick() {
	if (_decoder.needsUpdate())
		redraw();
	return true;
}

void Movie::draw() {
	GfxSurface s = getSurface();
	const Graphics::ManagedSurface *src = _decoder.decodeNextFrame();
	s.blitFrom(*src);
}

} // namespace Views
} // namespace Krondor
