/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/timer.h"
#include "spycraft/dmade/advscreen.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/flag.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/invent.h"
#include "spycraft/game/vlink.h"
#include "spycraft/logic/90000.h"
#include "spycraft/logic/93000.h"
#include "spycraft/dmade/advmusic.h"
#include "spycraft/game/html.h"
#include "spycraft/game/sndanal.h"
#include "spycraft/game/knee.h"

namespace Spycraft {

extern BlinkCursor*	normalCursor;
extern int currentFocus;
extern Intrface* intrface;
extern Movie* theMovie;
//extern Rm1000* rm1000;
extern int goingBackwards;
extern Sound* sound1;
int isIntrBuffered;
HotspotList* oldlist;
HotspotList* puzzle_hotspots;
List* web_intrface;
IntrPuzz* intrpuzz;
Webmap * webmap;
List * announceList;
List * usedList;
int stopglobe = false;
int fwdbutton = false;
int backbutton = false;
extern Flag GameFlag;
int usedbutton = false; 
int lasttool = -1;
int helppage = 92800;
int reportnum = 0;
int dumping = false;
extern Sndanal* anal;
extern Knee* knee;
Marker::Marker()
{
  name = "Marker";
} 

void Marker::init(int theX, int theY, int pic_id, int loop_id)
{
	view = pic_id;
	loop = loop_id;
	cel = 0;
	IntrPuzzView::init(theX, theY);		 	
}	

void Button::activate(void)
{
}	

int Button::handleEvent(MADEEventStamp* event)
{
  if (cel < 2)	
  {	
	  if (onMe(event) && !(flag & OBJECT_HIDDEN))  
	  {
	  	if (user->input)
		{
		  	if (event_type == USER_LEFT_UP)
		    {
		   		if (cel == 1)
		   		{
			   		setCel(0);
					activate();
					return(true);
				}
		    }
			else if (event_type == USER_LEFT_DOWN)
			{
				if (cel == 0)
				{
					setCel(1);
					return (true);
				}
			}
		}
	  }
	  else
	  {
		if(cel == 1)
		{	
	  		setCel(0);
		}
	  }
  }
  return false;
}

void WebButton::init(int theX, int theY, int pic_id, int loop_id, int cel_id)
{
	view = pic_id;
	loop = loop_id;
	cel = cel_id;
	x = theX;
	y = theY;
	draw();
	web_intrface->addToFront ( this );
}	

void WebView::init(int theX, int theY, int pic_id, int loop_id, int cel_id)
{
	view = pic_id;
	loop = loop_id;
	cel = cel_id;
	x = theX;
	y = theY;
	draw();
	web_intrface->addToFront ( this );
}	

void WebButton::activate(void)
{
}	

int WebButton::handleEvent(MADEEventStamp* event)
{
  if (cel < 2)
  {
	  if(!(flag & OBJECT_HIDDEN))
	  {
		  if (onMe(event))  
		  {
		  	if (user->input)
			{
			  	if (event_type == USER_LEFT_UP)
			    {
					if (cel == 1)
					{
				   		setCel(0);
						activate();
						return(true);
					}	
			    }
				else if (event_type == USER_LEFT_DOWN)
				{
					if (cel == 0)
					{
						setCel(1);
						return (true);
					}
				}
			}
		  }
		  else
		  {
			if(cel == 1)
			{	
		  		setCel(0);
			}
		  }
	  }	
  }
  return false;
}

/***********************************
 *		
 *			  Slider
 *		      
 **********************************/
Slider::Slider()
{
	name = "Slider";
	buttonDown = false;	
	flag = true;   //object is a slider	
}

void Slider::dispose()
{
	marker->dispose();
	IntrPuzzFeature::dispose();
}

Slider::~Slider()
{
}

int Slider::handleEvent(MADEEventStamp* event)
{

	if (event_type == USER_LEFT_UP)	//clear the slider button down
	{
		buttonDown = false;
	}

	if (user->input && onMe(event))
		{
		if (event_type == USER_LEFT_DOWN)
		{
			buttonDown = true;
		}

		if (buttonDown)
		{
			marker->x = event->cursor_pt.x;	
			marker->posn(marker->x,marker->y);
			step = (marker->x - left)/num_steps;
		}
			return (true);
	}
	return (false);	
}

bool Slider::init(int theX, int theY, int theWidth, int theHeight, int id, int loop_id)
{
	IDnum = id;
			 
	marker = new(Marker);
	marker->init(theX, theY, id, loop_id);
	marker->x = theX;
	marker->y = theY;
	num_steps = theWidth / 6;

	IntrPuzzFeature::init (theX,theY,theWidth,theHeight);
	return (true);
}

/***********************************
 *		
 *		  Puzzle Text Feature
 *		      
 **********************************/
void IntrPuzzTextFeature::init(int theX, int theY, int width, int height, int color)
{
	left = theX;
	top = theY;
	right = left + width - 1;
	bottom = top + height - 1;

  	myPrint = new(Print);
  	myPrint->x = theX;
  	myPrint->y = theY;
	myPrint->size = 11;
  	myPrint->fore_color = color;//75;//1;
	myPrint->init(name);

	intrpuzz->addToFront ( this );	 
}

void IntrPuzzTextFeature::dispose()
{
	if ( hotspot )
	{
		setHotspot ( NULL );
		hotspot = NULL;
	}

  	if (myPrint)
  	{
  		myPrint->dispose();
		myPrint = NULL;
	}

	intrpuzz->release ( this );
	deathList->add(this);
}

IntrPuzzTextFeature::~IntrPuzzTextFeature()
{
}
	
CommandLine::CommandLine(int theX, int theY, int theWidth, int theHeight, const char *message) {
	memset (&text,0,256*sizeof(char));
	Common::strcpy_s(text, message);

 	commandPrint = new(Print);
  	commandPrint->x = 23;
  	commandPrint->y = 383;
  	commandPrint->size = 14;
  	commandPrint->fore_color = TEXT_COLOR_RED;
	commandPrint->myString = text;

//	strcpy(commandPrint->myString,text);

	init(theX, theY, theWidth, theHeight);
}
		
void CommandLine::init ( int theX, int theY, int theWidth, int theHeight )
{
	left = theX;
	top = theY;
	right = left + theWidth - 1;
	bottom = top + theHeight - 1;

	intrpuzz->add( this );	 
}

int CommandLine::handleEvent ( MADEEventStamp *event )
{
	if (onMe(mouse_x, mouse_y))
	{
		if (!active)
		{ 
			active = true;
			commandPrint->init();		
		}
	}
	else if (active)
	{ 
		active = false;
		commandPrint->erase();
	}
	return false;	
}
void CommandLine::hide()
{
	flag |= OBJECT_HIDDEN;
}
void CommandLine::show()
{
	flag &= ~OBJECT_HIDDEN;
}

void CommandLine::dispose()
{
	if ( hotspot )
	{
		setHotspot ( NULL );
		hotspot = NULL;
	}

	intrpuzz->release ( this );
	deathList->add(this);
};
	
CommandLine::~CommandLine()
{
  	if (commandPrint)
  		commandPrint->dispose();
}
WebCommandLine::WebCommandLine(int theX, int theY, int theWidth, int theHeight, const char *message) {
	Common::strcpy_s(text, message);

 	commandPrint = new(Print);
  	commandPrint->x = 23;
  	commandPrint->y = 383;
  	commandPrint->size = 14;
  	commandPrint->fore_color = TEXT_COLOR_RED;
	commandPrint->myString = (char*)text;

//	strcpy(commandPrint->myString,text);

	init(theX, theY, theWidth, theHeight);
}
		
void WebCommandLine::init ( int theX, int theY, int theWidth, int theHeight )
{
	left = theX;
	top = theY;
	right = left + theWidth - 1;
	bottom = top + theHeight - 1;

	web_intrface->add( this );	 
}
void WebCommandLine::dispose()				  
{
	if ( hotspot )
	{
		setHotspot ( NULL );
		hotspot = NULL;
	}

	web_intrface->release ( this );
	deathList->add(this);
};
	
/***********************************
 *		
 *		 Puzzle Feature
 *		      
 **********************************/
void IntrPuzzFeature::init ( int theX, int theY, int theWidth, int theHeight )
{
	left = theX;
	top = theY;
	right = left + theWidth - 1;
	bottom = top + theHeight - 1;

	intrpuzz->addToFront ( this );	 
}

void IntrPuzzFeature::dispose()
{
	if ( hotspot )
	{
		setHotspot ( NULL );
		hotspot = NULL;
	}

	intrpuzz->release ( this );
	deathList->add(this);
}

/***********************************
 *		
 *		Puzzle Prop	  
 *		      
 **********************************/
IntrPuzzPage::IntrPuzzPage()
{
	site = NULL;
}

void IntrPuzzPage::init(int pagenum)
{
	intrpuzz->addToFront(this);
	intrpuzz->page = this;

	if  (pagenum != WEB_REPORT)
	{
		if (pagenum != WEB_HOME && pagenum != WEB_COM && pagenum != WEB_DATA && pagenum != WEB_NOTE && 
			pagenum != WEB_TRAVEL && pagenum != WEB_TARGET && pagenum != WEB_VLINKS && pagenum != WEB_HTMLPAGE)
			lasttool = pagenum;
		helppage = 92800;
		if (!usedbutton)
		{
	//		site = webmap->findPrevInst(pagenum);

	//		if (!site)
	//		{
				site = webmap->findPage();
				if (site)
					webmap->dumpAfter();

				site = new (Website);
				site->flag = pagenum;
				site->name = "current";
	//		}
	//		else
	//		{
	//			site->name = "current";
	//			webmap->dumpAfter();
	//			site->name = "current";
	//		}
	 	}
		else
		{
			site = webmap->findPage();
			usedbutton = false;
		}

	 	if (!strcmp(webmap->head->data->name, "current"))
	    {
	    	fwdbutton = false;
	    } 	
	    if (!strcmp(webmap->tail->data->name, "current"))
	    {
	    	backbutton = false;
	    } 	
	}
}

void IntrPuzzPage::init(char * pagename)
{
	intrpuzz->addToFront(this);
	intrpuzz->page = this;

	helppage = 92800;
	if (!usedbutton)
	{
//		site = webmap->findPrevInst(pagename);

//		if (!site)
//		{
			site = webmap->findPage();
			if (site)
				webmap->dumpAfter();

			site = new (Website);
			site->flag = WEB_HTMLPAGE;
			site->name = "current";
			Common::strcat_s(site->htmlname, pagename);
//		}
//		else
//		{
//			site->name = "current";
//			webmap->dumpAfter();
//			site->name = "current";
//		}
 	}
	else
	{
		site = webmap->findPage();
		usedbutton = false;
	}

 	if (!strcmp(webmap->head->data->name, "current"))
    {
    	fwdbutton = false;
    } 	
    if (!strcmp(webmap->tail->data->name, "current"))
    {
    	backbutton = false;
    } 	
}

void IntrPuzzPage::saveState()
{

}

void IntrPuzzPage::dispose()
{
	intrpuzz->release ( this );
	deathList->add(this);
}
/***********************************
 *		
 *		Puzzle View	  
 *		      
 **********************************/
void IntrPuzzView::init(int theX, int theY, int pic_id, int loop_id, int cel_id)
{
	x = theX;
	y = theY;
	view = pic_id;
	loop = loop_id;
	cel = cel_id;
	draw();
	intrpuzz->addToFront ( this );
}	

void IntrPuzzView :: init(int theX,int theY)
{
	x = theX;
	y = theY;
	draw();
	intrpuzz->addToFront ( this );
}

void IntrPuzzView :: dispose()
{
	if ( hotspot )
	{
		setHotspot ( NULL );
		hotspot = NULL;
	}

	intrpuzz->release ( this );
	deathList->add(this);
}
 
void IntrPuzzView::draw()
{	
	Viewport* port;
	
	myATS = (ATS*) sfxLoadRes(view, RES_ATS);
	sfxLockRes(view, RES_ATS);
	//numLoops = ats->numReels;
	//numCels = ats->reels[loop]->numFrames;
//	port = myATS->reels[loop]->frames[cel];
	port = sfxAnimFrame(myATS, loop, cel);
	
	mySprite = sfxCreateSprite ( intrpuzz->display_buffer, x, y, 512, 512, port );
	
	
	if(mySprite == NULL)	{
		sfxPrintf("Couldn't create sprite in View::draw()");
		return;
	}

	sfxAttachATS2Sprite( mySprite, myATS, loop, cel);
	setRect();
}

/***********************************
 *		
 *		 Puzzle Interface
 *		      
 **********************************/
void IntrPuzz :: show()
{
}

void IntrPuzz :: hide()
{
  
}

IntrPuzz::IntrPuzz()
{
	name = "InterPuzz";
	flag = currentFocus;
	currentFocus = PUZZLE_FOCUS;
	puzzle_focus = 0;
	backbutton = false;
	fwdbutton = false;
	soundNum = 0;
	musicNum = 0;
	page = 0;
	if (sound1->isPlaying())	{
		soundNum = sound1->number;
		if (sound1->flag)
			isIntrBuffered = true;
		sound1->stop();
	}

	if (theMusic->isPlaying())	{
			musicNum = theMusic->number;
			theMusic->stop();
	}
	if ((!strcmp(curRoom->name, "Rm1000")) && (rm1000->musicHandle != -1))	{	
		musicNum = ((Rm1000 *)curRoom)->musicNumber;
		sfxStopMusic( rm1000->musicHandle);
	}
	
		
	caller = 0;
	intrface->disable();
  	if (pdaProp) {
		intrface->blinking = false;
		intrface->release(pdaProp);
		pdaProp->setHotspot(NULL);
		pdaProp->setCycle(0);
		delete pdaProp;
		pdaProp = NULL;
	}
}

void IntrPuzz::init(int pic_num)
{
	display_rect.top = BACKGROUND_TOP;
	display_rect.left = BACKGROUND_LEFT;
	display_rect.bottom = BACKGROUND_BOTTOM;
	display_rect.right = BACKGROUND_RIGHT;
	display_buffer = -1;
	show_style = VE_FLIP;
	number = -1;

	webmap = new (Webmap);
	web_intrface = new(List);
	announceList = new (List);
	usedList = new (List);
	intrpuzz = this;
	drawPic(pic_num);	

	theGame->display_buffer = display_buffer;

	//this should be in web.cpp
	puzzle_hotspots = new(HotspotList);
	oldlist = hotspots;
	hotspots = puzzle_hotspots;

}

IntrPuzz :: ~IntrPuzz()
{
	intrpuzz = NULL;
}

void IntrPuzz::dispose()
{
	if (page)	{
		page->dispose();
		page = 0;
	}
	
	if(!intrpuzz->isEmpty())
		List::dispose();

	if ( !hotspots->isEmpty() )
		hotspots->releaseAll();
/*
	if (!usedbutton && webmap)
	{
		webmap->clearFlags();
	}
*/	
	backbutton = true;

	if (homobutt)
		homobutt->setCel(0);
}

void IntrPuzz::doit()
{
	Node *n = head;
	Node *tmp;
	while ( n ) {
		tmp = n;
		n = n->next;
		tmp->data->doit();
	}

	if (web_intrface)		 
		web_intrface->doit();
	
	Object::doit();
}

void IntrPuzz::quitIntrface()
{
									    
	currentFocus = flag;
	
	if(!web_intrface->isEmpty())
		web_intrface->dispose();
	
	delete web_intrface;
	web_intrface = NULL;	

	comlinkshort = NULL;
	homobutt = NULL;

	webmap->dispose();
	delete webmap;
	webmap = NULL;	

	if(!announceList->isEmpty())
		announceList->dispose();

	delete announceList;
	announceList = NULL;	

	if(!usedList->isEmpty())
		usedList->dispose();

	delete usedList;
	usedList = NULL;	

	IntrPuzz::dispose();
	
	hotspots = oldlist;

	if (deathList->size)	
		deathList->dispose();

	
	if ( display_buffer >= 0 ) {
		sfxKillDisplayBuffer ( display_buffer );
		display_buffer = -1;
	}
	number = -1;
//	sfxSetSpritelistSize(64);
	
	
	sfxVisualEffect ( show_style, curRoom->display_buffer, 0, 0 );

	theGame->display_buffer = curRoom->display_buffer;
	
	if (caller)
	{
		caller->cue();
		caller = 0;
	}
	
	if (soundNum)	{
		sound1->loop = -1;
		if (isIntrBuffered)
			sound1->playBuffered(soundNum, NULL);
		else
			sound1->play(soundNum, NULL);
		soundNum = 0;
		isIntrBuffered = false;
	}

	if (musicNum)	{
		if (!strcmp(curRoom->name, "Rm1000"))	
			rm1000->musicHandle = sfxPlayMusic( musicNum, 50, 0, 100, 0, -1, NULL );
		else	{
			theMusic->loop = -1;
			theMusic->play( musicNum );
			musicNum = 0;
		}
	}
	
	if (GameFlag.test(fGotRealProcatFile) && !GameFlag.test(fTom5))	{
	//	inventry->get(iNUMBERTOBLAKE);
		GameFlag.set(fTom5);
		new VLink( "Teleconference: Holt, David M.", "100.avi", 0, 20);
	}

	if (GameFlag.test(fSentProcatDachaMsg))
		GameFlag.set(fDachaAvailable);			// set dacha available after sending message

	intrface->enable();
	if (theGame->enableIntrDuringMovie) {
		sfxSystemCursor(SYS_WAIT);
		user->canInput(true);
		if (!GameFlag.test(fTravelLinkAvail) && !strcmp(curRoom->name, "Rm1270"))	{ //special case for monument
			theMovie->fromTo(60, 185);
			theMovie->play("1271.avi", curRoom, 0, 0, 0);
		}
		/*else if (!strcmp(curRoom->name, "Rm1000"))	{	
			goingBackwards = true;
			rm1000->showRegion(curMap);
		}*/
	}
	normalCursor->cel = 0;
//	if (GameFlag.test(fYuriPh))	  
//		inventry->get(iNUMBERTOYURI);
	if ((GameFlag.test(fTargetFreqAvailable) && GameFlag.test(fTargetGPSAvailable)) ||
		(GameFlag.test(fPBMedDownload) && GameFlag.test(fTargetGPSAvailable)) ||
		 GameFlag.test(fPBShortDownload))
        if (!strcmp(curRoom->name, "Rm90000") || !strcmp(curRoom->name, "Rm90001"))
        { 
                curRoom->setScript(new goTo91000); 	// Transfer to Tarmac after puzzle done
        }

	if (GameFlag.test(fLinus23))
	{
		GameFlag.clear(fLinus23);
		if (!strcmp(curRoom->name, "Rm93020"))		// Transfer to Heli after receiving vmail
			curRoom->setScript(new ToHeliTransfer);
	}

	delete this;
}

void IntrPuzz::drawPic ( int pic_num )
{
	
	if (deathList->size)	
			deathList->dispose();
	

	if ( display_buffer >= 0 ) {
		sfxKillDisplayBuffer ( display_buffer );
		display_buffer = -1;
	}
	
//	number = sfxMakeResourceID ( theGame->project_num, pic_num );
//	if (number == NULL)
//		sfxPrintf("Couldn't make resourceID in Room::drawPic");

	display_buffer = sfxMakeDisplayBuffer ( pic_num, &display_rect, 0, 0 );
	if ( display_buffer == -1 ) {
		sfxPrintf ( "Fatal: cannot make a valid buffer, IntrPuzz::drawPic" );
		sfxQuit();
	}
	else {
		//sfxReleaseSprites( backgrounds[curBack]);
		//sfxUpdate();
		sfxVisualEffect ( show_style, display_buffer, 0, 0 );
	}
}

int IntrPuzz::handleEvent ( MADEEventStamp *event )
{
	if (dumping)
	{
		changePause();
		dumping = false;
		return true;
	}	
	if (puzzle_focus == DIALOGBOX_FOCUS)
	{
		dialogList->handleEvent(event);
		return true;
	}		
  	
	if ((comlinks) && (!comlinks->isEmpty()) && comlinkshort)
	{
		if(!comlinkshort->linkflash && !(comlinkshort->flag & OBJECT_HIDDEN))	
			new(LinkFlash);
	}			 
/* 	if (event_type == USER_LEFT_UP && user->input)
	{
		clear_sliderDown(event);
	}
*/
	if (anal)
	{
		if (!anal->playing)
    	{
		    if ( hotspots && user->input )
				  hotspots->doit();

			if(web_intrface->handleEvent(event))
				return true;

			if(List::handleEvent(event))
				return true; 
		}
	}
	else if (knee)
	{
		if (!knee->playing)
    	{
		    if ( hotspots && user->input )
				  hotspots->doit();

			if(web_intrface->handleEvent(event))
				return true;

			if(List::handleEvent(event))
				return true; 
		}
	}
	else
	{
	    if ( hotspots && user->input )
			  hotspots->doit();

		if(web_intrface->handleEvent(event))
			return true;

		if(List::handleEvent(event))
			return true; 
	}

	if(List::handleEvent(event))
		return true; 
	return false;			
}

void IntrPuzz::clear_sliderDown(MADEEventStamp *event)
{
	Node *n = head;

	while ( n ) {
		if ( n->data->flag != 0)	   //only if the object is a slider
			n->data->handleEvent( event );  //call the slider handle event
		n = n->next;
	}
}
	
void IntrPuzz::remove_MarkedItems()
{
	Node *n = head;
	Node *tmp;  ///TOM

	while ( n ) {
 		tmp = n;
		n = n->next;
 		if ( strcmp(tmp->data->name, "delete" ) == 0)	   //only if the object is a button
			tmp->data->dispose();  //call the Button dispose function
		//n = n->next; //TOM
	}
}

int IntrPuzz::isStateSaved(const char *saved_name) {
	Node *n = web_intrface->head;

	while ( n )
	{
		if ( strcmp(n->data->name, saved_name ) == 0)	   
		{
			return true;
		}
		n = n->next;
	}
	return false;
}

Website * Webmap::findPrevPage()
{
	Node *n = head;

	while ( n )
	{
		if ( !strcmp(n->data->name, "current" ))	
		{
			if (n != tail)
			{
				n->data->name = "";  
				n = n->next;
				n->data->name = "current";
				return ((Website *)n->data);
			}
			else
			{
				backbutton = false;
				return nullptr;
			}
		}
		n = n->next;
	}

	return nullptr;
}

Website * Webmap::findNextPage()
{
	Node *n = head;

	while ( n )
	{
		if ( !strcmp(n->data->name, "current" ))	
		{
			if (n != head)
			{
				n->data->name = "";  
				n = n->prev;
				n->data->name = "current";
				return ((Website *)n->data);
			}
			else 
			{
				fwdbutton = false;
				return nullptr;
			}
		}
		n = n->next;
	}

	return nullptr;
}

Website * Webmap::findPage()
{
	Node *n = head;

	while ( n )
	{
		if ( !strcmp(n->data->name, "current" ))	
		{
			return (Website *)n->data;
		}
		n = n->next;
	}

	return nullptr;
}

void Webmap::dumpAfter()
{
	Node *n = head;
	Node *tmp;  //TOM
	while ( n )
	{
		tmp = n; //TOM
		n = n->next;
		if ( !strcmp(tmp->data->name, "current" ))	
		{
			break;
		}
		else
		{
			tmp->data->dispose();
		//	n = n->next; //TOM
		}
	}	
	webmap->clearFlags();												    
	return;
}

Website * Webmap::findPrevInst(int pagenum)
{
	Node *n = head;

	while ( n )
	{
		if ( (int)n->data->flag == pagenum)	
		{
			return ((Website *)n->data);
		}
		n = n->next;
	}

	return nullptr;
}

Website * Webmap::findPrevInst(char * pagename)
{
	Node *n = head;										 

	while ( n )
	{
		if ( !strcmp((char*)(&((Website *)n->data)->htmlname), pagename ))	
		{
			return ((Website *)n->data);
		}
		n = n->next;
	}

	return nullptr;
}

Website::Website()
{
	memset (&htmlname,0,sizeof(char));
	webmap->addToFront ( this );	 
}
void Website::dispose(void)
{
	webmap->release(this);
	deathList->add(this);
}

void Webmap::clearFlags()
{
	Node *n = head;

	while ( n )
	{
		if ( !strcmp(n->data->name, "current" ))	
		{
			n->data->name = "";
		}
		n = n->next;
	}
}


/***********************************
 *		
 *		 Prop
 *		      
 **********************************/
IntrPuzzProp::IntrPuzzProp()
{
	name = "Prop";
	cycler = NULL;
	cycleSpeed = 10;
}

IntrPuzzProp::~IntrPuzzProp()
{
	if ( cycler ) {
		delete cycler;
		cycler = NULL;
	}
}

void IntrPuzzProp::setCycle ( Cycle* theCycle )
{
	if ( cycler )
		delete cycler;

	cycler = theCycle;
}

void IntrPuzzProp::doit()
{

	View::doit();

	if ( cycler && ( ( gameTime - __cycle_cnt ) > (unsigned)cycleSpeed ) ) {
		__cycle_cnt = gameTime;
	
		int oldCel = cel;
		cycler->doit();
		if  (( oldCel != cel) && (cycler))	{
			//sfxSpriteFrame(mySprite, myATS->reels[loop]->frames[cel]);
			sfxSpriteFrame(mySprite, sfxAnimFrame(myATS, loop, cel));
			setRect();
		}
	}
}
void IntrPuzzProp :: dispose()
{
	setCycle(NULL);
	IntrPuzzView::dispose();
}

/***********************************
 *		
 *		 Actor
 *		      
 **********************************/

IntrPuzzActor::IntrPuzzActor()
{
	name = "Actor";
	xInc = 1;
	yInc = 1;
	moveSpeed = 6;
	mover = NULL;
}

void IntrPuzzActor::doit()
{

	IntrPuzzProp::doit();

	if ( mover && ( ( gameTime - __move_cnt ) > (unsigned)moveSpeed ) ) {
		__move_cnt = gameTime;
		mover->doit();
	}
}

void IntrPuzzActor::setSpeed(int theSpeed)
{
	moveSpeed = theSpeed;
}

void IntrPuzzActor::setInc(int theXInc, int theYInc)
{
	xInc = theXInc;
	yInc = theYInc;
}	
	
IntrPuzzActor::~IntrPuzzActor()
{
	if ( mover ) {
		delete mover;
		mover = NULL;
	}
}

void IntrPuzzActor::setMotion ( MoveTo* theMover )
{
	if ( mover )
		delete mover;

	mover = theMover;
}

/* Usage example
t = new(TestActor);
t->setMotion(new MoveTo(100, 200, t));

TestActor::TestActor()
{
	view = 930;
	init(30, 30);
	setInc(1,1);
	setSpeed(0);
}
*/

/***********************************
 *		
 *		 Actor
 *		      
 **********************************/
IntrPuzzTimer::IntrPuzzTimer()
{
	name = "IntrPuzzTimer";
	cycles = ticks = seconds = 0;
	old_ticks = 0;
	old_seconds = 0;
	caller = NULL;
}

void IntrPuzzTimer::pause ( int tOrF )
{
	if ( tOrF )
		intrpuzz->release ( this );
	else
		intrpuzz->add ( this );
}

void IntrPuzzTimer::setCycles ( int c )
{
	cycles = c;
	intrpuzz->add ( this );
}

void IntrPuzzTimer::setCycles ( int cy, Object *c )
{
	caller = c;
	cycles = cy;
	intrpuzz->add ( this );
}

void IntrPuzzTimer::setTicks ( int t )
{
	ticks = t;
	intrpuzz->add ( this );
}

void IntrPuzzTimer::setTicks ( int t, Object *c )
{
	caller = c;
	ticks = t;
	intrpuzz->add ( this );
}

void IntrPuzzTimer::setReal ( int s )
{
	seconds = s;
	intrpuzz->add ( this );
}

void IntrPuzzTimer::setReal ( int s, Object *c )
{
	caller  = c;
	seconds = s;
	intrpuzz->add ( this );
}

void IntrPuzzTimer::doit()
{
	long t;

	if ( cycles > 0 ) {
		cycles --;
		if ( cycles <= 0 ) {
			cue();
		}
	}

	if ( seconds > 0 ) {
		if ( !old_seconds )
			old_seconds = gameTime / 60;
		t = gameTime / 60;
		seconds -= ( t - old_seconds );
		old_seconds = t;
		if ( seconds <= 0 ) {
			seconds = 0;
			cue();
		}
	}

	if ( ticks > 0 ) {
		if ( !old_ticks )
			old_ticks = gameTime;
		ticks -= ( gameTime - old_ticks );
		old_ticks = gameTime;
		if ( ticks <= 0 ) {
			ticks = 0;
			cue();
		}
	}	
}

void IntrPuzzTimer::cue()
{
	if ( caller )
		caller->cue();

	dispose();
}

void IntrPuzzTimer::dispose()
{
	intrpuzz->release ( this );
	deathList->add ( this );
}

} // namespace Spycraft
