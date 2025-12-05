//Started by Tom DeSalvo
//Procat van interior
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_38000_H
#define SPYCRAFT_LOGIC_38000_H

//#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm38000 : public Room
{
public:
	Rm38000();
	~Rm38000();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit38000 : public SouthExit
{
public:
	SouthExit38000();
  	void respond ( void );
	int doVerb ( int );
};

/* feature */
class InsideVan38000 : public Feature
{
public:
	InsideVan38000();
	void respond ( void );
	int doVerb ( int );
};

class Rm38100 : public Room
{
public:
	Rm38100();
	~Rm38100();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit38100 : public SouthExit
{
public:
	SouthExit38100();
  	void respond ( void );
	int doVerb ( int );
};

class FolderHole38100 : public Feature
{
public:
	FolderHole38100();
	void respond ( void );
	int doVerb ( int );
};

class HandOverFileScript38100 : public Script
{
 public:
 	HandOverFileScript38100(); 
	void changeState( int );
};

class Rm38200 : public Room
{
public:
	Rm38200();
	~Rm38200();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

/* feature */
class VanHole38100 : public Feature
{
public:
	VanHole38100();
	void respond ( void );
	int doVerb ( int );
};

/* scripts */
class ProcatTeamScript38100 : public Script
{
 public:
 	ProcatTeamScript38100(); 
	void changeState( int );
};

class StupidThornScript38100 : public Script
{
 public:
 	StupidThornScript38100(); 
	void changeState( int );
};

class Rm38300 : public Room
{
public:
	Rm38300();
	~Rm38300();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit38300 : public SouthExit
{
public:
	SouthExit38300();
  	void respond ( void );
	int doVerb ( int );
};

class PegBullet38300 : public View
{
public:
	PegBullet38300();
  	void respond ( void );
	int doVerb ( int );
};

class PegBulletInset38300: public Inset
{
 public:
	PegBulletInset38300();
//	void respond();
};

class Rm38400 : public Room
{
public:
	Rm38400();
	~Rm38400();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit38400 : public SouthExit
{
public:
	SouthExit38400();
  	void respond ( void );
	int doVerb ( int );
};

class NorthExit38400 : public ExitFeature
{
public:
	NorthExit38400();
  	void respond ( void );
	int doVerb ( int );
};

class Rm38500 : public Room
{
public:
	Rm38500();
	~Rm38500();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

#endif

