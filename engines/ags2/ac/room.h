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

#ifndef AGS2_AC_ROOM_H
#define AGS2_AC_ROOM_H

#include "common/serializer.h"
#include "common/stream.h"
#include "common/textconsole.h"
#include "ags2/ac/platform.h"
#include "ags2/common/file.h"
#include "ags2/common/cscomp.h"
#include "ags2/common/wgt2allg.h"
#include "ags2/data/character_info.h"
#include "ags2/data/words_dictionary.h"

namespace AGS2 {

#define EXIT_NORMAL 91
#define EXIT_CRASH  92

#define ROOM_FILE_VERSION 29

// object flags (currently only a char)
#define OBJF_NOINTERACT        1  // not clickable
#define OBJF_NOWALKBEHINDS     2  // ignore walk-behinds
#define OBJF_HASTINT           4  // the tint_* members are valid
#define OBJF_USEREGIONTINTS    8  // obey region tints/light areas
#define OBJF_USEROOMSCALING 0x10  // obey room scaling areas
#define OBJF_SOLID          0x20  // blocks characters from moving
#define OBJF_DELETED        0x40  // object has been deleted

#define NOT_VECTOR_SCALED -10000
#define TINT_IS_ENABLED 0x80000000

//const int MISC_COND = MAX_OBJ * 4 + NUMOTCON + MAX_INIT_SPR * 4;

// NUMCONDIT : whataction[0]:  Char walks off left
//                       [1]:  Char walks off right
//                       [2]:  Char walks off bottom
//                       [3]:  Char walks off top
//			 [4]:  First enters screen
//                       [5]:  Every time enters screen
//                       [6]:  execute every loop
//                [5]...[19]:  Char stands on lookat type
//               [20]...[35]:  Look at type
//               [36]...[49]:  Action on type
//               [50]...[65]:  Use inv on type
//               [66]...[75]:  Look at object
//               [76]...[85]:  Action on object
//               [86]...[95]:  Speak to object
//		[96]...[105]:  Use inv on object
//             [106]...[124]:  Misc conditions 1-20

// game ver     whataction[]=
// v1.00              0  :  Go to screen
//                    1  :  Don't do anything
//                    2  :  Can't walk
//                    3  :  Man dies
//                    4  :  Run animation
//                    5  :  Display message
//                    6  :  Remove an object (set object.on=0)
//		                7  :  Remove object & add Val2 to inventory
//                    8  :  Add Val1 to inventory (Val2=num times)
//                    9  :  Run a script
// v1.00 SR-1        10  :  Run graphical script
// v1.1              11  :  Play sound effect SOUND%d.WAV
// v1.12             12  :  Play FLI/FLC animation FLIC%d.FLC or FLIC%d.FLI
//                   13  :  Turn object on
// v2.00             14  :  Run conversation
#define NUMRESPONSE   14
#define NUMCOMMANDS   15
#define GO_TO_SCREEN  0
#define NO_ACTION     1
#define NO_WALK       2
#define MAN_DIES      3
#define RUN_ANIMATE   4
#define SHOW_MESSAGE  5
#define OBJECT_OFF    6
#define OBJECT_INV    7
#define ADD_INV       8
#define RUNSCRIPT     9
#define GRAPHSCRIPT   10
#define PLAY_SOUND    11
#define PLAY_FLI      12
#define OBJECT_ON     13
#define RUN_DIALOG    14

#ifdef DJGPP
#include <unistd.h>
#endif

#ifdef _MSC_VER
#undef VTA_LEFT
#undef VTA_RIGHT
#endif


// thisroom.options[0] = startup music
// thisroom.options[1] = can save/load on screen (0=yes, 1=no)
// thisroom.options[2] = player character disabled? (0=no, 1=yes)
// thisroom.options[3] = player special view (0=normal)
//                 [4] = music volume (0=normal, <0 quiter, >0 louder)

const int ST_TUNE = 0, ST_SAVELOAD = 1, ST_MANDISABLED = 2, ST_MANVIEW = 3, ST_VOLUME = 4;

#ifndef CROOM_NOFUNCTIONS

extern block backups[5];

extern int cunpackbitl(unsigned char *, int size, Common::SeekableReadStream *infile);

#endif

#ifdef DJGPP
#define PCKD __attribute__((packed))
#else
#define PCKD
#endif

#include "common/pack-start.h"  // START STRUCT PACKING

struct sprstruc {
	short sprnum = 0;		// number from array
	short x = 0, y = 0;		// x,y co-ords
	short room = 0;			// room number
	short on = 0;

