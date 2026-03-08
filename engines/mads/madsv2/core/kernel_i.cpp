/*
/*      kernel_i.c      by Brian Reynolds       31-Mar-92
*/

#include <string.h>

#include <general.mac>
/* pl #include <sound.h> */
#include <timer.h>
#include <env.h>
#include <error.h>

#pragma optimize ("weglt", on)

int kernel_sound_handle = 0;


void fastcall kernel_unload_sound_driver(void)
{
/* pl  if (kernel_sound_handle) {
    sound_queue(0);

    timer_set_sound_flag (false);

    sound_system_shutdown();
    sound_driver_remove(kernel_sound_handle);

    sound_driver_null();
  }

  kernel_sound_handle = 0;
*/
}


int fastcall kernel_load_sound_driver(char *name, char sound_card, int sound_board_address, int sound_board_type, int sound_board_irq)
{
/* pl  char temp_buf[80];
  char temp_buf_2[80];
  char *mark;

  strcpy (temp_buf, name);
  for (mark = temp_buf; *mark; mark++) {
    if (*mark == '#') {
      *mark = sound_card;
    }
  }

  if (env_search_mode == ENV_SEARCH_MADS_PATH) {
    env_get_path (temp_buf_2, temp_buf);
  } else {
    if (temp_buf[0] == '*') {
      strcpy (temp_buf_2, &temp_buf[1]);
    } else {
      strcpy (temp_buf_2, temp_buf);
    }
  }

  kernel_unload_sound_driver();

  kernel_sound_handle = sound_driver_load(temp_buf_2);

  if (kernel_sound_handle) {
    sound_driver_init (kernel_sound_handle);
    while (sound_system_setup(sound_board_address, sound_board_type, sound_board_irq) != 0);
  } else {
    sound_driver_null();
  }

  timer_set_sound_flag (kernel_sound_handle);

  return (kernel_sound_handle);
  */
}

