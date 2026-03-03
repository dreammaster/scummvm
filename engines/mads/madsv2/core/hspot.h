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

#ifndef MADS_CORE_HSPOT_H
#define MADS_CORE_HSPOT_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

#define max_hot_spots   80     /* Size of hotspot list */
#define max_hot_lists   2       /* Maximum # of lists   */

#define HS_ALL          -1      /* Pass in ERASE to wipe entire class */
#define HS_DEATH        -1      /* Used for marking spots to be killed */
#define HS_STACK_SIZE   5       /* Size of stack */

typedef struct {
	int ul_x, ul_y;  /* Upper Left      */
	int lr_x, lr_y;  /* Lower Right     */
	int _class;      /* Clasification   */
	int num;        /* Spot number     */
	int video_mode; /* Spot video mode */
	int active;     /* TRUE if active  */
} Spot;

#define         HS_SIZE         16      /* Size of above structure */


extern Spot spot[max_hot_spots + 1];
extern int numspots;


extern int hotkeys[max_hot_spots + 1];


/* hspot_1.c */
int hspot_add(int ul_x, int ul_y,
	int lr_x, int lr_y,
	int class_, int num, int video_mode);

/* hspot_2.c */
int hspot_push(void);
int hspot_pop(void);

/* hspot_4.c */
int hspot_remove(int class_, int num);

/* hspot_5.c */
void hspot_toggle(int class_, int num, int active);

/* hspot_6.c */
void hspot_wipe(void);
int hspot_key(int key);
int hspot_begin(int x, int y, int class_, int num, int hotkey);
int hspot_end(void);

void hspot_dummy(void);

/* hspot_7.c */
int hspot_which(int x, int y, int video_mode);

/* hspot_8.c */
int hspot_which_reverse(int x, int y, int video_mode);

} // namespace MADSV2
} // namespace MADS

#endif
