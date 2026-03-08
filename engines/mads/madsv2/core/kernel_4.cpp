/*
/*      kernel_4.c      by Brian Reynolds       6-Nov-91
*/

#include <string.h>

#include <general.mac>
#include <pal.h>
#include <mouse.h>
#include <sprite.h>
#include <mem.h>
#include <inter.h>
#include <error.h>
#include <video.h>
#include <mcga.h>
#include <sprite.h>
#include <popup.h>

#include "kernel.mac"
#include "kernel_1.h"


#pragma optimize ("weglt", on)


/*
/*      kernel_section_shutdown()
/*
/*      Section level system shutdown.
*/
void fastcall kernel_section_shutdown(void)
{
  ;
}


/*
/*      kernel_section_startup()
/*
/*      Section level system startup.
*/
int fastcall kernel_section_startup (int new_section)
{
  int error_flag = true;
  #ifndef disable_error_check
    int error_code = 0;
  #endif

  /* Make note of new section number */

  previous_section = section_id;
  section_id       = new_section;

  error_flag = false;

done:
  /*
  if (error_flag) {
    #ifndef disable_error_check
      error_check_memory ();
      error_report (error_code, ERROR, MODULE_KERNEL, new_section, 0);
    #endif
    kernel_section_shutdown();
  }
  */
  return (error_flag);
}

