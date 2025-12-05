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

#include "spycraft/logic/95000.h"

namespace Spycraft {

int targets95000[19][15]={    9, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//0
		  					 13,  7,  3,  9, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//1	
							  5,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//2
							  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//3
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//4
							  7, 15, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//5
							  8,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//6
							 12,  5, 18,  3,  9, 15, 17,  0,  0,  0,  0,  0,  0,  0,  0,	//7
							 13,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//8
							 12,  7, 18,  3, 11, 15, 17,  0,  0,  0,  0,  0,  0,  0,  0,	//9
							 12, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//10
							  8, 13, 18,  3,  9, 15, 17,  0,  0,  0,  0,  0,  0,  0,  0,	//11
							 15, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//12
							  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//13
							 12, 18, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//14
							 12, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//15
							 13, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//16
							 12, 18, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//17
							 17,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};	//18

/*********************
*
*		Room Code 
*
**********************/
Rm95000::Rm95000()
{
	name = "95000";
	number = 95000;
	show_style = VE_FLIP;
}
void Rm95000::init()
{
	new ShootBack(95165);
	Room::init();
	theGame->handsOn();
	gridnumber = number;

	new(NorthExit95000);
	new(SouthExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);

	g1 = 95065;
	g2 = 95067;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95005;
	f2 = 95006;

	gogglesStart(0,2);
}

Rm95001::Rm95001()
{
	name = "95001";
	number = 95001;
	show_style = VE_FLIP;
}
void Rm95001::init()
{
	new ShootBack(95175);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);

	g1 = 95075;
	g2 = 95076;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95007;
	f2 = 95008;
	
	gogglesStart(1,1);
}

Rm95002::Rm95002()
{
	name = "95002";
	number = 95002;
	show_style = VE_FLIP;
}
void Rm95002::init()
{
	new ShootBack(95175);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 95075;
	g2 = 95076;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95009;
	f2 = 95010;
	gogglesStart(1,3);
}

Rm95003::Rm95003()
{
	name = "95003";
	number = 95003;
	show_style = VE_FLIP;
}
void Rm95003::init()
{
	new ShootBack(95192);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	gogglesStart(2,0);
}

Rm95004::Rm95004()
{
	name = "95004";
	number = 95004;
	show_style = VE_FLIP;
}
void Rm95004::init()
{
	new ShootBack(95130);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 95032;
	g2 = 95031;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95011;
	f2 = 95012;
	gogglesStart(2,2);
}

