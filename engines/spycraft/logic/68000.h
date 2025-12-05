//Started by Tom DeSalvo
//Hotel Schlump Int.

#ifndef SPYCRAFT_LOGIC_68000_H
#define SPYCRAFT_LOGIC_68000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm68000 : public Room
{
public:
	Rm68000();
	~Rm68000();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit68000 : public SouthExit
{
public:
	SouthExit68000();
  	void respond ( void );
	int doVerb ( int );
};

class NorthExit68000 : public ExitFeature
{
public:
	NorthExit68000();
  	void respond ( void );
	int doVerb ( int );
};
/*
class DisguiseKit68000 : public View
{
public:
	DisguiseKit68000();
  	void respond ( void );
	int doVerb ( int );
};

class Suitcase68000 : public View
{
public:
	Suitcase68000();
  	void respond ( void );
	int doVerb ( int );
};

class Closet68000 : public View
{
public:
	Closet68000();
  	void respond ( void );
	int doVerb ( int );
};

class SuitcaseInset : public Inset		
{
public:
	SuitcaseInset();
	void respond();
	int doVerb( int);
};

class Boxers68000 : public InsetFeature
{
public:
	Boxers68000();
	void respond( void );
	int doVerb ( int );
};
		
class Socks68000 : public InsetFeature
{
public:
	Socks68000();
	void respond( void );
	int doVerb ( int );
};

class ClosetInset : public Inset		
{
public:
	ClosetInset();
	void respond();
	int doVerb( int);
};

class Handcuffs68000 : public InsetFeature
{
public:
	Handcuffs68000();
	void respond( void );
	int doVerb ( int );
};

class Casuals68000 : public InsetFeature
{
public:
	Casuals68000();
	void respond( void );
	int doVerb ( int );
};

class Suit68000 : public InsetFeature
{
public:
	Suit68000();
	void respond( void );
	int doVerb ( int );
};

class Bonds68000 : public InsetFeature
{
public:
	Bonds68000();
	void respond( void );
	int doVerb ( int );
};

class PriestOutfit68000 : public InsetFeature
{
public:
	PriestOutfit68000();
	void respond( void );
	int doVerb ( int );
};

class Script68000 : public Script
{
public:
	Script68000() {name = "Script68000";};
	void changeState ( int );
};
*/
#endif
