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

#ifndef WASTELAND_WASTELAND1_VIEWS_TITLE_TITLE_H
#define WASTELAND_WASTELAND1_VIEWS_TITLE_TITLE_H

#include "graphics/managed_surface.h"
#include "wasteland/gfx/text_view.h"
#include "wasteland/wasteland1/views/dialogs/dialog.h"
#include "wasteland/wasteland1/views/gfx/text_button.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Title {

class Title : public Dialogs::Dialog {
private:
	Graphics::ManagedSurface _surface;
	Shared::Gfx::TextView _textView;
	Gfx::TextButton _start;
	int _textNum = 0;

	void setText();

public:
	Title();
	virtual ~Title() {}

	bool msgAction(const ActionMessage &msg) override;
	bool msgGame(const GameMessage &msg) override;
	bool msgFocus(const FocusMessage &msg) override;
	bool msgUnfocus(const UnfocusMessage &msg) override;
	void draw() override;
	void timeout() override;
};

} // namespace Title
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland

#endif
