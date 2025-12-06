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

#include "spycraft/game/cypher.h"
#include "spycraft/dmade/advmsg.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/email.h"
#include "spycraft/game/pcscreen.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

#define NONE			50
#define NOTONME			20

#define MONOPD			0
#define POLYPD			1
#define TRANSPD			2
#define BEALEPD			3
#define DESPD			4
#define RSAPD			5
 		  
#define A_B				0
#define C_D				1
#define E_H				2
#define I_M				3
#define N_P				4
#define Q_S				5
#define T_Z				6
#define PERIODICAL		7		
#define OTHER			8
	
#define SMALLHILIGHT	0
#define BIGHILIGHT		1

CodesPullDown	*codespd;
BookKeyPullDown *bookspd;

CodeButt		*codebutt;
CodeInfo		*codeinfo;
bool messageUp;
int currentKey;
Cypher *cypher;
TextList *decodeList;
TextList *codeList;
int glCurrentCode;
int glMsgCode;

char codes[6][15] = {"MonoAlpha", "PolyAlpha", "Transposition", "Beale",
					 "DES", "RSA"};

char keys[6][5][15] =  {"","","","","",
						"","","","","",
						"Standard", "",	"", "", "",
						"","","","","",
						"State Dept.","Commerce Dept.","HUD","USGS","",
						"Thorn private", "Thorn public", "Colby public", "Jamie public", "Holt public"};	

char bealeKeys[9][13][30] =  {"the Aenid","A Farewell to Arms","Alice in Wonderland","All the King's Men","Animal Farm",	"Anna Karenina", "A Separate Place", "As You Like It", "Beowulf", "The Bible", "Brave New World", "", "",
							  
							  "Candide", "Canterbury Tales", "Cathcer in the Rye", "Catch - 22", "Crime and Punishment", "The Crucible", "David Copperfield", "Death of a Salesman", "Doctor Faustus", "Don Quixote", "Dune", "", "",
							  
							  "Ethan Fromme", "For Whom the Bell Tolls", "Grapes of Wrath", "Great Expectations", "The Great Gatsby", "Gulliver's Travels", "Hamlet", "Heart of Darkness", "Henry IV: Part I", "The Hobbit", "Huckleberry Finn", "", "",
							  
							  "The LLiad", "The Invisible Man", "Julius Caesar", "the Jungle", "King Lear", "Lord of the Flies", "The Lord of the Rings", "Macbeth", "Madam Bovary", "The Merchant of Venice",
							  "Moby Dick", "My Antonia", "",
							  									
							  "Native Son", "Neuromancer", "The Odyssey", "Of Mice and Men", "The Old Man and the Sea", "Oliver Twist", "Othello", "Our Town", "Paradise", "The Pearl", "Plato's Republic", "Pride and Prejudice", "the Prince",

							  "The Red Badge of Courage", "Return of the Native", "Richard III", "Romeo and Juliet", "The Scarlet Letter", "Slaughterhouse Five", "Sons and Lovers", "The Sound and the Fury", "The Stranger",
							  "The Sun Also Rises", "", "", "",

							  "A Tale of Two Cities", "The Taming of the Shrew", "The Tempest", "To Kill a Mocking Bird", "Tom Jones", "Tom Sawyer", "Twelfth Night", "Uncle Tom's Cabin", "Wuthering Heights", "", "", "", "",

							  "Atlantic Monthly", "Business Week", "Chicago Monthly", "Congressional Quarterly", "National Geographic", "New Republic", "New York Times", "NewsWeek", "San Jose Mercury News", "Scientific American", "TIME",
							  "Weekly World News", ""};

/***********************************
 *		
 *		   Puzzle Page
 *		      
 **********************************/
Cypher::Cypher()
{
	cypher = this;
 	memset (&mailFlags,0,(5 * 80)*sizeof(bool));
	new(CypherBack);
	cypherSFX = new(Sound);
	decryptSFX = new(Sound);
	sfxLoadRes(170, RES_WAVE);
	sfxLockRes(170, RES_WAVE);

	codeList = new(TextList);
	decodeList = new(TextList);
	codebutt = new(CodeButt);
	messageUp = false;
	cypher->cypherSFX->play(40053, NULL);

 //	GameFlag.set(fGotNewElectBook);
//	GameFlag.set(fGotInitialElectBook);
	webreport->setCel(2);
}

