/*********************************
*                          
*       Colby- Tunisia Shooter        
* Copyright Activision 1995 
*  Started by:  Pat Griffith 
*                           
**********************************/
#include "95300.h"

int targets95300[19][15]={    6,  4,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//0
		  					  6,  4,  7, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//1	
							  6,  4,  7, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//2
							  6,  4, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//3
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//4
							  4,  7, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//5
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//6
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//7
							  4,  6, 11, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//8
							  4,  6,  7, 11, 12, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//9
							  4,  7, 12, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//10
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//11
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//12
							  4,  6, 11, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//13
							  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//14
							  4,  7, 12, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//15
							  6, 11, 12, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//16
							  7, 11, 12, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//17
							 11, 12, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};	//18
/*********************
*
*		Room Code 
*
**********************/
Rm95300::Rm95300()
{
	name = "95300";
	number = 95300;
	show_style = VE_FLIP;
}

void Rm95300::init()
{
	new ShootBack(95320);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);

	g1 = 95322;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95005;
	f2 = 95006;

	
	gogglesStart(1,1);
}

Rm95301::Rm95301()
{
	name = "95301";
	number = 95301;
	show_style = VE_FLIP;
}

void Rm95301::init()
{
	new ShootBack(95165);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);

	g1 = 95065;
	g2 = 95066;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95007;
	f2 = 95008;
	
	gogglesStart(1,1);
}

Rm95302::Rm95302()
{
	name = "95302";
	number = 95302;
	show_style = VE_FLIP;
}

void Rm95302::init()
{
	new ShootBack(95170);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

	g1 = 95071;
	g2 = 95072;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95009;
	f2 = 95010;
	
	gogglesStart(1,3);
}

Rm95303::Rm95303()
{
	name = "95303";
	number = 95303;
	show_style = VE_FLIP;
}

void Rm95303::init()
{
	new ShootBack(95315);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthEastExit95300);
//	new(NorthWestExit95300);
//	new(NorthExit95300);
	new(SouthEastExit95300);

	g1 = 95317;
	g2 = 95316;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95011;
	f2 = 95012;
	
	gogglesStart(2,0);
}

Rm95304::Rm95304()
{
	name = "95304";
	number = 95304;
	show_style = VE_FLIP;
}

void Rm95304::init()
{
	new ShootBack(95393);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	
	gogglesStart(2,2);
}

Rm95305::Rm95305()
{
	name = "95305";
	number = 95305;
	show_style = VE_FLIP;
}

void Rm95305::init()
{
	new ShootBack(95315);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);
//	new(NorthEastExit95300);
//	new(NorthExit95300);

	g1 = 95316;
	g2 = 95317;
//	g1 = 95318;
//	g2 = 95319;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95005;
	f2 = 95006;
	
	gogglesStart(2,4);
}
Rm95306::Rm95306()
{
	name = "95306";
	number = 95306;
	show_style = VE_FLIP;
}

void Rm95306::init()
{
	new ShootBack(95394);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;
	
	gogglesStart(3,1);
}
Rm95307::Rm95307()
{
	name = "95307";
	number = 95307;
	show_style = VE_FLIP;
}

void Rm95307::init()
{
	new ShootBack(95391);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

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
Rm95308::Rm95308()
{
	name = "95308";
	number = 95308;
	show_style = VE_FLIP;
}

void Rm95308::init()
{
	new ShootBack(95175);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);

	g1 = 95075;
	g2 = 95076;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95007;
	f2 = 95008;
	
	gogglesStart(4,0);
}
Rm95309::Rm95309()
{
	name = "95309";
	number = 95309;
	show_style = VE_FLIP;
}

void Rm95309::init()
{
	if (GameFlag.test(fStartCombat))
	{
		drawPic (-1);
		sfxLoadRes(623, RES_ATS);
		sfxLockRes(623, RES_ATS);
		Hp = 5;
	}

	new ShootBack(95170);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

	g1 = 0;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 0;
	h2 = 0;
	f1 = 0;
	f2 = 0;

	if (GameFlag.test(fStartCombat))
	{
		GameFlag.clear(fGotKilled);
		GameFlag.clear(fGotBeacon);
		intrface->disable();
		normalCursor = gunCursor;

		base = 95300;
		memcpy (&T_list,&targets95300,((19*15)*sizeof(int)));

		memcpy (&enePos,&coords,(19*sizeof(MADEPt)));

		memset (&E_map,0,(45*sizeof(int)));
		memset (&P_map,0,(45*sizeof(int)));

		addshooters = FALSE;
//		addshooters = TRUE;
/*
		E_map [2][0] = 2;
		E_map [2][4] = 2;
		E_map [6][4] = 2;

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
		P_map [8][2] = 3;
*/
		E_map [2][2] = NOGO;
		E_map [3][1] = NOGO;
		E_map [3][3] = NOGO;
		E_map [4][2] = NOGO;
		E_map [5][1] = NOGO;
		E_map [5][3] = NOGO;
		E_map [6][2] = NOGO;
	}
	
	gogglesStart(4,2);
	setScript(new TunisiaScript);
}
Rm95310::Rm95310()
{
	name = "95310";
	number = 95310;
	show_style = VE_FLIP;
}

