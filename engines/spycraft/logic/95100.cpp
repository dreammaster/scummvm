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

#include "spycraft/logic/95100.h"
#include "spycraft/game/html.h"

namespace Spycraft {

int targets95100[19][15]={    9, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//0
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
Rm95100::Rm95100()
{
	name = "95100";
	number = 95100;
	show_style = VE_FLIP;
}
void Rm95100::init()
{
	if (!GameFlag.test(fGotBeacon))
	{
		beacon = new Sound;
		beacon->loop = SND_LOOP_INFINITE;
		beacon->number = 95100;
		beacon->play();
	}
	else
		beacon = NULL;

	new ShootBack(95165);
	theGame->handsOn();
	gridnumber = number;

	new(SouthExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);

	g1 = 95065;
	g2 = 95066;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95011;
	f2 = 95012;

	
	gogglesStart(0,2);
}

Rm95100::~Rm95100()
{
	if (beacon)
		beacon->dispose();
}

Rm95101::Rm95101()
{
	name = "95101";
	number = 95101;
	show_style = VE_FLIP;
}
void Rm95101::init()
{
	if (!GameFlag.test(fGotBeacon))
	{
		beacon = new Sound;
		beacon->loop = SND_LOOP_INFINITE;
		beacon->number = 95100;
		beacon->play();
	}
	else
		beacon = NULL;

	new ShootBack(95175);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);

	g1 = 95075;
	g2 = 95076;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95005;
	f2 = 95006;
	
	gogglesStart(1,1);
}
Rm95101::~Rm95101()
{
	if (beacon)
		beacon->dispose();
}

Rm95102::Rm95102()
{
	name = "95102";
	number = 95102;
	show_style = VE_FLIP;
}
void Rm95102::init()
{
	if (!GameFlag.test(fGotBeacon))
	{
		beacon = new Sound;
		beacon->loop = SND_LOOP_INFINITE;
		beacon->number = 95100;
		beacon->play();
	}
	else
		beacon = NULL;

	new ShootBack(95175);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

	g1 = 95075;
	g2 = 95076;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95007;
	f2 = 95008;
	
	gogglesStart(1,3);
}
Rm95102::~Rm95102()
{
	if (beacon)
		beacon->dispose();
}


Rm95103::Rm95103()
{
	name = "95103";
	number = 95103;
	show_style = VE_FLIP;
}
void Rm95103::init()
{
	new ShootBack(95192);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);

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

Rm95104::Rm95104()
{
	name = "95104";
	number = 95104;
	show_style = VE_FLIP;
}
void Rm95104::init()
{
	if (!GameFlag.test(fGotBeacon))
	{
		beacon = new seeBeacon;
	}
	else
		beacon = NULL;

	new ShootBack(95130);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

	g1 = 95032;
	g2 = 95031;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95009;
	f2 = 95010;
	
	gogglesStart(2,2);
}
Rm95104::~Rm95104()
{
	if (beacon)
		beacon->dispose();
}

Rm95105::Rm95105()
{
	name = "95105";
	number = 95105;
	show_style = VE_FLIP;
}
void Rm95105::init()
{
	new ShootBack(95155);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

	g1 = 95055;
	g2 = 95056;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95011;
	f2 = 95012;
	
	gogglesStart(2,4);
}
Rm95106::Rm95106()
{
	name = "95106";
	number = 95106;
	show_style = VE_FLIP;
}
void Rm95106::init()
{
	if (!GameFlag.test(fGotBeacon))
	{
		beacon = new Sound;
		beacon->loop = SND_LOOP_INFINITE;
		beacon->number = 95100;
		beacon->play();
	}
	else
		beacon = NULL;

	new ShootBack(95135);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

	g1 = 95035;
	g2 = 95036;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95005;
	f2 = 95006;
	
	gogglesStart(3,1);
}
Rm95106::~Rm95106()
{
	if (beacon)
		beacon->dispose();
}
Rm95107::Rm95107()
{
	name = "95107";
	number = 95107;
	show_style = VE_FLIP;
}
void Rm95107::init()
{
	if (!GameFlag.test(fGotBeacon))
	{
		beacon = new Sound;
		beacon->loop = SND_LOOP_INFINITE;
		beacon->number = 95100;
		beacon->play();
	}
	else
		beacon = NULL;

	new ShootBack(95170);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

	g1 = 95072;
	g2 = 95071;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95007;
	f2 = 95008;
	
	gogglesStart(3,3);
}
Rm95107::~Rm95107()
{
	if (beacon)
		beacon->dispose();
}
Rm95108::Rm95108()
{
	name = "95108";
	number = 95108;
	show_style = VE_FLIP;
}
void Rm95108::init()
{
//	drawPic (95000);
	new ShootBack(95196);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);

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
Rm95109::Rm95109()
{
	name = "95109";
	number = 95109;
	show_style = VE_FLIP;
}
void Rm95109::init()
{
	if (!GameFlag.test(fGotBeacon))
	{
		beacon = new Sound;
		beacon->loop = SND_LOOP_INFINITE;
		beacon->number = 95100;
		beacon->play();
	}
	else
		beacon = NULL;

	new ShootBack(95165);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

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
Rm95109::~Rm95109()
{
	if (beacon)
		beacon->dispose();
}
Rm95110::Rm95110()
{
	name = "95110";
	number = 95110;
	show_style = VE_FLIP;
}
void Rm95110::init()
{
	new ShootBack(95310);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

	g1 = 95311;
	g2 = 95312;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95011;
	f2 = 95012;
	
	gogglesStart(4,4);
}
Rm95111::Rm95111()
{
	name = "95111";
	number = 95111;
	show_style = VE_FLIP;
}
void Rm95111::init()
{
	new ShootBack(95155);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

	g1 = 95057;
	g2 = 95056;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95005;
	f2 = 95006;
	
	gogglesStart(5,1);
}
Rm95112::Rm95112()
{
	name = "95112";
	number = 95112;
	show_style = VE_FLIP;
}
void Rm95112::init()
{
	new ShootBack(95194);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

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
Rm95113::Rm95113()
{
	name = "95113";
	number = 95113;
	show_style = VE_FLIP;
}
void Rm95113::init()
{
	new ShootBack(95195);
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);

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
Rm95114::Rm95114()
{
	name = "95114";
	number = 95114;
	show_style = VE_FLIP;
}
void Rm95114::init()
{
//	drawPic (95000);
	new ShootBack(95140);
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95100);
	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

	g1 = 95040;
	g2 = 95041;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95007;
	f2 = 95008;
	
	gogglesStart(6,2);
}

Rm95115::Rm95115()
{
	name = "95115";
	number = 95115;
	show_style = VE_FLIP;
}
void Rm95115::init()
{
	new ShootBack(95191);
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

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
Rm95116::Rm95116()
{
	name = "95116";
	number = 95116;
	show_style = VE_FLIP;
}
void Rm95116::init()
{
	new ShootBack(95315);
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthEastExit95100);
	new(NorthWestExit95100);

	g1 = 95316;
	g2 = 95318;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95009;
	f2 = 95010;
	
	gogglesStart(7,1);
}

Rm95117::Rm95117()
{
	name = "95117";
	number = 95117;
	show_style = VE_FLIP;
}
void Rm95117::init()
{
	new ShootBack(95190);
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95100);
	new(NorthEastExit95100);
	new(SouthWestExit95100);
	new(NorthWestExit95100);

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

Rm95118::Rm95118()
{
	name = "95118";
	number = 95118;
	show_style = VE_FLIP;
}
void Rm95118::init()
{
	if (GameFlag.test(fStartCombat))
	{
		drawPic (-1);
		sfxLoadRes(623, RES_ATS);
		sfxLockRes(623, RES_ATS);
		Hp = 5;
	}
	
	new ShootBack(95193);
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95100);
	new(NorthEastExit95100);
	new(NorthWestExit95100);

	if (GameFlag.test(fStartCombat))
	{
		intrface->disable();
		normalCursor = gunCursor;
		base = 95100;

		GameFlag.clear(fGotBeacon);
		GameFlag.clear(fGotKilled);
		memcpy (&T_list,&targets95100,((19*15)*sizeof(int)));

		memcpy (&enePos,&coords,(19*sizeof(MADEPt)));

		memset (&E_map,0,(45*sizeof(int)));
		memset (&P_map,0,(45*sizeof(int)));

		E_map [4][2] = 1;
		E_map [0][2] = 1;


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

		elapsed = new Elapsed;

	}
	else if(GameFlag.test(fGotBeacon))
	{
		setScript(new CrisisScript);
		return;
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
	setScript(new(COScript));
}

/*********************
*
*		ExitFeatures 
*
**********************/
void NorthEastExit95100::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northeastCursor);
}
void NorthWestExit95100::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northwestCursor);
}
void NorthExit95100::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northCursor);
}
void SouthEastExit95100::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southeastCursor);
}
void SouthWestExit95100::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southwestCursor);
}
void SouthExit95100::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southCursor);
}

