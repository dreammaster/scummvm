/***************************
*           Colby          *
*Copyright Activision 1995 *
*        Pat Griffith      *
***************************/

#ifndef _95200_H
#define _95200_H

#include "goggles.h"

#include "made.h"
#include "game.h"
#include "feature.h"
#include "script.h"
#include "actor.h"
#include "sound.h"
#include "colby.h"

class Rm95200 : public Room 
{
public:
	int 		number;	
	Rm95200();
	void init (void);
};
class Rm95201 : public Room 
{
public:

	int 		number;	
	Rm95201();
	void init (void);
};
class Rm95202 : public Room 
{
public:
	
	int 		number;	
	Rm95202();
	void init (void);
};
class Rm95203 : public Room 
{
public:
	
	int 		number;	
	Rm95203();
	void init (void);
};
class Rm95204 : public Room 
{
public:
	
	int 		number;	
	Rm95204();
	void init (void);
};
class Rm95205 : public Room 
{
public:
	
	int 		number;	
	Rm95205();
	void init (void);
};
class Rm95206 : public Room 
{
public:
	
	int 		number;	
	Rm95206();
			
	void init (void);
};
class Rm95207 : public Room 
{
public:
	
	int 		number;	
	Rm95207();
			
	void init (void);
};
class Rm95208 : public Room 
{
public:
	
	int 		number;	
	Rm95208();
			
	void init (void);
};
class Rm95209 : public Room 
{
public:
	
	int 		number;	
	Rm95209();
			
	void init (void);
};
class Rm95210 : public Room 
{
public:
	
	int 		number;	
	Rm95210();
			
	void init (void);
};
class Rm95211 : public Room 
{
public:
	
	int 		number;	
	Rm95211();
			
	void init (void);
};
class Rm95212 : public Room 
{
public:
	
	int 		number;	
	Rm95212();
			
	void init (void);
};
class Rm95213 : public Room 
{
public:
	
	int 		number;	
	Rm95213();
			
	void init (void);
};
class Rm95214 : public Room 
{
public:
	
	int 		number;	
	Rm95214();
			
	void init (void);
};
class Rm95215 : public Room 
{
public:
	
	int 		number;	
	Rm95215();
			
	void init (void);
};
class Rm95216 : public Room 
{
public:
	
	int 		number;	
	Rm95216();
			
	void init (void);
};
class Rm95217 : public Room 
{
public:
	
	int 		number;	
	Rm95217();
			
	void init (void);
};
class Rm95218 : public Room 
{
public:
	
	int 		number;	
	Rm95218();
			
	void init (void);
};
/***************************
*  EXITS
***************************/

class SouthExit95200 : public ExitFeature
{
public:
	Arrow * arrow;
	SouthExit95200();
	void respond ( void );
	int doVerb ( int );
};
class NorthExit95200 : public ExitFeature
{
public:
	Arrow * arrow;
	NorthExit95200();
	void respond ( void );
	int doVerb ( int );
};
class NorthEastExit95200 : public ExitFeature
{
public:
	Arrow * arrow;
	NorthEastExit95200();
	void respond ( void );
	int doVerb ( int );
};
class NorthWestExit95200 : public ExitFeature
{
public:
	Arrow * arrow;
	NorthWestExit95200();
	void respond ( void );
	int doVerb ( int );
};
class SouthEastExit95200 : public ExitFeature
{
public:
	Arrow * arrow;
	SouthEastExit95200();
	void respond ( void );
	int doVerb ( int );
};
class SouthWestExit95200 : public ExitFeature
{
public:
	Arrow * arrow;
	SouthWestExit95200();
	void respond ( void );
	int doVerb ( int );
};
#endif
