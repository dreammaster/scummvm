/******************************
 *		Ext. Tropical Desert 
 *		room 92000
 *		created by: Greg Scharf
 *****************************/ 	

#ifndef SPYCRAFT_LOGIC_92000_H
#define SPYCRAFT_LOGIC_92000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm92000 : public Room
{
public:
	Rm92000();
	~Rm92000();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

/*class SouthExit52000 : public ExitFeature
{
public:
	SouthExit52000();
  	void respond ( void );
	int doVerb ( int );
};
*/

class Script92000 : public Script
{
public:
	Script92000() {name = "Script92000";};
	void changeState ( int );
};
#endif
