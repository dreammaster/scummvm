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

#include "dungeoncraft/shared/..\shared\stdafx.h"

#ifdef UAFEDITOR
#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
#else
//#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/..\UAFWin\Dungeon.h"
#include "dungeoncraft/shared/..\UAFWin\DgnGame.h"
#endif

#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/Graphics.h"
#include "dungeoncraft/shared/PicSlot.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/Level.h"
#include "dungeoncraft/shared/SoundMgr.h"
#include "dungeoncraft/shared/monster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( GLOBAL_SOUND_DATA, CObject, 1 )
IMPLEMENT_SERIAL( SPECIAL_OBJECT_DATA, CObject, 1 )
IMPLEMENT_SERIAL( SPECIAL_OBJECT_LIST, CObject, 1 )
IMPLEMENT_SERIAL( QUEST_DATA, CObject, 1 )
IMPLEMENT_SERIAL( QUEST_LIST, CObject, 1 )
IMPLEMENT_SERIAL( GLOBAL_VAULT_DATA, CObject, 1 )
//IMPLEMENT_SERIAL( BACKGROUND_SOUNDS, CObject, 1 )
IMPLEMENT_SERIAL( LEVEL_STATS, CObject, 1 )
IMPLEMENT_SERIAL( LEVEL_INFO, CObject, 1 )
IMPLEMENT_SERIAL( GLOBAL_STATS, CObject, 1 )

GLOBAL_STATS globalData;

void FillDefaultFontData(LPCSTR font, BYTE size, LOGFONT *plf)
{
  memset(plf,0,sizeof(LOGFONT));
	CFont Font;
  Font.CreateFont(size,0,0,0,
		              FW_BOLD,
		              FALSE,
		              FALSE,
		              FALSE,
		              ANSI_CHARSET,
		              OUT_DEFAULT_PRECIS,
		              CLIP_DEFAULT_PRECIS,
		              NONANTIALIASED_QUALITY,
		              VARIABLE_PITCH,
		              font);
  Font.GetLogFont(plf);
}

//*****************************************************************************

void TITLE_SCREEN::Serialize(CArchive &ar)
{
  if (ar.IsStoring())
  {
    StripFilenamePath(TitleBgArt);
    AS(ar, TitleBgArt);
    AddDesignFolderToPath(TitleBgArt);
    ar << UseTrans;
    ar << UseBlend;
    ar << DisplayBy;
  }
  else
  {
    DAS(ar, TitleBgArt);
    AddDesignFolderToPath(TitleBgArt);
    ar >> UseTrans;
    ar >> UseBlend;
    ar >> DisplayBy;
  }
}

void TITLE_SCREEN::Serialize(CAR &ar)
{
  if (ar.IsStoring())
  {
    StripFilenamePath(TitleBgArt);
    AS(ar, TitleBgArt);
    AddDesignFolderToPath(TitleBgArt);
    ar << UseTrans;
    ar << UseBlend;
    ar << DisplayBy;
  }
  else
  {
    DAS(ar, TitleBgArt);
    AddDesignFolderToPath(TitleBgArt);
    ar >> UseTrans;
    ar >> UseBlend;
    ar >> DisplayBy;
  }
}

TITLE_SCREEN& TITLE_SCREEN::operator =(const TITLE_SCREEN& src)
{
  if (this==&src) return *this;
  Clear();
  TitleBgArt=src.TitleBgArt;
  UseTrans=src.UseTrans;
  UseBlend=src.UseBlend;
  DisplayBy=src.DisplayBy;
  return *this;
}

//*****************************************************************************

void TITLE_SCREEN_DATA::Serialize(CArchive &ar)
{
  DWORD count;
  if (ar.IsStoring())
  {
    ar << Timeout;
    count = Titles.GetCount();
    ar << count;
    POSITION pos = Titles.GetHeadPosition();
    while (pos != NULL)
      Titles.GetNext(pos).Serialize(ar);
  }
  else
  {
    ar >> Timeout;
    TITLE_SCREEN data;
    ar >> count;
    for (DWORD i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar);
      Titles.AddTail(data);
    }
  }
}

void TITLE_SCREEN_DATA::Serialize(CAR &ar)
{
  DWORD count;
  if (ar.IsStoring())
  {
    ar << Timeout;
    count = Titles.GetCount();
    ar << count;
    POSITION pos = Titles.GetHeadPosition();
    while (pos != NULL)
      Titles.GetNext(pos).Serialize(ar);
  }
  else
  {
    ar >> Timeout;
    TITLE_SCREEN data;
    ar >> count;
    for (DWORD i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar);
      Titles.AddTail(data);
    }
  }
}

TITLE_SCREEN_DATA& TITLE_SCREEN_DATA::operator =(const TITLE_SCREEN_DATA& src)
{
  if (this==&src) return *this;
  Clear();
  Timeout=src.Timeout;
  POSITION pos = src.Titles.GetHeadPosition();
  while (pos != NULL)
  {
    TITLE_SCREEN data = src.Titles.GetNext(pos);
    AddTitle(data);
  }
  return *this;
}

POSITION TITLE_SCREEN_DATA::AddTitle(TITLE_SCREEN &data)
{
  return (Titles.AddTail(data));
}

void TITLE_SCREEN_DATA::SetDefault(CString deftitle)
{
  TITLE_SCREEN data;
  data.TitleBgArt = deftitle;
  data.UseTrans=FALSE;
  data.DisplayBy=TITLE_SCREEN::tsFadeIn;
  Timeout=0;
  AddTitle(data);
}

//*****************************************************************************

void SPELL_SPECIAL_DATA::Serialize(CArchive &ar)
{
  POSITION pos;
  if (ar.IsStoring())
  {
    ar << m_DetectMagicSpells.GetCount();
    pos = m_DetectMagicSpells.GetHeadPosition();
    while (pos!=NULL)
      ar << m_DetectMagicSpells.GetNext(pos);

    ar << m_IdentifySpells.GetCount();
    pos = m_IdentifySpells.GetHeadPosition();
    while (pos!=NULL)
      ar << m_IdentifySpells.GetNext(pos);
  }
  else
  {
    Clear();

    DWORD count;
    DWORD temp;
    DWORD i;
    ar >> count;
    for (i=0;i<count;i++)
    {
      ar >> temp;
      m_DetectMagicSpells.AddTail(temp);
    }

    ar >> count;
    for (i=0;i<count;i++)
    {
      ar >> temp;
      m_IdentifySpells.AddTail(temp);
    }
  }
}

void SPELL_SPECIAL_DATA::Serialize(CAR &ar)
{
  POSITION pos;
  if (ar.IsStoring())
  {
    ar << m_DetectMagicSpells.GetCount();
    pos = m_DetectMagicSpells.GetHeadPosition();
    while (pos!=NULL)
      ar << m_DetectMagicSpells.GetNext(pos);

    ar << m_IdentifySpells.GetCount();
    pos = m_IdentifySpells.GetHeadPosition();
    while (pos!=NULL)
      ar << m_IdentifySpells.GetNext(pos);
  }
  else
  {
    Clear();

    DWORD count;
    DWORD temp;
    DWORD i;
    ar >> count;
    for (i=0;i<count;i++)
    {
      ar >> temp;
      m_DetectMagicSpells.AddTail(temp);
    }

    ar >> count;
    for (i=0;i<count;i++)
    {
      ar >> temp;
      m_IdentifySpells.AddTail(temp);
    }
  }
}

SPELL_SPECIAL_DATA& SPELL_SPECIAL_DATA::operator =(const SPELL_SPECIAL_DATA& src)
{
  if (&src==this) return *this;
  Clear();
  POSITION pos;

  pos = src.m_DetectMagicSpells.GetHeadPosition();
  while (pos!=NULL)
    m_DetectMagicSpells.AddTail(src.m_DetectMagicSpells.GetNext(pos));

  pos = src.m_IdentifySpells.GetHeadPosition();
  while (pos!=NULL)
    m_IdentifySpells.AddTail(src.m_IdentifySpells.GetNext(pos));

  return *this;
}

DWORD SPELL_SPECIAL_DATA::HasDetectMagicSpell(CHARACTER &data)
{
  if (m_DetectMagicSpells.GetCount()==0) return NO_SPELL;
  POSITION pos = m_DetectMagicSpells.GetHeadPosition();
  while (pos!=NULL)
  {
    if (data.spells.isMemorized(m_DetectMagicSpells.GetAt(pos)))
      return m_DetectMagicSpells.GetAt(pos);
    m_DetectMagicSpells.GetNext(pos);
  }
  return NO_SPELL;
}

DWORD SPELL_SPECIAL_DATA::HasIdentifySpell(CHARACTER &data)
{
  if (m_IdentifySpells.GetCount()==0) return NO_SPELL;
  POSITION pos = m_IdentifySpells.GetHeadPosition();
  while (pos!=NULL)
  {
    if (data.spells.isMemorized(m_IdentifySpells.GetAt(pos)))
      return m_IdentifySpells.GetAt(pos);
    m_IdentifySpells.GetNext(pos);
  }
  return NO_SPELL;
}

//*****************************************************************************

void DIFFICULTY_DATA::Serialize(CArchive &ar)
{
  if (ar.IsStoring())
  {
//    if (m_Name.IsEmpty()) m_Name="*";
//    ar << m_Name;
    AS(ar,m_Name);
    ar << m_ModifyHD;
    ar << m_ModifyQty;
    ar << m_ModifyMonsterExp;
    ar << m_ModifyAllExp;
    ar << m_HDAmount;
    ar << m_QtyAmount;
    ar << m_MExpAmount;
    ar << m_AExpAmount;
//    if (m_Name=="*") m_Name="";
  }
  else
  {
//    ar >> m_Name;
    DAS(ar,m_Name);
    ar >> m_ModifyHD;
    ar >> m_ModifyQty;
    ar >> m_ModifyMonsterExp;
    ar >> m_ModifyAllExp;
    ar >> m_HDAmount;
    ar >> m_QtyAmount;
    ar >> m_MExpAmount;
    ar >> m_AExpAmount;
//    if (m_Name=="*") m_Name="";
  }
}

void DIFFICULTY_DATA::Serialize(CAR &ar)
{
  if (ar.IsStoring())
  {
//    if (m_Name.IsEmpty()) m_Name="*";
    AS(ar,m_Name);
    ar << m_Name;
    ar << m_ModifyHD;
    ar << m_ModifyQty;
    ar << m_ModifyMonsterExp;
    ar << m_ModifyAllExp;
    ar << m_HDAmount;
    ar << m_QtyAmount;
    ar << m_MExpAmount;
    ar << m_AExpAmount;
//    if (m_Name=="*") m_Name="";
  }
  else
  {
//    ar >> m_Name;
    DAS(ar,m_Name);
    ar >> m_ModifyHD;
    ar >> m_ModifyQty;
    ar >> m_ModifyMonsterExp;
    ar >> m_ModifyAllExp;
    ar >> m_HDAmount;
    ar >> m_QtyAmount;
    ar >> m_MExpAmount;
    ar >> m_AExpAmount;
//    if (m_Name=="*") m_Name="";
  }
}

DIFFICULTY_DATA& DIFFICULTY_DATA::operator=(const DIFFICULTY_DATA& src)
{
  if (&src==this) return *this;
  m_Name=src.m_Name;
  m_ModifyHD=src.m_ModifyHD;
  m_ModifyQty=src.m_ModifyQty;
  m_ModifyMonsterExp=src.m_ModifyMonsterExp;
  m_ModifyAllExp=src.m_ModifyAllExp;
  m_HDAmount=src.m_HDAmount;
  m_QtyAmount=src.m_QtyAmount;
  m_MExpAmount=src.m_MExpAmount;
  m_AExpAmount=src.m_AExpAmount;
  return *this;
}
/*                   qty   hd  mexp  aexp
   { "NOVICE", 0 },  -50  -20  -30   -10
   { "SQUIRE", 0 },  -25  -10  -15    -5
   { "VETERAN", 0 },   0    0    0     0
   { "ADEPT", 0 },   +25  +10  +15    +5
   { "CHAMPION", 0 } +50  +20  +30   +10
*/
void DIFFICULTY_LEVEL_DATA::SetUADefaults()
{
  Clear();
  m_defaultLvl=2;

  m_data[0].m_Name="NOVICE";
  m_data[0].m_ModifyQty=TRUE;
  m_data[0].m_ModifyHD=TRUE;
  m_data[0].m_ModifyMonsterExp=TRUE;
  m_data[0].m_ModifyAllExp=TRUE;
  m_data[0].m_QtyAmount=-50;
  m_data[0].m_HDAmount=-20;
  m_data[0].m_MExpAmount=-30;
  m_data[0].m_AExpAmount=-10;

  m_data[1].m_Name="SQUIRE";
  m_data[1].m_ModifyQty=TRUE;
  m_data[1].m_ModifyHD=TRUE;
  m_data[1].m_ModifyMonsterExp=TRUE;
  m_data[1].m_ModifyAllExp=TRUE;
  m_data[1].m_QtyAmount=-25;
  m_data[1].m_HDAmount=-10;
  m_data[1].m_MExpAmount=-15;
  m_data[1].m_AExpAmount=-5;

  m_data[2].m_Name="VETERAN";
  m_data[2].m_ModifyQty=FALSE;
  m_data[2].m_ModifyHD=FALSE;
  m_data[2].m_ModifyMonsterExp=FALSE;
  m_data[2].m_ModifyAllExp=FALSE;
  m_data[2].m_QtyAmount=0;
  m_data[2].m_HDAmount=0;
  m_data[2].m_MExpAmount=0;
  m_data[2].m_AExpAmount=0;

  m_data[3].m_Name="ADEPT";
  m_data[3].m_ModifyQty=TRUE;
  m_data[3].m_ModifyHD=TRUE;
  m_data[3].m_ModifyMonsterExp=TRUE;
  m_data[3].m_ModifyAllExp=TRUE;
  m_data[3].m_QtyAmount=25;
  m_data[3].m_HDAmount=10;
  m_data[3].m_MExpAmount=15;
  m_data[3].m_AExpAmount=5;

  m_data[4].m_Name="CHAMPION";
  m_data[4].m_ModifyQty=TRUE;
  m_data[4].m_ModifyHD=TRUE;
  m_data[4].m_ModifyMonsterExp=TRUE;
  m_data[4].m_ModifyAllExp=TRUE;
  m_data[4].m_QtyAmount=50;
  m_data[4].m_HDAmount=20;
  m_data[4].m_MExpAmount=30;
  m_data[4].m_AExpAmount=10;
}

