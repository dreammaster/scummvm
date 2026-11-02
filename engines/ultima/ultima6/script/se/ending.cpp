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

#include "ultima/ultima6/script/common/cutscene_script.h"
#include "ultima/ultima6/script/se/se_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace SE {

namespace {

class Ending : public CutsceneScript {
private:
	const Lang &_lang;
	CSSprite *_text = nullptr;

	void showText(const char *code) {
		_text->text = _lang.get(code);
		waitForInput();
	}

public:
	explicit Ending(const Lang &lang) : _lang(lang) {}

	void play();
};

void Ending::play() {
	// Image sets: 0 party, 1 Lord British, 2 Aiela, 3 Tristia, 4 leaving Savage Empire
	ImageTable images;
	if (!loadImages("endgame.lzc", images))
		return;

	CSSprite *bg = newSprite(images[0][0], 0, 0, true);
	_cutscene->load_palette("savage.pal", 1);
	_cutscene->set_screen_opacity(0xff);
	showMouseCursor(false);
	_cutscene->set_update_interval(25);

	_text = newSprite(nullptr, 28, 135, true);
	_text->text_color = 0;
	_text->text = _lang.get("IMG1_TXT1_END");
	_text->text_align = 1;
	fadeIn();
	waitForInput();
	showText("IMG1_TXT2_END");
	showText("IMG1_TXT3_END");

	bg->image = images[0][1];
	_text->text = _lang.get("IMG2_TXT1_END");
	waitForInput();
	showText("IMG2_TXT2_END");

	bg->image = images[0][2];
	_text->text = _lang.get("IMG3_TXT1_END");
	waitForInput();
	showText("IMG3_TXT2_END");
	showText("IMG3_TXT3_END");

	bg->image = images[0][3];
	_text->text = _lang.get("IMG4_TXT1_END");
	waitForInput();
	showText("IMG4_TXT2_END");

	bg->image = images[0][4];
	_text->text = _lang.get("IMG5_TXT1_END");
	waitForInput();
	showText("IMG5_TXT2_END");
	showText("IMG5_TXT3_END");
	showText("IMG5_TXT4_END");

	bg->image = images[0][1];
	showText("IMG2_TXT3_END");
}

} // End of anonymous namespace

bool SEScripts::playEnding() {
	Ending ending(_lang);
	ending.play();
	return true;
}

} // End of namespace SE
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
