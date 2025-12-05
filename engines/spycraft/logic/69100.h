//Started by Ka Wah
//Hotel Schlump - closet west of int.
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_69100_H
#define SPYCRAFT_LOGIC_69100_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"
#include "sound.h"

class Rm69100 : public Room
{
public:
	Rm69100();
	~Rm69100();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class EastExit69100 : public ExitFeature
{
public:
	EastExit69100();
  	void respond ( void );
	int doVerb ( int );
};

class Rm69101 : public Room
{
public:
	Rm69101();
	~Rm69101();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class EastExit69101 : public ExitFeature
{
public:
	EastExit69101();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class Closet69100 : public Feature
{
public:
	Closet69100();
	void respond( void );
	int doVerb( int );
};

class Closet69101 : public Feature
{
public:
	Closet69101();
	void respond( void );
	int doVerb( int );
};

class LeftClosetDoor69101 : public Feature
{
public:
	LeftClosetDoor69101();
	void respond( void );
	int doVerb( int );
};

class RightClosetDoor69101 : public Feature
{
public:
	RightClosetDoor69101();
	void respond( void );
	int doVerb( int );
};

/* insets */
/*
class ClosetInset69100: public Inset
{
 public:
	ClosetInset69100();
//	void respond();
};
*/

/* views */

class Handcuffs69100 : public View
{
public:
	Handcuffs69100();
  	void respond ( void );
	int doVerb ( int );
};


/* Room 69102 */
class Rm69102 : public Room
{
public:
	Rm69102();
	~Rm69102();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit69102 : public SouthExit
{
public:
	SouthExit69102();
  	void respond ( void );
	int doVerb ( int );
};

/* Sound Scripts */

class ClosetOpen69100 : public Script
{
 public:
 	Sound *closetSnd;
 	ClosetOpen69100() { name = "ClosetOpen69100"; closetSnd = new Sound; };
	void changeState( int );
};

class ClosetClose69100 : public Script
{
 public:
 	Sound *closetSnd;
 	ClosetClose69100() { name = "ClosetClose69100"; closetSnd = new Sound; };
	void changeState( int );
};

class HandcuffSound69100 : public Script
{
 public:
 	Sound *handCuffSnd;
 	HandcuffSound69100() { name = "HandcuffSound69100"; handCuffSnd = new Sound; };
	void changeState( int );
};

#endif
