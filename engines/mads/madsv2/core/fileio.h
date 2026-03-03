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

#ifndef MADS_CORE_FILEIO_H
#define MADS_CORE_FILEIO_H

namespace MADS {
namespace MADSV2 {

#define fread_bufsize  1024

extern int fileio_suppress_unbuffering;


/* fileio_1 */
void       fileio_purge_trailing_spaces(char *myline);

/* fileio_2 */
void       fileio_name_new_ext(char *bakfile,
	char *mainfile,
	char *new_ext);

/* fileio_3 */
char *fileio_ffgets(char *mystring,
	int num,
	Common::Stream *stream);

/* fileio_4 */
int        fileio_ffputs(char *mystring,
	Common::Stream *stream);

/* fileio_5 */
char *fileio_fix_lf_input(char *mystring);

/* fileio_6 */
void       fileio_fix_lf_output(char *mystring);

int        fileio_copy(char *source, char *dest);


/* fileio_7 */
long       fileio_setpos(Common::Stream *handle, long pos);


/* fileio_8 */
long       fileio_fread_f(void *buffer, long size,
	long count, Common::Stream *stream);

/* fileio_9 */
long       fileio_fwrite_f(void *buffer, long size,
	long count, Common::Stream *stream);

/* fileio_a */
long       fileio_file_to_file(Common::Stream *from, Common::Stream *to, long count);

/* fileio_b */
long       fileio_get_file_size(char *filename);

/* fileio_c */
int                 fileio_exist(char *inp);

/* fileio_d */
char *    fileio_get_filetype(char *outp, char *inp);

/* fileio_e */
long       fileio_get_file_time(char *filename);

/* fileio_f */
char *    fileio_read_header(char *target, Common::Stream *handle);
void       fileio_write_header(char *text, Common::Stream *handle);

/* fileio_g */
char *fileio_get_line(char *target, Common::Stream *handle);
int        fileio_put_line(char *source, Common::Stream *handle);

/* fileio_h */
long fileio_get_disk_free(char drive);

/* fileio_i */
void fileio_add_ext(char *name, char *ext);
void fileio_new_ext(char *target,
	char *name,
	char *ext);

/* fileio_j */
int fileio_logpath(char *path);

/* fileio_k */
char *fileio_parse_filename(char *target,
	char *filepath);
char *fileio_parse_path(char *target,
	char *filepath);

/* fileio_l */
char *fileio_swap_path(char *target,
	char *base,
	char *file);

/* fileio_m */
char *fileio_join_path(char *target,
	char *path,
	char *file);

/* fileio_n */
void fileio_get_volume_label(char *volume_label, char drive_letter);

/* fileio_o */
int  fileio_set_file_time(char *filename, long new_time);

/* fileio_p */
int fileio_get_file_attributes(char *filename, word *attributes);

/* fileio_q */
int fileio_set_file_attributes(char *filename, word attributes);

/* fileio_r */
int fileio_read_till_null(char *target, Common::Stream *handle);

/* fileio_s */
char *fileio_prepend(char *target,
	char *source,
	char *prepend);

/* fileio_t */
char *fileio_chop_ext(char *target, char *source);

/* fileio_u */
void fileio_purge_all_spaces(char *text);

} // namespace MADSV2
} // namespace MADS

#endif
