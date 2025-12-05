/*********************************************
* Header file.
* Holt's office. 
* Rooms: 26000, 26100       
* Copyright Activision 1995
* Started by Daniel Kinzek   June, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_26000_H
#define SPYCRAFT_LOGIC_26000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"

/* rooms */
class Rm26000 : public Room
{
public:
	Rm26000();
	void init();
};

class Rm26100 : public Room
{
public:
	Rm26100();
	void init();
};

/* exit features */
class SouthExit26000 : public ExitFeature
{
public:
	SouthExit26000();
  void respond( void );
	int doVerb( int );
};

class SouthExit26100 : public ExitFeature
{
public:
	SouthExit26100();
  void respond( void );
	int doVerb( int );
};

/* features */
class Desk26000 : public Feature
{
public:
	Desk26000();
	void respond( void );
	int doVerb( int );
};

class Photos26000 : public Feature
{
public:
	Photos26000();
	void respond( void );
	int doVerb( int );
};

class Map26000 : public Feature
{
public:
	Map26000();
	void respond( void );
	int doVerb( int );
};

class TravelLetter26100 : public Feature
{
public:
	TravelLetter26100();
	void respond( void );
	int doVerb( int );
};

class Folders26100 : public Feature
{
public:
	Folders26100();
	void respond( void );
	int doVerb( int );
};

/* insets */
class MapInset26000 : public Inset		
{
public:
	MapInset26000();
};

class PhotosInset26000 : public Inset		
{
public:
	PhotosInset26000();
};

class MelnachovInset26000 : public Inset		
{
public:
	MelnachovInset26000();
	void dispose( void );
};

class TravelLetterInset26000 : public Inset		
{
public:
	TravelLetterInset26000();
};

/* Inset Features */
class MelnachovIFeature26000 : public InsetFeature
{
public:
	MelnachovIFeature26000();
	void respond( void );
	int doVerb( int );
};

/* Scripts */
class FolderSndScript26100 : public Script
{
 public:
 	FolderSndScript26100() { name = "FolderSndScript26100"; };
	void changeState( int );
};
#endif
