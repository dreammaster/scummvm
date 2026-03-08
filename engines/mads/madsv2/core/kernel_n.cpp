/*
/*      kernel_n.c      by Brian Reynolds       17-Jun-92
/*
/*      Facility to generate "idle chatter" messages randomly within
/*      a predefined region of the screen.
*/

#include <stdlib.h>
#include <stdarg.h>

#include <general.mac>
#include <quote.h>
#include <imath.h>

#include "kernel.mac"
#include "kernel_1.h"
#include "kernel_d.h"

#pragma optimize ("weglt", on)


int random_message_handle[KERNEL_MAX_RANDOM_MESSAGES];  /* List of active handles    */
int random_message_quote [KERNEL_MAX_RANDOM_MESSAGES];  /* List of active quote id's */
int random_max_messages = 1;                            /* # of active handles/ids   */

int random_quote_list[KERNEL_MAX_RANDOM_QUOTES];        /* List of available quote id's */
int random_quote_list_size = 0;                         /* # of available quote id's    */

int random_min_x = 0;                   /* X location allowable range */
int random_max_x = video_x;

int random_min_y = 0;                   /* Y location allowable range */
int random_max_y = display_y;
int random_spacing;                     /* Y location minimum spacing */

int random_teletype_rate = 0;           /* Rate for teletype          */

int random_message_color;               /* Color scheme for message   */
int random_message_duration;            /* Duration of messages       */


void fastcall kernel_random_purge (void)
{
  int count;

  for (count = 0; count < KERNEL_MAX_RANDOM_MESSAGES; count++) {
    random_message_handle[count] = -1;
    random_message_quote [count] = -1;
  }
}


/*
/*      kernel_random_messages_init()
/*
/*      Initializes a random chatter sequence.  (Parameters end with
/*      a zero-terminated list of already loaded quote id's to be
/*      used for messages).
*/
void kernel_random_messages_init (int max_messages_at_once,
                                 int min_x, int max_x,
                                 int min_y, int max_y,
                                 int min_y_spacing,
                                 int teletype_rate,
                                 int color,
                                 int duration,
                                 int quote_id, ...)
{
  va_list marker;
  int my_quote = quote_id;

  random_max_messages     = max_messages_at_once;
  random_min_x            = min_x;
  random_max_x            = max_x;
  random_min_y            = min_y;
  random_max_y            = max_y;
  random_spacing          = min_y_spacing;
  random_teletype_rate    = teletype_rate;
  random_message_color    = color;
  random_message_duration = duration;

  random_quote_list_size = 0;

  va_start (marker, quote_id);
  while (my_quote > 0) {
    if (random_quote_list_size < KERNEL_MAX_RANDOM_QUOTES) {
      random_quote_list[random_quote_list_size++] = my_quote;
    }

    my_quote = va_arg (marker, int);
  }

  kernel_random_purge();
}



/*
/*      kernel_check_random()
/*
/*      Returns the number of idle chatter messages currently being
/*      displayed.
*/
int fastcall kernel_check_random (void)
{
  int count;
  int sum = 0;

  for (count = 0; count < random_max_messages; count++) {
    if (random_message_handle[count] >= 0) sum++;
  }
  return (sum);
}



/*
/*      kernel_random_message_server()
/*
/*      Should be called regularly from daemon code whenever a random
/*      message sequence is running (intercepts triggers from terminating
/*      messages to determine when to free up control space).
*/
void fastcall kernel_random_message_server (void)
{
  if ((kernel.trigger >= KERNEL_RANDOM_MESSAGE_TRIGGER) &&
      (kernel.trigger <  KERNEL_RANDOM_MESSAGE_TRIGGER + random_max_messages)) {
    random_message_handle[kernel.trigger - KERNEL_RANDOM_MESSAGE_TRIGGER] = -1;
    random_message_quote [kernel.trigger - KERNEL_RANDOM_MESSAGE_TRIGGER] = -1;
  }
}



/*
/*      kernel_generate_random_message()
/*
/*      Called occasionally to (possibly) generate a random message.
/*
/*      (generated whenever random(chance_major) <= chance_minor)
*/
int fastcall kernel_generate_random_message (int chance_major, int chance_minor)
{
  int count, count2, scan;
  int bad;
  int generated_one;
  int idx, quote;
  int last_y;
  int message_x, message_y;
  int crash_timeout = 0;

  generated_one = false;

  for (count = 0; count < random_max_messages; count++) {
    if (random_message_handle[count] < 0) {

      /* Don't allow two phrases to teletype at once */

      bad = false;
      for (scan = 0; scan < random_max_messages; scan++) {
       if (random_message_handle[scan] >= 0) {
         if (kernel_message[random_message_handle[scan]].flags & KERNEL_MESSAGE_TELETYPE) {
           bad = true;
         }
       }
      }

      /* Check random chance for message to appear */

      if ((imath_random (1, chance_major) <= chance_minor) && !bad) {

       /* Pick randomly from our list of allowable quotes */

       do {
         idx   = imath_random(0, random_quote_list_size - 1);
         quote = random_quote_list[idx];
         bad   = false;
         for (scan = 0; scan < random_max_messages; scan++) {
           if (quote == random_message_quote[scan]) {
             bad = true;
           }
         }
       } while (bad);

       random_message_quote[count] = quote;

       /* Put message in a random location */

       message_x = imath_random (random_min_x, random_max_x);

       /* Be sure Y values are properly spaced */

       crash_timeout = 0;

       do {
         if (crash_timeout++ > 100) goto done;
         bad       = false;
         message_y = imath_random (random_min_y, random_max_y);
         for (count2 = 0; count2 < random_max_messages; count2++) {
           if (random_message_handle[count2] >= 0) {
             last_y = kernel_message[random_message_handle[count2]].y;
             if ((message_y >= (last_y - random_spacing)) &&
                 (message_y <= (last_y + random_spacing))) {
               bad = true;
             }
           }
         }
       } while (bad);

       /* Put our new message in the list */

       random_message_handle[count] =
              kernel_message_add(quote_string(kernel.quotes, random_message_quote[count]),
                                 message_x, message_y, random_message_color, random_message_duration,
                                 KERNEL_RANDOM_MESSAGE_TRIGGER + count, 0);
       if (random_teletype_rate > 0) {
         if (random_message_handle[count] >= 0) {
           kernel_message_teletype (random_message_handle[count], random_teletype_rate, true);
         }
       }

       generated_one = true;
      }
    }
  }

done:
  return (generated_one);
}


