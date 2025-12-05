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
#include "spycraft/game/game.h"
#include "spycraft/game/verbs.h"
#include "views.h"
#include "spycraft/game/flag.h"
#include "cursor.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/roomsnd.h"
#include "spycraft/logic/12000.h"
#include "spycraft/logic/10000.h"
#include "spycraft/logic/1000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 10

extern BlinkCursor* normalCursor;

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern SouthCursor* southCursor;
extern short	drawBackground;	// Travelink

Rm12000::Rm12000()
{
	name = "Rm12000";
	show_style = VE_FLIP;
}

void Rm12000::init()
{
  	//drawPic ( 10200 );	// door
    Room::init();

	#if BKSOUND == ON
		sound1->stop();
	#endif

	#if BKMIDI == ON
		theMusic->stop();
	#endif
  	//new SouthExit12000;
	//new PictureOne12000;
	//new PictureTwo12000;
	//new PictureThree12000;
	//new PictureFour12000;
	//new PictureFive12000;
	
  	//intrface->show();
  	//theGame->handsOn();
	GameFlag.set( fColbyMoleBriefing );
	setScript( new Script12000 ); // play the briefing movies
}


SouthExit12000::SouthExit12000()
{
	name = "SouthExit12000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
	//init (46, 460, 495, 20);
}

void SouthExit12000::respond(void)
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit12000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->newRoom(new Rm10200);
    	return true;
	}
	return false;
}


/*******************************
 *
 *	Text Features (not used)
 *
 *******************************/

	PictureOne12000::PictureOne12000()
	{
	 	name = "Picture One";
   		setHotspot ( DO_V );
		init (300, 50, 80, 40);
	}

	void PictureOne12000::respond(void)
	{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);	
	}

	int PictureOne12000::doVerb (int theVerb)
	{
		if ( theVerb == DO_V )
		{ 
			new PictureOneInset12000;	
			return true;
		}
		return false;
	}

	PictureTwo12000::PictureTwo12000()
	{
	 	name = "Picture Two";
   		setHotspot ( DO_V );
		init (300, 100, 80, 40);
	}

	void PictureTwo12000::respond(void)
	{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);	
	}

	int PictureTwo12000::doVerb (int theVerb)
	{
		if ( theVerb == DO_V )
		{ 
			new PictureTwoInset12000;	
			return true;
		}
		return false;
	}

	PictureThree12000::PictureThree12000()
	{
	 	name = "Picture Three";
   		setHotspot ( DO_V );
		init (300, 150, 80, 40);
	}

	void PictureThree12000::respond(void)
	{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);	
	}

	int PictureThree12000::doVerb (int theVerb)
	{
		if ( theVerb == DO_V )
		{ 
			new PictureThreeInset12000;	
			return true;
		}
		return false;
	}

		PictureFour12000::PictureFour12000()
	{
	 	name = "Picture Four";
   		setHotspot ( DO_V );
		init (300, 200, 80, 40);
	}

	void PictureFour12000::respond(void)
	{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);	
	}

	int PictureFour12000::doVerb (int theVerb)
	{
		if ( theVerb == DO_V )
		{ 
			new PictureFourInset12000;	
			return true;
		}
		return false;
	}

	PictureFive12000::PictureFive12000()
	{
	 	name = "Picture Five";
   		setHotspot ( DO_V );
		init (300, 250, 80, 40);
	}

	void PictureFive12000::respond(void)
	{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);	
	}

	int PictureFive12000::doVerb (int theVerb)
	{
		if ( theVerb == DO_V )
		{ 
			new PictureFiveInset12000;	
			return true;
		}
		return false;
	}


/*******************************
 *
 *	Insets (not used)
 *
 *******************************/

PictureOneInset12000::PictureOneInset12000()
{
 	init(903, 300, 50, 50, 50);
	normalCursor->rest();
}

void PictureOneInset12000::respond(void)
{

}

PictureTwoInset12000::PictureTwoInset12000()
{
 	init(903, 300, 100, 50, 50);
	normalCursor->rest();
}

void PictureTwoInset12000::respond(void)
{

}

PictureThreeInset12000::PictureThreeInset12000()
{
 	init(903, 300, 150, 50, 50);
	normalCursor->rest();
}

void PictureThreeInset12000::respond(void)
{

}

PictureFourInset12000::PictureFourInset12000()
{
 	init(903, 300, 200, 50, 50);
	normalCursor->rest();
}

void PictureFourInset12000::respond(void)
{

}

PictureFiveInset12000::PictureFiveInset12000()
{
 	init(903, 300, 250, 50, 50);
	normalCursor->rest();
}

void PictureFiveInset12000::respond(void)
{

}

/*******************************
 *
 *	Scripts
 *
 *******************************/

 void Script12000::changeState ( int newState )
 {
  	switchTo
		ticks = 2;	// Allows prev room view to dispose
	END

	BEG
		//theGame->handsOff(); not needed--never do handsOn().
//		theMovie->fromTo( 0, 1150 );
		theMovie->play( "12100.avi", this, false, /*false*/true, false );
	END

//	BEG
//		ticks = 2;
//	END
//
//	BEG
//		if( GameFlag.test( fMetMaxFoster ) ) {
//			theMovie->fromTo( 1170, 1325 ); 
//			theMovie->play( "12100.avi", this, false, true, false  ); // get back to Moscow	
//		}
//		else {
//			theMovie->caller = NULL;	// necessary?
//			theMovie->stop();
//			intrface->enable();	// movie won't enable when done--closeWhenDone false.
//			cue();
//		}
//		// removed Get back after PEG, 12101.avi (fKnowYingInMoscow false)
//	END

	BEG	
		drawBackground = false; // Travelink
		theGame->newRoom( new Rm1000 );
	END
 }
 
} // namespace Spycraft 
