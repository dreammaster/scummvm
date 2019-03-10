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

#include "dungeoncraft/shared/RunTimeIF.h"

#include "dungeoncraft/shared/Char.h"
#ifdef UAFEngine
#include "dungeoncraft/shared/..\UAFWin\combatants.h"
#endif
#include "dungeoncraft/shared/monster.h"
#include "dungeoncraft/shared/Level.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/SpecAb.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


char *IF_TYPE_TEXT[MAX_IF_TYPES] = 
{
  "Text",
  "Byte",
  "Word",
  "Dword",
  "Float"
};

void GetIfTypeText(int type, char *pText)
{
  if (pText==NULL) return;
  switch (type)
  {
  case IF_TYPE_STRING: strcpy(pText,"Text");    break;
  case IF_TYPE_BYTE:   strcpy(pText,"Byte");    break;
  case IF_TYPE_WORD:   strcpy(pText,"Word");    break;
  case IF_TYPE_DWORD:  strcpy(pText,"Dword");   break;
  case IF_TYPE_FLOAT:  strcpy(pText,"Float");   break;
  default:             strcpy(pText,"Unknown"); break;
  }
}

// set before calling runtimeif functions
//ActorType CurrentCharacter=NULL_ACTOR;
ActorType CurrentTarget=NULL_ACTOR;
CHARACTER NULL_CHAR;
#ifdef UAFEngine
COMBATANT NULL_COMBATANT;
#endif

CList <ActorType, ActorType> charContextStack;
extern "C"
{
void ClearCharContextStack(void) { charContextStack.RemoveAll(); }
int HaveCharContext(void) { return (charContextStack.GetCount()>0); }

void SetCharContext(ActorType data) 
{ 
#ifndef UAFEDITOR
  ASSERT(data!=NULL_ACTOR);
#endif
  charContextStack.AddHead(data); 
}
void ClearCharContext(void) { ASSERT(HaveCharContext()); if (charContextStack.GetCount()>0) charContextStack.RemoveHead(); }

ActorType GetCharContext(void) 
{ 
  ASSERT(charContextStack.GetCount()>0); 
  if (charContextStack.GetCount()>0)
    return charContextStack.GetHead(); 
  else
  {
    WriteDebugString("Returning NULL_CHAR in RTI GetCharContext() (empty stack)\n");
    return NULL_ACTOR;
  }
}

int HaveTargetContext(void) { return ((CurrentTarget==NULL_ACTOR)==FALSE); }
void SetTargetContext(ActorType data) { CurrentTarget=data; }
void ClearTargetContext(void) { CurrentTarget=NULL_ACTOR; }
ActorType GetTargetContext(void) { return CurrentTarget; }

}

#define CHAR_IF_TYPE 1
#define PARTY_IF_TYPE 2
#define GAME_IF_TYPE 3

struct IF_KEYWORD_DATA
{ 
  char iftype;   // char, party, or game var type
  char *keyword; // key text, such as '$CHAR_NAME'
  unsigned char type;     // string, byte, int, etc
  char readonly; // 1=read-only, 0=read/write
  int  keyindex; // enum that matches key text, such CHAR_NAME
  int  virtualtrait; // represents trait that is virtual, or maps to existing memory
};

#define CHARIF_PREFIX  "$CHAR_"
#define PARTYIF_PREFIX "$PARTY_"
#define GAMEIF_PREFIX  "$GAME_"


#define DEFCHARIF(a,b,c)  { CHAR_IF_TYPE, CHARIF_PREFIX#a,  IF_TYPE_##b, c, CHAR_##a, 0 }
#define DEFPARTYIF(a,b,c) { PARTY_IF_TYPE, PARTYIF_PREFIX#a, IF_TYPE_##b, c, PARTY_##a, 0 }
#define DEFGAMEIF(a,b,c)  { GAME_IF_TYPE, GAMEIF_PREFIX#a,  IF_TYPE_##b, c, GAME_##a, 0 }

#define DEFCHARIFV(a,b,c)  { CHAR_IF_TYPE, CHARIF_PREFIX#a,  IF_TYPE_##b, c, CHAR_##a, 1 }
#define DEFPARTYIFV(a,b,c) { PARTY_IF_TYPE, PARTYIF_PREFIX#a, IF_TYPE_##b, c, PARTY_##a, 1 }
#define DEFGAMEIFV(a,b,c)  { GAME_IF_TYPE, GAMEIF_PREFIX#a,  IF_TYPE_##b, c, GAME_##a, 1 }

