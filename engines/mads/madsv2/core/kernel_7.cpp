/*
/*      kernel_7.c      by Brian Reynolds       14-Nov-91
*/

#include <general.mac>
#include <inter.h>
#include <hspot.h>
#include <error.h>
#include "kernel.mac"
#include "kernel_1.h"


#pragma optimize ("weglt", on)
/*
/*      kernel_flip_hotspot()
/*
/*      Toggles an interface hotspot (referenced by its vocabulary word)
/*      on or off.  Hotspots that are off do not interact with the mouse
/*      cursor.
*/
void fastcall kernel_flip_hotspot (int vocab_code, int active)
{
  int count;

  for (count = 0; count < room_num_spots; count++) {
    if (room_spots[count].vocab == vocab_code) {
      room_spots[count].active = (byte) active;
      hspot_toggle(STROKE_INTERFACE, count, active);
    }
  }
}



/*
/*      kernel_flip_hotspot()
/*
/*      Toggles an interface hotspot (referenced by its vocabulary word)
/*      on or off.  Hotspots that are off do not interact with the mouse
/*      cursor.
/*
/*      Only hotspots which contain the point (X, Y) are affected.
*/
void fastcall kernel_flip_hotspot_loc (int vocab_code, int active, int x, int y)
{
  int count;

  for (count = 0; count < room_num_spots; count++) {
    if (room_spots[count].vocab == vocab_code) {
      if ((x >= room_spots[count].ul_x) &&
         (x <= room_spots[count].lr_x) &&
         (y >= room_spots[count].ul_y) &&
         (y <= room_spots[count].lr_y)) {
       room_spots[count].active = (byte) active;
       hspot_toggle(STROKE_INTERFACE, count, active);
      }
    }
  }
}

