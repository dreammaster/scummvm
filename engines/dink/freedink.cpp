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

#define NAME "Dink Smallwood v1.08"
#define TITLE "Dink Smallwood v1.08"

#include "dink/var.h"
#include "dink/directdraw/ddraw.h"

namespace Dink {

int fps_average;

bool initFail( HWND hwnd, char mess[200] );
void move(int u, int amount, char kind, char kindy);
void draw_box(Common::Rect box, int color);
void run_through_tag_list_push(int h);
void random_blood(int mx, int my, int h);
int check_if_move_is_legal(int u);
void change_dir_to_diag( int *dir);
int hurt_thing(int h, int damage, int special);



int but_timer = 0;
int water_timer;
bool fire_forward;
int fire_flip;
//const NUM_SOUND_EFFECTS = 10;
int fps_show = 0;

//idirectsound
LPDIRECTDRAWCLIPPER lpClipper;

bool InitSound( HWND );
bool DestroySound( void );
bool DSDisable( void );
bool DSEnable( HWND );


int drawthistime = true;
int x = 640;
int y = 480;
Common::Rect                rc;
int winoffset = 25;
int winoffsetx = 5;

int cx;
int cy;
int speed;

/*
* restoreAll
*
* restore all lost objects
*/



/*
* This function is called if the initialization function fails
*/


void text_draw(int h, HDC hdc)
{

    char crap[200];
    char *cr;
    Common::Rect rcRect;
    int color = 0;

    if (spr[h].damage == -1)
    {
        //redink1 fix for : and '%deee bugs?
        strcpy(crap, spr[h].text);
        //sprintf(crap, "%s", spr[h].text);
        cr = &crap[0];
        color = 14;
        while( cr[0] == '`') 
        {
            //color code at top
            if (cr[1] == '#') color = 13;
            if (cr[1] == '1') color = 1;
            if (cr[1] == '2') color = 2;
            if (cr[1] == '3') color = 3;
            if (cr[1] == '5') color = 5;
            if (cr[1] == '6') color = 6;
            if (cr[1] == '7') color = 7;
            if (cr[1] == '8') color = 8;
            if (cr[1] == '9') color = 9;
            if (cr[1] == '0') color = 10;
            if (cr[1] == '$') color = 14;
            if (cr[1] == '%') color = 15;
            //redink1 support for additional colors
            if (cr[1] == '@') color = 12;
            if (cr[1] == '!') color = 11;

            if (cr[1] == '4') color = 4;
            cr = &cr[2];
        }

        //Msg("Final is %s.",cr);
        if (spr[h].owner == 1000)
        {
            SetRect(&rcRect,spr[h].x,spr[h].y,spr[h].x+620,spr[h].y+400);
        } else
        {

            SetRect(&rcRect,spr[h].x,spr[h].y,spr[h].x+150,spr[h].y+150);

            if (spr[h].x+150 > 620)
                OffsetRect(&rcRect, ((spr[h].x+150)-620) - (((spr[h].x+150)-620) * 2), 0);



        }

    } else
    {


        sprintf(crap, "%d", spr[h].damage);
        cr = &crap[0];
        if (spr[h].brain_parm == 5000)
            color = 14;


        if (spr[h].y < 0) spr[h].y = 0;
        SetRect(&rcRect,spr[h].x,spr[h].y,spr[h].x+50 ,spr[h].y+50);


    }       

    if (truecolor && (bFadedDown || process_downcycle))
        color = 15;

    SetTextColor(hdc,RGB(8,14,21));
    if (spr[h].owner == 1200)
    {
        //this text has no sprite, and doesn't want to be centered.
        DrawText(hdc,cr,strlen(cr),&rcRect,DT_WORDBREAK);

        OffsetRect(&rcRect,-2,0);
        DrawText(hdc,cr,strlen(cr),&rcRect,DT_WORDBREAK);

        OffsetRect(&rcRect,1,1);
        DrawText(hdc,cr,strlen(cr),&rcRect,DT_WORDBREAK);
        OffsetRect(&rcRect,0,-2);
        DrawText(hdc,cr,strlen(cr),&rcRect,DT_WORDBREAK);


    } else
    {

        DrawText(hdc,cr,strlen(cr),&rcRect,DT_CENTER | DT_WORDBREAK);
        OffsetRect(&rcRect,-2,0);
        DrawText(hdc,cr,strlen(cr),&rcRect,DT_CENTER | DT_WORDBREAK);

        OffsetRect(&rcRect,1,1);
        DrawText(hdc,cr,strlen(cr),&rcRect,DT_CENTER | DT_WORDBREAK);
        OffsetRect(&rcRect,0,-2);
        DrawText(hdc,cr,strlen(cr),&rcRect,DT_CENTER | DT_WORDBREAK);

    }

    OffsetRect(&rcRect,0,1);

    //redink1 support for custom colors
    if (color >= 1 && color <= 15)
    {
        SetTextColor(hdc,RGB(font_colors[color].red,font_colors[color].green,font_colors[color].blue));
    }
    else
    {
        SetTextColor(hdc,RGB(255,255,255));
    }

    if (spr[h].owner == 1200)

        DrawText(hdc,cr,strlen(cr),&rcRect,DT_WORDBREAK);   
    else    
        DrawText(hdc,cr,strlen(cr),&rcRect,DT_CENTER | DT_WORDBREAK);   


}

HRESULT restoreAll() {
    HRESULT ddrval;
    char crap[100];
    char crap1[10];

    ddrval = lpDDSPrimary->Restore();
    if( ddrval == DD_OK )
    {

        ddrval = lpDDSTwo->Restore();

        ddrval = lpDDSTrick->Restore();
        ddrval = lpDDSTrick2->Restore();


        /*for (int h=1; h < tile_screens; h++)
        {
        ddrval = tiles[h]->Restore();

        if( ddrval == DD_OK )
        {
        if (h < 10) strcpy(crap1,"0"); else strcpy(crap1, "");
        sprintf(crap, "TILES\\TS%s%d.BMP",crap1,h);

        //      sprintf(crap, "TS%d.BMP",h);
        DDReLoadBitmap(tiles[h], crap); 
        Msg("Loading tile %d",h); 
        }
        }
        */
        Msg("Done with reloading tile set."); 
        //reload_batch();
        Msg("Have all sprites."); 

        //draw_map();
        Msg("Drew map."); 
        if (mode > 2)
        {
            fill_screen(0);
            draw_map_game_background();
            if ( *pupdate_status == 1 )
            {
                draw_status_all();
            }
        }
    }

    return ddrval;


} /* restoreAll */



void get_last_sprite(void)
{

    for (int i = max_sprites_at_once - 1; i > 2; i--)
    {

        if (spr[i].active)
        {
            last_sprite_created = i;
            //   Msg("last sprite created is %d.", i);
            return;
        }
    }
}













/*
* DestroySound
*
* Undoes everything that was done in a InitSound call
*/
bool DestroySound( void )
{
    DWORD       idxKill;

    for( idxKill = 0; idxKill < max_sounds; idxKill++ )
    {
        SoundDestroyEffect( idxKill );
    }

    DSDisable();
    return true;

} /* DestroySound */

/*
* SoundDestroyEffect
*
* Frees up resources associated with a sound effect
*/
bool SoundDestroyEffect( int sound )
{
    if(ssound[sound].sound)
    {
        ssound[sound].sound->Release();
        ssound[sound].sound = NULL;
    }
    return true;

} /* SoundDestryEffect */

/*
* SoundLoadEffect
*
* Initializes a sound effect by loading the WAV file from a resource
*/
/*bool SoundLoadEffect( int sound)
{
if (lpDS && lpSoundEffects[sound] == NULL && *szSoundEffects[sound])
{
//
//  use DSLoadSoundBuffer (in ..\misc\dsutil.c) to load
//  a sound from a resource.
//
lpSoundEffects[sound] = DSLoadSoundBuffer(lpDS, szSoundEffects[sound]);
if (lpSoundEffects[sound] == NULL) Msg("Damn, got a null one for sound %d.",sound);
}

return lpSoundEffects[sound] != NULL;

} 

*/
/* SoundLoadEffect */




/*
* SoundPlayEffect
*
* Plays the sound effect specified.  
* Returns true if succeeded.
*/



/*void draw_used( void)
{
for (int x=0; x<768; x++)
{

if (map.loc[x+1] == 0)  
lpDDSTwo->BltFast( (x) * 20 - ((x / 32) * 640), (x / 32) * 20, k[6].k,
&k[6].box, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

if (map.loc[x+1] > 0)   
lpDDSTwo->BltFast( (x) * 20 - ((x / 32) * 640), (x / 32) * 20, k[7].k,
&k[7].box, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT );

}

}
*/

bool keypressed( void )

{
    for (int x=0; x<256; x++)
    {
        if (GetKeyboard(x))
        {
            return(true);
        }
    }

    return(false);
}

/*
* updateFrame
* 
* Decide what needs to be blitted next, wait for flip to complete,
* then flip the buffers.
*/






void check_joystick(void)
{

    HRESULT ddrval;
    int total;
    //memset(&sjoy,0,sizeof(sjoy));

    for (int e2=1; e2 <=10; e2++) 
    {
        sjoy.joybit[e2] = false;

    }
    sjoy.right = false;
    sjoy.left = false;
    sjoy.up = false;
    sjoy.down = false;

    sjoy.rightd = false;
    sjoy.leftd = false;
    sjoy.upd = false;
    sjoy.downd = false;

    //joystick = false;
    if (joystick)
    {
        memset(&jinfo,0,sizeof(JOYINFOEX));
        jinfo.dwSize=sizeof(JOYINFOEX);
        jinfo.dwFlags=JOY_RETURNALL;
        ddrval = joyGetPosEx(JOYSTICKID1,&jinfo);
        if (ddrval == JOYERR_UNPLUGGED) goto pass;

        total = jinfo.dwButtons;

        if ((total - 512) >= 0)
        {
            sjoy.joybit[play.button[10]] = true;
            total = total - 512;
        }

        if ((total - 256) >= 0)
        {
            sjoy.joybit[play.button[9]] = true;
            total = total - 256;
        }

        if ((total - 128) >= 0)
        {
            sjoy.joybit[play.button[8]] = true;
            total = total - 128;
        }

        if ((total - 64) >= 0)
        {
            sjoy.joybit[play.button[7]] = true;
            total = total - 64;
        }

        if ((total - 32) >= 0)
        {
            sjoy.joybit[play.button[6]] = true;
            total = total - 32;
        }

        if ((total - 16) >= 0)
        {
            sjoy.joybit[play.button[5]] = true;
            total = total - 16;
        }

        if ((total - 8) >= 0)
        {
            sjoy.joybit[play.button[4]] = true;
            total = total - 8;
        }

        if ((total - 4) >= 0)
        {
            sjoy.joybit[play.button[3]] = true;
            total = total - 4;
        }

        if ((total - 2) >= 0)
        {
            sjoy.joybit[play.button[2]] = true;
            total = total - 2;
        }

        if ((total - 1) >= 0)
        {
            sjoy.joybit[play.button[1]] = true;
            total = total - 1;
        }


        if (jinfo.dwXpos > 40000) sjoy.right = true;
        if (jinfo.dwXpos < 25000) sjoy.left = true;
        if (jinfo.dwYpos > 40000) sjoy.down = true;
        if (jinfo.dwYpos < 17000) sjoy.up = true;

    }



pass:
    //redink1 Only accept keyboard input when console is not active.
    if ( mConsoleActive == false )
    {
        if (GetKeyboard(17)) sjoy.joybit[1] = true;
        if (GetKeyboard(32)) sjoy.joybit[2] = true;

        if (GetKeyboard(16)) sjoy.joybit[3] = true;
        if (GetKeyboard(13)) sjoy.joybit[4] = true;
        if (GetKeyboard(27)) sjoy.joybit[5] = true;
        if (GetKeyboard(54)) sjoy.joybit[6] = true;
        if (GetKeyboard(77)) sjoy.joybit[6] = true;
        if (GetKeyboard(55)) sjoy.joybit[7] = true;
    }

    for (int x5=1; x5 <=10; x5++) sjoy.button[x5] = false; 


    for (int x=1; x <=10; x++)

    {
        if (sjoy.joybit[x])
        {
            if (sjoy.letgo[x] == true) 
            {
                sjoy.button[x] = true;
                sjoy.letgo[x] = false;
            }

        }
    }

    //redink1 Only accept keyboard input when console is not active.
    if ( mConsoleActive == false )
    {
        if (GetKeyboard(39)) sjoy.right = true;
        if (GetKeyboard(37)) sjoy.left = true;
        if (GetKeyboard(40)) sjoy.down = true;
        if (GetKeyboard(38)) sjoy.up = true;
    }

    for (int x2=1; x2 <=10; x2++) 
    {
        if (sjoy.joybit[x2])  sjoy.letgo[x2] = false; else sjoy.letgo[x2] = true;

    }

    if (sjoy.right) if (sjoy.rightold == true)
    {
        sjoy.rightd = true;
        sjoy.rightold = false;
    }

    if (sjoy.right) sjoy.rightold = false; else sjoy.rightold = true;


    if (sjoy.left) if (sjoy.leftold == true)
    {
        sjoy.leftd = true;
        sjoy.leftold = false;
    }

    if (sjoy.left) sjoy.leftold = false; else sjoy.leftold = true;


    if (sjoy.up) if (sjoy.upold == true)
    {
        sjoy.upd = true;
        sjoy.upold = false;
    }

    if (sjoy.up) sjoy.upold = false; else sjoy.upold = true;


    if (sjoy.down) if (sjoy.downold == true)
    {
        sjoy.downd = true;
        sjoy.downold = false;
    }

    if (sjoy.down) sjoy.downold = false; else sjoy.downold = true;


    if (wait.active)
    {

        //check for dirs

        if (sjoy.rightd) wait.button = 16;
        if (sjoy.leftd) wait.button = 14;
        if (sjoy.upd) wait.button = 18;
        if (sjoy.downd) wait.button = 12;

        sjoy.rightd = false;
        sjoy.downd = false;
        sjoy.upd = false;
        sjoy.leftd = false;

        //check buttons
        for (int ll=1; ll <= 10; ll++)
        {
            if (sjoy.button[ll])
            {
                //button was pressed
                wait.button = ll;

            }
            sjoy.button[ll] = false;

        }

        if (wait.button != 0)
        {
            *presult = wait.button;
            wait.active = false;
            run_script(wait.script);
        }





    }


}




// ********* CHECK TO SEE IF THIS CORD IS ON A HARD SPOT *********
bool not_in_this_base(int seq, int base)
{

    int realbase = (seq / 10) * 10;


    if (realbase != base)
    {


        return(true); 
    }
    else
    {
        return(false);
    }
}

bool in_this_base(int seq, int base)
{

    int realbase = (seq / 10) * 10;
    if (realbase == base)
    {

        //  Msg("true - Ok, realbase is %d, compared to the base, which is %d.", realbase, base);
        return(true); 
    }
    else
    {
        //  Msg("false - Ok, realbase is %d, compared to the base, which is %d.", realbase, base);

        return(false);
    }
}


void automove (int j)
{

    char kindx,kindy;
    int speedx = 0;
    int speedy = 0;




    if (spr[j].mx != 0)
    { 
        if (spr[j].mx < 0)
            kindx = '-'; else kindx = '+';
        if (kindx == '-') speedx = (spr[j].mx - (spr[j].mx * 2)); else
            speedx = spr[j].mx;
    } else kindx = '0';

    if (spr[j].my != 0)
    { 
        if (spr[j].my < 0)
            kindy = '-'; else kindy = '+';
        if (kindy == '-') speedy = (spr[j].my - (spr[j].my * 2)); else
            speedy = spr[j].my;

    } else kindy = '0';

    int speed = speedx;
    if (speedy > speedx) speed = speedy;
    if (speed > 0)
        move(j,speed,kindx,kindy);
    //move(j, 1, '+','+'); 

}


int autoreverse(int j)
{
    //Msg("reversing die %d",spr[j].dir);
    int r = ((rand() % 2)+1);   
    if ( (spr[j].dir == 1) || (spr[j].dir == 2) ) 
    {
        if (r == 1)
            return(8);
        if (r == 2)
            return(6);

    }

    if ( (spr[j].dir == 3) || (spr[j].dir == 6) ) 
    {
        if (r == 1)
            return(2);
        if (r == 2)

            return(4);

    }

    if ( (spr[j].dir == 9) || (spr[j].dir == 8) ) 
    {
        if (r == 1)
            return(2);
        if (r == 2)

            return(6);


    }

    if ( (spr[j].dir == 7) || (spr[j].dir == 4) ) 
    {
        if (r == 1)
            return(8);
        if (r == 2)
            return(6);

    }

    return(0);
}


int autoreverse_diag(int j)
{
    if (spr[j].dir == 0) spr[j].dir = 7;
    int r = ((rand() % 2)+1);   

    if ( (spr[j].dir == 1) || (spr[j].dir == 3) ) 
    {

        if (r == 1)
            return(9);
        if (r == 2)
            return(7);
    }

    if ( (spr[j].dir == 3) || (spr[j].dir == 6) ) 
    {
        if (r == 1)
            return(7);
        if (r == 2)
            return(1);

    }

    if ( (spr[j].dir == 9) || (spr[j].dir == 8) ) 
    {
        if (r == 1)
            return(1);
        if (r == 2)
            return(7);
    }

    if ( (spr[j].dir == 7) || (spr[j].dir == 4) ) 
    {
        if (r == 1)
            return(3);
        if (r == 2)
            return(9);

    }

    Msg("Auto Reverse Diag was sent a dir %d sprite, base %d walk.",spr[j].dir, spr[j].base_walk);
    return(0);
}

void draw_damage(int h)
{

    int crap2 = add_sprite(spr[h].x,spr[h].y,8,0,0);

    spr[crap2].y -= k[seq[spr[h].pseq].frame[spr[h].pframe]].yoffset;
    spr[crap2].x -= k[seq[spr[h].pseq].frame[spr[h].pframe]].xoffset;
    spr[crap2].y -= k[seq[spr[h].pseq].frame[spr[h].pframe]].box.bottom / 3;
    spr[crap2].x += k[seq[spr[h].pseq].frame[spr[h].pframe]].box.right / 5;

    spr[crap2].speed = 1;
    spr[crap2].hard = 1;
    spr[crap2].brain_parm = h;  
    spr[crap2].my = -1;
    spr[crap2].kill = 1000;
    spr[crap2].dir = 8;
    spr[crap2].damage = spr[h].damage;

}


void add_kill_sprite(int h)
{
    if ( (spr[h].dir > 9) || (spr[h].dir < 1) )
    {
        Msg("Error:  Changing sprites dir from %d (!?) to 3.", spr[h].dir);
        spr[h].dir = 3;

    }


    int dir = spr[h].dir;
    int base = spr[h].base_die;

    //Msg("Base die is %d", base);
    if (base == -1) 
    {

        if (seq[spr[h].base_walk+5].active == true)
        {
            add_exp(spr[h].exp, h);

            int crap2 = add_sprite(spr[h].x,spr[h].y,5,spr[h].base_walk +5,1);
            spr[crap2].speed = 0;
            spr[crap2].seq = spr[h].base_walk + 5;
            //redink1 added this so corpses are the same size
            spr[crap2].size = spr[h].size;
            return;
        } else
        {
            dir = 0;
            base = 164;

        }
    }



    if (seq[base+dir].active == false)
    {  

        if (dir == 1) dir = 9;
        else if (dir == 3) dir = 7;         
        else if (dir == 7) dir = 3;         
        else if (dir == 9) dir = 1;         

        else if (dir == 4) dir = 6;         
        else if (dir == 6) dir = 4;         
        else if (dir == 8) dir = 2;         
        else if (dir == 2) dir = 8;         


    }
    if (seq[base+dir].active == false)

    {
        Msg("Can't make a death sprite for dir %d!", base+dir);
    }



    int crap2 = add_sprite(spr[h].x,spr[h].y,5,base +dir,1);
    spr[crap2].speed = 0;
    spr[crap2].base_walk = 0;
    spr[crap2].seq = base + dir;

    if (base == 164) spr[crap2].brain = 7;

    spr[crap2].size = spr[h].size;

    add_exp(spr[h].exp, h);

}


void done_moving(int h)
{

    spr[h].move_active = false;

    spr[h].move_nohard = false;

    if (spr[h].move_script > 0)
    {
        //  Msg("mover running script %d..", spr[h].move_script);
        run_script(spr[h].move_script);
    }



}

int get_distance_and_dir(int h, int h1, int *dir)
{
    if ( smooth_follow )
    {
        unsigned int x_diff( abs( spr[h].x - spr[h1].x ) );
        unsigned int y_diff( abs( spr[h].y - spr[h1].y ) );
        if ( spr[h].x < spr[h1].x )
        {
            if ( spr[h].y < spr[h1].y )
            {
                // 6, 3, 2
                if ( y_diff * 4 < x_diff )
                {
                    *dir = 6;
                }
                else if ( x_diff * 4 < y_diff )
                {
                    *dir = 2;
                }
                else
                {
                    *dir = 3;
                }
            }
            else if ( spr[h].y > spr[h1].y )
            {
                // 4, 9, 8
                if ( y_diff * 4 < x_diff )
                {
                    *dir = 6;
                }
                else if ( x_diff * 4 < y_diff )
                {
                    *dir = 8;
                }
                else
                {
                    *dir = 9;
                }
            }
            else
            {
                *dir = 6;
            }
        }
        else if ( spr[h].x > spr[h1].x )
        {
            if ( spr[h].y < spr[h1].y )
            {
                // 4, 1, 2
                if ( y_diff * 4 < x_diff )
                {
                    *dir = 4;
                }
                else if ( x_diff * 4 < y_diff )
                {
                    *dir = 2;
                }
                else
                {
                    *dir = 1;
                }
            }
            else if ( spr[h].y > spr[h1].y )
            {
                // 4, 7, 8
                if ( y_diff * 4 < x_diff )
                {
                    *dir = 4;
                }
                else if ( x_diff * 4 < y_diff )
                {
                    *dir = 8;
                }
                else
                {
                    *dir = 7;
                }
            }
            else
            {
                *dir = 4;
            }
        }
        else
        {
            if ( spr[h].y < spr[h1].y )
            {
                *dir = 2;
            }
            else if ( spr[h].y > spr[h1].y )
            {
                *dir = 8;
            }
        }
        return max( x_diff, y_diff );
    }

    int distancex = 5000;
    int distancey = 5000;
    int dirx = *dir;
    int diry = *dir;
    if (spr[h].x > spr[h1].x) if ((spr[h].x - spr[h1].x) < distancex)
    {
        distancex = (spr[h].x - spr[h1].x);
        dirx = 4;
    }

    if (spr[h].x <= spr[h1].x) if ((spr[h1].x - spr[h].x) < distancex)
    {
        distancex = (spr[h1].x - spr[h].x);
        dirx = 6;
    }
    if (spr[h].y > spr[h1].y) if ((spr[h].y - spr[h1].y) < distancey)
    {
        distancey = (spr[h].y - spr[h1].y);
        diry = 8;

    }
    if (spr[h].y <= spr[h1].y) if ((spr[h1].y - spr[h].y) < distancey)
    {
        distancey = (spr[h1].y - spr[h].y);
        diry = 2;
    }
    if (distancex > distancey)
    {

        *dir = dirx;
        return(distancex);
    }
    else 
    {
        *dir = diry;
        return(distancey);
    }


}

void process_follow(int h)
{
    int hx, hy;

    if (spr[h].follow > 299)
    {
        Msg("ERROR:  Sprite %d cannot 'follow' sprite %d??",h,spr[h].follow);
        return;
    }

    if (spr[spr[h].follow].active == false)
    {
        Msg("Killing follow");
        spr[h].follow = 0;
        return;
    }

    hx = spr[spr[h].follow].x;
    hy = spr[spr[h].follow].y;

    int dir;
    int distance = get_distance_and_dir(h, spr[h].follow, &dir);

    if (distance < 40) return;

    changedir(dir,h,spr[h].base_walk);
    automove(h);


}


void process_target(int h)
{
    int hx, hy;

    if (spr[h].target > 299)
    {
        Msg("ERROR:  Sprite %d cannot 'target' sprite %d??",h,spr[h].follow);
        return;
    }

    if (spr[spr[h].target].active == false)
    {
        Msg("Killing target");
        spr[h].target = 0;
        return;
    }

    hx = spr[spr[h].target].x;
    hy = spr[spr[h].target].y;

    int dir;
    int distance = get_distance_and_dir(h, spr[h].target, &dir);

    if (distance < spr[h].distance) return;

    changedir(dir,h,spr[h].base_walk);

    automove(h);


}


bool check_for_kill_script(int i)
{


    if (spr[i].script > 0)
    {
        //if (  (spr[i].brain == 0) | (spr[i].brain == 5) | (spr[i].brain == 6) | (spr[i].brain == 7))

        if (locate(spr[i].script, "DIE")) run_script(spr[i].script);

        return(true);   
    }

    return(false);
}

bool check_for_duck_script(int i)
{


    if (spr[i].script > 0)
    {
        //if (  (spr[i].brain == 0) | (spr[i].brain == 5) | (spr[i].brain == 6) | (spr[i].brain == 7))

        if (locate(spr[i].script, "DUCKDIE")) run_script(spr[i].script);

        return(true);   
    }

    return(false);
}



void process_move(int h)
{

    //  Msg("Proccesing sprite %d, dir %d (script is %d)", h, spr[h].dir, spr[h].move_script);



    if ((spr[h].move_dir == 4) | (spr[h].move_dir == 1) | (spr[h].move_dir == 7) )
    {
        if (spr[h].x <= spr[h].move_num)
        {
            //done moving
            done_moving(h);
            return;
        }
        changedir(spr[h].move_dir,h,spr[h].base_walk);      
        automove(h);    
    }

    if ( (spr[h].move_dir == 6) | (spr[h].move_dir == 9) | (spr[h].move_dir == 3))
    {
        if (spr[h].x >= spr[h].move_num)
        {
            //done moving
            done_moving(h);
            return;
        }
        changedir(spr[h].move_dir,h,spr[h].base_walk);      
        automove(h);    
    }


    if (spr[h].move_dir == 2)
    {
        if (spr[h].y >= spr[h].move_num)
        {
            //done moving
            done_moving(h);
            return;
        }
        changedir(spr[h].move_dir,h,spr[h].base_walk);      
        automove(h);    
    }


    if (spr[h].move_dir == 8)
    {
        if (spr[h].y <= spr[h].move_num)
        {
            //done moving
            done_moving(h);
            return;
        }
        changedir(spr[h].move_dir,h,spr[h].base_walk);      
        automove(h);    
    }


}

void duck_brain(int h)
{
    int hold;


start:


    if (   (spr[h].damage > 0) && (in_this_base(spr[h].pseq, 110)  ) )
    {

        check_for_duck_script(h);

        //hit a dead duck
        int crap2 = add_sprite(spr[h].x,spr[h].y,7,164,1);
        spr[crap2].speed = 0;
        spr[crap2].base_walk = 0;
        spr[crap2].seq = 164;
        draw_damage(h);
        spr[h].damage = 0;
        add_exp(spr[h].exp, h);

        kill_sprite_all(h);

        return;
    }


    if (   (spr[h].damage > 0) && (in_this_base(spr[h].pseq, spr[h].base_walk)  ) )
    {
        //SoundPlayEffect( 1,3000, 800 );  
        draw_damage(h);
        add_exp(spr[h].exp, h);
        spr[h].damage = 0;

        //lets kill the duck here, ha.
        check_for_kill_script(h);
        spr[h].follow = 0;
        int crap = add_sprite(spr[h].x,spr[h].y,5,1,1);
        spr[crap].speed = 0;
        spr[crap].base_walk = 0;
        spr[crap].size = spr[h].size;                       
        spr[crap].speed =  ((rand() % 3)+1);


        spr[h].base_walk = 110;
        spr[h].speed = 1;
        spr[h].timer = 0;
        spr[h].wait = 0;
        spr[h].frame = 0;

        if (spr[h].dir == 0) spr[h].dir = 1;
        if (spr[h].dir == 4) spr[h].dir = 7;
        if (spr[h].dir == 6) spr[h].dir = 3;

        changedir(spr[h].dir,h,spr[h].base_walk);
        spr[crap].dir = spr[h].dir;
        spr[crap].base_walk = 120;
        changedir(spr[crap].dir,crap,spr[crap].base_walk);


        automove(h);
        return;
    }


    if (spr[h].move_active)
    {
        process_move(h);
        return;
    }

    if (spr[h].freeze)
    {
        return;
    }


    if (spr[h].follow > 0)
    {
        process_follow(h);
        return;
    }



    if (spr[h].base_walk == 110)
    {
        if ( (rand() % 100)+1 == 1)
            random_blood(spr[h].x, spr[h].y-18, h);
        goto walk;
    }





    if (spr[h].seq == 0 ) 
    {

        if (((rand() % 12)+1) == 1 )
        {  
            hold = ((rand() % 9)+1);

            if ((hold != 2) && (hold != 8) && (hold != 5))
            {

                //Msg("random dir change started.. %d", hold);
                changedir(hold,h,spr[h].base_walk);

            }
            else
            {
                int junk = spr[h].size;

                if (junk >=  100)
                    junk = 18000 - (junk * 50);

                if (junk < 100)
                    junk = 16000 + (junk * 100);

                SoundPlayEffect( 1,junk, 800,h ,0);
                spr[h].mx = 0;
                spr[h].my = 0;
                spr[h].wait = thisTickCount + (rand() % 300)+200;

            }
            return;     
        } 

        if ((spr[h].mx != 0) || (spr[h].my != 0))

        {
            spr[h].seq = spr[h].seq_orig;

        }

    }


walk:
    if (spr[h].y > playy)

    {
        changedir(9,h,spr[h].base_walk);
    }         



    if (spr[h].x > playx-30)

    {
        changedir(7,h,spr[h].base_walk);
    }         

    if (spr[h].y < 10)
    {
        changedir(1,h,spr[h].base_walk);
    }         

    if (spr[h].x < 30) 
    {
        changedir(3,h,spr[h].base_walk);
    }         

    //   Msg("Duck dir is %d, seq is %d.", spr[h].dir, spr[h].seq); 
    automove(h);

    if (check_if_move_is_legal(h) != 0)

    {
        if (spr[h].dir != 0)
            changedir(autoreverse_diag(h),h,spr[h].base_walk);
    }

}
// end duck_brain

void change_dir_to_diag( int *dir)
{

    if (*dir == 8) *dir = 7;
    if (*dir == 4) *dir = 1;
    if (*dir == 2) *dir = 3;
    if (*dir == 6) *dir = 9;

}



void pill_brain(int h)
{
    int hold;

    if  (spr[h].damage > 0)
    {
        //got hit
        //SoundPlayEffect( 1,3000, 800 );  
        if (spr[h].hitpoints > 0)
        {
            draw_damage(h);
            if (spr[h].damage > spr[h].hitpoints) spr[h].damage = spr[h].hitpoints;
            spr[h].hitpoints -= spr[h].damage;

            if (spr[h].hitpoints < 1)
            {
                //they killed it
                check_for_kill_script(h);

                if (spr[h].brain == 9)
                {
                    if (spr[h].dir == 0) spr[h].dir = 3;
                    change_dir_to_diag(&spr[h].dir);
                    add_kill_sprite(h);
                    spr[h].active = false;
                }
                return;

            }
        }
        spr[h].damage = 0;

    }

    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }




    if (spr[h].freeze) return;

    if (spr[h].follow > 0)
    {
        process_follow(h);

    }


    if (spr[h].target != 0) 
    {

        if (in_this_base(spr[h].seq, spr[h].base_attack))
        {
            //still attacking
            return;
        }





        int dir;
        if (spr[h].distance == 0) spr[h].distance = 5;
        int distance = get_distance_and_dir(h, spr[h].target, &dir);

        if (distance < spr[h].distance) if (spr[h].attack_wait < thisTickCount)
        {
            //  Msg("base attack is %d.",spr[h].base_attack);
            if (spr[h].base_attack != -1)
            {
                int attackdir;
                bool old_smooth_follow = smooth_follow;
                smooth_follow = false;
                get_distance_and_dir(h, spr[h].target, &attackdir);
                smooth_follow = old_smooth_follow;
                //Msg("attacking with %d..", spr[h].base_attack+dir);

                spr[h].dir = attackdir;

                spr[h].seq = spr[h].base_attack+spr[h].dir;
                spr[h].frame = 0;

                if (spr[h].script != 0)
                    if (locate(spr[h].script, "ATTACK")) run_script(spr[h].script); else
                        spr[h].move_wait = thisTickCount + ((rand() % 300)+10);;
                return;

            }

        }



        if (spr[h].move_wait  < thisTickCount)
        {
            process_target(h);
            spr[h].move_wait = thisTickCount + 200;

        }
        else
        {
            /*  automove(h);

            if (check_if_move_is_legal(h) != 0)
            {

            }
            */

            goto walk_normal;
        }

        return;
    }



walk_normal:

    if (spr[h].base_walk != -1)
    {
        if ( spr[h].seq == 0) goto recal;
    }

    if (( spr[h].seq == 0) && (spr[h].move_wait < thisTickCount))
    {
recal:
        if (((rand() % 12)+1) == 1 )
        {  
            hold = ((rand() % 9)+1);
            if (  (hold != 4) &&   (hold != 6) &&  (hold != 2) && (hold != 8) && (hold != 5))
            {
                changedir(hold,h,spr[h].base_walk);
                spr[h].move_wait = thisTickCount +((rand() % 2000)+200);

            }

        } else
        {
            //keep going the same way
            if (in_this_base(spr[h].seq_orig, spr[h].base_attack)) goto recal;
            spr[h].seq = spr[h].seq_orig;
            if (spr[h].seq_orig == 0) goto recal;
        }

    }



    if (spr[h].y > (playy - 15))

    {
        changedir(9,h,spr[h].base_walk);
    }         

    if (spr[h].x > (playx - 15))

    {
        changedir(1,h,spr[h].base_walk);
    }         

    if (spr[h].y < 18)
    {
        changedir(1,h,spr[h].base_walk);
    }         

    if (spr[h].x < 18) 
    {
        changedir(3,h,spr[h].base_walk);
    }         

    automove(h);

    if (check_if_move_is_legal(h) != 0)
    {
        spr[h].move_wait = thisTickCount + 400;
        changedir(autoreverse_diag(h),h,spr[h].base_walk);
    }


    //              changedir(hold,h,spr[h].base_walk);


}

void find_action(int h)
{

    spr[h].action = (rand() % 2)+1;


    if (spr[h].action == 1)
    {
        //sit and think
        spr[h].move_wait = thisTickCount +((rand() % 3000)+400);
        if (spr[h].base_walk != -1)
        {
            int dir = (rand() % 4)+1;  

            spr[h].pframe = 1;
            if (dir == 1)  spr[h].pseq = spr[h].base_walk+1;
            if (dir == 2)  spr[h].pseq = spr[h].base_walk+3;
            if (dir == 3)  spr[h].pseq = spr[h].base_walk+7;
            if (dir == 4)  spr[h].pseq = spr[h].base_walk+9;
        }

        return;
    }

    if (spr[h].action == 2)
    {
        //move
        spr[h].move_wait = thisTickCount +((rand() % 3000)+500);
        int dir = (rand() % 4)+1;  
        spr[h].pframe = 1;
        if (dir == 1)  changedir(1,h,spr[h].base_walk);
        if (dir == 2)  changedir(3,h,spr[h].base_walk);
        if (dir == 3)  changedir(7,h,spr[h].base_walk);
        if (dir == 4)  changedir(9,h,spr[h].base_walk);
        return;
    }


    Msg("Internal error:  Brain 16, unknown action.");
}


void people_brain(int h)
{
    int hold;

    if  (spr[h].damage > 0)
    {
        //got hit
        //SoundPlayEffect( 1,3000, 800 );  
        if (spr[h].hitpoints > 0)
        {
            draw_damage(h);
            if (spr[h].damage > spr[h].hitpoints) spr[h].damage = spr[h].hitpoints;
            spr[h].hitpoints -= spr[h].damage;

            if (spr[h].hitpoints < 1)
            {
                //they killed it
                check_for_kill_script(h);

                if (spr[h].brain == 16)
                {
                    if (spr[h].dir == 0) spr[h].dir = 3;
                    spr[h].brain = 0;
                    change_dir_to_diag(&spr[h].dir);
                    add_kill_sprite(h);
                    spr[h].active = false;
                }
                return;

            }
        }
        spr[h].damage = 0;

    }

    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }




    if (spr[h].freeze) return;

    if (spr[h].follow > 0)
    {
        process_follow(h);
        return;
    }


    if ((spr[h].move_wait < thisTickCount) && (spr[h].seq == 0))
    {

        spr[h].action = 0;
    }



    if (spr[h].action == 0) find_action(h);


    if (spr[h].action != 2) 
    {
        spr[h].seq = 0;
        return;

    }
    if (spr[h].seq_orig != 0)
        if (spr[h].seq == 0) spr[h].seq = spr[h].seq_orig;


    if (spr[h].y > playy)

    {

        if ( ((rand() % 2)+1) == 1)
            changedir(9,h,spr[h].base_walk);
        else changedir(7,h,spr[h].base_walk);


    }         

    if (spr[h].x > playx)

    {
        if ( ((rand() % 2)+1) == 1)
            changedir(1,h,spr[h].base_walk);
        else changedir(7,h,spr[h].base_walk);

    }         

    if (spr[h].y < 20)
    {
        if ( ((rand() % 2)+1) == 1)
            changedir(1,h,spr[h].base_walk);
        else changedir(3,h,spr[h].base_walk);
    }         

    if (spr[h].x < 30) 
    {
        if ( ((rand() % 2)+1) == 1)
            changedir(3,h,spr[h].base_walk);
        else changedir(9,h,spr[h].base_walk);
    }         

    automove(h);

    if (check_if_move_is_legal(h) != 0)
    {
        if ((rand() % 3) == 2)
        {
            changedir(autoreverse_diag(h),h,spr[h].base_walk);

        } else
        {
            spr[h].move_wait = 0;
            spr[h].pframe = 1;
            spr[h].seq = 0;
        }
    }


    //              changedir(hold,h,spr[h].base_walk);


}


void no_brain(int h)
{
    int hold;

    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }

    if (spr[h].freeze) return;

    if (spr[h].follow > 0)
    {
        process_follow(h);
        return;
    }

}


void shadow_brain(int h)
{
    int hold;

    if (spr[spr[h].brain_parm].active == false)
    {
        spr[h].active = false;
        return;
    }

    spr[h].x = spr[spr[h].brain_parm].x;
    spr[h].y = spr[spr[h].brain_parm].y;
    spr[h].size = spr[spr[h].brain_parm].size;

    if (spr[h].seq == 0) if (spr[h].seq_orig != 0) spr[h].seq = spr[h].seq_orig;

}



void dragon_brain(int h)
{
    int hold;


    if  (spr[h].damage > 0)
    {
        //got hit
        //SoundPlayEffect( 1,3000, 800 );  
        if (spr[h].hitpoints > 0)
        {
            draw_damage(h);
            if (spr[h].damage > spr[h].hitpoints) spr[h].damage = spr[h].hitpoints;
            spr[h].hitpoints -= spr[h].damage;

            if (spr[h].hitpoints < 1)
            {
                //they killed it

                check_for_kill_script(h);
                if (spr[h].brain == 10)
                {
                    add_kill_sprite(h);
                    spr[h].active = false;

                }

                return;

            }
        }
        spr[h].damage = 0;
    }


    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }


    if (spr[h].freeze) return;


    if (spr[h].follow > 0)
    {
        process_follow(h);
        return;
    }

    if (spr[h].target != 0)
        if (spr[h].attack_wait < thisTickCount)
        {
            if (spr[h].script != 0) 
            {

                if (locate(spr[h].script, "ATTACK")) run_script(spr[h].script);
            }   
        }



        if (spr[h].seq == 0)
        {
recal:
            if (((rand() % 12)+1) == 1 )
            {  
                hold = ((rand() % 9)+1);
                if (  (hold != 1) &&   (hold != 3) &&  (hold != 7) && (hold != 9) && (hold != 5))
                {
                    changedir(hold,h,spr[h].base_walk);

                }

            } else
            {
                //keep going the same way
                spr[h].seq = spr[h].seq_orig;
                if (spr[h].seq_orig == 0) goto recal;
            }

        }


        if (spr[h].y > playy)

        {
            changedir(8,h,spr[h].base_walk);
        }         

        if (spr[h].x > x)
        {
            changedir(4,h,spr[h].base_walk);
        }         

        if (spr[h].y < 0)
        {
            changedir(2,h,spr[h].base_walk);
        }         

        if (spr[h].x < 0) 
        {
            changedir(6,h,spr[h].base_walk);
        }         

        automove(h);

        if (check_if_move_is_legal(h) != 0)

        {

            int mydir = autoreverse(h);

            //  Msg("Real dir now is %d, autoresver changed to %d.",spr[h].dir, mydir);

            changedir(mydir,h,spr[h].base_walk);

            Msg("real dir changed to %d",spr[h].dir);
        }

}




void pig_brain(int h)
{
    int hold;


start:

    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }

    if (   (spr[h].damage > 0) )
    {
        //SoundPlayEffect( 1,3000, 800 );  
        draw_damage(h);
        spr[h].hitpoints -= spr[h].damage;
        spr[h].damage = 0;
        if (spr[h].hitpoints < 1)
        {
            add_exp(spr[h].exp, h);
            spr[h].damage = 0;
            //lets kill the duck here, ha.
            check_for_kill_script(h);
            spr[h].speed = 0;
            spr[h].base_walk = -1;
            spr[h].seq = 164;
            spr[h].brain = 7;    
        }

        return;
    }


    if (spr[h].freeze) return;



    if (spr[h].seq == 0 ) 
    {

        if (((rand() % 12)+1) == 1 )
        {  
            hold = ((rand() % 9)+1);

            if (  (hold != 4) &&   (hold != 6) &&  (hold != 2) && (hold != 8) && (hold != 5))
            {
                changedir(hold,h,spr[h].base_walk);

            }
            else
            {
                int junk = spr[h].size;

                if (junk >=  100)
                    junk = 18000 - (junk * 50);

                if (junk < 100)
                    junk = 16000 + (junk * 100);


                hold = ((rand() % 4)+1);

                if (!playing(spr[h].last_sound)) spr[h].last_sound = 0;

                if (spr[h].last_sound == 0)
                {


                    if (hold == 1) 
                        spr[h].last_sound = SoundPlayEffect( 2,junk, 800 ,h,0);
                    if (hold == 2) 
                        spr[h].last_sound = SoundPlayEffect( 3,junk, 800,h ,0);
                    if (hold == 3) 
                        spr[h].last_sound = SoundPlayEffect( 4,junk, 800 ,h,0);
                    if (hold == 4) 
                        spr[h].last_sound = SoundPlayEffect( 5,junk, 800,h,0 );

                }

                spr[h].mx = 0;
                spr[h].my = 0;
                spr[h].wait = thisTickCount + (rand() % 300)+200;

            }

        } 
        else
        {

            if ((spr[h].mx != 0) || (spr[h].my != 0))

            {
                spr[h].seq = spr[h].seq_orig;

            }                                                                                                                                                                                                                                                                                                                          

        }
    }


    if (spr[h].y > (playy-k[getpic(h)].box.bottom / 4))
    {
        changedir(9,h,spr[h].base_walk);
    }         

    if (spr[h].x > (x-k[getpic(h)].box.right-10))
    {
        changedir(1,h,spr[h].base_walk);
    }         

    if (spr[h].y < 10)
    {
        changedir(1,h,spr[h].base_walk);
    }         

    if (spr[h].x < 10) 
    {
        changedir(3,h,spr[h].base_walk);
    }         

    automove(h);

    if (check_if_move_is_legal(h) != 0)

    {
        changedir(autoreverse_diag(h),h,spr[h].base_walk);
    }

}
// end duck_brain




