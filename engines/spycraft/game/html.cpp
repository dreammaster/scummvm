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

#include "spycraft/game/html.h"
#include "spycraft/game/htm.h"
#include "spycraft/game/email.h"
#include "spycraft/game/vlink.h"
#include "spycraft/dmade/advres.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advmusic.h"

namespace Spycraft {

Sound *typeSFX;
extern void GetCDROM(char *);

HTMLBack * htmlback;
HTMLAnnounce* speak= NULL;
int LeftMargin;
int TopMargin;
char commandText[256];
int temp_flag = 0;
int temp_x = 0;
int temp_y = 0;
int temp_width = 0;
int temp_speed = 0;					    
int tmpPage = 0;
int noPlay = false;
int noc = false;
//EMailSend	*emailsend;

HyperCursor* hyperCursor = new HyperCursor;

class SoundDelay : public Script
{
public:
	SoundDelay(){name = "sounddelay";};
	void changeState ( int );
};
void SoundDelay::changeState(int newState)
{
	switchTo
		noPlay = true;
		seconds = 1;
	END
	BEG
		noPlay = false;
	END
}
   
enum 
{
	TEXT_TOKEN,
	HYPER_LINK,													   										    
	HYPER_FONT,
	HYPER_BOLD,
	HYPER_BLINK,
	HYPER_ITALIC,
	HYPER_IMG,
	HYPER_TITLE,
	HYPER_HEADER,
	HYPER_HEADER2,
	HYPER_HEADER3,
	HYPER_HEADER4,
	HYPER_HEADER5,
	HYPER_HEADER6,
	HYPER_NEW_LINE,
	HYPER_BKG,
	HYPER_BUTT,
	HYPER_CENTER,
	HYPER_BOLD_ITALIC,
	HYPER_SKIP,
	HYPER_LINK_OLD,
	HYPER_SCROLLER
};

HTMLFile* currentHFile = NULL;

void changePause(void)
{
	HTMLMarker* newobj;
	intrpuzz->dispose();
	theGame->handsOn();
	new HTMLPage(tmpPage);
	newobj = new (HTMLMarker);
	newobj->flag = tmpPage;
}

HTMLPage::HTMLPage()
{
	name = "HTMLPage";
	currentFocus = PUZZLE_FOCUS;
	currentHFile = new(HTMLFile);
	press = new Sound;
	if (noPlay)
		noPlay = false;
}

HTMLPage::HTMLPage(char* pagename)
{
	name = "HTMLPage";
	currentFocus = PUZZLE_FOCUS;
	currentHFile = new(HTMLFile);
	press = new Sound;
	init(pagename);
	if (noPlay)
		noPlay = false;
}

HTMLPage::HTMLPage(int pagenumber)
{

	char theName[256];
	memset (&theName,0,256);
	name = "HTMLPage";
	press = new Sound;
	currentFocus = PUZZLE_FOCUS;
	currentHFile = new(HTMLFile);

	Common::sprintf_s(theName, "%d", pagenumber);
	init(theName);
	if (noPlay)
		noPlay = false;
}

void HTMLPage::init(char * pagename)
{
	int id;
	unsigned char * htmpage;
	currentFocus = PUZZLE_FOCUS;
  	LeftMargin = 3;
	TopMargin = 63;


	htmlback = NULL;
/*
	if (GameFlag.test(fYellow))
	{
		htmlback = NULL;
	}
	else
		htmlback = new HTMLBack(988);
*/
	currentHFile->newFile();
	IntrPuzzPage::init(pagename);

	theGame->handsOn();

//	currentHFile->open(pagename, FILE_READ);
	id = atoi(pagename);
	htmpage = (unsigned char*)LoadHTM(id);
	currentHFile->filesize = GetHTMSize();
	memcpy(&currentHFile->pagecopy,htmpage,currentHFile->filesize-1);	
	while (currentHFile->bytesRead <= currentHFile->filesize-1)
		currentHFile->readToken();

	UnLoadHTM();
	sfxPurgeRes(id,RES_HTM);
}										    

int HTMLPage::handleEvent( MADEEventStamp* event)
{
	if (currentHFile) 
		return currentHFile->hyperList->handleEvent(event);
	else
		return false;
}

void HTMLPage::dispose()
{
	if (currentHFile)
	{
		currentHFile->dispose();
		delete currentHFile;
		currentHFile = NULL;
   	}
	if (press)
		press->dispose();
	IntrPuzzPage::dispose();
}

HTMLBack::HTMLBack(int num)
{
	name = "backgrnd";
	view = num;
	loop = 0;
	cel = 0;
	init(0,15);
	noSkip();
	addToPic();
	dispose();
}

HTMLButt::HTMLButt(int num, int hypX, int hypY, int skip)
{
	name = "hyperbutton";
	view = num;
	loop = 0;
	cel = 0;
	init(hypX,hypY);
	if (!skip)
	{
		noSkip();
	}
	addToPic();
	dispose();
}

HTMLAnim::HTMLAnim(int num, int hypX, int hypY, int skip)
{
	name = "hyperAnim";
	view = num;
	loop = 0;
	cel = 0;
	cycleSpeed = 4;	

	init(hypX,hypY);
	if (!skip)
	{
		noSkip();
	}
	setCycle(new EndLoop((Prop*)this, this));
}

void HTMLAnim::cue(void)
{
	setCycle(new EndLoop((Prop*)this, this));
}

HTMLFile::HTMLFile()
{
	name = "HTMLFile";
	theY = TopMargin;
	theX = LeftMargin;
	temp_flag = 0;
 	temp_x = 0;
 	temp_y = 0;
 	temp_width = 0;
 	temp_speed = 0;					    
	typing  = false;
	bytesRead = 0;
	memset(pagecopy,0,4096);
	hyperList = new FeatureList;
	myPrints = new List;
}

void HTMLFile::newFile(void)
{
	bytesRead = 0;
	memset(pagecopy,0,4096);
	currentAction = TEXT_TOKEN;
	theX = LeftMargin;
	theY = TopMargin;
	theColumn = 0;
	theRow = 0;
	newRow = 0;
	currentAction = 0;
	rowHeight = 16;
}	

void HTMLFile::dispose()
{
	int a; 

	hyperList->dispose();

	if (myPrints->size)
	{
		for (a=0; a < myPrints->size; a++)
		{	
			if (intrpuzz->page->site)
			{
				if (/*intrpuzz->page->site->flag == WEB_VLINKS && */!(GameFlag.test(fPatsHTM_130)))
					((Print*)myPrints->at(a))->erase();
				else
					((Print*)myPrints->at(a))->addToPic();
			}
			else
				((Print*)myPrints->at(a))->erase();
		}
		myPrints->dispose();
	}													 

//	currentHFile->close();
	delete hyperList;
	hyperList = NULL;
	delete myPrints;
	myPrints = NULL;
}
void HTMLFile::readByteVols(unsigned char * theChar)
{
	memcpy(theChar,&pagecopy[bytesRead],1);
}													    
void HTMLFile::typeToken()
{
	unsigned char theChar;
	unsigned char currentToken[MAX_LENGTH];
	int i = 0;

	readByteVols(&theChar);
	++bytesRead;						   

	if ( theChar == '<')	
	{
		readByteVols(&theChar);
		++bytesRead;
		while ((theChar != '>') && (bytesRead <= filesize))	
		{
			currentToken[i++] =  theChar;
			readByteVols(&theChar);
			++bytesRead;
		}

		currentToken[i] = '\0';
		translateToken(currentToken);
	}
	else
	{
		while ((theChar != '\r') &&  (theChar != '<') && (theChar != '\n') && (bytesRead <= filesize) && (theColumn <= MAX_COLUMNS))	
		{
			currentToken[i++] =  theChar;
			++ theColumn;
			readByteVols(&theChar);
			++bytesRead;
		}

		if (theColumn > MAX_COLUMNS)	
		{
			--bytesRead;
//			seek(-1, FILE_SEEK_CUR);
			newRow = true;
		}

		if (theChar == '<')
		{
			--bytesRead;
//			seek(-1, FILE_SEEK_CUR);
		}
	
		currentToken[i] = '\0';

	 	currentLetter = 0;
	 	typeX = theX;
		typeY = theY;
		typeList = new(List);
		*typeText = *currentToken;

		typing = true;
		new(Secretary);
	}
}

void HTMLFile::cue()
{
}

Secretary::Secretary()
{
 	typeSFX = new(Sound);
	typeSFX->loop = 20;
	if(!GameFlag.test(fDCK10))
	{
		typeSFX->play(132, NULL);
	}

 	TypeTimer = new (IntrPuzzTimer);
	TypeTimer->setTicks(2,this);
}	

void Secretary::cue()
{
	if(currentHFile->typeText[currentHFile->currentLetter] != '\0')
	{		
		char tempChar[2];
		TypePrint* temp;

		strncpy((char*)tempChar, (char *)&currentHFile->typeText[currentHFile->currentLetter++], 1);
		tempChar[1] = '\0';
		temp = new TypePrint(currentHFile->typeX, currentHFile->typeY, (unsigned char *)&tempChar);
		currentHFile->typeX += temp->width;
		TypeTimer = new(IntrPuzzTimer);
		TypeTimer->setTicks(2, this);
	}
	else
	{
		if (currentHFile->typeList->size)
		{
			for (int a=0; a < currentHFile->typeList->size; a++)
			{	
				((Print*)currentHFile->typeList->at(a))->erase();
			}
			currentHFile->typeList->dispose();
		}													 
		delete currentHFile->typeList;
		currentHFile->typeList = NULL;
		currentHFile->displayToken(currentHFile->typeText);

		typeSFX->stop();
		typeSFX->dispose();
		currentHFile->typing = false;

		while ((currentHFile->bytesRead <= currentHFile->filesize) && !emailsend->decision && !currentHFile->typing)
			currentHFile->typeToken();

		delete this;
	}
}

TypePrint::TypePrint(int theX, int theY, unsigned char* theToken)
{
	name = "TypePrint";
	priority = 720; 
	fore_color = TEXT_COLOR_BLACK;
	size = 18;
	style = STYL_PLAIN;

	x = theX;
	y = theY;

	Print::init((char*)theToken);
	currentHFile->typeList->add(this);												   
}

void HTMLFile::readToken()
{
	unsigned char theChar;
	unsigned char currentToken[MAX_LENGTH];
	unsigned char ATSNum[32];
	unsigned char tempChar;
	int j,value;

	int i = 0;

 	memset (&ATSNum,0,32*sizeof(char));

	readByteVols(&theChar);
	++bytesRead;						   

	if ( theChar == '<')	
	{
		readByteVols(&theChar);
		++bytesRead;
		while ((theChar != '>') && (bytesRead <= filesize))	
		{
			currentToken[i++] =  theChar;
			readByteVols(&theChar);
			++bytesRead;
		}

		currentToken[i] = '\0';
		translateToken(currentToken);
	}
	else
	{
		while ((theChar != '\r') &&  (theChar != '<') && (theChar != '\n') && (bytesRead <= filesize) && (theColumn <= MAX_COLUMNS))	
		{
			currentToken[i++] =  theChar;
			++ theColumn;
			readByteVols(&theChar);
			++bytesRead;
		}

		if (theColumn > MAX_COLUMNS)	
		{
			--bytesRead;
//			seek(-1, FILE_SEEK_CUR);
			newRow = true;
		}

/*
 		if (theChar == '\r') 
		{
			newRow = true;
		}
*/
		if (theChar == '<')
		{
			--bytesRead;
//			seek(-1, FILE_SEEK_CUR);
		}
	
		currentToken[i] = '\0';

		if (currentAction == HYPER_TITLE)
		{
	       	i = 0;
	       	j = 0;
	       	tempChar = currentToken[i];
	        while (tempChar != '.')	{
				ATSNum[j++] = tempChar;
	        	tempChar = currentToken[++i];
			}
			ATSNum[j] = '\0';	
			value = atoi((char*)&ATSNum);

			if (htmlback)
			{
				intrpuzz->release(htmlback);
				deathList->add(htmlback);
			}
			htmlback = new HTMLBack(value);
		}
		else
		{
//		if (i)
			displayToken(currentToken);
		}

	}
}

void HTMLFile::translateToken(unsigned char* curToken)
{
	unsigned char linkFile[256];
	unsigned char ATSNum[32];
	unsigned char convert[32];
	unsigned char tempChar;
	char *subToken; 
	Website * prev;
	HTMLButt * 	button = nullptr;
	HTMLAnim * 	anim;
	HyperFeature* tmpFeat;
	int i, j,value = 0,len;
	int tflag = 0;
	int delay = 0;
	int counter = 0;
	int playit = true;
	int skip = false;							    
	noc = false;
	AnnounceMarker *newobj;
	Node* n;

	if (!strncmp((char*) curToken, "*", 1))        
	{
		return;
	}
	if (!strncmp((char*) curToken, "/", 1))        
	{
		currentAction = TEXT_TOKEN;
		return;
	}
	if (currentAction == HYPER_SKIP)
		return;

	if (!strstr((char*)curToken,"IMG"))
		memset (&commandText,0,256*sizeof(char));

	if (strstr((char*)curToken,"NO_COLOR"))
		noc = true;
	memset (&linkFile,0,256*sizeof(char));
	memset (&ATSNum,0,32*sizeof(char));
	memset (&convert,0,32*sizeof(char));

	subToken = strstr((char*)curToken,"COMMAND=");

 	if (!subToken)
		subToken = strstr((char*)curToken,"command=");

	if (subToken)
	{
		subToken  += (9 * sizeof(char));
        while (subToken[counter] != '\"')	
        {
			commandText[counter] = subToken[counter];
			counter ++;
		}
		commandText[counter] = '\0';
		counter = 0;
		subToken = NULL;
	}

	if ((!strncmp((char*) curToken, "CHOICES=",8)) || (!strncmp((char*) curToken, "IFCHOICES=",10)))	
	{
		//seperator = *
		bool conditional;
		int index = 0;
		char choiceList[40][80];
	 	memset (&choiceList,0,(40 * 80)*sizeof(char));
		
		if(curToken[0] == 'C')	//nonconditional choices
		{
	       	i = 9;
			conditional = false;
		}
		else	   				//conditional choices
		{
	       	i = 11;
			conditional = true;
		}
			
       	j = 0;
       	tempChar = curToken[i];
		do{		
	        while (tempChar != '*')
	        {
				if((tempChar != '\r') && (tempChar != '\n'))
					choiceList[index][j++] = tempChar;

	        	tempChar = curToken[++i];
			}
			index++;
       		tempChar = curToken[++i];
			j = 0;
							
		}while(tempChar != '\"');

		if(emailsend)
			emailsend->showChoices(choiceList, index, conditional);
		else
			sfxPrintf("you do have an Email send page up");

		linkFile[j] = '\0';	
		return;
	}

	sc_strupr((char*)curToken);

	counter = 0;
	memset (&convert,0,(5*sizeof(char)));

	subToken = strstr((char*)curToken,"X=");

	if (subToken)
	{
		subToken  += (2 * sizeof(char));
		len = strlen(subToken);
        while ((subToken[counter] != ' ') && (counter < len))	
        {
			convert[counter] = subToken[counter];
			counter ++;
		}
		convert[counter] = '\0';
		temp_x = atoi((char*)convert);
	}

	counter = 0;
	memset (&convert,0,(5*sizeof(char)));

	subToken = strstr((char*)curToken,"Y=");
 
	if (subToken)
	{
		subToken  += (2 * sizeof(char));
		len = strlen(subToken);
        while ((subToken[counter] != ' ') && (counter < len))	
        {
			convert[counter] = subToken[counter];
			counter ++;
		}
		convert[counter] = '\0';
		temp_y = atoi((char*)convert);
	} 
	counter = 0;
	memset (&convert,0,(5*sizeof(char)));

	subToken = strstr((char*)curToken,"MARGIN=");

	if (subToken)
	{
		subToken  += (7 * sizeof(char));
		len = strlen(subToken);
        while ((subToken[counter] != ' ') && (counter < len))	
        {
			convert[counter] = subToken[counter];
			counter ++;
		}
		convert[counter] = '\0';
		LeftMargin = atoi((char*)convert);
		currentHFile->theX = LeftMargin;
	}
	counter = 0;
	memset (&convert,0,(5*sizeof(char)));

	subToken = strstr((char*)curToken,"WIDTH=");

	if (subToken)
	{
		subToken  += (6 * sizeof(char));
		len = strlen(subToken);
        while ((subToken[counter] != ' ') && (counter < len))	
        {
			convert[counter] = subToken[counter];
			counter ++;
		}
		convert[counter] = '\0';
		temp_width = atoi((char*)convert);
	}
	
	counter = 0;
	memset (&convert,0,(5*sizeof(char)));

	subToken = strstr((char*)curToken,"SPEED=");

	if (subToken)
	{
		subToken  += (6 * sizeof(char));
		len = strlen(subToken);
        while ((subToken[counter] != ' ') && (counter < len))	
        {
			convert[counter] = subToken[counter];
			counter ++;
		}
		convert[counter] = '\0';
		temp_speed = atoi((char*)convert);
	}

	subToken = strstr((char*)curToken,"FLAG=");

	if (subToken)
	{
		subToken  += (5 * sizeof(char));
		len = strlen(subToken);

        while ((subToken[counter] != ' ') && (counter < len))	
        {
			convert[counter] = subToken[counter];
			counter ++;
		}
		convert[counter] = '\0';
		tflag = atoi((char*)convert);
		temp_flag = tflag;
	} 

	subToken = strstr((char*)curToken,"SKIP");
	if (subToken)
	{
		skip = true;
	}
	if (!strncmp((char*) curToken, "A HREF=", 7))        
	{
       	i = 8;
       	j = 0;								//file name starts on 8th char
       	tempChar = curToken[i];
        while (tempChar != '.')	{
			linkFile[j++] = tempChar;
        	tempChar = curToken[++i];
		}
		linkFile[j] = '\0';	
		
        tempChar = curToken[++i];
//		tempChar = toupper(tempChar);
		
		value = atoi((char*)linkFile);

		switch (tempChar)
		{
			case 'H':	
				Common::strcat_s(linkFile, ".HTM");
				new HyperFeature(linkFile);

				n = usedList->head;

				while ( n )
				{
					if ( (int)n->data->flag == value)	
					{
						playit = false;
						break;
					}
					n = n->next;
				}
				break;

			case 'W':	
				Common::strcat_s(linkFile, ".WAV");
				new HyperFeature(linkFile);
				break;

			case 'S':	
				Common::strcat_s(linkFile, ".SWAV");
				new HyperFeature(linkFile);
				break;
		}

		if (playit)
			currentAction = HYPER_LINK;
		else
			currentAction = HYPER_LINK_OLD;
		return;
	}

	if (!strncmp((char*) curToken, "FSET=", 5))        
	{
       	i = 5;
       	j = 0;								

		len = strlen((char*)curToken);
       	tempChar = curToken[i];

        while (tempChar != ' ' && i < len)	
        {
			linkFile[j++] = tempChar;
        	tempChar = curToken[++i];
		}

		linkFile[j] = '\0';	
		value = atoi((char*)linkFile);

		GameFlag.set(value);
		 				
		return;
	}
	if (!strncmp((char*) curToken, "SCROLL", 6))        
	{
		currentAction = HYPER_SCROLLER;
		return;
	}

	if (!strncmp((char*) curToken, "FCLR=", 5))        
	{
       	i = 5;
       	j = 0;								

		len = strlen((char*)curToken);
       	tempChar = curToken[i];

        while (tempChar != ' ' && i < len)	
        {
			linkFile[j++] = tempChar;
        	tempChar = curToken[++i];
		}

		linkFile[j] = '\0';	
		value = atoi((char*)linkFile);

		GameFlag.clear(value);
		 				
		return;
	}

	if (!strncmp((char*) curToken, "HELP=", 5))        
	{
       	i = 5;
       	j = 0;								

		len = strlen((char*)curToken);
       	tempChar = curToken[i];

        while (tempChar != ' ' && i < len)	
        {
			linkFile[j++] = tempChar;
        	tempChar = curToken[++i];
		}

		linkFile[j] = '\0';	
		helppage = atoi((char*)linkFile);

		return;
	}

	if (!strncmp((char*) curToken, "REPORT=", 7))        
	{
       	i = 7;
       	j = 0;								

		len = strlen((char*)curToken);
       	tempChar = curToken[i];

        while (tempChar != ' ' && i < len)	
        {
			linkFile[j++] = tempChar;
        	tempChar = curToken[++i];
		}

		linkFile[j] = '\0';	
		lasttool = atoi((char*)linkFile);

		return;
	}

	if (!strncmp((char*) curToken, "A CHK=", 6))        
	{
       	i = 6;
       	j = 0;								

       	tempChar = curToken[i];

        while (tempChar != ' ')	
        {
			linkFile[j++] = tempChar;
        	tempChar = curToken[++i];
		}

		linkFile[j] = '\0';	
		value = atoi((char*)linkFile);
		counter = 0;
		if(GameFlag.test(value))
		{
			subToken = strstr((char*)curToken,"true=");

			if (subToken)
			{
				subToken += (6 * sizeof(char));
		     
		    	if (subToken[counter] == 'N')
				{
					currentAction = TEXT_TOKEN;
					return;
				}

		        if (subToken[counter] == 'H')
				{
			        while (subToken[counter] != '\"')	
			        {
						linkFile[counter] = subToken[counter];
						counter ++;
					}
					linkFile[counter] = '\0';
					new HyperFeature(linkFile);
				}
				else
				{
			        while (subToken[counter] != '.')	
			        {
						linkFile[counter] = subToken[counter];
						counter ++;
					}
					linkFile[counter] = '\0';
					counter++;
					tempChar = 	subToken[counter];
	 				counter = 0;

					switch (tempChar)
					{
						case 'H':	
							value = atoi((char*)linkFile);
							Common::strcat_s(linkFile, ".HTM");
							new HyperFeature(linkFile);

							n = usedList->head;

							while ( n )
							{
								if ( (int)n->data->flag == value)	
								{
									playit = false;
									break;
								}
								n = n->next;
							}
							break;

						case 'W':	
							Common::strcat_s(linkFile, ".WAV");
							new HyperFeature(linkFile);
							break;

						case 'S':	
							Common::strcat_s(linkFile, ".SWAV");
							new HyperFeature(linkFile);
							break;
					}
				}
				if (playit)
					currentAction = HYPER_LINK;
				else
					currentAction = HYPER_LINK_OLD;
			}
			else
			{
				currentAction = HYPER_SKIP;
			} 
		}
		else
		{
			subToken = strstr((char*)curToken,"false=");

			if (subToken)
			{
				subToken  += (7 * sizeof(char));
		    	if (subToken[counter] == 'N')
				{
					currentAction = TEXT_TOKEN;
					return;
				}
		    
		        while (subToken[counter] != '.')	
		        {
					linkFile[counter] = subToken[counter];
					counter ++;
				}
				linkFile[counter] = '\0';
				counter++;
				tempChar = 	subToken[counter];
//				tempChar = toupper(tempChar);
 				counter = 0;

				switch (tempChar)
				{
					case 'H':	
						value = atoi((char*)linkFile);
						Common::strcat_s(linkFile, ".HTM");
						new HyperFeature(linkFile);
						n = usedList->head;

						while ( n )
						{
							if ( (int)n->data->flag == value)	
							{
								playit = false;
								break;
							}
							n = n->next;
						}
						break;

					case 'W':	
						Common::strcat_s(linkFile, ".WAV");
						new HyperFeature(linkFile);
						break;

					case 'S':	
						Common::strcat_s(linkFile, ".SWAV");
						new HyperFeature(linkFile);
						break;
				}
				if (playit)
					currentAction = HYPER_LINK;
				else
					currentAction = HYPER_LINK_OLD;
			}
			else
			{
				currentAction = HYPER_SKIP;
			} 
		
		}
		return;
	}

	if (!strcmp((char*) curToken, "B"))
	{
		if (currentAction == HYPER_ITALIC)
		{
			currentAction = HYPER_BOLD_ITALIC;
		}
		else
		{
			currentAction = HYPER_BOLD;
		}
		return;
	}

	if (!strcmp((char*) curToken, "I"))	
	{
		if (currentAction == HYPER_BOLD)
		{
			currentAction = HYPER_BOLD_ITALIC;
		}
		else
		{
			currentAction = HYPER_ITALIC;
		}
		return;
	}

	if (!strcmp((char*) curToken, "BR"))	
	{    
		currentAction = HYPER_NEW_LINE;
		return;
	}

	if (!strcmp((char*) curToken, "HR"))	
	{    
		if (currentHFile->theX != LeftMargin)
		{
			currentHFile->theX	= LeftMargin;
			currentHFile->theY += 16;
			++currentHFile->theRow;
			currentHFile->newRow = 0;
		}

		button = new HTMLButt(990,currentHFile->theX,currentHFile->theY,true);

		currentHFile->theY = button->bottom + 4;		
		currentHFile->theX = LeftMargin;		
		currentHFile->theColumn = 0;
		++currentHFile->theRow;
		currentHFile->newRow = 0;
		currentAction = TEXT_TOKEN;
		return;
	}
	
	if (!strcmp((char*) curToken, "TITLE"))	
	{
		currentAction = HYPER_TITLE;
		return;
	} 

	if (!strcmp((char*) curToken, "CENTER"))	
	{
		currentAction = HYPER_CENTER;
		return;
	} 
	
	if (!strcmp((char*) curToken, "H1"))
	{
		currentAction = HYPER_HEADER;
		return;
	}

	if (!strcmp((char*) curToken, "H2"))
	{
		currentAction = HYPER_HEADER2;
		return;
	}

	if (!strcmp((char*) curToken, "H3"))
	{
		currentAction = HYPER_HEADER3;
		return;
	}

	if (!strcmp((char*) curToken, "H4"))
	{
		currentAction = HYPER_HEADER4;
		return;
	}

	if (!strcmp((char*) curToken, "H5"))
	{
		currentAction = HYPER_HEADER5;
		return;
	}

	if (!strcmp((char*) curToken, "H6"))
	{
		currentAction = HYPER_HEADER6;
		return;
	}
	
	if (!strncmp((char*) curToken, "WAIT=",5))	
	{
       	i = 5;
       	j = 0;
       	tempChar = curToken[i];
        while (tempChar != ' ')	
        {
			ATSNum[j++] = tempChar;
        	tempChar = curToken[++i];
		}
		ATSNum[j] = '\0';	

		delay = atoi((char*)&ATSNum);
		
		counter = 0;
		memset (&convert,0,(5*sizeof(char)));

		subToken = strstr((char*)curToken,"PAGE=");

		if (subToken)
		{
			subToken  += (5 * sizeof(char));
			len = strlen(subToken);
	        while ((subToken[counter] != ' ') && (counter < len))	
	        {
				convert[counter] = subToken[counter];
				counter ++;
			}
			convert[counter] = '\0';
			value = atoi((char*)convert);
		}
		prev = webmap->findPrevPage();
		if (prev)
		{
			webmap->dumpAfter();
		}

		new HTMLPause(delay,value);
		return;
	}
	if (!strncmp((char*) curToken, "IMG SRC=",8))	
	{
       	i = 9;
       	j = 0;
       	tempChar = curToken[i];
        while (tempChar != '.')	
        {
			ATSNum[j++] = tempChar;
        	tempChar = curToken[++i];
		}
		ATSNum[j] = '\0';	

		value = atoi((char*)&ATSNum);
		
		if (curToken[++i] == 'A')
		{
			if (strstr((char*)curToken,"LOOP"))
			{
				if (temp_x)
					anim = new HTMLAnim(value, temp_x,temp_y,skip);
				else
					anim = new HTMLAnim(value, currentHFile->theX,currentHFile->theY,skip);

				if (currentAction == HYPER_LINK || currentAction == HYPER_LINK_OLD)
				{
					tmpFeat = (HyperFeature*) currentHFile->hyperList->at(0);	

					tmpFeat->left = anim->left;
					tmpFeat->top = anim->top;
					tmpFeat->right = anim->right;
					tmpFeat->bottom = anim->bottom;

					if (commandText[0])
					{	
						new CommandLine(anim->left,anim->top,(anim->right - anim->left),(anim->bottom - anim->top),
										(char *)&commandText);
			//			memset (&commandText,0,256*sizeof(char));
					}		
				}
			}
			else
			{	
				if (temp_x)
					button = new HTMLButt(value, temp_x,temp_y,skip);
				else
					button = new HTMLButt(value, currentHFile->theX,currentHFile->theY,skip);

				if (currentAction == HYPER_LINK || currentAction == HYPER_LINK_OLD)
				{
					tmpFeat = (HyperFeature*) currentHFile->hyperList->at(0);	

					tmpFeat->left = button->left;
					tmpFeat->top = button->top;
					tmpFeat->right = button->right;
					tmpFeat->bottom = button->bottom;

					if (commandText[0])
					{	
						new CommandLine(button->left,button->top,(button->right - button->left),
										(button->bottom - button->top),(char *)&commandText);
			//			memset (&commandText,0,256*sizeof(char));
					}		
				}
			}

			if (!temp_x)
			{
				currentHFile->theY = button->bottom + 4;		
				currentHFile->theX = LeftMargin;		
				currentHFile->theColumn = 0;
				++currentHFile->theRow;
				currentHFile->newRow = 0;
			}	
			currentAction = TEXT_TOKEN;
			temp_x = 0;
			temp_y = 0;
		}
		else
		{
			n = announceList->head;

			while ( n )
			{
				if ( (int)n->data->flag == value)	
				{
					playit = false;
					break;
				}
				n = n->next;
			}

			if (playit)
			{
				if (curToken[++i] == 'S')
					speak = new HTMLAnnounce(value,true);
				else
					speak = new HTMLAnnounce(value,false);

				subToken = strstr((char*)curToken,"REPEAT");
				if (!subToken)
				{
					newobj = new (AnnounceMarker);
					newobj->flag = value;
				}
			}
		}		
		return;
	}

	if (!strncmp((char*) curToken, "MAIL=",5))	
	{
       	i = 5;
       	j = 0;
       	tempChar = curToken[i];
        while (tempChar != ' ')	
        {
			ATSNum[j++] = tempChar;
        	tempChar = curToken[++i];
		}
		ATSNum[j] = '\0';	

		value = atoi((char*)&ATSNum);
		
		subToken = strstr((char*)curToken,"SUB=");

		if (subToken)
		{
			counter = 0;
			memset(&convert,0,32);
			subToken  += (5 * sizeof(char));
	        while (subToken[counter] != '\"')	
	        {
				convert[counter] = subToken[counter];
				counter ++;
			}
			convert[counter] = '\0';
//			new EMail((char*)convert,value);
			new EMail((char*)convert,value,tflag,delay);
		} 
	
		currentAction = TEXT_TOKEN;
		return;
	}

	if (!strncmp((char*) curToken, "VLINK=",6))	
	{
       	i = 7;
       	j = 0;
       	tempChar = curToken[i];
        while (tempChar != '\"')	{
			linkFile[j++] = tempChar;
        	tempChar = curToken[++i];
		}
		linkFile[j] = '\0';	

		subToken = strstr((char*)curToken,"DELAY=");

		if (subToken)
		{
			subToken  += (6 * sizeof(char));
			len = strlen(subToken);

	        while ((subToken[counter] != ' ') && (counter < len))	
	        {
				convert[counter] = subToken[counter];
				counter ++;
			}
			convert[counter] = '\0';
			delay = atoi((char*)convert);
		} 

		subToken = strstr((char*)curToken,"SUB=");

		if (subToken)
		{
			counter = 0;
			memset(&convert,0,32);
			subToken  += (5 * sizeof(char));
	        while (subToken[counter] != '\"')	
	        {
				convert[counter] = subToken[counter];
				counter ++;
			}
			convert[counter] = '\0';
		} 

		new VLink((char*)convert,(char*)linkFile,tflag,delay);
		temp_flag = 0;
		currentAction = TEXT_TOKEN;
		return;
	}														 
}

void HTMLFile::displayToken(unsigned char* curToken)
{
	if (currentHFile->currentAction == HYPER_NEW_LINE)
	{
		++currentHFile->theRow;

		if (currentHFile->theX == LeftMargin)
			currentHFile->theY += 6;
		else	
			currentHFile->theY += 16;

		currentHFile->theX = LeftMargin;
		currentHFile->theColumn = 0;
		currentHFile->currentAction = TEXT_TOKEN;
		return;
	}
	else if (currentHFile->currentAction == HYPER_SKIP)	
	{
		return;
	}
	else if (currentHFile->currentAction == HYPER_SCROLLER)
	{
		if (strlen((char*)curToken))
			new Scroller(curToken);
	}
	else
	{
		if (strlen((char*)curToken))
			new HyperPrint(curToken);
	}
}	
       	
HyperFeature::HyperFeature(unsigned char* theLinkFile)
{
	//int i;
	if (temp_flag)
		setFlag = temp_flag;
	else
		setFlag = 0;
	temp_flag = 0;
	memset(&linkTo,0,128);
	Common::strcat_s(linkTo,(char*)theLinkFile);
	name = "HyperFeature";
	currentHFile->hyperList->addToFront(this);
	setHotspot(ALL_V);
}

void HyperFeature::dispose(void)
{
	if ( hotspot )	{
		setHotspot ( NULL );
		hotspot = NULL;
	}
	currentHFile->hyperList->release ( this );
	deathList->add ( this );
}
void HyperFeature::respond()
{
	theGame->setCursor(hyperCursor);
}

int HyperFeature::doVerb( int theVerb )
{
   	int i = 0;
   	int j = 0;
	int value = 0;
   	unsigned char tempChar = linkTo[i];
	unsigned char WAVNum[32];
	char drive[8];
	char linksite[256];
	memset(&linksite,0,256);
	memset(&drive,0,8);
//	Common::strcat_s(linksite,"http://spycraft.activision.com/forward.cgi?site=");
	HTMLMarker* newobj;
	if (setFlag)
		GameFlag.set(setFlag);

	if (tempChar == 'H')
	{
		sfxDisableSound();
		sfxDisableMusic();
		sfxSystemCursor(SYS_ARROW);

		Common::strcat_s(linksite,"http://spycraft.activision.com/forward.cgi?site=");
		sc_strlwr((char*)linkTo);
		Common::strcat_s(linksite,(char*)linkTo);

		Common::strcat_s(linksite,"&cd=");
		GetCDROM((char*)&drive);			 
		Common::strcat_s(linksite,(char*)drive);
		theGame->startNet((char*)linksite);

		sfxMadeCursor();
		sfxEnableSound();
		sfxEnableMusic();
		flushMessageCue();
		sc_strupr((char*)linkTo);

		return true;
	}
	memset (&WAVNum,0,32*sizeof(char));
    while (tempChar != '.')	
    {
		WAVNum[j++] = tempChar;
    	tempChar = linkTo[++i];
	}
	WAVNum[j] = '\0';	

	value = atoi((char*)&WAVNum);
//	strupr((char*)linkTo);
	if (linkTo[++i] == 'H')
	{
		if (!strncmp((char*) linkTo, "PUZZ", 4))
		{
			Common::strcat_s(linksite, (char*)linkTo);
			intrpuzz->dispose();
			launchPuzz(linksite);	
		}
		else
		{
			intrpuzz->dispose();
			new HTMLPage(value);

			newobj = new (HTMLMarker);
			newobj->flag = value;

		}
	}
	else if (linkTo[i] == 'W')
	{
		if (!noPlay)
		{
			if (speak)
				speak->entrance->stop();
			((HTMLPage *)intrpuzz->page)->press->stop();
			((HTMLPage *)intrpuzz->page)->press->number = value;
			((HTMLPage *)intrpuzz->page)->press->play();
			intrpuzz->page->setScript(new SoundDelay);
		}
	}
	else if (linkTo[i] == 'S')
	{
		if (!noPlay)
		{
			if (speak)
				speak->entrance->stop();
			((HTMLPage *)intrpuzz->page)->press->stop();
			((HTMLPage *)intrpuzz->page)->press->number = value;
			((HTMLPage *)intrpuzz->page)->press->playBuffered(value,NULL);
			intrpuzz->page->setScript(new SoundDelay);
		}
	}
	return true;
}

HyperPrint::HyperPrint(unsigned char* theToken)
{
	name = "HyperPrint";
	priority = 720; //added 11-07 G.S.
	init((char*) theToken);
	currentHFile->myPrints->add(this);												   
}

int HyperPrint::init(char* theToken)
{															 
	fore_color = TEXT_COLOR_BLACK;
	size = 18;
	style = STYL_PLAIN;

	if (currentHFile->newRow)
	{
		
	}
	/*fore_color;
	back_color;
	high_color;
	/font = currentHFile->curFont;
	size = currentFile->curSize;
  	style; etc....*/

	if (currentHFile->currentAction == HYPER_BOLD)	
	{
		style = STYL_BOLD;	
	}
	else if (currentHFile->currentAction == HYPER_ITALIC)	
	{
		style = STYL_ITALIC;	
	}
	else if (currentHFile->currentAction == HYPER_BOLD_ITALIC)	
	{
		style = STYL_BOLD | STYL_ITALIC;	
	}

	if (currentHFile->currentAction == HYPER_LINK_OLD)	
	{
		if (!noc)
			fore_color = madeRGB(255,0,255);
		else
			noc = false;		
		currentHFile->currentAction = HYPER_LINK;
	}
	else if (currentHFile->currentAction == HYPER_LINK)	
	{
		if (!noc)
			fore_color = TEXT_COLOR_BLUE;
		else
			noc = false;		
	}
	else if (currentHFile->currentAction == HYPER_HEADER)	
	{	
		size = 20;
		currentHFile->theX = LeftMargin;
		currentHFile->theColumn = 0;
		++currentHFile->theRow;
		currentHFile->newRow = true;
	}
	else if (currentHFile->currentAction == HYPER_HEADER2)	
	{	
		size = 22;
		currentHFile->theX = LeftMargin;
		currentHFile->theColumn = 0;
		++currentHFile->theRow;
		currentHFile->newRow = true;
	}
	else if (currentHFile->currentAction == HYPER_HEADER3)	
	{	
		size = 24;
		currentHFile->theX = LeftMargin;
		currentHFile->theColumn = 0;
		++currentHFile->theRow;
 		currentHFile->newRow = true;
	}
	else if (currentHFile->currentAction == HYPER_HEADER4)	
	{	
		style = STYL_BOLD;
		size = 26;
		currentHFile->theX = LeftMargin;
		currentHFile->theColumn = 0;
		++currentHFile->theRow;
		currentHFile->newRow = true;
	}
	else if (currentHFile->currentAction == HYPER_HEADER5)	
	{	
		style = STYL_BOLD;
		size = 28;
		currentHFile->theX = LeftMargin;
		currentHFile->theColumn = 0;
		++currentHFile->theRow;
		currentHFile->newRow = true;
	}
	else if (currentHFile->currentAction == HYPER_HEADER6)	
	{	
		style = STYL_BOLD;
		size = 32;
		currentHFile->theX = LeftMargin;
		currentHFile->theColumn = 0;
		++currentHFile->theRow;
		currentHFile->newRow = true;
	}

	if (temp_x)
		x = temp_x;
	else
		x = currentHFile->theX;				   
	
	if (temp_y)
		y = temp_y;
	else
		y = currentHFile->theY;

	temp_x = 0;
	temp_y = 0;

	Print::init(theToken);
/*
	if (currentHFile->currentAction == HYPER_CENTER)
	{

		x = (3 + ((504 - (width)>>1)));
		sfxMoveSprite(text_ids[textBuffer].text,x,y);
		currentHFile->newRow = true;
	}
*/
	if (currentHFile->currentAction == HYPER_LINK)	{
		HyperFeature* tmpFeat = (HyperFeature*) currentHFile->hyperList->at(0);
		tmpFeat->left = x;
		tmpFeat->top = y;
		tmpFeat->right = x + width;
		tmpFeat->bottom = y + height;

		if (commandText[0])
		{	
			new CommandLine(x,y+3,width,height-4,(char *)&commandText);
//			memset (&commandText,0,256*sizeof(char));
		}		
	}

	currentHFile->theX += width;

	if ((currentHFile->theX > RIGHT_MARGIN) || (currentHFile->newRow))
	{
		currentHFile->theX = LeftMargin;
		currentHFile->theColumn = 0;
		++currentHFile->theRow;
		currentHFile->newRow = 0;
		currentHFile->theY += height;
	}
/*	
	if (currentHFile->currentAction == HYPER_TITLE ||currentHFile->currentAction == HYPER_HEADER)
	{
		currentHFile->theY += height;
	}

	currentHFile->theY += height;
	currentHFile->theY = TopMargin + currentHFile->theRow * height;
	currentHFile->rowHeight = height;
*/
	currentHFile->currentAction = TEXT_TOKEN;

	return true;	
}

HyperCursor::HyperCursor()
{
	name = "hyperCursor";
	view = 989;
	loop = 0;
	cel = 0;
	verb = DO_V;
}

HTMLAnnounce::HTMLAnnounce(int value,int streamit)
{
	soundNum = value;
	stream = streamit;
	entrance = new Sound;
	entrance->number = value;
	intrpuzz->add(this);
	setScript(new (AnnounceScript));
}

HTMLAnnounce::~HTMLAnnounce()
{
	entrance->dispose();
	speak = NULL;
}

void AnnounceScript::changeState ( int newState )
{
	switchTo
		seconds = 1;
    END

	BEG
		if (((HTMLAnnounce *)client)->stream)
		{
			((HTMLAnnounce *)client)->entrance->playBuffered(((HTMLAnnounce *)client)->soundNum,NULL);
			((HTMLAnnounce *)client)->setScript(NULL);	
		}
		else
		{
			((HTMLAnnounce *)client)->entrance->play();
			((HTMLAnnounce *)client)->setScript(NULL);	
		}
    END
}
		    
HTMLPause::HTMLPause(int value,int newpage)
{
	wait = value;
	page = newpage;
	intrpuzz->add(this);
	setScript(new (PauseScript));
}
void PauseScript::changeState ( int newState )
{
	//HTMLMarker* newobj;

	switchTo
		theGame->handsOff();
		seconds = ((HTMLPause*)client)->wait;
    END
	BEG
	//	((HTMLPause*)client)->setScript(NULL);
		dumping = true;
		tmpPage = ((HTMLPause*)client)->page;
/*		intrpuzz->dispose();
		theGame->handsOn();
		new HTMLPage(tmpPage);
		newobj = new (HTMLMarker);
		newobj->flag = tmpPage;
*/
    END
}

Scroller::Scroller(unsigned char* theToken)
{
	name = "scroller";
	x = temp_x;
	y = temp_y;																																				  
	width = temp_width;																																  
	speed = temp_speed;																															  

//	size = (3*width + strlen((char*)theToken));

//  for the courier model!
	size = (width + strlen((char*)theToken));

	offset = -1;													  
	temp_x = 0;														  
	temp_y = 0;														  
	temp_width = 0;
	temp_speed = 0;
	memset(&displayText,0,(1024*sizeof(char)));

//  for the courier model!
	memset(&displayText,32,(width*sizeof(char)));
//	memset(&displayText,32,(3*width*sizeof(char)));

	memset (&text,0,(1024*sizeof(char)));

//	memset (&text,32,(3*width*sizeof(char)));
//  for the courier model!
	memset (&text,32,(width*sizeof(char)));

	Common::strcat_s(text,(char*)theToken);
//	Common::strcat_s ((char*)&text,(char*)&displayText);
//	memset (&text[(size-(width*sizeof(char)))],32,(width*sizeof(char)));
	init();
}
void Scroller::init(void) {															 
	myPrint = new Print();
	myPrint->size	= 18;
	myPrint->fore_color = TEXT_COLOR_BLACK;
  	myPrint->x = x;
  	myPrint->y = y;
	myPrint->font = FNT_COURIER;

	memset(&displayText,0,(1024*sizeof(char)));
//	memcpy(&displayText,&text,(width*sizeof(char)));
//	myPrint->init((char*)displayText);
	setScript(new (ScrollerScript));
	intrpuzz->add(this);												   
}
Scroller::~Scroller()
{
	if (myPrint)
	{
		myPrint->addToPic();
		myPrint->dispose();
	}
	myPrint = NULL;
}
void ScrollerScript::changeState ( int newState )
{
	int counter = 0;
	//int width;
	switchTo
		if (++((Scroller*)client)->offset > ((Scroller*)client)->size - 1)
			((Scroller*)client)->offset = 0;			
		ticks = ((Scroller*)client)->speed;
    END

	BEG				   
		((Scroller*)client)->myPrint->erase();		
/*		if ((((Scroller*)client)->offset + ((Scroller*)client)->width)<((Scroller*)client)->size)
		{
			do					    
			{
				memset(&((Scroller*)client)->displayText,0,(1024*sizeof(char)));
				memcpy(&((Scroller*)client)->displayText,&((Scroller*)client)->text[((Scroller*)client)->offset],
						(((Scroller*)client)->width + counter));
				sfxSetTextFont (((Scroller*)client)->myPrint->font,((Scroller*)client)->myPrint->size, 
								((Scroller*)client)->myPrint->style );
				width = sfxGetTextWidth((char*)((Scroller*)client)->displayText);
				counter++;
			}while(width < ((((Scroller*)client)->width * 8) - (((Scroller*)client)->width>>1)));
		}
		else
		{
//  need only this for the courier model!
*/				memset(&((Scroller*)client)->displayText,0,(1024*sizeof(char)));
				memcpy(&((Scroller*)client)->displayText,&((Scroller*)client)->text[((Scroller*)client)->offset],
						(((Scroller*)client)->width + counter));
//		}
		((Scroller*)client)->myPrint->init((char*)((Scroller*)client)->displayText);
		((Scroller*)client)->setScript(new(ScrollerScript));		
    END
}
HTMLMarker::HTMLMarker()
{
	usedList->addToFront ( this );	 
}
void HTMLMarker::dispose(void)
{
	usedList->release(this);
	deathList->add(this);
}
AnnounceMarker::AnnounceMarker()
{
	announceList->addToFront ( this );	 
}
void AnnounceMarker::dispose(void)
{
	announceList->release(this);
	deathList->add(this);
}

} // namespace Spycraft
