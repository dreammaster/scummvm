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

#ifndef ULTIMA5_VIEWS_TITLE_DEMO_H
#define ULTIMA5_VIEWS_TITLE_DEMO_H

#include "common/array.h"
#include "graphics/managed_surface.h"
#include "ultima/ultima5/views/title_base.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

/**
 * The view of the game that plays under the logo until a key is pressed: short scenes of people and
 * places that come together, in the place where the menu is
 */
class TitleDemo : public TitleBase {
private:
	enum EventType {
		EVENT_SCENE,
		EVENT_ACTOR,
		EVENT_REMOVE,
		EVENT_MOVE,
		EVENT_WAIT,
		EVENT_TILE,
		EVENT_CLEAR_ACTORS,
		EVENT_HIDE_CELL,
		EVENT_SHOW_ACTOR,
		EVENT_SET_GLYPH,
		EVENT_REVEAL,
		EVENT_LINES,
		EVENT_BLANK_CELL,
		EVENT_NOISE
	};

	struct Event {
		EventType _type;
		int _v0 = 0;
		int _v1 = 0;
		int _v2 = 0;
		int _v3 = 0;

		Event(EventType type, int a = 0, int b = 0, int c = 0, int d = 0) : _type(type), _v0(a), _v1(b), _v2(c), _v3(d) {}
	};

	struct Actor {
		byte _tile = 0;
		byte _anim = 0;
		byte _x = 0;
		byte _y = 0;
	};

	Common::Array<byte> _script;
	Common::Array<Event> _events;
	uint _eventIndex = 0;
	int _wait = 0;
	uint32 _nextFrame = 0;

	// What the scene is: the ground, and a copy of it for when something is put back, and what is on it
	byte _map[4][32] = {};
	byte _copy[4][32] = {};
	byte _overlay[4][32] = {};
	Actor _actors[32];
	Graphics::ManagedSurface _band;
	Graphics::ManagedSurface _cell;

	// How much of the scene is shown, which widens from its middle
	int _left = 9;
	int _right = 9;
	bool _widen = false;
	int _scene = 0;
	int _clockBeat = 0;
	int _subtitleFrame = 0;
	const char *_caption = nullptr;

	// Where the shuffled order of the pixels of a tile has got to
	byte _revealX[256] = {};
	byte _revealY[256] = {};

	void expand();
	void apply(const Event &event);
	void frame();
	void drawCell(int x, int y);
	void animateActors();
	void revealPixels(int tile, int x, int y, int first, int last);

public:
	TitleDemo() : TitleBase("TitleDemo") {}
	~TitleDemo() override {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	void timeout() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
