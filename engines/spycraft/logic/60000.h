/*********************************************
* Header file.
* Death screen.  Game over man.
* Copyright Activision 1995
* Started by Ka Wah 
* Continued by:  Daniel Kinzek   November, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_60000_H
#define SPYCRAFT_LOGIC_60000_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

enum { END_NONE, END_JAIL, END_DEAD, END_BLACKDEAD, END_FIRED, END_GAME };// for endType global

class Rm60000 : public Room
{					
 public:
 	Rm60000();	// shouldn't use this one
	Rm60000( int endType );
	~Rm60000();
	void init ( void );
	int	theEnd;	// one of enum
};

class Rm60500 : public Room
{					
 public:
 	Rm60500();
	~Rm60500();
	void init( void );
};

/* views */
class ButtonView60000 : public View
{
 public:
	ButtonView60000();
	//int pressed;
	int handleEvent( MADEEventStamp * );
	virtual void activate( void ) {};
};
// 60000
class RestoreView60000 : public ButtonView60000
{
 public:
	RestoreView60000();
  	void respond( void );
	//int doVerb( int );
	void activate( void );
};

class QuitView60000 : public ButtonView60000
{
 public:
	QuitView60000();
  	void respond( void );
	//int doVerb( int );
	void activate( void );
};

// 60500
class PlayBt60500 : public ButtonView60000
{
 public:
	PlayBt60500();
  	void respond( void );
	//int doVerb( int );
	void activate( void );
};

class RestoreBt60500 : public ButtonView60000
{
 public:
	RestoreBt60500();
  	void respond( void );
	//int doVerb( int );
	void activate( void );
};

class QuitBt60500 : public ButtonView60000
{
 public:
	QuitBt60500();
  	void respond( void );
	//int doVerb( int );
	void activate( void );
};

/* scripts */
class JailScript60000 : public Script
{
 public:
 	JailScript60000() { name = "JailScript60000"; };
	void changeState( int );
};

class FiredScript60000 : public Script
{
 public:
 	FiredScript60000() { name = "FiredScript60000"; };
	void changeState( int );
};

class DeadScript60000 : public Script
{
 public:
 	DeadScript60000() { name = "DeadScript60000"; };
	void changeState( int );
};

class EndGameScript60000 : public Script
{
 public:
 	EndGameScript60000() { name = "EndGameScript60000"; };
	void changeState( int );
};

#endif

