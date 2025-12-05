//Started by Ka Wah
//Kneecaps Pad - 45 degree turn to left from 37200
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_37300_H
#define SPYCRAFT_LOGIC_37300_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm37300 : public Room
{					
public:
	Rm37300();
	~Rm37300();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class EastExit37300 : public ExitFeature
{
public:
	EastExit37300();
  	void respond ( void );
	int doVerb ( int );
};
/*
class SouthExit37300 : public SouthExit
{
public:
	SouthExit37300();
  	void respond ( void );
	int doVerb ( int );
};
*/
/* features */
class EgyptianPhotos37300 : public Feature
{
public:
	EgyptianPhotos37300();
	void respond( void );
	int doVerb( int );
};

/* insets */
class EgyptianPhotosInset37300: public Inset
{
 public:
	EgyptianPhotosInset37300();
//	void respond();
};
 
#endif

