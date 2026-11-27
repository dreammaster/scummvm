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

#include "ultima/ultima5/views/title.h"
#include "common/file.h"
#include "common/system.h"
#include "ultima/ultima5/views/keys.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int FRAMES_PER_STEP = 3;
constexpr uint32 IDLE_MS = 30000;

enum MenuChoice {
	CHOICE_JOURNEY, CHOICE_CREATE, CHOICE_TRANSFER, CHOICE_INTRODUCTION,
	CHOICE_ACKNOWLEDGEMENTS, CHOICE_VIEW
};

// The letter that picks each choice
static const char MENU_LETTERS[] = "JCTUAR";

constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 200;
constexpr byte COLOR_LOGO_BLUE = 9;

// The pictures of title.bit and british.bit
constexpr int PICTURE_ORIGIN_COUNT = 7;
constexpr int PICTURE_PRESENTS = 7;
constexpr int PICTURE_A = 8;
constexpr int PICTURE_PRODUCTION = 9;
static const uint16 ORIGIN_WIDTHS[PICTURE_ORIGIN_COUNT] = { 0x18, 0x28, 0x48, 0x70, 0x98, 0xD8, 0x118 };
static const uint16 ORIGIN_HEIGHTS[PICTURE_ORIGIN_COUNT] = { 3, 7, 0xB, 0x14, 0x20, 0x2D, 0x3D };

// How long each of the screens of credit is shown for
constexpr uint32 PRESENTS_MS = 1000;
constexpr uint32 PAUSE_MS = 1100;

// Where the writing of Lord British starts, a part at a time, and how fast it goes
constexpr int TRACE_PARTS = 4;
static const byte TRACE_X[TRACE_PARTS] = { 0x2C, 0x40, 0x8F, 0xA7 };
static const byte TRACE_Y[TRACE_PARTS] = { 0x44, 0x5E, 0x4E, 0x69 };
constexpr int TRACE_STEPS_PER_PAUSE = 32;
constexpr uint32 TRACE_PAUSE_MS = 55;

// The logo of the game comes up a scatter of pixels at a time
constexpr int LOGO_WIDTH = 320;
constexpr int LOGO_HEIGHT = 101;
constexpr int LOGO_MS = 3000;

// So does the subtitle, first the flames and then the letters in them
constexpr int SUBTITLE_LEFT = 0x10;
constexpr int SUBTITLE_TOP = 0x41;
constexpr int SUBTITLE_WIDTH = 0x120;
constexpr int SUBTITLE_ROWS = 0x31;
constexpr int SUBTITLE_FRAME_STEPS = 0x100;
constexpr int SUBTITLE_MAX_ITERATIONS = 0x8000;
constexpr uint16 SUBTITLE_TAPS = 0x3500;
constexpr int FLAMES_MS = 2300;
constexpr int LETTERS_MS = 1900;
constexpr int CLICK_ODDS_START = 400;
constexpr int CLICK_ODDS_FALL = 3;
constexpr int CLICK_ODDS_RANGE = 512;

