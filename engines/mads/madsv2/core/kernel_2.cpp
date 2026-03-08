/*
/*      kernel_2.c      by Brian Reynolds       5-Nov-91
/*
/*      Makes active any vocab words that will be needed for this room,
/*      and loads their text into memory.
/*
/*      Call after room definition and object list are loaded.
*/

#include <general.mac>

#include <vocab.h>
#include <object.h>
#include <room.h>
#include <inter.h>
#include <mem.h>
#include <fileio.h>

#include "kernel.mac"
#include "kernel_1.h"



#pragma optimize ("weglt", on)


int fastcall kernel_load_vocab (void)
{
  int error_flag;
  int count;
  int count2;
  #ifdef log_vocab
    FILE *handle;
    long before, after;
  #endif

  /* Load all main command verbs */

  for (count = 0; count < INTER_COMMANDS; count++) {
    vocab_make_active (command[count].id);
  }

  /* Load all object names, and all verbs associated with objects */

  for (count = 0; count < num_objects; count++) {
    vocab_make_active (object[count].vocab_id);

    for (count2 = 0; count2 < (int)object[count].num_verbs; count2++) {
      vocab_make_active (object[count].verb[count2].id);
    }
  }

  /* Load vocabulary for this room's hot spots */

  for (count = 0; count < room_num_spots; count++) {
    vocab_make_active(room_spots[count].vocab);
    if (room_spots[count].verb > 0) {
      vocab_make_active(room_spots[count].verb);
    }
  }

  #ifdef log_vocab
    before = mem_get_avail();
  #endif

  error_flag = vocab_load_active();

  #ifdef log_vocab
    after = mem_get_avail();
    if (fileio_exist("vocab.log")) {
      handle = fopen("vocab.log", "wt");
      fprintf (handle, "Room %d   Vocab words: %d    Memory: %ld\n",
              room_id, vocab_active, before-after);
      fclose (handle);
    }
  #endif

  return (error_flag);
}


