/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 * You may also modify/distribute the code in this file under that license.
 */

#include "engines/ags/ags.h"
#include "engines/ags/audio.h"
#include "engines/ags/character.h"
#include "engines/ags/constants.h"
#include "engines/ags/gamefile.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/graphics.h"
#include "engines/ags/overlay.h"

#include "graphics/font.h"

namespace AGS {

// TODO: the 'color' parameter is abused in these functions to mean two things, should
// switch it back to uint and add another parameter for the extra -ve functionality
uint AGSEngine::createTextOverlayCore(int x, int y, uint width, uint fontId, int color, const Common::String &text, int allowShrink) {
	if (width < 8)
		width = _graphics->_width / 2;
	if (x < 0)
		x = (_graphics->_width / 2) - (width / 2);
	if (!color)
		color = 16;
	// FIXME: stupid crovr_id etc hack: FIXME see the 2 below
	return displayMain(x, y, width, text, 2, fontId, -color, false, allowShrink, false);
}

uint AGSEngine::createTextOverlay(int x, int y, uint width, uint fontId, int color, const Common::String &text) {
	uint allowShrink = 0;

	if (x != OVR_AUTOPLACE) {
		x = multiplyUpCoordinate(x);
		y = multiplyUpCoordinate(y);
		width = multiplyUpCoordinate(width);
	} else {
		// allow DisplaySpeechBackground to be shrunk
		allowShrink = 1;
	}

	return createTextOverlayCore(x, y, width, fontId, color, text, allowShrink);
}

uint AGSEngine::addScreenOverlay(int x, int y, uint type, const Graphics::Surface &surface, bool alphaChannel) {
	if (type == OVER_CUSTOM) {
		// find an unused custom ID
		for (uint i = OVER_CUSTOM + 1; i < OVER_CUSTOM + 100; ++i) {
			if (findOverlayOfType(i) != (uint)-1)
				continue;
			type = i;
			break;
		}
	}

	ScreenOverlay *overlay = new ScreenOverlay(this, Common::Point(x, y), type, surface, alphaChannel);
	_overlays.push_back(overlay);

	return _overlays.size() - 1;
}

void AGSEngine::updateOverlayTimers() {
	// Remove expired (text) overlays.
	// TODO: This loop is a bit weird; it removes by type, and didn't
	// account for the removed overlays in the loop.
	for (int i = 0; (uint)i < _overlays.size(); ++i) {
		if (!_overlays[i]->_timeout)
			continue;

		if (--_overlays[i]->_timeout == 0) {
			removeScreenOverlay(_overlays[i]->getType());
			--i;
		}
	}
}

uint AGSEngine::findOverlayOfType(uint type) {
	for (uint i = 0; i < _overlays.size(); ++i)
		if (_overlays[i]->getType() == type)
			return i;

	return (uint)-1;
}

void AGSEngine::removeScreenOverlay(uint type) {
	for (uint i = 0; i < _overlays.size(); ++i) {
		if (type == (uint)-1 || _overlays[i]->getType() == type) {
			removeScreenOverlayIndex(i);
			i--;
		}
	}
}

void AGSEngine::removeScreenOverlayIndex(uint index) {
	delete _overlays[index];
	_overlays.remove_at(index);

	// if an overlay before the sierra-style speech one is removed,
	// update the index
	if (_faceTalkingOverlayIndex > index)
		_faceTalkingOverlayIndex--;
}

void AGSEngine::updateSpeech() {
	// determine if speech text should be removed
	if (_state->_messageTime >= 0) {
		_state->_messageTime--;

		if (_audio->_channels[SCHAN_SPEECH]->isValid()) {
			if (_audio->_channels[SCHAN_SPEECH]->isPlaying() && !_state->_fastForward) {
				// extend life of text if the voice hasn't finished yet
				if (_state->_messageTime < 1)
					_state->_messageTime = 1;
			} else {
				// if the voice has finished, remove the speech
				_state->_messageTime = 0;
			}
		}

		if (_state->_messageTime < 1) {
			if (_state->_fastForward) {
				removeScreenOverlay(OVER_TEXTMSG);
			} else if (_state->_cantSkipSpeech & SKIP_AUTOTIMER) {
				removeScreenOverlay(OVER_TEXTMSG);
				// TODO: Is storing milliseconds here rather than ticks okay?
				_state->_ignoreUserInputUntilTime =
					_system->getMillis() + _state->_ignoreUserInputAfterTextTimeoutMs;
			}
		}
	}

	// FIXME: update sierra-style speech
}

void AGSEngine::displayAtY(int y, const Common::String &text) {
	// FIXME: sanity-check y parameter

	// Display("") ... a bit of a stupid thing to do, so ignore it
	if (text.empty())
		return;

	if (y > 0)
		y = multiplyUpCoordinate(y);

	if (getGameOption(OPT_ALWAYSSPCH)) {
		displaySpeechAt(-1, (y > 0) ? divideDownCoordinate(y) : y, -1, _gameFile->_playerChar, text);
	} else {
		// Normal "Display" in text box

		// FIXME: some dirty stuff here

		displayAt(-1, y, _graphics->_width / 2 + _graphics->_width / 4, getTranslation(text), 1);
	}
}

void AGSEngine::displayAt(int x, int y, int width, Common::String text, int blocking, int asSpeech, bool isThought,
	int allowShrink, bool overlayPositionFixed) {

	uint usingFont = _state->_normalFont;
	if (asSpeech)
		usingFont = _state->_speechFont;

	endSkippingUntilCharStops();

	bool needStopSpeech = false;
	// TODO: this is almost identical to the auto-speech code in displaySpeech
	if (!text.empty() && text[0] == '&') {
		// auto-speech
		int speechId = atoi(text.c_str() + 1);
		if (speechId <= 0)
			error("displayAt: invalid voice id %d for auto-voice", speechId);

		uint offset = 1;
		while (offset < text.size() && text[offset] != ' ')
			++offset;
		if (offset < text.size())
			++offset;
		text = text.c_str() + offset;

		if (playSpeech(_state->_narratorSpeech, speechId)) {
			if (_state->_wantSpeech == 2)
				text = "  ";  // speech only, no text.
		}

		needStopSpeech = true;
	}

	displayMain(x, y, width, text, blocking, usingFont, asSpeech, isThought, allowShrink, overlayPositionFixed);

	if (needStopSpeech)
		stopSpeech();
}

// Pass yy = -1 to find Y co-ord automatically
// allowShrink = 0 for none, 1 for leftwards, 2 for rightwards
// pass blocking=2 to create permanent overlay
uint AGSEngine::displayMain(int x, int y, int width, const Common::String &text, int blocking, int usingFont,
	int asSpeech, bool isThought, int allowShrink, bool overlayPositionFixed) {

	bool alphaChannel = false;
	Graphics::Font *font = _graphics->getFont(usingFont);
	// FIXME: this is the wrong height, and this is wgetfontheight
	uint fontHeight = font->getFontHeight();

	// TODO: this code, to remove '&5 ' type prefixes and handle '[', should be elsewhere
	// (it is in break_up_text_into_lines in original code)
	Common::String newText;
	for (uint i = 0; i < text.size(); ++i) {
		if (text[i] == '&') {
			while (i < text.size() && text[i] != ' ')
				++i;
		} else if (text[i] == '\\' && text[i + 1] == '[') {
			// escaped bracket
			newText += '[';
			++i;
		} else if (text[i] == '[')
			newText += '\n';
		else
			newText += text[i];
	}
	Common::Array<Common::String> lines;
	font->wordWrapText(newText, width - 6, lines);
	uint longestLine = 0;
	for (uint i = 0; i < lines.size(); ++i) {
		uint lineLength = font->getStringWidth(lines[i]);
		longestLine = MAX(lineLength, longestLine);
	}

	// AGS 2.x: If the screen is faded out, fade in again when displaying a message box.
	// "The narrator messages after a fadeout in a Tale of Two Kingdoms are now drawn."
	if (!asSpeech && getGameFileVersion() <= kAGSVer272)
		_state->_screenIsFadedOut = 0;

	// if it's a normal message box and the game was being skipped,
	// ensure that the screen is up to date before the message box
	// is drawn on top of it
	if ((_state->_skipUntilCharStops != (uint)-1) && (blocking == 1))
		{ } // FIXME: render_graphics();

	endSkippingUntilCharStops();

	// FIXME: top bar code

	if (asSpeech > 0) {
		// update the all_buttons_disabled variable in advance
		// of the adjust_x/y_for_guis calls
		_state->_disabledUserInterface++;
		// FIXME: updateGUIDisabledStatus();
		_state->_disabledUserInterface--;
	}

	if (x != OVR_AUTOPLACE) {
		if (y < 0) {
			// centre text in middle of screen
			y = (_graphics->_height / 2 - (lines.size() * fontHeight) / 2) - 3;
		} else if (asSpeech > 0) {
			// speech, so it wants to be above the character's head
			y -= lines.size() * fontHeight;
			if (y < 5)
				y = 5;
			// FIXME: y = adjustYForGUIs(y);
		}
	} else {
		// TODO: sanity-check that param y is a valid charId
	}

	if (longestLine < width - getFixedPixelSize(6)) {
		// shrink the width of the dialog box to fit the text
		uint oldWidth = width;
		// If it's not speech, or a shrink is allowed, then shrink it
		if (!asSpeech || (allowShrink > 0))
			width = longestLine + getFixedPixelSize(6);

		// shift the dialog box right to align it, if necessary
		if ((allowShrink == 2) && (x >= 0))
			x += (oldWidth - width);
	}

	if (x < -1) {
		x = (-x) - width / 2;
		if (x < 0)
			x = 0;

		// FIXME: x = adjustXForGUIs(x);

		if (x + width >= _graphics->_width)
			x = (_graphics->_width - width - 5);
	} else if (x < 0)
		x = (_graphics->_width / 2) - (width / 2);

	uint extraHeight = getFixedPixelSize(6);
	// FIXME: text color 15
	if (blocking < 2)
		removeScreenOverlay(OVER_TEXTMSG);

	Graphics::Surface surface;
	surface.create((width > 0) ? width : 2, lines.size() * fontHeight + extraHeight, _graphics->getPixelFormat());
	surface.fillRect(Common::Rect(0, 0, surface.w, surface.h), _graphics->getTransparentColor());

	// if it's an empty speech line, don't draw anything
	if (!text.empty() && text[0] != ' ' && width) {
		if (asSpeech) {
			int left = 0;
			int top = getFixedPixelSize(3);
			uint textWidth = width - 6;

			// Work out if we want to draw into a GUI or not.
			bool drawBackground = false;
			uint usingGui = (uint)-1;
			if ((asSpeech < 0) && (getGameOption(OPT_SPEECHTYPE) >= 2)) {
				usingGui = _state->_speechTextWindowGUI;
				drawBackground = true;
			} else if (isThought && (getGameOption(OPT_THOUGHTGUI) > 0)) {
				usingGui = getGameOption(OPT_THOUGHTGUI);
				// make it treat it as drawing inside a window now
				if (asSpeech > 0)
					asSpeech = -asSpeech;
				drawBackground = true;
			}
			// TODO: sanity-check GUI value

			// FIXME: draw background, set alpha channel
			warning("displayMain as speech unimplemented");

			uint textColor;
			uint align;
			if (asSpeech < 0) {
				// asSpeech < 0 means it's inside a text box
				align = _state->_textAlign;

				if (usingGui != (uint)-1)
					textColor = _graphics->resolveHardcodedColor(
						_gameFile->_guiGroups[usingGui]->_fgColor);
				else
					textColor = _graphics->resolveHardcodedColor(-asSpeech);
			} else {
				align = _state->_speechTextAlign;
				textColor = _graphics->resolveHardcodedColor(asSpeech);
				textWidth = width;
			}

			for (uint i = 0; i < lines.size(); ++i) {
				int textX = left;
				int textY = top + i * fontHeight;

				// FIXME: wrong width (wgettextwidth_compensate)
				uint lineLength = font->getStringWidth(lines[i]);
				if (align == SCALIGN_CENTRE)
					textX += (textWidth / 2) - (lineLength / 2);
				else if (align == SCALIGN_RIGHT)
					textX += textWidth - lineLength;

				_graphics->drawOutlinedString(usingFont, &surface, lines[i], textX, textY,
					textWidth, textColor);
			}
		} else {
			uint textWidth = width - 6;

			// FIXME: draw background

			// FIXME: if (getGameOption(OPT_TWCUSTOM)), alpha

			// FIXME: adjust_y_coordinate_for_text to try and work around some ttf thing

			uint align = _state->_textAlign;
			uint textColor = _graphics->resolveHardcodedColor(15);

			// TODO: de-duplicate from above
			for (uint i = 0; i < lines.size(); ++i) {
				int textX = 0;
				int textY = 0 + i * fontHeight;

				// FIXME: wrong width (wgettextwidth_compensate)
				uint lineLength = font->getStringWidth(lines[i]);
				if (align == SCALIGN_CENTRE)
					textX += (textWidth / 2) - (lineLength / 2);
				else if (align == SCALIGN_RIGHT)
					textX += textWidth - lineLength;

				_graphics->drawOutlinedString(usingFont, &surface, lines[i], textX, textY,
					textWidth, textColor);
			}
			warning("displayMain as non-speech unimplemented");
		}
	}

	uint overlayType = OVER_TEXTMSG;
	if (blocking == 2)
		overlayType = OVER_CUSTOM;
	else if (blocking >= OVER_CUSTOM)
		overlayType = blocking;

	uint overlayId = addScreenOverlay(x, y, overlayType, surface, alphaChannel);

	// (note that OVER_CUSTOM overlays are often assigned new types)
	if (blocking >= 2)
		return _overlays[overlayId]->getType();

	if (!blocking) {
		// if the speech does not time out, but we are skipping a cutscene,
		// allow it to time out
		if ((_state->_messageTime < 0) && _state->_fastForward)
			_state->_messageTime = 2;

		if (!overlayPositionFixed) {
			_overlays[overlayId]->_positionRelativeToScreen = false;
			_overlays[overlayId]->_pos.x += _graphics->_viewportX;
			_overlays[overlayId]->_pos.y += _graphics->_viewportY;
		}

		blockUntil(kUntilNoTextOverlay, 0);

		_state->_messageTime = -1;
		return 0;
	}

	if (_state->_fastForward) {
		removeScreenOverlay(OVER_TEXTMSG);

		_state->_messageTime = -1;
		return 0;
	}

	// FIXME
	warning("displayMain '%s' unimplemented", text.c_str());

	removeScreenOverlay(OVER_TEXTMSG);

	_state->_messageTime = -1;
	return 0;
}

void AGSEngine::displaySpeech(Common::String text, uint charId, int x, int y, int width, bool isThought) {
	if (charId >= _characters.size())
		error("displaySpeech: character %d is invalid", charId);
	Character *speakingChar = _characters[charId];
	if (speakingChar->_view < 0 || (uint)speakingChar->_view >= _gameFile->_views.size())
		error("displaySpeech: character %d has invalid view %d", charId, speakingChar->_view);

	if (_textOverlayCount)
		error("displaySpeech: speech was already displayed");

	endSkippingUntilCharStops();

	_saidSpeechLine = true;

	if (!_state->_bgSpeechStayOnDisplay) {
		for (int i = 0; (uint)i < _overlays.size(); ++i) {
			if (!_overlays[i]->_timeout)
				continue;

			removeScreenOverlay(_overlays[i]->getType());
			--i;
		}
	}

	_saidText = true;

	// note: the strings are translated before they get here

	// if the message is all .'s, don't display anything
	bool isPause = true;
	for (uint i = 0; i < text.size(); ++i) {
		if (text[i] == '.')
			continue;

		isPause = false;
		break;
	}

	_state->_messageTime = getTextDisplayTime(text);

	if (isPause) {
		// FIXME: update_music_at

		blockUntil(kUntilMessageDone, 0);
		return;
	}

	uint textColor = speakingChar->_talkColor;

	// if it's 0, it won't be recognised as speech
	if (textColor == 0)
		textColor = 16;

	int allowShrink = 0;
	uint useWidth = width;
	if (width < 0)
		useWidth = (_graphics->_width / 2) + (_graphics->_width / 4);

	int useView = speakingChar->_talkView;
	if (isThought) {
		useView = speakingChar->_thinkView;
		// view 0 is not valid for think views
		if (useView == 0)
			useView = -1;
		// speech bubble can shrink to fit
		allowShrink = 1;
		if (speakingChar->_room != _displayedRoom) {
			// not in room, centre it
			x = -1;
			y = -1;
		}
	}

	int useX = x, useY = y;
	int oldView = -1, oldLoop = -1;
	uint overlayType = 0;

	_lipsyncTextOffset = 0;
	_lipsyncText = text;

	if (text[0] == '&') {
		// auto-speech
		int speechId = atoi(text.c_str() + 1);
		if (speechId <= 0)
			error("displaySpeech: invalid voice id %d for auto-voice", speechId);

		uint offset = 1;
		while (offset < text.size() && text[offset] != ' ')
			++offset;
		if (offset < text.size())
			++offset;
		text = text.c_str() + offset;
		_lipsyncText = text;

		if (playSpeech(charId, speechId)) {
			if (_state->_wantSpeech == 2)
				text = "  ";  // speech only, no text.
		}
	}

	if (getGameOption(OPT_SPEECHTYPE) == 3)
		removeScreenOverlay(OVER_COMPLETE);

	if (getGameOption(OPT_SPEECHTYPE) == 0)
		allowShrink = 1;

	if (speakingChar->_idleLeft < 0) {
		// if idle anim in progress for the character, stop it
		speakingChar->unlockView();
	}

	bool overlayPositionFixed = 0;
	uint charFrameWas = 0;
	bool viewWasLocked = (speakingChar->_flags & CHF_FIXVIEW);

	if (speakingChar->_room != _displayedRoom) {
		allowShrink = 1;
	} else {
		// If the character is in this room, go for it - otherwise
		// the above does text in the middle of the screen

		if (useX < 0)
			useX = multiplyUpCoordinate(speakingChar->_x) - _graphics->_viewportX;
		if (useX < 2)
			useX = 2;

		if (speakingChar->_walking)
			speakingChar->stopMoving();

		// save the frame we need to go back to
		// if they were moving, this will be 0 (because we just called
		// StopMoving); otherwise, it might be a specific animation
		// frame which we should return to
		if (viewWasLocked)
			charFrameWas = speakingChar->_frame;

		// if the current loop doesn't exist in talking view, use loop 0
		if (speakingChar->_loop >= _gameFile->_views[speakingChar->_view]._loops.size())
			speakingChar->_loop = 0;

		// FIXME: sanity-check view/loop/frame

		if (useY < 0) {
			// FIXME: frame 0: uint picId = getViewFrame(speakingChar->_view, speakingChar->_loop, 0)->_pic;
			useY = multiplyUpCoordinate(speakingChar->getEffectiveY()) - _graphics->_viewportY;
			useY -= getFixedPixelSize(5);
			useY -= speakingChar->getDrawHeight();
			// if it's a thought, lift it a bit further up
			if (isThought)
				useY -= getFixedPixelSize(10);
		}

		if (useY < 5)
			useY = 5;

		useX = -useX; // tell it to centre it

		if ((useView >= 0) && (getGameOption(OPT_SPEECHTYPE) > 0)) {
			// Sierra-style close-up portrait

			// FIXME
		} else if (useView >= 0) {
			// Lucasarts-style speech

			oldView = speakingChar->_view;
			oldLoop = speakingChar->_loop;

			// FIXME: animating
			speakingChar->_animating |= CHANIM_REPEAT;

			speakingChar->_view = useView;
			speakingChar->_frame = 0;
			speakingChar->_flags |= CHF_FIXVIEW;

			// current character loop is outside the normal talking directions
			if (speakingChar->_loop >= _gameFile->_views[speakingChar->_view]._loops.size())
				speakingChar->_loop = 0;

			// FIXME: _facetalkBlinkLoop = speakingChar->_loop;

			// FIXME: sanity-check loop and frame count

			// FIXME: wait

			if (width < 0) {
				useWidth = _graphics->_width / 2 + _graphics->_width / 6;

				// If they are close to the screen edge, make the text narrower
				int relX = multiplyUpCoordinate(speakingChar->_x) - _graphics->_viewportX;
				if ((relX < _graphics->_width / 4) || (relX > _graphics->_width - (_graphics->_width / 4)))
					useWidth -= _graphics->_width / 5;
			}

			/* FIXME: if (!isThought) // set up the lip sync if not thinking
				_charSpeaking = charId; */
		}
	}

	// it wants the centred position, so make it so
	if ((x >= 0) && (useX < 0))
		useX -= width / 2;

	// if they used DisplaySpeechAt, then use the supplied width
	if ((width > 0) && !isThought)
		allowShrink = 0;

	displayAt(useX, useY, useWidth, text, 0, textColor, isThought, allowShrink, overlayPositionFixed);

	// FIXME: close up
	// FIXME: face
	warning("displaySpeech '%s' unimplemented", text.c_str());

	if (oldView >= 0) {
		// restore previous state

		if (viewWasLocked)
			speakingChar->_flags |= CHF_FIXVIEW;
		else
			speakingChar->_flags &= ~CHF_FIXVIEW;
		speakingChar->_view = oldView;
		speakingChar->_loop = oldLoop;
		speakingChar->_frame = charFrameWas;
		speakingChar->_animating = 0;
		speakingChar->_wait = 0;
		speakingChar->_idleLeft = speakingChar->_idleTime;
		// restart the idle animation straight away
		speakingChar->_processIdleThisTime = true;
	}

	// FIXME: reset charSpeaking
	stopSpeech();
}

void AGSEngine::displaySpeechCore(const Common::String &text, uint charId) {
	if (text.empty()) {
		// no text, just update the current character who's speaking
		// this allows the portrait side to be switched with an empty
		// speech line
		_state->_swapPortraitLastChar = charId;
		return;
	}

	// adjust timing of text (so that DisplaySpeech("%s", str) pauses
	// for the length of the string not 2 frames)
	if (text.size() > _lastTranslationSourceTextLength + 3)
		_lastTranslationSourceTextLength = text.size();

	displaySpeech(text, charId);
}

void AGSEngine::displaySpeechAt(int x, int y, int width, uint charId, const Common::String &text) {
	multiplyUpCoordinates(x, y);
	width = multiplyUpCoordinate(width);
	displaySpeech(getTranslation(text), charId, x, y, width);
}

uint AGSEngine::displaySpeechBackground(uint charId, const Common::String &text) {
	// remove any previous background speech for this character
	for (uint i = 0; i < _overlays.size(); ++i) {
		if (_overlays[i]->_bgSpeechForChar == charId) {
			removeScreenOverlayIndex(i);
			i--;
		}
	}

	uint overlayType = createTextOverlay(OVR_AUTOPLACE, charId, _graphics->_width / 2, _state->_speechFont,
		-(int)_characters[charId]->_talkColor, getTranslation(text));

	uint overlayId = findOverlayOfType(overlayType);
	assert(overlayId != (uint)-1);
	_overlays[overlayId]->_bgSpeechForChar = charId;
	_overlays[overlayId]->_timeout = getTextDisplayTime(text, true);
	return overlayId;
}

ScreenOverlay::ScreenOverlay(AGSEngine *vm, const Common::Point &pos, uint type,
	const Graphics::Surface &surface, bool alphaChannel) : _vm(vm), _pos(pos), _type(type), _timeout(0),
	_bgSpeechForChar((uint)-1), _positionRelativeToScreen(true), _hasAlphaChannel(alphaChannel) {

	if (_type == OVER_COMPLETE)
		_vm->_completeOverlayCount++;
	else if (_type == OVER_TEXTMSG)
		_vm->_textOverlayCount++;

	// Take ownership.
	_surface = surface;
}

ScreenOverlay::~ScreenOverlay() {
	if (_type == OVER_COMPLETE)
		_vm->_completeOverlayCount--;
	else if (_type == OVER_TEXTMSG)
		_vm->_textOverlayCount--;

	_surface.free();
}

Common::Point ScreenOverlay::getDrawPos() {
	if (_type == OVER_COMPLETE)
		return _pos;

	Common::Point pos = _pos;
	if (_pos.x == OVR_AUTOPLACE) {
		// auto place on character (id in pos.y)
		Character *chr = _vm->_characters[_pos.y];
		if (chr->_room != _vm->getCurrentRoomId()) {
			return Common::Point((_vm->_graphics->_width / 2) - (_surface.w / 2),
				(_vm->_graphics->_height / 2) - (_surface.h / 2));
		}

		uint charPic = _vm->getViewFrame(chr->_view, chr->_loop, 0)->_pic;

		pos.y = _vm->multiplyUpCoordinate(chr->getEffectiveY()) - _vm->_graphics->_viewportY - 5;
		pos.y -= chr->getDrawHeight();
		pos.y -= _surface.h;
		if (pos.y < 5)
			pos.y = 5;

		pos.x = _vm->multiplyUpCoordinate(chr->_x) - _surface.w/2 - _vm->_graphics->_viewportX;
		if (pos.x < 0)
			pos.x = 0;

		if (pos.x + _surface.w >= _vm->_graphics->_width)
			pos.x = _vm->_graphics->_width - _surface.w - 1;
	} else {
		if (!_positionRelativeToScreen) {
			pos.x -= _vm->_graphics->_viewportX;
			pos.y -= _vm->_graphics->_viewportY;
		}
	}

	return pos;
}

int ScreenOverlay::getDrawOrder() const {
	return 0;
}

const Graphics::Surface *ScreenOverlay::getDrawSurface() {
	return &_surface;
}

uint ScreenOverlay::getDrawWidth() {
	return _surface.w;
}

uint ScreenOverlay::getDrawHeight() {
	return _surface.h;
}

uint ScreenOverlay::getDrawTransparency() {
	return 0;
}

bool ScreenOverlay::isDrawMirrored() {
	return false;
}

int ScreenOverlay::getDrawLightLevel() {
	return 0; // FIXME
}

void ScreenOverlay::getDrawTint(int &lightLevel, int &luminance, byte &red, byte &green, byte &blue) {
	// FIXME
}

} // End of namespace AGS