// What is done to the lines of the Origin logo to bring it together, passes of which are a header with where the
// lines come from and go to and how many there are, and a list of lines by when they appear, and then are cleared
static const byte ORIGIN_TABLE[] = {
	0x07,
	0x00, 0x4B, 0x03, 0x0B, 0xFF, 0xFF, 0xFF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x02, 0xFF,
	0x03, 0x48, 0x07, 0x0F, 0xFF, 0xFF, 0x01, 0x05, 0xFF, 0xFF, 0x02, 0x04, 0xFF, 0xFF, 0x03, 0xFF,
	0x00, 0x06, 0xFF,
	0x0A, 0x47, 0x0B, 0x13, 0xFF, 0xFF, 0x02, 0x08, 0xFF, 0x03, 0x07, 0xFF, 0x01, 0x09, 0xFF, 0x04,
	0x06, 0xFF, 0x05, 0xFF, 0x00, 0x0A, 0xFF,
	0x15, 0x42, 0x14, 0x1C, 0xFF, 0x04, 0x0F, 0xFF, 0x01, 0x07, 0x0C, 0x12, 0xFF, 0x05, 0x0E, 0xFF,
	0x02, 0x08, 0x0B, 0x11, 0xFF, 0x03, 0x06, 0x0D, 0x10, 0xFF, 0x09, 0x0A, 0xFF, 0x00, 0x13, 0xFF,
	0x29, 0x3C, 0x20, 0x28, 0xFF, 0x07, 0x18, 0xFF, 0x02, 0x0C, 0x13, 0x1D, 0xFF, 0x03, 0x08, 0x0D,
	0x12, 0x17, 0x1C, 0xFF, 0x01, 0x06, 0x0B, 0x14, 0x19, 0x1E, 0xFF, 0x04, 0x09, 0x0E, 0x11, 0x16,
	0x1B, 0xFF, 0x05, 0x0A, 0x0F, 0x10, 0x15, 0x1A, 0xFF, 0x00, 0x1F, 0xFF,
	0x49, 0x35, 0x2D, 0x35, 0xFF, 0x04, 0x0B, 0x12, 0x1A, 0x21, 0x28, 0xFF, 0x01, 0x08, 0x0F, 0x13,
	0x24, 0x2B, 0xFF, 0x06, 0x0D, 0x14, 0x18, 0x1F, 0x26, 0xFF, 0x03, 0x0A, 0x11, 0x16, 0x1B, 0x22,
	0x29, 0xFF, 0x02, 0x05, 0x09, 0x0C, 0x10, 0x13, 0x19, 0x1C, 0x20, 0x23, 0x27, 0x2A, 0xFF, 0x07,
	0x0E, 0x15, 0x17, 0x1E, 0x25, 0xFF, 0x00, 0x2C, 0xFF,
	0x76, 0x2E, 0x3D, 0x45, 0xFF, 0x1C, 0x17, 0x12, 0x0D, 0x08, 0x03, 0x20, 0x25, 0x2A, 0x2F, 0x34,
	0x39, 0xFF, 0x1A, 0x15, 0x10, 0x0B, 0x06, 0x01, 0x22, 0x27, 0x2C, 0x31, 0x36, 0x3B, 0xFF, 0x1D,
	0x18, 0x13, 0x0E, 0x09, 0x04, 0x1F, 0x24, 0x29, 0x2E, 0x33, 0x38, 0xFF, 0x1B, 0x16, 0x11, 0x0C,
	0x07, 0x02, 0x21, 0x26, 0x2B, 0x30, 0x35, 0x3A, 0xFF, 0x19, 0x0F, 0x05, 0x23, 0x2D, 0x37, 0xFF,
	0x1E, 0x28, 0x32, 0x14, 0x0A, 0xFF, 0x00, 0x3C, 0xFF
};

// Works out the steps that the Origin logo takes to come together
struct OriginBuilder {
	Common::Array<Title::OriginStep> &_steps;
	int _cursor = 0;
	int _listStart = 0;
	int _listLengthLess = 0;
	int _sourceBase = 0;
	int _targetBase = 0;
	int _lineCount = 0;
	byte _visible[128] = {};

	OriginBuilder(Common::Array<Title::OriginStep> &steps) : _steps(steps) {}

	byte readByte() {
		return _cursor >= (int)sizeof(ORIGIN_TABLE) ? 0 : ORIGIN_TABLE[_cursor++];
	}

	byte listAt(int index) const {
		return index < 0 || _listStart + index >= (int)sizeof(ORIGIN_TABLE) ? 0xFF : ORIGIN_TABLE[_listStart + index];
	}

	void readPass() {
		_sourceBase = readByte();
		_targetBase = readByte();
		_lineCount = readByte();

		const int length = readByte();
		_listLengthLess = length - 1;
		_listStart = _cursor;
		_cursor += length;
	}

	// Gives one picture of the logo, where the lines that are in are those drawn from the logo's picture
	void render(int direction) {
		int hidden = 0;
		int visible = 0;

		for (int i = 0; i < _lineCount; ++i) {
			if (_visible[i])
				++visible;
			else
				++hidden;
		}

		Title::OriginStep step;
		step._delay = 2 * (_lineCount / 3 + 15);

		int y = direction == 0 ? _targetBase : _targetBase + _lineCount;
		const int move = direction == 0 ? 1 : -1;
		int scan = 0;

		for (int i = 0; i < hidden / 2; ++i, y += move)
			step._ops.push_back({ y, -1 });

		for (int i = 0; i < visible; ++i) {
			while (scan < _lineCount && !_visible[scan])
				++scan;

			if (scan >= _lineCount)
				break;

			step._ops.push_back({ y, _sourceBase + scan });
			++scan;
			y += move;
		}

		++hidden;
		for (int i = 0; i < hidden / 2; ++i, y += move)
			step._ops.push_back({ y, -1 });

		_steps.push_back(step);
	}