int check_if_move_is_legal(int u)

{
    //redink1 removed so move_nohard is active for all movements, not just active moves.
    //if (spr[u].move_active)
    if (spr[u].move_nohard == 1)
        return(0);
    if (u == 1) if (in_this_base(spr[u].seq, mDinkBasePush)) return(0);

    if (u == 1) if (!no_cheat) if (debug_mode) return(0);
    int hardness = 0;
    if (spr[u].moveman > 0)
    {
        for (int i=1; i <= spr[u].moveman; i++)
        {
            hardness = get_hard(u,spr[u].lpx[i]-20  , spr[u].lpy[i]);
            if (hardness == 2) if (spr[u].flying) 
            {
                spr[u].moveman = 0;         
                // redink1 changed so flying works properly
                return(0);
            }
            if (hardness > 0)
            {

                spr[u].x = spr[u].lpx[i-1];
                spr[u].y = spr[u].lpy[i-1];
                spr[u].moveman = 0;         

                if (push_active)
                    if (u == 1) if (hardness != 2) if (play.push_active == false)

                    {

push:
                        if (  (spr[u].dir == 2) | (spr[u].dir == 4) |  (spr[u].dir == 6) | (spr[u].dir == 8) )
                        {
                            //he  (dink)  is definatly pushing on something
                            play.push_active = true;
                            play.push_dir = spr[u].dir;
                            play.push_timer = thisTickCount;

                        }
                    } else
                    {
                        if (play.push_dir != spr[1].dir) play.push_active = false;
                    }
                    return(hardness);
            }

        }

    }


    if (u == 1)  play.push_active = false;
    return(0);
}



void move(int u, int amount, char kind,  char kindy)
{
    int mx = 0;
    int my = 0; 
    bool clearx;
    bool cleary;
    clearx = false;
    cleary = false;

    for (int i=1; i <= amount; i++)
    {
        spr[u].moveman++;
        if (mx >= spr[u].mx) clearx = true;
        if (my >= spr[u].my) clearx = true;

        if ((clearx) & (cleary))
        {
            mx = 0;
            my = 0;
            clearx = false;
            cleary = false;

        }


        if (kind == '+')
        {
            if (mx < spr[u].mx)
                spr[u].x++;
            mx++;

        }
        if (kind == '-')
        {


            if (mx < (spr[u].mx - (spr[u].mx * 2)))
                spr[u].x--;
            mx++;
        }

        if (kindy == '+')
        {

            if (my < spr[u].my)
                spr[u].y++;
            my++;
        }
        if (kindy == '-')
        {

            if (my < (spr[u].my - (spr[u].my * 2)))
                spr[u].y--;
            my++;
        }

        spr[u].lpx[spr[u].moveman] = spr[u].x;
        spr[u].lpy[spr[u].moveman] = spr[u].y;
    }


}



void bounce_brain(int h)
{
    if (spr[h].y > (playy-k[getpic(h)].box.bottom))
    {
        spr[h].my -= (spr[h].my * 2);
    }         

    if (spr[h].x > (x-k[getpic(h)].box.right))
    {
        spr[h].mx -= (spr[h].mx * 2);
    }         

    if (spr[h].y < 0)
    {
        spr[h].my -= (spr[h].my * 2);
    }         


    if (spr[h].x < 0) 
    {
        spr[h].mx -= (spr[h].mx * 2);
    }         


    spr[h].x += spr[h].mx;
    spr[h].y += spr[h].my;


}
//end bounce brain      

void grab_trick(int trick)
{
    Common::Rect rcRect;
    HRESULT ddrval;
    //Msg("making trick.");

    if (no_transition)
    {

        move_screen = trick;            
        trig_man = true;

        move_counter = 0;
        return;
    }
    rcRect.left = playl;
    rcRect.top = 0;
    //redink1 fix so scroll works for entire screen
    rcRect.right = 620;
    rcRect.bottom = 400;

    ddrval = lpDDSTrick->BltFast( 0, 0, lpDDSBack,
        &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

    if (ddrval != DD_OK) dderror(ddrval);
    move_screen = trick;            
    trig_man = true;

    move_counter = 0;
}






void did_player_cross_screen(bool real, int h)
{

    if (walk_off_screen == 1) return;
    int middlex,middley;


    //DO MATH TO SEE IF THEY HAVE CROSSED THE SCREEN, IF SO LOAD NEW ONE

    if ((spr[h].x) < playl) 
    {
        if ((map.loc[*pmap-1] > 0) && (screenlock == 0) )
        {
            //move one map to the left
            if (real)
            {
                move_gonna = true;
                return;
            }
            update_screen_time();
            grab_trick(4);
            *pmap -= 1; 
            load_map(map.loc[*pmap]);
            if (map.indoor[*pmap] == 0) play.last_map = *pmap;

            draw_map_game();                                    
            spr[h].x = 619;
            spr[h].y = spr[h].lpy[0];
            goto b1end;
        } else
        {
            spr[h].x = playl;

        }


    }

    if ((spr[h].x) > 619) 
    {
        if ((map.loc[*pmap+1] > 0)  && (screenlock == 0) )
        {
            //move one map to the right
            if (real)
            {
                move_gonna = true;
                return;
            }

            update_screen_time();
            grab_trick(6);
            *pmap += 1; 
            load_map(map.loc[*pmap]);
            if (map.indoor[*pmap] == 0) play.last_map = *pmap;

            draw_map_game();
            spr[h].x =  playl;
            spr[h].y = spr[h].lpy[0];
            goto b1end;
        } else
        {
            spr[h].x = 619;
        }
    }

    if (spr[h].y < 0)

    {
        if ((map.loc[*pmap-32] > 0)  && (screenlock == 0) )
        {
            //move one map up
            if (real)
            {
                move_gonna = true;
                return;
            }
            update_screen_time();
            grab_trick(8);
            *pmap -= 32;    
            load_map(map.loc[*pmap]);
            if (map.indoor[*pmap] == 0) play.last_map = *pmap;

            spr[h].x = spr[h].lpx[0];
            draw_map_game();
            spr[h].y =  399;

            goto b1end;
        } else
        {
            spr[h].y = 0;
        }
    }


    if ( (spr[h].y > 399 ) )
    {
        if ( (map.loc[*pmap+32] > 0)  && (screenlock == 0) )
        {
            //move one map down
            if (real)
            {
                move_gonna = true;
                return;
            }
            update_screen_time();
            grab_trick(2);
            *pmap += 32;    
            load_map(map.loc[*pmap]);
            if (map.indoor[*pmap] == 0) play.last_map = *pmap;

            draw_map_game();
            spr[h].y = 0;

            spr[h].x = spr[h].lpx[0];

            goto b1end;
        } else
        {
            spr[h].y = 399;
        }
    }



b1end:;



}


bool run_through_tag_list_talk(int h)
{
    Common::Rect box;
    int amount, amounty;

    for (int i = 1; i <= last_sprite_created; i++)
    {

        if (spr[i].active) if (i != h) if (spr[i].brain != 8)
        {


            CopyRect(&box, &k[getpic(i)].hardbox);
            OffsetRect(&box, spr[i].x, spr[i].y);

            InflateRect(&box, 10,10);

            amount = 50;        
            amounty = 35;
            if (spr[h].dir == 6)
            {
                box.left -= amount;
            }

            if (spr[h].dir == 4)
            {
                box.right += amount;
            }


            if (spr[h].dir == 2)
            {
                box.top -= amounty;
            }

            if (spr[h].dir == 8)
            {
                box.bottom += amounty;
            }

            //      draw_box(box, 33);

            if (inside_box(spr[h].x, spr[h].y, box))
            {   
                //Msg("Talking to sprite %d", i);
                if (spr[i].script > 0)
                {
                    //if (  (spr[i].brain == 0) | (spr[i].brain == 5) | (spr[i].brain == 6) | (spr[i].brain == 7))
                    //Msg("trying to find TALK in script %d", spr[i].script);
                    if (locate(spr[i].script, "TALK")) 
                    {
                        kill_returning_stuff(spr[i].script);

                        run_script(spr[i].script);
                        return(true);   
                    }


                }

            }


        }


    }


    return(false);
}





void make_missile(int x1, int y1, int dir, int speed, int seq, int frame, int strength)
{
    int crap = add_sprite(x1,y1,11,seq,frame);
    spr[crap].speed = speed;
    spr[crap].seq = seq;
    spr[crap].timer = 0;
    spr[crap].strength = strength;
    spr[crap].flying = true;
    changedir(dir, crap, 430);

}




void missile_brain( int h, bool repeat)
{
    Common::Rect box;
    automove(h);

    *pmissle_source = h;
    int hard = check_if_move_is_legal(h);
    if (repeat)  if (spr[h].seq == 0) spr[h].seq = spr[h].seq_orig;
    spr[1].hitpoints = *plife; 

    if (hard > 0) if (hard != 2) 
    {

        //lets check to see if they hit a sprites hardness
        if (hard > 100)
        {
            for (int ii = 1; ii < last_sprite_created; ii++)
            {
                if (spr[ii].sp_index == hard-100)
                {
                    if (spr[ii].script > 0)
                    {
                        *pmissile_target = 1;
                        *penemy_sprite = 1;



                        if (locate(spr[ii].script, "HIT"))
                        {
                            kill_returning_stuff(spr[ii].script);
                            run_script(spr[ii].script);

                        }
                    }


                    if (spr[h].script > 0)
                    {
                        *pmissile_target = ii;
                        *penemy_sprite = 1;
                        if (locate(spr[h].script, "DAMAGE")) 
                        {
                            kill_returning_stuff(spr[h].script);
                            run_script(spr[h].script);
                        }
                    } else
                    {

                        if (spr[h].attack_hit_sound == 0)
                        {
                            SoundPlayEffect( 9,22050, 0 ,0,0);
                        } else
                        {
                            SoundPlayEffect( spr[h].attack_hit_sound,spr[h].attack_hit_sound_speed, 0 ,0,0);
                        }


                        spr[h].active = 0;


                    }


                    //run missile end   
                    return;
                }

            }

        }
        //run missile end   

        if (spr[h].script > 0)
        {


            *pmissile_target = 0;
            if (locate(spr[h].script, "DAMAGE")) run_script(spr[h].script);
        } else
        {

            if (spr[h].attack_hit_sound == 0)
            {
                SoundPlayEffect( 9,22050, 0 ,0,0);
            } else
            {
                SoundPlayEffect( spr[h].attack_hit_sound,spr[h].attack_hit_sound_speed, 0 ,0,0);
            }


            spr[h].active = 0;
            return;
        }

    }

    if (spr[h].x > 1000) spr[h].active = false;
    if (spr[h].y > 700) spr[h].active = false;
    if (spr[h].y < -500) spr[h].active = false;
    if (spr[h].x < -500) spr[h].active = false;

    //did we hit anything that can die?

    for (int j = 1; j <= last_sprite_created; j++)
    {
        if (spr[j].active) if (h != j) if (spr[j].nohit != 1) if (spr[j].notouch == false)
            if (spr[h].brain_parm != j) if (spr[h].brain_parm2!= j) //if (spr[j].brain != 15) if
                //(spr[j].brain != 11)
            {
                CopyRect(&box, &k[getpic(j)].hardbox);
                OffsetRect(&box, spr[j].x, spr[j].y);

                if (spr[h].range != 0)
                    InflateRect(&box, spr[h].range,spr[h].range);

                if (debug_mode) draw_box(box, 33);


                if (inside_box(spr[h].x, spr[h].y, box))
                {

                    spr[j].notouch = true;
                    spr[j].notouch_timer = thisTickCount+100;
                    spr[j].target = 1;
                    *penemy_sprite = 1;
                    //change later to reflect REAL target
                    if (spr[h].script > 0)
                    {

                        *pmissile_target = j;
                        if (locate(spr[h].script, "DAMAGE")) run_script(spr[h].script);
                    } else
                    {

                        if (spr[h].attack_hit_sound == 0)
                        {
                            SoundPlayEffect( 9,22050, 0 ,0,0);
                        } else
                        {
                            SoundPlayEffect( spr[h].attack_hit_sound,spr[h].attack_hit_sound_speed, 0 ,0,0);
                        }

                    }


                    if ( spr[j].hitpoints > 0)  if (spr[h].strength != 0)
                    {
                        int hit = 0;
                        if (spr[h].strength == 1) hit = spr[h].strength - spr[j].defense; else

                            hit = (spr[h].strength / 2) + ((rand() % (spr[h].strength / 2))+1)
                            - spr[j].defense;

                        if (hit < 0) hit = 0;


                        spr[j].damage += hit;
                        if (hit > 0)
                        {
                            random_blood(spr[j].x, spr[j].y-40, j);
                        }
                        spr[j].last_hit = 1;
                        //Msg("Damage done is %d..", spr[j].damage);
                    }

                    if (spr[j].script > 0)
                    {
                        //CHANGED did = h
                        *pmissile_target = 1;

                        if (locate(spr[j].script, "HIT"))
                        {
                            kill_returning_stuff(spr[j].script);
                            run_script(spr[j].script);
                        }
                    }

                }
                //run missile end   

            }
    }

}


void missile_brain_expire(int h)
{
    missile_brain(h, false);
    if (spr[h].seq == 0) spr[h].active = 0;

}

void run_through_mouse_list(int h, bool special)
{
    Common::Rect box;
    int amount, amounty;

    for (int i = 1; i <= last_sprite_created; i++)
    {

        if (spr[i].active) if (i != h) if
            ((spr[i].touch_damage != 0) )
        {

            if (spr[i].touch_damage != -1) if (spr[h].notouch) return;
            CopyRect(&box, &k[getpic(i)].hardbox);
            OffsetRect(&box, spr[i].x, spr[i].y);


            if (inside_box(spr[h].x, spr[h].y, box))
            {   

                if ((spr[i].touch_damage == -1) && (spr[i].script != 0))
                {
                    Msg("running %d's script..",spr[i].script);
                    if (locate(spr[i].script, "CLICK")) run_script(spr[i].script);
                } 
                else
                {
                    if (spr[i].touch_damage == -1)
                    {
                        Msg("Sprites touch damage is set to -1 but there is no script set!");
                    } else
                    {
                        //lets hurt the guy
                    }

                }

                if (special) return;    

            }


        }

    }

    if (special)        SoundPlayEffect(19, 22050, 0, 0,0);

}




void mouse_brain(int h)
{

    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }


    int diag = 0;

    if (sjoy.right) diag++;
    if (sjoy.left) diag++;
    if (sjoy.down) diag++;
    if (sjoy.up) diag++;


    //*********************************PROCESS MOVEMENT                         

    if (diag == 1)
    {

        if (sjoy.right)
        {
            move(h,spr[h].speed,'+','0');
            changedir(6,h,spr[h].base_walk);
        }


        if (sjoy.left) 
        {
            move(h,spr[h].speed,'-','0');
            changedir(4,h,spr[h].base_walk);
        }


        if (sjoy.down)
        {
            move(h,spr[h].speed,'0','+');
            changedir(2,h,spr[h].base_walk);
        }


        if (sjoy.up) 
        {
            move(h,spr[h].speed,'0','-');
            changedir(8,h,spr[h].base_walk);
        }

    }
    // ***************** DIAGONAL!!!!

    if (diag > 1)
    {

        if ( (sjoy.up) & (sjoy.left) ) 
        {
            changedir(7,h,spr[h].base_walk);
            move(h,spr[h].speed - (spr[h].speed / 3),'-','-');

        }

        if ( (sjoy.down) & (sjoy.left))
        {
            changedir(1,h,spr[h].base_walk);
            move(h,spr[h].speed - (spr[h].speed / 3),'-','+');

        }

        if ( (sjoy.down) & (sjoy.right))
        {
            changedir(3,h,spr[h].base_walk);
            move(h,spr[h].speed - (spr[h].speed / 3),'+','+');
        }


        if ( (sjoy.up) & (sjoy.right))
        {
            changedir(9,h,spr[h].base_walk);
            move(h,spr[h].speed - (spr[h].speed / 3),'+','-');
        }

    }




    if ( (sjoy.button[1] == true) | (mouse1) )
    {

        Msg("running through mouse list..");
        run_through_mouse_list(h, true);
        sjoy.button[1] = false;
        mouse1 = false;

    }

}

void process_bow( int h)
{
    int timetowait = 100;


    if (bow.wait < thisTickCount)
    {
        if (sjoy.right) spr[h].dir = 6;
        if (sjoy.left) spr[h].dir = 4;
        if (sjoy.up) spr[h].dir = 8;
        if (sjoy.down) spr[h].dir = 2;
    }



    if (sjoy.right) if (sjoy.up) 
    {
        spr[h].dir = 9;
        bow.wait = thisTickCount + timetowait;
    }
    if (sjoy.left) if (sjoy.up) 
    {
        spr[h].dir = 7;
        bow.wait = thisTickCount + timetowait;
    }
    if (sjoy.right) if (sjoy.down) 
    {
        spr[h].dir = 3;
        bow.wait = thisTickCount + timetowait;

    }
    if (sjoy.left) if (sjoy.down) 
    {
        spr[h].dir = 1;
        bow.wait = thisTickCount + timetowait;

    }
    spr[h].pseq = 100+spr[h].dir;


    if (bow.pull_wait < thisTickCount)
    {
        bow.pull_wait = thisTickCount + 10;
        if (bow.hitme) bow.time += 7;


        //  bowsound->SetFrequency(22050+(bow.time*10));

        if (bow.time > 500) bow.time = 500;
        spr[h].pframe = (bow.time / 100)+1;
    }


    if (sjoy.letgo[1])
    {
        bow.active = false;
        bow.last_power = bow.time;
        run_script(bow.script);
        //     bowsound->Stop();
        return;
    }

}




void human_brain(int h)

