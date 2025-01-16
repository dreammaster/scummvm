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

#ifndef WASTELAND_WASTELAND1_WASTELAND1_H
#define WASTELAND_WASTELAND1_WASTELAND1_H

#include "wasteland/engine.h"
#include "wasteland/wasteland1/views/views.h"
#include "wasteland/wasteland1/files/game_archive.h"
#include "wasteland/wasteland1/data/saved.h"
#include "wasteland/wasteland1/gfx/pics.h"

namespace Wasteland {
namespace Wasteland1 {

class Wasteland1Engine : public Wasteland::Engine {
private:
	Wasteland1::Views::Views *_views = nullptr;
	uint16 _mapX = 0, _mapY = 0;

protected:
	void setup() override;
	GUI::Debugger *getConsole() override;

public:
	GameArchive *_gameArchive = nullptr;
	Data::Saved _saved;
	Gfx::PicsDecoder _pics;
	Data::CurrentParty _party;
	Data::PartyMember *&_currentChar;

public:
	Wasteland1Engine(OSystem *syst, const WastelandGameDescription *gameDesc);
	~Wasteland1Engine() override;
	void initializePath(const Common::FSNode &gamePath) override;
};

extern Wasteland1Engine *g_engine;

} // namespace Wasteland1
} // namespace Wasteland

#endif