/* CHARACTER Values */
/* max of 512 values */
#define MAX_CHARACTER_IF ((LAST_CHAR_KEYWORD-CHAR_BASE)+1)
IF_KEYWORD_DATA CHARACTER_IF[MAX_CHARACTER_IF] =
{
  DEFCHARIF( BASE,         DWORD, 1),
  DEFCHARIF( NAME,         STRING,1),
  DEFCHARIF( AC,           DWORD, 0),
  DEFCHARIF( HITPOINTS,    DWORD, 0),
  DEFCHARIF( THACO,        DWORD, 0),
  DEFCHARIF( RDYTOTRAIN,   DWORD, 0),
  DEFCHARIF( CLERICEXP,    DWORD, 0),
  DEFCHARIF( MAGICUSEREXP, DWORD, 0),
  DEFCHARIF( FIGHTEREXP,   DWORD, 0),
  DEFCHARIF( RANGEREXP,    DWORD, 0),
  DEFCHARIF( THIEFEXP,     DWORD, 0),
  DEFCHARIF( DRUIDEXP,     DWORD, 0),
  DEFCHARIF( PALADINEXP,   DWORD, 0),
  DEFCHARIF( AGE,          DWORD, 0),
  DEFCHARIF( MAXAGE,       DWORD, 0),
  DEFCHARIF( MAXMOVE,      BYTE, 0),
  DEFCHARIF( STR,          BYTE, 0),
  DEFCHARIF( STRMOD,       BYTE, 0),
  DEFCHARIF( INT,          BYTE, 0),
  DEFCHARIF( WIS,          BYTE, 0),
  DEFCHARIF( DEX,          BYTE, 0),
  DEFCHARIF( CON,          BYTE, 0),
  DEFCHARIF( CHA,          BYTE, 0),
  DEFCHARIF( MAXENC,       DWORD, 0),
  DEFCHARIF( ENC,          DWORD, 1),
  DEFCHARIF( GENDER,       BYTE, 0),
  DEFCHARIF( CLASS,        BYTE, 0),
  DEFCHARIF( ALIGNMENT,    BYTE, 0),
  DEFCHARIF( STATUS,       BYTE, 0),
  DEFCHARIF( UNDEAD,       BYTE, 0),
  DEFCHARIF( SIZE,         BYTE, 0),
  DEFCHARIF( MAGICRESIST,  DWORD, 0),
  DEFCHARIF( SAVEVSPPDM,   DWORD, 0),
  DEFCHARIF( SAVEVSPP,     DWORD, 0),
  DEFCHARIF( SAVEVSRSW,    DWORD, 0),
  DEFCHARIF( SAVEVSBR,     DWORD, 0),
  DEFCHARIF( SAVEVSSP,     DWORD, 0),
  DEFCHARIF( CLERICLVL,    DWORD, 1),
  DEFCHARIF( FIGHTERLVL,   DWORD, 1),
  DEFCHARIF( RANGERLVL,    DWORD, 1),
  DEFCHARIF( DRUIDLVL,     DWORD, 1),
  DEFCHARIF( PALADINLVL,   DWORD, 1),
  DEFCHARIF( THIEFLVL,     DWORD, 1),
  DEFCHARIF( MAGUSERLVL,   DWORD, 1),
  DEFCHARIF( CLERICPREVLVL,  DWORD, 1),
  DEFCHARIF( FIGHTERPREVLVL, DWORD, 1),
  DEFCHARIF( RANGERPREVLVL,  DWORD, 1),
  DEFCHARIF( DRUIDPREVLVL,   DWORD, 1),
  DEFCHARIF( PALADINPREVLVL, DWORD, 1),
  DEFCHARIF( THIEFPREVLVL,   DWORD, 1),
  DEFCHARIF( MAGUSERPREVLVL, DWORD, 1),
  DEFCHARIF( CLERICPDLVL,    DWORD, 1),
  DEFCHARIF( FIGHTERPDLVL,   DWORD, 1),
  DEFCHARIF( RANGERPDLVL,  DWORD, 1),
  DEFCHARIF( DRUIDPDLVL,   DWORD, 1),
  DEFCHARIF( PALADINPDLVL, DWORD, 1),
  DEFCHARIF( THIEFPDLVL,   DWORD, 1),
  DEFCHARIF( MAGUSERPDLVL, DWORD, 1),
  DEFCHARIF( NBRHITDICE,   FLOAT, 1),
  DEFCHARIF( NBRATTACKS,   FLOAT, 1),
  DEFCHARIF( MORALE,       DWORD, 0),
  DEFCHARIF( OPENDOORS,    BYTE, 0),
  DEFCHARIF( OPENMAGICDOORS, BYTE, 0),
  DEFCHARIF( BENDLIFT,     BYTE, 0),
  DEFCHARIF( PICKPOCKETS,  DWORD, 0),
  DEFCHARIF( OPENLOCKS,    DWORD, 0),
  DEFCHARIF( FINDTRAPS,    DWORD, 0),
  DEFCHARIF( MOVESILENT,   DWORD, 0),
  DEFCHARIF( HIDESHADOWS,  DWORD, 0),
  DEFCHARIF( HEARNOISE,    DWORD, 0),
  DEFCHARIF( CLIMBWALLS,   DWORD, 0),
  DEFCHARIF( READLANG,     DWORD, 0),
  DEFCHARIF( ALLOWPLAYERCONTROL, BYTE, 0),
  DEFCHARIFV( DETECTMAGIC, BYTE, 0),
  DEFCHARIF( DETECTINVISIBLE, BYTE, 0),
  DEFCHARIF( DETECTTRAPS, BYTE, 0),
  DEFCHARIF( DAMAGEBONUS, BYTE, 0),
  DEFCHARIFV( BLESS, BYTE, 0),
  DEFCHARIFV( CURSE, BYTE, 0),
  DEFCHARIFV( UNDEADFEAR, BYTE, 0),
  DEFCHARIFV( ENLARGE, BYTE, 0),
  DEFCHARIFV( REDUCE, BYTE, 0),
  DEFCHARIFV( CHARMPERSON, BYTE, 0),
  DEFCHARIFV( REFLECTGAZEATTACK, BYTE, 0),
  DEFCHARIFV( PROTFROMEVIL, BYTE, 0),
  DEFCHARIFV( PROTFROMGOOD, BYTE, 0),
  DEFCHARIFV( SHIELD, BYTE, 0),
  DEFCHARIFV( SLEEP, BYTE, 0),
  DEFCHARIFV( FOG, BYTE, 0),
  DEFCHARIFV( ENTANGLE, BYTE, 0),
  DEFCHARIFV( INVISIBLETOANIMALS, BYTE, 0),
  DEFCHARIFV( INVISIBLETOUNDEAD, BYTE, 0),
  DEFCHARIFV( NONUNDEADFEAR, BYTE, 0),
  DEFCHARIFV( SANCTUARY, BYTE, 0),
  DEFCHARIFV( SHILLELAGH, BYTE, 0),
  DEFCHARIFV( DISPLACEMENT, BYTE, 0),
  DEFCHARIFV( WIZARDRY, BYTE, 0),
  DEFCHARIF( CHARTYPE, BYTE, 1),
  DEFCHARIFV( VORPALATTACK, BYTE, 0),
  DEFCHARIF( COINAMOUNT1, DWORD, 0),
  DEFCHARIF( COINAMOUNT2, DWORD, 0),
  DEFCHARIF( COINAMOUNT3, DWORD, 0),
  DEFCHARIF( COINAMOUNT4, DWORD, 0),
  DEFCHARIF( COINAMOUNT5, DWORD, 0),
  DEFCHARIF( COINAMOUNT6, DWORD, 0),
  DEFCHARIF( COINAMOUNT7, DWORD, 0),
  DEFCHARIF( COINAMOUNT8, DWORD, 0),
  DEFCHARIF( COINAMOUNT9, DWORD, 0),
  DEFCHARIF( COINAMOUNT10, DWORD, 0),
  DEFCHARIF( GEMAMOUNT, DWORD, 0),
  DEFCHARIF( JEWELRYAMOUNT, DWORD, 0),
  DEFCHARIFV( HOLDPERSON, BYTE, 0),
  DEFCHARIFV( SILENCE, BYTE, 0),
  DEFCHARIFV( ISMAMMAL, BYTE, 1),
  DEFCHARIFV( ISANIMAL, BYTE, 1),
  DEFCHARIFV( ISSNAKE, BYTE, 1),
  DEFCHARIFV( ISGIANT, BYTE, 1),
  DEFCHARIFV( ISALWAYSLARGE, BYTE, 1),
  DEFCHARIFV( HASDWARFACPENALTY, BYTE, 1),
  DEFCHARIFV( HASGNOMEACPENALTY, BYTE, 1),
  DEFCHARIFV( HASDWARFTHACOPENALTY, BYTE, 1),
  DEFCHARIFV( HASGNOMETHACOPENALTY, BYTE, 1),
  DEFCHARIFV( HASRANGERDMGPENALTY, BYTE, 1),
  DEFCHARIFV( HASPOISONIMMUNITY, BYTE, 1),
  DEFCHARIFV( HASDEATHIMMUNITY, BYTE, 1),
  DEFCHARIFV( HASCONFUSIONIMMUNITY, BYTE, 1),
  DEFCHARIFV( HASVORPALIMMUNITY, BYTE, 1),
  DEFCHARIFV( CANBEHELDORCHARMED, BYTE, 1),
  DEFCHARIFV( AFFECTEDBYDISPELEVIL, BYTE, 1),
  DEFCHARIFV( POISONED, BYTE, 0),
  DEFCHARIFV( SLOWPOISON, BYTE, 0),
  DEFCHARIFV( MIRRORIMAGE, BYTE, 0),
  DEFCHARIFV( INVISIBLE, BYTE, 0),
  DEFCHARIFV( ENFEEBLED, BYTE, 0),
  DEFCHARIF(  HITBONUS, BYTE, 0),
  DEFCHARIFV( BLINDNESS, BYTE, 0),
  DEFCHARIFV( DISEASED, BYTE, 0)
};

/* party values */
/* max of 512 values */
#define MAX_PARTY_IF ((LAST_PARTY_KEYWORD-PARTY_BASE)+1)
IF_KEYWORD_DATA PARTY_IF[MAX_PARTY_IF] =
{
  DEFPARTYIF( BASE,           DWORD, 1),
  DEFPARTYIF( FACING,         DWORD, 0),
  DEFPARTYIF( DAYS,           DWORD, 0),
  DEFPARTYIF( HOURS,          DWORD, 0),
  DEFPARTYIF( MINUTES,        DWORD, 0),
  DEFPARTYIF( ACTIVECHAR,     DWORD, 0),
  DEFPARTYIF( MONEYAVAILABLE, DWORD, 1)
};

/* game values */
/* max of 512 values */
#define MAX_GAME_IF ((LAST_GAME_KEYWORD-GAME_BASE)+1)
IF_KEYWORD_DATA GAME_IF[MAX_GAME_IF] =
{
  DEFGAMEIF( BASE,                DWORD,  1),
  DEFGAMEIF( CURRLEVEL,           DWORD,  1),
  DEFGAMEIF( VERSION,             FLOAT,  1),
  DEFGAMEIF( COINNAME1,           STRING, 1),
  DEFGAMEIF( COINNAME2,           STRING, 1),
  DEFGAMEIF( COINNAME3,           STRING, 1),
  DEFGAMEIF( COINNAME4,           STRING, 1),
  DEFGAMEIF( COINNAME5,           STRING, 1),
  DEFGAMEIF( COINNAME6,           STRING, 1),
  DEFGAMEIF( COINNAME7,           STRING, 1),
  DEFGAMEIF( COINNAME8,           STRING, 1),
  DEFGAMEIF( COINNAME9,           STRING, 1),
  DEFGAMEIF( COINNAME10,          STRING, 1),
  DEFGAMEIF( GEMNAME,             STRING, 1),
  DEFGAMEIF( JEWELRYNAME,         STRING, 1),
  DEFGAMEIF( VAULTMONEYAVAILABLE, FLOAT,  1)
};

// search by keyword string
IF_KEYWORD_DATA *GetKeywordData(const char *keyword)
{
  int i;
  if (keyword==NULL) return NULL;

  if (strncmp(keyword,CHARIF_PREFIX,6) == 0)
  {
    for (i=1;i<MAX_CHARACTER_IF;i++)
    {
      if (strcmp(&keyword[5], &CHARACTER_IF[i].keyword[5])==0)
        return &CHARACTER_IF[i];
    }
  }
  else if (strncmp(keyword,PARTYIF_PREFIX,7) == 0)
  {
    for (i=1;i<MAX_PARTY_IF;i++)
    {
      if (strcmp(&keyword[6], &PARTY_IF[i].keyword[6])==0)
        return &PARTY_IF[i];
    }
  }
  else if (strncmp(keyword,GAMEIF_PREFIX,5) == 0)
  {
    for (i=1;i<MAX_GAME_IF;i++)
    {
      if (strcmp(&keyword[4], &GAME_IF[i].keyword[4])==0)
        return &GAME_IF[i];
    }
  }

  return NULL;
}