{

    int diag, x5;
    int crap;
    bool bad;

    if (mode == 0) goto b1end;          


    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }


    if (spr[h].damage > 0)
    {

        draw_damage(h);

        *plife -= spr[h].damage;

        spr[h].damage = 0;
        if (*plife < 0) *plife = 0;

        int hurt = (rand() % 2)+1;

        if (hurt == 1) SoundPlayEffect( 15,25050, 2000 ,0,0);
        if (hurt == 2) SoundPlayEffect( 16,25050, 2000 ,0,0);


        //draw blood

    }


    if (play.push_active)
    {

        if (play.push_dir == 2) if (!sjoy.down) 
        {
            spr[h].nocontrol = false;
            play.push_active = false;
        }

        if (play.push_dir == 4) if (!sjoy.left) 
        {
            spr[h].nocontrol = false;
            play.push_active = false;
        }
        if (play.push_dir == 6) if (!sjoy.right) 
        {
            spr[h].nocontrol = false;
            play.push_active = false;
        }

        if (play.push_dir == 8) if (!sjoy.up) 
        {
            spr[h].nocontrol = false;
            play.push_active = false;
        }


    }

    if (spr[h].nocontrol) return;                   

    if (talk.active) goto freeze;


    if ( spr[h].freeze)

    {
        //they are frozen

        if ( (sjoy.button[2] == true) || (sjoy.key[32]))
        {
            //they hit the talk button while frozen, lets hurry up the process

            for (int jj = 1; jj <=  last_sprite_created; jj++)
            {

                //                          Msg("Checking %d, brain %d, script %d, my freeze is %d",jj, spr[jj].brain, spr[jj].script, spr[h].freeze);
                if (spr[jj].brain == 8) if (spr[jj].script == play.last_talk)
                {
                    //this sprite owns its freeze

                    spr[jj].kill_timer = 1;
                    //force the message to be over
                }

            }




        }




        goto freeze;
    }
    //******************************  KEYS THAT CAN BE PRESSED AT ANY TIME **************

    //if (GetKeyboard(127) PostMessage(hWnd, WM_CLOSE, 0, 0);


    if (bow.active)
    {

        //bow is active!!
        process_bow(h);
        return;
    }



    if (play.push_active) if (play.push_timer + 600 < thisTickCount)
    {
        spr[h].seq = mDinkBasePush+spr[h].dir;
        spr[h].frame = 1;
        spr[h].nocontrol = true;
        //play.push_active = false;
        run_through_tag_list_push(h);

        return;
    }



    if ( (sjoy.button[2] == true) )
    {

        if (!run_through_tag_list_talk(h))
        {
            //redink1 addition of 'not talking to anything' script
            int sc = load_script("DNOTALK", 0, false);
            if (sc != 0 && locate(sc,"MAIN"))
            {
                run_script(sc);
            }
            else
            {

                kill_text_owned_by(h);  
                int randy = (rand() % 6)+1;

#ifdef __GERMAN
                if (randy == 1) say_text("`$Hier ist nichts.",h,0);
                if (randy == 2) say_text("`$Huh?",h,0);
                if (randy == 3) say_text("`$Ich bin mir ziemlich sicher ich kann dies weder ansprechen, noch benutzen.",h,0);
                if (randy == 4) say_text("`$Was?",h,0);
                if (randy == 5) say_text("`$Bah, LANGWEILIG.",h,0);
                if (randy == 6) say_text("`$Hier passiert nichts.",h,0);

#endif

#ifdef __ENGLISH
                if (randy == 1) say_text("`$I don't see anything here.",h,0);
                if (randy == 2) say_text("`$Huh?",h,0);
                if (randy == 3) say_text("`$I'm fairly sure I can't talk to or use that.",h,0);
                if (randy == 4) say_text("`$What?",h,0);
                if (randy == 5) say_text("`$I'm bored.",h,0);
                if (randy == 6) say_text("`$Not much happening here.",h,0);
#endif  
            }
        }   


    }


    if ( (sjoy.button[1] == true) && (weapon_script != 0) )
    {



        if (spr[h].base_hit > 0)
        {


            if (locate(weapon_script, "USE")) run_script(weapon_script);

            goto b1end;

        }

    }


    //added AGAIN 10-19-99
    //Let's check keys for getting hit
    if (but_timer < thisTickCount)  
    {
        //redink1 Only accept keyboard input when console is not active.
        if ( mConsoleActive == false )
        {
            for (x5=29; x5<255; x5++)
            { 
                if (x5 == 32) x5++;
                if (x5 == 54) x5++;
                if (x5 == 55) x5++;
                if (x5 == 37) x5++;
                if (x5 == 38) x5++;
                if (x5 == 39) x5++;
                if (x5 == 40) x5++;
                if (x5 == 77) x5++;


                char msg[30];
                if (GetKeyboard(x5))
                {
                    sprintf(msg, "key-%d",x5);
                    but_timer = thisTickCount+200;

                    int mycrap = load_script(msg, 1, false);
                    if (locate(mycrap, "MAIN")) 
                    {
                        run_script(mycrap);
                        goto b1end;
                    }
                }
            }
        }
    }

    
    if ( sjoy.button[6] == true )
    {
        int mycrap = load_script("BUTTON6", 1, false);
        if (locate(mycrap, "MAIN")) run_script(mycrap);
        goto b1end;
    }

    if ( sjoy.button[7] == true )
    {
        int mycrap = load_script("BUTTON7", 1, false);
        if (locate(mycrap, "MAIN")) run_script(mycrap);
        goto b1end;
    }

    if ( sjoy.button[8] == true )
    {
        int mycrap = load_script("BUTTON8", 1, false);
        if (locate(mycrap, "MAIN")) run_script(mycrap);
        goto b1end;
    }

    if ( sjoy.button[9] == true )
    {
        int mycrap = load_script("BUTTON9", 1, false);
        if (locate(mycrap, "MAIN")) run_script(mycrap);
        goto b1end;
    }

    if ( sjoy.button[10] == true )
    {
        int mycrap = load_script("BUTTON10", 1, false);
        if (locate(mycrap, "MAIN")) run_script(mycrap);
        goto b1end;
    }

    if (magic_script != 0) if (sjoy.joybit[3]) goto shootm;
    if ( (sjoy.button[3] == true) )
    {
        if (magic_script == 0)
        {
            //redink1 addition of 'no magic' script
            int sc = load_script("DNOMAGIC", 0, false);
            if (sc != 0 && locate(sc,"MAIN"))
            {
                run_script(sc);
            }
            else
            {
                int randy = (rand() % 6)+1;
                kill_text_owned_by(h);  


#ifdef __GERMAN

                //if (randy == 3) 
                if (randy == 1) say_text("`$Ich mu� erst die Magie erlernen, bevor ich dies ausprobieren kann..",h,0);
                if (randy == 2) say_text("`$Ich gestikuliere hier wie wirr!",h,0);
                if (randy == 3) say_text("`$Ich gestikuliere hier wie wirr!",h,0);
                if (randy == 4) say_text("`$Ich gestikuliere hier wie wirr!",h,0);
                if (randy == 5) say_text("`$Nichts ist passiert.",h,0);
                if (randy == 6) say_text("`$Hokus pokus!",h,0);                                 
#endif

#ifdef __ENGLISH
                if (randy == 1) say_text("`$I don't know any magic.",h,0);
                if (randy == 2) say_text("`$I'm no wizard!",h,0);
                if (randy == 3) say_text("`$I need to learn magic before trying this.",h,0);
                if (randy == 4) say_text("`$I'm gesturing wildly to no avail!",h,0); //redink1 removed an extra space
                if (randy == 5) say_text("`$Nothing happened.",h,0);
                if (randy == 6) say_text("`$Hocus pocus!",h,0);
#endif
            }

            goto b1end;
        }

        //player pressed 1
        //lets magiced something
shootm: 
        if (*pmagic_level >= *pmagic_cost)
        {
            if (locate(magic_script, "USE")) run_script(magic_script);

            goto b1end; 
        } 
    }

    if (sjoy.button[4])
    {
        //redink1 addition of 'enter key/inventory' script
        int sc = load_script("BUTTON4", 0, false);
        if (sc != 0) {
            if (locate(sc,"MAIN")) {
                run_script(sc);
                return;
            }
        }

        item_screen = true;
        SoundPlayEffect(18, 22050,0,0,0);
        return;
    }



    /*if ( GetKeyboard(50) )
    {

        if (!no_cheat) 
        {
            //player pressed 2
            //lets add a duck with brain 2

            crap = add_sprite(spr[h].x-20,spr[h].y-50,3,26,1);
            spr[crap].speed = 1;
            spr[crap].base_walk = 20;
            spr[crap].exp = 11;
            spr[crap].hitpoints = 5;
        }
    }*/


    if ( (sjoy.button[5] == true) )
    {

        if (!showb.active) if (!bow.active) if (!talk.active)
        {
            int sc = load_script("ESCAPE", 1000, false);
            if (sc != 0) if (locate(sc,"MAIN")) run_script(sc);
            return;
        }
    }

    //redink1 Only accept keyboard input when console is not active.
    if ( mConsoleActive == false )
    {
        if (GetKeyboard(66))
        {
            ResumeMidi();
        }

        if (GetKeyboard(78))
        {
            PauseMidi();
        }
    }

    if (spr[h].skip > 0)

        if (spr[h].skip <= spr[h].skiptimer)
        {
            spr[h].skiptimer = 0;
            goto b1end;
        }


        diag = 0;
        if (sjoy.right) diag++;
        if (sjoy.left) diag++;
        if (sjoy.down) diag++;
        if (sjoy.up) diag++;


        //*********************************PROCESS MOVEMENT                         

        if (diag == 1)
        {

            if (sjoy.right)
            {
                move(h,spr[h].speed,'+','0');
                changedir(6,h,spr[h].base_walk);
            }


            if (sjoy.left) 
            {
                move(h,spr[h].speed,'-','0');
                changedir(4,h,spr[h].base_walk);
            }


            if (sjoy.down)
            {
                move(h,spr[h].speed,'0','+');
                changedir(2,h,spr[h].base_walk);

            }


            if (sjoy.up) 
            {
                move(h,spr[h].speed,'0','-');
                changedir(8,h,spr[h].base_walk);
            }

        }
        // ***************** DIAGONAL!!!!


        if (diag > 1) if (diag < 3)
        {

            if ( (sjoy.up) & (sjoy.left) ) 
            {
                changedir(7,h,spr[h].base_walk);
                move(h,spr[h].speed - (spr[h].speed / 3),'-','-');

            }

            if ( (sjoy.down) & (sjoy.left))
            {
                changedir(1,h,spr[h].base_walk);
                move(h,spr[h].speed - (spr[h].speed / 3),'-','+');

            }

            if ( (sjoy.down) & (sjoy.right))
            {
                changedir(3,h,spr[h].base_walk);
                move(h,spr[h].speed - (spr[h].speed / 3),'+','+');
            }


            if ( (sjoy.up) & (sjoy.right))
            {
                changedir(9,h,spr[h].base_walk);
                move(h,spr[h].speed - (spr[h].speed / 3),'+','-');
            }

        }


        bad = false;
        if (sjoy.right) bad = true;    
        if (sjoy.left) bad = true;    
        if (sjoy.up) bad = true;    
        if (sjoy.down) bad = true;    

        if (bad)
        {
            if (spr[h].idle)
            {
                spr[h].frame = 1;
                spr[h].idle = false;
            }
            goto badboy;
        }


        if (not_in_this_base(spr[h].seq, spr[h].base_idle)) //unccoment to allow walk anim to end before idle anim to start
        {
freeze:
            if (spr[h].dir == 1) spr[h].dir = 2;
            if (spr[h].dir == 3) spr[h].dir = 2;
            if (spr[h].dir == 7) spr[h].dir = 8;
            if (spr[h].dir == 9) spr[h].dir = 8;

            if (spr[h].base_idle != 0) changedir(spr[h].dir,h,spr[h].base_idle);                                
            spr[h].idle = true;   
        }


badboy: 



b1end:;

        if ( (spr[h].dir == 2)  | (spr[h].dir == 4) | (spr[h].dir == 6) | (spr[h].dir == 8)) goto smoothend;
        crap = check_if_move_is_legal(h);
        if (crap != 0)
        {
            if (pam.sprite[crap-100].prop != 0) flub_mode = crap;

            //hit something, can we move around it?


            if( (spr[h].seq == spr[h].base_walk + 4) |
                (spr[h].seq == spr[h].base_walk + 6) )
            {
                int hardm = get_hard_play(h, spr[h].x, spr[h].y-1);
                if (hardm == 0)
                {  
                    spr[h].y -= 1;

                }

            }







            if( (spr[h].seq == spr[h].base_walk + 8) |
                (spr[h].seq == spr[h].base_walk + 2) )
            {
                int hardm = get_hard_play(h, spr[h].x-1, spr[h].y);
                if (hardm == 0)
                {  
                    spr[h].x -= 1;

                }

            }




            if (spr[h].seq == spr[h].base_walk + 9)
            {
                int hardm = get_hard_play(h, spr[h].x+1, spr[h].y);
                if (hardm == 0)
                {  
                    spr[h].x += 1;

                } else
                {
                    int hardm = get_hard_play(h, spr[h].x+1, spr[h].y+1);
                    if (hardm == 0)
                    {  
                        spr[h].x += 1;
                        spr[h].y += 1;
                    } else
                    {
                        int hardm = get_hard_play(h, spr[h].x+1, spr[h].y+2);
                        if (hardm == 0)
                        {  
                            spr[h].x += 1;
                            spr[h].y += 2;
                        } else
                        {
                            int hardm = get_hard_play(h, spr[h].x, spr[h].y-1);
                            if (hardm == 0)
                            {  
                                spr[h].y -= 1;

                            } else
                            {
                                int hardm = get_hard_play(h, spr[h].x-1, spr[h].y-1);
                                if (hardm == 0)
                                {  
                                    spr[h].x -= 1;
                                    spr[h].y -= 1;
                                }

                            }



                        }

                    }


                }

            }

            if (spr[h].seq == spr[h].base_walk + 7)
            {
                int hardm = get_hard_play(h, spr[h].x-1, spr[h].y);
                if (hardm == 0)
                {  
                    spr[h].x -= 1;

                } else
                {
                    int hardm = get_hard_play(h, spr[h].x-1, spr[h].y+1);
                    if (hardm == 0)
                    {  
                        spr[h].x -= 1;
                        spr[h].y += 1;
                    } else
                    {
                        int hardm = get_hard_play(h, spr[h].x-1, spr[h].y+2);
                        if (hardm == 0)
                        {  
                            spr[h].x -= 1;
                            spr[h].y += 2;
                        } else
                        {

                            int hardm = get_hard_play(h, spr[h].x, spr[h].y-1);
                            if (hardm == 0)
                            {               
                                spr[h].y -= 1;
                            } else
                            {
                                int hardm = get_hard_play(h, spr[h].x+1, spr[h].y-1);
                                if (hardm == 0)
                                {               
                                    spr[h].x += 1;
                                    spr[h].y -= 1;
                                }
                            }

                        }

                    }


                }

            }



            if (spr[h].seq == spr[h].base_walk + 1)
            {
                int hardm = get_hard_play(h, spr[h].x-1, spr[h].y);
                if (hardm == 0)
                {
                    spr[h].x -= 1;
                } else
                {
                    int hardm = get_hard_play(h, spr[h].x-1, spr[h].y-1);
                    if (hardm == 0)
                    {  
                        spr[h].x -= 1;
                        spr[h].y -= 1;
                    } else
                    {
                        int hardm = get_hard_play(h, spr[h].x-1, spr[h].y-2);
                        if (hardm == 0)
                        {  
                            spr[h].x -= 1;
                            spr[h].y -= 2;
                        } else
                        {
                            int hardm = get_hard_play(h, spr[h].x, spr[h].y+1);
                            if (hardm == 0)
                            {  

                                spr[h].y += 1;
                            } else
                            {
                                int hardm = get_hard_play(h, spr[h].x+1, spr[h].y+1);
                                if (hardm == 0)
                                {  
                                    spr[h].x += 1;
                                    spr[h].y += 1;
                                } 

                            }

                        }

                    }


                }

            }

            if (spr[h].seq == spr[h].base_walk + 3)
            {
                int hardm = get_hard_play(h, spr[h].x+1, spr[h].y);
                if (hardm == 0)
                {  
                    spr[h].x += 1;

                } else
                {
                    int hardm = get_hard_play(h, spr[h].x+1, spr[h].y-1);
                    if (hardm == 0)
                    {  
                        spr[h].x += 1;
                        spr[h].y -= 1;
                    } else
                    {
                        int hardm = get_hard_play(h, spr[h].x+1, spr[h].y-2);
                        if (hardm == 0)
                        {  
                            spr[h].x += 1;
                            spr[h].y -= 2;
                        } else
                        {
                            int hardm = get_hard_play(h, spr[h].x, spr[h].y+1);
                            if (hardm == 0)
                            {  

                                spr[h].y += 1;
                            } else
                            {
                                int hardm = get_hard_play(h, spr[h].x-1, spr[h].y+1);
                                if (hardm == 0)
                                {  
                                    spr[h].x -= 1;
                                    spr[h].y += 1;
                                }

                            }

                        }

                    }


                }

            }



        }



smoothend:;

}