PowerBookOne::PowerBookOne()
{
	IntrPuzzPage::init(WEB_POWERBOOKONE);	
	helppage = 92400;
	numFlags = 1;
	numVlinks = 0;
	flags[0] = fTargetGPSAvailable;  
	keyMatch = 0; //Croak 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 117;
	loadCode(118);
}

PowerBookTwo::PowerBookTwo()
{
	IntrPuzzPage::init(WEB_POWERBOOKTWO);	
	helppage = 92400;
	numFlags = 0;
	numVlinks = 0;
	keyMatch = 0; //Croak 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 119;
	loadCode(120);
}

PowerBookThree::PowerBookThree()
{
	IntrPuzzPage::init(WEB_POWERBOOKTHREE);	
	helppage = 92400;
	numFlags = 0;
	numVlinks = 0;
	keyMatch = 0; //Croak 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 121;
	loadCode(122);
}

PowerBookFour::PowerBookFour()
{
	IntrPuzzPage::init(WEB_POWERBOOKFOUR);	
	helppage = 92400;
	numFlags = 0;
	numVlinks = 0;
	keyMatch = 0; //Croak 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 123;
	loadCode(124);
}

PowerBookFive::PowerBookFive()
{
	IntrPuzzPage::init(WEB_POWERBOOKFIVE);	
	helppage = 92400;
	numFlags = 1;
	numVlinks = 0;
	flags[0] = fTargetGPSAvailable;  
	keyMatch = 0; //Croak 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 125;
	loadCode(126);
}

PowerBookSix::PowerBookSix()
{
	IntrPuzzPage::init(WEB_POWERBOOKSIX);	
	helppage = 92400;
	numFlags = 0;
	numVlinks = 0;
	keyMatch = 0; //Croak 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 127;
	loadCode(128);
}

PowerBookSeven::PowerBookSeven()
{
	IntrPuzzPage::init(WEB_POWERBOOKSEVEN);	
	helppage = 92400;
	numFlags = 1;
	numVlinks = 0;
	flags[0] = fLinus25;  
	keyMatch = 0; //Croak 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 129;
	loadCode(130);
}

CypherPitOne::CypherPitOne()
{
	IntrPuzzPage::init(WEB_CYPHERPITONE);	
	helppage = 92400;
	numFlags = 1;
	numVlinks = 0;
	flags[0] = fDiscoverPit1;  
	keyMatch = 2; //rabid 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 101;
	loadCode(102);
	pcLastUsed[PC_CIPHER] = WEB_CYPHERPITONE;
}
						   
CypherPitTwo::CypherPitTwo()
{
	IntrPuzzPage::init(WEB_CYPHERPITTWO);	
	helppage = 92400;
	numFlags = 1;
	numVlinks = 0;
	flags[0] = fDiscoverPit2;
	keyMatch = 2; //rabid 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 103;
	loadCode(104);
	pcLastUsed[PC_CIPHER] = WEB_CYPHERPITTWO;
}

CypherPitThree::CypherPitThree()
{
	IntrPuzzPage::init(WEB_CYPHERPITTHREE);	
	helppage = 92400;
	numFlags = 1;
	numVlinks = 0;
	flags[0] = fDiscoverPit3;
	keyMatch = 2; //rabid 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 105;
	loadCode(106);
	pcLastUsed[PC_CIPHER] = WEB_CYPHERPITTHREE;
}

CypherMoleOne::CypherMoleOne()
{
	IntrPuzzPage::init(WEB_CYPHERMOLEONE);	
	helppage = 92400;
	numFlags = 0;
	numVlinks = 0;
//	flags[0] = fDoneMole1;
	keyMatch = 2; //rabid 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 107;
	loadCode(108);
	pcLastUsed[PC_CIPHER] = WEB_CYPHERMOLEONE;
}

CypherMoleTwo::CypherMoleTwo()
{
	IntrPuzzPage::init(WEB_CYPHERMOLETWO);	
	helppage = 92400;
	numFlags = 0;
	numVlinks = 0;
//	flags[0] = fDoneMole2;
	keyMatch = 2; //rabid 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 109;
	loadCode(110);
	pcLastUsed[PC_CIPHER] = WEB_CYPHERMOLETWO;
}

CypherMoleThree::CypherMoleThree()
{
	IntrPuzzPage::init(WEB_CYPHERMOLETHREE);	
	helppage = 92400;
	numFlags = 0;
	numVlinks = 0;
//	flags[0] = fDoneMole3;
	keyMatch = 0; //Croak 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 111;
	loadCode(112);
	pcLastUsed[PC_CIPHER] = WEB_CYPHERMOLETHREE;
}

