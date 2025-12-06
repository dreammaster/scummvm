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

#include "spycraft/game/maptool.h"
#include "spycraft/game/sndanal.h"
#include "spycraft/game/email.h"
#include "spycraft/game/flag.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

MapTool * tool;
MapInfo * mapinfo;
Map		* map;
MapButton	*buttPtr[5];
RussMapButton *russbuttptr[5];
CommandLine	*comLine[5];
IntrPuzzView	*cityMap;


/***********************************
 *		
 *		 Voice over sound object
 *		      
 **********************************/
void MapSound::playStream(int soundNumber)
{
	if ( isPlaying() )
	{
		stop();
	}
	playBuffered(soundNumber, NULL);
}

void MapSound::play(int soundNumber)
{
	if ( isPlaying() )
	{
		stop();
	}
	Sound::play(soundNumber, NULL);
}

void MapSound::play(int soundNumber, Object* whoToCue)
{
	if ( isPlaying() )
	{
		stop();
	}
	Sound::play(soundNumber, whoToCue);
}

/*********************
*
*		Page
*												  
**********************/

MapTool::MapTool()
{
	name = "MapTool";
 
	//testes	
/*	new(MapInfo);
	mapinfo->jet = true;
	mapinfo->car = true;
	mapinfo->church = true;
	mapinfo->arm = true;
	mapinfo->bird = true;
	///////////////////
*/  
	IntrPuzzPage::init(WEB_MAP);
	helppage = 92750;
	tool = this;
 	memset (&mailFlags,0,(5 * 80)*sizeof(bool));
	map = NULL;

	numButtsChecked = 0;
	numActiveButts = 0;
	totalButts = 0;
	createSpots();
	webreport->hide();
	mapreport = new(MapSendBut);
 	new MapToolBack();							    
	mapSound = new(MapSound);
	mapSound->playStream(45035);
}

void MapTool::removeSoundButtons()
{
	for(int i = 0; i < tool->numActiveButts; i++)
	{
		buttPtr[i]->dispose();
		comLine[i]->dispose();
	}	 	
}

void MapTool::createSpots()
{
 	erevanspot = new (ErevanSpot);
 	duchspot = new(DuchSpot);
	irkspot = new(IrkSpot);
 	moscowspot = new(MoscowSpot);
	rostovspot = new(RostovSpot);
	peterspot = new(PeterSpot);
	novosspot = new(NovosSpot);

	names = new(IntrPuzzView);
	names->init(3, 65, 643, 5, 0);
	names->setPri(608);
	
	int index = 0;

	if(mapinfo->jet)
	{
		russbuttptr[index] = new RussJetButt(index);
		index++;
	}
			
	if(mapinfo->car)
	{
		russbuttptr[index] = new RussCarButt(index);
		index++;
	}

	if(mapinfo->church)
	{
		russbuttptr[index] = new RussChurchButt(index);
		index++;
	}
			
	if(mapinfo->arm)
	{
		russbuttptr[index] = new RussRangeButt(index);
		index++;
	}
	
	if(mapinfo->bird)
	{
	 	russbuttptr[index] = new RussBirdButt(index);
		index++;
	}
	numActiveButts = index;
}

void MapTool::removeSpots()
{
	for(int i = 0; i < tool->numActiveButts; i++)
	{
		russbuttptr[i]->sound->dispose();	
		russbuttptr[i]->dispose();
		comLine[i]->dispose();
	}	  		
	names->dispose();
	erevanspot->comLine->dispose();
	erevanspot->dispose();
	duchspot->comLine->dispose();
	duchspot->dispose(); 
	irkspot->comLine->dispose();
	irkspot->dispose();  
	moscowspot->comLine->dispose();
	moscowspot->dispose();
	rostovspot->comLine->dispose();
	rostovspot->dispose();
	peterspot->comLine->dispose();
	peterspot->dispose(); 
	novosspot->comLine->dispose();
	novosspot->dispose(); 
}

MapTool::~MapTool()
{	  
}

void MapTool::dispose()
{

	if (web_intrface)
	{
		webreport->show();
		mapreport->dispose();
	}
	mapSound->dispose();
}

