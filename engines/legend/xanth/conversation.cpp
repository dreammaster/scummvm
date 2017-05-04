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

#include "legend/xanth/conversation.h"
#include "legend/legend.h"

namespace Legend {
namespace Xanth {

BEGIN_MESSAGE_MAP(Conversation, Background)
	ON_MESSAGE(ShowMsg)
END_MESSAGE_MAP()

Conversation::Conversation(const Common::String &name, int convId, int targetId) :
		Background(name), _conversationId(convId), _targetPicNum(targetId) {
	struct ConvBounds {
		int _targetPicNum;
		int _left, _top, _right, _bottom;
	};
	static const ConvBounds BOUNDS[6] = {
		{ 2476, 129, 13, 215, 117 },
		{ 4529, 75, 9, 165, 102 },
		{ 1024, 72, 0, 153, 68 },
		{ 1536, 92, 0, 165, 89 },
		{ 1614, 59, 0, 126, 84 },
		{ 3609, 103, 0, 190, 99 }
	};

	// There are six special numbers we set special bounds for
	_isSpecialTarget = false;
	for (int idx = 0; idx < 6; ++idx) {
		if (BOUNDS[idx]._targetPicNum == _targetPicNum) {
			_bounds = Common::Rect(BOUNDS[idx]._left, BOUNDS[idx]._top,
				BOUNDS[idx]._right, BOUNDS[idx]._bottom);
			_isSpecialTarget = true;
			break;
		}
	}

	_image.addUnder(this);
}

bool Conversation::ShowMsg(CShowMsg &msg) {
	getScreen().loadFont(10);
	Font::setColor(15);

	// Load the image for the conversation target
	if (_isSpecialTarget) {

	} else {
		_image.load(_targetPicNum);
		getScreen().setPalette();
	}

	return Background::ShowMsg(msg);
}

} // End of namespace Xanth
} // End of namespace Legend
