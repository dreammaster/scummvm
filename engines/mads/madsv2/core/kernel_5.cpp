/*
/*      kernel_5.c      by Brian Reynolds       7-Nov-91
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
/*      kernel_room_shutdown()
/*
/*      Room level system shutdown.
*/
void fastcall kernel_room_shutdown (void)
{
  inter_deallocate_objects();

  /* Dump all active series, followed by the interface animation */

  /*
  if (inter_anim != NULL) {
    anim_unload ((AnimPtr)inter_anim);
    buffer_free (&scr_inter_orig);
    mem_free (inter_anim);
    inter_anim = NULL;
  } else {
    if (scr_inter_orig.data != NULL) {
      buffer_free (&scr_inter_orig);
    }
  }
  */

  /* Dump the vocabulary list */
  /* vocab_unload_active();   */

  /* Dump the room hot spots */

  if (room_spots != NULL) {
    mem_free (room_spots);
    room_spots = NULL;
    room_num_spots = 0;
  }

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

  /*
  sprite_free (&box_param.menu, true);
  sprite_free (&box_param.logo, true);
  sprite_free (&box_param.series, true);
  */
}


int fastcall kernel_room_startup (int new_room, int initial_variant, char *interface, int new_palette)
{
  int error_flag = true;
  int load_flags;
  #ifndef disable_error_check
    int error_code = 0;
    int error_data = 0;
  #endif

  /* Make a note of the new room number & variant */

  previous_room = room_id;
  room_id       = new_room;
  room_variant  = initial_variant;

  /* Start a brand new palette, reserving the proper # of colors */

  if (new_palette) pal_init (KERNEL_RESERVED_LOW_COLORS, KERNEL_RESERVED_HIGH_COLORS);

  pal_white (master_palette);

  /* Load up popup box frame */

  /*
  if (popup_box_load()) {
    error_code = ERROR_KERNEL_NO_POPUP;
    goto done;
  }
  */

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
    #ifndef disable_error_check
      error_data = room_load_error;
      error_code = ERROR_KERNEL_NO_ROOM;
    #endif
    goto done;
  }

  tile_pan (&picture_map, picture_view_x, picture_view_y);
  tile_pan (&depth_map,   picture_view_x, picture_view_y);

  /* Set up color cycling table for this room */

  cycle_init (&room->cycle_list, false);

  /* Initialize rail-system parameters for this room */

  rail_num_nodes = room->num_rails + 2;
  rail_base      = (byte *) &room->rail[0];

  rail_connect_all_nodes();

  /* Load up the room's hotspot table */

  room_spots = room_load_hotspots (room_id, &room_num_spots);
  if (room_spots == NULL) {
    #ifndef disable_error_check
      error_code = ERROR_KERNEL_NO_HOTSPOTS;
    #endif
    goto done;
  }

  /* Load the necessary part of the vocabulary list */
  /*
  if (kernel_load_vocab()) {
    #ifndef disable_error_check
      error_code = ERROR_KERNEL_NO_VOCAB;
    #endif
    goto done;
  }
  */

  /* Load up an interface animation w/ background */

  /* scr_inter_orig.data = NULL; */

  /*
  pal_activate_shadow (&kernel_shadow_inter);

  load_flags = PAL_MAP_RESERVED | ANIM_LOAD_BACKGROUND;
  if (kernel.translating) load_flags |= ANIM_LOAD_TRANSLATE;
  if (!inter_animation_running) load_flags |= ANIM_LOAD_BACK_ONLY;

  inter_anim = (AnimInterPtr) anim_load (interface,
                                        &scr_inter_orig, NULL,
                                        NULL, NULL,
                                        NULL, NULL,
                                        NULL, NULL, load_flags);
  if (inter_anim == NULL) {
    #ifndef disable_error_check
      error_code = ERROR_KERNEL_NO_INTERFACE;
      strcpy (error_string, interface);
    #endif
    goto done;
  }

  if (!inter_anim->num_series) {
    mem_free (inter_anim);
    inter_anim = NULL;
  }

  pal_activate_shadow (&kernel_shadow_main);
  */

  inter_anim = NULL;

  /* Make preliminary scaling computations */

  kernel_room_bound_dif = room->front_y - room->back_y;
  kernel_room_scale_dif = room->front_scale - room->back_scale;

  /* Initialize the graphics image lists */

  image_marker = 1;
  image_list[0].flags      = IMAGE_REFRESH;
  image_list[0].segment_id = KERNEL_SEGMENT_SYSTEM;

  /* Set up graphics window locations */

  viewing_at_y = 0;
  inter_viewing_at_y = inter_base_y;

  /* Mark the boundary between interface and room sprite series */

  kernel_room_series_marker = series_list_marker;

  /* Set up interface background screen */

  kernel_set_interface_mode (inter_input_mode);

  /* Mouse cursor on */

  while ((char)mouse_showing > 0) mouse_show();

  inter_allocate_objects();

  error_flag = false;

done:
  if (error_flag) {
    #ifndef disable_error_check
      error_check_memory();
      error_report (error_code, ERROR, MODULE_KERNEL, room_id, error_data);
    #endif
    kernel_room_shutdown();
  }
  return (error_flag);
}