void MapTool::saveState()
{
/*
	if(!intrpuzz->isStateSaved("mapinfo"))
	{
		new(MapInfo);
	}		
	mapinfo->save();
*/
}

MapToolBack::MapToolBack()
{
	name = "backgrnd";
	view = 626;
	loop = 0;
	cel = 0;
	init(0,15);
	noSkip();
	addToPic();
  	dispose();
}

 Map::Map(int theView)
{
	name = "map";
	view = theView;
	loop = 0;
	cel = 0;
	init(3,65);
	map = this;
	setPri(650);
}
	
/*********************
*
*		Features
*												  
**********************/
ErevanSpot::ErevanSpot()
{
	name = "ErevanSpot";
	init(13, 233, 63, 48);
	comLine = new CommandLine(left, top, 63, 48, "Click to zoom on Erevan map.");
}

int ErevanSpot::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if (event_type == USER_LEFT_UP)	
		{
			//if the sound is unidentified then don't create the button
			mapinfo->seenErevan = true;
			tool->removeSpots();
			tool->numButtsChecked = 0;
			for(int i = 0; i < 5; i++)
			{
				tool->checked[i] = false;
			}
			int index = 0;

			if(mapinfo->jet)
			{
				buttPtr[index] = new JetButt(index, 0);
				index++;
			}	

			if(mapinfo->car)
			{
				buttPtr[index] = new CarButt(index, 1);
				index++;
			}

			if(mapinfo->church)
			{
				buttPtr[index] = new ChurchButt(index, 2);
				index++;
			}

			if(mapinfo->arm)
			{
				buttPtr[index] = new RangeButt(index, 3);
				index++;
			}
			tool->numActiveButts = index;
			tool->totalButts = 4;

			cityMap = new(IntrPuzzView);
			cityMap->init(3, 65, EREVAN, 0, 0);	
			cityMap->setPri(500);
			new Map(EREVAN);				
			map->currentMapView = EREVAN;
			new(BackButt);
			tool->mapSound->playStream(45044);
			return true;
		}
	}
	return false;
}

DuchSpot::DuchSpot()
{
	name = "DuchSpot";
	init(87, 312, 71, 34);
	comLine = new CommandLine(87, 312, 71, 34, "Click to zoom on Duchanbe map.");
}

int DuchSpot::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if (event_type == USER_LEFT_UP)	//clear the slider button down
		{
			//if the sound is unidentified then don't create the button
			mapinfo->seenDuchanbe = true;
			tool->removeSpots();
			tool->numButtsChecked = 0;
			for(int i = 0; i < 5; i++)
			{
				tool->checked[i] = false;
			}

			int index = 0;
			if(mapinfo->jet)
			{
				buttPtr[index] = new JetButt(index, 0);
				index++;
			}	

			if(mapinfo->bird)
			{
				buttPtr[index] = new BirdButt(index, 1);
				index++;
			}

			if(mapinfo->church)
			{
				buttPtr[index] = new ChurchButt(index, 2);
				index++;
			}
			tool->numActiveButts = index;
			tool->totalButts = 3;

			cityMap = new(IntrPuzzView);
			cityMap->init(3, 65, DUCHANBE, 0, 0);	
			cityMap->setPri(600);
			new Map(DUCHANBE);				
			map->currentMapView = DUCHANBE;
			new(BackButt);
			tool->mapSound->playStream(45045);
			return true;
		}
	}
	return false;
}

IrkSpot::IrkSpot()
{
	name = "IrkSpot";
	init(310, 240, 53, 35);
	comLine = new CommandLine(310, 240, 53, 35, "Click to zoom on Irkutsk map.");
}

int IrkSpot::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if (event_type == USER_LEFT_UP)	//clear the slider button down
		{
			//if the sound is unidentified then don't create the button
			mapinfo->seenIrkutsk = true;
			tool->removeSpots();
			tool->numButtsChecked = 0;
			for(int i = 0; i < 5; i++)
			{
				tool->checked[i] = false;
			}

			int index = 0;
			if(mapinfo->jet)
			{
				buttPtr[index] = new JetButt(index, 0);
				index++;
			}	

			if(mapinfo->church)
			{
				buttPtr[index] = new ChurchButt(index, 1);
				index++;
			}
			tool->numActiveButts = index;
			tool->totalButts = 2;

			cityMap = new(IntrPuzzView);
			cityMap->init(3, 65, IRKUTSK, 0, 0);	
			cityMap->setPri(500);
			new Map(IRKUTSK);				
			map->currentMapView = IRKUTSK;
			new(BackButt);
			tool->mapSound->playStream(45043);
			return true;
		}
	}
	return false;
}