	void ReadFromFile(Common::SeekableReadStream *fp) {
		sprnum = fp->readSint16BE();
		x = fp->readSint16BE();
		y = fp->readSint16BE();
		room = fp->readSint16BE();
		on = fp->readSint16BE();
	}
} PACKED_STRUCT;

#define MSG_DISPLAYNEXT 1 // supercedes using alt-200 at end of message
#define MSG_TIMELIMIT   2
struct MessageInfo {
	char  displayas  PCKD; // 0 = normal window, 1 = as speech
	char  flags      PCKD; // combination of MSG_xxx flags

	void ReadFromFile(Common::SeekableReadStream *fp) {
		displayas = fp->readByte();
		flags = fp->readByte();
	}
} PACKED_STRUCT;

#define AE_WAITFLAG   0x80000000
#define MAXANIMSTAGES 10
struct AnimationStruct {
	int32 x = 0, y = 0;
	int32 data = 0;
	int32 object = 0;
	int32 speed = 5;
	int8 action = 0;
	int8 wait = 1;

} PACKED_STRUCT;

struct FullAnimation {
  AnimationStruct stage[MAXANIMSTAGES];
  int32 numstages;
  FullAnimation() { numstages = 0; }
} PACKED_STRUCT;

struct _Point {
	int16 x, y;
} PACKED_STRUCT;

#include "common/pack-end.h"    // END STRUCT PACKING

#define MAXCOMMANDS 8
struct EventBlock {
  int   list[MAXCOMMANDS];
  int   respond[MAXCOMMANDS];
  int   respondval[MAXCOMMANDS];
  int   data[MAXCOMMANDS];
  int   numcmd;
  short score[MAXCOMMANDS];
};

// careful with this - the shadinginfo[] array needs to be
// MAX_WALK_AREAS + 1 if this gets changed

#define MAXPOINTS 30
struct PolyPoints {
	int x[MAXPOINTS];
	int y[MAXPOINTS];
	int numpoints;
	void add_point(int xxx, int yyy) {
		x[numpoints] = xxx;
		y[numpoints] = yyy;
		numpoints++;

		if (numpoints >= MAXPOINTS)
			quit("too many poly points added");
	}
	PolyPoints() {
		numpoints = 0;
	}

	void ReadFromFile(Common::SeekableReadStream *fp) {
		int i;
		for (i = 0; i < MAXPOINTS; ++i)
			x[i] = fp->readSint32LE();
		for (i = 0; i < MAXPOINTS; ++i)
			y[i] = fp->readSint32LE();
		numpoints = getw(fp);
	}
};

#define POPUP_NONE      0
#define POPUP_MOUSEY    1
#define POPUP_SCRIPT    2
#define POPUP_NOAUTOREM 3  // don't remove automatically during cutscene
#define POPUP_NONEINITIALLYOFF 4   // normal GUI, initially off
#define VTA_LEFT        0
#define VTA_RIGHT       1
#define VTA_CENTRE      2
#define IFLG_TEXTWINDOW 1

#define MAXBUTTON       20
#define IBACT_SETMODE   1
#define IBACT_SCRIPT    2
#define IBFLG_ENABLED   1
#define IBFLG_INVBOX    2
struct InterfaceButton {
  int x, y, pic, overpic, pushpic, leftclick;
  int rightclick; // if inv, then leftclick = wid, rightclick = hit
  int reserved_for_future;
  char flags;
  void set(int xx, int yy, int picc, int overpicc, int actionn) {
    x = xx; y = yy; pic = picc; overpic = overpicc; leftclick = actionn; pushpic = 0;
    rightclick = 0; flags = IBFLG_ENABLED;
    reserved_for_future = 0;
  }
};

// this struct should go in a Game struct, not the room structure.
struct InterfaceElement {
	int             x = 0, y = 0, x2 = 0, y2 = 0;
	int             bgcol = 8, fgcol = 15, bordercol = 0;
	int             vtextxp = 1, vtextyp = 0, vtextalign = 0;  // X & Y relative to topleft of interface
	char            vtext[40] = "@SCORETEXT@$r@GAMENAME@";
	int             numbuttons = 0;
	InterfaceButton button[MAXBUTTON];
	int             flags = 0;
	int             reserved_for_future = 0;
	int             popupyp;   // pops up when mousey < this
	char            popup;     // does it pop up? (like sierra icon bar)
	char            on = 1;
};

#define ANYWORD     29999
#define RESTOFLINE  30000

#define MAX_CUSTOM_PROPERTIES 30
#define MAX_CUSTOM_PROPERTY_VALUE_LENGTH 500
#define PROP_TYPE_BOOL   1
#define PROP_TYPE_INT    2
#define PROP_TYPE_STRING 3

struct CustomPropertySchema {
	char propName[MAX_CUSTOM_PROPERTIES][20];
	char propDesc[MAX_CUSTOM_PROPERTIES][100];
	char *defaultValue[MAX_CUSTOM_PROPERTIES];
	int propType[MAX_CUSTOM_PROPERTIES];
	int numProps;

