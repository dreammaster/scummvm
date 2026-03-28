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

#ifndef MADS_PHANTOM_MADS_TEXT_H
#define MADS_PHANTOM_MADS_TEXT_H

#include "common/scummsys.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {

/* Formula: text_RRR_NN = RRR * 100 + NN */

enum {
	/* Room 101 */
	text_101_10 = 10110,
	text_101_11 = 10111,
	text_101_12 = 10112,
	text_101_13 = 10113,
	text_101_14 = 10114,
	text_101_15 = 10115,
	text_101_16 = 10116,
	text_101_17 = 10117,
	text_101_18 = 10118,
	text_101_19 = 10119,
	text_101_20 = 10120,
	text_101_21 = 10121,
	text_101_22 = 10122,

	/* Room 102 */
	text_102_10 = 10210,
	text_102_11 = 10211,
	text_102_12 = 10212,
	text_102_13 = 10213,
	text_102_14 = 10214,
	text_102_15 = 10215,
	text_102_17 = 10217,
	text_102_18 = 10218,
	text_102_19 = 10219,
	text_102_20 = 10220,
	text_102_21 = 10221,
	text_102_22 = 10222,
	text_102_23 = 10223,
	text_102_24 = 10224,
	text_102_25 = 10225,
	text_102_26 = 10226,
	text_102_27 = 10227,
	text_102_28 = 10228,
	text_102_29 = 10229,
	text_102_30 = 10230,
	text_102_31 = 10231,
	text_102_32 = 10232,

	/* Cross-room */
	text_000_32 = 32,
	text_008_00 = 800,
	text_008_01 = 801,
	text_008_02 = 802,
	text_008_03 = 803,
	text_008_22 = 822,
	text_008_42 = 842,

	/* Room 103 */
	text_103_10 = 10310,
	text_103_11 = 10311,
	text_103_12 = 10312,
	text_103_13 = 10313,
	text_103_14 = 10314,
	text_103_15 = 10315,
	text_103_16 = 10316,
	text_103_17 = 10317,
	text_103_18 = 10318,
	text_103_19 = 10319,
	text_103_20 = 10320,
	text_103_21 = 10321,
	text_103_22 = 10322,
	text_103_23 = 10323,
	text_103_24 = 10324,
	text_103_25 = 10325,
	text_103_26 = 10326,
	text_103_27 = 10327,
	text_103_28 = 10328,
	text_103_29 = 10329,
	text_103_31 = 10331,
	text_103_33 = 10333,
	text_103_35 = 10335,
	text_103_36 = 10336,
	text_103_37 = 10337,
	text_103_38 = 10338,
	text_103_39 = 10339,
	text_103_40 = 10340,
	text_103_41 = 10341,
	text_103_42 = 10342,
	text_103_43 = 10343,
	text_103_44 = 10344,
	text_103_45 = 10345,
	text_103_46 = 10346,
	text_103_47 = 10347,
	text_103_48 = 10348,
	text_103_49 = 10349,
	text_103_50 = 10350,
	text_103_51 = 10351,

	/* Room 104 */
	text_104_10 = 10410,
	text_104_11 = 10411,
	text_104_12 = 10412,
	text_104_13 = 10413,
	text_104_14 = 10414,
	text_104_15 = 10415,
	text_104_16 = 10416,
	text_104_17 = 10417,
	text_104_18 = 10418,
	text_104_19 = 10419,
	text_104_20 = 10420,
	text_104_21 = 10421,
	text_104_22 = 10422,
	text_104_23 = 10423,
	text_104_24 = 10424,
	text_104_25 = 10425,
	text_104_26 = 10426,
	text_104_27 = 10427,
	text_104_28 = 10428,
	text_104_29 = 10429,
	text_104_30 = 10430,
	text_104_32 = 10432,
	text_104_33 = 10433,
	text_104_34 = 10434,
	text_104_35 = 10435,

	/* Room 105 */
	text_105_10 = 10510,
	text_105_11 = 10511,
	text_105_12 = 10512,
	text_105_13 = 10513,
	text_105_14 = 10514,
	text_105_15 = 10515,
	text_105_16 = 10516,
	text_105_17 = 10517,
	text_105_18 = 10518,
	text_105_19 = 10519,
	text_105_20 = 10520,
	text_105_21 = 10521,
	text_105_22 = 10522,
	text_105_23 = 10523,
	text_105_24 = 10524,
	text_105_25 = 10525,
	text_105_26 = 10526,
	text_105_27 = 10527,
	text_105_28 = 10528,
	text_105_29 = 10529,
	text_105_30 = 10530,
	text_105_31 = 10531,
	text_105_32 = 10532,
	text_105_33 = 10533,
	text_105_34 = 10534,
	text_105_35 = 10535,
	text_105_36 = 10536,
	text_105_37 = 10537,
	text_105_38 = 10538,
	text_105_39 = 10539,
	text_105_40 = 10540,

	/* Room 106 */
	text_106_10 = 10610,
	text_106_11 = 10611,
	text_106_12 = 10612,
	text_106_13 = 10613,
	text_106_14 = 10614,
	text_106_15 = 10615,
	text_106_16 = 10616,
	text_106_17 = 10617,
	text_106_18 = 10618,
	text_106_19 = 10619,
	text_106_20 = 10620,
	text_106_21 = 10621,
	text_106_22 = 10622,
	text_106_23 = 10623,
	text_106_24 = 10624,
	text_106_25 = 10625,
	text_106_26 = 10626,
	text_106_27 = 10627,
	text_106_28 = 10628,
	text_106_29 = 10629,
	text_106_30 = 10630,
	text_106_32 = 10632,
	text_106_33 = 10633,
	text_106_34 = 10634,
	text_106_35 = 10635,
	text_106_36 = 10636,
	text_106_37 = 10637,
	text_106_38 = 10638,
	text_106_39 = 10639
};

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS

#endif
