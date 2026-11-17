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

#include "common/file.h"
#include "ultima/ultima5/views/introduction.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int SCENE_COUNT = 21;
constexpr int MAX_TEXT_LENGTH = 2000;

// How a scene is laid out. Kind 1 has a banner from text.16 behind it, 2 has the
// door that is revealed beside the text, 3 has the door and fixed text, and 4 and
// above have a second picture below the first
enum SceneKind { KIND_PLAIN = 0, KIND_BANNER = 1, KIND_DOOR = 2, KIND_DOOR_OPENING = 3 };

// The left edge of each region of text, and the right edge, per scene
static const byte LEFT_EDGES[SCENE_COUNT][2] = {
	{ 0xB4, 0x00 }, { 0x00, 0xAC }, { 0x00, 0x00 }, { 0x00, 0xD2 }, { 0x00, 0x00 }, { 0xB0, 0x00 }, { 0x00, 0x00 },
	{ 0xBC, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },
	{ 0xB8, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x94 }, { 0x00, 0x00 }, { 0x94, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x9C }
};
static const uint16 RIGHT_EDGES[SCENE_COUNT][2] = {
	{ 0x140, 0x140 }, { 0x140, 0x140 }, { 0x084, 0x140 }, { 0x140, 0x140 }, { 0x140, 0x094 }, { 0x140, 0x140 }, { 0x140, 0x140 },
	{ 0x140, 0x140 }, { 0x140, 0x140 }, { 0x140, 0x140 }, { 0x140, 0x140 }, { 0x140, 0x140 }, { 0x140, 0x140 }, { 0x0AA, 0x140 },
	{ 0x140, 0x140 }, { 0x0AA, 0x140 }, { 0x140, 0x140 }, { 0x140, 0x0AA }, { 0x140, 0x140 }, { 0x140, 0x0AA }, { 0x140, 0x140 }
};

// Where the story text of each scene is in story.dat
static const uint16 TEXT_OFFSETS[SCENE_COUNT] = {
	0x0000, 0x0111, 0x03CB, 0x0590, 0x0803, 0x0AC4, 0x0000, 0x0D6D,
	0x0E23, 0x0FB6, 0x1173, 0x132A, 0x14BE, 0x162E, 0x192B, 0x1B9A,
	0x1E7B, 0x2172, 0x244B, 0x2780, 0x2A97
};

static const byte SPLIT_TOPS[SCENE_COUNT] = {
	0xB4, 0x46, 0x83, 0x20, 0x46, 0x85, 0xC8, 0xA8, 0xC8, 0xC8, 0xC8,
	0xC8, 0xC8, 0x72, 0x72, 0x60, 0x21, 0x46, 0x60, 0x33, 0x4F
};
static const byte SPLIT_BOTTOMS[SCENE_COUNT] = {
	0xC8, 0xC8, 0xC8, 0xA0, 0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0xC8,
	0xC8, 0xC8, 0xC8, 0xC8, 0xC8, 0x89, 0xC8, 0xC8, 0x92, 0xC8
};
static const byte START_X[SCENE_COUNT] = {
	0xB4, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x20, 0xBC, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00, 0xAE, 0x00, 0x00
};
static const byte START_Y[SCENE_COUNT] = {
	0x80, 0x00, 0x28, 0x00, 0x09, 0x00, 0x09, 0x88, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00
};

// Which picture of the scene's STORY file to show, and where
static const byte PICTURES[SCENE_COUNT] = {
	0x00, 0x01, 0x00, 0x01, 0x02, 0x02, 0x02, 0x00, 0x01, 0x00, 0x01,
	0x00, 0x01, 0x00, 0x01, 0x02, 0x06, 0x04, 0x02, 0x06, 0x04
};
static const byte STORY_FILES[SCENE_COUNT] = {
	0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03,
	0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05
};
static const byte PICTURE_X[SCENE_COUNT] = {
	0x00, 0x00, 0x88, 0x00, 0x98, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xB0, 0x00, 0xB0, 0x00, 0xB0, 0x00, 0xB0, 0x00
};
static const byte PICTURE_Y[SCENE_COUNT] = {
	0x00, 0x4A, 0x00, 0x26, 0x4C, 0x00, 0x26, 0x00, 0x52, 0x52, 0x52,
	0x52, 0x52, 0x00, 0x00, 0x00, 0x2E, 0x4E, 0x00, 0x37, 0x57
};
static const byte KINDS[SCENE_COUNT] = {
	1, 2, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 1, 4, 5, 6, 5, 6, 4
};

