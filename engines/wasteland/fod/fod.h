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

#ifndef WASTELAND_FOD_FOD_H
#define WASTELAND_FOD_FOD_H

#include "wasteland/engine.h"
#include "wasteland/fod/views/views.h"
#include "wasteland/fod/data/party.h"
#include "wasteland/fod/data/archetypes.h"
#include "wasteland/fod/data/pics.h"

namespace Wasteland {
namespace FOD {

class FountainOfDreamsEngine : public Wasteland::Engine {
private:
	FOD::Views::Views *_views = nullptr;

protected:
	void setup() override;

public:
	Data::Disk1 _disk1;
	bool _hasParty = false;
	Data::Archetypes _archetypes;
	Data::Pics _pics;

public:
	FountainOfDreamsEngine(OSystem *syst, const WastelandGameDescription *gameDesc);
	~FountainOfDreamsEngine() override;
};

extern FountainOfDreamsEngine *g_engine;

} // namespace FOD
} // namespace Wasteland

#endif // WASTELAND_H
