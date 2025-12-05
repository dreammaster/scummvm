//alley next to station house
//Started by Tom DeSalvo
//Continued by Ka Wah
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_32000_H
#define SPYCRAFT_LOGIC_32000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "timer.h"
#include "plistobj.h"

class Rm32000 : public Room
{
public:
	Rm32000();
	~Rm32000();
	void init ( void );
	void cue ( void );
	int handleEvent ( MADEEventStamp * );
};

class NorthExit32000 : public ExitFeature
{
public:
	NorthExit32000();
	void cue ( void );
  	void respond ( void );
	int doVerb ( int );
};

class SouthExit32000 : public ExitFeature
{
public:
	SouthExit32000();
  	void respond ( void );
	int doVerb ( int );
};

/* script */
class EnterScript32001 : public Script
{
 public:
 	EnterScript32001(); 
	void changeState( int );
};

class EnterScript32002 : public Script
{
 public:
 	EnterScript32002(); 
	void changeState( int );
};

class EnterScript32003 : public Script
{
 public:
 	EnterScript32003(); 
	void changeState( int );
};

class BackgroundScript32000 : public Script
{
 public:
 	BackgroundScript32000(); 
	void changeState( int );
};

class CloseBackgroundScript320000 : public Script
{
 public:
 	CloseBackgroundScript320000(); 
	void changeState( int );
};

/* dialog */
class HarmDialog : public Dialog
{
public:	
//	Timer* harmTimer32000;

	HarmDialog();
 	void show( void );
	void hide( void );
	void cue( void );
};

class TurnProcat : public DItem
{
public:
	TurnProcat();
	void doit( void );
};

class NoProcat : public DItem
{
public:
	NoProcat();
	void doit( void );
};

class SaveYou : public DItem
{
public:
	SaveYou();
	void doit( void );
};

class HarmKillPlayer : public PermListObject
{
 public:
// 	int	playingVoice;
	HarmKillPlayer();
	void dispose( void );	
};

class HarmDItem : public DItem
{
 public:
	void doit( void );
};

#endif

