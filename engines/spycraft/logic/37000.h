//Started by Tom DeSalvo
//Ext. Street Kneecap
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_37000_H
#define SPYCRAFT_LOGIC_37000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm37000 : public Room
{
public:
	Rm37000();
	~Rm37000();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit37000 : public SouthExit
{
public:
	SouthExit37000();
  	void respond ( void );
	int doVerb ( int );
};
/*
class NorthExit37000 : public ExitFeature
{
public:
	NorthExit37000();
  	void respond ( void );
	int doVerb ( int );
};
*/
/*features*/
/*class KneeRoof37000 : public TextFeature
{
public:
	KneeRoof37000();
	int doVerb ( int );
};
*/

class PanelVan37000 : public Feature
{
public:
	PanelVan37000();
	void respond();
	int doVerb ( int );
};

class PanelSign37000 : public Feature
{
public:
	PanelSign37000();
	void respond();
	int doVerb ( int );
};
/*
class Tenement37000 : public TextFeature
{
public:
	Tenement37000();
	int doVerb ( int );
};
*/
/* insets */
class PanelSignInset37000: public Inset
{
 public:
	PanelSignInset37000();
//	void respond();
};
 
/* script */
class EnterBadmanVanScript37000 : public Script
{
public:
	EnterBadmanVanScript37000();
	void changeState ( int );
};

#endif

