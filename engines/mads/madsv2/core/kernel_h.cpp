/*
/*      kernel_h.c      by Brian Reynolds       14-Jan-92
*/

#include <string.h>

#include <general.mac>
#include <env.h>

#include "kernel.mac"
#include "kernel_1.h"

#pragma optimize ("weglt", on)

static char kernel_work_name[20];



char * fastcall kernel_full_name(int my_room, char type, int num, char *text, int ext)
{
  char temp[2];

  if (my_room > 0) {
    if (my_room >= 100) {
      strcpy (kernel_work_name, "*RM");
    } else {
      strcpy (kernel_work_name, "*SC");
    }
    env_catint (kernel_work_name, my_room, 3);
  } else {
    strcpy (kernel_work_name, "*");
  }

  temp[0] = type;
  temp[1] = 0;
  strcat (kernel_work_name, temp);

  if ((num >= 0) && (ext < KERNEL_TT)) {
    if (num > 9) {
      env_catint (kernel_work_name, num, 2);
    } else {
      env_catint (kernel_work_name, num, 1);
    }
  }

  if (text != NULL) _fstrcat (kernel_work_name, text);

  switch (ext) {
    case KERNEL_SS:
      strcat (kernel_work_name, ".SS");
      break;
    case KERNEL_AA:
      strcat (kernel_work_name, ".AA");
      break;
    case KERNEL_DAT:
      strcat (kernel_work_name, ".DAT");
      break;
    case KERNEL_HH:
      strcat (kernel_work_name, ".HH");
      break;
    case KERNEL_ART:
      strcat (kernel_work_name, ".ART");
      break;
    case KERNEL_INT:
      strcat (kernel_work_name, ".INT");
      break;
    case KERNEL_TT:
      strcat (kernel_work_name, ".TT");
      break;
    case KERNEL_MM:
      strcat (kernel_work_name, ".MM");
      break;
    case KERNEL_WW:
      strcat (kernel_work_name, ".WW");
      break;
  }

  if ((num >= 0) && (ext >= KERNEL_TT)) {
    temp[0] = (byte)num;
    temp[1] = 0;
    strcat (kernel_work_name, temp);
  }

  return (kernel_work_name);
}


char * fastcall kernel_name(char type, int num)
{
  return (kernel_full_name(room_id, type, num, NULL, KERNEL_NONE));
}


char * fastcall kernel_interface_name (int num)
{
  return (kernel_full_name(0, 'I', num, NULL, KERNEL_AA));
}

