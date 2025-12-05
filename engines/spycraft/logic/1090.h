//Started by Tom DeSalvo
//Continued by Daniel Kinzek
//Blake Est. Shot

#ifndef SPYCRAFT_LOGIC_1090_H
#define SPYCRAFT_LOGIC_1090_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Rm1090 : public Room
{
public:
	Rm1090();
	void init ( void );
};

class Rm1091 : public Room
{
public:
	Rm1091();
	void init ( void );
};

class NorthExit1090 : public ExitFeature
{
public:
	NorthExit1090();
  	void respond ( void );
	int doVerb ( int );
};

class SouthExit1090 : public ExitFeature
{
public:
	SouthExit1090();
  	void respond ( void );
	int doVerb ( int );
};

class EastExit1090 : public ExitFeature
{
public:
	EastExit1090();
  	void respond ( void );
	int doVerb ( int );
};

class SouthExit1091 : public ExitFeature
{
public:
	SouthExit1091();
  	void respond ( void );
	int doVerb ( int );
};

class EastExit1091 : public ExitFeature
{
public:
	EastExit1091();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class DoorBell_1091 : public Feature
{
public:
	DoorBell_1091();
	void respond( void );
	int doVerb( int );
};

/* scripts */
//class ExitScript1090 : public Script
//{
// public:
// 	ExitScript1090() { name = "ExitScript1090"; };
//	void changeState( int );
//};

class DoorbellScript1091 : public Script
{
 public:
 	DoorbellScript1091() { name = "DoorbellScript1091"; };
	void changeState( int );
};

class HelloScript1091 : public Script
{
 public:
 	HelloScript1091() { name = "HelloScript1091"; };
	void changeState( int );
};

class ISaidHelloScript1091 : public Script
{
 public:
 	ISaidHelloScript1091() { name = "ISaidHelloScript1091"; };
	void changeState( int );
};

#endif

