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

#include "ags2/engine/routefnd.h"
#include "ags2/lib/allegro.h"
#include "ags2/vars.h"

namespace AGS2 {

#define MAXPATHBACK 1000
#define COPYRIGHT_CRC 172668
const int BEENHERE_SIZE = 2;

#define DIR_LEFT  0
#define DIR_RIGHT 2
#define DIR_UP    1
#define DIR_DOWN  3


static const char ac_engine_copyright[] = "Adventure Game Studio engine & tools (c) 1999-2000 by Chris Jones.";
constexpr int _stklen = 2048000;

void init_pathfinder() {
	pathbackx = (int *)malloc(sizeof(int) * MAXPATHBACK);
	pathbacky = (int *)malloc(sizeof(int) * MAXPATHBACK);
}

// stupid name, to deter hackers
int get_route_composition() {
	int aaa, crctotal = 0;
	for (aaa = 0; aaa < 66; aaa++) {
		crctotal += ac_engine_copyright[aaa] * (aaa + 1);
	}
	return crctotal;
}

int route_script_link() {
	if (COPYRIGHT_CRC != get_route_composition())
		return 1;
	if (routex1 != -10)
		return 1;
	return 0;
}

#define get_copyright_crc get_route_composition

void line_callback(block bmpp, int x, int y, int d) {
	/*  if ((x>=320) | (y>=200) | (x<0) | (y<0)) line_failed=1;
	  else */ if (getpixel(bmpp, x, y) < 1)
	line_failed = 1;
	  else if (line_failed == 0) {
		  lastcx = x;
		  lastcy = y;
	  }
}

// check the copyright message is intact
#ifdef _MSC_VER
extern void winalert(char *, ...);
#endif
// whether the message has been printed, deter hackers
int walk_area_zone5 = 0;

void print_welcome_text(char *verno, char *aciverno) {
	routex1 = -10;
}

int can_see_from(int x1, int y1, int x2, int y2) {
	line_failed = 0;
	lastcx = x1;
	lastcy = y1;

	if ((x1 == x2) && (y1 == y2))
		return 1;

	do_line(wallscreen, x1, y1, x2, y2, 0, line_callback);
	if (line_failed == 0)
		return 1;

	return 0;
}

int find_nearest_walkable_area(block tempw, int fromX, int fromY, int toX, int toY, int destX, int destY, int granularity) {
	int ex, ey, nearest = 99999, thisis, nearx = 0, neary = 0;
	if (fromX < 0) fromX = 0;
	if (fromY < 0) fromY = 0;
	if (toX >= tempw->w) toX = tempw->w - 1;
	if (toY >= tempw->h) toY = tempw->h - 1;

	for (ex = fromX; ex < toX; ex += granularity)
	{
		for (ey = fromY; ey < toY; ey += granularity)
		{
			if (tempw->line[ey][ex] != 232)
				continue;

			thisis = (int)::sqrt((double)((ex - destX) * (ex - destX) + (ey - destY) * (ey - destY)));
			if (thisis < nearest)
			{
				nearest = thisis;
				nearx = ex;
				neary = ey;
			}
		}
	}

	if (nearest < 90000) {
		suggestx = nearx;
		suggesty = neary;
		return 1;
	}

	return 0;
}

#define MAX_GRANULARITY 3
int walk_area_granularity[MAX_WALK_AREAS + 1];
int is_route_possible(int fromx, int fromy, int tox, int toy, block wss) {
	wallscreen = wss;
	suggestx = -1;

	// ensure it's a memory bitmap, so we can use direct access to line[] array
	if ((wss == NULL) || (!is_memory_bitmap(wss)) || (bitmap_color_depth(wss) != 8))
		quit("is_route_possible: invalid walkable areas bitmap supplied");

	if (getpixel(wallscreen, fromx, fromy) < 1)
		return 0;

	block tempw = create_bitmap_ex(8, wallscreen->w, wallscreen->h);

	if (tempw == NULL)
		quit("no memory for route calculation");
	if (!is_memory_bitmap(tempw))
		quit("tempw is not memory bitmap");

	blit(wallscreen, tempw, 0, 0, 0, 0, tempw->w, tempw->h);

	int dd, ff;
	// initialize array for finding widths of walkable areas
	int thisar, inarow = 0, lastarea = 0;
	int walk_area_times[MAX_WALK_AREAS + 1];
	for (dd = 0; dd <= MAX_WALK_AREAS; dd++) {
		walk_area_times[dd] = 0;
		walk_area_granularity[dd] = 0;
	}

	for (ff = 0; ff < tempw->h; ff++) {
		for (dd = 0; dd < tempw->w; dd++) {
			thisar = tempw->line[ff][dd];
			// count how high the area is at this point
			if ((thisar == lastarea) && (thisar > 0))
				inarow++;
			else if (lastarea > MAX_WALK_AREAS)
				quit("!Calculate_Route: invalid colours in walkable area mask");
			else if (lastarea != 0) {
				walk_area_granularity[lastarea] += inarow;
				walk_area_times[lastarea]++;
				inarow = 0;
			}
			lastarea = thisar;
		}
	}

	for (dd = 0; dd < tempw->w; dd++) {
		for (ff = 0; ff < tempw->h; ff++) {
			thisar = tempw->line[ff][dd];
			if (thisar > 0)
				putpixel(tempw, dd, ff, 1);
			// count how high the area is at this point
			if ((thisar == lastarea) && (thisar > 0))
				inarow++;
			else if (lastarea != 0) {
				walk_area_granularity[lastarea] += inarow;
				walk_area_times[lastarea]++;
				inarow = 0;
			}
			lastarea = thisar;
		}
	}

	// find the average "width" of a path in this walkable area
	for (dd = 1; dd <= MAX_WALK_AREAS; dd++) {
		if (walk_area_times[dd] == 0) {
			walk_area_granularity[dd] = MAX_GRANULARITY;
			continue;
		}

		walk_area_granularity[dd] /= walk_area_times[dd];
		if (walk_area_granularity[dd] <= 4)
			walk_area_granularity[dd] = 2;
		else if (walk_area_granularity[dd] <= 15)
			walk_area_granularity[dd] = 3;
		else
			walk_area_granularity[dd] = MAX_GRANULARITY;

		/*char toprnt[200];
		   sprintf(toprnt,"area %d: Gran %d", dd, walk_area_granularity[dd]);
		   winalert(toprnt); */
	}
	walk_area_granularity[0] = MAX_GRANULARITY;

	floodfill(tempw, fromx, fromy, 232);
	if (getpixel(tempw, tox, toy) != 232)
	{
		// Destination pixel is not walkable
		// Try the 100x100 square around the target first at 3-pixel granularity
		int tryFirstX = tox - 50, tryToX = tox + 50;
		int tryFirstY = toy - 50, tryToY = toy + 50;

		if (!find_nearest_walkable_area(tempw, tryFirstX, tryFirstY, tryToX, tryToY, tox, toy, 3))
		{
			// Nothing found, sweep the whole room at 5 pixel granularity
			find_nearest_walkable_area(tempw, 0, 0, tempw->w, tempw->h, tox, toy, 5);
		}

		wfreeblock(tempw);
		return 0;
	}
	wfreeblock(tempw);

	return 1;
}

int try_this_square(int srcx, int srcy, int tox, int toy) {
	if (beenhere[srcy][srcx] & 0x80)
		return 0;

	// nesting of 8040 leads to stack overflow
	if (nesting > 7000)
		return 0;

	nesting++;
	if (can_see_from(srcx, srcy, tox, toy)) {
		finalpartx = srcx;
		finalparty = srcy;
		nesting--;
		pathbackstage = 0;
		return 2;
	}

	int trydir = DIR_UP;
	int xdiff = abs(srcx - tox), ydiff = abs(srcy - toy);
	if (ydiff > xdiff) {
		if (srcy > toy)
			trydir = DIR_UP;
		else
			trydir = DIR_DOWN;
	} else if (srcx > tox)
		trydir = DIR_LEFT;
	else if (srcx < tox)
		trydir = DIR_RIGHT;

	int iterations = 0;

try_again:
	int nextx = srcx, nexty = srcy;
	if (trydir == DIR_LEFT)
		nextx--;
	else if (trydir == DIR_RIGHT)
		nextx++;
	else if (trydir == DIR_DOWN)
		nexty++;
	else if (trydir == DIR_UP)
		nexty--;

	iterations++;
	if (iterations > 5) {
		//    fprintf(stderr,"not found: %d,%d  beenhere 0x%X\n",srcx,srcy,beenhere[srcy][srcx]);
		nesting--;
		return 0;
	}

	if (((nextx < 0) | (nextx >= wallscreen->w) | (nexty < 0) | (nexty >= wallscreen->h)) ||
		(getpixel(wallscreen, nextx, nexty) == 0) || ((beenhere[srcy][srcx] & (1 << trydir)) != 0)) {

		if (leftorright == 0) {
			trydir++;
			if (trydir > 3)
				trydir = 0;
		} else {
			trydir--;
			if (trydir < 0)
				trydir = 3;
		}
		goto try_again;
	}
	beenhere[srcy][srcx] |= (1 << trydir);
	//  srcx=nextx; srcy=nexty;
	beenhere[srcy][srcx] |= 0x80; // being processed

	int retcod = try_this_square(nextx, nexty, tox, toy);
	if (retcod == 0)
		goto try_again;

	nesting--;
	beenhere[srcy][srcx] &= 0x7f;
	if (retcod == 2) {
		pathbackx[pathbackstage] = srcx;
		pathbacky[pathbackstage] = srcy;
		pathbackstage++;
		if (pathbackstage >= MAXPATHBACK - 1)
			return 0;

		return 2;
	}
	return 1;
}


#define CHECK_MIN(cellx, celly) { \
  if (beenhere[celly][cellx] == -1) {\
    adjcount = 0; \
    if ((wallscreen->line[celly][cellx] != 0) && (beenhere[j][i]+modifier <= min)) {\
      if (beenhere[j][i]+modifier < min) { \
        min = beenhere[j][i]+modifier; \
        numfound = 0; } \
      if (numfound < 40) { \
        newcell[numfound] = (celly) * wallscreen->w + (cellx);\
        cheapest[numfound] = j * wallscreen->w + i;\
        numfound++; \
      }\
    } \
  }}

