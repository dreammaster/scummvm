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

#ifndef MADS_CORE_VOCAB_H
#define MADS_CORE_VOCAB_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

#define vocab_version           "1.01"
#define vocab_date              "13-Jun-91"

#define VOCAB_MAX_ACTIVE        200


#define VC_MAXWORDS             1200    /* Maximum words in vocabulary */

#define VC_MAXWORDLEN           20      /* Maximum word length */

#define VC_FUDGEFACTOR          50      /* Number of words slack space */

#define VC_MAINFILENAME         "*VOCAB.DB"
#define VC_HARDFILENAME         "*VOCABH.DB"

#define VC_ERR_OPENMAINFILE     -1      /* Failed to open main file  */
#define VC_ERR_OPENHARDFILE     -2      /* Failed to open hard file  */
#define VC_ERR_READMAINFILE     -3      /* Read error on main file   */
#define VC_ERR_READHARDFILE     -4      /* Read error on hard file   */
#define VC_ERR_WORDTOOLONG      -5      /* Max. word length exceeded */
#define VC_ERR_SYNTAXHARDFILE   -6      /* Improper format hard file */
#define VC_ERR_NOMOREMEMORY     -7      /* Failed to allocate memory */
#define VC_ERR_MEMORYOVERFLOW   -8      /* No memory to add a word   */
#define VC_ERR_WORDALREADYEXISTS -9     /* Word alread in vocab list */
#define VC_ERR_WRITEMAINFILE    -10     /* Failed to write main file */
#define VC_ERR_RENAMEOUTFILE    -11     /* Failed to rename out file */
#define VC_ERR_TOOMANYWORDS     -12     /* VC_MAXWORDS exceeded      */
#define VC_ERR_NOSUCHWORD       -13     /* Word not in list          */


extern int vocab_allocation;
extern char *vocab;

/* vocab_1.c */
int         vocab_destroy(void);

/* vocab_2.c */
int         vocab_load(int allocation_flag);
int         vocab_get_code(char *inp);
char *vocab_get_word(char *out, int inp);

/* vocab_3.c */
int         vocab_write_file(char *last_word);
int         vocab_add_word(char *inp);

/* vocab_4.c */
void        vocab_report_error(int number);

/* vocab_5.c */
char * vocab_select_word(char *out, char *prompt,
	char *default_word);

/* vocab_6.c */
void        vocab_maint(void);

/* vocab_7.c */
int         vocab_build(void);

/* vocab_8.c */
void        vocab_unload_active(void);
void        vocab_init_active(void);
int         vocab_active_id(word id);
int         vocab_make_active(word id);
int         vocab_load_active(void);

extern int vocab_emergency;

extern char *vocab_text;
extern word vocab_active;
extern word vocab_list_id[VOCAB_MAX_ACTIVE];
extern word vocab_list_pointer[VOCAB_MAX_ACTIVE];

/* vocab_9.c */
char *vocab_string(int vocab_id);

} // namespace MADSV2
} // namespace MADS

#endif
