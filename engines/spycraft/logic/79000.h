/*********************************************
* Header file.
* Dacha.
* Rooms: 79000        
* Copyright Activision 1995
* Started by Tom DeSalvo 
* Continued by:  Daniel Kinzek   August, 1995
**********************************************/

#ifndef SPYCRAFT_LOGIC_79000_H
#define SPYCRAFT_LOGIC_79000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"

class Rm79000 : public Room
{
public:
	Rm79000();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit79000 : public ExitFeature
{
public:
	SouthExit79000();
  	void respond ( void );
	int doVerb ( int );
};

/* Scripts */
class Script79000 : public Script
{
public:
	Script79000() {name = "Script79000";};
	void changeState ( int );
};

/* Dialogs */
class DachaDialog : public Dialog
{
 public:
 	int	playedLoop;	// flag
	DachaDialog();
	virtual void cue( void );
};

class DachaD1_79000	: public DachaDialog
{
 public:
	void cue( void );
};

class DachaD2_79000	: public DachaDialog
{
 public:
	void cue( void );
};

class DachaD3_79000	: public DachaDialog
{
 public:
	void cue( void );
};

class DachaD8_79000	: public DachaDialog
{
 public:
	void cue( void );
};

class DachaD9_79000	: public DachaDialog
{
 public:
	void cue( void );
};

class DachaD10_79000	: public DachaDialog
{
 public:
	void cue( void );
};

class DachaD14_79000	: public DachaDialog
{
 public:
	void cue( void );
};

class DachaD15_79000	: public DachaDialog
{
 public:
 	void cue( void );
};

class DachaD18_79000	: public DachaDialog
{
 public:
 	void cue( void );
};

class DachaD19_79000	: public DachaDialog
{
 public:
 	void cue( void );
};

class DachaD20_79000	: public DachaDialog
{
 public:
 	void cue( void );
};

class DachaD21_79000	: public DachaDialog
{
 public:
 	void cue( void );
};

class DachaD22_79000	: public DachaDialog
{
 public:
 	void cue( void );
};

class DachaDExit_79000	: public DachaDialog
{
 public:
	void cue( void );
};

/* DItems */
class CutDItem : public DItem
{
 public:
 	void	doit( void );	// cuts to next movie immediately
};

class GrabShootDItem : public CutDItem
{
 public:
 	int  grabbedGun;
	int	 shootLoop;
	GrabShootDItem( int whoToShootLoop );
	void doit();
};

class NewsFlash_16_79000 : public DItem
{
 public:
	void	doit( void );
};


#endif

