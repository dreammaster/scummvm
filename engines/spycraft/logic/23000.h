//Started by Tom DeSalvo
//Debriefing chamber
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_23000_H
#define SPYCRAFT_LOGIC_23000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Rm23000 : public Room
{
public:
	Rm23000();
	~Rm23000();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};
/*
class EastExit23000 : public ExitFeature
{
public:
	EastExit23000();
  	void respond ( void );
	int doVerb ( int );
};
*/
class SouthExit23000 : public ExitFeature
{
public:
	SouthExit23000();
  	void respond ( void );
	int doVerb ( int );
};

class NorthExit23000 : public ExitFeature
{
public:
	NorthExit23000();
  	void respond ( void );
	int doVerb ( int );
};

/* scripts */
class EnterScript23000 : public Script
{
 public:
 	EnterScript23000(); 
	void changeState( int );
};

class EnterBlackScreenScript23001 : public Script
{
 public:
 	EnterBlackScreenScript23001(); 
	void changeState( int );
};
  
/* dialog */
class Dialog4_23000 : public Dialog
{
 	void cue( void );
};

class Dialog31_23000 : public Dialog
{
 	void cue( void );
};

class VlinkMaxConversation : public DItem
{
public:
	VlinkMaxConversation();
	void doit( void );
};

class BrokenYingInterrogation : public DItem
{
public:
	BrokenYingInterrogation();
	void doit( void );
};

class PegInfo : public DItem
{
public:
	PegInfo();
	void doit( void );
};

class PayCheck : public DItem
{
public:
	PayCheck();
	void doit( void );
};

class RestartInterrogation : public DItem
{
public:
	RestartInterrogation();
	void doit( void );
};

class DidPhotoAgain : public DItem
{
public:
	DidPhotoAgain();
	void doit( void );
};

class DidPhotoAgain2 : public DItem
{
public:
	DidPhotoAgain2();
	void doit( void );
};

class EndBrokenYing : public DItem
{
public:
	EndBrokenYing();
	void doit( void );
};

/* room23001 */
class Rm23001 : public Room
{
public:
	Rm23001();
	~Rm23001();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};

class Rm23002 : public Room
{
public:
	Rm23002();
	~Rm23002();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};

class EnterIntercomScript23000 : public Script
{
 public:
 	EnterIntercomScript23000(); 
	void changeState( int );
};

class MovetoBullpen : public DItem
{
public:
	MovetoBullpen();
	void doit( void );
};

class CheckBroken : public DItem
{
public:
	CheckBroken();
	void doit( void );
};

#endif