CypherMoleFour::CypherMoleFour()
{
	IntrPuzzPage::init(WEB_CYPHERMOLEFOUR);	
	helppage = 92400;
	numFlags = 1;
	numVlinks = 0;
	flags[0] = fTunisiaAvailable;
	keyMatch = 0; //Croak 
	codeMatch = OTHER;  //OTHER
	matchMsgID = 113;
	loadCode(114);
	pcLastUsed[PC_CIPHER] = WEB_CYPHERMOLEFOUR;
}

YingCypherNote::YingCypherNote()
{
	IntrPuzzPage::init(WEB_YINGCYPHER);	
	helppage = 92400;
	webreport->hide();
	yingsendbut = new (YingSendBut);
	numFlags = 1;
	numVlinks = 0;
	flags[0] = fBrokeYingBealeCode;
	keyMatch = 8; //The Rabid Hound 
	codeMatch = A_B;  //other beale keys
	matchMsgID = 115;
	loadCode(116);
	pcLastUsed[PC_CIPHER] = WEB_YINGCYPHER;
	GameFlag.set(fGreg17);
}

AlphaDecrypt::AlphaDecrypt()
{								 
	user->input = false;
	sfxSystemCursor(SYS_WAIT);
	name = "alphadecrypt";
	cycleSpeed = 8;	
	view = 100;
	loop = 24;
	cel = 0;
	x = 129; 
	y = 255;
	draw();
	setPri(720);
	intrpuzz->addToFront ( this );
//	cypher->decryptSFX->loop = 100;
//	cypher->decryptSFX->play(132, NULL);

//	setCycle(new EndLoop((Prop*)this, this));
	cypher->setScript(new AlphaDecryptScript(this));
}

AlphaDecryptScript::AlphaDecryptScript(AlphaDecrypt *theProp)
{
	alphaDecrypt = theProp;	
}
	
void AlphaDecryptScript::changeState ( int newState )
{
	switchTo
	//	cypher->decryptSFX->loop = 2;
		cypher->decryptSFX->play(170, NULL);
		alphaDecrypt->setCycle(new EndLoop((Prop*)alphaDecrypt, this));	
	END
	BEG
		cypher->decryptSFX->stop();
		alphaDecrypt->dispose();
		ticks = 10;
	END
	BEG
		cypher->cypherSFX->play(40057, this);
	END
	BEG
		ticks = 6;
	END
	BEG	
		sfxMadeCursor();
		user->input = true;
		dispose();
	END
}

void AlphaDecrypt::cue()
{
	cypher->cypherSFX->play(40057, NULL);
	cypher->decryptSFX->stop();

	dispose();
	sfxMadeCursor();
	user->input = true;
}

Decrypt::Decrypt(KeyPullDown* thisKey)
{
	user->input = false;
	sfxSystemCursor(SYS_WAIT);
	name = "decrypt";
	cycleSpeed = 8;	
	view = 100;
	loop = 24;
	cel = 0;
	x = 129; 
	y = 255;
	draw();
	setPri(720);
	theKey = thisKey;
	intrpuzz->addToFront ( this );
//	cypher->decryptSFX->loop = 100;
//	cypher->decryptSFX->play(132, NULL);
//	setCycle(new EndLoop((Prop*)this, this));
	cypher->setScript(new DecryptScript(this));
}

DecryptScript::DecryptScript(Decrypt *theProp)
{
	decrypt = theProp;	
}
	
void DecryptScript::changeState ( int newState )
{
	switchTo
//		cypher->decryptSFX->loop = 25;
		ticks = 2; //wait for the decrypted message to appear
	END
	BEG
		cypher->decryptSFX->play(170, NULL);
		decrypt->setCycle(new EndLoop((Prop*)decrypt, this));	
		cypher->loadDecode(glMsgCode);
	END
	BEG
		cypher->decryptSFX->stop();
		decrypt->dispose();
		ticks = 10;
	END
	BEG
	 	if((cypher->codeMatch == glCurrentCode) && (cypher->keyMatch == currentKey))
		{
			cypher->cypherSFX->play(40056, this);
		}						
		else
		{
			cypher->cypherSFX->play(40057, this);
		}
	END
	BEG
		ticks = 6;
	END
	BEG
		sfxMadeCursor();
		user->input = true;
		dispose();
	END
}

