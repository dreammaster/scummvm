/***************************
*
* file 1270.h                         
* Washington Monument        
* Copyright Activision 1995 
* Started by:  Tom DeSalvo
* Continued by:	Daniel C. Kinzek 
*                           
****************************/

#ifndef SPYCRAFT_LOGIC_1270_H
#define SPYCRAFT_LOGIC_1270_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Newspaper1270;

class Rm1270 : public Room
{
public:
	Rm1270();
	~Rm1270();
	void init ( void );
	void cue  ( void );
};

/* scripts */
class NewspaperScript1270 : public Script
{
public:
	NewspaperScript1270() {name = "NewspaperScript1270";};
	void changeState ( int );
};



#endif