  // Find the index of the specified property
	int findProperty(const char *pname) {
		for (int ii = 0; ii < numProps; ii++) {
			if (scumm_stricmp(pname, propName[ii]) == 0)
				return ii;
		}
		return -1;
	}
  
	void deleteProperty(int idx) {
		if ((idx < 0) || (idx >= numProps))
			return;

		if (defaultValue[idx])
			delete defaultValue[idx];

		numProps--;
		for (int qq = idx; qq < numProps; qq++) {
			Common::strcpy_s(propName[qq], propName[qq + 1]);
			Common::strcpy_s(propDesc[qq], propDesc[qq + 1]);
			propType[qq] = propType[qq + 1];
			defaultValue[qq] = defaultValue[qq + 1];
		}
		defaultValue[numProps] = NULL;
	}

	void resetProperty(int idx) {
		propName[idx][0] = 0;
		propDesc[idx][0] = 0;
		if (defaultValue[idx])
			delete defaultValue[idx];
		propType[idx] = PROP_TYPE_BOOL;
		defaultValue[idx] = new char[MAX_CUSTOM_PROPERTY_VALUE_LENGTH];
		defaultValue[idx][0] = 0;
	}

	CustomPropertySchema() {
		numProps = 0;
		for (int kk = 0; kk < MAX_CUSTOM_PROPERTIES; kk++) {
			defaultValue[kk] = NULL;
		}
	}

	void Serialize(Common::WriteStream *outto);
	int UnSerialize(Common::SeekableReadStream *infrom);
};


struct CustomProperties {
	char *propName[MAX_CUSTOM_PROPERTIES] = {};
	char *propVal[MAX_CUSTOM_PROPERTIES] = {};
	int   numProps = 0;
  
	CustomProperties() {
		numProps = 0;
	}

	const char *getPropertyValue(const char *pname) {
		int idxx = findProperty(pname);
		if (idxx < 0)
			return NULL;

		return propVal[idxx];
	}

	// Find the index of the specified property
	int findProperty(const char *pname) {
		for (int ii = 0; ii < numProps; ii++) {
			if (scumm_stricmp(pname, propName[ii]) == 0)
				return ii;
		}
		return -1;
	}

	void reset() {
		for (int ii = 0; ii < numProps; ii++) {
			free(propName[ii]);
			free(propVal[ii]);
		}
		numProps = 0;
	}

	void addProperty(const char *newname, const char *newval) {
		if (numProps >= MAX_CUSTOM_PROPERTIES) {
			return;
		}
		propName[numProps] = (char *)malloc(200);
		propVal[numProps] = (char *)malloc(MAX_CUSTOM_PROPERTY_VALUE_LENGTH);
		Common::strcpy_s(propName[numProps], 200, newname);
		Common::strcpy_s(propVal[numProps], MAX_CUSTOM_PROPERTY_VALUE_LENGTH, newval);
		numProps++;
	}

	void Serialize(Common::WriteStream *outto);
	int  UnSerialize (Common::SeekableReadStream *infrom);
};

/* THE WAY THIS WORKS:
 *
 * NewInteraction (Hotspot 1)
 *  |
 *  +-- eventTypes [NUM_EVENTS]
 *  +-- NewInteractionCommandList [NUM_EVENTS]   (Look at hotspot)
 *        |
 *        +-- NewInteractionCommand [NUM_COMMANDS]   (Display message)
 *             |
 *             +-- NewInteractionValue [NUM_ARGUMENTS]   (5)
 */

#define LOCAL_VARIABLE_OFFSET 10000
#define MAX_GLOBAL_VARIABLES 100
#define MAX_ACTION_ARGS 5
#define MAX_NEWINTERACTION_EVENTS 30
#define MAX_COMMANDS_PER_LIST 40
#define VALTYPE_LITERALINT 1
#define VALTYPE_VARIABLE   2
#define VALTYPE_BOOLEAN    3
#define VALTYPE_CHARNUM    4

struct NewInteractionValue {
	byte valType;
	int  val;
	int  extra;

	NewInteractionValue() {
		valType = VALTYPE_LITERALINT;
		val = 0;
		extra = 0;
	}

	void ReadFromFile(Common::SeekableReadStream *fp) {
		valType = (byte)fp->readUint32LE();
		val = getw(fp);
		extra = getw(fp);
	}
	void WriteToFile(Common::WriteStream *fp) {
		fp->writeUint32LE(valType);
		putw(val, fp);
		putw(extra, fp);
	}
};

struct NewInteractionAction {
	virtual ~NewInteractionAction() {}
	virtual void reset() = 0;
};
struct NewInteractionCommandList;

struct NewInteractionCommand : public NewInteractionAction {
	int32 type;
	NewInteractionValue data[MAX_ACTION_ARGS];
	NewInteractionAction *children;
	NewInteractionCommandList *parent;