NorthEastExit95100::NorthEastExit95100()
{
	name = "NorthEastExit";
	setHotspot ( GUN_V );
	init ( GUN_NORTHEAST_EXIT_X, GUN_NORTHEAST_EXIT_Y,100,150);
	arrow = new Arrow(1,487,14,(ExitFeature*)this);
}

int NorthEastExit95100::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();
		switch (gridnumber)
		{
			case 95101:
				theGame->newRoom(new Rm95100);
				break;
			case 95103:
				theGame->newRoom(new Rm95101);
				break;
			case 95104:
				if (((Rm95104*)curRoom)->beacon)
					((Rm95104*)curRoom)->beacon->addToPic();
				theGame->newRoom(new Rm95102);
				break;
			case 95106:
				theGame->newRoom(new Rm95104);
				break;
			case 95107:
				theGame->newRoom(new Rm95105);
				break;
			case 95108:
				theGame->newRoom(new Rm95106);
				break;
			case 95109:
				theGame->newRoom(new Rm95107);
				break;
			case 95111:
				theGame->newRoom(new Rm95109);
				break;
			case 95112:
				theGame->newRoom(new Rm95110);
				break;
			case 95113:
				theGame->newRoom(new Rm95111);
				break;
			case 95114:
				theGame->newRoom(new Rm95112);
				break;
			case 95116:
				theGame->newRoom(new Rm95114);
				break;
			case 95117:
				theGame->newRoom(new Rm95115);
				break;
			case 95118:
				theGame->newRoom(new Rm95117);
				break;
		}
	    return true;
	}
	return false;
}

