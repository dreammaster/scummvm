/*
/*      kernel_c.c      by Brian Reynolds       13-Jan-92
*/

#include <stdlib.h>
#include <string.h>

#include <general.mac>

#include <anim.h>
#include <player.h>
#include <matte.h>
#include <mem.h>
/* pl #include <sound.h> */
#include <error.h>
#include <buffer.h>
#include <pal.h>
#include <sprite.h>
#include <config.h>
#include <attr.h>
#include <camera.h>

#pragma optimize ("weglt", on)

#include "kernel.mac"
#include "kernel_1.h"
#include "kernel_8.h"
#include "kernel_d.h"
#include "kernel_f.h"

#define MESSAGE_COLOR_3 (((KERNEL_MESSAGE_COLOR_BASE_3 + 1) << 8) + KERNEL_MESSAGE_COLOR_BASE_3)





void fastcall kernel_animation_get_sprite (int handle, int id)
{
  int series_id;
  byte *pointer;

  if (id != kernel_anim[handle].sprite_loaded) {
    series_id = kernel_anim[handle].anim->series_id[kernel_anim[handle].anim->misc_packed_series];
    if (kernel_anim[handle].buffer_id >= 0) {
      pointer = kernel_anim[handle].buffer[kernel_anim[handle].buffer_id];
      kernel_anim[handle].buffer_id = 1 - kernel_anim[handle].buffer_id;
    } else {
      pointer = series_list[series_id]->arena;
    }
    if (sprite_data_load(series_list[series_id], id, pointer)) {
      error_report (ERROR_SPRITE_DATA_LOAD_FAILED, ERROR, MODULE_KERNEL, id, series_id);
    }
    kernel_anim[handle].sprite_loaded = id;
  }
}





void fastcall kernel_reset_animation (int handle, int frame)
{
  if (kernel_anim[handle].anim != NULL) {
    kernel_anim[handle].frame  = frame;
    kernel_anim[handle].image  = 0;
    kernel_anim[handle].doomed = false;
  }
}


static void near fastcall kernel_hot_check (int hot, int id, int seg_id)
{
  int count;
  int x, y, xs, ys;
  int x1, y1, x2, y2;
  byte scale;

  for (count = 0; count < KERNEL_DYNAMIC_MAX_SEGMENTS; count++) {
    if (seg_id == (int)kernel_dynamic_hot[hot].auto_segment[count]) {

      scale = image_list[id].scale;
      if (scale == IMAGE_UNSCALED) {
       xs = series_list[image_list[id].series_id]->index[image_list[id].sprite_id - 1].xs;
       ys = series_list[image_list[id].series_id]->index[image_list[id].sprite_id - 1].ys;
       x  = image_list[id].x;
       y  = image_list[id].y;
       x1 = x;
       y1 = y;
       x2 = x + xs - 1;
       y2 = y + ys - 1;
      } else {
       xs    = (series_list[image_list[id].series_id]->index[image_list[id].sprite_id - 1].xs * image_list[id].scale) / 200;
       ys    = (series_list[image_list[id].series_id]->index[image_list[id].sprite_id - 1].ys * image_list[id].scale) / 100;
       x  = image_list[id].x;
       y  = image_list[id].y;
       x1 = x - xs;
       x2 = x + xs;
       y1 = y - ys;
       y2 = y;
      }

      if ((xs > 0) && (ys > 0)) {
       x1 = max (0, x1);
       y1 = max (0, y1);
       x2 = min (picture_map.total_x_size - 1, x2);
       y2 = min (picture_map.total_y_size - 1, y2);
       xs = (x2 - x1) + 1;
       ys = (y2 - y1) + 1;
       if ((xs > 0) && (ys > 0)) {
         kernel_dynamic_hot[hot].x = x1;
         kernel_dynamic_hot[hot].y = y1;
         kernel_dynamic_hot[hot].xs= xs;
         kernel_dynamic_hot[hot].ys= ys;
         kernel_dynamic_hot[hot].valid = true;
       }
      }
    }
  }
}



