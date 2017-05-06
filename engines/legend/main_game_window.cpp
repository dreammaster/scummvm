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

#include "common/config-manager.h"
#include "legend/legend.h"
#include "legend/game_manager.h"
#include "legend/main_game_window.h"
#include "legend/messages.h"

namespace Legend {

MainGameWindow::MainGameWindow(LegendEngine *vm): _vm(vm),
		_priorLeftDownTime(0), _priorMiddleDownTime(0), _priorRightDownTime(0) {
	_gameManager = nullptr;

	// Create the game project
	_project = g_vm->createProject();

	// Set the window as an event target
	vm->_events->addTarget(this);
}

MainGameWindow::~MainGameWindow() {
	delete _project;
}

void MainGameWindow::applicationStarting() {
	// Create game view and manager
	_gameManager = new GameManager(_project, g_vm->_mixer);

	// Set the starting game view
	if (g_vm->getGameID() == GType_CompanionsOfXanth) {
		g_vm->_mouseCursor->show();
	}

	const char *name = g_vm->getGameID() == GType_Gateway ? "Help" : "Compendium";
	Gfx::VisualItem *firstView = dynamic_cast<Gfx::VisualItem *>(_project->findByName(name));
	_gameManager->setView(firstView);

	// Generate starting message for showing the view
	CShowMsg showMsg;
	showMsg.execute(firstView, nullptr, MSGFLAG_SCAN);
}

int MainGameWindow::getSavegameSlot() {
	return 0;
}

void MainGameWindow::draw() {
	if (_gameManager->_view)
		_gameManager->_view->draw();
}

void MainGameWindow::mouseChanged() {

}

void MainGameWindow::onIdle() {
	if (!_gameManager)
		return;

	// Let the game manager perform any game updates
	_gameManager->update();

	// Handle any drawing updates
	draw();
}

#define HANDLE_MESSAGE(METHOD) \
	_gameManager->_inputTranslator.METHOD(g_vm->_events->getSpecialButtons(), mousePos); \
	mouseChanged()


void MainGameWindow::mouseMove(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	HANDLE_MESSAGE(mouseMove);
}

void MainGameWindow::leftButtonDown(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	if ((_vm->_events->getTicksCount() - _priorLeftDownTime) < DOUBLE_CLICK_TIME) {
		_priorLeftDownTime = 0;
		leftButtonDoubleClick(mousePos);
	} else {
		_priorLeftDownTime = _vm->_events->getTicksCount();
		HANDLE_MESSAGE(leftButtonDown);
	}
}

void MainGameWindow::leftButtonUp(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	HANDLE_MESSAGE(leftButtonUp);
}

void MainGameWindow::leftButtonDoubleClick(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	HANDLE_MESSAGE(leftButtonDoubleClick);
}

void MainGameWindow::middleButtonDown(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	if ((_vm->_events->getTicksCount() - _priorMiddleDownTime) < DOUBLE_CLICK_TIME) {
		_priorMiddleDownTime = 0;
		middleButtonDoubleClick(mousePos);
	} else {
		_priorMiddleDownTime = _vm->_events->getTicksCount();
		HANDLE_MESSAGE(middleButtonDown);
	}
}

void MainGameWindow::middleButtonUp(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	HANDLE_MESSAGE(middleButtonUp);
}

void MainGameWindow::middleButtonDoubleClick(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	HANDLE_MESSAGE(middleButtonDoubleClick);
}

void MainGameWindow::rightButtonDown(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	if ((_vm->_events->getTicksCount() - _priorRightDownTime) < DOUBLE_CLICK_TIME) {
		_priorRightDownTime = 0;
		rightButtonDoubleClick(mousePos);
	} else {
		_priorRightDownTime = _vm->_events->getTicksCount();
		HANDLE_MESSAGE(rightButtonDown);
	}
}

void MainGameWindow::rightButtonUp(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	HANDLE_MESSAGE(rightButtonUp);
}

void MainGameWindow::mouseWheel(const Common::Point &mousePos, bool wheelUp) {
	if (!isMouseControlEnabled())
		return;

	_gameManager->_inputTranslator.mouseWheel(wheelUp, mousePos);
	mouseChanged();
}

void MainGameWindow::rightButtonDoubleClick(const Common::Point &mousePos) {
	if (!isMouseControlEnabled())
		return;

	HANDLE_MESSAGE(rightButtonDoubleClick);
}

void MainGameWindow::keyDown(Common::KeyState keyState) {
	if (keyState.keycode == Common::KEYCODE_d && (keyState.flags & Common::KBD_CTRL)) {
		// Attach to the debugger
		_vm->_debugger->attach();
		_vm->_debugger->onFrame();
	}

	_gameManager->_inputTranslator.keyDown(keyState);
}

} // End of namespace Legend
