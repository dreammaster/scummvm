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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/input_handler.h"
#include "legend/game_manager.h"
#include "legend/legend.h"
#include "legend/messages.h"

namespace Legend {

InputHandler::InputHandler(GameManager *owner) :
		_gameManager(owner), _inputTranslator(nullptr),
		_buttonDown(false), _lockCount(0), _abortMessage(false) {
}

InputHandler::~InputHandler() {
}

void InputHandler::setTranslator(InputTranslator *translator) {
	_inputTranslator = translator;
}

void InputHandler::incLockCount() {
	++_lockCount;
}

void InputHandler::decLockCount() {
	--_lockCount;
	assert(_lockCount >= 0);

	if (_lockCount == 0 && _inputTranslator) {
		if (_dragging && !_inputTranslator->isMousePressed()) {
			CMouseButtonUpMsg upMsg(_mousePos, MK_LBUTTON);
			handleMessage(upMsg);
		}

		_buttonDown = _inputTranslator->isMousePressed();
		_abortMessage = true;
	}
}

void InputHandler::handleMessage(CMessage &msg, bool respectLock) {
	if (!respectLock || _lockCount <= 0) {
		processMessage(&msg);
	}
}

void InputHandler::processMessage(CMessage *msg) {
	const CMouseMsg *mouseMsg = dynamic_cast<const CMouseMsg *>(msg);
	_abortMessage = false;
	dispatchMessage(msg);

	if (_abortMessage) {
		_abortMessage = false;
	} else if (mouseMsg) {
		// Keep the game state mouse position up to date
		if (_mousePos != mouseMsg->_mousePos) {
			_mousePos = mouseMsg->_mousePos;
		}

		// Set flag for whether a mouse button is currently being pressed
		if (mouseMsg->isButtonDownMsg())
			_buttonDown = true;
		else if (mouseMsg->isButtonUpMsg())
			_buttonDown = false;
	}
}

void InputHandler::dispatchMessage(CMessage *msg) {
	Gfx::VisualItem *view = _gameManager->getView();
	if (view)
		msg->execute(view);
}

} // End of namespace Legend