void Rm95310::init()
{
	new ShootBack(95165);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

	g1 = 95065;
	g2 = 95066;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95009;
	f2 = 95010;
	
	gogglesStart(4,4);
}
Rm95311::Rm95311()
{
	name = "95311";
	number = 95311;
	show_style = VE_FLIP;
}

void Rm95311::init()
{
	new ShootBack(95395);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

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
Rm95312::Rm95312()
{
	name = "95312";
	number = 95312;
	show_style = VE_FLIP;
}

void Rm95312::init()
{
	new ShootBack(95392);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

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
Rm95313::Rm95313()
{
	name = "95313";
	number = 95313;
	show_style = VE_FLIP;
}

void Rm95313::init()
{
	new ShootBack(95310);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

//	new(SouthExit95300);
//	new(SouthWestExit95300);
	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);

	g1 = 95311;
	g2 = 95313;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95011;
	f2 = 95012;
	
	gogglesStart(6,0);
}
Rm95314::Rm95314()
{
	name = "95314";
	number = 95314;
	show_style = VE_FLIP;
}

void Rm95314::init()
{
	new ShootBack(95390);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(SouthExit95300);
	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

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
Rm95315::Rm95315()
{
	name = "95315";
	number = 95315;
	show_style = VE_FLIP;
}

void Rm95315::init()
{
	new ShootBack(95310);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95300);
	new(SouthWestExit95300);
//	new(SouthEastExit95300);
//	new(SouthExit95300);
	new(NorthWestExit95300);

	g1 = 95311;
	g2 = 95313;
	g3 = 0;
	g4 = 0;

	h1 = 95019;
	h2 = 95020;
	f1 = 95005;
	f2 = 95006;
	
	gogglesStart(6,4);
}
Rm95316::Rm95316()
{
	name = "95316";
	number = 95316;
	show_style = VE_FLIP;
}

void Rm95316::init()
{
	new ShootBack(95170);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthEastExit95300);
	new(NorthWestExit95300);

	g1 = 95073;
	g2 = 95071;
	g3 = 0;
	g4 = 0;

	h1 = 95021;
	h2 = 95022;
	f1 = 95007;
	f2 = 95008;
	
	gogglesStart(7,1);
}
Rm95317::Rm95317()
{
	name = "95317";
	number = 95317;
	show_style = VE_FLIP;
}

void Rm95317::init()
{
	new ShootBack(95175);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95300);
	new(NorthEastExit95300);
	new(SouthWestExit95300);
	new(NorthWestExit95300);

	g1 = 95075;
	g2 = 95076;
	g3 = 0;
	g4 = 0;

	h1 = 95023;
	h2 = 95024;
	f1 = 95009;
	f2 = 95010;
	
	gogglesStart(7,3);
}
Rm95318::Rm95318()
{
	name = "95318";
	number = 95318;
	show_style = VE_FLIP;
}

void Rm95318::init()
{
	new ShootBack(95320);
	Room::init();
	theGame->handsOn();

	gridnumber = number;

	new(NorthExit95300);
//	new(SouthExit95300);
	new(NorthEastExit95300);
	new(NorthWestExit95300);

	g1 = 95322;
	g2 = 0;
	g3 = 0;
	g4 = 0;

	h1 = 95025;
	h2 = 95026;
	f1 = 95011;
	f2 = 95012;
	
	gogglesStart(8,2);
}

/*********************
*
*		ExitFeatures 
*
**********************/
void NorthEastExit95300::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northeastCursor);
}
void NorthWestExit95300::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northwestCursor);
}
void NorthExit95300::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(northCursor);
}
void SouthEastExit95300::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southeastCursor);
}
void SouthWestExit95300::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southwestCursor);
}
void SouthExit95300::respond(void)
{
	if (!colby.gunning)
		theGame->setCursor(southCursor);
}

NorthEastExit95300::NorthEastExit95300()
{
	name = "NorthEastExit";
	setHotspot ( GUN_V );
	init ( GUN_NORTHEAST_EXIT_X, GUN_NORTHEAST_EXIT_Y,100,150);
	arrow = new Arrow(1,487,14,(ExitFeature*)this);
}