	NewInteractionCommand() {
		type = 0;
		children = NULL;
		parent = NULL;
	}
	~NewInteractionCommand() override {
	}
	NewInteractionCommandList *get_child_list() {
		return (NewInteractionCommandList *)children;
	}
	void remove();

	void reset() {
		remove();
	}

	void ReadFromFile(Common::SeekableReadStream *fp) {
		getw(fp); // skip the vtbl ptr
		type = getw(fp);
		for (int i = 0; i < MAX_ACTION_ARGS; ++i)
		{
			data[i].ReadFromFile(fp);
		}
		// all that matters is whether or not these are null...
		children = (NewInteractionAction *)getw(fp);
		parent = (NewInteractionCommandList *)getw(fp);
	}
	void WriteToFile(Common::WriteStream *fp) {
		putw(0, fp); // write dummy vtbl ptr 
		putw(type, fp);
		for (int i = 0; i < MAX_ACTION_ARGS; ++i) {
			data[i].WriteToFile(fp);
		}
		putw((int)children, fp);
		putw((int)parent, fp);
	}
};

struct NewInteractionCommandList : public NewInteractionAction {
  int32 numCommands;
  NewInteractionCommand command[MAX_COMMANDS_PER_LIST];
  int32 timesRun;   // used by engine to track score changes

  NewInteractionCommandList () {
    numCommands = 0;
    timesRun = 0;
  }
  ~NewInteractionCommandList() override {}
  void reset();
};

struct NewInteraction {
	int numEvents;

	// the first few event types depend on the item - ID's of 100+ are
	// custom events (used for subroutines)
	int eventTypes[MAX_NEWINTERACTION_EVENTS];
	int timesRun[MAX_NEWINTERACTION_EVENTS];
	NewInteractionCommandList *response[MAX_NEWINTERACTION_EVENTS];


	NewInteraction() {
		numEvents = 0;
		// NULL all the pointers
		memset(response, 0, sizeof(NewInteractionCommandList *) * MAX_NEWINTERACTION_EVENTS);
		memset(&timesRun[0], 0, sizeof(int) * MAX_NEWINTERACTION_EVENTS);
	}

	void copy_timesrun_from(NewInteraction *nifrom) {
		memcpy(&timesRun[0], &nifrom->timesRun[0], sizeof(int) * MAX_NEWINTERACTION_EVENTS);
	}

	void reset() {
		for (int i = 0; i < numEvents; i++) {
			if (response[i] != NULL) {
				response[i]->reset();
				delete response[i];
				response[i] = NULL;
			}
		}
		numEvents = 0;
	}
	~NewInteraction() {
		reset();
	}

	void synchronize(Common::Serializer &s) {
		s.syncAsSint32LE(numEvents);
		for (int i = 0; i < MAX_NEWINTERACTION_EVENTS; ++i)
			s.syncAsSint32LE(eventTypes[i]);
		for (int i = 0; i < MAX_NEWINTERACTION_EVENTS; ++i)
			s.syncAsSint32LE(timesRun[i]);

		// dummy sync for responses pointers
		for (int i = 0; i < MAX_NEWINTERACTION_EVENTS; ++i) {
			int dummy = 0;
			s.syncAsSint32LE(dummy);
		}
	}

	void ReadFromFile(Common::SeekableReadStream *fp) {
		Common::Serializer s(fp, nullptr);
		synchronize(s);
	}
	void WriteToFile(Common::WriteStream *fp) {
		Common::Serializer s(nullptr, fp);
		synchronize(s);
	}
};

/**
 * This struct is only used in save games and by plugins
 */
struct RoomObject {
	int   x = 0, y = 0;
	int   transparent = 0;    // current transparency setting
	short tint_r = 0, tint_g = 0;   // specific object tint
	short tint_b = 0, tint_level = 0;
	short tint_light = 0;
	short last_zoom = 0;      // zoom level last time
	short last_width = 0, last_height = 0;   // width/height last time drawn
	short num = 0;            // sprite slot number
	short baseline = 0;       // <=0 to use Y co-ordinate; >0 for specific baseline
	short view = 0, loop = 0, frame = 0; // only used to track animation - 'num' holds the current sprite
	short wait = 0, moving = 0;
	char  cycling = 0;        // is it currently animating?
	char  overall_speed = 0;
	char  on = 0;
	char  flags = 0;
	short blocking_width = 0, blocking_height = 0;

	int get_width() const;
	int get_height() const;
	int get_baseline() const;

	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};


/**
 * This struct is saved in the save games - it contains everything about
 * a room that could change
 */
struct RoomStatus {
	int beenhere = 0;
	int numobj = 0;
	RoomObject obj[MAX_INIT_SPR];
	short flagstates[MAX_FLAGS] = {};
	int tsdatasize = 0;
	char *tsdata = nullptr;
	NewInteraction intrHotspot[MAX_HOTSPOTS];
	NewInteraction intrObject[MAX_INIT_SPR];
	NewInteraction intrRegion[MAX_REGIONS];
	NewInteraction intrRoom;