	void appear(int direction) {
		memset(_visible, 0, sizeof(_visible));

		int pos = _listLengthLess - 1;
		int count = 6;

		while (count >= 0) {
			const byte value = listAt(pos--);

			if (value == 0xFF) {
				render(direction);
				--count;
			} else if (value < _lineCount) {
				_visible[value] = 1;
			}
		}
	}

	void vanish(int direction) {
		int pos = 1;
		int count = 6;

		while (count > 0) {
			const byte value = listAt(pos++);

			if (value == 0xFF) {
				render(direction);
				--count;
			} else if (value < _lineCount) {
				_visible[value] = 0;
			}
		}
	}

	void run() {
		int passes = readByte();
		int direction = 1;

		while (passes != 0) {
			readPass();
			direction ^= 1;
			appear(direction);

			if (--passes == 0)
				break;

			vanish(direction);
		}
	}
};

// The shift registers that give the order of the pixels, by how many bits they have
static const uint16 REVEAL_MASKS[15] = {
	3, 6, 0xC, 0x14, 0x30, 0x60, 0xB8, 0x110, 0x240, 0x500, 0xCA0, 0x1B00, 0x3500, 0x6000, 0xB400
};

// Gives the pixels of a picture in a shuffled order, by a shift register that goes through all the numbers but nought
static void shuffledOrder(Common::Array<uint32> &order, int total) {
	int bits = 2;
	uint32 maxState = 3;

	while ((int)maxState < total) {
		++bits;
		maxState = (1 << bits) - 1;
	}

	const uint16 mask = REVEAL_MASKS[bits - 2];
	uint16 state = 1;

	order.clear();
	for (uint32 guard = 0; (int)order.size() < total && guard <= maxState; ++guard) {
		const int index = state - 1;
		const bool carry = (state & 1) != 0;

		state >>= 1;
		if (carry)
			state ^= mask;

		if (index < total)
			order.push_back(index);
	}
}

bool Title::loadStartup() {
	Common::File f;

	if (!_titleBits.load("title.bit") || !_britishBits.load("british.bit") || !_wdMask.load("wd.bit", false) ||
			_titleBits.size() <= PICTURE_PRODUCTION || !f.open("british.pth"))
		return false;

	_path.resize(f.size());
	if (_path.empty() || f.read(&_path[0], _path.size()) != _path.size())
		return false;

	_screen.create(SCREEN_WIDTH, SCREEN_HEIGHT);
	_source.create(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}

void Title::buildOrigin() {
	// The pictures that make up the logo are one under the other, in the picture that the lines are taken from
	_source.fillRect(Common::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), COLOR_BLACK);

	int y = 0;
	for (int i = 0; i < PICTURE_ORIGIN_COUNT; ++i) {
		_titleBits.draw(_source, i, (SCREEN_WIDTH - ORIGIN_WIDTHS[i]) >> 1, y, COLOR_WHITE);
		y += ORIGIN_HEIGHTS[i];
	}

	_originSteps.clear();
	OriginBuilder(_originSteps).run();
}

void Title::startOrigin() {
	buildOrigin();
	_screen.fillRect(Common::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), COLOR_BLACK);

	_phase = PHASE_ORIGIN;
	_originStep = 0;
	_originNext = g_system->getMillis();
	g_engine->cueMusic(Music::TRACK_NONE);
}

void Title::advanceOrigin(uint32 now) {
	while (_originStep < _originSteps.size() && now >= _originNext) {
		const OriginStep &step = _originSteps[_originStep++];

		for (uint i = 0; i < step._ops.size(); ++i) {
			const int y = step._ops[i]._y;
			const int source = step._ops[i]._source;

			if (y < 0 || y >= SCREEN_HEIGHT)
				continue;

			byte *line = (byte *)_screen.getBasePtr(0, y);
			if (source < 0) {
				memset(line, 0, SCREEN_WIDTH);
			} else if (source < SCREEN_HEIGHT) {
				const byte *from = (const byte *)_source.getBasePtr(0, source);

				for (int x = 0; x < SCREEN_WIDTH; ++x)
					line[x] = from[x] & COLOR_LOGO_BLUE;
			}
		}

		_originNext += step._delay;
	}

	if (_originStep >= _originSteps.size() && now >= _originNext)
		startPresents(now);
}