NorthWestExit95100::NorthWestExit95100()
{
	name = "NorthWestExit";
 	setHotspot ( GUN_V );

	init ( GUN_NORTHWEST_EXIT_X, GUN_NORTHWEST_EXIT_Y,100,150);
	arrow = new Arrow(2,2,12,(ExitFeature*)this);
}

int NorthWestExit95100::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95102:
				theGame->newRoom(new Rm95100);
				break;
			case 95104:
				if (((Rm95104*)curRoom)->beacon)
					((Rm95104*)curRoom)->beacon->addToPic();
				theGame->newRoom(new Rm95101);
				break;
			case 95105:
				theGame->newRoom(new Rm95102);
				break;
			case 95106:
				theGame->newRoom(new Rm95103);
				break;
			case 95107:
				theGame->newRoom(new Rm95104);
				break;
			case 95109:
				theGame->newRoom(new Rm95106);
				break;
			case 95110:
				theGame->newRoom(new Rm95107);
				break;
			case 95111:
				theGame->newRoom(new Rm95108);
				break;
			case 95112:
				theGame->newRoom(new Rm95109);
				break;
			case 95114:
				theGame->newRoom(new Rm95111);
				break;
			case 95115:
				theGame->newRoom(new Rm95112);
				break;
			case 95116:
				theGame->newRoom(new Rm95113);
				break;
			case 95117:
				theGame->newRoom(new Rm95114);
				break;
			case 95118:
				theGame->newRoom(new Rm95116);
				break;
		}
	    return true;
	}
	return false;
}

