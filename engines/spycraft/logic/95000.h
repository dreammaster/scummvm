/***************************
*           Colby          *
*Copyright Activision 1995 *
*        Pat Griffith      *
***************************/

#ifndef _95000_H
#define _95000_H

#include "goggles.h"

#include "made.h"
#include "game.h"
#include "feature.h"
#include "script.h"
#include "actor.h"
#include "sound.h"
#include "colby.h"

class Rm95000 : public Room 
{
public:
	int 		number;	
	Rm95000();
	void init(void);
};
class Rm95001 : public Room 				 
{
public:
	int 		number;	
	Rm95001();
	void init(void);
};
class Rm95002 : public Room 
{
public:
	int 		number;	
	Rm95002();
	void init(void);
};
class Rm95003 : public Room 
{
public:
	int 		number;	
	Rm95003();
	void init(void);
};
class Rm95004 : public Room 
{
public:
	int 		number;	
	Rm95004();
	void init(void);
};
class Rm95005 : public Room 
{
public:
	int 		number;	
	Rm95005();
	void init(void);
};
class Rm95006 : public Room 
{
public:
	int 		number;	
	Rm95006();
	void init(void);
};
class Rm95007 : public Room 
{
public:
	int 		number;	
	Rm95007();
	void init(void);
};
class Rm95008 : public Room 
{
public:
	int 		number;	
	Rm95008();
	void init(void);
};
class Rm95009 : public Room 
{
public:
	int 		number;	
	Rm95009();
	void init(void);
};
class Rm95010 : public Room 
{
public:
	int 		number;	
	Rm95010();
	void init(void);
};
class Rm95011 : public Room 
{
public:
	int 		number;	
	Rm95011();
	void init(void);
};
class Rm95012 : public Room 
{
public:
	int 		number;	
	Rm95012();
	void init(void);
};
class Rm95013 : public Room 
{
public:
	int 		number;	
	Rm95013();
	void init(void);
};
class Rm95014 : public Room 
{
public:
	int 		number;	
	Rm95014();
	void init(void);
};
class Rm95015 : public Room 
{
public:
	int 		number;	
	Rm95015();
	void init(void);
};
class Rm95016 : public Room 
{
public:
	int 		number;	
	Rm95016();
	void init(void);
};
class Rm95017 : public Room 
{
public:
	int 		number;	
	Rm95017();
	void init(void);
};
class Rm95018 : public Room 
{
public:
	int 		number;	
	Rm95018();
	void init(void);
};
/***************************
*  EXITS
***************************/

class SouthExit95000 : public ExitFeature
{
public:
	Arrow * arrow;
	SouthExit95000();
	void respond ( void );
	int doVerb ( int );
};
class NorthExit95000 : public ExitFeature
{
public:
	Arrow * arrow;
	NorthExit95000();
	void respond ( void );
	int doVerb ( int );
};
class NorthEastExit95000 : public ExitFeature
{
public:
	Arrow * arrow;
	NorthEastExit95000();
	void respond ( void );
	int doVerb ( int );
};
class NorthWestExit95000 : public ExitFeature
{
public:
	Arrow * arrow;
	NorthWestExit95000();
	void respond ( void );
	int doVerb ( int );
};
class SouthEastExit95000 : public ExitFeature
{
public:
	Arrow * arrow;
	SouthEastExit95000();
	void respond ( void );
	int doVerb ( int );
};
class SouthWestExit95000 : public ExitFeature
{
public:
	Arrow * arrow;
	SouthWestExit95000();
	void respond ( void );
	int doVerb ( int );
};
class WPScript : public Script
{
public:
	WPScript() {name = "wpscript";};
	void changeState ( int );
};
#endif
