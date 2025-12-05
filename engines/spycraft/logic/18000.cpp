//Started by Tom DeSalvo
//Continued by Daniel Kinzek
//Langley Management Hub/DCI's office
//Contains the remnants of what were formerly 18000 and 19000 (20000, ...)

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "flag.h"
#include "movie.h"
#include "timer.h"
#include "vlink.h"
#include "18000.h"				   
#include "1000.h"


/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;

extern SouthCursor* southCursor;
extern Movie* theMovie;

extern Timer*	hurryTimer;

Rm18000::Rm18000()
{
	name = "Rm18000";
	show_style = VE_FLIP;
}

void Rm18000::init()
{
	//int choice;

  	//drawPic( 1 );
    Room::init();

	/* force DCI movies, progress game */
	if( GameFlag.test( fForceDCI2 ) )
	{
		GameFlag.set( fFarmDone );
		GameFlag.clear( fFullBriefing );
		//GameFlag.set(  );
		//GameFlag.set(  );
	 	GameFlag.clear( fForceDCI2 );
	}
	if( GameFlag.test( fForceDCI3 ) )
	{
		GameFlag.set( fSolvedDubanskyKAT );
		GameFlag.set( fIDPEG );
		GameFlag.clear( fPEGBriefing );
		//
		GameFlag.set( fFarmDone );
		GameFlag.set( fFullBriefing );
		GameFlag.set( fMoscowAvailable );
		GameFlag.set( fPlayerOfficeStarted );
		//GameFlag.set(  );
	 	GameFlag.clear( fForceDCI3 );
	}

	GameFlag.clear( fDCIOfficeAvailable );	// clear DCI's office available

	// briefing after Jeffries is killed
	if( GameFlag.test( fFarmDone )  && !GameFlag.test( fFullBriefing ) )
	{
		comlinks->removeVoiceMail( 981 );	// get rid of hurry up if it's on the list
		setScript( new FullBriefScript18000 );
	}
	// PEG briefing
	else if( GameFlag.test( fSolvedDubanskyKAT )  && GameFlag.test( fIDPEG )  &&
	        !GameFlag.test( fPEGBriefing ) )
	{
		if( hurryTimer != NULL)
		{
			hurryTimer->cue();
			hurryTimer = NULL;
		}
		comlinks->removeVLink( "557.avi" );	// get rid of hurry up if it's on the list 
		setScript( new PEGBriefScript18000 );
	}
	// end of game ->final end tag	
	else if( GameFlag.test( fEndgame ) )
	{
		if( !GameFlag.test( fEndgameYuriShot ) )
			setScript( new BadScript18000 );
		else
			setScript( new MedalScript18000 );
	}
	else
	{
		#ifdef _DEBUG
			sfxPrintf( "Can't go there now." );
			sfxPrintf(
				   "fFarmDone, %d: %s\n"
		           "fFullBriefing, %d: %s\n"
				   "fSolvedDubanskyKAT, %d: %s\n"
				   "fIDPEG, %d: %s\n"
				   "fPEGBriefing, %d: %s\n"
				   "To get DCI2 set flag %d.\n"
				   "To get DCI3 set flag %d.\n"
				   "DCI office available %d.\n" ,
				   fFarmDone, GameFlag.test( fFarmDone )?"TRUE":"FALSE",
				   fFullBriefing, GameFlag.test( fFullBriefing )?"TRUE":"FALSE",
				   fSolvedDubanskyKAT, GameFlag.test( fSolvedDubanskyKAT )?"TRUE":"FALSE",
				   fIDPEG, GameFlag.test( fIDPEG )?"TRUE":"FALSE",
				   fPEGBriefing, GameFlag.test( fPEGBriefing )?"TRUE":"FALSE",
				   fForceDCI2, fForceDCI3, fDCIOfficeAvailable
				 );
		#endif
		setScript( new GoAwayScript18000 );
	}
}

/*******************************
/*
/*	Scripts
/*
/*******************************/

void FullBriefScript18000::changeState( int newState )
{
  	switchTo
		theMovie->play("18000.avi", this, FALSE, TRUE, FALSE );	// fromerly 94.avi
	END

	BEG
		GameFlag.set( fFullBriefing );
		GameFlag.set( fMoscowAvailable );
		GameFlag.set( fPlayerInCharge );
		//curMap = MAP_WORLD;	// global in 1000.cpp
		theGame->newRoom( new Rm1000 );
	END
}

void PEGBriefScript18000::changeState( int newState )
{
	switchTo
		// following movie formerly 123.avi
		if( GameFlag.test( fMetBirdSong ) ) //Find PEG shooter, don't forget about BS.
		{
			theMovie->fromTo( 0, 665 );	
			theMovie->play( "18001.avi", this, FALSE, FALSE, FALSE ); 
		}
		else //Find PEG shooter, no Birdsong tag
		{
			//theMovie->fromTo( 0, 720 );
			theMovie->fromTo( 0, 811 );
			theMovie->play( "18001.avi", this, FALSE, TRUE, FALSE ); 
		}
	END

	BEG
		if( GameFlag.test( fMetBirdSong ) ) // cut to TL, skips BS tag
		{
		 	theMovie->fromTo( 825, 919 );	
			theMovie->play( "18001.avi", this, FALSE, TRUE, FALSE );	
		}
		else
			cue();
	END

	BEG
		GameFlag.set( fPEGBriefing );
		theGame->newRoom( new Rm1000 );
	END
}

/*************  the following not used ****************/

	// plays in Red Square when pres gets killed
	void FiredScript18000::changeState( int newState )
	{
		switchTo
			theMovie->play("18010.avi", this); // You're fired!  formerly 471.avi
		END

		BEG
			sfxPrintf( "Go to end sequence." );
			dispose();
		END
	}

	void BadScript18000::changeState( int newState )
	{
		switchTo
			theMovie->play("18011.avi", this); // Bad job!  formerly 469.avi
		END

		BEG
			sfxPrintf( "Go to end sequence." );
			dispose();
		END
	}

	// not used
	void GoodScript18000::changeState( int newState )
	{
		switchTo
			theMovie->play("18012.avi", this); // Good job!  formerly 462.avi
		END

		BEG
			sfxPrintf( "Go to end sequence." );
			dispose();
		END
	}

	void MedalScript18000::changeState( int newState )
	{
		switchTo
			theMovie->play("18013.avi", this); // Good job! Have a medal. formerly 465.avi
		END

		BEG
			sfxPrintf( "Go to end sequence." );
			dispose();
		END
	}
/**********************  end not used  *******************************/

// Exit from this room back to map
void GoAwayScript18000::changeState( int newState )
{
  	switchTo
		ticks = 1;
	END

	BEG
		theGame->newRoom( new Rm1000 );
	END
}
