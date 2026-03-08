/*
/*      kernel_m.c      by Brian Reynolds       29-May-92
*/

#include <general.mac>
#include <loader.h>
#include <player.h>
#include <object.h>
#include <inter.h>
#include <timer.h>
#include <game.h>
/* pl #include <conv.h> */
#include <camera.h>
#include <error.h>
#include <matte.h>

#include "kernel.mac"
#include "kernel_1.h"

#pragma optimize ("weglt", on)

extern int first_inven;

#define OMR 40  /* OUAF_MAX_ROOMS in global.mac */

extern int room_state[OMR];


int fastcall kernel_save_game (char *filename)
{
  int error_flag = true;
  Load load_handle;

  load_handle.open = false;

  if (loader_open(&load_handle, filename, "wb", PACK_NONE)) goto done;

  if (!loader_write(&game,    sizeof(KernelGame), 1, &load_handle)) goto done;
  if (!loader_write(&room_id, sizeof(int),     1, &load_handle)) goto done;
  if (!loader_write(&player2, sizeof(Player2), 1, &load_handle)) goto done;
  if (!loader_write(&inven_num_objects, sizeof(int), 1, &load_handle)) goto done;
  if (inven_num_objects) {
    if (!loader_write(inven,    sizeof(int) * inven_num_objects, 1, &load_handle)) goto done;
  }
  if (!loader_write(&player,  sizeof(Player),  1, &load_handle)) goto done;
  if (!loader_write(global,   sizeof(int) * global_list_size, 1, &load_handle)) goto done;
  if (!loader_write(object,   sizeof(Object) * num_objects, 1, &load_handle)) goto done;
  if (!loader_write(0, sizeof(int), 1, &load_handle)) goto done;
  /* pl (see above line) if (!loader_write(&conv_control.running, sizeof(int), 1, &load_handle)) goto done; */

  if (!loader_write(&picture_view_x,  sizeof(int), 1, &load_handle)) goto done;
  if (!loader_write(&picture_view_y,  sizeof(int), 1, &load_handle)) goto done;
  if (!loader_write(room_state,   sizeof(int) * OMR, 1, &load_handle)) goto done;
  if (!loader_write(&previous_room,   sizeof(int), 1, &load_handle)) goto done;

  /* pl if (conv_append(load_handle.handle)) goto done; */

  error_flag = false;

done:
  if (load_handle.open) loader_close (&load_handle);
  return (error_flag);
}


int fastcall kernel_load_game (char *filename)
{
  int error_flag = true;
  Load load_handle;
  byte keep_video_mode;
  byte keep_sound_card;
  byte *keep_quotes;
  int save;

  save = player.walker_is_loaded;

  /*
  keep_video_mode = kernel.video_mode;
  keep_sound_card = kernel.sound_card;
  keep_quotes     = kernel.quotes;
  */

  load_handle.open = false;

  if (loader_open(&load_handle, filename, "rb", false)) goto done;

  if (!loader_read(&game,     sizeof(KernelGame), 1, &load_handle)) goto done;
  if (!loader_read(&new_room, sizeof(int),     1, &load_handle)) goto done;
  if (!loader_read(&player2,  sizeof(Player2), 1, &load_handle)) goto done;
  if (!loader_read(&inven_num_objects, sizeof(int), 1, &load_handle)) goto done;
  if (inven_num_objects) {
    if (!loader_read(inven,     sizeof(int) * inven_num_objects, 1, &load_handle)) goto done;
  }
  if (!loader_read(&player,   sizeof(Player),  1, &load_handle)) goto done;
  if (!loader_read(global,    sizeof(int) * global_list_size, 1, &load_handle)) goto done;
  if (!loader_read(object,    sizeof(Object) * num_objects, 1, &load_handle)) goto done;
  if (!loader_read(0, sizeof(int), 1, &load_handle)) goto done;
  /* pl (see above line) if (!loader_read(&conv_restore_running, sizeof(int), 1, &load_handle)) goto done; */

  /* Temporary support for old save file format */
  if (load_handle.pack_list_marker >= (int)load_handle.pack.num_records) goto expand;

  if (!loader_read(&camera_old_x_target, sizeof(int), 1, &load_handle)) goto done;
  if (!loader_read(&camera_old_y_target, sizeof(int), 1, &load_handle)) goto done;
  if (!loader_read(room_state,   sizeof(int) * OMR, 1, &load_handle)) goto done;
  if (!loader_read(&previous_room,   sizeof(int), 1, &load_handle)) goto done;

expand:
  /* pl if (conv_expand(load_handle.handle)) goto done; */

  if (inven_num_objects > 0) {
    active_inven = 0;
  } else {
    active_inven = -1;
  }

  first_inven = 0;

  section_id   = KERNEL_RESTORING_GAME;
  room_id      = KERNEL_RESTORING_GAME;

  new_section  = new_room / 100;

  kernel.clock = timer_read();
  game.going   = true;

  error_flag  = false;

done:
  if (load_handle.open) loader_close (&load_handle);

  /*
  kernel.video_mode = keep_video_mode;
  kernel.sound_card = keep_sound_card;
  kernel.quotes     = keep_quotes;
  */


  player.walker_is_loaded = save;
  /* player.walker_is_loaded   = false; */
  /* player.walker_must_reload = true; */

  return (error_flag);
}