MoscowSpot::MoscowSpot()
{
	name = "MoscowSpot";
	init(107, 143, 69, 32);
	comLine = new CommandLine(107, 143, 69, 32, "Click to zoom on Moscow map.");
}

int MoscowSpot::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if (event_type == USER_LEFT_UP)	//clear the slider button down
		{
			//if the sound is unidentified then don't create the button
			mapinfo->seenMoscow = true;
			tool->removeSpots();
			tool->numButtsChecked = 0;
			for(int i = 0; i < 5; i++)
			{
				tool->checked[i] = false;
			}

			int index = 0;
			if(mapinfo->jet)
			{
				buttPtr[index] = new JetButt(index, 0);
				index++;
			}	

			if(mapinfo->car)
			{
				buttPtr[index] = new CarButt(index, 1);
				index++;
			}

			if(mapinfo->church)
			{
				buttPtr[index] = new ChurchButt(index, 2);
				index++;
			}
			tool->numActiveButts = index;
			tool->totalButts = 3;

			cityMap = new(IntrPuzzView);
			cityMap->init(3, 65, MOSCOW, 0, 0);	
			cityMap->setPri(500);
			new Map(MOSCOW);				
			map->currentMapView = MOSCOW;
			new(BackButt);
			tool->mapSound->playStream(45039);
			return true;
		}
	}
	return false;
}

RostovSpot::RostovSpot()
{
	name = "RostovSpot";
	init(16, 180, 60, 30);
	comLine = new CommandLine(16, 180, 60, 30, "Click to zoom on Rostov map.");
}

int RostovSpot::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if (event_type == USER_LEFT_UP)	//clear the slider button down
		{
			//if the sound is unidentified then don't create the button
			mapinfo->seenRostov = true;
			tool->removeSpots();
			tool->numButtsChecked = 0;
			for(int i = 0; i < 5; i++)
			{
				tool->checked[i] = false;
			}

			int index = 0;
			if(mapinfo->jet)
			{
				buttPtr[index] = new JetButt(index, 0);
				index++;
			}	

			if(mapinfo->car)
			{
				buttPtr[index] = new CarButt(index, 1);
				index++;
			}

			if(mapinfo->church)
			{
				buttPtr[index] = new ChurchButt(index, 2);
				index++;
			}
			tool->numActiveButts = index;
			tool->totalButts = 3;

			cityMap = new(IntrPuzzView);
			cityMap->init(3, 65, ROSTOV, 0, 0);	
			cityMap->setPri(500);
			new Map(ROSTOV);				
			map->currentMapView = ROSTOV;
			new(BackButt);
			tool->mapSound->playStream(45041);
			return true;
		}
	}
	return false;
}

NovosSpot::NovosSpot()
{
	name = "MoscowSpot";
	init(225, 215, 84, 25);
	comLine = new CommandLine(225, 215, 84, 25, "Click to zoom on Novosibirsk map.");
}

int NovosSpot::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if (event_type == USER_LEFT_UP)	//clear the slider button down
		{
			//if the sound is unidentified then don't create the button
			mapinfo->seenNov = true;
			tool->removeSpots();
			tool->numButtsChecked = 0;
			for(int i = 0; i < 5; i++)
			{
				tool->checked[i] = false;
			}

			int index = 0;
			if(mapinfo->jet)
			{
				buttPtr[index] = new JetButt(index, 0);
				index++;
			}	

			if(mapinfo->car)
			{
				buttPtr[index] = new CarButt(index, 1);
				index++;
			}

			if(mapinfo->church)
			{
				buttPtr[index] = new ChurchButt(index, 2);
				index++;
			}
			tool->numActiveButts = index;
			tool->totalButts = 3;

			cityMap = new(IntrPuzzView);
			cityMap->init(3, 65, NOVOSIBIRSK, 0, 0);	
			cityMap->setPri(500);
			new Map(NOVOSIBIRSK);				
			map->currentMapView = NOVOSIBIRSK;
			new(BackButt);
			tool->mapSound->playStream(45042);
			return true;
		}
	}
	return false;
}