bool transition(void)
{
    Common::Rect rcRect;
    HRESULT             ddrval; 
    //we need to do our fancy screen transition
    int dumb = 5;
    //if (fps_final < 30) dumb = 50;
    dumb = fps_final * 2;

    move_counter += dumb;


    if (no_transition)
    {    

        total_trigger = false;
        move_screen = 0;
        move_counter = 0;
        trig_man = 0;
        //draw_map();
        return(false);
    }   


    //total_trigger = false;
    //return(false);

    if (move_screen == 4)
    {
        //move_counter =+ 20;

        if (move_counter > 598) move_counter = 598;


        rcRect.left = 0;
        rcRect.right = 600 - move_counter; 
        rcRect.top = 0;
        rcRect.bottom = 400;

        ddrval = lpDDSBack->BltFast( move_counter+20, 0, lpDDSTrick,
            &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
        if (ddrval != DD_OK) dderror(ddrval);

        rcRect.left = 600 -  move_counter; ;
        rcRect.right = 600;
        rcRect.top = 0;
        rcRect.bottom = 400;

        ddrval = lpDDSBack->BltFast( 20, 0, lpDDSTrick2,
            &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);




        if (ddrval != DD_OK) dderror(ddrval);

        if (move_counter >= 595)
        {
            total_trigger = false;
            move_screen = 0;
            move_counter = 0;
            trig_man = 0;
            //draw_map();
            return(false);
        }

        return(true);
    }

    if (move_screen == 6)
    {
        //move_counter =+ 20;


        if (move_counter > 598) move_counter = 598;
        rcRect.left = move_counter;
        rcRect.right = 600; //redink1 fix so entire screen scrolls 
        rcRect.top = 0;
        rcRect.bottom = 400; //redink1 fix so entire screen scrolls

        ddrval = lpDDSBack->BltFast(20, 0, lpDDSTrick,
            &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
        if (ddrval != DD_OK) dderror(ddrval);

        rcRect.left = 0;
        rcRect.right = move_counter;
        rcRect.top = 0;
        rcRect.bottom = 400; //redink1 fix so entire screen scrolls

        ddrval = lpDDSBack->BltFast( 620 - move_counter, 0, lpDDSTrick2,
            &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

        if (ddrval != DD_OK) dderror(ddrval);

        if (move_counter >= 595)
        {
            total_trigger = false;
            move_screen = 0;
            move_counter = 0;
            trig_man = 0;
            //draw_map();
            return(false);
        }

        return(true);
    }


    if (move_screen == 8)
    {
        //move_counter =+ 20;

        if (move_counter > 398) move_counter = 398;


        rcRect.left = 0;
        rcRect.right = 600; //redink1 fix so entire screen scrolls

        rcRect.top = 0;
        rcRect.bottom = 400 - move_counter; //redink1 fix so entire screen scrolls

        ddrval = lpDDSBack->BltFast( 20, move_counter, lpDDSTrick,
            &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
        if (ddrval != DD_OK) dderror(ddrval);

        rcRect.left = 0;
        rcRect.right = 600; //redink1 fix so entire screen scrolls
        rcRect.top = 400 -  move_counter; ; //redink1 fix so entire screen scrolls
        rcRect.bottom = 400; //redink1 fix so entire screen scrolls

        ddrval = lpDDSBack->BltFast( 20, 0, lpDDSTrick2,
            &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);




        if (ddrval != DD_OK) dderror(ddrval);

        if (move_counter >= 398)
        {
            total_trigger = false;
            move_screen = 0;
            move_counter = 0;
            trig_man = 0;
            //draw_map();
            return(false);
        }

        return(true);
    }



    if (move_screen == 2)
    {
        //move_counter =+ 20;


        if (move_counter > 398) move_counter = 398;
        rcRect.left = 0;

        rcRect.right = 600;  //redink1 fix so entire screen scrolls
        rcRect.top = move_counter;
        rcRect.bottom = 400; //redink1 fix so entire screen scrolls

        ddrval = lpDDSBack->BltFast(20, 0, lpDDSTrick,
            &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
        if (ddrval != DD_OK) dderror(ddrval);

        rcRect.left = 0;
        rcRect.right = 600; //redink1 fix so entire screen scrolls

        rcRect.top = 0;
        rcRect.bottom = move_counter;

        ddrval = lpDDSBack->BltFast( 20, 400 - move_counter, lpDDSTrick2, //redink1 fix so entire screen scrolls
            &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

        if (ddrval != DD_OK) dderror(ddrval);

        if (move_counter >= 398)
        {
            total_trigger = false;
            move_screen = 0;
            move_counter = 0;
            trig_man = 0;
            //draw_map();
            return(false);
        }

        return(true);
    }




    return(false);
}




int find_sprite(int block)
{

    for (int k = 1; k <= last_sprite_created; k++)
    {
        if (spr[k].sp_index == block)
        {
            return(k);
        }

    }

    return(0);

}

int special_block(int block, int h)
{

    if (pam.sprite[block].prop == 1)
    {
        //they touched a warp       

        if (pam.sprite[block].sound == 0)
            SoundPlayEffect( 7,12000, 0 , 0,0); else
            SoundPlayEffect( pam.sprite[block].sound,22050, 0 , 0,0);

        if (pam.sprite[block].parm_seq != 0)
        {
            //we'll also play an animation here

            int sprite = find_sprite(block);
            if (sprite > 0)
            {
                spr[sprite].seq = pam.sprite[block].parm_seq;
                process_warp = block;
            }
            return(1);
        }
        process_warp = block;   
        /*spr[1].x = pam.sprite[block].warp_x;
        spr[1].y = pam.sprite[block].warp_y;
        *pmap = pam.sprite[block].warp_map; 

        load_map(map.loc[pam.sprite[block].warp_map]);
        draw_map();
        */
        return(1); //redraw screen with fade
    }
    return(0);
}

void CyclePalette()
{
    //redink1 truecolor fadedown...
    if (truecolor)
    {
        process_downcycle = true;
    }
    else
    {
        int                 reg[15];
        int                 k;
        bool done_this_time = true;     

        if(lpDDPal->GetEntries(0,0,256,pe)!=DD_OK)
        {
            Msg("error with getting entries");
            return;
        }
        int lValue = (thisTickCount - lastTickCount) / 2;
        for (int kk = 1; kk < 256; kk++)
        {
            if (pe[kk].peBlue != 0)
            {
                done_this_time = false;
                if (pe[kk].peBlue > lValue)
                    pe[kk].peBlue -= lValue;
                else
                    pe[kk].peBlue = 0;
            }

            if (pe[kk].peGreen != 0)
            {
                done_this_time = false;

                if (pe[kk].peGreen > lValue)
                    pe[kk].peGreen -= lValue;
                else
                    pe[kk].peGreen = 0;

            }
            if (pe[kk].peRed != 0)
            {
                done_this_time = false;

                if (pe[kk].peRed > lValue)
                    pe[kk].peRed -= lValue;
                else
                    pe[kk].peRed = 0;
            }
        }

        lpDD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN,NULL);


        if(lpDDPal->SetEntries(0,0,256,pe) !=DD_OK)
        {
            Msg("error with setting entries");
            return;
        }


        if (process_downcycle)  
        {
            if  (thisTickCount > cycle_clock)
            {
                process_downcycle = false;

                if (cycle_script != 0)
                {
                    int junk = cycle_script;
                    cycle_script = 0;
                    run_script(junk);


                }
            }
        }
    }
}


void up_cycle(void)
{
    //redink1 added this for true-color fade support
    if (truecolor)
    {
        process_upcycle = true;
    }
    else
    {
        bool donethistime = true;

        if(lpDDPal->GetEntries(0,0,256,pe)!=DD_OK)
        {
            Msg("error with getting entries");
            return;
        }

        //redink1 changed <= to < so it doesn't modify stuff it should not (i.e. fgold, somehow!?)
        for (int kk = 1; kk < 256; kk++)
        {


            if (pe[kk].peBlue != real_pal[kk].peBlue)
            {
                if (pe[kk].peBlue > 246) pe[kk].peBlue++; else
                    pe[kk].peBlue += 10;
                donethistime = false;
            }

            if (pe[kk].peBlue > real_pal[kk].peBlue) pe[kk].peBlue = real_pal[kk].peBlue;


            if (pe[kk].peGreen != real_pal[kk].peGreen)
            {

                if (pe[kk].peGreen > 246) pe[kk].peGreen++; else
                    pe[kk].peGreen += 10;
                donethistime = false;
            }

            if (pe[kk].peGreen > real_pal[kk].peGreen) pe[kk].peGreen = real_pal[kk].peGreen;

            if (pe[kk].peRed != real_pal[kk].peRed)
            {
                if (pe[kk].peRed> 246) pe[kk].peRed++; else
                    pe[kk].peRed += 10;
                donethistime = false;
            }

            if (pe[kk].peRed > real_pal[kk].peRed) pe[kk].peRed = real_pal[kk].peRed;


        }


        lpDD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN,NULL);

        if(lpDDPal->SetEntries(0,0,256,pe) !=DD_OK)
        {
            Msg("error with setting entries");
            //     return;
        }

        if (process_upcycle) if (donethistime)

        {
            process_upcycle = false;


            if (cycle_script != 0)
            {
                int junk = cycle_script;
                cycle_script = 0;   
                run_script(junk);

            }

        }
    }

}
void draw_box(Common::Rect box, int color)
{
    DDBLTFX     ddbltfx;

    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = color;


    ddrval = lpDDSBack->Blt(&box ,NULL, NULL, DDBLT_COLORFILL| DDBLT_WAIT, &ddbltfx);


}

//redink1 added for true-color fade...
WORD GetMaskPos( DWORD dwMask )
{
    WORD wPos = 0;
    while( !(dwMask & (1 << wPos)) ) wPos++;
    return wPos;
}

//redink1 added for true-color fade...
WORD GetNumberOfBits( DWORD dwMask )
{
    WORD wBits = 0;
    while( dwMask )
    {
        dwMask = dwMask & ( dwMask - 1 );
        wBits++;
    }
    return wBits;
}

//redink1 added for true-color fade...
DDSURFACEDESC sdBack;

void ApplyFade16( unsigned short aValue, unsigned short* aBuffer, unsigned int aPitch )
{
    unsigned int lJump( aPitch - 640 * 2 );
    for ( unsigned short lYCoordinate( 0 ); lYCoordinate < 480; lYCoordinate++ )
    {
        for ( unsigned short lXCoordinate( 0 ); lXCoordinate < 640; lXCoordinate++ )
        {
            *aBuffer = m16BitFadeValues[*aBuffer][aValue];
            aBuffer += 1;
        }
        aBuffer += lJump;
    }
    return;
}

void ApplyFade24( unsigned char aValue, unsigned char* aBuffer, unsigned int aPitch )
{
    unsigned int lJump( aPitch - 640 * 3 );
    for ( unsigned short lYCoordinate( 0 ); lYCoordinate < 480; lYCoordinate++ )
    {
        for ( unsigned short lXCoordinate( 0 ); lXCoordinate < 640; lXCoordinate++ )
        {
            if ( *((DWORD*)aBuffer) != 0xFFFFFF )
            {
                *aBuffer = m8BitFadeValues[*aBuffer][aValue];
                aBuffer += 1;
                *aBuffer = m8BitFadeValues[*aBuffer][aValue];
                aBuffer += 1;
                *aBuffer = m8BitFadeValues[*aBuffer][aValue];
                aBuffer += 1;
            }
            else
            {
                aBuffer += 3;
            }
        }
        aBuffer += lJump;
    }
    return;
}

void ApplyFade32( register unsigned char aValue, register unsigned char* aBuffer, unsigned int aPitch )
{
    unsigned int lJump( aPitch - 640 * 4 );
    for ( register unsigned short lYCoordinate( 0 ); lYCoordinate < 480; lYCoordinate++ )
    {
        for ( register unsigned short lXCoordinate( 0 ); lXCoordinate < 640; lXCoordinate++ )
        {
            if ( *((DWORD*)aBuffer) != 0xFFFFFF )
            {
                // Blue
                *aBuffer = m8BitFadeValues[*aBuffer][aValue];
                aBuffer += 1;

                // Green
                *aBuffer = m8BitFadeValues[*aBuffer][aValue];
                aBuffer += 1;

                // Red
                *aBuffer = m8BitFadeValues[*aBuffer][aValue];
                aBuffer += 2;
            }
            else
            {
                aBuffer += 4;
            }
        }
        aBuffer += lJump;
    }
    return;
}

//redink1 and Invertigo fix for windowed/high color mode
void flip_it(void)
{
    DDBLTFX     ddbltfx;

    Common::Rect rcRectSrc;    Common::Rect rcRectDest;
    POINT p;


    /*int timer = GetTickCount() + 50;
    while(GetTickCount() < timer)
    {
    }
    */

    //redink1 fix for true-color transition
    //The idea is to apply the fade to the backbuffer right before the main flip/blt.
    if (truecolor && (process_downcycle || process_upcycle || bFadedDown))
    {
        Common::Rect r;
        r.bottom = 480;
        r.left = 0;
        r.right = 640;
        r.top = 0;

        //Make sure we're not 'stuck'... i.e. fade down when already black, or fade up when not black
        if (process_downcycle && bFadedDown || process_upcycle && !bFadedDown)
        {
            if (process_downcycle)
                process_downcycle = false;
            if (process_upcycle)
            {
                process_upcycle = false;
                if ( mSwapped )
                {
//                    if ( windowed )
//                    {
                        lpDDSFade->BltFast( 0, 0, lpDDSBack, &r, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
                        LPDIRECTDRAWSURFACE lTemp;
                        lTemp = lpDDSBack;
                        lpDDSBack = lpDDSFade;
                        lpDDSFade = lTemp;
/*                    }
                    else
                    {
                        DDSURFACEDESC ddsd;
                        lpDDSBack->GetSurfaceDesc( &ddsd );
                        ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

                        LPDIRECTDRAWSURFACE lTemp;
                        ddrval = lpDDSPrimary->GetAttachedSurface( &ddsd.ddsCaps, &lTemp );

                        lTemp->BltFast( 0, 0, lpDDSBack, &r, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

                        lpDDSBack->Release();
                        lpDDSBack = lTemp;
                    }
*/                    mSwapped = false;
                }
            }
            if (cycle_script != 0)
            {
                int junk = cycle_script;
                cycle_script = 0;
                run_script(junk);   
            }
        }
        if ( process_downcycle || bFadedDown || process_upcycle )
        {
            if ( mSwapped == false )
            {
//                if ( windowed )
//                {
                    lpDDSFade->BltFast( 0, 0, lpDDSBack, &r, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
                    LPDIRECTDRAWSURFACE lTemp;
                    lTemp = lpDDSBack;
                    lpDDSBack = lpDDSFade;
                    lpDDSFade = lTemp;
/*                }
                else
                {
                    DDSURFACEDESC ddsd;
                    lpDDSBack->GetSurfaceDesc( &ddsd );
                    ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER | DDSCAPS_SYSTEMMEMORY;

                    lpDDSBack->Release();

                    LPDIRECTDRAWSURFACE lTemp;
                    ddrval = lpDDSPrimary->GetAttachedSurface( &ddsd.ddsCaps, &lTemp );

                    lTemp->BltFast( 0, 0, lpDDSBack, &r, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

                    lpDDSBack->Release();
                    lpDDSBack = lTemp;
                }
*/                mSwapped = true;
            }

            unsigned char lFadeValue = static_cast<unsigned char>( 30.0f * ( static_cast<float>( iTrueColorFadeTime ) / 400.0f ) );

            if ( lFadeValue >= 30 )
            {
                iTrueColorFadeTime = 0;
                if ( process_downcycle == true )
                {
                    bFadedDown = true;
                }
                else if ( process_upcycle == true )
                {
                    bFadedDown = false;
                }
                lFadeValue = 29;
            }

            if ( process_upcycle == true )
            {
                lFadeValue = 29 - lFadeValue;
            }
            else if ( bFadedDown == true )
            {
                lFadeValue = 29;
            }

            //Setup surface description data-structure
            memset(&sdBack, 0, sizeof(DDSURFACEDESC));
            sdBack.dwSize = sizeof(DDSURFACEDESC);  

            //Lock our surfaces
            lpDDSBack->Lock(NULL, &sdBack, DDLOCK_DONOTWAIT, NULL);

            //Copy backbuffer to fade surface
//            HRESULT h = lpDDSFade->BltFast( 0, 0, lpDDSBack, &r, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);

            //Lock our surfaces
//            lpDDSFade->Lock(NULL, &sdBack, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL);

            if ( mColorDepth == ColorDepth16Bit_565 ||  mColorDepth == ColorDepth16Bit_555 )
            {
                unsigned short* lBuffer = (unsigned short*)sdBack.lpSurface;
                ApplyFade16( lFadeValue, lBuffer, sdBack.lPitch );
            }
            else if ( mColorDepth == ColorDepth24Bit )
            {
                unsigned char* lBuffer = (unsigned char*)sdBack.lpSurface;
                ApplyFade24( lFadeValue, lBuffer, sdBack.lPitch );
            }
            else if ( mColorDepth == ColorDepth32Bit )
            {
                unsigned char* lBuffer = (unsigned char*)sdBack.lpSurface;
                ApplyFade32( lFadeValue, lBuffer, sdBack.lPitch );
            }

            iTrueColorFadeTime += fps_final;

            lpDDSBack->Unlock( NULL );

            if ( !windowed )
            {
                lpDDSFade->BltFast( 0, 0, lpDDSBack, &r, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
            }

//            lpDDSFade->Unlock( NULL );
//            h = lpDDSBack->BltFast( 0, 0, lpDDSFade, &r, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
        }
    }

    if (!windowed)
    {



        while( 1 )
        {
            ddrval = lpDDSPrimary->Flip(NULL,DDFLIP_WAIT );
            if( ddrval == DD_OK )
            {
                break;
            }
            if( ddrval == DDERR_SURFACELOST )
            {

                restoreAll();
                if( ddrval != DD_OK )
                {
                    break;
                }
            }
            if( ddrval != DDERR_WASSTILLDRAWING )
            {


            }
        } 

    } else
    {
        //windowed mode, no flipping         
        p.x = 0; p.y = 0;    
        ClientToScreen(hWndMain, &p);
        GetClientRect(hWndMain, &rcRectDest);

        //rcRectDest.top += winoffset;
        rcRectDest.bottom = 480;
        rcRectDest.right = 640;

        OffsetRect(&rcRectDest, p.x, p.y);
        SetRect(&rcRectSrc, 0, 0, 640, 480);

        ddbltfx.dwSize = sizeof(ddbltfx);

        ddbltfx.dwDDFX = DDBLTFX_NOTEARING;

        lpDD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL);
        ddrval = lpDDSPrimary->Blt( &rcRectDest, lpDDSBack, &rcRectSrc, DDBLT_DDFX | DDBLT_WAIT, &ddbltfx);
    }



}



void run_through_tag_list(int h, int strength)
{
    Common::Rect box;
    int amount, amounty;

    for (int i = 1; i <= last_sprite_created; i++)
    {
        if (spr[i].active) if (i != h) if
            (! ( (spr[i].nohit == 1) && (spr[i].script == 0)) )
        {

            CopyRect(&box, &k[getpic(i)].hardbox);
            OffsetRect(&box, spr[i].x, spr[i].y);

            //InflateRect(&box, 10,10);

            box.right += 5;
            box.left -= 5;
            box.top -= 5;
            box.bottom += 10;
            if (spr[h].range == 0)      
                amount = 28; else amount = spr[h].range;

            if (spr[h].range == 0)      

                amounty = 36; else amounty = (spr[h].range + (spr[h].range / 6));

            int range_amount = spr[h].range / 8;

            if (spr[h].dir == 6)
            {
                box.top -= 10;
                box.bottom += 10;
                if (spr[h].range != 0) box.top -= range_amount;
                if (spr[h].range != 0) box.bottom += range_amount;


                box.left -= amount;
            }

            if (spr[h].dir == 4)
            {
                box.right += amount;

                box.top -= 10;
                box.bottom += 10;
                if (spr[h].range != 0) box.top -= range_amount;
                if (spr[h].range != 0) box.bottom += range_amount;

            }


            if (spr[h].dir == 2)
            {
                box.right += 10;
                box.left -= 10;
                box.top -= amounty;

                if (spr[h].range != 0) box.right += range_amount;
                if (spr[h].range != 0) box.left -= range_amount;

            }

            if (spr[h].dir == 8)
            {
                box.right += 10;
                box.left -= 10;
                box.bottom += amounty;

                if (spr[h].range != 0) box.right += range_amount;
                if (spr[h].range != 0) box.right -= range_amount;

            }

            if (debug_mode) draw_box(box, 33);

            if (inside_box(spr[h].x, spr[h].y, box))
            {   
                //redink1 addition for fixing missle_source problems
                *pmissle_source = h;
                if (spr[i].nohit == 1)
                {
                    if (spr[i].script > 0)
                    {
                        //if (  (spr[i].brain == 0) | (spr[i].brain == 5) | (spr[i].brain == 6) | (spr[i].brain == 7))
                        *penemy_sprite = h;

                        if (  (spr[i].base_attack != -1) || (spr[i].touch_damage > 0))
                            spr[i].target = h;   

                        if (locate(spr[i].script, "HIT"))
                        {
                            kill_returning_stuff(spr[i].script);
                            run_script(spr[i].script);
                        }


                    }


                } else
                {
                    //hit this personb/thing
                    if (spr[h].attack_hit_sound == 0)
                    {
                        SoundPlayEffect( 9,22050, 0 ,0,0);
                    } else
                    {
                        SoundPlayEffect( spr[h].attack_hit_sound,spr[h].attack_hit_sound_speed, 0 ,0,0);
                    }
                    if (  (spr[i].base_attack != -1) || (spr[i].touch_damage > 0))
                        spr[i].target = h;   
                    if (spr[h].strength == 0)
                    {

                    } else
                    {
                        if (  (spr[i].hitpoints > 0) || (i == 1) )
                        {

                            spr[i].last_hit = h; 
                            if ( hurt_thing(i, (spr[h].strength / 2) + ((rand() % ((spr[h].strength+1) / 2))+1), 0) > 0)
                                random_blood(spr[i].x, spr[i].y-40, i); //redink1
                        }

                    }
                    if (spr[i].script > 0)
                    {
                        //if (  (spr[i].brain == 0) | (spr[i].brain == 5) | (spr[i].brain == 6) | (spr[i].brain == 7))
                        spr[i].last_hit = h;    
                        *penemy_sprite = h;
                        if (  (spr[i].base_attack != -1) || (spr[i].touch_damage > 0))
                            spr[i].target = h;   

                        if (locate(spr[i].script, "HIT"))
                        {
                            kill_returning_stuff(spr[i].script);
                            run_script(spr[i].script);
                        }

                    }

                }

            }

        }

    }

}



void run_through_tag_list_push(int h)
{
    Common::Rect box;

    for (int i = 1; i <= last_sprite_created; i++)
    {
        if (spr[i].active) if (i != h) if
            ((spr[i].script != 0) )
        {

            CopyRect(&box, &k[getpic(i)].hardbox);
            OffsetRect(&box, spr[i].x, spr[i].y);

            //InflateRect(&box, 10,10);

            box.right += 2;
            box.left -= 2;
            box.top -= 2;
            box.bottom += 2;
            //draw_box(box, 33);

            if (inside_box(spr[h].x, spr[h].y, box))
            {   
                if (locate(spr[i].script, "PUSH")) run_script(spr[i].script);
            }

        }

    }

}




void run_through_touch_damage_list(int h)
{
    Common::Rect box;
    int amount, amounty;
    for (int i = 1; i <= last_sprite_created; i++)
    {
        if (spr[i].active) if (i != h) if
            ((spr[i].touch_damage != 0) )
        {

            if (spr[i].touch_damage != -1) if (spr[h].notouch) return;
            CopyRect(&box, &k[getpic(i)].hardbox);
            OffsetRect(&box, spr[i].x, spr[i].y);

            //InflateRect(&box, 10,10);

            box.right += 2;
            box.left -= 2;
            box.top -= 2;
            box.bottom += 2;
            if (debug_mode)     
                draw_box(box, 33);


            if (inside_box(spr[h].x, spr[h].y, box))
            {   

                if ((spr[i].touch_damage == -1) && (spr[i].script != 0))
                {
                    if (locate(spr[i].script, "TOUCH")) run_script(spr[i].script);
                } else
                {
                    if (spr[i].touch_damage == -1)
                    {
                        Msg("Sprites touch damage is set to -1 but there is no script set!");
                    } else
                    {
                        //lets hurt the guy

                        spr[h].notouch = true;
                        spr[h].notouch_timer = thisTickCount+400;
                        spr[h].last_hit = i;
                        if (spr[i].script != 0)
                            if (locate(spr[i].script, "TOUCH")) run_script(spr[i].script);
                        if (hurt_thing(h, spr[i].touch_damage, 0) > 0)
                            random_blood(spr[h].x, spr[h].y-40, h);


                    }

                }



            }


        }

    }

}




void process_warp_man(void)
{
    Common::Rect box_crap;
    DDBLTFX     ddbltfx;

    int sprite = find_sprite(process_warp);


    if (spr[sprite].seq == 0)
    {
        process_count++;
        if (!truecolor || truecolor && process_count == 1) //redink1 Limit palette cycles for true color mode
            CyclePalette();
        if (process_count > 5 && !truecolor || truecolor && process_downcycle == false) //redink1 more limits for fade down stuff
        {
            ddbltfx.dwSize = sizeof(ddbltfx);

            ddbltfx.dwFillColor = 0;
            SetRect(&box_crap, 0,0,640,480);

            ddrval = lpDDSBack->Blt(&box_crap ,NULL, NULL, DDBLT_COLORFILL| DDBLT_WAIT, &ddbltfx);

            flip_it();

            process_count = 0;
            int block = process_warp;
            update_screen_time();
            spr[1].x = pam.sprite[block].warp_x;
            spr[1].y = pam.sprite[block].warp_y;
            *pmap = pam.sprite[block].warp_map; 

            //redink1 change so map indicator is correct on warp.
            if (map.indoor[pam.sprite[block].warp_map] == 0)
                play.last_map = pam.sprite[block].warp_map;

            load_map(map.loc[pam.sprite[block].warp_map]);

            draw_map_game();

            process_upcycle = true;
            process_warp = 0;
        }

    } else
    {
        process_count = 0;      

    }

}

void one_time_brain(int h)
{

    //goes once then draws last frame to background

    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }

    if (spr[h].follow > 0)
    {
        process_follow(h);
    }


    if (spr[h].seq == 0)
    {
        draw_sprite_game(lpDDSTwo,h);
        spr[h].active = false;          
        return;
    }

    changedir(spr[h].dir,h,-1);
    automove(h);

}

void one_time_brain_for_real(int h)
{

    if (spr[h].move_active) 
    {
        process_move(h);
    }


    if (spr[h].follow > 0)
    {
        process_follow(h);
    }


    if (spr[h].seq == 0)
    {

        spr[h].active = false;          
        return;
    }
    if (spr[h].dir > 0)
    {
        changedir(spr[h].dir,h,-1);
        automove(h);
    }
}


void scale_brain(int h)
{

    if (spr[h].size == spr[h].brain_parm)
    {
        spr[h].active = false;


        return;
    }

    int num = 5 * (base_timing / 4);



    if (spr[h].size > spr[h].brain_parm)
    {
        if (spr[h].size - num < spr[h].brain_parm) num = spr[h].size - spr[h].brain_parm;
        spr[h].size -= num;
    }

    if (spr[h].size < spr[h].brain_parm) 
    {
        if (spr[h].size + num > spr[h].brain_parm) num = spr[h].brain_parm - spr[h].size;   
        spr[h].size += num;
    }
    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }



    if (spr[h].dir > 0)
    {
        changedir(spr[h].dir,h,-1);
        automove(h);
    }


}



void repeat_brain(int h)
{

    if (spr[h].move_active) 
    {
        process_move(h);
        //      return;
    }


    if (spr[h].seq_orig == 0) if (spr[h].sp_index != 0) 
    {
        spr[h].seq_orig = pam.sprite[spr[h].sp_index].seq;
        spr[h].frame = pam.sprite[spr[h].sp_index].frame;
        spr[h].wait = 0;

        //pam.sprite[spr[h].sp_index].frame;

    }

    if (spr[h].seq == 0) spr[h].seq = spr[h].seq_orig;

}


void text_brain(int h)
{



    if (  (spr[h].damage == -1) && (spr[h].owner != 1000))
    {

        if (spr[spr[h].owner].active == false)
        {
            //msg("Killing text brain %d, because owner %d is dead.",h, spr[h].owner);
            spr[h].active = false;
            return;
        }

        //give this text the cords from it's owner sprite
        spr[h].x = spr[spr[h].owner].x - spr[h].strength;


        spr[h].y = spr[spr[h].owner].y - spr[h].defense;

        if (spr[h].x < 1) spr[h].x = 1;

        if (spr[h].y < 1) spr[h].y = 1;


    } else
    {
        //Msg("automoving %d.. ", h);

        if (spr[h].move_active) 
        {
            process_move(h);
            return;
        }


        automove(h);
    }

}


void process_talk()
{

    int px = 48, py = 44;

    int sx = 184;
    int sy = 94, sy_hold, sy_ho;
    int spacing = 12;
    int curxl = 126;
    int curxr = 462;
    int curyr = 200;
    int curyl = 200;

    int y_last = 0, y_hold = 0, y_ho; 
    HDC         hdc;
    Common::Rect rcRect;
    int i;
    int x_depth = 335;
    if (talk.newy != -5000)
        sy = talk.newy;

    sy_hold = sy;
    sy_ho = sy;

    check_seq_status(30);

    int fake_page;
again:
    ddrval = lpDDSBack->BltFast( px, py, k[seq[30].frame[2]].k,
        &k[seq[30].frame[2]].box  , DDBLTFAST_SRCCOLORKEY  );

    if (ddrval == DDERR_WASSTILLDRAWING) goto again;

again2: 
    ddrval = lpDDSBack->BltFast( px+169, py+42, k[seq[30].frame[3]].k,
        &k[seq[30].frame[3]].box  , DDBLTFAST_SRCCOLORKEY  );
    if (ddrval == DDERR_WASSTILLDRAWING) goto again2;

again3:
    ddrval = lpDDSBack->BltFast( px+169+180, py+1, k[seq[30].frame[4]].k,
        &k[seq[30].frame[4]].box  , DDBLTFAST_SRCCOLORKEY  );

    if (ddrval == DDERR_WASSTILLDRAWING) goto again3;



    int talk_hold = talk.cur;  
    if (sjoy.rightd) talk.cur++;
    if (sjoy.downd) talk.cur++;
    if (sjoy.upd) talk.cur--;
    if (sjoy.leftd) talk.cur--;

    if (play.mouse > 20)
    {
        talk.cur++;
        play.mouse = 0;
    }

    if (play.mouse < -20)
    {
        talk.cur--;
        play.mouse = 0;
    }


    if (talk_hold != talk.cur)
    {
        if (talk.cur >= talk.cur_view) if (talk.cur <= talk.cur_view_end) 
            SoundPlayEffect(11, 22050,0,0,0);
    }

    if (lpDDSBack->GetDC(&hdc) == DD_OK)
    {      

        SelectObject (hdc, hfont_small);
        SetBkMode(hdc, TRANSPARENT); 



        if (strlen(talk.buffer) > 0)
        {

            SetRect(&rcRect,sx,94,463,400);
            if (talk.newy != -5000) rcRect.bottom = talk.newy+15;



            SetTextColor(hdc,RGB(8,14,21));
            DrawText(hdc,talk.buffer,strlen(talk.buffer),&rcRect,DT_VCENTER | DT_CENTER | DT_WORDBREAK);    

            if (talk.color >= 1 && talk.color <= 15)
            {
                SetTextColor(hdc,RGB(font_colors[talk.color].red, font_colors[talk.color].green, font_colors[talk.color].blue));
            }
            else
            {
                SetTextColor(hdc,RGB(255,255,255));
            }

            /*if (talk.color == 1) SetTextColor(hdc,RGB(49,90,140)); 
            else    if (talk.color == 2) SetTextColor(hdc,RGB(131,181,74)); 
            else if (talk.color == 3) SetTextColor(hdc,RGB(99,242,247)); 

            else    if (talk.color == 4) SetTextColor(hdc,RGB(255,156,74)); //right


            else        if (talk.color == 5) SetTextColor(hdc,RGB(222,173,255)); 
            else    if (talk.color == 6) SetTextColor(hdc,RGB(244,188,73)); //right
            else    if (talk.color == 7) SetTextColor(hdc,RGB(173,173,173)); //right
            else    if (talk.color == 8) SetTextColor(hdc,RGB(85,85,85)); //right
            else    if (talk.color == 9) SetTextColor(hdc,RGB(148,198,255)); //right

            else            if (talk.color == 10) SetTextColor(hdc,RGB(0,255,0)); 
            else    if (talk.color == 13) SetTextColor(hdc,RGB(255,132,132)); 
            else    if (talk.color == 14) SetTextColor(hdc,RGB(255,255,2)); 
            else    if (talk.color == 15) SetTextColor(hdc,RGB(255,255,255)); 
            else
                SetTextColor(hdc,RGB(255,255,2));*/

            OffsetRect(&rcRect, 1, 1);
            DrawText(hdc,talk.buffer,strlen(talk.buffer),&rcRect,DT_VCENTER | DT_CENTER | DT_WORDBREAK);    

            SetTextColor(hdc,RGB(8,14,21));
        }




        //tabulate distance needed by text, LORDII experience helped here
recal: 
        for (i = talk.cur_view; i < talk.last; i++)
        {
            SetRect(&rcRect,sx,y_hold,463,x_depth+100);
            y_hold =  DrawText(hdc,talk.line[i],lstrlen(talk.line[i]),&rcRect,DT_CALCRECT | DT_CENTER | DT_WORDBREAK);  
            sy_hold += y_hold;   

            //Msg("Sy_hold = %d (%d)", sy_hold,i);

            if (sy_hold > x_depth) 
            {

                talk.cur_view_end = i-1;
                //Msg("Sy is over, sp cur_view is %d ", talk.cur_view_end);
                goto death;
            }
        }

        talk.cur_view_end = i;

        if (talk.cur_view == 1) if (talk.cur_view_end == talk.last)
        {

            //Msg("Small enough to fit on one screen, lets center it!");
            sy += ( (x_depth - sy_hold) / 2) - 20;

        }
death:


        if (talk.cur > talk.last) 
        {
            SoundPlayEffect(11, 22050,0,0,0);

            talk.cur = 1;

        }
        if (talk.cur < 1) 
        {
            SoundPlayEffect(11, 22050,0,0,0);

            talk.cur = talk.last;
        }


        //if (talk.cur_view_end != talk.last)
        {
            //Msg("Talkcur is %d, talk cur view is %d", talk.cur, talk.cur_view);
            //total options too large for page, lets scroll


            if (talk.cur > talk.cur_view_end) 
            {
                //     Msg("advancing page:  talkcur is %d, changing cur_view to same", talk.cur, talk.cur_view);
                talk.cur_view = talk.cur;
                talk.page ++;

                // Msg("Page advanced to %d. (cur_end is %d, cur is %d)", talk.page,talk.cur_view_end, talk.cur);
                goto fin;
            }



            if (talk.cur < talk.cur_view) 

            {
                //  Msg("Turning back the clock from page %d..", talk.page);

                talk.cur_view = 1;
                // talk.cur = 1;

                talk.page--;
                Msg("Page backed to %d.", talk.page);
                fake_page = 1;
                for (i = 1; i < talk.last; i++)
                {
                    SetRect(&rcRect,sx,sy_ho,463,x_depth);

                    y_ho =  DrawText(hdc,talk.line[i],lstrlen(talk.line[i]),&rcRect,DT_CALCRECT | DT_CENTER | DT_WORDBREAK);    
                    sy_ho += y_ho;   
                    //Msg("adding y_yo %d.. (on %d)", y_ho,i);
                    if (sy_ho > x_depth) 
                    {
                        /*if (fake_page == talk.page)
                        {
                        goto fin;
                        }
                        */
                        fake_page++;      
                        sy_ho = sy+ y_ho;
                        //Msg("Does fake page (%d) match desired page (%d) %d", fake_page, talk.page, i);
                    }
                    if (fake_page == talk.page)
                    {
                        talk.cur_view = i;
                        talk.cur_view_end = talk.cur;
                        //Msg("Going to fin with end being %d, and.cur being %d.  View is %d.",
                        //         talk.cur_view_end, talk.cur, talk.cur_view);
                        goto fin;
                    }

                    //         Msg("Second: Sy is over, sp cur_view is %d", talk.cur_view_end);


                }

                talk.cur_view_end = i;


            }


        }



        //Msg("talk last is %d.  cur_view_end is %d, Cur is %d", talk.last, talk.cur_view_end, talk.cur);

        //   talk.cur_view_end = talk.last;

        for ( i = talk.cur_view; i <= talk.cur_view_end; i++)

        {
            //lets figure out where to draw this line

            SetRect(&rcRect,sx,sy,463,x_depth+100);
            SetTextColor(hdc,RGB(8,14,21));
            DrawText(hdc,talk.line[i],lstrlen(talk.line[i]),&rcRect, DT_CENTER | DT_WORDBREAK);
            OffsetRect(&rcRect,-2,-2);
            DrawText(hdc,talk.line[i],lstrlen(talk.line[i]),&rcRect,DT_CENTER | DT_WORDBREAK);

            OffsetRect(&rcRect,1,1);
            if (i == talk.cur)
            {
                curyl = sy-4;
                curyr = sy-4;


                SetTextColor(hdc,RGB(255,255,255));

            }
            else
                SetTextColor(hdc,RGB(255,255,2));
            y_last =  DrawText(hdc,talk.line[i],lstrlen(talk.line[i]),&rcRect,DT_CENTER | DT_WORDBREAK);    
            sy += y_last;


        }

fin:


        //     dum =  GetTextFace(hdc,100,shit) ;
        lpDDSBack->ReleaseDC(hdc);

        if (talk.timer < thisTickCount)
        {   
            talk.curf++;
            talk.timer = thisTickCount+100;
        }


        if (talk.curf == 0) talk.curf = 1;

        if (talk.curf > 7) talk.curf = 1;
again4:
        ddrval = lpDDSBack->BltFast( curxl, curyl, k[seq[456].frame[talk.curf]].k,
            &k[seq[456].frame[talk.curf]].box  , DDBLTFAST_SRCCOLORKEY  );
        if (ddrval == DDERR_WASSTILLDRAWING) goto again4;

again5:
        ddrval = lpDDSBack->BltFast( curxr, curyr, k[seq[457].frame[talk.curf]].k,
            &k[seq[456].frame[talk.curf]].box  , DDBLTFAST_SRCCOLORKEY  );
        if (ddrval == DDERR_WASSTILLDRAWING) goto again5;


    }


    if ( (sjoy.button[1]) | (mouse1))

    {
        mouse1 = false;
        talk.active = false;
        *presult = talk.line_return[talk.cur];
        SoundPlayEffect(17, 22050,0,0,0);

        if (talk.script != 0) 
        { 
            //we need to continue a script
            run_script(talk.script);

        }
    }

}



void UpdateCursorPosition(int dx, int dy)
{

    /*
    *  Pick up any leftover fuzz from last time.  This is important
    *  when scaling down mouse motions.  Otherwise, the user can
    *  drag to the right extremely slow for the length of the table
    *  and not get anywhere.
    */
    if (spr[1].active) if (spr[1].brain == 13)
    {
        spr[1].x += dx;
        spr[1].y += dy;
        /* Clip the cursor to our client area */

        if (spr[1].x > 640) spr[1].x = 640;
        if (spr[1].y > 480) spr[1].y = 480;
        if (spr[1].x < 0) spr[1].x = 0;
        if (spr[1].y < 0) spr[1].y = 0;
    }
    if (mode == 1) 
    {
        play.mouse += dy;
        //Msg("play mousey is now %d", play.mouse);
    }

}


void  Scrawl_OnMouseInput(void)
{

    mouse1 = false;

    bool fDone = 0;

    while (!fDone) {

        DIDEVICEOBJECTDATA od;

        DWORD dwElements = 1;

        HRESULT hr = g_pMouse->GetDeviceData(
            sizeof(DIDEVICEOBJECTDATA), &od,
            &dwElements, 0);

        if (hr == DIERR_INPUTLOST) {
            /*
            *  We had acquisition, but lost it.  Try to reacquire it.
            *
            *  WARNING!  DO NOT ATTEMPT TO REACQUIRE IF YOU GET
            *  DIERR_NOTACQUIRED!  Otherwise, you're extremely likely
            *  to get caught in an infinite loop:  The acquire will fail,
            *  and you'll get another DIERR_NOTACQUIRED so you'll
            *  try to aquire again, and that'll fail, etc.
            */
            //PostMessage(hwnd, WM_SYNCACQUIRE, 0, 0L);
            //   break;

            g_pMouse->Acquire();

        }

        /* Unable to read data or no data available */
        if (FAILED(hr) || dwElements == 0) {
            // Msg("No mouse data there.");
            break;
        }

        /* Look at the element to see what happened */

        switch (od.dwOfs) {

            /* DIMOFS_X: Mouse horizontal motion */
        case DIMOFS_X: UpdateCursorPosition(od.dwData, 0); break;


            /* DIMOFS_Y: Mouse vertical motion */
        case DIMOFS_Y: UpdateCursorPosition(0, od.dwData); break;

        case DIDFT_BUTTON: if (od.dwData > 0) mouse1 = true; break;

        }

    }

}

void button_brain(int h )
{
    Common::Rect box;
    if (spr[h].move_active) 
    {
        process_move(h);
        return;
    }


    if (spr[h].script == 0) return;

    CopyRect(&box, &k[getpic(h)].hardbox);
    OffsetRect(&box, spr[h].x, spr[h].y);

    if (spr[h].brain_parm == 0)
    {
        if (inside_box(spr[1].x, spr[1].y, box))
        {   
            spr[h].brain_parm = 1;

            if (locate(spr[h].script, "BUTTONON")) 
            {
                run_script(spr[h].script);

                return;
            }

        }

    }
    else
    {
        if (!inside_box(spr[1].x, spr[1].y, box))
        {   
            spr[h].brain_parm = 0;

            if (locate(spr[h].script, "BUTTONOFF")) 
            {

                run_script(spr[h].script);
                return;
            }

        }

    }


}

void draw_item(int num, bool magic, int mseq, int mframe)
{
    int mx = 20;
    int my = 0;
    int vert = 0;
    if (magic == false)
    {
        mx = 260;
        my = 83;

        vert = ((num-1) / 4);
        mx += (((num-1) - (vert * 4)) * (18 + 65));
        my += (vert * (20 + 55));
    } else
    {
        mx = 45;
        my = 83;

        vert = ((num-1) / 2);
        mx += (((num-1) - (vert * 2)) * (18 + 65));
        my += (vert * (20 + 55));


    }

again:

    check_seq_status(mseq);

    if (k[seq[mseq].frame[mframe]].k == NULL) 
    {

        if (!magic)
        {
            Msg("Whups, item %d seq %d frame %d not loaded, killed it",
                num, mseq, mframe);
            play.item[num].active = false;
        } else
        {
            Msg("Whups, magic %d seq %d frame %d not loaded, killed it",
                num, mseq, mframe);
            play.mitem[num].active = false;

        }

        return;
    }

    ddrval = lpDDSBack->BltFast( mx, my, k[seq[mseq].frame[mframe]].k,
        &k[seq[mseq].frame[mframe]].box, DDBLTFAST_SRCCOLORKEY);

    if( ddrval == DDERR_WASSTILLDRAWING ) goto again;


}


void process_item( void )
{

    Common::Rect                rcRect;
    rcRect.left = 0;
    rcRect.top = 0;
    rcRect.right = x;
    rcRect.bottom = y;
    int hor, virt;  



    while( 1 )
    {
        ddrval = lpDDSBack->BltFast( 0, 0, lpDDSTwo,
            &rcRect, DDBLTFAST_NOCOLORKEY);

        if( ddrval == DD_OK )
        {
            break;
        }
        if( ddrval == DDERR_SURFACELOST )
        {
            ddrval = restoreAll();
            break;
        }
        if( ddrval != DDERR_WASSTILLDRAWING )
        {
            dderror(ddrval);
            return;
        }
    }


    check_seq_status(423);
    //lets blit the main screen over it
again:  
    ddrval = lpDDSBack->BltFast( 20, 0, k[seq[423].frame[1]].k,
        &k[seq[423].frame[1]].box, DDBLTFAST_SRCCOLORKEY);

    if( ddrval == DDERR_WASSTILLDRAWING )
        goto again;
    //draw all currently owned items; magic
    for (int i = 1; i < 9; i++)
    {
        if (play.mitem[i].active) draw_item(i, true, play.mitem[i].seq,play.mitem[i].frame);
    }

    //draw all currently owned items; normal
    for ( i = 1; i < 17; i++)
    {
        if (play.item[i].active) draw_item(i, false, play.item[i].seq,play.item[i].frame);

    }

    //draw selection box around armed weapon
    if (*pcur_weapon != 0) if (play.item[*pcur_weapon].active)
        draw_item(*pcur_weapon, false, 423, 4);


    //draw selection box around armed magic
    if (*pcur_magic != 0) if (play.item[*pcur_magic].active)
        draw_item(*pcur_magic, true, 423, 5);


    //draw the selector around it, alternating from 2 to 3
    if (play.curitem < 1) play.curitem = 1;


    if (thisTickCount > item_timer)
    {
        if (item_pic == 2) item_pic = 3; else item_pic = 2;
        item_timer = thisTickCount + 400;

    }
    draw_item(play.curitem, play.item_magic, 423, item_pic);




    if (!play.item_magic)
    {
        hor = (play.curitem - (((play.curitem-1) / 4) * 4));
        virt = ((play.curitem-1) / 4);



        //choosing weapon/item

        if (sjoy.button[1])
        {
            if (play.item[play.curitem].active)
            {
                //arm weapon
                SoundPlayEffect(18, 42050,0,0,0);        
                if (*pcur_weapon != 0)
                {
                    //disarm old weapon
                    if (locate(weapon_script, "DISARM")) run_script(weapon_script);
                }
                //load weapons script
                *pcur_weapon = play.curitem;
                weapon_script = load_script(play.item[*pcur_weapon].name, 1000, false);
                if (locate(weapon_script, "ARM")) run_script(weapon_script);
                if (locate(weapon_script, "ARMMOVIE")) run_script(weapon_script);

                draw_status_all();
            } else
            {
                //can't arm nothing, play sound
            }
        } else
            if (sjoy.rightd) 
            {
                if (hor < 4) play.curitem++;
                SoundPlayEffect(11, 22050,0,0,0);   
            } else
                if (sjoy.leftd) 
                {
                    if (hor > 1)
                    {
                        play.curitem--; 
                        SoundPlayEffect(11, 22050,0,0,0);   

                    }
                    else
                    {
                        SoundPlayEffect(11, 22050,0,0,0);   

                        play.item_magic = true;
                        play.curitem = (virt * 2) + 2;
                        //switch to magic mode
                    }
                } else


                    if (sjoy.downd)
                    {
                        if (virt < 3)
                        {
                            play.curitem += 4;
                            SoundPlayEffect(11, 22050,0,0,0);   

                        }
                    } else

                        if (sjoy.upd)
                        {
                            if (virt > 0)
                            {
                                play.curitem -= 4;
                                SoundPlayEffect(11, 22050,0,0,0);   

                            }
                        }


    } else

    {
        hor = (play.curitem - (((play.curitem-1) / 2) * 2));
        virt = ((play.curitem-1) / 2);

        if (sjoy.button[1])
        {
            if (play.mitem[play.curitem].active)
            {
                //arm magic
                SoundPlayEffect(18, 42050,0,0,0);  
                if (*pcur_magic != 0)
                {
                    //disarm old weapon
                    if (locate(magic_script, "DISARM")) run_script(magic_script);
                }
                //load magics script
                *pcur_magic = play.curitem;
                magic_script = load_script(play.mitem[*pcur_magic].name, 1000, false);
                if (locate(magic_script, "ARM")) run_script(magic_script);
                if (locate(magic_script, "ARMMOVIE")) run_script(magic_script);
                draw_status_all();
            } else
            {
                //can't arm nothing, play sound
            }
        }

        if (sjoy.rightd) 
        {
            if (hor < 2)
            {
                play.curitem++;
                SoundPlayEffect(11, 22050,0,0,0);   

            }
            else
            { 
                play.item_magic = false;
                play.curitem = (virt * 4) +1;
                SoundPlayEffect(11, 22050,0,0,0);   

            }
        } else
            if (sjoy.leftd) 
            {
                if (hor > 1)
                {
                    play.curitem--;
                    SoundPlayEffect(11, 22050,0,0,0);   

                }
                else
                {
                }
            } else


                if (sjoy.downd)
                {
                    if (virt < 3)
                    {
                        play.curitem += 2;
                        SoundPlayEffect(11, 22050,0,0,0);   

                    }
                } else

                    if (sjoy.upd)
                    {
                        if (virt > 0) 
                        {
                            play.curitem -= 2;
                            SoundPlayEffect(11, 22050,0,0,0);   

                        }
                    }

    }
    if (talk.active) process_talk();

    //a special process callbacks for just stuff that was created in this mode? 
    // process_callbacks_special();
    flip_it(); 

    if (sjoy.button[4])
    {
        SoundPlayEffect(17, 22050,0,0,0);

        item_screen = false;
    }


}

void process_animated_tiles( void )
{
    Common::Rect rcRect;
    int cool;
    int flip;
    int pa;

    //process water tiles

    if (water_timer < thisTickCount)
    {

        water_timer = thisTickCount + ((rand() % 2000));

        flip = ((rand() % 2)+1);        




        for (int x=0; x<96; x++)
        {
            //redink1 fix for first broken water tile
            if (pam.t[x].num >= 896) if (pam.t[x].num < (896+128))
            {

                cool = pam.t[x].num / 128;
                pa = pam.t[x].num - (cool * 128);
                rcRect.left = (pa * 50- (pa / 12) * 600);
                rcRect.top = (pa / 12) * 50;
                rcRect.right = rcRect.left + 50;
                rcRect.bottom = rcRect.top + 50;


                lpDDSTwo->BltFast( (x * 50 - ((x / 12) * 600))+playl, (x / 12) * 50, tiles[cool+flip],
                    &rcRect, DDBLTFAST_NOCOLORKEY| DDBLTFAST_WAIT );

            }   
        }

    }

    //end of water processing


    //if (water_timer < thisTickCount)
    {

        //  water_timer = thisTickCount + ((rand() % 2000)+1000);

        if (fire_forward) fire_flip++;
        if (!fire_forward) fire_flip--;

        if (fire_flip < 1)
        {
            fire_flip = 5;
            fire_forward = false;
        }

        //  if (fire_flip > 4)
        //  {
        //  fire_flip = 4;
        //fire_forward = false;
        //}



        for (int x=0; x<96; x++)
        {
            //redink1 fix for first broken fire tile
            if (pam.t[x].num >= 2304) if (pam.t[x].num < (2304+128))
            {

                cool = pam.t[x].num / 128;
                pa = pam.t[x].num - (cool * 128);
                rcRect.left = (pa * 50- (pa / 12) * 600);
                rcRect.top = (pa / 12) * 50;
                rcRect.right = rcRect.left + 50;
                rcRect.bottom = rcRect.top + 50;


                lpDDSTwo->BltFast( (x * 50 - ((x / 12) * 600))+playl, (x / 12) * 50, tiles[cool+fire_flip],
                    &rcRect, DDBLTFAST_NOCOLORKEY| DDBLTFAST_WAIT );

            }   
        }

    }

    //end of water processing



}


void process_show_bmp( void )
{

    Common::Rect rcRect;
    SetRect(&rcRect, 0,0,x, y);

again:
    ddrval = lpDDSBack->BltFast( 0, 0, lpDDSTrick,
        &rcRect, DDBLTFAST_NOCOLORKEY);

    if( ddrval == DDERR_WASSTILLDRAWING ) goto again;


    if (showb.showdot)
    {
        //let's display a nice dot to mark where they are on the map
        int x = play.last_map - 1;


        int mseq = 165;

        showb.picframe++;
        if (showb.picframe > index[mseq].last) showb.picframe = 1;
        int mframe = showb.picframe;
        lpDDSBack->BltFast( (x % 32) * 20, (x / 32) * 20, k[seq[mseq].frame[mframe]].k,
            &k[seq[mseq].frame[mframe]].box, DDBLTFAST_SRCCOLORKEY| DDBLTFAST_WAIT );

    }


    if (  (sjoy.button[1])
        || (sjoy.button[2])
        || (sjoy.button[3])
        || (sjoy.button[4])
        || (sjoy.button[5])
        || (sjoy.button[6])


        )
    {
        showb.active = false;
        if (showb.script != 0)
            run_script(showb.script);
        showb.stime = thisTickCount+2000;
        but_timer = thisTickCount + 200;

        int sprite = say_text_xy("", 1, 440, 0);                                
        spr[sprite].noclip = 1;


        if(lpDDPal->SetEntries(0,0,256,real_pal) !=DD_OK)
        {
            Msg("error with setting entries");
            return;
        }

    }



    /*if (!cd_inserted)
    {


    if (showb.stime > thisTickCount)
    {           
    if (lpDDSBack->GetDC(&hdc) == DD_OK)
    {      
    SelectObject (hdc, hfont_small);
    SetBkMode(hdc, TRANSPARENT); 
    sprintf(msg, "Please wait, checking CD.  (or Alt-Q to abort and exit program)");
    rcRect.left = 0;
    rcRect.top = 430;
    rcRect.right = playx;
    rcRect.bottom = 480;
    SetTextColor(hdc, RGB(255,255,2));
    DrawText(hdc,msg,lstrlen(msg),&rcRect,DT_CENTER | DT_WORDBREAK);
    lpDDSBack->ReleaseDC(hdc);

    }

    }

    }

    */
}

void drawscreenlock( void )
{
    HRESULT     ddrval;

loop:
    //draw the screenlock icon
    ddrval = lpDDSBack->BltFast(0, 0, k[seq[423].frame[9]].k,
        &k[seq[423].frame[9]].box  , DDBLTFAST_NOCOLORKEY  );

    if (ddrval == DDERR_WASSTILLDRAWING ) goto loop;

    //if (ddrval != DD_OK) dderror(ddrval);

loop2:
    //draw the screenlock icon
    ddrval = lpDDSBack->BltFast(620, 0, k[seq[423].frame[10]].k,
        &k[seq[423].frame[10]].box  , DDBLTFAST_NOCOLORKEY  );

    if (ddrval == DDERR_WASSTILLDRAWING ) goto loop2;
    // if (ddrval != DD_OK) dderror(ddrval);


}    



#include "update_frame.cpp"





/*
* finiObjects
*
* finished with all objects we use; release them
*/
void finiObjects()
{
    //wDeviceID = mciGetDeviceID("MCI_ALL_DEVICE_ID"); 

    if (last_saved_game > 0)
    {
        Msg("Modifying saved game.");

        if (!add_time_to_saved_game(last_saved_game))
            Msg("Error modifying saved game.");
    }

    if (::sound_on)
    {
        mciSendCommand(CD_ID, MCI_CLOSE, 0, NULL);    

        Msg("Shutting down CD stuff.");
        killcd(g_hWnd, 1);
    }
    log_path(false);



    if( lpDD != NULL )
    {

        //change coop mode back

        /*
        if( lpDDSBack != NULL )
        {
        lpDDSBack->Release();
        lpDDSBack = NULL;
        }
        */ 
        if( lpDDPal != NULL )
        {
            lpDDPal->Release();
            lpDDPal = NULL;
        }

        if( lpDDSPrimary != NULL )
        {
            lpDDSPrimary->Release();
            lpDDSPrimary = NULL;
        }
        if (lpDD->RestoreDisplayMode() != DD_OK)
            Msg("Error restoring display mode.");

        /*
        HRESULT ddrval = lpDD->SetCooperativeLevel( hWndMain, DDSCL_NORMAL);

        if( ddrval != DD_OK )  
        {        
        Msg("Unable to set cooperative level on exit.");
        }

        */
        lpDD->Release();
        lpDD = NULL;

        /*for (int oo = 1; oo <= max_sprites; oo++)   
        {

        if( k[oo].k != NULL )
        {

        k[oo].k->Release();
        k[oo].k = NULL;
        }


        }

        */
    } 

    //destroy direct input mouse stuff

    if (g_pMouse)
    {
        g_pMouse->Unacquire();
        g_pMouse->Release();
        g_pMouse = NULL;
    }

    if (g_hevtMouse)
    {
        CloseHandle(g_hevtMouse);
        g_hevtMouse = NULL;
    }

    if (g_pdi)     
    {
        g_pdi->Release();
        g_pdi    = NULL;
    }



    if (sound_on)
        DestroySound();


    if (::sound_on)
    {
        //lets kill the cdaudio too
        if (mciSendString("close all", NULL, 0, NULL) != 0)
        {
            Msg("Couldn't close all MCI events..");
            //  return(false);
        }

    }

    kill_all_scripts_for_real();
    FastFileFini();
    void kill_fonts();
    kill_fonts();
    g_b_kill_app = true;
    ShowWindow(hWndMain, SW_HIDE);
    SendMessage(hWndMain, WM_IMDONE, 0,0);
    //PostQuitMessage(0);

} /* finiObjects */

bool initFail( HWND hwnd, char mess[200] )
{
    MessageBox( hwnd, mess, TITLE, MB_OK );
    finiObjects();
    //  DestroyWindow( hwnd );
    return false;

} /* initFail */


long FAR PASCAL WindowProc( HWND hWnd, UINT message, 
                           WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
    case WM_ACTIVATEAPP:
        bActive = wParam;
        break;

    case WM_SETCURSOR:
        if (!windowed)
        {
            SetCursor(NULL);
        }
        return true;

    case WM_CREATE:
        break;
    case WM_IMDONE:
        Msg("Sending quit message.");
        PostQuitMessage(0);
        break;



    case WM_KEYDOWN:
        if ( mConsoleActive )
        {
            switch( wParam )
            {
                /*case VK_F1:
                {
                Msg("F1 pressed");
                //g_pMouse->Unacquire();

                int crap;
                }
                break;

                case VK_F3:
                {
                fill_whole_hard();     
                fill_hard_sprites();
                fill_back_sprites();
                }
                break;
                */

                /*case VK_F4:
                {
                process_downcycle = true;
                cycle_clock = thisTickCount;
                }
                */
                case VK_UP:
                    if ( mConsoleHistoryIndex > 0 )
                    {
                        mConsoleHistoryIndex -= 1;
                        mConsoleLine = mConsoleLineHistory[mConsoleHistoryIndex];
                    }
                    break;

                case VK_DOWN:
                    mConsoleHistoryIndex += 1;
                    if ( mConsoleHistoryIndex < mConsoleLineHistory.size() )
                    {
                        mConsoleLine = mConsoleLineHistory[mConsoleHistoryIndex];
                    }
                    else
                    {
                        mConsoleHistoryIndex = mConsoleLineHistory.size();
                        mConsoleLine = "";
                    }
                    break;
                default:
                    break;
            }
        }
        break;
    // redink1 addition of console
    case WM_CHAR:
        if ( mConsoleActive )
        {
            switch ( wParam )
            {
                case 0x08: // Process a backspace.
                    if ( mConsoleLine.length() > 0 )
                    {
                        mConsoleLine = mConsoleLine.substr( 0, mConsoleLine.length() - 1 );
                    }
                    break; 

                case 0x0A: // Process a linefeed
                    break; 

                case 0x1B: // Process an escape. 
                    mConsoleActive = false;
                    mConsoleLine = "";
                    break; 

                case 0x09: // Process a tab. 
                    break; 

                case 0x0D: // Process a carriage return, try to parse the string.
                    if ( mConsoleScript == 0 || rbuf[mConsoleScript] == NULL )
                    {
                        mConsoleScript = 0;
                        for (int k=1; k < max_scripts; k++)
                        {
                            if (rbuf[k] == NULL)
                            {
                                mConsoleScript = k;
                                rinfo[mConsoleScript] = (struct refinfo *) malloc( sizeof(struct refinfo));
                                memset(rinfo[mConsoleScript], 0, sizeof(struct refinfo));
                                rinfo[mConsoleScript]->sprite = 1000;
                                rinfo[mConsoleScript]->level = 1;
                                rbuf[mConsoleScript] = (char *) malloc( 255 );
                                break;
                            }
                        }
                    }
                    if ( mConsoleScript != 0 )
                    {
                        strncpy( rbuf[mConsoleScript], mConsoleLine.c_str(), 254 );
                        process_line(mConsoleScript, rbuf[mConsoleScript], false);
                        mConsoleReturnValue = returnint;
                        mConsoleLineHistory.push_back( mConsoleLine );
                        if ( mConsoleLineHistory.size() > 20 )
                        {
                            mConsoleLineHistory.erase( mConsoleLineHistory.begin() );
                        }
                        mConsoleHistoryIndex = mConsoleLineHistory.size();
                    }
                    mConsoleLine = "";
                    break; 

                default:   // Process displayable characters.
                    mConsoleLine += static_cast<char>( wParam );
                    break; 
            }
            // clear keyboard buffer
            for (int x=0; x<256; x++)
            {
                if (GetKeyboard(x))
                {
                }
            }
        }
        break;
    //redink1 and Invertigo fix for Alt-F4/X button exiting problem
    case WM_CLOSE:
        finiObjects();
        PostQuitMessage( 0 );
        break;
    }


    //cycle through keys


    return DefWindowProc(hWnd, message, wParam, lParam);

} /* WindowProc */






bool CheckJoyStickPresent (void)
{

    // first tests if a joystick driver is present
    // if true it makes certain that a joystick is plugged in

    if(joyGetNumDevs())
    {
        memset(&jinfo,0,sizeof(JOYINFOEX));
        jinfo.dwSize=sizeof(JOYINFOEX);
        jinfo.dwFlags=JOY_RETURNALL;
        if(joyGetPosEx(JOYSTICKID1,&jinfo) == JOYERR_NOERROR)
        {
            Msg("Joystick IS plugged in");
            return true;
        } else
        {
            Msg("Joystick not plugged in");

            return false;

        }
        Msg("No joysticks found");
    }
    return false;
}




void load_batch(void)
{

    FILE *stream;  
    char line[255];

    spr[1].x = 200;
    spr[1].y = 300;


    Msg("Loading .ini");      
    if (!exist("dink.ini")) 
    {
        Msg("File not found.");   

        sprintf(line,"Error finding the dink.ini file in the %s dir.",dir);
        TRACE(line);

    }

    if( (stream = fopen( "dink.ini", "r" )) != NULL )   
    {

        while(1)
        {
            if( fgets( line, 255, stream ) == NULL) 
                goto done;
            else    
            {

                pre_figure_out(line, 0);
            }

        }

done:
        fclose( stream );  
    } else
    {
        TRACE("Dink.ini missing.");
    }

    program_idata();

}


//redink1 and Invertigo fix for windowed/high color mode
bool check_arg(char crap[255])
{
    truecolor = false;
    char shit[200];

    //  strupr(crap);
    strcpy(dir, "dink");
    for (int i=1; i <= 10; i++)
    {
        seperate_string(crap, i,' ',shit);
        if (strnicmp(shit,"-window",strlen("-window")) == 0)
        {
            windowed = true;
            truecolor = true;
            //          no_transition = true;     
        }

        if (strnicmp(shit,"-dinkpal",strlen("-dinkpal")) == 0)
        {
            dinkpal = true;
        }

        if (strnicmp(shit,"-truecolor",strlen("-truecolor")) == 0)
        {
            truecolor = true;
        }

        if (strnicmp(shit,"-debug",strlen("-debug")) == 0)
        {
            debug_mode = true;
            unlink("dink\\debug.txt");
        }

        if (strnicmp(shit,"-nojoy",strlen("-nojoy")) == 0)
        {
            disablejoystick = true;
        }
        if (strnicmp(shit,"-noini",strlen("-noini")) == 0)
        {
            g_b_no_write_ini = true;
        }

#ifndef __DEMO

        if (strnicmp(shit,"-game",strlen("-game")) == 0)
        {
            seperate_string(crap, i+1,' ',shit);
            strcpy(dir, shit);  
            Msg("Working directory %s requested.",dir);  
        }

#endif

#ifdef __DEMO
        if (strnicmp(shit,"-game",strlen("-game")) == 0)
        {
#ifdef __GERMAN
            sprintf(shit,"Die Shareware-Version akzeptiert keine selbsterstellten Abenteuer oder Addons.",dir);
#endif
#ifdef __ENGLISH

            sprintf(shit,"The shareware version cannot play player made quests or addons.",dir);
            initFail(hWndMain, shit);
#endif

            return(0);
        }

#endif

        if (strnicmp(shit,"-nosound",strlen("-nosound")) == 0)  sound_on = false;

    }

    //redink1 code so DMOD version can't run D-Mods
#ifdef DMOD
    strcpy(dir, ".");
#endif

    if (chdir(dir) == -1) 
    {
#ifdef __ENGLISH
        sprintf(shit,"Game dir \"%s\" not found!",dir);
#endif
#ifdef __GERMAN

        sprintf(shit,"Spiele-direktory \"%s\" nicht gefunden!",dir);
#endif

        initFail(hWndMain, shit);
        exit(0);
        return(0);
    }     


    Msg("Dir is now %s.",dir);
    return(true);
}
/*
* doInit - do work required for every instance of the application:
*                create the window, initialize data
*/


//redink1 and invertigo fix for windowed/high color mode
static bool doInit( HINSTANCE hInstance, int nCmdShow )
{
    memset(id, '\0', sizeof(id));
    //    HRESULT             dsrval;
    // bool                bUseDSound;
    WNDCLASS            wc;
    DDSURFACEDESC       ddsd;
    DDSCAPS             ddscaps;
    HRESULT             ddrval;
    Common::Rect                rcRect;


    char crap[30];
    char crap1[10];

    Common::Rect rcRectSrc;    Common::Rect rcRectDest;
    POINT p;
    /*
    * set up and register window class
    */



    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON1) );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = GetStockBrush(BLACK_BRUSH);
    wc.lpszMenuName = NAME;
    wc.lpszClassName = NAME;
    RegisterClass( &wc );
    /*
    * create a window
    */

    windowed = false;
    //  hWndMain = hwnd;   


    if (!check_arg(command_line))
    {
        return(0);
        //return initFail(hwnd, "Bad game directory.  Check your -game setting.");      
    }





    if (windowed)
    {
        /*hwnd = CreateWindowEx(
            0,
            NAME,
            TITLE,
            //        WS_POPUP,

            WS_SYSMENU|WS_CAPTION,

            0,
            0,
            640, 480,
            //GetSystemMetrics(SM_CXSCREEN),
            //GetSystemMetrics(SM_CYSCREEN),
            NULL,
            NULL,
            hInstance,
            NULL );

        if( !hwnd )
        {
            return false;
        }

        // Now check actual size of client area
        Common::Rect check;
        GetClientRect(hwnd, &check);
        int ExtendWidth = 640 - (check.right - check.left);
        int ExtendHeight = 480 - (check.bottom - check.top);

        DestroyWindow(hwnd);*/

        hwnd = CreateWindowEx(
            0,
            NAME,
            TITLE,
            //        WS_POPUP,

            WS_SYSMENU|WS_CAPTION,

            0,
            0,
            640,480,
            //        GetSystemMetrics(SM_CXSCREEN),
            //      GetSystemMetrics(SM_CYSCREEN),
            NULL,
            NULL,
            hInstance,
            NULL );
        hWndMain = hwnd;   

        if( !hwnd )
        {
            return false;
        }

        Common::Rect check;
        GetClientRect(hwnd, &check);
        int ExtendWidth = 640 - (check.right - check.left);
        int ExtendHeight = 480 - (check.bottom - check.top);

        MoveWindow( hWndMain, check.left, check.top, 640 + ExtendWidth, 480 + ExtendHeight, true );

        ShowWindow( hwnd, nCmdShow );
        UpdateWindow( hwnd );
        SetFocus( hwnd );
        /*
        * create the main DirectDraw object
        */
        ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
        if( ddrval != DD_OK )
        {
            return initFail(hwnd, "Couldn't use DirectX 8+...  Install it first.");
        }



        // Get exclusive mode


        // using DDSCL_NORMAL means we will coexist with GDI
        ddrval = lpDD->SetCooperativeLevel( hwnd, DDSCL_NORMAL );

        if( ddrval != DD_OK )  
        {        
            lpDD->Release(); 
            return initFail(hwnd, "Couldn't make windowed screen.");

        }   
        memset( &ddsd, 0, sizeof(ddsd) );
        ddsd.dwSize = sizeof( ddsd );
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

        // The primary surface is not a page flipping surface this time
        ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );

        if( ddrval != DD_OK )    
        {        lpDD->Release();  
        return initFail(hwnd, "Couldn't make primary surface.");


        } 

        memset( &ddsd, 0, sizeof(ddsd) ); 
        ddsd.dwSize = sizeof( ddsd );
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        //redink1 added SYSTEMMEMORY to speed things up
        ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
        ddsd.dwWidth = 640;
        ddsd.dwHeight = 480;    // create the backbuffer separately

        ddrval = lpDD->CreateSurface( &ddsd, &lpDDSBack, NULL );
        if( ddrval != DD_OK )
        {        lpClipper-> Release();

        lpDDSPrimary->Release();    
        lpDD->Release(); 
        return initFail(hwnd, "Couldn't make Back buffer in Windowed mode.");
        }   



        // Create a clipper to ensure that our drawing stays inside our window
        ddrval = lpDD->CreateClipper( 0, &lpClipper, NULL );
        if( ddrval != DD_OK )   
        {      
            lpDDSPrimary->Release();

            lpDD->Release();  
            return initFail(hwnd, "Couldn't make a Clipper object, god knows why.");

        }


        // setting it to our hwnd gives the clipper the coordinates from our window
        ddrval = lpClipper->SetHWnd( 0, hwnd );   
        if( ddrval != DD_OK )  
        {
            lpClipper-> Release();   
            lpDDSPrimary->Release();

            lpDD->Release();     
            return initFail(hwnd, "Couldn't give Clipper window cords.");

        }
        // attach the clipper to the primary surface
        ddrval = lpDDSPrimary->SetClipper( lpClipper ); 
        if( ddrval != DD_OK )
        {  
            lpClipper-> Release();   
            lpDDSPrimary->Release();
            lpDD->Release();    

            return initFail(hwnd, "Couldn't attach Clipper to primary buffer.");
        }


    }

    if (!windowed)
    {

        hwnd = CreateWindowEx(
            0,
            NAME,
            TITLE,
            WS_POPUP ,

            //WS_SYSMENU|WS_CAPTION,

            0,
            0,
            640, 480,
            //        GetSystemMetrics(SM_CXSCREEN),
            //      GetSystemMetrics(SM_CYSCREEN),
            NULL,
            NULL,
            hInstance,
            NULL );
        hWndMain = hwnd;   

        if( !hwnd )
        {
            return false;
        }

        ShowWindow( hwnd, nCmdShow );
        UpdateWindow( hwnd );
        SetFocus( hwnd );


        /*
        * create the main DirectDraw object
        */
        ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
        if( ddrval != DD_OK )
        {
            return initFail(hwnd, "Couldn't use DirectX 3+...  Install it first.");
        }

        // Get exclusive mode




        ddrval = lpDD->SetCooperativeLevel( hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );


        if( ddrval != DD_OK )
        {
            return initFail(hwnd, "SetCooperative level failed.");
        }


        // Set the video mode to 640x480xX
        if ( truecolor == true )
        {
            // Keep trying until one works, or not.  There should be a way to see what DirectX supports... hrm
            ddrval = lpDD->SetDisplayMode( x, y, 32 );
            if ( ddrval != DD_OK )
            {
                ddrval = lpDD->SetDisplayMode( x, y, 24 );
                if ( ddrval != DD_OK )
                {
                    ddrval = lpDD->SetDisplayMode( x, y, 16 );
                    if ( ddrval != DD_OK )
                    {
                        return initFail( hwnd, "640 X 480, true color mode not supported." );
                    }
                }
            }
        }
        else
        {
            ddrval = lpDD->SetDisplayMode( x, y, 8 );
            if ( ddrval != DD_OK )
            {
                return initFail( hwnd, "640 X 480, 8 bit not supported." );
            }
        }

        // finiObjects();   
        //  return false;

        ZeroMemory(&hm, sizeof(hit_map));



        // Create the primary surface with 1 back buffer
        ddsd.dwSize = sizeof( ddsd );
        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
            DDSCAPS_FLIP |
            DDSCAPS_COMPLEX;

        ddsd.dwBackBufferCount = 1;
        ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
        if( ddrval != DD_OK )
        {
            return initFail(hwnd, "Could not create primary surface.");
        }

        ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

        /* if (ddsd.ddsCaps.dwCaps == DDCAPS_BLTSTRETCH)
        {
        return initFail(hwnd, "Hardware blit stretching available.");
        }
        */
        ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);
        if( ddrval != DD_OK )
        {
            return initFail(hwnd, "Could not create backbuffer,");
        }

    }

    //redink1 init for color depth information
    if (truecolor)
    {
        DDSURFACEDESC dds;
        memset(&dds, 0, sizeof(DDSURFACEDESC));
        dds.dwSize = sizeof(DDSURFACEDESC); 
        lpDDSBack->GetSurfaceDesc(&dds);
        if ( dds.ddpfPixelFormat.dwGBitMask == 0x07E0 )
        {
            // 565
            mColorDepth = ColorDepth16Bit_565;
            m16BitFadeValues = new unsigned short[65536][30];

            for ( unsigned short lFade( 0 ); lFade < 30; lFade++ )
            {
                float lFadeAdjustment( ( 30.0f - static_cast<float>( lFade ) ) / 30.0f );
                for ( unsigned short lRed( 0 ); lRed < 32; lRed++ )
                {
                    unsigned short lRedFadeValue = static_cast<unsigned short>( static_cast<float>( lRed ) * lFadeAdjustment ) << 11;
                    for ( unsigned short lGreen( 0 ); lGreen < 64; lGreen++ )
                    {
                        unsigned short lGreenFadeValue = static_cast<unsigned short>( static_cast<float>( lGreen ) * lFadeAdjustment ) << 5;
                        for ( unsigned short lBlue( 0 ); lBlue < 32; lBlue++ )
                        {
                            unsigned short lBlueFadeValue = static_cast<unsigned short>( static_cast<float>( lBlue ) * lFadeAdjustment );
                            if ( ( lRed << 11 | lGreen << 5 | lBlue ) == 0xFFFF )
                            {
                                lRedFadeValue = lRed << 11;
                                lGreenFadeValue = lGreen << 5;
                                lBlueFadeValue = lBlue;
                            }
                            m16BitFadeValues[ lRed << 11 | lGreen << 5 | lBlue ][ lFade ] = lRedFadeValue | lGreenFadeValue | lBlueFadeValue;
                        }
                    }
                }
            }
        }
        else if ( dds.ddpfPixelFormat.dwGBitMask == 0x03E0 )
        {
            // 555
            mColorDepth = ColorDepth16Bit_555;
            m16BitFadeValues = new unsigned short[65536][30];

            for ( unsigned short lFade( 0 ); lFade < 30; lFade++ )
            {
                float lFadeAdjustment( ( 30.0f - static_cast<float>( lFade ) ) / 30.0f );
                for ( unsigned short lRed( 0 ); lRed < 32; lRed++ )
                {
                    unsigned short lRedFadeValue = static_cast<unsigned short>( static_cast<float>( lRed ) * lFadeAdjustment ) << 10;
                    for ( unsigned short lGreen( 0 ); lGreen < 32; lGreen++ )
                    {
                        unsigned short lGreenFadeValue = static_cast<unsigned short>( static_cast<float>( lGreen ) * lFadeAdjustment ) << 5;
                        for ( unsigned short lBlue( 0 ); lBlue < 32; lBlue++ )
                        {
                            unsigned short lBlueFadeValue = static_cast<unsigned short>( static_cast<float>( lBlue ) * lFadeAdjustment );
                            if ( ( lRed << 10 | lGreen << 5 | lBlue ) == 0xFFFF )
                            {
                                lRedFadeValue = lRed << 10;
                                lGreenFadeValue = lGreen << 5;
                                lBlueFadeValue = lBlue;
                            }
                            m16BitFadeValues[ lRed << 10 | lGreen << 5 | lBlue ][ lFade ] = lRedFadeValue | lGreenFadeValue | lBlueFadeValue;
                        }
                    }
                }
            }
        }
        else if ( dds.ddpfPixelFormat.dwRGBBitCount == 32 || dds.ddpfPixelFormat.dwRGBBitCount == 24 )
        {
            // 32 & 24
            if ( dds.ddpfPixelFormat.dwRGBBitCount == 32 )
            {
                mColorDepth = ColorDepth32Bit;
            }
            else
            {
                mColorDepth = ColorDepth24Bit;
            }
            m8BitFadeValues = new unsigned char[256][30];
            for ( unsigned char lFade( 0 ); lFade < 30; lFade++ )
            {
                double lFadeAdjustment( ( 30.0 - static_cast<double>( lFade ) ) / 30.0 );
                for ( unsigned short lValue( 0 ); lValue <= 255; lValue++ )
                {
                    m8BitFadeValues[lValue][lFade] = static_cast<unsigned char>( static_cast<double>( lValue ) * lFadeAdjustment );
                }
            }
        }
        else
        {
            mColorDepth = ColorDepthNotRecognized;
            // No fade
        }

        dwRMask = dds.ddpfPixelFormat.dwRBitMask;
        dwGMask = dds.ddpfPixelFormat.dwGBitMask;
        dwBMask = dds.ddpfPixelFormat.dwBBitMask;
        wRBits=GetNumberOfBits(dwRMask);
        wGBits=GetNumberOfBits(dwGMask);
        wBBits=GetNumberOfBits(dwBMask);
        
        wRPos  = GetMaskPos(dwRMask);
        wGPos  = GetMaskPos(dwGMask);
        wBPos  = GetMaskPos(dwBMask);

        memset( &ddsd, 0, sizeof(ddsd) ); 
        ddsd.dwSize = sizeof( ddsd );
        ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER | DDSCAPS_SYSTEMMEMORY;
        ddsd.dwWidth = 640;
        ddsd.dwHeight = 480;
        ddrval = lpDD->CreateSurface( &ddsd, &lpDDSFade, NULL );
    }

    //init is finished, now lets load some more junk
    //return initFail(hwnd, "Couldn't make a Clipper object, god knows why.");
    // create and set the palette

    //  getCDTrackStartTimes();



    if (exist("tiles\\TS01.bmp"))
        lpDDPal = DDLoadPalette(lpDD, "tiles\\TS01.BMP"); else
        lpDDPal = DDLoadPalette(lpDD, "..\\dink\\tiles\\TS01.BMP");
    if (lpDDPal)
        lpDDSPrimary->SetPalette(lpDDPal);

    if(lpDDPal->GetEntries(0,0,256,real_pal)!=DD_OK)
    {
        Msg("error with getting entries in beginning");
    }

    //redink1 Faster true-color fades
    /*if ( truecolor )
    {
        mPrecalculatedFadeValues = new std::map<DWORD, FadeValues>;
        DWORD dwWhite    = dwRMask | dwGMask | dwBMask;
        PALETTEENTRY lPaletteEntries[256];
        lpDDPal->GetEntries( 0, 0, 256, lPaletteEntries );
        for ( int lPaletteEntry = 0; lPaletteEntry < 256; lPaletteEntry++ )
        {
            DWORD lColor = ( lPaletteEntries[lPaletteEntry].peRed >> (8 - wRBits) << wRPos |
                             lPaletteEntries[lPaletteEntry].peGreen >> (8 - wGBits) << wGPos |
                             lPaletteEntries[lPaletteEntry].peBlue >> (8 - wBBits) << wBPos );
            mPrecalculatedFadeValues->[lColor].reserve(256);
            for ( int lFadeValue = 0; lFadeValue < 256; lFadeValue += 1 )
            {
                DWORD red   = (lColor & dwRMask) << (8 - wRBits) >> wRPos;
                red   = (red   >= lFadeValue ? red   - lFadeValue : 0);

                DWORD green = (lColor & dwGMask) << (8 - wGBits) >> wGPos;
                green = (green >= lFadeValue ? green - lFadeValue : 0);

                DWORD blue  = (lColor & dwBMask) << (8 - wBBits) >> wBPos;
                blue  = (blue  >= lFadeValue ? blue  - lFadeValue : 0);

                DWORD lResult = ( red >> (8 - wRBits) << wRPos |
                      green >> (8 - wGBits) << wGPos |
                      blue >> (8 - wBBits) << wBPos );
                mPrecalculatedFadeValues[lColor].push_back( lResult );
            }
        }
    }*/

    if (exist("tiles\\splash.bmp"))
        lpDDPal = DDLoadPalette(lpDD, "tiles\\SPLASH.BMP"); else
        lpDDPal = DDLoadPalette(lpDD, "..\\dink\\tiles\\SPLASH.BMP");

    if (lpDDPal)
        lpDDSPrimary->SetPalette(lpDDPal);


    //crashes here!     
    Msg("loading tilescreens...");

    for (int h=1; h < tile_screens; h++)
    {
        if (h < 10) strcpy(crap1,"0"); else strcpy(crap1, "");
        sprintf(crap, "TILES\\TS%s%d.BMP",crap1,h);

        if (!exist(crap))
        {
            sprintf(crap, "..\\DINK\\TILES\\TS%s%d.BMP",crap1,h);
        }

        tiles[h] = DDTileLoad(lpDD, crap, 0, 0,h); 


        if( tiles[h] == NULL )
        {
            return initFail(hwnd, "Couldn't find one of the tilescreens!");
        } else
        {
            //tilerect[h].right =  lpDD->dwWidth;

            DDSetColorKey(tiles[h], RGB(0,0,0));
        }
    }
    Msg("Done with tilescreens...");

    // Create the offscreen surface, by loading our bitmap.
    //lpDDSOne = DDLoadBitmap(lpDD, szBitmap, 0, 0);
    if (sound_on) 
    {
        Msg("Initting sound");
        sound_on = InitSound(hwnd);
    }


