//Started by Kawah Wong
//Hotel Schlump - closet east of interior

#ifndef SPYCRAFT_LOGIC_69200_H
#define SPYCRAFT_LOGIC_69200_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm69200 : public Room
{
public:
	Rm69200();
	//~Rm69200();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class WestExit69200 : public ExitFeature
{
public:
	WestExit69200();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class Doll69200 : public Feature
{
public:
	Doll69200();
	void respond( void );
	int doVerb( int );
};

class Card69200 : public Feature
{
public:
	Card69200();
	void respond( void );
	int doVerb( int );
};


/* insets */
class DollInset69200: public Inset
{
 public:
	DollInset69200();
//	void respond();
};

class CardInset69200: public Inset
{
 public:
	CardInset69200();
//	void respond();
};


#endif

