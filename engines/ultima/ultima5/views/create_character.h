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

#ifndef ULTIMA5_VIEWS_CREATE_CHARACTER_H
#define ULTIMA5_VIEWS_CREATE_CHARACTER_H

#include "common/array.h"
#include "common/str.h"
#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/gfx/image_file.h"
#include "ultima/ultima5/gfx/story_text.h"
#include "ultima/ultima5/logic/creation_logic.h"
#include "ultima/ultima5/views/title_base.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

/**
 * Creates the Avatar. The player gives a name and a gender, and then the gypsy
 * asks a series of questions, whose answers decide the attributes
 */
class CreateCharacter : public TitleBase {
private:
	enum Phase { PHASE_NAME, PHASE_GENDER, PHASE_INTRO, PHASE_QUESTION, PHASE_RESULT };

	Gfx::ImageFile _images;
	Gfx::StoryText _text;
	Common::Array<byte> _questions;

	Data::Savegame _newGame;
	CreationLogic _logic;
	GypsyQuestion _question;
	Phase _phase = PHASE_NAME;
	Common::String _name;
	int _cursorFrame = 0;

	/**
	 * Returns the text at an offset in question.dat
	 */
	Common::String questionText(uint offset) const;

	void drawNameScreen();
	void drawIntroScreen();
	void drawQuestionScreen();
	void drawResultScreen();

	void showQuestion();

	/**
	 * Adds the new character to the game
	 */
	void finish();

	bool nameKeypress(const KeypressMessage &msg);
	bool genderKeypress(const KeypressMessage &msg);
	bool questionKeypress(const KeypressMessage &msg);

public:
	CreateCharacter() : TitleBase("CreateCharacter") {}
	~CreateCharacter() override {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	void timeout() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