Rm95005::Rm95005()
{
	name = "95005";
	number = 95005;
	show_style = VE_FLIP;
}
void Rm95005::init()
{
	new ShootBack(95155);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 95056;
	g2 = 95055;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95005;
	f2 = 95006;
	gogglesStart(2,4);
}
Rm95006::Rm95006()
{
	name = "95006";
	number = 95006;
	show_style = VE_FLIP;
}
void Rm95006::init()
{
	new ShootBack(95135);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 95035;
	g2 = 95036;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95007;
	f2 = 95008;
	gogglesStart(3,1);
}
Rm95007::Rm95007()
{
	name = "95007";
	number = 95007;
	show_style = VE_FLIP;
}
void Rm95007::init()
{
	new ShootBack(95170);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 95073;
	g2 = 95071;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95009;
	f2 = 95010;
	gogglesStart(3,3);
}
Rm95008::Rm95008()
{
	name = "95008";
	number = 95008;
	show_style = VE_FLIP;
}
void Rm95008::init()
{
	new ShootBack(95196);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	gogglesStart(4,0);
}
Rm95009::Rm95009()
{
	name = "95009";
	number = 95009;
	show_style = VE_FLIP;
}
void Rm95009::init()
{
	new ShootBack(95165);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 95067;
	g2 = 95066;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95011;
	f2 = 95012;
	gogglesStart(4,2);
}
Rm95010::Rm95010()
{
	name = "95010";
	number = 95010;
	show_style = VE_FLIP;
}
void Rm95010::init()
{
	new ShootBack(95310);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 95311;
	g2 = 95312;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95005;
	f2 = 95006;
	gogglesStart(4,4);
}
Rm95011::Rm95011()
{
	name = "95011";
	number = 95011;
	show_style = VE_FLIP;
}
void Rm95011::init()
{
	new ShootBack(95155);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 95057;
	g2 = 95056;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95007;
	f2 = 95008;

	gogglesStart(5,1);
}
Rm95012::Rm95012()
{
	name = "95012";
	number = 95012;
	show_style = VE_FLIP;
}
void Rm95012::init()
{
	new ShootBack(95194);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

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
Rm95013::Rm95013()
{
	name = "95013";
	number = 95013;
	show_style = VE_FLIP;
}
void Rm95013::init()
{
	new ShootBack(95195);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	gogglesStart(6,0);
}	
Rm95014::Rm95014()
{
	name = "95014";
	number = 95014;
	show_style = VE_FLIP;
}
void Rm95014::init()
{
	new ShootBack(95140);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95000);
	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 95040;
	g2 = 95041;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95009;
	f2 = 95010;
	gogglesStart(6,2);
}
Rm95015::Rm95015()
{
	name = "95015";
	number = 95015;
	show_style = VE_FLIP;
}
void Rm95015::init()
{
	new ShootBack(95191);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	gogglesStart(6,4);
}
Rm95016::Rm95016()
{
	name = "95016";
	number = 95016;
	show_style = VE_FLIP;
}
void Rm95016::init()
{
	new ShootBack(95315);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthEastExit95000);
	new(NorthWestExit95000);

	g1 = 95316;
	g2 = 95318;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95011;
	f2 = 95012;
	gogglesStart(7,1);
}
Rm95017::Rm95017()
{
	name = "95017";
	number = 95017;
	show_style = VE_FLIP;
}
void Rm95017::init()
{
//	drawPic (95000);
	new ShootBack(95190);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95000);
	new(NorthEastExit95000);
	new(SouthWestExit95000);
	new(NorthWestExit95000);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	gogglesStart(7,3);
}
Rm95018::Rm95018()
{
	name = "95018";
	number = 95018;
	show_style = VE_FLIP;
}
void Rm95018::init()
{
	if (GameFlag.test(fStartCombat))
	{
		drawPic (-1);
		sfxLoadRes(623, RES_ATS);
		sfxLockRes(623, RES_ATS);
		Hp = 5;
	}
	new ShootBack(95193);
	Room::init();
	theGame->handsOn();

	gridnumber = number;


	new(NorthExit95000);
	new(NorthEastExit95000);
	new(NorthWestExit95000);

	if (GameFlag.test(fStartCombat))
	{
		intrface->disable();
		normalCursor = gunCursor;
		base = 95000;
		GameFlag.clear(fWPSqueekBy);
		GameFlag.clear(fGotKilled);

		memcpy (&T_list,&targets95000,((19*15)*sizeof(int)));

 		memcpy (&enePos,&coords,(19*sizeof(MADEPt)));

		memset (&E_map,0,(45*sizeof(int)));
		memset (&P_map,0,(45*sizeof(int)));

		E_map [1][1] = 1;
		E_map [4][2] = 1;

		P_map [4][4] = 1;										 

		E_map [2][0] = NOGO;
		E_map [4][0] = NOGO;
		E_map [5][3] = NOGO;
		E_map [6][0] = NOGO;
		E_map [6][4] = NOGO;
		E_map [7][3] = NOGO;
		E_map [8][2] = NOGO;
/*
		P_map [0][2] = 2;										 
		P_map [1][1] = 2;										 
		P_map [1][3] = 2;										 
		P_map [2][2] = 2;										 
		P_map [2][4] = 2;										 
		P_map [3][1] = 2;										 
		P_map [3][3] = 2;										 
		P_map [4][2] = 2;										 
		P_map [4][4] = 2;										 
		P_map [5][1] = 2;										 
		P_map [6][2] = 2;										 
		P_map [7][1] = 2;										 
*/
		secOnes = 0;
		secTens = 0;
		minOnes = 0;
		minTens = 0;

		psecOnes = new SecOnes;
		psecTens = new SecTens;
		pminOnes = new MinOnes;
		pminTens = new MinTens;
		seperator = new Seperator;

	}

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;

	gogglesStart(8,2);
	setScript(new(WPStartScript));
}

/*********************
*
*		ExitFeatures 
*
**********************/
void NorthEastExit95000::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northeastCursor);
}
void NorthWestExit95000::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northwestCursor);
}
void NorthExit95000::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northCursor);
}
void SouthEastExit95000::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southeastCursor);
}
void SouthWestExit95000::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southwestCursor);
}
void SouthExit95000::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southCursor);
}

