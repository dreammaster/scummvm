//Started by Tom DeSalvo
//Foster's office
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_36000_H
#define SPYCRAFT_LOGIC_36000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"
#include "list.h"
#include "plistobj.h"

class Rm36000 : public Room
{
public:
	Rm36000();
	~Rm36000();
	void cue();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class NorthExit36000 : public ExitFeature
{
public:
	NorthExit36000();
  	void respond ( void );
	int doVerb ( int );
};

class SouthExit36000 : public SouthExit
{
public:
	SouthExit36000();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class Camera36000 : public Feature
{
public:
	Camera36000();
  	void respond( void );
	int doVerb( int );
};

/* insets */
class CameraInset36000: public Inset
{
 public:
	CameraInset36000();
//	void respond();
};

/* scripts */
class BackgroundScript36000 : public Script
{
 public:
 	BackgroundScript36000(); 
	void changeState( int );
};

class MinuteScript36000 : public Script
{
 public:
 	MinuteScript36000(); 
	void changeState( int );
};

/* props */
class MinuteMax36000 : public Prop
{
public:
	MinuteMax36000();
	void cue( void );
};

/*class ProcatTimer : public PermListObject
{
 public:
	ProcatTimer();
	void dispose( void );	
};*/
 
#endif

