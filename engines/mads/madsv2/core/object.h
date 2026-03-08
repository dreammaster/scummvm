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

#ifndef MADS_CORE_OBJECT_H
#define MADS_CORE_OBJECT_H

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/vocab.h"
#include "mads/madsv2/core/qual.h"
#include "mads/madsv2/core/inter.h"

namespace MADS {
namespace MADSV2 {


#define OBJECT_FILE_VERSION     "4.0"   /* Object DEF file format version */

#define OBJECT_MAX_VERBS        5
#define OBJECT_MAX_QUALITIES    4

#define OBJECT_SHORT_NAME_LEN   5

#define OBJECT_VIEW_OFFSET      6
#define OBJECT_GREY_BASE        248
#define OBJECT_GREY_COLORS      8
#define OBJECT_GREY_SPEED       1
#define OBJECT_GREY_STEPS       16


typedef struct {
	char name[VC_MAXWORDLEN + 1];
	byte verb_type;
	byte prep_type;
} HagVerb;


/* Format for storing objects on disk */

struct FileObjectBuf {
	int  number;                            /* Official object number */
	char vocab_name[VC_MAXWORDLEN + 1];
	char variable_name[VC_MAXWORDLEN + 1];
	char desc[80];
	int  location;
	byte prep;
	byte num_verbs;
	byte num_qualities;
	byte syntax;
	HagVerb verb[OBJECT_MAX_VERBS];
	char quality_name[OBJECT_MAX_QUALITIES][QU_MAXWORDLEN + 1];
	long quality_value[OBJECT_MAX_QUALITIES];
	/* char short_name[OBJECT_SHORT_NAME_LEN + 1]; */
};

typedef struct FileObjectBuf FileObject;
typedef FileObject *FileObjectPtr;

struct ObjectBuf {
	word vocab_id;                                /* Vocab word for name     */
	int  location;                                /* Current location        */
	byte prep;                                    /* "Put" preposition       */
	byte num_verbs;                               /* Number of special verbs */
	byte num_qualities;                           /* Number of qualities     */
	byte syntax;                                  /* Syntax                  */
	Verb verb[OBJECT_MAX_VERBS];                  /* Verb list for objects   */
	byte quality_id[OBJECT_MAX_QUALITIES];        /* Quality list            */
	long quality_value[OBJECT_MAX_QUALITIES];     /* Quality values          */
};

typedef struct ObjectBuf Object;
typedef Object *ObjectPtr;


extern ObjectPtr object;
extern int       num_objects;

extern int object_ems_handle;


/* object_1.cpp */
int  fastcall object_load(void);
void fastcall object_unload(void);

/* object_2.cpp */
int fastcall object_named(int vocab_id);
int fastcall object_is_here(int object_id);

/* object_3.cpp */
int  fastcall object_has_quality(int object_id, int quality_id);
long fastcall object_check_quality(int object_id, int quality_id);
void fastcall object_set_quality(int object_id, int quality_id,
	long quality_value);

/* object_4.cpp */
int  fastcall object_examine(int number, long message, int speech);

} // namespace MADSV2
} // namespace MADS

#endif
