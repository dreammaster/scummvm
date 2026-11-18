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

#ifndef ULTIMA5_VIEWS_ASSET_VIEWER_H
#define ULTIMA5_VIEWS_ASSET_VIEWER_H

#include "common/str.h"
#include "ultima/ultima5/gfx/image_file.h"
#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

/**
 * Debugging view for browsing the tiles and the images of the .16 files.
 * Left and right change page or image, and Escape returns to the title
 */
class AssetViewer : public View {
private:
	enum Mode { MODE_TILES, MODE_IMAGE };

	Mode _mode = MODE_TILES;
	int _index = 0;
	Common::String _imageName;
	Gfx::ImageFile _image;

	int pageCount() const;

public:
	AssetViewer() : View("AssetViewer") {}
	~AssetViewer() override {}

	/**
	 * Shows the tiles a screenful at a time
	 */
	void showTiles();

	/**
	 * Shows the images of a file, given without its extension
	 * @returns		False if the file couldn't be loaded
	 */
	bool showImages(const Common::String &name, int index = 0);

	void draw() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
