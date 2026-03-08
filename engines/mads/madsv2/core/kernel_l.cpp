/*
/*      kernel_l.c      by Brian Reynolds       27-May-92
*/

#include <general.mac>
#include <mem.h>
#include <matte.h>
#include <player.h>
#include <sprite.h>

#include "kernel.mac"
#include "kernel_1.h"
#include "kernel_6.h"
#include "kernel_8.h"


void fastcall kernel_dump_quotes (void)
{
  if (kernel.quotes != NULL) {
    mem_free (kernel.quotes);
    kernel.quotes = NULL;
  }
}


void fastcall kernel_dump_all (void)
{
  kernel_dump_quotes();
  kernel_unload_all_series();
  kernel_seq_init();
  image_marker = 0;
  matte_refresh_work();
}


void fastcall kernel_dump_walker_only (void)
{
  int count;
  int marker;

  marker = player.series_base;

  for (count = 0; count < 8; count++) {
    if (player.available[count]) {
      sprite_free(&series_list[marker++], true);
      player.available[count] = false;
    }
  }

  image_marker = 0;
  matte_refresh_work();

  player.walker_visible = false;
}