// search by constant enum
IF_KEYWORD_DATA *GetKeywordData(const int keyword)
{
  int index;
  if (keyword > GAME_BASE)
  {
    index = keyword-GAME_BASE;
    if ((index >= 1) && (index < MAX_GAME_IF))
    {
      if (GAME_IF[index].keyindex == keyword)
      {
        // hopefully, the table is arranged in the same
        // order as the enumerations.
        return &GAME_IF[index];
      }
      else
      {
        for (int i=1;i<MAX_GAME_IF;i++)
        {
          if (GAME_IF[i].keyindex==keyword)
            return &GAME_IF[i];
        }
        return NULL;
      }
    }
  }
  else if (keyword > PARTY_BASE)
  {
    index = keyword-PARTY_BASE;
    if ((index >= 1) && (index < MAX_PARTY_IF))
    {
      if (PARTY_IF[index].keyindex == keyword)
      {
        // hopefully, the table is arranged in the same
        // order as the enumerations.
        return &PARTY_IF[index];
      }
      else
      {
        for (int i=1;i<MAX_PARTY_IF;i++)
        {
          if (PARTY_IF[i].keyindex==keyword)
            return &PARTY_IF[i];
        }
        return NULL;
      }
    }
  }
  else if (keyword > CHAR_BASE)
  {
    index = keyword-CHAR_BASE;
    if ((index >= 1) && (index < MAX_CHARACTER_IF))
    {
      if (CHARACTER_IF[index].keyindex == keyword)
      {
        // hopefully, the table is arranged in the same
        // order as the enumerations.
        return &CHARACTER_IF[index];
      }
      else
      {
        for (int i=1;i<MAX_CHARACTER_IF;i++)
        {
          if (CHARACTER_IF[i].keyindex==keyword)
            return &CHARACTER_IF[i];
        }
        return NULL;
      }
    }
  }
  return NULL;
}


IF_KEYWORD_DATA *LocateKeywordData(const int keyindex)
{
  static IF_KEYWORD_DATA *charTable[MAX_CHARACTER_IF];
  static IF_KEYWORD_DATA *partyTable[MAX_PARTY_IF];
  static IF_KEYWORD_DATA *gameTable[MAX_GAME_IF];

  static int initialized = 0;
  int i;
  if (initialized == 0)
  {
    for (i=0; i<MAX_CHARACTER_IF; i++) charTable[i]  = NULL;
    for (i=0; i<MAX_PARTY_IF; i++)     partyTable[i] = NULL;
    for (i=0; i<MAX_GAME_IF; i++)      gameTable[i]  = NULL;
    initialized = 1;
  };
  if ((keyindex > CHAR_BASE) && (keyindex < CHAR_BASE+MAX_CHARACTER_IF))
  {
    i = keyindex - CHAR_BASE;
    if (charTable[i] == NULL) charTable[i] = GetKeywordData(keyindex);
    return charTable[i];
  }
  else if ((keyindex > PARTY_BASE) && (keyindex < PARTY_BASE+MAX_PARTY_IF))
  {
    i = keyindex - PARTY_BASE;
    if (partyTable[i] == NULL) partyTable[i] = GetKeywordData(keyindex);
    return partyTable[i];
  }
  else if ((keyindex > GAME_BASE) && (keyindex < GAME_BASE+MAX_GAME_IF))
  {
    i = keyindex - GAME_BASE;
    if (gameTable[i] == NULL) gameTable[i] = GetKeywordData(keyindex);
    return gameTable[i];
  }
  else return NULL;
}



// convert keyword text to enum
extern "C" unsigned long KeywordTextToNum(const char *keyword)
{
  int i;
  if (keyword==NULL) return 0;

  if (strncmp(keyword,CHARIF_PREFIX,6) == 0)
  {
    for (i=1;i<MAX_CHARACTER_IF;i++)
    {
      if (strcmp(&keyword[5], &CHARACTER_IF[i].keyword[5])==0)
        return i+CHAR_BASE;
    }
  }
  else if (strncmp(keyword,PARTYIF_PREFIX,7) == 0)
  {
    for (i=1;i<MAX_PARTY_IF;i++)
    {
      if (strcmp(&keyword[6], &PARTY_IF[i].keyword[6])==0)
        return i+PARTY_BASE;
    }
  }
  else if (strncmp(keyword,GAMEIF_PREFIX,6) == 0)
  {
    for (i=1;i<MAX_GAME_IF;i++)
    {
      if (strcmp(&keyword[5], &GAME_IF[i].keyword[5])==0)
        return i+GAME_BASE;
    }
  }

  return 0;
}

extern "C" const char *NumToKeywordText(const unsigned long keyword)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if (pData != NULL) return pData->keyword;
  return NULL;
}

unsigned char GetDataSize(const char *keyword)
{
  IF_KEYWORD_DATA *pData = GetKeywordData(keyword);
  if (pData != NULL) return pData->type;
  return MAX_IF_TYPES;
}

extern "C" unsigned char GetDataSize(const unsigned long keyword)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if (pData != NULL) return pData->type;
  return MAX_IF_TYPES;
}

extern "C" void GetDataSizeText(const unsigned long keyword, char *text)
{
  if (text==NULL)return;
  BYTE size = GetDataSize(keyword);
  BOOL IsUnsigned = (size & 128);
  size &= 127;
  if (IsUnsigned) strcpy(text, "Unsigned ");
  if (size >= MAX_IF_TYPES) strcpy(text, "???");
  else strcpy(text, IF_TYPE_TEXT[size]);
}
void GetDataText(const char *keyword, char *text)
{
  if (text == NULL) return;
  IF_KEYWORD_DATA *pData = GetKeywordData(keyword);
  if (pData != NULL) strcpy(text, pData->keyword);
}

extern "C" void GetDataText(const unsigned long keyword, char *text)
{
  if (text == NULL) return;
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if (pData != NULL) strcpy(text, pData->keyword);
}

extern "C" char IsDataReadOnly(const unsigned long keyword)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if (pData != NULL) return pData->readonly;
  return 1;
}

extern "C" char IsVirtualTrait(const unsigned long keyword)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if (pData != NULL) return pData->virtualtrait;
  return 1;
}

//#ifdef UAFEngine

extern "C" CHARACTER &GetCharacterContext(ActorType X)
{
  ASSERT ((X.Flags & FLAG_COMBAT)==0);
  if ((X.Flags & FLAG_COMBAT) > 0)
  {
    WriteDebugString("Returning NULL_CHAR in RTI GetCharacterContext() (combat flag set)\n");
    return NULL_CHAR;
  }

  if (X.Flags & FLAG_NONCOMBAT)
  {
    if (X.IsPartySrc())
    {
      for (int i=0;i<party.numCharacters;i++)
      {
        if (party.characters[i].marchOrder == X.Instance)
          return (party.characters[i]);
      }
      WriteDebugString("Unable to find party char context in RTI GetCharacterContext()\n");
    }
    else if (X.IsNPCSrc())
    {
      if (!globalData.charData.HaveKey(X.Instance))
      {
        WriteDebugString("Invalid npc actor instance in RTI GetCharacterContext()\n");
        return NULL_CHAR;
      }
      CHARACTER *pchar = globalData.charData.GetCharacterData(X.Instance);
      if (pchar!=NULL)
        return *pchar;
      else
        return NULL_CHAR;
    }
    else if (X.IsMonsterSrc())
    {
      if (!monsterData.HaveMonster(X.Instance))
      {
        WriteDebugString("Invalid monster actor instance in RTI GetCharacterContext()\n");
        return NULL_CHAR;
      }      
      //MONSTER_DATA *pchar = monsterData.GetMonsterData(X.Instance);
      ASSERT(FALSE);
      return NULL_CHAR;
    }
  }
#ifndef UAFEDITOR
  WriteDebugString("Returning NULL_CHAR in RTI GetCharacterContext()\n");
  ASSERT(FALSE);
#endif
  return NULL_CHAR;
}

extern "C" CHARACTER *GetCurrentlyActiveContext(ActorType *pActor)
{
  CHARACTER *pChar=NULL;
  if (IsCombatActive())
  {
#ifdef UAFEngine
    if (pActor==NULL)
      pChar = &GetCombatCharacterContext(GetCharContext());
    else
      pChar = &GetCombatCharacterContext(*pActor);
#else
    return NULL;
#endif
  }
  else
  {
    if (pActor==NULL)      
      pChar = &GetCharacterContext(GetCharContext());
    else
      pChar = &GetCharacterContext(*pActor);
  }
#ifdef UAFEngine
  ASSERT(pChar!=NULL);
  ASSERT(pChar->GetContext()!=NULL_ACTOR);
#endif
  return pChar;
}

#ifdef UAFEngine
extern "C" COMBATANT &GetCombatCharacterContext(ActorType X)
{
  ASSERT (X.Flags & FLAG_COMBAT);
  if ((X.Flags & FLAG_COMBAT) == 0)
  {
    WriteDebugString("Returning NULL_CHAR in RTI GetCombatCharContext() (no combat flag)\n");
    return NULL_COMBATANT;
  }
  if ((X.Instance >= 0) && (X.Instance < (DWORD)combatData.numCombatants))
  {
    return (combatData.combatants[X.Instance]);
  }
  else
  {
    ASSERT(FALSE);
    WriteDebugString("Returning NULL_CHAR in RTI GetCombatCharContext()\n");
    return NULL_COMBATANT;
  }
}
#endif

extern "C" CHARACTER &GetTargetCharacterContext()
{
  CHARACTER *pChar = GetCurrentlyActiveContext(&CurrentTarget);
  if (pChar !=NULL)
    return *pChar;
  else
    return NULL_CHAR;
}


