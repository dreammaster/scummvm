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

#include "wizardry/detection.h"

namespace Wizardry {

const PlainGameDescriptor wizardryGames[] = {
	{ "wizardry1", "Wizardry: Proving Grounds of the Mad Overlord" },
	{ 0, 0 }
};

const WizardryGameDescription gameDescriptions[] = {
	{
		{
			"wizardry1",
			nullptr,
			AD_ENTRY1s("Wizardry-Proving-Grounds-Original-Scenario.dsk", "1cde92e99c4543c8d06e22a2bd61a9ec", 143360),
			Common::EN_ANY,
			Common::kPlatformApple2,
			ADGF_UNSTABLE,
			GUIO1(GUIO_NONE)
		},
		kWizardry1V1
	},

	{
		{
			"wizardry1",
			nullptr,
			AD_ENTRY1s("wiz1.dsk", "85fffb1632eccf747b5ac84ad88000a6", 327680),
			Common::EN_ANY,
			Common::kPlatformDOS,
			ADGF_UNSTABLE,
			GUIO1(GUIO_NONE)
		},
		kWizardry1V2,
	},

	{ AD_TABLE_END_MARKER, (WizardryVersion)0 }
};

} // End of namespace Wizardry