static void near fastcall kernel_process_animation (int handle, int asynchronous)
{
  int view_changed = false;
  int image_base;
  int count;
  int match;
  int id;
  int clock_frame;
  int hot, seg_id;
  word temp1, temp2;

  if (kernel_anim[handle].anim->misc_any_packed) {
    id = -1;
    match = kernel_anim[handle].anim->series_id[kernel_anim[handle].anim->misc_packed_series];
    for (count = kernel_anim[handle].image;
        (count < kernel_anim[handle].anim->num_images) &&
        (kernel_anim[handle].anim->image[count].flags <= kernel_anim[handle].frame);
        count++) {
      if (kernel_anim[handle].anim->image[count].series_id == (byte)match) {
       id = kernel_anim[handle].anim->image[count].sprite_id;
      }
    }
    if (id >= 0) {
      kernel_animation_get_sprite(handle, id);
    }
  }

  if (kernel.clock < kernel_anim[handle].next_clock) goto done;

  for (count = 0; count < (int) image_marker; count++) {
    if (image_list[count].segment_id == (byte)(KERNEL_SEGMENT_ANIMATION + handle)) {
      image_list[count].flags = IMAGE_ERASE;
    }
  }

  kernel_anim[handle].cycled = false;
  if (kernel_anim[handle].frame >= kernel_anim[handle].anim->num_frames) {
    if (kernel_anim[handle].repeat) {
      kernel_anim[handle].frame  = 0;
      kernel_anim[handle].image  = 0;
      kernel_anim[handle].cycled = true;
    } else {
      kernel_anim[handle].doomed = true;
      goto done;
    }
  }

  if (!asynchronous) {
    if (kernel_anim[handle].anim->frame[kernel_anim[handle].frame].sound) {
      /* pl sound_play(kernel_anim[handle].anim->frame[kernel_anim[handle].frame].sound); */
    }

    if ((kernel_anim[handle].anim->misc_peel_x != 0) || (kernel_anim[handle].anim->misc_peel_y != 0)) {
      buffer_peel_horiz (&scr_orig, kernel_anim[handle].anim->misc_peel_x);
      buffer_peel_vert  (&scr_orig, kernel_anim[handle].anim->misc_peel_y, NULL, 0);
      matte_refresh_work();
      if (!kernel_allow_peel) error_report (ERROR_PEELING_DISABLED, ERROR, MODULE_KERNEL, handle, 0);
    }

    if (kernel_anim[handle].view_changes) {
      if (kernel_anim[handle].anim->frame[kernel_anim[handle].frame].view_x != (word) picture_view_x) {
       picture_view_x = kernel_anim[handle].anim->frame[kernel_anim[handle].frame].view_x;
       view_changed = true;
      }

      if (kernel_anim[handle].anim->frame[kernel_anim[handle].frame].view_y != (word) picture_view_y) {
       picture_view_y = kernel_anim[handle].anim->frame[kernel_anim[handle].frame].view_y;
       view_changed = true;
      }
    }
  }

  if (view_changed) {
    id = matte_allocate_image();
    image_list[id].segment_id  = KERNEL_SEGMENT_SYSTEM;
    image_list[id].flags       = IMAGE_REFRESH;
    camera_jump_to (picture_view_x, picture_view_y);
  }

  image_base = image_marker;

  hot         = kernel_anim[handle].dynamic_hotspot;
  if (hot >= 0) {
    kernel_dynamic_hot[hot].x = 0;
    kernel_dynamic_hot[hot].y = 0;
    kernel_dynamic_hot[hot].xs= 0;
    kernel_dynamic_hot[hot].ys= 0;
    kernel_dynamic_hot[hot].valid = false;
    kernel_dynamic_changed = true;
  }

  while ((kernel_anim[handle].image < kernel_anim[handle].anim->num_images) &&
        (kernel_anim[handle].anim->image[kernel_anim[handle].image].flags <= kernel_anim[handle].frame)) {
    if (kernel_anim[handle].anim->image[kernel_anim[handle].image].flags == kernel_anim[handle].frame) {
      match = false;
      for (count = 0; !match && (count < image_base); count++) {
       if (image_list[count].segment_id == (byte)(KERNEL_SEGMENT_ANIMATION + handle)) {
         if (_fmemcmp (&image_list[count].series_id,
                       &kernel_anim[handle].anim->image[kernel_anim[handle].image].series_id, 9) == 0) {
           image_list[count].flags = 0;

           if (hot >= 0) {
             seg_id = kernel_anim[handle].anim->image[kernel_anim[handle].image].segment_id;
             kernel_hot_check (hot, count, seg_id);
           }

           match = true;
         }
       }
      }

      if (!match) {
       id = matte_allocate_image();
       image_list[id]             = kernel_anim[handle].anim->image[kernel_anim[handle].image];

       seg_id = image_list[id].segment_id;

       /* image_list[id].segment_id += KERNEL_SEGMENT_ANIMATION; */
       image_list[id].segment_id  = (byte)(KERNEL_SEGMENT_ANIMATION + handle);
       image_list[id].flags       = series_list[image_list[id].series_id]->delta_series ? IMAGE_DELTA : IMAGE_UPDATE;
       /*
       if (kernel_anim[handle].anim->misc_any_packed) {
         if (image_list[id].series_id == (byte)kernel_anim[handle].anim->series_id[kernel_anim[handle].anim->misc_packed_series]) {
           series_id = image_list[id].series_id;
           sprite_data_load (series_list[series_id], image_list[id].sprite_id, series_list[series_id]->arena);
         }
       }
       */

       if (hot >= 0) {
         kernel_hot_check (hot, id, seg_id);
       }
      }
    }
    kernel_anim[handle].image++;
  }

  for (count = 0; count < kernel_anim[handle].anim->num_speech; count++) {
    if ((int)(kernel_anim[handle].anim->speech[count].flags) >= 0) {
      if ((kernel_anim[handle].frame < kernel_anim[handle].anim->speech[count].first_frame) ||
         (kernel_anim[handle].frame > kernel_anim[handle].anim->speech[count].last_frame)) {
       kernel_message_delete (kernel_anim[handle].anim->speech[count].flags);
       kernel_anim[handle].anim->speech[count].flags = -1;
       kernel_anim[handle].messages--;
      }
    } else {
      if ((kernel_anim[handle].frame >= kernel_anim[handle].anim->speech[count].first_frame) &&
         (kernel_anim[handle].frame <= kernel_anim[handle].anim->speech[count].last_frame)) {

       switch (kernel_anim[handle].messages) {
         case 1:
           temp1 = KERNEL_MESSAGE_COLOR_BASE_2;
           break;

         case 2:
           temp1 = KERNEL_MESSAGE_COLOR_BASE;
           break;

         default:
           temp1 = KERNEL_MESSAGE_COLOR_BASE_3;
           break;
       }

       pal_change_color (temp1,
                         kernel_anim[handle].anim->speech[count].color[0].r,
                         kernel_anim[handle].anim->speech[count].color[0].g,
                         kernel_anim[handle].anim->speech[count].color[0].b);

       pal_change_color (temp1 + 1,
                         kernel_anim[handle].anim->speech[count].color[1].r,
                         kernel_anim[handle].anim->speech[count].color[1].g,
                         kernel_anim[handle].anim->speech[count].color[1].b);

       temp2 = ((temp1 + 1) << 8) + temp1;

       kernel_anim[handle].anim->speech[count].flags =
           kernel_message_add (kernel_anim[handle].anim->speech[count].text,
                               kernel_anim[handle].anim->speech[count].x,
                               kernel_anim[handle].anim->speech[count].y,
                               temp2, 9999999, 0, 0);

       kernel_anim[handle].messages++;
      }
    }
  }

  kernel_anim[handle].last_frame = kernel_anim[handle].frame;
  kernel_anim[handle].frame++;

  if (!asynchronous) {
    if (kernel_anim[handle].frame == kernel_anim[handle].anim->num_frames) {
      if (kernel_anim[handle].trigger_code) {
       kernel.trigger      = kernel_anim[handle].trigger_code;
       kernel.trigger_mode = kernel_anim[handle].trigger_mode;
       if (kernel.trigger_mode != KERNEL_TRIGGER_DAEMON) {
         for (count = 0; count < 3; count++) {
           player2.words[count] = kernel_anim[handle].trigger_words[count];
         }
       }
      }
    }
  }

  clock_frame = min (kernel_anim[handle].frame, kernel_anim[handle].anim->num_frames - 1);
  kernel_anim[handle].next_clock = kernel.clock + kernel_anim[handle].anim->frame[clock_frame].ticks;

done:
  ;
}


