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

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "flag.h"
#include "vlink.h"
#include "advmusic.h"
#include "roomsnd.h"
#include "gamebox.h"
#include "winscrn.h"

#include "1000.h"
#include "1070.h"
#include "1075.h"
#include "10000.h"
#include "2000.h"
#include "18000.h"
#include "24000.h"
#include "26000.h"
#include "27000.h"
#include "7000.h"
#include "5000.h"
#include "2000.h"
#include "1090.h"
//#include "1085.h"
#include "37000.h"
#include "56000.h"   
#include "53000.h"   
#include "50000.h"	 
#include "48000.h"
#include "32000.h"
#include "36000.h"
#include "35000.h"
#include "23000.h"
#include "25000.h"
#include "96000.h"
#include "38000.h"
#include "95200.h"
#include "35700.h"
#include "69000.h"
#include "79000.h"

#include "pcscreen.h"

#ifdef DOS4GW
	#include "gamebox.h"
#else
	#include "windows.h"
#endif

namespace Spycraft {

enum	// Washington
{
	WASH_LANG,
	WASH_FARM,
	WASH_LANG_COLBY,
	WASH_MAX,
};

enum	// Langley
{
	LANG_THORN,
	LANG_THORN_DCI2,
	LANG_THORN_DCI3,
	LANG_THORN_HOLT_SEATON,
	LANG_MAX,
};

enum	// Farm
{
	FARM_FRANK,
	FARM_IA,	
	FARM_ZONE,	
	FARM_MAX
};

enum	// Moscow
{
	MOS_STAKE,
	MOS_LUB_BIRD,
	MOS_BIRD,
	MOS_VIL,
	MOS_VIL_BIRD,
	MOS_STATION_ONLY,	
	MOS_KNEE,
	MOS_LUB,
	MOS_MAX,
};

enum	// world --no world level
{
//	WORLD_WASH,
//	WORLD_WASH_DACHA,
//	WORLD_WASH_MOSC,
//	WORLD_WASH_MOSC_LOND,
//	WORLD_WASH_MOSC_HEIDL,
//	WORLD_WASH_MOSC_TUNIS,
	WORLD_MAX
};

enum	// Current and available cities
{
	CITY_NONE,
	CITY_WASH,
	CITY_WASH_DACHA,
	CITY_WASH_MOSC,
	CITY_WASH_MOSC_LOND,
	CITY_WASH_MOSC_HEIDL,
	CITY_WASH_MOSC_TUNIS,
	CITY_MAX
};

enum	// station house Moscow exterior
{
	STAEXT_LOBBY_ALLEY,
	STAEXT_MAX_ALLEY,
	STAEXT_LOBBY_VAN,
	STAEXT_MAX
};

enum	// text titles
{
	/*WORLDLOOP,*/
	CITYLOOP,
	WASHLOOP,
	MOSCOWLOOP,
	HEIDELLOOP,
	LANGLEYLOOP,
	FARMLOOP,
	STATIONLOOP,
	MAXLOOP
};

enum	// text titles, World loop
{
 /*	WASHCELL,
	DACHACELL,
	MOSCOWCELL,
	LONDONCELL,
	HEIDELCELL,
	TUNISIACELL,
	MAXCELL
*/
};

enum 	// text titles, lonley city loop
{
	LONDONCELL,
	TUNISIACELL,
	CRIMEACELL,
	HEIDELHOTELCELL,
	HEIDELREFINERYCELL
};

enum	// text titles, Washington loop
{
	LANGLEYCELL,
	FARMCELL,
	COLBYCELL
};

enum	// text titles, Moscow loop
{
	ASSASSCELL,
	BIRDCELL,
	KNEECELL,
	STAKECELL,
	STATIONCELL,
	VILNIUSCELL,
	YASEVNOCELL
};

enum	// text titles, Heidelberg loop
{
 	DEALCELL,
 	SHOOTCELL
};

enum	// text titles, Langley loop
{
	THORNCELL,
	HOLTCELL,
	JAIMIECELL,
	DCICELL
};

enum	// text titles, Farm loop
{
	IACELL,
	ZONECELL,
	FRANKCELL
};

enum	// text titles, Station House Moscow loop
{
	ALLEYCELL,
	BULLPENCELL,
	FOSTERCELL,
	LOBBYCELL,
	INTERROGCELL,
	PROCVANCELL,
	THORN2CELL
};

// ------- buttons, etc
enum
{
 	CT_TOPBARLOOP,
	CT_TOPTITLELOOP
};

enum
{
 	CT_BOTBARLOOP,
 	CT_BACKLOOP
};

