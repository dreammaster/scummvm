#ifndef SPYCRAFT_LOGIC_12000_H
#define SPYCRAFT_LOGIC_12000_H

#include "made.h"
#include "game.h"
#include "feature.h"
#include "actor.h"
#include "globals.h"
#include "inset.h"

class Rm12000 : public Room
{
public:
	Rm12000();
	void init ( void );
	//int handleEvent ( MADEEventStamp * );
};

class SouthExit12000 : public ExitFeature
{
public:
	SouthExit12000();
  	void respond ( void );
	int doVerb ( int );
};

class PictureOne12000 : public TextFeature
{
public:
	PictureOne12000();
  	void respond ( void );
	int doVerb ( int );
};

class PictureTwo12000 : public TextFeature
{
public:
	PictureTwo12000();
  	void respond ( void );
	int doVerb ( int );
};

class PictureThree12000 : public TextFeature
{
public:
	PictureThree12000();
  	void respond ( void );
	int doVerb ( int );
};

class PictureFour12000 : public TextFeature
{
public:
	PictureFour12000();
  	void respond ( void );
	int doVerb ( int );
};

class PictureFive12000 : public TextFeature
{
public:
	PictureFive12000();
  	void respond ( void );
	int doVerb ( int );
};

class PictureOneInset12000 : public Inset
{
 public:
	PictureOneInset12000();
	void respond();
};

class PictureTwoInset12000 : public Inset
{
 public:
	PictureTwoInset12000();
	void respond();
};

class PictureThreeInset12000 : public Inset
{
 public:
	PictureThreeInset12000();
	void respond();
};

class PictureFourInset12000 : public Inset
{
 public:
	PictureFourInset12000();
	void respond();
};

class PictureFiveInset12000 : public Inset
{
 public:
	PictureFiveInset12000();
	void respond();
};


class Script12000 : public Script
{
 public:
 	Script12000() {name = "Script12000";};
 	void changeState( int );
};

#endif

