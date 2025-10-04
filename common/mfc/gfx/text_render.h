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

#ifndef COMMON_MFC_GFX_TEXT_RENDER_H
#define COMMON_MFC_GFX_TEXT_RENDER_H

#include "common/str-array.h"
#include "graphics/font.h"
#include "common/mfc/gfx/surface.h"
#include "common/mfc/minwindef.h"
#include "common/mfc/atltypes.h"
#include "common/mfc/wingdi.h"

namespace Common {
namespace MFC {
namespace Gfx {

class TextRender {
protected:
	CSize renderText(const Common::String &str,
		Gfx::Surface *dest, Graphics::Font *font,
		uint textCol, LPCRECT lpRect, unsigned int nFormat,
		const Common::Array<int> &tabStops,
		int nTabOrigin, uint bkColor, int bkMode,
		uint textColor, uint textAlign);

	void wordWrapText(Graphics::Font *font, const Common::String &str,
		const Common::Array<int> tabStops,
		int maxWidth, Common::StringArray &lines);
	int getStringWidth(Graphics::Font *font, const Common::String &str);
};

} // namespace Gfx
} // namespace MFC
} // namespace Common

#endif
