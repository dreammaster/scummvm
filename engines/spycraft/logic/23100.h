//Started by Ka Wah
//Interrogation Room - View of the desk from 23000
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_23100_H
#define SPYCRAFT_LOGIC_23100_H

#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm23100 : public Room
{					
public:
	Rm23100();
	//~Rm23100();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit23100 : public SouthExit
{
public:
	SouthExit23100();
  	void respond ( void );
	int doVerb ( int );
};

/* features */
class Microphone23100 : public Feature
{
public:
	Microphone23100();
	void respond( void );
	int doVerb( int );
};

class TapePlayer23100 : public Feature
{
public:
	TapePlayer23100();
	void respond( void );
	int doVerb( int );
};

class PolygraphFinger23100 : public Feature
{
public:
	PolygraphFinger23100();
	void respond( void );
	int doVerb( int );
};
/*
class PolygraphRead23100 : public TextFeature
{
public:
	PolygraphRead23100();
	void respond( void );
	int doVerb( int );
};
*/
/* insets */
class MicrophoneInset23100: public Inset
{
 public:
	MicrophoneInset23100();
//	void respond();
};

class TapePlayerInset23100: public Inset
{
 public:
	TapePlayerInset23100();
//	void respond();
};

class PolygraphFingerInset23100: public Inset
{
 public:
	PolygraphFingerInset23100();
//	void respond();
};
/*
class PolygraphReadInset23100: public Inset
{
 public:
	PolygraphReadInset23100();
//	void respond();
};
*/
#endif

