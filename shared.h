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

#ifndef AESOP_SHARED_H
#define AESOP_SHARED_H

namespace Aesop {

#define FK_1      0x3b00
#define FK_2      0x3c00
#define FK_3      0x3d00
#define FK_4      0x3e00
#define FK_5      0x3f00
#define FK_6      0x4000
#define FK_7      0x4100
#define FK_8      0x4200
#define FK_9      0x4300
#define FK_10     0x4400

#define SFK_1     0x5400
#define SFK_2     0x5500
#define SFK_3     0x5600
#define SFK_4     0x5700
#define SFK_5     0x5800
#define SFK_6     0x5900
#define SFK_7     0x5a00
#define SFK_8     0x5b00
#define SFK_9     0x5c00
#define SFK_10    0x5d00

#define GRK_INS   0x52e0
#define GRK_DEL   0x53e0
#define GRK_HOME  0x47e0
#define GRK_END   0x4fe0
#define GRK_PGUP  0x49e0
#define GRK_PGDN  0x51e0
#define GRK_UP    0x48e0
#define GRK_DN    0x50e0
#define GRK_L     0x4be0
#define GRK_R     0x4de0

#define KP_INS    0x5200
#define KP_DEL    0x5300
#define KP_HOME   0x4700
#define KP_END    0x4f00
#define KP_PGUP   0x4900
#define KP_PGDN   0x5100
#define KP_UP     0x4800
#define KP_DN     0x5000
#define KP_L      0x4b00
#define KP_R      0x4d00

#define KP_5      0x4cf0

#define K_CR      0x000d
#define K_ESC     0x001b
#define K_BS      0x0008

	//ллллллллллллллллллллллллллллллллллллллллллл
	//лл                                       лл
	//лл System and input event types          лл
	//лл                                       лл
	//ллллллллллллллллллллллллллллллллллллллллллл

#define SYS_FREE                  0
#define SYS_TIMER                 1
#define SYS_MOUSEMOVE             2
#define SYS_ENTER_REGION          3
#define SYS_LEAVE_REGION          4
#define SYS_LEFT_CLICK            5
#define SYS_LEFT_RELEASE          6
#define SYS_RIGHT_CLICK           7
#define SYS_RIGHT_RELEASE         8
#define SYS_CLICK                 9
#define SYS_RELEASE               10
#define SYS_LEFT_CLICK_REGION     11
#define SYS_LEFT_RELEASE_REGION   12
#define SYS_RIGHT_CLICK_REGION    13
#define SYS_RIGHT_RELEASE_REGION  14
#define SYS_CLICK_REGION          15
#define SYS_RELEASE_REGION        16
#define SYS_KEYDOWN               17

#define NUM_EVTYPES               256

#define FIRST_INPUT_EVENT         2
#define LAST_INPUT_EVENT          17

#define FIRST_SYS_EVENT           0
#define LAST_SYS_EVENT            31

#define FIRST_APP_EVENT           32
#define LAST_APP_EVENT            (NUM_EVTYPES-1)

	//ллллллллллллллллллллллллллллллллллллллллллл
	//лл                                       лл
	//лл Shared application equates            лл
	//лл                                       лл
	//ллллллллллллллллллллллллллллллллллллллллллл

#define NUM_OBJECTS   2048 // Up to 2,048 objects in Eye III universe
#define NUM_ENTITIES  2000 // Up to 2,000 physical entities in Eye III universe

#define FIRST_ITEM    0    // Object # range for global "items"
#define LAST_ITEM     999

#define FIRST_LVL_OBJ 1000 // Object # range for level features & monsters
#define LAST_LVL_OBJ  1999

#define FIRST_PC      32   // Object # range for player characters
#define LAST_PC       47

#define FIRST_D_ITM   48   // Object # range for spells/temporary objects
#define LAST_D_ITM    99

#define FIRST_D_NPC 1990   // Object # range for generated monsters
#define LAST_D_NPC  1999

#define NUM_SAVEGAMES 12   // 12 possible savegame positions
#define SAVE_LEN      26   // Max. # of characters in savegame title

#define NUM_LEVELS    14   // # of "levels" (feature/monster regions) in game

#define bootstrap     2000 // Object which launches AESOP application

	//
	// Names of four basic palette regions
	//

#define PAL_FIXED 0     // Fixed color palette = colors 00-AF
#define PAL_WALLS 1     // Wallset palette     = colors B0-BF
#define PAL_M1    2     // Monster palette #1  = colors C0-DF
#define PAL_M2    3     // Monster palette #2  = colors E0-FF
#define PAL_OUT   4     // Outtake palette     = colors B0-FF

#define F_BLU     11    // illumination value to render FIXED colors in blues
#define F_GRN     12    // illumination value to render FIXED colors in greens
#define F_RED     13    // illumination value to render FIXED colors in reds
#define F_GRY     14    // illumination value to render FIXED colors in grays

#define M_GRY     11    // illumination value to render M1/M2 colors in grays
#define M_WHT     12    // illumination value to render M1/M2 colors in white
#define M_GRN     13    // illumination value to render M1/M2 colors in greens
#define M_BLU     14    // illumination value to render M1/M2 colors in blues
#define M_BRN     15    // illumination value to render M1/M2 colors in browns

	//
	// Fixed palette color names
	//

#define XCOLOR    0
#define DK_GRN    1
#define LT_GRN    2
#define GRN       2
#define YEL       3
#define LT_RED    4
#define DK_RED    6
#define RED       4
#define BLU       8
#define BLK       10
#define WHT       11
#define VIO       0x80
#define CYN       0x57
#define LT_CYN    7
#define LT_BLU    8

#define BRN_1     0x12
#define BRN_2     0x14
#define BRN_3     0x16

	//
	// Other application-specific equates
	//

#define LVL_Y      32   // Max. Y-dimension of level map = 32 (must be 2^^n)
#define LVL_X      32   // Max. X-dimension of level map = 32 (must be 2^^n)

#define DIR_N      0    // Cardinal directions
#define DIR_E      1
#define DIR_S      2
#define DIR_W      3

#define MTYP_INIT  0    // Movement type equates
#define MTYP_TL    1    // Turn left
#define MTYP_F     2    // Forward
#define MTYP_TR    3    // Turn right
#define MTYP_L     4    // Slide left
#define MTYP_B     5    // Retreat
#define MTYP_R     6    // Slide right
#define MTYP_ML    7    // Left maze passage
#define MTYP_MM    8    // Middle maze passage
#define MTYP_MR    9    // Right maze passage

#define GRAVITY    5    // Needed by explosion/vortex resources
#define FRICTION   1
#define MAXDOTS    150
#define ACCUR	    6

#define EYE2_FLAGS 0    // Flags used by item transfer routines
#define EYE2_TYPE  1
#define EYE2_SPEC  2

} // End of namespace Aesop

#endif
