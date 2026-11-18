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

#include "common/keyboard.h"
#include "common/util.h"
#include "ultima/ultima5/views/asset_viewer.h"
#include "ultima/ultima5/gfx/tileset.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int TILE_COLUMNS = 20;
constexpr int TILE_ROWS = 12;
constexpr int TILES_PER_PAGE = TILE_COLUMNS * TILE_ROWS;

int AssetViewer::pageCount() const {
	return _mode == MODE_TILES ? (Gfx::TILE_COUNT + TILES_PER_PAGE - 1) / TILES_PER_PAGE : (int)_image.size();
}

void AssetViewer::showTiles() {
	_mode = MODE_TILES;
	_index = 0;
}

bool AssetViewer::showImages(const Common::String &name, int index) {
	if (!_image.load(name.c_str(), _G(renderMode)) || _image.size() == 0)
		return false;

	_mode = MODE_IMAGE;
	_imageName = name;
	_index = CLIP<int>(index, 0, _image.size() - 1);
	return true;
}

void AssetViewer::draw() {
	auto s = getSurface();
	s.clear();

	if (_mode == MODE_TILES) {
		const int first = _index * TILES_PER_PAGE;

		for (int i = 0; i < TILES_PER_PAGE && first + i < Gfx::TILE_COUNT; ++i)
			_G(tileset).drawTile(s, (i % TILE_COLUMNS) * Gfx::TILE_SIZE, (i / TILE_COLUMNS) * Gfx::TILE_SIZE, first + i);

		s.writeString(Common::Point(0, 24), Common::String::format("Tiles %d-%d", first,
			MIN(first + TILES_PER_PAGE, Gfx::TILE_COUNT) - 1));
	} else {
		_image.draw(s, _index, 0, 0);

		s.writeString(Common::Point(0, 24), Common::String::format("%s %d/%d (%dx%d)", _imageName.c_str(),
			_index, _image.size() - 1, _image.getWidth(_index), _image.getHeight(_index)));
	}
}

bool AssetViewer::msgKeypress(const KeypressMessage &msg) {
	switch (msg.keycode) {
	case Common::KEYCODE_LEFT:
		_index = (_index + pageCount() - 1) % pageCount();
		redraw();
		break;
	case Common::KEYCODE_RIGHT:
		_index = (_index + 1) % pageCount();
		redraw();
		break;
	case Common::KEYCODE_ESCAPE:
		replaceView("Title", true);
		break;
	default:
		return false;
	}

	return true;
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
