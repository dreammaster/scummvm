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

#ifndef WASTELAND_FOD_LOGIC_ACTION_H
#define WASTELAND_FOD_LOGIC_ACTION_H

#include "wasteland/fod/data/party.h"

namespace Wasteland {
namespace FOD {
namespace Logic {

class MapAction {
	typedef void(MapAction:: *OpcodeFunction)(int params[]);
private:
	static const OpcodeFunction OPCODE_FUNCTIONS[76];
	const uint16 *_idPtr = nullptr;
	int _actionNum = 0;
	int _charNum = 0;
	int _partyNum = 0;
	bool _redrawFlag = false;
	int _val2 = 0;
	Data::PartyMember *_partyMember = nullptr;

	bool mapActionInner(const uint16 *idPtr, int action, int partyNum, int charNum, int arg4);
	void mapActionDone();
	void setPartyMember(int partyNum);
	void readParams(const byte *&scriptP, int *dest, int action);

private:
	void opcode00(int params[]);
	void opcode01(int params[]);
	void opcode02(int params[]);
	void opcode03(int params[]);
	void opcode04(int params[]);
	void opcode05(int params[]);
	void opcode06(int params[]);
	void opcode07(int params[]);
	void opcode08(int params[]);
	void opcode09(int params[]);
	void opcode10(int params[]);
	void opcode11(int params[]);
	void opcode12(int params[]);
	void opcode13(int params[]);
	void opcode14(int params[]);
	void opcode15(int params[]);
	void opcode16(int params[]);
	void opcode17(int params[]);
	void opcode18(int params[]);
	void opcode19(int params[]);
	void opcode20(int params[]);
	void opcode21(int params[]);
	void opcode22(int params[]);
	void opcode23(int params[]);
	void opcode24(int params[]);
	void opcode25(int params[]);
	void opcode26(int params[]);
	void opcode27(int params[]);
	void opcode28(int params[]);
	void opcode29(int params[]);
	void opcode30(int params[]);
	void opcode31(int params[]);
	void opcode32(int params[]);
	void opcode33(int params[]);
	void opcode34(int params[]);
	void opcode35(int params[]);
	void opcode36(int params[]);
	void opcode37(int params[]);
	void opcode38(int params[]);
	void opcode39(int params[]);
	void opcode40(int params[]);
	void opcode41(int params[]);
	void opcode42(int params[]);
	void opcode43(int params[]);
	void opcode44(int params[]);
	void opcode45(int params[]);
	void opcode46(int params[]);
	void opcode47(int params[]);
	void opcode48(int params[]);
	void opcode49(int params[]);
	void opcode50(int params[]);
	void opcode51(int params[]);
	void opcode52(int params[]);
	void opcode53(int params[]);
	void opcode54(int params[]);
	void opcode55(int params[]);
	void opcode56(int params[]);
	void opcode57(int params[]);
	void opcode58(int params[]);
	void opcode59(int params[]);
	void opcode60(int params[]);
	void opcode61(int params[]);
	void opcode62(int params[]);
	void opcode63(int params[]);
	void opcode64(int params[]);
	void opcode65(int params[]);
	void opcode66(int params[]);
	void opcode67(int params[]);
	void opcode68(int params[]);
	void opcode69(int params[]);
	void opcode70(int params[]);
	void opcode71(int params[]);
	void opcode72(int params[]);
	void opcode73(int params[]);
	void opcode74(int params[]);
	void opcode75(int params[]);

protected:
	void doMapAction(const uint16 *idPtr, int action, int arg2);
};

} // namespace Logic
} // namespace FOD
} // namespace Wasteland

#endif