#ifdef __DEMO

    PlayMidi("4.MID");
#endif  
    srand( (unsigned)time( NULL ) );

    if (exist("tiles\\splash.bmp"))
        lpDDSTwo = DDLoadBitmap(lpDD, "tiles\\splash.BMP", 0, 0); else
        lpDDSTwo = DDLoadBitmap(lpDD, "..\\dink\\tiles\\splash.BMP", 0, 0);
    DDSetColorKey(lpDDSTwo, RGB(0,0,0));
#ifndef __DEMO

    getCDTrackStartTimes();

    if (cd_inserted)
        PlayCD(g_hWnd, 7);
#endif

    if ( disablejoystick == false )
    {
        if (CheckJoyStickPresent() == false)
        {
            //  return initFail(hwnd, "Could not init the joystick.");
            joystick = false;
        } else
        {
            joystick = true;
        }
    }

    rcRect.left = 0;
    rcRect.top = 0;
    rcRect.right = x;
    rcRect.bottom = y;

    //if (lpDDSBack->GetBltStatus( DDGBS_ISBLTDONE) == DD_OK)
    //draw version #



    ddrval = lpDDSBack->BltFast( 0, 0, lpDDSTwo,
        &rcRect, DDBLTFAST_NOCOLORKEY);

    if (!windowed)
    {   
        while( 1 )
        {
            ddrval = lpDDSPrimary->Flip(NULL,DDFLIP_WAIT );
            if( ddrval == DD_OK )
            {
                break;
            }
            if( ddrval == DDERR_SURFACELOST )
            {
                ddrval = restoreAll();
                if( ddrval != DD_OK )
                {
                    break;
                }
            }
            if( ddrval != DDERR_WASSTILLDRAWING )
            {

                //              ddrval = DD_OK;
                dderror(ddrval);
                //  return;
                //        ddrval = DD_OK;
            }
            //goto done;
        }
    } else
    {

        // instead of a flip, this will work for Windowed mode:
        // first we need to figure out where on the primary surface our window lives
        p.x = 0; p.y = 0;        ClientToScreen(hwnd, &p);
        GetClientRect(hwnd, &rcRectDest);
        OffsetRect(&rcRectDest, p.x, p.y);
        SetRect(&rcRectSrc, 0, 0, 640, 480);
        ddrval = lpDDSPrimary->Blt( &rcRectDest, lpDDSBack, &rcRectSrc, DDBLT_WAIT, NULL);
    }




    //dinks normal walk


    Msg("loading batch");
    load_batch();


    Msg("done loading batch");


    load_hard();

    //Activate dink, but don't really turn him on
    //spr[1].active = true;
    spr[1].timer = 33;

    //copy from player info
    spr[1].x = play.x;
    spr[1].y = play.y;



    if (exist("tiles\\TS01.bmp"))
        lpDDPal = DDLoadPalette(lpDD, "tiles\\TS01.BMP"); else
        lpDDPal = DDLoadPalette(lpDD, "..\\dink\\tiles\\TS01.BMP");
    if (lpDDPal)
        lpDDSPrimary->SetPalette(lpDDPal);

    Msg("Loading splash");
    if (exist("tiles\\SPLASH.bmp"))
        lpDDSTrick = DDLoadBitmap(lpDD, "tiles\\SPLASH.BMP", 0, 0); else
        lpDDSTrick = DDLoadBitmap(lpDD, "..\\dink\\tiles\\SPLASH.BMP", 0, 0);
    DDSetColorKey(lpDDSTrick, RGB(0,0,0));

    if (exist("tiles\\SPLASH.bmp"))
        lpDDSTrick2 = DDLoadBitmap(lpDD, "tiles\\SPLASH.BMP", 0, 0); else
        lpDDSTrick2 = DDLoadBitmap(lpDD, "..\\dink\\tiles\\SPLASH.BMP", 0, 0);
    DDSetColorKey(lpDDSTrick2, RGB(0,0,0));

    // ** SETUP **

    rcRect.left = 0;
    rcRect.top = 0;
    rcRect.right = 639;
    rcRect.bottom = 79;
    last_sprite_created = 1;

    mode = 0;

    load_info();        
    for (int x=0; x<256; x++)
    {
        if (GetKeyboard(x))
        {
        }
    }  //clear keyboard buffer 

    for (int u = 1; u <= 10; u++)
        play.button[u] = u;


    for (int x1=1; x1 <= 10; x1++) 
    {
        sjoy.letgo[x1] = true;
    }

    //lets run our init script
    int script = load_script("main", 0, true);
    locate(script, "main");
    run_script(script);
    //lets attach our vars to the scripts

    attach();


    //init_mouse(hwnd);

    memset( &spr, NULL, sizeof(spr) );
    init_font_colors();
    initfonts("Arial");
    //g_pMouse->Acquire();


    return true;

} /* doInit */