	char hotspot_enabled[MAX_HOTSPOTS] = {};
	char region_enabled[MAX_REGIONS] = {};
	short walkbehind_base[MAX_OBJ] = {};
	int interactionVariableValues[MAX_GLOBAL_VARIABLES] = {};

	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *f) {
		Common::Serializer s(f, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *f) {
		Common::Serializer s(nullptr, f);
		synchronize(s);
	}
};

#define NUM_ACTION_TYPES 48
#define ARG_TYPE_INT 1
#define ARG_TYPE_INV 2
#define ARG_TYPE_MSG 3
#define ARG_TYPE_CHAR 4
#define ARG_TYPE_BOOL 5
#define ARG_TYPE_VARONLY 6  // must be variable, no literal values
#define AFLG_COND      1
#define AFLG_RUNSCRIPT 2
#define AFLG_INVCHECK  4
#define AFLG_MESSAGE   8

struct ActionTypes {
  char  name[80];
  short flags;
  char  numArgs;
  char  argTypes[MAX_ACTION_ARGS];
  char  argNames[MAX_ACTION_ARGS][25];
  char  description[200];
  char  textscript[80];
};
extern ActionTypes actions[NUM_ACTION_TYPES];

struct InteractionVariable {
	char name[23];
	char type;
	int  value;

	void synchronize(Common::Serializer &s) {
		s.syncBytes((byte *)name, 23);
		s.syncAsByte(type);
		s.syncAsSint32LE(value);
	}

	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};

struct InteractionScripts {
  int numEvents;
  char *scriptFuncNames[MAX_NEWINTERACTION_EVENTS];

  InteractionScripts() {
    numEvents = 0;
  }

  ~InteractionScripts() {
    for (int i = 0; i < numEvents; i++)
      delete scriptFuncNames[i];
  }
};

struct roomstruct {
  block         walls, object, lookat;          // 'object' is the walk-behind
  block         regions;
  color         pal[256];
  short         numobj;                         // num hotspots, not sprites
  short         objyval[MAX_OBJ];               // baselines of walkbehind areas
  // obsolete v2.00 action editor stuff below
  short         whataction[NUM_CONDIT+3];       // what to do if condition appears
  short         val1[NUM_CONDIT+3];             // variable, eg. screen number to go to
  short         val2[NUM_CONDIT+3];             // 2nd var, optional, eg. which side of screen to come on
  short         otcond[NUM_CONDIT+3];           // do extra misc condition
  char          points[NUM_CONDIT+3];           // extra points for doing it
  // end obsolete v2.00 action editor
  short         left,right,top,bottom;          // to walk off screen
  short         numsprs,nummes;                 // number of initial sprites and messages
  sprstruc      sprs[MAX_INIT_SPR];             // structures for each sprite
  NewInteraction *intrObject[MAX_INIT_SPR];
  InteractionScripts **objectScripts;
  int           objbaseline[MAX_INIT_SPR];                // or -1 (use bottom of object graphic)
  short         objectFlags[MAX_INIT_SPR];
  char          objectnames[MAX_INIT_SPR][MAXOBJNAMELEN];
  char          objectscriptnames[MAX_INIT_SPR][MAX_SCRIPT_NAME_LEN];
  CustomProperties objProps[MAX_INIT_SPR];
  char          password[11];
  char          options[10];                    // [0]=startup music
  char          *message[MAXMESS];
  MessageInfo   msgi[MAXMESS];
  short         wasversion;                     // when loaded from file
  short         flagstates[MAX_FLAGS];
  FullAnimation anims[MAXANIMS];
  short         numanims;
  short         shadinginfo[16];    // walkable area-specific view number
  // new version 2 roommake stuff below
  int           numwalkareas;
  PolyPoints    wallpoints[MAX_WALK_AREAS];
  int           numhotspots;
  _Point        hswalkto[MAX_HOTSPOTS];
  char*         hotspotnames[MAX_HOTSPOTS];
  char          hotspotScriptNames[MAX_HOTSPOTS][MAX_SCRIPT_NAME_LEN];
  NewInteraction *intrHotspot[MAX_HOTSPOTS];
  NewInteraction *intrRoom;
  NewInteraction *intrRegion[MAX_REGIONS];
  InteractionScripts **hotspotScripts;
  InteractionScripts **regionScripts;
  InteractionScripts *roomScripts;
  int           numRegions;
  short         regionLightLevel[MAX_REGIONS];
  int           regionTintLevel[MAX_REGIONS];
  short         width,height;                             // in 320x200 terms (scrolling room size)
  short         resolution;                               // 1 = 320x200, 2 = 640x400
  short         walk_area_zoom[MAX_WALK_AREAS + 1];       // 0 = 100%, 1 = 101%, -1 = 99%
  short         walk_area_zoom2[MAX_WALK_AREAS + 1];      // for vector scaled areas
  short         walk_area_light[MAX_WALK_AREAS + 1];      // 0 = normal, + lighter, - darker
  short         walk_area_top[MAX_WALK_AREAS + 1];     // top YP of area
  short         walk_area_bottom[MAX_WALK_AREAS + 1];  // bottom YP of area
  char          *scripts;
  ccScript      *compiled_script;
  int           cscriptsize;
  int           num_bscenes, bscene_anim_speed;
  int           bytes_per_pixel;
  block         ebscene[MAX_BSCENE];
  color         bpalettes[MAX_BSCENE][256];
  InteractionVariable *localvars;
  int           numLocalVars;
  char          ebpalShared[MAX_BSCENE];  // used internally by engine atm
  CustomProperties roomProps;
  CustomProperties hsProps[MAX_HOTSPOTS];
  int           gameId;
  int           lastLoadNumHotspots;
  int           lastLoadNumObjects;
  int           lastLoadNumRegions;

