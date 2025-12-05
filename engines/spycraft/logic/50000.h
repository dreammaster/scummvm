//Started by Tom DeSalvo
//Surveilled Apt. Birdsong
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_50000_H
#define SPYCRAFT_LOGIC_50000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm50000 : public Room
{
public:
	Rm50000();
	~Rm50000();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit50000 : public SouthExit
{
public:
	SouthExit50000();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class BloodyCamera50000 : public Feature
{
public:
	BloodyCamera50000();
	void respond( void );
	int doVerb( int );
};

class Blanket50000 : public Feature
{
public:
	Blanket50000();
	void respond( void );
	int doVerb( int );
};

class Garbage50000 : public Feature
{
public:
	Garbage50000();
	void respond( void );
	int doVerb( int );
};

class JunkyWall50000 : public Feature
{
public:
	JunkyWall50000();
	void respond( void );
	int doVerb( int );
};

/* insets */
class BloodyCameraInset50000: public Inset
{
 public:
	BloodyCameraInset50000();
//	void respond();
};

class BlanketInset50000: public Inset
{
 public:
	BlanketInset50000();
//	void respond();
};

class GarbageInset50000: public Inset
{
 public:
	GarbageInset50000();
//	void respond();
};

class JunkyWallInset50000: public Inset
{
 public:
	JunkyWallInset50000();
//	void respond();
};

/* script */
class EnterScript50000 : public Script
{
 public:
 	EnterScript50000(); 
	void changeState( int );
};

class EnterBlackScreenScript50300 : public Script
{
 public:
 	EnterBlackScreenScript50300(); 
	void changeState( int );
};

class Rm50001 : public Room
{
public:
	Rm50001();
	~Rm50001();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit50001 : public SouthExit
{
public:
	SouthExit50001();
  	void respond ( void );
	int doVerb ( int );
};

class CameraChip50001 : public View
{
public:
	CameraChip50001();
  	void respond ( void );
	int doVerb ( int );
};

class Rm50100 : public Room
{
public:
	Rm50100();
	~Rm50100();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit50100 : public SouthExit
{
public:
	SouthExit50100();
  	void respond ( void );
	int doVerb ( int );
};

class Rm50200 : public Room
{
public:
	Rm50200();
	~Rm50200();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit50200 : public SouthExit
{
public:
	SouthExit50200();
  	void respond ( void );
	int doVerb ( int );
};

class Rm50300 : public Room
{
public:
	Rm50300();
	~Rm50300();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit50300 : public SouthExit
{
public:
	SouthExit50300();
  	void respond ( void );
	int doVerb ( int );
};

class NorthExit50300 : public ExitFeature
{
public:
	NorthExit50300();
  	void respond ( void );
	int doVerb ( int );
};

class GetCamera50000 : public Script
{
 public:
 	GetCamera50000(); 
	void changeState( int );
};

#endif

