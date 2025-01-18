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

#ifndef WASTELAND_WASTELAND1_VIEWS_CHARACTER_PAGED_PANE_H
#define WASTELAND_WASTELAND1_VIEWS_CHARACTER_PAGED_PANE_H

#include "common/str-array.h"
#include "wasteland/wasteland1/views/gfx/pane.h"
#include "wasteland/wasteland1/views/gfx/button.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

#define PAGED_Y_START 2
#define PAGED_LINES 9

class PagedPane : public Pane {
private:
	Button _upArrow;
	Button _downArrow;
	Common::StringArray _text;
	int _pageNum = 0;

protected:
	void clearText();
	void addText(const Common::String &str);

public:
	PagedPane(const Common::String &name);
	virtual ~PagedPane() {}

	void draw() override;
	bool msgGame(const GameMessage &msg) override;
};

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland

#endif
