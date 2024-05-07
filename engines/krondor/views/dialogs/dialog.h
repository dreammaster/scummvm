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

#ifndef KRONDOR_VIEWS_DIALOGS_DIALOG_H
#define KRONDOR_VIEWS_DIALOGS_DIALOG_H

#include "krondor/events.h"
#include "krondor/res/resources.h"
#include "krondor/gfx/widgets/widget.h"

namespace Krondor {
namespace Views {
namespace Dialogs {

class Dialog : public UIElement {
private:
	Common::String _requestName;
	Common::String _paletteName;
	Common::String _screenName;
	Common::String _normalName;
	Common::String _fontName;
	Common::String _labelName;

protected:
	RequestResource _request;
	PaletteResource _palette;
	ScreenResource _screen;
	ImageResource _normal;
	FontResource _font;
	LabelResource _label;

protected:
	/**
	 * Creates a widget based on a request data entry
	 */
	virtual Gfx::Widgets::Widget *createWidget(const RequestData *reqData);

public:
	Dialog(const Common::String &name,
		const Common::String &requestName, const Common::String &paletteName,
		const Common::String &screenName, const Common::String &normalName,
		const Common::String &fontName, const Common::String &labelName) :
		UIElement(name), _requestName(requestName), _paletteName(paletteName),
		_screenName(screenName), _normalName(normalName),
		_fontName(fontName), _labelName(labelName) {
	}
	virtual ~Dialog() {
	}

	bool msgFocus(const FocusMessage &msg) override;
	bool msgUnfocus(const UnfocusMessage &msg) override;
	void draw() override;
};

} // namespace Dialogs
} // namespace Views
} // namespace Krondor

#endif
