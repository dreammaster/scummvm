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

#ifndef ULTIMA5_VIEWS_TITLE_H
#define ULTIMA5_VIEWS_TITLE_H

#include "common/array.h"
#include "graphics/managed_surface.h"
#include "ultima/ultima5/gfx/bit_image_file.h"
#include "ultima/ultima5/views/title_base.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

/**
 * The start of the game: the Origin logo and the credits to Lord British, the logo of the game coming up
 * and the subtitle after it, and then the main menu, which comes once a key is pressed over the view
 */
class Title : public TitleBase {
public:
	// A line of the Origin logo that is drawn from another, or cleared
	struct OriginOp {
		int _y;
		int _source;
	};

	// What is on the screen after one of the steps of the Origin logo coming together
	struct OriginStep {
		uint32 _delay;
		Common::Array<OriginOp> _ops;
	};

private:
	enum Phase {
		PHASE_ORIGIN,
		PHASE_PRESENTS,
		PHASE_A,
		PHASE_TRACE,
		PHASE_PRODUCTION,
		PHASE_LOGO,
		PHASE_SUBTITLE_FIRST,
		PHASE_SUBTITLE_SECOND,
		PHASE_MENU
	};

	Phase _phase = PHASE_ORIGIN;
	bool _started = false;
	bool _showNotice = false;
	int _selected = 0;
	int _animFrame = 0;
	uint32 _phaseStart = 0;
	uint32 _lastKey = 0;

	// What the start of the game shows, which is drawn in a screen of its own
	Gfx::BitImageFile _titleBits;
	Gfx::BitImageFile _britishBits;
	Gfx::BitImageFile _wdMask;
	Common::Array<byte> _path;
	Graphics::ManagedSurface _screen;
	Graphics::ManagedSurface _source;

	// The Origin logo
	Common::Array<OriginStep> _originSteps;
	uint _originStep = 0;
	uint32 _originNext = 0;

	// The writing of Lord British
	int _segment = 0;
	int _pathPos = 0;
	int _penX = 0;
	int _penY = 0;
	byte _penColor = 0;
	uint32 _traceTime = 0;

	// The pixels of the logo and the subtitle that are shown so far, and the order that comes next
	Common::Array<uint32> _revealOrder;
	Common::Array<byte> _shown;
	uint _revealed = 0;
	int _subtitleFrame = 0;
	uint16 _wdSeed = 1;
	int _wdIterations = 0;
	int _wdCountdown = 0;
	int _wdPlotted = 0;
	int _wdTotal = 0;
	int _clickOdds = 0;

	bool loadStartup();
	void buildOrigin();
	void startOrigin();
	void advanceOrigin(uint32 now);
	void startPresents(uint32 now);
	void startA(uint32 now);
	void startTrace(uint32 now);
	void advanceTrace(uint32 now);
	void startProduction(uint32 now);
	void startLogo(uint32 now);
	void advanceLogo(uint32 now);
	void startSubtitle(uint32 now, bool second);
	void advanceSubtitle(uint32 now);
	void finishIntro(bool skipped);
	void drawStartup();
	void drawReveal();

	/**
	 * Moves the highlight to a choice, and carries it out if it's one that's
	 * chosen directly by its letter
	 */
	void choose(int index, bool activate);

	void activate(int index);

public:
	Title() : TitleBase("Title") {}
	~Title() override {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	void timeout() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
