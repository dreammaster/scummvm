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
#include "engines/engine.h"
#include "ultima/ultima6/core/nuvie_defs.h"
#include "ultima/ultima6/core/cursor.h"
#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/screen/screen.h"
#include "ultima/ultima6/script/common/cutscene_script.h"
#include "ultima/ultima6/sound/sound_manager.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {

// Highest frame index for each of the nine musicians
static const int PLAYER_FRAME_MAX[9] = { 12, 8, 2, 6, 4, 2, 4, 4, 3 };

CutsceneScript::CutsceneScript() : _cutscene(get_cutscene()), _shouldExit(false) {
}

CutsceneScript::~CutsceneScript() {
	for (uint i = 0; i < _sprites.size(); ++i) {
		_cutscene->remove_sprite(_sprites[i]);
		delete _sprites[i];
	}

	for (uint i = 0; i < _images.size(); ++i) {
		delete _images[i]->orig_shp;
		delete _images[i]->scaled_shp;
		delete _images[i];
	}
}

CSSprite *CutsceneScript::newSprite(CSImage *image, int x, int y, bool visible) {
	CSSprite *sprite = new CSSprite();
	sprite->image = image;
	sprite->x = x;
	sprite->y = y;
	sprite->visible = visible;

	_cutscene->add_sprite(sprite);
	_sprites.push_back(sprite);
	return sprite;
}

void CutsceneScript::moveToFront(CSSprite *sprite) {
	_cutscene->remove_sprite(sprite);
	_cutscene->add_sprite(sprite);
}

void CutsceneScript::setOpacity(CSSprite *sprite, int opacity) {
	sprite->opacity = (uint8)CLIP(opacity, 0, 255);
}

void CutsceneScript::setClip(CSSprite *sprite, int x, int y, int w, int h) {
	int left = x + _cutscene->get_x_off();
	int top = y + _cutscene->get_y_off();
	sprite->clip_rect = Common::Rect(left, top, left + w, top + h);
}

CSImage *CutsceneScript::newImage(uint16 w, uint16 h, int fill) {
	U6Shape *shape = new U6Shape();
	if (!shape->init(w, h)) {
		delete shape;
		return nullptr;
	}

	if (fill >= 0)
		shape->fill((uint8)fill);

	CSImage *image = new CSImage(shape);
	_images.push_back(image);
	return image;
}

CSImage *CutsceneScript::newStarField(uint16 w, uint16 h) {
	U6Shape *shape = new U6Shape();
	if (!shape->init(w, h)) {
		delete shape;
		return nullptr;
	}

	CSImage *image = new CSStarFieldImage(shape);
	_images.push_back(image);
	return image;
}

void CutsceneScript::setTransparency(CSImage *image, uint8 colour) {
	unsigned char *data = image->shp->get_data();
	uint16 w, h;
	image->shp->get_size(&w, &h);

	for (int i = 0; i < w * h; ++i) {
		if (data[i] == colour)
			data[i] = 0xff;
	}
}

void CutsceneScript::printText(CSImage *image, const char *text, uint16 startX, uint16 width, uint16 x, uint16 y,
		uint8 colour) {
	_cutscene->print_text(image, text, &x, &y, startX, width, colour);
}

void CutsceneScript::printWrapped(CSImage *image, const char *text, uint16 startX, uint16 width, uint16 &x, uint16 &y,
		uint8 colour) {
	_cutscene->print_text(image, text, &x, &y, startX, width, colour);
}

CSImage *CutsceneScript::copyImage(const CSImage *image) {
	uint16 w, h;
	image->shp->get_size(&w, &h);

	CSImage *copy = newImage(w, h);
	if (copy)
		copy->shp->blit(image->shp, 0, 0);

	return copy;
}

bool CutsceneScript::loadImages(const char *filename, ImageTable &images) {
	images = _cutscene->load_all_images(filename);
	if (images.empty())
		return false;

	for (uint i = 0; i < images.size(); ++i)
		adoptImages(images[i]);

	return true;
}

CSImage *CutsceneScript::loadImage(const char *filename, int idx) {
	CSImage *image = _cutscene->load_image(filename, idx);
	if (image)
		_images.push_back(image);

	return image;
}

