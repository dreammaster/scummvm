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
									 
#include "spycraft/game/sndanal.h"
#include "spycraft/game/maptool.h"

#ifdef DIRECTX
#include "spycraft/dmade/advmovie.h"
#include "spycraft/dmade/advsound.h"
#endif

namespace Spycraft {

extern BlinkCursor* normalCursor;
extern GunCursor* gunCursor;
extern Colby colby;		  
											     
extern Flag GameFlag;						 
extern int currentFocus;				 		   
#ifdef DIRECTX
//int pausedanal = false;
extern int soundGone;
extern void SetMovieErase (int);
#endif
extern void AddScreen2Pic(void);
extern void AddScreen2PicEx ( int dx, int dy, int w, int h );

Sndanal * anal = NULL;
SndInfo * sndinfo[22] = {NULL,NULL,NULL,NULL,NULL,
						 NULL,NULL,NULL,NULL,NULL,
						 NULL,NULL,NULL,NULL,NULL,
						 NULL,NULL,NULL,NULL,NULL,
			 			 NULL,NULL};
extern MapInfo * mapinfo;

char permnames [5][64] = {"Source","Cohen, Dr. Darren M.","Benton, Lt. Warren T.","Magruder, Dr. Kate","Sung, Dr. Lee Tae"};
char movienames [22][64] = {"650.avi","99926.avi","99927.avi","99928.avi","99929.avi","99930.avi","99931.avi","999311.avi",
							"99932.avi","99933.avi","99934.avi","99936.avi","99935.avi","99937.avi","99938.avi","99939.avi",
							"99940.avi","99941.avi","99942.avi","99943.avi","99945.avi","99944.avi"};

char cy1names [4][64] = {"Source","Cohen, Dr. Darren M.","Chungwang, Ying","#45125 Rotor, Rostov Mfg."};
int cy1links[8]		  = {0,99902,99406,97809,0,0,0,0};	
int cy1points[4][2]   = {{0,794},{2446,3243},{3271,4064},{4081,4584}};
int cy1marks[25][4]  =	{{7,16,10,19},{78,96,81,99},{173,190,176,199},{201,230,204,233},{285,291,288,294},
						 {348,368,351,371},{422,472,425,475},{579,608,582,611},{626,708,629,711},{734,756,737,759},
						 {-1,-1,-1,-1},
						  						 
						 {21,64,24,67},{129,170,132,173},{192,199,195,202},{234,240,237,243},{250,282,253,285},
						 {295,328,298,331},{372,419,375,422},{475,542,478,545},{612,623,615,626},{711,731,714,734},
						 {759,777,762,780},{-1,-1,-1,-1},

						 {252,423,255,426},{-1,-1,-1,-1}};

char cy2names [3][64] = {"Source","Chungwang, Ying","Cohen, Dr. Darren M."};				    
int cy2links[8]		  = {0,99406,99902,0,0,0,0,0};	
int cy2points[3][2] =   {{0,374},{406,744},{811,1184}};
int cy2marks[12][4] = 	{{18,21,21,24},{106,109,109,112},{118,130,121,133},{186,203,189,206},{299,334,302,337},
					 	 {-1,-1,-1,-1},
						 {4,13,7,17},{30,103,33,106},{136,183,139,186},{207,295,210,298},{348,354,351,357},											   
						 {-1,-1,-1,-1}};

char c13names [3][64] = {"Source","Cohen, Dr. Darren M.","Male 35-45 yrs."};
int c13links[8]		  = {0,99902,97810,0,0,0,0,0};	
int c13points[3][2] =   {{0,404},{841,1241},{2101,2502}/*{1680,2084}*/};
int c13marks[21][4] = 	{{3,13,6,16},{54,73,57,76},{136,156,139,159},{197,209,200,212},{245,253,248,256},
					 	 {319,332,322,338},{369,376,372,379},{-1,-1,-1,-1},

						 {18,43,21,46},{77,88,80,91},{98,104,101,107},{114,135,117,138},{159,166,162,169},											   
						 {174,196,177,199},{227,243,230,246},{256,260,259,263},{269,317,272,320},{335,339,338,342},											   
						 {348,362,351,365},{379,386,382,389},{-1,-1,-1,-1}};

char c26names [3][64] = {"Source","Cohen, Dr. Darren M.","Male 25-35 yrs."};
int c26links[8]		  = {0,99902,97813,0,0,0,0,0};	
int c26points[3][2] =   {{0,434},{886,1316},{451,859}};
int c26marks[21][4] = 	{{6,15,9,18},{37,51,40,54},{97,118,100,121},{126,158,129,161},{186,212,189,215},
					 	 {218,243,221,246},{252,277,255,280},{325,336,328,339},{346,365,349,368},{411,423,414,426},
					 	 {-1,-1,-1,-1},

						 {23,27,26,30},{58,67,61,70},{74,95,77,98},{166,168,169,171},{173,184,176,187},											   
						 {282,292,285,295},{299,323,302,326},{371,390,374,393},{397,406,400,409},{-1,-1,-1,-1}};

char b6names [3][64] = {"Source","Benton, Lt. Warren T.","Female 20-30 yrs."};
int b6links[8]		  = {0,99903,97801,0,0,0,0,0};	
int b6points[3][2] =   {{0,321},{676,999},{346,639}};
int b6marks[14][4] = 	{{2,12,5,15},{52,71,55,74},{108,122,111,125},{151,174,154,177},{189,201,192,204},
					 	 {223,275,226,278},{293,303,296,306},{-1,-1,-1,-1},

						 {13,44,16,47},{73,98,76,101},{127,149,130,152},{203,215,206,218},{277,286,280,289},
						 {-1,-1,-1,-1}};

char b14names [3][64] = {"Source","Benton, Lt. Warren T.","Male 35-45 yrs."};
int b14links[8]		  = {0,99903,97802,0,0,0,0,0};	
int b14points[3][2] =   {{0,496},{2086,2582},{2611,3064}};
int b14marks[19][4] = 	{{8,11,11,14},{17,19,20,22},{40,50,43,53},{64,73,67,76},{173,179,176,182},
					 	 {258,285,261,288},{391,406,394,409},{455,482,458,485},{-1,-1,-1,-1},

						 {27,31,30,34},{82,109,85,112},{188,206,191,209},{212,225,215,228},{233,256,236,259},											   
						 {301,333,304,336},{342,369,345,372},{409,415,412,418},{422,448,425,451},{-1,-1,-1,-1}};

char b14bnames [4][64] = {"Source","Benton, Lt. Warren T.","Male 35-45 yrs.","#34124CTG Jet engine, Wheaton Aviation"};
int b14blinks[8]		  = {0,99903,97803,97804,0,0,0,0};	
int b14bpoints[4][2] =   {{0,599},{2431,3027},{3046,3594},{1831,2349}/*{1831,2427},{2446,2994},{3061,3579}*/};
int b14bmarks[30][4] = 	{{14,17,17,20},{58,63,61,66},{73,84,77,87},{191,195,194,198},{220,237,223,240},
					 	 {283,302,286,305},{309,312,309,312},{331,340,334,343},{429,446,432,449},
					 	 {520,525,523,528},{554,579,557,582},{-1,-1,-1,-1},

						 {26,56,29,59},{89,111,92,114},{123,144,126,147},{169,180,172,183},{210,216,213,219},											   
						 {241,243,244,246},{254,262,257,265},{274,277,277,280},{352,363,355,366},
						 {382,394,385,397},{415,421,418,424},{454,474,457,477},{495,511,498,514},{538,548,541,551},
						 {-1,-1,-1,-1},											   
			 
						 {58,207,61,210},{556,596,559,599},{-1,-1,-1,-1}};

char b20names [3][64] = {"Source","Benton, Lt. Warren T.","Male 35-45 yrs."};
int b20links[8]		  = {0,99903,97806,0,0,0,0,0};	
int b20points[3][2] =   {{0,356},{736,1091},{376,694}};
int b20marks[15][4] = 	{{20,25,23,28},{50,90,53,93},{184,187,187,190},{196,217,199,220},{260,284,263,287},
					 	 {304,309,307,312},{318,321,321,324},{332,342,335,345},{-1,-1,-1,-1},

						 {27,39,30,42},{94,147,97,150},{155,180,158,183},{242,247,245,250},{287,297,290,300},											   
						 {-1,-1,-1,-1}};

char m14names [3][64] = {"Source","Magruder, Dr. Kate","Male 35-45 yrs."};
int m14links[8]		  = {0,99905,97814,0,0,0,0,0};	
int m14points[3][2] =   {{0,515},{1066,1581},{541,1044}};
int m14marks[21][4] = 	{{5,11,8,14},{69,110,72,113},{173,196,176,199},{245,253,248,256},{311,323,314,326},
					 	 {434,440,437,443},{465,481,468,484},{498,508,501,511},{-1,-1,-1,-1},

						 {21,36,24,39},{48,63,51,66},{119,124,122,127},{133,171,136,174},{206,243,209,246},											   
						 {268,310,271,313},{339,375,342,378},{381,411,384,414},{417,433,420,435},{452,456,455,459},											   
						 {492,496,495,499},{-1,-1,-1,-1}};

char m28names [4][64] = {"Source","Magruder, Dr. Kate","Female 35-45 yrs.","#3129FSW Fire Alarm, Securetech Systems"};
int m28links[8]		  = {0,99905,97815,97816,0,0,0,0};	
int m28points[4][2] =   {{0,210},{706,915},{946,1149},{1171,1374}};
int m28marks[15][4] = 	{{5,11,8,14},{41,59,44,62},{146,151,149,154},{155,161,158,164},{180,196,183,199},{-1,-1,-1,-1},

						 {13,39,16,42},{61,80,64,83},{87,100,90,103},{105,136,108,139},{167,175,170,178},											   
						 {198,201,201,204},{-1,-1,-1,-1},

						 {156,203,159,206},{-1,-1,-1,-1}};

char s19names [3][64] = {"Source","Female 20-30 yrs.","Sung, Dr. Lee Tae"};
int s19links[8]		  = {0,97817,99907,0,0,0,0,0};	
int s19points[3][2] =   {{0,342},{361,699},{721,1062}};
int s19marks[13][4] = 	{{19,28,22,31},{43,47,46,50},{50,86,53,89},{113,152,116,155},{202,245,205,248},
						 {252,323,255,326},{-1,-1,-1,-1},

						 {7,17,10,20},{38,41,41,44},{104,111,107,114},{174,201,177,204},{323,335,326,338},											   
						 {-1,-1,-1,-1}};

char s22names [3][64] = {"Source","Sung, Dr. Lee Tae","Male 20-30 yrs."};
int s22links[8]		  = {0,99907,97818,0,0,0,0,0};	
int s22points[3][2] =   {{0,379},{796,1174},{406,779}};
int s22marks[16][4] = 	{{6,13,9,16},{50,110,53,113},{174,215,177,218},{250,254,253,257},{264,284,267,287},
						 {335,349,338,352},{357,359,360,362},{-1,-1,-1,-1},

						 {15,19,15,19},{21,28,24,31},{112,131,115,134},{145,163,148,166},{217,230,220,233},											   
						 {286,305,289,308},{313,321,317,324},{-1,-1,-1,-1}};
									  
char bvr15names [3][64] ={"Source","Male 25-35 yrs.","Benton, Lt. Warren T."};
int bvr15links[8]	  = {0,97819,99903,0,0,0,0,0};		   
int bvr15points[3][2] = {{0,41},{61,104},{121,164}};
int bvr15marks[5][4] ={{0,21,3,24},{-1,-1,-1,-1},
						 {2,12,5,15},{21,38,24,41},{-1,-1,-1,-1}};
														 
char cvr23names [4][64] ={"Source","Cohen, Dr. Darren M.","Unknown Distortion","Plastic Button Depression"};
int cvr23links[8]		  = {0,99902,0,97820,0,0,0,0};	
int cvr23points[4][2] = {{0,44},{481,519},{301,349},{241,284}};
int cvr23marks[7][4] ={{4,35,7,38},{-1,-1,-1,-1},
					   {0,39,3,42},{-1,-1,-1,-1},
					   {0,2,0,2},{38,42,38,42},{-1,-1,-1,-1}};

char birdnames [7][64] = {"Source","Foster, Maxine M.","Birdsong","10 Ton church bell","1L176 Passenger Aircraft","Uzbeki Mtn. Sparrow","#34 Electric Tram, Dzerzinsky Ltd."/*,"Unknown Voices. Armenian Dialect"*/};
int birdlinks[7]		  = {97835,99020,99403,97830,97831,97832,97833/*,97834*/};	
int birdpoints[7][2] = {{0,589},{601,1039},{1051,1529},{1546,1724},{1741,2079},{2341,2914},{2101,2329}/*,{2925,3469}*/};

int birdmarks[29][4] = {{12,15,15,18},{23,29,26,32},{38,62,41,65},{74,81,77,84},{136,155,139,158},{243,259,246,262},
					 	 {268,283,271,286},{414,433,417,436},{-1,-1,-1,-1},

						 {93,109,96,112},{119,134,122,137},{169,191,172,194},{198,234,201,237},{294,328,297,331},											   
						 {336,370,339,373},{379,403,382,406},{435,440,438,443},{457,483,460,486},{501,541,504,544},											   
						 {561,572,564,575},{-1,-1,-1,-1},			

						 {8,187,11,190},{-1,-1,-1,-1},			
						 {8,580,11,583},{-1,-1,-1,-1},			
						 {8,344,11,347},{-1,-1,-1,-1},			
						 {161,401,164,404},{-1,-1,-1,-1}/*,			
						 {9,498,12,501},{-1,-1,-1,-1}*/};

/*********************
*
*		Page CODE
*												  
**********************/

Sndanal::Sndanal(int number)
{
	name = "Sndanal";
	currentFocus = PUZZLE_FOCUS;				 
	puzzle = number;
	sfxSystemCursor(SYS_ARROW);
	GameFlag.set(fDCK10);
	anal = this;
	user->input = false;
	sfxLoadRes(650,RES_ATS);
	sfxLoadRes(651,RES_ATS);
	sfxLoadRes(652,RES_ATS);
	sfxLoadRes(653,RES_ATS);
	sfxLockRes(650,RES_ATS);
	sfxLockRes(651,RES_ATS);
	sfxLockRes(652,RES_ATS);
	sfxLockRes(653,RES_ATS);
	new (PuzzBack);
	memset(&tabs,0,(8 * (sizeof(Tab*))));
	memset(&marks,-1,(400 * (sizeof(int))));
	memset(&names,0,(8 * (sizeof(char*))));
	memset(&links,0,(8 * (sizeof(int))));
	memset(&namesPoints,0,(16 * (sizeof(int))));
	memset(&moviename,0,(256 * (sizeof(char))));
	playing = false;
	lastloc = 0;
	inbutt = new InButt;
	playbutt = new PlayButt;
	stopbutt = new StopButt;
	new Speaker;	
	if (number)
		new Cover;
	else
		mapbutt = new MapButt;
	
	outclick = false;
	curTab = 0;
	new CanButt;
	new LtButt;
	new RtButt;
	idbutt = new IdButt;
	seeker = new SeekSlider;	
	inmark = 99999;
	anchor = 99999;
	outmark = 99999;
	ledmarks[0] =  new MarkPoint(285,10);
	ledmarks[1] =  new MarkPoint( 87,0);
	ledmarks[2] =  new MarkPoint(105,0);
	ledmarks[3] =  new MarkPoint(123,0);
	ledmarks[4] =  new MarkPoint(141,0);
	ledmarks[5] =  new MarkPoint(159,0);
	ledmarks[6] =  new MarkPoint(177,0);
	ledmarks[7] =  new MarkPoint(195,0);
	ledmarks[8] =  new MarkPoint(213,0);
	ledmarks[9] =  new MarkPoint(231,0);
	ledmarks[10] = new MarkPoint(249,0);
	ledmarks[11] = new MarkPoint(267,0);
	ledmarks[12] = new MarkPoint(285,0);
	ledmarks[13] = new MarkPoint(303,0);
	ledmarks[14] = new MarkPoint(321,0);
	ledmarks[15] = new MarkPoint(339,0);
	ledmarks[16] = new MarkPoint(357,0);
	ledmarks[17] = new MarkPoint(375,0);	    
	ledmarks[18] = new MarkPoint(393,0);
	ledmarks[19] = new MarkPoint(411,0);
	ledmarks[20] = new MarkPoint(429,0);
	ledmarks[21] = new MarkPoint(447,0);
	ledmarks[22] = new MarkPoint(465,0);
									 
	ledmarks[23] = new MarkPoint( 79,1);
	ledmarks[24] = new MarkPoint(482,2);
	range = 0;
	intrpuzz->setScript(new(MovieScript));
}

void Sndanal::init()
{
	//int count = 0;
	char thisanal[16];
	char temp[3];
    char tempPath[256];
    memset(&tempPath,0,256);
	memset(&temp,0,3);
	memset(&thisanal,0,16);
	Common::strcat_s(thisanal,"sndinfo");	
	Common::sprintf_s(temp, "%d", puzzle);

	Common::strcat_s(thisanal,temp);

	webreport->hide();

	if(intrpuzz->isStateSaved(thisanal))
	{
		sndinfo[puzzle]->load();
	}		
	else
	{
		specific();
		
		tabs[0] = new Tab;
		Common::strcpy_s(tabs[0]->label,names[0]);
		tabs[0]->init(40,268,madeRGB(21,194,0));				
		tabs[0]->slot = 0;
		tabs[0]->in = namesPoints[0][0];
		tabs[0]->out = namesPoints[0][1];
		tabs[0]->pos = tabs[0]->in;
		tabs[0]->clicked = true;
		tabs[0]->highlighted = true;
		tabs[0]->myPrint->highlight(true);
		if (!puzzle)
		{
			tabs[0]->doss = new LinkButt;
			tabs[0]->doss->init(24,268);				
			tabs[0]->doss->linkto = tabs[0]->linkto = links[0];
			new CommandLine(tabs[0]->doss->left+1,tabs[0]->doss->top+1,tabs[0]->doss->width-2,tabs[0]->doss->height-2,
							"More Info");
			new CommandLine(anal->tabs[0]->left,anal->tabs[0]->top,240,9,"Click Red Folder Icon for More Info");

		}
#ifdef DIRECTX
   	sfxGetDir(tempPath, atoi(moviename), RES_MOVIE);
	if (sounds->size)	{
		if (sound1->isPlaying())	{
			sound1->stop();
		}
		Node *n = sounds->head;
		SoundObject* obj;
		while ( n ) {
			obj = (SoundObject*)n->data;
			n = n->next;
			if ( obj->type == RES_WAVE)	{
				obj->stop();
			}
		}
	}
	if (!soundGone)	{
		soundGone = true;
		sfxDisableSound();
	}
		sfxOpenMovie(tempPath,78,80,1);
		sfxStepMovie(0);
		theMovie->caller = NULL;
		theMovie->stop();
		AddScreen2PicEx (78,80,416,96);
#else
		theMovie->fromTo(0,0);
		theMovie->play((char*)moviename,78,80,416,96,NULL,false,false,false);
#endif
	}
	helppage = 92350;
	range = tabs[curTab]->out - tabs[curTab]->in;
//	IntrPuzzPage::init(WEB_SOUND);
	user->input = true;
	flushMessageCue();
	AddScreen2PicEx (78,80,416,96);
}

Sndanal::~Sndanal()
{
}

void Sndanal::specific(void)
{
	int a = 0;
	names[0] = &permnames[0][0];

	switch (puzzle)
	{
		case 0:
//			GameFlag.set(fGreg13);
			memcpy(&marks,&birdmarks,(116*sizeof(int)));
			for (a=0;a<7;a++)
			{
				names[a] = &birdnames[a][0];
				links[a] = birdlinks[a];
				namesPoints[a][0] = birdpoints[a][0]; 		
				namesPoints[a][1] = birdpoints[a][1];
			}
//			Common::strcat_s(moviename,"650.avi");
			memcpy(&moviename,&movienames[0][0],strlen(movienames[0]));
			report = (WebReport*)new MapSendBut;
			if(!intrpuzz->isStateSaved("mapinfo"))
			{
				new(MapInfo);
			}		
			IntrPuzzPage::init(WEB_SOUND_BIRD);
			break;
		case 1:
			memcpy(&marks,&cy1marks,(100*sizeof(int)));
			for (a=0;a<4;a++)
			{
				names[a] = &cy1names[a][0];
				links[a] = cy1links[a];
				namesPoints[a][0] = cy1points[a][0]; 		
				namesPoints[a][1] = cy1points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99926.avi");
			memcpy(&moviename,&movienames[1][0],strlen(movienames[1]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init( WEB_SOUND_C8);
			break;
		case 2:
			memcpy(&marks,&c13marks,(84*sizeof(int)));
			for (a=0;a<3;a++)
			{
				names[a] = &c13names[a][0];
				links[a] = c13links[a];
				namesPoints[a][0] = c13points[a][0]; 		
				namesPoints[a][1] = c13points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99927.avi");
			memcpy(&moviename,&movienames[2][0],strlen(movienames[2]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_C13);
			break;
		case 3:
			memcpy(&marks,&cy2marks,(48*sizeof(int)));
			for (a=0;a<3;a++)
			{
				names[a] = &cy2names[a][0];
				links[a] = cy2links[a];
				namesPoints[a][0] = cy2points[a][0]; 		
				namesPoints[a][1] = cy2points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99928.avi");
			memcpy(&moviename,&movienames[3][0],strlen(movienames[3]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_C21);
			break;
		case 4:
			memcpy(&marks,&c26marks,(84*sizeof(int)));
			for (a=0;a<3;a++)
			{
				names[a] = &c26names[a][0];
				links[a] = c26links[a];
				namesPoints[a][0] = c26points[a][0]; 		
				namesPoints[a][1] = c26points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99929.avi");
			memcpy(&moviename,&movienames[4][0],strlen(movienames[4]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_C26);
			break;
		case 5:
			memcpy(&marks,&b6marks,(56*sizeof(int)));
			for (a=0;a<3;a++)
			{
				names[a] = &b6names[a][0];
				links[a] = b6links[a];
				namesPoints[a][0] = b6points[a][0]; 		
				namesPoints[a][1] = b6points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99930.avi");
			memcpy(&moviename,&movienames[5][0],strlen(movienames[5]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_B6);				    
			break;										    
		case 6:											    
			memcpy(&marks,&b14marks,(76*sizeof(int)));	    
			for (a=0;a<3;a++)				
			{											    
				names[a] = &b14names[a][0];				   
				links[a] = b14links[a];					   
				namesPoints[a][0] = b14points[a][0]; 	   
				namesPoints[a][1] = b14points[a][1];	   
			}											   
			report = (WebReport*)new InfosciSendButt;
			memcpy(&moviename,&movienames[6][0],strlen(movienames[6]));
//			Common::strcpy_s((char*)moviename,"99931.avi");		   
			IntrPuzzPage::init(WEB_SOUND_B14);			   
			break;										   
		case 7:											   
			memcpy(&marks,&b14bmarks,(120*sizeof(int)));   
			for (a=0;a<4;a++)							   
			{											   
				names[a] = &b14bnames[a][0];			   
				links[a] = b14blinks[a];				   
				namesPoints[a][0] = b14bpoints[a][0]; 		
				namesPoints[a][1] = b14bpoints[a][1];
			}
//			Common::strcpy_s((char*)moviename,"999311.avi");
			memcpy(&moviename,&movienames[7][0],strlen(movienames[7]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_B14B);
			break;
		case 8:
			memcpy(&marks,&b20marks,(60*sizeof(int)));
			for (a=0;a<3;a++)
			{
				names[a] = &b20names[a][0];
				links[a] = b20links[a];
				namesPoints[a][0] = b20points[a][0]; 		
				namesPoints[a][1] = b20points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99932.avi");
			memcpy(&moviename,&movienames[8][0],strlen(movienames[8]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_B20);
			break;
		case 9:
			memcpy(&marks,&m14marks,(84*sizeof(int)));
			for (a=0;a<3;a++)
			{
				names[a] = &m14names[a][0];
				links[a] = m14links[a];
				namesPoints[a][0] = m14points[a][0]; 		
				namesPoints[a][1] = m14points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99933.avi");
			memcpy(&moviename,&movienames[9][0],strlen(movienames[9]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_M14);
			break;
		case 10:
			memcpy(&marks,&m28marks,(60*sizeof(int)));
			for (a=0;a<4;a++)
			{
				names[a] = &m28names[a][0];
				links[a] = m28links[a];
				namesPoints[a][0] = m28points[a][0]; 		
				namesPoints[a][1] = m28points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99934.avi");
			memcpy(&moviename,&movienames[10][0],strlen(movienames[10]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_M28);
			break;
		case 11:
			memcpy(&marks,&s19marks,(52*sizeof(int)));
			for (a=0;a<3;a++)
			{
				names[a] = &s19names[a][0];
				links[a] = s19links[a];
				namesPoints[a][0] = s19points[a][0]; 		
				namesPoints[a][1] = s19points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99936.avi");
			memcpy(&moviename,&movienames[11][0],strlen(movienames[11]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_S19);
			break;
		case 12:
			memcpy(&marks,&s22marks,(64*sizeof(int)));
			for (a=0;a<3;a++)
			{
				names[a] = &s22names[a][0];
				links[a] = s22links[a];
				namesPoints[a][0] = s22points[a][0]; 		
				namesPoints[a][1] = s22points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99935.avi");
			memcpy(&moviename,&movienames[12][0],strlen(movienames[12]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_S22);
			break;
		case 13:
			marks[0][0] = 0;
			marks[0][1] = 42;
			marks[0][2] = 1;
			marks[0][3] = 43;

			names[1] = &permnames[1][0];
//			names[1] = "Cohen, Dr. Darren M.";
			links[1] = 99902;
			namesPoints[0][0] = 0; 		
			namesPoints[0][1] = 42;
			namesPoints[1][0] = 0; 		
			namesPoints[1][1] = 42;

//			Common::strcpy_s((char*)moviename,"99937.avi");
			memcpy(&moviename,&movienames[13][0],strlen(movienames[13]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_VRC5);
			break;
		case 14:
			marks[0][0] = 0;
			marks[0][1] = 41;
			marks[0][2] = 1;
			marks[0][3] = 42;

			names[1] = &permnames[1][0];
//			names[1] = "Cohen, Dr. Darren M.";
			links[1] = 99902;
			namesPoints[0][0] = 0; 		
			namesPoints[0][1] = 42;
			namesPoints[1][0] = 0; 		
			namesPoints[1][1] = 42;

//			Common::strcpy_s((char*)moviename,"99938.avi");
			memcpy(&moviename,&movienames[14][0],strlen(movienames[14]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_VRC9);
			break;
		case 15:
			memcpy(&marks,&cvr23marks,(28*sizeof(int)));
			for (a=0;a<4;a++)
			{
				names[a] = &cvr23names[a][0];
				links[a] = cvr23links[a];
				namesPoints[a][0] = cvr23points[a][0]; 		
				namesPoints[a][1] = cvr23points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99939.avi");		   
			memcpy(&moviename,&movienames[15][0],strlen(movienames[15]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_VRC23);		
			break;										   
		case 16:										   
			marks[0][0] = 0;							   
			marks[0][1] = 43;							   
			marks[0][2] = 1;							   
			marks[0][3] = 44;							   

			names[1] = &permnames[2][0];
//			names[1] = "Benton, Lt. Warren T";
			links[1] = 99903;
			namesPoints[0][0] = 0; 		
			namesPoints[0][1] = 44;
			namesPoints[1][0] = 0; 		
			namesPoints[1][1] = 44;

//			Common::strcpy_s((char*)moviename,"99940.avi");
			memcpy(&moviename,&movienames[16][0],strlen(movienames[16]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_VRB9);
			break;
		case 17:
			memcpy(&marks,&bvr15marks,(20*sizeof(int)));
			for (a=0;a<3;a++)
			{
				names[a] = &bvr15names[a][0];
				links[a] = bvr15links[a];
				namesPoints[a][0] = bvr15points[a][0]; 		
				namesPoints[a][1] = bvr15points[a][1];
			}
//			Common::strcpy_s((char*)moviename,"99941.avi");
			memcpy(&moviename,&movienames[17][0],strlen(movienames[17]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_VRB15);
			break;
		case 18:
			marks[0][0] = 0;
			marks[0][1] = 38;
			marks[0][2] = 1;
			marks[0][3] = 39;

			names[1] = &permnames[3][0];
//			names[1] = "Magruder, Dr. Kate";
			links[1] = 99905;
			namesPoints[0][0] = 0; 		
			namesPoints[0][1] = 38;
			namesPoints[1][0] = 1; 		
			namesPoints[1][1] = 39;

//			Common::strcpy_s((char*)moviename,"99942.avi");
			memcpy(&moviename,&movienames[18][0],strlen(movienames[18]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_VRM5);
			break;
		case 19:
			marks[0][0] = 0;
			marks[0][1] = 33;
			marks[0][2] = 1;
			marks[0][3] = 34;

			names[1] = &permnames[3][0];
//			names[1] = "Magruder, Dr. Kate";
			links[1] = 99905;
			namesPoints[0][0] = 0; 		
			namesPoints[0][1] = 33;
			namesPoints[1][0] = 1; 		
			namesPoints[1][1] = 34;

//			Common::strcpy_s((char*)moviename,"99943.avi");
			memcpy(&moviename,&movienames[19][0],strlen(movienames[19]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_VRM9);
			break;
		case 20:
			marks[0][0] = 0;
			marks[0][1] = 48;
			marks[0][2] = 1;
			marks[0][3] = 49;

			names[1] = &permnames[4][0];
//			names[1] = "Sung, Dr. Lee Tae";
			links[1] = 99907;
			namesPoints[0][0] = 0; 		
			namesPoints[0][1] = 49;
			namesPoints[1][0] = 0; 		
			namesPoints[1][1] = 49;

//			Common::strcpy_s((char*)moviename,"99945.avi");
			memcpy(&moviename,&movienames[20][0],strlen(movienames[20]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_VRS9);
			break;
		case 21:
			marks[0][0] = 0;
			marks[0][1] = 33;
			marks[0][2] = 1;
			marks[0][3] = 34;

			names[1] = &permnames[4][0];
//			names[1] = "Sung, Dr. Lee Tae";
			links[1] = 99907;
			namesPoints[0][0] = 0; 		
			namesPoints[0][1] = 34;
			namesPoints[1][0] = 0; 		
			namesPoints[1][1] = 34;

//			Common::strcpy_s((char*)moviename,"99944.avi");
			memcpy(&moviename,&movienames[21][0],strlen(movienames[21]));
			report = (WebReport*)new InfosciSendButt;
			IntrPuzzPage::init(WEB_SOUND_VRS29);
			break;
	}
}
int Sndanal::handleEvent ( MADEEventStamp *event )
{
	//int count = 0;
	if	(event_type & USER_LEFT_UP)
	{
		if (seeker->buttonDown)
		{
			lastloc += 1;
			seeker->buttonDown = false;
			updateplay();		
			flushMessageCue();
		}
	}
	return false;
}

void Sndanal::doit()
{
	int loc = 0;	
	int diff = 0;
	int diff2 = 0;
	int temp;
	int count = 0;

	if (playing)
	{
		loc = theMovie->getPosn();	
		seeker->step = (((loc - tabs[curTab]->in) * seeker->num_steps)/range);	
		seeker->marker->x = seeker->step + 252;	
		seeker->marker->posn(seeker->marker->x,seeker->marker->y);
	}
	else
	{
		loc = anal->tabs[anal->curTab]->pos;
	}

	if (!seeker->buttonDown)
	{
		if (!curTab && (loc != lastloc))
		{
			if (!anal->curTab)
			{
				for (count=0;count < 25;count++)
				{
					if (!(ledmarks[count]->flag & OBJECT_HIDDEN)) 
						ledmarks[count]->hide();
				}

				if (!outclick && inmark != 99999)
				{
					if (anchor > loc)
					{
						inmark = loc;
						outmark = anchor;
					}
					else if (anchor < loc)
					{
						inmark = anchor;
						outmark = loc;
					}
					else
					{
						outmark = loc;
						inmark = loc;
					}
				}

				if ((outmark != inmark))
				{
					idbutt->textwin->setCel(0);
					idbutt->textwin->show();
					idbutt->setCel(0);

					diff2 = loc - outmark;
					diff = loc - inmark;
				
					if (diff || diff2)
					{
						count = 12 - diff;
						temp = 12 - diff2;
				
						if (count < 1)
						{
							count = 1;
							if (temp > 0)
							{
									ledmarks[23]->show();
							}
						}
						else
						{
							 if (count > 22)
								count = 100;
						}
					
						if (count < 100 && temp > 0)
						{

							if (temp > 23)
							{
								temp = 23;											
								ledmarks[24]->show();
							}

							for (count = count;count < temp;count++)
							{
								ledmarks[count]->show();
							}
						}
					}
					else
					{
						idbutt->textwin->setCel(0);
						idbutt->textwin->hide();
						ledmarks[0]->show();
						idbutt->setCel(2);
					}	
				}
				else
				{
					idbutt->textwin->setCel(0);
					idbutt->textwin->hide();
					if (inmark != 99999)
						ledmarks[0]->show();
					idbutt->setCel(2);
				}
			}
		}
	}
	else
	{
		for (count=0;count < 25;count++)
		{
			if (!(ledmarks[count]->flag & OBJECT_HIDDEN)) 
				ledmarks[count]->hide();
		}
	}
	lastloc = loc;
	IntrPuzzPage::doit();
}

void Sndanal::dispose()
{								   
	int a=0;
	if (web_intrface)
	{
		webreport->show();
		report->dispose();
	}
	discharge();
	for (a=0;a<8;a++)
	{
		if (tabs[a])
			tabs[a]->dispose();
	}											 
	theMovie->caller = NULL;
	theMovie->stop();
	sfxMadeCursor();
	anal = NULL;

#ifdef DIRECTX
	if (soundGone)
	{
		sfxEnableSound();
		soundGone = false;
	}
#endif

	sfxUnlockRes(650,RES_ATS);
	sfxUnlockRes(651,RES_ATS);
	sfxUnlockRes(652,RES_ATS);
	sfxUnlockRes(653,RES_ATS);
	playing = false;
	GameFlag.clear(fDCK10);
	IntrPuzzPage::dispose();		    
}
void Sndanal::updateplay()
{
//	theMovie->pause(false);

#ifdef DIRECTX
      char tempPath[256];
      memset(&tempPath,0,256);
	  sfxGetDir(tempPath, atoi(moviename), RES_MOVIE);
	  if (!soundGone)	
	  {
			if (sounds->size)	{
				if (sound1->isPlaying())	{
					sound1->stop();
				}
				Node *n = sounds->head;
				SoundObject* obj;
				while ( n ) {
					obj = (SoundObject*)n->data;
					n = n->next;
					if ( obj->type == RES_WAVE)	{
						obj->stop();
					}
				}
			}
			soundGone = true;
			sfxDisableSound();
		}
		sfxOpenMovie(tempPath,78,80,1);
	   	sfxStepMovie(tabs[curTab]->pos);
		theMovie->caller = NULL;
		theMovie->stop();
		AddScreen2PicEx (78,80,416,96);
#else
	theMovie->fromTo(tabs[curTab]->pos,tabs[curTab]->pos);
	theMovie->play((char*)moviename,78,80,416,96,NULL,false,false,false);
#endif

	if (!seeker->buttonDown)
	{
		seeker->step = (((tabs[curTab]->pos - tabs[curTab]->in) * seeker->num_steps)/range);	
		seeker->marker->x = seeker->step + 252;	
		seeker->marker->posn(seeker->marker->x,seeker->marker->y);
 	}
	flushMessageCue();
}
void Sndanal::discharge(void) {
	switch (puzzle)
	{														  
		case 0:
			break;
/*
		case 1:
		case 3:
			for (a=0;a<8;a++)
			{
				if (anal->tabs[a])
				{
					if ((anal->tabs[a]->wavnum == 1 && puzzle == 3) ||(anal->tabs[a]->wavnum == 2 && puzzle == 1))
					{
						GameFlag.set(fIDYingVoice);
						break;
					}
				}
			}				  
			break;
*/		default:
			break;
	}
}

void Sndanal::saveState()
{
	char thisanal[16];
	char temp[3];

	memset(&temp,0,3);
	memset(&thisanal,0,16);
	Common::strcat_s(thisanal,"sndinfo");	
	Common::sprintf_s(temp, "%d", puzzle);

	Common::strcat_s(thisanal,temp);

	if(!intrpuzz->isStateSaved(thisanal))
	{
		sndinfo[puzzle] = new (SndInfo);
	}		
	sndinfo[puzzle]->save();
}

PuzzBack::PuzzBack()
{
	name = "backgrnd";
	view = 650;
	loop = 0;
	cel = 0;
	init(0,15);
	noSkip();
	addToPic();
}

/*********************
*
*		Features
*												  
**********************/
Tab::Tab()
{
	name = "                                                           ";
	memset (&label,0,(64*sizeof(char)));
	pos = 0;
	wavnum = 0;
  	highlighted = false;
  	clicked = false;
  	slot = 0;
  	in = 0;
  	out = 0;
	linkto = 0;
	doss = NULL;
}	

void Tab::init(int theX, int theY, int color)
{
	left = theX;
	top = theY;

  	myPrint = new(Print);
  	myPrint->x = theX;									  
  	myPrint->y = theY;
	myPrint->size = 8;
  	myPrint->fore_color = color;//75;//1;     
  	myPrint->high_color = TEXT_COLOR_WHITE;//75;//1;     
	myPrint->style = STYL_PLAIN;
	myPrint->myString = (char*)label;
	myPrint->init();
	right = left + 240;
	bottom = top + 9;
	intrpuzz->add ( this );	 
}
 
void Tab::activate(void)
{
	int count = 0;

	if (!clicked)
	{
		for (count = 0;count<8;count++)
		{
			if(anal->tabs[count])
				anal->tabs[count]->clicked = false;	
		}

		clicked = true;
        if (myPrint)
		  	myPrint->highlight(true);

		if (anal->playing)
		{
#ifdef DIRECTX
//	        pausedanal = true;
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
			theMovie->caller = NULL;
			theMovie->resume();
			theMovie->stop();
			AddScreen2PicEx (78,80,416,96);
#else
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
#endif
			if(anal->tabs[anal->curTab]->pos > anal->tabs[anal->curTab]->in)
				anal->tabs[anal->curTab]->pos -=1;	 

			anal->playing = false;
			anal->playbutt->setCel(0);
			anal->stopbutt->setCel(2);
			anal->report->setCel(0);
		}

		anal->curTab = slot;
		if (pos < in)
			pos = in;

		if (pos >= out)
			pos = in;
		 
		anal->range = out - in;

		anal->updateplay();	

		if (!slot)
		{
			if (anal->anchor == 99999)
			{
				anal->inbutt->setLoop(2);
				anal->inbutt->setCel(0);
			}
			else if (!anal->outclick)
			{
				anal->inbutt->setLoop(3);
				anal->inbutt->setCel(0);

				anal->inbutt->abs->setCel(1);
				anal->inbutt->abs->show();
			}
			else
			{
				anal->inbutt->abs->setCel(2);	
				anal->inbutt->abs->show();
				anal->inbutt->setLoop(2);
				anal->inbutt->setCel(2);
			}
		}
		else
		{
			for (count=0;count < 25;count++)
			{
				if (!(anal->ledmarks[count]->flag & OBJECT_HIDDEN)) 
					anal->ledmarks[count]->hide();
			}
			anal->inbutt->setLoop(2);
			anal->inbutt->setCel(2);
			anal->idbutt->setCel(2);
			anal->inbutt->abs->hide();
		}
	}
}

int Tab::handleEvent(MADEEventStamp* event)
{
  if (!anal->playing)	
  {	
	  if (onMe(event))  
	  {
	    if ((! highlighted) && (! event_type))
	    {
	      highlighted = true;
		  if (myPrint)
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
		  if (!clicked)	
		  {	
			  if (myPrint)
				  myPrint->highlight(false);
			  highlighted = false;
		  }
	  }	
  }	
  return false;

}
/*********************
*
*		BUTTONS
*												  
**********************/

TextWin::TextWin()
{
	name = "idbutt";
	view = 651;
	loop = 9;
	cel = 0;
	init(353,263);
	noSkip();
}

MarkPoint::MarkPoint(int startx, int startcel)
{
	name = "MarkPoint";
	if (startcel == 10)
	{
		view = 651;
		loop = 10;
		cel = 0;
	}
	else		 
	{
		view = 652;
		loop = 0;
		cel = startcel;
	}

	init(startx,177);
	if (startcel != 10)
		noSkip();
	
	hide();
}

LinkButt::LinkButt()
{
	name = "LinkButt";
	view = 652;
	loop = 1;
	cel = 0;
	linkto = 0;
}

void LinkButt::activate(void)								    
{
	int link = linkto;
	if (link)
	{
		anal->stopbutt->activate();
		intrpuzz->page->saveState();
		intrpuzz->dispose();
		new HTMLPage(link);
	}
}

IdButt::IdButt()
{
	name = "idbutt";
	view = 651;
	loop = 0;
	cel = 2;
	textwin = new TextWin;
	go = false;	
	count = 1;
	count2 = 0;
	count3 = 0;
	init(355,331);
	noSkip();
	new CommandLine(left, top, width, height, "Identify selected wave.");
}

void IdButt::activate(void)								    
{
	int a = 0;
	int counter = 1;
	int temp = 0;
	int diff = anal->outmark - anal->inmark;
	count2 = 0;
	count3 = 0;

	go = false;
	
	if (diff > 1 || anal->puzzle == 15)
	{
		for (count3=1;count3<8;count3++)
		{
			if (!anal->tabs[count3])
				break;
		}
	
		while(1)
		{
			while (anal->marks[count2][0] != -1)
			{
				if (anal->inmark >= anal->marks[count2][0] && anal->inmark <= anal->marks[count2][1] 
					&& anal->outmark >= anal->marks[count2][2] && anal->outmark <= anal->marks[count2][3])		
				{
					for (a=1;a<8;a++)
					{
						if (anal->tabs[a])
						{
							if (anal->tabs[a]->wavnum == counter)
							{
								go = true;
								break;
							}
						}
						else
							break;
					}
					if (!go)
					{
						go = true;
						count = counter;
						break;	
					}
					else
					{
						temp = counter + 100;							      
						go = false;
					}
				}
		 		count2++;
			}

			if (counter > 7)
			{
				if (temp)
				{
					go = true;
					count = temp;
				}
				else
					go = false;	
				break;
			}

			if (go)
				break;

			counter ++;
		 	count2++;
		}
	}
	setScript(new (MsgScript));
	setCel(2);
}

void IdButt::doit(void)
{

	if (cel == 2 && !script)
	{
		if (((anal->inmark != 99999 && anal->outmark != 99999) && (anal->inmark != anal->outmark)) && !anal->curTab)
		{
			setCel(0);
//			if(textwin)
//				textwin->hide(); 
		}
	}
	Button::doit();
}

void IdButt::cue(void)
{
	int a = 0;
	
	if (count < 100)
	{
		anal->tabs[count3] = new Tab;
		Common::strcpy_s(anal->tabs[count3]->label,anal->names[count]);
		anal->tabs[count3]->init(40,268+(11*count3) ,madeRGB(21,194,0));				
		anal->tabs[count3]->slot = count3;
		anal->tabs[count3]->in = anal->namesPoints[count][0];
		anal->tabs[count3]->out = anal->namesPoints[count][1]; 
		anal->tabs[count3]->pos = anal->tabs[count3]->in;
		anal->tabs[count3]->wavnum = count;

		if (anal->links[count])
		{
			anal->tabs[count3]->doss = new LinkButt;
			anal->tabs[count3]->doss->init(24,268+(11*count3));				
			anal->tabs[count3]->doss->linkto = anal->links[count];
			anal->tabs[count3]->linkto = anal->links[count];
			new CommandLine(anal->tabs[count3]->doss->left+1,anal->tabs[count3]->doss->top+1,anal->tabs[count3]->doss->width-2,
							anal->tabs[count3]->doss->height-2, "More Info");
			new CommandLine(anal->tabs[count3]->left,anal->tabs[count3]->top,240,9,"Click Red Folder Icon for More Info");
		}
//			anal->tabs[count3]->activate();
	}
	else
	{
		count -= 100;
		for (a=1;a<8;a++)
		{
			if (anal->tabs[a])
			{
				if (anal->tabs[a]->wavnum == count)
				{
//					anal->tabs[a]->activate();
					break;
				}
			}	
		}
	}
											   
	setCel(2);
	anal->inbutt->abs->setCel(0);

	for (a=1;a<8;a++)
	{
		if (anal->tabs[a])
		{
			if ((anal->tabs[a]->wavnum == 1 && anal->puzzle == 3) ||(anal->tabs[a]->wavnum == 2 && anal->puzzle == 1))
			{
				GameFlag.set(fIDYingVoice);
			}
		}	
	}

	if (!anal->puzzle)
	{
		for (a=1;a<8;a++)
		{
			if (anal->tabs[a])
			{
				if (anal->tabs[a]->wavnum >2) 
					anal->mapbutt->setCel(0);

				switch(anal->tabs[a]->wavnum)
				{
					case 4:
						mapinfo->jet = true;
						break;
					case 6:
						mapinfo->car = true;
						break;
					case 5:
						mapinfo->bird = true;
						break;
					case 3:
						mapinfo->church = true;
						break;
/*					case 7:
						mapinfo->arm = true;
						break;
*/				}
			}
			else
				break;
		}
	}
}

InButt::InButt()
{
	name = "inbutt";
	view = 652;
	loop = 2;
	cel = 0;
	init(168,201);
	noSkip();
	new CommandLine(left, top, width, height, "Mark selection.");
	abs = new ABS;
}

void InButt::activate(void)
{
	if (anal->playing)
	{	
#ifdef DIRECTX
//          pausedanal = true;
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
			theMovie->caller = NULL;
			theMovie->resume();
			theMovie->stop();
			AddScreen2PicEx (78,80,416,96);
#else
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
#endif
		anal->playing = false;
		anal->playbutt->setCel(0);
		anal->stopbutt->setCel(2);
		anal->report->setCel(0);
	}

	if (anal->inmark == 99999)
	{
		anal->inmark = anal->tabs[anal->curTab]->pos;
		anal->anchor = anal->inmark;

		if (anal->inmark < anal->tabs[anal->curTab]->in)
			anal->inmark = anal->tabs[anal->curTab]->in;

		anal->ledmarks[0]->show();
		setLoop(3);
		abs->setCel(1);
		abs->show();
	}
	else
	{
		if (anal->anchor != anal->tabs[anal->curTab]->pos)
		{
			setLoop(2);
			setCel(2);
			abs->setCel(2);	
			anal->outclick = true;
		}
	}
}
Cover::Cover()
{
	name = "cover";
	view = 653;
	loop = 0;
	cel = 0;
	init(434,316);
	noSkip();
}
ABS::ABS()
{
	name = "abs";
	view = 652;
	loop = 6;
	cel = 0;
	init(23,152);
	noSkip();
	hide();
}

Speaker::Speaker()
{
	name = "speaker";
	looping = false;
	timer = NULL;
	view = 652;
	loop = 7;
	cel = 1;
	init(23,90);
	noSkip();
	hide();
}
void Speaker::doit(void)
{
	if (anal->playing)
	{
		if (!looping)
		{
			if (!cycler)
			{	
				looping = true;
				setCel(1);
				setCycle(new EndLoop((Prop*)this, this));
				show();
			}
		}	  
	}
	else		  
	{
		if (cycler)
		{
			setCycle(NULL); 
			setCel(1);
			hide();
			looping = false;
			if (timer)
			{
				timer->dispose();
				timer = NULL;
			}	
		}											    
	}
	IntrPuzzProp::doit();
}
void Speaker::cue(void)
{
	if (!timer)
	{
		setCel(4);
		timer = new IntrPuzzTimer;
		timer->setTicks(8,this);
		return;
	}
	else
	{
		timer->dispose();
		timer = NULL;
	}	

	setCel(1);
	if (anal->playing)
		setCycle(new EndLoop((Prop*)this, this));
	else
	{
		looping = false;
		hide();	
	}
}

CanButt::CanButt()
{
	name = "canbutt";
	view = 652;
	loop = 4;
	cel = 0;
	init(192,201);
	noSkip();
	new CommandLine(left, top, width, height, "Clear Marks.");
}

void CanButt::activate(void)
{
	int count = 0;
	anal->inmark = 99999;
	anal->outmark = 99999;
	anal->anchor = 99999;
	anal->outclick = false;
 	for (count = 0;count <25; count++)
	{
		if (!(anal->ledmarks[count]->flag & OBJECT_HIDDEN)) 
			anal->ledmarks[count]->hide();
	}
	if (!anal->curTab)
	{
		anal->inbutt->setLoop(2);
		anal->inbutt->setCel(0);
	}
	anal->idbutt->textwin->setCel(0);
	anal->idbutt->textwin->hide();
	anal->idbutt->setCel(2);
	anal->inbutt->abs->setCel(0);
	anal->inbutt->abs->hide();
	anal->tabs[anal->curTab]->activate();
}

MapButt::MapButt()
{
	name = "mapbutt";
	view = 651;
	loop = 2;
	cel = 2;
	init(443,331);					   
	noSkip();
	new CommandLine(left, top, width, height, "Import data into mapping tool.");
}

void MapButt::activate(void)
{
	anal->saveState();
	intrpuzz->dispose();
	new MapTool;
}
							 

PlayButt::PlayButt()
{
	name = "playbutt";
	view = 651;
	loop = 5;
	cel = 0;
	init(58,201);								 
	noSkip();
	new CommandLine(left, top, width, height, "Play selected wave.");
}

void PlayButt::activate(void)
{
#ifdef DIRECTX
   char tempPath[256];
   memset(&tempPath,0,256);
#endif
	if (!anal->playing)
	{
		if (anal->tabs[anal->curTab]->pos >= anal->tabs[anal->curTab]->out)
		{
			anal->tabs[anal->curTab]->pos = anal->tabs[anal->curTab]->in;
		}
		anal->playing = true;
		anal->report->setCel(2);
#ifdef DIRECTX
      if (!theMovie->isOpen)
      {
      	sfxGetDir(tempPath, atoi(anal->moviename), RES_MOVIE);
/*	if (sounds->size)	{
		if (sound1->isPlaying())	{
			sound1->stop();
		}
		Node *n = sounds->head;
		SoundObject* obj;
		while ( n ) {
			obj = (SoundObject*)n->data;
			n = n->next;
			if ( obj->type == RES_WAVE)	{
				obj->stop();
			}
		}
	}
	if (!soundGone)	{
		soundGone = true;
		sfxDisableSound();
	}
*/   		sfxOpenMovie(tempPath,78,80,1);
      }
      SetMovieErase (false);
#endif
		theMovie->fromTo(anal->tabs[anal->curTab]->pos,anal->tabs[anal->curTab]->out);

#ifdef DIRECTX
		theMovie->play((char*)anal->moviename,78,80,416,96,this,false,-1,false);
#else
		theMovie->play((char*)anal->moviename,78,80,416,96,this,false,false,false);
#endif
		setCel(2);
		anal->stopbutt->setCel(0);
	}
}

void PlayButt::cue(void)
{
#ifdef DIRECTX
/* if (!pausedanal)
   {
   	anal->playing = false;
   	anal->tabs[anal->curTab]->pos = anal->tabs[anal->curTab]->out;
   	setCel(0);
   	anal->stopbutt->setCel(2);
   	anal->report->setCel(0);
   }
   pausedanal = false;
*/
#else
#endif
	anal->playing = false;
	anal->tabs[anal->curTab]->pos = anal->tabs[anal->curTab]->out;
	setCel(0);
	anal->stopbutt->setCel(2);
	anal->report->setCel(0);
	theMovie->caller = NULL;
	theMovie->stop();
#ifdef DIRECTX
	AddScreen2PicEx (78,80,416,96);
#endif
}

StopButt::StopButt()
{
	name = "stopbutt";
	view = 651;
	loop = 8;
	cel = 2;
	init(28,201);
	noSkip();
	new CommandLine(left, top, width, height, "Stop selected wave.");
}

void StopButt::activate(void)
{
	if (anal->playing)
	{	
#ifdef DIRECTX
//	        pausedanal = true;
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
			theMovie->caller = NULL;
			theMovie->resume();
			theMovie->stop();
			AddScreen2PicEx (78,80,416,96);
#else
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
#endif
		anal->playing = false;
		anal->playbutt->setCel(0);
		anal->stopbutt->setCel(2);
		anal->report->setCel(0);
	}
}

LtButt::LtButt()
{
	name = "ltbutt";
	view = 651;
	loop = 6;
	cel = 0;
	init(87,201);
	noSkip();
	new CommandLine(left, top, width, height, "Step wave backwards.");
}

void LtButt::activate(void)
{
	if (anal->playing)
	{	
#ifdef DIRECTX
//	        pausedanal = true;
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
			theMovie->caller = NULL;
			theMovie->resume();
			theMovie->stop();
			AddScreen2PicEx (78,80,416,96);
#else
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
#endif
		anal->playing = false;
		anal->playbutt->setCel(0);
		anal->stopbutt->setCel(2);
		anal->report->setCel(0);
	}

	if ((anal->tabs[anal->curTab]->pos) > anal->tabs[anal->curTab]->in)
	{
		anal->tabs[anal->curTab]->pos -= 1;
		anal->updateplay();	
	}
}
void LtButt::doit(void)
{
	if (cel ==1 && !script)
		setScript(new DelayButtScript);
/*
	if (cel == 1)
	{
		if (anal->playing)
		{	
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
			anal->playing = false;
			anal->playbutt->setCel(0);
			anal->stopbutt->setCel(2);
			anal->report->setCel(0);

		}

		if ((anal->tabs[anal->curTab]->pos - 1)> anal->tabs[anal->curTab]->in)
		{
			anal->tabs[anal->curTab]->pos -= 1;
			anal->updateplay();	
		}
	}
*/
	Button::doit();
}

RtButt::RtButt()
{
	name = "rtbutt";
	view = 651;
	loop = 7;
	cel = 0;
	init(115,201);
	noSkip();
	new CommandLine(left, top, width, height, "Step wave forwards.");
}

void RtButt::activate(void)
{
	if (anal->playing)
	{	
#ifdef DIRECTX
//          pausedanal = true;
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
			theMovie->caller = NULL;
			theMovie->resume();
			theMovie->stop();
			AddScreen2PicEx (78,80,416,96);
#else
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
#endif
		anal->playing = false;
		anal->playbutt->setCel(0);
		anal->stopbutt->setCel(2);
		anal->report->setCel(0);
	}

	if (anal->tabs[anal->curTab]->pos < anal->tabs[anal->curTab]->out)
	{
		anal->tabs[anal->curTab]->pos += 1;
		anal->updateplay();
	}
}																															   
void RtButt::doit(void)
{
	if (cel ==1 && !script)
		setScript(new DelayButtScript);
/*
	if (cel == 1)
	{
		if (anal->playing)
		{	
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
			anal->playing = false;
			anal->playbutt->setCel(0);
			anal->stopbutt->setCel(2);
			anal->report->setCel(0);
		}

		if (anal->tabs[anal->curTab]->pos < anal->tabs[anal->curTab]->out)
		{
			anal->tabs[anal->curTab]->pos += 1;
			anal->updateplay();
		}
	}
*/
	Button::doit();
}																															   
/***********************************
 *		
 *		 SLIDER
 *		      
 **********************************/
SeekSlider::SeekSlider()
{
	init(252,195,226,28,651,4);
	num_steps = 226;
}														 
int SeekSlider::handleEvent(MADEEventStamp* event)
{
	if (onMe(event))  
	{
		if (event_type & USER_LEFT_UP)	//clear the slider button down
		{
			anal->lastloc += 1;
			buttonDown = false;
			anal->updateplay();
			flushMessageCue();
			return true;
		}													 
		
		if (event_type & USER_LEFT_DOWN)
		{
			buttonDown = true;
			if (anal->playing)
			{
#ifdef DIRECTX
//          pausedanal = true;
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
			theMovie->caller = NULL;
			theMovie->resume();
			theMovie->stop();
			AddScreen2PicEx (78,80,416,96);
#else
			theMovie->pause(false);
			anal->tabs[anal->curTab]->pos = theMovie->getPosn();	
#endif
				anal->playing = false;
				anal->playbutt->setCel(0);
				anal->stopbutt->setCel(2);
				anal->report->setCel(0);
			}																			 
		}
		if(buttonDown)									 
		{ 
			marker->x = event->cursor_pt.x;	
			marker->posn(marker->x,marker->y);
			step = (marker->x - left);

			anal->tabs[anal->curTab]->pos = anal->tabs[anal->curTab]->in + (step*anal->range/num_steps);

			if (anal->tabs[anal->curTab]->pos > anal->tabs[anal->curTab]->out)
				anal->tabs[anal->curTab]->pos = anal->tabs[anal->curTab]->out;
		}
	}
	else
	{
		if (buttonDown)
		{
			anal->lastloc += 1;
			anal->updateplay();
			flushMessageCue();
			buttonDown = false;
			return true;
		}
	}
	return (false);	
}

/***********************************
 *		
 *		 Puzzle Save Status
 *		      
 **********************************/
SndInfo::SndInfo()
{
	static char sndName[16];
	char temp[3];
	memset(&temp,0,3);

	Common::sprintf_s(sndName,"sndinfo%d", anal->puzzle);	
	name = sndName;

	memset(&tabs,0,(8*sizeof(Tab)));
	lastloc = 0;
	puzzle = 0;
	inmark = 99999;
	outmark = 99999;
	anchor = 99999;
	showin = 0;
	curTab = 0;
	memset (&moviename,0,(256*sizeof(char)));
	markerx = 0;
	seekerstep = 0;
	web_intrface->addToFront(this); 
	sndinfo[anal->puzzle] = this;
}

void SndInfo::save()
{
	int a=0;
	for (a=0;a<8;a++)
	{
		if (anal->tabs[a])
			memcpy(&tabs[a],anal->tabs[a],sizeof(Tab));
		else
		{
			memset(&tabs[a],0,sizeof(Tab));
		}
	}
	lastloc = anal->lastloc;
	puzzle = anal->puzzle;
	inmark = anal->inmark;
	outmark = anal->outmark;
	anchor = anal->anchor;

	curTab = anal->curTab;
	Common::strcpy_s(moviename, anal->moviename);
	markerx = anal->seeker->marker->x;
	seekerstep = anal->seeker->step;

}	  

void SndInfo::load()
{
	int a=0;

	anal->puzzle = puzzle;

	anal->specific();

	for (a=0;a<8;a++)
	{
		if (((Tab*)&tabs[a])->out)
		{
			anal->tabs[a]=new Tab;
			memcpy(anal->tabs[a],&tabs[a],sizeof(Tab));
			anal->tabs[a]->init(40,(268+(11*a)),madeRGB(21,194,0));
			if (anal->tabs[a]->highlighted)
				anal->tabs[a]->myPrint->highlight(true);
			if (anal->tabs[a]->doss)
			{
				anal->tabs[a]->doss = new LinkButt;
				anal->tabs[a]->doss->init(24,268+(11*a));				
				anal->tabs[a]->doss->linkto = anal->tabs[a]->linkto;
				new CommandLine(anal->tabs[a]->doss->left+1,anal->tabs[a]->doss->top+1,
								anal->tabs[a]->doss->width-2,anal->tabs[a]->doss->height-2, "More Info");
				new CommandLine(anal->tabs[a]->left,anal->tabs[a]->top,240,9, "Click Red Folder Icon for More Info");
			}
		}
		else
		{
			anal->tabs[a] = NULL;
		}
	}

	anal->lastloc = lastloc - 1;
	anal->inmark = inmark;
	anal->outmark = outmark;
	anal->anchor = anchor;
	if (!anal->puzzle)
	{
		for (a=1;a<8;a++)
		{
			if (anal->tabs[a])
			{
				if (anal->tabs[a]->wavnum >2) 
					anal->mapbutt->setCel(0);
			}
		}
	}
	anal->curTab = curTab;
	if (curTab)
	{
		anal->inbutt->setLoop(2);
		anal->inbutt->setCel(2);
	}

	Common::strcpy_s(anal->moviename, moviename);
	anal->range = anal->tabs[curTab]->out - anal->tabs[curTab]->in;
	anal->updateplay();
}

void SndInfo::dispose()
{
	web_intrface->release ( this );
	deathList->add(this);
	sndinfo[puzzle] = NULL;
}
void MovieScript::changeState ( int newState )
{
	switchTo
		ticks=15;
	END
	BEG
		anal->init();
		intrpuzz->setScript(NULL);
	END
}
void RedrawScript::changeState ( int newState )
{
	switchTo
		ticks=15;
	END
	BEG
		anal->updateplay();
		intrpuzz->setScript(NULL);
	END
}
void DelayButtScript::changeState ( int newState )
{
	switchTo
		ticks=15;
	END
	BEG
		if (((Button*)client)->cel == 1)
		{
			((Button*)client)->activate();			 
			((Button*)client)->setScript(new DelayButtScript); 			
		}
		else
			((Button*)client)->setScript(NULL); 			
	END
}
void MsgScript::changeState ( int newState )
{
	int count = 0;
	switchTo
		((IdButt*)client)->textwin->show();
		((IdButt*)client)->textwin->setCel(1);
		ticks = 20;
	END
	BEG
		((IdButt*)client)->textwin->hide();
		ticks = 20;
	END
    BEG
		((IdButt*)client)->textwin->show();
		ticks = 20;
    END
	BEG
		((IdButt*)client)->textwin->hide();
		ticks = 20;
	END
    BEG
		((IdButt*)client)->textwin->show();
		ticks = 20;
    END
	BEG
		((IdButt*)client)->textwin->hide();
		ticks = 20;
	END
	BEG
		((IdButt*)client)->textwin->show();
		((IdButt*)client)->textwin->setCel(2);
		ticks = 20;
	END
	BEG
		((IdButt*)client)->textwin->hide();
		ticks = 20;
	END
    BEG
		((IdButt*)client)->textwin->show();
		ticks = 20;
    END
	BEG
		((IdButt*)client)->textwin->hide();
		ticks = 20;
	END
    BEG
		((IdButt*)client)->textwin->show();
		ticks = 20;
    END
	BEG
		((IdButt*)client)->textwin->hide();
		ticks = 20;
	END
    BEG
		((IdButt*)client)->textwin->show();
		ticks = 20;
    END
	BEG
		((IdButt*)client)->textwin->hide();
		ticks = 20;
	END
	BEG
		((IdButt*)client)->textwin->show();

		anal->inmark = 99999;
		anal->outmark = 99999;
		anal->anchor = 99999;
		anal->outclick = false;
		anal->inbutt->setLoop(2);
		anal->inbutt->setCel(0);
		for (count = 0;count <24; count++)
		{
			if (!(anal->ledmarks[count]->flag & OBJECT_HIDDEN)) 
				anal->ledmarks[count]->hide();
		}
		if (((IdButt*)client)->go)
		{
			((IdButt*)client)->textwin->setCel(3);
			((IdButt*)client)->cue();			
			((IdButt*)client)->setScript(NULL);
	 	}
		else
		{
			((IdButt*)client)->setCel(2);
			((IdButt*)client)->textwin->setCel(4);
			((IdButt*)client)->setScript(NULL);
		}
	END
}

} // namespace Spycraft
