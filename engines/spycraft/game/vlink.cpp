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

#include "spycraft/game/vlink.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/cypher.h"
#include "spycraft/game/web.h"
#include "spycraft/game/pcscreen.h"
#ifdef DIRECTX
#include "spycraft/dmade/advmovie.h"
#define YAKE     !sfxIsMovieOpen()
#else
#define YAKE     !sfxIsMCIOpen()
#endif

namespace Spycraft {

#define MAILLOG				1
#define ARCHIVELOG			2

#define PAGE_BOTTOM			340
#define VIDEOWIN			4
#define MSGWIN				403
#define MAILWIN				404
#define ARCHIVEWIN			405

#define VIDEOICON			0
#define MAILICON			1
#define VOICEICON			2

//DEBUG
int iconAllocated = 0;

SelectBar *selectbar;

int winPosnX[30] = {372, 332, 280, 245, 207, 160, 116, 60, 30, 30,
 					425, 370, 325, 270, 225, 175, 124, 75, 30, 30,
 					385, 335, 290, 250, 206, 161, 117, 75, 30, 31};

MADEPt dotPosn[33] = {{400, 227}, {380, 212}, {356, 182}, {319, 147}, {274, 116}, {217,91},
					 {188, 100}, {177, 136}, {181, 176}, {206, 248}, {241, 320},
					 {176, 328}, {154, 256}, {146, 178}, {156, 126}, {177, 106}, {223,110},
					 {273, 130}, {315, 155}, {354, 186}, {380, 212}, {400, 227},
					 {400, 227}, {380, 212}, {353, 191}, {313, 164}, {273, 142}, {222,126},
					 {179, 120}, {145, 126}, {114, 160}, {105, 238}, {122, 336}};

Vplay*			vplay;
ComWindow*		window;
ComItemList* 	comitemList;
EMailPage*		emailpage;
ComLinkInfo*	comlinkinfo;
VidWindow* 		vWindow;
VideoCloseButt*	vCloseButt;

int currentEmail;
int currentLog;

extern DialTree* dialogTree;
extern int currentFocus;
//MessageSound* msgSound = new(MessageSound);  //global sound for comlink

/***********************************
 *		
 *		 Global mail sound
 *		      
 **********************************/
void MessageSound::play(int soundNumber)
{
	if ( isPlaying() )
		return;

	Sound::play(soundNumber, NULL);
}

/***********************************
 *		
 *		 Base Mail class
 *		      
 **********************************/
ComData::ComData()
{
	myFlag = 0;
	caller = 0;
	myDelay = 0;
	caller = 0;
	ready = true;
}

void ComData::dispose()
{
	if(currentLog == MAILLOG)
	{
		comlinks->release ( this );
		deathList->add ( this );
	}

	if (caller)
		intrpuzz->caller = caller;
	if (myFlag)
		GameFlag.set(myFlag); 
}

int ComData::isReady()
{
	if ((!myDelay) || ((gameTime - time0) > myDelay))
		return true;
	else
		return false;
}
		   
/***********************************
 *		
 *		 Voice mail 
 *		      
 **********************************/
VoiceMail::VoiceMail(const char *title, int soundnum, int ID, int archive) {
	comType = VOICEMAIL;
	Common::strcpy_s(msgName, title);	
	soundNumber = soundnum;
	idNumber = ID;

	if(!archiveList->isOnList(this))
	{
		if(archive == 2)
			archiveList->add( this );
		else
			archiveList->addToFront ( this );
	}
	else
		delete this;
}

VoiceMail::VoiceMail(const char *title, int soundnum, int ID)
{
	comType = VOICEMAIL;
	Common::strcpy_s(msgName, title);	
	soundNumber = soundnum;
	idNumber = ID;
	if(!comlinks->isOnList(this))
	{
		comlinks->addToFront ( this);
		if(user->input)
		{
			if(!sound2->isPlaying() && YAKE && !GameFlag.test(fDCK10))
				sound2->play(40002, NULL);	
		}
	}
	else
		delete this;
}

VoiceMail::VoiceMail(const char *title, int soundnum, int ID, int flag, int delay)
{
	comType = VOICEMAIL;
	Common::strcpy_s(msgName, title);	
	soundNumber = soundnum;
	idNumber = ID;
	myFlag = flag;
	myDelay = delay * 60;
	time0 = gameTime;

	if(!comlinks->isOnList(this))
	{
		ready = false;
		comlinks->addToFront ( this);
	}
	else
		delete this;	
}

/***********************************
 *		
 *		 Email 
 *		      
 **********************************/
EMail::EMail(const char *title, int pagenum, int archive)
{
	comType = EMAIL;
	Common::strcpy_s(msgName, title);	
	pageNumber = pagenum;
	if(!archiveList->isOnList(this))
	{
		if(archive == 2)
			archiveList->add( this );
		else
			archiveList->addToFront ( this );
	}
	else
		delete this;
}

EMail::EMail(const char *title, int pagenum)
{
	comType = EMAIL;
	Common::strcpy_s(msgName, title);	
	pageNumber = pagenum;
	if(!comlinks->isOnList(this))
	{
		comlinks->addToFront ( this);
		if(user->input)
		{
			if(!sound2->isPlaying() && YAKE && !GameFlag.test(fDCK10))
				sound2->play(40002, NULL);	
		}
	}
	else
		delete this;
}

EMail::EMail(const char *title, int pagenum, int flag, int delay)
{
	comType = EMAIL;
	Common::strcpy_s(msgName, title);	
	pageNumber = pagenum;
	myFlag = flag;
	myDelay = delay * 60;
	time0 = gameTime;
	if(!comlinks->isOnList(this))
	{
		ready = false;
		comlinks->addToFront ( this);
	}
	else
		delete this;	
}

EMailAttachment::EMailAttachment(const char *title, int pagenum)
{
	comType = EMAIL;
	Common::strcpy_s(msgName, title);	
	pageNumber = pagenum;
//	comlinks->add ( this );
}

EMailAttachment::EMailAttachment(const char *title, int pagenum, int flag, int delay)
{
	comType = EMAIL;
	Common::strcpy_s(msgName, title);	
	pageNumber = pagenum;
	myFlag = flag;
	myDelay = delay * 60;
	time0 = gameTime;
//	comlinks->add ( this );
//	msgSound->play(40001);	
}

/***********************************
 *		
 *		 VLinks 
 *		      
 **********************************/
VLink::VLink(const char *moviename, EMailAttachment *attachment, int archive) {
	comType = VLINKMAIL;
	emailAttachment = attachment;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,moviename);   //temporary for vlink name
	if(!archiveList->isOnList(this))
	{
		if(archive == 2)
			archiveList->add( this );
		else
			archiveList->addToFront ( this );
	}
	else
		delete this;
}

VLink::VLink(const char *moviename)
{
	comType = VLINKMAIL;
	emailAttachment = NULL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,moviename);   //temporary for vlink name
	if(!comlinks->isOnList(this))
	{
		comlinks->addToFront ( this);
		if(user->input) 
		{
			if(!sound2->isPlaying() && YAKE && !GameFlag.test(fDCK10))
				sound2->play(40002, NULL);	
		}
	}
	else
		delete this;
}

VLink::VLink(const char *moviename, int theFlag, Object* whoToCue)
{
	comType = VLINKMAIL;
	emailAttachment = NULL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,moviename);   //temporary for vlink name
	myFlag = theFlag;
	caller = whoToCue;
	if(!comlinks->isOnList(this))
	{
		comlinks->addToFront ( this);
		if(user->input)
		{
			if(!sound2->isPlaying() && YAKE && !GameFlag.test(fDCK10))
				sound2->play(40002, NULL);	
		}
	}
	else
		delete this;
}

