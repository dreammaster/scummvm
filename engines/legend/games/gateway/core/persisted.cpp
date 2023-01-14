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

#include "legend/games/gateway/core/persisted.h"

namespace Legend {
namespace Early {
namespace Gateway {

void STRUC24::clear() {
	_dummy1 = 0;
	_dummy2 = 0;
	_dummy3 = 0;
	_dummy4 = 0;
	_dummy5 = 0;
	_dummy6 = 0;
	_dummy7 = 0;
	_dummy8 = 0;
	_dummy9 = 0;
	_dummy10 = 0;
	_dummy11 = 0;
	_dummy12 = 0;
}

void STRUC24::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_dummy1);
	s.syncAsUint16LE(_dummy2);
	s.syncAsUint16LE(_dummy3);
	s.syncAsUint16LE(_dummy4);
	s.syncAsUint16LE(_dummy5);
	s.syncAsUint16LE(_dummy6);
	s.syncAsUint16LE(_dummy7);
	s.syncAsUint16LE(_dummy8);
	s.syncAsUint16LE(_dummy9);
	s.syncAsUint16LE(_dummy10);
	s.syncAsUint16LE(_dummy11);
	s.syncAsUint16LE(_dummy12);
}

void STRUC3C::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_field0);
	s.skip(58);
}

void STRUC16::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_field0);
	s.syncAsUint16LE(_field2);
	s.syncAsUint16LE(_field4);
	s.syncAsUint16LE(_field6);
	s.syncAsUint16LE(_field8);
	s.syncAsUint16LE(_fielda);
	s.syncAsUint16LE(_fieldc);
	s.syncAsUint16LE(_fielde);
	s.syncAsUint16LE(_field10);
	s.syncAsUint16LE(_field12);
	s.syncAsUint16LE(_field14);
}

void STRUC20::Entry::synchronize(Common::Serializer &s) {
	s.syncAsByte(_v1);
	s.skip(1);
	s.syncAsUint16LE(_v2);
}

void STRUC20::synchronize(Common::Serializer &s) {
	_v1.synchronize(s);
	_v2.synchronize(s);
	_v3.synchronize(s);
	_v4.synchronize(s);
	_v5.synchronize(s);
	_v6.synchronize(s);
	_v7.synchronize(s);
	_v8.synchronize(s);
}

void STRUC10::synchronize(Common::Serializer &s) {
	s.syncAsByte(_field0);
	s.syncAsByte(_field1);
	s.syncAsByte(_field2);
	s.syncAsByte(_field3);
	s.syncAsByte(_field4);
	s.syncAsByte(_field5);
	s.syncAsByte(_field6);
	s.syncAsByte(_field7);
	s.syncAsByte(_field8);
	s.syncAsByte(_field9);
	s.syncAsByte(_fielda);
	s.syncAsByte(_fieldb);
	s.syncAsByte(_fieldc);
	s.syncAsByte(_fieldd);
	s.syncAsByte(_fielde);
	s.syncAsByte(_fieldf);
}