NorthExit95100::NorthExit95100()
{
	name = "NorthExit";
 	setHotspot ( GUN_V );
	init ( GUN_NORTH_EXIT_X, GUN_NORTH_EXIT_Y,300,100);
	arrow = new Arrow(0,244,10,(ExitFeature*)this);
}

int NorthExit95100::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95104:
				if (((Rm95104*)curRoom)->beacon)
					((Rm95104*)curRoom)->beacon->addToPic();
				theGame->newRoom(new Rm95100);
				break;
			case 95106:
				theGame->newRoom(new Rm95101);
				break;
			case 95107:
				theGame->newRoom(new Rm95102);
				break;
			case 95108:
				theGame->newRoom(new Rm95103);
				break;
			case 95109:
				theGame->newRoom(new Rm95104);
				break;
			case 95110:
				theGame->newRoom(new Rm95105);
				break;
			case 95111:
				theGame->newRoom(new Rm95106);
				break;
			case 95112:
				theGame->newRoom(new Rm95107);
				break;
			case 95113:
				theGame->newRoom(new Rm95108);
				break;
			case 95114:
				theGame->newRoom(new Rm95109);
				break;
			case 95115:
				theGame->newRoom(new Rm95110);
				break;
			case 95116:
				theGame->newRoom(new Rm95111);
				break;
			case 95117:
				theGame->newRoom(new Rm95112);
				break;
			case 95118:
				theGame->newRoom(new Rm95114);
				break;
		}
	   	return true;
	}
	return false;
}

SouthEastExit95100::SouthEastExit95100()
{
	name = "SouthEastExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTHEAST_EXIT_X, GUN_SOUTHEAST_EXIT_Y,100,150);
	arrow = new Arrow(4,488,378,(ExitFeature*)this);
}

int SouthEastExit95100::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95100:
				theGame->newRoom(new Rm95102);
				break;
			case 95101:
				theGame->newRoom(new Rm95104);
				break;
			case 95102:
				theGame->newRoom(new Rm95105);
				break;
			case 95103:
				theGame->newRoom(new Rm95106);
				break;
			case 95104:
				if (((Rm95104*)curRoom)->beacon)
					((Rm95104*)curRoom)->beacon->addToPic();
				theGame->newRoom(new Rm95107);
				break;
			case 95106:
				theGame->newRoom(new Rm95109);
				break;
			case 95107:
				theGame->newRoom(new Rm95110);
				break;
			case 95108:
				theGame->newRoom(new Rm95111);
				break;
			case 95109:
				theGame->newRoom(new Rm95112);
				break;
			case 95111:
				theGame->newRoom(new Rm95114);
				break;
			case 95112:
				theGame->newRoom(new Rm95115);
				break;
			case 95113:
				theGame->newRoom(new Rm95116);
				break;
			case 95114:
				theGame->newRoom(new Rm95117);
				break;
			case 95116:
				theGame->newRoom(new Rm95118);
				break;
		}
	    return true;
	}
	return false;
}

SouthWestExit95100::SouthWestExit95100()
{
	name = "SouthWestExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTHWEST_EXIT_X, GUN_SOUTHWEST_EXIT_Y,90,150);
	arrow = new Arrow(5,1,376,(ExitFeature*)this);
}

