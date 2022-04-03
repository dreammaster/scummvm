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

#include "common/debug.h"
#include "common/str.h"
#include "common/textconsole.h"
#include "dink/text.h"
#include "dink/globals.h"
#include "dink/var.h"

namespace Dink {

void Msg(const char *fmt, ...) {
	va_list  va;
	va_start(va, fmt);
	Common::String s = Common::String::vformat(fmt, va);
	va_end(va);

	if (debug_mode)
		debug("%s", s.c_str());
}

void TRACE(const char *fmt, ...) {
	va_list  va;
	va_start(va, fmt);
	Common::String s = Common::String::vformat(fmt, va);
	va_end(va);

	if (debug_mode)
		debug("%s", s.c_str());
}

void dderror(int err) {
	error("dderror - %d", err);
}

void get_word(char line[300], int word, char *crap) {
	int cur = 0;

	bool space_mode = false;
	char save_word[100];
	save_word[0] = 0;

	for (int k = 0; k < (int)strlen(line); k++) {
		if (space_mode == true) {
			if (line[k] != ' ') {
				space_mode = false;
				strcpy(save_word, "");
			}
		}

		if (space_mode == false) {
			if (line[k] == ' ') {
				cur++;
				if (word == cur) goto done;
				space_mode = true;
				strcpy(save_word, "");

				goto dooba;
			} else {
				strchar(save_word, line[k]);
			}
		}
dooba: ;
	}

	if (space_mode == false) {
		if (cur + 1 != word)
			strcpy(save_word, "");
	}

done:
	strcpy(crap, save_word);
}

void reverse(char *st) {
	int i, ii;
	char don[255];
	don[0] = 0;
	ii = strlen(st);
	for (i = ii; i > -1; i--) {
		strchar(don, st[i]);
	}
	strcpy(st, don);
}

char *lmon(long money, char *dest) {
	char ho[30];
	int len, c;
	char lmon1[30];
	char buffer[30];
	bool quit1;
	quit1 = false;

	strcpy(lmon1, ltoa(money, buffer, 10));
	// prf("ORG IS '%s'",lmon1);

	if (strlen(lmon1) < 4) {
		strcpy(dest, lmon1);
		return dest;
	}

	strcpy(ho, ltoa(money, buffer, 10));
	len = strlen(ho);
	c = -1;
	lmon1[0] = 0;
	do {
		strchar(lmon1, ho[len]);
		len--;
		c++;
		if (c == 3) {
			if (len > -1) {
				strchar(lmon1, ',');
				c = 0;
			}
		}
		if (len < 0) quit1 = true;
	} while (quit1 == false);

	reverse(lmon1);
	strcpy(dest, lmon1);

	return dest;
}

bool compare(const char *orig, const char *comp) {
	uint len;

	len = strlen(comp);
	if (strlen(orig) != len)
		return false;

	if (scumm_strnicmp(orig, comp, len) == 0) {
		return true;
	}

	return false;
}

void replace(const char *this1, const char *that, char *line) {
	char hold[500];
	char thisup[200], lineup[500];
	uint u, i;
	int checker;
start:
	strcpy(hold, "");

	strcpy(lineup, line);
	strcpy(thisup, this1);

	strupr(lineup);
	strupr(thisup);
	if (strstr(lineup, thisup) == NULL) return;
	checker = -1;
	strcpy(hold, "");
	for (u = 0; u < strlen(line); u++) {
		if (checker > -1) {
			if (toupper(line[u]) == toupper(this1[checker])) {
				if (checker + 1 == (int)strlen(this1)) {
doit:
					u = u - strlen(this1);
					u++;
					for (i = 0; i < u; i++) hold[i] = line[i];
					for (i = 0; i < strlen(that); i++) hold[(u)+i] = that[i];
					hold[strlen(that) + u] = 0;
					for (i = 0; i < (strlen(line) - u) - strlen(this1); i++) {
						hold[(u + strlen(that)) + i] = line[(u + strlen(this1)) + i];
					}
					hold[(strlen(line) - strlen(this1)) + strlen(that)] = 0;
					strcpy(line, hold);
					goto start;
				}
				checker++;
			} else {
				checker = -1;
			}
		}
		if (checker == -1) {
			if (toupper(line[u]) == toupper(this1[0])) {

				//  if (line[u] < 97) that[0] = toupper(that[0]);
				checker = 1;
				if (strlen(this1) == 1) goto doit;
			}
		}
	}
}

bool seperate_string(const char *str, int num, char liney, char *return1) {
	int l;
	uint i;

	l = 1;
	strcpy(return1, "");

	for (i = 0; i <= strlen(str); i++) {
		if (str[i] == liney) {
			l++;
			if (l == num + 1)
				goto done;

			if (i < strlen(str))
				strcpy(return1, "");
		}
		if (str[i] != liney)
			sprintf(return1, "%s%c", return1, str[i]);
	}
	if (l < num)
		strcpy(return1, "");

	replace("\n", "", return1); //Take the /n off it.

	return false;

done:

	if (l < num)  strcpy(return1, "");

	replace("\n", "", return1); //Take the /n off it.

	//Msg("Took %s and turned it to %s.",str, return1);
	return true;
}


int var_figure(char h[200], int script) {
	char crap[200];
	int ret = 0;
	int n1 = 0, n2 = 0;
	get_word(h, 2, crap);

	if (compare(crap, "")) {
		// One word equation
		if (h[0] == '&') {
			// It's a var possibly
			decipher_string(h, script);
		}

		ret = atol(h);
		return ret;
	}

	get_word(h, 1, crap);
	decipher_string(crap, script);
	n1 = atol(crap);

	get_word(h, 3, crap);
	replace(")", "", crap);
	decipher_string(crap, script);
	n2 = atol(crap);

	get_word(h, 2, crap);
	if (debug_mode)
		Msg("Compared %d to %d", n1, n2);

	if (compare(crap, "==")) {
		if (n1 == n2) ret = 1;
		else ret = 0;
		return ret;
	}

	if (compare(crap, ">")) {
		if (n1 > n2) ret = 1;
		else ret = 0;
		return ret;
	}

	if (compare(crap, ">=")) {
		if (n1 >= n2) ret = 1;
		else ret = 0;
		return ret;
	}


	if (compare(crap, "<")) {
		if (n1 < n2) ret = 1;
		else ret = 0;
		return ret;
	}
	if (compare(crap, "<=")) {
		if (n1 <= n2) ret = 1;
		else ret = 0;
		return ret;
	}

	if (compare(crap, "!=")) {
		if (n1 != n2) ret = 1;
		else ret = 0;
		return ret;
	}

	return ret;
}

void kill_text_owned_by(int sprite) {
	for (int i = 1; i < max_sprites_at_once; i++) {
		if (spr[i].active) {
			if (spr[i].brain == 8) if (spr[i].owner == sprite) {
				spr[i].active = false;
			}
		}
	}
}

bool text_owned_by(int sprite) {
	for (int i = 1; i < max_sprites_at_once; i++) {
		if (spr[i].active) {
			if (spr[i].brain == 8) if (spr[i].owner == sprite)
				return true;
		}
	}

	return false;
}

void kill_text_owned_by_safe(int sprite) {
	for (int i = 1; i < max_sprites_at_once; i++) {
		if (spr[i].active) {
			if (spr[i].brain == 8) if (spr[i].owner == sprite) {
				spr[i].active = false;

				if (spr[i].callback != 0)
					run_script(spr[i].callback);
			}
		}
	}
}

} // namespace Dink