#include <sys/types.h>
#include <sys/stat.h>

bool random_date(char file[255])

{ 
    //let's randomize the date on this file to trick Windows
    //cacheing into doing it right!

    int fh, result; 

    unsigned int nbytes = BUFSIZ;   /* Open a file */

    if( (fh = _open( file, _O_RDWR | _O_CREAT, _S_IREAD 
        | _S_IWRITE ))  != -1 )   
    {
        Msg( "File length before: %ld\n", _filelength( fh ) );
        if( ( result = _chsize( fh, _filelength( fh )+((rand() % 1000)+1) ) ) == 0 )
            Msg( "Size successfully changed\n" );      
        else
            Msg( "Problem in changing the size\n" );

        Msg( "File length after:  %ld\n", _filelength( fh ) );

        _close( fh );   

    } else return(false);


    return(true);
}




int dir_num ( char path[255])
{

    HANDLE      dir;
    WIN32_FIND_DATA fd;
    unsigned long   cnt;

    char        *dename;

    /*
    * build a header
    */

    char search_specs[255];

    sprintf(search_specs, "%s\\*.*",path);
    cnt = 0;
    ODS( "ASking for dir info." );
    dir = FindFirstFile( search_specs, &fd );
    if( dir == NULL )
    {
        ODS( "Could not open current directory\n" );
        return(0);
    }


    while( 1 ) 
    {
        if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) 
        {
            cnt++;

            dename = fd.cAlternateFileName;
            if( dename[0] == 0 ) {
                dename = fd.cFileName;
            }
            Msg( "File %d: %s                 \r", cnt, dename );

        }
        if( !FindNextFile( dir, &fd ) ) {
            break;
        }
    }

    FindClose( dir );
    ODS("All done finding files.");
    return(cnt);
}

void getdir(char final[])
{
    //converted to non CString version that spits back path + filename seperately.
    //Using GetModuleFileName instead of ParamStr, works with Win2000/nt.
    char dir[255];
    char path[255];
    GetModuleFileName(NULL, path, 255);
    char c_cur = 0;

    for (int k=strlen(path);path[k] != '\\'; k--)
    {
        c_cur = k;
    }
    strcpy(dir, "");
    //copy file name
    strncat((char*)&dir, &path[c_cur], strlen(path)-c_cur);
    path[c_cur] = 0; //truncate
    strcpy(final, path);

}   
/*
* WinMain - initialization, message loop
*/
int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    sound_support = ((rand() % 200000)+1);  

    mycode = ((rand() % 200000)+1); 
    char crap2[256];


    //    strcpy(crap2, GetCommandLine());
    ///*      
    getdir((char*)&crap2);



    //   crap2[strlen(crap2)-1] = 0;

    //    char st_temp[500];
    //    getdir(st_temp, crap2);     

    strcpy(dinkpath, crap2);




    if (chdir(crap2)) 
    {
        char crap[256];
        sprintf(crap, "Dink Error: Couldn't change to dir %s.  Why?", crap2);
        initFail(hWndMain, crap2);
        return(0);   
    }





#ifdef __DEMO

    //if (chdir("story")) 
    Msg("Error finding story dir.");
    mcc = sound_support;

    if (dir_num("dink\\story") != 250+9)
    {

        mcc = 0;

#ifdef __GERMAN 
        sprintf(crap, "Die Shareware-Version akzeptiert keine selbsterstellten Abenteuer.", crap2);
#endif

#ifdef __ENGLISH

        sprintf(crap, "Shareware version cannot run player made quests.", crap2);
#endif



        initFail(hWndMain, crap);
        return(0);   
    }

    //if (chdir("..")) 

#endif

    if (exist("cd.dat")) burn_revision = 1;


    //int junk = GetTickCount() + 5000;

    //yeah:
    //    if (junk > GetTickCount()) goto yeah;

    //random_date("DMOVIE.EXE");


    MSG         msg;
    lpCmdLine = lpCmdLine; 

    command_line = lpCmdLine; 
    /*
    if (compare("", command_line))
    {
    if (exist("DMOVIE.EXE"))
    {
    Msg("No command line.  Let's run movies.");
    char run_this[255];
    sprintf(run_this, "%s\\DMOVIE.EXE",dinkpath); 

    int myreturn =  _execl( run_this,run_this, NULL );
    if (myreturn == 1)
    {
    ODS("Spawn error:  Path not found.");

    }
    if (myreturn == 2)
    {
    ODS("Spawn error:  Not enough memory.");

    }


    return(0);
    }
    } else
    {

    //  Msg("Older version of Dink detected.");
    }
    */
    MyhInstance = hInstance;
    hPrevInstance = hPrevInstance;
    //return(0);
    if( !doInit( hInstance, nCmdShow ) )
    {

        //  return false;
    }

    log_path(true);   





    while( 1 )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
            //  Msg("Got %d", msg.message);
            if( !GetMessage( &msg, NULL, 0, 0 ) )
            {
                return msg.wParam;
            }
            
            //Msg("Got message - %d and %d", msg.wParam, msg.lParam);
            TranslateMessage(&msg); 
            DispatchMessage(&msg);
        }
        else if(  (bActive) || ( (debug_mode) && (windowed))   )
        {
            if (g_b_kill_app == false)
            {
                SetFocus( hwnd );
                updateFrame();
            }
            //WaitMessage();

        }
        else
        {
            // make sure we go to sleep if we have nothing else to do
            Sleep(1);
            SetFocus( NULL );
            WaitMessage();

        }
    }
} /* WinMain */

