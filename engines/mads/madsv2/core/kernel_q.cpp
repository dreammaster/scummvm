/*
/*      kernel_q.c      by Brian Reynolds       11-Jan-93
/*
/*      Manually adjusts room scaling parameters.
*/

#include <general.mac>

#include "kernel.mac"
#include "kernel_1.h"

#pragma optimize ("weglt", on)

void fastcall kernel_room_scale (int front_y, int front_scale,
                                int back_y,  int back_scale)
{
  room->front_y     = front_y;
  room->front_scale = front_scale;
  room->back_y      = back_y;
  room->back_scale  = back_scale;

  kernel_room_bound_dif = room->front_y - room->back_y;
  kernel_room_scale_dif = room->front_scale - room->back_scale;
}

