/*
/*      kernel_a.c      by Brian Reynolds       8-Apr-92
*/

#include <general.mac>
#include <player.h>
#include <matte.h>
#include <timer.h>
#include "kernel.mac"
#include "kernel_8.h"

#pragma optimize ("weglt", on)


void fastcall kernel_seq_player (int sequence_id, int synch_me)
{
  kernel_seq_loc   (sequence_id, player.x, player.y + ((player.center_of_gravity * player.scale) / 100));
  kernel_seq_depth (sequence_id, player.depth);
  kernel_seq_scale (sequence_id, player.scale);

  if (synch_me) {
    kernel_synch (KERNEL_SERIES, sequence_id,
                 KERNEL_PLAYER, 0);
  }
}

