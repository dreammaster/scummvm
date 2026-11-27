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

#ifndef ULTIMA5_VIEWS_H
#define ULTIMA5_VIEWS_H

#include "ultima/shared/engine/events.h"
#include "ultima/ultima5/views/acknowledgements.h"
#include "ultima/ultima5/views/asset_viewer.h"
#include "ultima/ultima5/views/create_character.h"
#include "ultima/ultima5/views/game.h"
#include "ultima/ultima5/views/introduction.h"
#include "ultima/ultima5/views/title.h"
#include "ultima/ultima5/views/title_demo.h"
#include "ultima/ultima5/views/transfer_character.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

struct Views {
	Acknowledgements _acknowledgements;
	AssetViewer _assetViewer;
	CreateCharacter _createCharacter;
	Game _game;
	Introduction _introduction;
	Title _title;
	TitleDemo _titleDemo;
	TransferCharacter _transferCharacter;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