  roomstruct() {
    ebscene[0] = NULL; walls = NULL; object = NULL; lookat = NULL; nummes = 0;
    left = 0; right = 317; top = 40; bottom = 199; numobj = MAX_OBJ; numsprs = 0; password[0] = 0;
    wasversion = ROOM_FILE_VERSION; numanims = 0; regions = NULL; numwalkareas = 0;
    numhotspots = 0;
    memset(&objbaseline[0], 0xff, sizeof(int) * MAX_INIT_SPR);
    memset(&objectFlags[0], 0, sizeof(short) * MAX_INIT_SPR);
    width = 320; height = 200; scripts = NULL; compiled_script = NULL;
    cscriptsize = 0;
    memset(&walk_area_zoom[0], 0, sizeof(short) * (MAX_WALK_AREAS + 1));
    memset(&walk_area_light[0], 0, sizeof(short) * (MAX_WALK_AREAS + 1));
    resolution = 1; num_bscenes = 1; ebscene[0] = NULL;
    bscene_anim_speed = 5; bytes_per_pixel = 1;
    numLocalVars = 0;
    localvars = NULL;
    lastLoadNumHotspots = 0;
    lastLoadNumRegions = 0;
    lastLoadNumObjects = 0;
    int i;
    for (i = 0; i <= MAX_WALK_AREAS; i++) {
      walk_area_zoom2[i] = NOT_VECTOR_SCALED;
      walk_area_top[i] = -1;
      walk_area_bottom[i] = -1;
    }
    for (i = 0; i < MAX_HOTSPOTS; i++) {
      intrHotspot[i] = new NewInteraction();
	  hotspotnames[i] = NULL;
      hotspotScriptNames[i][0] = 0;
    }
    for (i = 0; i < MAX_INIT_SPR; i++)
      intrObject[i] = new NewInteraction();
    for (i = 0; i < MAX_REGIONS; i++)
      intrRegion[i] = new NewInteraction();
    intrRoom = new NewInteraction();
    gameId = 0;
	numRegions = 0;
	hotspotScripts = NULL;
	regionScripts = NULL;
	objectScripts = NULL;
	roomScripts = NULL;
  }
  //void allocall();
  //void freeall();
  void freemessage();
};

extern int in_interaction_editor;  // whether to remove script functions/etc

#pragma pack(1)
struct ScriptEvent {
  long type     PCKD;   // eg. display message, or if is less
  char sort     PCKD;
  long _using   PCKD;   // ^var1
  long with     PCKD;   // number 3 than 9
  long data     PCKD;
  long branchto PCKD;
  long screeny  PCKD;
  void settype(long);
};

#define MAXINBLOCK 10
struct ScriptBlock {
  long        numevents           PCKD;
  ScriptEvent events[MAXINBLOCK]  PCKD;
};
#pragma pack()

#define VFLG_FLIPSPRITE 1

struct ViewFrame {
	int   pic;
	short xoffs, yoffs;
	short speed;
	int   flags;
	int   sound;  // play sound when this frame comes round
	int   reserved_for_future[2];
	ViewFrame() {
		pic = 0; xoffs = 0; yoffs = 0; speed = 0;
	}

	void synchronize(Common::Serializer &s) {
		s.syncAsSint32LE(pic);
		s.syncAsSint16LE(xoffs);
		s.syncAsSint16LE(yoffs);
		s.syncAsSint32LE(speed);

		s.syncAsSint32LE(flags);
		s.syncAsSint32LE(sound);
		s.syncAsSint32LE(reserved_for_future[0]);
		s.syncAsSint32LE(reserved_for_future[1]);
	}
	void ReadFromFile(Common::SeekableReadStream *fp) {
		Common::Serializer s(fp, nullptr);
		synchronize(s);
	}
	void WriteToFile(Common::WriteStream *fp) {
		Common::Serializer s(nullptr, fp);
		synchronize(s);
	}
};

#define LOOPFLAG_RUNNEXTLOOP 1

struct ViewLoopNew {
	short numFrames;
	int   flags;
	ViewFrame *frames;