#define MAX_TRAIL_LENGTH 5000

// Round down the supplied co-ordinates to the area granularity,
// and move a bit if this causes them to become non-walkable
void round_down_coords(int &tmpx, int &tmpy) {
	int startgran = walk_area_granularity[_getpixel(wallscreen, tmpx, tmpy)];
	tmpy = tmpy - tmpy % startgran;

	if (tmpy < 0)
		tmpy = 0;

	tmpx = tmpx - tmpx % startgran;
	if (tmpx < 0)
		tmpx = 0;

	if (_getpixel(wallscreen, tmpx, tmpy) == 0) {
		tmpx += startgran;
		if ((_getpixel(wallscreen, tmpx, tmpy) == 0) && (tmpy < wallscreen->h - startgran)) {
			tmpy += startgran;

			if (_getpixel(wallscreen, tmpx, tmpy) == 0)
				tmpx -= startgran;
		}
	}
}

int find_route_dijkstra(int fromx, int fromy, int destx, int desty) {
	int i, j;

	// This algorithm doesn't behave differently the second time, so ignore
	if (leftorright == 1)
		return 0;

	for (i = 0; i < wallscreen->h; i++)
		memset(&beenhere[i][0], 0xff, wallscreen->w * BEENHERE_SIZE);

	round_down_coords(fromx, fromy);
	beenhere[fromy][fromx] = 0;

	int temprd = destx, tempry = desty;
	round_down_coords(temprd, tempry);
	if ((temprd == fromx) && (tempry == fromy)) {
		// already at destination
		pathbackstage = 0;
		return 1;
	}

	int allocsize = int(wallscreen->w) * int(wallscreen->h) * sizeof(int);
	int *parent = (int *)malloc(allocsize);
	int min = 999999, cheapest[40], newcell[40], replace[40];
	int *visited = (int *)malloc(MAX_TRAIL_LENGTH * sizeof(int));
	int iteration = 1;
	visited[0] = fromy * wallscreen->w + fromx;
	parent[visited[0]] = -1;

	//  write_log("Pathfind starting");
	int granularity = 3, newx = -1, newy, foundAnswer = -1, numreplace;
	int changeiter, numfound, adjcount;
	int destxlow = destx - MAX_GRANULARITY;
	int destylow = desty - MAX_GRANULARITY;
	int destxhi = destxlow + MAX_GRANULARITY * 2;
	int destyhi = destylow + MAX_GRANULARITY * 2;
	int modifier = 0;
	int totalfound = 0;
	int DIRECTION_BONUS = 0;

	update_polled_stuff();

	while (foundAnswer < 0) {
		min = 29999;
		changeiter = iteration;
		numfound = 0;
		numreplace = 0;

		for (int n = 0; n < iteration; n++) {
			if (visited[n] == -1)
				continue;

			i = visited[n] % wallscreen->w;
			j = visited[n] / wallscreen->w;
			granularity = walk_area_granularity[wallscreen->line[j][i]];
			adjcount = 1;

			//    char tempb[200]; sprintf(tempb, "checking: %d,%d\n",i,j); write_log(tempb);
			if (i >= granularity) {
				modifier = (destx < i) ? DIRECTION_BONUS : 0;
				CHECK_MIN(i - granularity, j)
			}

			if (j >= granularity) {
				modifier = (desty < j) ? DIRECTION_BONUS : 0;
				CHECK_MIN(i, j - granularity)
			}

			if (i < wallscreen->w - granularity) {
				modifier = (destx > i) ? DIRECTION_BONUS : 0;
				CHECK_MIN(i + granularity, j)
			}

			if (j < wallscreen->h - granularity) {
				modifier = (desty > j) ? DIRECTION_BONUS : 0;
				CHECK_MIN(i, j + granularity)
			}

			// If all the adjacent cells have been done, stop checking this one
			if (adjcount) {
				if (numreplace < 40) {
					visited[numreplace] = -1;
					replace[numreplace] = n;
					numreplace++;
				}
			}
		}

		if (numfound == 0) {
			free(visited);
			free(parent);
			return 0;
		}

		totalfound += numfound;
		for (int p = 0; p < numfound; p++) {
			newx = newcell[p] % wallscreen->w;
			newy = newcell[p] / wallscreen->w;
			beenhere[newy][newx] = beenhere[cheapest[p] / wallscreen->w][cheapest[p] % wallscreen->w] + 1;
			//      int wal = walk_area_granularity[getpixel(wallscreen, newx, newy)];
			//      beenhere[newy - newy%wal][newx - newx%wal] = beenhere[newy][newx];
			parent[newcell[p]] = cheapest[p];

			// edges of screen pose a problem, so if current and dest are within
			// certain distance of the edge, say we've got it
			if ((newx >= wallscreen->w - MAX_GRANULARITY) && (destx >= wallscreen->w - MAX_GRANULARITY))
				newx = destx;

			if ((newy >= wallscreen->h - MAX_GRANULARITY) && (desty >= wallscreen->h - MAX_GRANULARITY))
				newy = desty;

			// Found the desination, abort loop
			if ((newx >= destxlow) && (newx <= destxhi) && (newy >= destylow)
				&& (newy <= destyhi)) {
				foundAnswer = newcell[p];
				break;
			}

			if (totalfound >= 1000) {
				//Doesn't work cos it can see the destination from the point that's
				//not nearest
				// every so often, check if we can see the destination
				if (can_see_from(newx, newy, destx, desty)) {
					DIRECTION_BONUS -= 50;
					totalfound = 0;
				}

			}

			if (numreplace > 0) {
				numreplace--;
				changeiter = replace[numreplace];
			} else
				changeiter = iteration;

			visited[changeiter] = newcell[p];
			if (changeiter == iteration)
				iteration++;

			changeiter = iteration;
			if (iteration >= MAX_TRAIL_LENGTH) {
				free(visited);
				free(parent);
				return 0;
			}
		}
		if (totalfound >= 1000) {
			update_polled_stuff();
			totalfound = 0;
		}
	}
	free(visited);

	int on;
	pathbackstage = 0;
	pathbackx[pathbackstage] = destx;
	pathbacky[pathbackstage] = desty;
	pathbackstage++;

	for (on = parent[foundAnswer];; on = parent[on]) {
		if (on == -1)
			break;

		newx = on % wallscreen->w;
		newy = on / wallscreen->w;
		if ((newx >= destxlow) && (newx <= destxhi) && (newy >= destylow)
			&& (newy <= destyhi))
			break;

		pathbackx[pathbackstage] = on % wallscreen->w;
		pathbacky[pathbackstage] = on / wallscreen->w;
		pathbackstage++;
		if (pathbackstage >= MAXPATHBACK) {
			free(parent);
			return 0;
		}
	}
	free(parent);
	return 1;
}