void Persisted::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_val1);
	s.syncAsUint16LE(_val2);
	s.syncAsUint16LE(_val3);
	s.syncAsUint16LE(_val4);
	s.syncAsUint16LE(_val5);
	s.syncAsUint16LE(_val6);
	s.syncAsUint16LE(_val7);
	s.syncAsUint16LE(_val8);
	s.syncAsUint16LE(_val9);
	s.syncAsUint16LE(_val10);
	_struc24.synchronize(s);
	s.syncBytes(_array1, 8);
	s.syncAsUint16LE(_val11);
	s.syncAsUint16LE(_val12);
	s.syncAsUint16LE(_val13);
	s.syncAsUint16LE(_val14);
	s.syncAsUint16LE(_val15);
	s.syncAsUint16LE(_val16);
	s.syncAsUint16LE(_val17);
	s.syncAsUint16LE(_val18);
	_val19.synchronize(s);
	_val20.synchronize(s);
	s.syncAsUint16LE(_val21);
	s.syncAsUint16LE(_val22);
	s.syncAsUint16LE(_val23);
	s.syncAsUint16LE(_val24);
	s.syncAsUint16LE(_val25);
	s.syncAsUint16LE(_val26);
	s.syncAsUint16LE(_val27);
	s.syncAsUint16LE(_val28);
	s.syncAsUint16LE(_val29);
	s.syncAsUint16LE(_val30);
	s.syncAsUint16LE(_val31);
	s.syncAsUint16LE(_val32);
	s.syncAsUint16LE(_val33);
	s.syncAsUint16LE(_val34);
	s.syncAsUint16LE(_val35);
	s.syncAsUint16LE(_val36);
	s.syncAsUint16LE(_val37);
	s.syncAsUint16LE(_val38);
	s.syncAsUint16LE(_val39);
	s.syncAsUint16LE(_val40);
	s.syncAsUint16LE(_val41);
	s.syncAsUint16LE(_val42);
	s.syncAsUint16LE(_val43);
	s.syncAsUint16LE(_val44);
	s.syncAsUint16LE(_val45);
	s.syncAsUint16LE(_val46);
	s.syncAsUint16LE(_val47);
	s.syncAsUint16LE(_val48);
	s.syncAsUint16LE(_val49);
	s.syncAsUint16LE(_val50);
	s.syncAsUint16LE(_val51);
	s.syncAsUint32LE(_val52);
	s.syncAsUint16LE(_val53);
	s.syncAsUint16LE(_val54);
	s.syncAsUint16LE(_val55);
	s.syncAsUint16LE(_val56);
	s.syncAsUint16LE(_val57);
	s.syncAsUint16LE(_val58);
	for (int i = 0; i < 3; ++i)
		// Only the first three elements are persisted
		s.syncAsUint16LE(_val59[i]);
	s.syncAsUint16LE(_val60);
	s.syncAsUint16LE(_val61);
	s.syncAsUint16LE(_val62);
	s.syncAsUint16LE(_val63);
	s.syncAsUint16LE(_val64);
	s.syncAsUint16LE(_val65);
	s.syncAsUint16LE(_val66);
	s.syncAsUint16LE(_val67);
	s.syncAsUint16LE(_val68);
	for (int i = 0; i < 8; ++i)
		s.syncAsUint16LE(_val69[i]);
	s.syncAsUint16LE(_val70);
	s.syncAsUint16LE(_val71);
	s.syncAsUint16LE(_val72);
	s.syncAsUint16LE(_val73);
	s.syncAsUint16LE(_val74);
	s.syncAsUint16LE(_val75);
	s.syncAsUint16LE(_val76);
	s.syncAsUint16LE(_val77);
	s.syncAsUint16LE(_val78);
	s.syncAsUint16LE(_val79);
	s.syncAsUint16LE(_val80);
	s.syncAsUint16LE(_val81);
	s.syncAsUint16LE(_val82);
	s.syncAsUint16LE(_val83);
	s.syncAsUint16LE(_val84);
	s.syncAsUint16LE(_val85);
	s.syncAsUint16LE(_val86);
	s.syncAsUint16LE(_val87);
	s.syncAsUint16LE(_val88);
	s.syncAsUint16LE(_val89);
	s.syncAsUint16LE(_val90);
	s.syncAsUint16LE(_val91);
	s.syncAsUint16LE(_val92);
	s.syncAsUint16LE(_val93);
	s.syncAsUint16LE(_val94);
	s.syncAsUint16LE(_val95);
	s.syncAsUint16LE(_val96);
	s.syncAsUint16LE(_val97);
	s.syncBytes(_val98, 12);
	s.syncAsUint16LE(_val99);
	s.syncAsUint16LE(_val100);
	s.syncAsUint16LE(_val101);
	s.syncAsUint16LE(_val102);
	s.syncAsUint16LE(_val103);
	s.syncAsUint16LE(_val104);
	s.syncAsUint16LE(_val105);
	s.syncAsUint16LE(_val106);
	s.syncAsUint16LE(_val107);
	s.syncAsUint16LE(_val108);
	s.syncAsUint16LE(_val109);
	s.syncAsUint16LE(_val110);
	s.syncAsUint16LE(_val111);
	s.syncAsUint16LE(_val112);
	s.syncAsUint16LE(_val113);
	s.syncAsUint16LE(_val114);
	s.syncAsUint16LE(_val115);
	s.syncAsUint16LE(_val116);
	s.syncAsUint16LE(_val117);
	s.syncAsUint16LE(_val118);
	s.syncAsUint16LE(_val119);
	s.syncAsUint16LE(_val120);
	s.syncAsUint16LE(_val121);
	s.syncAsUint16LE(_val122);
	s.syncAsUint16LE(_val123);
	s.syncAsUint16LE(_val124);
	s.syncAsUint16LE(_val125);
	s.syncAsUint16LE(_val126);
	s.syncAsUint16LE(_val127);
	s.syncAsUint16LE(_val128);
	s.syncBytes(_val129, 16);
	s.syncAsUint16LE(_val130);
	s.syncAsUint16LE(_val131);
	s.syncAsUint16LE(_val132);
	s.syncAsUint16LE(_val133);
	s.syncAsUint16LE(_val134);
	s.syncAsUint16LE(_val135);
	_val136.synchronize(s);
	s.syncAsUint16LE(_val137);
	s.syncAsUint16LE(_val138);
	s.syncAsUint16LE(_val139);
	s.syncAsUint16LE(_val140);
	s.syncAsUint16LE(_val141);
	s.syncAsUint16LE(_val142);
	s.syncAsUint16LE(_val143);
	s.syncAsUint16LE(_val144);
	s.syncAsUint16LE(_val145);
	s.syncAsUint16LE(_val146);
	s.syncAsUint16LE(_val147);
	s.syncAsUint16LE(_val148);
	s.syncAsUint16LE(_val149);
	for (int i = 0; i < 9; ++i)
		s.syncAsUint16LE(_val150[i]);
	s.syncAsUint16LE(_val151);
	s.syncBytes(_val152, 20);
	s.syncAsUint16LE(_val153);
	s.syncAsUint16LE(_val154);
	s.syncAsUint16LE(_val155);
	s.syncAsUint16LE(_val156);
	s.syncAsUint16LE(_val157);
	s.syncAsUint16LE(_val158);
	s.syncAsUint16LE(_val159);
	s.syncAsUint16LE(_val160);
	s.syncAsUint16LE(_val161);
	s.syncAsUint16LE(_val162);
	s.syncAsUint16LE(_val163);
	s.syncAsUint16LE(_val164);
	s.syncAsUint16LE(_val165);
	s.syncAsUint16LE(_val166);
	s.syncAsUint16LE(_val167);
	s.syncAsUint16LE(_val168);
	s.syncAsUint16LE(_val169);
	s.syncAsUint16LE(_val170);
	s.syncAsUint16LE(_val171);
	s.syncAsUint16LE(_val172);
	s.syncAsUint16LE(_val173);
	s.syncAsUint16LE(_val174);
	s.syncAsUint16LE(_val175);
	s.syncAsUint16LE(_val176);
	s.syncAsUint16LE(_val177);
	s.syncAsUint16LE(_val178);
	s.syncAsUint16LE(_val179);
	s.syncAsUint16LE(_val180);
	s.syncAsUint16LE(_val181);
	s.syncAsUint16LE(_val182);
	s.syncAsUint16LE(_val183);
	s.syncAsUint16LE(_val184);
	s.syncAsUint16LE(_val185);
	s.syncAsUint16LE(_val186);
	s.syncAsUint16LE(_val187);
	s.syncAsUint16LE(_val188);
	s.syncAsUint16LE(_val189);
	for (int i = 0; i < 10; ++i)
		s.syncAsUint32LE(_val190[i]);
	for (int i = 0; i < 12; ++i)
		s.syncAsUint32LE(_val191[i]);
	for (int i = 0; i < 5; ++i)
		s.syncAsUint16LE(_val192[i]);
	s.syncAsUint16LE(_val193);
	s.syncAsSint16LE(_val194);
	s.syncAsUint16LE(_val195);
	s.syncAsUint16LE(_val196);
	s.syncAsUint16LE(_val197);
	s.syncAsUint16LE(_val198);
	s.syncAsUint16LE(_val199);
	s.syncAsUint16LE(_val200);
	s.syncAsUint16LE(_val201);
	s.syncAsUint16LE(_val202);
	s.syncAsUint16LE(_val203);
	s.syncAsUint16LE(_val204);
	s.syncAsUint16LE(_val205);
	s.syncAsUint16LE(_val206);
	s.syncAsUint16LE(_val207);
	s.syncBytes((byte *)_val208, 8);
	s.syncAsUint16LE(_val209);
	s.syncAsUint16LE(_val210);
	s.syncAsUint16LE(_val211);
	s.syncAsUint16LE(_val212);
	s.syncBytes(_random04, 5);
	s.syncAsUint32LE(_val213);
	s.syncAsUint32LE(_val214);
	s.syncAsUint16LE(_val215);
	s.syncAsUint16LE(_val216);
	s.syncBytes(_val217, 16);
	s.syncBytes(_val218, 16);
	s.syncBytes(_val219, 16);
	s.syncBytes(_val220, 16);
	s.syncAsUint16LE(_val221);
	s.syncAsUint16LE(_val222);
	s.syncAsUint16LE(_val223);
	s.syncAsUint16LE(_val224);
	s.syncAsUint16LE(_val225);
	s.syncAsUint16LE(_val226);
	s.syncAsUint16LE(_val227);
	s.syncAsUint16LE(_val228);
	s.syncAsUint16LE(_val229);
	s.syncAsUint16LE(_val230);
	s.syncAsUint16LE(_val231);
	s.syncAsUint16LE(_val232);
	s.syncBytes(_val233, 16);
	s.syncBytes(_val234, 16);
	for (int i = 0; i < 4; ++i)
		s.syncAsUint16LE(_val235[i]);
	s.syncAsUint16LE(_val236);
	s.syncAsUint16LE(_val237);
	_val238.synchronize(s);
	s.syncBytes(_val239, 5);
	_val240.synchronize(s);
	s.syncAsUint16LE(_val241);
	s.syncAsUint16LE(_val242);
	s.syncAsUint16LE(_val243);
	s.syncAsUint16LE(_val244);
	s.syncBytes(_val245, 12);
	s.syncAsUint16LE(_val246);
	s.syncBytes(_val247, 10);
	s.syncAsUint16LE(_val248);
	s.syncAsUint16LE(_val249);
	s.syncAsUint16LE(_val250);
	s.syncAsSint16LE(_val251);
	s.syncAsSint16LE(_val252);
	s.syncAsUint16LE(_val253);
	s.syncBytes(_val254, 17);
	s.syncBytes(_val255, 17);
	s.syncBytes(_val256, 17);
	s.syncAsUint16LE(_val257);
}

} // namespace Gateway
} // namespace Early
} // namespaceCo Legend
