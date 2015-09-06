/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

//
// Sound effects/music equates (shared by C and AESOP)
//

#ifndef AESOP_SOUND_H
#define AESOP_SOUND_H

namespace Aesop {

//
// Predefined sound block handles
//

#define BLK_COMMON   0                 // COMMON sounds at EMS blocks 0-8
#define END_COMMON   8

#define BLK_LEVEL    9                 // LEVEL sounds at EMS blocks 9-14
#define END_LEVEL    14

#define FIRST_COMMON 0                 // COMMON sound array indices = 0-49
#define FIRST_LEVEL  50                // LEVEL sound array indices = 50-63

//
// COMMON sound array indices
//

#define S_LAPC_HIT  0
#define S_SWING1    1
#define S_THUMP2    2
#define S_WEASEL    3
#define S_TRNSPTR1  4
#define S_STRAVHIT  5
#define S_MISS7     6
#define S_MISS9     7
#define S_13SLING   8
#define S_PRESSURE  9
#define S_BOWTWANG  10
#define S_TWIGSNAP  11
#define S_CLINK1    12

#define S_FIREMIX1  13
#define S_WHAMMY1   14
#define S_SWARM2    15
#define S_INVISIBL  16
#define S_SPELL1    17
#define S_SPELL10   18
#define S_SPELL100  19
#define S_SPELL101  20
#define S_SPELL11   21
#define S_SPELL12   22
#define S_SPELL15   23
#define S_SPELL2    24
#define S_WATER2A   25
#define S_SPELL22   26
#define S_SPELL24   27
#define S_SPELL25   28
#define S_SPELL26   29
#define S_SPELL28   30
#define S_METALDWN  31
#define S_LGOLOM    32
#define S_CRACKLE2  33
#define S_SPELL38   34
#define S_SPELL44   35
#define S_SPELL46   36
#define S_SPELL52   37
#define S_SPELL66   38
#define S_SPELL54   39
#define S_SPELL64   40
#define S_SPELL81   41
#define S_SPELL87   42
#define S_SPELL95   43
#define S_SPELL98   44

} // End of namesspace Aesop

#endif