PeterSpot::PeterSpot()
{
	name = "MoscowSpot";
	init(106, 115, 88, 26);
	comLine = new CommandLine(106, 115, 88, 26, "Click to zoom on St. Petersburg map.");
}

int PeterSpot::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if (event_type == USER_LEFT_UP)	//clear the slider button down
		{
			//if the sound is unidentified then don't create the button
			mapinfo->seenPeters = true;
			tool->removeSpots();
			tool->numButtsChecked = 0;
			for(int i = 0; i < 5; i++)
			{
				tool->checked[i] = false;
			}

			int index = 0;
			if(mapinfo->jet)
			{
				buttPtr[index] = new JetButt(index, 0);
				index++;
			}	

			if(mapinfo->car)
			{
				buttPtr[index] = new CarButt(index, 1);
				index++;
			}

			if(mapinfo->church)
			{
				buttPtr[index] = new ChurchButt(index, 2);
				index++;
			}
			tool->numActiveButts = index;
			tool->totalButts = 3;

			cityMap = new(IntrPuzzView);
			cityMap->init(3, 65, PETERSBURG, 0, 0);	
			cityMap->setPri(500);
			new Map(PETERSBURG);				
			map->currentMapView = PETERSBURG;
			new(BackButt);
			tool->mapSound->playStream(45040);
			return true;
		}
	}
	return false;
}

/*********************
*
*		BUTTONS
*												  
**********************/
BackButt::BackButt()
{
	name = "backbutt";
	view = 625;
	loop = 0;
	cel = 0;
	init(476,313);
	setPri(660);
	comLine = new CommandLine(left, top, right - left, bottom - top, "Zoom out to Russia map.");
}

void BackButt::activate()
{
	tool->removeSoundButtons();
	map->dispose();
	map = NULL;
	cityMap->dispose();
	tool->createSpots();
	tool->mapSound->stop();
	comLine->dispose();
	dispose();
}
		
void MapButton::setMap()
{
	int celIndex;
	int i, j;		    

	switch(tool->numButtsChecked)
	{
		case 0: 
			map->setLoop(0);	
			map->setView(map->currentMapView);
			map->setPri(650);
		break;
			
		case 1:
			for(i = 0; i < tool->totalButts; i++)
			{
				if(tool->checked[i])
				{
					map->setLoop(0);	
					map->setView(map->currentMapView + 1);
					map->setLoop(i);	
					map->setPri(650);
				}
			}
		break;

		case 2:
			celIndex = 0;  //this is the start of cels with two layers
			for(i = 0; i < tool->totalButts; i++)
			{
				if(tool->checked[i])
				{
					for(j = (i + 1); j < tool->totalButts; j++)
					{
						if(tool->checked[j])
						{
							map->setLoop(0);
							map->setView(map->currentMapView + 2);
							map->setLoop(celIndex);
							map->setPri(650);
							return;
						}
						celIndex++;
					}			
				}
				celIndex += (tool->totalButts - 1) - i;
			}								
		break;
			
		case 3:
			celIndex = 0;  //this is the start of cels with three layers
			for(i = 0; i < tool->totalButts; i++)
			{
				if(tool->checked[i])
				{
					bool gotTwo = false;
					for(j = (i + 1); j < tool->totalButts; j++)
					{
						if(tool->checked[j])
						{
							if(!gotTwo)
							{
								gotTwo = true;
								//celIndex--;
							}
							else
							{
								map->setLoop(0);
								map->setView(map->currentMapView + 3);
								map->setLoop(celIndex);
								map->setPri(650);
								return;
							}	
						}
						else
						{
							celIndex += (tool->totalButts - 1) - j ;
						}
					}			
				}
				celIndex += (tool->totalButts - 1) - i;
			}								
		break;

		case 4:
			map->setLoop(0);
			map->setView(map->currentMapView + 4);
			map->setPri(650);
		break;
	}
}
	   