void DIFFICULTY_LEVEL_DATA::Serialize(CArchive &ar)
{
  if (ar.IsStoring())
  {
    ar << m_defaultLvl;
  }
  else
  {
    ar >> m_defaultLvl;
  }
  for (int i=0;i<NUM_DIFF_LVLS;i++)
    m_data[i].Serialize(ar);
}

void DIFFICULTY_LEVEL_DATA::Serialize(CAR &ar)
{
  if (ar.IsStoring())
  {
    ar << m_defaultLvl;
  }
  else
  {
    ar >> m_defaultLvl;
  }
  for (int i=0;i<NUM_DIFF_LVLS;i++)
    m_data[i].Serialize(ar);
}

DIFFICULTY_LEVEL_DATA& DIFFICULTY_LEVEL_DATA::operator=(const DIFFICULTY_LEVEL_DATA& src)
{
  if (&src==this) return *this;
  Clear();
  m_defaultLvl=src.m_defaultLvl;
  for (int i=0;i<NUM_DIFF_LVLS;i++)
    m_data[i]=src.m_data[i];
  return *this;
}

//*****************************************************************************

GLOBAL_SOUND_DATA::GLOBAL_SOUND_DATA() : hPartyBump(-1),hPartyStep(-1),hDeathMusic(-1),
                        /*hIntroMusic(-1),*/ hCharHit(-1), hCharMiss(-1)
{  
  ClearSounds();
  PartyBump="";PartyStep="";DeathMusic="";IntroMusic.Clear();
  CharHit="";CharMiss="";
  CampMusic.Clear();
}

void GLOBAL_SOUND_DATA::Clear(BOOL ctor) 
{ 
  ClearSounds();
  PartyBump="";PartyStep="";DeathMusic="";IntroMusic.Clear();
  CharHit="";CharMiss="";
  CampMusic.Clear();

#if (defined UAFEDITOR)
  if (!ctor)
  {
    CharHit.Format("%s%s", EditorArt, DEF_HIT_SOUND);
    CharMiss.Format("%s%s", EditorArt, DEF_MISS_SOUND);
    PartyBump.Format("%s%s", EditorArt, DEF_BUMP_SOUND);
    PartyStep.Format("%s%s", EditorArt, DEF_STEP_SOUND);
    DeathMusic.Format("%s%s", EditorArt, DEF_DEATH_SOUND);
    //IntroMusic.Format("%s%s", EditorArt, DEF_INTRO_SOUND);
    CString tmp;
    tmp.Format("%s%s", EditorArt, DEF_INTRO_SOUND);
    IntroMusic.sounds.AddHead(tmp);
  }
#endif
}

void GLOBAL_SOUND_DATA::ClearSounds()
{
  if (hPartyBump >= 0)
    ClearSound(hPartyBump);
  if (hPartyStep >= 0)
    ClearSound(hPartyStep);
  if (hDeathMusic >= 0)
    ClearSound(hDeathMusic);
  //if (hIntroMusic >= 0)
  //  ClearSound(hIntroMusic);
  if (hCharHit >= 0)
    ClearSound(hCharHit);
  if (hCharMiss >= 0)
    ClearSound(hCharMiss);

  hPartyBump=-1; hPartyStep=-1;hDeathMusic=-1;//hIntroMusic=-1;
  hCharHit=-1;hCharMiss=-1;
}

void GLOBAL_SOUND_DATA::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);
  if (ar.IsStoring())
  {
    StripFilenamePath(CharHit);
    StripFilenamePath(CharMiss);
    StripFilenamePath(PartyBump);
    StripFilenamePath(PartyStep);
    StripFilenamePath(DeathMusic);
    //StripFilenamePath(IntroMusic);

    AS(ar,CharHit);
    AS(ar,CharMiss);
    AS(ar,PartyBump);
    AS(ar,PartyStep);
    AS(ar,DeathMusic);

    IntroMusic.Serialize(ar);
    CampMusic.Serialize(ar);

    AddDesignFolderToPath(CharHit);
    AddDesignFolderToPath(CharMiss);
    AddDesignFolderToPath(PartyBump);
    AddDesignFolderToPath(PartyStep);
    AddDesignFolderToPath(DeathMusic);
    //AddDesignFolderToPath(IntroMusic);
  }
  else
  {
    DAS(ar,CharHit);
    DAS(ar,CharMiss);
    DAS(ar,PartyBump);
    DAS(ar,PartyStep);
    DAS(ar,DeathMusic);

    IntroMusic.Clear();
    if (globalData.version < _VERSION_0710_)
    {
      CString tmp;
      ar >> tmp;
      AddDesignFolderToPath(tmp);
      IntroMusic.sounds.AddHead(tmp);
    }
    else
      IntroMusic.Serialize(ar);

    CampMusic.Clear();
    if (globalData.version >= _VERSION_0910_)
    { 
      CampMusic.Serialize(ar);
    }

    AddDesignFolderToPath(CharHit);
    AddDesignFolderToPath(CharMiss);
    AddDesignFolderToPath(PartyBump);
    AddDesignFolderToPath(PartyStep);
    AddDesignFolderToPath(DeathMusic);
    //AddDesignFolderToPath(IntroMusic);
  }
}

void GLOBAL_SOUND_DATA::Serialize(CAR &ar)
{
  if (ar.IsStoring())
  {
    StripFilenamePath(CharHit);
    StripFilenamePath(CharMiss);
    StripFilenamePath(PartyBump);
    StripFilenamePath(PartyStep);
    StripFilenamePath(DeathMusic);
    //StripFilenamePath(IntroMusic);

    AS(ar,CharHit);
    AS(ar,CharMiss);
    AS(ar,PartyBump);
    AS(ar,PartyStep);
    AS(ar,DeathMusic);


    IntroMusic.Serialize(ar);
    CampMusic.Serialize(ar);

    AddDesignFolderToPath(CharHit);
    AddDesignFolderToPath(CharMiss);
    AddDesignFolderToPath(PartyBump);
    AddDesignFolderToPath(PartyStep);
    AddDesignFolderToPath(DeathMusic);
    //AddDesignFolderToPath(IntroMusic);
  }
  else
  {
    DAS(ar,CharHit);
    DAS(ar,CharMiss);
    DAS(ar,PartyBump);
    DAS(ar,PartyStep);
    DAS(ar,DeathMusic);

    IntroMusic.Clear(); 
    if (globalData.version < _VERSION_0710_)
    {
      CString tmp;
      ar >> tmp;
      AddDesignFolderToPath(tmp);
      IntroMusic.sounds.AddHead(tmp);
    }
    else
      IntroMusic.Serialize(ar);

    CampMusic.Clear();
    if (globalData.version >= _VERSION_0910_)
    {
      CampMusic.Serialize(ar);
    }

    AddDesignFolderToPath(CharHit);
    AddDesignFolderToPath(CharMiss);
    AddDesignFolderToPath(PartyBump);
    AddDesignFolderToPath(PartyStep);
    AddDesignFolderToPath(DeathMusic);
    //AddDesignFolderToPath(IntroMusic);
  }
}

GLOBAL_SOUND_DATA& GLOBAL_SOUND_DATA::operator =(const GLOBAL_SOUND_DATA& src)
{
  if (&src == this)
    return *this;
  hCharHit = src.hCharHit;
  hCharMiss = src.hCharMiss;
  hPartyBump = src.hPartyBump;
  hPartyStep = src.hPartyStep;
  hDeathMusic = src.hDeathMusic;
  //hIntroMusic = src.hIntroMusic;

  CharHit = src.CharHit;
  CharMiss = src.CharMiss;
  PartyBump = src.PartyBump;
  PartyStep = src.PartyStep;
  DeathMusic = src.DeathMusic;
  IntroMusic = src.IntroMusic;
  CampMusic = src.CampMusic;
  return *this;
}

//*****************************************************************************
#ifdef UAFEDITOR
CString SPECIAL_OBJECT_DATA::GetSpecialObjectText()
{
  CString tmp;
  tmp.Format("Special Item Event: %s", name);
  return tmp;
}
#endif

void SPECIAL_OBJECT_DATA::Save(CArchive& ar) 
{
	obj_asl.Save(ar);
}

void SPECIAL_OBJECT_DATA::PrepareForRestore(void) 
{
	temp_asl.Clear();
}

void SPECIAL_OBJECT_DATA::Restore(CArchive& ar) 
{
	temp_asl.Serialize(ar);
}

void SPECIAL_OBJECT_DATA::CommitRestore(void) 
{
	obj_asl.CommitRestore(temp_asl);
	temp_asl.Clear();
}

void SPECIAL_OBJECT_DATA::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);
  if (ar.IsStoring())
  {
    AS(ar,name);
    ar << id;
    ar << stage;
    ar << ExamineEvent;
    AS(ar, ExamineLabel);
    ar << CanBeDropped;
  }
  else
  {
    DAS(ar,name);
    ar >> id;
    ar >> stage;
    if (ver >= _VERSION_0810_)
    {
      ar >> ExamineEvent;
      DAS(ar,ExamineLabel);
    }
    if (ver >= _VERSION_0830_)
      ar >> CanBeDropped;
  }
  obj_asl.Serialize(ar);
}

void SPECIAL_OBJECT_DATA::Serialize(CAR &ar, double ver)
{
  if (ar.IsStoring())
  {
    AS(ar,name);
    ar << id;
    ar << stage;
    ar << ExamineEvent;
    AS(ar, ExamineLabel);
    ar << CanBeDropped;
  }
  else
  {
    DAS(ar,name);
    ar >> id;
    ar >> stage;
    if (ver >= _VERSION_0810_)
    {
      ar >> ExamineEvent;
      DAS(ar,ExamineLabel);
    }
    if (ver >= _VERSION_0830_)
      ar >> CanBeDropped;
  }
  obj_asl.Serialize(ar);
}

SPECIAL_OBJECT_DATA& SPECIAL_OBJECT_DATA::operator =(const SPECIAL_OBJECT_DATA& src)
{
  if (&src == this)
    return *this;
  name = src.name;
  id = src.id;
  stage = src.stage;
  ExamineEvent=src.ExamineEvent;
  ExamineLabel=src.ExamineLabel;
  CanBeDropped=src.CanBeDropped;
  obj_asl=src.obj_asl;
  temp_asl=src.temp_asl;
  return *this;
}

//*****************************************************************************
int SPECIAL_OBJECT_LIST::GetNextKey()
{
  int key=0;
  if (GetCount() > 0)
  {
    key = items.GetTail().id;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_MONSTERS.
      // Gaps occur when deletions are made to the monster list
      POSITION pos = items.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = items.GetNext(pos).id;
        if (currKey != prevKey+1)
        {
          key = prevKey+1;
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key++;
  }
  else
    key = 1;
  return key;
}

BOOL SPECIAL_OBJECT_LIST::CanBeDropped(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).CanBeDropped;
  else
    return FALSE;  
}

void SPECIAL_OBJECT_LIST::SetCanBeDropped(int index, BOOL val)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    items.GetAt(pos).CanBeDropped=val;
}

CString SPECIAL_OBJECT_LIST::GetExamineLabel(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).ExamineLabel;
  else
    return "";
}

DWORD SPECIAL_OBJECT_LIST::GetExamineEvent(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).ExamineEvent;
  else
    return 0;
}

void SPECIAL_OBJECT_LIST::SetExamineLabel(int index, CString label)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    items.GetAt(pos).ExamineLabel = label;
}

void SPECIAL_OBJECT_LIST::SetExamineEvent(int index, DWORD event)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    items.GetAt(pos).ExamineEvent = event;
}

BOOL SPECIAL_OBJECT_LIST::DeleteSpecialObjectEvents(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    globalData.eventData.DeleteEvents(items.GetAt(pos).ExamineEvent);
    items.GetAt(pos).ExamineLabel="EXAMINE";
    return TRUE;
  }
  else
    return FALSE;
}

void SPECIAL_OBJECT_LIST::Save(CArchive& ar) 
{
	int count;
	POSITION so_pos;
	if (globalData.version < _ASL_LEVEL_) return;
	count=items.GetCount();
	ar << count;
	so_pos=items.GetHeadPosition();
	while (so_pos!=NULL) 
	{
		ar << items.GetAt(so_pos).name;
		items.GetAt(so_pos).Save(ar);
		items.GetNext(so_pos);
	};
}

void SPECIAL_OBJECT_LIST::Restore(CArchive& ar) 
{
	int count;
	CString name;
	if (globalData.version<_ASL_LEVEL_) return;
	POSITION pos=items.GetHeadPosition();
  // Let items know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ATTRIBUTES in case no restore operation clears it.
	while (pos!=NULL) items.GetNext(pos).PrepareForRestore();
	ar >> count;
	for (int i=0; i<count; i++) {
		ar >> name;
		pos=items.GetHeadPosition();
		while (pos!=NULL)
		{
			if (name==items.GetAt(pos).name)
			{
				items.GetAt(pos).Restore(ar);
				break;
			};
			items.GetNext(pos);
		}
		if (pos==NULL)
		{
			SPECIAL_OBJECT_DATA junk;
			junk.Restore(ar); // discard
			CString msg;
			msg.Format("SAVE-FILE SPECIAL_OBJECT_DATA \"%s\" is not in design\n",
				name);
			WriteDebugString(msg);
		};
	};
}

void SPECIAL_OBJECT_LIST::CommitRestore(void) {
	POSITION p;
	p=items.GetHeadPosition();
	while (p!=NULL) items.GetNext(p).CommitRestore();
}

void SPECIAL_OBJECT_LIST::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {      
    ar << GetCount();
    POSITION pos = GetHeadPosition();
    while (pos != NULL)
      items.GetNext(pos).Serialize(ar, ver);
  }
  else
  {
    Clear();
    SPECIAL_OBJECT_DATA data;
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar, ver);
      AddItemWithCurrKey(data);
    }
  }
}

void SPECIAL_OBJECT_LIST::Serialize(CAR &ar, double ver)
{
  if (ar.IsStoring())
  {      
    ar << GetCount();
    POSITION pos = GetHeadPosition();
    while (pos != NULL)
      items.GetNext(pos).Serialize(ar, ver);
  }
  else
  {
    Clear();
    SPECIAL_OBJECT_DATA data;
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar, ver);
      AddItemWithCurrKey(data);
    }
  }
}

SPECIAL_OBJECT_LIST& SPECIAL_OBJECT_LIST::operator=(const SPECIAL_OBJECT_LIST& src)
{    
  if (&src == this)
    return *this;
  Clear();
  m_Max = const_cast<SPECIAL_OBJECT_LIST&>(src).m_Max;
  POSITION pos = const_cast<SPECIAL_OBJECT_LIST&>(src).GetHeadPosition();
  while (pos != NULL)
    AddItemWithCurrKey(const_cast<SPECIAL_OBJECT_LIST&>(src).GetNext(pos));
  return *this;
}  

BOOL SPECIAL_OBJECT_LIST::HaveItem(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  return (items.FindKeyPos(index) != NULL);
}

const CString SPECIAL_OBJECT_LIST::GetName(const int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).name;
  else
    return "No Item";
}

void SPECIAL_OBJECT_LIST::SetName(const int index, const CString &name)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    items.GetAt(pos).name = name;
}

// called when loading from scripts, or
// when adding from editor GUI
int SPECIAL_OBJECT_LIST::AddItem(SPECIAL_OBJECT_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (items.GetCount() < m_Max)
  {
    data.id = GetNextKey();
    items.Insert(data, data.id);
    return data.id;
  }
  else
    return 0;
}

// called when loading binary monster data that
// was saved with key info
int SPECIAL_OBJECT_LIST::AddItemWithCurrKey(SPECIAL_OBJECT_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (items.GetCount() < m_Max)
  {
    items.Insert(data, data.id);
    return data.id;
  }
  else
    return 0;
}

BOOL SPECIAL_OBJECT_LIST::DeleteItem(const int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    items.RemoveAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}

BOOL SPECIAL_OBJECT_LIST::GetItem(const int index, SPECIAL_OBJECT_DATA &item) 
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    item = items.GetAt(pos);
    return TRUE;
  }
  return FALSE;
}

BOOL SPECIAL_OBJECT_LIST::SetItem(const int index, const SPECIAL_OBJECT_DATA &item)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    items.GetAt(pos) = item;
    return TRUE;
  }
  return FALSE;
}

WORD SPECIAL_OBJECT_LIST::GetStage(const int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {      
    return items.GetAt(pos).stage;
  }
  return 0;
}

void SPECIAL_OBJECT_LIST::SetStage(const int index, const WORD stage)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {      
    items.GetAt(pos).stage = stage;
  }
}

int SPECIAL_OBJECT_LIST::GetItemKeyUsingSequence(int SeqNum)
{
  int count = 0;
  POSITION pos = GetHeadPosition();
  while (pos != NULL)
  {
    count++;
    if (count == SeqNum)
      return (GetAt(pos).id);
    GetNext(pos);
  }
  return 0;
}

//*****************************************************************************

void QUEST_DATA::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);
  if (ar.IsStoring())
  {
    AS(ar,name);
    ar << (int)state;
    ar << stage;
    ar << id;
  }
  else
  {
    DAS(ar,name);
    int temp;
    ar >> temp;
    state = (QuestStateType)temp;
    ar >> stage;
    ar >> id;
  }
  quest_asl.Serialize(ar);
}

void QUEST_DATA::Serialize(CAR &ar, double ver)
{
  if (ar.IsStoring())
  {
    AS(ar,name);
    ar << (int)state;
    ar << stage;
    ar << id;
  }
  else
  {
    DAS(ar,name);
    int temp;
    ar >> temp;
    state = (QuestStateType)temp;
    ar >> stage;
    ar >> id;
  }
  quest_asl.Serialize(ar);
}

QUEST_DATA& QUEST_DATA::operator=(const QUEST_DATA& src)
{
  if (&src == this)
    return *this;
  name = src.name;
  state = src.state;
  stage = src.stage;
  id = src.id;
  quest_asl=src.quest_asl;
  return *this;
}

//*****************************************************************************

int QUEST_LIST::GetNextKey()
{
  int key=0;
  if (GetCount() > 0)
  {
    key = quests.GetTail().id;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_MONSTERS.
      // Gaps occur when deletions are made to the monster list
      POSITION pos = quests.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = quests.GetNext(pos).id;
        if (currKey != prevKey+1)
        {
          key = prevKey+1;
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key++;
  }
  else
    key = 1;
  return key;
}

QUEST_LIST& QUEST_LIST::operator=(const QUEST_LIST& src)
{
  if (&src == this)
    return *this;
  Clear();
  POSITION pos = const_cast<QUEST_LIST&>(src).GetHeadPosition();
  while (pos != NULL)
    AddQuestWithCurrKey(const_cast<QUEST_LIST&>(src).GetNext(pos));
  return *this;
}

void QUEST_LIST::Reset() 
{
  POSITION pos = GetHeadPosition();
  while (pos != NULL)
    quests.GetNext(pos).Reset();
}

void QUEST_LIST::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {      
    ar << GetCount();
    POSITION pos = GetHeadPosition();
    while (pos != NULL)
      quests.GetNext(pos).Serialize(ar, ver);
  }
  else
  {
    Clear();
    QUEST_DATA data;
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar, ver);
      AddQuestWithCurrKey(data);
    }
  }
}

void QUEST_LIST::Serialize(CAR &ar, double ver)
{
  if (ar.IsStoring())
  {      
    ar << GetCount();
    POSITION pos = GetHeadPosition();
    while (pos != NULL)
      quests.GetNext(pos).Serialize(ar, ver);
  }
  else
  {
    Clear();
    QUEST_DATA data;
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar, ver);
      AddQuestWithCurrKey(data);
    }
  }
}

BOOL QUEST_LIST::HaveQuest(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  return (quests.FindKeyPos(index) != NULL);
}

const CString QUEST_LIST::GetName(const int id)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
    return quests.GetAt(pos).name;
  else
    return "No Quest";
}

void QUEST_LIST::SetName(const int id, const CString &name)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
    quests.GetAt(pos).name = name;
}

WORD QUEST_LIST::GetStage(const int id)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
    return quests.GetAt(pos).stage;
  else
    return MAX_QUEST_STAGE+1;
}

int QUEST_LIST::FindQuest(const CString& questName)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos = GetHeadPosition();
  while (pos != NULL)
  {
    if (quests.GetAt(pos).name == questName)
    {
      return quests.GetAt(pos).id;
    };
    quests.GetNext(pos);
  };
  return -1;
}

WORD QUEST_LIST::GetStage(const CString& questName)
{
  return GetStage(FindQuest(questName));
}


void QUEST_LIST::SetStage(const int id, const WORD stage)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
  {
    WORD temp = min(stage, MAX_QUEST_STAGE);
    quests.GetAt(pos).stage = temp;

    if (quests.GetAt(pos).stage > MAX_QUEST_STAGE)
      quests.GetAt(pos).stage = MAX_QUEST_STAGE;
    if (quests.GetAt(pos).stage < 0)
      quests.GetAt(pos).stage = 0;

    if (quests.GetAt(pos).stage == QUEST_COMPLETED_STAGE)
      SetComplete(id);
    else if (quests.GetAt(pos).stage == QUEST_FAILED_STAGE)
      SetFailed(id);
    else if (quests.GetAt(pos).stage == 0)
      SetNotStarted(id);
    else
      SetInProgress(id);
  }
  WriteDebugString("Quest %u, SetStage(%u)\n", id,stage);
}

void QUEST_LIST::SetStage(const CString& questName, const WORD stage)
{
  SetStage(FindQuest(questName), stage);
}

// cannot add to a quest and make it be failed
void QUEST_LIST::IncStage(const int id, const WORD value)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
  {
    if (quests.GetAt(pos).stage < QUEST_COMPLETED_STAGE)
    {
      long result = quests.GetAt(pos).stage + value;
      //quests.GetAt(pos).stage = quests.GetAt(pos).stage += value;
      if (result > QUEST_COMPLETED_STAGE) result = QUEST_COMPLETED_STAGE;
      if (result < 0) result = 0;
      quests.GetAt(pos).stage = (WORD)result;

      if (quests.GetAt(pos).stage == QUEST_COMPLETED_STAGE)
        SetComplete(id);
      else if (quests.GetAt(pos).stage == QUEST_FAILED_STAGE)
        SetFailed(id); // shouldn't get here
      else if (quests.GetAt(pos).stage == 0)
        quests.GetAt(pos).state = QS_NotStarted;
      else
        SetInProgress(id);

      WriteDebugString("Quest %u, IncStage()=%u\n", id, quests.GetAt(pos).stage);
    }
  }
}

BOOL QUEST_LIST::IsPresent(const int id)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
    return (quests.GetAt(pos).state != QS_NotStarted);
  else
    return FALSE;
}

BOOL QUEST_LIST::IsInProgress(const int id) 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
    return (quests.GetAt(pos).state == QS_InProg);
  else
    return FALSE;
}

BOOL QUEST_LIST::IsComplete(const int id) 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
    return (quests.GetAt(pos).state == QS_Complete);
  else
    return FALSE;
}

BOOL QUEST_LIST::IsFailed(const int id) 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
    return (quests.GetAt(pos).state == QS_Failed);
  else
    return FALSE;
}

void QUEST_LIST::SetInProgress(const int id) 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
  {
    quests.GetAt(pos).state = QS_InProg;
    if (quests.GetAt(pos).stage == 0)
      quests.GetAt(pos).stage = 1;
    if (quests.GetAt(pos).stage >= QUEST_COMPLETED_STAGE)
      quests.GetAt(pos).stage = 1;
    if (quests.GetAt(pos).stage == QUEST_FAILED_STAGE)
      quests.GetAt(pos).stage = 1;
  }
  WriteDebugString("Quest %u, SetInProgress()\n", id);
}

void QUEST_LIST::SetComplete(const int id) 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
  {
    quests.GetAt(pos).stage = QUEST_COMPLETED_STAGE;
    quests.GetAt(pos).state = QS_Complete;
  }
  WriteDebugString("Quest %u, SetComplete()\n", id);
}

void QUEST_LIST::SetFailed(const int id) 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
  {
    quests.GetAt(pos).stage = QUEST_FAILED_STAGE;
    quests.GetAt(pos).state = QS_Failed;
  }
  WriteDebugString("Quest %u, SetFailed()\n", id);
}

void QUEST_LIST::SetNotStarted(const int id) 
{ 
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
  {
    quests.GetAt(pos).stage = 0;
    quests.GetAt(pos).state = QS_NotStarted;
  }
  WriteDebugString("Quest %u, SetNotStarted()\n", id);
}

// called when loading from scripts, or
// when adding from editor GUI
int QUEST_LIST::AddQuest(QUEST_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (quests.GetCount() < MAX_QUESTS)
  {
    data.id = GetNextKey();
    quests.Insert(data, data.id);
    return data.id;
  }
  else
    return 0;
}

// called when loading binary monster data that
// was saved with key info
int QUEST_LIST::AddQuestWithCurrKey(QUEST_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (quests.GetCount() < MAX_QUESTS)
  {
    quests.Insert(data, data.id);
    return data.id;
  }
  else
    return 0;
}

BOOL QUEST_LIST::DeleteQuest(const int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(index)) != NULL)
  {
    quests.RemoveAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}

BOOL QUEST_LIST::GetQuest(const int id, QUEST_DATA &quest) 
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
  {
    quest = quests.GetAt(pos);
    return TRUE;
  }
  return FALSE;
}

BOOL QUEST_LIST::SetQuest(const int id, const QUEST_DATA &quest)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = quests.FindKeyPos(id)) != NULL)
  {
    quests.GetAt(pos) = quest;
    return TRUE;
  }
  return FALSE;
}

//*****************************************************************************


LEVEL_STATS& LEVEL_STATS::operator =(const LEVEL_STATS& src)
{
  if (&src == this)
    return *this;
  area_height = src.area_height;
  area_width = src.area_width;
  used = src.used;
  overland = src.overland;
  level_name = src.level_name;
  AVStyle = src.AVStyle;
  bgSounds=src.bgSounds;
  StepSound=src.StepSound;
  BumpSound=src.BumpSound;

	level_asl=src.level_asl;
  temp_asl = src.temp_asl;

  for (int i=0;i<MAX_ENTRY_POINTS;i++)
    entryPoints[i] = src.entryPoints[i];
  return *this;
}

BOOL LEVEL_STATS::operator ==(LEVEL_STATS& src)
{
  if (&src == this) return TRUE;
  if (area_height != src.area_height) return FALSE;
  if (area_width != src.area_width) return FALSE;
  if (used != src.used) return FALSE;
  if (overland != src.overland) return FALSE;
  if (level_name != src.level_name) return FALSE;
  if (AVStyle != src.AVStyle) return FALSE;
  if (!(bgSounds==src.bgSounds)) return FALSE;
  if (StepSound != src.StepSound) return FALSE;
  if (BumpSound != src.BumpSound) return FALSE;
	if ( !(level_asl==src.level_asl) ) return FALSE;
  for (int i=0;i<MAX_ENTRY_POINTS;i++)
    if ( !(entryPoints[i] == src.entryPoints[i]) ) return FALSE;
  return TRUE;
}

void LEVEL_STATS::Clear()
{
  area_height=0;area_width=0;used=FALSE;overland=FALSE;level_name="";
  StepSound="";
  BumpSound="";
  AVStyle=AnyView;
  bgSounds.Clear();
  for (int i=0;i<MAX_ENTRY_POINTS;i++)
  {
    entryPoints[i].x=0;
    entryPoints[i].y=i;
    entryPoints[i].facing=0;
  }

  level_asl.Clear();
}

void LEVEL_STATS::SetDefaults()
{
  area_height=50;
  area_width=50;
  overland=FALSE;
  AVStyle=AnyView;
  bgSounds.Clear();
  bgSounds.UseNightMusic=FALSE;
  bgSounds.EndTime=600;
  bgSounds.StartTime=1800;
}

void LEVEL_STATS::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);
  int i;

  PreSerialize(ar.IsStoring());

  if (ar.IsStoring())
  {
    ar << area_height;
    ar << area_width;
    ar << used;
    ar << overland;
    ar << (int)AVStyle;

    AS(ar,level_name);

    for (i=0;i<MAX_ENTRY_POINTS;i++)
    {
      ar << entryPoints[i].x;  
      ar << entryPoints[i].y;  
      // ar << entryPoints[i].facing;  stored as attribute
    }
    AS(ar,StepSound);
    AS(ar,BumpSound);
    bgSounds.backgroundSounds.Serialize(ar);
    bgSounds.nightSounds.Serialize(ar);
    ar << bgSounds.UseNightMusic;
    ar << bgSounds.EndTime;
    ar << bgSounds.StartTime;
  }
  else
  {
    ar >> area_height;
    ar >> area_width;
    ar >> used;
    ar >> overland;
    if (globalData.version >= _VERSION_0576_)
    {
      ar >> i;
      AVStyle = (GlobalAreaViewStyle)i;
    }

    DAS(ar,level_name);

    POINT pt;
    for (i=0;i<MAX_ENTRY_POINTS;i++)
    {
      ar >> pt;
      entryPoints[i].x = pt.x;
      entryPoints[i].y = pt.y;
      entryPoints[i].facing = 0;
    }

    if (globalData.version >= _VERSION_0640_)
    {
      DAS(ar,StepSound);
      DAS(ar,BumpSound);
    }
    else
    {
      StepSound="";
      BumpSound="";
    }

    if (globalData.version >= _VERSION_0710_)
    {
      bgSounds.backgroundSounds.Serialize(ar);
      bgSounds.nightSounds.Serialize(ar);
      ar >> bgSounds.UseNightMusic;
      ar >> bgSounds.EndTime;
      ar >> bgSounds.StartTime;
    }
  }
  level_asl.Serialize(ar);

  PostSerialize(ar.IsStoring());
}

void LEVEL_STATS::Serialize(CAR &ar)
{
  int i;
  PreSerialize(ar.IsStoring());
  if (ar.IsStoring())
  {
    ar << area_height;
    ar << area_width;
    ar << used;
    ar << overland;
    ar << (int)AVStyle;

    AS(ar,level_name);

    for (i=0;i<MAX_ENTRY_POINTS;i++)
    {
      ar << entryPoints[i].x;  
      ar << entryPoints[i].y;  
      // ar << entryPoints[i].facing;  stored as attribute
    }

    AS(ar,StepSound);
    AS(ar,BumpSound);
    bgSounds.backgroundSounds.Serialize(ar);
    bgSounds.nightSounds.Serialize(ar);
    ar << bgSounds.UseNightMusic;
    ar << bgSounds.EndTime;
    ar << bgSounds.StartTime;
  }
  else
  {
    ar >> area_height;
    ar >> area_width;
    ar >> used;
    ar >> overland;
    if (globalData.version >= _VERSION_0576_)
    {
      ar >> i;
      AVStyle = (GlobalAreaViewStyle)i;
    }

    DAS(ar,level_name);
//    ar >> level_name;
//
//    if (level_name == "*")
//      level_name="";     

    POINT pt;
    for (i=0;i<MAX_ENTRY_POINTS;i++)
    {
      ar >> pt;
      entryPoints[i].x = pt.x;
      entryPoints[i].y = pt.y;
      entryPoints[i].facing = 0;
    }
    if (globalData.version >= _VERSION_0640_)
    {
      DAS(ar,StepSound);
      DAS(ar,BumpSound);
    }
    else
    {
      StepSound="";
      BumpSound="";
    }
    if (globalData.version >= _VERSION_0710_)
    {
      bgSounds.backgroundSounds.Serialize(ar);
      bgSounds.nightSounds.Serialize(ar);
      ar >> bgSounds.UseNightMusic;
      ar >> bgSounds.EndTime;
      ar >> bgSounds.StartTime;
    }
  }
  level_asl.Serialize(ar);

  PostSerialize(ar.IsStoring());
}

void LEVEL_STATS::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    // move fields that need to be serialized
    // as attributes into the attribute map  

    if (used)
    {
      CString key;
      // Entry Point facings    
      for (int i=0;i<MAX_ENTRY_POINTS;i++)
      {
        key.Format("EPFace_%i", i);
        StoreIntAsASL(level_asl, entryPoints[i].facing, key);
      }
    }
  }
}

void LEVEL_STATS::PostSerialize(BOOL IsStoring)
{
  if (!IsStoring)
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map

    if (used)
    {
      CString key;
      int temp;
      // Entry Point facings    
      for (int i=0;i<MAX_ENTRY_POINTS;i++)
      {
        key.Format("EPFace_%i", i);
        RetrieveIntFromASL(level_asl, temp, key);
        entryPoints[i].facing = (BYTE)temp;
      }
    }
  }
}

void LEVEL_STATS::Save(CArchive& ar) 
{
	level_asl.Save(ar);
};

void LEVEL_STATS::Restore(CArchive& ar) 
{
	temp_asl.Serialize(ar);
}

void LEVEL_STATS::CommitRestore(void) 
{
	level_asl.CommitRestore(temp_asl);
	temp_asl.Clear();
}

//*****************************************************************************

LEVEL_INFO& LEVEL_INFO::operator =(const LEVEL_INFO& src)
{
 if (&src == this)
   return *this;
 numLevels = src.numLevels;
 for (int i=0;i<MAX_LEVELS;i++)
   stats[i] = src.stats[i];
 return *this;
}

void LEVEL_INFO::Clear() 
{ 
 numLevels=0; 
 for (int i=0;i<MAX_LEVELS;i++)
   Clear(i);
}

void LEVEL_INFO::Clear(int level) 
{ 
 stats[level].Clear();
 stats[level].level_name.Format("Level %i", level+1);
}

void LEVEL_INFO::SaveSounds(void)
{
  for (int i=0;i<MAX_LEVELS;i++)
  {
    saveSound(stats[i].StepSound, i);
    saveSound(stats[i].BumpSound, i);
    stats[i].bgSounds.backgroundSounds.SaveSounds(i);
    stats[i].bgSounds.nightSounds.SaveSounds(i);
  }
}

int LEVEL_INFO::GetEPX(int ep)
{
  return (stats[GetCurrentLevel()].entryPoints[ep].x); 
}

int LEVEL_INFO::GetEPY(int ep)
{
  return (stats[GetCurrentLevel()].entryPoints[ep].y); 
}

BYTE LEVEL_INFO::GetEPFacing(int ep)
{
  return (stats[GetCurrentLevel()].entryPoints[ep].facing); 
}

void LEVEL_INFO::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);

  DWORD count = 0;
  DWORD i;
  for (i=0;i<MAX_LEVELS;i++)
  {
    if (stats[i].used)
      count++;
  }

  if (ar.IsStoring())
  {
    ar << numLevels;
    ar << count;
    for (i=0;i<MAX_LEVELS;i++)
    {
      if (stats[i].used)
      {
        ar << i;
        stats[i].Serialize(ar);
      }
    }
  }
  else
  {
    ar >> numLevels;

    if (globalData.version >= _VERSION_0566_)
    {
      ar >> count;
      DWORD index;
      for (i=0;i<count;i++)
      {
        ar >> index;
        stats[index].Serialize(ar);
      }
    }
    else
    {
      for (i=0;i<MAX_LEVELS;i++)
          stats[i].Serialize(ar);
    }
  }
}

void LEVEL_INFO::Serialize(CAR &ar)
{
  DWORD count = 0;
  DWORD i;
  for (i=0;i<MAX_LEVELS;i++)
  {
    if (stats[i].used)
      count++;
  }

  if (ar.IsStoring())
  {
    ar << numLevels;
    ar << count;
    for (i=0;i<MAX_LEVELS;i++)
    {
      if (stats[i].used)
      {
        ar << i;
        stats[i].Serialize(ar);
      }
    }
  }
  else
  {
    ar >> numLevels;

    if (globalData.version >= _VERSION_0566_)
    {
      ar >> count;
      DWORD index;
      for (i=0;i<count;i++)
      {
        ar >> index;
        stats[index].Serialize(ar);
      }
    }
    else
    {
      for (i=0;i<MAX_LEVELS;i++)
          stats[i].Serialize(ar);
    }
  }
}

void LEVEL_INFO::Save(CArchive& ar) {
	for (int i=0; i<MAX_LEVELS; i++) 
	{
		stats[i].Save(ar);
	};
}

void LEVEL_INFO::Restore(CArchive& ar) 
{
	for (int i=0; i<MAX_LEVELS; i++) 
	{
		stats[i].Restore(ar);
	};
}

void LEVEL_INFO::CommitRestore(void) 
{
	for (int i=0; i<MAX_LEVELS; i++) 
	{
		stats[i].CommitRestore();
	};
}

//*****************************************************************************

GLOBAL_VAULT_DATA& GLOBAL_VAULT_DATA::operator =(const GLOBAL_VAULT_DATA& src)
{
  if (&src == this)
    return *this;

  money = src.money;
  items = src.items;
  return *this;
}

void GLOBAL_VAULT_DATA::Clear() 
{ 
  money.Clear();
  items.Clear();
}

void GLOBAL_VAULT_DATA::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);
  double ver;
  // older designs stored vault data in game.dat file
  // instead of saved game file
  //if (version < _VERSION_0661_)
  //  ver = version;
  //else
    ver = globalData.SaveGameVersion;

  if (ar.IsStoring())
  {
    money.Serialize(ar, ver);
  }
  else
  {
    money.Clear();

    if (ver < _VERSION_0661_)
    {
      int temp;
      ar >> temp; money.Coins[CopperType] = temp;
      ar >> temp; money.Coins[ElectrumType] = temp;
      ar >> temp; money.Coins[GoldType] = temp;
      ar >> temp; money.Coins[PlatinumType] = temp;
      ar >> temp; money.Coins[SilverType] = temp;
      ar >> temp; { for (int g=0;g<temp;g++) money.AddGem(); }
      ar >> temp; { for (int j=0;j<temp;j++) money.AddJewelry(); }
    }
    else
    {
      money.Serialize(ar, ver);
    }
  }

  items.Serialize(ar,ver);
}

void GLOBAL_VAULT_DATA::Serialize(CAR &ar)
{
  double ver;
  // older designs stored vault data in game.dat file
  // instead of saved game file
  //if (version < _VERSION_0661_)
  //  ver = version;
  //else
    ver = globalData.SaveGameVersion;

  if (ar.IsStoring())
  {
    money.Serialize(ar, ver);
  }
  else
  {
    money.Clear();
    if (globalData.SaveGameVersion < _VERSION_0661_)
    {
      int temp;
      ar >> temp; money.Coins[CopperType] = temp;
      ar >> temp; money.Coins[ElectrumType] = temp;
      ar >> temp; money.Coins[GoldType] = temp;
      ar >> temp; money.Coins[PlatinumType] = temp;
      ar >> temp; money.Coins[SilverType] = temp;
      ar >> temp; { for (int g=0;g<temp;g++) money.AddGem(); }
      ar >> temp; { for (int j=0;j<temp;j++) money.AddJewelry(); }
    }
    else
    {
      money.Serialize(ar, ver);
    }
  }
  items.Serialize(ar,ver);
}

//*****************************************************************************

GLOBAL_STATS& GLOBAL_STATS::operator =(const GLOBAL_STATS& src)
{
  if (&src == this) return *this;
  Clear();
  version = src.version;
  designName = src.designName;
  startLevel = src.startLevel;
  currLevel = src.currLevel;
  startX = src.startX;
  startY = src.startY;
  startFacing = src.startFacing;
  useAreaView = src.useAreaView;
  startTime = src.startTime;
  startExp = src.startExp;
  startExpType=src.startExpType;
  UNUSED_startEquip = src.UNUSED_startEquip;
  startPlatinum = src.startPlatinum;
  startGem = src.startGem;
  startJewelry = src.startJewelry;
  minPartySize = src.minPartySize;
  maxPartySize = src.maxPartySize;
  allowCharCreate = src.allowCharCreate;
  DungeonTimeDelta = src.DungeonTimeDelta;
  DungeonSearchTimeDelta = src.DungeonSearchTimeDelta;
  WildernessTimeDelta = src.WildernessTimeDelta;
  WildernessSearchTimeDelta = src.WildernessSearchTimeDelta;
  AutoDarkenViewport = src.AutoDarkenViewport;
  AutoDarkenAmount = src.AutoDarkenAmount;
  StartDarken = src.StartDarken;
  EndDarken = src.EndDarken;
  MapArt = src.MapArt;
  UseAVZones = src.UseAVZones;
  MagicAdjFont = src.MagicAdjFont;
  WhiteFont = src.WhiteFont;
  YellowFont = src.YellowFont;
  RedFont = src.RedFont;
  GreenFont = src.GreenFont;
  BlueFont = src.BlueFont;
  Custom1Font=src.Custom1Font;
  KeyboardFont = src.KeyboardFont;
  KeyboardHighlightFont=src.KeyboardHighlightFont;
  HighlightFont = src.HighlightFont;
  //TitleBgArt = src.TitleBgArt;
  titleData = src.titleData;
  IconBgArt = src.IconBgArt;
  BackgroundArt = src.BackgroundArt;
  CreditsBgArt = src.CreditsBgArt;
  MapArtSurf = src.MapArtSurf;
  BackgroundSurf=src.BackgroundSurf;
  keyData = src.keyData;
  specialItemData = src.specialItemData;
  charData = src.charData;
  
  for (int i=0;i<MAX_GLOBAL_VAULTS;i++)    
    vault[i] = src.vault[i];

  levelInfo = src.levelInfo;
  questData = src.questData;
  sounds = src.sounds;
  logfont=src.logfont;
  startEquip=src.startEquip;
  ClericEquip=src.ClericEquip;
  FighterEquip=src.FighterEquip;
  MagicUserEquip=src.MagicUserEquip;
  ThiefEquip=src.ThiefEquip;
  RangerEquip=src.RangerEquip;
  PaladinEquip=src.PaladinEquip;
  DruidEquip=src.DruidEquip;
  moneyData = src.moneyData;
  m_diffLvlData=src.m_diffLvlData;
  journalData=src.journalData;
  SaveGameVersion=src.SaveGameVersion;
  eventData = const_cast<GLOBAL_STATS&>(src).eventData;
  fixSpellBook=src.fixSpellBook;
  global_asl=src.global_asl;
  temp_asl = src.temp_asl;
  
  return *this;
}

void GLOBAL_STATS::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);
  int i;
  POSITION pos;

  if (ar.IsStoring())
  {
    ar << version;
    ar << GetDesignName();
    ar << startLevel;
    ar << startX;
    ar << startY;
    ar << startFacing;
    ar << startTime;
    ar << startExp;
    ar << startExpType;
    ar << UNUSED_startEquip;
    ar << startPlatinum;
    ar << startGem;
    ar << startJewelry;   
    
    ar << DungeonTimeDelta;
    ar << DungeonSearchTimeDelta;
    ar << WildernessTimeDelta;
    ar << WildernessSearchTimeDelta;
    ar << AutoDarkenViewport;
    ar << AutoDarkenAmount;
    ar << StartDarken;
    ar << EndDarken;
    ar << minPartySize;
    ar << maxPartySize;
    ar << allowCharCreate;    
    
    StripFilenamePath(MapArt);

    AS(ar,MapArt);
    AddDesignFolderToPath(MapArt);

    if (strlen(logfont.lfFaceName) == 0)
      FillDefaultFontData("SYSTEM", 16, &logfont);

    ar.Write(&logfont, sizeof(logfont));
         
    StripFilenamePath(IconBgArt);

    AS(ar,IconBgArt);
    AddDesignFolderToPath(IconBgArt);

    StripFilenamePath(BackgroundArt);

    AS(ar,BackgroundArt);
    AddDesignFolderToPath(BackgroundArt);

    StripFilenamePath(CreditsBgArt);

    AS(ar,CreditsBgArt);
    AddDesignFolderToPath(CreditsBgArt);

   i = m_SmallPicImport.GetCount();
   ar << i;
   pos = m_SmallPicImport.GetHeadPosition();
   while (pos != NULL)
   {
     m_SmallPicImport.GetAt(pos).picType=SmallPicDib;;
     m_SmallPicImport.GetAt(pos).SetDefaults();
     m_SmallPicImport.GetNext(pos).Serialize(ar, version);
   }

   i = m_IconPicImport.GetCount();
   ar << i;
   pos = m_IconPicImport.GetHeadPosition();
   while (pos != NULL)
   {
     m_IconPicImport.GetAt(pos).picType=IconDib;
     m_IconPicImport.GetAt(pos).SetDefaults();
     m_IconPicImport.GetNext(pos).Serialize(ar, version);
   }
   titleData.Serialize(ar);
  }
  else
  {
    titleData.Clear();

    ar >> version;
    DAS(ar,designName);
    ar >> startLevel;
    ar >> startX;
    ar >> startY;
    ar >> startFacing;
    ar >> startTime;
    ar >> startExp;
    if (version >= _VERSION_0770_)
      ar >> startExpType;
    ar >> UNUSED_startEquip;
    ar >> startPlatinum;
    ar >> startGem;
    ar >> startJewelry;

    if (version >= _VERSION_0574_)
    {
     ar >> DungeonTimeDelta;
     ar >> DungeonSearchTimeDelta;
     ar >> WildernessTimeDelta;
     ar >> WildernessSearchTimeDelta;
    }

    if (version >= _VERSION_0620_)
    {
      ar >> AutoDarkenViewport;
      ar >> AutoDarkenAmount;
      ar >> StartDarken;
      ar >> EndDarken;
    }

    if (version >= _VERSION_0575_)
    {
      ar >> minPartySize;
      ar >> maxPartySize;
      ar >> allowCharCreate;    
    }

    DAS(ar,MapArt);
    AddDesignFolderToPath(MapArt);

    if (version < _VERSION_0830_)
    {
      CString font="";
      BYTE fontSize=16;
      ar >> font;
      if (version >= _VERSION_0681_)
        ar >> fontSize;
      
      FillDefaultFontData(font, fontSize, &logfont);
    }
    else
    {
      ar.Read(&logfont, sizeof(logfont));
    }

    if (version < _VERSION_0800_)
    {
      CString TitleBgArt("");
      DAS(ar,TitleBgArt);
      AddDesignFolderToPath(TitleBgArt);
      TITLE_SCREEN data;
      data.TitleBgArt = TitleBgArt;
      data.DisplayBy = TITLE_SCREEN::tsFadeIn;
      data.UseTrans = FALSE;
      titleData.AddTitle(data);
    }

    if (version >= _VERSION_0660_)
    {
      DAS(ar,IconBgArt);
      AddDesignFolderToPath(IconBgArt);

      DAS(ar,BackgroundArt);
      AddDesignFolderToPath(BackgroundArt);
    }

    if (version >= _VERSION_0566_)
    {
      DAS(ar,CreditsBgArt);
      AddDesignFolderToPath(CreditsBgArt);
    }

    int count;
    PIC_DATA data;

    ar >> count;
    for (i=0;i<count;i++)
    {
      data.Serialize(ar, version);
      m_SmallPicImport.AddTail(data);
    }

    data.Clear();
    ar >> count;
    CString icon;
    for (i=0;i<count;i++)
    {
      if (version < _VERSION_0640_)
      {        
        DAS(ar,icon);
        AddDesignFolderToPath(icon);
        data.filename = icon;
        data.picType=IconDib;
        data.SetDefaults();
      }
      else
      {
        data.Serialize(ar, version);
      }
      m_IconPicImport.AddTail(data);
    }
    if (version >= _VERSION_0800_)
      titleData.Serialize(ar);
  }

  global_asl.Serialize(ar);
  HBarVPArt.Serialize(ar);
  VBarVPArt.Serialize(ar);
  FrameVPArt.Serialize(ar);
  VBarCombArt.Serialize(ar);
  HBarCombArt.Serialize(ar);
  CombatWallArt.Serialize(ar);
  CombatCursorArt.Serialize(ar);
  CombatDeathIconArt.Serialize(ar);
  CombatDeathArt.Serialize(ar);
  if (version >= _VERSION_0575_)
    CursorArt.Serialize(ar, version);
  sounds.Serialize(ar);
  keyData.Serialize(ar, version);
  specialItemData.Serialize(ar, version);
  questData.Serialize(ar, version);
  charData.Serialize(ar, version);

  if (version < _VERSION_0661_)
  {
    // only versions prior to 0.661 stored
    // vault data, all others store it
    // in the saved game file.
    SaveGameVersion=version;
    vault[0].Serialize(ar);
  }

  levelInfo.Serialize(ar);

  if (ar.IsStoring())
  {
    startEquip.Serialize(ar,version);
    ClericEquip.Serialize(ar,version);
    FighterEquip.Serialize(ar,version);
    MagicUserEquip.Serialize(ar,version);
    ThiefEquip.Serialize(ar,version);
    RangerEquip.Serialize(ar,version);
    PaladinEquip.Serialize(ar,version);
    DruidEquip.Serialize(ar,version);
    moneyData.Serialize(ar,version);
    m_diffLvlData.Serialize(ar);
    eventData.m_level = GLOBAL_ART;
    eventData.SetEventSource(GlobalEventSrc);
    eventData.Serialize(ar, version);
    eventData.SetEventSource(GlobalEventSrc);
    journalData.Serialize(ar);
  }
  else
  {
    if (version >= _VERSION_0575_)
      startEquip.Serialize(ar,version);
    if (version >= _VERSION_0830_)
    {
      ClericEquip.Serialize(ar,version);
      FighterEquip.Serialize(ar,version);
      MagicUserEquip.Serialize(ar,version);
      ThiefEquip.Serialize(ar,version);
      RangerEquip.Serialize(ar,version);
      PaladinEquip.Serialize(ar,version);
      DruidEquip.Serialize(ar,version);
    }
    if (version >= _VERSION_0642_)
      moneyData.Serialize(ar,version);
    if (version >= _VERSION_0697_)
      m_diffLvlData.Serialize(ar);
    if (version >= _VERSION_0681_)
      eventData.Serialize(ar, version);
    eventData.m_level = GLOBAL_ART;
    eventData.SetEventSource(GlobalEventSrc);

    if ((version >= _VERSION_06991_)&&(version<=_VERSION_0842_))
    {
      // load it, but don't do anything with it.
      SPELL_SPECIAL_DATA tempData;
      tempData.Serialize(ar);
    }

    if (version >= _VERSION_0780_)
      journalData.Serialize(ar);
  }  

  if (ar.IsStoring())
  {
    fixSpellBook.Serialize(ar, version);
  }
  else
  {
    if (version >= _VERSION_0909_)
    {
      fixSpellBook.Serialize(ar, version);
    }
  }
}
/*
void GLOBAL_STATS::Serialize(CAR &ar)
{
  int i;
  POSITION pos;

  if (ar.IsStoring())
  {
    CString name(GetDesignName());
    ar << version;
    ar << name;
    ar << startLevel;
    ar << startX;
    ar << startY;
    ar << startFacing;
    ar << startTime;
    ar << startExp;
    ar << startExpType;
    ar << UNUSED_startEquip;
    ar << startPlatinum;
    ar << startGem;
    ar << startJewelry;   
    
    ar << DungeonTimeDelta;
    ar << DungeonSearchTimeDelta;
    ar << WildernessTimeDelta;
    ar << WildernessSearchTimeDelta;
    ar << AutoDarkenViewport;
    ar << AutoDarkenAmount;
    ar << StartDarken;
    ar << EndDarken;
    ar << minPartySize;
    ar << maxPartySize;
    ar << allowCharCreate;    
    
    StripFilenamePath(MapArt);
    AS(ar,MapArt);
    AddDesignFolderToPath(MapArt);

    if (strlen(logfont.lfFaceName) == 0)
      FillDefaultFontData("SYSTEM", 16, &logfont);
    //ar << font;
    //ar << fontSize;
    ar.Write(&logfont, sizeof(logfont));

    //StripFilenamePath(TitleBgArt);
    //AS(ar,TitleBgArt);
    //AddDesignFolderToPath(TitleBgArt);
     
    StripFilenamePath(IconBgArt);
    AS(ar,IconBgArt);
    AddDesignFolderToPath(IconBgArt);

    StripFilenamePath(BackgroundArt);
    AS(ar,BackgroundArt);
    AddDesignFolderToPath(BackgroundArt);

    StripFilenamePath(CreditsBgArt);
    AS(ar,CreditsBgArt);
    AddDesignFolderToPath(CreditsBgArt);

   i = m_SmallPicImport.GetCount();
   ar << i;

   pos = m_SmallPicImport.GetHeadPosition();
   while (pos != NULL)
   {
     m_SmallPicImport.GetAt(pos).picType=SmallPicDib;
     m_SmallPicImport.GetAt(pos).SetDefaults();
     m_SmallPicImport.GetNext(pos).Serialize(ar, version);
   }

   i = m_IconPicImport.GetCount();
   ar << i;
   pos = m_IconPicImport.GetHeadPosition();
   while (pos != NULL)
   {
     m_IconPicImport.GetAt(pos).picType=IconDib;
     m_IconPicImport.GetAt(pos).SetDefaults();
     m_IconPicImport.GetNext(pos).Serialize(ar, version);
   }
   titleData.Serialize(ar);
  }
  else
  {
    titleData.Clear();

    ar >> version;
    DAS(ar,designName);
    ar >> startLevel;
    ar >> startX;
    ar >> startY;
    ar >> startFacing;
    ar >> startTime;
    ar >> startExp;
    if (version >= _VERSION_0770_)
      ar >> startExpType;
    ar >> UNUSED_startEquip;
    ar >> startPlatinum;
    ar >> startGem;
    ar >> startJewelry;

    if (version >= _VERSION_0574_)
    {
     ar >> DungeonTimeDelta;
     ar >> DungeonSearchTimeDelta;
     ar >> WildernessTimeDelta;
     ar >> WildernessSearchTimeDelta;
    }

    if (version >= _VERSION_0620_)
    {
      ar >> AutoDarkenViewport;
      ar >> AutoDarkenAmount;
      ar >> StartDarken;
      ar >> EndDarken;
    }

    if (version >= _VERSION_0575_)
    {
      ar >> minPartySize;
      ar >> maxPartySize;
      ar >> allowCharCreate;    
    }

    DAS(ar,MapArt);
    AddDesignFolderToPath(MapArt);

    if (version < _VERSION_0830_)
    {
      CString font="";
      BYTE fontSize=16;
      ar >> font;
      if (version >= _VERSION_0681_)
        ar >> fontSize;
      FillDefaultFontData(font, fontSize, &logfont);
    }
    else
    {
      ar.Read(&logfont, sizeof(logfont));
    }

    if (version < _VERSION_0800_)
    {
      CString TitleBgArt("");
      DAS(ar,TitleBgArt);
      AddDesignFolderToPath(TitleBgArt);
      TITLE_SCREEN data;
      data.TitleBgArt = TitleBgArt;
      data.DisplayBy = TITLE_SCREEN::tsFadeIn;
      data.UseTrans = FALSE;
      titleData.AddTitle(data);
    }

    if (version >= _VERSION_0660_)
    {
      DAS(ar,IconBgArt);
      AddDesignFolderToPath(IconBgArt);

      DAS(ar,BackgroundArt);
      AddDesignFolderToPath(BackgroundArt);
    }

    if (version >= _VERSION_0566_)
    {
      DAS(ar,CreditsBgArt);
      AddDesignFolderToPath(CreditsBgArt);
    }

    int count;
    PIC_DATA data;

    ar >> count;
    for (i=0;i<count;i++)
    {
      data.Serialize(ar, version);
      m_SmallPicImport.AddTail(data);
    }

    data.Clear();
    ar >> count;
    CString icon;
    for (i=0;i<count;i++)
    {
      if (version < _VERSION_0640_)
      {        
        DAS(ar,icon);
//        ar >> icon;
//        if (icon == "*") icon = "";
        AddDesignFolderToPath(icon);
        data.filename = icon;
        data.picType=IconDib;
        data.SetDefaults();
      }
      else
      {
        data.Serialize(ar, version);
      }
      m_IconPicImport.AddTail(data);
    }
    if (version >= _VERSION_0800_)
      titleData.Serialize(ar);
  }

  global_attributes.Serialize(ar);
  HBarVPArt.Serialize(ar);
  VBarVPArt.Serialize(ar);
  FrameVPArt.Serialize(ar);
  VBarCombArt.Serialize(ar);
  HBarCombArt.Serialize(ar);
  CombatWallArt.Serialize(ar);
  CombatCursorArt.Serialize(ar);
  CombatDeathIconArt.Serialize(ar);
  CombatDeathArt.Serialize(ar);
  if (version >= _VERSION_0575_)
    CursorArt.Serialize(ar, version);
  sounds.Serialize(ar);
  keyData.Serialize(ar, version);
  specialItemData.Serialize(ar, version);
  questData.Serialize(ar, version);
  charData.Serialize(ar, version);
  
  // only versions prior to 0.661 stored
  // vault data, all others store it
  // in the saved game file.
  if (ar.IsStoring())
  {   
    ASSERT(FALSE);
    ar << MAX_GLOBAL_VAULTS;
    for (int i=0;i<MAX_GLOBAL_VAULTS;i++)
      vault[i].Serialize(ar);
  }
  else
  {
    ASSERT(FALSE);
    // if loading old design with only one vault
    if (version < _VERSION_0910_)
    {
      vault[0].Serialize(ar);
    }
    else
    {
      int numvaults;
      ar >> numvaults;
      if (numvaults != MAX_GLOBAL_VAULTS)
      {
        TRACE("*** ERROR: Invalid numvaults read from design file\n");
        ASSERT(FALSE);
        if (numvaults > MAX_GLOBAL_VAULTS)
          numvaults = MAX_GLOBAL_VAULTS;
      }
      for (int i=0;i<numvaults;i++)
        vault[i].Serialize(ar);
    }
  }
  
  levelInfo.Serialize(ar);

  if (ar.IsStoring())
  {
    startEquip.Serialize(ar,version);
    ClericEquip.Serialize(ar,version);
    FighterEquip.Serialize(ar,version);
    MagicUserEquip.Serialize(ar,version);
    ThiefEquip.Serialize(ar,version);
    RangerEquip.Serialize(ar,version);
    PaladinEquip.Serialize(ar,version);
    DruidEquip.Serialize(ar,version);
    moneyData.Serialize(ar,version);
    m_diffLvlData.Serialize(ar);
    eventData.m_level = GLOBAL_ART;
    eventData.SetEventSource(GlobalEventSrc);
    eventData.Serialize(ar, version);
    eventData.SetEventSource(GlobalEventSrc);
    //m_spellSpecialData.Serialize(ar);
    journalData.Serialize(ar);
  }
  else
  {
    if (version >= _VERSION_0575_)
      startEquip.Serialize(ar,version);
    if (version >= _VERSION_0830_)
    {
      ClericEquip.Serialize(ar,version);
      FighterEquip.Serialize(ar,version);
      MagicUserEquip.Serialize(ar,version);
      ThiefEquip.Serialize(ar,version);
      RangerEquip.Serialize(ar,version);
      PaladinEquip.Serialize(ar,version);
      DruidEquip.Serialize(ar,version);
    }
    if (version >= _VERSION_0642_)
      moneyData.Serialize(ar,version);
    if (version >= _VERSION_0697_)
      m_diffLvlData.Serialize(ar);
    if (version >= _VERSION_0681_)
      eventData.Serialize(ar, version);
    eventData.m_level = GLOBAL_ART;
    eventData.SetEventSource(GlobalEventSrc);

    if ((version >= _VERSION_06991_)&&(version<=_VERSION_0842_))
    {
      // load it, but don't do anything with it.
      SPELL_SPECIAL_DATA tempData;
      tempData.Serialize(ar);
    }

    if (version >= _VERSION_0780_)
      journalData.Serialize(ar);
  }  

  if (ar.IsStoring())
  {
    fixSpellBook.Serialize(ar, version);
  }
  else
  {
    if (version >= _VERSION_0909_)
    {
      fixSpellBook.Serialize(ar, version);
    }
  }
}
*/

void GLOBAL_STATS::Save(CArchive& ar) 
{
	global_asl.Save(ar);
}

void GLOBAL_STATS::Restore(CArchive& ar) 
{
	temp_asl.Serialize(ar);
}

void GLOBAL_STATS::CommitRestore(void) 
{
	global_asl.CommitRestore(temp_asl);
	temp_asl.Clear();
}

GLOBAL_STATS::GLOBAL_STATS() : keyData(MAX_SPECIAL_KEYS), 
				 global_asl("GLOBAL_STATS_ATTRIBUTES"),
				 temp_asl("GLOBAL_STATS_ATTRIBUTES"),
                 specialItemData(MAX_SPECIAL_ITEMS),
                 WhiteFont(-1),YellowFont(-1),RedFont(-1),GreenFont(-1),BlueFont(-1),HighlightFont(-1),KeyboardFont(-1),
                 MapArtSurf(-1),BackgroundSurf(-1),Custom1Font(-1),MagicAdjFont(-1),KeyboardHighlightFont(-1)
{ 
   version = PRODUCT_VER;
   SaveGameVersion = 0.0;
   startLevel = 0;
   currLevel = 0;
   startX = 0;
   startY = 0;
   startFacing = FACE_EAST;
   startTime = 800; // 24-hour format
   startExp = 0;
   startExpType=START_EXP_VALUE;
   UNUSED_startEquip = 0;
   startPlatinum = START_PLATINUM;
   startGem = 0;
   startJewelry = 0;
   minPartySize = 1;
   maxPartySize = MAX_PARTY_MEMBERS;
   allowCharCreate = TRUE;
   useAreaView = FALSE;
   DungeonTimeDelta = 1;
   DungeonSearchTimeDelta = DungeonTimeDelta*10;
   WildernessTimeDelta = 12 * 60; // 12 hours
   WildernessSearchTimeDelta = 18 * 60; // 18 hours
   AutoDarkenViewport=FALSE;
   AutoDarkenAmount=256;
   StartDarken=0;
   EndDarken=0;
   designName="";
   MapArt="";
   UseAVZones=FALSE;
   //font = "System";
   //fontSize=16;
   FillDefaultFontData("SYSTEM", 16, &logfont);

   //TitleBgArt="";
   titleData.Clear();
   IconBgArt="";
   BackgroundArt="";
   CreditsBgArt="";
   
   MapArtSurf = -1; 
   BackgroundSurf=-1;
   
  startEquip.Clear();
  ClericEquip.Clear();
  FighterEquip.Clear();
  MagicUserEquip.Clear();
  ThiefEquip.Clear();
  RangerEquip.Clear();
  PaladinEquip.Clear();
  DruidEquip.Clear();

  charData.Clear();
  ReleaseSurfaces();
  keyData.Clear();
  specialItemData.Clear();
  questData.Clear();
#ifndef UAFEDITOR
  sounds.Clear(TRUE);
#endif
  for (int i=0;i<MAX_GLOBAL_VAULTS;i++)
    vault[i].Clear();

  levelInfo.Clear(); 
  global_asl.Clear();
  moneyData.Clear();
  m_diffLvlData.Clear();
  eventData.Clear();
  eventData.SetEventSource(GlobalEventSrc);
  journalData.Clear();
  fixSpellBook.Clear();
}

void GLOBAL_STATS::Clear(BOOL ctor, BOOL npcclear)
{
   version = PRODUCT_VER;
   startLevel = 0;
   currLevel = 0;
   startX = 0;
   startY = 0;
   startFacing = FACE_EAST;
   startTime = 800; // 24-hour format
   startExp = 0;
   startExpType=START_EXP_VALUE;
   UNUSED_startEquip = 0;
   startPlatinum = START_PLATINUM;
   startGem = 0;
   startJewelry = 0;
   minPartySize = 1;
   maxPartySize = MAX_PARTY_MEMBERS;
   allowCharCreate = TRUE;
   useAreaView = FALSE;
   DungeonTimeDelta = 1;
   DungeonSearchTimeDelta = DungeonTimeDelta*10;
   WildernessTimeDelta = 12 * 60; // 12 hours
   WildernessSearchTimeDelta = 18 * 60; // 18 hours
   AutoDarkenViewport=FALSE;
   AutoDarkenAmount=256;
   StartDarken=0;
   EndDarken=0;
   designName="";
   MapArt="";
   UseAVZones=FALSE;
   //font = "";
   //fontSize=16;
   FillDefaultFontData("SYSTEM", 16, &logfont);
   //TitleBgArt="";
   titleData.Clear();
   IconBgArt="";
   BackgroundArt="";
   CreditsBgArt="";

#ifdef UAFEDITOR
   if (!ctor)
   {
     designName="Default Design";
     //font = "System"; // required by game engine
     MapArt.Format("%s%s", EditorArt, AREAVIEWART); // required by game engine
     IconBgArt.Format("%s%s", EditorArt, DEFAULT_IB);
     CString TitleBgArt("");
     TitleBgArt.Format("%s%s", EditorArt, DEFAULT_TITLE_BG);
     titleData.SetDefault(TitleBgArt);
     CreditsBgArt.Format("%s%s", EditorArt, DEFAULT_CREDITS_BG);
   }
#else
   if (!ctor)
   {
     // this stuff should be in the design data files, but not 
     // all of the older file formats include it.
     //font = "System";
     MapArt = AREAVIEWART;
     IconBgArt = DEFAULT_IB;
     CString TitleBgArt("");
     TitleBgArt = DEFAULT_TITLE_BG;
     titleData.SetDefault(TitleBgArt);
     CreditsBgArt = DEFAULT_CREDITS_BG;
   }
#endif
   
  startEquip.Clear();
  ClericEquip.Clear();
  FighterEquip.Clear();
  MagicUserEquip.Clear();
  ThiefEquip.Clear();
  RangerEquip.Clear();
  PaladinEquip.Clear();
  DruidEquip.Clear();

  if (npcclear) charData.Clear();
  ReleaseSurfaces();

  keyData.Clear();
  specialItemData.Clear();
  questData.Clear();
  sounds.Clear(ctor);
  
  for (int i=0;i<MAX_GLOBAL_VAULTS;i++)
    vault[i].Clear();

  levelInfo.Clear();
  global_asl.Clear();
#ifdef UAFEDITOR
  CString vertext;
  vertext.Format("%1.4f", version);
  global_asl.Update("RunAsVersion", vertext, ASLF_EDITOR);
  global_asl.Update("GuidedTourVersion", vertext, ASLF_EDITOR);
  global_asl.Update("ItemUseEventVersion", vertext, ASLF_EDITOR);
  global_asl.Update("SpecialItemKeyQtyVersion", vertext, ASLF_EDITOR);
#endif
  moneyData.Clear();
  m_diffLvlData.Clear();
  eventData.Clear();
  eventData.SetEventSource(GlobalEventSrc);
  moneyData.SetUADefaults();
  m_diffLvlData.SetUADefaults();
  //m_spellSpecialData.Clear();
  journalData.Clear();
  fixSpellBook.Clear();
}

void GLOBAL_STATS::AddMoney(int which, MONEY_SACK &src) 
{ 
  if ((which >= 0) && (which < MAX_GLOBAL_VAULTS))
    vault[which].money.Add(src); 
}

void GLOBAL_STATS::TransferMoney(int which, MONEY_SACK &src) 
{ 
  if ((which >= 0) && (which < MAX_GLOBAL_VAULTS))
    vault[which].money.Transfer(src); 
}

double GLOBAL_STATS::VaultTotal(int which)
{
  double total = 0.0;

  if (which < 0)
  {
    // check all vaults
    for (int i=0;i<MAX_GLOBAL_VAULTS;i++)
      total += vault[i].money.Total();
  }
  else
  {
    if ((which >= 0) && (which < MAX_GLOBAL_VAULTS))
      total = vault[which].money.Total();
  }

  return total;
}

BOOL GLOBAL_STATS::VaultHasMoney(int which)
{
  if (which < 0)
  {
    // check all vaults
    for (int i=0;i<MAX_GLOBAL_VAULTS;i++)
    {
      if (!vault[i].money.IsEmpty())
        return TRUE;
    }
  }
  else
  {
    if ((which >= 0) && (which < MAX_GLOBAL_VAULTS))
      return (!vault[which].money.IsEmpty());
  }

  return FALSE;
}

void GLOBAL_STATS::UpdateMoneyInVault(int which, itemClassType type, int qty)
{
  if ((which < 0) || (which >= MAX_GLOBAL_VAULTS))
    return;

  if (qty == 0)
    return;

  switch (type)
  {
  case GemType:
    if (qty > 0)
    {
      for (int i=0;i<qty;i++) 
        vault[which].money.AddGem();
    }
    else
    {
      vault[which].money.RemoveMultGems(abs(qty));
    }
    break;

  case JewelryType:
    if (qty > 0)
    {
      for (int i=0;i<qty;i++) 
        vault[which].money.AddJewelry();
    }
    else
    {
      vault[which].money.RemoveMultJewelry(abs(qty));
    }
    break;

  default:
    if (qty < 0)
      vault[which].money.Subtract(type, abs(qty));
    else
      vault[which].money.Add(type, qty);
    break;
  }
}

BOOL GLOBAL_STATS::AddItemToVault(int which, ITEM item)
{
  if ((which >= 0) && (which < MAX_GLOBAL_VAULTS))
    return (vault[which].items.AddItem(item));

  return FALSE;
}

BOOL GLOBAL_STATS::RemoveItemFromVault(int which, int index, int qty)
{
  if ((which >= 0) && (which < MAX_GLOBAL_VAULTS))
    return (vault[which].items.deleteItem(index, qty));

  return FALSE;
}

void GLOBAL_STATS::SaveSounds()
{
  saveSound(sounds.CharHit, GLOBAL_SOUND);
  saveSound(sounds.CharMiss, GLOBAL_SOUND);
  saveSound(sounds.PartyBump, GLOBAL_SOUND);
  saveSound(sounds.PartyStep, GLOBAL_SOUND);
  saveSound(sounds.DeathMusic, GLOBAL_SOUND);
  //saveSound(sounds.IntroMusic, GLOBAL_SOUND);

  POSITION pos = sounds.IntroMusic.sounds.GetHeadPosition();
  while (pos!=NULL)
    saveSound(sounds.IntroMusic.sounds.GetNext(pos), GLOBAL_SOUND);

  pos = sounds.CampMusic.sounds.GetHeadPosition();
  while (pos!=NULL)
    saveSound(sounds.CampMusic.sounds.GetNext(pos), GLOBAL_SOUND);

  levelInfo.SaveSounds();
}

void GLOBAL_STATS::SaveArt()
{ 
  POSITION pos;
   // save common pics (frame bars, combat common pics, etc)
   saveArt(HBarVPArt.name, CommonDib,GLOBAL_ART);
   saveArt(VBarVPArt.name, CommonDib,GLOBAL_ART); 
   saveArt(FrameVPArt.name, CommonDib,GLOBAL_ART);
   saveArt(VBarCombArt.name, CommonDib,GLOBAL_ART);
   saveArt(HBarCombArt.name, CommonDib,GLOBAL_ART);
   saveArt(CombatCursorArt.name, CommonDib,GLOBAL_ART);
   saveArt(CombatDeathIconArt.name, CommonDib,GLOBAL_ART);
   saveArt(CombatDeathArt.name, CommonDib,GLOBAL_ART);
   saveArt(CursorArt.filename, CommonDib,GLOBAL_ART);
   
   saveArt(MapArt, CommonDib, GLOBAL_ART);
   //saveArt(TitleBgArt, TitleDib, GLOBAL_ART);
   saveArt(IconBgArt, CommonDib, GLOBAL_ART);
   saveArt(BackgroundArt, TransBufferDib, GLOBAL_ART);
   saveArt(CreditsBgArt, TitleDib, GLOBAL_ART);

   // save pre-gen character pics (smallpics, icons, sprites)
   pos = charData.GetHeadPosition();
   while (pos != NULL)
     charData.GetNext(pos).SaveArt();

   pos = m_SmallPicImport.GetHeadPosition();
   while (pos != NULL)
     m_SmallPicImport.GetNext(pos).SavePicData(GLOBAL_ART);

   pos = m_IconPicImport.GetHeadPosition();
   while (pos != NULL)
     m_IconPicImport.GetNext(pos).SavePicData(GLOBAL_ART);
   
   pos = titleData.Titles.GetHeadPosition();
   while (pos != NULL)
     saveArt(titleData.Titles.GetNext(pos).TitleBgArt, TitleDib, GLOBAL_ART);

   eventData.saveUsedEventArt(GLOBAL_ART);
}

BOOL GLOBAL_STATS::ConfirmUsedLevels()
{
  BOOL success = TRUE;
  int i;

  for (i=0;((i<MAX_LEVELS) && (success));i++)
  {
    if (levelInfo.stats[i].used)
    {
      if (!levelInfo.stats[i].level_name.IsEmpty())
      {
        success = levelExists(i);

        if (!success)
        {
           WriteDebugString("Used level %i does not exist: %s\n", i,levelInfo.stats[i].level_name);
           levelInfo.stats[i].used = FALSE;
        }
      }
      else 
      {
        WriteDebugString("level %i used=TRUE when level_name is empty: %s\n", i,levelInfo.stats[i].level_name);
        levelInfo.stats[i].used = FALSE;
      }
    }
  }

  return success;
}

BOOL GLOBAL_STATS::LoadFonts(COLORREF green, COLORREF yellow, COLORREF red, COLORREF blue, COLORREF keyb, COLORREF BackColor, COLORREF TransColor, COLORREF Custom)
{
#ifdef UAFEngine
  ReleaseFonts();
  //fontSize = max(8, fontSize);
  WriteDebugString("Creating fonts from %s\n", logfont.lfFaceName);
  
  //WhiteFont = GraphicsMgr.CreateBitmappedFont(font, fontSize, White, BackColor, TransColor);
  WhiteFont = GraphicsMgr.CreateBitmappedFont(&logfont, White, BackColor, TransColor);
  if (WhiteFont < 0) return FALSE;
  
  //YellowFont = GraphicsMgr.CreateBitmappedFont(font, fontSize, yellow, BackColor, TransColor);
  YellowFont = GraphicsMgr.CreateBitmappedFont(&logfont, yellow, BackColor, TransColor);
  if (YellowFont < 0) return FALSE;
  
  //RedFont = GraphicsMgr.CreateBitmappedFont(font, fontSize, red, BackColor, TransColor);
  RedFont = GraphicsMgr.CreateBitmappedFont(&logfont, red, BackColor, TransColor);
  if (RedFont < 0) return FALSE;
  
  //GreenFont = GraphicsMgr.CreateBitmappedFont(font, fontSize, green, BackColor, TransColor);
  GreenFont = GraphicsMgr.CreateBitmappedFont(&logfont, green, BackColor, TransColor);
  if (GreenFont < 0) return FALSE;
  
  //BlueFont = GraphicsMgr.CreateBitmappedFont(font, fontSize, blue, BackColor, TransColor);
  BlueFont = GraphicsMgr.CreateBitmappedFont(&logfont, blue, BackColor, TransColor);
  if (BlueFont < 0) return FALSE;
  
  //Custom1Font = GraphicsMgr.CreateBitmappedFont(font, fontSize, Custom, BackColor, TransColor);
  Custom1Font = GraphicsMgr.CreateBitmappedFont(&logfont, Custom, BackColor, TransColor);
  if (Custom1Font < 0) return FALSE;

  //KeyboardFont = GraphicsMgr.CreateBitmappedFont(font, fontSize, keyb, BackColor, TransColor);
  KeyboardFont = GraphicsMgr.CreateBitmappedFont(&logfont, keyb, BackColor, TransColor);
  if (KeyboardFont < 0) return FALSE;
  
  //KeyboardHighlightFont = GraphicsMgr.CreateBitmappedFont(font, fontSize, keyb, White, red);
  KeyboardHighlightFont = GraphicsMgr.CreateBitmappedFont(&logfont, keyb, White, red);
  if (KeyboardHighlightFont < 0) return FALSE;

  //HighlightFont = GraphicsMgr.CreateBitmappedFont(font, fontSize, Black, White, red);
  HighlightFont = GraphicsMgr.CreateBitmappedFont(&logfont, Black, White, red);
  if (HighlightFont < 0) return FALSE;   
  
  MagicAdjFont = GraphicsMgr.CreateBitmappedFont(&logfont, RGB(255,128,0), BackColor, TransColor);
  if (MagicAdjFont < 0) return FALSE; 

#endif
  return TRUE;
}

void GLOBAL_STATS::ReleaseSurfaces()
{
  if (MapArtSurf >= 0)
    GraphicsMgr.ReleaseSurface(MapArtSurf);
  MapArtSurf = -1; 
  if (BackgroundSurf >= 0)
    GraphicsMgr.ReleaseSurface(BackgroundSurf);
  BackgroundSurf = -1; 
  ReleaseFonts();
}

void GLOBAL_STATS::ReleaseFonts()
{
  if (WhiteFont >= 0)
    GraphicsMgr.ReleaseSurface(WhiteFont);
  WhiteFont = -1;
  if (YellowFont >= 0)
    GraphicsMgr.ReleaseSurface(YellowFont);
  YellowFont = -1;
  if (RedFont >= 0)
    GraphicsMgr.ReleaseSurface(RedFont);
  RedFont = -1;
  if (GreenFont >= 0)
    GraphicsMgr.ReleaseSurface(GreenFont);
  GreenFont = -1;
  if (BlueFont >= 0)
    GraphicsMgr.ReleaseSurface(BlueFont);
  BlueFont = -1;
  if (KeyboardFont >= 0)
    GraphicsMgr.ReleaseSurface(KeyboardFont);
  KeyboardFont=-1;  
  if (KeyboardHighlightFont >= 0)
    GraphicsMgr.ReleaseSurface(KeyboardHighlightFont);
  KeyboardHighlightFont=-1;
  if (HighlightFont >= 0)
    GraphicsMgr.ReleaseSurface(HighlightFont);
  HighlightFont = -1;  
  if (Custom1Font >= 0)
    GraphicsMgr.ReleaseSurface(Custom1Font);
  Custom1Font = -1;
  if (MagicAdjFont >= 0)
    GraphicsMgr.ReleaseSurface(MagicAdjFont);
  MagicAdjFont=-1;
}

//*****************************************************************************

void saveSounds(GLOBAL_STATS& data)
{
  data.SaveSounds();
}

void saveArt(GLOBAL_STATS& data) 
{
  data.SaveArt();
}

void GetKeyNames(GLOBAL_STATS& data, CString (*names)[8])
{
  int index=1;
  while (index <= 8)
  {    
    int key = data.keyData.GetItemKeyUsingSequence(index);
    if (key > 0)
      (*names)[index-1] = data.keyData.GetName(key);
      else (*names)[index-1]="";
    index++;
  }
}

long int GetKeyboardFont(GLOBAL_STATS& data)
{
  return data.KeyboardFont;
}

long int GetKeyboardHighlightFont(GLOBAL_STATS& data)
{
  return data.KeyboardHighlightFont;
}

long int GetHighlightFont(GLOBAL_STATS& data)
{
  return data.HighlightFont;
}

double GetGameVersion(GLOBAL_STATS& data)
{
  return data.version;
}

void DeleteLevelASL(GLOBAL_STATS& data, int level, const CString& key)
{
  data.levelInfo.stats[level].level_asl.Delete(key);
}

CString LookupGlobalASL(GLOBAL_STATS& data, const CString& key)
{
  ASL_VALUE AV;
  data.global_asl.Lookup(key,AV);
  return AV.Value();
}

void InsertGlobalASL(
          GLOBAL_STATS& data,
          const CString& key,
          const CString& value,
          int flags)
{
  data.global_asl.Insert(key,value,flags);
}

void InsertLevelASL(
          GLOBAL_STATS& data,
          int level,
          const CString& key,
          const CString& value,
          int flags)
{
  data.levelInfo.stats[level].level_asl.Insert(key,value,flags);
}

CString GetDesignName(GLOBAL_STATS& data)
{
  return data.designName;
}

void SetGameVersion(GLOBAL_STATS& data, double version)
{
  data.version=version;
}

BYTE CurrLevelWidth() 
{ 
  return globalData.levelInfo.stats[globalData.currLevel].area_width; 
}

BYTE CurrLevelHeight() 
{ 
  return globalData.levelInfo.stats[globalData.currLevel].area_height; 
}


void PlayCharHit(BOOL play) 
{ 
  if ((play) && PlaySoundEffects)
  {
    if (!ValidSound(globalData.sounds.hCharHit))
      globalData.sounds.hCharHit = LoadSound(globalData.sounds.CharHit);
    if (!PlaySound(globalData.sounds.hCharHit))
      globalData.sounds.hCharHit=-1;
  }
  else
    StopSound(globalData.sounds.hCharHit);
}

void PlayCharMiss(BOOL play) 
{ 
  if (play && PlaySoundEffects)
  {
    if (!ValidSound(globalData.sounds.hCharMiss))
      globalData.sounds.hCharMiss = LoadSound(globalData.sounds.CharMiss);
    if (!PlaySound(globalData.sounds.hCharMiss))
      globalData.sounds.hCharMiss=-1;
  }
  else
    StopSound(globalData.sounds.hCharMiss);
}

void PlayPartyStep(BOOL play) 
{ 
  if (    play 
       && PlaySoundEffects
       && !globalData.levelInfo.stats[globalData.currLevel].level_asl.Exists("Supress Step Sound")
     )
  {
    if (!ValidSound(globalData.sounds.hPartyStep))
    {
      CString tmp;
      if (globalData.levelInfo.stats[globalData.currLevel].StepSound != "")
        tmp = globalData.levelInfo.stats[globalData.currLevel].StepSound;
      else
        tmp = globalData.sounds.PartyStep;
      globalData.sounds.hPartyStep = LoadSound(tmp);
    }
    if (!PlaySound(globalData.sounds.hPartyStep))
      globalData.sounds.hPartyStep=-1;
  }
  else
    StopSound(globalData.sounds.hPartyStep);
}

void PlayPartyBump(BOOL play) 
{ 
  if (    play 
       && PlaySoundEffects
       && !globalData.levelInfo.stats[globalData.currLevel].level_asl.Exists("Supress Bump Sound")
     )  
  {
    if (!ValidSound(globalData.sounds.hPartyBump))
    {
      CString tmp;
      if (globalData.levelInfo.stats[globalData.currLevel].BumpSound != "")
        tmp = globalData.levelInfo.stats[globalData.currLevel].BumpSound;
      else
        tmp = globalData.sounds.PartyBump;
      globalData.sounds.hPartyBump = LoadSound(tmp);
    }
    if (!PlaySound(globalData.sounds.hPartyBump))
      globalData.sounds.hPartyBump=-1;
  }
  else
    StopSound(globalData.sounds.hPartyBump);
}

void PlayIntro(BOOL play) 
{ 
  if (play)
  {
    if (pSndMgr != NULL)
    {  
      pSndMgr->StopBgndQueue();
      POSITION pos = globalData.sounds.IntroMusic.sounds.GetHeadPosition();
      while (pos != NULL)
        pSndMgr->QueueBgndSound(globalData.sounds.IntroMusic.sounds.GetNext(pos),FALSE);
      pSndMgr->PlayBgndQueue();
    }  
  }
  else
    pSndMgr->StopBgndQueue();
}

void PlayCampMusic(BOOL play) 
{ 
  if (pSndMgr == NULL)
    return;
  if (!pSndMgr->GetMusicEnable()) 
    return;

  if (globalData.sounds.CampMusic.sounds.GetCount() == 0)
    return;
  
  pSndMgr->StopBgndQueue();

  if (play)
  {    
    POSITION pos = globalData.sounds.CampMusic.sounds.GetHeadPosition();

    while (pos != NULL)
      pSndMgr->QueueBgndSound(globalData.sounds.CampMusic.sounds.GetNext(pos),FALSE);

    pSndMgr->PlayBgndQueue();
  }
}

void PlayDeath(BOOL play) 
{ 
  if (play && PlaySoundEffects)
  {
    if (!ValidSound(globalData.sounds.hDeathMusic))
      globalData.sounds.hDeathMusic = LoadSound(globalData.sounds.DeathMusic);
    if (!PlaySound(globalData.sounds.hDeathMusic, FALSE))
      globalData.sounds.hDeathMusic=-1;
  }
  else
    StopSound(globalData.sounds.hDeathMusic);
}

void StopAllSounds()
{
  globalData.sounds.ClearSounds();  
  itemData.ClearSounds();
  spellData.ClearSounds();
  monsterData.ClearSounds();
  //combatData.ClearSounds(); auto cleared when combat ends

  if (pSndMgr == NULL) return;
  pSndMgr->StopAll();
  //WriteDebugString("StopAllSounds\n");
}

void ReleaseAllSounds()
{
  globalData.sounds.ClearSounds();  
  itemData.ClearSounds();
  spellData.ClearSounds();
  monsterData.ClearSounds();
  //combatData.ClearSounds(); auto cleared when combat ends

  if (pSndMgr == NULL) return;
  pSndMgr->Clear();
  //WriteDebugString("RemoveAllSounds\n");
}

#ifdef UAFEngine
void CheckAndPlayBackgroundMusic(int &UsingDayMusic)
{
  // no background music during combat, or during encamp (encamp has it's own music)
  if (!IsCombatActive())
  { 
    bool usebg = true;
    if (GetCurrEventType() == CONTROL_Encamp)
    {
      // see if camp music is available
      if (globalData.sounds.CampMusic.sounds.GetCount() > 0)
        usebg = false;
    }

    if (usebg)
    {
      int currZone = levelData.GetCurrZone(party.Posx, party.Posy);
      ZONE &zdata = levelData.zoneData.zones[currZone];
      LEVEL_STATS &data = globalData.levelInfo.stats[GetCurrentLevel()];

      BOOL znight = (   (zdata.bgSounds.UseNightMusic) 
                     && (zdata.bgSounds.nightSounds.sounds.GetCount()>0)
                     && (IsTriggerTimeInCurrentTime(zdata.bgSounds.StartTime, zdata.bgSounds.EndTime)));

      BOOL lnight = (   (data.bgSounds.UseNightMusic) 
                     && (data.bgSounds.nightSounds.sounds.GetCount()>0)
                     && (IsTriggerTimeInCurrentTime(data.bgSounds.StartTime, data.bgSounds.EndTime)));


      BOOL nightTrigger = (znight || lnight);

      if (UsingDayMusic==1) // playing day music
      {
        if (nightTrigger)
        {
          // switch to night music
          StartBackgroundMusic();
          UsingDayMusic=0;
        }
      }
      else if (UsingDayMusic==0) // playing night music
      {
        if (!nightTrigger)
        {
          // switch to day music
          StartBackgroundMusic();
          UsingDayMusic=1;
        }
      }
      else // day music=-1 at program start
      {
        // music should already be playing,
        // just init day/night flag
        UsingDayMusic = !nightTrigger;
      }
    }
  }
}
#endif // UAFEngine