void Decrypt::cue(void)
{
//	currentKey->displayInfo();
	cypher->decryptSFX->stop();
	if((cypher->codeMatch == glCurrentCode) && (cypher->keyMatch == currentKey))
	{
		cypher->cypherSFX->play(40056, NULL);
	}						
	else
	{
		cypher->cypherSFX->play(40057, NULL);
	}
	dispose();
	sfxMadeCursor();
	user->input = true;
}

void YingCypherNote::dispose()
{
	if(web_intrface)
	{
		yingsendbut->dispose();
		webreport->show();
	}		
	Cypher::dispose();
}

CypherEmpty::CypherEmpty()
{
	IntrPuzzPage::init(WEB_YINGCYPHER);	
	numFlags = 0;
	numVlinks = 0;
	codebutt->dispose();
	new(NoCypherBox);
}

YingSendBut::YingSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void YingSendBut::activate()
{
	GameFlag.set(fReportYingCypher);
	mailFlags[0][1] = true;
	mailFlags[0][2] = true;
	mailFlags[0][3] = true;
	mailFlags[0][4] = true;
	//these mailFlags will need to check global flags
	mailFlags[0][5] = true;
	mailFlags[0][6] = true;
	mailFlags[0][7] = true;
	mailFlags[0][8] = true;
	mailFlags[0][9] = true;
	mailFlags[0][10] = true;

	if(GameFlag.test(fBrokeYingBealeCode))
	{
		mailFlags[1][0] = true;
		mailFlags[1][1] = true;
	}
	mailFlags[1][2] = true;
	mailFlags[1][3] = true;
	mailFlags[1][4] = true;

	if(!GameFlag.test(fKnowYingInMoscow))	
	{
		new EMailSend(206, this);
	}
	else if (!(GameFlag.test(fFullBriefing)))
		new EMailSend(229, this);
	else
		new EMailSend(230, this);
}

void YingSendBut::cue()
{
	if((choiceIndex[0] == 10) && (choiceIndex[3] == 0))	//Ying ChuWang
	{
		if(!GameFlag.test(fKnowYingInMoscow))	
		{
			GameFlag.set(fKnowYingInMoscow);
		 	new VLink("Use Ying against Grendel: Holt, David M.", "644.avi", fSaw644, 0, new EMailAttachment("Photo Doctoring", 98030));
//			new VLink("158.avi", 0, 0);
		}	
	}		
	else if(!GameFlag.test(fKnowYingInMoscow))
	{
		if(!GameFlag.test(fGreg26))		
		{
			new VoiceMail("Doesn't add up: Holt, David", 30651, HOLT);
			GameFlag.set(fGreg26);
		}
		else if(!GameFlag.test(fGreg27))
		{
			new VoiceMail("Stop guessing: Holt, David", 30653, HOLT);
			GameFlag.set(fGreg27);
		}
		else
		{
			if(!(webreport->flag & OBJECT_HIDDEN))
			{
				WebReport::cue();
			}

			comlinkshort = NULL;
			intrpuzz->quitIntrface();
			if(theInset)
				theInset->dispose();
			theGame->newRoom(new Rm60000(END_FIRED));
			return;	
		}
	}
									
	if(!(webreport->flag & OBJECT_HIDDEN))
	{
		WebReport::cue();
	}
}

void Cypher::loadCode ( int id)
{
	MSGTag *tag = (MSGTag *)sfxLoadRes ( id, RES_TEXT );
	sfxLockRes( id, RES_TEXT );

	if ( tag == NULL )
		return;

   	int theX = 128;
	int theY = 94;
	for(int index = 0; index < tag->size; index++)
	{
		Print* codePrint;
		codePrint = new(Print);
		codePrint->x = theX;
		codePrint->y = theY;
		codePrint->size = 8;
		codePrint->fore_color = TEXT_COLOR_BLACK;
		codePrint->init(tag->data[index]);
		codeList->add(codePrint);
		theY += 13;
	}
	sfxUnlockRes( id, RES_TEXT );
}

