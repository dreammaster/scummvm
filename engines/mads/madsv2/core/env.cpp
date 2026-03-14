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

#include "common/config-manager.h"
#include "common/file.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/mads.h"
#include "mads/madsv2/core/concat.h"
#include "mads/madsv2/core/fileio.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/error.h"

namespace MADS {
namespace MADSV2 {

extern int art_hags_are_on_hd;

int env_privileges;
int env_search_mode = ENV_SEARCH_MADS_PATH;
int env_search_cd = false;
char env_cd_drive = 'D';
int env_sound_override = false;
long env_concat_file_size = 0;
char env_null[7] = "<NULL>";

static const char env_sect_string[5] = "SECT";
static const char env_room_string[5] = "ROOM";
static const char env_art_string[4] = "ART";
static const char env_objects_string[8] = "OBJECTS";
static const char env_inter_string[6] = "INTER";
static const char env_text_string[5] = "TEXT";
static const char env_quotes_string[7] = "QUOTES";
static const char env_font_string[5] = "FONT";
static const char env_sound_string[6] = "SOUND";
static const char env_conv_string[5] = "CONV";
static const char env_speech_string[7] = "SPEECH";


static const char env_section_string[8] = "SECTION";
static const char env_global_string[7] = "GLOBAL";
static const char env_slash_string[3] = "\\";

static const char env_speech1_string[8] = "SPEECH1";
static const char env_speech2_string[8] = "SPEECH2";


int env_verify() {
	return 0;
}

char *env_catint(char *out, int value, int digits) {
	int mark;
	int power;
	int digit;

	power = 1;
	for (mark = 0; mark < digits; mark++) {
		if (mark > 0) power *= 10;
		Common::strcat_s(out, 65536, "0");
	}

	for (mark = strlen(out) - digits; (unsigned)mark < strlen(out); mark++) {
		if (value >= power) {
			digit = value / power;
			value = value - (digit * power);
			out[mark] += digit;
		}
		power = power / 10;
	}

	return (out);
}

static void path_concat(char *target, const char *string) {
	Common::strcat_s(target, 65536, string);
	Common::strcat_s(target, 65536, env_slash_string);
}

char *env_fill_path(char *path, int env_mode, int env_room) {
	Common::String madsptr;
	char *infile;
	char *madspath;
	char work[80];
	int madslen;
	int env_sect = 0;

	madspath = &work[0];

	infile = path;

	madsptr = ConfMan.get(MADS_ENV);

	if (env_sound_override && (env_mode == ENV_SOUND)) {
		madsptr = ConfMan.get(MADS_SOUND_ENV);
	}

	if (env_mode == ROOM) {
		env_sect = env_room / 100;
	} else if (env_mode == SECTION) {
		env_sect = env_room;
	}

	if (madsptr != NULL) {

		madslen = strlen(madsptr.c_str());

		Common::strcpy_s(madspath, 65536, madsptr.c_str());

		if (madsptr[madslen - 1] != '\\') {
			Common::strcat_s(madspath, 65536, env_slash_string);
		}

		if ((env_mode == ROOM) || (env_mode == SECTION)) {
			Common::strcat_s(madspath, 65536, env_sect_string);
			env_catint(madspath, env_sect, 3);
			Common::strcat_s(madspath, 65536, env_slash_string);
		}

		if (env_mode == ROOM) {
			Common::strcat_s(madspath, 65536, env_room_string);
			env_catint(madspath, env_room, 3);
			Common::strcat_s(madspath, 65536, env_slash_string);
		}

		switch (env_mode) {
		case ENV_OBJECTS:
			path_concat(madspath, env_objects_string);
			break;
		case ENV_INTERFACE:
			path_concat(madspath, env_inter_string);
			break;
		case ENV_TEXT:
			path_concat(madspath, env_text_string);
			break;
		case ENV_QUOTES:
			path_concat(madspath, env_quotes_string);
			break;
		case ENV_FONT:
			path_concat(madspath, env_font_string);
			break;
		case ENV_ART:
			path_concat(madspath, env_art_string);
			break;
		case ENV_SOUND:
			if (!env_sound_override) path_concat(madspath, env_sound_string);
			break;
		case ENV_CONV:
			path_concat(madspath, env_conv_string);
			break;
		case ENV_SPEECH:
			path_concat(madspath, env_speech_string);
			break;
		}

		if (*infile == '\\') {
			infile++;
		}

		Common::strcat_s(madspath, 65536, infile);
		mads_strupr(madspath);

		Common::strcpy_s(path, 65536, madspath);
		madspath = path;

	} else {
		Common::strcpy_s(path, 65536, "");
		madspath = NULL;
	}

	return madspath;
}

char *env_get_path(char *madspath, const char *infile) {
	char *mark;
	int env_mode;
	int env_room = 0;
	int env_sect = 0;
	char temp_buf_1[80];
	char temp_buf_2[80];

	if (infile[0] != '*') {
		Common::strcpy_s(temp_buf_1, infile);
		mads_fullpath(temp_buf_2, temp_buf_1, 79);
		Common::strcpy_s(madspath, 65536, temp_buf_2);
		mads_strupr(madspath);

	} else {
		/* infile is '*d322u001.rac' */
		infile++;
		Common::strcpy_s(temp_buf_1, infile);
		mads_strupr(temp_buf_1);

		if (strncmp(temp_buf_1, "RM", 2) == 0) {
			env_mode = ROOM;
			env_room = atoi(&temp_buf_1[2]);
			env_sect = env_room / 100;

		} else if (strncmp(temp_buf_1, "SC", 2) == 0) {
			env_mode = SECTION;
			env_sect = atoi(&temp_buf_1[2]);
			env_room = env_sect;

		} else if (strstr(temp_buf_1, ".TXT")) {
			env_mode = ENV_TEXT;

		} else if (strstr(temp_buf_1, ".QUO")) {
			env_mode = ENV_QUOTES;

		} else if (strncmp(temp_buf_1, "CONV", 4) == 0) {
			env_mode = ENV_CONV;

		} else if (temp_buf_1[0] == 'I') {
			env_mode = ENV_INTERFACE;

		} else if ((temp_buf_1[0] == 'O') && (temp_buf_1[1] == 'B')) {
			env_mode = ENV_OBJECTS;

		} else if (strncmp(temp_buf_1, "FONT", 4) == 0) {
			env_mode = ENV_FONT;

		} else if ((strncmp(&temp_buf_1[1], "SOUND", 5) == 0) ||
			(strstr(temp_buf_1, ".HMI")) ||
			(strstr(temp_buf_1, ".DRV")) ||
			(strstr(temp_buf_1, ".BNK"))) {
			env_mode = ENV_SOUND;

		} else if ((strstr(temp_buf_1, ".RAW")) ||
			(strstr(temp_buf_1, ".RAC")) ||
			(strncmp(temp_buf_1, "SPCH", 4) == 0)) {
			env_mode = ENV_SPEECH;

		} else {
			env_mode = GLOBAL;

			mark = strchr(temp_buf_1, '.');
			if (mark != NULL) {
				mark++;
				if ((strncmp(mark, "FL", 2) == 0) ||
					(strncmp(mark, "LBM", 3) == 0) ||
					(strncmp(mark, "ANM", 3) == 0) ||
					(strstr(mark, "AA") != NULL) ||
					(strstr(mark, "SS") != NULL)) {
					env_mode = ENV_ART;
				}
			}
		}

		if (env_search_mode == ENV_SEARCH_MADS_PATH) {
			Common::strcpy_s(madspath, 65536, temp_buf_1);
			madspath = env_fill_path(madspath, env_mode, env_room);

		} else {
			if ((env_mode == ROOM) || (env_mode == SECTION)) {
				Common::strcpy_s(madspath, 65536, env_section_string);
				env_catint(madspath, env_sect, 1);
			} else if (env_mode == ENV_SPEECH) {
				Common::strcpy_s(madspath, 65536, env_speech_string);
			} else if (env_mode == ENV_SOUND) {
				Common::strcpy_s(madspath, 65536, env_sound_string);
			} else {
				Common::strcpy_s(madspath, 65536, env_global_string);
			}
			Common::strcat_s(madspath, 65536, CONCAT_EXT);

			/* returns 'global.hag', 'section9.hag', etc */
		}
	}

	return madspath;
}

Common::SeekableReadStream *env_open(const char *filename, const char *options) {
	int error_flag = true;
	int count;
	int found;
	int num_files;
	long mem_to_get;
	char file_path[80];
	char index_file[80];
	char temp_file[80];
	char load_file[80];
	char temp_buf[80];
	char check_buf[80];

	const char *mark;
	Common::SeekableReadStream *index_handle = NULL;
	Common::SeekableReadStream *handle = NULL;
	Concat concat;
	concat.file_offset = concat.file_size = 0;
	Concat *concat_array = NULL;

	if (env_get_path(load_file, file_path) == NULL) goto done;

	Common::strcpy_s(file_path, filename);
	mark = strchr(file_path, '*');

	mads_strupr(file_path);

	/* if CD version and checking RAW or RAC file */
	if (env_search_cd && (strstr(file_path, ".RAW") || strstr(file_path, ".RAC"))) {
		Common::strcpy_s(check_buf, env_speech1_string);
		Common::strcat_s(check_buf, env_slash_string);

		if (file_path[0] == '*') {
			Common::strcat_s(check_buf, &file_path[1]);
		} else {
			Common::strcat_s(check_buf, file_path);
		}
		/* Common::strcat_s (check_buf, temp_buf); */
		Common::strcpy_s(load_file, check_buf);

		if (fileio_exist(load_file)) {
			Common::File *f = new Common::File();
			f->open(load_file);

			if (!f->isOpen()) {
				delete f;
				goto done;
			}

			handle = f;

		} else {
			Common::strcpy_s(check_buf, env_speech2_string);
			Common::strcat_s(check_buf, env_slash_string);

			if (file_path[0] == '*') {
				Common::strcat_s(check_buf, &file_path[1]);
			} else {
				Common::strcat_s(check_buf, file_path);
			}

			/* Common::strcat_s (check_buf, temp_buf); */
			Common::strcpy_s(load_file, check_buf);

			if (fileio_exist(load_file)) {
				Common::File *f = new Common::File();
				f->open(load_file);

				if (handle == NULL) {
					delete f;
					goto done;
				}

				handle = f;

			} else {
				goto done;
			}
		}

	} else if ((env_search_mode == ENV_SEARCH_CONCAT_FILES) && (mark != NULL)) {

		mark++;
		Common::strcpy_s(temp_buf, mark);
		mads_strupr(temp_buf);


		/* load_file = 'global.hag' */
		if (art_hags_are_on_hd) {
			Common::strcpy_s(index_file, load_file);
		} else {
			fileio_new_ext(index_file, load_file, "IDX");
		}
		/* index_file = 'global.idx' */

		if (env_search_cd && !art_hags_are_on_hd) {

			if (fileio_exist(index_file)) {
				/* if 'global.idx' exists */
				Common::File *f = new Common::File();
				f->open(index_file);
				handle = f;
			}

			Common::strcpy_s(temp_file, "X:\\");
			temp_file[0] = env_cd_drive;
			Common::strcat_s(temp_file, load_file);
			Common::strcpy_s(load_file, temp_file);
		}

		if (!fileio_exist(load_file)) {
			if (env_verify()) {
				/* 'MADS' is set in DOS environment */
				Common::strcpy_s(temp_file, load_file);
				Common::strcpy_s(load_file, ConfMan.get(MADS_ENV).c_str());
				Common::strcat_s(load_file, "\\");
				Common::strcat_s(load_file, temp_file);
			}
		}

		if (index_handle == NULL) {
			Common::File *f = new Common::File();
			f->open(load_file);

			if (index_handle == NULL) {
				delete f;
				goto done;
			}

			index_handle = f;
		}

		if (!fileio_fread_f(&check_buf, CONCAT_ID_LENGTH, 1, index_handle)) goto done;
		if (strncmp(check_buf, CONCAT_ID_STRING, CONCAT_ID_CHECK) != 0) goto done;

		if (!fileio_fread_f(&num_files, sizeof(int), 1, index_handle)) goto done;

		found = false;

		mem_to_get = (long)num_files * sizeof(Concat);
		concat_array = (Concat *)mem_get(mem_to_get);

		if (concat_array != NULL) {
			if (!fileio_fread_f(concat_array, mem_to_get, 1, index_handle)) goto done;
			for (count = 0; !found && (count < num_files); count++) {
				found = (strcmp(temp_buf, concat_array[count].name) == 0);
				if (found) {
					concat = concat_array[count];
				}
			}
		} else {
			for (count = 0; !found && (count < num_files); count++) {
				if (!fileio_fread_f(&concat, sizeof(Concat), 1, index_handle)) goto done;
				found = (strcmp(temp_buf, concat.name) == 0);
			}
		}

		delete index_handle;
		index_handle = NULL;

		if (found) {
			Common::File *f = new Common::File();
			f->open(load_file);

			if (handle == NULL) {
				delete f;
				goto done;
			}

			handle = f;

			if (fileio_setpos(handle, concat.file_offset) != concat.file_offset) goto done;

			env_concat_file_size = concat.file_size;
		}

	} else {
		Common::File *f = new Common::File();
		f->open(load_file);
		handle = f;
	}

	error_flag = false;

done:
	if (concat_array != NULL) mem_free(concat_array);
	delete index_handle;

	if (error_flag) {
		delete handle;
		handle = nullptr;
	}

	return handle;
}

long env_get_file_size(Common::SeekableReadStream *handle) {
	long result;

	if (env_search_mode == ENV_SEARCH_CONCAT_FILES) {
		result = env_concat_file_size;

	} else {
		result = handle->size();
	}

	return result;
}

int env_exist(const char *filename) {
	int exist = false;
	Common::SeekableReadStream *handle;
	char check_buf[80];
	char file_name[80];

	Common::strcpy_s(file_name, filename);
	mads_strupr(file_name);

	/* if CD version and checking RAW or RAC file */
	if (env_search_cd && (strstr(file_name, ".RAW") || strstr(file_name, ".RAC"))) {

		Common::strcpy_s(check_buf, env_speech1_string);
		Common::strcat_s(check_buf, env_slash_string);

		if (file_name[0] == '*') {
			Common::strcat_s(check_buf, &file_name[1]);
		} else {
			Common::strcat_s(check_buf, file_name);
		}

		if (fileio_exist(check_buf)) {
			exist = true;

		} else {
			Common::strcpy_s(check_buf, env_speech2_string);
			Common::strcat_s(check_buf, env_slash_string);

			if (file_name[0] == '*') {
				Common::strcat_s(check_buf, &file_name[1]);
			} else {
				Common::strcat_s(check_buf, file_name);
			}

			if (fileio_exist(check_buf)) {
				exist = true;
			}
		}

	} else if (env_search_mode == ENV_SEARCH_CONCAT_FILES) {
		handle = env_open(file_name, "rb");
		if (handle != NULL) {
			exist = true;
			delete handle;
		}

	} else {
		exist = fileio_exist(file_name);
	}

	return exist;
}

char *env_dos_error_name(char *error_buf) {
	switch (errno) {
	case EEXIST:
		Common::strcpy_s(error_buf, 65536, "File already exists.");
		break;

	case EINVAL:
		Common::strcpy_s(error_buf, 65536, "Invalid argument.");
		break;

	case EMFILE:
		Common::strcpy_s(error_buf, 65536, "Too many open files.");
		break;

	case ENOENT:
		Common::strcpy_s(error_buf, 65536, "No such file or directory.");
		break;

	case ENOSPC:
		Common::strcpy_s(error_buf, 65536, "Disk full.");
		break;

	case EACCES:
		Common::strcpy_s(error_buf, 65536, "Permission denied.");
		break;

	case EBADF:
		Common::strcpy_s(error_buf, 65536, "Bad file number.");
		break;

	default:
		Common::sprintf_s(error_buf, 65536, "Error Code %d.", errno);
		break;
	}

	return error_buf;
}

char *env_get_level_path(char *out, int item_type, const char *file_spec, int first_level, int second_level) {
	char temp_buf[80];
	char *result;

	Common::strcpy_s(temp_buf, "*");

	if ((item_type == SECTION) || (item_type == ROOM)) {
		if ((item_type == ROOM) && (first_level == 0)) {
			first_level = second_level / 100;
			/* first_level = room_get_section (second_level); */
		}
	}

	if (file_spec != NULL) {
		switch (item_type) {
		case GLOBAL:
			Common::strcat_s(temp_buf, "GL000");
			break;
		case SECTION:
			Common::strcat_s(temp_buf, "SC");
			env_catint(temp_buf, first_level, 3);
			break;
		case ROOM:
			Common::strcat_s(temp_buf, "RM");
			env_catint(temp_buf, second_level, 3);
			break;
		case OBJECT:
			break;
		}

		Common::strcat_s(temp_buf, file_spec);
	}

	if (env_search_mode == ENV_SEARCH_MADS_PATH) {
		if (env_get_path(out, temp_buf) != NULL) {
			result = out;
		} else {
			result = NULL;
		}
	} else {
		Common::strcpy_s(out, 65536, temp_buf);
		result = out;
	}

	return result;
}

Common::SeekableReadStream *env_open_level(int item_type, const char *file_spec,
		int level_one, int level_two, const char *options) {
	char temp_buf[80];
	Common::SeekableReadStream *result;

	if (env_get_level_path(temp_buf, item_type, file_spec, level_one, level_two) != NULL) {
		result = env_open(temp_buf, options);
	} else {
		result = NULL;
	}

	return result;
}

char *env_get(char *target, const char *env) {
	Common::String get;

	get = ConfMan.get(env);
	if (get.empty()) {
		Common::strcpy_s(target, 65536, env_null);
	} else {
		Common::strcpy_s(target, 65536, get.c_str());
	}

	mads_strupr(target);
	fileio_purge_all_spaces(target);

	return target;
}

char *env_next(char *variable) {
	while (*variable) {
		variable++;
	}

	variable++;

	return variable;
}

char *env_find_end(char *environment) {
	/* Scan through variables */
	while (*environment) {
		environment = env_next(environment);
	}

	/* Scan past load name if exists */
	if ((*(environment + 1) == 1) && (*(environment + 2) == 0)) {
		environment += 3;
		while (*environment) {
			environment++;
		}
	}

	/* Point to byte after everything */
	environment++;

	return environment;
}

int env_size(char *environment) {
	error("TODO: env_size");
}

int env_free(char *environment) {
	char *bottom;
	int size;
	int used;

	bottom = env_find_end(environment);

	used = (bottom - environment);

	size = env_size(environment);

	return size - used;
}

char *env_find(char *environment, char *variable) {
	char *result = NULL;
	char *mark;
	char work_var[256];

	while (*environment && (result == NULL)) {
		Common::strcpy_s(work_var, environment);
		mark = strchr(work_var, '=');
		if (mark != NULL) *mark = 0;
		if (!scumm_stricmp(work_var, variable)) {
			result = environment;
		}
		environment = env_next(environment);
	}

	return result;
}

void env_delete(char *environment, char *variable) {
	char *target;
	char *next;
	char *bottom;
	int size;

	target = env_find(environment, variable);
	if (target == NULL) goto done;

	next = env_next(target);
	bottom = env_find_end(target);
	size = bottom - next;

	memmove(target, next, size);

done:
	;
}

int env_insert(char *environment, char *variable, char *value) {
	int  error_flag = true;
	int  new_size;
	int  free;
	int  copy_size;
	char *target;
	char *bottom;
	char *new_home;
	char var_name[80];
	char var_string[256];

	Common::strcpy_s(var_name, variable);
	mads_strupr(var_name);

	/* Delete any previous copy of this variable */
	env_delete(environment, var_name);

	/* Prepare full string for insertion */
	Common::strcpy_s(var_string, var_name);
	Common::strcat_s(var_string, "=");
	Common::strcat_s(var_string, value);

	/* Get size of string & check of there is space in the environment */
	new_size = strlen(var_string) + 1;
	free = env_free(environment);
	if (new_size > free) goto done;

	/* Find end of environment (for new string).  Also find end of whole */
	/* environment structure (must be moved out of the way).             */

	target = environment;
	while (*target) {
		target = env_next(target);
	}

	bottom = env_find_end(environment);
	copy_size = bottom - target;
	new_home = target + new_size;

	/* Insert string */
	memmove(new_home, target, copy_size);
	memmove(target, var_string, new_size);

	error_flag = false;

done:
	return error_flag;
}

char *env_level_path(char *string, int first_level, int second_level) {
	int env_mode;
	int env_sect;
	int env_room = 0;

	if (second_level >= 0) {
		env_mode = ROOM;
		env_sect = first_level;
		env_room = second_level;
	} else {
		env_mode = SECTION;
		env_sect = first_level;
	}

	return env_fill_path(string, env_mode, env_room);
}

} // namespace MADSV2
} // namespace MADS