constexpr int SECOND_PICTURE_OFFSET = 0x37;

bool Introduction::msgFocus(const FocusMessage &msg) {
	if (_page.empty())
		_page.create(320, 200);

	Common::File f;
	if (_story.empty() && f.open("story.dat")) {
		_story.resize(f.size());
		f.read(&_story[0], _story.size());
	}

	if (!_text.load() || _story.empty() || !_banners.load("text", _G(renderMode)))
		error("Could not load the introduction");

	_pictureFile = -1;
	_scene = 0;
	buildScene(_scene);

	return Shared::View::msgFocus(msg);
}

Common::String Introduction::sceneText(int scene) const {
	Common::String result;

	for (uint i = TEXT_OFFSETS[scene]; i < _story.size() && i < (uint)TEXT_OFFSETS[scene] + MAX_TEXT_LENGTH &&
			_story[i] != 0; ++i)
		result += (char)_story[i];

	return result;
}

void Introduction::buildScene(int scene) {
	if (STORY_FILES[scene] != _pictureFile) {
		const Common::String name = Common::String::format("story%d", STORY_FILES[scene] + 1);
		if (!_pictures.load(name.c_str(), _G(renderMode)))
			error("Could not load %s", name.c_str());

		_pictureFile = STORY_FILES[scene];
	}

	_page.clear();
	const int kind = KINDS[scene];

	if (kind == KIND_BANNER) {
		switch (scene) {
		case 0:
			_banners.draw(_page, 0, 0xE0, 0x1E);
			_banners.draw(_page, 1, 0xA8, 0x3A);
			break;
		case 7:
			_banners.draw(_page, 0, 0xE8, 0x1A);
			_banners.draw(_page, 2, 200, 0x36);
			break;
		default:
			_banners.draw(_page, 0, 0xB8, 0);
			_banners.draw(_page, 3, 0xF8, 0);
			break;
		}
	}

	_pictures.draw(_page, PICTURES[scene], PICTURE_X[scene], PICTURE_Y[scene]);
	if (kind >= 4)
		_pictures.draw(_page, kind * 2 - 5, PICTURE_X[scene], PICTURE_Y[scene] + SECOND_PICTURE_OFFSET);

	Gfx::StoryLayout layout;
	layout._left[0] = LEFT_EDGES[scene][0];
	layout._left[1] = LEFT_EDGES[scene][1];
	layout._right[0] = RIGHT_EDGES[scene][0];
	layout._right[1] = RIGHT_EDGES[scene][1];
	layout._splitTop = SPLIT_TOPS[scene];
	layout._splitBottom = SPLIT_BOTTOMS[scene];

	int x = START_X[scene];
	int y = START_Y[scene];

	if (kind == KIND_DOOR_OPENING) {
		_pictures.draw(_page, 3, 0x60, 0x27);
		_text.draw(_page, "Instantly, a shimmering blue door springs up!", x, y, layout);

		x = START_X[scene];
		y = 0xB4;
		_text.draw(_page, "With heart beating rapidly, you step into it.", x, y, layout);
	} else {
		_text.draw(_page, sceneText(scene), x, y, layout);
	}

	if (kind == KIND_DOOR)
		_pictures.draw(_page, 2, 0x28, 0x56);
}

void Introduction::draw() {
	auto s = getSurface();
	s.blitFrom(_page);
}

bool Introduction::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode))
		return false;

	if (++_scene < SCENE_COUNT) {
		buildScene(_scene);
		redraw();
	} else {
		close();
	}

	return true;
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