void Cypher::loadDecode ( int id)
{
	MSGTag *tag = (MSGTag *)sfxLoadRes ( id, RES_TEXT );
	sfxLockRes( id, RES_TEXT );

	if ( tag == NULL )
		return;

   	int theX = 128;
	int theY = 253;
	if (!decodeList->isEmpty())
	{
		decodeList->dispose();
	}

	for(int index = 0; index < tag->size; index++)
	{
		Print* codePrint;
		codePrint = new(Print);
		codePrint->x = theX;
		codePrint->y = theY;
		codePrint->size = 8;
		//Common::strcpy_s (codePrint->myString, tag->data[index] );
		codePrint->fore_color = TEXT_COLOR_BLACK;

		codePrint->init(tag->data[index]);
		decodeList->add(codePrint);
		theY += 13;
	}
	sfxUnlockRes( id, RES_TEXT );
}

void Cypher::dispose()
{
	if (!codeList->isEmpty())
	{
		codeList->dispose();
	}
	delete codeList;

	if (!decodeList->isEmpty())
	{
		decodeList->dispose();
	}
	delete decodeList;

	cypherSFX->dispose();
	decryptSFX->dispose();
	sfxUnlockRes(170, RES_WAVE);

	if(messageUp)
	{
		codeinfo->dispose();
	}
	if(web_intrface)
	{
		webreport->setCel(0);
	}		

	IntrPuzzPage::dispose();
}
	
Cypher::~Cypher()
{
}	

void TextList::dispose()
{
	Node *n = head;
	while ( n )
	{
		if (n->data)
    		((Print *)n->data)->erase();

 		n = n->next;
	}
//	releaseAll();
}	

/***********************************
 *		
 *			  Print
 *		      
 **********************************/
CodeInfo::CodeInfo(char *infoText)
{
	codeinfo = this;

	fore_color = TEXT_COLOR_BLACK;
	x = 125;  //174
	y = 230;
	size = 14;
	init(infoText);
}

/***********************************
 *		
 *			  List
 *		      
 **********************************/
int ItemsList::handleEvent(MADEEventStamp* event)
{
	Node *n = head;
	int index = 0;

	while ( n )
	{
		if(n->data->handleEvent(event))
			return(index);
		n = n->next;
		index++;
	}
	return(NOTONME);
}

/***********************************
 *		
 *			  Views
 *		      
 **********************************/
CypherBack::CypherBack()
{
	view = 100;
	loop = 0;
	cel = 0;
	init(0, 16);
	noSkip();
	addToPic();
}

NoCypherBox::NoCypherBox()
{
	view = 100;
	loop = 23;
	cel = 0;
	init(119, 200);
	noSkip();
	addToPic();
}

PDHighLight::PDHighLight(int hilightNum)
{
	view = 100;
	loop = 15;
	cel = hilightNum;
	init(0, 62);
	sfxSpriteGlass(mySprite,1,MK_COLOR16(0,0,255),40);
	hide();
	setPri(601);
}

PullDown::PullDown()
{
	FeatureItems = new(ItemsList);	
}	

PullDown::~PullDown()
{
    				   
//	FeatureItems->dispose();
//	delete FeatureItems;
}	

void PullDown::dispose()
{
	FeatureItems->dispose();
	delete FeatureItems;
	IntrPuzzView::dispose();
}
    			
void PullDown::createItems()
{
	int index;
	int theY;

	for(index = 0; index < numItems; index++)
	{
		theY = top + (index * 22);
		FeatureItems->add( new PullDownFeature(left, theY, index) );		
	}
}

CodesPullDown::CodesPullDown()
{
	view = 100;
	loop = 2;
	cel = 0;
	init(9, 97);
	noSkip();
	numItems = 6;
	codespd = this;
	hilight = new PDHighLight(SMALLHILIGHT);
	currentCode = NONE;
	createItems();	
	setPri(599);
}

void CodesPullDown::removePullDown()
{
	if(bookspd)
	{
		if(bookspd->currentCode != NONE)
		{
			bookspd->removePullDown();
		}			
		bookspd->hilight->dispose();
		bookspd->dispose();
	}		
	else
	{
		keypd->hilight->dispose();
		keypd->dispose();
	}
}

