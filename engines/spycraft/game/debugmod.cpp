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

#include "spycraft/game/object.h"
#include "spycraft/game/debugmod.h"
#include "spycraft/game/print.h"
#include "spycraft/game/user.h"			 
#include "spycraft/game/game.h"
#include "spycraft/game/flag.h"
#include "spycraft/dmade/advtime.h"

#ifdef _DEBUG
#define DEBUG_CODE
#endif
#ifdef DIRECTX
#include "spycraft/dmade/advmovie.h"
#define DEBUG_YAKE     sfxIsMovieOpen()
#else
#define DEBUG_YAKE     sfxIsMCIOpen()
#endif


#ifdef DIRECTX
#include "spycraft/dmade/advmovie.h"
#define DEBUG_YAKE_ME     sfxIsMoviePlaying()
#else
#define DEBUG_YAKE_ME     sfxIsMCIOpen()
#endif



#if 1
#define TURNON_1
#include "spycraft/game/room1.h"
#endif

#if 1
#define TURNON_90000
#include "spycraft/logic/90000.h"
#endif

#if 1
#define TURNON_91000
#include "spycraft/logic/91000.h"
#endif

#if 1
#define TURNON_1070
#include "spycraft/logic/1070.h"
#endif

#if 1
#define TURNON_1075
#include "spycraft/logic/1075.h"
#endif

#if 0
#define TURNON_1085
#include "spycraft/logic/1085.h"
#endif

#if 1
#define TURNON_1090
#include "spycraft/logic/1090.h"
#endif

#if 1
#define TURNON_1000
#include "spycraft/logic/1000.h"
#endif

#if 1
#define TURNON_1270
#include "spycraft/logic/1270.h"
#endif

#if 1 
#define TURNON_2000
#include "spycraft/logic/2000.h"
#endif

#if 1
#define TURNON_5000
#include "spycraft/logic/5000.h"
#endif

#if 1
#define TURNON_7000
#include "spycraft/logic/7000.h"
#endif

#if 1
#define TURNON_10000
#include "spycraft/logic/10000.h"
#endif

#if 1 
#define TURNON_12000
#include "spycraft/logic/12000.h"
#endif

#if 1
#define TURNON_18000
#include "spycraft/logic/18000.h"
#endif

#if 1
#define TURNON_24000
#include "spycraft/logic/24000.h"
#endif

#if 1
#define TURNON_26000
#include "spycraft/logic/26000.h"
#endif

#if 1
#define TURNON_27000
#include "spycraft/logic/27000.h"
#endif

#if 1
#define TURNON_36000
#include "spycraft/logic/36000.h"
#endif

#if 1
#define TURNON_37000
#include "spycraft/logic/37000.h"
#endif

#if 1
#define TURNON_38000
#include "spycraft/logic/38000.h"
#endif

#if 1
#define TURNON_35000
#include "spycraft/logic/35000.h"
#endif

#if 1
#define TURNON_25000
#include "spycraft/logic/25000.h"
#endif

#if 1
#define TURNON_25100
#include "spycraft/logic/25100.h"
#endif

#if 1
#define TURNON_50000
#include "spycraft/logic/50000.h"
#endif

#if 1
#define TURNON_50001
#include "spycraft/logic/50000.h"
#endif

#if 1
#define TURNON_50100
#include "spycraft/logic/50000.h"
#endif

#if 1
#define TURNON_50200
#include "spycraft/logic/50000.h"
#endif

#if 1
#define TURNON_50300
#include "spycraft/logic/50000.h"
#endif

#if 1
#define TURNON_53000
#include "spycraft/logic/53000.h"
#endif

#if 1
#define TURNON_56000
#include "spycraft/logic/56000.h"
#endif

#if 1
#define TURNON_56001
#include "spycraft/logic/56000.h"
#endif

#if 1
#define TURNON_57000
#include "spycraft/logic/57000.h"
#endif

#if 1
#define TURNON_48000
#include "spycraft/logic/48000.h"
#endif

#if 1
#define TURNON_48200
#include "spycraft/logic/48000.h"
#endif

#if 1
#define TURNON_48300
#include "spycraft/logic/48000.h"
#endif

#if 1
#define TURNON_69000
#include "spycraft/logic/69000.h"
#endif

#if 1
#define TURNON_69100
#include "spycraft/logic/69100.h"
#endif

#if 1
#define TURNON_69101
#include "spycraft/logic/69100.h"
#endif

#if 1
#define TURNON_69102
#include "spycraft/logic/69100.h"
#endif

#if 1
#define TURNON_69200
#include "spycraft/logic/69200.h"
#endif

#if 1
#define TURNON_23000
#include "spycraft/logic/23000.h"
#endif

#if 1
#define TURNON_23001
#include "spycraft/logic/23000.h"
#endif

#if 1
#define TURNON_23100
#include "spycraft/logic/23100.h"
#endif

#if 1
#define TURNON_93000
#include "spycraft/logic/93000.h"
#endif

#if 1
#define TURNON_32000
#include "spycraft/logic/32000.h"
#endif

#if 1
#define TURNON_41500
#include "spycraft/logic/41100.h"
#endif

#if 1
#define TURNON_35200
#include "spycraft/logic/35200.h"
#endif

#if 1
#define TURNON_37200
#include "spycraft/logic/37200.h"
#endif

#if 1
#define TURNON_37300
#include "spycraft/logic/37300.h"
#endif

#if 1
#define TURNON_36400
#include "spycraft/logic/36400.h"
#endif

#if 1
#define TURNON_36500
#include "spycraft/logic/36500.h"
#endif

#if 1
#define TURNON_48400
#include "spycraft/logic/48400.h"
#endif

#if 1
#define TURNON_48500
#include "spycraft/logic/48500.h"
#endif

#if 1
#define TURNON_57100
#include "spycraft/logic/57100.h"
#endif

#if 1
#define TURNON_36600
#include "spycraft/logic/36600.h"
#endif

#if 1
#define TURNON_36700
#include "spycraft/logic/36700.h"
#endif

#if 1
#define TURNON_35400
#include "spycraft/logic/35400.h"
#endif 

#if 1
#define TURNON_36100
#include "spycraft/logic/36100.h"
#endif 

#if 1
#define TURNON_36200
#include "spycraft/logic/36200.h"
#endif 

#if 1
#define TURNON_36300
#include "spycraft/logic/36300.h"
#endif 

#if 1
#define TURNON_36301
#include "spycraft/logic/36300.h"
#endif 

#if 1
#define TURNON_36302
#include "spycraft/logic/36300.h"
#endif 

#if 1
#define TURNON_36303
#include "spycraft/logic/36300.h"
#endif 

#if 1
#define TURNON_56100
#include "spycraft/logic/56100.h"
#endif 

#if 1
#define TURNON_56101
#include "spycraft/logic/56100.h"
#endif 

#if 1
#define TURNON_35100
#include "spycraft/logic/35000.h"
#endif

#if 1
#define TURNON_32100
#include "spycraft/logic/32100.h"
#endif

#if 1
#define TURNON_32200
#include "spycraft/logic/32200.h"
#endif

#if 1
#define TURNON_32300
#include "spycraft/logic/32300.h"
#endif

#if 1
#define TURNON_35500
#include "spycraft/logic/35400.h"
#endif

#if 1
#define TURNON_35600
#include "spycraft/logic/35400.h"
#endif

#if 1 
#define TURNON_60000
#include "spycraft/logic/60000.h"
#endif

#if 1 
#define TURNON_79000
#include "spycraft/logic/79000.h"
#endif

#if 1
#define TURNON_80000
#include "spycraft/logic/80000.h"
#endif

#if 1
#define TURNON_96000
#include "spycraft/logic/96000.h"
#endif

#if 1
#define TURNON_93010
#include "spycraft/logic/93000.h"
#endif

#if 1
#define TURNON_93020
#include "spycraft/logic/93000.h"
#endif

#if 1
#define TURNON_41100
#include "spycraft/logic/41100.h"
#endif

#if 1
#define TURNON_41200
#include "spycraft/logic/41100.h"
#endif

#if 1
#define TURNON_41300
#include "spycraft/logic/41100.h"
#endif

#if 1
#define TURNON_41400
#include "spycraft/logic/41100.h"
#endif

#if 1
#define TURNON_69002
#include "spycraft/logic/69000.h"
#endif

#if 1
#define TURNON_35700
#include "spycraft/logic/35700.h"
#endif

#if 1
#define TURNON_35300
#include "spycraft/logic/35300.h"
#endif

#if 1
#define TURNON_35001
#include "spycraft/logic/35000.h"
#endif

#if 1
#define TURNON_35002
#include "spycraft/logic/35000.h"
#endif

#if 1
#define TURNON_38100
#include "spycraft/logic/38000.h"
#endif

#if 1
#define TURNON_38200
#include "spycraft/logic/38000.h"
#endif

#if 1
#define TURNON_38300
#include "spycraft/logic/38000.h"
#endif

#if 1
#define TURNON_38400
#include "spycraft/logic/38000.h"
#endif

#if 1
#define TURNON_38500
#include "spycraft/logic/38000.h"
#endif


#include "spycraft/game/movie.h"
#include "spycraft/dmade/advfile.h"

namespace Spycraft {

int scratchGlobal;
int isDemo;

unsigned long maxSpaceTime;

extern unsigned beatCount;

#define DEBWIDTH	641
#define DEBHEIGHT	481

#define TELEPORT	0
#define FLAGSTUFF	1

extern Flag GameFlag;
extern Movie* theMovie;

Print* debugPrint;
DebugModule* theDebug = new DebugModule;

DebugModule::DebugModule()
{
	//file = new(File);
	//file->open("debug.txt", FILE_WRITE);
}

DebugModule::~DebugModule()
{
	//file->close();
}

extern "C" int allow_heapCheck;

int DebugModule :: handleEvent ( MADEEventStamp* event)	{
#ifdef DEBUG_CODE

	static int count, whatFunct, theNum;
	static char charArray[10];
	static char charCatArray[10];

#endif
	int theMsg;
	theMsg = event->message;

//	if ((event_type & USER_LEFT_UP ) && (!GameFlag.test(fDCK10) &&  !GameFlag.test(fPatsHTM_125)))
		//	theMovie->stop();
//	sfxPrintf("theMsg: %d", theMsg);

#ifdef DEBUG_CODE
	if (event_type & USER_KEY_DOWN)	{

      if ( theMsg == VK_F1 ) {
	         if ( allow_heapCheck ) {
	            sfxPrintf ( "Heap check turn OFF" );
	            allow_heapCheck = 0;
	         }
	         else {
	            sfxPrintf ( "Heap check turn ON" );
	            allow_heapCheck = 1;
	         }
      }

	


		if (theMsg == 84)  //t	
			whatFunct = TELEPORT;

		if (theMsg == 69) //e
			scratchGlobal = true; //DEBUG

		if (theMsg == 70)  //f
			whatFunct = FLAGSTUFF;
#endif
		if (( theMsg == 32 ) && (event_type & USER_KEY_DOWN)) {
			if (!GameFlag.test(fDCK10) && !GameFlag.test(fPatsHTM_125)){
				if (DEBUG_YAKE && DEBUG_YAKE_ME && !theMovie->paused && theMovie->isPlaying && strcmp( theMovie->movieName, "1271.avi") )	{
					 
					if (sfxGetTime() > maxSpaceTime)	{
						maxSpaceTime = sfxGetTime() + 5000L;
						theMovie->stop();
					}
				}
				else	{
					Node *n = sounds->head;
					SoundObject* obj;
					while ( n ) {
						obj = (SoundObject*)n->data;
						n = n->next;
						if ( obj->type == RES_WAVE && (obj->canInterupt))	
							obj->stop();
					}
				}
			}
			//flushMessageCue();
			theMsg = 0;
			return true;
		}
#ifdef DEBUG_CODE
		if (theMsg == 80)	{  //p
			sfxPrintf("x: %d, y: %d", event->cursor_pt.x,  event->cursor_pt.y);
			return true;
		}

		if (theMsg == 82)	{  //r
			GameFlag.set(fRoomPrintIndicate);
			return true;
		}

		if (theMsg == 66)	{  //b
		
			++beatCount;
			GameFlag.set(0);
			
		//	file->writeASCFile("\n\n room = %s, mouse_x = %d, mouse_y = %d \n", curRoom->name, mouse_x, mouse_y);	
		//	return true;
		}

		if (theMsg == 68)	{	  //d
			isDemo = true;
			return true;
		}
		//sfxPrintf("msg: %d", theMsg);
			
		if (((theMsg > 47) && (theMsg < 58)  || (theMsg == 189))  && (whatFunct == TELEPORT))	{	
		
			
				charArray[count] = (char) theMsg;
				
				charArray[count + 1] = '\0';
				count++;
			
				if (count == 1)
					debugPrint = new Print;

				if (debugPrint->myString)
					debugPrint->dispose();
			
				if (count > 1)
					debugPrint = new Print;

				debugPrint->init(charArray);
			}
			else if ((theMsg == 13) && (count))	{
			
				if (debugPrint->myString)
					debugPrint->dispose();
				//scratchGlobal = 0;

				charCatArray[0] = 'R';
				charCatArray[1] = 'm';
				charCatArray[2] = '\0';
				Common::strcat_s(charCatArray, charArray);
				teleport(charCatArray);
				charArray[0] = 0;
				charArray[1] = 0;
				charArray[2] = 0;
				charArray[3] = 0;
				charArray[4] = 0;
				charArray[5] = 0;
				charArray[6] = 0;
				charArray[7] = 0;
				charArray[8] = 0;
				charArray[9] = 0;

				charCatArray[0] = 0;
				charCatArray[1] = 0;
				charCatArray[2] = 0;
				charCatArray[3] = 0;
				charCatArray[4] = 0;
				charCatArray[5] = 0;
				charCatArray[6] = 0;
				charCatArray[7] = 0;
				charCatArray[8] = 0;
				charCatArray[9] = 0;
				count = 0;
				whatFunct = 0;
			}
			else if ((theMsg > 47) && (theMsg < 58) && (whatFunct == FLAGSTUFF))		
				theNum = (theNum * 10) + (theMsg - 48);
			else if ((theMsg == 13) && (theNum))	{
			
				if (GameFlag.test(theNum))	{
					GameFlag.clear(theNum);
					sfxPrintf("Clear flag %d", theNum);
				}
				else	{ 
					GameFlag.set(theNum);
					sfxPrintf("Set flag %d", theNum);
				}
				theNum = 0;
				whatFunct = 0;


			}
			return true;
		}
#endif
	return false;
}


void DebugModule::teleport(const char *whereTo)	{
	#ifdef TURNON_1
	if (!strcmp(whereTo, "Rm1"))  {
	     theGame->newRoom(new Room1);
	     return;
	}
	#endif

	#ifdef TURNON_1001
	if (!strcmp(whereTo, "Rm1001"))   {
	     theGame->newRoom(new Rm1001);
	     return;
	}
	#endif

	#ifdef TURNON_1005
	if (!strcmp(whereTo, "Rm1005"))   {
	     theGame->newRoom(new Rm1005);
	     return;
	}
	#endif

	#ifdef TURNON_1035
	if (!strcmp(whereTo, "Rm1035"))   {
	     theGame->newRoom(new Rm1035);
	     return;
	}
	#endif

	#ifdef TURNON_1070
	if (!strcmp(whereTo, "Rm1070"))   {
	     theGame->newRoom(new Rm1070);
	     return;
	}
	#endif

	#ifdef TURNON_1075
	if (!strcmp(whereTo, "Rm1075"))   {
	     theGame->newRoom(new Rm1075);
	     return;
	}
	#endif

	#ifdef TURNON_1085
	if (!strcmp(whereTo, "Rm1085"))   {
	     theGame->newRoom(new Rm1085);
	     return;
	}
	#endif

	#ifdef TURNON_1090
	if (!strcmp(whereTo, "Rm1090"))   {
	     theGame->newRoom(new Rm1090);
	     return;
	}
	else if (!strcmp(whereTo, "Rm1091"))   {
	     theGame->newRoom(new Rm1091);
	     return;
	}
	#endif

	#ifdef TURNON_1000
	if (!strcmp(whereTo, "Rm1000"))  {
	     theGame->newRoom(new Rm1000 );
	     return;
	}
	#endif

	#ifdef TURNON_1270
	if (!strcmp(whereTo, "Rm1270"))   {
	     theGame->newRoom(new Rm1270);
	     return;
	}
	#endif

	#ifdef TURNON_2000	
	if (!strcmp(whereTo, "Rm2000"))   {
	     theGame->newRoom(new Rm2000);
	     return;
	}
	#endif

	#ifdef TURNON_2000	// 2001
	if (!strcmp(whereTo, "Rm2001"))   {
	     theGame->newRoom(new Rm2001);
	     return;
	}
	#endif

	#ifdef TURNON_5000
	if (!strcmp(whereTo, "Rm5000"))   {
	     theGame->newRoom(new Rm5000);
	     return;
	}
	if (!strcmp(whereTo, "Rm5100"))   {
	     theGame->newRoom(new Rm5100);
	     return;
	}
	if (!strcmp(whereTo, "Rm5200"))   {
	     theGame->newRoom(new Rm5200);
	     return;
	}
	#endif

	#ifdef TURNON_7000
	if (!strcmp(whereTo, "Rm7000"))   {
	     theGame->newRoom(new Rm7000);
	     return;
	}
	#endif

	#ifdef TURNON_10000
	if (!strcmp(whereTo, "Rm10000"))  {
		theGame->newRoom(new Rm10000);
		return;
	}
	if (!strcmp(whereTo, "Rm10100"))  {
		theGame->newRoom(new Rm10100);
		return;
	}
	if (!strcmp(whereTo, "Rm10200"))  {
		theGame->newRoom(new Rm10200);
		return;
	}
	#endif

	#ifdef TURNON_11000
	if (!strcmp(whereTo, "Rm11000"))  {
	     theGame->newRoom(new Rm11000);
	     return;
	}
	#endif

	#ifdef TURNON_12000
	if (!strcmp(whereTo, "Rm12000"))  {
	     theGame->newRoom(new Rm12000);
	     return;
	}
	#endif
  
	#ifdef TURNON_18000
	if (!strcmp(whereTo, "Rm18000"))  {
	     theGame->newRoom(new Rm18000);
	     return;
	}
	#endif

	#ifdef TURNON_24000
	if (!strcmp(whereTo, "Rm24000"))  {
	     theGame->newRoom(new Rm24000);
	     return;
	}
	if (!strcmp(whereTo, "Rm24100"))  {
	     theGame->newRoom(new Rm24100);
	     return;
	}
	#endif

	#ifdef TURNON_27000
	if (!strcmp(whereTo, "Rm27000"))  {
	     theGame->newRoom(new Rm27000);
	     return;
	}
	if (!strcmp(whereTo, "Rm27100"))  {
	     theGame->newRoom(new Rm27100);
	     return;
	}
	if (!strcmp(whereTo, "Rm27200"))  {
	     theGame->newRoom(new Rm27200);
	     return;
	}
	#endif

	#ifdef TURNON_22000
	if (!strcmp(whereTo, "Rm22000"))  {
	     theGame->newRoom(new Rm22000);
	     return;
	}
	#endif

	#ifdef TURNON_26000
	if (!strcmp(whereTo, "Rm26000"))  {
	     theGame->newRoom(new Rm26000);
	     return;
	}
	if (!strcmp(whereTo, "Rm26100"))  {
	     theGame->newRoom(new Rm26100);
	     return;
	}
	#endif

	#ifdef TURNON_36000
	if (!strcmp(whereTo, "Rm36000"))  {
	     theGame->newRoom(new Rm36000);
	     return;
	}
	#endif

	#ifdef TURNON_47000
	if (!strcmp(whereTo, "Rm47000"))  {
	     theGame->newRoom(new Rm47000);
	     return;
	}
	#endif

	#ifdef TURNON_34000
	if (!strcmp(whereTo, "Rm34000"))  {
	     theGame->newRoom(new Rm34000);
	     return;
	}
	#endif

	#ifdef TURNON_42000
	if (!strcmp(whereTo, "Rm42000"))  {
	     theGame->newRoom(new Rm42000);
	     return;
	}
	#endif

	#ifdef TURNON_37000
	if (!strcmp(whereTo, "Rm37000"))  {
    
	    //new(BadMan);
    
	    theGame->newRoom(new Rm37000);
	     return;
	}
	#endif

	#ifdef TURNON_38000
	if (!strcmp(whereTo, "Rm38000"))  {
	     theGame->newRoom(new Rm38000);
	     return;
	}
	#endif

	#ifdef TURNON_39000
	if (!strcmp(whereTo, "Rm39000"))  {
	     theGame->newRoom(new Rm39000);
	     return;
	}
	#endif

	#ifdef TURNON_43000
	if (!strcmp(whereTo, "Rm43000"))  {
	     theGame->newRoom(new Rm43000);
	     return;
	}
	#endif

	#ifdef TURNON_44000
	if (!strcmp(whereTo, "Rm44000"))  {
	     theGame->newRoom(new Rm44000);
	     return;
	}
	#endif

	#ifdef TURNON_45000
	if (!strcmp(whereTo, "Rm45000"))  {
	     theGame->newRoom(new Rm45000);
	     return;
	}
	#endif

	#ifdef TURNON_46000
	if (!strcmp(whereTo, "Rm46000"))  {
	     theGame->newRoom(new Rm46000);
	     return;
	}
	#endif

	#ifdef TURNON_35000
	if (!strcmp(whereTo, "Rm35000"))  {
	     theGame->newRoom(new Rm35000);
	     return;
	}
	#endif

	#ifdef TURNON_25000
	if (!strcmp(whereTo, "Rm25000"))  {
	     theGame->newRoom(new Rm25000);
	     return;
	}
	#endif

	#ifdef TURNON_25100
	if (!strcmp(whereTo, "Rm25100"))  {
	     theGame->newRoom(new Rm25100);
	     return;
	}
	#endif

	#ifdef TURNON_50000
	if (!strcmp(whereTo, "Rm50000"))  {
	     theGame->newRoom(new Rm50000);
	     return;
	}
	#endif

	#ifdef TURNON_50001
	if (!strcmp(whereTo, "Rm50001"))  {
	     theGame->newRoom(new Rm50001);
	     return;
	}
	#endif

	#ifdef TURNON_50100
	if (!strcmp(whereTo, "Rm50100"))  {
	     theGame->newRoom(new Rm50100);
	     return;
	}
	#endif

	#ifdef TURNON_50200
	if (!strcmp(whereTo, "Rm50200"))  {
	     theGame->newRoom(new Rm50200);
	     return;
	}
	#endif

	#ifdef TURNON_50300
	if (!strcmp(whereTo, "Rm50300"))  {
	     theGame->newRoom(new Rm50300);
	     return;
	}
	#endif

	#ifdef TURNON_41000
	if (!strcmp(whereTo, "Rm41000"))  {
	     theGame->newRoom(new Rm41000);
	     return;
	}
	#endif

	#ifdef TURNON_53000
	if (!strcmp(whereTo, "Rm53000"))  {
	     theGame->newRoom(new Rm53000);
	     return;
	}
	#endif

	#ifdef TURNON_56000
	if (!strcmp(whereTo, "Rm56000"))  {
	     theGame->newRoom(new Rm56000);
	     return;
	}
	#endif

	#ifdef TURNON_56001
	if (!strcmp(whereTo, "Rm56001"))  {
	     theGame->newRoom(new Rm56001);
	     return;
	}
	#endif

	#ifdef TURNON_57000
	if (!strcmp(whereTo, "Rm57000"))  {
	     theGame->newRoom(new Rm57000);
	     return;
	}
	#endif

	#ifdef TURNON_59000
	if (!strcmp(whereTo, "Rm59000"))  {
	     theGame->newRoom(new Rm59000);
	     return;
	}
	#endif

	#ifdef TURNON_60000
	if (!strcmp(whereTo, "Rm60000"))  {
	     theGame->newRoom(new Rm60000);
	     return;
	}
	if (!strcmp(whereTo, "Rm60500"))  {
	     theGame->newRoom(new Rm60500);
	     return;
	}
	#endif

	#ifdef TURNON_48000
	if (!strcmp(whereTo, "Rm48000"))  {
	     theGame->newRoom(new Rm48000);
	     return;
	}
	#endif

	#ifdef TURNON_48200
	if (!strcmp(whereTo, "Rm48200"))  {
	     theGame->newRoom(new Rm48200);
	     return;
	}
	#endif

	#ifdef TURNON_48300
	if (!strcmp(whereTo, "Rm48300"))  {
	     theGame->newRoom(new Rm48300);
	     return;
	}
	#endif

	#ifdef TURNON_69002
	if (!strcmp(whereTo, "Rm69002"))  {
	     theGame->newRoom(new Rm69002);
	     return;
	}
	#endif

	#ifdef TURNON_69000
	if (!strcmp(whereTo, "Rm69000"))  {
	     theGame->newRoom(new Rm69000);
	     return;
	}
	#endif

	#ifdef TURNON_69100
	if (!strcmp(whereTo, "Rm69100"))  {
	     theGame->newRoom(new Rm69100);
	     return;
	}
	#endif

	#ifdef TURNON_69101
	if (!strcmp(whereTo, "Rm69101"))  {
	     theGame->newRoom(new Rm69101);
	     return;
	}
	#endif

	#ifdef TURNON_69102
	if (!strcmp(whereTo, "Rm69102"))  {
	     theGame->newRoom(new Rm69102);
	     return;
	}
	#endif
	
	#ifdef TURNON_69200
	if (!strcmp(whereTo, "Rm69200"))  {
	     theGame->newRoom(new Rm69200);
	     return;
	}
	#endif

	#ifdef TURNON_23000
	if (!strcmp(whereTo, "Rm23000"))  {
	     theGame->newRoom(new Rm23000);
	     return;
	}
	#endif
	
	#ifdef TURNON_23001
	if (!strcmp(whereTo, "Rm23001"))  {
	     theGame->newRoom(new Rm23001);
	     return;
	}
	#endif
 	
 	#ifdef TURNON_23100
	if (!strcmp(whereTo, "Rm23100"))  {
	     theGame->newRoom(new Rm23100);
	     return;
	}
	#endif

	#ifdef TURNON_19000
	if (!strcmp(whereTo, "Rm19000"))  {
	     theGame->newRoom(new Rm19000);
	     return;
	}
	#endif

	#ifdef TURNON_93000
	if (!strcmp(whereTo, "Rm93000"))  {
	     theGame->newRoom(new Rm93000);
	     return;
	}
	#endif

	#ifdef TURNON_32000
	if (!strcmp(whereTo, "Rm32000"))  {
	     theGame->newRoom(new Rm32000);
	     return;
	}
	#endif

	#ifdef TURNON_41500
	if (!strcmp(whereTo, "Rm41500"))  {
	     theGame->newRoom(new Rm41500);
	     return;
	}
	#endif

	#ifdef TURNON_35200
	if (!strcmp(whereTo, "Rm35200"))   {
	     theGame->newRoom(new Rm35200);
	     return;
	}
	#endif

	#ifdef TURNON_37200
	if (!strcmp(whereTo, "Rm37200"))  {
	     theGame->newRoom(new Rm37200);
	     return;
	}
	#endif
	
	#ifdef TURNON_37300
	if (!strcmp(whereTo, "Rm37300"))  {
	     theGame->newRoom(new Rm37300);
	     return;
	}
	#endif

	#ifdef TURNON_36400
	if (!strcmp(whereTo, "Rm36400"))   {
	     theGame->newRoom(new Rm36400);
	     return;
	}
	#endif

	#ifdef TURNON_36500
	if (!strcmp(whereTo, "Rm36500"))  {
	     theGame->newRoom(new Rm36500);
	     return;
	}
	#endif

	#ifdef TURNON_48400
	if (!strcmp(whereTo, "Rm48400"))   {
	     theGame->newRoom(new Rm48400);
	     return;
	}
	#endif

	#ifdef TURNON_48500
	if (!strcmp(whereTo, "Rm48500"))  {
	     theGame->newRoom(new Rm48500);
	     return;
	}
	#endif

	#ifdef TURNON_57100
	if (!strcmp(whereTo, "Rm57100"))  {
	     theGame->newRoom(new Rm57100);
	     return;
	}
	#endif

	#ifdef TURNON_36600
	if (!strcmp(whereTo, "Rm36600"))  {
	     theGame->newRoom(new Rm36600);
	     return;
	}
	#endif

	#ifdef TURNON_36700
	if (!strcmp(whereTo, "Rm36700"))  {
	     theGame->newRoom(new Rm36700);
	     return;
	}
	#endif

	#ifdef TURNON_35100
	if (!strcmp(whereTo, "Rm35100"))  {
	     theGame->newRoom(new Rm35100);
	     return;
	}
	#endif

	#ifdef TURNON_32100
	if (!strcmp(whereTo, "Rm32100"))  {
	     theGame->newRoom(new Rm32100);
	     return;
	}
	#endif

	#ifdef TURNON_32200
	if (!strcmp(whereTo, "Rm32200"))  {
	     theGame->newRoom(new Rm32200);
	     return;
	}
	#endif

	#ifdef TURNON_32300
	if (!strcmp(whereTo, "Rm32300"))  {
	     theGame->newRoom(new Rm32300);
	     return;
	}
	#endif

	#ifdef TURNON_35400
	if (!strcmp(whereTo, "Rm35400"))  {
	     theGame->newRoom(new Rm35400);
	     return;
	}
	#endif

	#ifdef TURNON_36100
	if (!strcmp(whereTo, "Rm36100"))  {
	     theGame->newRoom(new Rm36100);
	     return;
	}
	#endif

	#ifdef TURNON_36200
	if (!strcmp(whereTo, "Rm36200"))  {
	     theGame->newRoom(new Rm36200);
	     return;
	}
	#endif

	#ifdef TURNON_36300
	if (!strcmp(whereTo, "Rm36300"))  {
	     theGame->newRoom(new Rm36300);
	     return;
	}
	#endif
	
	#ifdef TURNON_36301
	if (!strcmp(whereTo, "Rm36301"))  {
	     theGame->newRoom(new Rm36301);
	     return;
	}
	#endif

	#ifdef TURNON_36302
	if (!strcmp(whereTo, "Rm36302"))  {
	     theGame->newRoom(new Rm36302);
	     return;
	}
	#endif
	
	#ifdef TURNON_36303
	if (!strcmp(whereTo, "Rm36303"))  {
	     theGame->newRoom(new Rm36303);
	     return;
	}
	#endif

	#ifdef TURNON_56100
	if (!strcmp(whereTo, "Rm56100"))  {
	     theGame->newRoom(new Rm56100);
	     return;
	}
	#endif

	#ifdef TURNON_56101
	if (!strcmp(whereTo, "Rm56101"))  {
	     theGame->newRoom(new Rm56101);
	     return;
	}
	#endif

	#ifdef TURNON_35500
	if (!strcmp(whereTo, "Rm35500"))  {
	     theGame->newRoom(new Rm35500);
	     return;
	}
	#endif

	#ifdef TURNON_35600
	if (!strcmp(whereTo, "Rm35600"))  {
	     theGame->newRoom(new Rm35600);
	     return;
	}
	#endif

	#ifdef TURNON_35700
	if (!strcmp(whereTo, "Rm35700"))  {
	     theGame->newRoom(new Rm35700);
	     return;
	}
	#endif

	#ifdef TURNON_79000
	if (!strcmp(whereTo, "Rm79000"))  {
	     theGame->newRoom(new Rm79000);
	     return;
	}
	#endif

	#ifdef TURNON_80000
	if (!strcmp(whereTo, "Rm80000"))  {
	     theGame->newRoom(new Rm80000);
	     return;
	}
	#endif

	#ifdef TURNON_80000	// 80100
	if (!strcmp(whereTo, "Rm80100"))  {
	     theGame->newRoom(new Rm80100);
	     return;
	}
	#endif

	#ifdef TURNON_80000	// 80200
	if (!strcmp(whereTo, "Rm80200"))  {
	     theGame->newRoom(new Rm80200);
	     return;
	}
	#endif

	#ifdef TURNON_96000
	if (!strcmp(whereTo, "Rm96000"))  {
	     theGame->newRoom(new Rm96000);
	     return;
	}
	if (!strcmp(whereTo, "Rm96100"))  {
	     theGame->newRoom(new Rm96100);
	     return;
	}
	if (!strcmp(whereTo, "Rm96200"))  {
	     theGame->newRoom(new Rm96200);
	     return;
	}
	if (!strcmp(whereTo, "Rm96300"))  {
	     theGame->newRoom(new Rm96300);
	     return;
	}
	#endif

	#ifdef TURNON_96100	// 96100
	if (!strcmp(whereTo, "Rm96000"))  {
	     theGame->newRoom(new Rm96000);
	     return;
	}
	#endif

	#ifdef TURNON_96200	// 96200
	if (!strcmp(whereTo, "Rm96000"))  {
	     theGame->newRoom(new Rm96000);
	     return;
	}
	#endif

	#ifdef TURNON_41100	// 41100
	if (!strcmp(whereTo, "Rm41100"))  {
	     theGame->newRoom(new Rm41100);
	     return;
	}
	#endif

	#ifdef TURNON_41200	// 41200
	if (!strcmp(whereTo, "Rm41200"))  {
	     theGame->newRoom(new Rm41200);
	     return;
	}
	#endif

	#ifdef TURNON_41300
	if (!strcmp(whereTo, "Rm41300"))  {
	     theGame->newRoom(new Rm41300);
	     return;
	}
	#endif

	#ifdef TURNON_41400	
	if (!strcmp(whereTo, "Rm41400"))  {
	     theGame->newRoom(new Rm41400);
	     return;
	}
	#endif

	#ifdef TURNON_90000	// 90000
	if (!strcmp(whereTo, "Rm90000"))  {
	     theGame->newRoom(new Rm90000);
	     return;
	}
	#endif

	#ifdef TURNON_90000	// 90001
	if (!strcmp(whereTo, "Rm90001"))  {
	     theGame->newRoom(new Rm90001);
	     return;
	}
	#endif

	#ifdef TURNON_91000	// 91000
	if (!strcmp(whereTo, "Rm91000"))  {
	     theGame->newRoom(new Rm91000);
	     return;
	}
	#endif

	#ifdef TURNON_93010	// 93010
	if (!strcmp(whereTo, "Rm93010"))  {
	     theGame->newRoom(new Rm93010);
	     return;
	}
	#endif

	#ifdef TURNON_93020	// 93020
	if (!strcmp(whereTo, "Rm93020"))  {
	     theGame->newRoom(new Rm93020);
	     return;
	}
	#endif
	
	#ifdef TURNON_35300
   	if (!strcmp(whereTo, "Rm35300"))  {
         theGame->newRoom(new Rm35300);
         return;
   	}
   	#endif

	#ifdef TURNON_35001
   	if (!strcmp(whereTo, "Rm35001"))  {
         theGame->newRoom(new Rm35001);
         return;
   	}
   	#endif

	#ifdef TURNON_35002
   	if (!strcmp(whereTo, "Rm35002"))  {
         theGame->newRoom(new Rm35002);
         return;
   	}
   	#endif

	#ifdef TURNON_38100
   	if (!strcmp(whereTo, "Rm38100"))  {
         theGame->newRoom(new Rm38100);
         return;
   	}
   	#endif

	#ifdef TURNON_38200
   	if (!strcmp(whereTo, "Rm38200"))  {
         theGame->newRoom(new Rm38200);
         return;
   	}
   	#endif
	
	#ifdef TURNON_38300
   	if (!strcmp(whereTo, "Rm38300"))  {
         theGame->newRoom(new Rm38300);
         return;
   	}
   	#endif

	#ifdef TURNON_38400
   	if (!strcmp(whereTo, "Rm38400"))  {
         theGame->newRoom(new Rm38400);
         return;
   	}
   	#endif

	#ifdef TURNON_38500
   	if (!strcmp(whereTo, "Rm38500"))  {
         theGame->newRoom(new Rm38500);
         return;
   	}
   	#endif

	sfxPrintf("Error: Corrupted save game");
}

} // namespace Spycraft
