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

#ifndef ULTIMA3_H
#define ULTIMA3_H

#include "engines/engine.h"
#include "ultima/detection.h"

namespace Ultima {
namespace Ultima3 {

class Ultima3Engine : public Engine {
private:
	const Ultima::UltimaGameDescription *_gameDescription;

protected:
	// Engine APIs
	Common::Error run() override;

public:
	Ultima3Engine(OSystem *syst, const Ultima::UltimaGameDescription *gameDesc);
	~Ultima3Engine() override;

};

extern Ultima3Engine *g_ultima;

} // namespace Ultima3
} // namespace Ultima

#endif
