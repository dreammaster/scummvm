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

#ifndef WASTELAND_WASTELAND1_VIEWS_CHARACTER_REORDER_PANE_H
#define WASTELAND_WASTELAND1_VIEWS_CHARACTER_REORDER_PANE_H

#include "common/array.h"
#include "wasteland/wasteland1/core/array1.h"
#include "wasteland/wasteland1/views/gfx/paged_pane.h"
#include "wasteland/wasteland1/views/gfx/text_button.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Character {

class ReorderPane : public Gfx::PagedPane {
	struct Entry : public Common::String {
		int _index = 0;
		Entry() : Common::String() {}
		Entry(const Common::String &str, int index) :
			Common::String(str), _index(index) {}
	};
private:
	Gfx::TextButton _esc;
	Gfx::TextButton _reorder;
	Gfx::TextButton _next;
	Array1<Entry> _lines;
	bool _reordering = false;

	void setReordering(bool flag);

protected:
	Array1<int> _newOrder;

	virtual void reordered() = 0;

protected:
	void clearLines();
	void addLine(const Common::String &str);
	virtual void resetLines() {}

public:
	ReorderPane(const Common::String &name);
	virtual ~ReorderPane() {}

	void draw() override;
	bool msgFocus(const FocusMessage &msg) override;
	bool msgGame(const GameMessage &msg) override;
};

} // namespace Character
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland

#endif
