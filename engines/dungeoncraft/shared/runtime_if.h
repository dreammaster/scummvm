/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

namespace DungeonCraft {

extern "C" /* disable C++ name decorations */
{ 

#define MAX_KEYWORD_TEXT_LEN 50
#define MAX_TYPE_TEXT_LEN 50

#define NULL_KEYWORD 0

enum
{
  /* character keywords */
  CHAR_BASE=0,
  CHAR_NAME=1,
  CHAR_AC=2,
  CHAR_HITPOINTS=3,
  CHAR_THACO=4,
  CHAR_RDYTOTRAIN=5,
  CHAR_CLERICEXP=6,
  CHAR_MAGICUSEREXP=7,
  CHAR_FIGHTEREXP=8,
  CHAR_RANGEREXP=9,
  CHAR_THIEFEXP=10,
  CHAR_DRUIDEXP=11,
  CHAR_PALADINEXP=12,
  CHAR_AGE=13,
  CHAR_MAXAGE=14,
  CHAR_MAXMOVE=15,
  CHAR_STR=16,
  CHAR_STRMOD=17,
  CHAR_INT=18,
  CHAR_WIS=19,
  CHAR_DEX=20,
  CHAR_CON=21,
  CHAR_CHA=22,
  CHAR_MAXENC=23,
  CHAR_ENC=24,
  CHAR_GENDER=25,
  CHAR_CLASS=26,
  CHAR_ALIGNMENT=27,
  CHAR_STATUS=28,
  CHAR_UNDEAD=29,
  CHAR_SIZE=30,
  CHAR_MAGICRESIST=31,
  CHAR_SAVEVSPPDM=32,
  CHAR_SAVEVSPP=33,
  CHAR_SAVEVSRSW=34,
  CHAR_SAVEVSBR=35,
  CHAR_SAVEVSSP=36,
  CHAR_CLERICLVL=37,
  CHAR_FIGHTERLVL=38,
  CHAR_RANGERLVL=39,
  CHAR_DRUIDLVL=40,
  CHAR_PALADINLVL=41,
  CHAR_THIEFLVL=42,
  CHAR_MAGUSERLVL=43,
  CHAR_CLERICPREVLVL=44,
  CHAR_FIGHTERPREVLVL=45,
  CHAR_RANGERPREVLVL=46,
  CHAR_DRUIDPREVLVL=47,
  CHAR_PALADINPREVLVL=48,
  CHAR_THIEFPREVLVL=49,
  CHAR_MAGUSERPREVLVL=50,
  CHAR_CLERICPDLVL=51,
  CHAR_FIGHTERPDLVL=52,
  CHAR_RANGERPDLVL=53,
  CHAR_DRUIDPDLVL=54,
  CHAR_PALADINPDLVL=55,
  CHAR_THIEFPDLVL=56,
  CHAR_MAGUSERPDLVL=57,
  CHAR_NBRHITDICE=58,
  CHAR_NBRATTACKS=59,
  CHAR_MORALE=60,
  CHAR_OPENDOORS=61,
  CHAR_OPENMAGICDOORS=62,
  CHAR_BENDLIFT=63,
  CHAR_PICKPOCKETS=64,
  CHAR_OPENLOCKS=65,
  CHAR_FINDTRAPS=66,
  CHAR_MOVESILENT=67,
  CHAR_HIDESHADOWS=68,
  CHAR_HEARNOISE=69,
  CHAR_CLIMBWALLS=70,
  CHAR_READLANG=71,
  CHAR_ALLOWPLAYERCONTROL=72,
  CHAR_DETECTMAGIC=73,
  CHAR_DETECTINVISIBLE=74,
  CHAR_DETECTTRAPS=75,
  CHAR_DAMAGEBONUS=76,
  CHAR_BLESS=77,
  CHAR_CURSE=78,
  CHAR_UNDEADFEAR=79,
  CHAR_ENLARGE=80,
  CHAR_REDUCE=81,
  CHAR_CHARMPERSON=82,
  CHAR_REFLECTGAZEATTACK=83,
  CHAR_PROTFROMEVIL=84,
  CHAR_PROTFROMGOOD=85,
  CHAR_SHIELD=86,
  CHAR_SLEEP=87,
  CHAR_FOG=88,
  CHAR_ENTANGLE=89,
  CHAR_INVISIBLETOANIMALS=90,
  CHAR_INVISIBLETOUNDEAD=91,
  CHAR_NONUNDEADFEAR=92,
  CHAR_SANCTUARY=93,
  CHAR_SHILLELAGH=94,
  CHAR_DISPLACEMENT=95,
  CHAR_WIZARDRY=96,
  CHAR_CHARTYPE=97,
  CHAR_VORPALATTACK=98,
  CHAR_COINAMOUNT1=99,
  CHAR_COINAMOUNT2=100,
  CHAR_COINAMOUNT3=101,
  CHAR_COINAMOUNT4=102,
  CHAR_COINAMOUNT5=103,
  CHAR_COINAMOUNT6=104,
  CHAR_COINAMOUNT7=105,
  CHAR_COINAMOUNT8=106,
  CHAR_COINAMOUNT9=107,
  CHAR_COINAMOUNT10=108,
  CHAR_GEMAMOUNT=109,
  CHAR_JEWELRYAMOUNT=110,
  CHAR_HOLDPERSON=111,
  CHAR_SILENCE=112,
  CHAR_ISMAMMAL=113,
  CHAR_ISANIMAL=114,
  CHAR_ISSNAKE=115,
  CHAR_ISGIANT=116,
  CHAR_ISALWAYSLARGE=117,
  CHAR_HASDWARFACPENALTY=118,
  CHAR_HASGNOMEACPENALTY=119,
  CHAR_HASDWARFTHACOPENALTY=120,
  CHAR_HASGNOMETHACOPENALTY=121,
  CHAR_HASRANGERDMGPENALTY=122,
  CHAR_HASPOISONIMMUNITY=123,
  CHAR_HASDEATHIMMUNITY=124,
  CHAR_HASCONFUSIONIMMUNITY=125,
  CHAR_HASVORPALIMMUNITY=126,
  CHAR_CANBEHELDORCHARMED=127,
  CHAR_AFFECTEDBYDISPELEVIL=128,
  CHAR_POISONED=129,
  CHAR_SLOWPOISON=130,
  CHAR_MIRRORIMAGE=131,
  CHAR_INVISIBLE=132,
  CHAR_ENFEEBLED=133,
  CHAR_HITBONUS=134,
  CHAR_BLINDNESS=135,
  CHAR_DISEASED=136,
        
  /* party values */
  PARTY_BASE=512,
  PARTY_FACING=513,
  PARTY_DAYS=514,
  PARTY_HOURS=515,
  PARTY_MINUTES=516,
  PARTY_ACTIVECHAR=517,
  PARTY_MONEYAVAILABLE=518,

  /* game values */
  GAME_BASE=1024,
  GAME_CURRLEVEL=1025,
  GAME_VERSION=1026,
  GAME_COINNAME1=1027,
  GAME_COINNAME2=1028,
  GAME_COINNAME3=1029,
  GAME_COINNAME4=1030,
  GAME_COINNAME5=1031,
  GAME_COINNAME6=1032,
  GAME_COINNAME7=1033,
  GAME_COINNAME8=1034,
  GAME_COINNAME9=1035,
  GAME_COINNAME10=1036,
  GAME_GEMNAME=1037,
  GAME_JEWELRYNAME=1038,
  GAME_VAULTMONEYAVAILABLE=1039
};

#define FIRST_CHAR_KEYWORD CHAR_NAME
#define LAST_CHAR_KEYWORD  CHAR_DISEASED

#define FIRST_PARTY_KEYWORD PARTY_FACING
#define LAST_PARTY_KEYWORD  PARTY_MONEYAVAILABLE

#define FIRST_GAME_KEYWORD GAME_CURRLEVEL
#define LAST_GAME_KEYWORD  GAME_VAULTMONEYAVAILABLE

#define PUBLIC extern

#define IF_TYPE_STRING 0
#define IF_TYPE_BYTE   1
#define IF_TYPE_WORD   2
#define IF_TYPE_DWORD  3
#define IF_TYPE_FLOAT  4
#define MAX_IF_TYPES   5

#define IF_TYPE_UBYTE   129
#define IF_TYPE_UWORD   130
#define IF_TYPE_UDWORD  132


/* converts "$CHAR_NAME" to enum CHAR_NAME, which
   can be used directly by the following functions */
PUBLIC unsigned long KeywordTextToNum(const char *keyword);
PUBLIC const char *NumToKeywordText(const unsigned long keyword);

PUBLIC unsigned char GetDataSize(const unsigned long keyword);
PUBLIC void GetDataSizeText(const unsigned long keyword, char *text);
PUBLIC void GetDataText(const unsigned long keyword, char *text);
PUBLIC char IsDataReadOnly(const unsigned long keyword);
PUBLIC char IsVirtualTrait(const unsigned long keyword);
PUBLIC int  GetDataAsText(const unsigned long keyword, char *buf, int blen,ActorType *pActor);

PUBLIC const char *   GetDataSTRING(const unsigned long keyword,ActorType *pActor=NULL);
PUBLIC char           GetDataBYTE(  const unsigned long keyword,ActorType *pActor=NULL);
PUBLIC short          GetDataWORD(  const unsigned long keyword,ActorType *pActor=NULL);
PUBLIC long           GetDataDWORD( const unsigned long keyword,ActorType *pActor=NULL);
PUBLIC unsigned char  GetDataUBYTE( const unsigned long keyword,ActorType *pActor=NULL);
PUBLIC unsigned short GetDataUWORD( const unsigned long keyword,ActorType *pActor=NULL);
PUBLIC unsigned long  GetDataUDWORD(const unsigned long keyword,ActorType *pActor=NULL);
PUBLIC float          GetDataFLOAT( const unsigned long keyword,ActorType *pActor=NULL);

PUBLIC void SetDataSTRING(const unsigned long keyword, const char *data,ActorType *pActor=NULL);
PUBLIC void SetDataBYTE(  const unsigned long keyword, char data,ActorType *pActor=NULL);
PUBLIC void SetDataWORD(  const unsigned long keyword, short data,ActorType *pActor=NULL);
PUBLIC void SetDataDWORD( const unsigned long keyword, long data,ActorType *pActor=NULL);
PUBLIC void SetDataUBYTE( const unsigned long keyword, unsigned char data,ActorType *pActor=NULL);
PUBLIC void SetDataUWORD( const unsigned long keyword, unsigned short data,ActorType *pActor=NULL);
PUBLIC void SetDataUDWORD(const unsigned long keyword, unsigned long data,ActorType *pActor=NULL);
PUBLIC void SetDataFLOAT( const unsigned long keyword, float data,ActorType *pActor=NULL);

} /* extern "C" */

} // End of namespace DungeonCraft

#endif