NorthEastExit95000::NorthEastExit95000()
{
	name = "NorthEastExit";
	setHotspot ( GUN_V );
	init ( GUN_NORTHEAST_EXIT_X, GUN_NORTHEAST_EXIT_Y,100,150);
	arrow = new Arrow(1,487,14,(ExitFeature*)this);
}
int NorthEastExit95000::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();
		
		switch (gridnumber)
		{
			case 95001:
				theGame->newRoom(new Rm95000);
				break;
			case 95003:
				theGame->newRoom(new Rm95001);
				break;
			case 95004:
				theGame->newRoom(new Rm95002);
				break;
			case 95006:
				theGame->newRoom(new Rm95004);
				break;
			case 95007:
				theGame->newRoom(new Rm95005);
				break;
			case 95008:
				theGame->newRoom(new Rm95006);
				break;
			case 95009:
				theGame->newRoom(new Rm95007);
				break;
			case 95011:
				theGame->newRoom(new Rm95009);
				break;
			case 95012:
				theGame->newRoom(new Rm95010);
				break;
			case 95013:
				theGame->newRoom(new Rm95011);
				break;
			case 95014:
				theGame->newRoom(new Rm95012);
				break;
			case 95016:
				theGame->newRoom(new Rm95014);
				break;
			case 95017:
				theGame->newRoom(new Rm95015);
				break;
			case 95018:
				theGame->newRoom(new Rm95017);
				break;
		}
	    return true;
	}
	return false;
}

NorthWestExit95000::NorthWestExit95000()
{
	name = "NorthWestExit";
 	setHotspot ( GUN_V );

	init ( GUN_NORTHWEST_EXIT_X, GUN_NORTHWEST_EXIT_Y,100,150);
	arrow = new Arrow(2,2,12,(ExitFeature*)this);
}

int NorthWestExit95000::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();
		switch (gridnumber)
		{
			case 95002:
				theGame->newRoom(new Rm95000);
				break;
			case 95004:
				theGame->newRoom(new Rm95001);
				break;
			case 95005:
				theGame->newRoom(new Rm95002);
				break;
			case 95006:
				theGame->newRoom(new Rm95003);
				break;
			case 95007:
				theGame->newRoom(new Rm95004);
				break;
			case 95009:
				theGame->newRoom(new Rm95006);
				break;
			case 95010:
				theGame->newRoom(new Rm95007);
				break;
			case 95011:
				theGame->newRoom(new Rm95008);
				break;
			case 95012:
				theGame->newRoom(new Rm95009);
				break;
			case 95014:
				theGame->newRoom(new Rm95011);
				break;
			case 95015:
				theGame->newRoom(new Rm95012);
				break;
			case 95016:
				theGame->newRoom(new Rm95013);
				break;
			case 95017:
				theGame->newRoom(new Rm95014);
				break;
			case 95018:
				theGame->newRoom(new Rm95016);
				break;
		}
	    return true;
	}
	return false;
}

NorthExit95000::NorthExit95000()
{
	name = "NorthExit";
 	setHotspot ( GUN_V );
	init ( GUN_NORTH_EXIT_X, GUN_NORTH_EXIT_Y,300,100);
	arrow = new Arrow(0,244,10,(ExitFeature*)this);
}

int NorthExit95000::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95000:
				endPuzzle();
//				curRoom->setScript(new WPScript);
				break;
			case 95004:
				theGame->newRoom(new Rm95000);
				break;
			case 95006:
				theGame->newRoom(new Rm95001);
				break;
			case 95007:
				theGame->newRoom(new Rm95002);
				break;
			case 95008:
				theGame->newRoom(new Rm95003);
				break;
			case 95009:
				theGame->newRoom(new Rm95004);
				break;
			case 95010:
				theGame->newRoom(new Rm95005);
				break;
			case 95011:
				theGame->newRoom(new Rm95006);
				break;
			case 95012:
				theGame->newRoom(new Rm95007);
				break;
			case 95013:
				theGame->newRoom(new Rm95008);
				break;
			case 95014:
				theGame->newRoom(new Rm95009);
				break;
			case 95015:
				theGame->newRoom(new Rm95010);
				break;
			case 95016:
				theGame->newRoom(new Rm95011);
				break;
			case 95017:
				theGame->newRoom(new Rm95012);
				break;
			case 95018:
				theGame->newRoom(new Rm95014);
				break;
		}
	   	return true;
	}
	return false;
}