 enum 	// BUTTONS
{
	CT_TUNISIALOOP,
	CT_GERMANYLOOP,
	CT_CRIMEALOOP,
	CT_DACHALOOP,
	CT_MOSCOWLOOP,
	CT_LONDONLOOP,
	CT_WASHINGTONLOOP	
};

enum	// TITLES
{
 	CT_TUNISIACELL,
	CT_HEIDELBERGCELL,
	CT_DACHACELL,
	CT_MOSCOWCELL,
	CT_LONDONCELL,
	CT_WASHINGTONCELL
};

enum	//  L/R SIDES
{
	LEFTBT,
	RIGHTBT
};

enum	// midi info
{
 	M1000_WASH,
	M1000_MOSCOW,
	M1000_STAHSE,
	M1000_HARMONICA,
	M1000_PROCVAN,
	M1000_BIRD,
	M1000_LONDON,
	M1000_HEIDEL,
	M1000_REFINE,
	M1000_ASSASS,
	M1000_DASCHA,
	M1000_MAX
};

typedef struct 
{
	int		used;
	int		number;
} midiInfo;

#define CT_NOCITY -1

#define TRAVELMIDI ON
#define TL_MIDIVOL	50

#define INCLUDE_EUROPE
#define INCLUDE_USA		
#define INCLUDE_LONDON
#define INCLUDE_DACHA

#define USADISC		0
#define EURODISC	1
#define ENDGAMEDISC	2

// Boxes--hot spots
#define LEFT2_PARAMS		32,  148, 160, 150
#define	RIGHT2_PARAMS		320, 148, 160, 150
#define TOP3_PARAMS			176, 38,  160, 150
#define BOT_LEFT3_PARAMS	32,  227, 160, 150
#define BOT_RIGHT3_PARAMS	320, 227, 160, 150
#define TOP_LEFT4_PARAMS	52,  38,  160, 150
#define TOP_RIGHT4_PARAMS	300, 38,  160, 150
#define BOT_LEFT4_PARAMS	32,  227, 160, 150
#define BOT_RIGHT4_PARAMS	320, 227, 160, 150

// locations for titles, (x,y)
#define TX_LEFT2_PARAMS			32,  268
#define	TX_RIGHT2_PARAMS		320, 268
#define TX_TOP3_PARAMS			176, 158
#define TX_BOT_LEFT3_PARAMS		32,  347
#define TX_BOT_RIGHT3_PARAMS	320, 347
#define TX_TOP_LEFT4_PARAMS		52,  158
#define TX_TOP_LEFT4X_PARAMS	32,  158	// movie error compensation
#define TX_TOP_RIGHT4_PARAMS	300, 158
#define TX_BOT_LEFT4_PARAMS		32,  347
#define TX_BOT_RIGHT4_PARAMS	320, 347
#define TX_BOT_RIGHT4X_PARAMS	332, 347	// movie error compensation
#define TX_BUTTON_PARAMS		191, 354


extern Intrface* intrface;
extern short  restoring;
extern int currentFocus;
extern int quitting;
extern int tryingToQuit; 

//We are the world
static WashingtonMap* washington;
static LondonMap* london;
static TunisiaMap* tunisia;
static MoscowMap* moscow;
//static CrimeaMap* crimea;
static HeidelbergMap* heidelberg;	// Hotel or refinery 
//We are Washington
static Farm* theFarm;
static Langley* theLangley;
static ColbyHouse* theColbyHouse;
static MapFrank* theFrank;
static ZoneMap* theZone;
static IAMap* theIA;
static HoltMap* theHolt;
static JaimieMap* theJaimie;
static DCIMap* theDCI;
static WashPlayerMap* theWashPlayer;
//We are Russia
static VilniusCenterMap* theVilniusCenter;
static BirdsongMap* theBirdsong;
static LubyankaMap* theLubyanka;
static EmbassyMap* theEmbassy;
static EmbassyIntMap* theEmbassyInt; 
static AlleyMap*  theAlleyMap;
static ProcatVanMap* theProcatVanMap;
static MaxOfficeMap* theMaxOfficeMap;
static ThornOfficeMap*  theThornOfficeMap;
static InterrogationRoomMap*  theInterrogationMap;
static BullpenMap* theBullpenMap; 
static StakeOutMap* theStakeOut;
static KneecapMap* theKneecap;
static AssSuiteMap* theAssSuite;
//We are Germany
static RefineryShooterMap* theRefineryShooter;
static RefineryDialogMap* theRefineryDialog;
//We are Crimea
static DachaMap* dacha;

short 	curMap = MAP_WASHINGTON;
short	curDisc = USADISC;
short	drawBackground = TRUE;	// flag: do drawPic in enter.
static short inTransit;	// user requested change, not completed yet.	

static Script*	midiFade;

static int availWashRegions;
static int availLangleyRegions;
static int availFarmRegions;
static int availMoscowRegions; 
static int availGermRegions;
static int availStationExtRegions;

static int titleCounter;	// debug
int worldAvail;
int cityAvail;
int goingBackwards;  //set in intrpuzz quit as well as this file
int wentBackwards;	// last time was going backwards

Rm1000* rm1000;


/*********************
*
*		Room Code 
*
**********************/

Rm1000::Rm1000()
{
	name = "Rm1000";
	show_style = VE_FLIP;
	from = 0;
	to = 0;
	loopFrom = 0;
	loopTo = 0;
	seq2From = 0;
	seq2To = 0;
	doSeq2 = FALSE;
	city = NULL;
}

void Rm1000::init()
{
	// temp set all world flags DEBUG
	//GameFlag.set( fWashingtonAvailable );
	//GameFlag.set( fFinlandAvailable );
	//GameFlag.set( fMoscowAvailable );
	//GameFlag.set( fDachaAvailable );
	//GameFlag.set( fCrimeaAvailable );
	//GameFlag.set( fLondonAvailable );
	//GameFlag.set( fHeidelbergAvailable );

 //#pragma message( "TEMP!!!" )
 //GameFlag.set( fKnowYingInMoscow );
 //GameFlag.clear( fSetVL_Max164 );
 //curMap = MAP_LANGLEY;
 //curMap = MAP_MOSCOWPLATE;
 //curDisc = 2;

	sfxEnableSaveGame( TRUE );
	if( drawBackground )
		drawPic(-1);
	drawBackground = TRUE;
	Room::init();

//	ASSERT( titleCounter == 0, 0 );	// debug

	#if BKSOUND == ON
		sound1->stop();
	#endif

	#if BKMIDI == ON  && TRAVELMIDI == ON
		theMusic->stop();
		if( midiFade != NULL ) {
			midiFade->cue();
			midiFade = NULL;
		}
		startMidi();
	#endif

///	user->canInput(TRUE);
	//new CityTriad1000;
	mPlay = new MoviePlayer1000;
	//city = new CityTriad1000;	--delayed to cue()
	theGame->enableIntrDuringMovie = TRUE;

	//ASSERT( currentFocus == 0, 0 );	// if focus not zero timers may not work

	rm1000 = ( Rm1000* )curRoom;
	goingBackwards = TRUE;
	sfxSystemCursor( SYS_WAIT );
	showRegion( curMap );
}

Rm1000::~Rm1000()
{
	sfxEnableSaveGame( FALSE );
	inTransit = FALSE;
	intrface->enable();
	if( !quitting  && !tryingToQuit  && !restoring )
	{
		theGame->enableIntrDuringMovie = FALSE;
		#if BKMIDI == ON  && TRAVELMIDI == ON
			midiFade = new MusicFadeOut1000( musicHandle, 1500 );
		#endif
	}
	else
	{
		#if BKMIDI == ON  && TRAVELMIDI == ON
			stopMidi();	
		#endif
	}
	delete mPlay;
	delete city;
	// handle unusual exit case, eg teleport:
	cleanUp();
	theMovie->caller = NULL;
	theMovie->stop();
}

void Rm1000::fromTo( int theFrom, int theTo)
{
	from = theFrom;
	to = theTo;
	theMovie->fromTo(from, to);
	sfxSystemCursor(SYS_WAIT);
	if( curDisc == USADISC )
		rm1000->mPlay->movieDelayPlay( "1000.avi", this, FALSE, TRUE, FALSE );	
	else if( curDisc == EURODISC )
		rm1000->mPlay->movieDelayPlay( "1001.avi", this, FALSE, TRUE, FALSE );
	else
		rm1000->mPlay->movieDelayPlay( "1002.avi", this, FALSE, TRUE, FALSE );	
}

void Rm1000::seq2(int theFrom, int theTo)
{
	seq2From = theFrom;
	seq2To = theTo;
}

void Rm1000::cue()
{
	if ( doSeq2 )	{
		theMovie->fromTo(seq2From, seq2To);
		doSeq2 = FALSE;
		if( curDisc == USADISC )
			rm1000->mPlay->movieDelayPlay( "1000.avi", this, FALSE, TRUE, FALSE );	
		else if( curDisc == EURODISC )
			rm1000->mPlay->movieDelayPlay( "1001.avi", this, FALSE, TRUE, FALSE );
		else
			rm1000->mPlay->movieDelayPlay( "1002.avi", this, FALSE, TRUE, FALSE );
	}
	else	{
		AddScreen2Pic();
		if( city == NULL )
			city = new CityTriad1000;
		city->update();
		drawTitle( &title1 );
		drawTitle( &title2 );
		drawTitle( &title3 );				
		drawTitle( &title4 );
		inTransit = FALSE;
		user->canInput( TRUE );
		if( curMap == MAP_STATION_EXT )
			checkStationExt();
		else if( curMap == MAP_STATION_INT )		
			checkStationInt();	
		else if( curMap == MAP_MOSCOW )
			checkMoscow();	// extra check for special timing
	}
	//sfxAddMCI2Pic();
	sfxSystemCursor( SYS_ARROW );
}
  
void Rm1000::showRegion( int theRegion )
{
	// Note: transitions within Travelink are in this function
	user->canInput(FALSE);
	curMap = theRegion;
	inTransit = TRUE;
	 	
	switch( theRegion )
	{
	   	case MAP_WORLD:
			/*ASSERT( washington == NULL, 0 );
			ASSERT( moscow == NULL, 0 );
			ASSERT( dacha == NULL, 0 );
			ASSERT( london == NULL, 0 );
			ASSERT( heidelberg == NULL, 0 );
			ASSERT( tunisia == NULL, 0 );*/
		/*	
			queryRegion( MAP_WORLD );
			switch ( worldAvail )	{
				case WORLD_WASH:
					washington = new WashingtonMap( LEFT2_PARAMS );  //MID LEFT 
				// world loop removed
				//	title1.set( 1005, "WashingtonMapTitle", POSN_L2, WORLDLOOP, WASHCELL );
					fromTo( 465, 474 );
					break;
				case WORLD_WASH_MOSC:
					washington = new WashingtonMap( LEFT2_PARAMS );
				// world loop removed
				//	title1.set( 1005, "WashingtonMapTitle", POSN_L2, WORLDLOOP, WASHCELL );  
					moscow = new MoscowMap( RIGHT2_PARAMS );
				// world loop removed
				//	title2.set( 1005, "MoscowMapTitle", POSN_R2, WORLDLOOP, MOSCOWCELL );
					fromTo( 390, 397 );
					break;
				case WORLD_WASH_DACHA:
					washington = new WashingtonMap( LEFT2_PARAMS );
				// world loop removed
				//	title1.set( 1005, "WashingtonMapTitle", POSN_L2, WORLDLOOP, WASHCELL );  
					dacha = new DachaMap( RIGHT2_PARAMS );
				//	title2.set( 1005, "DachaMapTitle", POSN_R2, WORLDLOOP, DACHACELL );
					fromTo( 0, 9 );
					break;
				case WORLD_WASH_MOSC_LOND:
					washington = new WashingtonMap( BOT_LEFT3_PARAMS );
				// world loop removed
				//	title1.set( 1005, "WashingtonMapTitle", POSN_BL3, WORLDLOOP, WASHCELL );  
					moscow = new MoscowMap( BOT_RIGHT3_PARAMS );
				//	title2.set( 1005, "MoscowMapTitle", POSN_BR3, WORLDLOOP, MOSCOWCELL );
					london = new LondonMap( TOP3_PARAMS );
				//	title3.set( 1005, "LondonMapTitle", POSN_T3, WORLDLOOP, LONDONCELL );
					fromTo( 285, 294 );
					break;
				case WORLD_WASH_MOSC_HEIDL:
					washington = new WashingtonMap( BOT_LEFT3_PARAMS );
				// world loop removed
				//	title1.set( 1005, "WashingtonMapTitle", POSN_BL3, WORLDLOOP, WASHCELL );  
					moscow = new MoscowMap( BOT_RIGHT3_PARAMS );
				//	title2.set( 1005, "MoscowMapTitle", POSN_BR3, WORLDLOOP, MOSCOWCELL );
					heidelberg = new HeidelbergMap( TOP3_PARAMS );
				//	title3.set( 1005, "HeidelbergMapTitle", POSN_T3, WORLDLOOP, HEIDELCELL );
					fromTo( 180, 189 );
					break;
				case WORLD_WASH_MOSC_TUNIS:
					washington = new WashingtonMap( BOT_LEFT3_PARAMS );
				// world loop removed
				//	title1.set( 1005, "WashingtonMapTitle", POSN_BL3, WORLDLOOP, WASHCELL );  
					moscow = new MoscowMap( BOT_RIGHT3_PARAMS );
				//	title2.set( 1005, "MoscowMapTitle", POSN_BR3, WORLDLOOP , MOSCOWCELL );
					tunisia = new TunisiaMap( TOP3_PARAMS );
				//	title3.set( 1005, "TunisiaMap", POSN_T3, WORLDLOOP, TUNISIACELL );
					fromTo( 75, 84 );
					break;
				default:
					#ifdef _DEBUG
						sfxPrintf( "Bad worldAvail value %d", worldAvail );
					#endif
					NULL;
			}
		*/
			break;	// end case MAP_WORLD	
					
		case MAP_WASHINGTON:
		/*	ASSERT( theFarm == NULL, 0 );
			ASSERT( theLangley == NULL, 0 );
			ASSERT( theColbyHouse == NULL, 0 );*/
			checkWashington();
			queryRegion( MAP_WASHINGTON );
			if( FALSE ) {
				theMovie->caller = 0;
				theMovie->stop();
				fromTo( 510, 524 );
				//doSeq2 = TRUE;
				//seq2(864, 875);
				theLangley = new Langley( LEFT2_PARAMS );
				title1.set( 1005, "LangleyTitle", POSN_L2, WASHLOOP, LANGLEYCELL );
				//title2.set( 1005, "RemoveTitle", POSN_R2, REMOVELOOP, REMOVECELL );
			}
			else	{
				/* From World */
			/*
				if( !goingBackwards )  {
					switch( worldAvail )
					{
					 case WORLD_WASH:
					 	fromTo( 480, 509 );
					 break;
					 case WORLD_WASH_MOSC:
					 	fromTo( 405, 434 ); 
					 break;
					 case WORLD_WASH_DACHA:
					 	fromTo( 15, 44 );
					 break;
					 case WORLD_WASH_MOSC_LOND:
					 	fromTo( 300, 329 );
					 break;
					 case WORLD_WASH_MOSC_HEIDL:
					 	fromTo( 195, 224 );
					 break;
					 case WORLD_WASH_MOSC_TUNIS:
					 	fromTo( 90, 119 );
					 break;
					 default:
					 	#ifdef _DEBUG
							sfxPrintf( "Unknown world type %d", worldAvail );
						#endif
						NULL;
					}
				}
			*/
				/* Washington fade ups */
				switch( availWashRegions )
				{
				 //case WASH_LANG_FARM:
				 //sfxPrintf( "How'd you get here?" );
				 //	theLangley = new Langley( LEFT2_PARAMS );	//mid left 2
				 //	title1.set( 1005, "LangleyTitle", POSN_L2, WASHLOOP, LANGLEYCELL );
				 //	theFarm = new Farm( RIGHT2_PARAMS );		//mid right 2
				 //	title2.set( 1005, "FarmTitle", POSN_R2, WASHLOOP, FARMCELL );
				 //	
				 //	if (goingBackwards)
				 //		fromTo( 330, 334 );
				 //	else	{	// coming from World
				 //		doSeq2 = TRUE; 
				 //		seq2( 330, 334 ); 	
				 //	}
				 //	break;

				 case WASH_LANG_COLBY:
					theColbyHouse = new ColbyHouse( RIGHT2_PARAMS );  //mid top 3
					title1.set( 1005,  "ColbyHouseTitle", POSN_R2, WASHLOOP, COLBYCELL );
					theLangley = new Langley( LEFT2_PARAMS );   //bot left 3
					title2.set( 1005,  "LangleyTitle", POSN_L2, WASHLOOP, LANGLEYCELL );

					if (goingBackwards)
						fromTo( 210, 224 );
					else	{	// coming from World
						doSeq2 = TRUE; 
						seq2( 210, 224 ); 	
					}
				 	break;

				 case WASH_LANG:
					theLangley = new Langley( LEFT2_PARAMS );	//mid left 2
					title1.set( 1005, "LangleyTitle", POSN_L2, WASHLOOP, LANGLEYCELL );
					//title2.set( 1005, "RemoveFarmTitle", POSN_R2, REMOVELOOP, REMOVECELL );
					
					if (goingBackwards)
						fromTo( 0, 14 );
					else	{	// coming from World
						doSeq2 = TRUE; 
						seq2( 0, 14 ); 	
					}
				 	break;
				 case WASH_FARM:
					theFarm = new Farm( RIGHT2_PARAMS );		//mid right 2
					title1.set( 1005, "FarmTitle", POSN_R2, WASHLOOP, FARMCELL );
					
					if (goingBackwards)
						fromTo( 75, 89 );
					else	{	// coming from World
						doSeq2 = TRUE; 
						seq2( 75, 89 ); 	
					}
				 	break;

				 default:
					#ifdef _DEBUG
						sfxPrintf( "Unknown Wash region %d", availWashRegions );
					#endif
					NULL;
				}
			}
			break;

	   	case MAP_LONDON:
			//ASSERT( london == NULL, 0 );
			// always going backwards to get here
			if( GameFlag.test( fBlakePatioTalkDone ) ) 
					fromTo( 1140, 1154 );	// no option
			else {
				fromTo( 1050, 1064 );
				london = new LondonMap( TOP_LEFT4_PARAMS );
				title1.set( 1005, "LondonTitle", POSN_TL4X, CITYLOOP, LONDONCELL ); 
			}	   		 
			break;

		case MAP_MOSCOW:
		/*	ASSERT( theStakeOut == NULL, 0 ); 			
    	 	ASSERT( theLubyanka == NULL, 0 );
    	 	ASSERT( theEmbassy == NULL, 0 );				
    	 	ASSERT( theBirdsong == NULL, 0 );
			ASSERT( theVilniusCenter == NULL, 0 );*/
			checkMoscow();
			queryRegion( MAP_MOSCOW );
			/* World to generic Moscow sequence */
		/*
			if( !goingBackwards ) {	// coming from World
				switch( worldAvail )
				{
			 		case WORLD_WASH_MOSC:
						fromTo( 435, 464 );	
						break;
					case WORLD_WASH_MOSC_LOND:
						fromTo( 330, 359 );
						break;
					case WORLD_WASH_MOSC_HEIDL:
						fromTo( 225, 254 );
						break;
					case WORLD_WASH_MOSC_TUNIS:
						fromTo( 120, 149 );
						break;
					default:
						#ifdef _DEBUG
							sfxPrintf( "Bad World going to Moscow" );
						#endif
						NULL;
				}
			}
		*/
			/* fade up Moscow sequences */
			switch( availMoscowRegions ) {
			  case MOS_STAKE:
			 //station always avail
			 	theEmbassy = new EmbassyMap( LEFT2_PARAMS );
				title1.set( 1005, "EmbassyMapTitle", POSN_L2, MOSCOWLOOP, STATIONCELL );
	    		theStakeOut = new StakeOutMap( RIGHT2_PARAMS );
	    		title2.set( 1005, "theStakeOutTitle", POSN_R2, MOSCOWLOOP, STAKECELL ); 			
				
				if (goingBackwards)
					fromTo( 330, 344 );
				else	{	// coming from World
					doSeq2 = TRUE; 
					seq2( 330, 344 ); 	
				}
				break;
			 case MOS_VIL:
			 //station always avail
				theEmbassy = new EmbassyMap( LEFT2_PARAMS );
				title1.set( 1005, "EmbassyMapTitle", POSN_L2, MOSCOWLOOP, STATIONCELL );		
				theVilniusCenter = new VilniusCenterMap( RIGHT2_PARAMS );
				title2.set( 1005, "theVilniusCenterTitle", POSN_R2, MOSCOWLOOP, VILNIUSCELL ); 
	
				if( goingBackwards )
					fromTo( 45, 59 );
				else {
					doSeq2 = TRUE;
					seq2( 45, 59 ); 
				}
				break;					  
			  case MOS_VIL_BIRD:
			 //station always avail
	    		theBirdsong = new BirdsongMap( BOT_RIGHT3_PARAMS ); 		//mid top 3;
				title1.set( 1005, "BirdsongMapTitle", POSN_BR3, MOSCOWLOOP, BIRDCELL );
	    		theEmbassy = new EmbassyMap( TOP3_PARAMS );	//bot left 3
	    		title2.set( 1005, "EmbassyMapTitle", POSN_T3, MOSCOWLOOP, STATIONCELL );			
		 		theVilniusCenter = new VilniusCenterMap( BOT_LEFT3_PARAMS );	//bot right 3;
				title3.set( 1005, "theVilniusCenterTitle", POSN_BL3, MOSCOWLOOP, VILNIUSCELL );
	 				
				if (goingBackwards)
					fromTo( 530, 544 );
				else {
					doSeq2 = TRUE;
					seq2( 530, 544 ); 
				}
				break;
			 case MOS_LUB_BIRD:
			 //station always avail
	    		theBirdsong = new BirdsongMap( TOP3_PARAMS ); 		//mid top 3;
				title1.set( 1005, "BirdsongMapTitle", POSN_T3, MOSCOWLOOP, BIRDCELL );
	    		theEmbassy = new EmbassyMap( BOT_LEFT3_PARAMS );	//bot left 3
	    		title2.set( 1005, "EmbassyMapTitle", POSN_BL3, MOSCOWLOOP, STATIONCELL );			
		 		theLubyanka = new LubyankaMap( BOT_RIGHT3_PARAMS );	//bot right 3;
				title3.set( 1005, "LubyankaMapTitle", POSN_BR3, MOSCOWLOOP, YASEVNOCELL );
	 				
				if (goingBackwards)
					fromTo( 210, 224 );
				else {
					doSeq2 = TRUE;
					seq2( 210, 224 ); 
				}
				break;				
			 case MOS_BIRD:
			 //station always avail
				theEmbassy = new EmbassyMap( LEFT2_PARAMS );
				title1.set( 1005, "EmbassyMapTitle", POSN_L2, MOSCOWLOOP, STATIONCELL );		
				theBirdsong = new BirdsongMap( RIGHT2_PARAMS );
				title2.set( 1005, "BirdsongMapTitle", POSN_R2, MOSCOWLOOP, BIRDCELL ); 

				if (goingBackwards)
					fromTo( 120, 134 );
				else  {
					doSeq2 = TRUE;
					seq2( 120, 134 );
				} 
				break;
			 case MOS_KNEE:
			 	theEmbassy = new EmbassyMap( LEFT2_PARAMS );
				title1.set( 1005, "EmbassyMapTitle", POSN_L2, MOSCOWLOOP, STATIONCELL );	
				theKneecap = new KneecapMap( RIGHT2_PARAMS );
				title3.set( 1005, "KneecapsMapTitle", POSN_R2, MOSCOWLOOP, KNEECELL );

				if (goingBackwards)
					fromTo( 375, 389 );
				else  {
					doSeq2 = TRUE;
					seq2( 375, 389 );
				} 
				break;
			 case MOS_LUB:
			 	theEmbassy = new EmbassyMap( LEFT2_PARAMS );
				title1.set( 1005, "EmbassyMapTitle", POSN_L2, MOSCOWLOOP, STATIONCELL );	
				theLubyanka = new LubyankaMap( RIGHT2_PARAMS );
				title2.set( 1005, "LubyankaMapTitle", POSN_R2, MOSCOWLOOP, YASEVNOCELL );

				if (goingBackwards)
					fromTo( 455, 469 );
				else  {
					doSeq2 = TRUE;
					seq2( 455, 469 );
				} 
				break;
			 case MOS_STATION_ONLY:
				theEmbassy = new EmbassyMap( LEFT2_PARAMS );
				title1.set( 1005, "EmbassyMapTitle", POSN_L2, MOSCOWLOOP, STATIONCELL );	
		
				if (goingBackwards)
					fromTo( 0, 14 );
				else {
					doSeq2 = TRUE;
					seq2( 0, 14 );
				}
				break; 
			default:
				#ifdef _DEBUG
					sfxPrintf( "Unknown Moscow setup" );
				#endif
				NULL;
			} // end switch
			break;

		case MAP_MOSCOWPLATE:
			fromTo( 1380, 1390 );
			//fromTo( 1380, 1390 ); // can't do much here.
			setScript( new MoscowPlateScript1000 );		// poll for dacha, rm1000-> script
			break;

		case MAP_TUNISIA:
			//ASSERT( tunisia == NULL, 0 );
			// always going backwards here
			fromTo( 1320, 1334 ); // if you can get here, this is it.
			tunisia = new TunisiaMap( BOT_RIGHT4_PARAMS  );
			title1.set( 1005, "tunisiaMapTitle", POSN_BR4X, CITYLOOP, TUNISIACELL );
			break;

		case MAP_CRIMEA:
		//	ASSERT( dacha == NULL, 0 );
			// always going backwards to get here
			if( GameFlag.test( fEndgame ) )
				fromTo( 1350, 1364 );	// no options,
			else {
				fromTo( 1230, 1244 );
				dacha = new DachaMap( BOT_LEFT4_PARAMS );
				title1.set( 1005, "DachaMapTitle", POSN_BL4, CITYLOOP, CRIMEACELL );
			}
			break;

		case MAP_HEIDELBERG:	// Hotel or refinery
		//	ASSERT( heidelberg == NULL, 0 );
			// top level, always going backwards
			#define GFT GameFlag.test
			if( GFT( fEndgamePitStopped )  || !GFT( fHeidelbergAvailable ) )
				fromTo( 1335, 1349 );	// no option
			else {
				heidelberg = new HeidelbergMap( BOT_RIGHT4_PARAMS );
				if( GameFlag.test( fHKRoomEnter ) )  {  // already been to the hotel
					fromTo( 1125, 1139 );	// refinery 					
					title1.set( 1005, "heidelbergMapTitle",
					            POSN_BR4X, CITYLOOP, HEIDELREFINERYCELL ); 
				}
				else  {
					fromTo( 1185, 1199 );	// hotel
					title1.set( 1005, "heidelbergMapTitle",
					            POSN_BR4X, CITYLOOP, HEIDELHOTELCELL );
				}
			}
			break;

		case MAP_REFINERY:	// choice screen
		//	ASSERT( theRefineryShooter == NULL, 0 );
		//	ASSERT( theRefineryDialog == NULL, 0 );
			if( !goingBackwards ) {
				fromTo( 1140, 1169 );	// segue
				doSeq2 = TRUE;
				seq2( 1170, 1184 );		// intro
			}
			else
				fromTo( 1170, 1184 );
			theRefineryShooter = new RefineryShooterMap( RIGHT2_PARAMS );
			title1.set( 1005, "RefineryMapTitle", POSN_R2, HEIDELLOOP, SHOOTCELL );
			theRefineryDialog = new RefineryDialogMap( LEFT2_PARAMS );
			title2.set( 1005, "RefineryMapTitle", POSN_L2, HEIDELLOOP, DEALCELL  );
			break;

		case MAP_FARM:
			/*ASSERT( theIA == NULL, 0 );
			ASSERT( theZone == NULL, 0 );
			ASSERT( theFrank == NULL, 0 );*/
			queryRegion( MAP_FARM );
			if (!goingBackwards)	{
				if( availWashRegions == WASH_FARM )
					fromTo( 90, 201 );
				else	{
					//ASSERT( FALSE, 0 ); 
					//fromTo( 345, 456 );	// lang, farm, (colby)--no longer used
				}
			}
			switch( availFarmRegions ) {
			 case FARM_FRANK:
				//title1.set( 1005, "RemoveIAMapTitle", POSN_BR3, REMOVELOOP, REMOVECELL );
				//title2.set( 1005, "RemoveZoneMapTitle", POSN_BL3, REMOVELOOP, REMOVECELL );
				theFrank = new MapFrank( TOP3_PARAMS );		//middle top 3
				title3.set( 1005, "MapFrankTitle", POSN_T3, FARMLOOP, FRANKCELL );
				if( goingBackwards )
					fromTo( 765, 779 );
				else {
					doSeq2 = TRUE;
					seq2( 765, 779 );
				}
			 	break;

			 case FARM_IA:
				theIA = new IAMap( BOT_RIGHT3_PARAMS );		//bot left 3
				title1.set( 1005, "IAMapTitle", POSN_BR3, FARMLOOP, IACELL );
				//title2.set( 1005, "RemoveZoneMapTitle", POSN_BL3, REMOVELOOP, REMOVECELL );
				//title3.set( 1005, "RemoveMapFrankTitle", POSN_T3, REMOVELOOP, REMOVECELL );
				if( goingBackwards )
					fromTo( 810, 824 );
				else {
					doSeq2 = TRUE;
					seq2( 810, 824 );
				}
			 	break;

			 case FARM_ZONE:
				//title1.set( 1005, "RemoveIAMapTitle", POSN_BR3, REMOVELOOP, IACELL );
				theZone = new ZoneMap( BOT_LEFT3_PARAMS );	//bot right 3
				title2.set( 1005, "ZoneMapTitle", POSN_BL3, FARMLOOP, ZONECELL );
				//title3.set( 1005, "RemoveMapFrankTitle", POSN_T3, REMOVELOOP, REMOVECELL );
				if( goingBackwards )
					fromTo( 855, 869 );
				else {
					doSeq2 = TRUE;
					seq2( 855, 869 );
				}
			 	break;

			 default:
				#ifdef _DEBUG
					sfxPrintf( "Unknown Farm setup" );
				#endif
				NULL;
			}				
			break;

		case MAP_LANGLEY:
			/*ASSERT( theHolt == NULL, 0 );
			ASSERT( theJaimie == NULL, 0 );
			ASSERT( theWashPlayer == NULL, 0 );
			ASSERT( theDCI == NULL, 0 );*/
			checkLangley();
			queryRegion( MAP_LANGLEY );
			// World segue to Langley
			if( !goingBackwards ) {
				switch( availWashRegions )
				{
				 case WASH_LANG:
					fromTo( 15, 74 );	//Wash w/o Colby ->Langley
				 	break;

				 case WASH_LANG_COLBY:
					fromTo( 270, 329 );	//Wash w/Colby  ->Langley
				 	break;

				 default:
					#ifdef _DEBUG
						sfxPrintf( "Unknown Langley predecessor" );
					#endif
					NULL;
				}
			}
			/* fade up to Langley */
			switch( availLangleyRegions ) {
			 case LANG_THORN:
				//title1.set( 1005, "RemoveHoltMapTitle", POSN_BL3, REMOVELOOP, REMOVECELL );
				//title2.set( 1005, "RemoveJaimieMapTitle", POSN_BR3, REMOVELOOP, REMOVECELL );
				theWashPlayer = new WashPlayerMap( TOP_LEFT4_PARAMS );	//TOP MID 3
				title3.set( 1005, "WashPlayerMapTitle", POSN_TL4, LANGLEYLOOP, THORNCELL );
				// Lang: Jaimie, player's, Holt, -- not DCI
				if (goingBackwards)
					fromTo( 675, 689 );
				else {
					doSeq2 = TRUE;
					seq2( 675, 689 ); 
				}
			 	break;

			 case LANG_THORN_DCI2:
				//title1.set( 1005, "RemoveHoltMapTitle", POSN_BL4, REMOVELOOP, REMOVECELL );
				//title2.set( 1005, "RemoveJaimieMapTitle", POSN_TR4, REMOVELOOP, REMOVECELL );
				theWashPlayer = new WashPlayerMap( TOP_LEFT4_PARAMS );	//TOP LEFT 4
				title3.set( 1005, "WashPlayerMapTitle", POSN_TL4, LANGLEYLOOP, THORNCELL );
				theDCI = new DCIMap( BOT_RIGHT4_PARAMS );				//BOT RIGHT 4
				title4.set( 1005, "DCIMapTitle", POSN_BR4, LANGLEYLOOP, DCICELL );
				// Lang: Jaimie, player's, Holt, and DCI
				if (goingBackwards)
					fromTo( 525, 539 );
				else {
					doSeq2 = TRUE;
					seq2( 525, 539 ); 
				}
			 	break;

			 case LANG_THORN_DCI3:
				//title1.set( 1005, "RemoveHoltMapTitle", POSN_BL4, REMOVELOOP, REMOVECELL );
				//title2.set( 1005, "RemoveJaimieMapTitle", POSN_TR4, REMOVELOOP, REMOVECELL );
				theWashPlayer = new WashPlayerMap( TOP_LEFT4_PARAMS );	//TOP LEFT 4
				title3.set( 1005, "WashPlayerMapTitle", POSN_TL4, LANGLEYLOOP, THORNCELL );
				theDCI = new DCIMap( BOT_RIGHT4_PARAMS );				//BOT RIGHT 4
				title4.set( 1005, "DCIMapTitle", POSN_BR4, LANGLEYLOOP, DCICELL );
				// Lang: Jaimie, player's, Holt, and DCI
				if (goingBackwards)
					fromTo( 720, 734 );
				else {
					doSeq2 = TRUE;
					seq2( 720, 734 ); 
				}
			 	break;

			 case LANG_THORN_HOLT_SEATON:
				theHolt = new HoltMap( BOT_LEFT3_PARAMS );			//BOT LEFT 3
				title1.set( 1005, "HoltMapTitle", POSN_BL3, LANGLEYLOOP, HOLTCELL );
				theJaimie = new JaimieMap( BOT_RIGHT3_PARAMS );		//BOT RIGHT 3
				title2.set( 1005, "JaimieMapTitle", POSN_BR3, LANGLEYLOOP, JAIMIECELL );
				theWashPlayer = new WashPlayerMap( TOP3_PARAMS );	//TOP MID 3
				title3.set( 1005, "WashPlayerMapTitle", POSN_T3, LANGLEYLOOP, THORNCELL );
				// Lang: Jaimie, player's, Holt, -- not DCI
				if (goingBackwards)
					fromTo( 570, 584 );
				else {
					doSeq2 = TRUE;
					seq2( 570, 584 ); 
				}
			 	break;

			 //case LANG_THORN_HOLT_SEATON_DCI:
			 //	theHolt = new HoltMap( BOT_LEFT4_PARAMS );				//BOT LEFT 4
			 //	title1.set( 1005, "HoltMapTitle", POSN_BL4, LANGLEYLOOP, HOLTCELL );
			 //	theJaimie = new JaimieMap( TOP_RIGHT4_PARAMS );			//TOP RIGHT4
			 //	title2.set( 1005, "JaimieMapTitle", POSN_TR4, LANGLEYLOOP, JAIMIECELL );
			 //	theWashPlayer = new WashPlayerMap( TOP_LEFT4_PARAMS );	//TOP LEFT 4
			 //	title3.set( 1005, "WashPlayerMapTitle", POSN_TL4, LANGLEYLOOP, THORNCELL );
			 //	theDCI = new DCIMap( BOT_RIGHT4_PARAMS );				//BOT RIGHT 4
			 //	title4.set( 1005, "DCIMapTitle", POSN_BR4, LANGLEYLOOP, DCICELL );
			 //	// Lang: Jaimie, player's, Holt, and DCI
			 //	if (goingBackwards)
			 //		fromTo( 975, 989 );
			 //	else {
			 //		doSeq2 = TRUE;
			 //		seq2( 975, 989 ); 
			 //	}
			 //	break;
			   
			 default:
				#ifdef _DEBUG
					sfxPrintf( "Unknown Langley %d", availLangleyRegions );
				#endif
				NULL;
			}
			break;

		case MAP_STATION_EXT:
			/*ASSERT( theAlleyMap == NULL, 0 );
			ASSERT( theEmbassyInt == NULL, 0 );
			ASSERT( theMaxOfficeMap == NULL, 0 );*/
			checkStationExt();
			queryRegion( MAP_STATION_EXT );
			if( !goingBackwards ) {
				switch( availMoscowRegions )
				{
				 case MOS_STAKE:
				 	fromTo( 345, 374 );
					break;
				 case MOS_KNEE:
				 	fromTo( 425, 454 );
				 	break;								 
				 case MOS_LUB_BIRD:
					fromTo( 270, 299 );
				 	break;
				 case MOS_BIRD:
					fromTo( 180, 209 );
				 	break;
				 case MOS_LUB:
				 	fromTo( 470, 499 );
					break;
				 case MOS_VIL:
					fromTo( 60, 89 );
				 	break;
				 case MOS_VIL_BIRD:
				 	fromTo( 595, 624 );
					break;
				 case MOS_STATION_ONLY:
					fromTo( 15, 44 );
				 	break;
				 default:
				 	#ifdef _DEBUG
						sfxPrintf( "Unknown previous Moscow state" );
					#endif
					NULL;
				}
			}
			switch( availStationExtRegions ) {
			 case STAEXT_LOBBY_ALLEY:
				if( goingBackwards ) {
					fromTo( 700, 714 );
				}
				else 	{
					doSeq2 = TRUE;
					seq2( 700, 714 );
				}
				theAlleyMap = new AlleyMap( LEFT2_PARAMS );			//mid left 2
				title1.set( 1005, "theAlleyMapTitle", POSN_L2, STATIONLOOP, ALLEYCELL );
				theEmbassyInt = new EmbassyIntMap( RIGHT2_PARAMS );		//mid right 2
				title2.set( 1005, "theEmbassyIntTitle", POSN_R2, STATIONLOOP, LOBBYCELL );
				if( goingBackwards ) 
				 	GameFlag.set(fLeavingSafeHouse);
				break;
			 case STAEXT_MAX_ALLEY:
				if( goingBackwards )
					fromTo( 655, 669 );
				else 	{
					doSeq2 = TRUE;
					seq2( 655, 669 );
				}
				theAlleyMap = new AlleyMap( LEFT2_PARAMS );			//mid left 2
				title1.set( 1005, "AlleyMapTitle", POSN_L2, STATIONLOOP, ALLEYCELL );
				theMaxOfficeMap = new MaxOfficeMap( RIGHT2_PARAMS );
				title2.set( 1005, "MaxOfficeMapTitle", POSN_R2, STATIONLOOP, FOSTERCELL );
				break;
			  case STAEXT_LOBBY_VAN:
				if( goingBackwards )
					fromTo( 805, 819 );
				else 	{
					doSeq2 = TRUE;
					seq2( 805, 819 );
				}
				theProcatVanMap = new ProcatVanMap( LEFT2_PARAMS );			//mid left 2
				title1.set( 1005, "ProcatVanMapTitle", POSN_L2, STATIONLOOP, PROCVANCELL );
				theEmbassyInt = new EmbassyIntMap( RIGHT2_PARAMS );		//mid right 2
				title2.set( 1005, "theEmbassyIntTitle", POSN_R2, STATIONLOOP, LOBBYCELL );
				//GameFlag.clear( fPlayerTurnsProcat );	// reset if he goes into van
				//GameFlag.clear( fWahKa79 );
				GameFlag.set( fDCK11 );	// used to turn off fPlayerTurnsProcat and fWahKa79
			 	break;
			 default:
			 	#ifdef _DEBUG
					sfxPrintf( "Bad station exterior" );
				#endif
				NULL;
			}
			break;

		case MAP_STATION_INT:
		/*	ASSERT( theThornOfficeMap == NULL, 0 );
			ASSERT( theMaxOfficeMap == NULL, 0 );
			ASSERT( theBullpenMap == NULL, 0 );
			ASSERT( theInterrogationMap == NULL, 0 );*/
			checkStationInt();
			if( !goingBackwards ) {
				switch( availStationExtRegions )
				{
				 case STAEXT_LOBBY_ALLEY:
				 	fromTo( 775, 804 );
				 	break;
				 case STAEXT_LOBBY_VAN:
				 	fromTo( 820, 849 );
				 	break;
				 default:
				 	#ifdef _DEBUG
						sfxPrintf( "bad station interior predecessor" );
					#endif
					NULL;
				}
			}
			if( goingBackwards )
				fromTo( 895, 909 );
			else {
				doSeq2 = TRUE;			
				seq2( 895, 909 );
			}
			theThornOfficeMap = new ThornOfficeMap( TOP_RIGHT4_PARAMS );
			title1.set( 1005, "theThornOfficeMapTitle", POSN_TR4, STATIONLOOP, THORN2CELL );
			theMaxOfficeMap = new MaxOfficeMap( TOP_LEFT4_PARAMS );	
			title2.set( 1005, "theMaxOfficeMapTitle", POSN_TL4, STATIONLOOP, FOSTERCELL );
			theBullpenMap = new BullpenMap( BOT_LEFT4_PARAMS );	
			title3.set( 1005, "theBullpenMapTitle", POSN_BL4, STATIONLOOP, BULLPENCELL );
			theInterrogationMap = new InterrogationRoomMap( BOT_RIGHT4_PARAMS );
			title4.set( 1005, "theInterrogationMapTitle", POSN_BR4, STATIONLOOP, INTERROGCELL );		
			break;
		
		case MAP_ASSASS:
			//ASSERT( theAssSuite == NULL, 0 );
			// should be going backwards, coming from Procat van
			theAssSuite = new AssSuiteMap( BOT_RIGHT4_PARAMS );
			title1.set( 1005, "theAssSuiteMapTitle", POSN_BR4, MOSCOWLOOP, ASSASSCELL );
			fromTo( 880, 888 );
			//GameFlag.clear( fPlayerTurnsProcat );	// in case player backs out
			//GameFlag.clear( fWahKa79 );				// also	in case player backs out
			break;

		default:
			#ifdef _DEBUG
				sfxPrintf( "Bad region in showRegion: %d", theRegion );
			#endif
			NULL;
	};
	wentBackwards = goingBackwards;
	goingBackwards = FALSE;
}

void Rm1000::queryRegion( int theRegion )
{	//// figures out what's available in the region	 
	switch( theRegion )	{
	 case MAP_WORLD:
		break;

	 case MAP_CITY:
	 	if ( GameFlag.test( fMoscowAvailable ) ) {
			if( GameFlag.test( fPlayerTurnsProcat ) )
				cityAvail = CITY_NONE;	// assass suite, van too
			else if( GameFlag.test( fLondonAvailable ) )
				cityAvail = CITY_WASH_MOSC_LOND;
			else if( GameFlag.test( fDachaAvailable ) )
				cityAvail = CITY_WASH_DACHA;
			else if( GameFlag.test( fTunisiaAvailable ) )
				cityAvail = CITY_WASH_MOSC_TUNIS;
			else if( GameFlag.test( fHeidelbergAvailable ) )
				cityAvail = CITY_WASH_MOSC_HEIDL;
			else
				cityAvail = CITY_WASH_MOSC;
		}
		else
			cityAvail = CITY_WASH;
		break;

	 case MAP_WASHINGTON:
	 	if( GameFlag.test( fInitialBriefing ) && !GameFlag.test( fFarmDone ) )
			availWashRegions = WASH_FARM;
		else if( GameFlag.test( fColbySummons ) && !GameFlag.test( fColbyMoleBriefing ) ) 
			availWashRegions = WASH_LANG_COLBY;
		else
			availWashRegions = WASH_LANG;
		break;

	 case MAP_LANGLEY:
	 	if( GameFlag.test( fDCIOfficeAvailable ) ) {
			if( !GameFlag.test( fFullBriefing ) )
				availLangleyRegions = LANG_THORN_DCI2;
			else
				availLangleyRegions = LANG_THORN_DCI3;
		}
		else { 	// DCI not available
			if( GameFlag.test( fHoltSeatonAvailable ) )
				availLangleyRegions = LANG_THORN_HOLT_SEATON;
			else
				availLangleyRegions = LANG_THORN;
		}
	 	break;

	 case MAP_FARM:
			if( !GameFlag.test( fFarmDidPhotoIntro ) )
				availFarmRegions = FARM_FRANK;
			else if( !GameFlag.test( fFarmIADone ) )
				availFarmRegions = FARM_IA;
			else
				availFarmRegions = FARM_ZONE;
	 	break;

	 case MAP_MOSCOW:
		//Kneecaps appears by the hyperlink in Pearson's Operation Log and handwritten -> Image Analysis report
	 	if(GameFlag.test( fKneecapsFound )  && !GameFlag.test(fPlayerTurnsProcat) && !GameFlag.test( fWahKa100 ) )	// kneecaps avail
			availMoscowRegions = MOS_KNEE;

	//	else if(GameFlag.test(fBSMeetingSet) &&	//		//		//		//		//	bird
	//	   		GameFlag.test(fMORoomEnter) && GameFlag.test(fOnce35001) &&		//	lubyanka
	//	   		GameFlag.test(fDisplayStakout) )//		//		//		//		//	stakeout
		else if( GameFlag.test( fCanDisplayStakeOut ) && !GameFlag.test(fPlayerTurnsProcat))
			availMoscowRegions = /*MOS_STAKE_LUB_BIRD;*/MOS_STAKE; 

		else if(GameFlag.test(fYORoomEnter) &&
				!GameFlag.test(fVCRoomEnter) &&
				GameFlag.test(fOnce35000) ) //DEBUG change flag name later, maybe correct flag name
			availMoscowRegions = MOS_VIL;

		else if( GameFlag.test( fDCK2 /*fYasevnoAvailable*/ )  &&
		         GameFlag.test( fDCK1 /*fBirdsongAvailable*/ )  &&
		         !GameFlag.test( fPlayerTurnsProcat ) )
			availMoscowRegions = MOS_LUB_BIRD;

		else if( GameFlag.test( fDCK2 /*fYasevnoAvailable*/ ) &&
				 !GameFlag.test(fPlayerTurnsProcat) )
			availMoscowRegions = MOS_LUB;

		else if( GameFlag.test( fDCK1 /*fBirdsongAvailable*/ ) &&
				!GameFlag.test(fPlayerTurnsProcat) )	
			availMoscowRegions = MOS_BIRD;
		
		else	
			availMoscowRegions = MOS_STATION_ONLY;
		break;

	 case MAP_STATION_EXT:
		//Option to meet Harmonica in the alley or warn Max by getting H's call, 35110.wav
	 	if(GameFlag.test(fHarmCalls) && !GameFlag.test(fWahKa62))
	 		availStationExtRegions = STAEXT_MAX_ALLEY;
		else if( GameFlag.test( fPlayerTurnsProcat )  && !GameFlag.test( fEnteredProcatVan ) )
			availStationExtRegions = STAEXT_LOBBY_VAN;
		else 
			availStationExtRegions = STAEXT_LOBBY_ALLEY;	 
		break;

	 default:
		#ifdef _DEBUG
			sfxPrintf( "unknown map region %d in query fn.", theRegion );
		#endif
		NULL;
	}

	/************ debug ******************
	{
		static int world;
		static int city;
		static int moscow;
		static int station;
		static int washington;
		static int farm;
		static int langley;

	 	switch( theRegion )
		{
		 case MAP_WASHINGTON:
		 	++world;
			++city;
			++washington;
			if( world == WORLD_MAX )
				world = 0; 
			if( washington == WASH_MAX )
				washington = 0;
			if( city == CITY_MAX )
				city = 0;
		 break;
		 case MAP_MOSCOW:
			++moscow;
			if( moscow == MOS_MAX )
				moscow = 0;	
			++world;
			if( world == WORLD_MAX )
				world = 0; 
			++city;
			if( city == CITY_MAX )
				city = 0;	 
		 break;
		 case MAP_STATION_EXT:
		 	if( goingBackwards )
				break;
		 	++station;
		 	if( station == STAEXT_MAX )
		 		station = 0;	  
		 break;
		 case MAP_FARM:
		 	++farm;
		 	if( farm == FARM_MAX )
				farm = 0;
		 case MAP_LANGLEY:
		 	++langley;
			if( langley == LANG_MAX )
				langley = 0;
		}
		worldAvail = world;
		cityAvail = city;
		availMoscowRegions = moscow;
		availStationExtRegions = station;
		availWashRegions = washington;
		availFarmRegions = farm;
		availLangleyRegions	= langley;
	}
	/*************************************/
}

void Rm1000::checkWashington( void )
{
	#define GFT GameFlag.test
	if( GFT( fShow157 ) && !GFT( fInit156 ) && GFT( fColbyMoleBriefing ) )
	{		// checked in Moscow and Washington
		new VLink( "Ying got away: Warhurst, Eugene J.", "156.avi", fSaw156, 0 );
		GameFlag.set( fInit156 );
	}	
}

void Rm1000::checkLangley( void )
{
	//Jaimie gives the KAT
	#define GFT GameFlag.test
	if( GFT( fFullBriefing ) && !GFT( fKATAvailable )  && !GFT( fSolvedDubanskyKAT ) )
	{
		//Jaimie calls, Dubansky video on net
		new VLink(	"Assassination imagery: Seaton, Jaimie A.",
		            "634.avi", fSaw634, (rand() %5) +3,
		          	new EMailAttachment( "KAT and Dubansky Video", 1111 ) ); 
		GameFlag.set( fKATAvailable );
		new EMail( "Congratulations: Holt, David M.", 98083 );
		//pcLastUsed[PC_KAT] = TRUE;
	}
	// After PEG briefing  Jaimie gives Security Model
	if( GameFlag.test( fPEGBriefing )  && !GameFlag.test( fSetJaimies139 ) )
	{
		// mail order purposely reversed: 98091, 98009, 139
		new EMail("Possible lead: Foster, Maxine M.", 98091, 0, 0 );
		new VoiceMail( "Not your usual suspects: Holt, David M.", 98009, HOLT );
		new VLink(	"Find the PEG thief: Seaton, Jaimie A.", "139.avi", fSaw139, 2 );
		GameFlag.set( fSetJaimies139 );	// here's security model
		GameFlag.set( fSecurityModelAvailable );	
	}
	// Ying's stuff--even if you haven't seen email.  Otherwise may appear in strange ways
	// in the office.
	#define GFT GameFlag.test
	if( GFT( fShow157 ) && GFT( fInit156 ) && GFT( fColbyMoleBriefing ) )
	{
		GameFlag.set( fYingStuffArrived );	// vlink sets this too, but don't wait for that.
	}
	// Max has Ying
	#define GFT GameFlag.test
	if( GFT( fKnowYingInMoscow ) && !GFT( fDCK8 ) )
	{	// Note: fKnowYingInMoscow coincides with send of vlink 644 in cypher.cpp.
		if( !GFT( fSetVL_Max164 ) ) {
			if( GameFlag.test( fMetMaxFoster ) )
				new VLink( "Ying is bagged: Foster, Maxine M.", "1641.avi" );
			else
				new VLink( "Ying is bagged: Foster, Maxine M.", "164.avi" );
			GameFlag.set( fSetVL_Max164 );
		}
		GameFlag.set( fYingCaptured );
		GameFlag.set( fDCK8 );
	}
}

void Rm1000::checkMoscow( void )
{
	if( GameFlag.test( fSolvedDubanskyKAT )  && GameFlag.test( fSawHarmonicaDossier ) )
	{
	 	if( !GameFlag.test( fSetVL_Warh35104 ) && !inTransit )
		{
		 	new VLink( "Tommy's in town: Holt, David M.", "35104.avi", 0, 0, new EMailAttachment("tom" , 98200) );
			GameFlag.set( fSetVL_Warh35104 );	// actually Holt, not Warhurst
		}		
	}
	#define GFT GameFlag.test
	if( GFT( fShow157 ) && !GFT( fInit156 ) && GFT( fColbyMoleBriefing ) )
	{		// checked in Moscow and Washington
		new VLink( "Ying got away: Warhurst, Eugene J.", "156.avi", fSaw156, 0 );
		GameFlag.set( fInit156 );
	}
	#define GFT GameFlag.test
	if( GFT( fKnowYingInMoscow ) && !GFT( fSetVL_Max164 ) )
	{
		new EMail ( "Ying is bagged: Foster, Maxine M.", 98334 );
		GameFlag.set( fYingCaptured );
		GameFlag.set( fSetVL_Max164 );
	}	
}

void Rm1000::checkStationExt( void )
{
//	if( GameFlag.test( fVCRoomEnter )  && !GameFlag.test( fPlayerTurnsProcat )  &&
//	    !GameFlag.test( fWahKa97 ) &&  !GameFlag.test( fMetHarmonica ) )

	if (!GameFlag.test( fWahKa97 ) &&	   // test if max 1 sent before
		((GameFlag.test(fBeenTo57000)  && !GameFlag.test(fWahKa72) && GameFlag.test(fPlayBSDeed)) ||
		 (GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51))))
	{
		if (GameFlag.test(fBeenTo57000)  && !GameFlag.test(fWahKa72) && GameFlag.test(fPlayBSDeed))
			GameFlag.set(fLinus18);
		if (GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51))
			GameFlag.set(fWahKa82);
		new EMail( "Ringing in my ears: Foster, Maxine M.", 98095 );
		GameFlag.set( fWahKa97 );
	}