void Title::startPresents(uint32 now) {
	_screen.fillRect(Common::Rect(0, 0x8C, SCREEN_WIDTH, SCREEN_HEIGHT), COLOR_BLACK);
	_titleBits.draw(_screen, PICTURE_PRESENTS, 0x6C, 0x8C, COLOR_WHITE);

	_phase = PHASE_PRESENTS;
	_phaseStart = now;
}

void Title::startA(uint32 now) {
	_screen.fillRect(Common::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), COLOR_BLACK);
	_titleBits.draw(_screen, PICTURE_A, 0x98, 0, COLOR_WHITE);

	_phase = PHASE_A;
	_phaseStart = now;
}

void Title::startTrace(uint32 now) {
	_phase = PHASE_TRACE;
	_segment = 0;
	_pathPos = 0;
	_penX = TRACE_X[0];
	_penY = TRACE_Y[0];
	_penColor = COLOR_WHITE;
	_traceTime = now;
}

void Title::advanceTrace(uint32 now) {
	int steps = (int)((now - _traceTime) * TRACE_STEPS_PER_PAUSE / TRACE_PAUSE_MS);
	_traceTime += steps * TRACE_PAUSE_MS / TRACE_STEPS_PER_PAUSE;

	for (; steps > 0 && _phase == PHASE_TRACE; --steps) {
		if (_pathPos >= (int)_path.size()) {
			startProduction(now);
			return;
		}

		// The pen leaves a mark where it was unless it was lifted by the move before
		if (_penColor != 0 && _penX >= 0 && _penX < SCREEN_WIDTH && _penY >= 0 && _penY < SCREEN_HEIGHT)
			*(byte *)_screen.getBasePtr(_penX, _penY) = _penColor;

		const byte move = _path[_pathPos];
		int dy = move & 7;
		int dx = (move >> 4) & 7;

		_penColor = COLOR_WHITE;
		if (dy > 2 || dx > 2)
			_penColor = 0;

		if (move & 8)
			dy = -dy;
		if (move & 0x80)
			dx = -dx;

		_penX += dx;
		_penY += dy;

		if (move == 0) {
			// The part is done, and the next starts where it is told to
			++_pathPos;

			if (++_segment >= TRACE_PARTS) {
				startProduction(now);
				return;
			}

			_penX = TRACE_X[_segment];
			_penY = TRACE_Y[_segment];
			_penColor = COLOR_WHITE;
		} else {
			++_pathPos;
		}
	}
}

void Title::startProduction(uint32 now) {
	_screen.fillRect(Common::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), COLOR_BLACK);
	_titleBits.draw(_screen, PICTURE_A, 0x98, 0, COLOR_WHITE);
	_britishBits.draw(_screen, 0, 0x18, 0x42, COLOR_WHITE);
	_titleBits.draw(_screen, PICTURE_PRODUCTION, 0x68, 0xA0, COLOR_WHITE);

	_phase = PHASE_PRODUCTION;
	_phaseStart = now;
}

void Title::startLogo(uint32 now) {
	shuffledOrder(_revealOrder, LOGO_WIDTH * LOGO_HEIGHT);
	_shown.clear();
	_shown.resize(LOGO_WIDTH * LOGO_HEIGHT);
	_revealed = 0;
	_phase = PHASE_LOGO;
	_phaseStart = now;

	g_engine->cueMusic(Music::TRACK_THEME);
	g_engine->sound().titleReveal(LOGO_MS);
}

void Title::advanceLogo(uint32 now) {
	const uint target = MIN<uint>(_revealOrder.size(), (uint)((now - _phaseStart) * _revealOrder.size() / LOGO_MS));

	for (; _revealed < target; ++_revealed)
		_shown[_revealOrder[_revealed]] = 1;

	if (_revealed >= _revealOrder.size())
		startSubtitle(now, false);
}

