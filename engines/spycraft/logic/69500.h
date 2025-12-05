//Started by Kawah Wong
//Hotel Schlump - round table, left and ahead

#ifndef SPYCRAFT_LOGIC_69500_H
#define SPYCRAFT_LOGIC_69500_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm69500 : public Room
{					
public:
	Rm69500();
	//~Rm69500();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit69500 : public SouthExit
{
public:
	SouthExit69500();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class Menu69500 : public Feature
{
public:
	Menu69500();
	void respond( void );
	int doVerb( int );
};

/* insets */
class MenuInset69500: public Inset
{
 public:
	MenuInset69500();
//	void respond();
};

#endif