int NorthEastExit95300::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95301:
				theGame->newRoom(new Rm95300);
				break;
			case 95303:
				theGame->newRoom(new Rm95301);
				break;
			case 95304:
				theGame->newRoom(new Rm95302);
				break;
/*			case 95305:
				endPuzzle();
				break;
*/			case 95306:
				theGame->newRoom(new Rm95304);
				break;
			case 95307:
				theGame->newRoom(new Rm95305);
				break;
			case 95308:
				theGame->newRoom(new Rm95306);
				break;
			case 95309:
				theGame->newRoom(new Rm95307);
				break;
			case 95311:
				if (GameFlag.test(fGotBeacon))
					endPuzzle();				
				else
					theGame->newRoom(new Rm95309);
				break;
			case 95312:
				theGame->newRoom(new Rm95310);
				break;
			case 95313:
				theGame->newRoom(new Rm95311);
				break;
			case 95314:
				theGame->newRoom(new Rm95312);
				break;
			case 95316:
				theGame->newRoom(new Rm95314);
				break;
			case 95317:
				theGame->newRoom(new Rm95315);
				break;
			case 95318:
				theGame->newRoom(new Rm95317);
				break;
		}
	    return TRUE;
	}
	return FALSE;
}

NorthWestExit95300::NorthWestExit95300()
{
	name = "NorthWestExit";
 	setHotspot ( GUN_V );

	init ( GUN_NORTHWEST_EXIT_X, GUN_NORTHWEST_EXIT_Y,100,150);
	arrow = new Arrow(2,2,12,(ExitFeature*)this);
}

int NorthWestExit95300::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95302:
				theGame->newRoom(new Rm95300);
				break;
/*			case 95303:
				endPuzzle();
				break;
*/			case 95304:
				theGame->newRoom(new Rm95301);
				break;
			case 95305:
				theGame->newRoom(new Rm95302);
				break;
			case 95306:
				theGame->newRoom(new Rm95303);
				break;
			case 95307:
				theGame->newRoom(new Rm95304);
				break;
			case 95309:
				theGame->newRoom(new Rm95306);
				break;
			case 95310:
				theGame->newRoom(new Rm95307);
				break;
			case 95311:
				theGame->newRoom(new Rm95308);
				break;
			case 95312:
				if (GameFlag.test(fGotBeacon))
					endPuzzle();				
				else
					theGame->newRoom(new Rm95309);
				break;
			case 95314:
				theGame->newRoom(new Rm95311);
				break;
			case 95315:
				theGame->newRoom(new Rm95312);
				break;
			case 95316:
				theGame->newRoom(new Rm95313);
				break;
			case 95317:
				theGame->newRoom(new Rm95314);
				break;
			case 95318:
				theGame->newRoom(new Rm95316);
				break;
		}
	    return TRUE;
	}
	return FALSE;
}

NorthExit95300::NorthExit95300()
{
	name = "NorthExit";
 	setHotspot ( GUN_V );
	init ( GUN_NORTH_EXIT_X, GUN_NORTH_EXIT_Y,300,100);
	arrow = new Arrow(0,244,10,(ExitFeature*)this);
}

int NorthExit95300::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
/*			case 95303:
				endPuzzle();
				break;
*/			case 95304:
				theGame->newRoom(new Rm95300);
				break;
/*			case 95305:
				endPuzzle();
				break;
*/			case 95306:
				theGame->newRoom(new Rm95301);
				break;
			case 95307:
				theGame->newRoom(new Rm95302);
				break;
			case 95308:
				theGame->newRoom(new Rm95303);
				break;
			case 95309:
				theGame->newRoom(new Rm95304);
				break;
			case 95310:
				theGame->newRoom(new Rm95305);
				break;
			case 95311:
				theGame->newRoom(new Rm95306);
				break;
			case 95312:
				theGame->newRoom(new Rm95307);
				break;
			case 95313:
				theGame->newRoom(new Rm95308);
				break;
			case 95314:
				if (GameFlag.test(fGotBeacon))
					endPuzzle();				
				else
					theGame->newRoom(new Rm95309);
				break;
			case 95315:
				theGame->newRoom(new Rm95310);
				break;
			case 95316:
				theGame->newRoom(new Rm95311);
				break;
			case 95317:
				theGame->newRoom(new Rm95312);
				break;
			case 95318:
				theGame->newRoom(new Rm95314);
				break;
		}
	   	return TRUE;
	}
	return FALSE;
}

SouthEastExit95300::SouthEastExit95300()
{
	name = "SouthEastExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTHEAST_EXIT_X, GUN_SOUTHEAST_EXIT_Y,100,150);
	arrow = new Arrow(4,488,378,(ExitFeature*)this);
}

