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

#ifndef LEGEND_GAMES_GATEWAY_CORE_PERSISTED_H
#define LEGEND_GAMES_GATEWAY_CORE_PERSISTED_H

#include "legend/early/core/persisted.h"

namespace Legend {
namespace Early {
namespace Gateway {

class STRUC24 {
public:
	uint16 _dummy1 = 0;
	uint16 _dummy2 = 0;
	uint16 _dummy3 = 0;
	uint16 _dummy4 = 0;
	uint16 _dummy5 = 0;
	uint16 _dummy6 = 0;
	uint16 _dummy7 = 0;
	uint16 _dummy8 = 0;
	uint16 _dummy9 = 0;
	uint16 _dummy10 = 0;
	uint16 _dummy11 = 0;
	uint16 _dummy12 = 0;

	void clear();
	void synchronize(Common::Serializer &s);
};

struct STRUC3C {
	int _field0 = 0;
	void synchronize(Common::Serializer &s);
};

struct STRUC20 {
	struct Entry {
		byte _v1;
		uint16 _v2;
	};
	Entry _v1;
	Entry _v2;
	Entry _v3;
	Entry _v4;
	Entry _v5;
	Entry _v6;
	Entry _v7;
	Entry _v8;
};

struct STRUC10 {
	byte _field0 = 0;
	byte _field1 = 1;
	byte _field2 = 1;
	byte _field3 = 1;
	byte _field4 = 1;
	byte _field5 = 1;
	byte _field6 = 1;
	byte _field7 = 1;
	byte _field8 = 1;
	byte _field9 = 0;
	byte _fielda = 0;
	byte _fieldb = 0;
	byte _fieldc = 0;
	byte _fieldd = 1;
	byte _fielde = 0;
	byte _fieldf = 0;
};

struct STRUC16 {
	uint16 _field0 = 0;
	uint16 _field2 = 0;
	uint16 _field4 = 0;
	uint16 _field6 = 0;
	uint16 _field8 = 0;
	uint16 _fielda = 0;
	uint16 _fieldc = 0;
	uint16 _fielde = 0;
	uint16 _field10 = 0;
	uint16 _field12 = 0;
	uint16 _field14 = 0;
};

/**
 * Container for all the fields that are persisted to save files
 */
struct Persisted : public Legend::Early::Persisted {
	uint16 _val1 = 0;
	uint8 _val2 = 1;
	uint16 _val3 = 0;
	uint16 _val4 = 0;
	uint16 _val5 = 0;
	uint16 _val6 = 0;
	uint8 _val7 = 0;
	uint8 _val8 = 0;
	uint16 _val9 = 0;
	uint16 _val10 = 0;
	STRUC24 _struc24;
	byte _array1[8];
	uint8 _val11 = 0;
	uint8 _val12 = 1;
	uint16 _val13 = 254;
	uint8 _val14 = 0;
	uint8 _val15 = 0;
	uint8 _val16 = 0;
	uint8 _val17 = 0;
	uint16 _val18 = 0;
	STRUC3C _val19;
	STRUC3C _val20;
	uint16 _val21 = 0;
	uint16 _val22 = 0;
	uint16 _val23 = 0;
	uint8 _val24 = 0;
	uint8 _val25 = 0;
	uint8 _val26 = 0;
	uint8 _val27 = 0;
	uint16 _val28 = 0;
	uint16 _val29 = 0;
	uint16 _val30 = 0;
	uint16 _val31 = 0;
	uint16 _val32 = 0;
	uint8 _val33 = 0;
	uint8 _val34 = 0;
	uint16 _val35 = 0;
	uint16 _val36 = 0;
	uint16 _val37 = 0;
	uint16 _val38 = 0;
	uint16 _val39 = 0;
	uint16 _val40 = 0;
	uint16 _val41 = 0;
	uint16 _val42 = 0;
	uint8 _val43 = 0;
	uint8 _val44 = 0;
	uint8 _val45 = 0;
	uint8 _val46 = 0;
	uint8 _val47 = 0;
	uint16 _val48 = 0;
	uint16 _val49 = 0;
	uint16 _val50 = 0;
	uint8 _val51 = 0;
	uint32 _val52 = 0;
	uint8 _val53 = 0;
	uint8 _val54 = 0;
	uint8 _val55 = 0;
	uint8 _val56 = 0;
	uint8 _val57 = 0;
	uint16 _val58 = 0;
	uint16 _val59[6] = { 71, 72, 73, 73, 71, 72 };
	uint8 _val60 = 0;
	uint8 _val61 = 0;
	uint16 _val62 = 0;
	uint16 _val63 = 0;
	uint16 _val64 = 1;
	uint8 _val65 = 0;
	uint8 _val66 = 0;
	uint16 _val67 = 0;
	uint16 _val68 = 0;
	uint8 _val69[8] = { 71, 0, 72, 133, 74, 0, 73, 0 };
	uint16 _val70 = 0;
	uint8 _val71 = 1;
	uint8 _val72 = 0;
	uint8 _val73 = 0;
	uint16 _val74 = 0;
	uint8 _val75 = 0;
	uint8 _val76 = 0;
	uint8 _val77 = 0;
	uint8 _val78 = 0;
	uint16 _val79 = 0;
	uint8 _val80 = 0;
	uint8 _val81 = 0;
	uint8 _val82 = 0;
	uint16 _val83 = 0;
	uint16 _val84 = 0;
	uint16 _val85 = 0;
	uint8 _val86 = 0;
	uint8 _val87 = 0;
	uint16 _val88 = 0;
	uint16 _val89 = 5;
	uint8 _val90 = 0;
	uint8 _val91 = 0;
	uint8 _val92 = 0;
	uint16 _val93 = 0;
	uint8 _val94 = 0;
	uint8 _val95 = 0;
	uint16 _val96 = 0;
	uint8 _val97 = 0;
	uint8 _val98[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8 _val99 = 0;
	uint16 _val100 = 0;
	uint16 _val101 = 2;
	uint16 _val102 = 1;
	uint8 _val103 = 0;
	uint16 _val104 = 0;
	uint16 _val105 = 0;
	uint8 _val106 = 0;
	uint16 _val107 = 0;
	uint8 _val108 = 0;
	uint8 _val109 = 0;
	uint8 _val110 = 0;
	uint8 _val111 = 0;
	uint8 _val112 = 0;
	uint16 _val113 = 0;
	uint8 _val114 = 0;
	uint8 _val115 = 0;
	uint16 _val116 = 0;
	uint16 _val117 = 0;
	uint8 _val118 = 0;
	uint16 _val119 = 0;
	uint8 _val120 = 0;
	uint8 _val121 = 0;
	uint16 _val122 = 0;
	uint8 _val123 = 0;
	uint8 _val124 = 0;
	uint16 _val125 = 0;
	uint8 _val126 = 0;
	uint16 _val127 = 0;
	uint16 _val128 = 0;
	uint8 _val129[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8 _val130 = 0;
	uint16 _val131 = 0;
	uint16 _val132 = 0;
	uint16 _val133 = 0;
	uint8 _val134 = 0;
	uint16 _val135 = 0;
	STRUC16 _val136;
	uint8 _val137 = 0;
	uint16 _val138 = 0;
	uint16 _val139 = 0;
	uint16 _val140 = 0;
	uint16 _val141 = 0;
	uint8 _val142 = 0;
	uint16 _val143 = 0;
	uint16 _val144 = 0;
	uint8 _val145 = 0;
	uint8 _val146 = 0;
	uint16 _val147 = 657;
	uint16 _val148 = 0;
	uint8 _val149 = 0;
	uint16 _val150[9] = { 445, 0, 7, 453, 0, 4, 448, 0, 7 };
	uint16 _val151 = 0;
	byte _val152[20] = { 0 };
	uint8 _val153 = 0;
	uint8 _val154 = 0;
	uint8 _val155 = 0;
	uint8 _val156 = 0;
	uint8 _val157 = 0;
	uint8 _val158 = 0;
	uint8 _val159 = 0;
	uint8 _val160 = 0;
	uint8 _val161 = 0;
	uint8 _val162 = 0;
	uint8 _val163 = 0;
	uint8 _val164 = 1;
	uint8 _val165 = 0;
	uint8 _val166 = 0;
	uint8 _val167 = 0;
	uint8 _val168 = 0;
	uint8 _val169 = 0;
	uint8 _val170 = 0;
	uint8 _val171 = 1;
	uint8 _val172 = 0;
	uint8 _val173 = 0;
	uint8 _val174 = 0;
	uint16 _val175 = 0;
	uint8 _val176 = 0;
	uint8 _val177 = 0;
	uint8 _val178 = 0;
	uint16 _val179 = 0;
	uint16 _val180 = 0;
	uint16 _val181 = 0;
	uint16 _val182 = 0;
	uint8 _val183 = 1;
	uint8 _val184 = 0;
	uint8 _val185 = 0;
	uint8 _val186 = 0;
	uint16 _val187 = 0;
	uint16 _val188 = 0;
	uint16 _val189 = 0;
	uint32 _val190[10] = {
		141, 142, 143, 144, 145, 146, 147, 150, 151, 152
	};
	uint32 _val191[12] = {
		141, 147, 146, 142, 144, 145, 149, 150, 151, 155, 154, 153
	};
	uint8 _val192[5] = { 0, 0, 0, 0, 0 };
	uint16 _val193 = 0;
	int16 _val194 = -1;
	uint16 _val195 = 0;
	uint16 _val196 = 0;
	uint8 _val197 = 0;
	uint8 _val198 = 0;
	uint8 _val199 = 0;
	uint8 _val200 = 0;
	uint8 _val201 = 0;
	uint8 _val202 = 0;
	uint16 _val203 = 0;
	uint16 _val204 = 0;
	uint16 _val205 = 0;
	uint16 _val206 = 0;
	uint16 _val207 = 0;
	char _val208[8] = { '0', '0', '-', '0', '0', '0', '0', '\0' };
	uint16 _val209 = 0;
	uint16 _val210 = 0;
	uint16 _val211 = 0;
	uint16 _val212 = 0;
	uint8 _random04[5] = { 0 };
	uint32 _val213 = 1500;
	uint32 _val214 = 1500;
	uint16 _val215 = 0;
	uint16 _val216 = 0;
	uint8 _val217[16] = { 0 };
	uint8 _val218[16] = {
		3, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	uint8 _val219[16] = {
		15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
	};
	uint8 _val220[16] = {
		15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
	};
	uint8 _val221 = 1;
	uint16 _val222 = 0;
	uint16 _val223 = 0;
	uint16 _val224 = 0;
	uint16 _val225 = 0;
	uint16 _val226 = 0;
	uint16 _val227 = 0;
	uint8 _val228 = 0;
	uint16 _val229 = 0;
	uint16 _val230 = 0;
	uint16 _val231 = 0;
	uint16 _val232 = 0;
	uint8 _val233[16] = { 0 };
	uint8 _val234[16] = { 0 };
	uint8 _val235[4] = { 0 };
	uint8 _val236 = 0;
	uint8 _val237 = 0;
	STRUC20 _val238;
	byte _val239[5] = { 0 };
	STRUC10 _val240;
	uint16 _val241 = 13;
	uint8 _val242 = 0;
	uint16 _val243 = 0;
	uint16 _val244 = 0;
	byte _val245[12] = { 0 };
	uint16 _val246 = 0;
	uint8 _val247[10] = { 0 };
	uint16 _val248 = 1;
	uint8 _val249 = 0;
	uint16 _val250 = 0;
	int16 _val251 = -1;
	int16 _val252 = -1;
	uint16 _val253 = 0;
	uint8 _val254[17] = { 0 };
	uint8 _val255[17] = { 0 };
	uint8 _val256[17] = { 0 };
	uint16 _val257 = 0;
};

} // namespace Gateway
} // namespace Early
} // namespace Legend

#endif