//	if( GameFlag.test( fMetHarmonica )  && !GameFlag.test( fPlayerTurnsProcat )  &&
//	    !GameFlag.test( fWahKa104 ) &&  !GameFlag.test( fOnceOnyx48000 ) )

	if (!GameFlag.test( fWahKa104 ) &&	 	// test if max 2 set before
		((GameFlag.test(fBeenTo57000)  && !GameFlag.test(fWahKa72) && GameFlag.test(fPlayBSDeed) && GameFlag.test(fWahKa51)) ||
		 (GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51) && GameFlag.test(fWahKa72))))
	{
		if (GameFlag.test(fBeenTo57000)  && !GameFlag.test(fWahKa72) && GameFlag.test(fPlayBSDeed) && GameFlag.test(fWahKa51))
			GameFlag.set(fLinus18);
		if (GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51) && GameFlag.test(fWahKa72))
			GameFlag.set(fWahKa82);
		new EMail( "Get me off the hook: Foster, Maxine M.", 98096 );
		GameFlag.set( fWahKa104 );
	}

	if( GameFlag.test( fYORoomEnter ) && 
	    GameFlag.test( fMetBirdSong ) && !GameFlag.test( fWahKa103 ) )
	{
		new VLink( "Nukes for sale: Holt, David M.", "675.avi" );
		GameFlag.set( fWahKa103 );
	}
	// Special Case: Lena Calls
	if( !inTransit )
	{
		if( GameFlag.test( fPlayLenaMsg ) &&
		    ( !GameFlag.test(fWahKa11) || GameFlag.test(fMetHarmonica) )  &&
		    !GameFlag.test( fPlayerTurnsProcat ) && !GameFlag.test( fWahKa52 ) )
		{	
				GameFlag.set(fWahKa52);
				GameFlag.set(fMoscowAlleyAvailable);
				GameFlag.set(fWahKa10);
				GameFlag.set(fWahKa20);
				GameFlag.set(fLenaCalls);
				curRoom->setScript( new LenaCallsScript1000 );
		}
	}
}