const char *GetCharacterSTRING(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  //CHARACTER &dude = GetCharacterContext();
  CHARACTER *pDude = GetCurrentlyActiveContext(pActor);
  if (pDude==NULL) return NULL;

  switch (pData->keyindex)
  {
  case CHAR_NAME: return pDude->GetName();
  }
  return NULL;
}

const char GetCharacterBYTE(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  //CHARACTER &dude = GetCharacterContext();
  CHARACTER *pDude = GetCurrentlyActiveContext(pActor);
  if (pDude==NULL) return NULL;

  switch (pData->keyindex)
  {
  case CHAR_MAXMOVE: return pDude->GetAdjMaxMovement(); break;
  case CHAR_STR: return pDude->GetAdjStr(); break;
  case CHAR_STRMOD: return pDude->GetAdjStrMod(); break;
  case CHAR_INT: return pDude->GetAdjInt(); break;
  case CHAR_WIS: return pDude->GetAdjWis(); break;
  case CHAR_DEX: return pDude->GetAdjDex(); break;
  case CHAR_CON: return pDude->GetAdjCon(); break;
  case CHAR_CHA: return pDude->GetAdjCha();break;

  case CHAR_GENDER: return pDude->GetAdjGender();break;
  case CHAR_CLASS: return pDude->GetAdjClass();break;
  case CHAR_ALIGNMENT: return pDude->GetAdjAlignment();break;
  case CHAR_STATUS: return pDude->GetAdjStatus();break;
  case CHAR_UNDEAD: return pDude->GetAdjUndeadType();break;
  case CHAR_SIZE: return pDude->GetAdjSize();break;

  case CHAR_OPENDOORS: return pDude->GetAdjOpenDoors(); break;
  case CHAR_OPENMAGICDOORS: return pDude->GetAdjOpenMagicDoors(); break;
  case CHAR_BENDLIFT: return pDude->GetAdjBendBarsLiftGates(); break;
  case CHAR_ALLOWPLAYERCONTROL: return pDude->GetAdjAllowPlayerControl(); break;
  case CHAR_DETECTMAGIC: return pDude->GetAdjSpecAb(SA_DetectMagic); break;
  case CHAR_DETECTINVISIBLE: return pDude->GetAdjDetectingInvisible(); break;
  case CHAR_DETECTTRAPS: return pDude->GetAdjDetectingTraps(); break;
  case CHAR_DAMAGEBONUS: return pDude->GetAdjDmgBonus(); break;
  case CHAR_HITBONUS: return pDude->GetAdjHitBonus(); break;

  case CHAR_BLESS: return pDude->GetAdjSpecAb(SA_Bless); break;
  case CHAR_CURSE: return pDude->GetAdjSpecAb(SA_Curse); break;
  case CHAR_UNDEADFEAR: return pDude->GetAdjSpecAb(SA_UndeadFear); break;
  case CHAR_ENLARGE: return pDude->GetAdjSpecAb(SA_Enlarge); break;
  case CHAR_REDUCE: return pDude->GetAdjSpecAb(SA_Reduce); break;
  case CHAR_CHARMPERSON: return pDude->GetAdjSpecAb(SA_CharmPerson); break;
  case CHAR_REFLECTGAZEATTACK: return pDude->GetAdjSpecAb(SA_ReflectGazeAttack); break;
  case CHAR_PROTFROMEVIL: return pDude->GetAdjSpecAb(SA_ProtFromEvil); break;
  case CHAR_PROTFROMGOOD: return pDude->GetAdjSpecAb(SA_ProtFromGood); break;
  case CHAR_SHIELD: return pDude->GetAdjSpecAb(SA_Shield); break;
  case CHAR_SLEEP: return pDude->GetAdjSpecAb(SA_Sleep); break;
  case CHAR_FOG: return pDude->GetAdjSpecAb(SA_Fog); break;
  case CHAR_ENTANGLE: return pDude->GetAdjSpecAb(SA_Entangle); break;
  case CHAR_INVISIBLETOANIMALS: return pDude->GetAdjSpecAb(SA_InvisibleToAnimals); break;
  case CHAR_INVISIBLETOUNDEAD: return pDude->GetAdjSpecAb(SA_InvisibleToUndead); break;
  case CHAR_NONUNDEADFEAR: return pDude->GetAdjSpecAb(SA_NonUndeadFear); break;
  case CHAR_SANCTUARY: return pDude->GetAdjSpecAb(SA_Sanctuary); break;
  case CHAR_SHILLELAGH: return pDude->GetAdjSpecAb(SA_Shillelagh); break;
  case CHAR_DISPLACEMENT: return pDude->GetAdjSpecAb(SA_Displacement); break;
  case CHAR_WIZARDRY: return pDude->GetAdjSpecAb(SA_Wizardry); break;
  case CHAR_CHARTYPE: return pDude->GetType(); break;
  case CHAR_VORPALATTACK: return pDude->GetAdjSpecAb(SA_VorpalAttack); break;
  case CHAR_HOLDPERSON: return pDude->GetAdjSpecAb(SA_HoldPerson); break;
  case CHAR_SILENCE: return pDude->GetAdjSpecAb(SA_Silence); break;
  case CHAR_POISONED: return pDude->GetAdjSpecAb(SA_Poisoned); break;
  case CHAR_SLOWPOISON: return pDude->GetAdjSpecAb(SA_SlowPoison); break;
  case CHAR_MIRRORIMAGE: return pDude->GetAdjSpecAb(SA_MirrorImage); break;
  case CHAR_INVISIBLE: return pDude->GetAdjSpecAb(SA_Invisible); break;
  case CHAR_ENFEEBLED: return pDude->GetAdjSpecAb(SA_Enfeebled); break;
  case CHAR_ISMAMMAL: return pDude->IsMammal();
  case CHAR_ISANIMAL: return pDude->IsAnimal();
  case CHAR_ISSNAKE: return pDude->IsSnake();
  case CHAR_ISGIANT: return pDude->IsGiant();
  case CHAR_ISALWAYSLARGE: return pDude->IsAlwaysLarge();
  case CHAR_HASDWARFACPENALTY: return pDude->HasDwarfACPenalty();
  case CHAR_HASGNOMEACPENALTY: return pDude->HasGnomeACPenalty();
  case CHAR_HASDWARFTHACOPENALTY: return pDude->HasDwarfTHACOPenalty();
  case CHAR_HASGNOMETHACOPENALTY: return pDude->HasGnomeTHACOPenalty();
  case CHAR_HASRANGERDMGPENALTY: return pDude->HasRangerDmgPenalty();
  case CHAR_HASPOISONIMMUNITY: return pDude->HasPoisonImmunity();
  case CHAR_HASDEATHIMMUNITY: return pDude->HasDeathImmunity();
  case CHAR_HASCONFUSIONIMMUNITY: return pDude->HasConfusionImmunity();
  case CHAR_HASVORPALIMMUNITY: return pDude->HasVorpalImmunity();
  case CHAR_CANBEHELDORCHARMED: return pDude->CanBeHeldOrCharmed();
  case CHAR_AFFECTEDBYDISPELEVIL: return pDude->AffectedByDispelEvil();
  case CHAR_BLINDNESS: return pDude->GetAdjSpecAb(SA_Blindness); break;
  case CHAR_DISEASED: return pDude->GetAdjSpecAb(SA_Diseased); break;
  }
  return 0;
}

const unsigned char GetCharacterUBYTE(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  return 0;
}

const short GetCharacterWORD(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  return 0;
}

const unsigned short GetCharacterUWORD(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  return 0;
}