int __find_route(int srcx, int srcy, short *tox, short *toy, int noredx) {
	if ((noredx == 0) && (getpixel(wallscreen, tox[0], toy[0]) == 0))
		return 0; // clicked on a wall

	int is_straight = 0;
	if ((srcx - tox[0] == 0) || (srcy - toy[0] == 0) || (abs(srcx - tox[0]) == abs(srcy - toy[0])))
		is_straight = 1;

	pathbackstage = 0;

	if (leftorright == 0) {
		waspossible = 1;

findroutebk:
		if ((srcx == tox[0]) && (srcy == toy[0])) {
			pathbackstage = 0;
			return 1;
		}

		if ((waspossible = is_route_possible(srcx, srcy, tox[0], toy[0], wallscreen)) == 0) {
			if (suggestx >= 0) {
				tox[0] = suggestx;
				toy[0] = suggesty;
				goto findroutebk;
			}
			return 0;
		}
	}

	if (leftorright == 1) {
		if (waspossible == 0)
			return 0;
	}

	if (is_straight)
		;            // don't use new algo on arrow key presses
	else if (find_route_dijkstra(srcx, srcy, tox[0], toy[0])) {
		return 1;
	}

	// if the new pathfinder failed, try the old one
	pathbackstage = 0;
	memset(&beenhere[0][0], 0, wallscreen->w * wallscreen->h * BEENHERE_SIZE);
	if (try_this_square(srcx, srcy, tox[0], toy[0]) == 0)
		return 0;

	return 1;
}

