//Started by Tom DeSalvo
//Continued by Daniel Kinzek
//Jamie's office

#ifndef SPYCRAFT_LOGIC_27000_H
#define SPYCRAFT_LOGIC_27000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"


/* rooms */
class Rm27000 : public Room
{
public:
	Rm27000();
	void init ( void );
};

class Rm27100 : public Room
{
public:
	Rm27100();
	void init();
};

class Rm27200 : public Room
{
public:
	Rm27200();
	void init();
};

/* exits */
// 27000
class SouthExit27000 : public ExitFeature
{
public:
	SouthExit27000();
  	void respond ( void );
	int doVerb ( int );
};

class EastExit27000 : public ExitFeature
{
public:
	EastExit27000();
  void respond( void );
	int doVerb( int );
};

class WestExit27000 : public ExitFeature
{
public:
	WestExit27000();
  void respond( void );
	int doVerb( int );
};

// 27100
class SouthExit27100 : public ExitFeature
{
public:
	SouthExit27100();
  void respond( void );
	int doVerb( int );
};

class EastExit27100 : public ExitFeature
{
public:
	EastExit27100();
  void respond( void );
	int doVerb( int );
};

// 27200
class SouthExit27200 : public ExitFeature
{
public:
	SouthExit27200();
  void respond( void );
	int doVerb( int );
};

class WestExit27200 : public ExitFeature
{
public:
	WestExit27200();
  void respond( void );
	int doVerb( int );
};

/* features */
class Photo27000 : public Feature
{
public:
	Photo27000();
	void respond( void );
	int doVerb( int );
};

class Phone27000 : public Feature
{
public:
	Phone27000();
	void respond( void );
	int doVerb( int );
};

class Camera27000 : public Feature
{
public:
	Camera27000();
	void respond( void );
	int doVerb( int );
};

// 27100
class Phone27100 : public Feature
{
public:
	Phone27100();
	void respond( void );
	int doVerb( int );
};

class Camera27100 : public Feature
{
public:
	Camera27100();
	void respond( void );
	int doVerb( int );
};

class Folder27100 : public Feature
{
public:
	Folder27100();
	void respond( void );
	int doVerb( int );
};

// 27200


/* insets */
//27000
class PhotoInset27000 : public Inset		
{
public:
	PhotoInset27000();
	void respond();
	int doVerb( int );
};

class PhoneInset27000 : public Inset		
{
public:
	PhoneInset27000();
	void respond();
	int doVerb( int );
};

class CameraInset27000 : public Inset		
{
public:
	CameraInset27000();
	void respond();
	int doVerb( int );
};

// 27100
class PhoneInset27100 : public Inset		
{
public:
	PhoneInset27100();
	void respond();
	int doVerb( int );
};

class CameraInset27100 : public Inset		
{
public:
	CameraInset27100();
	void respond();
	int doVerb( int );
};

class FolderInset27100 : public Inset		
{
public:
	FolderInset27100();
};


/* Inset Features */

class PhoneIFeature27000 : public InsetFeature
{
public:
	PhoneIFeature27000();
  	void respond( void );
	int doVerb( int );
};

class PhoneIFeature27100 : public InsetFeature
{
public:
	PhoneIFeature27100();
  	void respond( void );
	int doVerb( int );
};

class CorrTabIFeature27100 : public InsetFeature
{
public:
	CorrTabIFeature27100();
  	void respond( void );
	int doVerb( int );
};

class BSTabIFeature27100 : public InsetFeature
{
public:
	BSTabIFeature27100();
  	void respond( void );
	int doVerb( int );
};


#endif

