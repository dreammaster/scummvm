/*
/*      kernel_o.c      by Brian Reynolds       26-Jun-92
*/

#include <string.h>

#include <general.mac>

#include <matte.h>
#include <inter.h>
#include <mem.h>
#include <buffer.h>
#include <room.h>
#include <pal.h>
#include <error.h>
#include <anim.h>
#include <vocab.h>
#include <timer.h>
#include <mouse.h>
#include <video.h>
#include <mcga.h>
#include <rail.h>
#include <cycle.h>

#include "kernel.mac"
#include "kernel_1.h"
#include "kernel_2.h"
#include "kernel_8.h"
#include "kernel_d.h"
#include "kernel_g.h"

#pragma optimize ("weglt", on)

char kernel_interface_loaded[40] = "";


void fastcall kernel_load_interface (void)
{
  char temp_buf[80];
  char *mark;

  _fstrcpy (temp_buf, kernel.interface);
  mark = strchr(temp_buf, '.');
  if (mark != NULL) {
    *mark = 0;
  }
  if (inter_input_mode != INTER_BUILDING_SENTENCES) {
    strcat (temp_buf, "A");
  }
  strcat (temp_buf, ".INT");

  if (strcmp(kernel_interface_loaded, temp_buf)) {
    buffer_free (&scr_inter_orig);
    pal_activate_shadow (&kernel_shadow_inter);
    if (inter_load_background (temp_buf, &scr_inter_orig)) {
      error_report (ERROR_KERNEL_NO_INTERFACE, SEVERE, MODULE_KERNEL, inter_input_mode, 0);
    }
    strcpy (kernel_interface_loaded, temp_buf);
    pal_activate_shadow (&kernel_shadow_main);
  }
}





/*
/*      kernel_set_interface_mode()
/*
/*      Switches interface modes (INTER_BUILDING_SENTENCES is the
/*      normal mode; INTER_CONVERSATION is for dialog scenes), and
/*      properly updates the graphic structures.
*/
void fastcall kernel_set_interface_mode (int mode)
{
  char temp_buf[80];
  char *mark;

  inter_input_mode = mode;
  kernel_load_interface();

  image_inter_marker = 1;
  image_inter_list[0].flags      = IMAGE_REFRESH;
  image_inter_list[0].segment_id = -1;

  /* Set up interface animation clock */

  inter_base_time = timer_read();

  left_command = -1;
  left_action  = -1;
  left_inven   = -1;

  /* Initialize interface work area */

  if (!viewing_at_y) {
    buffer_rect_copy (scr_inter_orig, scr_inter, 0, 0, video_x, inter_size_y);

    /* Initialize interface grammar driver */

    if (kernel_mode == KERNEL_ACTIVE_CODE) matte_inter_frame (false, false);
  }

  inter_init_sentence();
  inter_setup_hotspots();

  if (!viewing_at_y) inter_prepare_background();

  kernel_refresh_dynamic();
}

