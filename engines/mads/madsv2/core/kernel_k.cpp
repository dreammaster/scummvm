/*
/*      kernel_k.c      by Brian Reynolds       10-Apr-92
/*
/*      Changes palette to reflect newly loaded sprite series, but does
/*      not affect cycling color ranges.
*/

#include <general.mac>
#include <mcga.h>
#include <pal.h>
#include <cycle.h>
#include "kernel.mac"

#pragma optimize ("weglt", on)

void fastcall kernel_new_palette (void)
{
  int palette_base, palette_size;

  palette_base = KERNEL_RESERVED_LOW_COLORS;
  if (cycling_active) {
    if (cycle_list.num_cycles) {
      palette_base = cycle_list.table[0].first_palette_color + total_cycle_colors;
    }
  }
  palette_size  = 256 - palette_base;

  mcga_setpal_range(&master_palette, palette_base, palette_size);
}