void MapButton::activate(void)
{
	tool->mapSound->stop();
	if(tool->checked[buttID])
	{
		tool->checked[buttID] = false;
		tool->numButtsChecked--;
	}
	else
	{
		tool->checked[buttID] = true;
		tool->numButtsChecked++;
	}
	setMap();
}
	
int MapButton::handleEvent(MADEEventStamp* event)
{
	if (user->input)
	{
		if (onMe(event))  
		{
/*	  	if (event_type == USER_LEFT_UP)
	    {
	   		if (cel == 1)
	   		{
		   		setCel(0);
				activate();
				return(true);
			}
	    }
*/			if (event_type == USER_LEFT_DOWN)
			{
		   		if (cel == 1)
		   		{
			   		setCel(0);
					activate();
					return(true);
				}
				if (cel == 0)
				{
					setCel(1);
					activate();
					return (true);
				}
			}
		}
	}
  return false;
}
/*********************
*										  
*		city map buttons
*												  
**********************/
JetButt::JetButt(int index, int id)
{
	name = "soundbutt";
	view = 625;
	loop = 1;
	cel = 0;
	int theX = (index * 100) + 10;
	init(theX,348);
	buttID = id;
	noSkip();
	comLine[index] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Jet aircraft sounds.");
}

CarButt::CarButt(int index, int id)
{
	name = "soundbutt";
	view = 625;
	loop = 2;
	cel = 0;
	int theX = (index * 100) + 10;
	init(theX,348);
	buttID = id;
	noSkip();
	comLine[index] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Street Car sounds.");
}

BirdButt::BirdButt(int index, int id)
{
	name = "soundbutt";
	view = 625;
	loop = 3;
	cel = 0;
	int theX = (index * 100) + 10;
	init(theX,348);
	buttID = id;
	noSkip();						 
	comLine[index] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Bird Range sounds.");
}

ChurchButt::ChurchButt(int index, int id)
{
	name = "soundbutt";
	view = 625;
	loop = 4;
	cel = 0;
	int theX = (index * 100) + 10;
	init(theX,348);
	buttID = id;
	noSkip();
	comLine[index] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Church Bell sounds.");
}

RangeButt::RangeButt(int index, int id)
{
	name = "soundbutt";
	view = 625;
	loop = 5;
	cel = 0;
	int theX = (index * 100) + 10;
	init(theX,348);
	buttID = id;
	noSkip();
	comLine[index] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Armenian Range sounds.");
}

/*********************
*
*	russia map buttons
*												  
**********************/
int RussMapButton::handleEvent(MADEEventStamp* event)
{
	if (user->input)
	{
		if (onMe(event))  
		{
/*	  	if (event_type == USER_LEFT_UP)
	    {
	   		if (cel == 1)
	   		{
		   		setCel(0);
				activate();
				return(true);
			}
	    }
*/			if (event_type == USER_LEFT_DOWN)
			{
		   		if (cel == 1)
		   		{
			   		setCel(0);
					activate();
					return(true);
				}
				if (cel == 0)
				{
					setCel(1);
					activate();
					return (true);
				}
			}
		}
	}
  return false;
}

void RussMapButton::activate()
{
	if(hidden)
	{
		hidden = false;
		sound->show();
	}
	else
	{
		hidden = true;
		sound->hide();
	}
}

RussJetButt::RussJetButt(int id)
{
	name = "soundbutt";
	view = 625;
	loop = 1;
	cel = 0;
	int theX = (id * 100) + 10;
	init(theX, 348);
	buttID = id;
	noSkip();
	setPri(602);
	comLine[buttID] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Jet aircraft sounds.");
	sound = new(IntrPuzzView);
	sound->init(3, 65, 643, 2, 0);	
	sound->setPri(604);
	sound->hide();
	hidden = true;
}


RussCarButt::RussCarButt(int id)
{
	name = "soundbutt";
	view = 625;
	loop = 2;
	cel = 0;
	int theX = (id * 100) + 10;
	init(theX,348);
	buttID = id;
	noSkip();
	comLine[buttID] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Street Car sounds.");
	sound = new(IntrPuzzView);
	sound->init(3, 65, 643, 1, 0);	
	sound->setPri(605);
	sound->hide();
	hidden = true;
}