VLink::VLink(const char *moviename, int theFlag, int delay)
{
	comType = VLINKMAIL;
	emailAttachment = NULL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,moviename);   //temporary for vlink name
	myFlag = theFlag;
	myDelay = delay * 60;
	time0 = gameTime;
	if(!comlinks->isOnList(this))
	{
		ready = false;
		comlinks->addToFront ( this);
	}
	else
		delete this;	
}

VLink::VLink(const char *moviename, EMailAttachment *attachment)
{
	comType = VLINKMAIL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,moviename);   //temporary for vlink name
	emailAttachment = attachment;
	if(!comlinks->isOnList(this))
	{
		comlinks->addToFront ( this);
		if(user->input)
		{
			if(!sound2->isPlaying() && YAKE && !GameFlag.test(fDCK10))
				sound2->play(40002, NULL);	
		}
	}
	else
		delete this;
}

VLink::VLink(const char *moviename, int theFlag, int delay, EMailAttachment *attachment)
{
	comType = VLINKMAIL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,moviename);   //temporary for vlink name
	myFlag = theFlag;
	myDelay = delay * 60;
	time0 = gameTime;
	emailAttachment = attachment;
	if(!comlinks->isOnList(this))
	{
		ready = false;
		comlinks->addToFront ( this);
	}
	else
		delete this;	
//	msgSound->play(40001);	
}


/***********************************
 *		
 *		 these are the new vlink contructors (VLinks) 
 *		      
 **********************************/
VLink::VLink(const char *title, const char *moviename, EMailAttachment *attachment, int archive)
{
	comType = VLINKMAIL;
	emailAttachment = attachment;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,title);   //temporary for vlink name
	
	if(!archiveList->isOnList(this))
		archiveList->addToFront ( this );
	else
		delete this;
}

VLink::VLink(const char *title, const char *moviename)
{
	comType = VLINKMAIL;
	emailAttachment = NULL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,title);   //temporary for vlink name
	if(!comlinks->isOnList(this))
	{
		comlinks->addToFront ( this);
		if(user->input)
		{
			if(!sound2->isPlaying() && YAKE && !GameFlag.test(fDCK10))
				sound2->play(40002, NULL);	
		}
	}
	else
		delete this;
}

VLink::VLink(const char *title, const char *moviename, int theFlag, Object *whoToCue) {
	comType = VLINKMAIL;
	emailAttachment = NULL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,title);   //temporary for vlink name
	myFlag = theFlag;
	caller = whoToCue;
	if(!comlinks->isOnList(this))
	{
		comlinks->addToFront ( this);
		if(user->input)
		{
			if(!sound2->isPlaying() && YAKE && !GameFlag.test(fDCK10))
				sound2->play(40002, NULL);	
		}
	}
	else
		delete this;
}

VLink::VLink(const char *title, const char *moviename, int theFlag, int delay)
{
	comType = VLINKMAIL;
	emailAttachment = NULL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,title);   //temporary for vlink name
	myFlag = theFlag;
	myDelay = delay * 60;
	time0 = gameTime;
	if(!comlinks->isOnList(this))
	{
		ready = false;
		comlinks->addToFront ( this);
	}
	else
		delete this;	
}

VLink::VLink(const char *title, const char *moviename, EMailAttachment *attachment)
{
	comType = VLINKMAIL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,title);   //temporary for vlink name
	emailAttachment = attachment;
	if(!comlinks->isOnList(this))
	{
		comlinks->addToFront ( this); 
		if(user->input)
		{
			if(!sound2->isPlaying() && YAKE && !GameFlag.test(fDCK10))
				sound2->play(40002, NULL);	
		}
	}
	else
		delete this;
}

VLink::VLink(const char *title, const char *moviename, int theFlag, int delay, EMailAttachment *attachment)
{
	comType = VLINKMAIL;
	Common::strcpy_s(movieName,moviename);
	Common::strcpy_s(msgName,title);   //temporary for vlink name
	myFlag = theFlag;
	myDelay = delay * 60;
	time0 = gameTime;
	emailAttachment = attachment;

	if(!comlinks->isOnList(this))
	{
		ready = false;
		comlinks->addToFront ( this);
	}
	else
		delete this;	
}

/***********************************
 *		
 *		 Max Interactive Vlink
 *		      
 **********************************/
Dialog* 	vlMaxDialog0;
Dialog* 	vlMaxDialog1;
Dialog*		vlMaxDialog2;
Dialog*		vlMaxDialog3;
Dialog*		vlMaxDialog4;
Dialog*		vlMaxDialog5;

maxDItem* offerHouse;
maxDItem* cooperate;
maxDItem* noIdeas; 
maxDItem* whatToDo; 
maxDItem* goHouse; 

enum	
{
	OFFERHOUSE,
	COOPERATE,
	NOIDEAS,
	WHATTODO,
	GOHOUSE
};

void ScriptMax::changeState( int newState )
{
	switchTo
		//flag = intrface->canBlink;
		//intrface->canBlink = false;
		theMovie->fromTo(0, 85);
		theMovie->play("101.avi", this, 0);
	END
	BEG
		dialogTree = new DialTree;
		vlMaxDialog0 = new Dialog;
		vlMaxDialog1 = new Dialog;
		vlMaxDialog2 = new Dialog;
		vlMaxDialog3 = new Dialog;
		vlMaxDialog4 = new Dialog;
		vlMaxDialog5 = new Dialog;

		offerHouse = new maxDItem;
		offerHouse->addInfo(25003, OFFERHOUSE, vlMaxDialog1);
		cooperate = new maxDItem;
		cooperate->addInfo(25003, COOPERATE, vlMaxDialog2 );
		noIdeas = new maxDItem;
		noIdeas->addInfo(25003, NOIDEAS, NULL);	//nowhere
		whatToDo = new maxDItem;
		whatToDo->addInfo(25003, WHATTODO, vlMaxDialog4);
		goHouse = new maxDItem;
		goHouse->addInfo(25003, GOHOUSE, vlMaxDialog5);

		vlMaxDialog1->addMovieInfo(135, 221, 0, 0);   
		vlMaxDialog2->addMovieInfo(330, 458, 0, 0);   
		vlMaxDialog3->addMovieInfo(330, 458, 0, 0);	
		vlMaxDialog4->addMovieInfo(465, 570, 0, 0);	
		vlMaxDialog5->addMovieInfo(225, 317, 0, 0);	

		if (GameFlag.test(fNoticedWestern))
		{
			vlMaxDialog0->add(offerHouse);
		}	
		vlMaxDialog0->add(cooperate);
//		vlMaxDialog0->add(noIdeas);

		vlMaxDialog2->add(whatToDo);
		if (GameFlag.test(fNoticedWestern))
		{		
			vlMaxDialog2->add(goHouse);
		}

		dialogTree->rootNode = vlMaxDialog0;
		dialogTree->resourceID = "101.avi";
		dialogTree->show(this);
	END
		
	BEG
		if(GameFlag.test(fWahKa109))
			GameFlag.set(fStickPath);
		else if(GameFlag.test(fWahKa110))
			GameFlag.set(fCarrotPath);
		GameFlag.set(fBSClosed);
		theMovie->caller = 0;
		theMovie->stop();
		dispose();
		currentFocus = PUZZLE_FOCUS;
		//intrface->canBlink = flag;
		sfxMadeCursor();
		client->cue();
	END
}
			   