SouthEastExit95000::SouthEastExit95000()
{
	name = "SouthEastExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTHEAST_EXIT_X, GUN_SOUTHEAST_EXIT_Y,100,150);
	arrow = new Arrow(4,488,378,(ExitFeature*)this);
}

int SouthEastExit95000::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95000:
				theGame->newRoom(new Rm95002);
				break;
			case 95001:
				theGame->newRoom(new Rm95004);
				break;
			case 95002:
				theGame->newRoom(new Rm95005);
				break;
			case 95003:
				theGame->newRoom(new Rm95006);
				break;
			case 95004:
				theGame->newRoom(new Rm95007);
				break;
			case 95006:
				theGame->newRoom(new Rm95009);
				break;
			case 95007:
				theGame->newRoom(new Rm95010);
				break;
			case 95008:
				theGame->newRoom(new Rm95011);
				break;
			case 95009:
				theGame->newRoom(new Rm95012);
				break;
			case 95011:
				theGame->newRoom(new Rm95014);
				break;
			case 95012:
				theGame->newRoom(new Rm95015);
				break;
			case 95013:
				theGame->newRoom(new Rm95016);
				break;
			case 95014:
				theGame->newRoom(new Rm95017);
				break;
			case 95016:
				theGame->newRoom(new Rm95018);
				break;
		}
	    return true;
	}
	return false;
}

SouthWestExit95000::SouthWestExit95000()
{
	name = "SouthWestExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTHWEST_EXIT_X, GUN_SOUTHWEST_EXIT_Y,100,150);
	arrow = new Arrow(5,1,376,(ExitFeature*)this);
}

int SouthWestExit95000::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95000:
				theGame->newRoom(new Rm95001);
				break;
			case 95001:
				theGame->newRoom(new Rm95003);
				break;
			case 95002:
				theGame->newRoom(new Rm95004);
				break;
			case 95004:
				theGame->newRoom(new Rm95006);
				break;
			case 95005:
				theGame->newRoom(new Rm95007);
				break;
			case 95006:
				theGame->newRoom(new Rm95008);
				break;
			case 95007:
				theGame->newRoom(new Rm95009);
				break;
			case 95009:
				theGame->newRoom(new Rm95011);
				break;
			case 95010:
				theGame->newRoom(new Rm95012);
				break;
			case 95011:
				theGame->newRoom(new Rm95013);
				break;
			case 95012:
				theGame->newRoom(new Rm95014);
				break;
			case 95014:
				theGame->newRoom(new Rm95016);
				break;
			case 95015:
				theGame->newRoom(new Rm95017);
				break;
			case 95017:
				theGame->newRoom(new Rm95018);
				break;
		}
	    return true;
	}
	return false;
}

SouthExit95000::SouthExit95000()
{
	name = "SouthExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTH_EXIT_X, GUN_SOUTH_EXIT_Y,300,100);
	arrow = new Arrow(3,243,380,(ExitFeature*)this);
	arrow->setPri(0xffff);
}
int SouthExit95000::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95000:
				theGame->newRoom(new Rm95004);
				break;
			case 95001:
				theGame->newRoom(new Rm95006);
				break;
			case 95002:
				theGame->newRoom(new Rm95007);
				break;
			case 95003:
				theGame->newRoom(new Rm95008);
				break;
			case 95004:
				theGame->newRoom(new Rm95009);
				break;
			case 95005:
				theGame->newRoom(new Rm95010);
				break;
			case 95006:
				theGame->newRoom(new Rm95011);
				break;
			case 95007:
				theGame->newRoom(new Rm95012);
				break;
			case 95008:
				theGame->newRoom(new Rm95013);
				break;
			case 95009:
				theGame->newRoom(new Rm95014);
				break;
			case 95010:
				theGame->newRoom(new Rm95015);
				break;
			case 95011:
				theGame->newRoom(new Rm95016);
				break;
			case 95012:
				theGame->newRoom(new Rm95017);
				break;
			case 95014:
				theGame->newRoom(new Rm95018);
				break;
		}
	   	return true;
	}
	return false;
}
void WPScript::changeState ( int newState )
{
	switchTo
		ticks = 2;
	END

	BEG
		client->setScript(NULL);
		endPuzzle();
	END
}
 
} // namespace Spycraft 
