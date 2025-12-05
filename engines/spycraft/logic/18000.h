#ifndef SPYCRAFT_LOGIC_18000_H
#define SPYCRAFT_LOGIC_18000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
//#include "globals.h"

class Rm18000 : public Room
{
public:
	Rm18000();
	void init ( void );
};

class SouthExit18000 : public ExitFeature
{
public:
	SouthExit18000();
  	void respond ( void );
	int doVerb ( int );
};

class DCI18000 : public TextFeature
{
public:
	DCI18000();
	int doVerb ( int );
};

/*class ScriptBrief18000 : public Script
{
public:
	ScriptBrief18000() {name = "ScriptBrief18000";};
	void changeState ( int );
};*/

class FullBriefScript18000 : public Script
{
public:
	FullBriefScript18000() {name = "FullBriefScript18000";};
	void changeState ( int );
};


class PEGBriefScript18000 : public Script
{
public:
	PEGBriefScript18000() { name = "PEGBriefScript18000"; };
	void changeState( int );	
};

class FiredScript18000 : public Script
{
public:
	FiredScript18000() { name = "FiredScript18000"; };
	void changeState( int );	
};

class BadScript18000 : public Script
{
public:
	BadScript18000() { name = "BadScript18000"; };
	void changeState( int );	
};

class GoodScript18000 : public Script
{
public:
	GoodScript18000() { name = "GoodScript18000"; };
	void changeState( int );	
};

class MedalScript18000 : public Script
{
public:
	MedalScript18000() { name = "MedalScript18000"; };
	void changeState( int );	
};

class GoAwayScript18000 : public Script
{
public:
	GoAwayScript18000() { name = "GoAwayScript18000"; };
	void changeState( int );	
};

/* Ending choosing stuff */
struct Endgame		// of course, this whole thing could be an object . . .
{
	int	condition;	// bits specify game state
	int	outcome;
};
enum { ENDFIRED, ENDBAD, ENDGOOD, ENDMEDAL, ENDNUM }; // outcomes
#define PS	 1			/* Pit Stopped		*/
#define HK	 (PS << 1)	/* Harmonica Killed */
#define YS	 (HK << 1)	/* Yuri Shot		*/
#define ENDTABLESIZE 8 /* 2^number of conditions (PS, HK, YS) */
int	chooseEnding( void );


#endif

