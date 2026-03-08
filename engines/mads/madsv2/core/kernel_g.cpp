/*
/*      kernel_g.c      by Brian Reynolds       22-Apr-92
*/

#include <general.mac>

#include <inter.h>
#include <hspot.h>
#include <video.h>
#include <error.h>

#include "kernel.mac"
#include "kernel_1.h"

#pragma optimize ("weglt", on)


int  fastcall kernel_dynamic_consecutive(int id)
{
  int scan;

  for (scan = 0; (id >= 0) && (scan < KERNEL_MAX_DYNAMIC); scan++) {
    if (kernel_dynamic_hot[scan].flags && kernel_dynamic_hot[scan].valid) {
      id--;
      if (id < 0) goto done;
    }
  }

  if (id >= 0) scan = -1;

done:
  return (scan);
}


void fastcall kernel_refresh_dynamic (void)
{
  int count;

  numspots = inter_base_hotspots;

  for (count = 0; count < KERNEL_MAX_DYNAMIC; count++) {
    if (kernel_dynamic_hot[count].flags && kernel_dynamic_hot[count].valid && ((inter_input_mode == INTER_BUILDING_SENTENCES) || (inter_input_mode == INTER_LIMITED_SENTENCES)) ) {
      hspot_add (kernel_dynamic_hot[count].x,  kernel_dynamic_hot[count].y,
                kernel_dynamic_hot[count].x + kernel_dynamic_hot[count].xs - 1,
                kernel_dynamic_hot[count].y + kernel_dynamic_hot[count].ys - 1,
                STROKE_DYNAMIC | STROKE_INTERFACE, kernel_dynamic_hot[count].vocab_id,
                RELATIVE_MODE);
      inter_force_rescan = true;
    }
  }

  kernel_dynamic_changed = false;
}
