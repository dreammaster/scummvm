//Started by Tom DeSalvo
//Continued by GS and DCK
//Birdosng Apt. Cluster
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_57000_H
#define SPYCRAFT_LOGIC_57000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "globals.h"

class Rm57000 : public Room
{
public:
	Rm57000();
	~Rm57000();
	void cue();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit57000 : public SouthExit
{
public:
	SouthExit57000();
  	void respond ( void );
	int doVerb ( int );
};

class TopWestExit57000 : public ExitFeature
{
public:
	TopWestExit57000();
  	void respond ( void );
	int doVerb ( int );
};

class BottomWestExit57000 : public ExitFeature
{
public:
	BottomWestExit57000();
  	void respond ( void );
	int doVerb ( int );
};

/*class Bedroom57000 : public TextFeature
{
public:
	Bedroom57000();
	int doVerb ( int );
};

class Kitchen57000 : public TextFeature
{
public:
	Kitchen57000();
	int doVerb ( int );
};

class Bathroom57000 : public TextFeature
{
public:
	Bathroom57000();
	int doVerb ( int );
};

class Office57000 : public TextFeature
{
public:
	Office57000();
	void respond (void);
	int doVerb ( int );
};
*/

/* features */
class StatueAndLamp57000 : public Feature
{
public:
	StatueAndLamp57000();
	void respond( void );
	int doVerb( int );
};

class BookOnTable57000 : public Feature
{
public:
	BookOnTable57000();
	void respond( void );
	int doVerb( int );
};

class BearPhoto57000 : public Feature
{
public:
	BearPhoto57000();
	void respond( void );
	int doVerb( int );
};

class HorseStatue57000 : public Feature
{
public:
	HorseStatue57000();
	void respond( void );
	int doVerb( int );
};

class Calendar57000 : public Feature
{
public:
	Calendar57000();
	void respond( void );
	int doVerb( int );
};

/* insets */
class StatueAndLampInset57000 : public Inset		
{
public:
	StatueAndLampInset57000();
	int doVerb( int );
};

class BookOnTableInset57000 : public Inset		
{
public:
	BookOnTableInset57000();
	int doVerb( int );
};

class BearPhotoInset57000 : public Inset		
{
public:
	BearPhotoInset57000();
	int doVerb( int );
};

class HorseStatueInset57000 : public Inset		
{
public:
	HorseStatueInset57000();
	int doVerb( int );
};

class CalendarInset57000 : public Inset		
{
public:
	CalendarInset57000();
	int doVerb( int );
};

/* scripts */
class EnterScript57000 : public Script
{
 public:
 	EnterScript57000(); 
	void changeState( int );
};

class ArrestPlayerScript57000 : public Script
{
 public:
 	ArrestPlayerScript57000(); 
	void changeState( int );
};

/* dialog */
class BetrayMaxMafia : public DItem
{
public:
	BetrayMaxMafia();
	void doit( void );
};

class BetrayMaxKneecaps : public DItem
{
public:
	BetrayMaxKneecaps();
	void doit( void );
};

class LiveMax : public DItem
{
public:
	LiveMax();
	void doit( void );
};

#endif