void Rm1000::checkStationInt( void )
{
// 		if( GameFlag.test(fBeenTo57000)  && !GameFlag.test(fWahKa72) && GameFlag.test(fPlayBSDeed)) {            
//          if (GameFlag.test(fBSTrue)) {
//              GameFlag.set(fWahKa72);
 										//CLUif( GameFlag.test(fBeenTo57000) && !GameFlag.test(fBSVoiceMessage) && GameFlag.test(fHaveSwanTwo)) {		
										//CLU		if (GameFlag.test(fBSTrue)) {
										//CLU			GameFlag.set(fBSVoiceMessage);
				//sfxPrintf("Message from BS-panic, they're onto me. Temp sound file.");
//				new VoiceMail("35126.wav", 35126, fBSVoiceMessage, 0);
//			}
//			else if(GameFlag.test(fBetrayMaxKneecaps)) {
//				GameFlag.set(fWahKa72);
				//sfxPrintf("Message from BS-I'm on your side. Temp sound file.Include new report");
//				new VoiceMail("35125.wav", 35125, fBSVoiceMessage, 0);
//			}
//			else {
//				GameFlag.set(fWahKa72);
//				sfxPrintf("this is a bug-no Ditem selected.");          //DEBUG wahka
//			}
//		}
///	if( GameFlag.test(fYORoomEnter) && GameFlag.test(fKnowGrendelStolePEG) && !GameFlag.test(fWahKa50)) {	
///		GameFlag.set(fWahKa50);
		//	sfxPrintf("phone is ringing VO#553 Yuri calling about gift");
///		new VoiceMail("35106.wav", 35106, fYuriGift, 0);
///	}
//	if( GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51)) {
//		GameFlag.set(fWahKa51);	
		//	sfxPrintf("phone is ringing Yuri calling about meeting at VC");								    
//		new VoiceMail("35109.wav", 35109, fOnce35000, 0);
//	}
//  if( GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52)) {	
//		GameFlag.set(fWahKa52);
		//	GameFlag.set(fMoscowAlleyAvailable);
		//	sfxPrintf("Lena calls, VO#724. Meet her at the alley");
//		new VoiceMail("35108.wav", 35108, fWahKa10, 0);
//	}
//	if( GameFlag.test(fHaveSwanTwo) && !GameFlag.test(fWahKa53)) {
//		GameFlag.set(fWahKa53);
		//	sfxPrintf("Yuri calling to meet about Onyx.");
//		new VoiceMail("35111.wav", 35111, fYuriAboutOnyx, 0);
//	}
//	if( GameFlag.test(fYingCaptured) && GameFlag.test(fBlakePatioTalkDone) && !GameFlag.test(fWahKa54)) {   	 
//		GameFlag.set(fWahKa54);
		//	GameFlag.set(fMoscowAlleyAvailable);
		//	GameFlag.set(fMetHarmonica);
		//	sfxPrintf("Harmonica calls, VO#106. Meet me in ally in 1 min");
//		new VoiceMail("35110.wav", 35110, fWahKa11, 0);
//	}
//	if( GameFlag.test(fRescuedBS) && !GameFlag.test(fWahKa65)) {
	if( GameFlag.test(fYingCaptured) && GameFlag.test(fHKRoomEnter) && GameFlag.test(fRescuedBS) && GameFlag.test(fBlakePatioTalkDone) && !GameFlag.test(fWahKa65)) 
	{
		GameFlag.set(fWahKa65);
//		new VLink( "Beale blues: Holt, David M.", "1080.avi", fWahKa55, 0 );
		new VLink( "Mole murmurs: Holt, David M.", "1081.avi", fWahKa56, 0, new EMailAttachment( "Mole Links", 98310 )  );	
//		new VLink( "1081.avi", fWahKa56, 0 );
//		new EMail( "Mole links: Holt, David M.", 98310);
	}
	if(GameFlag.test(fBlakePatioTalkDone) && !GameFlag.test(fWahKa117)) {
		GameFlag.set(fWahKa117);
		new VLink( "Beale blues: Holt, David M.", "1080.avi", fWahKa55, 0 );
		GameFlag.set( fNewElectBookAvail );
	}
	//CLUelse if(GameFlag.test(fOnceOnyx48000)) {
	//CLU 	new EMail( "OnyxHuntTwo98331", 98331);
	//CLU}
	if(GameFlag.test(fCanDoMaxIntrVLink) && !GameFlag.test(fPlayerTurnsProcat) && !GameFlag.test(fWahKa74) )
	{ 
		new VLink( "Teleconference: Foster, Maxine M.", "101.avi", fWahKa18, 0 );
		GameFlag.set(fWahKa74);
		GameFlag.clear(fDCK1);
	}
	if( GameFlag.test( fSolvedDubanskyKAT )  && !GameFlag.test( fWahKa41 ) &&
		GameFlag.test( fWahKa42) && !GameFlag.test( fWahKa75 ) && GameFlag.test( fWahKa84 ) &&
		!GameFlag.test(fWahKa106) && !inTransit && !GameFlag.test( fDCK7 ) )
	{
		new EMail( "Birdsong and Harmonica: Foster, Maxine M.", 98093 );
	 	GameFlag.set( fWahKa75 );
		GameFlag.set( fDCK7 );
	}
	if( GameFlag.test(fSolvedDubanskyKAT) && !GameFlag.test(fWahKa41)  && 
		GameFlag.test(fWahKa42)  && !GameFlag.test(fWahKa88) &&	GameFlag.test( fWahKa84 ) &&
		/*!GameFlag.test(fWahKa106) &&*/ !inTransit && !GameFlag.test( fDCK7 ) )
	{
		new EMail( "Harmonica notes: Foster, Maxine M.", 98094 );
	 	GameFlag.set( fWahKa88 );
		GameFlag.set( fDCK7 );
	}
	if( GameFlag.test(fBrokenYingInterrogation) && GameFlag.test( fSolvedDubanskyKAT )  && 
		!GameFlag.test( fWahKa41 ) && !GameFlag.test( fWahKa42) && GameFlag.test( fWahKa84 ) &&
		!GameFlag.test( fWahKa106 )  && !inTransit && !GameFlag.test( fDCK7 ) )
	{
		new EMail( "Birdsong and Harmonica: Foster, Maxine M.", 98093 );
	 	GameFlag.set( fWahKa106 );
		GameFlag.set( fDCK7 );
	}	
	if( !GameFlag.test(fSolvedDubanskyKAT) && GameFlag.test(fColbyMoleBriefing) && GameFlag.test(fMetBirdSong) && !GameFlag.test(fWahKa107)) 
	{
		new VoiceMail( "Waiting on you, Thorn: Holt, David M.", 98152, HOLT );
		GameFlag.set( fWahKa107 );
	}	
	if( GameFlag.test(fSolvedDubanskyKAT) && !GameFlag.test(fBrokeYingBealeCode) && GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa108)) 
	{
		new VoiceMail( "Waiting on you, Thorn: Holt, David M.", 98154, HOLT );
		GameFlag.set( fWahKa108 );
	}								
}

