/*
/*      kernel_j.c      by Brian Reynolds       7-Apr-92
*/

#include <general.mac>
#include <room.h>
#include <matte.h>
#include <tile.h>
#include <error.h>
#include <camera.h>
#include <rail.h>

#include "kernel.mac"
#include "kernel_1.h"

#pragma optimize ("weglt", on)

void fastcall kernel_load_variant (int variant)
{
  room_variant = variant;

  if (room_load_variant (room_id, room_variant, NULL, room,
                        &scr_depth,
                        &scr_walk,
                        &scr_special,
                        &depth_map,
                        &depth_resource,
                        tile_attribute_handle)) {
    error_report (ERROR_VARIANT_LOAD_FAILURE, WARNING, MODULE_KERNEL, room_load_error, (room_id * 10) + room_variant);
  }

  rail_connect_all_nodes();

  camera_jump_to (picture_view_x, picture_view_y);
}