int CodesPullDown::handleEvent(MADEEventStamp* event)
{
	int codeIndex;
	int codeNum;

	if (onMe(event) && user->input)  
	{
		codeIndex = FeatureItems->handleEvent(event);		
		if(codeIndex == currentCode || codeIndex == NOTONME)
			return false;

		if(currentCode != NONE)	//we have a key pulldown showing
 			removePullDown();

		char message[80];
		if (event_type == USER_LEFT_DOWN)
		{
			switch(codeIndex)
			{

				case MONOPD:
					Common::strcpy_s(message, "      Key: ");
					Common::strcat_s(message, "MonoAlpha");
					if(messageUp)
					{
						codeinfo->dispose();
					}
					new CodeInfo(message);
				 	messageUp = true;
					currentCode = NONE;
					//new
					//currentKey = keyIndex;
					codebutt->removeMenus();
					codeNum = 131 + (rand() % 3);
					cypher->loadDecode(codeNum);
				//	cypher->loadDecode(199);
					new AlphaDecrypt();
					break;

				case POLYPD:
					Common::strcpy_s(message, "      Key: ");
					Common::strcat_s(message, "PolyAlpha");
					if(messageUp)
					{
						codeinfo->dispose();
					}
					new CodeInfo(message);
				 	messageUp = true;
					currentCode = NONE;
					//new
				//	currentKey = keyIndex;
					codebutt->removeMenus();
					codeNum = 131 + (rand() % 3);
					cypher->loadDecode(codeNum);
					//cypher->loadDecode(199);
					new AlphaDecrypt();
					break;
			}
		}

		switch(codeIndex)
		{
					
			case MONOPD:
				currentCode = NONE;
				break;

			case POLYPD:
				currentCode = NONE;
				break;

			case TRANSPD:	
				keypd = new KeyPullDown(119, 133, 3, 1, SMALLHILIGHT);
				currentCode = TRANSPD;
				break;
					
			case BEALEPD:
//					keypd = new KeyPullDown(119, 97, 3, 2);
				new(BookKeyPullDown);
				currentCode = BEALEPD;
				break;
				
			case DESPD:
				keypd = new KeyPullDown(119, 114, 5, 4, BIGHILIGHT);
				currentCode = DESPD;
				break;

			case RSAPD:
				keypd = new KeyPullDown(119, 133, 6, 5, SMALLHILIGHT);
				currentCode = RSAPD;
//				new(SimplePullDown);
				break;
			
		}						
		hilight->show();
		hilight->posn(left + 1, (top + 1) + (codeIndex * 22));
	}
	return(false);
}

BookKeyPullDown::BookKeyPullDown()
{
	view = 100;
	loop = 4;
	if(GameFlag.test(fGotInitialElectBook) || GameFlag.test(fGotNewElectBook) || GameFlag.test(fGotOldElectBook))
	{
		numItems = 9;
		cel = 0;
	}
	else
	{
		numItems = 8;
		cel = 1;
	}
	init(119, 97);
	noSkip();

	bookspd = this;
	hilight = new PDHighLight(SMALLHILIGHT);
	currentCode = NONE;
	createItems();	
	setPri(599);
}

BookKeyPullDown::~BookKeyPullDown()
{
	bookspd = NULL;
}	   

void BookKeyPullDown::dispose()
{
	bookspd = NULL;
	PullDown::dispose();
}
		
void BookKeyPullDown::removePullDown()
{
	keypd->hilight->dispose();
	keypd->dispose();
}