RussBirdButt::RussBirdButt(int id)
{
	name = "soundbutt";
	view = 625;
	loop = 3;
	cel = 0;
	int theX = (id * 100) + 10;
	init(theX,348);
	buttID = id;
	noSkip();						 
	comLine[buttID] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Bird Range sounds.");
	sound = new(IntrPuzzView);
	sound->init(3, 65, 643, 4, 0);	
	sound->setPri(601);
	sound->hide();
	hidden = true;
}

RussChurchButt::RussChurchButt(int id)
{
	name = "soundbutt";
	view = 625;
	loop = 4;
	cel = 0;
	int theX = (id * 100) + 10;
	init(theX,348);
	buttID = id;
	noSkip();
	comLine[buttID] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Church Bell sounds.");
	sound = new(IntrPuzzView);
	sound->init(3, 65, 643, 0, 0);	
	sound->setPri(603);
	sound->hide();
	hidden = true;
}

RussRangeButt::RussRangeButt(int id)
{
	name = "soundbutt";
	view = 625;
	loop = 5;
	cel = 0;
	int theX = (id * 100) + 10;
	init(theX,348);
	buttID = id;
	noSkip();
	comLine[buttID] = new CommandLine(left, top, 27, 26, "Overlay possible locations of Armenian Range sounds.");
	sound = new(IntrPuzzView);
	sound->init(3, 65, 643, 3, 0);	
	sound->setPri(602);
	sound->hide();
	hidden = true;
}

/***********************************
 *		
 *		 Send Button
 *		      
 **********************************/
MapSendBut::MapSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void MapSendBut::activate()
{
	if ((webreport->flag & OBJECT_HIDDEN) && !GameFlag.test(fDCK10))
		tool->mapSound->stop();

	GameFlag.set(fReportMapTool);
	if(mapinfo->seenDuchanbe)
	{
		mailFlags[0][0] = true;
		mailFlags[0][4] = true;
	}
		
	if(mapinfo->seenErevan)
	{
		mailFlags[0][1] = true;
		mailFlags[0][3] = true;
	}

	if(mapinfo->seenPeters)
	{
		mailFlags[0][2] = true;
		mailFlags[0][5] = true;
		mailFlags[0][12] = true;
	}

	if(mapinfo->seenNov)
	{
		mailFlags[0][6] = true;
		mailFlags[0][8] = true;
		mailFlags[0][11] = true;
	}

	if(mapinfo->seenMoscow)
	{
		mailFlags[0][9] = true;
		mailFlags[0][13] = true;
		mailFlags[0][16] = true;
		mailFlags[0][17] = true;
	}

	if(mapinfo->seenRostov)
	{
		mailFlags[0][10] = true;
		mailFlags[0][14] = true;
		mailFlags[0][15] = true;
	}

	if(mapinfo->seenIrkutsk)
	{
		mailFlags[0][7] = true;
	}
	mailFlags[0][17] = true;

	if(!GameFlag.test(fDoneSoundAnal))
		new EMailSend(205, this);
	else if (!(GameFlag.test(fFullBriefing)))
		new EMailSend(229, this);
	else
		new EMailSend(230, this);
}

