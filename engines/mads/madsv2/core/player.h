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

#ifndef MADS_CORE_PLAYER_H
#define MADS_CORE_PLAYER_H

#include "common/scummsys.h"

namespace MADS {
namespace MADSV2 {

#define player_verb             player2.words[0]/* Player's recent verb   */
#define player_main_noun        player2.words[1]/* Player's recent noun 1 */
#define player_second_noun      player2.words[2]/* Player's recent noun 2 */

#define PLAYER_VERTICAL         1               /* Player moving vertically   */
#define PLAYER_DIAGONAL         2               /* Player moving diagonally   */
#define PLAYER_HORIZONTAL       3               /* Player moving horizontally */

#define PLAYER_DIAGONAL_THRESHOLD  141          /* sqrt(2) * 100 */

#define PLAYER_MAX_ROOMS        120             /* Max rooms player can go to */

#define PLAYER_MAX_STOP         12              /* Max items in stop-walker stack */


typedef struct {
	int walking;                  /* Player is "on the move"                 */
	int x, y;                     /* Player's current screen location        */
	int target_x, target_y;       /* Player's screen destination             */
	int sign_x, sign_y;           /* Axis pixel signs of movement            */
	int x_count, y_count;         /* Internal motion tracking info           */
	int x_counter, y_counter;     /* Internal motion tracking info           */
	int target_facing;            /* Facing to assume at destination         */
	int special_code;             /* Special code currently on top of        */
	int sprite_changed;           /* Flag if sprite changes this frame       */
	int frame_delay;              /* Tick delay between player frames        */
	int center_of_gravity;        /* Center of gravity displacement          */

	int walk_freedom;             /* Player can always walk anywhere         */

	int walk_anywhere;            /* Player can walk anywhere                */

	int walk_off_edge_to_room;    /* Player should walk off edge unless told */
	/* otherwise.                              */
	int walk_off_edge;            /* Player walking off edge of screen       */

	int need_to_walk;             /* Player needs to walk somewhere          */
	int ready_to_walk;            /* Player is ready to perform that walk    */
	int prepare_walk_facing;      /* Facing preparing to walk to             */
	int prepare_walk_x;           /* Destination preparing to walk to        */
	int prepare_walk_y;

	int commands_allowed;         /* Flag if accepting player input          */
	int walker_visible;           /* Flag if player's sprite is visible      */
	int walker_previously_visible;/* Flag if player's sprite was visible     */
	int series_base;              /* Lowest series list handle for walker    */
	int available[8];             /* Flag if series are available or mirrored*/
	int facing;                   /* Player's current directional facing     */
	int turn_to_facing;           /* Player is turning to this facing        */
	int series;                   /* Player's current active series #        */
	int mirror;                   /* Flag if player series is now mirrored   */
	int sprite;                   /* Current sprite # being displayed        */

	byte depth;                   /* Depth of current player image           */
	byte scale;                   /* Scale of current player image           */

	int stop_walker_sequence;     /* Active stop-walker sequence number.     */
	int stop_walker_stack[PLAYER_MAX_STOP];   /* Stack of future stop walkers*/
	int stop_walker_trigger[PLAYER_MAX_STOP]; /* Trigger codes when complete */
	int stop_walker_pointer;                  /* Stack pointer               */

	int upcoming_trigger;         /* Impending stop-walker trigger code      */
	int trigger;                  /* Active stop-walker trigger code         */

	int next_special_code;        /* Next special code player will pass over */

	int scaling_velocity;         /* Flag if scaling player velocity         */
	int pixel_accum;              /* Internal bresenham motion tracking      */
	int dist_accum;               /* Internal bresenham distance tracking    */
	word delta_distance;          /* Internal bresenham distance tracking    */
	word total_distance;          /* Internal total distance current segment */
	int velocity;                 /* Player movement velocity (pix/frame)    */
	int high_sprite;              /* Highest sprite # in primary progression */

	int command_ready;            /* Flag if a player command is ready to go */
	int command_error;            /* Flag if player command is an error      */
	int command_source;           /* Interface stroke type for player verb   */
	int command;
	int main_object;
	int second_object;
	int main_object_source;       /* Interface stroke type for player noun 1 */
	int second_object_source;     /* Interface stroke type for player noun 2 */
	int prep;                     /* Preposition                             */
	int look_around;              /* Flag for special "look around" command  */

	int main_syntax;              /* Syntax of player's main noun            */
	int second_syntax;            /* Syntax of player's second noun          */

	char series_name[20];         /* Major series name for player walker     */

	char sentence[64];            /* Player's most recent sentence.          */

	long clock;                   /* Timing clock for player frame rate      */

	byte been_here_before;        /* Flag if player has been here before     */
	int  num_rooms_been_in;       /* # of rooms player has been in           */
	int  rooms_been_in[PLAYER_MAX_ROOMS]; /* List of rooms player has been in*/

	int  num_series;              /* Number of player series                 */
	byte walker_loads_first;      /* Walker should be loaded first           */
	byte walker_loaded_first;     /* Walker walker is loaded first           */
	byte walker_is_loaded;        /* Flag if walker is loaded                */
	byte walker_must_reload;      /* Flag if walker must reload              */

	int walker_been_visible;      /* Flag if has been visible this room      */

	byte force_series;            /* Flag to force player series             */

	int walk_trigger;             /* Trigger when reach destination          */
	byte walk_trigger_dest;       /* Type of code to activate for trigger    */
	int walk_trigger_words[3];    /* Vocabulary words for reactivating parser*/

	int enable_at_target;         /* Enable commands at walk target          */

} Player;


typedef struct {
	int words[3];                 /* Vocab word array of player's sentence   */
} Player2;


extern Player  player;
extern Player2 player2;

extern byte player_facing_to_series[10];
extern byte player_clockwise[10];
extern byte player_counter_clockwise[10];


/* player.h */
#define player_said_1(aa)         (player_parse (words_##aa , 0) )
#define player_said_2(aa, bb)     (player_parse (words_##aa, words_##bb, 0) )
#define player_said_3(aa, bb, cc) (player_parse (words_##aa, words_##bb, words_##cc, 0) )


/* player_1.c */
void player_new_stop_walker(void);
void player_stationary_update(void);
void player_set_facing(void);
void player_set_final_facing(void);
void player_select_series(void);
void player_set_sprite(void);
void player_keep_walking(void);
int  player_search_image(void);
void player_set_image(void);

/* player_2.c */
int  player_load_series(char *name);
void player_himem_preload(char *name, int level);
void player_dump_walker(void);
void player_preserve_palette(void);

/* player_3.c */
void player_new_command(void);
void player_new_walk(void);
int           player_parse(int vocab_word, ...);

/* player_4.c */
void player_cancel_walk(void);
void player_cancel_command(void);

/* player_5.c */
int  player_has_been_in_room(int id);
void player_discover_room(int id);

/* player_6.c */
int player_has(int object_id);

/* player_7.c */
void player_clear_stop_walkers(void);
void player_init(void);
void player_start_walking(int walk_x, int walk_y,
	int walk_facing);

/* player_7.c */
int  player_add_stop_walker(int walker, int trigger);
void player_walk(int x, int y, int facing);
void player_walk_trigger(int trigger);

/* player_7.c */
void player_demand_facing(int facing);
void player_demand_location(int x, int y);

/* player_7.c */
void player_first_walk(int from_x, int from_y, int from_facing,
	int to_x, int to_y, int to_facing,
	int enable_at_target);

} // namespace MADSV2
} // namespace MADS

#endif
