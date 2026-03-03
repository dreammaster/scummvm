/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MADS_CORE_ENV_H
#define MADS_CORE_ENV_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

#define MADS_ENV        "MADS"
#define MADS_PRIV_ENV   "MADSPRIV"
#define MADS_SOUND_ENV  "MADSOUND"

#define MADS_PRIV_ARTIST        0x01
#define MADS_PRIV_DESIGNER      0x02
#define MADS_PRIV_PROGRAM       0x04
#define MADS_PRIV_SYSTEM        0x08

#define ENV_CONFIGNAME  "*options.cfg"

#define ENV_SEARCH_MADS_PATH            0
#define ENV_SEARCH_CONCAT_FILES         1

#define ENV_OBJECTS                     16
#define ENV_INTERFACE                   17
#define ENV_TEXT                        18
#define ENV_QUOTES                      19
#define ENV_FONT                        20
#define ENV_ART                         21
#define ENV_SOUND                       22
#define ENV_CONV                        23
#define ENV_SPEECH                      24


struct env_options_type
{
	int     linemode;          /* 50/25/0=auto */
	int     compile_final;     /* TRUE if compilation is to final set */
	int     compile_debug;     /* TRUE for debugging mode */
	int     current_room;      /* User's current room */
	long    game_revision;     /* Version of game */
	char    game_name[25];     /* Game's working title */
	int     mono_on_right;     /* Is monochrome monitor on right? */
};


extern int env_search_mode;
extern int env_search_cd;
extern char env_cd_drive;
extern int env_privileges;

extern int env_sound_override;

extern long env_concat_file_size; /* Size of last concat file opened */



/* env_0.c */
int         env_verify(void);

/* env_1.c */
Common::Stream * env_open(char *file_path, char *options);
int         env_exist(char *file_name);
long        env_get_file_size(Common::Stream *handle);


/* env_1.c */
char *env_get_path(char *madspath,
	char *infile);

/* env_1.c */
char *env_catint(char *out, int value, int digits);

/* env_1.c */
char * env_fill_path(char *path, int env_mode, int env_room);


/* env_2.c */
char * env_dos_error_name(char *error_buf);

/* env_3.c */
char * env_get_level_path(char *out, int item_type,
	char *file_spec,
	int first_level,
	int second_level);

Common::Stream * env_open_level(int item_type,
	char *file_spec,
	int level_one, int level_two,
	char *options);


/* env_4.c */
extern char env_null[7];

char *env_get(char *target, char *env);


/* env_5.c */
char * env_next(char *variable);
char * env_find_end(char *environment);
int         env_size(char *environment);
int         env_free(char *environment);

/* env_5.c */
char * env_find(char *environment, char *variable);
void        env_delete(char *environment, char *variable);
int         env_insert(char *environment, char *variable,
	char *value);

} // namespace MADSV2
} // namespace MADS

#endif
