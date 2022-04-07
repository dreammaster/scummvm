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

#include "dink/scripts.h"
#include "dink/dink.h"
#include "dink/events.h"
#include "dink/file.h"
#include "dink/globals.h"
#include "dink/graphics.h"
#include "dink/music.h"
#include "dink/savegame.h"
#include "dink/sound.h"
#include "dink/text.h"
#include "dink/var.h"
#include "dink/directdraw/ddutil.h"

namespace Dink {

static int change_sprite_inner(int h, int val, int *change);
static int process_line(int script, char *s, bool doelse);

template<typename TYPE>
long change_sprite(int h, int val, TYPE *change) {
	int temp = *change;
	long result = (long)change_sprite_inner(h, val, &temp);
	*change = temp;

	return result;
}

static int change_sprite_inner(int h, int val, int *change) {
	if (h < 1) {
		Msg("Error with an SP command - Sprite %d is invalid.", h);
		return -1;
	}
	if (spr[h].active == false)
		return -1;
	if (val != -1) {
		*change = val;
	}

	return *change;
}

static int var_equals(char name[20], char newname[20], char math_, int script, char rest[200]) {
	int k;
	//redink1 set newret to NULL so debug errors did not appear.
	int newret = 0; // NULL;

	if (name[0] != '&') {
		Msg("ERROR (var equals): Unknown var %s in %s offset %d.", name, rinfo[script]->name, rinfo[script]->current);
		return 0;
	}

	int i = get_var(script, name);
	if (i > 0) {
		goto next;
	}

	Msg("ERROR: (var equals2) Unknown var %s in %s offset %d.", name, rinfo[script]->name, rinfo[script]->current);
	return 0;

next:
	int newval = 0;

	if (strchr(rest, '(') != nullptr) {
		newret = process_line(script, rest, false);
		newval = returnint;
		goto next2;
	}

	if (strchr(newname, ';') != NULL)
		replace(";", "", newname);

	// redink1 fixed for scope and such
	k = get_var(script, newname);
	if (k > 0) {
		newval = play.var[k].var;
		goto next2;
	}

	if (compare(newname, "&current_sprite")) {
		newval = rinfo[script]->sprite;
		goto next2;
	}

	if (compare(newname, "&current_script")) {
		newval = script;
		goto next2;

	}

	// v1.08 special variables.
	if (compare(newname, "&return")) {
		newval = returnint;
		goto next2;
	}

	if (compare(newname, "&arg1")) {
		newval = rinfo[script]->arg1;
		goto next2;
	}

	if (compare(newname, "&arg2")) {
		newval = rinfo[script]->arg2;
		goto next2;
	}

	if (compare(newname, "&arg3")) {
		newval = rinfo[script]->arg3;
		goto next2;
	}

	if (compare(newname, "&arg4")) {
		newval = rinfo[script]->arg4;
		goto next2;
	}

	if (compare(newname, "&arg5")) {
		newval = rinfo[script]->arg5;
		goto next2;
	}

	if (compare(newname, "&arg6")) {
		newval = rinfo[script]->arg6;
		goto next2;
	}

	if (compare(newname, "&arg7")) {
		newval = rinfo[script]->arg7;
		goto next2;
	}

	if (compare(newname, "&arg8")) {
		newval = rinfo[script]->arg8;
		goto next2;
	}

	if (compare(newname, "&arg9")) {
		newval = rinfo[script]->arg9;
		goto next2;
	}

	newval = atol(newname);

next2:
	if (math_ == '=')
		play.var[i].var = newval;

	if (math_ == '+')
		play.var[i].var += newval;

	if (math_ == '-')
		play.var[i].var -= newval;

	if (math_ == '/')
		play.var[i].var = play.var[i].var / newval;

	if (math_ == '*')
		play.var[i].var = play.var[i].var * newval;

	return newret;
}

static void decipher(char *crap, int script) {
	if (compare(crap, "&current_sprite")) {
		sprintf(crap, "%d", rinfo[script]->sprite);
		return;
	}

	if (compare(crap, "&current_script")) {
		sprintf(crap, "%d", script);
		return;
	}

	// v1.08 special variables.
	if (compare(crap, "&return")) {
		sprintf(crap, "%d", returnint);
		return;
	}

	if (compare(crap, "&arg1")) {
		sprintf(crap, "%d", rinfo[script]->arg1);
		return;
	}

	if (compare(crap, "&arg2")) {
		sprintf(crap, "%d", rinfo[script]->arg2);
		return;
	}

	if (compare(crap, "&arg3")) {
		sprintf(crap, "%d", rinfo[script]->arg3);
		return;
	}

	if (compare(crap, "&arg4")) {
		sprintf(crap, "%d", rinfo[script]->arg4);
		return;
	}

	if (compare(crap, "&arg5")) {
		sprintf(crap, "%d", rinfo[script]->arg5);
		return;
	}

	if (compare(crap, "&arg6")) {
		sprintf(crap, "%d", rinfo[script]->arg6);
		return;
	}

	if (compare(crap, "&arg7")) {
		sprintf(crap, "%d", rinfo[script]->arg7);
		return;
	}

	if (compare(crap, "&arg8")) {
		sprintf(crap, "%d", rinfo[script]->arg8);
		return;
	}

	if (compare(crap, "&arg9")) {
		sprintf(crap, "%d", rinfo[script]->arg9);
		return;
	}

	for (int i = 1; i < max_vars; i++) {
		if (play.var[i].active == true) if (i == get_var(script, play.var[i].name))
			// redink1 changed for recursive scoping
			if (compare(play.var[i].name, crap)) {
				sprintf(crap, "%d", play.var[i].var);
				//        check_for_real_vars(crap, i);
				return;
			}
	}
}

static bool get_parms(char proc_name[20], int script, char *h, const int p[10]) {
	memset(nlist, 0, 10 * sizeof(int));

	char crap[100];

	strip_beginning_spaces(h);
	if (h[0] == '(') {
		h = &h[1];
	} else {
		Msg("Missing ( in %s, offset %d.", rinfo[script]->name, rinfo[script]->current);
		return false;
	}

	for (int i = 0; i < 10; i++) {
		strip_beginning_spaces(h);

		if (p[i] == 1) {
			if (strchr(h, ',') != NULL)
				seperate_string(h, 1, ',', crap);
			else if (strchr(h, ')') != NULL)
				seperate_string(h, 1, ')', crap);

			h = &h[strlen(crap)];

			if (crap[0] == '&') {
				replace(" ", "", crap);
				//  Msg("Found %s, 1st is %c",crap, crap[0]);
				decipher(crap, script);
			}

			nlist[i] = atol(crap);

		} else if (p[i] == 2) {
			// Msg("Checking for string..");
			seperate_string(h, 2, '"', crap);
			h = &h[strlen(crap) + 2];

			//Msg("Found %s",crap);
			strcpy(slist[i], crap);
		}

		if (p[i + 1] == 0) {
			//finish
			strip_beginning_spaces(h);

			if (h[0] == ')') {
				h = &h[1];
			} else {

				Msg("Missing ) in %s, offset %d.", rinfo[script]->name, rinfo[script]->current);
				h = &h[1];

				return false;
			}

			strip_beginning_spaces(h);

			if (h[0] == ';') {
				//  Msg("Found ending ;");
				h = &h[1];

			} else {
				//Msg("Missing ; in %s, offset %d.", rinfo[script]->name, rinfo[script]->current);
				//  h = &h[1];
				return true;
			}

			return true;
		}

		//got a parm, but there is more to get, lets make sure there is a comma there
		strip_beginning_spaces(h);

		if (h[0] == ',') {
			h = &h[1];
		} else {
			Msg("Procedure %s does not take %d parms in %s, offset %d. (%s?)", proc_name, i + 1, rinfo[script]->name, rinfo[script]->current, h);
			return false;
		}
	}

	return true;
}

static int add_callback(const char *name, int n1, int n2, int script) {
	for (int ii = 1; ii < max_callbacks; ii++) {
		if (callback[ii].active == false) {
			memset(&callback[ii], 0, sizeof(callback[ii]));

			callback[ii].active = true;
			callback[ii].min = n1;
			callback[ii].max = n2;
			callback[ii].owner = script;
			strcpy(callback[ii].name, name);

			if (debug_mode)
				Msg("Callback added to %d.", ii);
			return ii;
		}
	}

	Msg("Couldn't add callback, all out of space");
	return 0;
}

static void add_item(char name[10], int mseq, int mframe, bool magic) {
	if (magic == false) {
		//add reg item

		for (int i = 1; i < 17; i++) {
			if (play.item[i].active == false) {
				if (debug_mode)
					Msg("Weapon/item %s added to inventory.", name);
				play.item[i].seq = mseq;
				play.item[i].frame = mframe;
				strcpy(play.item[i].name, name);
				play.item[i].active = true;

				int crap1 = load_script(play.item[i].name, 1000, false);
				if (locate(crap1, "PICKUP")) run_script(crap1);

				return;
			}
		}

	} else {
		//add magic item
		for (int i = 1; i < 9; i++) {
			if (play.mitem[i].active == false) {
				if (debug_mode)
					Msg("Magic %s added to inventory.", name);
				play.mitem[i].seq = mseq;
				play.mitem[i].frame = mframe;
				strcpy(play.mitem[i].name, name);

				play.mitem[i].active = true;

				int crap = load_script(play.mitem[i].name, 1000, false);
				if (locate(crap, "PICKUP"))
					run_script(crap);

				return;
			}
		}
	}
}

static void kill_cur_item() {
	if (*pcur_weapon != 0) {
		if (play.item[*pcur_weapon].active == true) {

			if (weapon_script != 0) if (locate(weapon_script, "DISARM")) run_script(weapon_script);
			weapon_script = load_script(play.item[*pcur_weapon].name, 0, false);
			play.item[*pcur_weapon].active = false;
			*pcur_weapon = 0;
			if (weapon_script != 0) if (locate(weapon_script, "HOLDINGDROP")) run_script(weapon_script);

			if (weapon_script != 0) if (locate(weapon_script, "DROP")) run_script(weapon_script);
			weapon_script = 0;
		} else {
			Msg("Error:  Can't kill cur item, none armed.");
		}
	}
}

static void kill_cur_magic() {
	if (*pcur_magic != 0) {
		if (play.mitem[*pcur_magic].active == true) {

			if (magic_script != 0) if (locate(magic_script, "DISARM")) run_script(magic_script);
			magic_script = load_script(play.mitem[*pcur_magic].name, 0, false);
			play.mitem[*pcur_magic].active = false;
			*pcur_magic = 0;

			if (magic_script != 0) if (locate(magic_script, "HOLDINGDROP")) run_script(magic_script);
			if (magic_script != 0) if (locate(magic_script, "DROP")) run_script(magic_script);
			magic_script = 0;
		} else {
			Msg("Error:  Can't kill cur magic, none armed.");
		}
	}
}

static void kill_cur_item_script(char name[20]) {
	int select = 0;
	for (int i = 1; i < 17; i++) {
		if (play.item[i].active)
			if (compare(play.item[i].name, name)) {
				select = i;
				goto found;
			}
	}
	return;

found:
	if (*pcur_weapon == select) {
		// Holding it right now
		if (locate(weapon_script, "HOLDINGDROP"))
			run_script(weapon_script);
		if (locate(weapon_script, "DISARM"))
			run_script(weapon_script);

		*pcur_weapon = 0;
		weapon_script = 0;
	}

	int script = load_script(play.item[select].name, 0, false);
	play.item[select].active = false;

	if (locate(script, "DROP"))
		run_script(script);

	draw_status_all();
}

static void kill_cur_magic_script(char name[20]) {
	int select = 0;
	for (int i = 1; i < 9; i++) {
		if (play.mitem[i].active)
			if (compare(play.mitem[i].name, name)) {
				select = i;
				goto found;
			}
	}
	return;

found:
	if (*pcur_magic == select) {
		// Holding it right now
		if (locate(magic_script, "HOLDINGDROP"))
			run_script(magic_script);
		if (locate(magic_script, "DISARM"))
			run_script(magic_script);

		*pcur_weapon = 0;
		magic_script = 0;
	}

	int script = load_script(play.mitem[select].name, 0, false);
	play.mitem[select].active = false;

	if (locate(script, "DROP"))
		run_script(script);

	draw_status_all();
}

static int change_sprite_noreturn(int h, int val, int *change) {
	//Msg("Searching sprite %s with val %d.  Cur is %d", h, val, *change);
	if (spr[h].active == false)
		return -1;

	*change = val;
	return *change;
}

static int change_edit(int h, int val, unsigned short *change) {
	//Msg("Searching sprite %s with val %d.  Cur is %d", h, val, *change);

	if (h > 99) return -1;
	if (h < 1) return -1;
	if (val != -1) {
		*change = val;
	}

	return *change;
}

static int change_edit_char(int h, int val, unsigned char *change) {
	//Msg("Searching sprite %s with val %d.  Cur is %d", h, val, *change);
	//  Msg("h is %d..",val);
	if (h > 99) return -1;
	if (h < 1) return -1;
	if (val != -1) {
		*change = val;
	}

	return *change;
}

static bool locate_goto(char proc[50], int script) {
	rinfo[script]->current = 0;
	char line[200];
	char ev[3][100];
	//    char temp[100];
	replace(";", "", proc);
	strchar(proc, ':');
	//  Msg("locate is looking for %s", proc);

	while (read_next_line(script, line)) {
		strip_beginning_spaces(line);

		get_word(line, 1, ev[1]);
		replace("\n", "", ev[1]);

		if (compare(ev[1], proc)) {
			if (debug_mode) Msg("Found goto : Line is %s, word is %s.", line, ev[1]);

			rinfo[script]->skipnext = false;
			rinfo[script]->onlevel = 0;
			rinfo[script]->level = 0;

			return true;
			//this is desired proc
		}

	}
	Msg("ERROR:  Cannot goto %s in %s.", proc, rinfo[script]->name);
	return false;
}

static int does_sprite_have_text(int sprite) {
	//Msg("getting callback # with %d..", sprite);
	for (int k = 1; k <= max_sprites_at_once; k++) {
		if (spr[k].active) if (spr[k].owner == sprite) if (spr[k].brain == 8) {
			//Msg("Found it!  returning %d.", k);

			return k;
		}
	}

	return 0;
}

static int var_exists(char name[20], int scope) {
	for (int i = 1; i < max_vars; i++) {
		if (play.var[i].active) {
			if (compare(play.var[i].name, name)) {

				if (play.var[i].scope == scope) {
					//redink1 changed to check recursively... then changed back.  Hrm.
					//Msg("Found match for %s.", name);
					return i;
				}
			}
		}
	}

	return 0;
}

static void make_int(char name[80], int value, int scope, int script) {
	int dupe;
	if (strlen(name) > 19) {

		Msg("ERROR:  Varname %s is too long in script %s.", name, rinfo[script]->name);
		return;
	}
	dupe = var_exists(name, scope);

	if (dupe > 0) {
		if (scope != 0) {
			Msg("Local var %s already used in this procedure in script %s.", name, rinfo[script]->name);

			play.var[dupe].var = value;

		} else
			Msg("Var %s is already a global, not changing value.", name);

		return;
	}

	// make new var

	for (int i = 1; i < max_vars; i++) {
		if (play.var[i].active == false) {
			play.var[i].active = true;
			play.var[i].scope = scope;
			strcpy(play.var[i].name, name);
			//g("var %s created, used slot %d ", name,i);
			play.var[i].var = value;
			return;
		}
	}

	Msg("ERROR: Out of var space, all %d used.", max_vars);
}

static void int_prepare(char line[100], int script) {
	int def = 0;
	char hold[100];
	strcpy(hold, line);
	char name[100];
	char crap[100];
	replace("=", " ", line);
	strcpy(crap, line);
	seperate_string(crap, 1, ';', line);
	get_word(line, 2, name);

	if (name[0] != '&') {
		Msg("ERROR:  Can't create var %s, should be &%s.", name, name);
		return;
	}

	make_int(name, def, script, script);

	strcpy(line, hold);
}

static void kill_all_vars() {
	FillMemory(&play, sizeof(play), 0);
}

//redink1 added this to make new global functions
static void make_function(char file[10], char func[20]) {
	//See if it already exists
	bool exists = false;
	int i;
	for (i = 0; strlen(play.func[i].func) > 0 && i < 100; i++) {
		if (compare(func, play.func[i].func)) {
			exists = true;
			break;
		}
	}

	if (exists) {
		strncpy(play.func[i].file, file, 10);
	} else {
		strncpy(play.func[0].file, file, 10);
		strncpy(play.func[0].func, func, 20);
	}
}

static int process_line(int script, char *s, bool doelse) {
	char *h, *p;
	int i;
	char line[200];
	char ev[15][100];
	char temp[100];
	char first[2];
	//char rstring[200];
	//int rint;
	int sprite = 0;

	if (rinfo[script]->level < 1) rinfo[script]->level = 1;


	for (int kk = 1; kk < 15; kk++) ev[kk][0] = 0;
	//refinfo save_rinfo;
	h = s;
	if (h[0] == 0) return 0;
	if ((h[0] == '/') && (h[1] == '/'))

	{
		//Msg("It was a comment!");
		goto bad;
	}



	for (i = 1; i <= 14; i++) {
		if (seperate_string(h, i, ' ', ev[i]) == false) goto pass;
	}

pass:
	//Msg("first line is %s (second is %s)", ev[1], ev[2]);
	if (compare(ev[1], "VOID")) {

		if (rinfo[script]->proc_return != 0) {
			run_script(rinfo[script]->proc_return);
			kill_script(script);
		}

		//Msg("returning..");
		return 2;
	}
	//replace("\n","",ev[1]);
	if (ev[1][strlen(ev[1]) - 1] == ':') if (strlen(ev[2]) < 2) if (strncmp(ev[1], "say", 3) != 0)

	{
		//  Msg("Found label %s..",ev[1]);
		return 0; //its a label
	}
	if (ev[1][0] == '(') {
		//this procedure has been passed a conditional statement finder
		//what kind of conditional statement is it?
		p = h;
		seperate_string(h, 2, ')', temp);
		//Msg("We found %s, woah!", temp);
		seperate_string(h, 1, ')', ev[1]);

		// Msg("Ok, turned h %s to  ev1 %s.",h,ev[1]);
		p = &p[strlen(ev[1]) + 1];

		strip_beginning_spaces(p);
		//  Msg("does %s have a ( in front?", p);
		//Msg("We found %s, woah!", temp);

		//These are used for conditionals??
		if (strchr(temp, '=') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d == %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}

		if (strchr(temp, '>') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d > %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}

		if (strchr(temp, '<') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d < %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}
		if (strchr(temp, '<=') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d <= %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}
		if (strchr(temp, '>=') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d >= %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}

		if (strchr(temp, '!=') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d != %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}


		if (p[0] == ')') {
			//its a procedure in the if statement!!!
			h = &h[1];
			p = &p[1];
			strcpy(line, p);
			process_line(script, h, false);

			//8
			Msg("Returned %d for the returnint", returnint);
			h = s;
			strcpy(s, line);

			//  Msg("Returing %s..", s);
			return 0;
		} else {
			h = &h[1];

			seperate_string(h, 1, ')', line);
			h = &h[strlen(line) + 1];
			returnint = var_figure(line, script);

			strcpy(s, h);

			return 0;
		}

		strip_beginning_spaces(h);
		strip_beginning_spaces(ev[1]);

		s = h;


	} else {



	}


	if (strchr(ev[1], '(') != NULL) {
		//Msg("Has a (, lets change it");
		seperate_string(h, 1, '(', ev[1]);
		//Msg("Ok, first is now %s",ev[1]);


	}

	sprintf(first, "%c", ev[1][0]);



	if (compare(first, "{")) {


		rinfo[script]->level++;
		//Msg("Went up level, now at %d.", rinfo[script]->level);
		h = &h[1];
		if (rinfo[script]->skipnext) {
			rinfo[script]->skipnext = false;
			rinfo[script]->onlevel = (rinfo[script]->level - 1);
			//Msg("Skipping until level %d is met..", rinfo[script]->onlevel);

		}
		goto good;
	}

	if (compare(first, "}")) {
		rinfo[script]->level--;
		//Msg("Went down a level, now at %d.", rinfo[script]->level);
		h = &h[1];

		if (rinfo[script]->onlevel > 0) if (rinfo[script]->level == rinfo[script]->onlevel) {
			strip_beginning_spaces(h);

			strcpy(s, h);
			return 4;
		}
		goto good;
	}




	if (rinfo[script]->level < 0) {
		rinfo[script]->level = 0;
	}

	if (compare(ev[1], "void")) {
		//     Msg("Next procedure starting, lets quit");
		strcpy(s, h);
		if (rinfo[script]->proc_return != 0) {
			run_script(rinfo[script]->proc_return);
			kill_script(script);
		}

		return 2;
	}


	{
		//used to be an if..


		if (rinfo[script]->onlevel > 0) {
			if (rinfo[script]->level > rinfo[script]->onlevel) return 0;

		}
		rinfo[script]->onlevel = 0;

		if (rinfo[script]->skipnext) {
			//sorry, can't do it, you were told to skip the next thing
			rinfo[script]->skipnext = false;
			strcpy(s, h);
			return 3;
		}

		//if (debug_mode) Msg("%s",s);


		if (compare(ev[1], "void")) {
			Msg("ERROR: Missing } in %s, offset %d.", rinfo[script]->name, rinfo[script]->current);
			strcpy(s, h);
			return 2;
		}

		if (compare(ev[1], "else")) {
			//Msg("Found else!");
			h = &h[strlen(ev[1])];


			if (doelse) {
				//Msg("Yes to else...");



			} else {
				//they shouldn't run the next thing
				rinfo[script]->skipnext = true;
				//Msg("No to else...");

			}
			strcpy(s, h);
			return 1;

		}

		if (compare(ev[1], "show_console")) {
			h = &h[strlen(ev[1])];
			mConsoleActive = true;
			/*int ARR[20] = {1,0,0,0,0,0,0,0,0,0};
			if (get_parms(ev[1], script, h, ARR))
			{
				if ( nlist[0] >= 1 )
				{
					mConsoleActive = true;
				}
				else
				{
					mConsoleActive = false;
				}
			}
			returnint = 0;
			if ( mConsoleActive )
			{
				returnint = 1;
			}*/
		}

		//redink1 - sets font color
		if (compare(ev[1], "set_font_color")) {
			h = &h[strlen(ev[1])];
			const int ARR[20] = { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] >= 1 && nlist[0] <= 15 &&
					nlist[1] >= 0 && nlist[1] <= 255 &&
					nlist[2] >= 0 && nlist[2] <= 255 &&
					nlist[3] >= 0 && nlist[3] <= 255) {
					font_colors[nlist[0]].red = nlist[1];
					font_colors[nlist[0]].green = nlist[2];
					font_colors[nlist[0]].blue = nlist[3];
				}
			}

			strcpy(s, h);
			return 0;
		}