void set_route_move_speed(int speed_x, int speed_y) {
	// negative move speeds like -2 get converted to 1/2
	if (speed_x < 0) {
		move_speed_x = itofix(1) / (-speed_x);
	} else {
		move_speed_x = itofix(speed_x);
	}

	if (speed_y < 0) {
		move_speed_y = itofix(1) / (-speed_y);
	} else {
		move_speed_y = itofix(speed_y);
	}
}

// Calculates the X and Y per game loop, for this stage of the
// movelist
void calculate_move_stage(MoveList *mlsp, int aaa) {
	// work out the x & y per move. First, opp/adj=tan, so work out the angle
	if (mlsp->pos[aaa] == mlsp->pos[aaa + 1]) {
		mlsp->xpermove[aaa] = 0;
		mlsp->ypermove[aaa] = 0;
		return;
	}

	short ourx = (mlsp->pos[aaa] >> 16) & 0x000ffff;
	short oury = (mlsp->pos[aaa] & 0x000ffff);
	short destx = ((mlsp->pos[aaa + 1] >> 16) & 0x000ffff);
	short desty = (mlsp->pos[aaa + 1] & 0x000ffff);

	// Special case for vertical and horizontal movements
	if (ourx == destx) {
		mlsp->xpermove[aaa] = 0;
		mlsp->ypermove[aaa] = move_speed_y;
		if (desty < oury)
			mlsp->ypermove[aaa] = -mlsp->ypermove[aaa];

		return;
	}

	if (oury == desty) {
		mlsp->xpermove[aaa] = move_speed_x;
		mlsp->ypermove[aaa] = 0;
		if (destx < ourx)
			mlsp->xpermove[aaa] = -mlsp->xpermove[aaa];

		return;
	}

	fixed xdist = itofix(abs(ourx - destx));
	fixed ydist = itofix(abs(oury - desty));

	fixed useMoveSpeed;

	if (move_speed_x == move_speed_y) {
		useMoveSpeed = move_speed_x;
	} else {
		// different X and Y move speeds
		// the X proportion of the movement is (x / (x + y))
		fixed xproportion = fixdiv(xdist, (xdist + ydist));

		if (move_speed_x > move_speed_y) {
			// speed = y + ((1 - xproportion) * (x - y))
			useMoveSpeed = move_speed_y + fixmul(xproportion, move_speed_x - move_speed_y);
		} else {
			// speed = x + (xproportion * (y - x))
			useMoveSpeed = move_speed_x + fixmul(itofix(1) - xproportion, move_speed_y - move_speed_x);
		}
	}

	fixed angl = fatan(fdiv(ydist, xdist));

	// now, since new opp=hyp*sin, work out the Y step size
	//fixed newymove = useMoveSpeed * fsin(angl);
	fixed newymove = fixmul(useMoveSpeed, fsin(angl));

	// since adj=hyp*cos, work out X step size
	//fixed newxmove = useMoveSpeed * fcos(angl);
	fixed newxmove = fixmul(useMoveSpeed, fcos(angl));

	if (destx < ourx)
		newxmove = -newxmove;
	if (desty < oury)
		newymove = -newymove;

	mlsp->xpermove[aaa] = newxmove;
	mlsp->ypermove[aaa] = newymove;
}


