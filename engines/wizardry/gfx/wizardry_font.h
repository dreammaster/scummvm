
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

#ifndef WIZARDRY_GFX_WIZARDRY_FONT_H
#define WIZARDRY_GFX_WIZARDRY_FONT_H

#include "common/file.h"
#include "graphics/font.h"
#include "graphics/surface.h"

namespace Wizardry {

constexpr int CHAR_SIZE = 8;

class WizardryFont : public Graphics::Font {
private:
	const byte *_data;

public:
	WizardryFont(const byte *data) : _data(data) {
	}
	virtual ~WizardryFont() {}

	int getFontHeight() const override {
		return CHAR_SIZE;
	}
	int getMaxCharWidth() const override {
		return CHAR_SIZE;
	}
	int getCharWidth(uint32 chr) const override {
		return CHAR_SIZE;
	}
	void drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const override;
};

class WizardryFontV1 : public WizardryFont {
private:
	byte _data[64 * 8] = {};

public:
	WizardryFontV1(const byte *data);
	WizardryFontV1(Common::SeekableReadStream &src);
	~WizardryFontV1() override {}

};

class WizardryFontNormal : public WizardryFont {
public:
	WizardryFontNormal();
	~WizardryFontNormal() override {
	}
};

class WizardryFontGfx : public WizardryFont {
public:
	WizardryFontGfx();
	~WizardryFontGfx() override {
	}
};

} // namespace Wizardry

#endif