		//redink1 - clears the editor information, useful for save games and such
		if (compare(ev[1], "clear_editor_info")) {
			h = &h[strlen(ev[1])];
			for (int ii = 0; ii < 769; ii++) {
				for (int j = 0; j < 100; j++) {
					play.spmap[ii].seq[j] = 0;
					play.spmap[ii].frame[j] = 0;
					play.spmap[ii].type[j] = 0;
					play.spmap[ii].last_time = 0;
				}
			}
			returnint = 1;
			return 0;
		}

		//redink1 - returns the number of variables used
		if (compare(ev[1], "var_used")) {
			h = &h[strlen(ev[1])];
			int m = 0;
			for (int ii = 1; ii < max_vars; ii++)
				if (play.var[ii].active == true)
					m++;
			returnint = m;
			return 0;
		}

		//redink1 added this function to load a new map/dink.dat
		if (compare(ev[1], "load_map")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				strcpy(current_map, slist[0]);
				strcpy(current_dat, slist[1]);
				map_info::load();
			}

			strcpy(s, h);
			return 0;
		}


		//redink1 added this function to load a pallete from any bmp
		if (compare(ev[1], "load_palette")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (File::exists(slist[0])) {
					LPDIRECTDRAWSURFACE oldTrick = DDLoadBitmap(lpDD, slist[0], 0, 0);
					lpDDPal = DDLoadPalette(lpDD, slist[0]);
					if (lpDDPal) {
						lpDDSPrimary->SetPalette(lpDDPal);
						lpDDPal->GetEntries(0, 0, 256, pe);
						lpDDPal->GetEntries(0, 0, 256, real_pal);
						//Store in save game
						strncpy(play.palette, slist[0], 50);
					}
					oldTrick->Release();
				}
			}

			strcpy(s, h);
			return 0;
		}

		//redink1 added this function to load new tiles, because he is a l33t guy
		if (compare(ev[1], "load_tile")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (!File::exists(slist[0])) {
					sprintf(slist[0], "..\\DINK\\%s", slist[0]);
				}

				if (File::exists(slist[0]) && nlist[1] > 0 && nlist[1] < tile_screens) {
					//Need to unload old tiles...
					tiles[nlist[1]]->Release();
					//Load in the new tiles...
					tiles[nlist[1]] = DDTileLoad(lpDD, slist[0], 0, 0, nlist[1]);
					DDSetColorKey(tiles[nlist[1]], RGB(0, 0, 0));
					//Store in save game
					strncpy(play.tile[nlist[1]].file, slist[0], 50);
				}
			}

			strcpy(s, h);
			return 0;
		}

		//redink1 added this function to change the save game 'info'
		if (compare(ev[1], "set_save_game_info")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				strcpy(save_game_info, slist[0]);
			}
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function to show the item screen
		if (compare(ev[1], "show_inventory")) {
			h = &h[strlen(ev[1])];
			item_screen = true;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function,, and took it away.
		/*if (compare(ev[1], "get_compatibility"))
		{
		returnint = 0;

		h = &h[strlen(ev[1])];
		int ARR[20] = {2,1,0,0,0,0,0,0,0,0};
		if (get_parms(ev[1], script, h, ARR))
		{
		if (compare(slist[0],"get_compatibility"))
		{
		if (nlist[1] <= 1)
		{
		returnint = 1;
		}
		}
		}
		strcpy(s, h);
		return 0;
		}*/

		//redink1 added this function
		if (compare(ev[1], "get_time_game")) {
			h = &h[strlen(ev[1])];
			returnint = play.minutes +
				(g_events->getTotalSeconds() / 60);
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "get_time_real")) {
			h = &h[strlen(ev[1])];
			TimeDate td;
			g_system->getTimeAndDate(td);

			returnint = td.tm_hour * 60 + td.tm_min;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "get_date_year")) {
			h = &h[strlen(ev[1])];
			TimeDate td;
			g_system->getTimeAndDate(td);
			returnint = td.tm_year;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "get_date_month")) {
			h = &h[strlen(ev[1])];
			TimeDate td;
			g_system->getTimeAndDate(td);
			returnint = td.tm_mon;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "get_date_day")) {
			h = &h[strlen(ev[1])];
			TimeDate td;
			g_system->getTimeAndDate(td);
			returnint = td.tm_mday;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "math_abs")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = abs(nlist[0]);
			}
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		/*if (compare(ev[1], "math_sin"))
		{
			h = &h[strlen(ev[1])];
			int ARR[20] = {1,0,0,0,0,0,0,0,0,0};
			if (get_parms(ev[1], script, h, ARR))
			{
				returnint = sin((double)nlist[0]);
			}
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "math_cos"))
		{
			h = &h[strlen(ev[1])];
			int ARR[20] = {1,0,0,0,0,0,0,0,0,0};
			if (get_parms(ev[1], script, h, ARR))
			{
				returnint = cos((double)nlist[0]);
			}
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "math_tan"))
		{
			h = &h[strlen(ev[1])];
			int ARR[20] = {1,0,0,0,0,0,0,0,0,0};
			if (get_parms(ev[1], script, h, ARR))
			{
				returnint = tan((double)nlist[0]);
			}
			strcpy(s, h);
			return 0;
		}*/

		//redink1 added this function
		if (compare(ev[1], "math_sqrt")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = sqrt((double)abs(nlist[0]));
			}
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "math_mod")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = (nlist[0] % nlist[1]);
			}
			strcpy(s, h);
			return 0;
		}

		//redink1
		if (compare(ev[1], "sp_custom")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR) && spr[nlist[1]].active == true) {
				if (nlist[1] < 1 || spr[nlist[1]].active == false) {
					returnint = -1;
				} else {
					// If key doesn't exist, create it.
					if (spr[nlist[1]].custom->find(slist[0]) == spr[nlist[1]].custom->end()) {
						(*spr[nlist[1]].custom)[slist[0]] = 0;
					}

					// Set the value
					if (nlist[2] != -1) {
						spr[nlist[1]].custom->erase(slist[0]);
						(*spr[nlist[1]].custom)[slist[0]] = nlist[2];
					}

					returnint = spr[nlist[1]].custom->find(slist[0])->_value;
				}
				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1
		if (compare(ev[1], "sp_blood_seq")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].bloodseq);

				returnint = spr[nlist[0]].bloodseq;

				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1
		if (compare(ev[1], "sp_blood_num")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].bloodnum);

				returnint = spr[nlist[0]].bloodseq;

				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1 added to get index of specified item
		if (compare(ev[1], "get_item")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				for (i = 1; i < 17; i++) {
					if (play.item[i].active) {
						if (compare(play.item[i].name, slist[0])) {
							returnint = i;
							break;
						}
					}
				}
				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1 added to get index of specified magic spell
		if (compare(ev[1], "get_magic")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				for (i = 1; i < 9; i++) {
					if (play.mitem[i].active) {
						if (compare(play.mitem[i].name, slist[0])) {
							returnint = i;
							break;
						}
					}
				}
				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1 clip stuff
		if (compare(ev[1], "sp_clip_left")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].alt.left);

				returnint = spr[nlist[0]].alt.left;

				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1 clip stuff
		if (compare(ev[1], "sp_clip_top")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].alt.top);

				returnint = spr[nlist[0]].alt.top;

				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1 clip stuff
		if (compare(ev[1], "sp_clip_right")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].alt.right);

				returnint = spr[nlist[0]].alt.right;

				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1 clip stuff
		if (compare(ev[1], "sp_clip_bottom")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].alt.bottom);

				returnint = spr[nlist[0]].alt.bottom;

				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1 added so users can check truecolor or not
		if (compare(ev[1], "get_truecolor")) {
			h = &h[strlen(ev[1])];
			returnint = truecolor;
			strcpy(s, h);
			return 0;
		}

		//redink1 added so developers can change or see what tile is at any given position
		if (compare(ev[1], "map_tile")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				//Yeah... they can only modify valid tiles
				if (nlist[0] > 0 && nlist[0] <= 96) {
					//Only change the value if it is greater than 0...
					if (nlist[1] > 0) {
						pam.t[nlist[0] - 1].num = nlist[1];
					}
					returnint = pam.t[nlist[0] - 1].num;
					return 0;
				}
			}
			returnint = -1;
			return 0;
		}

		//redink1 added so a developer can retrieve/modify a hard tile
		if (compare(ev[1], "map_hard_tile")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				//Yeah... they can only modify valid tiles
				if (nlist[0] > 0 && nlist[0] <= 96) {
					//Only change the value if it is greater than 0...
					if (nlist[1] > 0) {
						pam.t[nlist[0] - 1].althard = nlist[1];
					}
					returnint = pam.t[nlist[0] - 1].althard;
					return 0;
				}
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "unfreeze")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				//Msg("UnFreeze called for %d.", nlist[0]);
				if (spr[nlist[0]].active) spr[nlist[0]].freeze = 0;
				else
					Msg("Couldn't unfreeze sprite %d in script %d, it doesn't exist.", nlist[0], script);

			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "freeze")) {
			//Msg("Freeze called (%s)", h);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				if (spr[nlist[0]].active) spr[nlist[0]].freeze = script;
				else
					Msg("Couldn't freeze sprite %d in script %d, it doesn't exist.", nlist[0], script);

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "set_callback_random")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				int cb = add_callback(slist[0], nlist[1], nlist[2], script);
				//got all parms, let do it
				returnint = cb;
			}

			strcpy(s, h);
			return 0;
		}

		// redink1 added
		if (compare(ev[1], "callback_kill")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] >= 0 && nlist[0] <= 99) {
					callback[nlist[0]].active = false;
				}
			}
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "set_dink_speed")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR) && nlist[0] != 0) {
				dinkspeed = nlist[0];
			}

			strcpy(s, h);
			return 0;
		}

		//redink1
		if (compare(ev[1], "set_dink_base_push")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				mDinkBasePush = nlist[0];
			}
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "reset_timer")) {
#ifdef TODO
			h = &h[strlen(ev[1])];
			time(&time_start);
			play.minutes = 0;
			strcpy(s, h);
			return 0;
#else
			error("TODO: reset_timer");
#endif
		}


		if (compare(ev[1], "set_keep_mouse")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				keep_mouse = nlist[0];

			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "add_item")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				add_item(slist[0], nlist[1], nlist[2], false);
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "add_exp")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				add_exp(nlist[0], nlist[1], true);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "add_magic")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				add_item(slist[0], nlist[1], nlist[2], true);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "kill_this_item")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				kill_cur_item_script(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "kill_this_magic")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				kill_cur_magic_script(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}



		if (compare(ev[1], "show_bmp")) {
			Msg("showing BMP");
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				wait.active = false;
				show_bmp(slist[0], nlist[1], nlist[2], script);
			}

			strcpy(s, h);
			return 2;
		}


		if (compare(ev[1], "wait_for_button")) {
			Msg("waiting for button with script %d", script);
			h = &h[strlen(ev[1])];
			strcpy(s, h);
			wait.script = script;
			wait.active = true;
			wait.button = 0;
			return 2;
		}

		if (compare(ev[1], "stop_wait_for_button")) {
			wait.active = false;

			return 0;
		}

		if (compare(ev[1], "copy_bmp_to_screen")) {
			Msg("copying BMP");
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				copy_bmp(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "say")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				if (nlist[1] == 0) {
					Msg("Say_stop error:  Sprite 0 can talk? Yeah, didn't think so.");
					return 0;
				}

				if (nlist[1] != 1000)
					kill_text_owned_by(nlist[1]);
				decipher_string(slist[0], script);
				returnint = say_text(slist[0], nlist[1], script);
				//Msg("Just said %s.", slist[0]);
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "draw_screen")) {

			if (rinfo[script]->sprite == 1000) {
				draw_map_game();
				return 0;
			}
			draw_map_game();
			return 2;
		}


		if (compare(ev[1], "free_items")) {
			returnint = 0;
			for (i = 1; i < 17; i++) {
				if (play.item[i].active == false) {
					returnint += 1;
				}
			}
			return 0;
		}


		if (compare(ev[1], "kill_cur_item")) {
			returnint = 0;
			kill_cur_item();
			return 2;
		}



		if (compare(ev[1], "kill_cur_magic")) {
			returnint = 0;
			kill_cur_magic();
			return 2;
		}



		if (compare(ev[1], "free_magic")) {
			returnint = 0;

			for (i = 1; i < 9; i++) {
				if (play.mitem[i].active == false) {
					returnint += 1;
				}
			}
			return 0;
		}




		if (compare(ev[1], "draw_status")) {
			draw_status_all();
			return 0;
		}


		if (compare(ev[1], "arm_weapon")) {

			if (weapon_script != 0) if (locate(weapon_script, "DISARM")) run_script(weapon_script);
			weapon_script = load_script(play.item[*pcur_weapon].name, 1000, false);
			if (locate(weapon_script, "ARM")) run_script(weapon_script);


			return 0;
		}

		if (compare(ev[1], "arm_magic")) {


			if (magic_script != 0) if (locate(magic_script, "DISARM")) run_script(magic_script);
			magic_script = load_script(play.mitem[*pcur_magic].name, 1000, false);
			if (locate(magic_script, "ARM")) run_script(magic_script);

			return 0;
		}


		if (compare(ev[1], "load_screen")) {
			//Msg("Loading map %d..",*pmap);
			update_screen_time();
			load_map(map.loc[*pmap]);

			//redink1 fix for correct indicator on mini-map
			if (map.indoor[*pmap] == 0)
				play.last_map = *pmap;
			return 0;
		}


		if (compare(ev[1], "choice_start")) {

			kill_text_owned_by(1);
			if (talk_get(script)) {

				//      Msg("Question gathered successfully.");
				return 2;
			}

			return 0;
		}


		if (compare(ev[1], "say_stop")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[1] == 0) {
					Msg("Say_stop error:  Sprite 0 can talk? Yeah, didn't think so.");
					return 0;
				}

				kill_text_owned_by(nlist[1]);
				kill_text_owned_by(1);
				kill_returning_stuff(script);

				decipher_string(slist[0], script);
				sprite = say_text(slist[0], nlist[1], script);
				returnint = sprite;
				spr[sprite].callback = script;
				play.last_talk = script;
				//Msg("Sprite %d marked callback true.", sprite);

				strcpy(s, h);

				return 2;

			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "say_stop_npc")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				if (text_owned_by(nlist[1])) {
					returnint = 0;
					return 0;
				}

				kill_returning_stuff(script);
				decipher_string(slist[0], script);
				sprite = say_text(slist[0], nlist[1], script);
				returnint = sprite;
				spr[sprite].callback = script;
				strcpy(s, h);

				return 2;

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "say_stop_xy")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				Msg("Say_stop_xy: Adding %s", slist[0]);
				kill_returning_stuff(script);
				decipher_string(slist[0], script);
				sprite = say_text_xy(slist[0], nlist[1], nlist[2], script);
				spr[sprite].callback = script;
				spr[sprite].live = true;
				play.last_talk = script;
				strcpy(s, h);

				return 2;

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "say_xy")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				kill_returning_stuff(script);
				decipher_string(slist[0], script);
				sprite = say_text_xy(slist[0], nlist[1], nlist[2], script);
				returnint = sprite;
				strcpy(s, h);
				return 0;

			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "restart_game")) {
			while (kill_last_sprite());
			kill_repeat_sounds_all();
			kill_all_scripts_for_real();
			mode = 0;
			screenlock = 0;
			kill_all_vars();
			FillMemory(&hm, sizeof(hm), 0);
			for (int u = 1; u <= 10; u++)
				play.button[u] = u;
			int scriptMain = load_script("main", 0, true);

			locate(scriptMain, "main");
			run_script(scriptMain);
			//lets attach our vars to the scripts
			attach();
			return 2;
		}

		if (compare(ev[1], "wait")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				//           Msg("Wait called for %d.", nlist[0]);
				strcpy(s, h);
				kill_returning_stuff(script);
				add_callback("", nlist[0], 0, script);

				return 2;
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "preload_seq")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				check_seq_status(nlist[0]);
			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "script_attach")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				rinfo[script]->sprite = nlist[0];
			}
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "draw_hard_sprite")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				update_play_changes();
				int l = nlist[0];
				RECT mhard;
				CopyRect(&mhard, &picInfo[seq[spr[l].pseq].frame[spr[l].pframe]].hardbox);
				OffsetRect(&mhard, (spr[l].x - 20), spr[l].y);

				fill_hardxy(mhard);
				fill_back_sprites();
				fill_hard_sprites();


			}
			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "move")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				spr[nlist[0]].move_active = true;
				spr[nlist[0]].move_dir = nlist[1];
				spr[nlist[0]].move_num = nlist[2];
				spr[nlist[0]].move_nohard = nlist[3];
				spr[nlist[0]].move_script = 0;
				if (debug_mode) Msg("Moving: Sprite %d, dir %d, num %d", nlist[0], nlist[1], nlist[2]);


			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "sp_script")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] == 0) {
					Msg("Error: sp_script cannot process sprite 0??");
					return 0;
				}
				kill_scripts_owned_by(nlist[0]);
				if (load_script(slist[1], nlist[0], true) == 0) {
					returnint = 0;
					return 0;
				}
				if (no_running_main == true) Msg("Not running %s until later..", rinfo[spr[nlist[0]].script]->name);

				if (no_running_main == false)
					locate(spr[nlist[0]].script, "MAIN");


				int tempreturn = spr[nlist[0]].script;

				if (no_running_main == false)
					run_script(spr[nlist[0]].script);


				returnint = tempreturn;
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "spawn")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				int mysc = load_script(slist[0], 1000, true);
				if (mysc == 0) {
					returnint = 0;
					return 0;
				}
				locate(mysc, "MAIN");
				int tempreturn = mysc;
				run_script(mysc);
				returnint = tempreturn;
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "run_script_by_number")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 2, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (locate(nlist[0], slist[1])) {
					run_script(nlist[0]);
				}

			}

			strcpy(s, h);
			return 0;
		}



		if (compare(ev[1], "draw_hard_map")) {
			// (sprite, direction, until, nohard);
			Msg("Drawing hard map..");
			update_play_changes();
			fill_whole_hard();
			fill_hard_sprites();
			fill_back_sprites();
			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "draw_background")) {
			// (sprite, direction, until, nohard);
			draw_map_game_background();
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "fade_down")) {
			// (sprite, direction, until, nohard);
			process_downcycle = true;
			cycle_clock = thisTickCount + 1000;
			cycle_script = script;

			strcpy(s, h);

			return 2;
		}

		if (compare(ev[1], "fade_up")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			process_upcycle = true;
			cycle_script = script;

			strcpy(s, h);
			return 2;
		}

		if (compare(ev[1], "kill_this_task")) {
			// (sprite, direction, until, nohard);
			if (rinfo[script]->proc_return != 0) {
				run_script(rinfo[script]->proc_return);
			}
			kill_script(script);
			return 2;
		}

		if (compare(ev[1], "kill_game")) {
			// (sprite, direction, until, nohard);

			Msg("Was told to kill game, so doing it like a good boy.");
			//  PostMessage(hWndMain, WM_CLOSE, 0, 0);
			finiObjects();
			return 2;
		}


		/*if (compare(ev[1], "playavi"))
		{
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {2,0,0,0,0,0,0,0,0,0};
			if (get_parms(ev[1], script, h, ARR))
			{
				//DSDisable();


				//ShowWindow(hWndMain, SW_MINIMIZE);

				int myreturn =  _spawnl(_P_WAIT, "..\\dplay.exe","..\\dplay.exe",slist[0], NULL );

				Msg("Return is %d", myreturn);
				//ShowWindow(hWndMain, SW_MAXIMIZE);

				//SetFocus(hWndMain);
			}
			//InitSound(hWndMain);
			strcpy(s, h);
			return 2;

		}*/

		//redink1 added
		if (compare(ev[1], "loopmidi")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] > 0) {
					mLoopMidi = true;
				} else {
					mLoopMidi = false;
				}
			}
		}

		if (compare(ev[1], "playmidi")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				//StopMidi();
				int regm = atol(slist[0]);
				Msg("Processing playmidi command.");
				if (regm > 1000) {
					Msg("playmidi - cd play command detected.");

					//cd directive

					if (cd_inserted) {
						if (regm == last_cd)
							if (cdplaying())
								return 2;
						Msg("Playing CD track %d.", regm - 1000);

						PlayCD(regm - 1000);
						strcpy(s, h);
						return 0;
					} else {
						char hold[255];
						//cd isn't instered, can't play CD song!!!
						sprintf(hold, "%d.mid", regm - 1000);
						PlayMidi(hold);
					}
				}

				PlayMidi(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}
		if (compare(ev[1], "stopmidi")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			StopMidi();
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "kill_all_sounds")) {
			kill_repeat_sounds_all();
			strcpy(s, h);
			return 0;

		}

		if (compare(ev[1], "turn_midi_off")) {
			midi_active = false;
			strcpy(s, h);
			return 0;

		}
		if (compare(ev[1], "turn_midi_on")) {
			midi_active = true;
			strcpy(s, h);
			return 0;

		}


		if (compare(ev[1], "Playsound")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on)
					returnint = SoundPlayEffect(nlist[0], nlist[1], nlist[2], nlist[3], nlist[4]);
				else returnint = 0;

			} else
				returnint = 0;

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "sound_set_survive")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on) {
					//let's set one sound to survive
					if (nlist[0] > 0)
						soundinfo[nlist[0]].survive = nlist[1];
				}
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "sound_set_vol")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on) {
					//let's set one sound to survive
					if (nlist[0] > 0) {
						soundinfo[nlist[0]].vol = nlist[1];

						soundbank[nlist[0]]->SetVolume(nlist[1]);
					}
				}
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "sound_set_kill")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on) {
					//let's set one sound to survive
					if (nlist[0] > 0)

						soundbank[nlist[0]]->Stop();
				}
			}

			strcpy(s, h);
			return 0;
		}



		if (compare(ev[1], "save_game")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				save_game(nlist[0]);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "force_vision")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				*pvision = nlist[0];
				rinfo[script]->sprite = 1000;
				fill_whole_hard();

				draw_map_game();

			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "fill_screen")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				fill_screen(nlist[0]);

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "load_game")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				kill_all_scripts_for_real();
				returnint = load_game(nlist[0]);
				Msg("load completed. ");
				if (rinfo[script] == NULL) Msg("Script %d is suddenly null!", script);


				*pupdate_status = 1;
				draw_status_all();
				return 2;
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "game_exist")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				sprintf(temp, "save%d.dat", nlist[0]);
				if (File::exists(temp)) returnint = 1;
				else returnint = 0;

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "move_stop")) {
			// (sprite, direction, until, nohard);

			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				//Msg("Move stop running %d to %d..", nlist[0], nlist[0]);
				spr[nlist[0]].move_active = true;
				spr[nlist[0]].move_dir = nlist[1];
				spr[nlist[0]].move_num = nlist[2];
				spr[nlist[0]].move_nohard = nlist[3];
				spr[nlist[0]].move_script = script;
				strcpy(s, h);
				if (debug_mode) Msg("Move_stop: Sprite %d, dir %d, num %d", nlist[0], nlist[1], nlist[2]);
				return 2;

			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "load_sound")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on) {
					Msg("getting %s..", slist[0]);
					CreateBufferFromWaveFile(slist[0], nlist[1]);
				}
			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "debug")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				decipher_string(slist[0], script);
				Msg(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "goto")) {

			locate_goto(ev[2], script);
			return 0;
		}

		//redink1 added for global functions
		if (compare(ev[1], "make_global_function")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				make_function(slist[0], slist[1]);
				//Msg(slist[0]);
			}
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "make_global_int")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				make_int(slist[0], nlist[1], 0, script);
				//Msg(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "int")) {

			int_prepare(h, script);

			//Msg(slist[0]);

			h = &h[strlen(ev[1])];

			//Msg("Int is studying %s..", h);
			if (strchr(h, '=') != NULL) {
				strip_beginning_spaces(h);
				//Msg("Found =...continuing equation");
				strcpy(s, h);
				return 4;
			}

			return 0;

		}



		if (compare(ev[1], "busy")) {

			h = &h[strlen(ev[1])];
			// Msg("Running busy, h is %s", h);
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] == 0) Msg("ERROR:  Busy cannot get info on sprite 0 in %s.", rinfo[script]->name);
				else {
					returnint = does_sprite_have_text(nlist[0]);

					Msg("Busy: Return int is %d and %d.  Nlist got %d.", returnint, does_sprite_have_text(nlist[0]), nlist[0]);

				}

			} else Msg("Failed getting parms for Busy()");

			strcpy(s, h);
			return 0;
		}

		//redink1 added
		if (compare(ev[1], "sp_freeze")) {
			h = &h[strlen(ev[1])];
			// Msg("Running busy, h is %s", h);
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				// Set the value
				if (nlist[1] == 0) {
					spr[nlist[0]].freeze = 0;
				} else if (nlist[1] == 1) {
					spr[nlist[0]].freeze = script;
				}

				// Return the value
				if (spr[nlist[0]].freeze > 0) {
					returnint = 1;
				} else {
					returnint = 0;
				}
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "inside_box")) {

			h = &h[strlen(ev[1])];
			Msg("Running pigs with h", h);
			int ARR[20] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				RECT myrect;
				SetRect(&myrect, nlist[2], nlist[3], nlist[4], nlist[5]);
				returnint = inside_box(nlist[0], nlist[1], myrect);

				if (debug_mode)
					Msg("Inbox is int is %d and %d.  Nlist got %d.", returnint, nlist[0], nlist[1]);



			} else Msg("Failed getting parms for inside_box");

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "random")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = RANDOM(nlist[1], nlist[0]);
			} else Msg("Failed getting parms for Random()");

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "initfont")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				initfonts(slist[0]);
				Msg("Initted font %s", slist[0]);
			} else Msg("Failed getting parms for Initfont()");

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "get_version")) {
			h = &h[strlen(ev[1])];
			returnint = dversion;
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "get_burn")) {
			h = &h[strlen(ev[1])];
			returnint = burn_revision;
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "set_mode")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				mode = nlist[0];
				returnint = mode;
			} else Msg("Failed to set mode");

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "kill_shadow")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				for (int jj = 1; jj <= last_sprite_created; jj++) {
					if (spr[jj].brain == 15) if (spr[jj].brain_parm == nlist[0]) {

						spr[jj].active = 0;
					}


				}
			}

			strcpy(s, h);
			return 0;
		}



		if (compare(ev[1], "create_sprite")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				returnint = add_sprite_dumb(nlist[0], nlist[1], nlist[2],
					nlist[3], nlist[4],
					100);

				return 0;
			}
			returnint = 0;
			return 0;
		}



		if (compare(ev[1], "sp")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				for (int ii = 1; ii <= last_sprite_created; ii++) {

					if (spr[ii].sp_index == nlist[0]) {

						if (debug_mode) Msg("Sp returned %d.", ii);
						returnint = ii;
						return 0;
					}

				}
				if (last_sprite_created == 1) {
					Msg("warning - you can't call SP() from a screen-ref, no sprites have been created yet.");
				}

			}
			returnint = 0;
			return 0;
		}


		if (compare(ev[1], "is_script_attached")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {


				returnint = spr[nlist[0]].script;

			}
			return 0;
		}



		if (compare(ev[1], "sp_speed")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].speed);

				if (nlist[1] != -1) changedir(spr[nlist[0]].dir, nlist[0], spr[nlist[0]].base_walk);

				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_range")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].range);

				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_nocontrol")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].nocontrol);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_nodraw")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].nodraw);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_picfreeze")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].picfreeze);
				return 0;
			}
			returnint = -1;
			return 0;
		}



		if (compare(ev[1], "get_sprite_with_this_brain")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				for (i = 1; i <= last_sprite_created; i++) {
					if ((spr[i].brain == nlist[0]) && (i != nlist[1])) if
						(spr[i].active == 1) {
						Msg("Ok, sprite with brain %d is %d", nlist[0], i);
						returnint = i;
						return 0;
					}

				}
			}
			Msg("Ok, sprite with brain %d is 0", nlist[0], i);

			returnint = 0;
			return 0;
		}

		//redink1 added this to make Paul Pliska's life more fulfilling
		if (compare(ev[1], "get_next_sprite_with_this_brain")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				for (i = nlist[2]; i <= last_sprite_created; i++) {
					if ((spr[i].brain == nlist[0]) && (i != nlist[1])) if
						(spr[i].active == 1) {
						Msg("Ok, sprite with brain %d is %d", nlist[0], i);
						returnint = i;
						return 0;
					}

				}
			}
			Msg("Ok, sprite with brain %d is 0", nlist[0], i);

			returnint = 0;
			return 0;
		}


		if (compare(ev[1], "get_rand_sprite_with_this_brain")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				int cter = 0;
				for (i = 1; i <= last_sprite_created; i++) {
					if ((spr[i].brain == nlist[0]) && (i != nlist[1])) if
						(spr[i].active == 1) {
						cter++;

					}

				}

				if (cter == 0) {
					Msg("Get rand brain can't find any brains with %d.", nlist[0]);
					returnint = 0;
					return 0;
				}

				int mypick = RANDOM(1, cter);
				cter = 0;
				for (int ii = 1; ii <= last_sprite_created; ii++) {
					if ((spr[ii].brain == nlist[0]) && (ii != nlist[1])) if
						(spr[ii].active == 1) {
						cter++;
						if (cter == mypick) {
							returnint = ii;
							return 0;
						}
					}

				}


			}


			returnint = 0;
			return 0;
		}



		if (compare(ev[1], "sp_sound")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].sound);

				if (nlist[1] > 0) {
					SoundPlayEffect(spr[nlist[0]].sound, 22050, 0, nlist[0], 1);

				}
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_attack_wait")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				returnint = change_sprite(nlist[0], nlist[1] + thisTickCount, &spr[nlist[0]].attack_wait);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_active")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].active);


				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_disabled")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].disabled);


				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_size")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].size);
				return 0;
			}
			returnint = -1;
			return 0;
		}
		if (compare(ev[1], "activate_bow")) {

			spr[1].seq = 0;
			spr[1].pseq = 100 + spr[1].dir;
			spr[1].pframe = 1;
			bow.active = true;
			bow.script = script;
			bow.hitme = false;
			bow.time = 0;

			/*  bowsound->Release();

			lpDS->DuplicateSoundBuffer(ssound[42].sound,&bowsound);
			bowsound->Play(0, 0, DSBPLAY_LOOPING);
			*/

			return 2;
		}

		if (compare(ev[1], "get_last_bow_power")) {


			returnint = bow.last_power;
			return 0;
		}


		if (compare(ev[1], "sp_que")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].que);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_gold")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].gold);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_base_walk")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_walk);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_target")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].target);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "stopcd")) {
			mciSendCommand(CD_ID, MCI_CLOSE, 0, NULL);
			Msg("Stopped cd");
			killCD(1);
			return 0;
		}


		if (compare(ev[1], "sp_base_hit")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_hit);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_base_attack")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_attack);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_base_idle")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_idle);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if ((compare(ev[1], "sp_base_die")) || (compare(ev[1], "sp_base_death"))) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_die);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "disable_all_sprites")) {
			for (int jj = 1; jj < last_sprite_created; jj++)
				if (spr[jj].active) spr[jj].disabled = true;
			return 0;
		}
		if (compare(ev[1], "enable_all_sprites")) {
			for (int jj = 1; jj < last_sprite_created; jj++)
				if (spr[jj].active) spr[jj].disabled = false;
			return 0;
		}


		if (compare(ev[1], "sp_pseq")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].pseq);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_pframe")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].pframe);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_seq")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].seq);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "editor_type")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				//Msg("Setting editor_type..");
				returnint = change_edit_char(nlist[0], nlist[1], &play.spmap[*pmap].type[nlist[0]]);
				return 0;
			}
			returnint = -1;
			return 0;
		}
		if (compare(ev[1], "editor_seq")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_edit(nlist[0], nlist[1], &play.spmap[*pmap].seq[nlist[0]]);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "editor_frame")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_edit_char(nlist[0], nlist[1], &play.spmap[*pmap].frame[nlist[0]]);
				return 0;
			}
			returnint = -1;
			return 0;
		}



		if (compare(ev[1], "sp_editor_num")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = spr[nlist[0]].sp_index;
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_brain")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].brain);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_exp")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].exp);
				return 0;
			}
			returnint = -1;
			return 0;
		}
		if (compare(ev[1], "set_button")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				play.button[nlist[0]] = nlist[1];

				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_reverse")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].reverse);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_noclip")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].noclip);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_touch_damage")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].touch_damage);
				return 0;
			}
			returnint = -1;
			return 0;
		}



		if (compare(ev[1], "sp_brain_parm")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].brain_parm);
				return 0;
			}
			returnint = -1;
			return 0;
		}
		if (compare(ev[1], "sp_brain_parm2")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].brain_parm2);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_follow")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].follow);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "set_smooth_follow")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] == 0) {
					smooth_follow = false;
				} else if (nlist[0] == 1) {
					smooth_follow = true;
				}
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_frame")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].frame);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_frame_delay")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].frame_delay);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "hurt")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				//redink1 fix for freeze if hurt value is less than 0
				if (nlist[1] < 0)
					return 0;
				if (hurt_thing(nlist[0], nlist[1], 0) > 0)
					random_blood(spr[nlist[0]].x, spr[nlist[0]].y - 40, nlist[0]);
				if (spr[nlist[0]].nohit != 1)
					if (spr[nlist[0]].script != 0)

						if (locate(spr[nlist[0]].script, "HIT")) {

							if (rinfo[script]->sprite != 1000) {
								*penemy_sprite = rinfo[script]->sprite;
								//redink1 addition of missle_source stuff
								*pmissle_source = rinfo[script]->sprite;
							}

							kill_returning_stuff(spr[nlist[0]].script);
							run_script(spr[nlist[0]].script);
						}

				return 0;
			}
			returnint = -1;
			return 0;
		}







		if (compare(ev[1], "sp_hard")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].hard);
				if (spr[nlist[0]].sp_index != 0) if (nlist[1] != -1) {

					pam.sprite[spr[nlist[0]].sp_index].hard = returnint;
				}
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_move_nohard")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].move_nohard);
				return 0;
			}
			returnint = -1;
			return 0;
		}
		if (compare(ev[1], "sp_flying")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].flying);
				return 0;
			}
			returnint = -1;
			return 0;
		}




		if (compare(ev[1], "sp_kill_wait")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				spr[nlist[0]].wait = 0;
				return 0;
			}
			returnint = -1;
			return 0;
		}
		if (compare(ev[1], "sp_kill")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				spr[nlist[0]].kill = nlist[1];
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "screenlock")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] == 0 || nlist[0] == 1) {
					screenlock = nlist[0];
				}
			}
			//redink1 - set screenlock() to return the screenlock value
			returnint = screenlock;
			return 0;
		}

		if (compare(ev[1], "stop_entire_game")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				stop_entire_game = nlist[0];




				while (1) {

					RECT rcRect;

					SetRect(&rcRect, 0, 0, 640, 480);
					ddrval = lpDDSTwo->BltFast(0, 0, lpDDSBack,
						&rcRect, DDBLTFAST_NOCOLORKEY);

					if (ddrval == DD_OK) {
						break;
					}
					if (ddrval != DDERR_WASSTILLDRAWING) {
						dderror(ddrval);
						return 0;
					}
				}










				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "dink_can_walk_off_screen")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				walk_off_screen = nlist[0];
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "push_active")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				push_active = nlist[0];
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_x")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].x);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "count_item")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				for (i = 1; i < 17; i++) {
					if (play.item[i].active) {
						if (compare(play.item[i].name, slist[0])) returnint++;
					}

				}

				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "count_magic")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				for (i = 1; i < 9; i++) {
					if (play.mitem[i].active) {
						if (compare(play.mitem[i].name, slist[0])) returnint++;
					}

				}

				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_mx")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].mx);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_move_x")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].mx);
				return 0;
			}
			return 0;
		}

		if (compare(ev[1], "sp_my")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].my);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_move_y")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].my);
				return 0;
			}
			return 0;
		}

		if (compare(ev[1], "scripts_used")) {
			h = &h[strlen(ev[1])];
			int m = 0;
			for (i = 1; i < max_scripts; i++)
				if (rinfo[i] != NULL) m++;
			returnint = m;
			return 0;
		}




		if (compare(ev[1], "sp_dir")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].dir);

				if (nlist[1] != -1) changedir(spr[nlist[0]].dir, nlist[0], spr[nlist[0]].base_walk);
				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_hitpoints")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].hitpoints);

				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_attack_hit_sound")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].attack_hit_sound);

				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_attack_hit_sound_speed")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].attack_hit_sound_speed);

				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_strength")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].strength);

				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "sp_defense")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].defense);

				return 0;
			}
			returnint = -1;
			return 0;
		}

		if (compare(ev[1], "init")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {

				figure_out(slist[0], 0);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_distance")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].distance);

				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_nohit")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].nohit);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_notouch")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].notouch);
				return 0;
			}
			returnint = -1;
			return 0;
		}



		if (compare(ev[1], "compare_weapon")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				if (*pcur_weapon == 0) {
					return 0;
				}

				if (compare(play.item[*pcur_weapon].name, slist[0])) {
					returnint = 1;

				}
				return 0;
			}


			return 0;
		}


		if (compare(ev[1], "compare_magic")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				if (*pcur_magic == 0) {
					return 0;
				}

				//redink1 fix so compare_magic works!
				if (compare(play.mitem[*pcur_magic].name, slist[0])) {
					returnint = 1;

				}
				return 0;
			}


			return 0;
		}


		if (compare(ev[1], "compare_sprite_script")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;

				if (nlist[0] == 0) {
					Msg("Error: Can't compare sprite script for sprite 0!??!?!");
					return 0;
				}
				if (spr[nlist[0]].active) {

					if (spr[nlist[0]].script == 0) {
						Msg("Compare sprite script says: Sprite %d has no script.", nlist[0]);
						return 0;
					}
					if (compare(slist[1], rinfo[spr[nlist[0]].script]->name)) {
						returnint = 1;
						return 0;
					}

				} else {
					Msg("Can't compare sprite script, sprite not active.");
				}



				return 0;
			}


			return 0;
		}


		if (compare(ev[1], "sp_y")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].y);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "sp_timing")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].timer);
				return 0;
			}
			returnint = -1;
			return 0;
		}


		if (compare(ev[1], "return;")) {

			if (debug_mode) Msg("Found return; statement");

			if (rinfo[script]->proc_return != 0) {
				bKeepReturnInt = true;
				run_script(rinfo[script]->proc_return);
				kill_script(script);
			}

			return 2;
		}

		//redink1 added so we can have return values and crap.
		if (compare(ev[1], "return")) {

			if (debug_mode) Msg("Found return; statement");

			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			process_line(script, h, false);

			if (rinfo[script]->proc_return != 0) {
				bKeepReturnInt = true;
				run_script(rinfo[script]->proc_return);
				kill_script(script);
			}

			return 2;
		}


		if (compare(ev[1], "if")) {

			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			//Msg("running if with string of %s", h);

			process_line(script, h, false);

			//Msg("Result is %d", returnint);

			if (returnint != 0) {
				if (debug_mode) Msg("If returned true");


			} else {
				//don't do it!
				rinfo[script]->skipnext = true;
				if (debug_mode) Msg("If returned false, skipping next thing");
			}

			//DO STUFF HERE!
			strcpy(s, h);
			//g("continuing to run line %s..", h);

			return 5;
		}



		if (compare(ev[2], "=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[1];
			strip_beginning_spaces(h);
			var_equals(ev[1], ev[3], '=', script, h);
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[2], "+=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[2];
			strip_beginning_spaces(h);
			var_equals(ev[1], ev[3], '+', script, h);
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[2], "*=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[2];
			strip_beginning_spaces(h);
			var_equals(ev[1], ev[3], '*', script, h);
			strcpy(s, h);
			return 0;
		}



		if (compare(ev[2], "-=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[2];
			strip_beginning_spaces(h);

			var_equals(ev[1], ev[3], '-', script, h);

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[2], "/") || compare(ev[2], "/=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[1];
			strip_beginning_spaces(h);

			var_equals(ev[1], ev[3], '/', script, h);

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[2], "*") || compare(ev[2], "*=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[1];
			strip_beginning_spaces(h);

			var_equals(ev[1], ev[3], '*', script, h);

			strcpy(s, h);
			return 0;
		}
		if (compare(ev[1], "external")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = { 2, 2, 1, 1, 1, 1, 1, 1, 1, 1 };
			memset(slist, 0, 10 * 200);
			get_parms(ev[1], script, h, ARR);
			if (strlen(slist[0]) > 0 && strlen(slist[1]) > 0) {
				int myscript1 = load_script(slist[0], rinfo[script]->sprite, false);
				if (myscript1 == 0) {
					Msg("Error:  Couldn't find %s.c (for procedure %s)", slist[0], slist[1]);
					return 0;
				}
				rinfo[myscript1]->arg1 = nlist[2];
				rinfo[myscript1]->arg2 = nlist[3];
				rinfo[myscript1]->arg3 = nlist[4];
				rinfo[myscript1]->arg4 = nlist[5];
				rinfo[myscript1]->arg5 = nlist[6];
				rinfo[myscript1]->arg6 = nlist[7];
				rinfo[myscript1]->arg7 = nlist[8];
				rinfo[myscript1]->arg8 = nlist[9];
				if (locate(myscript1, slist[1])) {
					rinfo[myscript1]->proc_return = script;
					run_script(myscript1);

					return 2;
				} else {
					Msg("Error:  Couldn't find procedure %s in %s.", slist[1], slist[0]);
					kill_script(myscript1);
				}
			}
			strcpy(s, h);
			return 0;
		}


		if (strchr(h, '(') != NULL) {

			//lets attempt to run a procedure

			int myscript = load_script(rinfo[script]->name, rinfo[script]->sprite, false);

			h = &h[strlen(ev[1])];

			int ARR[20] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
			get_parms(ev[1], script, h, ARR);

			if (locate(myscript, ev[1])) {
				rinfo[myscript]->arg1 = nlist[0];
				rinfo[myscript]->arg2 = nlist[1];
				rinfo[myscript]->arg3 = nlist[2];
				rinfo[myscript]->arg4 = nlist[3];
				rinfo[myscript]->arg5 = nlist[4];
				rinfo[myscript]->arg6 = nlist[5];
				rinfo[myscript]->arg7 = nlist[6];
				rinfo[myscript]->arg8 = nlist[7];
				rinfo[myscript]->arg9 = nlist[8];

				rinfo[myscript]->proc_return = script;
				run_script(myscript);
				return 2;
			} else {
				for (i = 0; strlen(play.func[i].func) > 0 && i < 100; i++) {
					if (compare(play.func[i].func, ev[1])) {
						myscript = load_script(play.func[i].file, rinfo[script]->sprite, false);
						rinfo[myscript]->arg1 = nlist[0];
						rinfo[myscript]->arg2 = nlist[1];
						rinfo[myscript]->arg3 = nlist[2];
						rinfo[myscript]->arg4 = nlist[3];
						rinfo[myscript]->arg5 = nlist[4];
						rinfo[myscript]->arg6 = nlist[5];
						rinfo[myscript]->arg7 = nlist[6];
						rinfo[myscript]->arg8 = nlist[7];
						rinfo[myscript]->arg9 = nlist[8];
						if (locate(myscript, ev[1])) {
							rinfo[myscript]->proc_return = script;
							run_script(myscript);
							return 2;
						}
						break;
					}
				}
				Msg("ERROR:  Procedure void %s( void ); not found in script %s. (word 2 was %s) ", line,
					ev[2], rinfo[myscript]->name);
				kill_script(myscript);
			}

			/*seperate_string(h, 1,'(',line);


			int myscript = load_script(rinfo[script]->name, rinfo[script]->sprite, false);

			if (locate( myscript, line))
			{
			rinfo[myscript]->proc_return = script;
			run_script(myscript);
			return 2;
			} else
			{
			Msg("ERROR:  Procedure void %s( void ); not found in script %s. (word 2 was %s) ", line,
			ev[2], rinfo[myscript]->name);
			kill_script(myscript);
			}*/


			return 0;

		}






		Msg("MERROR: \"%s\" unknown in %s, offset %d.", ev[1], rinfo[script]->name, rinfo[script]->current);


		//in a thingie, ready to go

	}

bad:
	strcpy(s, h);
	return 0;

good:
	strcpy(s, h);
	//s = h
	//Msg("ok, continuing with running %s..",s);
	return 1;

}