void fastcall kernel_process_all_animations (void)
{
  int count;
  int ok_to_update;

  for (count = 0; count < KERNEL_MAX_ANIMATIONS; count++) {
    if (kernel_anim[count].anim != NULL) {
      ok_to_update = (kernel.fx || !kernel.trigger ||
                     (kernel_anim[count].frame != kernel_anim[count].anim->num_frames - 1));
      if (ok_to_update) {
       kernel_process_animation (count, false);
      }
    }
  }
}


void fastcall kernel_reconstruct_screen (int anim_handle)
{
  int count;
  int player_found;
  int old_frame;
  long old_clock;

  player_found = false;
  for (count = 0; count < (int)image_marker; count++) {
    if (image_list[count].flags >= 0) {
      if (image_list[count].segment_id == KERNEL_SEGMENT_PLAYER) {
       player_found = true;
      }
    }
  }

  image_marker = 0;
  matte_refresh_work();
  kernel_seq_full_update();

  for (count = 0; count < KERNEL_MAX_ANIMATIONS; count++) {
    if (!kernel_anim[count].doomed) {
      if (kernel_anim[count].anim != NULL) {
       if (count != anim_handle) {
         if (kernel_anim[count].last_frame >= 0) {
           old_frame = kernel_anim[count].frame;
           old_clock = kernel_anim[count].next_clock;
           kernel_anim[count].frame      = kernel_anim[count].last_frame;
           kernel_anim[count].image      = 0;
           kernel_anim[count].next_clock = kernel.clock;

           kernel_process_animation (count, true);

           kernel_anim[count].next_clock = old_clock;
           kernel_anim[count].frame      = old_frame;
           kernel_anim[count].image      = 0;
         }
       }
      }
    }
  }

  if (player.walker_visible && player_found) {
    player.sprite_changed = true;
    player_set_image();
  }
}






