/***************************
*           Colby          *
*Copyright Activision 1995 *
*        Pat Griffith      *
***************************/

#ifndef _95300_H
#define _95300_H

#include "goggles.h"

#include "made.h"
#include "game.h"
#include "feature.h"
#include "script.h"
#include "actor.h"
#include "sound.h"
#include "colby.h"

class Rm95300 : public Room 
{
public:
	int 		number;	
	Rm95300();
	void init (void);
};
class Rm95301 : public Room 
{
public:

	int 		number;	
	Rm95301();
	void init (void);
};
class Rm95302 : public Room 
{
public:
	
	int 		number;	
	Rm95302();
	void init (void);
};
class Rm95303 : public Room 
{
public:
	
	int 		number;	
	Rm95303();
	void init (void);
};
class Rm95304 : public Room 
{
public:
	
	int 		number;	
	Rm95304();
	void init (void);
};
class Rm95305 : public Room 
{
public:
	
	int 		number;	
	Rm95305();
	void init (void);
};
class Rm95306 : public Room 
{
public:
	
	int 		number;	
	Rm95306();
	void init (void);
};
class Rm95307 : public Room 
{
public:
	
	int 		number;	
	Rm95307();
	void init (void);
};
class Rm95308 : public Room 
{
public:
	
	int 		number;	
	Rm95308();
	void init (void);
};
class Rm95309 : public Room 
{
public:
	
	int 		number;	
	Rm95309();
	void init (void);
};
class Rm95310 : public Room 
{
public:
	
	int 		number;	
	Rm95310();
	void init (void);
};
class Rm95311 : public Room 
{
public:
	
	int 		number;	
	Rm95311();
	void init (void);
};
class Rm95312 : public Room 
{
public:
	
	int 		number;	
	Rm95312();
	void init (void);
};
class Rm95313 : public Room 
{
public:
	
	int 		number;	
	Rm95313();
	void init (void);
};
class Rm95314 : public Room 
{
public:
	
	int 		number;	
	Rm95314();
	void init (void);
};
class Rm95315 : public Room 
{
public:
	
	int 		number;	
	Rm95315();
	void init (void);
};
class Rm95316 : public Room 
{
public:
	
	int 		number;	
	Rm95316();
	void init (void);
};
class Rm95317 : public Room 
{
public:
	
	int 		number;	
	Rm95317();
	void init (void);
};
class Rm95318 : public Room 
{
public:
	
	int 		number;	
	Rm95318();
	void init (void);
};
/***************************
*  EXITS
***************************/

class SouthExit95300 : public ExitFeature
{
public:
	Arrow * arrow;
	SouthExit95300();
	void respond ( void );
	int doVerb ( int );
};
class NorthExit95300 : public ExitFeature
{
public:
	Arrow * arrow;
	NorthExit95300();
	void respond ( void );
	int doVerb ( int );
};
class NorthEastExit95300 : public ExitFeature
{
public:
	Arrow * arrow;
	NorthEastExit95300();
	void respond ( void );
	int doVerb ( int );
};
class NorthWestExit95300 : public ExitFeature
{
public:
	Arrow * arrow;
	NorthWestExit95300();
	void respond ( void );
	int doVerb ( int );
};
class SouthEastExit95300 : public ExitFeature
{
public:
	Arrow * arrow;
	SouthEastExit95300();
	void respond ( void );
	int doVerb ( int );
};
class SouthWestExit95300 : public ExitFeature
{
public:
	Arrow * arrow;
	SouthWestExit95300();
	void respond ( void );
	int doVerb ( int );
};
#endif
