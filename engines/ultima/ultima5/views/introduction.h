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

#ifndef ULTIMA5_VIEWS_INTRODUCTION_H
#define ULTIMA5_VIEWS_INTRODUCTION_H

#include "common/array.h"
#include "graphics/managed_surface.h"
#include "ultima/shared/engine/view.h"
#include "ultima/ultima5/gfx/image_file.h"
#include "ultima/ultima5/gfx/story_text.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

/**
 * The introduction, a series of illustrated scenes of the story of how the
 * Avatar comes to Britannia, each shown on a key press
 */
class Introduction : public Shared::View {
private:
	Gfx::ImageFile _banners;
	Gfx::ImageFile _pictures;
	int _pictureFile = -1;
	Gfx::StoryText _text;
	Common::Array<byte> _story;
	Graphics::ManagedSurface _page;
	int _scene = 0;

	/**
	 * Draws a scene onto the page
	 */
	void buildScene(int scene);

	/**
	 * Returns the story text of a scene
	 */
	Common::String sceneText(int scene) const;

public:
	Introduction() : Shared::View("Introduction") {}
	~Introduction() override {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
