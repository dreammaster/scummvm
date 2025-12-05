#ifndef SPYCRAFT_LOGIC_1000_H
#define SPYCRAFT_LOGIC_1000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "timer.h"
#include "plistobj.h"


enum 
{
	MAP_WORLD,		// you can't be at this level
	MAP_CITY,		// you can't be at this level
	MAP_WASHINGTON,
	MAP_LONDON, 
	MAP_MOSCOW,
	MAP_TUNISIA,
	MAP_HEIDELBERG,
	MAP_REFINERY,
	MAP_CRIMEA,
	MAP_FARM,
	MAP_LANGLEY,
	MAP_COLBY,
	MAP_STATION_INT,
	MAP_STATION_EXT,
	MAP_ASSASS,
	MAP_YASEVNO,
	MAP_BIRDSONG,
	MAP_STAKEOUT,
	MAP_KNEECAP,
	MAP_MOSCOWPLATE	// Moscow with no options, third disc, going to dacha (button)
};

enum
{
	POSN_NONE,
	POSN_L2,
	POSN_R2,
	POSN_T3,
	POSN_BL3,
	POSN_BR3,
	POSN_TL4,
	POSN_TL4X,	// error in movie
	POSN_TR4,
	POSN_BL4,
	POSN_BR4,
	POSN_BR4X	// error in movie
};

class MapTitle : public Object
{
 public:
	MapTitle();
	void set( int theView, char* theName, int thePosn, int theLoop, int theCell );
	void reset( void );
	int		posn;	// one of enum POSN_...
	View*	title;
};

class MoviePlayer1000 : public Object
{
 public:
	void movieDelayPlay( char* theMovie, Object* whoToCue, int wait, int close, int erase);
	void cue( void );
	Timer*  timer;
	char*	myMovie;
	Object*	myWhoToCue;
	int		myWait;
	int		myClose;
	int		myErase;
};

/** city triad **/

class CityTitle1000: public View
{
 public:
 	CityTitle1000();
	void dispose( void );
	void update( int city );
	View*	title;
};

class CityBt1000 : public View
{
 public:
	CityBt1000( int side, int city );
	int doVerb( int );
	int verifyDestinationDisc( int destination );
	int		myLoop;
	int		myCity;
};

class BackBt1000 : public View
{
 public:
	BackBt1000();
	int doVerb( int );
};

class CityTriad1000 : public Object
{
 public:
 	CityTriad1000();
	void dispose( void );
	void update( void );
	void calcTriad( int local, int& here, int& left, int& right );
	CityTitle1000*	topTitle;
	View*			wingThing;
	BackBt1000*		backBt;	
	CityBt1000*		lBt;
	CityBt1000*		rBt;
};	

class Rm1000 : public Room
{
public:
	int from;
	int to;
	int loopFrom;
	int loopTo;
	int seq2From;
	int seq2To;
	int doSeq2;
	Rm1000();
	~Rm1000();
	void init ( void );
	void fromTo( int theFrom, int theTo);
	void seq2(int theFrom, int theTo);
	void cue ( void );
	void showRegion( int theRegion );
	void cleanUp( void );
	void queryRegion( int theRegion ); 
	void drawTitle( MapTitle* theTitle );
	void checkWashington( void );
	void checkLangley( void );
	void checkMoscow( void );
	void checkStationExt( void );
	void checkStationInt( void );
	void startMidi( void );
	void startMidi( int number );
	void stopMidi( void );
	int findMidiTableRow( int location );
	CityTriad1000*	city;
	MapTitle		title1;
	MapTitle		title2;
	MapTitle		title3;
	MapTitle		title4;
	MoviePlayer1000*	mPlay;
	int  	musicHandle;
	int		musicNumber;
};

/* features */
class MapBack : public Feature
{
public:
	MapBack();
	int doVerb ( int );
};

/* map features */
class MapFeature : public Feature
{
public:
	void verifyDisc( int theDisc );
	void cutToRoom( Room* theRoom );
	virtual int doVerb ( int );
};

class TunisiaMap : public MapFeature
{
public:
	TunisiaMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class MapFrank : public MapFeature
{
public:
	MapFrank(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class IAMap : public MapFeature
{
public:
	IAMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class HoltMap : public MapFeature
{
public:
	HoltMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class JaimieMap : public MapFeature
{
public:
	JaimieMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class WashPlayerMap : public MapFeature
{
public:
	WashPlayerMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class DCIMap : public MapFeature
{
public:
	DCIMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class ZoneMap : public MapFeature
{
public:
	ZoneMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class StakeOutMap : public MapFeature
{
public:
	StakeOutMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class KneecapMap : public MapFeature
{
public:
	KneecapMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class HeidelbergMap : public MapFeature
{
public:
	HeidelbergMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class HotelMap : public MapFeature
{
public:
	HotelMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class RefineryShooterMap : public MapFeature
{
public:
	RefineryShooterMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class RefineryDialogMap : public MapFeature
{
public:
	RefineryDialogMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class CrimeaMap : public MapFeature
{
public:
	CrimeaMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class MoscowMap : public MapFeature
{
public:
	MoscowMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class EmbassyIntMap : public MapFeature
{
public:
	EmbassyIntMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class AlleyMap  : public MapFeature
{
public:
	AlleyMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class ProcatVanMap  : public MapFeature
{
public:
	ProcatVanMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class MaxOfficeMap : public MapFeature
{
public:
	MaxOfficeMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class ThornOfficeMap  : public MapFeature
{
public:
	ThornOfficeMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class InterrogationRoomMap  : public MapFeature
{
public:
	InterrogationRoomMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class BullpenMap  : public MapFeature
{
public:
	BullpenMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class VilniusCenterMap : public MapFeature
{
public:
	VilniusCenterMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class BirdsongMap : public MapFeature
{
public:
	BirdsongMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class SchlumpfenMap : public MapFeature
{
public:
	SchlumpfenMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class LubyankaMap : public MapFeature
{
public:
	LubyankaMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class EmbassyMap : public MapFeature
{
public:
	EmbassyMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class LondonMap : public MapFeature
{
public:
	LondonMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class WashingtonMap : public MapFeature
{
public:
	WashingtonMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
	void checkWashington( void );
};

class Farm : public MapFeature
{
public:
	Farm(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class Langley : public MapFeature
{
public:
	Langley(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class ColbyHouse : public MapFeature
{
public:
	ColbyHouse(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class DachaMap : public MapFeature
{
public:
	DachaMap(int theX, int theY, int theWidth, int theHeight);
	int doVerb ( int );
};

class AssSuiteMap : public MapFeature
{
 public:
	AssSuiteMap( int theX, int theY, int theWidth, int theHeight );
	int doVerb ( int );
};

/* scripts */
class MapExit1000 : public Script
{
public:
	int from;
	int to;						 
	Room* nextRoom;

	MapExit1000(int theFrom, int theTo, Room* theRoom);
	void changeState ( int );
};

class MusicFadeOut1000 : public Script
{
public:
	MusicFadeOut1000( int handle, long msec );
	~MusicFadeOut1000();
	void changeState ( int );
	int  myHandle;
	long myDelay;
	PermListObject* fadeObj;	
};

class LenaCallsScript1000 : public Script
{
 public:
 	LenaCallsScript1000() { name = "LenaCallsScript1000"; };
	void changeState( int );
};

class MoscowPlateScript1000 : public Script
{
 public:
 	MoscowPlateScript1000() { name = "MoscowPlateScript1000"; };
	void changeState( int );
}; 

extern short curMap;
extern short drawBackground;	// set to FALSE to suppress TL drawPic, may be temp
extern Rm1000* rm1000;

#endif