void Rm1000::drawTitle( MapTitle* theTitle )
{
	if( theTitle->posn == POSN_NONE )
		return;
	
 	switch( theTitle->posn )
	{
	 case POSN_L2:
	 	theTitle->title->init ( TX_LEFT2_PARAMS );
		break ;
	 case POSN_R2:
	 	theTitle->title->init ( TX_RIGHT2_PARAMS );
		break;
	 case POSN_T3:
	 	theTitle->title->init ( TX_TOP3_PARAMS );
		break;
	 case POSN_BL3:
	 	theTitle->title->init ( TX_BOT_LEFT3_PARAMS );
		break;
	 case POSN_BR3:
	 	theTitle->title->init ( TX_BOT_RIGHT3_PARAMS );
		break;
	 case POSN_TL4:
	 	theTitle->title->init ( TX_TOP_LEFT4_PARAMS );
		break;
	 case POSN_TL4X:
	 	theTitle->title->init ( TX_TOP_LEFT4X_PARAMS );
		break;
	 case POSN_TR4:
	 	theTitle->title->init ( TX_TOP_RIGHT4_PARAMS );
		break;
	 case POSN_BL4:
	 	theTitle->title->init ( TX_BOT_LEFT4_PARAMS );
		break;
	 case POSN_BR4:
	 	theTitle->title->init ( TX_BOT_RIGHT4_PARAMS );
		break;
	 case POSN_BR4X:
	 	theTitle->title->init ( TX_BOT_RIGHT4X_PARAMS );
		break;
	 default:
	 	#ifdef _DEBUG
			sfxPrintf( "Bad title position id" );
			return;
		#endif
		NULL;
	}
	theTitle->title->noSkip();
	theTitle->title->addToPic();
}

void Rm1000::cleanUp( void )
{
  switch( curMap )
  {
   case MAP_WORLD:
   	if( washington != NULL )
	{
		washington->dispose();
		washington = NULL;
	}
 	if( london  != NULL )
 	{ 
		london->dispose(); 
		london = NULL;
	}
	if( tunisia != NULL )
	{
  		tunisia->dispose(); 
		tunisia = NULL;
	}
	if( moscow != NULL )
  	{
  		moscow->dispose();
  		moscow = NULL;
	} 
	if( heidelberg != NULL )
  	{
  		heidelberg->dispose();
  		heidelberg = NULL;
	}
	//if( crimea != NULL )
  	//{
  	//	crimea->dispose();
	//	crimea = NULL;
	//}
	if( dacha != NULL )
	{
  		dacha->dispose();
		dacha = NULL;
	}
	break;

   case MAP_WASHINGTON:
    if ( theFarm )	{
    	theFarm->dispose();
		theFarm = NULL;
	}
	if ( theLangley )	{
    	theLangley->dispose();
		theLangley = NULL;
	}
	if( theColbyHouse )	{
    	theColbyHouse->dispose();
		theColbyHouse = NULL;
	}
    break;
  
   case  MAP_LONDON:
   	if( london ) {
		london->dispose();
		london = NULL;
	}
	break;

	case MAP_TUNISIA:
		if( tunisia ) {
		tunisia->dispose();
		tunisia = NULL;
	}
	break;

	case MAP_HEIDELBERG:
		if( heidelberg ) {
		heidelberg->dispose();
		heidelberg = NULL;
	}
	break;

	case MAP_CRIMEA:
		if( dacha ) {
		dacha->dispose();
		dacha = NULL;
	}
	break;

	case MAP_REFINERY:
	if (theRefineryShooter)	{
		theRefineryShooter->dispose();
		theRefineryShooter = NULL;
	}
	if (theRefineryDialog)	{
		theRefineryDialog->dispose();
		theRefineryDialog = NULL;
	}
	break;
  
   case MAP_MOSCOW:
  		if (theBirdsong)	{
    		theBirdsong->dispose();
			theBirdsong = NULL;
		}
		if (theVilniusCenter)	{  
			theVilniusCenter->dispose();
			theVilniusCenter = NULL;
		}
		if (theLubyanka)	{
			theLubyanka->dispose();
			theLubyanka = NULL;
		}
		if (theEmbassy)	{
			theEmbassy->dispose();
			theEmbassy = NULL;
		}
		if ( theStakeOut   )	{
			theStakeOut->dispose();
			theStakeOut = NULL;
		}
		if (theKneecap   )	{
			theKneecap->dispose();
			theKneecap = NULL;
		}
		break;

	case MAP_LANGLEY:
		if (theJaimie)	{
			theJaimie->dispose();
			theJaimie = NULL;
		}
		if (theHolt)	{
			theHolt->dispose();
			theHolt = NULL;
		}
		if (theDCI)	{
			theDCI->dispose();
			theDCI = NULL;
		}
		if (theWashPlayer)	{
			theWashPlayer->dispose();
			theWashPlayer = NULL;
		}
		break;

	case MAP_STATION_EXT:
			if (theEmbassyInt)	{
				theEmbassyInt->dispose();
				theEmbassyInt = NULL;
			}
			if (theAlleyMap)	{
				theAlleyMap->dispose();
				theAlleyMap = NULL;
			}
			if ( theMaxOfficeMap )	{
				theMaxOfficeMap->dispose();
				theMaxOfficeMap = NULL;
			}
			if ( theProcatVanMap )	{
				theProcatVanMap->dispose();
				theProcatVanMap = NULL;
			}
			break;

	case MAP_STATION_INT:
			if ( theBullpenMap)	{
				theBullpenMap->dispose();
				theBullpenMap = NULL;
			}
			if (theThornOfficeMap )	{
				theThornOfficeMap->dispose();
				theThornOfficeMap = NULL;
			}
			if (theMaxOfficeMap )	{
				theMaxOfficeMap->dispose();
				theMaxOfficeMap = NULL;
			}
			if (theInterrogationMap )	{
				theInterrogationMap->dispose();
				theInterrogationMap = NULL;
			}
			break;                                                                                                   
                                                                           		
	case MAP_FARM:
		if (theFrank)	{
			theFrank->dispose();
			theFrank = NULL;
		}

		if (theIA)	{
			theIA->dispose();
			theIA = NULL;
		}

		if (theZone)	{
			theZone->dispose();
			theZone = NULL;
		}
		break;
	 case MAP_ASSASS:
		if( theAssSuite )	{
			theAssSuite->dispose();
			theAssSuite = NULL;
		}
	 	break;
	 case MAP_MOSCOWPLATE:
	 	break;
	 default:
		#ifdef _DEBUG
			sfxPrintf( "Bad curMap case in Cleanup" );
		#endif
		NULL;
 	}	// end switch

	rm1000->title1.reset();
	rm1000->title2.reset();
	rm1000->title3.reset();
	rm1000->title4.reset();
}