	bool RunNextLoop() {
		return (flags & LOOPFLAG_RUNNEXTLOOP);
	}

	void Initialize(int frameCount);
	void Dispose();
	void WriteToFile(Common::WriteStream *ooo);
	void ReadFromFile(Common::SeekableReadStream *iii);
};

struct ViewStruct {
	short numLoops;
	ViewLoopNew *loops;

	void Initialize(int loopCount);
	void Dispose();
	void WriteToFile(Common::WriteStream *ooo);
	void ReadFromFile(Common::SeekableReadStream *iii);
};

struct ViewStruct272 {
	short     numloops = 0;
	short     numframes[16] = {};
	int       loopflags[16] = {};
	ViewFrame frames[16][20];
};

#define CHF_MANUALSCALING   1
#define CHF_FIXVIEW         2     // between SetCharView and ReleaseCharView
#define CHF_NOINTERACT      4
#define CHF_NODIAGONAL      8
#define CHF_ALWAYSIDLE      0x10
#define CHF_NOLIGHTING      0x20
#define CHF_NOTURNING       0x40
#define CHF_NOWALKBEHINDS   0x80
#define CHF_FLIPSPRITE      0x100  // ?? Is this used??
#define CHF_NOBLOCKING      0x200
#define CHF_SCALEMOVESPEED  0x400
#define CHF_NOBLINKANDTHINK 0x800
#define CHF_SCALEVOLUME     0x1000
#define CHF_HASTINT         0x2000   // engine only
#define CHF_BEHINDSHEPHERD  0x4000   // engine only
#define CHF_AWAITINGMOVE    0x8000   // engine only
#define CHF_MOVENOTWALK     0x10000   // engine only - do not do walk anim
#define CHF_ANTIGLIDE       0x20000
// Speechcol is no longer part of the flags as of v2.5
#define OCHF_SPEECHCOL      0xff000000
#define OCHF_SPEECHCOLSHIFT 24
#define UNIFORM_WALK_SPEED  0
#define FOLLOW_ALWAYSONTOP  0x7ffe


struct OldCharacterInfo {
	int   defview;
	int   talkview;
	int   view;
	int   room, prevroom;
	int   x, y, wait;
	int   flags;
	short following;
	short followinfo;
	int   idleview;           // the loop will be randomly picked
	short idletime, idleleft; // num seconds idle before playing anim
	short transparency;       // if character is transparent
	short baseline;
	int   activeinv;          // this is an INT to support SeeR (no signed shorts)
	short loop, frame;
	short walking, animating;
	short walkspeed, animspeed;
	short inv[100];
	short actx, acty;
	char  name[30];
	char  scrname[16];
	char  on;
};

#define MAXTOPICOPTIONS     30
#define DFLG_ON             1  // currently enabled
#define DFLG_OFFPERM        2  // off forever (can't be trurned on)
#define DFLG_NOREPEAT       4  // character doesn't repeat it when clicked
#define DFLG_HASBEENCHOSEN  8  // dialog option is 'read'
#define DTFLG_SHOWPARSER    1  // show parser in this topic
#define DCMD_SAY            1
#define DCMD_OPTOFF         2
#define DCMD_OPTON          3
#define DCMD_RETURN         4
#define DCMD_STOPDIALOG     5
#define DCMD_OPTOFFFOREVER  6
#define DCMD_RUNTEXTSCRIPT  7
#define DCMD_GOTODIALOG     8
#define DCMD_PLAYSOUND      9
#define DCMD_ADDINV         10
#define DCMD_SETSPCHVIEW    11
#define DCMD_NEWROOM        12
#define DCMD_SETGLOBALINT   13
#define DCMD_GIVESCORE      14
#define DCMD_GOTOPREVIOUS   15
#define DCMD_LOSEINV        16
#define DCMD_ENDSCRIPT      0xff
#define DCHAR_NARRATOR  999
#define DCHAR_PLAYER    998
#define MAX_DIALOG          500

struct DialogTopic {
	char          optionnames[MAXTOPICOPTIONS][150];
	int           optionflags[MAXTOPICOPTIONS];
	unsigned char *optionscripts;
	short         entrypoints[MAXTOPICOPTIONS];
	short         startupentrypoint;
	short         codesize;
	int           numoptions;
	int           topicFlags;

