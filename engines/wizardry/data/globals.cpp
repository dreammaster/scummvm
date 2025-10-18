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

#include "wizardry/data/globals.h"
#include "wizardry/libs/memory.h"
#include "wizardry/gfx/palette.h"
#include "wizardry/wizardry.h"
#include "wizardry/libs/wizardry_archive.h"

namespace Wizardry {

Globals *g_globals;

Globals::Globals() {
	g_globals = this;
}

Globals::~Globals() {
	g_globals = nullptr;
	delete _font;
}

void Globals::setup() {
	// Setup file access
	WizardryArchive *arc;
	if (g_engine->getVersion() == kWizardry1V2)
		arc = new Compressed::WizardryV2Archive();
	else if (!g_engine->isUncompressed())
		arc = new Compressed::WizardryV1Archive();
	else
		arc = new Uncompressed::Wizardry1V1Archive();
	SearchMan.add("dsk", arc, 1);

	// Setup palette
	Gfx::setupPalette();

	// Setup font
	if (g_engine->getVersion() == kWizardry1V1)
		_font = new WizardryFontV1();
	else
		_font = new WizardryFontV2();
	_font->setGfxMode(false);

#if 0
	_SCNTOCBL = FINDFILE(DRIVE1, "scenario.data");
	assert(_SCNTOCBL >= 0);
	UNITREAD(DRIVE1, _IOCACHE, sizeof(_IOCACHE), _SCNTOCBL);
	_SCNTOC.load(_IOCACHE);
#endif
}

} // namespace Wizardry
