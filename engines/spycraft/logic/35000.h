//Player's office
//Started by Tom DeSalvo
//Continued by Ka Wah
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_35000_H
#define SPYCRAFT_LOGIC_35000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm35000 : public Room
{
public:
	Rm35000();
	~Rm35000();
	void init ( void );
	void doit ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit35000 : public SouthExit
{
public:
	SouthExit35000();
  	void respond ( void );
	int doVerb ( int );
};

class Script35000 : public Script
{
public:
	Script35000() {name = "Script35000";};
	void changeState ( int );
};

/* features */
class FileCabinet35000 : public Feature
{
public:
	FileCabinet35000();
	void respond( void );
	int doVerb( int );
};

class Desk35000 : public Feature
{
public:
	Desk35000();
	void respond( void );
	int doVerb( int );
};

class SetYingPath35000 : public Feature
{
public:
	SetYingPath35000();
	void respond( void );
	int doVerb( int );
};

class SetMoscowMap35000 : public TextFeature
{
public:
	SetMoscowMap35000();
	void respond( void );
	int doVerb( int );
};

class SetYuriFlag35000 : public TextFeature
{
public:
	SetYuriFlag35000();
	void respond( void );
	int doVerb( int );
};

class SetMaxFlag35000 : public Feature
{
public:
	SetMaxFlag35000();
	void respond( void );
	int doVerb( int );
};

class SetBSFlag35000 : public TextFeature
{
public:
	SetBSFlag35000();
	void respond( void );
	int doVerb( int );
};

class SetWalkPathPart1 : public TextFeature
{
public:
	SetWalkPathPart1();
	void respond( void );
	int doVerb( int );
};

class SetWalkPathPart2 : public TextFeature
{
public:
	SetWalkPathPart2();
	void respond( void );
	int doVerb( int );
};

class SetWalkPathPart3 : public TextFeature
{
public:
	SetWalkPathPart3();
	void respond( void );
	int doVerb( int );
};

/* insets */
class FileCabinetInset35000: public Inset
{
 public:
	FileCabinetInset35000();
	void respond();
	int doVerb( int );
};

class FileCabinetInset35001: public Inset
{
 public:
	FileCabinetInset35001();
//	void respond();
};

/* views */
class WideRoomOrangeSwan35001 : public View
{
public:
	WideRoomOrangeSwan35001();
// 	void respond ( void );
//	int doVerb ( int );
};

class WideRoomMusicSwan35001 : public View
{
public:
	WideRoomMusicSwan35001();
// 	void respond ( void );
//	int doVerb ( int );
};

class WideRoomPresident35001 : public View
{
public:
	WideRoomPresident35001();
// 	void respond ( void );
//	int doVerb ( int );
};

class WideRoomEnvelope35001 : public View
{
public:
	WideRoomEnvelope35001();
// 	void respond ( void );
//	int doVerb ( int );
};

class WideRoomVideoCamera35001 : public View
{
public:
	WideRoomVideoCamera35001();
// 	void respond ( void );
//	int doVerb ( int );
};

/*************
*  RM35100
**************/
class Rm35100 : public Room
{	
				
public:
	Rm35100();
	~Rm35100();
	void init ( void );
	void doit ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit35100: public SouthExit
{
public:
	SouthExit35100();
  	void respond ( void );
	int doVerb ( int );
};

class WestExit35100: public ExitFeature
{
public:
	WestExit35100();
  	void respond ( void );
	int doVerb ( int );
};

class Computer35100 : public Feature
{
public:
	Computer35100();
	void respond( void );
	int doVerb( int );
};

class Phone35100 : public Feature
{
public:
	Phone35100();
	void respond( void );
	int doVerb( int );
};

class TopEnvelope35100 : public Feature
{
public:
	TopEnvelope35100();
	void respond( void );
	int doVerb( int );
};

class BottomEnvelope35100 : public Feature
{
public:
	BottomEnvelope35100();
	void respond( void );
	int doVerb( int );
};

/* views */
class SwanOne35100 : public View
{
public:
	SwanOne35100();
  	void respond ( void );
	int doVerb ( int );
};

class SwanTwo35100 : public View
{
public:
	SwanTwo35100();
  	void respond ( void );
	int doVerb ( int );
};

class President35100 : public View
{
public:
	President35100();
  	void respond ( void );
	int doVerb ( int );
};

class Envelope35100 : public View
{
public:
	Envelope35100();
  	void respond ( void );
	int doVerb ( int );
};

class VideoCamera35100 : public View
{
public:
	VideoCamera35100();
  	void respond ( void );
	int doVerb ( int );
};

class VoiceMail35100 : public View
{
public:
	VoiceMail35100();
// 	void respond ( void );
//	int doVerb ( int );
};

class VoiceLight36000 : public Prop
{
public:
	VoiceLight36000();
	void cue( void );
};

/* insets */
class SwanOneInset35100: public Inset
{
 public:
	SwanOneInset35100();
	void respond();
	int doVerb( int );
};

class SwanOneInset35101: public Inset
{
 public:
	SwanOneInset35101();
//	void respond();
};

class SwanTwoInset35100: public Inset
{
 public:
	SwanTwoInset35100();
	void respond();
	int doVerb( int );
};

class SwanTwoInset35101: public Inset
{
 public:
	SwanTwoInset35101();
//	void respond();
};

class PresidentInset35100: public Inset
{
 public:
	PresidentInset35100();
//	void respond();
};

class EnvelopeInset35100: public Inset
{
 public:
	EnvelopeInset35100();
//	void respond();
};


class VideoCamerInset35100: public Inset
{
 public:
	VideoCamerInset35100();
//	void respond();
};

/* scripts */
class IDKneeScript35000 : public Script
{
 public:
 	IDKneeScript35000(); 
	void changeState( int );
};

class UnfoldMusicSwanScript35100 : public Script
{
 public:
 	UnfoldMusicSwanScript35100(); 
	void changeState( int );
};

class UnfoldEnvelopeScript35100 : public Script
{
 public:
 	UnfoldEnvelopeScript35100(); 
	void changeState( int );
};

class MusicSwanCypherScript35000 : public Script
{
 public:
 	MusicSwanCypherScript35000(); 
	void changeState( int );
};

class Rm35001 : public Room
{
public:
	Rm35001();
	~Rm35001();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class SouthExit35001: public SouthExit
{
public:
	SouthExit35001();
  	void respond ( void );
	int doVerb ( int );
};

class WestExit35001: public ExitFeature
{
public:
	WestExit35001();
  	void respond ( void );
	int doVerb ( int );
};

class EastExit35001: public ExitFeature
{
public:
	EastExit35001();
  	void respond ( void );
	int doVerb ( int );
};

class OpenMusciSwan35001 : public Feature
{
public:
	OpenMusciSwan35001();
	void respond( void );
	int doVerb( int );
};

class Rm35002 : public Room
{	
				
public:
	Rm35002();
	~Rm35002();
	void init ( void );
	int handleEvent ( MADEEventStamp * );
};

class MusicSwanCursor : public MadeCursor
{
public:
	MusicSwanCursor();
};

#endif

