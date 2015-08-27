/* RogueVM - RPG Engines
*
* RogueVM is the legal property of its developers, whose names
* are too numerous to list here. Please refer to the COPYRIGHT
* file distributed with this source distribution.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*/

//#include "aesop/vfx.h"

#include "aesop/aesop.h"
#include "aesop/gil2vfx.h"
#include "aesop/defs.h"
#include "aesop/shared.h"
#include "aesop/rtcode.h"
#include "aesop/rtsystem.h"
#include "aesop/rtmsg.h"
#include "aesop/rtobject.h"
#include "aesop/graphics.h"
#include "aesop/event.h"
#include "aesop/intrface.h"
#include "aesop/rtres.h"
#include "aesop/rt.h"
//#include "aesop/modsnd32.h"
#include "aesop/sound.h"

namespace Aesop {

#define SAVETYPE          (diag_flag ? SF_TXT:SF_BIN)

#define SAVEDIR_FN        "SAVEGAME\\SAVEGAME.DIR"

char savegame_dir[NUM_SAVEGAMES][SAVE_LEN+1];

static BYTE items_bin[] = "SAVEGAME\\ITEMS_yy.BIN";
static BYTE items_txt[] = "SAVEGAME\\ITEMS_yy.TXT";
static BYTE lvl_bin[] =   "SAVEGAME\\LVLxx_yy.BIN";
static BYTE lvl_txt[] =   "SAVEGAME\\LVLxx_yy.TXT";

static BYTE lvl_tmp[] =   "SAVEGAME\\LVLxx.TMP";
static BYTE itm_tmp[] =   "SAVEGAME\\ITEMS.TMP";

BYTE DX_offset[6][4] = {{ 0, 0, 0 ,0 },
                        { 0, 1, 0,-1 },
                        { 0, 0, 0, 0 },
                        {-1, 0, 1, 0 },
                        { 0,-1, 0, 1 },
                        { 1, 0,-1, 0 }};

BYTE DY_offset[6][4] = {{ 0, 0, 0, 0 },
                        {-1, 0, 1, 0 },
                        { 0, 0, 0, 0 },
                        { 0,-1, 0, 1 },
                        { 1, 0,-1, 0 },
                        { 0, 1, 0,-1 }};

extern BYTE txtbuf[2400]; // used as dot buffer -- needs 8 * MAXDOTS words

/*********************************************************/
#pragma off (unreferenced)
LONG step_X(LONG argcnt, ULONG x, ULONG fdir, ULONG mtype, ULONG distance)
#pragma on (unreferenced)
{
   BYTE xx = (BYTE) x;

   if (!distance) return x;

   if (mtype == MTYP_ML)
      {
      xx += DX_offset[MTYP_F-1][(UBYTE) fdir];
      xx += DX_offset[MTYP_L-1][(UBYTE) fdir];
      }
   else if (mtype == MTYP_MR)
      {
      xx += DX_offset[MTYP_F-1][(UBYTE) fdir];
      xx += DX_offset[MTYP_R-1][(UBYTE) fdir];
      }
   else if (mtype == MTYP_MM)
      {
      xx += 2 * DX_offset[MTYP_F-1][(UBYTE) fdir];
      }
   else if (mtype != MTYP_INIT)
      {
      xx += (BYTE) distance * DX_offset[(UBYTE) mtype-1][(UBYTE) fdir];
      }

   xx &= (LVL_X-1);

   return xx;
}

#pragma off (unreferenced)
LONG step_Y(LONG argcnt, ULONG y, ULONG fdir, ULONG mtype, ULONG distance)
#pragma on (unreferenced)
{
   BYTE yy = (BYTE) y;

   if (!distance) return y;

   if (mtype == MTYP_ML)
      {
      yy += DY_offset[MTYP_F-1][(UBYTE) fdir];
      yy += DY_offset[MTYP_L-1][(UBYTE) fdir];
      }
   else if (mtype == MTYP_MR)
      {
      yy += DY_offset[MTYP_F-1][(UBYTE) fdir];
      yy += DY_offset[MTYP_R-1][(UBYTE) fdir];
      }
   else if (mtype == MTYP_MM)
      {
      yy += 2 * DY_offset[MTYP_F-1][(UBYTE) fdir];
      }
   else if (mtype != MTYP_INIT)
      {
      yy += (BYTE) distance * DY_offset[(UBYTE) mtype-1][(UBYTE) fdir];
      }

   yy &= (LVL_Y-1);

   return yy;
}

#pragma off (unreferenced)
ULONG step_FDIR(LONG argcnt, ULONG fdir, ULONG mtype)
#pragma on (unreferenced)
{
   UBYTE f = (UBYTE) fdir;

   switch (mtype)
      {
      case MTYP_TL:
         return (f) ? (ULONG) f-1L : 3L;

      case MTYP_TR:
         return (f==3) ? 0L : (ULONG) f+1L;
      }

   return (ULONG) f;
}

/*********************************************************/
#pragma off (unreferenced)
LONG step_square_X(LONG argcnt, ULONG x, ULONG r, ULONG dir)
#pragma on (unreferenced)
{
   switch (dir)
      {
      case DIR_E: x = step_X(0,x,dir,MTYP_F,r & 0x01); break;
      case DIR_W: x = step_X(0,x,dir,MTYP_F,!(r & 0x01)); break;
      }

   return x;
}
                                                
#pragma off (unreferenced)
LONG step_square_Y(LONG argcnt, ULONG y, ULONG r, ULONG dir)
#pragma on (unreferenced)
{
   switch (dir)
      {
      case DIR_N: y = step_Y(0,y,dir,MTYP_F,r < 2); break;
      case DIR_S: y = step_Y(0,y,dir,MTYP_F,r >= 2); break;
      }

   return y;
}

#pragma off (unreferenced)
LONG step_region(LONG argcnt, ULONG r, ULONG dir)
#pragma on (unreferenced)
{
   switch (dir)
      {
      case DIR_N: 
      case DIR_S: r ^= 2; break;

      case DIR_E:
      case DIR_W: r ^= 1; break;
      }

   return r;
}

/*********************************************************/
#pragma off (unreferenced)
ULONG distance(LONG argcnt, ULONG x1, ULONG y1, ULONG x2, ULONG y2)
#pragma on (unreferenced)
{
   ULONG dx,dy,root;
   LONG num;
   static WORD square_root[32] =
      {
        0,1,4,9,16,25,36,49,64,81,100,121,144,169,196,225,256,
        289,324,361,400,441,484,529,576,625,676,729,784,841,900,961
      };

   dx = absv(0,x1-x2);
   dy = absv(0,y1-y2);

   num = (dx*dx) + (dy*dy);

   for (root=0;root<31;root++)
      {
      if ( ((LONG) square_root[root]) >= num) break;
      }

   return root;
}

/*********************************************************/
//
// Return octal direction in which entity at cur_x,y should 
// move in order to approach dest_x,y, or -1 if already there
//
// Direction is cardinal if bit 0 is clear, else diagonal
//
// Directions: N (NE) E (SE) S (SW) W (NW)
//             0  1   2  3   4  5   6  7
//
// Note that opposite direction may be obtained by XOR'ing with 4
//
/*********************************************************/

#pragma off (unreferenced)
ULONG seek_direction(LONG argcnt, ULONG cur_x, ULONG cur_y, ULONG
                           dest_x, ULONG dest_y)
#pragma on (unreferenced)
{
   LONG dx,dy;

   dx = dest_x - cur_x;
   dy = dest_y - cur_y;

   if (dx < 0)
      {                 // move west (- in X)
      if (dy > 0)
         return 5;      //  (southwest)
      else if (dy < 0)
         return 7;      //  (northwest)
      else
         return 6;      //  (due west)
      }
   else if (dx > 0)
      {                 // move east (+ in X)
      if (dy > 0)
         return 3;      //  (southeast)
      else if (dy < 0)
         return 1;      //  (northeast)
      else
         return 2;      //  (due east)
      }

   if (dy > 0)
      return 4;         // move south (+ in Y)
   else if (dy < 0)
      return 0;         // move north (- in Y)

   return -1;           // cur == dest, return -1
}

/*********************************************************/
//
// Fast status check for spell-request arrays
//
// Returns 1 if valid spells requested > spells known
//
// Assumes array = [2][10][10] = [ST] [LVL] [#], and that
// spell #s begin indexing the array at level 1 (not 0)
//
// This code resource is needed to speed up resting in camp, since the
// players' spell arrays must be searched once per 10-minute interval
//
/*********************************************************/

#pragma off (unreferenced)
ULONG spell_request(LONG argcnt, BYTE *stat, BYTE *cnt, ULONG typ, ULONG num)
#pragma on (unreferenced)
{
   ULONG i,toff;
   BYTE n,h;

   toff = typ ? 110:10;

   stat += toff;
   cnt  += toff;

   for (i=0;i<num;i++)
      {
      n = *stat++;
      h = *cnt++;

      if ((n != -1) && (h < n))
         return 1;
      }

   return 0;
}

/*********************************************************/
//
// Fast list creation for spell menu arrays
//
// Fills list with spells from cnt[typ 0-1][lvl 1-9][# 0-9]
//
// Returns # of spells placed in list (<= max)
//
/*********************************************************/

#pragma off (unreferenced)
ULONG spell_list(LONG argcnt, BYTE *cnt, ULONG typ, ULONG lvl, BYTE *list,
                       ULONG max)
#pragma on (unreferenced)
{
   ULONG i,l,num;
   BYTE m,n,j;

   l = (10 * (lvl-1));
   num = 0;

   cnt = cnt + (typ ? 110:10) + l;
   
   for (i=0;i<10;i++)
      {
      m = i+l;
      n = *cnt++;

      if (n < 0) continue;

      for (j=0;j<n;j++)
         {
         *list++ = m;

         if (++num == max) return num;
         }
      }

   return num;
}

/*=========================================================================*/
/*	MAGIC_FIELD:																				*/
/*																									*/
/*		This routine draws a magic field around the desired character.			*/
/*		(Munged from Eye II)				                                       */
/*																									*/
/*	INPUTS:	pl, red, yel                                                   */
/*	RETURNS:	none																				*/
/*=========================================================================*/

#pragma off (unreferenced)
void magic_field(LONG argcnt, ULONG p, ULONG redfield, ULONG yelfield,
                       LONG sparkle)
#pragma on (unreferenced)
{
	static UBYTE _x[]={8,80};
	static BYTE	 _y[]={2,54,106};
	WORD red,yel,color;
	WORD x,y,lp,save;

   red = 0x23;
   yel = 0x37;

   if (sparkle != -1L)
      {
      red += sparkle;
      yel += sparkle;
      }

	color = red;

	x=_x[p&1];											
	y=_y[p>>1];											

	x+=176;

   if ((redfield)&&(!yelfield))
      {
		GIL2VFX_draw_rect(PAGE2,x,y,x+63,y+49,color);
	   }
   else if ((yelfield)&&(!redfield))
      {
		GIL2VFX_draw_rect(PAGE2,x,y,x+63,y+49,yel);
	   }
   else
      {
		save=x;
		for (lp=0;lp<64;lp+=16)
         {						                        
			x=save+lp;
			if (redfield)
            {								               
				GIL2VFX_draw_line(PAGE2,x,y,x+7,y,color);				
				GIL2VFX_draw_line(PAGE2,x+8,y+49,x+15,y+49,color);	
			   }

			if (yelfield)
            {								               
				GIL2VFX_draw_line(PAGE2,x+8,y,x+15,y,yel);			
				GIL2VFX_draw_line(PAGE2,x,y+49,x+7,y+49,yel);		
			   }

		   }

		x=save;
		save=y;

		for (lp=1;lp<48;lp+=12)
         {						
			y=save+lp-1;
			if (yelfield)
            {								               
				GIL2VFX_draw_line(PAGE2,x,y+1,x,y+6,yel);			
				GIL2VFX_draw_line(PAGE2,x+63,y+7,x+63,y+12,yel);	
			   }

			if (redfield)
            {								               
				GIL2VFX_draw_line(PAGE2,x,y+7,x,y+12,color);			
				GIL2VFX_draw_line(PAGE2,x+63,y+1,x+63,y+6,color);
			   }
		   }
	   }
}

/*=========================================================================*/
/*	COORD_IN_REGION:																			*/
/*																									*/
/*		This routine tests to see if two coordinates passed to it are in the	*/
/*	region passed to it.																		*/
/*																									*/
/*	INPUTS:	WORD testx,WORD testy,WORD left x,WORD top y,WORD right x,		*/
/*				WORD bottom y																	*/
/*	RETURNS:	none																				*/
/*=========================================================================*/
LONG Coord_In_Region(LONG x,LONG y,LONG x1,LONG y1,LONG x2,LONG y2)
{
	if ((x<x1)||(x>x2)) return(0);	/* if it exceeds x bound then false	*/
	if ((y<y1)||(y>y2)) return(0);	/* if it exceeds y bound then false	*/
	return(1);								/* otherwise its true					*/
}

/*********************************************************/
//
// Explosion effect boosted from Eye II
//
/*********************************************************/

#pragma off (unreferenced)
void do_dots(LONG argcnt, LONG view, LONG scrn, LONG exp_x, LONG exp_y,
   LONG scale, LONG power, LONG dots, LONG life, LONG upval, BYTE *colors)
#pragma on (unreferenced)
{
	static WORD _floor[]=
      {
		119,103,79,63,
	   };

	static int i,pixcol,active,cx,cy,px,py,mask;
	WORD *xpos,*ypos,*xvel,*yvel,*color,*colcnt,*colidx,*dotbuffer;
	WORD lside,rside,top,bottom;
	WORD roof,floor,lwall,rwall;

   hide_mouse();

	floor = _floor[scale];
	if (scale) scale--;
	
	dotbuffer = (WORD*) txtbuf;
	roof = 0;
	lwall = -100;
	rwall = 276;

   top = GIL2VFX_get_y1(view);
	bottom = GIL2VFX_get_y2(view);
	lside = GIL2VFX_get_x1(view);
	rside = GIL2VFX_get_x2(view);

	xpos = dotbuffer;
	ypos = dotbuffer+MAXDOTS;
	xvel = dotbuffer+(MAXDOTS*2);
	yvel = dotbuffer+(MAXDOTS*3);
	color = dotbuffer+(MAXDOTS*4);
	colcnt = dotbuffer+(MAXDOTS*5);
	colidx = dotbuffer+(MAXDOTS*6);

	if (dots > MAXDOTS) dots = MAXDOTS;

	cx = exp_x;
	cy = exp_y;

	for (i=0; i<dots; i++)
      {
		xpos[i] = ypos[i] = 0;
		xvel[i] = rnd(0,0,power)-(power>>1);
		yvel[i] = rnd(0,0,power)-(power>>1)-(power>>(8-upval));
		colcnt[i] = rnd(0,(4<<8)/life,(8<<8)/life);
		colidx[i] = scale<<8;
	   }

	active = 2;
	while (active)
      {
		if (active != 2)
			for (i=dots-1; i>=0; i--)
            {
				px = ((xpos[i]>>ACCUR)>>scale)+cx;
				py = ((ypos[i]>>ACCUR)>>scale)+cy;
				if (py > floor) py = floor;

            
				if (Coord_In_Region(px,py,lside,top,rside,bottom))
               GIL2VFX_draw_dot(scrn,px,py,color[i]);
			   }

		active = 0;

		for (i=0; i<dots; i++)
         {
			if (xvel[i]>0)	xvel[i]-=FRICTION;
			else				xvel[i]+=FRICTION;

			xpos[i]+=xvel[i];
			yvel[i]+=GRAVITY;
			ypos[i]+=yvel[i];

			colidx[i]+=colcnt[i];

			px=((xpos[i]>>ACCUR)>>scale)+cx;
			py=((ypos[i]>>ACCUR)>>scale)+cy;

			if (( py >= floor ) || ( py < roof )) yvel[i]=0-(yvel[i]>>1);
			if (( px >= rwall ) || ( px < lwall)) xvel[i]=0-(xvel[i]>>1);
			if (py > floor) py = floor;

			mask = GIL2VFX_read_dot(view,px,py);
			color[i] = GIL2VFX_read_dot(scrn,px,py);

			pixcol = colors[colidx[i]>>8];

			if (pixcol != XCOLOR)
            {
				active = 1;

				if ((mask == XCOLOR) && Coord_In_Region(px,py,lside,top,rside,bottom))
					GIL2VFX_draw_dot(scrn,px,py,pixcol);
			   }
			else colcnt[i] = 0;
		   }

      PollMod();
		VFX_wait_vblank_leading();
		VFX_wait_vblank_leading();
	   }

   show_mouse();
}

/*=========================================================================*/
/*	DO_ICE:																						*/
/*																									*/
/*		Ice special effect for the cone of cold spell.  It is way cool.		*/
/*		(at least according to its original Westwood implementer)            */
/*																									*/
/*	INPUTS:	as below																			*/
/*	RETURNS:	none																				*/
/*																									*/
/*=========================================================================*/

#pragma off (unreferenced)
void do_ice(LONG argcnt, LONG view, LONG scrn, LONG dots, LONG mag, 
   LONG grav, LONG life, BYTE *colors)
#pragma on (unreferenced)
{
	WORD i,pixcol,active,cx,cy,px,py,mask,count;
	WORD *xpos,*ypos,*xvel,*yvel,*color,*colcnt,*colidx,*delay,*dotbuffer;
	WORD m,v,grav78,t;

   hide_mouse();

	dotbuffer = (WORD*) txtbuf;

	xpos   = dotbuffer;
	ypos   = dotbuffer+MAXDOTS;
	xvel   = dotbuffer+(MAXDOTS*2);
	yvel   = dotbuffer+(MAXDOTS*3);
	color  = dotbuffer+(MAXDOTS*4);
	colcnt = dotbuffer+(MAXDOTS*5);
	colidx = dotbuffer+(MAXDOTS*6);
	delay  = dotbuffer+(MAXDOTS*7);

	mag <<= ACCUR;

	if (dots > MAXDOTS) dots = MAXDOTS;

	cx = 88;
	cy = 48;

	for (i=0;i < dots;i++)
      {
		m = rnd(0,mag>>2,mag);
		v = t = 0;

		while (t<m)
         {
			v += grav;
			t += v;
		   }

		switch (_vm->getRandomNumber(3))
         {
			case 0:
				xpos[i] = 1 << (ACCUR-1);
				ypos[i] = t;
				xvel[i] = v;
				yvel[i] = 0;
				break;

			case 1:
				xpos[i] = t;
				ypos[i] = 1 << (ACCUR-1);
				xvel[i] = 0;
				yvel[i] = v;
				break;

			case 2:
				xpos[i] = 1 << (ACCUR-1);
				ypos[i] = -t;
				xvel[i] = v;
				yvel[i] = 0;
				break;

			case 3:
				xpos[i] = -t;
				ypos[i] = 1 << (ACCUR-1);
				xvel[i] = 0;
				yvel[i] = v;
				break;
		   }

		if (_vm->getRandomNumber(1))
         {
			xvel[i] *= -1;
			yvel[i] *= -1;
		   }

		colcnt[i] = rnd(0,(4<<8)/life,(8<<8)/life);
		colidx[i] = 0;
		delay[i] = rnd(0,0,life>>2);
	   }

	active = 2;
	count = 0;

	while (active)
      {
		if (active != 2)
			for (i=dots-1;i >= 0;i--)
            {
				px = (xpos[i]>>ACCUR)+cx;
				py = (ypos[i]>>ACCUR)+cy;
            GIL2VFX_draw_dot(scrn,px,py,color[i]);
			   }

		active = 0;
		grav78 = (grav>>1)+(grav>>2)+(grav>>3);

		for (i=0; i<dots; i++)
         {
			if (delay[i])
            delay[i]--;
			else
            {
				if (xpos[i] > 0)
					if (xvel[i] > 0) xvel[i] -= grav;
					else xvel[i] -= grav78;
				else 
					if (xvel[i] < 0) xvel[i] += grav;
					else xvel[i] += grav78;
				if (ypos[i]>0)
					if (yvel[i] > 0) yvel[i] -= grav;
					else yvel[i] -= grav78;
				else 
					if (yvel[i] < 0) yvel[i] += grav;
					else yvel[i] += grav78;

				xpos[i] += xvel[i];
				ypos[i] += yvel[i];
				colidx[i] += colcnt[i];
			   }

			px = (xpos[i] >> ACCUR) + cx;
			py = (ypos[i] >> ACCUR) + cy;

			if (count < (life>>2))
            mask = 0;
			else
				mask = GIL2VFX_read_dot(view,px,py);

			color[i] = GIL2VFX_read_dot(scrn,px,py);
	  		pixcol = colors[colidx[i] >> 8];

			if (pixcol)
            {
				active = 1;

				if (mask == XCOLOR && !delay[i])
               GIL2VFX_draw_dot(scrn,px,py,pixcol);
			   }
			else colcnt[i] = 0;
		   }

      PollMod();
		VFX_wait_vblank_leading();
		count++;
	   }

   show_mouse();
}

/*********************************************************/
//
// Read savegame directory file into string array
//
/*********************************************************/

void read_save_directory(void)
{
   LONG i;
   TF_class *TF;

   TF = TF_construct(SAVEDIR_FN,TF_READ);

   if (TF == NULL)
      abend(MSG_COSDR);

   for (i=0;i<NUM_SAVEGAMES;i++)
      TF_readln(TF,savegame_dir[i],SAVE_LEN+1);

   TF_destroy(TF);
}

/*********************************************************/
//
// Return the address ('string' override type) of string in
// savegame directory
//
/*********************************************************/

#pragma off (unreferenced)
char *savegame_title(LONG argcnt, ULONG num)
#pragma on (unreferenced)
{
   return savegame_dir[num];
}

/*********************************************************/
//
// Copy the AESOP string 'string' to savegame directory entry
// savegame directory
//
/*********************************************************/

#pragma off (unreferenced)
void set_savegame_title(LONG argcnt, BYTE *string, ULONG num)
#pragma on (unreferenced)
{
   strcpy(savegame_dir[num], (char *)string);
}

/*********************************************************/
//
// Write string array as savegame directory file
//
/*********************************************************/

void write_save_directory(void)
{
   LONG i;
   TF_class *TF;

   TF = TF_construct(SAVEDIR_FN,TF_WRITE);

   if (TF == NULL)
      abend(MSG_COSDW);

   for (i=0;i<NUM_SAVEGAMES;i++)
      if (!TF_writeln(TF,savegame_dir[i]))
         abend(MSG_CWSD);

   TF_destroy(TF);
}

/*********************************************************/
//
// Update savegame filename strings with slot #
//
/*********************************************************/

void set_save_slotnum(ULONG slot)
{
   char num[3];

   sprintf(num,"%02u",slot);

   strncpy((char *)&items_bin[15],num,2);
   strncpy((char *)&items_txt[15], num, 2);
   strncpy((char *)&lvl_bin[15], num, 2);
   strncpy((char *)&lvl_txt[15], num, 2);
}

/*********************************************************/
//
// Update savegame filename strings with level #
//
// Use directory specified in TEMP environment variable for 
// temporary file storage; use current working directory if
// TEMP undefined
//
/*********************************************************/

void set_save_lvlnum(ULONG lvl)
{
   char num[3];

   sprintf(num,"%02u",lvl);

   strncpy((char *)&lvl_bin[12], num, 2);
   strncpy((char *)&lvl_txt[12], num, 2);
   strncpy((char *)&lvl_tmp[12], num, 2);
}

/*********************************************************/
//
// Remove all temporary savegame files
//
// Called before restoring or leaving game
//
/*********************************************************/

void remove_temporary_save_files(void)
{
   LONG lvl;

   for (lvl = 1;lvl <= NUM_LEVELS;lvl++)
      {
      set_save_lvlnum(lvl);
      delete_file(lvl_tmp);
      }
}

/*********************************************************/
//
// Save game at slotnum on level lvlnum
//
// Return 0 if save failed due to lack of disk space or other
// system error
//
/*********************************************************/

#pragma off (unreferenced)
ULONG save_game(LONG argcnt, ULONG slotnum, ULONG lvlnum)
#pragma on (unreferenced)
{
   ULONG lvl;

   if (slotnum == 0)
      abend(MSG_IASS0);

   set_save_slotnum(slotnum);
   set_save_lvlnum(lvlnum);

   if (!save_range(items_bin,SAVETYPE,FIRST_ITEM,LAST_ITEM))
      return 0;

   if (!save_range(lvl_bin,SAVETYPE,FIRST_LVL_OBJ,LAST_LVL_OBJ))
      return 0;

   for (lvl = 1;lvl <= NUM_LEVELS;lvl++)
      {
      if (lvl == lvlnum) continue;

      set_save_lvlnum(lvl);

      if (copy_file(lvl_tmp,lvl_bin) == -1)
         return 0;
      }

   return 1;
}

/*********************************************************/
//
// Save current level context to temporary file
//
// This function assumes that the non-current-level tempfiles
// are already present and valid
//
// Used before spawning a process via the launch() function handler
//
/*********************************************************/

#pragma off (unreferenced)
void suspend_game(LONG argcnt, ULONG cur_lvl)
#pragma on (unreferenced)
{
   if (!save_range(itm_tmp,SAVETYPE,FIRST_ITEM,LAST_ITEM))
      abend(MSG_CNSI);

   set_save_lvlnum(cur_lvl);

   if (!save_range(lvl_tmp,SAVETYPE,FIRST_LVL_OBJ,LAST_LVL_OBJ))
      abend(MSG_CNSCL);
}

/*********************************************************/
//
// Resume current level context from temporary file
//
// This function assumes that the non-current-level tempfiles
// are already present and valid
//
// Used when resuming game after launched process terminates
//
/*********************************************************/

#pragma off (unreferenced)
void resume_level(LONG argcnt, ULONG cur_lvl)
#pragma on (unreferenced)
{
   set_save_lvlnum(cur_lvl);

   restore_range(lvl_tmp,FIRST_LVL_OBJ,LAST_LVL_OBJ,1);
}

/*********************************************************/
//
// Resume global items (including kernel, PCs, etc.)
//
// Release any windows owned by entities
// Cancel notification requests issued by all entities
// Restore items from temporary context file
//
// This function must be called before restoring any level-specific
// objects
//
// Failure results in unrecoverable system error; function will not
// return unless successful
//
/*********************************************************/

#pragma off (unreferenced)
void resume_items(LONG argcnt, ULONG first, ULONG last, ULONG restoring)
#pragma on (unreferenced)
{
   release_owned_windows(-1);
   cancel_entity_requests(-1);

   restore_range(itm_tmp,first,last,restoring);
}

/*********************************************************/
//
// Leave level old_lvl; enter level new_lvl
//
// Save level objects to temporary file; restore level objects 
// from previously saved temporary file
//
// Failure results in unrecoverable system error; function will not
// return unless successful
//
/*********************************************************/

#pragma off (unreferenced)
void change_level(LONG argcnt, ULONG old_lvl, ULONG new_lvl)
#pragma on (unreferenced)
{
   set_save_lvlnum(old_lvl);

   if (!save_range(lvl_tmp,SAVETYPE,FIRST_LVL_OBJ,LAST_LVL_OBJ))
      abend(MSG_CNSLT);

   set_save_lvlnum(new_lvl);

   restore_range(lvl_tmp,FIRST_LVL_OBJ,LAST_LVL_OBJ,1);
}

/*********************************************************/
//
// Restore global items (including kernel, PCs, etc.)
//
// Release any windows owned by entities
// Cancel notification requests issued by all entities
// Restore items from context file
//
// This function must be called before restoring any level-specific
// objects
//
// Failure results in unrecoverable system error; function will not
// return unless successful
//
/*********************************************************/

#pragma off (unreferenced)
void restore_items(LONG argcnt, ULONG slotnum)
#pragma on (unreferenced)
{
   set_save_slotnum(slotnum);

   release_owned_windows(-1);
   cancel_entity_requests(-1);

//   aprint(0,"restore_range(%s, %d, %d, 1)\n",items_bin,FIRST_ITEM,LAST_ITEM);
   restore_range(items_bin,FIRST_ITEM,LAST_ITEM,1);
}

/*********************************************************/
//
// Restore level-specific objects (features & NPCs)
//
// Restore current level objects from context file
// Copy non-current level context files to temporary files
//
// Failure results in unrecoverable system error; function will not
// return unless successful
//
/*********************************************************/

#pragma off (unreferenced)
void restore_level_objects(LONG argcnt, ULONG slotnum, ULONG lvlnum)
#pragma on (unreferenced)
{
   ULONG lvl;

   set_save_slotnum(slotnum);
   set_save_lvlnum(lvlnum);

   restore_range(lvl_bin,FIRST_LVL_OBJ,LAST_LVL_OBJ,1);

   for (lvl = 1;lvl <= NUM_LEVELS;lvl++)
      {
      if (lvl == lvlnum) continue;

      set_save_lvlnum(lvl);

      if (copy_file(lvl_bin,lvl_tmp) == -1)
         abend(MSG_CNCLT);
      }
}

/*********************************************************/
//
// Read initial (slot 0) items (including kernel, PCs, etc.) into
// object list
//
// Used during transfer/create process
//
// Failure results in unrecoverable system error; function will not
// return unless successful
//
/*********************************************************/

void read_initial_items(void)
{
   set_save_slotnum(0);

   restore_range(items_bin,FIRST_ITEM,LAST_ITEM,0);
}

/*********************************************************/
//
// Write initial temporary files based on slot 0 binaries
//
// Used during transfer/create process
//
// Failure results in unrecoverable system error; function will not
// return unless successful
//
/*********************************************************/

void write_initial_tempfiles(void)
{
   ULONG lvl;

   if (!save_range(itm_tmp,SAVETYPE /* SF_TXT */,FIRST_ITEM,LAST_ITEM))
      abend(MSG_CNSI);

   set_save_slotnum(0);

   for (lvl = 1;lvl <= NUM_LEVELS;lvl++)
      {
      set_save_lvlnum(lvl);

      if (copy_file(lvl_bin,lvl_tmp) == -1)
         abend(MSG_CNCLT);
      }
}

/*********************************************************/
//
// Translate level-specific objects from text to binary format
//
/*********************************************************/

void create_initial_binary_files(void)
{
   ULONG lvl;

   set_save_slotnum(0);

//   if (copy_file(items_txt,items_bin) == -1)
//      abend(MSG_CNTI);

   if (file_time(items_txt) >= file_time(items_bin))
      {
      debugN("Translating %s to %s\n",items_txt,items_bin);
	  translate_file((const char *)items_txt, (const char *)items_bin, FIRST_ITEM, LAST_ITEM);
      }

   for (lvl = 1;lvl <= NUM_LEVELS;lvl++)
      {
      set_save_lvlnum(lvl);

      if (file_time(lvl_txt) < file_time(lvl_bin))
         continue;

	  debugN("Translating %s to %s\n", lvl_txt, lvl_bin);

	  translate_file((const char *)lvl_txt, (const char *)lvl_bin, FIRST_LVL_OBJ, LAST_LVL_OBJ);
      }
}

/*********************************************************/
//
// Launch a secondary process
//
// Application is responsible for preserving its state and arranging
// for its resurrection
//
// Failure results in unrecoverable system error; function will not
// return under any circumstances
//
/*********************************************************/

#pragma off (unreferenced)
void launch(LONG argcnt, const char *dirname, const char *prgname, const char *argn1,
	const char *argn2)
#pragma on (unreferenced)
{
	struct stag {
      char  prg[128];
      char arg1[128];
      char arg2[128];
   };

   stag *s;
   char dir[128];

   s = *(stag **) 0x4fa;

   strcpy(dir,dirname);
   strcpy(s->prg,prgname);

   if (argn1 != NULL)
      strcpy(s->arg1,argn1);
   else
      s->arg1[0] = 0;

   if (argn1 != NULL)
      strcpy(s->arg2,argn2);
   else
      s->arg2[0] = 0;

   RT_execute(bootstrap,MSG_DESTROY,-1U);

   shutdown_sound();
   RTR_destroy(RTR,RTR_FREEBASE);

   locate(0,51);
   //chdir(dir);
   //exit(127);
}

} // End of namespace Aesop
