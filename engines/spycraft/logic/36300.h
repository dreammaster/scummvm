//Started by Tom DeSalvo
//Foster's desk, view of drawers.
//Copyright Activision 1995
//Finished by Ka Wah

#ifndef SPYCRAFT_LOGIC_36300_H
#define SPYCRAFT_LOGIC_36300_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "inset.h"
#include "flag.h"
#include "invent.h"

class Rm36300 : public Room
{
public:
	Rm36300();
	~Rm36300();
	void init();
	int handleEvent ( MADEEventStamp * );
};

class SouthExit36300 : public ExitFeature
{
public:
	SouthExit36300();
  void respond( void );
	int doVerb( int );
};

/* features */
class Drawer36300 : public Feature
{
public:
	Drawer36300();
	void respond( void );
	int doVerb( int );
	int	playerHasAllFolders( Invent *inventory );
};

/* insets */
class DrawerWFolderInset36300 : public Inset		
{
public:
	DrawerWFolderInset36300();
};
/*
class EmptyDrawerInset36300 : public Inset		
{
public:
	EmptyDrawerInset36300();
};
*/
/* inset features */
/*
class FolderInsetFeature36300 : public InsetFeature
{
public:
	FolderInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class PaymentsInsetFeature36300 : public InsetFeature
{
public:
	PaymentsInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class CitationInsetFeature36300 : public InsetFeature
{
public:
	CitationInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class ZhironevInsetFeature36300 : public InsetFeature
{
public:
	ZhironevInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class OrdersInsetFeature36300 : public InsetFeature
{
public:
	OrdersInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};

class ShustakInsetFeature36300 : public InsetFeature
{
public:
	ShustakInsetFeature36300();
	void respond( void );
	int doVerb ( int );
};
*/
class Rm36301 : public Room
{
public:
	Rm36301();
	~Rm36301();
	void init();
	int handleEvent ( MADEEventStamp * );
};

class SouthExit36301 : public ExitFeature
{
public:
	SouthExit36301();
  void respond( void );
	int doVerb( int );
};

class Rm36302 : public Room
{
public:
	Rm36302();
	~Rm36302();
	void init();
	int handleEvent ( MADEEventStamp * );
};

class SouthExit36302 : public ExitFeature
{
public:
	SouthExit36302();
  void respond( void );
	int doVerb( int );
};

class OpenDrawer36300 : public Feature
{
public:
	OpenDrawer36300();
	void respond( void );
	int doVerb( int );
};

class Rm36303 : public Room
{
public:
	Rm36303();
	~Rm36303();
	void init();
	int handleEvent ( MADEEventStamp * );
};

class SouthExit36303 : public ExitFeature
{
public:
	SouthExit36303();
  void respond( void );
	int doVerb( int );
};

#endif