int BookKeyPullDown::handleEvent(MADEEventStamp* event)
{
	int keyIndex;

	if (onMe(event) && user->input)  
	{
		keyIndex = FeatureItems->handleEvent(event);		
		if(keyIndex == currentCode || keyIndex == NOTONME)
			return false;

		if(currentCode != NONE)
 			removePullDown();

		switch(keyIndex)
		{
			case A_B:
					keypd = new KeyPullDown(229, 97, 7, 11, BIGHILIGHT);
					currentCode = A_B;
				break;
				
			case C_D:
					keypd = new KeyPullDown(229, 114, 8, 11, BIGHILIGHT);
					currentCode = C_D;
				break;

			case E_H:
					keypd = new KeyPullDown(229, 133, 9, 11, BIGHILIGHT);
				 	currentCode = E_H;
				break;
			
			case I_M:
					keypd = new KeyPullDown(229, 133, 10, 12, BIGHILIGHT);
				 	currentCode = I_M;
				break;

			case N_P:
					keypd = new KeyPullDown(229, 133, 11, 13, BIGHILIGHT);
				 	currentCode = N_P;
				break;

			case Q_S:
					keypd = new KeyPullDown(229, 133, 12, 10, BIGHILIGHT);
				 	currentCode = Q_S;
				break;

			case T_Z:
					keypd = new KeyPullDown(229, 133, 13, 9, BIGHILIGHT);
				 	currentCode = T_Z;
				break;

			case PERIODICAL:
					keypd = new KeyPullDown(229, 133, 14, 12, BIGHILIGHT);
				 	currentCode = PERIODICAL;
				break;
			
			case OTHER://this case looks like this because the design changed a few days before we were supposed to ship
					if(GameFlag.test(fGotInitialElectBook))
					{
						if(GameFlag.test(fGotNewElectBook))
						{
							if(GameFlag.test(fGotOldElectBook))
							{
								Common::strcpy_s(bealeKeys[8][0], "Croak! goeth the Raven"); 
								Common::strcpy_s(bealeKeys[8][1], "The Khan and I"); 
								Common::strcpy_s(bealeKeys[8][2], "The Rabid Hound"); 
								keypd = new KeyPullDown(229, 270, 22, 3, BIGHILIGHT);
							}
							else
							{
								Common::strcpy_s(bealeKeys[8][0], "Croak! goeth the Raven"); 
								Common::strcpy_s(bealeKeys[8][1], "The Rabid Hound"); 
								keypd = new KeyPullDown(229, 270, 19, 2, BIGHILIGHT);
							}
						}
						else if(GameFlag.test(fGotOldElectBook))
						{
							Common::strcpy_s(bealeKeys[8][0], "The Khan and I"); 
							Common::strcpy_s(bealeKeys[8][1], "The Rabid Hound"); 
							keypd = new KeyPullDown(229, 270, 20, 2, BIGHILIGHT);
						}
						else
						{
							Common::strcpy_s(bealeKeys[8][0], "The Rabid Hound"); 
							keypd = new KeyPullDown(229, 270, 16, 1, BIGHILIGHT);
						}
					 	currentCode = OTHER;
						break;
					}								
					else if(GameFlag.test(fGotNewElectBook))
					{
						if(GameFlag.test(fGotOldElectBook))
						{
							Common::strcpy_s(bealeKeys[8][0], "Croak! goeth the Raven"); 
							Common::strcpy_s(bealeKeys[8][1], "The Khan and I"); 
							keypd = new KeyPullDown(229, 270, 21, 2, BIGHILIGHT);
						}
						else
						{
							Common::strcpy_s(bealeKeys[8][0], "Croak! goeth the Raven"); 
							keypd = new KeyPullDown(229, 270, 17, 1, BIGHILIGHT);
						}
					 	currentCode = OTHER;
					}
					else
					{
						Common::strcpy_s(bealeKeys[8][0], "The Khan and I"); 
						keypd = new KeyPullDown(229, 270, 18, 1, BIGHILIGHT);
					 	currentCode = OTHER;
					}	
				break;
		}						
		hilight->show();
		hilight->posn(left + 1, (top + 1) + (keyIndex * 22));
	}
	return(false);
}

KeyPullDown::KeyPullDown(int theX, int theY, int theLoop, int totalItems, int hilightNum)
{
	view = 100;
	loop = theLoop;
	cel = 0;
	init(theX, theY);
	noSkip();
	numItems = totalItems;
	createItems();	
	hilight = new PDHighLight(hilightNum);
	setPri(599);
}

