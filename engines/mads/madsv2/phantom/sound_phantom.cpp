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

#include "common/md5.h"
#include "mads/madsv2/phantom/sound_phantom.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {

void PhantomSoundManager::validate() {
	Common::File f;
	static const char *const MD5[] = {
		"205398468de2c8873b7d4d73d5be8ddc",
		"f9b2d944a2fb782b1af5c0ad592306d3",
		"7431f8dad77d6ddfc24e6f3c0c4ac7df",
		"eb1f3f5a4673d3e73d8ac1818c957cf4",
		"f936dd853073fa44f3daac512e91c476",
		nullptr,
		nullptr,
		nullptr,
		"a31e4783e098f633cbb6689adb41dd4f"
	};

	for (int i = 1; i <= 9; ++i) {
		if (i >= 6 && i <= 8)
			continue;
		Common::Path filename(Common::String::format("ASOUND.00%d", i));
		if (!f.open(filename))
			error("Could not process - %s", filename.toString().c_str());
		Common::String md5str = Common::computeStreamMD5AsString(f, 8192);
		f.close();

		if (md5str != MD5[i - 1])
			error("Invalid sound file - %s", filename.toString().c_str());
	}
}

void PhantomSoundManager::loadDriver(int sectionNumber) {
	switch (sectionNumber) {
	case 1:
		_driver = new ASound1(_mixer, _opl);
		break;
	case 2:
		_driver = new ASound2(_mixer, _opl);
		break;
	case 3:
		_driver = new ASound3(_mixer, _opl);
		break;
	case 4:
		_driver = new ASound4(_mixer, _opl);
		break;
	case 5:
		_driver = new ASound5(_mixer, _opl);
		break;
	case 9:
		_driver = new ASound9(_mixer, _opl);
		break;
	default:
		_driver = nullptr;
		return;
	}
}

/*-----------------------------------------------------------------------*/

ASound1::ASound1(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph1", 0) {
}

int ASound1::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound2::ASound2(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph1", 0) {
}

int ASound2::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound3::ASound3(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph1", 0) {
}

int ASound3::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound4::ASound4(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph1", 0) {
}

int ASound4::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound5::ASound5(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph1", 0) {
}

int ASound5::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound9::ASound9(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph1", 0) {
}

int ASound9::command(int commandId, int param) {
	// TODO
	return 0;
}

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS
