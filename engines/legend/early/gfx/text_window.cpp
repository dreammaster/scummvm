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

#include "common/savefile.h"
#include "legend/early/gfx/text_window.h"

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

void TextWindow::add(const String &msg) {
	// Unlike the original, which took care of doing vardiac formatting
	// here, in ScummVM string formatting will be done before calling this.
	// As such, I don't think this method needs any implementation
#if 1
	addDirect(msg);
#else
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
			addDirect(line);

			lineP += lineEnd + 1;
			lineEnd = -1;

			// If already at end of msg, then break
			if (!c)
				break;
		}
	}
#endif
}

void TextWindow::addDirect(const String &msg) {
	if (_hasMore) {
		// Waiting for user, so cache further adds
		_pendingAdds.push_back(msg);
		return;
	}

	// If logging is enabled, write out the text
	if (_logFile)
		_logFile->writeString(msg);

	// Get the current line being added to
	String &currentLine = _lines[_lineNum];
	const int width = _innerBounds.width();
	const int spaceWidth = _font->charWidth(' ');
	int xPos = _font->_xCenter + getLineWidth(currentLine);
	int widthRemaining;

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
		int wordWidth = _font->stringWidth(word);

		if (wordWidth > width) {
			// Word is too big to fit in the entire line. Must be something like
			// antidisestablishmentarism. Figure out how much of it we can fit
			// into the current line, given whatever position the cursor is at
			widthRemaining = width - xPos;
			wordWidth = 0;

			for (endPos = 0; text[endPos]; ++endPos) {
				wordWidth += _font->charWidth(text[endPos]);
				if (wordWidth > widthRemaining)
					break;
			}

			word = String(text.c_str(), text.c_str() + endPos);

		} else {
			// Word can fit on a line. Now see if it will fit on the current one
			widthRemaining = width - xPos;

			if (wordWidth > widthRemaining) {
				// Yep. It won't fit on the line
				newLine();
				if (_hasMore) {
					// This will cache remaining text until after user dismisses MORE
					addDirect(text);
					return;
				}
			}
		}

		// Update word, and strip it off the text
		word = String(text.c_str(), text.c_str() + endPos);
		currentLine += word;
		text = String(text.c_str() + endPos);

		// Update the total pixel width of the line wih the word added
		xPos += _font->stringWidth(word);

		switch (text.firstChar()) {
		case '\t':
			// Tab characters get included in the line, since draw handles
			// them specially
			text.deleteChar(0);
			currentLine += '\t';
			xPos = _font->_xCenter + getLineWidth(currentLine);
			widthRemaining = width - xPos;
			break;

		case '\n':
			text.deleteChar(0);
			newLine();
			if (_hasMore) {
				// This will cache remaining text until after user dismisses MORE
				addDirect(text);
				return;
			}
			break;

		case ' ':
			text.deleteChar(0);
			if (spaceWidth > widthRemaining) {
				newLine();
				if (_hasMore) {
					// This will cache remaining text until after user dismisses MORE
					addDirect(text);
					return;
				}

			} else {
				currentLine += ' ';
				xPos += spaceWidth;
				widthRemaining -= spaceWidth;
			}
			break;

		default:
			break;
		}
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
			addDirect(adds[i]);
	}
}

size_t TextWindow::getLineWidth(const String &line) const {
	String str = line;
	int width = 0;
	Legend::Gfx::Font *font = _font;
	size_t pos;

	// Handle any text up to tab characters
	while ((pos = str.findFirstOf('\t')) != String::npos) {
		width += font->stringWidth(String(str.c_str(), str.c_str() + pos));
		width = ((width + font->_tabWidth - 1) / font->_tabWidth) * font->_tabWidth;
		str = String(str.c_str() + pos + 1);
	}

	// Handle remaining text
	width += font->stringWidth(str);
	return width;
}

void TextWindow::draw() {
	BoxedElement::draw();
	Legend::Gfx::Font *font = _font;
	size_t pos;

	for (uint i = 0; i < _lines.size(); ++i) {
		Common::Point pt(_innerBounds.left,
			_innerBounds.top + font->_lineHeight * i);
		pos = _lines[i].findFirstOf('\t');

		if (i == (_lines.size() - 1) && _hasMore) {
			writeString(pt, "- MORE -");
		} else if (pos == String::npos) {
			writeString(pt, _lines[i]);
		} else {
			// Line containing tab characters
			String str = _lines[i];
			_textPos = pt;
			do {
				String fragment(str.c_str(), str.c_str() + pos);
				writeString(fragment);

				// FIgure out increment for tab
				int width = font->stringWidth(fragment);
				_textPos.x += font->_tabWidth - ((width + font->_tabWidth) % font->_tabWidth);

				pos = str.findFirstOf('\t');
			} while (pos != String::npos);

			// Handle anything after last tab
			if (!str.empty())
				writeString(str);
		}
	}
}

bool TextWindow::msgText(const TextMessage &msg) {
	if (msg._formatted)
		add(msg._text);
	else
		addDirect(msg._text);

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
		_linesRemaining = (int)_lines.size() - 1;
		needsRedraw();

		flush();
		return true;
	}

	return false;
}

} // namespace Gfx
} // namespace Early
} // namespace Legend
