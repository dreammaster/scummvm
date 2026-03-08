/*
/*      kernel_6.c      by Brian Reynolds       7-Nov-91
*/

#include <string.h>

#include <general.mac>
#include <matte.h>
#include <sprite.h>
#include <mem.h>
#include <error.h>
#include <pal.h>
#include <mcga.h>

#include "kernel.mac"
#include "kernel_1.h"

#pragma optimize ("weglt", on)

/*
/*      kernel_unload_all_series(void)
/*
/*      Unloads all room series.
*/
void fastcall kernel_unload_all_series(void)
{
  int count;

  /* Unload all series (but don't unload those for the interface background) */

  for (count = series_list_marker - 1; count >= kernel_room_series_marker; count--) {
    if (series_user[count] > 1) series_user[count] = 1;
    matte_deallocate_series (count, true);
  }
}


/*
/*      kernel_load_series()
/*
/*      Loads a sprite series for the room.
*/
int fastcall kernel_load_series (char *name, int load_flags)
{
  int handle = -2;

  if (kernel.translating) load_flags |= SPRITE_LOAD_TRANSLATE;

  handle = matte_load_series (name, load_flags, 0);

done:
  if ((handle < 0) && !kernel_ok_to_fail_load) {
    #ifndef disable_error_check
      strcpy (error_string, name);
      error_report (ERROR_SERIES_LOAD_FAILED, WARNING, MODULE_KERNEL, handle, sprite_error);
    #endif
  }

  return (handle);
}