void Rm1000::startMidi( void )
{	
	static midiInfo*	midiTable[M1000_MAX];
	static int			rowSizes[M1000_MAX];
	int		row;
	int		choice;
	int		altChoice;

	// Washington
	#define WASZ	4
	static midiInfo	wash[WASZ] 		= {	0, 1001,	0, 1002,	0, 1003,	0, 1004 	};
	rowSizes[M1000_WASH]			= WASZ;
	midiTable[M1000_WASH]			= wash;
	// Moscow
	#define MOSZ	4
	static midiInfo moscow[MOSZ]	= { 0, 1500,	0, 1500,	0, 1501,	0, 1501		};
	rowSizes[M1000_MOSCOW] 			= MOSZ;
	midiTable[M1000_MOSCOW] 		= moscow;
	// Station House
	#define STSZ	4
	static midiInfo staHse[STSZ]	= { 0, 1021,	0, 1022,	0, 1023,	0, 1024		};
	rowSizes[M1000_STAHSE] 			= STSZ;
	midiTable[M1000_STAHSE] 		= staHse;
	// Go meet Harmonica
	#define HASZ	1
	static midiInfo	harmonica[HASZ]	= { 0, 1040		};
	rowSizes[M1000_HARMONICA]		= HASZ;
	midiTable[M1000_HARMONICA] 		= harmonica;
	#define PRSZ	1
	// Procat Van
	static midiInfo procVan[PRSZ]	= { 0, 1080		};
	rowSizes[M1000_PROCVAN]			= PRSZ;
	midiTable[M1000_PROCVAN] 		= procVan;
	// Birdsong
	#define BISZ	4
	static midiInfo bird[BISZ]		= { 0, 1031,	0, 1032,	0, 1033,	0, 1034		};
	rowSizes[M1000_BIRD]			= BISZ;  
	midiTable[M1000_BIRD] 			= bird;
	// London
	#define LOSZ	1
	static midiInfo london[LOSZ]	= { 0, 1050		};
	rowSizes[M1000_LONDON] 			= LOSZ;								
	midiTable[M1000_LONDON] 		= london;
	// Heidelberg
	#define HESZ	1
	static midiInfo heidel[HESZ]	= { 0, 1060		};
	rowSizes[M1000_HEIDEL] 			= HESZ;
	midiTable[M1000_HEIDEL] 		= heidel;
	// Refinery
	#define RESZ	1
	static midiInfo refine[RESZ]	= { 0, 1070		};
	rowSizes[M1000_REFINE] 			= RESZ;
	midiTable[M1000_REFINE] 		= refine;
	// Assassination
	#define ASSZ	1
	static midiInfo assass[ASSZ]	= { 0, 1081		};
	rowSizes[M1000_ASSASS] 			= ASSZ;
	midiTable[M1000_ASSASS] 		= assass;
	// Dascha
	#define DASZ	1
	static midiInfo	dascha[DASZ]	= { 0, 1060		};
	rowSizes[M1000_DASCHA] 			= DASZ;
	midiTable[M1000_DASCHA] 		= dascha;

	row = findMidiTableRow( curMap );
	// Select the music
	choice = rand() %rowSizes[row];
	altChoice = rand() %rowSizes[row];
	if( midiTable[row][altChoice].used < midiTable[row][choice].used )
		choice = altChoice;
	++midiTable[row][choice].used;
	startMidi( midiTable[row][choice].number );  
}

void Rm1000::startMidi( int midiNumber )
{
 	musicHandle = sfxPlayMusic( midiNumber, TL_MIDIVOL, 0, 100, 0, -1, NULL );
 	musicNumber = midiNumber;	
	#ifdef _DEBUG
		//if( musicHandle == -1 )
		//	sfxPrintf( "Error: Travelink midi request failed" );
	#endif
}								   

void Rm1000::stopMidi( void )
{
	if( musicHandle != -1 )
		sfxStopMusic( musicHandle ); 	
	musicHandle = -1;
}

int  Rm1000::findMidiTableRow( int location )
{
	int		row;

	switch( location )
	{
	 case MAP_WASHINGTON:
	 case MAP_FARM:
	 case MAP_LANGLEY:
	 case MAP_COLBY:
		row = M1000_WASH;
	 	break;
	 case MAP_LONDON:
		row = M1000_LONDON;
	 	break;
	 case MAP_MOSCOW:
	 case MAP_MOSCOWPLATE:
	 case MAP_TUNISIA:
	 case MAP_KNEECAP:
	 case MAP_STAKEOUT:
	 case MAP_YASEVNO:
		row = M1000_MOSCOW;
	 	break;
	 case MAP_STATION_EXT:
	 case MAP_STATION_INT:
	 	queryRegion( MAP_STATION_EXT );
	 	switch( availStationExtRegions )
		{
	 	 case STAEXT_LOBBY_ALLEY:
			if( GameFlag.test( fHarmCalls ) && !GameFlag.test( fWahKa62 ) )
				row = M1000_HARMONICA;	// warned Max, not met Harmonica
			else
				row = M1000_STAHSE;
			break;
		 case STAEXT_MAX_ALLEY: 
			row = M1000_HARMONICA;
			break;
		 case STAEXT_LOBBY_VAN:
			row = M1000_PROCVAN;
			break;
		 default:
		 	row = M1000_STAHSE;
		
		 	
		 	//ASSERT( FALSE, 0 );
		}
	 	break;
	 case MAP_HEIDELBERG:
		if( GameFlag.test( fHKRoomEnter ) &&  !GameFlag.test( fEndgamePitStopped ) )
			row = M1000_REFINE; // already been to the hotel but not finished pit
		else
			row = M1000_HEIDEL;
	 	break;
	 case MAP_REFINERY:
		row = M1000_REFINE;
	 	break;
	 case MAP_CRIMEA:
		row = M1000_DASCHA;
	 	break;
	 case MAP_ASSASS:
		row = M1000_ASSASS;
	 	break;
	 case MAP_BIRDSONG:
		row = M1000_BIRD;
	 	break;
	 default:
	 	#ifdef _DEBUG
			sfxPrintf( "Unknown location for midi table" );
		#endif
		NULL;
	 	break;
	}
	return row;	
}

/*********************
*
*		MapTitle 
*
**********************/

MapTitle::MapTitle()
{
	title = NULL;
	posn = POSN_NONE;
}

void MapTitle::set( int theView, char* theName, int thePosn, int theLoop, int theCell )
{
	//ASSERT( title == NULL, 0 );
	title = new View;
	title->setView( theView );
 	if( theName != NULL)
		title->name = theName;
	else
		title->name = "MapTitleView";
	title->loop = theLoop;
	title->cel = theCell;
	posn = thePosn;
	++titleCounter;		// debug
}

void MapTitle::reset( void )
{
 	if( title != NULL )	{
		title->dispose();
		title = NULL;
		--titleCounter;	// debug
	}
	posn = POSN_NONE;
}

/*********************
*
*	City Triad
*
**********************/

CityBt1000::CityBt1000( int side, int city )
{
	name = "CityBt1000";
	//setHotspot( DO_V );
	myCity = city;
	switch( city )
	{	
	 case MAP_WASHINGTON:
	 	myLoop = CT_WASHINGTONLOOP;
	 	break;
	 case MAP_LONDON:
		myLoop = CT_LONDONLOOP;
	 	break;
	 case MAP_MOSCOW:
	 case MAP_MOSCOWPLATE: 		// this one probably never used
		myLoop = CT_MOSCOWLOOP;
	 	break;
	 case MAP_TUNISIA:
		myLoop = CT_TUNISIALOOP;
	 	break;
	 case MAP_HEIDELBERG:
		myLoop = CT_GERMANYLOOP;
	 	break;
	 case MAP_CRIMEA:
		myLoop = CT_CRIMEALOOP;
	 	break;
	 default:
	 	myLoop = CT_MOSCOWLOOP;
	 //	ASSERT( FALSE, 0 );
	}
	setView( 1008 );
	setLoop( myLoop );
	if( side == LEFTBT ) {
		init ( 120, 403 );
		noSkip();
	}
	else if( side == RIGHTBT ) {
		init ( 296, 403 );
		noSkip();
	}
	else	{
		init ( 120, 403 );
		noSkip();
		//sfxPrintf( "Unknown button side" );
	}
}

int CityBt1000::doVerb( int theVerb )
{
	int	 go;

	if( theVerb == DO_V ) {
		rm1000->cleanUp();
		go = verifyDestinationDisc( myCity );
		if( go ) {
			//rm1000->cleanUp(); --moved to verify; happens for transition --moved again:above
			inTransit = TRUE;
			sfxSystemCursor(SYS_WAIT);
			user->canInput( FALSE );
			goingBackwards = TRUE;
			rm1000->showRegion( myCity );
		}
    	return TRUE;
	}
	return FALSE;
}

int  CityBt1000::verifyDestinationDisc( int destination )
{	// returns: boolean--go, and changes TL curDisc
	//int		ok;
	int		change;

	change = FALSE;

	switch( destination )
	{
	 case MAP_WASHINGTON:
	 case MAP_LONDON:
		if( curDisc != USADISC )	{
		//	ok = MessageBox( NULL, "Change Discs?", "Verify Next Action", MB_OKCANCEL );	
		//	if( ok == IDOK ) {
				theMovie->caller = 0;
				theMovie->stop();
				//rm1000->cleanUp();
				//sfxPrintf("Please insert USA disc");
				curDisc = USADISC;
				#if BKMIDI == ON  && TRAVELMIDI == ON
					rm1000->stopMidi();
				#endif
				curMap = destination;	// startMidi() reads this
				#if BKMIDI == ON  && TRAVELMIDI == ON						
					rm1000->startMidi();
				#endif
				change = TRUE;
		//	}
		//	else
		//		change = FALSE;
		}
		change = TRUE;	// London may be on the same disk.  This is a design problem --dck
		break;
	 case MAP_MOSCOW:
		if( curDisc != EURODISC )	{
		//	ok = MessageBox( NULL, "Change Discs?", "Verify Next Action", MB_OKCANCEL );	
		//	if( ok == IDOK ) {	
				theMovie->caller = 0;
				theMovie->stop();
				//rm1000->cleanUp();
				//sfxPrintf("Please insert Moscow disc");
				curDisc = EURODISC;
				#if BKMIDI == ON  && TRAVELMIDI == ON
					rm1000->stopMidi();
				#endif
				curMap = destination;	// startMidi() reads this			
				#if BKMIDI == ON  && TRAVELMIDI == ON
					rm1000->startMidi();
				#endif
				change = TRUE;
		//	}
		//	else
		//		change = FALSE;
		}
	 	break;
	 case MAP_HEIDELBERG:
	 case MAP_TUNISIA:
	 case MAP_CRIMEA:
	 case MAP_MOSCOWPLATE:
		if( curDisc != ENDGAMEDISC )	{
			//ok = MessageBox( NULL, "Change Discs?", "Verify Next Action", MB_OKCANCEL );	
			//if( ok == IDOK ) {
				theMovie->caller = 0;
				theMovie->stop();
				//rm1000->cleanUp();
				//sfxPrintf("Please insert Endgame disc");
				curDisc = ENDGAMEDISC;
				#if BKMIDI == ON  && TRAVELMIDI == ON
					rm1000->stopMidi();
				#endif
				curMap = destination;	// startMidi() reads this
				#if BKMIDI == ON  && TRAVELMIDI == ON
					rm1000->startMidi();
				#endif
				change = TRUE;
			//}
			//else
			//	change = FALSE;			
		}
		change = TRUE;	// MAP_MOSCOWPLATE doesn't require disk change
	 	break;
	 default:
	 	#ifdef _DEBUG
			sfxPrintf( "Unknown destination %d for disc test", destination );
		#endif
			change = FALSE;
		NULL;
	}
	return change; 	
}

CityTitle1000::CityTitle1000()
{
	name = "CityTitle";
	setView( 1006 );
	setLoop( CT_TOPBARLOOP );
	init( 64, 5 );
	title = NULL;
}

// not used at clean up--cast list cleans up
void CityTitle1000::dispose( void )
{
	//sfxPrintf( "how'd you get here?" );
	if( title != NULL )
 		title->dispose();
	View::dispose();
}

void CityTitle1000::update( int city )
{
	int		cell;

	if( title != NULL )
		title->dispose();
	title = new View;
	title->name = "City name";
 	switch( city )
	{		
	 case MAP_WASHINGTON:
	 	cell = CT_WASHINGTONCELL;
	 	break;
	 case MAP_LONDON:
		cell = CT_LONDONCELL;
	 	break;
	 case MAP_MOSCOW:
	 case MAP_MOSCOWPLATE:
		cell = CT_MOSCOWCELL;
	 	break;
	 case MAP_TUNISIA:
		cell = CT_TUNISIACELL;
	 	break;
	 case MAP_HEIDELBERG:
		cell = CT_HEIDELBERGCELL;
	 	break;
	 case MAP_CRIMEA:
		cell = CT_DACHACELL;
		break;
	 default:
	 	cell = CT_MOSCOWCELL;
	//	ASSERT( FALSE, 0 );
	}
	title->setView( 1006 );
	title->setLoop( CT_TOPTITLELOOP );
	title->setCel( cell );
	title->init ( 209, 5 );
}