void maxDItem::doit()
{
	vlinkDItem::doit();

	if(cooperate->hasBeenChosen)
	{
		GameFlag.set(fWahKa109);			//do stick path
///		GameFlag.set(fStickPath);		    //do stick path
		GameFlag.set(fDisplayStakout);
	}		
	if(offerHouse->hasBeenChosen)
	{
		GameFlag.set(fWahKa110);			//do carrot path
///		GameFlag.set(fCarrotPath);	  	 	//do carrot path
//		GameFlag.set(fDisplayKnee);
	}
	if(goHouse->hasBeenChosen)
	{
		GameFlag.clear(fWahKa109);
		GameFlag.set(fWahKa110);
///		GameFlag.clear(fStickPath);	
		GameFlag.clear(fDisplayStakout);
///		GameFlag.set(fCarrotPath);	  	 	//do carrot after stick path
//		GameFlag.set(fDisplayKnee);
	}
}	   

/***********************************
 *		
 *		 Holt Interactive vlink
 *		      
 **********************************/
Dialog* 	holtDialog0;
Dialog* 	holtDialog1;
Dialog*		holtDialog2;
Dialog*		holtDialog3;
Dialog*		holtDialog4;
Dialog*		holtDialog5;

vlinkDItem* aboutProcat;
vlinkDItem* aboutPawPrint;
vlinkDItem* afterPawPrint; 
vlinkDItem* procatOperating; 
vlinkDItem* aboutAccident; 

enum	
{
	ABOUTPROCAT,
	ABOUTPAWPRINT,
	AFTERPAWPRINT,
	PROCATOPERATING,
	ABOUTACCIDENT
};

void ScriptHolt::changeState( int newState )
{
	switchTo
		flag = intrface->canBlink;
		intrface->canBlink = false;
		theMovie->fromTo(0, 56);
		theMovie->play("100.avi", this, 0);
	END
	BEG
		dialogTree = new DialTree;
		holtDialog0 = new Dialog;
		holtDialog1 = new Dialog;
		holtDialog2 = new Dialog;
		holtDialog3 = new Dialog;
		holtDialog4 = new Dialog;
		holtDialog5 = new Dialog;

		aboutProcat = new vlinkDItem;
		aboutProcat->addInfo(25002, ABOUTPROCAT, holtDialog1);
		aboutPawPrint = new vlinkDItem;
		aboutPawPrint->addInfo(25002, ABOUTPAWPRINT, holtDialog2 );
		afterPawPrint = new vlinkDItem;
		afterPawPrint->addInfo(25002, AFTERPAWPRINT, holtDialog3 );
		procatOperating = new vlinkDItem;
		procatOperating->addInfo(25002, PROCATOPERATING, holtDialog4 );
		aboutAccident = new vlinkDItem;
		aboutAccident->addInfo(25002, ABOUTACCIDENT, holtDialog5 );

//		holtDialog1->addMovieInfo(55, 117, 116, 117);   
//		holtDialog2->addMovieInfo(325, 488, 322, 323);  
//		holtDialog3->addMovieInfo(121, 323, 323, 323);	
//		holtDialog4->addMovieInfo(492, 805, 804, 805);	
//		holtDialog5->addMovieInfo(809, 1072, 1071, 1072);

		holtDialog1->addMovieInfo(60, 127, 0, 0);   
		holtDialog2->addMovieInfo(330, 507, 0, 0);  
		holtDialog3->addMovieInfo(135, 327, 0, 0);	
		holtDialog4->addMovieInfo(510, 827, 0, 0);	
		holtDialog5->addMovieInfo(840, 1108, 0, 0);

		holtDialog0->add(aboutProcat);

		holtDialog1->add(aboutPawPrint);
		holtDialog1->add(afterPawPrint);

		holtDialog2->add(afterPawPrint);
		holtDialog2->add(procatOperating);
		//holtDialog2->add(aboutAccident);

		holtDialog3->add(aboutPawPrint);
		holtDialog3->add(procatOperating);
		//holtDialog3->add(aboutAccident);

		holtDialog4->add(afterPawPrint);
		holtDialog4->add(procatOperating);
		//holtDialog4->add(aboutAccident);
  
		holtDialog5->add(procatOperating);
		holtDialog5->add(aboutPawPrint);
		holtDialog5->add(afterPawPrint);

		dialogTree->rootNode = holtDialog0;
		dialogTree->resourceID = "100.avi";
		dialogTree->show(this);
	END
		
	BEG
		theMovie->caller = 0;
		theMovie->stop();
		dispose();
		GameFlag.set(fSaw100);
		sfxMadeCursor();
		intrface->canBlink = flag;
		currentFocus = PUZZLE_FOCUS;
		client->cue();
	END
}

/***********************************
 *		
 *		 Archive List (video, email, and voice links)
 *		      
 **********************************/
int ArchiveList::isOnList ( Object *obj)
{
	if ( contains ( obj ) )
		return false;

	Node* linkNode = head;
	int objType = ((ComData*)obj)->comType;

	while ( linkNode )
	{
		switch(((ComData*)linkNode->data)->comType)
		{
			case VLINKMAIL:
				if(objType == VLINKMAIL)
					if(strcmp(((VLink*)linkNode->data)->movieName, ((VLink*)obj)->movieName) == 0)
						return true; 	 
			break;

			case EMAIL:
				if(objType == EMAIL)
					if(((EMail*)linkNode->data)->pageNumber == ((EMail*)obj)->pageNumber)
						return true; 	 
			break;

			case VOICEMAIL:
				if(objType == VOICEMAIL)
					if(((VoiceMail*)linkNode->data)->soundNumber == ((VoiceMail*)obj)->soundNumber)
						return true; 	 
			break;
		}

		linkNode = linkNode->next;
	}

	return false;
}

/***********************************
 *		
 *		 Global Communication List (video, email, and voice links)
 *		      
 **********************************/
int ComLinks::isEmpty() {
	bool empty = true;

	if (List::isEmpty())
		return true;
	else
	{
		Node *n = head;
		while ( n )
		{
			if (((ComData*)n->data)->isReady())//timer ran out or there was no delay
			{      
				if(!((ComData*)n->data)->ready)
				{
					((ComData*)n->data)->ready = true;

					if(!sound2->isPlaying() && YAKE && !GameFlag.test(fDCK10))
						sound2->play(40002, NULL);	
					return false;
				}
				empty = false;	
			}
			n = n->next;
		}

		if(!empty)
			return false;
	}

	return true;
}

int ComLinks::isOnList(Object *obj) {
	if ( contains ( obj ) )
		return false;

	Node* linkNode = head;
	int objType = ((ComData*)obj)->comType;

	while ( linkNode )
	{
		switch(((ComData*)linkNode->data)->comType)
		{
			case VLINKMAIL:
				if(objType == VLINKMAIL)
					if(strcmp(((VLink*)linkNode->data)->movieName, ((VLink*)obj)->movieName) == 0)
						return true; 	 
			break;

			case EMAIL:
				if(objType == EMAIL)
					if(((EMail*)linkNode->data)->pageNumber == ((EMail*)obj)->pageNumber)
						return true; 	 
			break;

			case VOICEMAIL:
				if(objType == VOICEMAIL)
					if(((VoiceMail*)linkNode->data)->soundNumber == ((VoiceMail*)obj)->soundNumber)
						return true; 	 
			break;
		}

		linkNode = linkNode->next;
	}
	return false;
}

int ComLinks::removeEMail(unsigned long idNumber)
{
	if (List::isEmpty())
	{
		return false;
	}
	else
	{
		Node *n = head;

		while ( n )
		{
			if(((ComData*)n->data)->comType == EMAIL)
			{
				if (((EMail*)n->data)->pageNumber == idNumber)
				{      
					comlinks->release( n->data );
					deathList->add ( n->data );
					return true;	
				}
			}
			n = n->next;
		}
	}
	return false;
}