void Title::startSubtitle(uint32 now, bool second) {
	if (!second) {
		_shown.clear();
		_shown.resize(SUBTITLE_WIDTH * SUBTITLE_ROWS);
		_subtitleFrame = 0;
		_clickOdds = CLICK_ODDS_START;
	}

	_phase = second ? PHASE_SUBTITLE_SECOND : PHASE_SUBTITLE_FIRST;
	_phaseStart = now;
	_wdSeed = 1;
	_wdIterations = 0;
	_wdCountdown = SUBTITLE_FRAME_STEPS;
	_wdPlotted = 0;

	// What each of the passes has to show is spread over the time that it takes
	_wdTotal = 0;
	for (int row = 0; row < SUBTITLE_ROWS; ++row) {
		for (int x = 0; x < SUBTITLE_WIDTH; ++x) {
			if (_wdMask.isSet(0, x, row) == second)
				++_wdTotal;
		}
	}
}

void Title::advanceSubtitle(uint32 now) {
	const bool second = _phase == PHASE_SUBTITLE_SECOND;
	const int target = (int)((uint64)(now - _phaseStart) * _wdTotal / (second ? LETTERS_MS : FLAMES_MS));
	bool finished = false;

	while (_wdPlotted < target && !finished) {
		const int row = _wdSeed / SUBTITLE_WIDTH;
		const int x = _wdSeed % SUBTITLE_WIDTH;

		// The flames are the pixels that the picture of the letters doesn't have set, and the letters are those it has
		if (row < SUBTITLE_ROWS && _wdMask.isSet(0, x, row) == second) {
			_shown[row * SUBTITLE_WIDTH + x] = 1;
			++_wdPlotted;
		}

		if (--_wdCountdown == 0) {
			// The flames flicker, and sometimes make a click
			_subtitleFrame = (_subtitleFrame + 1) & 3;
			_wdCountdown = SUBTITLE_FRAME_STEPS;

			if (g_engine->getRandomNumber(CLICK_ODDS_RANGE - 1) < MAX<int>(_clickOdds, 0))
				g_engine->sound().noise(1, 25, 1500);

			_clickOdds -= CLICK_ODDS_FALL;
		}

		const bool carry = (_wdSeed & 1) != 0;
		_wdSeed >>= 1;
		if (carry)
			_wdSeed ^= SUBTITLE_TAPS;

		if (_wdSeed == 1 || ++_wdIterations >= SUBTITLE_MAX_ITERATIONS)
			finished = true;
	}

	if (!finished)
		return;

	if (second) {
		finishIntro(false);
	} else {
		startSubtitle(now, true);
	}
}

void Title::finishIntro(bool skipped) {
	_phase = PHASE_MENU;
	_lastKey = g_system->getMillis();
	_shown.clear();

	g_engine->sound().stop();
	g_engine->cueMusic(Music::TRACK_THEME);

	// The view of the world plays until a key is pressed, unless the introduction was cut short
	if (!skipped)
		addView("TitleDemo");
}

bool Title::msgFocus(const FocusMessage &msg) {
	if (!loadTitleGraphics())
		error("Could not load the title graphics");

	_selected = 0;
	_animFrame = 0;
	_showNotice = false;

	if (!_started) {
		_started = true;

		if (loadStartup()) {
			startOrigin();
		} else {
			// Without the pictures of the start it is the menu that is shown
			_phase = PHASE_MENU;
		}
	}

	if (_phase == PHASE_MENU) {
		g_engine->cueMusic(Music::TRACK_THEME);
		_lastKey = g_system->getMillis();
	}

	delayFrames(_phase == PHASE_MENU ? FRAMES_PER_STEP : 1);
	return TitleBase::msgFocus(msg);
}

void Title::drawStartup() {
	auto s = getSurface();

	for (int y = 0; y < SCREEN_HEIGHT; ++y)
		memcpy(s.getBasePtr(0, y), _screen.getBasePtr(0, y), SCREEN_WIDTH);

	// The pixels were written directly, so the surface has to be told that it all changed
	s.markAllDirty();
}

void Title::drawReveal() {
	const bool subtitle = _phase != PHASE_LOGO;

	drawLogo();
	if (subtitle)
		drawSubtitle(_subtitleFrame);

	auto s = getSurface();
	const int width = subtitle ? SUBTITLE_WIDTH : LOGO_WIDTH;
	const int left = subtitle ? SUBTITLE_LEFT : 0;
	const int top = subtitle ? SUBTITLE_TOP : 0;

	if (s.format.bytesPerPixel != 1)
		return;

	for (uint i = 0; i < _shown.size(); ++i) {
		if (!_shown[i])
			*(byte *)s.getBasePtr(left + i % width, top + i / width) = COLOR_BLACK;
	}

	s.markAllDirty();
}