int SouthEastExit95300::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95300:
				theGame->newRoom(new Rm95302);
				break;
			case 95301:
				theGame->newRoom(new Rm95304);
				break;
			case 95302:
				theGame->newRoom(new Rm95305);
				break;
			case 95303:
				theGame->newRoom(new Rm95306);
				break;
			case 95304:
				theGame->newRoom(new Rm95307);
				break;
			case 95306:
				if (GameFlag.test(fGotBeacon))
					endPuzzle();				
				else
					theGame->newRoom(new Rm95309);
				break;
			case 95307:
				theGame->newRoom(new Rm95310);
				break;
			case 95308:
				theGame->newRoom(new Rm95311);
				break;
			case 95309:
				theGame->newRoom(new Rm95312);
				break;
			case 95311:
				theGame->newRoom(new Rm95314);
				break;
			case 95312:
				theGame->newRoom(new Rm95315);
				break;
			case 95313:
				theGame->newRoom(new Rm95316);
				break;
			case 95314:
				theGame->newRoom(new Rm95317);
				break;
/*			case 95315:
				endPuzzle();
				break;
*/			case 95316:
				theGame->newRoom(new Rm95318);
				break;
		}
	    return TRUE;
	}
	return FALSE;
}

SouthWestExit95300::SouthWestExit95300()
{
	name = "SouthWestExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTHWEST_EXIT_X, GUN_SOUTHWEST_EXIT_Y,100,150);
	arrow = new Arrow(5,1,376,(ExitFeature*)this);
}

int SouthWestExit95300::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95300:
				theGame->newRoom(new Rm95301);
				break;
			case 95301:
				theGame->newRoom(new Rm95303);
				break;
			case 95302:
				theGame->newRoom(new Rm95304);
				break;
			case 95304:
				theGame->newRoom(new Rm95306);
				break;
			case 95305:
				theGame->newRoom(new Rm95307);
				break;
			case 95306:
				theGame->newRoom(new Rm95308);
				break;
			case 95307:
				if (GameFlag.test(fGotBeacon))
					endPuzzle();				
				else
					theGame->newRoom(new Rm95309);
				break;
			case 95309:
				theGame->newRoom(new Rm95311);
				break;
			case 95310:
				theGame->newRoom(new Rm95312);
				break;
			case 95311:
				theGame->newRoom(new Rm95313);
				break;
			case 95312:
				theGame->newRoom(new Rm95314);
				break;
/*			case 95313:
				endPuzzle();
				break;
*/			case 95314:
				theGame->newRoom(new Rm95316);
				break;
			case 95315:
				theGame->newRoom(new Rm95317);
				break;
			case 95317:
				theGame->newRoom(new Rm95318);
				break;
		}
	    return TRUE;
	}
	return FALSE;
}

SouthExit95300::SouthExit95300()
{
	name = "SouthExit";
 	setHotspot ( GUN_V );
	init ( GUN_SOUTH_EXIT_X, GUN_SOUTH_EXIT_Y,300,100);
	arrow = new Arrow(3,243,380,(ExitFeature*)this);
	arrow->setPri(0xffff);
}

int SouthExit95300::doVerb ( int theVerb )
{
	if (!colby.gunning) 
	{
		gogglesClean();

		switch (gridnumber)
		{
			case 95300:
				theGame->newRoom(new Rm95304);
				break;
			case 95301:
				theGame->newRoom(new Rm95306);
				break;
			case 95302:
				theGame->newRoom(new Rm95307);
				break;
			case 95303:
				theGame->newRoom(new Rm95308);
				break;
			case 95304:
				if (GameFlag.test(fGotBeacon))
					endPuzzle();				
				else
					theGame->newRoom(new Rm95309);
				break;
			case 95305:
				theGame->newRoom(new Rm95310);
				break;
			case 95306:
				theGame->newRoom(new Rm95311);
				break;
			case 95307:
				theGame->newRoom(new Rm95312);
				break;
			case 95308:
				theGame->newRoom(new Rm95313);
				break;
			case 95309:
				theGame->newRoom(new Rm95314);
				break;
			case 95310:
				theGame->newRoom(new Rm95315);
				break;
			case 95311:
				theGame->newRoom(new Rm95316);
				break;
			case 95312:
				theGame->newRoom(new Rm95317);
				break;
/*			case 95313:
				endPuzzle();
				break;
*/			case 95314:
				theGame->newRoom(new Rm95318);
				break;
/*			case 95315:
				endPuzzle();
				break;
			case 95318:
				endPuzzle();
				break;
*/		}
	   	return TRUE;
	}
	return FALSE;
}
