/*
/*      kernel_e.c      by Brian Reynolds       16-May-92
*/

#include <stdlib.h>
#include <string.h>

#include <general.mac>
#include <error.h>
#include <matte.h>
#include <font.h>
#include <player.h>

#include "kernel.mac"
#include "kernel_1.h"
#include "kernel_8.h"
#include "kernel_d.h"

#pragma optimize ("weglt", on)


static void near fastcall kernel_message_update (KernelMessagePtr my_message)
{
  int count;
  int x, y;
  int xx, yy;
  int x2;
  int ys;
  int width;
  int matte_id;
  int segment_id;
  int strobe_flag = false;
  int frame;
  int image = -1;
  Animation *anim = NULL;
  SequencePtr sequence = NULL;
  ImagePtr image_list = NULL;

  if (my_message->flags & KERNEL_MESSAGE_EXPIRED) {
    matte_clear_message (my_message->matte_message_handle);
    my_message->flags &= ~KERNEL_MESSAGE_ACTIVE;
    goto done;
  }

  if (!(my_message->flags & KERNEL_MESSAGE_TELETYPE)) {
    my_message->expire_ticks -= KERNEL_MESSAGE_INTERVAL;
  }

  if (my_message->flags & KERNEL_MESSAGE_ATTACHED) {
    sequence = &sequence_list[my_message->sequence_id];
    if (sequence->expired || !sequence->active_flag) my_message->expire_ticks = 0;
  }

  if (my_message->flags & KERNEL_MESSAGE_ANIM) {
    anim = &kernel_anim[my_message->sequence_id];
    segment_id = my_message->segment_id;
    if (anim->doomed || (anim->anim == NULL)) {
      my_message->expire_ticks = 0;
    } else {
      frame      = anim->last_frame;
      image_list = anim->anim->image;
      for (count = 0; (image < 0) && (count < anim->anim->num_images); count++) {
       if (image_list[count].flags == frame) {
         if (image_list[count].segment_id == (byte)segment_id) {
           image = count;
         }
       }
      }
      if (image < 0) my_message->expire_ticks = 0;
    }
  }

  if (my_message->expire_ticks <= 0) {
    if (!kernel.trigger) {
      my_message->flags |= KERNEL_MESSAGE_EXPIRED;
      if (my_message->trigger_code) {
       kernel.trigger      = my_message->trigger_code;
       kernel.trigger_mode = my_message->trigger_dest;
       if (kernel.trigger_mode != KERNEL_TRIGGER_DAEMON) {
         for (count = 0; count < 3; count++) {
           player2.words[count] = my_message->trigger_words[count];
         }
       }
      }
    }
  }

  my_message->update_time = kernel.clock + KERNEL_MESSAGE_INTERVAL;

  x = my_message->x;
  y = my_message->y;

  xx = 0;
  yy = 0;

  if (my_message->flags & KERNEL_MESSAGE_ANIM) {
    xx = image_list[image].x - picture_view_x;
    yy = image_list[image].y - picture_view_y;
  }

  if (my_message->flags & KERNEL_MESSAGE_ATTACHED) {
    if (!sequence->auto_locating) {
      xx = sequence->x;
      yy = sequence->y;
    } else {
      xx = series_list[sequence->series_id]->index[sequence->sprite - 1].x;
      yy = series_list[sequence->series_id]->index[sequence->sprite - 1].y;
    }
  }

  if (my_message->flags & KERNEL_MESSAGE_PLAYER) {
    if (player.walker_been_visible) {
      ys = (50 + (series_list[player.series_base + player.series]->index[player.sprite - 1].ys * player.scale)) / 100;
      xx = player.x;
      yy = player.y + ((50 + (player.center_of_gravity * player.scale)) / 100) - ys;
      yy-= 15;
    } else {
      xx = video_x   >> 1;
      yy = display_y >> 1;
    }
  }

  xx += x;
  yy += y;

  if (my_message->flags & KERNEL_MESSAGE_TELETYPE) {
    if (kernel.clock >= my_message->strobe_time) {
      my_message->message[my_message->strobe_marker++] = my_message->strobe_save;
      my_message->message[my_message->strobe_marker]   = my_message->strobe_save_2;
      my_message->strobe_save   = my_message->message[my_message->strobe_marker];
      my_message->strobe_save_2 = my_message->message[my_message->strobe_marker+1];
      if (!my_message->strobe_save) {
       my_message->message[my_message->strobe_marker] = 0;
       my_message->flags &= ~KERNEL_MESSAGE_TELETYPE;
      } else if (my_message->flags & KERNEL_MESSAGE_QUOTE) {
       my_message->message[my_message->strobe_marker]   = '"';
       my_message->message[my_message->strobe_marker+1] = 0;
      }
      my_message->update_time = my_message->strobe_time = kernel.clock + my_message->strobe_rate;
      strobe_flag = true;
    }
  }

  width = font_string_width (kernel_message_font, my_message->message, kernel_message_spacing);

  if (my_message->flags & (KERNEL_MESSAGE_CENTER | KERNEL_MESSAGE_RIGHT)) {
    if (my_message->flags & KERNEL_MESSAGE_CENTER) {
      xx -= (width >> 1);
    } else {
      xx -= width;
    }
  }

  x2 = xx + width;
  if (x2 > video_x) xx -= (x2 - video_x);

  xx = max (0, min (video_x - 1, xx));
  yy = max (0, min (display_y - 1, yy));

  if (my_message->matte_message_handle >= 0) {
    if (strobe_flag ||
       (xx != message_list[my_message->matte_message_handle].x) ||
       (yy != message_list[my_message->matte_message_handle].y)) {
      matte_clear_message(my_message->matte_message_handle);
      my_message->matte_message_handle = -1;
    }
  }

  if (my_message->matte_message_handle < 0) {
    matte_id = matte_add_message(kernel_message_font, my_message->message, xx, yy, my_message->color, kernel_message_spacing);
    if (matte_id < 0) goto done;
    my_message->matte_message_handle = matte_id;
  }

done:
  ;
}



void fastcall kernel_message_update_all (void)
{
  int count;

  for (count = 0; (count < KERNEL_MAX_MESSAGES) && !kernel.trigger; count++) {
    if (kernel_message[count].flags & KERNEL_MESSAGE_ACTIVE) {
      if (kernel.clock >= kernel_message[count].update_time) {
       kernel_message_update (&kernel_message[count]);
      }
    }
  }
}


void fastcall kernel_message_correction (long old_clock, long new_clock)
{
  int count;

  for (count = 0; (count < KERNEL_MAX_MESSAGES); count++) {
    if (kernel_message[count].flags & KERNEL_MESSAGE_ACTIVE) {
      kernel_message[count].update_time += (new_clock - old_clock);
    }
  }
}