void fastcall kernel_abort_animation (int handle)
{
  int count;
  int old_frame;
  long old_clock;

  if (kernel_anim[handle].anim != NULL) {
    if (!kernel_anim[handle].doomed) {

      kernel_reconstruct_screen (handle);

      /*
      image_marker = 0;
      matte_refresh_work();
      kernel_seq_full_update();

      for (count = 0; count < KERNEL_MAX_ANIMATIONS; count++) {
       if (!kernel_anim[count].doomed) {
         if (kernel_anim[count].anim != NULL) {
           if (count != handle) {
             if (kernel_anim[count].last_frame >= 0) {
               old_frame = kernel_anim[count].frame;
               old_clock = kernel_anim[count].next_clock;
               kernel_anim[count].frame      = kernel_anim[count].last_frame;
               kernel_anim[count].image      = 0;
               kernel_anim[count].next_clock = kernel.clock;

               kernel_process_animation (count, true);

               kernel_anim[count].next_clock = old_clock;
               kernel_anim[count].frame      = old_frame;
               kernel_anim[count].image      = 0;
             }
           }
         }
       }
      }

      */
    }

    /*
    if (player.walker_visible) {
      player.sprite_changed = true;
      player_set_image();
    }
    */

    for (count = 0; count < kernel_anim[handle].anim->num_speech; count++) {
      if ((int)(kernel_anim[handle].anim->speech[count].flags) >= 0) {
       kernel_message_delete (kernel_anim[handle].anim->speech[count].flags);
      }
    }

    if (kernel_anim[handle].dynamic_hotspot >= 0) {
      kernel_delete_dynamic(kernel_anim[handle].dynamic_hotspot);
    }

    kernel_anim[handle].repeat = false;
    anim_unload (kernel_anim[handle].anim);
    kernel_anim[handle].anim = NULL;
  }

  kernel_anim[handle].doomed = false;

  go_ahead_and_frag_the_palette();
}


void fastcall kernel_abort_all_animations (void)
{
  int count;

  for (count = KERNEL_MAX_ANIMATIONS - 1; count >= 0; count--) {
    kernel_abort_animation (count);
  }
}


void fastcall kernel_doom_all_animations (void)
{
  int count;

  for (count = 0; count < KERNEL_MAX_ANIMATIONS; count++) {
    kernel_anim[count].doomed = true;
  }
}


void fastcall kernel_abort_doomed_animations (void)
{
  int count;

  for (count = KERNEL_MAX_ANIMATIONS - 1; count >= 0; count--) {
    if (kernel_anim[count].doomed) {
      kernel_abort_animation (count);
    }
  }
}



