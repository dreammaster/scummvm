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

void TextWindow::clear() {
	assert(hasFont());

	// Reset the lines array
	int numLines = _innerBounds.height() / _font->_lineHeight;
	_lines.clear();
	_lines.resize(numLines);

	// Reset row variables
	_lineNum = 0;
	_linesRemaining = numLines - 1;
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
	if (_hasMore) {
		// Waiting for user, so cache further adds
		_pendingAdds.push_back(msg);
		return;
	}

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

	while (!text.empty()) {
		size_t endPos = text.findFirstOf(" \t\n-");
		if (endPos == String::npos && _cached)
			break;

		if (text[endPos] == '-')
			// Dashes are included on line
			++endPos;

		String word(text.c_str(), text.c_str() + endPos);
		size_t wordWidth = _font->stringWidth(word);


	}

	for (const char *msgP = text.c_str(); *msgP;) {
//		const char *endP = 
//		for (endP = msgP; *endP != )
	}
}

void TextWindow::newLine() {
	if (_lineNum < ((int)_lines.size() - 1)) {
		// Move to next line
		++_lineNum;
	} else {
		// On last line already, so scroll text
		_lines.remove_at(0);
		_lines.push_back(String());
	}

	if (--_linesRemaining <= 0) {
		// Flag for displaying has more indicator
		_hasMore = true;

		// Until the Has More is dismissed, the text window will
		// have complete focus to get key or mouse events
		g_events->addView(this);
	}

	needsRedraw();
}

void TextWindow::flush() {
	if (!_pendingAdds.empty()) {
		Common::Array<String> adds = _pendingAdds;
		_pendingAdds.clear();

		for (uint i = 0; i < adds.size(); ++i)
			add(adds[i]);
	}
}

void TextWindow::draw() {
	BoxedElement::draw();

	for (uint i = 0; i < _lines.size(); ++i) {
		Common::Point pt(_innerBounds.left,
			_innerBounds.top + _font->_lineHeight * i);

		if (i == (_lines.size() - 1) && _hasMore) {
			writeString(pt, "- MORE -");
		} else {
			writeString(pt, _lines[i]);
		}
	}
}

bool TextWindow::msgText(const TextMessage &msg) {
	if (msg._wrapped)
		addLines(msg._text);
	else
		add(msg._text);

	return true;
}

bool TextWindow::msgKeypress(const KeypressMessage &msg) {
	if (_hasMore) {
		_hasMore = false;
		needsRedraw();

		flush();
		return true;
	}

	return false;
}

bool TextWindow::msgMouseDown(const MouseDownMessage &msg) {
	if (_hasMore) {
		_hasMore = false;
		needsRedraw();

		flush();
		return true;
	}

	return false;
}

} // namespace Gfx
} // namespace Early
} // namespace Legend
