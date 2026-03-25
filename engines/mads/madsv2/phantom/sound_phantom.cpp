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
#include "common/textconsole.h"
#include "mads/madsv2/phantom/sound_phantom.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {

void PhantomSoundManager::validate() {
	Common::File f;
	static const char *const MD5[] = {
		"8edcb79a8c3514eac0835496326a72ae",
		"4b81a46440f8404d9eda1ce5ae2c5579",
		"11d8d441e47ad1ccd8faafd6572a17d0",
		"4cd5c4d45126e60ca701690489ab8afa",
		"588357d711bbcdabdf7d7e5d96013ce5",
		nullptr,
		nullptr,
		nullptr,
		"3d4843074c1dcbfd7919179c58aec9bc"
	};

	for (int i = 1; i <= 9; ++i) {
		if (i >= 6 && i <= 8)
			continue;
		Common::Path filename(Common::String::format("asound.ph%d", i));
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

ASound2::ASound2(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph2", 0) {
}

int ASound2::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound3::ASound3(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph3", 0) {
}

int ASound3::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound4::ASound4(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph4", 0) {
}

int ASound4::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound5::ASound5(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph5", 0) {
}

int ASound5::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

const ASound9::CommandPtr ASound9::_commandList[72] = {
	&ASound9::command0,  &ASound9::command1,  &ASound9::command2,  &ASound9::command3,
	&ASound9::command4,  &ASound9::command5,  &ASound9::command6,  &ASound9::command7,
	&ASound9::command8,  nullptr,             nullptr,             nullptr,
	nullptr,             nullptr,             nullptr,             nullptr,
	&ASound9::command16, nullptr,             nullptr,             nullptr,
	nullptr,             nullptr,             nullptr,             nullptr,
	&ASound9::command24, &ASound9::command25, &ASound9::command26, &ASound9::command27,
	nullptr,             nullptr,             nullptr,             nullptr,
	&ASound9::command32, &ASound9::command33, &ASound9::command34, &ASound9::command35,
	&ASound9::command36, &ASound9::command37, &ASound9::command38, &ASound9::command39,
	nullptr,             nullptr,             nullptr,             nullptr,
	nullptr,             nullptr,             nullptr,             nullptr,
	nullptr,             nullptr,             nullptr,             nullptr,
	nullptr,             nullptr,             nullptr,             nullptr,
	nullptr,             nullptr,             nullptr,             nullptr,
	nullptr,             nullptr,             nullptr,             nullptr,
	&ASound9::command64, &ASound9::command65, &ASound9::command66, &ASound9::command67,
	&ASound9::command68, &ASound9::command69, &ASound9::command70, &ASound9::command71
};

ASound9::ASound9(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph9", 0) {
}

int ASound9::command(int commandId, int param) {
	if (commandId > 71 || !_commandList[commandId])
		return 0;

	_commandParam = param;
	_frameCounter = 0;
	return (this->*_commandList[commandId])();
}

int ASound9::command0() {
	return 0;
}

int ASound9::command1() {
	return 0;
}

int ASound9::command2() {
	return 0;
}

int ASound9::command3() {
	return 0;
}

int ASound9::command4() {
	return 0;
}

int ASound9::command5() {
	return 0;
}

int ASound9::command6() {
	return 0;
}

int ASound9::command7() {
	return 0;
}

int ASound9::command8() {
	return 0;
}

int ASound9::command16() {
	return 0;
}

int ASound9::command24() {
	return 0;
}

int ASound9::command25() {
	return 0;
}

int ASound9::command26() {
	return 0;
}

int ASound9::command27() {
	return 0;
}

int ASound9::command32() {
	return 0;
}

int ASound9::command33() {
	return 0;
}

int ASound9::command34() {
	return 0;
}

int ASound9::command35() {
	return 0;
}

int ASound9::command36() {
	return 0;
}

int ASound9::command37() {
	return 0;
}

int ASound9::command38() {
	return 0;
}

int ASound9::command39() {
	return 0;
}

int ASound9::command64() {
	return 0;
}

int ASound9::command65() {
	return 0;
}

int ASound9::command66() {
	return 0;
}

int ASound9::command67() {
	return 0;
}

int ASound9::command68() {
	return 0;
}

int ASound9::command69() {
	return 0;
}

int ASound9::command70() {
	return 0;
}

int ASound9::command71() {
	return 0;
}

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS
