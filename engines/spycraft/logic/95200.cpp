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

#include "95200.h"

namespace Spycraft {

int targets95200[19][15]={    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//0
		  					  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//1	
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//2
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//3
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//4
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//5
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//6
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//7
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//8
				 			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//9
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//10
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//11
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//12
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//13
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//14
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//15
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//16
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//17
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};	//18
/*********************
*
*		Room Code 
*
**********************/
Rm95200::Rm95200()
{
	name = "95200";
	number = 95200;
	show_style = VE_FLIP;
}

void Rm95200::init()
{
	new ShootBack(95230);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthEastExit95200);
	new(SouthWestExit95200);

	g1 = 95231;
	g2 = 95233;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95013;
	f2 = 95014;
	
	gogglesStart(0,2);
}

Rm95201::Rm95201()
{
	name = "95201";
	number = 95201;
	show_style = VE_FLIP;
}

void Rm95201::init()
{
	if (GameFlag.test(fStartCombat))
	{
		drawPic (-1);
		sfxLoadRes(623, RES_ATS);
		sfxLockRes(623, RES_ATS);
	}

	new ShootBack(95210);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95200);
	new(NorthEastExit95200);
	new(SouthWestExit95200);

	if (GameFlag.test(fStartCombat))
	{
		grendlPos = 0;
		intrface->disable();
		normalCursor = gunCursor;
		base = 95200;
		memcpy (&T_list,&targets95200,((19*15)*sizeof(int)));
		memcpy (&enePos,&coords,(19*sizeof(MADEPt)));

		memset (&E_map,0,(45*sizeof(int)));
		memset (&P_map,0,(45*sizeof(int)));
		E_map [2][2] = NOGO;
		E_map [3][1] = NOGO;
		E_map [3][3] = NOGO;
		E_map [4][4] = NOGO;
		E_map [5][1] = NOGO;
		E_map [5][3] = NOGO;
		E_map [6][2] = NOGO;
		
		addshooters = FALSE;

		theMusic->loop = -1;
		theMusic->setVolume(5);
		theMusic->play(95200);
		elapsed = NULL;
	}
	
	g1 = 95213;
	g2 = 95212;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95015;
	f2 = 95016;
	
	gogglesStart(1,1);
	GameFlag.clear(fStartCombat);
}

Rm95202::Rm95202()
{
	name = "95202";
	number = 95202;
	show_style = VE_FLIP;
}

void Rm95202::init()
{
	new ShootBack(95210);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95200);
	new(SouthWestExit95200);
	new(NorthWestExit95200);

	g1 = 95212;
	g2 = 95213;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95017;
	f2 = 95018;
	
	gogglesStart(1,3);
}

Rm95203::Rm95203()
{
	name = "95203";
	number = 95203;
	show_style = VE_FLIP;
}

void Rm95203::init()
{
	if (GameFlag.test(fStartCombat))
	{
		drawPic (-1);
		sfxLoadRes(623, RES_ATS);
		sfxLockRes(623, RES_ATS);
	}
	new ShootBack(95220);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95200);
	new(NorthEastExit95200);
	new(NorthWestExit95200);
	if (GameFlag.test(fStartCombat))
	{
		grendlPos = 0;
		intrface->disable();
		normalCursor = gunCursor;
		base = 95200;
		memcpy (&T_list,&targets95200,((19*15)*sizeof(int)));
		memcpy (&enePos,&coords,(19*sizeof(MADEPt)));

		memset (&E_map,0,(45*sizeof(int)));
		memset (&P_map,0,(45*sizeof(int)));
		E_map [2][2] = NOGO;
		E_map [3][1] = NOGO;
		E_map [3][3] = NOGO;
		E_map [4][4] = NOGO;
		E_map [5][1] = NOGO;
		E_map [5][3] = NOGO;
		E_map [6][2] = NOGO;
		
		addshooters = FALSE;

		theMusic->loop = -1;
		theMusic->setVolume(5);
		theMusic->play(95200);
		elapsed = NULL;
	}

	g1 = 95221;
	g2 = 95223;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95013;
	f2 = 95014;
	
	gogglesStart(2,0);
	GameFlag.clear(fStartCombat);
}

Rm95204::Rm95204()
{
	name = "95204";
	number = 95204;
	show_style = VE_FLIP;
}

void Rm95204::init()
{
	if (GameFlag.test(fStartCombat))
	{
		drawPic (-1);
		sfxLoadRes(623, RES_ATS);
		sfxLockRes(623, RES_ATS);
	}
	new ShootBack(95220);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95200);
	new(NorthEastExit95200);
	new(SouthWestExit95200);
	if (GameFlag.test(fStartCombat))
	{
		grendlPos = 0;
		intrface->disable();
		normalCursor = gunCursor;
		base = 95200;
		memcpy (&T_list,&targets95200,((19*15)*sizeof(int)));
		memcpy (&enePos,&coords,(19*sizeof(MADEPt)));

		memset (&E_map,0,(45*sizeof(int)));
		memset (&P_map,0,(45*sizeof(int)));
		E_map [2][2] = NOGO;
		E_map [3][1] = NOGO;
		E_map [3][3] = NOGO;
		E_map [4][4] = NOGO;
		E_map [5][1] = NOGO;
		E_map [5][3] = NOGO;
		E_map [6][2] = NOGO;
		
		addshooters = FALSE;

		theMusic->loop = -1;
		theMusic->setVolume(5);
		theMusic->play(95200);
		elapsed = NULL;
	}

	g1 = 95222;
	g2 = 95224;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95015;
	f2 = 95016;

//	if (!GameFlag.test(fRefineryCombat))
//		GameFlag.set(fGotBeacon);

	gogglesStart(2,2);
	GameFlag.clear(fStartCombat);
}

Rm95205::Rm95205()
{
	name = "95205";
	number = 95205;
	show_style = VE_FLIP;
}

void Rm95205::init()
{
	new ShootBack(95220);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95200);
	new(SouthWestExit95200);
	new(NorthExit95200);
	new(NorthEastExit95200);

	g1 = 95221;
	g2 = 95223;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95017;
	f2 = 95018;
	
	gogglesStart(2,4);
}
Rm95206::Rm95206()
{
	name = "95206";
	number = 95206;
	show_style = VE_FLIP;
}

void Rm95206::init()
{
	if (GameFlag.test(fStartCombat))
	{
		drawPic (-1);
		sfxLoadRes(623, RES_ATS);
		sfxLockRes(623, RES_ATS);
	}
	new ShootBack(95292);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95200);
	new(NorthExit95200);
	new(NorthEastExit95200);
	new(SouthWestExit95200);
	if (GameFlag.test(fStartCombat))
	{
		grendlPos = 0;
		intrface->disable();
		normalCursor = gunCursor;
		base = 95200;
		memcpy (&T_list,&targets95200,((19*15)*sizeof(int)));
		memcpy (&enePos,&coords,(19*sizeof(MADEPt)));

		memset (&E_map,0,(45*sizeof(int)));
		memset (&P_map,0,(45*sizeof(int)));
		E_map [2][2] = NOGO;
		E_map [3][1] = NOGO;
		E_map [3][3] = NOGO;
		E_map [4][4] = NOGO;
		E_map [5][1] = NOGO;
		E_map [5][3] = NOGO;
		E_map [6][2] = NOGO;
		
		addshooters = FALSE;

		theMusic->loop = -1;
		theMusic->setVolume(5);
		theMusic->play(95200);
		elapsed = NULL;
	}

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	
	gogglesStart(3,1);
	GameFlag.clear(fStartCombat);
}
Rm95207::Rm95207()
{
	name = "95207";
	number = 95207;
	show_style = VE_FLIP;
}

void Rm95207::init()
{
	new ShootBack(95294);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95200);
	new(NorthEastExit95200);
	new(SouthWestExit95200);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	
	gogglesStart(3,3);
}
Rm95208::Rm95208()
{
	name = "95208";
	number = 95208;
	show_style = VE_FLIP;
}

void Rm95208::init()
{
	new ShootBack(95230);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95200);
	new(NorthEastExit95200);
	new(SouthEastExit95200);

	g1 = 95232;
	g2 = 95233;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95013;
	f2 = 95014;
	
	gogglesStart(4,0);
}
Rm95209::Rm95209()
{
	name = "95209";
	number = 95209;
	show_style = VE_FLIP;
}

void Rm95209::init()
{
	if (GameFlag.test(fStartCombat))
	{
		drawPic (-1);
		sfxLoadRes(623, RES_ATS);
		sfxLockRes(623, RES_ATS);
	}
	new ShootBack(95295);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95200);
	new(NorthExit95200);
	new(NorthEastExit95200);
	new(SouthEastExit95200);
	if (GameFlag.test(fStartCombat))
	{
		grendlPos = 0;
		intrface->disable();
		normalCursor = gunCursor;
		base = 95200;
		memcpy (&T_list,&targets95200,((19*15)*sizeof(int)));
		memcpy (&enePos,&coords,(19*sizeof(MADEPt)));

		memset (&E_map,0,(45*sizeof(int)));
		memset (&P_map,0,(45*sizeof(int)));
		E_map [2][2] = NOGO;
		E_map [3][1] = NOGO;
		E_map [3][3] = NOGO;
		E_map [4][4] = NOGO;
		E_map [5][1] = NOGO;
		E_map [5][3] = NOGO;
		E_map [6][2] = NOGO;
		
		addshooters = FALSE;

		theMusic->loop = -1;
		theMusic->setVolume(5);
		theMusic->play(95200);
		elapsed = NULL;
	}

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	
	gogglesStart(4,2);
	GameFlag.clear(fStartCombat);
}

Rm95210::Rm95210()
{
	name = "95210";
	number = 95210;
	show_style = VE_FLIP;
}

void Rm95210::init()
{
	new ShootBack(95230);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95200);
	new(NorthExit95200);
	new(SouthWestExit95200);

	g1 = 95231;
	g2 = 95233;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95015;
	f2 = 95016;
			 
 	gogglesStart(4,4);
}
Rm95211::Rm95211()
{
	name = "95211";
	number = 95211;
	show_style = VE_FLIP;
}

void Rm95211::init()
{
	new ShootBack(95293);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95200);
	new(SouthEastExit95200);
	new(NorthWestExit95200);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	
	gogglesStart(5,1);
}
Rm95212::Rm95212()
{
	name = "95212";
	number = 95212;
	show_style = VE_FLIP;
}

void Rm95212::init()
{
	new ShootBack(95290);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95200);
	new(NorthEastExit95200);
	new(NorthWestExit95200);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	
	gogglesStart(5,3);
}
Rm95213::Rm95213()
{
	name = "95213";
	number = 95213;
	show_style = VE_FLIP;
}

void Rm95213::init()
{
	new ShootBack(95220);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95200);
	new(SouthExit95200);
	new(SouthEastExit95200);

	g1 = 95221;
	g2 = 95223;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95017;
	f2 = 95018;
	
	gogglesStart(6,0);
}
Rm95214::Rm95214()
{
	name = "95214";
	number = 95214;
	show_style = VE_FLIP;
}

void Rm95214::init()
{
	new ShootBack(95291);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95200);
	new(SouthEastExit95200);
	new(SouthWestExit95200);
	new(NorthWestExit95200);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	
	gogglesStart(6,2);
}
Rm95215::Rm95215()
{
	name = "95215";
	number = 95215;
	show_style = VE_FLIP;
}

void Rm95215::init()
{
	new ShootBack(95210);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95200);
	new(SouthExit95200);
	new(SouthWestExit95200);
	new(SouthEastExit95200);

	g1 = 95213;
	g2 = 95212;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95013;
	f2 = 95014;
	
	gogglesStart(6,4);
}
Rm95216::Rm95216()
{
	name = "95216";
	number = 95216;
	show_style = VE_FLIP;
}

void Rm95216::init()
{
	new ShootBack(95210);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthWestExit95200);
	new(NorthEastExit95200);
	new(SouthEastExit95200);
	new(NorthWestExit95200);

	g1 = 95212;
	g2 = 95213;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95015;
	f2 = 95016;
	
	gogglesStart(7,1);
}
Rm95217::Rm95217()
{
	name = "95217";
	number = 95217;
	show_style = VE_FLIP;
}

void Rm95217::init()
{
	new ShootBack(95220);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95200);
	new(NorthEastExit95200);
	new(NorthWestExit95200);

	g1 = 95222;
	g2 = 95224;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95017;
	f2 = 95018;
	
	gogglesStart(7,3);
}
Rm95218::Rm95218()
{
	name = "95218";
	number = 95218;
	show_style = VE_FLIP;
}

void Rm95218::init()
{
	if (GameFlag.test(fStartCombat))
	{
		drawPic (-1);
		sfxLoadRes(623, RES_ATS);
		sfxLockRes(623, RES_ATS);
		Hp = 5;
	}

	new ShootBack(95230);
	Room::init();	  
	theGame->handsOn();

	gridnumber = number;


	new(SouthExit95200);
	new(NorthWestExit95200);

	if (GameFlag.test(fStartCombat))
	{
		GameFlag.clear(fGotKilled);
		GameFlag.clear(fGotBeacon);
		GameFlag.clear(fRefineryCombat);
		intrface->disable();
		normalCursor = gunCursor;
		base = 95200;
		memcpy (&T_list,&targets95200,((19*15)*sizeof(int)));
		memcpy (&enePos,&coords,(19*sizeof(MADEPt)));

		memset (&E_map,0,(45*sizeof(int)));
		memset (&P_map,0,(45*sizeof(int)));

/*		E_map [4][0] = 2;
		E_map [1][3] = 2;
		E_map [6][4] = 2;

		P_map [2][2] = 2;

		P_map [0][2] = 3;
		P_map [1][1] = 3;					  
		P_map [1][3] = 3;
		P_map [2][0] = 3;
		P_map [2][4] = 3;
		P_map [4][0] = 3;
		P_map [4][4] = 3;
		P_map [6][0] = 3;
		P_map [6][4] = 3;
		P_map [7][1] = 3;
		P_map [7][3] = 3;
*/
		E_map [2][2] = NOGO;
		E_map [3][1] = NOGO;
		E_map [3][3] = NOGO;
		E_map [4][4] = NOGO;
		E_map [5][1] = NOGO;
		E_map [5][3] = NOGO;
		E_map [6][2] = NOGO;
		
		addshooters = FALSE;
		elapsed = NULL;
	}


	g1 = 95231;
	g2 = 95233;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95013;
	f2 = 95014;
	
	gogglesStart(8,2);
	setScript(new RefScript);
}

/*********************
*
*		ExitFeatures 
*
**********************/
void NorthEastExit95200::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northeastCursor);
}
void NorthWestExit95200::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northwestCursor);
}
void NorthExit95200::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northCursor);
}
void SouthEastExit95200::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southeastCursor);
}
void SouthWestExit95200::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southwestCursor);
}
void SouthExit95200::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southCursor);
}

NorthEastExit95200::NorthEastExit95200()
{
	name = "NorthEastExit";
	setHotspot ( GUN_V );
	init ( GUN_NORTHEAST_EXIT_X, GUN_NORTHEAST_EXIT_Y,100,150);
	arrow = new Arrow(1,487,14,(ExitFeature*)this);
}

int NorthEastExit95200::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();
		switch (gridnumber)
		{
			case 95201:
				theGame->newRoom(new Rm95200);
				break;
			case 95203:
				if (grendlPos == 95201)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95201);
				break;
			case 95204:
				theGame->newRoom(new Rm95202);
				break;
			case 95205:
				endPuzzle();
				break;
			case 95206:
				if (grendlPos == 95204)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95204);
				break;
			case 95207:
				theGame->newRoom(new Rm95205);
				break;
			case 95208:
				if (grendlPos == 95206)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95206);
				break;
			case 95209:
				theGame->newRoom(new Rm95207);
				break;
			case 95212:
				theGame->newRoom(new Rm95210);
				break;
			case 95216:
				theGame->newRoom(new Rm95214);
				break;
			case 95217:
				theGame->newRoom(new Rm95215);
				break;
		}
	    return TRUE;
	}
	return FALSE;
}

NorthWestExit95200::NorthWestExit95200()
{
	name = "NorthWestExit";
 	setHotspot ( GUN_V );

	init ( GUN_NORTHWEST_EXIT_X, GUN_NORTHWEST_EXIT_Y,100,150);
	arrow = new Arrow(2,2,12,(ExitFeature*)this);
}

int NorthWestExit95200::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();
		switch (gridnumber)
		{
			case 95202:
				theGame->newRoom(new Rm95200);
				break;
			case 95203:
				endPuzzle();
				break;
			case 95211:
				theGame->newRoom(new Rm95208);
				break;
			case 95212:
				if (grendlPos == 95209)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95209);
				break;
			case 95214:
				theGame->newRoom(new Rm95211);
				break;
			case 95215:
				theGame->newRoom(new Rm95212);
				break;
			case 95216:
				theGame->newRoom(new Rm95213);
				break;
			case 95217:
				theGame->newRoom(new Rm95214);
				break;
			case 95218:
				theGame->newRoom(new Rm95216);
				break;
		}
	    return TRUE;
	}
	return FALSE;
}

NorthExit95200::NorthExit95200()
{
	name = "NorthExit";
 	setHotspot ( GUN_V );
	init ( GUN_NORTH_EXIT_X, GUN_NORTH_EXIT_Y,300,100);
	arrow = new Arrow(0,244,10,(ExitFeature*)this);
}

int NorthExit95200::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();
		switch (gridnumber)
		{
			case 95203:
				endPuzzle();
				break;
			case 95205:
				endPuzzle();
				break;
			case 95206:
				if (grendlPos == 95201)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95201);
				break;
			case 95207:
				theGame->newRoom(new Rm95202);
				break;
			case 95209:
				if (grendlPos == 95204)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95204);
				break;
			case 95210:
				theGame->newRoom(new Rm95205);
				break;
			case 95211:
				if (grendlPos == 95206)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95206);
				break;
			case 95213:
				theGame->newRoom(new Rm95208);
				break;
			case 95214:
				if (grendlPos == 95209)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95209);
				break;
			case 95215:
				theGame->newRoom(new Rm95210);
				break;
			case 95217:
				theGame->newRoom(new Rm95212);
				break;
		}
	   	return TRUE;
	}
	return FALSE;
}

SouthEastExit95200::SouthEastExit95200()
{
	name = "SouthEastExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTHEAST_EXIT_X, GUN_SOUTHEAST_EXIT_Y,100,150);
	arrow = new Arrow(4,488,378,(ExitFeature*)this);
}

int SouthEastExit95200::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();
		switch (gridnumber)
		{
			case 95200:
				theGame->newRoom(new Rm95202);
				break;
			case 95208:
				theGame->newRoom(new Rm95211);
				break;
			case 95209:
				theGame->newRoom(new Rm95212);
				break;
			case 95211:
				theGame->newRoom(new Rm95214);
				break;
			case 95213:
				theGame->newRoom(new Rm95216);
				break;
			case 95214:
				theGame->newRoom(new Rm95217);
				break;
			case 95215:
				endPuzzle();
				break;
			case 95216:
				theGame->newRoom(new Rm95218);
				break;
		}
	    return TRUE;
	}
	return FALSE;
}

SouthWestExit95200::SouthWestExit95200()
{
	name = "SouthWestExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTHWEST_EXIT_X, GUN_SOUTHWEST_EXIT_Y,100,150);
	arrow = new Arrow(5,1,376,(ExitFeature*)this);
}

int SouthWestExit95200::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();
		switch (gridnumber)
		{
			case 95200:
				if (grendlPos == 95201)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95201);
				break;
			case 95201:
				if (grendlPos == 95203)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95203);
				break;
			case 95202:
				if (grendlPos == 95204)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95204);
				break;
			case 95204:
				if (grendlPos == 95206)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95206);
				break;
			case 95205:
				theGame->newRoom(new Rm95207);
				break;
			case 95206:
				theGame->newRoom(new Rm95208);
				break;
			case 95207:
				if (grendlPos == 95209)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95209);
				break;
			case 95210:
				theGame->newRoom(new Rm95212);
				break;
			case 95214:
				theGame->newRoom(new Rm95216);
				break;
			case 95215:
				theGame->newRoom(new Rm95217);
				break;
			case 95216:
				endPuzzle();
				break;
		}
	    return TRUE;
	}
	return FALSE;
}

SouthExit95200::SouthExit95200()
{
	name = "SouthExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTH_EXIT_X, GUN_SOUTH_EXIT_Y,300,100);
	arrow = new Arrow(3,243,380,(ExitFeature*)this);
	arrow->setPri(0xffff);
}

int SouthExit95200::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();
		switch (gridnumber)
		{
			case 95201:
				if (grendlPos == 95206)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95206);
				break;
			case 95202:
				theGame->newRoom(new Rm95207);
				break;
			case 95204:
				if (grendlPos == 95209)
				{
					GameFlag.set(fGotBeacon);
					endPuzzle();
				}
				else
					theGame->newRoom(new Rm95209);
				break;
			case 95205:
				theGame->newRoom(new Rm95210);
				break;
			case 95206:
				theGame->newRoom(new Rm95211);
				break;
			case 95208:
				theGame->newRoom(new Rm95213);
				break;
			case 95209:
				theGame->newRoom(new Rm95214);
				break;
			case 95210:
				theGame->newRoom(new Rm95215);
				break;
			case 95212:
				theGame->newRoom(new Rm95217);
				break;
			case 95213:
				endPuzzle();
				break;
			case 95215:
				endPuzzle();
				break;
			case 95218:
				endPuzzle();
				break;
		}
	   	return TRUE;
	}
	return FALSE;
}
 
} // namespace Spycraft 