	void load(Common::SeekableReadStream *src);
};

#define PAL_GAMEWIDE        0
#define PAL_LOCKED          1
#define PAL_BACKGROUND      2

#define OPT_DEBUGMODE       0
#define OPT_SCORESOUND      1
#define OPT_WALKONLOOK      2
#define OPT_DIALOGIFACE     3
#define OPT_ANTIGLIDE       4
#define OPT_TWCUSTOM        5
#define OPT_DIALOGGAP       6
#define OPT_NOSKIPTEXT      7
#define OPT_DISABLEOFF      8
#define OPT_ALWAYSSPCH      9
#define OPT_SPEECHTYPE      10
#define OPT_PIXPERFECT      11
#define OPT_NOWALKMODE      12
#define OPT_LETTERBOX       13
#define OPT_FIXEDINVCURSOR  14
#define OPT_NOLOSEINV       15
#define OPT_NOSCALEFNT      16
#define OPT_SPLITRESOURCES  17
#define OPT_ROTATECHARS     18
#define OPT_FADETYPE        19
#define OPT_HANDLEINVCLICKS 20
#define OPT_MOUSEWHEEL      21
#define OPT_DIALOGNUMBERED  22
#define OPT_DIALOGUPWARDS   23
#define OPT_CROSSFADEMUSIC  24
#define OPT_ANTIALIASFONTS  25
#define OPT_THOUGHTGUI      26
#define OPT_TURNTOFACELOC   27
#define OPT_RIGHTLEFTWRITE  28  // right-to-left text writing
#define OPT_DUPLICATEINV    29  // if they have 2 of the item, draw it twice
#define OPT_SAVESCREENSHOT  30
#define OPT_PORTRAITSIDE    31
#define OPT_STRICTSCRIPTING 32  // don't allow MoveCharacter-style commands
#define OPT_LEFTTORIGHTEVAL 33  // left-to-right operator evaluation
#define OPT_COMPRESSSPRITES 34
#define OPT_STRICTSTRINGS   35  // don't allow old-style strings
#define OPT_NEWGUIALPHA     36
#define OPT_RUNGAMEDLGOPTS  37
#define OPT_NATIVECOORDINATES 38
#define OPT_OLDTALKANIMSPD  39
#define OPT_HIGHESTOPTION   39
#define OPT_NOMODMUSIC      98
#define OPT_LIPSYNCTEXT     99
#define PORTRAIT_LEFT       0
#define PORTRAIT_RIGHT      1
#define PORTRAIT_ALTERNATE  2
#define PORTRAIT_XPOSITION  3
#define FADE_NORMAL         0
#define FADE_INSTANT        1
#define FADE_DISSOLVE       2
#define FADE_BOXOUT         3
#define FADE_CROSSFADE      4
#define FADE_LAST           4   // this should equal the last one
#define SPF_640x400         1
#define SPF_HICOLOR         2
#define SPF_DYNAMICALLOC    4
#define SPF_TRUECOLOR       8
#define SPF_ALPHACHANNEL 0x10
#define SPF_HADALPHACHANNEL 0x80  // the saved sprite on disk has one
//#define FFLG_NOSCALE        1
#define FFLG_SIZEMASK 0x003f
#define FONT_OUTLINE_AUTO -10
#define MAX_FONT_SIZE 63

struct SpeechLipSyncLine {
	char  filename[14];
	int *endtimeoffs;
	short *frame;
	short numPhenomes;
};

// permission flags
#define SMP_NOEDITINFO    1
#define SMP_NOEDITSCRIPTS 2
struct ScriptModule {
	char *name;
	char *author;
	char *version;
	char *description;
	char *scriptHeader;
	char *script;
	int  uniqueKey;
	int  permissions;
	int  weAreOwner;
	ccScript *compiled;

	void init() {
		name = NULL;
		author = NULL;
		version = NULL;
		description = NULL;
		script = NULL;
		scriptHeader = NULL;
		uniqueKey = 0;
		permissions = 0;
		weAreOwner = 1;
		compiled = NULL;
	}

	ScriptModule() {
		init();
	}
};

#define MAXNEEDSTAGES 40
struct MoveList {
	int   pos[MAXNEEDSTAGES];
	int   numstage;
	fixed xpermove[MAXNEEDSTAGES], ypermove[MAXNEEDSTAGES];
	int   fromx, fromy;
	int   onstage, onpart;
	int   lastx, lasty;
	char  doneflag;
	char  direct;  // MoveCharDirect was used or not

	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};

extern void new_room(int newnum, CharacterInfo *forchar);
extern void read_string_decrypt(Common::SeekableReadStream *ooo, char *sss);
extern void load_room(char *files, roomstruct *rstruc, bool gameIsHighRes);
extern void deserialize_interaction_scripts(Common::SeekableReadStream *iii, InteractionScripts *scripts);
extern void read_dictionary(WordsDictionary *dict, Common::SeekableReadStream *writeto);

} // namespace AGS2

#endif