int SouthWestExit95100::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95100:
				theGame->newRoom(new Rm95101);
				break;
			case 95101:
				theGame->newRoom(new Rm95103);
				break;
			case 95102:
				theGame->newRoom(new Rm95104);
				break;
			case 95104:
				if (((Rm95104*)curRoom)->beacon)
					((Rm95104*)curRoom)->beacon->addToPic();
				theGame->newRoom(new Rm95106);
				break;
			case 95105:
				theGame->newRoom(new Rm95107);
				break;
			case 95106:
				theGame->newRoom(new Rm95108);
				break;
			case 95107:
				theGame->newRoom(new Rm95109);
				break;
			case 95109:
				theGame->newRoom(new Rm95111);
				break;
			case 95110:
				theGame->newRoom(new Rm95112);
				break;
			case 95111:
				theGame->newRoom(new Rm95113);
				break;
			case 95112:
				theGame->newRoom(new Rm95114);
				break;
			case 95114:
				theGame->newRoom(new Rm95116);
				break;
			case 95115:
				theGame->newRoom(new Rm95117);
				break;
			case 95117:
				theGame->newRoom(new Rm95118);
				break;
		}
	    return true;
	}
	return false;
}

SouthExit95100::SouthExit95100()
{
	name = "SouthExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTH_EXIT_X + 45, GUN_SOUTH_EXIT_Y,255,100);
	arrow = new Arrow(3,243,380,(ExitFeature*)this);
}

int SouthExit95100::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95100:
				theGame->newRoom(new Rm95104);
				break;
			case 95101:
				theGame->newRoom(new Rm95106);
				break;
			case 95102:
				theGame->newRoom(new Rm95107);
				break;
			case 95103:
				theGame->newRoom(new Rm95108);
				break;
			case 95104:
				if (((Rm95104*)curRoom)->beacon)
					((Rm95104*)curRoom)->beacon->addToPic();
				theGame->newRoom(new Rm95109);
				break;
			case 95105:
				theGame->newRoom(new Rm95110);
				break;
			case 95106:
				theGame->newRoom(new Rm95111);
				break;
			case 95107:
				theGame->newRoom(new Rm95112);
				break;
			case 95108:
				theGame->newRoom(new Rm95113);
				break;
			case 95109:
				theGame->newRoom(new Rm95114);
				break;
			case 95110:
				theGame->newRoom(new Rm95115);
				break;
			case 95111:
				theGame->newRoom(new Rm95116);
				break;
			case 95112:
				theGame->newRoom(new Rm95117);
				break;
			case 95114:
				theGame->newRoom(new Rm95118);
				break;
		}
	   	return true;
	}
	return false;
}

void CrisisScript::changeState ( int newState )
{
	switchTo
		ticks = 1;
	END

	BEG
		client->setScript(NULL);
		endPuzzle();
	END
}

void BleepScript::changeState ( int newState )
{
	switchTo
		((seeBeacon*)client)->beacon->play(95100,this);
		((seeBeacon*)client)->setCel(0);
	END

	BEG
		((seeBeacon*)client)->setCel(1);
		seconds = 1;
	END

	BEG
		((seeBeacon*)client)->setScript(new BleepScript);
	END
}

void seeBeacon::respond(void)
{
//	if (!colby.gunning)
//	{
		theGame->setCursor(hyperCursor);
//	}
}

int seeBeacon::doVerb ( int theVerb )
{
//	if (!colby.gunning) 
//	{
		dispose();
		((Rm95100*)curRoom)->beacon = NULL;
		GameFlag.set(fGotBeacon);
		return true;
//	}
//	return false;
}
seeBeacon::seeBeacon()
{
	view = 95199;
	loop = 0;
	cel = 0;
	init(100,338);
	setPri(98);
	setHotspot(GUN_V);
	beacon = new Sound;
	beacon->number = 95100;
	setScript(new (BleepScript));
}
void seeBeacon::dispose(void)
{
	setScript(NULL);
	if (beacon)
	{
		beacon->caller = NULL;
		beacon->dispose();
		beacon = NULL;
	}
	View::dispose();
}
 
} // namespace Spycraft 
