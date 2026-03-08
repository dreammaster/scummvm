/*
/*      kernel_r.c      by Brian Reynolds       1-Apr-93
*/

#include <string.h>

#include <general.mac>

#include <matte.h>
#include <inter.h>
#include <mem.h>
#include <buffer.h>
#include <room.h>
#include <pal.h>
#include <error.h>
#include <anim.h>
#include <vocab.h>
#include <timer.h>
#include <mouse.h>
#include <video.h>
#include <mcga.h>
#include <rail.h>
#include <cycle.h>
#include <tile.h>
#include <sprite.h>
#include <popup.h>

#include "kernel.mac"
#include "kernel_1.h"
#include "kernel_2.h"
#include "kernel_8.h"
#include "kernel_d.h"
#include "kernel_o.h"


#pragma optimize ("weglt", on)



/*
/*      kernel_background_shutdown()
/*
/*      Room level system shutdown.
*/
void fastcall kernel_background_shutdown (void)
{
  /* Remove our palette shadowing list */

  pal_activate_shadow (NULL);

  /* Dump the picture & attribute buffers, along with the room header */

  if (room != NULL) {
    room_unload (room,
                &scr_orig,
                &scr_depth,
                &scr_walk,
                &scr_special,
                &picture_map,
                &depth_map);
    room = NULL;
  }
}


int fastcall kernel_background_startup (int new_room, int initial_variant)
{
  int error_flag = true;
  int load_flags;
  int error_code = 0;
  int error_data = 0;

  /* Make a note of the new room number & variant */

  previous_room = room_id;
  room_id       = new_room;
  room_variant  = initial_variant;

  /* Start a brand new palette, reserving the proper # of colors */

  pal_init (KERNEL_RESERVED_LOW_COLORS, KERNEL_RESERVED_HIGH_COLORS);
  pal_white (master_palette);

  /* Initialize the matteing system */

  matte_init (false);

  /* Initialize graphics sequence data structures */

  kernel_seq_init();
  kernel_message_init();

  /* Activate the main shadow list */

  pal_activate_shadow (&kernel_shadow_main);

  /* Load header, picture, and attribute screen for this room */

  load_flags = ROOM_LOAD_HARD_SHADOW;
  if (kernel.translating) load_flags |= ROOM_LOAD_TRANSLATE;

  room = room_load (room_id, room_variant, NULL,
                   &scr_orig,
                   &scr_depth,
                   &scr_walk,
                   &scr_special,
                   &picture_map,
                   &depth_map,
                   &picture_resource,
                   &depth_resource,
                   tile_picture_handle,
                   tile_attribute_handle,
                   load_flags);
  if (room == NULL) {
    error_data = room_load_error;
    error_code = ERROR_KERNEL_NO_ROOM;
    goto done;
  }

  tile_pan (&picture_map, picture_view_x, picture_view_y);
  tile_pan (&depth_map,   picture_view_x, picture_view_y);

  /* Set up color cycling table for this room */

  cycle_init (&room->cycle_list, false);

  /* Initialize the graphics image lists */

  image_marker = 1;
  image_list[0].flags      = IMAGE_REFRESH;
  image_list[0].segment_id = KERNEL_SEGMENT_SYSTEM;

  /* Mark the boundary between interface and room sprite series */

  kernel_room_series_marker = series_list_marker;

  error_flag = false;

done:
  if (error_flag) {
    error_check_memory();
    error_report (error_code, ERROR, MODULE_KERNEL, room_id, error_data);
    kernel_background_shutdown();
  }
  return (error_flag);
}