int ComLinks::removeVoiceMail(unsigned long idNumber)
{
	if (List::isEmpty())
	{
		return false;
	}
	else
	{
		Node *n = head;

		while ( n )
		{
			if(((ComData*)n->data)->comType == VOICEMAIL)
			{
				if (((EMail*)n->data)->pageNumber == idNumber)
				{      
					comlinks->release( n->data );
					deathList->add ( n->data );
					return true;	
				}
			}
			n = n->next;
		}
	}
	return false;
}

int ComLinks::removeVLink(const char *movie) {
	if (List::isEmpty())
	{
		return false;
	}
	else
	{
		Node *n = head;

		while ( n )
		{
			if(((ComData*)n->data)->comType == VLINKMAIL)
			{
				if (strcmp(((VLink*)n->data)->movieName, movie) == 0)
				{      
					comlinks->release( n->data );
					deathList->add ( n->data );
					return true;	
				}
			}
			n = n->next;
		}
	}
	return false;
}
/***********************************
 *		
 * Com item List(contains all video, voice, email)
 * contains all of the text features     
 **********************************/
ComItemList::ComItemList()
{
	reserved = false;
}

void ComItemList::dispose()
{
	Node *t;
	Node *n = head;

	if ( reserved ) {
		sfxPrintf("Generic dispose reserved = %d (not returning from dispose", reserved); //DEBUG
//		#ifdef DO_DEBUG
//			LOG_ERROR(LIST_ERROR,reserved);
//		#endif
		//return;
	}

	reserved = 9;

	while ( n )
	{
		if ( n->data ) 
		{
			if(((MailItem*)n->data)->icon)
			{
				((MailItem*)n->data)->icon->dispose();
				((MailItem*)n->data)->icon = NULL;
			}					

	    	delete n->data;
		}
		t = n;
		n = n->next;
		delete t;
	}

	head = NULL;
	tail = NULL;
	size = 0;

	reserved = false;
}

void ComItemList::dispose(MailItem* thisItem)
{
	Node *t;
	Node *n = head;

	if ( reserved ) {
		sfxPrintf("Removing item's reserved = %d (not returning from dispose)", reserved); //DEBUG
//		#ifdef DO_DEBUG
//			LOG_ERROR(LIST_ERROR,reserved);
//		#endif
	//	return;
	}

	while ( n )
	{
		if ( (n->data) && ((MailItem *)n->data != thisItem))
		{
			((MailItem*)n->data)->icon->dispose();
	    	delete n->data;
		}

	 	t = n;
 		n = n->next;
		delete t;
	}
	head = NULL;
	tail = NULL;
	size = 0;
 
	reserved = false;
}

/***********************************
 *		
 *		 Puzzle Save Status
 *		      
 **********************************/
ComLinkInfo::ComLinkInfo()
{
	name = "comlinkinfo";
	web_intrface->addToFront(this); 
	comlinkinfo = this;
}

void ComLinkInfo::save()
{
	if(currentHFile)
	{
		emailNum = currentEmail;
	}
	else
	{
		emailNum = 0;
	}			
}	  

void ComLinkInfo::load()
{

	if(emailNum)
	{
		window->setView(MAILWIN);
		window->setCel(9);
		window->posn(winPosnX[9], 85);
		window->show();
 		vplay->closebutt->show();
		//vplay->mailbutt->setCel(2);  //grayed out button
		emailpage = new EMailPage(emailNum);
		vplay->wiredot = NULL;
	}
	else
	{
		vplay->wiredot = new(WireDot);
	}
}

void ComLinkInfo :: dispose()
{
	web_intrface->release ( this );
	deathList->add(this);
}

/***********************************
 *		
 *		 Dot Animations
 *		      
 **********************************/
WireDot::WireDot()
{
	name = "WireDot";
 	init(400, 227, 400, 5, 0);
	increment = 1;
	kill = false;

	totalFrames = 32; //yATS->reels[loop]->numFrames;
 	moveTimer = new (IntrPuzzTimer);
	moveTimer->setTicks(7,this);
}

void WireDot::cue(void)
{
	if(cel == totalFrames)
		increment = -1;

	if(cel == 0)
		increment = 1;

	setCel(cel + increment);
	posn(dotPosn[cel].x, dotPosn[cel].y);
	if(kill)
	{
		dispose();
		vplay->wiredot = NULL;
	}
	else
	{
	 	moveTimer = new (IntrPuzzTimer);
		moveTimer->setTicks(7,this);
	}
}

/***********************************
 *		
 *		 Com Window
 *		      
 **********************************/
ComWindow::ComWindow()
{
	name = "comWindow";
	init(0, 0, 403, 0, 0);
	hide();
	window = this;
}

void ComWindow::cue(void)
{
	if(endCel == cel)
	{
		if(endCel == 0)  //the window is closed
			hide();

		if(endCel == 9)	//window open
			vplay->closebutt->show();

		if(caller)
			caller->cue();
	}
	else
	{
		setCel(cel + increment);
		posn(winPosnX[currentWindowOffset + cel], 85);
		moveTimer = new (IntrPuzzTimer);
		moveTimer->setTicks(4,this);
	}		
}
		 
void ComWindow::close(Object* whoToCue)
{
	endCel = 0;
 	increment = -1;
	currentWindowOffset = (view - 403) * 10; //7 is the offset to the first window loop
 	caller = whoToCue;
	vplay->closebutt->hide();
	if(vplay->prevpage)
	{
		vplay->prevpage->prevCL->dispose();
		vplay->prevpage->dispose();
		vplay->prevpage = NULL;
	}
	if(vplay->nextpage)
	{
		vplay->nextpage->nextCL->dispose();
		vplay->nextpage->dispose();
	 	vplay->nextpage = NULL;
	}

				   
 	moveTimer = new (IntrPuzzTimer);
	moveTimer->setTicks(4,this);
}

void ComWindow::open(int theView, Object* whoToCue)
{
	setView(theView);
	endCel = 9;
 	increment = 1;
	currentWindowOffset = (theView - 403) * 10; //7 is the offset to the first window loop
 	caller = whoToCue;
	setCel(0);
	posn(winPosnX[currentWindowOffset + cel], 85);

	show();

	vplay->windowSFX->play(123, NULL);
 	moveTimer = new (IntrPuzzTimer);
	moveTimer->setTicks(4,this);
}

/***********************************
 *		
 *		 ComLink Page
 *		      
 **********************************/
Vplay::Vplay()
{
	name = "vlinkspage";
	numPageItems = 0;
	currentPage = 0;
	prevpage = NULL;
	nextpage = NULL;
	vWindow = NULL;
	wiredot = NULL;	
	vplay = this;	
	comlinkshort->hide();

	IntrPuzzPage::init(WEB_VLINKS);
	helppage = 92860;

	if(comlinkshort->linkflash)
	{
		comlinkshort->linkflash->dispose();
		comlinkshort->linkflash = NULL;
	}		

	new (ComBack);
	new (ComWindow);
	new(SelectBar);
	new(OpManager);

	windowSFX		=	new(Sound);
	closebutt		=	new(CloseButt);
	comitemList		= 	new(ComItemList);
	voiceSound 		= 	new(Sound);
	mailbutt 		=	new(MailButt);
	archivebutt 	= 	new(ArchiveButt);

	if(intrpuzz->isStateSaved("comlinkinfo"))
	{
		comlinkinfo->load();
	}
	else
	{
		mailbutt->activate();
		//wiredot	= new(WireDot);
	}
}

void Vplay::saveState()
{
		if(!intrpuzz->isStateSaved("comlinkinfo"))
		{
			new(ComLinkInfo);
		}		
		comlinkinfo->save();
}

void Vplay::dispose()
{
	sfxMadeCursor();
	
	if(!(window->flag & OBJECT_HIDDEN))
		window->addToPic();

 	if(!comitemList->isEmpty())
		comitemList->dispose();
	delete comitemList;

 	if(comlinkshort)
 		comlinkshort->show();

	if(currentHFile)
		emailpage->dispose();

	vplay->voiceSound->dispose();
	vplay->windowSFX->dispose();

	IntrPuzzPage::dispose();
}

int Vplay::handleEvent(MADEEventStamp* event)
{
	return comitemList->handleEvent(event);
	//return false;
}

void Vplay::init(void) {
	bool pageFull = false;
	short thisx = 64;
	short thisy = 135;//123;
	textHeight = 0;
	int counter = 0;
	Vitem * vitem;
	Eitem * eitem;
	Voiceitem * voiceitem;
	Node *n;

	if(window->view == MSGWIN)
		n = comlinks->head;
	else
		n = archiveList->head;

	currentFocus = PUZZLE_FOCUS;

	
	if(prevpage)
	{
		prevpage->prevCL->dispose();
		prevpage->dispose();
		prevpage = NULL;
	}
	if(nextpage)
	{
		nextpage->nextCL->dispose();
		nextpage->dispose();
	 	nextpage = NULL;
	}
	
	if(currentPage > 0)
	{
		int offset = numPageItems * currentPage;
		for(int i =0; i < offset; i++)
		{
			n = n->next;
		}
		prevpage = new(PrevPage);
	}

	while ( n )
	{
		if(pageFull)
		{
			nextpage = new(NextPage);
			break;	
		}
						
		if (((ComData*)n->data)->isReady())  //timer ran out or there was no delay
		{      
			switch(((ComData*)n->data)->comType)
			{
				case VLINKMAIL:
					vitem = new(Vitem);
					vitem->vlink = ((VLink*)n->data);
					vitem->init(thisx + 40, thisy, ((ComData*)n->data)->msgName);
					textHeight = vitem->myPrint->height;
					vitem->icon = new Icon(thisx, thisy, VIDEOICON);
				break;

				case EMAIL:
					eitem = new(Eitem);
					eitem->email = ((EMail*)n->data);
					eitem->init(thisx + 40, thisy, ((ComData*)n->data)->msgName);
					textHeight = eitem->myPrint->height;
					eitem->icon = new Icon(thisx, thisy, MAILICON);
				break;

				case VOICEMAIL:
					voiceitem = new(Voiceitem);
					voiceitem->voicemail = ((VoiceMail*)n->data);
					voiceitem->init(thisx + 40, thisy, ((ComData*)n->data)->msgName);
					textHeight = voiceitem->myPrint->height;
					voiceitem->icon = new Icon(thisx, thisy, VOICEICON);
				break;
			}

			if((thisy += textHeight + 5) > (PAGE_BOTTOM - (textHeight + 5)))
			{
				pageFull = true;
				numPageItems = counter + 1;
			}
			counter++;
		}
		n = n->next;
	}   
}

/*********************
*
*		Buttons
*
**********************/
OpManager::OpManager()
{
	name = "opmanager";
	init(444, 72, 400, 12, 0);
	noSkip();
}
		
void OpManager::activate()
{
	vplay->saveState();
	intrpuzz->dispose();
	new HTMLPage(91500);
}

MailButt::MailButt()
{
	name = "mailButt";
	init(385, 273, 400, 2, 0);
}
		
void MailButt::activate()
{
/*	if(currentHFile)
	{
		emailpage->dispose();
		mailscreen->dispose();
		vplay->init();
		vplay->currentPage = 0;
	}
*/
	helppage = 92860;
	user->input = false;
	currentLog = MAILLOG;
	setCel(2);  //grayed out button
	vplay->archivebutt->setCel(0);
	vplay->setScript(new(MailActivateScript));
}
				
VideoCloseButt::VideoCloseButt(Object* whoToCue)
{
	name = "mailButt";
	caller = whoToCue;
	init(328, 91, 400, 9, 0);
}

void VideoCloseButt::activate()
{
	caller->cue();
	dispose();
}

CloseButt::CloseButt()
{
	name = "mailButt";
	init(401, 96, 400, 6, 0);
	hide();
}

void CloseButt::activate()
{
	user->input = false;
	vplay->setScript(new(CloseActivateScript));
}

ArchiveButt::ArchiveButt()
{
	name = "archiveButt";
	init(385, 312, 400, 3, 0);
}
		
void ArchiveButt::activate()
{
	user->input = false;
	currentLog = ARCHIVELOG;
	setCel(2);  //grayed out button
	vplay->mailbutt->setCel(0);
	helppage = 92870;

	vplay->setScript(new(ArchiveActivateScript));
}

PrevPage::PrevPage()
{
	name = "PrevButt";
	init(330, 327, 400, 7, 0);
	prevCL = new CommandLine(left, top, right - left, bottom - top, "Click to go to previous page.");
}
		
void PrevPage::activate()
{
	vplay->currentPage--;
	comitemList->dispose();
	vplay->init();
}

NextPage::NextPage()
{
	name = "nextButt";
	init(358, 327, 400, 8, 0);
	nextCL = new CommandLine(left, top, right - left, bottom - top, "Click to go to next page.");
}
		
void NextPage::activate()
{
	vplay->currentPage++;
	comitemList->dispose();
	vplay->init();
}

/*********************
*
*		Views
*
**********************/
SelectBar::SelectBar()
{
	view = 400;
	loop = 0;
	cel = 0;
	init(64, 123);
	sfxSpriteGlass(mySprite,2,MK_COLOR16(82,181,173),50);
	selectbar = this;
	hide();
}

ComBack::ComBack()
{
	name = "comLinkback";
	view = 401;
	loop = 0;
	cel = 0;
	init(0,16);
	noSkip();
	addToPic();
	dispose();
}

IDCard::IDCard(int IdNumber)
{
	name = "comLinkback";
	view = IdNumber;
	loop = 0;
	cel = 0;
	init(68,129);
	noSkip();
}

VidWindow::VidWindow()
{
	view = 400;
	loop = 4;
	cel = 0;
	title = NULL;
	vWindow = this;
	init(45, 76);
	noSkip();
	videoblink = new(VideoBlink);
	attachblink = NULL;
}

VidWindow::VidWindow(char *name)
{
	view = 400;
	loop = 4;
	cel = 0;
	init(59, 65);
	title = new WindowTitle(160, 75, name);
}

void VidWindow::dispose()
{
	if(attachblink)
		attachblink->dispose();
	attachblink = NULL;

	if(title)
		title->dispose();
	vWindow = NULL;
	vCloseButt->dispose();
	IntrPuzzView::dispose();
}

AttachBlink::AttachBlink()
{
	name = "AttachBlink";
	cycleSpeed = 8;	
	view = 400;
	loop = 10;
	cel = 2;
	x = 101;
	y = 317;
	
	draw();
	intrpuzz->addToFront ( this );
//	setCycle(new EndLoop((Prop*)this, this));
}

void AttachBlink::cue(void)
{
	setCycle(new EndLoop((Prop*)this, this));
}

VideoBlink::VideoBlink()
{
	name = "VideoBlink";
	cycleSpeed = 4;	
	view = 400;
	loop = 11;
	cel = 0;
	x = 203;
	y = 91;
	
	draw();
	intrpuzz->addToFront ( this );
	setCycle(new EndLoop((Prop*)this, this));
}

void VideoBlink::cue(void)
{
	setCycle(new EndLoop((Prop*)this, this));
}

WindowTitle::WindowTitle(int theX, int theY, char *name)
{ 
  	x = theX;
  	y = theY;
	size = 11;
  	fore_color = TEXT_COLOR_BLACK;

	init(name);
}

Icon::Icon(int theX, int theY, int theCel)
{
	view = 400;
	loop = 1;
	cel = theCel;
	x = theX;
	y = theY;
	draw();
//	intrpuzz->addToFront ( this );
//	init(theX, theY);
	iconAllocated++;
}

void Icon::dispose()
{
	deathList->add(this);
}

Icon::~Icon()
{
	iconAllocated--;
}

/*********************
*
*		Scripts
*
**********************/
VoiceitemActivateScript::VoiceitemActivateScript(Voiceitem* voiceItem)
{
	voiceitem = voiceItem;
}

void VoiceitemActivateScript::changeState( int newState)
{
	switchTo
		if(!comitemList->isEmpty())
			comitemList->dispose((MailItem *)voiceitem);

		window->close(this);
	END
	BEG
		ticks = 6;  //delay a few seconds
	END
	BEG
		idcard = new IDCard(voiceitem->voicemail->idNumber);
		vplay->voiceSound->number = voiceitem->voicemail->soundNumber;
		vplay->voiceSound->canInterupt = true;
		vplay->voiceSound->playBuffered(vplay->voiceSound->number, this); 	
	END
	BEG
		user->input = false;
		voiceitem->cue();
		idcard->dispose();

		if(currentLog == MAILLOG)
			window->open(MSGWIN, this);
		else
			window->open(ARCHIVEWIN, this);
	END
	BEG
		vplay->currentPage = 0;
		vplay->init();
		dispose();
		user->input = true;
	END
}

VitemActivateScript::VitemActivateScript(Vitem* videoItem)
{
	vitem = videoItem;
}

void VitemActivateScript::changeState( int newState)
{
	switchTo
		if(!comitemList->isEmpty())
			comitemList->dispose((MailItem *)vitem);

		window->close(this);
	END
	BEG
		ticks = 6;  //delay a few seconds
	END
	BEG
//		vWindow = new VidWindow(vitem->vlink->msgName);
		vWindow = new VidWindow();
		if(vitem->vlink->emailAttachment)
			vWindow->attachblink = new (AttachBlink);
		vCloseButt = new VideoCloseButt(this);
		ticks = 8;	
	END
	BEG
		user->canInput(false);
		
		if(strcmp(vitem->vlink->movieName, "100.avi") == 0) //holt interactive vlink
		{
			this->setScript(new(ScriptHolt));
		}
		else if(strcmp(vitem->vlink->movieName, "101.avi") == 0) //holt interactive vlink
		{
			this->setScript(new(ScriptMax));
		}
		else
			theMovie->play(vitem->vlink->movieName, this, 1);
	END
	BEG
		vWindow->videoblink->dispose();
		if(vitem->vlink->emailAttachment)
		{
			user->input = true;
			vplay->mailbutt->setCel(0);
			emailpage = new EMailPage(vitem->vlink->emailAttachment->pageNumber, ATTACH);
			vitem->cue();
		}
		else
		{
			vitem->cue();
			cue();
		}
	END
	BEG
		user->input = false;
		if(currentHFile)
			emailpage->dispose();

		vWindow->dispose();

		if(currentLog == MAILLOG)
			window->open(MSGWIN, this);
		else
			window->open(ARCHIVEWIN, this);
	END
	BEG
	//	vitem->vlink->dispose();
		vplay->currentPage = 0;
		vplay->init();
		dispose();
		user->input = true;
	END
}

EitemActivateScript::EitemActivateScript(Eitem* mailItem)
{
	pageNum = mailItem->email->pageNumber;
	mailItem->email->dispose();
}

void EitemActivateScript::changeState( int newState)
{
	switchTo
	 	if(!comitemList->isEmpty())	
			comitemList->dispose(); 

		window->close(this);
	END
	BEG
		ticks = 6;  //delay a few seconds
	END
	BEG
		window->open(MAILWIN, this);
	END
	BEG

		emailpage = new EMailPage(pageNum);
		vplay->saveState();
		dispose();
		vplay->mailbutt->setCel(0);
		user->input = true;
	END
}

void MailActivateScript::changeState ( int newState )
{
	switchTo
	 	if(!comitemList->isEmpty())
			comitemList->dispose();

		if(vplay->wiredot)
			vplay->wiredot->kill = true;

		if(currentHFile)
		{
			emailpage->dispose();
	 		vplay->currentPage = 0;
			currentEmail = 0;
			vplay->saveState();
		}

		if(vWindow)
			vWindow->dispose();

		if(window->cel == 9)  //window is open
			window->close(this);
		else
			ticks = 2;	
	END
	BEG
		ticks = 6; //wait a couple of ticks before opening
	END
	BEG
		window->open(MSGWIN, this);
	END
	BEG
		vplay->currentPage = 0;
		vplay->init();
		dispose();
		user->input = true;
	END
}

void ArchiveActivateScript::changeState ( int newState )
{
	switchTo
	 	if(!comitemList->isEmpty())
			comitemList->dispose();

		if(vplay->wiredot)
			vplay->wiredot->kill = true;
	 	
		if(currentHFile)
		{
			emailpage->dispose();
	 		vplay->currentPage = 0;
		}

		if(vWindow)
			vWindow->dispose();

		if(window->cel == 9)  //window is open
			window->close(this);
		else
			ticks = 2;	
	END
	BEG
		ticks = 6;  //delay a few seconds
	END
	BEG
		window->open(ARCHIVEWIN, this);
	END
	BEG
		vplay->currentPage = 0;
		vplay->init();
		user->input = true;
		dispose();
	END
}

void CloseActivateScript::changeState ( int newState )
{
	switchTo
	 	if(!comitemList->isEmpty())
			comitemList->dispose();

		user->input = false;
		closingMail = false;
		if(currentHFile)
		{
			emailpage->dispose();
			closingMail = true;
			currentEmail = 0;
			vplay->saveState();
		}

		window->close(this);
	END
	BEG										    
		ticks = 6;
	END
	BEG
		if(closingMail)
		{
			if(currentLog == MAILLOG)
				window->open(MSGWIN, this);
			else
				window->open(ARCHIVEWIN, this);
		}
		else	
		{
			vplay->archivebutt->setCel(0);
			vplay->mailbutt->setCel(0);
			vplay->wiredot = new(WireDot);		
			dispose();
			user->input = true;
		}
	END
	BEG
		vplay->currentPage = 0;
		vplay->init();
		dispose();
		user->input = true;
	END
}

void playLink(Script * cueme)
{
	Node *n = comlinks->head;

	theMovie->play(((VLink*)n->data)->movieName, cueme, 0);
	n->data->dispose();
}

/***********************************
 *		
 *   Mail item (base class for all items)
 *		      
 **********************************/
MailItem::~MailItem()
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
}

void MailItem::init(int theX, int theY, char *theName)
{
	printHidden = false;
	onIt = false;
	icon = 0;

	left = theX - 40;
	top = theY;
	right = left + 298;
	bottom = top + 15;


  	myPrint = new(Print);
  	myPrint->x = theX;
  	myPrint->y = theY;
	myPrint->size = 11;

  	myPrint->fore_color = TEXT_COLOR_BLACK;

	myPrint->init(theName);
	comitemList->addToFront ( this );
}

/***********************************
 *		
 *		 Voice mail item
 *		      
 **********************************/
void Voiceitem::cue ( void )
{
	this->voicemail->dispose();
	onIt = false;
	user->canInput(true);
	vplay->currentPage = 0;
//	vplay->init();
	
	/* extra events */
	if(currentLog == MAILLOG)
	{
		if( voicemail->soundNumber == 98900 )
		{
			new EMail("Report Immediately: Source Unknown",98120,fColbySummons,0);
		}
		if( voicemail->soundNumber == 98002  && !GameFlag.test( fDCK3 ) )
		{
			pcLastUsed[PC_PEGID] = true;	// id PEG
			GameFlag.set( fDCK3 );
		}
	}
 	delete this;

//	if(GameFlag.test(fWahKa10) && !GameFlag.test(fWahKa20)) {
//		GameFlag.set(fWahKa20);
//	   	GameFlag.set(fLenaCalls);
//		GameFlag.set(fMoscowAlleyAvailable);
//	}
//	if(GameFlag.test(fWahKa11) && !GameFlag.test(fWahKa21)) {
//		GameFlag.set(fWahKa21);
//		GameFlag.set(fHarmCalls);
//		GameFlag.set(fMoscowAlleyAvailable);
///		GameFlag.set(fMetHarmonica);
//	}
}

void Voiceitem::activate(void)
{
	user->canInput(false);
	selectbar->hide();

	if(currentLog == MAILLOG)
		new VoiceMail(this->voicemail->msgName, this->voicemail->soundNumber, this->voicemail->idNumber, 1); 
		
	this->myPrint->erase();
	this->icon->dispose();
	this->icon = NULL;

//	if(!comitemList->isEmpty())
//		comitemList->dispose((MailItem *)this);

//	vplay->voiceSound->number = voicemail->soundNumber;
//	vplay->voiceSound->play(this); 	
	vplay->setScript(new VoiceitemActivateScript(this));
}

int Voiceitem::handleEvent(MADEEventStamp* event)
{
	if (user->input)  
	{
		if(onMe(event))
		{
			if(!onIt)
			{	
				selectbar->show();
				selectbar->posn(left, top);
			  	onIt = true;
			}
		  	if (event_type == USER_LEFT_UP)
		    {
				activate();
				return true;
		    }
		}
		else if(onIt)
		{
			onIt = false;
			selectbar->hide();
		}			
	}
  return false;
}

/***********************************
 *		
 *		 Email item
 *		      
 **********************************/
void Eitem::activate(void)
{
	currentEmail = email->pageNumber;
	if(currentLog == MAILLOG)
		new EMail(this->email->msgName, this->email->pageNumber, 1); //archive this

	selectbar->hide();
	onIt = false;
//	if(currentHFile)
//		emailpage->dispose();
	user->input = false;

	/* extra effects */
	if( email->pageNumber == 98070  && !GameFlag.test( fDCK4 ) )// Jaimie's real Procat file
	{
		new VLink( "It's all about access: Colby, William", 
		           "12000.avi", fSaw12000, 0, new EMailAttachment( "Colby98341", 98341 ) );
		GameFlag.set( fEclipse2Avail );
		GameFlag.set( fDCK4 );
	}
	if( email->pageNumber == 98110 )
	{
	 	GameFlag.set(fHoltSeatonAvailable );
	}			  

	vplay->setScript(new EitemActivateScript(this));
}

int Eitem::handleEvent(MADEEventStamp* event)
{
	if (user->input)  
	{
		if(onMe(event))
		{
			if(!onIt)
			{			
				selectbar->show();
				selectbar->posn(left, top);
				onIt = true;	
			}
		  	if (event_type == USER_LEFT_UP)
		    {
				activate();
				return true;
		    }
		}
		else if(onIt)
		{
			selectbar->hide();
			onIt = false;
		}				
	}
	
  return false;
}

/***********************************
 *		
 *		 Vlink item
 *		      
 **********************************/
void Vitem::activate(void)
{
	this->myPrint->erase();
	this->icon->dispose();
	this->icon = NULL;

	if(currentLog == MAILLOG)
		if((strcmp(vlink->movieName, "101.avi") != 0) && (strcmp(vlink->movieName, "100.avi") != 0)) 
			new VLink(vlink->msgName, vlink->movieName, vlink->emailAttachment, 1);	//archive this

	selectbar->hide();
	onIt = false;
	user->input = false;
	vplay->setScript(new VitemActivateScript(this));
}

int Vitem::handleEvent(MADEEventStamp* event)
{
	if (user->input)  
	{
		if(onMe(event))
		{
			if(!onIt)
			{	
				selectbar->show();
				selectbar->posn(left, top);
			  	onIt = true;
			}
		  	if (event_type == USER_LEFT_UP)
		    {
				activate();
				return true;
		    }
		}
		else if(onIt)
		{
			onIt = false;
			selectbar->hide();
		}			
	}
  return false;
}

void Vitem::cue ( void )
{
	this->vlink->dispose();

	if(currentLog == MAILLOG)
	{
		if (!strcmp(curRoom->name,"Rm1270") && (!comlinks || comlinks->isEmpty()))
			new QuitAlert(4,19,666,3,3);

		//if (GameFlag.test(fSaw646) && !GameFlag.test(fInit631))	{
		//	new VLink( "631.avi", 0, 30 );
		//	GameFlag.set(fInit631 );
		//	new VoiceMail("op. proc.", 980, 0, 25);
		//}
/*		if ( !GameFlag.test( fInit99020 ) && !GameFlag.test( fKilledLange )	 &&
		     ( GameFlag.test( fSecurityModelAvailable ) || GameFlag.test( fInfosciComplete ) ) )
		{
			new VoiceMail( "No luck", 99020, LANGE);
			GameFlag.set(fInit99020 );
		}*/ //taken out by greg for daniel

		if (GameFlag.test(fSaw12001) && !GameFlag.test(fInit90))	{
			new VLink( "90.avi", fSaw90, 30 );	// NOT USED!
			GameFlag.set(fInit90 );
		}
		if (GameFlag.test(fSaw12002) && !GameFlag.test(fInit731))	{
			new VLink( "731.avi", fSaw731, 30 );	// NOT USED!
			GameFlag.set(fInit731 );
		}
		if (GameFlag.test(fSaw147) && !GameFlag.test(fInit153))	{
			new VLink( "Not the answer, Colby, William", "153.avi", 0, 40 );
			GameFlag.set(fInit153 );
		}

	//	if (GameFlag.test(fHKRoomEnter) && GameFlag.test(fMaxReportBSDis) && !GameFlag.test(fInitCarrot))
	//	{
	//		sfxPrintf("Topic #80, found BS, if BS took carrot");
	//		new VLink( "403.avi", fWahKa26, 30);
	//		new VLink( "374.avi", 0, 0);
	//		new VLink( "375.avi", 0, 0);
	//		GameFlag.set(fInitCarrot );
	//	}

		/*#define GFT GameFlag.test
		if( GFT( fFullBriefing )  && !GFT( fPEGIDEmailSent ) && GFT (fSaw634 ) )
		{
			new EMail( "PEG ID Mail 1", 98002 );
		 	GameFlag.set( fPEGIDEmailSent );
		}*/
		#define GFT GameFlag.test
		/*if( GFT( fSecurityModelAvailable )  && !GFT( fISciEmailSent ))
		{
		 	new EMail( "Infosci Security Data", 98020 );
			GameFlag.set( fISciEmailSent );
		}*/
		#define GFT GameFlag.test
		if( GFT( fShow157 ) && GFT( fInit156 ) && GFT( fColbyMoleBriefing ) )
		{
		 	new VLink( "Ying's Beale codes: Seaton, Jaimie A.",
		 	           "157.avi", fSaw157, 0,	new EMailAttachment( "Beale Code Tool", 98040 ) );
			GameFlag.set( fSetJaimies157 );
			GameFlag.set( fYingStuffArrived );
			GameFlag.clear( fShow157 );
			new VLink( " Tracking Ying: Holt, David M.", "645.avi", 0, 0 );
			GameFlag.set( fSetVL_Holt645 );	// extraneous
			new VLink( "Signs of betrayal: Colby, William", "12002.avi",
			           new EMailAttachment( "The eclipse hour: Colby, William", 98340 ) );// Eclipse
		}	
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "12002.avi" ) == 0  || strcmp( VNAME, "12002.AVI" ) == 0 )
		{
			// detecting attached email 98340
			pcLastUsed[ PC_ECLIPSE ] = true;
		}			 
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "657.avi" ) == 0  || strcmp( VNAME, "657.AVI" ) == 0 )
		{
			new VLink( "Think twice, Thorn: Holt, David M.", "25009.avi", 0, 0); //vlink 27
		}
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "53001.avi" ) == 0  || strcmp( VNAME, "53001.AVI" ) == 0 )
		{
			GameFlag.set( fDCK1/*fBirdsongAvailable*/ );
		}
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "157.avi" ) == 0  || strcmp( VNAME, "157.AVI" ) == 0 )
		{
		// 	new EMail( "Beale Code Tool", 98040 );	
			pcLastUsed[PC_CIPHER] = WEB_YINGCYPHER;
			GameFlag.set( fYingCipherAvailable );
		}
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "164.avi" ) == 0  || strcmp( VNAME, "164.AVI" ) == 0	||
		    strcmp( VNAME, "1641.avi" ) == 0  || strcmp( VNAME, "1641.AVI" ) == 0 )
		{   
			if( !GameFlag.test( fLockOut ) )
			{              
				new VLink( "Bullpen is open: Milkovsky, Frank", "646.avi", fSaw646, 0, new EMailAttachment( "More", 98140 ) );
				GameFlag.set(fInit646 );
			}
			else
				GameFlag.set( fSaw646 );
		}
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "646.avi" ) == 0  || strcmp( VNAME, "646.AVI" ) == 0 )
		{
			ASSERT( !GameFlag.test( fLockOut ), 0 );	// 646 won't happen 
			new EMail("Dangerous waters: Warhurst, Eugene J.", 98111);                 
			new VoiceMail("Bullpen not an option: Warhurst, Eugene J.", 980, WARHURST );	  				
		}
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "644.avi" ) == 0  || strcmp( VNAME, "644.AVI" ) == 0 )
		{
			pcLastUsed[ PC_PHOTODOC ] = true;	  
		}
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "634.avi" ) == 0  || strcmp( VNAME, "634.AVI" ) == 0 )
		{
			pcLastUsed[PC_KAT] = true;
		}
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "35202.avi" ) == 0  || strcmp( VNAME, "35202.AVI" ) == 0 )
		{
			GameFlag.set( fCanDisplayStakeOut );
		}
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "1080.avi" ) == 0  || strcmp( VNAME, "1080.AVI" ) == 0 )
		{
			new VLink( "No one above suspicion: Colby, William", "12001.avi",
			           new EMailAttachment( "title", 98342 ) );
			GameFlag.set( fEclipse3Avail );
		}	
		#define VNAME vlink->movieName
		if( strcmp( VNAME, "53001.avi" ) == 0  || strcmp( VNAME, "53001.AVI" ) == 0 )
		{
			pcLastUsed[PC_SNDANAL] = true;
		}
		/*if(GameFlag.test(fWahKa12) && !GameFlag.test(fWahKa22)) {
			GameFlag.set(fWahKa22);
			new VLink ( "Imminent boom: Holt, David M.", "35012.avi", fWahKa13, 0 );
			new EMail( "CypherOne", 98300);
		}
		if(GameFlag.test(fWahKa13) && !GameFlag.test(fWahKa23)) {
			GameFlag.set(fWahKa23);
			new VLink ( "35013.avi", fWahKa14, 0 );
		}*/
		if(GameFlag.test(fWahKa14) && !GameFlag.test(fWahKa24)) {
			GameFlag.set(fWahKa24);
			new EMail( "CypherOne", 98300);
		//CLU	new VLink ( "Red line communication: Oval Office", "35014.avi", fWahKa15, 0 );
		}
		if(GameFlag.test(fWahKa15) && !GameFlag.test(fWahKa25)) {
			GameFlag.set(fWahKa25);
			new VLink ( "The pit's the priority: DCI Sterling, Peter R.", "35015.avi", fWahKa16, 0 );
		}
		if(GameFlag.test(fWahKa16) && !GameFlag.test(fWahKa26)) {
			GameFlag.set(fWahKa26);
//			new VLink ( "35016.avi", 0, 0 );
//			new EMail( "CypherTwo", 98301);
		}
		/*if(GameFlag.test(fWahKa18) && !GameFlag.test(fWahKa28)) {
			GameFlag.set(fWahKa28);
			if(GameFlag.test(fCarrotPath))
//				new VLink( "35201.avi", 0, 60);
				new VLink("Birdsong's in trouble: Foster, Maxine M.", "35201.avi", 0, 60, new EMailAttachment("Camera", 98092));
			//else if(GameFlag.test(fStickPath))
			//	new VLink( "On to the stakeout: Foster, Maxine M.", "35202.avi", fSaw35202, 60);
		}*/					
		/* CUT 12/16
		if(GameFlag.test(fWahKa19) && !GameFlag.test(fWahKa29)) {
			GameFlag.set(fWahKa29);
			new VLink( "25005.avi", fWahKa32, 0);		//vlink 205, Sterling heras about Procat
		}
		*/
		/* MOVED TO FLAG 12/16
		if(GameFlag.test(fWahKa32) && !GameFlag.test(fWahKa33)) { 
			GameFlag.set(fWahKa33); 
			new VLink( "25008.avi", fWahKa34, 0); MOVED TO FLAG 12/16		//vlink 657, Jaimie implicates Holt
			new EMail( "Kipling98050", 98050);  //CUT 12/16
		}
		*/
	//	if(GameFlag.test(fWahKa34) && !GameFlag.test(fWahKa35)) {
	//		GameFlag.set(fWahKa35);
	//		new VLink( "25009.avi", 0, 0);				//vlink 27, I didn't do it
	//		new EMail( "Jaimie98100", 98100);
	//	}
		if(GameFlag.test(fWahKa68) && !GameFlag.test(fWahKa69)) {
			GameFlag.set(fWahKa69);
//			new VLink( "Churbanov rising: Newscast", "48007.avi", 0, 60);	//vlink 541 - BCC Word Churbanov lead poll
		}
//		if(GameFlag.test(fWahKa85) && !GameFlag.test(fWahKa86)) {
//			GameFlag.set(fWahKa86);
//			new EMail("From: Holt, D. - Remember BirdSong", 98151);
//		}		
		if(GameFlag.test(fWahKa55) && !GameFlag.test(fWahKa98)) {
			GameFlag.set(fWahKa98);
//			new VLink( "Mole murmurs: Holt, David M.", "1081.avi", fWahKa56, 0 );
			
		}
		if(GameFlag.test(fWahKa56) && !GameFlag.test(fWahKa102)) {
			GameFlag.set(fWahKa102);
//			new EMail( "Mole links: Holt, David M.", 98310);
			
		}
		if(GameFlag.test(fSawAchtung) && !GameFlag.test(fWahKa112)) {
			GameFlag.set(fWahKa112);
			new EMail( "Put Onyx out of business, Sterling Peter R.", 98202);		
		}
		if(GameFlag.test(fWahKa113) && !GameFlag.test(fWahKa114)) {
			GameFlag.set(fWahKa114);
			new EMail( "Odds are slipping, Sterling Peter R.", 98201);		
		}
	}
	delete this;
}

} // namespace Spycraft
