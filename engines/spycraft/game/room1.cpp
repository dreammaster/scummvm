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

#include "spycraft/game/globals.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/room1.h"
#include "spycraft/game/print.h"
#include "spycraft/logic/24000.h"
#include "spycraft/logic/24000.h"
#include "spycraft/logic/48000.h"
#include "spycraft/logic/69000.h"
#include "spycraft/logic/36000.h"
#include "spycraft/logic/50000.h"
#include "spycraft/logic/56000.h"
#include "spycraft/logic/37200.h"
#include "spycraft/logic/1270.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/dialog.h"

namespace Spycraft {

extern WestCursor *westCursor;
extern NorthCursor *northCursor;
extern SouthCursor *southCursor;
extern BlinkCursor *normalCursor;
extern GunCursor *gunCursor;
extern Intrface *intrface;
extern Colby colby;
Print *tPrint;
extern DialTree *dialogTree;

/*********************
*
*		Room Code
*
**********************/

Prop *theProp;

DItem11 *item1;
DItem12 *item2;
DItem13 *item3;
DItem14 *item4;
DItem15 *item5;
DItem16 *item6;
DItem17 *item7;
DItem18 *item8;
DItem19 *item9;
DItem20 *item10;


extern Dialog *theDialog;



/*LogoRoom::LogoRoom()
{
	name = "LogoRoom";
	//show_style = VE_FULL_DISSOLVE;
	show_style = VE_FLIP;
}
void LogoRoom::init()
{
	drawPic ( 998 );

	Room::init();
	//theGame->handsOn();
	setScript (new openingScript);
}

int LogoRoom::handleEvent(MADEEventStamp* event)
{
	/*if (event_type == USER_LEFT_UP){
		if (script)
			script->cue();
		return true;
	}*/
	//	return false;
	//}*/

	/*void openingScript::changeState ( int newState )
	{
		switchTo
			//sndPlaySound("c:\\movies\\299.WAV", SND_ASYNC );

			//seconds = 4;
		END

		BEG
			//sndPlaySound(NULL, NULL);
			theGame->newRoom(new Room1);
		END
	} */


Room1::Room1() {
	name = "Room1";
	//show_style = VE_FULL_DISSOLVE;
	show_style = VE_FLIP;
}

void Room1::init() {
	drawPic(1);

	Room::init();
	theGame->handsOn();



	tPrint = new Print;

	tPrint->x = 180;
	tPrint->y = 90;
	tPrint->size = 28;
	tPrint->fore_color = madeRGB(230, 230, 230);
	tPrint->init("Where to Thorn?");

	theDialog = new Dialog;


	item1 = new DItem11;
	item1->myPrint = new Print;
	//item1->myPrint->size = 24; 
	item1->myPrint->fore_color = madeRGB(63, 125, 185);
	item1->myPrint->high_color = madeRGB(230, 230, 230);
	item1->myPrint->myString = "The Beginning";
	theDialog->add(item1, NULL);

	item2 = new DItem12;
	item2->myPrint = new Print;
	item2->myPrint->fore_color = madeRGB(63, 125, 185);
	item2->myPrint->high_color = madeRGB(230, 230, 230);

	item2->myPrint->myString = "My Office";
	theDialog->add(item2, NULL);

	item3 = new DItem13;
	item3->myPrint = new Print;
	item3->myPrint->fore_color = madeRGB(63, 125, 185);
	item3->myPrint->high_color = madeRGB(230, 230, 230);
	item3->myPrint->myString = "Yuri's Office";
	theDialog->add(item3, NULL);

	item4 = new DItem14;
	item4->myPrint = new Print;
	item4->myPrint->fore_color = madeRGB(63, 125, 185);
	item4->myPrint->high_color = madeRGB(230, 230, 230);
	item4->myPrint->myString = "Hotel Schumpfen";
	theDialog->add(item4, NULL);

	item5 = new DItem15;
	item5->myPrint = new Print;
	item5->myPrint->fore_color = madeRGB(63, 125, 185);
	item5->myPrint->high_color = madeRGB(230, 230, 230);
	item5->myPrint->myString = "Max's Office";
	theDialog->add(item5, NULL);

	item6 = new DItem16;
	item6->myPrint = new Print;
	item6->myPrint->fore_color = madeRGB(63, 125, 185);
	item6->myPrint->high_color = madeRGB(230, 230, 230);
	item6->myPrint->myString = "StakeOut";
	theDialog->add(item6, NULL);

	item7 = new DItem17;
	item7->myPrint = new Print;
	item7->myPrint->fore_color = madeRGB(63, 125, 185);
	item7->myPrint->high_color = madeRGB(230, 230, 230);
	item7->myPrint->myString = "Birdsong's Pad";
	theDialog->add(item7, NULL);

	item8 = new DItem18;
	item8->myPrint = new Print;
	item8->myPrint->high_color = madeRGB(230, 230, 230);
	item8->myPrint->fore_color = madeRGB(63, 125, 185);
	item8->myPrint->myString = "Kneecap's Pad";
	theDialog->add(item8, NULL);

	item9 = new DItem19;
	item9->myPrint = new Print;
	//item1->myPrint->size = 24; 
	item9->myPrint->fore_color = madeRGB(63, 125, 185);
	item9->myPrint->high_color = madeRGB(230, 230, 230);
	item9->myPrint->myString = "Langley";
	theDialog->add(item9, NULL);

	item10 = new DItem20;
	item10->myPrint = new Print;
	//item1->myPrint->size = 24; 
	item10->myPrint->fore_color = madeRGB(63, 125, 185);
	item10->myPrint->high_color = madeRGB(230, 230, 230);
	item10->myPrint->myString = "World Map";
	theDialog->add(item10, NULL);

	theDialog->init(215, 135, NULL);
	dialogTree = new DialTree;
	dialogTree->rootNode = theDialog;
	dialogTree->show(0);

}

void DItem11::doit() //langley
{
	theGame->newRoom(new Rm1270);
}

void DItem12::doit() {
	theGame->newRoom(new Rm24000);
	//sfxPrintf( "d1");
}

void DItem13::doit() {
	theGame->newRoom(new Rm48000);
	//sfxPrintf( "d1");
}

void DItem14::doit() {
	theGame->newRoom(new Rm69000);
	//sfxPrintf( "d1");
}

void DItem15::doit() {
	theGame->newRoom(new Rm36000);
	//	sfxPrintf( "d1");
}

void DItem16::doit() {
	theGame->newRoom(new Rm50000);
	//sfxPrintf( "d1");
}

void DItem17::doit() {
	theGame->newRoom(new Rm56000);
	//sfxPrintf( "d1");
}

void DItem18::doit() {
	theGame->newRoom(new Rm37200);
	//sfxPrintf( "d1");
}

void DItem19::doit() //langley
{
	theGame->newRoom(new Rm24000);
}

void DItem20::doit() //langley
{
	theGame->newRoom(new Rm1000);
}

Room1::~Room1()

{
	tPrint->dispose();
	dialogTree->rootNode->dispose();
	dialogTree->dispose();
}

int Room1::handleEvent(MADEEventStamp *event) {
	return false;
}

} // namespace Spycraft
