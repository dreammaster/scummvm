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

ASound1::ASound1(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph1", 0x21e0) {
}

int ASound1::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound2::ASound2(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph2", 0x2040) {
}

int ASound2::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound3::ASound3(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph3", 0x20c0) {
}

int ASound3::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound4::ASound4(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph4", 0x1f90) {
}

int ASound4::command(int commandId, int param) {
	// TODO
	return 0;
}

/*-----------------------------------------------------------------------*/

ASound5::ASound5(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph5", 0x2140) {
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

ASound9::ASound9(Audio::Mixer *mixer, OPL::OPL *opl) : ASound(mixer, opl, "asound.ph9", 0x20c0) {
}

int ASound9::command(int commandId, int param) {
	if (commandId > 71 || !_commandList[commandId])
		return 0;

	_commandParam = param;
	_frameCounter = 0;
	return (this->*_commandList[commandId])();
}

// commands 0-8 delegate to the base ASound implementations
int ASound9::command0() {
	return ASound::command0();
}
int ASound9::command1() {
	return ASound::command1();
}
int ASound9::command2() {
	return ASound::command2();
}
int ASound9::command3() {
	return ASound::command3();
}
int ASound9::command4() {
	return ASound::command4();
}
int ASound9::command5() {
	return ASound::command5();
}
int ASound9::command6() {
	return ASound::command6();
}
int ASound9::command7() {
	return ASound::command7();
}
int ASound9::command8() {
	return ASound::command8();
}

// ---------------------------------------------------------------------------
// Commands 24-27  (asound_command offsets2, range 16-27)
//
// Each call to sub_1039C loads a sound into any available channel from the
// upper three channels (6-8) first.  cx holds a seg001-relative data offset;
// the C++ loadData/playSound API takes the same value as the offset argument
// (the base class adds _dataOffset when seeking the file).
// ---------------------------------------------------------------------------

// asound_command24: loads two percussion-style voices (chimes/bells) into
// the upper channel pool.
int ASound9::command24() {
	playSound(0x203E, 51);	// cx = 0x203E
	playSound(0x2071, 46);	// cx = 0x2071
	return 0;
}

// asound_command25: two more upper-channel voices.
int ASound9::command25() {
	playSound(0x209F, 44);	// cx = 0x209F
	playSound(0x20CB, 46);	// cx = 0x20CB
	return 0;
}

// asound_command26: single upper-channel voice.
int ASound9::command26() {
	playSound(0x20F9, 12);	// cx = 0x20F9
	return 0;
}

// asound_command27: single upper-channel voice.
int ASound9::command27() {
	playSound(0x2105, 81);	// cx = 0x2105
	return 0;
}

// ---------------------------------------------------------------------------
// Commands 32-39  (asound_command offsets3)
//
// Each command calls asound_command1 first (which calls both command3 and
// command5, fading out channels 0-5 and 6-8 respectively), then loads new
// sound data into specific channels via AdlibChannel_load0..8.
//
// asound_command32 and asound_command39 instead use AdlibChannel_loadAny
// (sub_1039C / AdlibChannel_loadAny) which picks the first free channel.
// ---------------------------------------------------------------------------

// asound_command32: eight voices loaded into any available channels.
int ASound9::command32() {
	playSound(0x2B16, 86);
	playSound(0x2B6C, 74);
	playSound(0x2BB6, 722);
	playSound(0x2E88, 16);
	playSound(0x2E98, 11);
	playSound(0x2EA3, 11);
	playSound(0x2EAE, 9);
	playSound(0x2EB7, 15);
	return 0;
}

// asound_command33: no-op in the original (bare retn).
int ASound9::command33() {
	return 0;
}

// asound_command34: seven voices into channels 0-6, and a continuation
// fragment into channel 7.
int ASound9::command34() {
	_channels[0].load(loadData(0x31D0, 81));
	_channels[1].load(loadData(0x3221, 97));
	_channels[2].load(loadData(0x3282, 73));
	_channels[3].load(loadData(0x32CB, 79));
	_channels[4].load(loadData(0x331A, 79));
	_channels[5].load(loadData(0x3369, 71));
	_channels[6].load(loadData(0x33B0, 7));
	_channels[7].load(loadData(0x33B7, 0));	// trailing fragment
	return 0;
}

// asound_command35: seven voices into channels 0-6.
int ASound9::command35() {
	_channels[0].load(loadData(0x295E, 64));
	_channels[1].load(loadData(0x299E, 37));
	_channels[2].load(loadData(0x29C3, 37));
	_channels[3].load(loadData(0x29E8, 94));
	_channels[4].load(loadData(0x2A46, 95));
	_channels[5].load(loadData(0x2AA5, 59));
	_channels[6].load(loadData(0x2AE0, 0));
	return 0;
}

// asound_command36: six voices into channels 0-5.
int ASound9::command36() {
	_channels[0].load(loadData(0x30AA, 51));
	_channels[1].load(loadData(0x30DD, 44));
	_channels[2].load(loadData(0x3109, 52));
	_channels[3].load(loadData(0x313D, 56));
	_channels[4].load(loadData(0x3175, 38));
	_channels[5].load(loadData(0x319B, 0));
	return 0;
}

// asound_command37: seven voices into channels 0-6.
int ASound9::command37() {
	_channels[0].load(loadData(0x2156, 80));
	_channels[1].load(loadData(0x21A6, 232));
	_channels[2].load(loadData(0x228E, 105));
	_channels[3].load(loadData(0x22F7, 90));
	_channels[4].load(loadData(0x2351, 599));
	_channels[5].load(loadData(0x25A8, 791));
	_channels[6].load(loadData(0x28BF, 0));
	return 0;
}

// asound_command38: eight voices into channels 0-7.
// Preceded by asound_command1 (fade-out both banks).
int ASound9::command38() {
	_channels[0].load(loadData(0x11BC, 699));
	_channels[1].load(loadData(0x1477, 278));
	_channels[2].load(loadData(0x158D, 490));
	_channels[3].load(loadData(0x1777, 512));
	_channels[4].load(loadData(0x1977, 590));
	_channels[5].load(loadData(0x1BC5, 314));
	_channels[6].load(loadData(0x1CFF, 432));
	_channels[7].load(loadData(0x1EAF, 0));
	return 0;
}

// asound_command39: six voices loaded into channels 0-5 via AdlibChannel_load.
// Preceded by asound_command0 (full reset), then specific channel loads.
int ASound9::command39() {
	byte *pData = loadData(0x0C36, 0);
	if (!isSoundActive(pData)) {
		stop();
		_channels[0].load(loadData(0x0C36, 311));
		_channels[1].load(loadData(0x0D7D, 211));
		_channels[2].load(loadData(0x0E50, 204));
		_channels[3].load(loadData(0x0F1C, 178));
		_channels[4].load(loadData(0x0FCE, 236));
		_channels[5].load(loadData(0x10BA, 0));
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Commands 64-71  (asound_command offsets4)
//
// Commands 64, 65, 68-71 use sub_1039C (loads into upper channels 6-8 first).
// Commands 66 and 67 load explicitly named channels.
// ---------------------------------------------------------------------------

// asound_command64: single voice into upper channel pool.
int ASound9::command64() {
	playSound(0x2EC6, 20);	// cx = 0x2EC6
	return 0;
}

// asound_command65: single voice into upper channel pool.
int ASound9::command65() {
	playSound(0x2EDA, 10);	// cx = 0x2EDA
	return 0;
}

// asound_command66: eight voices into channels 0-7 in a repeating
// four-voice pattern (0/1/2/3 mirrored onto 4/5/6, then 7 gets the 4th).
int ASound9::command66() {
	_channels[0].load(loadData(0x2EE4, 42));
	_channels[1].load(loadData(0x2F0E, 48));
	_channels[2].load(loadData(0x2F3E, 48));
	_channels[3].load(loadData(0x2F6E, 48));
	_channels[4].load(loadData(0x2EE4, 42));	// mirrors channel 0
	_channels[5].load(loadData(0x2F0E, 48));	// mirrors channel 1
	_channels[6].load(loadData(0x2F3E, 48));	// mirrors channel 2
	_channels[7].load(loadData(0x2F6E, 48));	// mirrors channel 3
	return 0;
}

// asound_command67: three voices into channels 6-8.
int ASound9::command67() {
	_channels[6].load(loadData(0x2F9E, 31));
	_channels[7].load(loadData(0x2FBD, 15));
	_channels[8].load(loadData(0x2FCC, 31));
	return 0;
}

// asound_command68: single voice into upper channel pool.
int ASound9::command68() {
	playSound(0x2FEB, 10);	// cx = 0x2FEB
	return 0;
}

// asound_command69: three voices into upper channel pool.
int ASound9::command69() {
	playSound(0x2FF5, 38);	// cx = 0x2FF5
	playSound(0x301B, 38);	// cx = 0x301B
	playSound(0x3041, 26);	// cx = 0x3041
	return 0;
}

// asound_command70: two voices into upper channel pool.
int ASound9::command70() {
	playSound(0x305B, 9);	// cx = 0x305B
	playSound(0x3064, 9);	// cx = 0x3064
	return 0;
}

// asound_command71: two voices into upper channel pool.
int ASound9::command71() {
	playSound(0x306D, 29);	// cx = 0x306D
	playSound(0x308A, 0);	// cx = 0x308A
	return 0;
}

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS
