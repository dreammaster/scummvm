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

#include "spycraft/game/globals.h"
#include "spycraft/game/flag.h"			   
#include "spycraft/game/print.h"
#include "spycraft/game/sound.h"						  
#include "spycraft/game/web.h"
#include "spycraft/game/sndanal.h"
#include "spycraft/game/kat.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/mixmatch.h"
#include "spycraft/game/maptool.h"
#include "spycraft/game/onyxhunt.h"
#include "spycraft/game/phodoc.h"
#include "spycraft/game/cypher.h"
#include "spycraft/game/image.h"						   
#include "spycraft/game/html.h"
#include "spycraft/game/htm.h"						     
#include "spycraft/dmade/advscreen.h"
#include "spycraft/game/target.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advmusic.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

extern BlinkCursor* normalCursor;
extern GunCursor* gunCursor;
extern Colby colby;

extern"C"{extern void GetCDROM(char*);}

extern Flag GameFlag;						 

extern int currentFocus;                          
WebHome * homobutt; 
ComLinkShort* comlinkshort;
WebReport * webreport;
ReportBox	*reportbox;

/*********************
*
*		Intrpuzz 
*
**********************/

Web::Web()
{
	name = "Web";
	show_style = VE_FLIP;
//	sfxSetSpritelistSize(256);
	intrface->disable();
	usedbutton = false;
	fwdbutton = false;
	backbutton = false;
	normalCursor->cel = 1;
	theGame->handsOn();
	init (-1);
}

void Web::init(int pic_num)
{
	IntrPuzz::init(pic_num);

	new (WebBack);
	new (WebFwd);
	homobutt = new (WebHome);
	comlinkshort = new(ComLinkShort);

	webreport = new (WebReport);

	new (WebHelp);
	new (WebExit);
	new (GlobosAnim);
}

Web::~Web()
{
	currentHFile = NULL;
	webreport = NULL;
}

Yellow::Yellow(int page)
{
	name = "Yellow";
	show_style = VE_FLIP;
//	sfxSetSpritelistSize(256);
	intrface->disable();
	usedbutton = false;
	fwdbutton = false;
	backbutton = false;
	normalCursor->cel = 1;
	GameFlag.set(fYellow);
	init (-1,page);
}

void Yellow::init(int pic_num,int pageNum)
{
	IntrPuzz::init(pic_num);
	new HTMLPage(pageNum);
}

Yellow::~Yellow()
{
	GameFlag.clear(fYellow);
	currentHFile = NULL;
}

Back::Back(int num)
{
	name = "puzzback";
	view = num;
	loop = 0;
	cel = 0;
	init(0,15);
	noSkip();
	addToPic();
}
/*********************
*
*		WebButtons 
*
**********************/
void WebBack::activate(void)
{
	Website * id = webmap->findPrevPage();
	if (id)
	{
// set globals for buttons 
		//let the game know a button was used
		usedbutton = true;
		//allow forward button to operate
		fwdbutton = true;
		intrpuzz->page->saveState();
		intrpuzz->dispose();
		changePage (id);	
	}
}

void WebHome::activate(void)
{
// set globals for buttons 
	//let the game know a button was used
//	usedbutton = true;
	//allow back button to operate
//	backbutton = false;
//	fwdbutton = false;
//	webmap->dispose();
	intrpuzz->page->saveState();
	intrpuzz->dispose();
	new (Home);
}

void WebHelp::activate(void)
{
	if (!target)
	{
		intrpuzz->page->saveState();
		intrpuzz->dispose();
		new HTMLPage(helppage);
	}
}

void WebFwd::activate(void)
{
	Website * id = webmap->findNextPage();
	if (id)
	{
// set globals for buttons 
		//let the game know a button was used
		usedbutton = true;
		backbutton = true;
		intrpuzz->page->saveState();
		intrpuzz->dispose();
		changePage (id);	
	}
}
void WebExit::activate(void)
{
	if (target)
	{
		target->leave();
		comlinkshort = NULL;
	}
	else	
	{
		if(needToReport())
		{
			new(ReportBox);	
		}
		else
		{
			comlinkshort = NULL;
			intrpuzz->quitIntrface();
		}
	}
}

int WebExit::needToReport()
{
	if(GameFlag.test(fGreg20))
	{
		GameFlag.clear(fGreg20); //this flag tells us we entered the mole hunt from the pc screen
		return false;
	}
		
	switch (lasttool)
	{
		case WEB_SOUND_C8:	
		case WEB_SOUND_C13:	
		case WEB_SOUND_C21:	
		case WEB_SOUND_C26:	
		case WEB_SOUND_B6:	
		case WEB_SOUND_B14:	
		case WEB_SOUND_B14B:
		case WEB_SOUND_B20:	
		case WEB_SOUND_M14:	
		case WEB_SOUND_M28:	
		case WEB_SOUND_S19:	
		case WEB_SOUND_S22:	
		case WEB_SOUND_VRC5:
		case WEB_SOUND_VRC9:
		case WEB_SOUND_VRC23:
		case WEB_SOUND_VRB9:
		case WEB_SOUND_VRB15:
		case WEB_SOUND_VRM5:
		case WEB_SOUND_VRM9:
		case WEB_SOUND_VRS9:
		case WEB_SOUND_VRS29:
		case WEB_GRENDELMATCH:
			if (GameFlag.test(fIDPEG) && !GameFlag.test(fInfosciComplete)&& GameFlag.test(fGreg12))
			{
				if(!GameFlag.test(fReportInfosci))
				{
				//	GameFlag.set(fReportInfosci);
					return true;
				}
			}
			break;
		case WEB_YINGCYPHER:
			if (!GameFlag.test(fKnowYingInMoscow))
			{
				if(!GameFlag.test(fReportYingCypher))
				{
				//	GameFlag.set(fReportYingCypher);
					return true;
				}
			}
			break;
		case WEB_IMAGEPEG:
			if (GameFlag.test(fSolvedDubanskyKAT) && !GameFlag.test(fIDPEG) && GameFlag.test(fGreg11))
			{
				if(!GameFlag.test(fReportPEG))
				{
				//	GameFlag.set(fReportPEG);
					return true;
				}
			}
			break;
		case WEB_KAT:
		case WEB_KATMATCH:
			if (GameFlag.test(fFullBriefing) && !GameFlag.test(fSolvedDubanskyKAT) && GameFlag.test(fGreg10))
			{
				if(!GameFlag.test(fReportKat))
				{
			//		GameFlag.set(fReportKat);
					return true;
				}
			}
			break;
		case WEB_ONYXHUNTONE:
		case WEB_ONYXHUNTTWO:
			if (!GameFlag.test(fOnyxHunt1))
			{
				if(!GameFlag.test(fReportOnyxOne))
				{
				//	GameFlag.set(fReportOnyxOne);
					return true;
				}
			}
			else if (GameFlag.test(fOnceOnyx48000) && !GameFlag.test(fOnyxHunt2))
			{
				if(!GameFlag.test(fReportOnyxTwo))
				{
				//	GameFlag.set(fReportOnyxTwo);
					return true;
				}
			}
			break;
		case WEB_MAP:
		case WEB_SOUND_BIRD:
			if (GameFlag.test(fMaxReportBSDis) && !GameFlag.test(fDoneSoundAnal) && GameFlag.test(fGreg13))
			{
				if(!GameFlag.test(fReportMapTool))
				{
				//	GameFlag.set(fReportMapTool);
					return true;
				}
			}
			break;
		case WEB_KNEE:
		case WEB_IMAGEKNEEID:
			if (!GameFlag.test(fKneecapsFound))
			{
				if(!GameFlag.test(fReportKneecaps))
				{
				//	GameFlag.set(fReportKneecaps);
					return true;
				}
			}
			break;
	}

/*	if (GameFlag.test(fFullBriefing) && !GameFlag.test(fSolvedDubanskyKAT) && GameFlag.test(fGreg10))
	{
		if(!GameFlag.test(fReportKat))
		{
	//		GameFlag.set(fReportKat);
			return true;
		}
	}
	else if (GameFlag.test(fSolvedDubanskyKAT) && !GameFlag.test(fIDPEG) && GameFlag.test(fGreg11))
	{
		if(!GameFlag.test(fReportPEG))
		{
		//	GameFlag.set(fReportPEG);
			return true;
		}
	}
	else if (GameFlag.test(fIDPEG) && !GameFlag.test(fInfosciComplete)&& GameFlag.test(fGreg12))
	{
		if(!GameFlag.test(fReportInfosci))
		{
		//	GameFlag.set(fReportInfosci);
			return true;
		}
	}
	else if (GameFlag.test(fMaxReportBSDis) && !GameFlag.test(fDoneSoundAnal) && GameFlag.test(fGreg13))
	{
		if(!GameFlag.test(fReportMapTool))
		{
		//	GameFlag.set(fReportMapTool);
			return true;
		}
	}
	else if (GameFlag.test(fSawAchtung) && !GameFlag.test(fOnyxHunt1) && GameFlag.test(fGreg14))
	{
		if(!GameFlag.test(fReportOnyxOne))
		{
		//	GameFlag.set(fReportOnyxOne);
			return true;
		}
	}
	else if (GameFlag.test(fOnceOnyx48000) && !GameFlag.test(fOnyxHunt2) && GameFlag.test(fGreg15))
	{
		if(!GameFlag.test(fReportOnyxTwo))
		{
		//	GameFlag.set(fReportOnyxTwo);
			return true;
		}
	}
	else if (GameFlag.test(fPickedUpCamera) && !GameFlag.test(fSawKneeAddress) && GameFlag.test(fGreg16))
	{
		if(!GameFlag.test(fReportKneecaps))
		{
		//	GameFlag.set(fReportKneecaps);
			return true;
		}
	}
	else if (GameFlag.test(fSaw156) && !GameFlag.test(fBrokeYingBealeCode) && GameFlag.test(fGreg17))
	{
		if(!GameFlag.test(fReportYingCypher))
		{
		//	GameFlag.set(fReportYingCypher);
			return true;
		}
	}*/
	return false;
}

ReportBox::ReportBox()
{
	name = "ReportBox";
	view = 450;
	loop = 0;
	cel = 0;
	DlgBox::init(82, 141);
	setPri(900);
	noSkip();
	reportbox = this;
	new(ReportOk);
	new (ReportCancel);
}

ReportOk::ReportOk()
{
	view = 450;
	loop = 1;
	cel = 0;
	init(157, 55);
	setPri(902);
	noSkip();
}

void ReportOk::activate()
{
	reportbox->dispose();
	webreport->activate();
}

ReportCancel::ReportCancel()
{
	view = 450;
	loop = 2;
	cel = 0;
	init(245, 54);
	setPri(902);
	noSkip();
}

void ReportCancel::activate()
{
	reportbox->dispose();
	comlinkshort = NULL;
	intrpuzz->quitIntrface();
}

WebBack::WebBack()
{
	name = "webback";
	init(98,19,666,1,2);
	noSkip();
	if (!GameFlag.test(fPatsHTM_127))
		new WebCommandLine(left, top, width, height, "Access Previous Document");
}														 

WebExit::WebExit()
{
	name = "webexit";
	init(4,19,666,3,0);
	noSkip();
	if (!GameFlag.test(fPatsHTM_127))
		new WebCommandLine(left, top, width, height, "Disconnect from InteLink Network");
}

WebFwd::WebFwd()
{
	name = "webfwd";
	init(145,19,666,2,2);
	noSkip();
	if (!GameFlag.test(fPatsHTM_127))
		new WebCommandLine(left, top, width, height, "Access Next Document");
}

WebHome::WebHome()
{
	name = "webhome";
	init(51,19,666,0,0);
	noSkip();
	if (!GameFlag.test(fPatsHTM_127))
		new WebCommandLine(left, top, width, height, "Return to InteLink Homepage");
}

WebHelp::WebHelp()
{
	name = "webhelp";
	init(192,19,666,4,0);
	noSkip();		
	if (!GameFlag.test(fPatsHTM_127))
		new WebCommandLine(left, top, width, height, "Online Help");
}

int WebBack::handleEvent(MADEEventStamp* event)
{
// I should write a function to do this upon entry not every update
  if (!backbutton && cel != 2)
  {
  	setCel(2);
  }	 	
  else 
  {
  	if (backbutton && cel == 2)
	{
		setCel(0);
	}
  }
  return (WebButton::handleEvent(event));
}

int WebFwd::handleEvent(MADEEventStamp* event)
{
// I should write a function to do this upon entry not every update
  if (!fwdbutton && cel != 2)
  {
  	setCel(2);
  }	 	
  else 
  {
  	if (fwdbutton && cel == 2)
	{
		setCel(0);
	}
  }
  return (WebButton::handleEvent(event));
}

ComLinkShort::ComLinkShort()
{
	linkflash = NULL;
	init(365, 383, 666, 7, 0);
	noSkip();
}

void ComLinkShort::activate()
{
	if(intrpuzz->isStateSaved("comlinkinfo"))
		comlinkinfo->dispose();

	intrpuzz->page->saveState();
	intrpuzz->dispose();
	new (Vplay);
}

void ComLinkShort::dispose()
{
	if ( hotspot )
	{
		setHotspot ( NULL );
		hotspot = NULL;
	}

	web_intrface->release ( this );
	deathList->add(this);
	comlinkshort = NULL;
}	

/*********************
*
*		pages 
*
**********************/

NewsPage::NewsPage(int pagenumber)
{
	name = "HTMLPage";
	init(pagenumber);
}

void NewsPage::init(int pagenumber)
{
	unsigned char * htmpage;

	LeftMargin = 5;
	TopMargin = 327;
	currentHFile->newFile();
	
	htmpage = (unsigned char*)LoadHTM(pagenumber);
	currentHFile->filesize = GetHTMSize();
	memcpy(&currentHFile->pagecopy,htmpage,currentHFile->filesize-1);	
	while (currentHFile->bytesRead <= currentHFile->filesize-1)
		currentHFile->readToken();

	UnLoadHTM();
	sfxPurgeRes(pagenumber,RES_HTM);
}										    
Home::Home()
{
	name = "Homepage";
	new Back(667);
	homobutt->setCel(2);
	init();
}

void Home::init(void)
{
	int playthe_f_n_sound_already = false;
	currentFocus = PUZZLE_FOCUS;
	if (!webmap->findPrevInst(WEB_HOME))
		playthe_f_n_sound_already = true;		

	IntrPuzzPage::init(WEB_HOME);
	helppage = 92810;

	new ComLink;
	new DataLink;
	new NoteLink;
	new TravelLink;

	comlinkshort->hide();
	if(comlinkshort->linkflash)
	{
		comlinkshort->linkflash->dispose();
		comlinkshort->linkflash = NULL;
	}		

	if (GameFlag.test(fHKRoomEnter))
		news = new NewsPage(2006);
	else if (GameFlag.test(fWahKa42))
		news = new NewsPage(2005);
	else
		news = new NewsPage(2000);
	
	if (playthe_f_n_sound_already)
		setScript(new (HomoScript));
}

void Home::dispose(void)
{
	if (news)
		news->dispose();
	
	if(comlinkshort)
		comlinkshort->show();

	news = NULL;
	IntrPuzzPage::dispose();
}
	
int Home::handleEvent(MADEEventStamp* event)
{
	int ret = false;

  	if (news)
	  	ret = news->handleEvent(event);

	if (!ret)
		return (IntrPuzzPage::handleEvent(event));
	else 
		return true;
}

DataPage::DataPage()
{
	name = "datapage";
	entrance = 0;
	intrpuzz->add ( this );	 
	new Back(631);
	new ATFButt;
	new CIAButt;
	new DEAButt;
	new FBIButt;
	new NROButt;
	new NSAButt;
	new USSSButt;
	new (WireAnim);
	init();
}
DataPage::~DataPage()
{
	if (entrance)
	{
		entrance->caller = NULL;
		entrance->stop();
		delete entrance;
		entrance = NULL;
	}
}

void DataPage::init(void) {
	currentFocus = PUZZLE_FOCUS;

	if (!webmap->findPrevInst(WEB_DATA))
	{
		entrance = new Sound;
		entrance->number = 45001;
		entrance->play();
	}
	else
		entrance = NULL;
	IntrPuzzPage::init(WEB_DATA);
}

NotePage::NotePage()
{
	name = "notepage";
	intrpuzz->add ( this );	 
	new Back(988);
	init();
}

void NotePage::init(void) {
	currentFocus = PUZZLE_FOCUS;

	IntrPuzzPage::init(WEB_NOTE);
}

TravelPage::TravelPage()
{
	name = "travelpage";
	intrpuzz->add ( this );	 
	new Back(988);
	init();
}

void TravelPage::init(void) {
	currentFocus = PUZZLE_FOCUS;
	IntrPuzzPage::init(WEB_TRAVEL);

}
/*********************
*
*		TextLinks 
*
**********************/

/*	NOT USED RIGHT NOW!!!!!!!!1
void Link::init(int theX, int theY, int color)
{
	left = theX;
	top = theY;

  	myPrint = new(Print);
  	myPrint->x = theX;
  	myPrint->y = theY;
	myPrint->size = 11;
  	myPrint->fore_color = color;//75;//1;
	myPrint->init(name);

	right = left + myPrint->width;
	bottom = top + myPrint->height;

	intrpuzz->add ( this );	 
}

void Link::activate(void)
{
}

int Link::handleEvent(MADEEventStamp* event)
{
  if (onMe(event))  
  {
    if ((! highlighted) && (! event_type))
    {
      highlighted = true;
	  myPrint->highlight(true);
      return(false);
    }
    else if (event_type == USER_LEFT_UP && user->input)
    {
	  activate();
      return(false);
    }
  }
  else
  {
	  myPrint->highlight(false);
	  highlighted = false;
  }	
  return false;
}
*/
/*********************
*
*		Home Buttons 
*
**********************/
ComLink::ComLink()
{
	name = "comlink";
	view = 601;
	loop = 0;
	cel = 0;
	init(17,89);
	noSkip();
	
	if(comlinks && !comlinks->isEmpty())
		new Alert(4, 78, 603, 0, 0);
	
	new CommandLine(left, top, width, height, "Video, Voice and Electronic Mail");
}

void ComLink::activate(void)
{
	user->input = false;
	new ButtAnim(602,0,20,93,(Button*) this);
}

void ComLink::cue(void)
{
	if(intrpuzz->isStateSaved("comlinkinfo"))
		comlinkinfo->dispose();

	user->input = true;
	intrpuzz->dispose();
	flushMessageCue();
	new (Vplay);
}

DataLink::DataLink()
{
	name = "datalink";
	view = 601;
	loop = 1;
	cel = 0;
	init(446,89);
	noSkip();
	
//	if()
//		new Alert(433, 78, 603, 0, 1);

	new CommandLine(left, top, width, height, "Connect to InteLink Agencies");
}

void DataLink::activate(void)
{
	user->input = false;
	new ButtAnim(602,1,373,93,(Button*) this);
}

void DataLink::cue(void)
{
	user->input = true;
	intrpuzz->dispose();
	flushMessageCue();
	new(DataPage);
}

NoteLink::NoteLink()
{
	name = "notelink";
	view = 601;
	loop = 2;
	cel = 0;
	init(446,258);
	noSkip();
	
//	if()
//		new Alert(432, 246, 603, 0, 2);

	new CommandLine(left, top, width, height, "Online Newscasts, National Intelligence Daily");
}

void NoteLink::activate(void)
{
	user->input = false;
	new ButtAnim(602,2,372,216,(Button*) this);
}

void NoteLink::cue(void)
{
	//91950
/*	int num = 99550;
	intrpuzz->dispose();
	if (GameFlag.test(f1stNIDOnLine))
		num = 99550;
	if (GameFlag.test(f2ndNIDOnLine))
		num = 99551;
	if (GameFlag.test(f3rdNIDOnLine))
		num = 99552;
*/
	user->input = true;
	intrpuzz->dispose();
	flushMessageCue();
	new HTMLPage(91950);
	
}

TravelLink::TravelLink()
{
	name = "travellink";
	view = 601;
	loop = 3;
	cel = 0;
	init(17,258);
	noSkip();									   
	
//	if()
//		new Alert(4, 247, 603, 0, 3);
	new CommandLine(left, top, width, height, "Go Online");
}

void TravelLink::activate(void)
{
	user->input = false;
	new ButtAnim(602,3,20,216,(Button*) this);
}

void TravelLink::cue(void)
{
	char drive[16];
	char urlname[256];
	memset (&drive,0,16);
	memset (&urlname,0,256);

	user->input = true;
	if(GameFlag.test(fOnlineAvailable))
	{
		if (intrpuzz->page->script)
			intrpuzz->page->setScript(NULL);	
		sfxDisableSound();
		sfxDisableMusic();
		sfxSystemCursor(SYS_ARROW);

		GetCDROM(drive);
		Common::strcat_s(urlname, "http://spycraft.activision.com/forward.cgi?site=http://spycraft.activision.com/spy1.html&cd=");
		Common::strcat_s(urlname, drive);
		theGame->startNet(urlname);
		sfxMadeCursor();
		sfxEnableSound();
		sfxEnableMusic();

		//intrpuzz->dispose();
		//	new(TravelPage);
	}
	else
	{
		sfxPrintf("No online component available.");
	}
	flushMessageCue();
}											 

ButtAnim::ButtAnim(int pic_id, int loop_id,int thex, int they, Button* from)
{
	name = "buttanim";
	view = pic_id;
	loop = loop_id;
	cueme = from;
	cycleSpeed = 2;
	init(thex,they);
	setScript(new AnimScript(from));
}

void AnimScript::changeState ( int newState )
{
	switchTo
	  ticks = 1;
    END

	BEG
	  ((ButtAnim *)client)->setCycle(new EndLoop(((Prop*)client), this));
    END

	BEG
		((ButtAnim *)client)->dispose();
		if (from)
			from->cue();
    END
}

void HomoScript::changeState ( int newState )
{
	static int web_virgin = true;
	switchTo
		entrance = new Sound;
		if (!strcmp(curRoom->name,"Rm1270") && (!comlinks->isEmpty()) && web_virgin)
		{
			web_virgin = false;
			entrance->canInterupt = true;
			entrance->number = 100004;
			entrance->playBuffered(100004,this);
		}
		else
		{
			entrance->number = 41001;
			entrance->play(41001,this);
		}
    END

	BEG											   
/*													    
		if (!strcmp(curRoom->name,"Rm1270"))
			ticks = 30;		
		else
		{
*/			entrance->canInterupt = false;
			((Home*)client)->script = NULL;
			dispose();
//		}
	END
/*	
	BEG	
		entrance->number = 41002;
		entrance->play(41002,this);
    END
	
	BEG
		ticks = 30;		
	END

	BEG
		entrance->number = 41003;
		entrance->play(41003,this);
    END												 

	BEG
		ticks = 30;		
	END

	BEG
		entrance->number = 41004;
		entrance->play(41004,this);										 
    END

	BEG
		((Home*)client)->script = NULL;
		dispose();
	END

//	BEG
//		entrance->stop();
//		entrance->number = 41005;
//		entrance->play();
//		((Home*)client)->setScript(NULL);
//   END
*/
}
HomoScript::~HomoScript()
{
	if (entrance)
	{
		entrance->caller = NULL;
		entrance->stop();
		delete entrance;
		entrance = NULL;
	}
}

LinkFlash::LinkFlash()
{
	name = "linkflash";
	view = 666;
	loop = 8;
	cel = 0;
	x = 365;
	y = 383;

	draw();
	web_intrface->addToFront ( this );
	comlinkshort->linkflash = this;
	setScript(new LinkFlashScript);
}

void LinkFlash::dispose()   
{
	web_intrface->release(this);
	deathList->add(this);
}	

void LinkFlashScript::changeState ( int newState )
{
	switchTo
	  seconds = 1;
    END

	BEG
	  ((LinkFlash *)client)->hide();
	  seconds = 1;
    END

	BEG
		((LinkFlash *)client)->show();
		((LinkFlash *)client)->setScript(new LinkFlashScript);
    END
}

QuitAlert::QuitAlert(int theX, int theY, int pic_id, int loop_id, int cel_id)
{
	init(theX,theY,pic_id,loop_id,cel_id);
	noSkip();
	setPri(500);
	setScript(new QuitAlertScript);
}

int QuitAlert::handleEvent(MADEEventStamp* event)
{
  return (false);
}

void QuitAlertScript::changeState ( int newState )
{
	switchTo
	  ticks = 30;
    END

	BEG
	  ((Alert *)client)->hide();
	  ticks = 30;
    END

	BEG
		((Alert *)client)->show();
		((Alert *)client)->setScript(new QuitAlertScript);
    END
}

Alert::Alert(int theX, int theY, int pic_id, int loop_id, int cel_id)
{
	IntrPuzzView::init(theX,theY,pic_id,loop_id,cel_id);
	noSkip();
	setPri(500);
	setScript(new AlertScript);
}	

int Alert::handleEvent(MADEEventStamp* event)
{
  return (false);
}

void AlertScript::changeState ( int newState )
{
	switchTo
	  ticks = 30;
    END

	BEG
	  ((Alert *)client)->hide();
	  ticks = 30;
    END

	BEG
		((Alert *)client)->show();
		((Alert *)client)->setScript(new AlertScript);
    END
}

GlobosAnim::GlobosAnim()
{
	name = "globosanim";
	view = 604;
	loop = 0;
	cel=0;
	cycleSpeed = 4;
	x = 465;
	y = 20;
	draw();
	web_intrface->addToFront ( this );
	setCycle(new EndLoop(((Prop*)this), this));
}

void GlobosAnim::cue()
{
	setCel(0);	
	if (!stopglobe)
		setCycle(new EndLoop(((Prop*)this), this));
	else
		setScript(new GlobosPause);
}


void GlobosPause::changeState ( int newState )
{
	switchTo
		seconds = 5;
    END

	BEG
		((GlobosAnim *)client)->setScript(new GlobosPause);
    END
}

WireAnim::WireAnim()
{
	name = "wireanim";
	view = 630;
	loop = 0;
	cel=0;
	cycleSpeed = 12;
	x = 144;
	y = 65;
	draw();
	intrpuzz->addToFront ( this );
	setCycle(new EndLoop(((Prop*)this), this));
}

void WireAnim::cue()
{
	setCel(0);	
	setCycle(new EndLoop(((Prop*)this), this));
}
ATFButt::ATFButt()
{
	name = "atflink";
	view = 630;
	loop = 1;
	cel = 0;
	init(20,86);
	noSkip();
}
void ATFButt::activate()
{
	intrpuzz->dispose();
	new HTMLPage(99600);
}
CIAButt::CIAButt()
{
	name = "cialink";
	view = 630;
	loop = 2;
	cel = 0;
	init(20,128);
	noSkip();
}
void CIAButt::activate()
{
	intrpuzz->dispose();
	new HTMLPage(99002);
}
DEAButt::DEAButt()
{
	name = "dealink";
	view = 630;
	loop = 3;
	cel = 0;
	init(20,165);
	noSkip();
}
void DEAButt::activate()
{
	intrpuzz->dispose();
	new HTMLPage(99800);
}
FBIButt::FBIButt()
{
	name = "fbilink";
	view = 630;
	loop = 4;
	cel = 0;
	init(20,202);
	noSkip();
}
void FBIButt::activate()
{
	intrpuzz->dispose();
	new HTMLPage(99250);
}
NROButt::NROButt()
{
	name = "nrolink";
	view = 630;
	loop = 5;
	cel = 0;
	init(20,237);
	noSkip();
}
void NROButt::activate()
{
	intrpuzz->dispose();
	new HTMLPage(99500);
}
NSAButt::NSAButt()
{
	name = "nsalink";
	view = 630;
	loop = 6;
	cel = 0;
	init(20,273);
	noSkip();
}
void NSAButt::activate()
{
	intrpuzz->dispose();
	new HTMLPage(99200);
}
USSSButt::USSSButt()
{
	name = "ussslink";
	view = 630;
	loop = 7;
	cel = 0;
	init(20,309);
	noSkip();
}
void USSSButt::activate()
{
	intrpuzz->dispose();
	new HTMLPage(99700);
}


/****************************
*
*		Mole Hunt Send Button 
*
*****************************/
MoleSendButt::MoleSendButt()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void MoleSendButt::activate()
{
	memset(&mailFlags,0,(400*sizeof(bool)));

	mailFlags[0][1] = true;
	mailFlags[0][2] = true;

	if(GameFlag.test(fSawCollinsDossier))
		mailFlags[0][0] = true;

	if(GameFlag.test(fSawGrenaldDossier))
		mailFlags[0][5] = true;

	if(GameFlag.test(109)) //fSawMenoherDossier))
	{
		mailFlags[0][4] = true;
		mailFlags[0][6] = true;
	}

	if(GameFlag.test(fEclipse2Avail))
	{
		mailFlags[0][7] = true;
		mailFlags[0][8] = true;
		mailFlags[0][9] = true;
	}

	if(GameFlag.test(fEclipse3Avail))
		mailFlags[0][3] = true;

	new EMailSend(299, this);
}

void MoleSendButt::cue()
{
/*	int numAccused = 0;

	if(GameFlag.test(fMoleWarhurst1)) //sent one email
		numAccused++;
	if(GameFlag.test(fMoleSterling1)) //sent two emails
		numAccused++;

	if(numAccused == 2)
	{
		if (!(webreport->flag & OBJECT_HIDDEN))
			WebReport::cue();

		comlinkshort = NULL;
		intrpuzz->quitIntrface();
		theInset->dispose();
		theGame->newRoom(new Rm60000(END_FIRED));
		return;	
	}
  */
	if(GameFlag.test(fEclipse2Avail))
	{
		if(GameFlag.test(fEclipse3Avail))
			new EMail("Mole Hunt: Colby, William", 98223);
		else
			new EMail("Mole Hunt: Colby, William", 98221);
	}
	else if(!GameFlag.test(fEclipse3Avail))
		new EMail("Mole Hunt: Colby, William", 98220);
																	
/*	if(numAccused == 1)
		GameFlag.set(fMoleSterling1); //sent two emails
	else
		GameFlag.test(fMoleWarhurst1); //sent one email
*/						
	if (!(webreport->flag & OBJECT_HIDDEN))
		WebReport::cue();
}

InfosciSendButt::InfosciSendButt()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void InfosciSendButt::activate()
{
	memset(&mailFlags,0,(400*sizeof(bool)));
	GameFlag.set(fReportInfosci);
	if (!GameFlag.test(fInfosciPartial))
	{
		mailFlags[0][0] = true;
		mailFlags[0][1] = true;
		mailFlags[0][2] = true;
		mailFlags[0][3] = true;
		if (GameFlag.test(fSawGrendelDossier))
			mailFlags[0][4] = true;
		if (GameFlag.test(fIDYingVoice))
			mailFlags[0][5] = true;
		if (GameFlag.test(fHeardPhoneCohen4))
			mailFlags[0][6] = true;
		if (GameFlag.test(fHeardPhoneCohen2))
			mailFlags[0][7] = true;
		if (GameFlag.test(fHeardPhoneBenton2))
			mailFlags[0][8] = true;
		if (GameFlag.test(fHeardPhoneBenton1))
			mailFlags[0][9] = true;
		if (GameFlag.test(fHeardPhoneBenton3))
			mailFlags[0][10] = true;
		if (GameFlag.test(fHeardPhoneBenton4))
			mailFlags[0][11] = true;
		if (GameFlag.test(fHeardPhoneMagruder1))
			mailFlags[0][12] = true;
		if (GameFlag.test(fHeardPhoneMagruder2))
			mailFlags[0][13] = true;
		if (GameFlag.test(fHeardPhoneSung2))
			mailFlags[0][14] = true;
		if (GameFlag.test(fHeardPhoneSung1))
			mailFlags[0][15] = true;

		new EMailSend(207, this);
	}
	else if (!GameFlag.test(fInfosciComplete))
	{
		if (!GameFlag.test(fKnowYingAddress))
		{
			if (GameFlag.test(fSawGrendelDossier))
			{
				mailFlags[0][0] = true;
				mailFlags[1][0] = true;
			}
			mailFlags[0][1] = true;
			mailFlags[0][2] = true;
			mailFlags[0][3] = true;
			mailFlags[1][1] = true;
			mailFlags[1][2] = true;
			mailFlags[1][3] = true;

			if (GameFlag.test(fIDYingVoice))
			{
				mailFlags[0][5] = true;
				mailFlags[1][5] = true;
			}
			if (GameFlag.test(fHeardPhoneCohen4))
			{
				mailFlags[0][6] = true;
				mailFlags[1][6] = true;
			}
			if (GameFlag.test(fHeardPhoneCohen2))
			{
				mailFlags[0][7] = true;
				mailFlags[1][7] = true;
			}
			if (GameFlag.test(fHeardPhoneBenton2))
			{
				mailFlags[0][8] = true;
				mailFlags[1][8] = true;
			}
			if (GameFlag.test(fHeardPhoneBenton1))
			{
				mailFlags[0][9] = true;
				mailFlags[1][9] = true;
			}
			if (GameFlag.test(fHeardPhoneBenton3))
			{
				mailFlags[0][10] = true;
				mailFlags[1][10] = true;
			}
			if (GameFlag.test(fHeardPhoneBenton4))
			{
				mailFlags[0][11] = true;
				mailFlags[1][11] = true;
			}
			if (GameFlag.test(fHeardPhoneMagruder1))
			{
				mailFlags[0][12] = true;
				mailFlags[1][12] = true;
			}
			if (GameFlag.test(fHeardPhoneMagruder2))
			{
				mailFlags[0][13] = true;
				mailFlags[1][13] = true;
			}
			if (GameFlag.test(fHeardPhoneSung2))
			{
				mailFlags[0][14] = true;
				mailFlags[1][14] = true;
			}
			if (GameFlag.test(fHeardPhoneSung1))
			{
				mailFlags[0][15] = true;
				mailFlags[1][15] = true;
			}

			if (GameFlag.test(fCrossPhoneCohen1))
				mailFlags[2][0] = true;
			if (GameFlag.test(fCrossPhoneCohen2))
				mailFlags[2][1] = true;
			if (GameFlag.test(fCrossPhoneCohen3))
				mailFlags[2][2] = true;
			if (GameFlag.test(fCrossPhoneCohen4))
				mailFlags[2][3] = true;
			if (GameFlag.test(fCrossPhoneBenton1))
				mailFlags[2][4] = true;
			if (GameFlag.test(fCrossPhoneBenton2))
				mailFlags[2][5] = true;
			if (GameFlag.test(fCrossPhoneBenton3))
				mailFlags[2][6] = true;
			if (GameFlag.test(fCrossPhoneBenton4))
				mailFlags[2][7] = true;
			if (GameFlag.test(fCrossPhoneMagruder1))
				mailFlags[2][8] = true;
			if (GameFlag.test(fCrossPhoneMagruder2))
				mailFlags[2][9] = true;
			if (GameFlag.test(fCrossPhoneSung1))
				mailFlags[2][10] = true;
			if (GameFlag.test(fCrossPhoneSung2))
				mailFlags[2][11] = true;

				mailFlags[2][12] = true;
			new EMailSend(208, this);
		}
		else
		{
			if (GameFlag.test(fCrossPhoneCohen1))
				mailFlags[0][0] = true;
			if (GameFlag.test(fCrossPhoneCohen2))
				mailFlags[0][1] = true;
			if (GameFlag.test(fCrossPhoneCohen3))
				mailFlags[0][2] = true;
			if (GameFlag.test(fCrossPhoneCohen4))
				mailFlags[0][3] = true;
			if (GameFlag.test(fCrossPhoneBenton1))
				mailFlags[0][4] = true;
			if (GameFlag.test(fCrossPhoneBenton2))
				mailFlags[0][5] = true;
			if (GameFlag.test(fCrossPhoneBenton3))
				mailFlags[0][6] = true;
			if (GameFlag.test(fCrossPhoneBenton4))
				mailFlags[0][7] = true;
			if (GameFlag.test(fCrossPhoneMagruder1))
				mailFlags[0][8] = true;
			if (GameFlag.test(fCrossPhoneMagruder2))
				mailFlags[0][9] = true;
			if (GameFlag.test(fCrossPhoneSung1))
				mailFlags[0][10] = true;
			if (GameFlag.test(fCrossPhoneSung2))
				mailFlags[0][11] = true;

			mailFlags[0][12] = true;
			new EMailSend(209, this);
		}
	}
	else
	{
		new EMailSend(230,this);
	}
}

void InfosciSendButt::cue()
{
	static int flipflop = true;

	if (!GameFlag.test(fInfosciPartial))
	{
		if(choiceIndex[0] == 4)	
		{
			new EMail("There's still more: Holt, David M.",98901);
			GameFlag.set(fInfosciPartial);
		}
		else if(choiceIndex[0] == 5)	
		{
			new EMail("There's still more: Holt, David M.",98900);
			GameFlag.set(fInfosciPartial);
		}
		else if (flipflop)
				new VoiceMail("No good: Holt, David M.",98007, HOLT);
			else
				new VoiceMail("Think it through: Holt, David M.",98008, HOLT);
	}
	else if (!GameFlag.test(fInfosciComplete))
	{
		if (!GameFlag.test(fKnowYingAddress))
		{
			if ((choiceIndex[0] == 0 && choiceIndex[1] == 5) || (choiceIndex[1] == 0 && choiceIndex[0] == 5))
			{
				if (choiceIndex[2] == 2)
				{
 					new VoiceMail("Good Work: Warhurst, Eugene J.", 98900,WARHURST);
					GameFlag.set(fInfosciComplete);
					//new EMail("Report Immediately: Source Unknown",98120,fColbySummons,0);
					//new VoiceMail( "No luck : Lange, Theodore H.", 99020, LANGE);
					GameFlag.set(fInit99020 );
				}
				else
				{
					new EMail("No Dice",98902);
					GameFlag.set(fKnowYingAddress);		
				}
			}
			else if (flipflop)
				new VoiceMail("No good: Holt, David M.",98007, HOLT);
			else
				new VoiceMail("Think it through: Holt, David M.",98008, HOLT);
		}
		else
		{
			if (choiceIndex[0] == 2)
			{
				new VoiceMail("Good Work: Warhurst, Eugene J.", 98900,WARHURST);
				GameFlag.set(fInfosciComplete);
				//new EMail("Report",98120,fColbySummons,0);
				//new VoiceMail( "No luck : Lange, Theodore H.", 99020, LANGE);
				GameFlag.set(fInit99020 );
			}
			else if (flipflop)
				new VoiceMail("No good: Holt, David M.",98007, HOLT);
			else
				new VoiceMail("Think it through: Holt, David M.",98008, HOLT);
		}
	}
	if (GameFlag.test(fDCK10))
	{
		intrpuzz->setScript(new RedrawScript);
	}
	else if (!(webreport->flag & OBJECT_HIDDEN))
		WebReport::cue();
}

/***************************
*
*	@&$%!#* REPORT BUTTON!!!
*
****************************/
class ReportPage : public IntrPuzzPage
{
public:
	ReportPage();
};


WebReport::WebReport()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void WebReport::activate()
{										 			  
	int onHTMPage = false;
	int default_butt = false; 
	GameFlag.set(fPatsHTM_130);
	if (intrpuzz->page->site->flag == WEB_HTMLPAGE || intrpuzz->page->site->flag == WEB_HOME )
	{
		intrpuzz->dispose();
		new ReportPage;
		onHTMPage = true;
	}							  
	else if (intrpuzz->page->site->flag == WEB_VLINKS)
	{
		intrpuzz->dispose();
		new ReportPage;
		comlinkshort->hide();
		onHTMPage = true;
	}
	dispose();

//here's where I'm gonna check on the send crap and see which one to bring up!
	if(GameFlag.test(fGreg20))  //this is the mole hunt puzzle( all HTM)
	{
		webreport = new MoleSendButt;
		webreport->activate();
	}		
	else if(GameFlag.test(fPatsHTM_120))	//second I.A. puzzle in farm
	{
		webreport = new TankSendBut;
		webreport->activate();
	}
	else if(GameFlag.test(fPatsHTM_119))	//first I.A. puzzle in farm
	{
		webreport = new PlateSendBut;
		webreport->activate();
	}		
	else
	{
		switch (lasttool)
		{
			case WEB_SOUND_C8:	
			case WEB_SOUND_C13:	
			case WEB_SOUND_C21:	
			case WEB_SOUND_C26:	
			case WEB_SOUND_B6:	
			case WEB_SOUND_B14:	
			case WEB_SOUND_B14B:
			case WEB_SOUND_B20:	
			case WEB_SOUND_M14:	
			case WEB_SOUND_M28:	
			case WEB_SOUND_S19:	
			case WEB_SOUND_S22:	
			case WEB_SOUND_VRC5:
			case WEB_SOUND_VRC9:
			case WEB_SOUND_VRC23:
			case WEB_SOUND_VRB9:
			case WEB_SOUND_VRB15:
			case WEB_SOUND_VRM5:
			case WEB_SOUND_VRM9:
			case WEB_SOUND_VRS9:
			case WEB_SOUND_VRS29:
			case WEB_GRENDELMATCH:
				if (GameFlag.test(fIDPEG) && !GameFlag.test(fInfosciComplete))
				{
					webreport = new InfosciSendButt;
//					webreport->activate();
				}
				else
					default_butt = true;				
				break;
			case WEB_YINGCYPHER:
				if (!GameFlag.test(fKnowYingInMoscow))
				{
					webreport = new YingSendBut;
//					webreport->activate();
				}
				else
					default_butt = true;				
				break;
			case WEB_IMAGEPEG:
				if (GameFlag.test(fSolvedDubanskyKAT) && !GameFlag.test(fIDPEG))
				{
					webreport = new PegSendBut;
//					webreport->activate();
				}
				else
					default_butt = true;				
				break;
			case WEB_KAT:
			case WEB_KATMATCH:
				if (GameFlag.test(fFullBriefing) && !GameFlag.test(fSolvedDubanskyKAT))
				{
					webreport = new KatSendBut;
//					webreport->activate();
				}
				else
					default_butt = true;				
				break;
			case WEB_ONYXHUNTONE:
			case WEB_ONYXHUNTTWO:
				if (!GameFlag.test(fOnyxHunt1))
				{
					webreport = new OnyxOneSendBut;
//					webreport->activate();
				}
				else if (GameFlag.test(fOnceOnyx48000) && !GameFlag.test(fOnyxHunt2))
				{
					webreport = new OnyxTwoSendBut;
//					webreport->activate();
				}
				else
					default_butt = true;				
				break;
			case WEB_MAP:
			case WEB_SOUND_BIRD:
				if (GameFlag.test(fMaxReportBSDis) && !GameFlag.test(fDoneSoundAnal))
				{
					webreport = new MapSendBut;
//					webreport->activate();
				}
				else
					default_butt = true;				
				break;
			case WEB_KNEE:
			case WEB_IMAGEKNEEID:
				if (!GameFlag.test(fKneecapsFound))
				{
					webreport = new CapsSendBut;
//					webreport->activate();
				}
				else
					default_butt = true;				
				break;
		
			default:
				default_butt = true;				
/*				webreport = new WebReport;
				if (!(GameFlag.test(fFullBriefing)))
					new EMailSend(229, webreport);
				else
					new EMailSend(230, webreport);
*/				break;
		}
		if (default_butt)
		{												  
			webreport = new WebReport;
			if (!(GameFlag.test(fFullBriefing)))
				new EMailSend(229, webreport);
			else
				new EMailSend(230, webreport);

		}
		else
		{
			webreport->activate();
		}
	}
/*
	else if (GameFlag.test(fFullBriefing) && !GameFlag.test(fSolvedDubanskyKAT))
	{
		webreport = new KatSendBut;
		webreport->activate();
	}
	else if (GameFlag.test(fSolvedDubanskyKAT) && !GameFlag.test(fIDPEG))
	{
		webreport = new PegSendBut;
		webreport->activate();
	}
	else if (GameFlag.test(fIDPEG) && !GameFlag.test(fInfosciComplete))
	{
		webreport = new InfosciSendButt;
		webreport->activate();
	}
	else if (GameFlag.test(fMaxReportBSDis) && !GameFlag.test(fDoneSoundAnal))
	{
		webreport = new MapSendBut;
		webreport->activate();
	}
	else if (GameFlag.test(fSawAchtung) && !GameFlag.test(fOnyxHunt1))
	{
		webreport = new OnyxOneSendBut;
		webreport->activate();
	}
	else if (GameFlag.test(fOnceOnyx48000) && !GameFlag.test(fOnyxHunt2))
	{
		webreport = new OnyxTwoSendBut;
		webreport->activate();
	}
	else if (GameFlag.test(fPickedUpCamera) && !GameFlag.test(fSawKneeAddress))
	{
		webreport = new CapsSendBut;
		webreport->activate();
	}
	else if (GameFlag.test(fSaw156) && !GameFlag.test(fBrokeYingBealeCode))
	{
		webreport = new YingSendBut;
		webreport->activate();
	}
	else
	{
		webreport = new WebReport;
		new EMailSend(230, webreport);
	}
*/
	GameFlag.clear(fPatsHTM_130);

}

void WebReport::cue()
{
	Website * id = webmap->findPage();

	if (!intrpuzz->page->site)
	{
		if (id)
		{
			//let the game know a button was used
			usedbutton = true;
			//allow forward button to operate
	//		fwdbutton = true;
			intrpuzz->dispose();
			changePage (id);	
		}
	}
	dispose();
	webreport = new WebReport;

	switch(intrpuzz->page->site->flag)
	{
		case WEB_SOUND_C8:	
		case WEB_SOUND_C13:	
		case WEB_SOUND_C21:	
		case WEB_SOUND_C26:	
		case WEB_SOUND_B6:	
		case WEB_SOUND_B14:	
		case WEB_SOUND_B14B:
		case WEB_SOUND_B20:	
		case WEB_SOUND_M14:	
		case WEB_SOUND_M28:	
		case WEB_SOUND_S19:	
		case WEB_SOUND_S22:	
		case WEB_SOUND_VRC5:
		case WEB_SOUND_VRC9:
		case WEB_SOUND_VRC23:
		case WEB_SOUND_VRB9:
		case WEB_SOUND_VRB15:
		case WEB_SOUND_VRM5:
		case WEB_SOUND_VRM9:
		case WEB_SOUND_VRS9:
		case WEB_SOUND_VRS29:
		case WEB_GRENDELMATCH:
		case WEB_YINGCYPHER:
		case WEB_IMAGEPEG:
		case WEB_KAT:
		case WEB_KATMATCH:
		case WEB_ONYXHUNTONE:
		case WEB_ONYXHUNTTWO:
		case WEB_MAP:
		case WEB_SOUND_BIRD:
		case WEB_KNEE:
		case WEB_IMAGEKNEEID:
			webreport->hide();
			break;
	}
}
void WebReport::dispose()
{
	if ( hotspot )
	{
		setHotspot ( NULL );
		hotspot = NULL;
	}

	web_intrface->release ( this );
	deathList->add(this);
}

ReportPage::ReportPage()
{
	name = "reportpage";
	init(WEB_REPORT);
}
 
} // namespace Spycraft