void CutsceneScript::adoptImages(const Common::Array<CSImage *> &images) {
	for (uint i = 0; i < images.size(); ++i)
		_images.push_back(images[i]);
}

uint16 CutsceneScript::imageWidth(const CSImage *image) {
	uint16 w, h;
	image->shp->get_size(&w, &h);
	return w;
}

void CutsceneScript::blit(CSImage *dest, const CSImage *src, uint16 x, uint16 y) {
	dest->shp->blit(src->shp, x, y);
}

int CutsceneScript::mouseX() const {
	int x, y;
	_cutscene->get_screen()->get_mouse_location(&x, &y);
	return x - _cutscene->get_x_off();
}

int CutsceneScript::mouseY() const {
	int x, y;
	_cutscene->get_screen()->get_mouse_location(&x, &y);
	return y - _cutscene->get_y_off();
}

void CutsceneScript::showMouseCursor(bool show) {
	Cursor *cursor = Game::get_game()->get_cursor();
	if (!cursor)
		return;

	if (show)
		cursor->show();
	else
		cursor->hide();
}

bool CutsceneScript::engineShouldQuit() {
	return g_engine->shouldQuit();
}

void CutsceneScript::playSfx(uint16 sfxId, bool async) {
	_cutscene->get_sound_manager()->playSfx(sfxId, async);
}

void CutsceneScript::musicPlay(const char *filename, uint16 songNum) {
	_cutscene->get_sound_manager()->musicPlay(filename, songNum);
}

void CutsceneScript::musicStop() {
	_cutscene->get_sound_manager()->musicStop();
}

void CutsceneScript::canvasUpdate() {
	_cutscene->update();
}

int CutsceneScript::inputPoll(bool pollMouseMotion) {
	return _cutscene->input_poll(pollMouseMotion);
}

int CutsceneScript::waitForInput() {
	int input;

	do {
		canvasUpdate();
		input = inputPoll();
	} while (input == INPUT_NONE);

	return input;
}

bool CutsceneScript::pollForKeyOrButton(int cycles) {
	if (engineShouldQuit()) {
		_shouldExit = true;
		return true;
	}

	if (cycles < 0) {
		int input = inputPoll();
		if (input != INPUT_NONE) {
			if (input == Common::KEYCODE_ESCAPE)
				_shouldExit = true;
			return true;
		}

		return false;
	}

	for (int i = 0; i <= cycles; ++i) {
		int input = inputPoll();
		if (input != INPUT_NONE) {
			if (input == Common::KEYCODE_ESCAPE)
				_shouldExit = true;
			return true;
		}

		if (engineShouldQuit()) {
			_shouldExit = true;
			return true;
		}

		canvasUpdate();
	}

	return false;
}

bool CutsceneScript::pollForEsc(int cycles) {
	if (cycles < 0) {
		int input = inputPoll();
		return input == Common::KEYCODE_ESCAPE || engineShouldQuit();
	}

	for (int i = 0; i <= cycles; ++i) {
		if (inputPoll() == Common::KEYCODE_ESCAPE || engineShouldQuit())
			return true;

		canvasUpdate();
	}

	return false;
}

int CutsceneScript::pollForInput() {
	int input = inputPoll();

	if (input == Common::KEYCODE_ESCAPE)
		_shouldExit = true;

	if (engineShouldQuit())
		_shouldExit = true;

	return input;
}