void Title::draw() {
	switch (_phase) {
	case PHASE_ORIGIN:
	case PHASE_PRESENTS:
	case PHASE_A:
	case PHASE_TRACE:
	case PHASE_PRODUCTION:
		drawStartup();
		return;

	case PHASE_LOGO:
	case PHASE_SUBTITLE_FIRST:
	case PHASE_SUBTITLE_SECOND:
		drawReveal();
		return;

	case PHASE_MENU:
		break;
	}

	drawTitle(_animFrame);

	if (_showNotice) {
		drawMessage("No active game.", "Please create a character or transfer", "one from Ultima IV.");
	} else {
		drawMenu(_selected);
		drawPrompt(_animFrame);
	}
}

void Title::timeout() {
	const uint32 now = g_system->getMillis();

	switch (_phase) {
	case PHASE_ORIGIN:
		advanceOrigin(now);
		break;
	case PHASE_PRESENTS:
		if (now - _phaseStart >= PRESENTS_MS + PAUSE_MS)
			startA(now);
		break;
	case PHASE_A:
		if (now - _phaseStart >= PAUSE_MS)
			startTrace(now);
		break;
	case PHASE_TRACE:
		advanceTrace(now);
		break;
	case PHASE_PRODUCTION:
		if (now - _phaseStart >= PAUSE_MS)
			startLogo(now);
		break;
	case PHASE_LOGO:
		advanceLogo(now);
		break;
	case PHASE_SUBTITLE_FIRST:
	case PHASE_SUBTITLE_SECOND:
		advanceSubtitle(now);
		break;
	case PHASE_MENU:
		_animFrame = (_animFrame + 1) & 3;

		// Left alone the menu goes back to showing the view
		if (!_showNotice && now - _lastKey >= IDLE_MS) {
			_lastKey = now;
			addView("TitleDemo");
		}

		redraw();
		delayFrames(FRAMES_PER_STEP);
		return;
	}

	redraw();
	delayFrames(1);
}

void Title::choose(int index, bool activate) {
	_selected = index;
	redraw();

	if (activate)
		this->activate(index);
}

void Title::activate(int index) {
	switch (index) {
	case CHOICE_JOURNEY:
		if (_G(savegame).hasActiveGame()) {
			replaceView("Game", true);
		} else {
			_showNotice = true;
			redraw();
		}
		break;
	case CHOICE_CREATE:
		addView("CreateCharacter");
		break;
	case CHOICE_TRANSFER:
		addView("TransferCharacter");
		break;
	case CHOICE_INTRODUCTION:
		addView("Introduction");
		break;
	case CHOICE_ACKNOWLEDGEMENTS:
		addView("Acknowledgements");
		break;
	case CHOICE_VIEW:
		addView("TitleDemo");
		break;
	default:
		break;
	}
}

bool Title::msgKeypress(const KeypressMessage &msg) {
	if (isModifierKey(msg.keycode))
		return false;

	_lastKey = g_system->getMillis();

	switch (_phase) {
	case PHASE_ORIGIN:
	case PHASE_PRESENTS:
	case PHASE_A:
	case PHASE_TRACE:
	case PHASE_PRODUCTION:
		// A key cuts the start short and goes to the menu
		finishIntro(true);
		redraw();
		return true;

	case PHASE_LOGO:
	case PHASE_SUBTITLE_FIRST:
	case PHASE_SUBTITLE_SECOND:
		// It shows the logo and subtitle at once, and then there is the view
		finishIntro(false);
		redraw();
		return true;

	case PHASE_MENU:
		break;
	}

	if (_showNotice) {
		_showNotice = false;
		redraw();
		return true;
	}

	switch (msg.keycode) {
	case Common::KEYCODE_LEFT:
	case Common::KEYCODE_UP:
		choose((_selected + MENU_ITEM_COUNT - 1) % MENU_ITEM_COUNT, false);
		return true;
	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_DOWN:
		choose((_selected + 1) % MENU_ITEM_COUNT, false);
		return true;
	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
	case Common::KEYCODE_SPACE:
		activate(_selected);
		return true;
	default:
		break;
	}

	const char letter = toupper(msg.ascii);
	for (int i = 0; MENU_LETTERS[i]; ++i) {
		if (letter == MENU_LETTERS[i]) {
			choose(i, true);
			return true;
		}
	}

	return false;
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