CityTriad1000::CityTriad1000()
{
	name = "CityTriad1000";
	topTitle = new CityTitle1000();		// top bar (no title yet)
	wingThing = new View;				// bottom bar
	wingThing->name = "WingThing";
	wingThing->setView( 1007 );
	wingThing->setLoop( CT_BOTBARLOOP );
	wingThing->init( 78, 400 );
	backBt = new BackBt1000;
	lBt = NULL;
	rBt = NULL;
	update();
}

// not used at shut down--cast list cleans up
void CityTriad1000::dispose( void ) // disposes of attachments
{
	//sfxPrintf( "how'd you get here? CT dispose()" );
 	topTitle->dispose();
	if( lBt != NULL )
		lBt->dispose();
	if( rBt != NULL )
		rBt->dispose();
	wingThing->dispose();
	backBt->dispose();
}

void CityTriad1000::update( void )
{
	int	here;
	int left;
	int right;
	
	calcTriad( curMap, here, left, right );
//	ASSERT( topTitle != NULL, 0 );
	topTitle->update( here );
	if( lBt != NULL )
	{ 	
		lBt->dispose();
		lBt = NULL;
	}
	if( left != CT_NOCITY )
		lBt = new CityBt1000( LEFTBT, left );
	if( rBt != NULL )
	{ 	
		rBt->dispose();
		rBt = NULL;
	}
	if( right != CT_NOCITY )
		rBt = new CityBt1000( RIGHTBT, right );
}

void CityTriad1000::calcTriad( int local, int& here, int& left, int& right )
{
	rm1000->queryRegion( MAP_CITY );
	left = right = CT_NOCITY;

 	switch( local )
	{		
	 case MAP_WASHINGTON:
	 case MAP_FARM:
	 case MAP_LANGLEY:
	 case MAP_COLBY:
	 	here = MAP_WASHINGTON;

		switch( cityAvail )
		{
		 case CITY_NONE:
		 case CITY_WASH:
			break;
		 case CITY_WASH_DACHA:
			left = MAP_CRIMEA;
			break;
		 case CITY_WASH_MOSC:
			left = MAP_MOSCOW;
			break;
		 case CITY_WASH_MOSC_LOND:
			left = 	MAP_MOSCOW;
			right = MAP_LONDON;
			break;
		 case CITY_WASH_MOSC_HEIDL:
			left = 	MAP_MOSCOW;
			right = MAP_HEIDELBERG; 
			break;
		 case CITY_WASH_MOSC_TUNIS:
			left = 	MAP_MOSCOW;
			right = MAP_TUNISIA; 
			break;
		 //default:
		 //	ASSERT( FALSE, 0 );
		}
	 	break;

	 case MAP_LONDON:
		here = MAP_LONDON;
		left = MAP_WASHINGTON;
		right = MAP_MOSCOW;		
		break;

	 case MAP_MOSCOW:
	 case MAP_STATION_INT:
	 case MAP_STATION_EXT:
	 case MAP_ASSASS:
	 case MAP_YASEVNO:
	 case MAP_BIRDSONG:
	 case MAP_STAKEOUT:
	 case MAP_KNEECAP:
	 	here = MAP_MOSCOW;

		switch( cityAvail )
		{
		 case CITY_NONE:		// assass suite
		 	break;
		 case CITY_WASH:
		 //	sfxPrintf( "Debug state only" );
			break;
		 case CITY_WASH_DACHA:
		 	left = MAP_CRIMEA;
			break;
		 case CITY_WASH_MOSC:
		 	left = MAP_WASHINGTON;
			break;
		 case CITY_WASH_MOSC_LOND:
		 	left = MAP_WASHINGTON;
			right = MAP_LONDON;
			break;
		 case CITY_WASH_MOSC_HEIDL:
		 	left = MAP_WASHINGTON;
			right = MAP_HEIDELBERG;
			break;
		 case CITY_WASH_MOSC_TUNIS:
		 	left = MAP_WASHINGTON;
			right = MAP_TUNISIA;
			break;
		 //default:
		 //	ASSERT( FALSE, 0 );
		}
	 	break;

	 case MAP_TUNISIA:
	 	here = MAP_TUNISIA;
		left = MAP_WASHINGTON;
		right = MAP_MOSCOW;
	 	break;

	 case MAP_HEIDELBERG:	
	 case MAP_REFINERY:
		here = MAP_HEIDELBERG;
		left = MAP_WASHINGTON;
		right = MAP_MOSCOW;
		break;

	 case MAP_CRIMEA:
	 	here = MAP_CRIMEA;
		//left = MAP_WASHINGTON;
		break;
	 case MAP_MOSCOWPLATE:
	 	here = MAP_MOSCOWPLATE;
	 	break;

	// default:
		//ASSERT( FALSE, 0 );
	}
}

BackBt1000::BackBt1000()
{
	name = "BackBt1000";
	setView( 1007 );
	setLoop( CT_BACKLOOP );
	init( 242, 400 );
}

int BackBt1000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
	    rm1000->cleanUp();
		inTransit = TRUE;
		sfxSystemCursor(SYS_WAIT);
		user->canInput( FALSE );
		goingBackwards = TRUE;
		
		switch ( curMap )
		{
			case MAP_WASHINGTON:
			case MAP_MOSCOW:
			case MAP_MOSCOWPLATE:
			case MAP_HEIDELBERG:
			case MAP_CRIMEA:
			case MAP_LONDON:
			case MAP_TUNISIA:
				break;	// no change
			case MAP_REFINERY:
				curMap = MAP_HEIDELBERG;
				break;
			case MAP_STATION_EXT:
				curMap = MAP_MOSCOW;
				if( GameFlag.test( fDCK11 ) )
				{
				 	GameFlag.clear( fPlayerTurnsProcat );
					GameFlag.clear( fWahKa79 );
				}	// in case backing out of Procat van choice
				break;
			case MAP_STATION_INT:
				curMap = MAP_STATION_EXT;	
				break;
			case MAP_FARM:
				curMap = MAP_WASHINGTON;
				break;
			case MAP_LANGLEY:
				curMap = MAP_WASHINGTON;
				break;
			case MAP_ASSASS:
				curMap = MAP_ASSASS; 	// you're trapped!
				break;
			default:
				#ifdef _DEBUG
					sfxPrintf( "Can't back up from %d", curMap );
				#endif
				NULL;
		}
		rm1000->showRegion( curMap );
		return TRUE;
	}
	return FALSE;
}

/*********************
*
*	Map Features 
*
**********************/

void MapFeature::verifyDisc( int theDisc )
{
	// World level remnant.  Using buttons now.
	// Note that startMidi() won't work right unless curMap is set
	// to the destination before this function is called.
	switch( theDisc )
	{
	 case USADISC:
		if( curDisc != USADISC )	{	
			//sfxPrintf("Please insert USA disc");
			curDisc = USADISC;
			#if BKMIDI == ON  && TRAVELMIDI == ON
				rm1000->stopMidi();
				rm1000->startMidi();			
			#endif
		}
		break;
	 case EURODISC:
		if( curDisc != EURODISC )	{	
			//sfxPrintf("Please insert Moscow disc");
			curDisc = EURODISC;
			#if BKMIDI == ON  && TRAVELMIDI == ON
				rm1000->stopMidi();
				rm1000->startMidi();			
			#endif
		}
	 	break;
	 case ENDGAMEDISC:
		if( curDisc != ENDGAMEDISC )	{
			//sfxPrintf("Please insert Endgame disc");
			curDisc = ENDGAMEDISC;
			#if BKMIDI == ON  && TRAVELMIDI == ON
				rm1000->stopMidi();
				rm1000->startMidi();
			#endif
		}
	 	break;
	 default:
	 	#ifdef _DEBUG
			sfxPrintf( "Unknown disc type %d for test", theDisc );
		#endif
		NULL;
	} 
	theMovie->caller = 0;
	theMovie->stop();	
}

void MapFeature::cutToRoom( Room* theRoom )
{
	rm1000->cleanUp();
	theMovie->caller = 0;
	theMovie->stop();
	sfxSystemCursor( SYS_WAIT );
	theGame->newRoom( theRoom );				
}

int MapFeature::doVerb( int )
{
	inTransit = TRUE;
	return TRUE;
}
	
MapFrank::MapFrank(int theX, int theY, int theWidth, int theHeight)
{
	name = "MapFrank";
	init (theX, theY, theWidth, theHeight);	
}

int MapFrank::doVerb ( int theVerb )
{
	if( theVerb == DO_V )	{		
	#ifdef INCLUDE_USA
		MapFeature::doVerb( theVerb );
		cutToRoom( new Rm2001 );
	#endif
		return TRUE;
	}
	return FALSE;
}

IAMap::IAMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "IAMap";
	init (theX, theY, theWidth, theHeight);
}

int IAMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
	#ifdef INCLUDE_USA
		MapFeature::doVerb( theVerb );
	    curRoom->setScript( new MapExit1000( 825, 854, new Rm5000 ) );
	#endif
		return TRUE;
	}
	return FALSE;
}

ZoneMap::ZoneMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "ZoneMap";
	init (theX, theY, theWidth, theHeight);	
}

int ZoneMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
	#ifdef INCLUDE_USA
		MapFeature::doVerb( theVerb );
		curRoom->setScript( new MapExit1000( 870, 899, new Rm7000) );
	#endif
		return TRUE;
	}
	return FALSE;
}

JaimieMap::JaimieMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "JaimieMap";
	init (theX, theY, theWidth, theHeight);
}

int JaimieMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
	#ifdef INCLUDE_USA
		MapFeature::doVerb( theVerb );
		if(GameFlag.test( fDCIOfficeAvailable )) // no longer used
			curRoom->setScript( new MapExit1000( 1050, 1079, new Rm27000 ) );
	    else
	    	curRoom->setScript( new MapExit1000( 645, 674, new Rm27000 ) );
	#endif
		return TRUE;
	}
	return FALSE;
}

HoltMap::HoltMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "HoltMap";
	init (theX, theY, theWidth, theHeight);
}

int HoltMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
	#ifdef INCLUDE_USA
		MapFeature::doVerb( theVerb );
		if (GameFlag.test( fDCIOfficeAvailable ))	// no longer used
			curRoom->setScript( new MapExit1000( 990, 1019, new Rm26000 ) );
	    else
	    	curRoom->setScript( new MapExit1000( 585, 614, new Rm26000 ) );
	#endif
		return TRUE;
	}
	return FALSE;
}

WashPlayerMap::WashPlayerMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "WashPlayerMap";
	init (theX, theY, theWidth, theHeight);
}

int WashPlayerMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
	#ifdef INCLUDE_USA
		MapFeature::doVerb( theVerb );
		switch( availLangleyRegions )
		{
		 case LANG_THORN_DCI2:
		 	curRoom->setScript( new MapExit1000( 540, 569, new Rm24000 ) );
			break;
		 case LANG_THORN_DCI3:
		 	curRoom->setScript( new MapExit1000( 735, 764, new Rm24000 ) );
			break;
		 case LANG_THORN_HOLT_SEATON:
		 	curRoom->setScript( new MapExit1000( 615, 644, new Rm24000 ) );
			break;
		 case LANG_THORN:
		 	curRoom->setScript( new MapExit1000( 690, 719, new Rm24000 ) );
			break;
		 default:
		 	#ifdef _DEBUG
				sfxPrintf( "Unknown player's office predecessor %d", availLangleyRegions );
			#endif
			NULL;
		}
	#endif
		return TRUE;
	}
	return FALSE;
}

DCIMap::DCIMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "DCIMap";
	init (theX, theY, theWidth, theHeight);
}

int DCIMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
	#ifdef INCLUDE_USA
		drawBackground = FALSE;
		MapFeature::doVerb( theVerb );
		cutToRoom( new Rm18000 );
	#endif
		return TRUE;
	}
	return FALSE;
}
   
MoscowMap::MoscowMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "MoscowMap";
	init (theX, theY, theWidth, theHeight);
}

// not used: no World level
int MoscowMap::doVerb ( int theVerb )
{
//	sfxPrintf( "How'd you get here?" );
	if (( theVerb == DO_V ) && (curMap != MAP_MOSCOW)) {
		MapFeature::doVerb( theVerb );
		verifyDisc( EURODISC );
		sfxSystemCursor(SYS_WAIT);
    	rm1000->cleanUp();
		rm1000->showRegion( MAP_MOSCOW );
		return TRUE;
	}
	return FALSE;
}

HeidelbergMap::HeidelbergMap( int theX, int theY, int theWidth, int theHeight )
{
// Hotel or refinery
	name = "Heidelberg";
	init (theX, theY, theWidth, theHeight);
}

int HeidelbergMap::doVerb ( int theVerb )
{
// Hotel or refinery (not world level Heidelberg choice)
    if( theVerb == DO_V ) {
		MapFeature::doVerb( theVerb );
		#ifdef INCLUDE_EUROPE
			//verifyDisc( ENDGAMEDISC );
			if( GameFlag.test( fHKRoomEnter ) )  {	// already been to the hotel
				rm1000->cleanUp();
				sfxSystemCursor( SYS_WAIT );
				rm1000->showRegion( MAP_REFINERY );// to choice screen
			}
			else  { 	// go to hotel, go directly to hotel
				cutToRoom( new Rm69000 );		
				//curMap = MAP_REFINERY;	// (comes after cut to allow for proper cleanup)
			}
      	#endif
        return TRUE;
	}
	return FALSE;
}

RefineryShooterMap::RefineryShooterMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Refinery";
	init (theX, theY, theWidth, theHeight);
}

int RefineryShooterMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			GameFlag.set( fStartCombat );
			curRoom->setScript( new  MapExit1000( 1090, 1119, new Rm95218 ) );
		#endif	   
    	return TRUE;
	}
	return FALSE;
}

RefineryDialogMap::RefineryDialogMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Refinery";
	init (theX, theY, theWidth, theHeight);
}

int RefineryDialogMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			cutToRoom( new Rm1075 );
		#endif	   
    	return TRUE;
	}
	return FALSE;
}

// not used
CrimeaMap::CrimeaMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Cirmea";
	init (theX, theY, theWidth, theHeight);
}

// not used
int CrimeaMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			//curRoom->setScript( new MapExit1000( 32, 61, new Rm1085 ) );
		#endif
    	return TRUE;
	}
	return FALSE;
}

VilniusCenterMap::VilniusCenterMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "VilniusCenter";
	init (theX, theY, theWidth, theHeight);
}

int VilniusCenterMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			switch( availMoscowRegions ) {
			 case MOS_VIL:
			 	curRoom->setScript( new  MapExit1000( 90, 119, new Rm53000 ) );
				break;
			 case MOS_VIL_BIRD:
			 	curRoom->setScript( new  MapExit1000( 625, 654, new Rm53000 ) );
				break;
			}
		#endif	   
    	return TRUE;
	}
	return FALSE;
}

BirdsongMap::BirdsongMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Birdsong";
	init (theX, theY, theWidth, theHeight);
}

int BirdsongMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			switch( availMoscowRegions ) {
				case MOS_LUB_BIRD:			
					curRoom->setScript(new  MapExit1000( 225, 269, new Rm56000 ));
					break;
				case MOS_VIL_BIRD:			
					curRoom->setScript(new  MapExit1000( 545, 594, new Rm56000 ));
					break;
				case MOS_BIRD:
					curRoom->setScript(new  MapExit1000( 135, 179, new Rm56000 ));
					break;
				default:
					#ifdef _DEBUG
						sfxPrintf( "Bad initial state to go to Birdsong's" );
						return TRUE;
					#endif
					NULL;
			}
		#endif
		return TRUE;
	}
	return FALSE;
}

StakeOutMap::StakeOutMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "StakeOut";
	init (theX, theY, theWidth, theHeight);
}

int StakeOutMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			cutToRoom( new Rm50000 );
		#endif
    	return TRUE;
	}
	return FALSE;
}

KneecapMap::KneecapMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Kneecap";
	init (theX, theY, theWidth, theHeight);
}

int KneecapMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
	 		curRoom->setScript( new  MapExit1000( 390, 424, new Rm37000 ) );
		#endif
    	return TRUE;
	}
	return FALSE;
}

LubyankaMap::LubyankaMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Lubyanka";
	init (theX, theY, theWidth, theHeight);
}

int LubyankaMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			if( availMoscowRegions == MOS_LUB_BIRD )
				curRoom->setScript( new  MapExit1000( 300, 329, new Rm48000 ) );
			else  {
			//	ASSERT( availMoscowRegions == MOS_LUB, 0 );
				curRoom->setScript( new  MapExit1000( 500, 529, new Rm48000 ) );
			}
		#endif
		return TRUE;
	}
	return FALSE;
}

EmbassyMap::EmbassyMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Embassy";
	init (theX, theY, theWidth, theHeight);
}

int EmbassyMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		MapFeature::doVerb( theVerb );
		rm1000->cleanUp();
		rm1000->showRegion( MAP_STATION_EXT );
		return TRUE;
	}
	return FALSE;
}

EmbassyIntMap::EmbassyIntMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "EmbassyIntMap";
	init (theX, theY, theWidth, theHeight);
}

int EmbassyIntMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		MapFeature::doVerb( theVerb );
		if( GameFlag.test( fYingCaptured ) && !GameFlag.test( fWahKa101 ) ) {
			GameFlag.set( fWahKa101 );
			cutToRoom( new Rm36000 );	// meet Max at the door
			curMap = MAP_STATION_INT;	// (comes after cut to allow for proper cleanup)
		}
		else if( GameFlag.test( fMetMaxFoster ) ) {  // usual, go to lobby
			rm1000->cleanUp();
			sfxSystemCursor(SYS_WAIT);
	  		curMap = MAP_STATION_INT;
			rm1000->showRegion( curMap );
		}
		else {	// first time go meet Max in Lobby sort of
			cutToRoom( new Rm36000 );	// meet Max at the door
			curMap = MAP_STATION_INT;	// (comes after cut to allow for proper cleanup)
		}
		/* extra game check */
		if( GameFlag.test( fDCK11 ) )
		{
		 	GameFlag.clear( fPlayerTurnsProcat );
			GameFlag.clear( fWahKa79 );
		}	// in case backing out of Procat van choice
		return TRUE;
	}
	return FALSE;
}

AlleyMap::AlleyMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Alley";
	init (theX, theY, theWidth, theHeight);
}

int AlleyMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			sfxSystemCursor(SYS_WAIT);
			if( availStationExtRegions == STAEXT_LOBBY_ALLEY )
				curRoom->setScript( new MapExit1000( 745, 774, new Rm32000) );
			else {
			//	ASSERT( availStationExtRegions == STAEXT_MAX_ALLEY, 0 );
				curRoom->setScript( new MapExit1000( 670, 699, new Rm32000) );
			}
		#endif
    	return TRUE;
	}
	return FALSE;
}

ProcatVanMap::ProcatVanMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Procat Van";
	init (theX, theY, theWidth, theHeight);
}

int ProcatVanMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			//GameFlag.set( fPlayerTurnsProcat );
			//GameFlag.set( fWahKa79 );
			GameFlag.clear( fDCK11 ); // used to clear fPlayerTurnsProcat and fWahKa79
			MapFeature::doVerb( theVerb );
			curRoom->setScript( new  MapExit1000( 850, 879, new Rm38000) );
		#endif
    	return TRUE;
	}
	return FALSE;
}

MaxOfficeMap::MaxOfficeMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Max's Office";
	init (theX, theY, theWidth, theHeight);
}

int MaxOfficeMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		//GameFlag.set( fBeenToMoscow ); use fMetMaxFoster
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			if( curMap == MAP_STATION_INT ) 
				curRoom->setScript( new  MapExit1000( 970, 999, new Rm36000 ) );
			else if( curMap == MAP_STATION_EXT )	// warn Max special case
				cutToRoom( new Rm35700 );
			//else
			//	ASSERT( FALSE, 0 );
		#endif
    	return TRUE;
	}
	return FALSE;
}

// Russia
ThornOfficeMap::ThornOfficeMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Thorn's Office";
	init(theX, theY, theWidth, theHeight);
}

// Russia
int ThornOfficeMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			curRoom->setScript(new  MapExit1000( 1000, 1029, new Rm35000));
		#endif
    	return TRUE;
	}
	return FALSE;
}

InterrogationRoomMap::InterrogationRoomMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Interrogation Room";
	init(theX, theY, theWidth, theHeight);
}

int InterrogationRoomMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			curRoom->setScript(new  MapExit1000( 940, 969, new Rm23000));
		#endif
    	return TRUE;
	}
	return FALSE;
}

BullpenMap::BullpenMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Bullpen";
	init(theX, theY, theWidth, theHeight);
}

int BullpenMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			curRoom->setScript(new  MapExit1000( 910, 939, new Rm25000));
		#endif
    	return TRUE;
	}
	return FALSE;
}

WashingtonMap::WashingtonMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Washington";
	init (theX, theY, theWidth, theHeight);
}

// not used: no World level
int WashingtonMap::doVerb ( int theVerb )
{
//	sfxPrintf( "How'd you get here?" );
	if (( theVerb == DO_V ) && (curMap != MAP_WASHINGTON)) {
		MapFeature::doVerb( theVerb );
		verifyDisc( USADISC );
		rm1000->cleanUp();
		sfxSystemCursor(SYS_WAIT);
		curMap = MAP_WASHINGTON;
		rm1000->showRegion(curMap);
		return TRUE;
	}
	return FALSE;
}

Farm::Farm(int theX, int theY, int theWidth, int theHeight)
{
  	name = "Farm";
	init (theX, theY, theWidth, theHeight);
}
int Farm::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		MapFeature::doVerb( theVerb );
		rm1000->cleanUp();
		rm1000->showRegion( MAP_FARM );
		return TRUE;
	}
	return FALSE;
}

Langley::Langley(int theX, int theY, int theWidth, int theHeight)
{
  	name = "Langley";
	init (theX, theY, theWidth, theHeight);
}

int Langley::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_USA
			MapFeature::doVerb( theVerb );
			rm1000->cleanUp();
			rm1000->showRegion( MAP_LANGLEY );
		#endif
    	return TRUE;
	}
	return FALSE;
}

ColbyHouse::ColbyHouse(int theX, int theY, int theWidth, int theHeight)
{
	name = "Colby";
	init (theX, theY, theWidth, theHeight);
}

int ColbyHouse::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_USA
			MapFeature::doVerb( theVerb );
			curRoom->setScript(new  MapExit1000( 225, 264, new Rm10200 ));
		#endif
    	return TRUE;
	}
	return FALSE;
}

LondonMap::LondonMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "London";
	init (theX, theY, theWidth, theHeight);
}

int LondonMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_LONDON
			MapFeature::doVerb( theVerb );
			rm1000->cleanUp();	// has to come before change of curMap
			curMap = MAP_LONDON;	// set things up for verifyDisc() * startMidi()
			verifyDisc( USADISC );
			sfxSystemCursor( SYS_WAIT );
			curRoom->setScript( new  MapExit1000( 1065, 1094, new Rm1090 ) );
		#endif
	}
	return FALSE;
}

TunisiaMap::TunisiaMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "tunisia";
	init (theX, theY, theWidth, theHeight);
}

int TunisiaMap::doVerb( int theVerb )
{
	if ( theVerb == DO_V )  {
		#ifdef INCLUDE_EUROPE
			MapFeature::doVerb( theVerb );
			verifyDisc( ENDGAMEDISC );
			cutToRoom( new Rm1070 );		
			curMap = MAP_MOSCOWPLATE;	// (comes after cut to allow for proper cleanup)
			//curDisc = EURODISC;
		#endif
	    return TRUE;
	}
	return FALSE;
}

AssSuiteMap::AssSuiteMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Assassination Suite";
	init (theX, theY, theWidth, theHeight);
}

int AssSuiteMap::doVerb( int theVerb )
{
	if( theVerb == DO_V )  {
		#ifdef INCLUDE_EUROPE
			GameFlag.set( fPlayerTurnsProcat );	// previously cleared in TL
			MapFeature::doVerb( theVerb );
			rm1000->cleanUp();	// has to come before change of curMap
			curMap = MAP_ASSASS;	// set things up for verifyDisc() * startMidi()
			verifyDisc( ENDGAMEDISC );
			cutToRoom( new Rm96000 );
		#endif
	    return TRUE;
	}
  return FALSE;
}

DachaMap::DachaMap(int theX, int theY, int theWidth, int theHeight)
{
	name = "Dacha";
	init (theX, theY, theWidth, theHeight);
}

int DachaMap::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		#ifdef INCLUDE_DACHA
			MapFeature::doVerb( theVerb );
			verifyDisc( ENDGAMEDISC );
			curRoom->setScript( new MapExit1000( 1245, 1274, new Rm79000 ) );
		#endif
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	MoviePlayer1000
 *
/*******************************/

void MoviePlayer1000::movieDelayPlay( char* theMovie, Object* whoToCue,
     int wait, int close, int erase)
{
	myMovie = theMovie;	// note: assumes static char pointer
	myWhoToCue = whoToCue;
	myWait = wait;
	myClose = close;
	myErase = erase;
	timer = new Timer;
	timer->setTicks( 2, this );
}

void MoviePlayer1000::cue( void )
{
	//ASSERT( myErase == FALSE, 0 );
 	theMovie->play( myMovie, myWhoToCue, myWait, myClose, myErase );
	//#pragma message( "temp test" )
	//	if( rm1000->musicHandle == -1 )
	//		rm1000->startMidi();
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

MusicFadeOut1000::MusicFadeOut1000( int handle, long msec )
{
	name = "musicFadeOut1000";
	myHandle = handle;
	myDelay = msec;
	fadeObj = new PermListObject;
	fadeObj->setScript( this );
}

void MusicFadeOut1000::changeState ( int newState )
{
 #define ZEROVOL	0

	switchTo
		//if( myHandle != -1 )
			//sfxSetMusicVolume( myHandle, ZEROVOL, myDelay ); // fade (if not FM card)
			// note: we had problems with FM cards hanging on sustained notes.  removed fade
		seconds = myDelay / 1000 +1;	
	END

	BEG
		fadeObj->dispose();	// deletes fadeObj and this script
		if( myHandle != -1 )
			sfxStopMusic( myHandle );// have to do this here too to beat the midi start
		//							 //  in 1000::init (quick reentry case)
		myHandle = -1;			  
		midiFade = NULL;
	END
}

MusicFadeOut1000::~MusicFadeOut1000()
{
	// doing this here so that restores work during fades:
	// second state of script is not called on restore--the script
	// is just deleted.
	if( myHandle != -1 ) {
		sfxStopMusic( myHandle );
		midiFade = NULL;	
	}
}

MapExit1000::MapExit1000(int theFrom, int theTo, Room* theRoom)
{
	name = "MapExit1000";
	from = theFrom;
	to = theTo;
	nextRoom = theRoom;
}

void MapExit1000::changeState ( int newState )
{
	switchTo
		rm1000->cleanUp();
		user->canInput( FALSE );
		theMovie->fromTo( from, to );
		if( curDisc == USADISC )
			rm1000->mPlay->movieDelayPlay( "1000.avi", this, FALSE, TRUE, FALSE );	
		else if( curDisc == EURODISC )
			rm1000->mPlay->movieDelayPlay( "1001.avi", this, FALSE, TRUE, FALSE );
		else
			rm1000->mPlay->movieDelayPlay( "1002.avi", this, FALSE, TRUE, FALSE );
	END

	BEG
		sfxSystemCursor(SYS_WAIT);
		theGame->newRoom( nextRoom );
	END
}


void LenaCallsScript1000::changeState( int newState )
{
	static Sound*	sndptr;
	static int		mNumber;

	switchTo
		user->canInput( FALSE );
		theMovie->caller = 0;
		theMovie->stop();
		#if TRAVELMIDI == ON
		  if( rm1000->musicHandle != -1 )
			rm1000->stopMidi();
		#endif
		mNumber = rm1000->musicNumber;
		ticks = 40;
	END

	BEG
		sndptr = new Sound;
		sndptr->play( 35108, this );
	END

	BEG
		sndptr->dispose();
		#if TRAVELMIDI == ON
		  rm1000->startMidi( mNumber );
		#endif
		sfxLoadRes( 960, RES_ATS );
		user->canInput( TRUE );
		dispose();
	END
}

// poll for dacha avail.
void MoscowPlateScript1000::changeState( int newState )
{
 	switchTo
		if( GameFlag.test( fDachaAvailable ) ) {
			new CityBt1000( LEFTBT, MAP_CRIMEA );
			if( !GameFlag.test( fDCK9 ) )
			{
				new VLink( "Yuri's waiting for you: DCI Sterling, Peter R.", "707.avi" );
				new EMail( "Churbanov: DCI Sterling, Peter R.", 98215 ); 
				GameFlag.set( fDCK9 );
			}
			dispose();
		}
		else
			seconds = 1;			// poll interval
	END

	BEG
		client->script = NULL;
		seconds = 0;
		state = -1;
		client->setScript( this );	// reset	
	END
}
 
} // namespace Spycraft 
