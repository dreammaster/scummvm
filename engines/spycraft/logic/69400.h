//Started by Kawah Wong
//Hotel Schlump - round table, left and ahead

#ifndef SPYCRAFT_LOGIC_69400_H
#define SPYCRAFT_LOGIC_69400_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm69400 : public Room
{					
public:
	Rm69400();
	//~Rm69400();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit69400 : public SouthExit
{
public:
	SouthExit69400();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class Bond69400 : public Feature
{
public:
	Bond69400();
	void respond( void );
	int doVerb( int );
};

/* insets */
class BondInset69400: public Inset
{
 public:
	BondInset69400();
	void respond( void );
	int doVerb( int );
};

class BondInset69420: public Inset
{
 public:
	BondInset69420();
//	void respond( void );
//	int doVerb( int );
};

#endif

