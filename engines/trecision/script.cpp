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

#include "trecision/nl/message.h"
#include "trecision/trecision.h"

namespace Trecision {

/*-------------------------------------------------------------------------*/
/*                                EndScript           					   */
/*-------------------------------------------------------------------------*/
void TrecisionEngine::EndScript() {
	_curStack--;
	if (_curStack == 0) {
		_flagscriptactive = false;
		g_vm->_flagMouseEnabled = true;
		redrawString();
	}
}

/*-------------------------------------------------------------------------*/
/*                               PlayScript           					   */
/*-------------------------------------------------------------------------*/
void TrecisionEngine::PlayScript(uint16 i) {
	_curStack++;
	_flagscriptactive = true;
	g_vm->_flagMouseEnabled = false;
	g_vm->_curScriptFrame[_curStack] = g_vm->_script[i]._firstFrame;

	SScriptFrame *curFrame = &g_vm->_scriptFrame[g_vm->_curScriptFrame[_curStack]];
	// If the event is empty, terminate the script
	if ((curFrame->_class == 0) && (curFrame->_event == 0)) {
		EndScript();
		return;
	}

	bool loop = true;
	while (loop) {
		loop = false;
		curFrame = &g_vm->_scriptFrame[g_vm->_curScriptFrame[_curStack]];
		SScriptFrame *nextFrame = &g_vm->_scriptFrame[g_vm->_curScriptFrame[_curStack] + 1];
		curFrame->sendFrame();
		if (curFrame->_noWait && !((nextFrame->_class == 0) && (nextFrame->_event == 0))) {
			g_vm->_curScriptFrame[_curStack]++;
			loop = true;
		}
	}
}

/*-------------------------------------------------------------------------*/
/*                               EvalScript           					   */
/*-------------------------------------------------------------------------*/
void TrecisionEngine::EvalScript() {
	if (g_vm->_characterQueue.testEmptyCharacterQueue4Script() && g_vm->_gameQueue.testEmptyQueue(MC_DIALOG) && _flagScreenRefreshed) {
		g_vm->_curScriptFrame[_curStack]++;
		g_vm->_flagMouseEnabled = false;

		SScriptFrame *curFrame = &g_vm->_scriptFrame[g_vm->_curScriptFrame[_curStack]];
		if ((curFrame->_class == 0) && (curFrame->_event == 0)) {
			EndScript();
			return;
		}

		bool loop = true;
		while (loop) {
			loop = false;
			curFrame = &g_vm->_scriptFrame[g_vm->_curScriptFrame[_curStack]];
			SScriptFrame *nextFrame = &g_vm->_scriptFrame[g_vm->_curScriptFrame[_curStack] + 1];
			curFrame->sendFrame();
			if (curFrame->_noWait && !((nextFrame->_class == 0) && (nextFrame->_event == 0))) {
				g_vm->_curScriptFrame[_curStack]++;
				loop = true;
			}
		}
	}
}

} // End of namespace Trecision