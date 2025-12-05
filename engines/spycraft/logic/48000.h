//Yuri's Office
//Started by Tom DeSalvo
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_48000_H
#define SPYCRAFT_LOGIC_48000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
//#include "47000.h"
#include "inset.h"
#include "flag.h"

class Timer;

class Rm48000 : public Room
{
public:
	
	Timer* myTimer;
	Rm48000();
	~Rm48000();
	void init ( void );
	void cue ( void);
	int handleEvent ( MADEEventStamp * );
};

class SouthExit48000 : public SouthExit
{
public:
	SouthExit48000();
	void respond ( void );
	int doVerb ( int);
};

class Photo48000 : public Feature
{
public:
	Photo48000();
	void respond( void );
	int doVerb ( int );
};

class PhotoHead48000 : public Feature
{
public:
	PhotoHead48000();
	void respond( void );	
	int doVerb ( int );
};

class Booze48000 : public Feature
{
public:
	Booze48000();
	void respond( void );
	int doVerb ( int );
};

class Shelf48000 : public Feature
{
public:
	Shelf48000();
	void respond( void );
	int doVerb ( int );
};

class Desk48000 : public Feature
{
public:
	Desk48000();
	int doVerb ( int );
};

class GiftBox48000 : public View
{
public:
	GiftBox48000();
	void respond( void );
	int doVerb ( int );
};

class PhotoInset48000 : public Inset
{
public:
	PhotoInset48000();
//	void respond();
//	int doVerb( int);
};

class PhotoHeadInset48000 : public Inset
{
public:
	PhotoHeadInset48000();
//	void respond();
//	int doVerb( int);
};

class BoozeInset48000 : public Inset
{
public:
	BoozeInset48000();
//	void respond();
//	int doVerb( int);
};

/* scripts */
class EnterScriptIntro48000 : public Script
{
 public:
 	EnterScriptIntro48000(); 
	void changeState( int );
};

class EnterScriptHarmonica48000 : public Script
{
 public:
 	EnterScriptHarmonica48000(); 
	void changeState( int );
};

class EnterScriptOnyx48000 : public Script
{
 public:
 	EnterScriptOnyx48000(); 
	void changeState( int );
};
/*
class GiftScript : public Script
{
public:
	GiftScript() {name = "GiftScript";};
	void changeState ( int );
};
*/

/* dialog */
class Dialog2_48000 : public Dialog
{
 	void cue( void );
};

class PhotoMaxSend : public DItem
{
public:
	PhotoMaxSend();
	void doit( void );
	virtual void cue( void );
};

class MafiaInfo : public DItem
{
public:
	MafiaInfo();
	void doit( void );
};

class ProblemsInfo : public DItem
{
public:
	ProblemsInfo();
	void doit( void );
};

/* Room48200 */
class Rm48200 : public Room
{
public:
	Rm48200();
	void init ( void );
};

class SouthExit48200 : public ExitFeature
{
public:
	SouthExit48200();
  	void respond ( void );
	int doVerb ( int );
};

class BallInset48200 : public Inset
{
public:
	BallInset48200();
//	int doVerb ( int );
};

class Softball48200 : public Feature
{
public:
	Softball48200();
	int doVerb ( int );
};

/* Room 48300 */
class Rm48300 : public Room
{
public:
	Rm48300();
	void init ( void );
};

class SouthExit48300 : public ExitFeature
{
public:
	SouthExit48300();
  	void respond ( void );
	int doVerb ( int );
};

class Box48300 : public Feature
{
public:
	Box48300();
	void respond ( void );
	int doVerb ( int );
};

class HarmInset48300 : public Inset
{
public:
	HarmInset48300();
//	void respond();
	int doVerb( int);
};

class EnterScript48300 : public Script
{
 public:
 	EnterScript48300(); 
	void changeState( int );
};


/*class Softball48000 : public TextFeature
{
public:
	Softball48000();
  	void respond ( void );
	int doVerb ( int );
};

class CDPlayer48000 : public TextFeature
{
public:
	short fPlayerClicked;
public:
	CDPlayer48000();
  //	void respond ( void );
	int doVerb ( int );
};

class PhotoMaxSendDialog : public Dialog
{
 public:
 	int	playedLoop;	// flag
	PhotoMaxSendDialog();
	virtual void cue( void );
};

class PhotoMaxSend	: public PhotoMaxSendDialog
{
 public:
	void cue( void );
};

class DeskInset48000 : public Inset
{
public:
	DeskInset48000();
	void respond();
	int doVerb( int);
};

class Note48000 : public TextFeature
{
public:
	Note48000();
	int doVerb ( int );
};

class Harmonica48000 : public TextFeature
{
public: 
	short fHarmonicaTouched;
public:
	Harmonica48000();
	int doVerb ( int );
};

class CommendationInset48000 : public Inset
{
public:
	CommendationInset48000();
	void respond();
	int doVerb( int);
};

class movieTest : public Script
{
public:
	movieTest() {name = "movieTest";};
	void changeState ( int );
};

class Script48000 : public Script
{
public:
	Script48000() {name = "Script48000";};
	void changeState ( int );
};
*/

#endif

