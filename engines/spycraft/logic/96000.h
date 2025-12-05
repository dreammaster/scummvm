/*********************************************
* Header file.
* Procat player assasination of president
* Rooms: 96000, 96100, 96200, ...        
* Copyright Activision 1995
* Started by Daniel Kinzek   September, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_96000_H
#define SPYCRAFT_LOGIC_96000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"
#include "movie.h"

// states for 96300
enum{ COVEREDFLOOR96300, OPENEDFLOOR96300, CASECLOSED96300, CASEOPENED96300, PEGOUT96300 };

/* rooms */
class Rm96000 : public Room
{
public:
	Rm96000();
	void init();
};

class Rm96100 : public Room
{
public:
	Rm96100();
	void init();
};

class Rm96200 : public Room
{
public:
	Rm96200();
	void init();
};

class Rm96300 : public Room
{
public:
	Rm96300();
	~Rm96300();
	void init();
	void update( void );
};

class Rm96700 : public Room
{
public:
	Rm96700();
	void init();
};

/* exit features */
// 96000
class NorthExit96000 : public ExitFeature
{
public:
	NorthExit96000();
	void respond( void );
	int doVerb( int );
};
// 96100
class NorthExit96100 : public ExitFeature
{
public:
	NorthExit96100();
	void respond( void );
	int doVerb( int );
};

class WestExit96100 : public ExitFeature
{
public:
	WestExit96100();
	void respond( void );
	int doVerb( int );
};
// 96200
class NorthExit96200 : public ExitFeature
{
public:
	NorthExit96200();
	void respond( void );
	int doVerb( int );
};

class EastExit96200 : public ExitFeature
{
public:
	EastExit96200();
	void respond( void );
	int doVerb( int );
};
// 96300
class WestExit96300 : public ExitFeature
{
public:
	WestExit96300();
	void respond( void );
	int doVerb( int );
};

class EastExit96300 : public ExitFeature
{
public:
	EastExit96300();
	void respond( void );
	int doVerb( int );
};

/* features */
class Floor96300 : public Feature
{
public:
	Floor96300();
	void respond( void );
	int doVerb( int );
};

class ClosedCase96300 : public Feature
{
public:
	ClosedCase96300();
	void respond( void );
	int doVerb( int );
};

class OpenedCase96300 : public Feature
{
public:
	OpenedCase96300();
	void respond( void );
	int doVerb( int );
};

class PEG96300 : public Feature
{
public:
	PEG96300();
	void respond( void );
	int doVerb( int );
};

class OutWindow96300 : public Feature
{
public:
	OutWindow96300();
	void respond( void );
	int doVerb( int );
};

/* views */
class CoveredFloorView96300 : public View
{
 public:
 	CoveredFloorView96300();
	void respond( void );
	int doVerb( int );
};

class PEGTriggerView96300 : public View
{
 public:
 	PEGTriggerView96300();
	~PEGTriggerView96300();
	int handleEvent( MADEEventStamp* );
	void cue( void );
	int doVerb( int );
	int	count;
};

/* props */
class WindowProp96100 : public Prop
{
public:
	WindowProp96100();
	void respond( void );
	int	doVerb( int );
	void cue( void );
};

class WindowProp96300 : public Prop
{
public:
	WindowProp96300();
	void respond( void );
	int	doVerb( int );
	void cue( void );
};

/* scripts */
class GrendelStairsScript96000 : public Script
{
 public:
 	GrendelStairsScript96000() { name = "GrendelStairsScript96000"; };
	void changeState( int );
};

class ExitScript96200 : public Script
{
 public:
 	ExitScript96200() { name = "ExitScript96200"; };
	void changeState( int );
};

class PEGUpWindowScript96300 : public Script
{
 public:
 	PEGUpWindowScript96300() { name = "PEGUpWindowScript96300"; };
	void changeState( int );
};

class OutWindowScript96300 : public Script
{
 public:
 	OutWindowScript96300() { name = "OutWindowScript96300"; };
	void changeState( int );
};

class ShootPresidentScript96300 : public Script
{
 public:
 	ShootPresidentScript96300() { name = "ShootPresidentScript96300"; };
	void changeState( int );
};

class AssemblePEGScript96700 : public Script
{
 public:
 	AssemblePEGScript96700() { name = "AssemblePEGScript96700"; };
	void changeState( int );
};

#endif
