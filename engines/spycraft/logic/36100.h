/*********************************************
* Header file.
* Foster's office, side desk view.        
* Copyright Activision 1995
* Started by Daniel Kinzek   June, 1995
**********************************************/
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_36100_H
#define SPYCRAFT_LOGIC_36100_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"

class Rm36100 : public Room
{
public:
	Rm36100();
	void init();
};

/* exit features */
class SouthExit36100 : public SouthExit
{
public:
	SouthExit36100();
  void respond( void );
	int doVerb( int );
};

/* features */
class Desk36100 : public Feature
{
public:
	Desk36100();
	void respond( void );
	int doVerb( int );
};

/* text features */
/*
class Photo36100 : public TextFeature
{
public:
	Photo36100();
  	void respond( void );
	int doVerb( int );
};
*/

class Computer36100 : public Feature
{
public:
	Computer36100();
  	void respond( void );
	int doVerb( int );
};

/* insets */
class ComputerInset36100: public Inset
{
 public:
	ComputerInset36100();
//	void respond();
};

/* scripts */
class EnterScript36100 : public Script
{
 public:
 	EnterScript36100(); 
	void changeState( int );
};
#endif