char CutsceneScript::keyToChar(int key) {
	if (key == ' ' || key == '\'' || (key >= ',' && key <= '.') || (key >= '0' && key <= '9') ||
			(key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z'))
		return (char)key;

	return 0;
}

void CutsceneScript::drawLine(CSImage *image, int startX, int startY, int endX, int endY, uint8 color) {
	image->shp->draw_line(startX, startY, endX, endY, color);
}

void CutsceneScript::setMousePointer(uint8 pointer) {
	Cursor *cursor = Game::get_game()->get_cursor();
	if (cursor)
		cursor->set_pointer(pointer);
}

void CutsceneScript::fadeIn(int speed) {
	for (int i = 0; i <= 0xff; i += speed) {
		_cutscene->set_screen_opacity(i);
		canvasUpdate();

		if (engineShouldQuit())
			return;
	}
}

void CutsceneScript::fadeOut(int speed) {
	for (int i = 0xff; i >= 0; i -= speed) {
		_cutscene->set_screen_opacity(i);
		canvasUpdate();

		if (engineShouldQuit())
			return;
	}
}

void CutsceneScript::updatePlayers(CSSprite *const *players, const ImageTable &images) {
	for (uint i = 0; i < PLAYER_COUNT; ++i)
		players[i]->image = images[i + 1][randomRange(0, PLAYER_FRAME_MAX[i])];
}

CSSprite *CutsceneScript::createPlayerSprite(CSImage *image, int x, int y) {
	CSSprite *sprite = newSprite(image, x, y, true);
	setClip(sprite, 0, 0, 320, 152);
	return sprite;
}

CSSprite *CutsceneScript::createSprite(CSImage *image, int x, int y) {
	CSSprite *sprite = newSprite(image, x, y, true);
	setClip(sprite, 0, 24, 320, 128);
	return sprite;
}

CutsceneScript::Firework CutsceneScript::createFirework(const ImageTable &images) {
	Firework firework;
	firework._colour = randomRange(0, 2);
	int x = randomRange(0, 319);
	int y = randomRange(0, 127);
	firework._sprite = createSprite(images[11][8 * firework._colour], x, y);
	firework._frame = 0;
	return firework;
}

int CutsceneScript::updateFireworks(Common::Array<Firework> &fireworks, const ImageTable &images) {
	int finished = 0;

	// Skips the entry after a removed one, as the original table iteration did
	for (uint i = 0; i < fireworks.size(); ++i) {
		Firework &firework = fireworks[i];

		if (firework._frame == 7) {
			firework._sprite->visible = false;
			fireworks.remove_at(i);
			++finished;
		} else {
			++firework._frame;
			firework._sprite->image = images[11][8 * firework._colour + firework._frame];
		}
	}

	return finished;
}

void CutsceneScript::fireworks(const ImageTable &images, CSSprite *logo) {
	Common::Array<Firework> active;
	int count = 0;

	for (int i = 0; i <= 125; ++i) {
		if (count < 5 && randomRange(0, 5) == 0) {
			active.push_back(createFirework(images));
			++count;
			moveToFront(logo);
			playSfx(12);
		}

		count -= updateFireworks(active, images);
		pollForEsc(1);
	}

	while (count > 0) {
		count -= updateFireworks(active, images);
		pollForEsc(1);
	}

	pollForEsc(10);

	for (int i = 0; i < 30; ++i) {
		active.push_back(createFirework(images));
		++count;
	}

	while (count > 0) {
		count -= updateFireworks(active, images);
		pollForEsc(1);
	}
}

void CutsceneScript::aboutMartianDreams() {
	const int LINES = 82;

	_cutscene->hide_sprites();
	newSprite(loadImage("mars.lzc", 0), 0, 24, true);

	Common::Array<Common::String> text = _cutscene->load_text("scenetxt.lzc", 4);
	musicPlay("mdd_mus.lzc", 8);

	// Each line has a shadow beneath it
	CSSprite *lines[LINES * 2];
	for (int i = 0; i < LINES; ++i) {
		Common::String line = (uint)i < text.size() ? text[i] : Common::String();

		lines[i * 2] = newSprite(nullptr, 11, 153 + i * 14, true);
		lines[i * 2]->text_color = 6;
		lines[i * 2]->text = line;

		lines[i * 2 + 1] = newSprite(nullptr, 12, 152 + i * 14, true);
		lines[i * 2 + 1]->text_color = 14;
		lines[i * 2 + 1]->text = line;
	}

	// Black bars at the top and bottom of the screen hide the text as it scrolls in and out
	newSprite(newImage(220, 24, 0), 0, 0, true);
	newSprite(newImage(220, 48, 0), 0, 152, true);

	for (int i = 0; i <= 90 * 14; ++i) {
		for (int j = 0; j < LINES * 2; ++j)
			lines[j]->y -= 1;

		pollForKeyOrButton(2);
		if (_shouldExit) {
			fadeOut();
			return;
		}
	}

	musicStop();
	fadeOut();
}

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
