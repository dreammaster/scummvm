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

#include "legend/early/gfx/text_window.h"
#include "common/savefile.h"

namespace Legend {
namespace Early {
namespace Gfx {

#define MAX_LINE_WIDTH 80

TextWindow::~TextWindow() {
	delete _logFile;
}

void TextWindow::addLines(const String &msg) {
	const char *lineP = msg.c_str();

	for (int lineEnd = 0; lineP[lineEnd] || lineEnd > 0; ++lineEnd) {
		// Handle outputing lines
		char c = lineP[lineEnd];
		if (lineEnd >= (MAX_LINE_WIDTH - 1) || !c) {
			// Scan backwards to find an appropriate line break point
			for (int splitPos = lineEnd; splitPos > 0; --splitPos) {
				if (lineP[splitPos] == ' ' || lineP[splitPos] == '-') {
					lineEnd = splitPos;
					break;
				}
			}

			String line(lineP, lineP + lineEnd + 1);
			line += '\n';
			add(line);

			lineP += lineEnd + 1;
			lineEnd = -1;

			// If already at end of msg, then break
			if (!c)
				break;
		}
	}
}

void TextWindow::add(const String &msg) {
	// If logging is enabled, write out the text
	if (_logFile)
		_logFile->writeString(msg);

	// Form the text to add
	String text;
	if (_cached) {
		text += _cachedText;
		_cachedText = "";
	}
	text += msg;

}

void TextWindow::newLine() {
	int xp = _bounds.left + _font->_xCenter;
}

void TextWindow::draw() {
}

bool TextWindow::msgText(const TextMessage &msg) {
	if (msg._wrapped)
		addLines(msg._text);
	else
		add(msg._text);

	return true;
}

} // namespace Gfx
} // namespace Early
} // namespace Legend
