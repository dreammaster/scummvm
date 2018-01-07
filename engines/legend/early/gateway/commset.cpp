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

#include "legend/early/gateway/commset.h"
#include "legend/early/gfx/screen.h"
#include "legend/legend.h"
#include "legend/core/resources.h"

namespace Legend {
namespace Early {

BEGIN_MESSAGE_MAP(Commset, VisualItem)
	ON_MESSAGE(ShowMsg)
	ON_MESSAGE(HideMsg)
	ON_MESSAGE(MouseButtonDownMsg)
	ON_MESSAGE(KeyCharMsg)
END_MESSAGE_MAP()

Commset::Commset() : VisualItem(), _logo(nullptr) {
	_name = "Commset";
	setBounds(Screen::get().getBounds());

	const int TEXT_COLUMNS = 72;
	const int TEXT_ROWS = 25;
	// TODO: Regions
}

Commset::~Commset() {
	delete _logo;
}

void Commset::draw() {
	if (!_isDirty)
		return;
	VisualItem::draw();

	// Fill out the background
	Gfx::VisualSurface s = getSurface();
	s.fillRect(s.getBounds(), 2);

	s.blitFrom(*_logo);

	// TODO
}

bool Commset::ShowMsg(CShowMsg &msg) {
	_logo = g_vm->_picFile->load(574, 1);
	return true;
}

bool Commset::HideMsg(CHideMsg &msg) {
	delete _logo;
	_logo = nullptr;
	return true;
}

bool Commset::MouseButtonDownMsg(CMouseButtonDownMsg &msg) {
	return true;
}

bool Commset::KeyCharMsg(CKeyCharMsg &msg) {
	return true;
}

} // End of namespace Early
} // End of namespace Legend