void init_scripts() {
	for (int k = 1; k < max_scripts; k++) {
		if (rinfo[k] != NULL && rinfo[k]->sprite != 0 && rinfo[k]->sprite < 300 && spr[rinfo[k]->sprite].active) {
			if (locate(k, "main")) {
				if (debug_mode) Msg("Screendraw: running main of script %s..", rinfo[k]->name);
				run_script(k);
			}
		}
	}
}

void run_script(int script) {
	int result;
	char line[200];

	if (bKeepReturnInt) {
		bKeepReturnInt = false;
	} else {
		returnint = 0;
	}

	returnstring[0] = 0;
	if (rinfo[script] != NULL) {
		if (debug_mode)
			Msg("Script %s is entered at offset %d.", rinfo[script]->name, rinfo[script]->current);
	} else {
		Msg("Error:  Tried to run a script that doesn't exist in memory.  Nice work.");
	}

	while (read_next_line(script, line)) {
		while (1) {
			strip_beginning_spaces(line);
			if (compare(line, "\n"))
				break;

			result = process_line(script, line, false);
			if (result == 3) {
redo:
				read_next_line(script, line);
crappa:
				strip_beginning_spaces(line);
				if (compare(line, "\n")) goto redo;
				if (compare(line, "\\\\")) goto redo;
				//       Msg("processing %s knowing we are going to skip it...", line);
				result = process_line(script, line, true);
			}

			if (result == 5)
				goto crappa;
			if (result == 3)
				goto redo;

			if (result == 2) {
				if (debug_mode) Msg("giving script the boot");
				// Quit script
				return;
			}
			if (result == 0)
				break;

			if (result == 4) {
				if (strlen(line) < 2) {
redo2:
					read_next_line(script, line);
					strip_beginning_spaces(line);
					//Msg("Comparing to %s.", line);
					if (compare(line, "\n")) goto redo2;
					if (compare(line, "\\\\")) goto redo2;
				}
				result = process_line(script, line, true);
			}

			if (result == 2) {
				if (debug_mode) Msg("giving script the boot");
				// Quit script
				return;
			}
			if (result == 0)
				break;
		}
	}

	if (rinfo[script] != NULL) {
		if (rinfo[script]->proc_return != 0) {
			run_script(rinfo[script]->proc_return);
			kill_script(script);
		}
	}
}

