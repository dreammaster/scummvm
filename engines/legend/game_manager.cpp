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

#include "legend/game_manager.h"
#include "legend/legend.h"
#include "legend/core/project_item.h"
#include "legend/messages.h"

namespace Legend {

GameManager::GameManager(ProjectItem *project, Audio::Mixer *mixer):
		_project(project), _inputHandler(this), _inputTranslator(&_inputHandler) {
}

GameManager::~GameManager() {
	_project->resetGameManager();
}

void GameManager::save(SimpleFile *file) {
}

void GameManager::load(SimpleFile *file) {
}

void GameManager::preLoad() {

}

void GameManager::postLoad(ProjectItem *project) {

}

void GameManager::preSave(ProjectItem *project) {

}

void GameManager::postSave() {

}

void GameManager::initBounds() {
}

void GameManager::update() {
	/*
	updateMovies();
	frameMessage(getRoom());
	_timers.update(g_vm->_events->getTicksCount());
	_trueTalkManager.removeCompleted();

	CScreenManager::_screenManagerPtr->_mouseCursor->update();

	CViewItem *view = getView();
	if (view) {
		// Expand the game manager's bounds to encompass all the view's items
		for (CTreeItem *item = view; item; item = item->scan(view)) {
			Rect r = item->getBounds();
			if (!r.isEmpty())
				_bounds.extend(r);
		}

		// Also include the PET control in the bounds
		if (_project) {
			CPetControl *pet = _project->getPetControl();
			if (pet)
				_bounds.extend(pet->getBounds());
		}

		// And the text cursor
		CScreenManager *screenManager = CScreenManager::_screenManagerPtr;
		CTextCursor *textCursor = screenManager->_textCursor;
		if (textCursor && textCursor->_active)
			_bounds.extend(textCursor->getCursorBounds());

		// Set the surface bounds
		screenManager->setSurfaceBounds(SURFACE_BACKBUFFER, _bounds);

		// Handle redrawing the view
		if (!_bounds.isEmpty()) {
			_gameView->draw(_bounds);
			_bounds = Rect();
		}

		_gameState.checkForViewChange();
	}
	*/
}

} // End of namespace Legend