void KeyPullDown::displayInfo()
{
	char message[80];

	Common::strcpy_s(message, "Code: ");
	Common::strcat_s(message, codes[codespd->currentCode]);
	Common::strcat_s(message, "  Key: ");
	if(bookspd)
	{
		Common::strcat_s(message, bealeKeys[bookspd->currentCode][currentKey]);
		if(bookspd->currentCode == OTHER)
		{
			if(strcmp(bealeKeys[bookspd->currentCode][currentKey], "Croak! goeth the Raven") == 0)
			{
				currentKey = 0;
			}
			else if(strcmp(bealeKeys[bookspd->currentCode][currentKey], "The Rabid Hound") == 0)
			{
				currentKey = 2;
			}
			if(strcmp(bealeKeys[bookspd->currentCode][currentKey], "The Khan and I") == 0)
			{
				currentKey = 1;
			}	
		}
		thisCode = bookspd->currentCode;	   		
	}
	else
	{
		Common::strcat_s(message, keys[codespd->currentCode][currentKey]);
		thisCode = codespd->currentCode;	   		
	}
	glCurrentCode = thisCode;  //we need to set this for dos version, thisCode is getting trashed

	if(messageUp)
	{
		codeinfo->dispose();
	}
	new CodeInfo(message);
 	messageUp = true;

	if((cypher->codeMatch == thisCode) && (cypher->keyMatch == currentKey))
	{
		int i;

	//	cypher->loadDecode(cypher->matchMsgID);
		glMsgCode = cypher->matchMsgID;
		 
		for(i = 0; i < cypher->numVlinks; i++)
		{
			new VLink(cypher->vlinks[i], 0, 0);
		}											 
		
		for(i = 0; i < cypher->numFlags; i++)
		{
			if((cypher->flags[i] == fLinus25) && !GameFlag.test(fLinus25))
			{
				new VLink("Procat's rudder: Colby, William", "93110.avi", fSaw93110, 0);	
			}

			if((cypher->flags[i] == fDiscoverPit1) && !GameFlag.test(fDiscoverPit1))
			{
		//		new VLink("675.avi", 0, 0);	
			}

			if((cypher->flags[i] == fTunisiaAvailable) && !GameFlag.test(fGreg25))
			{
				comlinks->removeVoiceMail(999);	 //if they haven't watched this voice mail don't set it again
				GameFlag.set(fGreg25);
				new VoiceMail("Danger's what you do: Holt, David M.", 999, HOLT, fTunisiaAvailable, 6);
			}
			else
			{
				GameFlag.set(cypher->flags[i]);   //this always gets set unless this is the tunisia puzzle
			}

			if(GameFlag.test(fDiscoverPit1) || GameFlag.test(fDiscoverPit2))
			{
				if(!GameFlag.test(fSwanSolved))
				{
					GameFlag.set(fSwanSolved);
					new VLink("Imminent boom: Holt, David M.", "35012.avi", fSawAchtung, 0, new EMailAttachment("Track Onyx", 98330));
				}
			}
		}
	}						
	else
	{
		// code num's 131, 132, 133
		int codeNum = 131 + (rand() % 3);
	//	cypher->loadDecode(codeNum);
		glMsgCode = codeNum;
	}
}

int KeyPullDown::handleEvent(MADEEventStamp* event)
{
	int keyIndex;

	if (onMe(event) && user->input)  
	{
		keyIndex = FeatureItems->handleEvent(event);		
		if(keyIndex == NOTONME)
			return false;
		hilight->show();
		hilight->posn(left + 1, (top + 1) + (keyIndex * 22));
		if (event_type == USER_LEFT_DOWN)
		{
			currentKey = keyIndex;
			displayInfo();
			codebutt->removeMenus();
			new Decrypt(this);
			//displayInfo();
			return true;
		}
	}
	else
	{
		hilight->hide();
	}
	return false;
}

/***********************************
 *		
 *			  Features
 *		      
 **********************************/
PullDownFeature::PullDownFeature(int theX, int theY, int theIndex)
{
	index = theIndex;
	init(theX, theY, 110, 22);
}
		 	
void PullDownFeature::init ( int theX, int theY, int theWidth, int theHeight )
{
	left = theX;
	top = theY;
	right = left + theWidth - 1;
	bottom = top + theHeight - 1;
}

int PullDownFeature::handleEvent(MADEEventStamp* event)
{
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_DOWN)
	    {
		}
		return(true);
	}
	return(false);
}

/***********************************
 *		
 *			  Buttons
 *		      
 **********************************/
CodeButt::CodeButt()
{
	init(8, 67, 100, 1, 0);
	menusActive = false;
	noSkip();
}

void CodeButt::removeMenus()
{
	codespd->hilight->dispose();
//	codespd->FeatureItems->dispose();
/*	if(bookspd)
	{
		if(bookspd->currentCode != NONE)
		{
			bookspd->removePullDown();
		}		
		bookspd->hilight->dispose();
		bookspd->dispose();
	}*/

	if(codespd->currentCode != NONE)
	{
		codespd->removePullDown();
	}
	codespd->dispose();
	menusActive = false;
	setCel(0);
}
	
int CodeButt::handleEvent(MADEEventStamp* event)
{
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_DOWN)
	    {
			if(cel == 0)
			{ 
				setCel(1);
				menusActive = true;
				new(CodesPullDown);
			}
			else
			{
				removeMenus();
			}		
			return(true);
		}
		if(menusActive)
		{
			codespd->hilight->hide();
		//	codespd->currentCode = NONE;
		}
	}
	return(false);
}

} // namespace Spycraft