void kill_scripts_owned_by(int sprite) {
	for (int i = 1; i < max_scripts; i++) {
		if (rinfo[i] != NULL) {
			if (rinfo[i]->sprite == sprite) {
				kill_script(i);
			}
		}
	}
}

static void kill_callbacks_owned_by_script(int script) {
	for (int i = 1; i < max_callbacks; i++) {
		if (callback[i].owner == script) {
			if (debug_mode) Msg("Kill_all_callbacks just killed %d for script %d", i, script);
			//killed callback
			callback[i].active = false;
		}
	}
}

void kill_script(int num) {
	if (rinfo[num] != NULL) {

		kill_callbacks_owned_by_script(num);

		//now lets kill all local vars assocated with this script

		for (int i = 1; i < max_vars; i++) {
			if (play.var[i].active) if (play.var[i].scope == num) {
				play.var[i].active = false;
			}

		}
		if (debug_mode) Msg("Killed script %s. (num %d)", rinfo[num]->name, num);


		free(rinfo[num]);
		rinfo[num] = NULL;
		free(rbuf[num]);
		rbuf[num] = NULL;
	}
}

void kill_all_scripts() {
	int ii;

	for (ii = 1; ii < max_scripts; ii++) {

		if (rinfo[ii] != NULL) if (rinfo[ii]->sprite != 1000)
			kill_script(ii);
	}

	for (ii = 1; ii <= max_callbacks; ii++) {
		if (callback[ii].active) {
			if ((rinfo[callback[ii].owner] != NULL) && (rinfo[callback[ii].owner]->sprite == 1000)) {

			} else {
				if (debug_mode) Msg("Killed callback %d.  (was attached to script %d.)", ii, callback[ii].owner);
				callback[ii].active = 0;
			}
		}
	}
}

void kill_all_scripts_for_real() {
	int ii;

	for (ii = 1; ii < max_scripts; ii++) {

		if (rinfo[ii] != NULL)
			kill_script(ii);
	}

	for (ii = 1; ii <= max_callbacks; ii++) {

		callback[ii].active = 0;
	}
}

bool read_next_line(int script, char *line) {
	if ((rinfo[script] == NULL) || (rbuf == NULL)) {

		Msg("  ERROR:  Tried to read script %d, it doesn't exist.", script);
		return false;
	}

	if (rinfo[script]->current >= rinfo[script]->end) {
		//at end of buffer
		return false;
	}

	/*      if (rinfo[script]->current < -1);
	{
	//something errored out, why did it go negetive?
	return false;
	}
	*/

	strcpy(line, "");

	for (int ii = rinfo[script]->current; (ii < rinfo[script]->end); ii++) {


		//      Msg("..%d",ii);
		strchar(line, rbuf[script][ii]);
		rinfo[script]->current++;

		if ((rbuf[script][ii] == '\n') || (rbuf[script][ii] == '\r')) {
			return true;
		}

		if (rinfo[script]->current >= rinfo[script]->end) return false;

	}

	return false;
}

} // namespace Dink