const long GetCharacterDWORD(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{

  //CHARACTER &dude = GetCharacterContext();
  CHARACTER *pDude = GetCurrentlyActiveContext(pActor);
  if (pDude==NULL) return NULL;

  switch (pData->keyindex)
  {
  case CHAR_AC:return pDude->GetAdjAC(); break;
  case CHAR_HITPOINTS: return pDude->GetAdjHitPoints(); break;
  case CHAR_THACO: return pDude->GetAdjTHACO(); break;
  case CHAR_RDYTOTRAIN: return pDude->GetAdjReadyToTrain(); break;
  case CHAR_CLERICEXP: return pDude->GetAdjClericExp(); break;
  case CHAR_MAGICUSEREXP: return pDude->GetAdjMagicUserExp(); break;
  case CHAR_FIGHTEREXP: return pDude->GetAdjFighterExp(); break;
  case CHAR_RANGEREXP: return pDude->GetAdjRangerExp(); break;
  case CHAR_THIEFEXP: return pDude->GetAdjThiefExp(); break;
  case CHAR_DRUIDEXP: return pDude->GetAdjDruidExp(); break;
  case CHAR_PALADINEXP: return pDude->GetAdjPaladinExp(); break;
  case CHAR_AGE: return pDude->GetAdjAge(); break;
  case CHAR_MAXAGE: return pDude->GetAdjMaxAge(); break;
  case CHAR_MAXENC: return pDude->GetAdjMaxEncumbrance(); break;
  case CHAR_ENC: return pDude->GetEncumbrance(); break;
  case CHAR_MAGICRESIST: return pDude->GetAdjMagicResistance(); break;
  case CHAR_SAVEVSPPDM: return pDude->GetAdjSaveVsPPDM(); break;
  case CHAR_SAVEVSPP: return pDude->GetAdjSaveVsPP(); break;
  case CHAR_SAVEVSRSW: return pDude->GetAdjSaveVsRSW(); break;
  case CHAR_SAVEVSBR: return pDude->GetAdjSaveVsBr(); break;
  case CHAR_SAVEVSSP: return pDude->GetAdjSaveVsSp(); break;
  case CHAR_CLERICLVL: return pDude->GetCurrentLevel(ClericFlag); break;
  case CHAR_FIGHTERLVL: return pDude->GetCurrentLevel(FighterFlag); break;
  case CHAR_RANGERLVL: return pDude->GetCurrentLevel(RangerFlag); break;
  case CHAR_DRUIDLVL: return pDude->GetCurrentLevel(DruidFlag); break;
  case CHAR_PALADINLVL: return pDude->GetCurrentLevel(PaladinFlag); break;
  case CHAR_THIEFLVL: return pDude->GetCurrentLevel(ThiefFlag); break;
  case CHAR_MAGUSERLVL: return pDude->GetCurrentLevel(MagicUserFlag); break;
  case CHAR_CLERICPREVLVL: return pDude->GetPrevLevel(ClericFlag); break;
  case CHAR_FIGHTERPREVLVL: return pDude->GetPrevLevel(FighterFlag); break;
  case CHAR_RANGERPREVLVL: return pDude->GetPrevLevel(RangerFlag); break;
  case CHAR_DRUIDPREVLVL: return pDude->GetPrevLevel(DruidFlag); break;
  case CHAR_PALADINPREVLVL: return pDude->GetPrevLevel(PaladinFlag); break;
  case CHAR_THIEFPREVLVL: return pDude->GetPrevLevel(ThiefFlag); break;
  case CHAR_MAGUSERPREVLVL: return pDude->GetPrevLevel(MagicUserFlag); break;
  case CHAR_CLERICPDLVL: return pDude->GetPreDrainLevel(ClericFlag); break;
  case CHAR_FIGHTERPDLVL: return pDude->GetPreDrainLevel(FighterFlag); break;
  case CHAR_RANGERPDLVL: return pDude->GetPreDrainLevel(RangerFlag); break;
  case CHAR_DRUIDPDLVL: return pDude->GetPreDrainLevel(DruidFlag); break;
  case CHAR_PALADINPDLVL: return pDude->GetPreDrainLevel(PaladinFlag); break;
  case CHAR_THIEFPDLVL: return pDude->GetPreDrainLevel(ThiefFlag); break;
  case CHAR_MAGUSERPDLVL: return pDude->GetPreDrainLevel(MagicUserFlag); break;
  case CHAR_MORALE: return pDude->GetAdjMorale(); break;
  case CHAR_PICKPOCKETS: return pDude->GetAdjPickPockets(); break;
  case CHAR_OPENLOCKS: return pDude->GetAdjOpenLocks(); break;
  case CHAR_FINDTRAPS: return pDude->GetAdjFindTraps(); break;
  case CHAR_MOVESILENT: return pDude->GetAdjMoveSilent(); break;
  case CHAR_HIDESHADOWS: return pDude->GetAdjHideInShadows(); break;
  case CHAR_HEARNOISE: return pDude->GetAdjHearNoise(); break;
  case CHAR_CLIMBWALLS: return pDude->GetAdjClimbWalls(); break;
  case CHAR_READLANG: return pDude->GetAdjReadLanguages(); break;
  case CHAR_COINAMOUNT1: return pDude->money.GetCoinAmount(PlatinumType); break;
  case CHAR_COINAMOUNT2: return pDude->money.GetCoinAmount(GoldType); break;
  case CHAR_COINAMOUNT3: return pDude->money.GetCoinAmount(ElectrumType); break;
  case CHAR_COINAMOUNT4: return pDude->money.GetCoinAmount(SilverType); break;
  case CHAR_COINAMOUNT5: return pDude->money.GetCoinAmount(CopperType); break;
  case CHAR_COINAMOUNT6: return pDude->money.GetCoinAmount(CoinType6); break;
  case CHAR_COINAMOUNT7: return pDude->money.GetCoinAmount(CoinType7); break;
  case CHAR_COINAMOUNT8: return pDude->money.GetCoinAmount(CoinType8); break;
  case CHAR_COINAMOUNT9: return pDude->money.GetCoinAmount(CoinType9); break;
  case CHAR_COINAMOUNT10: return pDude->money.GetCoinAmount(CoinType10); break;
  case CHAR_GEMAMOUNT: return pDude->money.NumGems(); break;
  case CHAR_JEWELRYAMOUNT: return pDude->money.NumJewelry(); break;
  }
  return 0;
}

const unsigned long GetCharacterUDWORD(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  return 0;
}

const float GetCharacterFLOAT(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  //CHARACTER &dude = GetCharacterContext();
  CHARACTER *pDude = GetCurrentlyActiveContext(pActor);
  if (pDude==NULL) return NULL;

  switch (pData->keyindex)
  {
  case CHAR_NBRHITDICE: return pDude->GetNbrHD(); break;
  case CHAR_NBRATTACKS: return pDude->GetNbrAttacks(); break;  
  }
  return 0.0;
}

void SetCharacterSTRING(const IF_KEYWORD_DATA *pData, const char *data,ActorType *pActor)
{
  //CHARACTER &dude = GetCharacterContext();
  CHARACTER *pDude = GetCurrentlyActiveContext(pActor);
  if (pDude==NULL) return;

  switch (pData->keyindex)
  {
  case CHAR_NAME: pDude->SetName(data);
  }
}

void SetCharacterBYTE(const IF_KEYWORD_DATA *pData, char data,ActorType *pActor)
{
  //CHARACTER &dude = GetCharacterContext();
  CHARACTER *pDude = GetCurrentlyActiveContext(pActor);
  if (pDude==NULL) return;

  switch (pData->keyindex)
  {
    // these auto-validate the new data
  case CHAR_MAXMOVE: pDude->SetMaxMovement(data); break;
  case CHAR_STR: pDude->SetStr(data); break;
  case CHAR_STRMOD: pDude->SetStrMod(data); break;
  case CHAR_INT: pDude->SetInt(data); break;
  case CHAR_WIS: pDude->SetWis(data); break;
  case CHAR_DEX: pDude->SetDex(data); break;
  case CHAR_CON: pDude->SetCon(data); break;
  case CHAR_CHA: pDude->SetCha(data);break;

    // these don't auto validate the new data
  case CHAR_GENDER: 
    {
      if ((data>=0)&&(data<NUM_GENDER_TYPES))
        pDude->SetGender((genderType)data);
    }
    break;
  case CHAR_CLASS: 
    {
      if ((data>=0)&&(data<NUM_CLASS_TYPES))
        pDude->SetClass((classType)data);
    }
    break;
  case CHAR_ALIGNMENT: 
    {
      if ((data>=0)&&(data<NUM_ALIGNMENT_TYPES))
        pDude->SetAlignment((alignmentType)data);
    }
    break;
  case CHAR_STATUS: 
    {
      if ((data>=0)&&(data<NUM_CHAR_STATUS_TYPES))
        pDude->SetStatus((charStatusType)data);
    }
    break;
  case CHAR_UNDEAD: 
    {
      if ((data>=0)&&(data<NUM_UNDEAD_TYPES))
        pDude->SetUndead((undeadType)data);
    }
    break;
  case CHAR_SIZE: 
    {
      if ((data>=0)&&(data<NUM_SIZE_TYPES))
        pDude->SetSize((creatureSizeType)data);
    }
    break;

  case CHAR_OPENDOORS: pDude->SetOpenDoors(data); break;
  case CHAR_OPENMAGICDOORS: pDude->SetOpenMagicDoors(data); break;
  case CHAR_BENDLIFT: pDude->SetBendBarsLiftGates(data); break;
  case CHAR_ALLOWPLAYERCONTROL: pDude->SetAllowPlayerControl(data); break;
  case CHAR_DETECTMAGIC: pDude->SetSpecAb(SA_DetectMagic,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_DETECTINVISIBLE: pDude->SetDetectingInvisible(data); break;
  case CHAR_DETECTTRAPS: pDude->SetDetectingTraps(data); break; 
  case CHAR_DAMAGEBONUS: pDude->SetDmgBonus(data); break;
  case CHAR_HITBONUS: pDude->SetHitBonus(data); break;

  case CHAR_BLESS:  pDude->SetSpecAb(SA_Bless,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_CURSE: pDude->SetSpecAb(SA_Curse,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_UNDEADFEAR: pDude->SetSpecAb(SA_UndeadFear,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_ENLARGE: pDude->SetSpecAb(SA_Enlarge,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_REDUCE: pDude->SetSpecAb(SA_Reduce,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_CHARMPERSON: pDude->SetSpecAb(SA_CharmPerson,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_REFLECTGAZEATTACK: pDude->SetSpecAb(SA_ReflectGazeAttack,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_PROTFROMEVIL: pDude->SetSpecAb(SA_ProtFromEvil,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_PROTFROMGOOD: pDude->SetSpecAb(SA_ProtFromGood,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_SHIELD: pDude->SetSpecAb(SA_Shield,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_SLEEP: pDude->SetSpecAb(SA_Sleep,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_FOG: pDude->SetSpecAb(SA_Fog,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_ENTANGLE: pDude->SetSpecAb(SA_Entangle,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_INVISIBLETOANIMALS: pDude->SetSpecAb(SA_InvisibleToAnimals,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_INVISIBLETOUNDEAD: pDude->SetSpecAb(SA_InvisibleToUndead,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_NONUNDEADFEAR: pDude->SetSpecAb(SA_NonUndeadFear,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_SANCTUARY: pDude->SetSpecAb(SA_Sanctuary,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_SHILLELAGH: pDude->SetSpecAb(SA_Shillelagh,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_DISPLACEMENT: pDude->SetSpecAb(SA_Displacement,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_WIZARDRY: pDude->SetSpecAb(SA_Wizardry,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_VORPALATTACK: pDude->SetSpecAb(SA_VorpalAttack,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_HOLDPERSON: pDude->SetSpecAb(SA_HoldPerson,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_SILENCE: pDude->SetSpecAb(SA_Silence,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_POISONED: pDude->SetSpecAb(SA_Poisoned,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_SLOWPOISON: pDude->SetSpecAb(SA_SlowPoison,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_MIRRORIMAGE: pDude->SetSpecAb(SA_MirrorImage,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_INVISIBLE: pDude->SetSpecAb(SA_Invisible,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_ENFEEBLED: pDude->SetSpecAb(SA_Enfeebled,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_BLINDNESS: pDude->SetSpecAb(SA_Blindness,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  case CHAR_DISEASED: pDude->SetSpecAb(SA_Diseased,data,SPELL_EFFECTS_DATA::EFFECT_SPELLSPECAB); break;
  }
}

void SetCharacterUBYTE(const IF_KEYWORD_DATA *pData, unsigned char data,ActorType *pActor)
{
}

void SetCharacterWORD(const IF_KEYWORD_DATA *pData, short data,ActorType *pActor)
{
}

void SetCharacterUWORD(const IF_KEYWORD_DATA *pData, unsigned short data,ActorType *pActor)
{
}

void SetCharacterDWORD(const IF_KEYWORD_DATA *pData, long data,ActorType *pActor)
{
  //CHARACTER &dude = GetCharacterContext();
  CHARACTER *pDude = GetCurrentlyActiveContext(pActor);
  if (pDude==NULL) return;
  switch (pData->keyindex)
  {
  case CHAR_AC: pDude->SetAC(data); break;
  case CHAR_HITPOINTS: pDude->SetHitPoints(data); break;
  case CHAR_THACO: pDude->SetTHACO(data); break;
  case CHAR_RDYTOTRAIN: pDude->SetReadyToTrain(data); break;
  case CHAR_CLERICEXP: pDude->SetCurrExp(ClericFlag, data); break;
  case CHAR_MAGICUSEREXP: pDude->SetCurrExp(MagicUserFlag, data); break;
  case CHAR_FIGHTEREXP: pDude->SetCurrExp(FighterFlag, data); break;
  case CHAR_RANGEREXP: pDude->SetCurrExp(RangerFlag, data); break;
  case CHAR_THIEFEXP: pDude->SetCurrExp(ThiefFlag, data); break;
  case CHAR_DRUIDEXP: pDude->SetCurrExp(DruidFlag, data); break;
  case CHAR_PALADINEXP: pDude->SetCurrExp(PaladinFlag, data); break;
  case CHAR_AGE: pDude->SetAge(data); break;
  case CHAR_MAXAGE: pDude->SetMaxAge(data); break;
  case CHAR_MAXENC: pDude->SetMaxEncumbrace(data); break;
  case CHAR_ENC: ASSERT(FALSE); break; // read-only curr encumbrance
  case CHAR_MAGICRESIST: pDude->SetMagicResistance(data); break;
  case CHAR_SAVEVSPPDM: pDude->SetSaveVsPPDM(data); break;
  case CHAR_SAVEVSPP: pDude->SetSaveVsPP(data); break;
  case CHAR_SAVEVSRSW: pDude->SetSaveVsRSW(data); break;
  case CHAR_SAVEVSBR: pDude->SetSaveVsBr(data); break;
  case CHAR_SAVEVSSP: pDude->SetSaveVsSp(data); break;
  case CHAR_CLERICLVL: ASSERT(FALSE); break; // read-only curr level
  case CHAR_FIGHTERLVL: ASSERT(FALSE); break; // read-only
  case CHAR_RANGERLVL: ASSERT(FALSE); break; // read-only
  case CHAR_DRUIDLVL: ASSERT(FALSE); break; // read-only
  case CHAR_PALADINLVL: ASSERT(FALSE); break; // read-only
  case CHAR_THIEFLVL: ASSERT(FALSE); break; // read-only
  case CHAR_MAGUSERLVL: ASSERT(FALSE); break; // read-only
  case CHAR_CLERICPREVLVL: ASSERT(FALSE); break; // read-only prev level
  case CHAR_FIGHTERPREVLVL: ASSERT(FALSE); break; // read-only
  case CHAR_RANGERPREVLVL: ASSERT(FALSE); break; // read-only
  case CHAR_DRUIDPREVLVL: ASSERT(FALSE); break; // read-only
  case CHAR_PALADINPREVLVL: ASSERT(FALSE); break; // read-only
  case CHAR_THIEFPREVLVL: ASSERT(FALSE); break; // read-only
  case CHAR_MAGUSERPREVLVL: ASSERT(FALSE); break; // read-only
  case CHAR_CLERICPDLVL: ASSERT(FALSE); break; // read-only pre-drain level
  case CHAR_FIGHTERPDLVL: ASSERT(FALSE); break; // read-only
  case CHAR_RANGERPDLVL: ASSERT(FALSE); break; // read-only
  case CHAR_DRUIDPDLVL: ASSERT(FALSE); break; // read-only
  case CHAR_PALADINPDLVL: ASSERT(FALSE); break; // read-only
  case CHAR_THIEFPDLVL: ASSERT(FALSE); break; // read-only
  case CHAR_MAGUSERPDLVL: ASSERT(FALSE); break; // read-only
  case CHAR_MORALE: pDude->SetMorale(data); break;
  case CHAR_PICKPOCKETS: pDude->SetPickPockets(data); break;
  case CHAR_OPENLOCKS: pDude->SetOpenLocks(data); break;
  case CHAR_FINDTRAPS: pDude->SetFindTraps(data); break;
  case CHAR_MOVESILENT: pDude->SetMoveSilent(data); break;
  case CHAR_HIDESHADOWS: pDude->SetHideInShadows(data); break;
  case CHAR_HEARNOISE: pDude->SetHearNoise(data); break;
  case CHAR_CLIMBWALLS: pDude->SetClimbWalls(data); break;
  case CHAR_READLANG: pDude->SetReadLanguages(data); break;
  case CHAR_COINAMOUNT1: pDude->money.Add(PlatinumType, data); break;
  case CHAR_COINAMOUNT2: pDude->money.Add(GoldType, data); break;
  case CHAR_COINAMOUNT3: pDude->money.Add(ElectrumType, data); break;
  case CHAR_COINAMOUNT4: pDude->money.Add(SilverType, data); break;
  case CHAR_COINAMOUNT5: pDude->money.Add(CopperType, data); break;
  case CHAR_COINAMOUNT6: pDude->money.Add(CoinType6, data); break;
  case CHAR_COINAMOUNT7: pDude->money.Add(CoinType7, data); break;
  case CHAR_COINAMOUNT8: pDude->money.Add(CoinType8, data); break;
  case CHAR_COINAMOUNT9: pDude->money.Add(CoinType9, data); break;
  case CHAR_COINAMOUNT10: pDude->money.Add(CoinType10, data); break;
  case CHAR_GEMAMOUNT: { for (DWORD i=0;i<(DWORD)data;i++) pDude->money.AddGem(); } break;
  case CHAR_JEWELRYAMOUNT: { for (DWORD i=0;i<(DWORD)data;i++) pDude->money.AddJewelry(); } break;
  }
}

void SetCharacterUDWORD(const IF_KEYWORD_DATA *pData, unsigned long data,ActorType *pActor)
{
}

void SetCharacterFLOAT(const IF_KEYWORD_DATA *pData, float data,ActorType *pActor)
{
  switch (pData->keyindex)
  {
  case CHAR_NBRHITDICE: ASSERT(FALSE); break; // read-only nbr hitdice
  case CHAR_NBRATTACKS: ASSERT(FALSE); break; // read-only nbr attacks
  }
}


const char *GetPartySTRING(const IF_KEYWORD_DATA *pData)
{
  return NULL;
}

const char GetPartyBYTE(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const unsigned char GetPartyUBYTE(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const short GetPartyWORD(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const unsigned short GetPartyUWORD(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const long GetPartyDWORD(const IF_KEYWORD_DATA *pData)
{
  switch (pData->keyindex)
  {
  case PARTY_FACING: return party.facing; break;
  case PARTY_DAYS: return party.days; break;
  case PARTY_HOURS: return party.hours; break;
  case PARTY_MINUTES: return party.minutes; break;
  case PARTY_ACTIVECHAR: return party.activeCharacter; break;
  case PARTY_MONEYAVAILABLE: 
    {
      double total = 0;
      for (int i=0;i<party.numCharacters;i++)
        total += party.characters[i].money.Total();
      return (DWORD)total;
    }
    break;
  }
  return 0;
}

const unsigned long GetPartyUDWORD(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const float GetPartyFLOAT(const IF_KEYWORD_DATA *pData)
{
  return 0.0;
}

void SetPartySTRING(const IF_KEYWORD_DATA *pData, const char *data)
{
}

void SetPartyBYTE(const IF_KEYWORD_DATA *pData, char data)
{
}

void SetPartyUBYTE(const IF_KEYWORD_DATA *pData, unsigned char data)
{
}

void SetPartyWORD(const IF_KEYWORD_DATA *pData, short data)
{
}

void SetPartyUWORD(const IF_KEYWORD_DATA *pData, unsigned short data)
{
}

void SetPartyDWORD(const IF_KEYWORD_DATA *pData, long data)
{
  switch (pData->keyindex)
  {
  case PARTY_FACING: 
    if ((data >= FACE_NORTH) && (data <= FACE_SE))
      party.facing = data;
    break;
  case PARTY_DAYS: party.days = data; break;
  case PARTY_HOURS: party.hours = (data%24); break;
  case PARTY_MINUTES: party.minutes = (data%60); break;
  case PARTY_ACTIVECHAR: party.activeCharacter = (data%party.numCharacters); break;
  case PARTY_MONEYAVAILABLE: /* is read-only */ break;
  }
}

void SetPartyUDWORD(const IF_KEYWORD_DATA *pData, unsigned long data)
{
}

void SetPartyFLOAT(const IF_KEYWORD_DATA *pData, float data)
{
}



const char *GetGameSTRING(const IF_KEYWORD_DATA *pData)
{
  switch (pData->keyindex)
  {
  case GAME_COINNAME1: return globalData.moneyData.GetName(PlatinumType); break;
  case GAME_COINNAME2: return globalData.moneyData.GetName(GoldType); break;
  case GAME_COINNAME3: return globalData.moneyData.GetName(ElectrumType); break;
  case GAME_COINNAME4: return globalData.moneyData.GetName(SilverType); break;
  case GAME_COINNAME5: return globalData.moneyData.GetName(CopperType); break;
  case GAME_COINNAME6: return globalData.moneyData.GetName(CoinType6); break;
  case GAME_COINNAME7: return globalData.moneyData.GetName(CoinType7); break;
  case GAME_COINNAME8: return globalData.moneyData.GetName(CoinType8); break;
  case GAME_COINNAME9: return globalData.moneyData.GetName(CoinType9); break;
  case GAME_COINNAME10: return globalData.moneyData.GetName(CoinType10); break;
  case GAME_GEMNAME: return globalData.moneyData.GetName(GemType); break;
  case GAME_JEWELRYNAME: return globalData.moneyData.GetName(JewelryType); break;  
  }

  return NULL;
}

const char GetGameBYTE(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const unsigned char GetGameUBYTE(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const short GetGameWORD(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const unsigned short GetGameUWORD(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const long GetGameDWORD(const IF_KEYWORD_DATA *pData)
{
  //long index;
  //index = KeywordTextToNum(keyword);
  switch (pData->keyindex)
  {
  case GAME_CURRLEVEL: return globalData.currLevel;
  }
  return 0;
}

const unsigned long GetGameUDWORD(const IF_KEYWORD_DATA *pData)
{
  return 0;
}

const float GetGameFLOAT(const IF_KEYWORD_DATA *pData)
{
  switch (pData->keyindex)
  {
  case GAME_VERSION: return globalData.version;
  case GAME_VAULTMONEYAVAILABLE: return globalData.VaultTotal(); break;
  }
  return 0.0;
}



void SetGameSTRING(const IF_KEYWORD_DATA *pData, const char *data)
{
}

void SetGameBYTE(const IF_KEYWORD_DATA *pData, char data)
{
}

void SetGameUBYTE(const IF_KEYWORD_DATA *pData, unsigned char data)
{
}

void SetGameWORD(const IF_KEYWORD_DATA *pData, short data)
{
}

void SetGameUWORD(const IF_KEYWORD_DATA *pData, unsigned short data)
{
}

void SetGameDWORD(const IF_KEYWORD_DATA *pData, long data)
{
  //long index;
  //index = KeywordTextToNum(keyword);
  //switch (index)
  //{
  //case GAME_CURRLEVEL: globalData.currLevel = data; break; /* read-only */
  //}
}

void SetGameUDWORD(const IF_KEYWORD_DATA *pData, unsigned long data)
{
}

void SetGameFLOAT(const IF_KEYWORD_DATA *pData, float data)
{
  //long index;
  //index = KeywordTextToNum(keyword);
  //switch (index)
  //{
  //case GAME_VERSION: globalData.version = data; break;
  //}
}
  

const char *GetSTRING(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  if (pData==NULL) return NULL;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  return GetCharacterSTRING(pData,pActor);
  case PARTY_IF_TYPE: return GetPartySTRING(pData);
  case GAME_IF_TYPE:  return GetGameSTRING(pData);
  }
  return NULL;
}

const char GetBYTE(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  if (pData==NULL) return 0;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  return GetCharacterBYTE(pData,pActor);
  case PARTY_IF_TYPE: return GetPartyBYTE(pData);
  case GAME_IF_TYPE:  return GetGameBYTE(pData);
  }
  return 0;
}

const unsigned char GetUBYTE(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  if (pData==NULL) return 0;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  return GetCharacterUBYTE(pData,pActor);
  case PARTY_IF_TYPE: return GetPartyUBYTE(pData);
  case GAME_IF_TYPE:  return GetGameUBYTE(pData);
  }
  return 0;
}

const short GetWORD(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  if (pData==NULL) return 0;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  return GetCharacterWORD(pData,pActor);
  case PARTY_IF_TYPE: return GetPartyWORD(pData);
  case GAME_IF_TYPE:  return GetGameWORD(pData);
  }
  return 0;
}
const unsigned short GetUWORD(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  if (pData==NULL) return 0;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  return GetCharacterUWORD(pData,pActor);
  case PARTY_IF_TYPE: return GetPartyUWORD(pData);
  case GAME_IF_TYPE:  return GetGameUWORD(pData);
  }
  return 0;
}

const long GetDWORD(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  if (pData==NULL) return 0;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  return GetCharacterDWORD(pData,pActor);
  case PARTY_IF_TYPE: return GetPartyDWORD(pData);
  case GAME_IF_TYPE:  return GetGameDWORD(pData);
  }
  return 0;
}

const unsigned long GetUDWORD(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  if (pData==NULL) return 0;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  return GetCharacterUDWORD(pData,pActor);
  case PARTY_IF_TYPE: return GetPartyUDWORD(pData);
  case GAME_IF_TYPE:  return GetGameUDWORD(pData);
  }
  return 0;
}

const float GetFLOAT(const IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  if (pData==NULL) return 0;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  return GetCharacterFLOAT(pData,pActor);
  case PARTY_IF_TYPE: return GetPartyFLOAT(pData);
  case GAME_IF_TYPE:  return GetGameFLOAT(pData);
  }
  return 0.0;
}

void SetSTRING(const IF_KEYWORD_DATA *pData, const char *data,ActorType *pActor)
{
  if (pData==NULL) return;
  if (data==NULL) return;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  SetCharacterSTRING(pData,data,pActor); break;
  case PARTY_IF_TYPE: SetPartySTRING(pData,data); break;
  case GAME_IF_TYPE:  SetGameSTRING(pData,data); break;
  }
}

void SetBYTE(const IF_KEYWORD_DATA *pData, char data,ActorType *pActor)
{
  if (pData==NULL) return;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  SetCharacterBYTE(pData,data,pActor); break;
  case PARTY_IF_TYPE: SetPartyBYTE(pData,data); break;
  case GAME_IF_TYPE:  SetGameBYTE(pData,data); break;
  }
}

void SetUBYTE(const IF_KEYWORD_DATA *pData, unsigned char data,ActorType *pActor)
{
  if (pData==NULL) return;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  SetCharacterUBYTE(pData,data,pActor); break;
  case PARTY_IF_TYPE: SetPartyUBYTE(pData,data); break;
  case GAME_IF_TYPE:  SetGameUBYTE(pData,data); break;
  }
}

void SetWORD(const IF_KEYWORD_DATA *pData, short data,ActorType *pActor)
{
  if (pData==NULL) return;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  SetCharacterWORD(pData,data,pActor); break;
  case PARTY_IF_TYPE: SetPartyWORD(pData,data); break;
  case GAME_IF_TYPE:  SetGameWORD(pData,data); break;
  }
}

void SetUWORD(const IF_KEYWORD_DATA *pData, unsigned short data,ActorType *pActor)
{
  if (pData==NULL) return;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  SetCharacterUWORD(pData,data,pActor); break;
  case PARTY_IF_TYPE: SetPartyUWORD(pData,data); break;
  case GAME_IF_TYPE:  SetGameUWORD(pData,data); break;
  }
}

void SetDWORD(const IF_KEYWORD_DATA *pData, long data,ActorType *pActor)
{
  if (pData==NULL) return;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  SetCharacterDWORD(pData,data,pActor); break;
  case PARTY_IF_TYPE: SetPartyDWORD(pData,data); break;
  case GAME_IF_TYPE:  SetGameDWORD(pData,data); break;
  }
}

void SetUDWORD(const IF_KEYWORD_DATA *pData, unsigned long data,ActorType *pActor)
{
  if (pData==NULL) return;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  SetCharacterUDWORD(pData,data,pActor); break;
  case PARTY_IF_TYPE: SetPartyUDWORD(pData,data); break;
  case GAME_IF_TYPE:  SetGameUDWORD(pData,data); break;
  }
}

void SetFLOAT(const IF_KEYWORD_DATA *pData, float data,ActorType *pActor)
{
  if (pData==NULL) return;
  switch (pData->iftype)
  {
  case CHAR_IF_TYPE:  SetCharacterFLOAT(pData,data,pActor); break;
  case PARTY_IF_TYPE: SetPartyFLOAT(pData,data); break;
  case GAME_IF_TYPE:  SetGameFLOAT(pData,data); break;
  }
}


/*          These are the public functions        */

CString GetDataAsCString(IF_KEYWORD_DATA *pData,ActorType *pActor)
{
  if ((pActor != NULL) && (*pActor==NULL_ACTOR))
    return CString("");
  if (pData == NULL) return CString("");
  CString result("");
  switch (pData->type)
  {
  case IF_TYPE_STRING: 
    {
      const char *pSTRING = GetSTRING(pData,pActor);
      if (pSTRING != NULL) result = pSTRING;
    }
    break;
  case IF_TYPE_BYTE:   result.Format("%i", GetBYTE(pData,pActor));     break;
  case IF_TYPE_WORD:   result.Format("%i", GetWORD(pData,pActor));     break;
  case IF_TYPE_DWORD:  result.Format("%i", GetDWORD(pData,pActor));    break;
  case IF_TYPE_FLOAT:  result.Format("%1.8f", GetFLOAT(pData,pActor)); break;
  case IF_TYPE_UBYTE:  result.Format("%u", GetUBYTE(pData,pActor));    break;
  case IF_TYPE_UWORD:  result.Format("%i", GetUWORD(pData,pActor));    break;
  case IF_TYPE_UDWORD: result.Format("%i", GetUDWORD(pData,pActor));   break;
  }
  return result;
}

extern "C" 
{

const char *GetDataSTRING(const unsigned long keyword,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_STRING))
    return GetSTRING(pData,pActor);
  return NULL;
}

char GetDataBYTE(const unsigned long keyword,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_BYTE))
    return GetBYTE(pData,pActor);
  return 0;
}

short GetDataWORD(const unsigned long keyword,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_WORD))
    return GetWORD(pData,pActor);
  return 0;
}

long GetDataDWORD(const unsigned long keyword,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_DWORD))
    return GetDWORD(pData,pActor);
  return 0;
}

unsigned char GetDataUBYTE(const unsigned long keyword,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_UBYTE))
    return GetUBYTE(pData,pActor);
  return 0;
}

unsigned short GetDataUWORD(const unsigned long keyword,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_UWORD))
    return GetUWORD(pData,pActor);
  return 0;
}

unsigned long GetDataUDWORD(const unsigned long keyword,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_UDWORD))
    return GetUDWORD(pData,pActor);
  return 0;
}

float GetDataFLOAT(const unsigned long keyword,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_UDWORD))
    return GetFLOAT(pData,pActor);
  return 0.0;
}

int GetDataAsText(const unsigned long keyword, char *buf, int blen,ActorType *pActor)
{
  if ((buf == NULL)||(blen==0)) return 0;
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if (pData == NULL) return 0;
  char temp[16];
  switch (pData->type)
  {
  case IF_TYPE_STRING: 
    {
      const char *pSTRING = GetSTRING(pData,pActor);
      if (pSTRING != NULL)
      {
        strncpy(buf, pSTRING, blen); 
        buf[blen-1]='\0'; 
      }
      else
      {
        buf[0]='\0';
        return 0;
      }
    }
    break;
  case IF_TYPE_BYTE:   
    sprintf(temp, "%i", GetBYTE(pData,pActor));   
    strncpy(buf, temp, blen); 
    buf[blen-1]='\0'; 
    break;
  case IF_TYPE_WORD:   
    sprintf(temp, "%i", GetWORD(pData,pActor));   
    strncpy(buf, temp, blen); 
    buf[blen-1]='\0'; 
    break;
  case IF_TYPE_DWORD:  
    sprintf(temp, "%i", GetDWORD(pData,pActor));  
    strncpy(buf, temp, blen); 
    buf[blen-1]='\0'; 
    break;
  case IF_TYPE_FLOAT:  
    sprintf(temp, "%1.*f", 14, GetFLOAT(pData,pActor));  
    strncpy(buf, temp, blen); 
    buf[blen-1]='\0'; 
    break;
  case IF_TYPE_UBYTE:  
    sprintf(temp, "%u", GetUBYTE(pData,pActor));  
    strncpy(buf, temp, blen); 
    buf[blen-1]='\0'; 
    break;
  case IF_TYPE_UWORD:  
    sprintf(temp, "%i", GetUWORD(pData,pActor));  
    strncpy(buf, temp, blen); 
    buf[blen-1]='\0'; 
    break;
  case IF_TYPE_UDWORD: 
    sprintf(temp, "%i", GetUDWORD(pData,pActor)); 
    strncpy(buf, temp, blen); 
    buf[blen-1]='\0'; 
    break;
  default: return 0; break;
  }
  return 1;
}

int text2long(char *value)
{
  int result = 0;
  int sign = +1;
  while (*value != 0)
  {
    if (*value == '-') sign *= -1;
    else if ((*value >= '0') && (*value <= '9'))
    {
      result = 10*result + *value - '0';
    };
    value++;
  };
  return sign * result;
}

double text2float(char *value)
{
  double result = 0.0;
  double sign = 1.0;
  double scale = 1.0;
  int decimal = 0;
  while (*value != 0)
  {
    if (*value == '-') sign = -sign;
    else if (*value == '.') decimal = 1;
    else if ((*value >= '0') && (*value <= '9'))
    {
      if (decimal) scale *= 10;
      result = 10 * result + (*value -'0');
    };
    value++;
  };
  return sign * result / scale;
}

void SetDataSTRING(const unsigned long keyword, const char *string,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if (   (pData != NULL) && (pData->type == IF_TYPE_STRING) && (!pData->readonly))
    SetSTRING(pData, string,pActor);
}

void SetDataBYTE(const unsigned long keyword, char data,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_BYTE) && (!pData->readonly))
    SetBYTE(pData, data,pActor);
}

void SetDataWORD(const unsigned long keyword, short data,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_WORD) && (!pData->readonly))
    SetWORD(pData, data,pActor);
}

void SetDataDWORD(const unsigned long keyword, long data,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_DWORD) && (!pData->readonly))
    SetDWORD(pData, data,pActor);
}

void SetDataUBYTE(const unsigned long keyword, unsigned char data,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_UBYTE) && (!pData->readonly))
    SetUBYTE(pData, data,pActor);
}

void SetDataUWORD(const unsigned long keyword, unsigned short data,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_UWORD) && (!pData->readonly))
    SetUWORD(pData, data,pActor);
}

