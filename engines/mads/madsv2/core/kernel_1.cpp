/*
/*      kernel_1.c      by Brian Reynolds       5-Nov-91
*/

#include <general.mac>
#include <room.mac>
#include <color.mac>

#include "kernel.mac"

RoomPtr room     = NULL;
int room_id      = KERNEL_STARTING_GAME;
int section_id   = KERNEL_STARTING_GAME;
int room_variant = 0;

int new_room     = 101;
int new_section  = 1;

int previous_room    = 0;
int previous_section = 0;

int kernel_initial_variant = 0;

HotPtr room_spots     = NULL;
int    room_num_spots = 0;

int kernel_room_series_marker = 0;

int kernel_room_bound_dif;
int kernel_room_scale_dif;

int kernel_allow_peel = false;

int kernel_panning_speed = 0;
int kernel_screen_fade   = 0;

Animation kernel_anim[KERNEL_MAX_ANIMATIONS];

/*
AnimPtr kernel_animation        = NULL;
int     kernel_animation_cycled = false;
int     kernel_repeat_animation = false;

int     kernel_animation_sprite_loaded;
int     kernel_animation_buffer_id;
byte *kernel_animation_buffer[2];

int     kernel_animation_messages;

int     kernel_animation_frame;
int     kernel_animation_image;
int     kernel_animation_doomed = false;
int     kernel_animation_trigger_code;
int     kernel_animation_trigger_mode;
int     kernel_animation_trigger_words[3];
long    kernel_animation_next_clock;
*/

ShadowList kernel_shadow_main  = { 0 };
ShadowList kernel_shadow_inter = { 1, { 15 } };

int kernel_ok_to_fail_load = false;

byte kernel_mode = KERNEL_GAME_LOAD;

char kernel_cheating_password[64];
int  kernel_cheating_allowed   = 0;
int  kernel_cheating_forbidden = 0;

KernelDynamicHotSpot kernel_dynamic_hot[KERNEL_MAX_DYNAMIC];
int kernel_num_dynamic     = 0;
int kernel_dynamic_changed = 0;


SeriesPtr cursor = NULL;                /* Mouse cursor series */

int cursor_id   = 1;
int cursor_last = -1;

Kernel     kernel;                      /* Kernel data            */
KernelGame game;                        /* Kernel level game data */

