/*
/*      kernel_9.c      by Brian Reynolds       16-May-92
*/

#include <stdlib.h>

#include <general.mac>
#include <anim.mac>
#include <matte.h>
#include <error.h>
#include <player.h>
#include <sprite.h>
#include <attr.h>

#include "kernel.mac"
#include "kernel_1.h"
#include "kernel_8.h"
#include "kernel_c.h"
#include "kernel_f.h"

#pragma optimize ("weglt", on)


void fastcall kernel_seq_delete (int sequence_id)
{
  if (sequence_list[sequence_id].active_flag) {
    if (sequence_list[sequence_id].dynamic_hotspot >= 0) {
      kernel_delete_dynamic(sequence_list[sequence_id].dynamic_hotspot);
    }
  }

  sequence_list[sequence_id].active_flag = false;

  if (!sequence_list[sequence_id].expired) {
    kernel_reconstruct_screen (-1);
  } else {
    kernel_seq_purge (sequence_id);
  }
}


void fastcall kernel_seq_image (SequencePtr sequence, ImagePtr image, int sequence_id)
{
  image->flags      = series_list[sequence->series_id]->delta_series ? IMAGE_DELTA : IMAGE_UPDATE;
  image->segment_id = (byte) sequence_id;
  image->series_id  = sequence->series_id;
  image->sprite_id  = sequence->sprite | (sequence->mirror ? MIRROR_MASK : 0);

  image->depth      = sequence->depth;
  image->scale      = sequence->scale;

  if (!sequence->auto_locating) {
    image->x = sequence->x;
    image->y = sequence->y;
  } else {
    image->x = series_list[image->series_id]->index[sequence->sprite - 1].x;
    image->y = series_list[image->series_id]->index[sequence->sprite - 1].y;
  }

  sequence->last_image = *image;
}



static int near fastcall kernel_seq_update (SequencePtr sequence, int sequence_id)
{
  int id;
  int count;
  int cycling = false;
  int trigger = -1;
  int x, y, xs, ys;
  int x1, y1;
  int x2, y2;
  int purged;
  ImagePtr image;

  purged = kernel_seq_purge(sequence_id);

  if (purged >= 0) {
    if (sequence->loop_direction == AA_STAMP) {
      image_list[purged].flags = IMAGE_STATIC;
      goto done;
    }
  }

  if (sequence->expired) {
    kernel_seq_delete (sequence_id);
    goto done;
  } else {
    if (sequence->series_id == KERNEL_SPECIAL_TIMING) {
      sequence->expired = true;
      goto triggers;
    }

    id = matte_allocate_image();
    if (id < 0) {
      sequence->expired = true;
      goto triggers;
    }

    image    = &image_list[id];

    kernel_seq_image (sequence, image, sequence_id);

  }

  if (sequence->motion ||
      (sequence->dynamic_hotspot >= 0)) {
    xs = (series_list[sequence->series_id]->index[sequence->sprite - 1].xs * sequence->scale) / 200;
    ys = (series_list[sequence->series_id]->index[sequence->sprite - 1].ys * sequence->scale) / 100;
    x  = image->x;
    y  = image->y;

    if (sequence->motion & KERNEL_MOTION) {
      sequence->accum_x += sequence->delta_x;
      while (sequence->accum_x >= 100) {
       sequence->accum_x -= 100;
       sequence->x += sequence->sign_x;
      }
      sequence->accum_y += sequence->delta_y;
      while (sequence->accum_y >= 100) {
       sequence->accum_y -= 100;
       sequence->y += sequence->sign_y;
      }

      if (sequence->motion & KERNEL_MOTION_OFFSCREEN) {
       if ( (((x + xs) < 0) || ((x - xs) >= picture_map.total_x_size)) ||
            ((y < 0) || ((y - ys) >= picture_map.total_y_size))) {
         cycling           = true;
         sequence->expired = true;
       }
      }
    }

    if (sequence->dynamic_hotspot >= 0) {
      x1 = x - xs;
      x2 = x + xs;
      y1 = y - ys;
      y2 = y;
      x1 = max (0, x1);
      y1 = max (0, y1);
      x2 = min (picture_map.total_x_size - 1, x2);
      y2 = min (picture_map.total_y_size - 1, y2);
      kernel_dynamic_hot[sequence->dynamic_hotspot].x = x1;
      kernel_dynamic_hot[sequence->dynamic_hotspot].y = y1;
      kernel_dynamic_hot[sequence->dynamic_hotspot].xs= (x2 - x1) + 1;
      kernel_dynamic_hot[sequence->dynamic_hotspot].ys= (y2 - y1) + 1;
      kernel_dynamic_hot[sequence->dynamic_hotspot].valid = true;
      kernel_dynamic_changed = true;
    }
  }

  if (sequence->start_sprite != sequence->end_sprite) {
    sequence->sprite += sequence->loop_direction;
  }

  if (sequence->sprite < sequence->start_sprite) {
    cycling = true;
    if (sequence->loop_mode == AA_PINGPONG) {
      sequence->sprite         = sequence->start_sprite + 1;
      sequence->loop_direction = 1;
    } else {
      sequence->sprite         = sequence->end_sprite;
    }
  } else if (sequence->sprite > sequence->end_sprite) {
    cycling = true;
    if (sequence->loop_mode == AA_PINGPONG) {
      sequence->sprite         = sequence->end_sprite - 1;
      sequence->loop_direction = -1;
    } else {
      sequence->sprite         = sequence->start_sprite;
    }
  }

  if (cycling) {
    if (sequence->expire) {
      sequence->expire--;
      if (!sequence->expire) {
       sequence->expired = true;
      }
    }
  }

triggers:
  for (count = 0; count < (int) sequence->num_triggers; count++) {
    switch(sequence->trigger_type[count]) {
      case KERNEL_TRIGGER_EXPIRE:
       if (sequence->expired) trigger = count;
       break;
      case KERNEL_TRIGGER_LOOP:
       if (cycling) trigger = count;
       break;
      case KERNEL_TRIGGER_SPRITE:
      default:
       if ((sequence->sprite == sequence->trigger_sprite[count]) ||
           (sequence->trigger_sprite[count] == 0)) {
         trigger = count;
       }
       break;
    }
  }

  if (trigger >= 0) {
    kernel.trigger      = sequence->trigger_code[trigger];
    kernel.trigger_mode = sequence->trigger_dest;
    if (kernel.trigger_mode != KERNEL_TRIGGER_DAEMON) {
      for (count = 0; count < 3; count++) {
       player2.words[count] = sequence->trigger_words[count];
      }
    }
  }

  if (sequence->series_id == KERNEL_SPECIAL_TIMING) {
    sequence->active_flag = false;
  }

done:
  return (cycling);
}



void fastcall kernel_seq_update_all (void)
{
  int count;
  int ok_to_update;
  SequencePtr sequence;

  for (count = 0; (count < KERNEL_MAX_SEQUENCES); count++) {
    sequence = &sequence_list[count];
    if (sequence->active_flag) {
      if (kernel.clock >= sequence->base_time) {
       ok_to_update = (kernel.fx         || !kernel.trigger ||
                       sequence->expired || !sequence->num_triggers);
       if (ok_to_update) {
         sequence->base_time = kernel.clock + sequence->ticks;
         if (kernel_seq_update (sequence, count)) {
           sequence->base_time += sequence->interval_ticks;
         }
       }
      }
    }
  }
}








