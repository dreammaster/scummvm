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

#ifndef MADS_PHANTOM_SOUND_H
#define MADS_PHANTOM_SOUND_H

#include "mads/core/sound.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {

class PhantomSoundManager : public SoundManager {
protected:
	void loadDriver(int sectionNum) override;

public:
	PhantomSoundManager(Audio::Mixer *mixer, bool &soundFlag) : SoundManager(mixer, soundFlag) {
	}
	~PhantomSoundManager() override {
	}

	void validate() override;
};

class ASound1 : public ASound {
public:
	ASound1(Audio::Mixer *mixer, OPL::OPL *opl);
	~ASound1() override {
	}
	int command(int commandId, int param) override;
};

class ASound2 : public ASound {
public:
	ASound2(Audio::Mixer *mixer, OPL::OPL *opl);
	~ASound2() override {}
	int command(int commandId, int param) override;
};

class ASound3 : public ASound {
public:
	ASound3(Audio::Mixer *mixer, OPL::OPL *opl);
	~ASound3() override {
	}
	int command(int commandId, int param) override;
};

class ASound4 : public ASound {
public:
	ASound4(Audio::Mixer *mixer, OPL::OPL *opl);
	~ASound4() override {
	}
	int command(int commandId, int param) override;
};

class ASound5 : public ASound {
public:
	ASound5(Audio::Mixer *mixer, OPL::OPL *opl);
	~ASound5() override {
	}
	int command(int commandId, int param) override;
};

class ASound9 : public ASound {
private:
	typedef int (ASound9:: *CommandPtr)();
	int command0();
	int command1();
	int command2();
	int command3();
	int command4();
	int command5();
	int command6();
	int command7();
	int command8();

	int command16() {
		return command24();
	}

	int command24();
	int command25();
	int command26();
	int command27();

	int command32();
	int command33();
	int command34();
	int command35();
	int command36();
	int command37();
	int command38();
	int command39();

	int command64();
	int command65();
	int command66();
	int command67();
	int command68();
	int command69();
	int command70();
	int command71();

	static const CommandPtr _commandList[72];

public:
	ASound9(Audio::Mixer *mixer, OPL::OPL *opl);
	~ASound9() override {
	}
	int command(int commandId, int param) override;
};

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS

#endif