#define MAKE_INTCOORD(x,y) (((unsigned short)x << 16) | ((unsigned short)y))

int find_route(short srcx, short srcy, short xx, short yy, block onscreen, int movlst, int nocross, int ignore_walls) {
	wallscreen = onscreen;
	leftorright = 0;
	int aaa;

	if (wallscreen->h > beenhere_array_size)
	{
		beenhere = (short **)realloc(beenhere, sizeof(short *) * wallscreen->h);
		beenhere_array_size = wallscreen->h;

		if (beenhere == NULL)
			quit("insufficient memory to allocate pathfinder beenhere buffer");
	}

	int orisrcx = srcx, orisrcy = srcy;
	finalpartx = -1;

	if (ignore_walls) {
		pathbackstage = 0;
	} else if (can_see_from(srcx, srcy, xx, yy)) {
		pathbackstage = 0;
	} else {
		beenhere[0] = (short *)malloc((wallscreen->w) * (wallscreen->h) * BEENHERE_SIZE);

		for (aaa = 1; aaa < wallscreen->h; aaa++)
			beenhere[aaa] = beenhere[0] + aaa * (wallscreen->w);

		if (__find_route(srcx, srcy, &xx, &yy, nocross) == 0) {
			leftorright = 1;
			if (__find_route(srcx, srcy, &xx, &yy, nocross) == 0)
				pathbackstage = -1;
		}
		free(beenhere[0]);
	}

	if (pathbackstage >= 0) {
		int nearestpos = 0, nearestindx;
		int reallyneed[MAXNEEDSTAGES], numstages = 0;
		reallyneed[numstages] = MAKE_INTCOORD(srcx, srcy);
		numstages++;
		nearestindx = -1;

		int lastpbs = pathbackstage;

stage_again:
		nearestpos = 0;
		aaa = 1;
		// find the furthest point that can be seen from this stage
		for (aaa = pathbackstage - 1; aaa >= 0; aaa--) {
			//      fprintf(stderr,"stage %2d: %2d,%2d\n",aaa,pathbackx[aaa],pathbacky[aaa]);
			if (can_see_from(srcx, srcy, pathbackx[aaa], pathbacky[aaa])) {
				nearestpos = MAKE_INTCOORD(pathbackx[aaa], pathbacky[aaa]);
				nearestindx = aaa;
			}
		}

		if ((nearestpos == 0) && (can_see_from(srcx, srcy, xx, yy) == 0) &&
			(srcx >= 0) && (srcy >= 0) && (srcx < wallscreen->w) && (srcy < wallscreen->h) && (pathbackstage > 0)) {
			// If we couldn't see anything, we're stuck in a corner so advance
			// to the next square anyway (but only if they're on the screen)
			nearestindx = pathbackstage - 1;
			nearestpos = MAKE_INTCOORD(pathbackx[nearestindx], pathbacky[nearestindx]);
		}

		if (nearestpos > 0) {
			reallyneed[numstages] = nearestpos;
			numstages++;
			if (numstages >= MAXNEEDSTAGES - 1)
				quit("too many stages for auto-walk");
			srcx = (nearestpos >> 16) & 0x000ffff;
			srcy = nearestpos & 0x000ffff;
			//      Display("Added: %d, %d pbs:%d",srcx,srcy,pathbackstage);
			lastpbs = pathbackstage;
			pathbackstage = nearestindx;
			goto stage_again;
		}

		if (finalpartx >= 0) {
			reallyneed[numstages] = MAKE_INTCOORD(finalpartx, finalparty);
			numstages++;
		}

		// Make sure the end co-ord is in there
		if (reallyneed[numstages - 1] != MAKE_INTCOORD(xx, yy)) {
			reallyneed[numstages] = MAKE_INTCOORD(xx, yy);
			numstages++;
		}

		if ((numstages == 1) && (xx == orisrcx) && (yy == orisrcy)) {
			return 0;
		}
		//Display("Route from %d,%d to %d,%d - %d stage, %d stages", orisrcx,orisrcy,xx,yy,pathbackstage,numstages);

		int mlist = movlst;
		mls[mlist].numstage = numstages;
		memcpy(&mls[mlist].pos[0], &reallyneed[0], sizeof(int) * numstages);
		//    fprintf(stderr,"stages: %d\n",numstages);

		for (aaa = 0; aaa < numstages - 1; aaa++) {
			calculate_move_stage(&mls[mlist], aaa);
		}

		mls[mlist].fromx = orisrcx;
		mls[mlist].fromy = orisrcy;
		mls[mlist].onstage = 0;
		mls[mlist].onpart = 0;
		mls[mlist].doneflag = 0;
		mls[mlist].lastx = -1;
		mls[mlist].lasty = -1;

		return mlist;
	} else {
		return 0;
	}
}

} // namespace AGS2