void SetDataUDWORD(const unsigned long keyword, unsigned long data,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_UDWORD) && (!pData->readonly))
    SetUDWORD(pData, data,pActor);
}

void SetDataFLOAT(const unsigned long keyword, float data,ActorType *pActor)
{
  IF_KEYWORD_DATA *pData = LocateKeywordData(keyword);
  if ((pData != NULL) && (pData->type == IF_TYPE_FLOAT) && (!pData->readonly))
    SetFLOAT(pData, data,pActor);
}

} // extern "C" 

void SetDataFromCString(IF_KEYWORD_DATA *pData, CString value,ActorType *pActor)
{
  if (pData == NULL) return;
  switch (pData->type)
  {
  case IF_TYPE_STRING: if (value.GetLength()>0) SetSTRING(pData, value.GetBuffer(0) ,pActor); break;
  case IF_TYPE_BYTE:   SetBYTE  (pData,text2long(value.GetBuffer(0)) ,pActor); break;
  case IF_TYPE_WORD:   SetWORD  (pData,text2long(value.GetBuffer(0)) ,pActor); break;
  case IF_TYPE_DWORD:  SetDWORD (pData,text2long(value.GetBuffer(0)) ,pActor); break;
  case IF_TYPE_FLOAT:  SetFLOAT (pData,text2float(value.GetBuffer(0)),pActor); break;
  case IF_TYPE_UBYTE:  SetUBYTE (pData,text2long(value.GetBuffer(0)) ,pActor); break;
  case IF_TYPE_UWORD:  SetUWORD (pData,text2long(value.GetBuffer(0)) ,pActor); break;
  case IF_TYPE_UDWORD: SetUDWORD(pData,text2long(value.GetBuffer(0)) ,pActor); break;
  }
  return;
}

//#endif