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

#ifndef ULTIMA5_VIEWS_DUNGEON_SCENE_H
#define ULTIMA5_VIEWS_DUNGEON_SCENE_H

#include "ultima/ultima5/gfx/image_file.h"
#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

/**
 * What the party sees in front of it in a dungeon: the walls, doors and ladders, the
 * things in the cells, and the sides
 */
class DungeonScene {
private:
	Gfx::ImageFile _walls;
	Gfx::ImageFile _items;
	int _style = 0;
	int _wobble = 0;
	View *_view = nullptr;
	Graphics::ManagedSurface *_surface = nullptr;

	void drawFrame(int frame, bool items, bool mirrored);
	bool drawWall(int x, int y, int depth);
	void drawSide(int x, int y, bool mirrored, int depth);
	void drawFeatures(int x, int y, int depth);
	void drawField(int depth, int kind);
	void drawTorchGlow(int depth, int frame);
	int drawFlame(int x, int y, int state, int depth);
	void drawSign(int number);

public:
	/**
	 * Draws the view into the window of the map
	 */
	void draw(View &view);
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