void MapSendBut::cue()
{
	bool fireTheJackass = false; 

	//last minute fix so I am making it safe
	if(GameFlag.test(fDoneSoundAnal))
	{
		if (GameFlag.test(fDCK10))
		{
			intrpuzz->setScript(new RedrawScript); 
		}
		else if (!(webreport->flag & OBJECT_HIDDEN))
			WebReport::cue();

		return;
	}

	if(choiceIndex[0] == 3 && choiceIndex[1] == 11)	
	{
		if(!GameFlag.test(fDoneSoundAnal))
		{
			new VLink( "Good catch: Foster, Maxine M.", "35101.avi", 0, 0 );  //vlink674, direct agents after BS
			GameFlag.set(fDoneSoundAnal);
		}
	}
	else if(GameFlag.test(fGreg21) && GameFlag.test(fGreg22))
	{
		fireTheJackass = true;
	}
	else if(!mapinfo->church || !mapinfo->car || !mapinfo->bird)
	{
		new EMail("Got all the clues?: Foster, Maxine M.", 98388);
		if(GameFlag.test(fGreg21))
		{
			GameFlag.set(fGreg22);
		}
		else
			GameFlag.set(fGreg21);
	}
	else if(choiceIndex[1] != 17)
	{
		if(GameFlag.test(fGreg21))
		{
			GameFlag.set(fGreg22);
		}
		else
			GameFlag.set(fGreg21);

		if(choiceIndex[0] == 0)
		{	
			if(choiceIndex[1] == 9 || choiceIndex[1] == 13 || choiceIndex[1] == 16 || choiceIndex[1] == 17)	
			{
				new EMail("What about the bird?: Foster, Maxine M.", 98390);						
			}
			else
				new EMail("Don't waste my time?: Foster, Maxine M.", 98381);						
		}
		else if(choiceIndex[0] == 1)
		{	
			if(choiceIndex[1] == 2 || choiceIndex[1] == 5 || choiceIndex[1] == 12)	
			{
				new EMail("What about the bird?: Foster, Maxine M.", 98390);						
			}
			else
				new EMail("Don't waste my time?: Foster, Maxine M.", 98382);						
		}
		else if(choiceIndex[0] == 2)
		{	
			if(choiceIndex[1] == 10 || choiceIndex[1] == 14 || choiceIndex[1] == 15)	
			{
				new EMail("What about the bird?: Foster, Maxine M.", 98390);						
			}
			else
				new EMail("Don't waste my time?: Foster, Maxine M.", 98383);						
		}
		else if(choiceIndex[0] == 3)
		{	
			if(choiceIndex[1] == 6 || choiceIndex[1] == 8 || choiceIndex[1] == 11)	
			{
				new EMail("What about the bird?: Foster, Maxine M.", 98390);						
			}
			else
				new EMail("Don't waste my time?: Foster, Maxine M.", 98383);						
		}
		else if(choiceIndex[0] == 4)
		{	
			if(choiceIndex[1] == 1 || choiceIndex[1] == 3)	
			{
				new EMail("Are you sure?: Foster, Maxine M.", 98391);						
			}
			else
				new EMail("Don't waste my time?: Foster, Maxine M.", 98385);						
		}
		else if(choiceIndex[0] == 5)
		{	
			if(choiceIndex[1] == 7)	
			{
				new EMail("What about the Tram?: Foster, Maxine M.", 98389);						
			}
			else
				new EMail("Don't waste my time?: Foster, Maxine M.", 98385);						
		}
		else if(choiceIndex[0] == 6)
		{	
			if(choiceIndex[1] == 0 || choiceIndex[1] == 4)	
			{
				new EMail("What about the Tram?: Foster, Maxine M.", 98389);						
			}
			else
				new EMail("Don't waste my time?: Foster, Maxine M.", 98387);						
		}
	}					  

	if (GameFlag.test(fDCK10))
	{
		intrpuzz->setScript(new RedrawScript); 
	}
	else if (!(webreport->flag & OBJECT_HIDDEN))
		WebReport::cue();

	if(fireTheJackass)
	{
		comlinkshort = NULL;
		intrpuzz->quitIntrface();
		if(theInset)
			theInset->dispose();
		theGame->newRoom(new Rm60000(END_FIRED));
	}
}
/***********************************
 *		
 *		 Puzzle Save Status
 *		      
 **********************************/
MapInfo::MapInfo()
{
	name = "mapinfo";
	jet = false;
	car = false;
	bird = false;
	church = false;
	arm = false;
	seenDuchanbe = false;
	seenErevan = false;
	seenPeters = false;
	seenNov = false;
	seenMoscow = false;
	seenRostov = false;
	seenIrkutsk = false;
	web_intrface->addToFront(this); 
	mapinfo = this;
}

void MapInfo::save()
{
}	  

void MapInfo::load()
{
}

void MapInfo :: dispose()
{
	web_intrface->release ( this );
	deathList->add(this);
	mapinfo = NULL;
}

} // namespace Spycraft
