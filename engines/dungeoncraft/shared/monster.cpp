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

#ifdef UAFEDITOR
#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
#else
#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/..\UAFWin\Dungeon.h"
#endif

#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/globalData.h"
#include "dungeoncraft/shared/SoundMgr.h"
#include "dungeoncraft/shared/monster.h"
#include "dungeoncraft/shared/PicSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( MONSTER_DATA, CObject, 1 )
IMPLEMENT_SERIAL( MONSTER_DATA_TYPE, CObject, 1 )

MONSTER_DATA_TYPE monsterData;

MONSTER_DATA::MONSTER_DATA() :
	mon_asl("MONSTER_DATA_ATTRIBUTES"),
	temp_asl("MONSTER_DATA_ATTRIBUTES"),
 hHitSound(-1), hMissSound(-1), hMoveSound(-1), hDeathSound(-1)
{ 
  key=0;
  Name="";
  Icon.filename="DEFAULT";
  Icon.picType=IconDib;
  Icon.SetDefaults();
  HitSound = "DEFAULT";
  MissSound = "DEFAULT";
  MoveSound="DEFAULT";
  DeathSound="DEFAULT";
  ClearSounds();
  Intelligence = 0;
  Armor_Class = 0;
  Movement = 0;
  Hit_Dice = 0.0F;
  UseHitDice=TRUE;
  Hit_Dice_Bonus = 0;
  THACO = 20;
  //Nbr_Attacks = 0;
  //Dmg_Dice_for_Attack = 0;
  //Dmg_Dice_Bonus = 0;
  //Nbr_Dmg_Dice = 0;
  Magic_Resistance = 0;
  Size = Medium;
  Morale = 0;
  XP_Value = 0;
 	specAbs.Clear();
  ItemMask = AllClasses;
  Form_Type = 0;
  Penalty_Type = 0;
  Immunity_Type = 0;
  MiscOptions_Type = 0;
  myItems.Clear();
  money.Clear();
  undeadStatus = NotUndead;
  attackData.Clear();
}

void MONSTER_DATA::Save(CArchive& ar)
{
	mon_asl.Save(ar);
}

void MONSTER_DATA::PrepareForRestore(void) {
	temp_asl.Clear();
}

void MONSTER_DATA::Restore(CArchive& ar) 
{
	temp_asl.Serialize(ar);
}

void MONSTER_DATA::CommitRestore(void) 
{
	mon_asl.CommitRestore(temp_asl);
	temp_asl.Clear();
}


void MONSTER_DATA::Clear(BOOL ctor)
{
  key=0;
  Name="";
  Icon.filename="DEFAULT";
  Icon.picType=IconDib;
  Icon.SetDefaults();
  HitSound = "DEFAULT";
  MissSound = "DEFAULT";
  MoveSound="DEFAULT";
  DeathSound="DEFAULT";
  ClearSounds();
  Intelligence = 0;
  Armor_Class = 0;
  Movement = 0;
  Hit_Dice = 0.0F;
  UseHitDice=TRUE;
  Hit_Dice_Bonus = 0;
  THACO = 20;
  //Nbr_Attacks = 0;
  //Dmg_Dice_for_Attack = 0;
  //Dmg_Dice_Bonus = 0;
  //Nbr_Dmg_Dice = 0;
  Magic_Resistance = 0;
  Size = Medium;
  Morale = 0;
  XP_Value = 0;
 	specAbs.Clear();
  ItemMask = AllClasses;
  Form_Type = 0;
  Penalty_Type = 0;
  Immunity_Type = 0;
  MiscOptions_Type = 0;
  mon_asl.Clear();
  myItems.Clear();
  money.Clear();
  undeadStatus=NotUndead;
  attackData.Clear();
}

MONSTER_DATA& MONSTER_DATA::operator =(const MONSTER_DATA& src)
{
  if (&src == this)
    return *this;
  hHitSound = src.hHitSound;
  hMissSound = src.hMissSound;
  hMoveSound = src.hMoveSound;
  hDeathSound = src.hDeathSound;
  key = src.key;
  Name=src.Name;
  Icon=src.Icon;
  HitSound = src.HitSound;
  MissSound = src.MissSound;
  MoveSound = src.MoveSound;
  DeathSound = src.DeathSound;
  Intelligence = src.Intelligence;
  Armor_Class = src.Armor_Class;
  Movement = src.Movement;
  Hit_Dice = src.Hit_Dice;
  UseHitDice=src.UseHitDice;
  Hit_Dice_Bonus = src.Hit_Dice_Bonus;
  THACO = src.THACO;
  //Nbr_Attacks = src.Nbr_Attacks;
  //Dmg_Dice_for_Attack = src.Dmg_Dice_for_Attack;
  //Dmg_Dice_Bonus = src.Dmg_Dice_Bonus;
  //Nbr_Dmg_Dice = src.Nbr_Dmg_Dice;
  Magic_Resistance = src.Magic_Resistance;
  Size = src.Size;
  Morale = src.Morale;
  XP_Value = src.XP_Value;
	specAbs = src.specAbs;
  ItemMask = src.ItemMask;
  Form_Type = src.Form_Type;
  Penalty_Type = src.Penalty_Type;
  Immunity_Type = src.Immunity_Type;
  MiscOptions_Type = src.MiscOptions_Type;
  mon_asl=src.mon_asl;
  temp_asl=src.temp_asl;
  myItems=src.myItems;
  money=src.money;
  undeadStatus=src.undeadStatus;
  attackData=src.attackData;
  return *this;
}

bool MONSTER_DATA::operator ==(const MONSTER_DATA& src) const
{
  if (hHitSound           != src.hHitSound          ) return false;
  if (hMissSound          != src.hMissSound         ) return false;
  if (hMoveSound          != src.hMoveSound         ) return false;
  if (hDeathSound         != src.hDeathSound        ) return false;
  if (key                 != src.key                ) return false;
  if (Name                != src.Name               ) return false;
  if (!(Icon              == src.Icon              )) return false;
  if (HitSound            != src.HitSound           ) return false;
  if (MissSound           != src.MissSound          ) return false;
  if (MoveSound           != src.MoveSound          ) return false;
  if (DeathSound          != src.DeathSound         ) return false;
  if (Intelligence        != src.Intelligence       ) return false;
  if (Armor_Class         != src.Armor_Class        ) return false;
  if (Movement            != src.Movement           ) return false;
  if (Hit_Dice            != src.Hit_Dice           ) return false;
  if (UseHitDice          != src.UseHitDice         ) return false;
  if (Hit_Dice_Bonus      != src.Hit_Dice_Bonus     ) return false;
  if (THACO               != src.THACO              ) return false;
  //if (Nbr_Attacks         != src.Nbr_Attacks        ) return false;
  //if (Dmg_Dice_for_Attack != src.Dmg_Dice_for_Attack) return false;
  //if (Dmg_Dice_Bonus      != src.Dmg_Dice_Bonus     ) return false;
  //if (Nbr_Dmg_Dice        != src.Nbr_Dmg_Dice       ) return false;
  if (Magic_Resistance    != src.Magic_Resistance   ) return false;
  if (Size                != src.Size               ) return false;
  if (Morale              != src.Morale             ) return false;
  if (XP_Value            != src.XP_Value           ) return false;
	if (!(specAbs           == src.specAbs)           ) return false;
  if (ItemMask            != src.ItemMask           ) return false;
  if (Form_Type           != src.Form_Type          ) return false;
  if (Penalty_Type        != src.Penalty_Type       ) return false;
  if (Immunity_Type       != src.Immunity_Type      ) return false;
  if (MiscOptions_Type    != src.MiscOptions_Type   ) return false;
  if (!(mon_asl    == src.mon_asl)    ) return false;
  if (!(temp_asl   == src.temp_asl)   ) return false;
  if (!(myItems           == src.myItems)           ) return false;      
  if (!(const_cast<MONEY_SACK&>(money) == src.money)) return false;
  if (undeadStatus        != src.undeadStatus       ) return false;
  if (!(attackData        == src.attackData)        ) return false;  
  return true;
}

void MONSTER_DATA::GuessUndeadStatus()
{
  //enum undeadType { NotUndead, Skeleton, Zombie, Ghoul, Shadow, Wight, Ghast, Wraith, Mummy,
	//					Spectre, Vampire, Ghost, Lich, Special };

  if (undeadStatus != NotUndead) return; // already have undead status selected

  CString tmp(Name);
  tmp.MakeLower();
  if (tmp.Find("skeleton",0)>=0)
    undeadStatus = Skeleton;
  else if (tmp.Find("zombie",0)>=0)
    undeadStatus = Zombie;
  else if (tmp.Find("ghoul",0)>=0)
    undeadStatus = Ghoul;
  else if (tmp.Find("shadow",0)>=0)
    undeadStatus = Shadow;
  else if (tmp.Find("wight",0)>=0)
    undeadStatus = Wight;
  else if (tmp.Find("ghast",0)>=0)
    undeadStatus = Ghast;
  else if (tmp.Find("wraith",0)>=0)
    undeadStatus = Wraith;
  else if (tmp.Find("mummy",0)>=0)
    undeadStatus = Mummy;
  else if (tmp.Find("spectre",0)>=0)
    undeadStatus = Spectre;
  else if (tmp.Find("vampire",0)>=0)
    undeadStatus = Vampire;
  else if (tmp.Find("ghost",0)>=0)
    undeadStatus = Ghost;
  else if (tmp.Find("lich",0)>=0)
    undeadStatus = Lich;
  else
    undeadStatus = NotUndead;
}

// this version will only be called if the monster data
// file is older and does not contain a version header.
//
void MONSTER_DATA::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);
  long temp;

  if (ar.IsStoring())
  {
    if (ver <= _VERSION_0830_)
      Name.Replace('/','|');
    ar << key;
    AS(ar,Name);
    Icon.Serialize(ar, ver);
    AS(ar,HitSound);
    AS(ar,MissSound);
    AS(ar,MoveSound);
    AS(ar,DeathSound);
    ar << Intelligence;
    ar << Armor_Class;
    ar << Movement;
    ar << Hit_Dice;
    ar << UseHitDice;
    ar << Hit_Dice_Bonus;
    ar << THACO;
    attackData.Serialize(ar, ver);
    ar << Magic_Resistance;
    temp = Size;
    ar << temp;
    ar << Morale;
    ar << XP_Value;
    ar << ItemMask;
    ar << Form_Type;
    ar << Penalty_Type;
    ar << Immunity_Type;
    ar << MiscOptions_Type;
    ar << (long)undeadStatus;    
  }
  else
  {
    ar >> key;
    DAS(ar,Name);

#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Kobald|Stupid" will be
    // read as simply "Kobald".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    if (Name.Find('|') >= 0) Name = Name.Left(Name.Find('|')); // Remove qualifier.
#endif

    if (ver < _VERSION_0640_)
    {
      CString tmp;
      DAS(ar,tmp);
      Icon.filename=tmp;
      Icon.picType = IconDib;
      Icon.SetDefaults();
    }
    else
    {
      Icon.Serialize(ar, ver);
      if (ver <= _VERSION_0840_)
        Icon.SetPre0840NonLoopDefaults();
    }
    DAS(ar,HitSound);
    DAS(ar,MissSound);
    if (ver >= _VERSION_0575_)
    {
      DAS(ar,MoveSound);
      DAS(ar,DeathSound);
    }
    else
    {
      MoveSound="DEFAULT";
      DeathSound="DEFAULT";
    }
    ar >> Intelligence;
    ar >> Armor_Class;
    ar >> Movement;
    ar >> Hit_Dice;
    if (ver >= _VERSION_0906_)
      ar >> UseHitDice;
    ar >> Hit_Dice_Bonus;
    ar >> THACO;
    
    if (ver < _VERSION_0750_)
    {
      long Nbr_Attacks;
      long Dmg_Dice_for_Attack;
      long Dmg_Dice_Bonus;
      long Nbr_Dmg_Dice;

      ar >> Nbr_Attacks;
      ar >> Dmg_Dice_for_Attack;
      ar >> Dmg_Dice_Bonus;
      ar >> Nbr_Dmg_Dice;

      if (Nbr_Attacks<=0) Nbr_Attacks=1;
      if (Dmg_Dice_for_Attack <= 0) Dmg_Dice_for_Attack = 10;
      if (Nbr_Dmg_Dice <= 0) Nbr_Dmg_Dice = 1;

      MONSTER_ATTACK_DETAILS tmp;
      tmp.sides = Dmg_Dice_for_Attack;
      tmp.nbr = Nbr_Dmg_Dice;
      tmp.bonus = Dmg_Dice_Bonus;
      tmp.msg = "attacks";
      for (int cnt=0;cnt<Nbr_Attacks;cnt++)
        attackData.Add(tmp);
    }
    else
    {
      attackData.Serialize(ar, ver);
    }

    if (attackData.GetCount()==0)
    {
      WriteDebugString("Forcing monster %s to have at least one attack\n", Name);
      MONSTER_ATTACK_DETAILS tmp;
      tmp.sides = 6;
      tmp.nbr = 1;
      tmp.bonus = 0;
      tmp.msg = "attacks";
      attackData.Add(tmp);
    }

    ar >> Magic_Resistance;

    ar >> temp;
    Size = (creatureSizeType)temp;

    ar >> Morale;
    ar >> XP_Value;
    ar >> ItemMask;
    ar >> Form_Type;
    ar >> Penalty_Type;
    ar >> Immunity_Type;
    ar >> MiscOptions_Type;

    if (ver >= _VERSION_0750_)
    {
      ar >> temp;
      undeadStatus = (undeadType)temp;
    }
    else
      GuessUndeadStatus();
  }

  specAbs.Serialize(ar, ver);
  mon_asl.Serialize(ar);
}


void MONSTER_DATA::Serialize(CAR &ar, double ver)
{
  long temp;

  if (ar.IsStoring())
  {
    if (ver <= _VERSION_0830_)
      Name.Replace('/','|');
    ar << key;
    AS(ar,Name);
    Icon.Serialize(ar, ver);
    AS(ar,HitSound);
    AS(ar,MissSound);
    AS(ar,MoveSound);
    AS(ar,DeathSound);
    ar << Intelligence;
    ar << Armor_Class;
    ar << Movement;
    ar << Hit_Dice;
    ar << UseHitDice;
    ar << Hit_Dice_Bonus;
    ar << THACO;
    attackData.Serialize(ar, ver);
    ar << Magic_Resistance;
    temp = Size;
    ar << temp;
    ar << Morale;
    ar << XP_Value;
    ar << ItemMask;
    ar << Form_Type;
    ar << Penalty_Type;
    ar << Immunity_Type;
    ar << MiscOptions_Type;
    ar << undeadStatus;
  }
  else
  {
    ar >> key;
    DAS(ar,Name);
#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Kobald|Stupid" will be
    // read as simply "Kobald".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    if (Name.Find('|') >= 0) Name = Name.Left(Name.Find('|')); // Remove qualifier.
#endif

    if (ver < _VERSION_0640_)
    {
      CString tmp;
      DAS(ar,tmp);
      Icon.filename=tmp;
      Icon.picType = IconDib;
      Icon.SetDefaults();
    }
    else
    {
      Icon.Serialize(ar, ver);
      if (ver <= _VERSION_0840_)
        Icon.SetPre0840NonLoopDefaults();
    }
    DAS(ar,HitSound);
    DAS(ar,MissSound);
    if (ver >= _VERSION_0575_)
    {
      DAS(ar,MoveSound);
      DAS(ar,DeathSound);
    }
    else
    {
      MoveSound="DEFAULT";
      DeathSound="DEFAULT";
    }
    ar >> Intelligence;
    ar >> Armor_Class;
    ar >> Movement;
    ar >> Hit_Dice;
    if (ver >= _VERSION_0906_)
      ar >> UseHitDice;
    ar >> Hit_Dice_Bonus;
    ar >> THACO;

    if (ver < _VERSION_0750_)
    {
      long Nbr_Attacks;
      long Dmg_Dice_for_Attack;
      long Dmg_Dice_Bonus;
      long Nbr_Dmg_Dice;

      ar >> Nbr_Attacks;
      ar >> Dmg_Dice_for_Attack;
      ar >> Dmg_Dice_Bonus;
      ar >> Nbr_Dmg_Dice;

      if (Nbr_Attacks<=0) Nbr_Attacks=1;
      if (Dmg_Dice_for_Attack <= 0) Dmg_Dice_for_Attack = 10;
      if (Nbr_Dmg_Dice <= 0) Nbr_Dmg_Dice = 1;

      MONSTER_ATTACK_DETAILS tmp;
      tmp.sides = Dmg_Dice_for_Attack;
      tmp.nbr = Nbr_Dmg_Dice;
      tmp.bonus = Dmg_Dice_Bonus;
      tmp.msg = "attacks";
      for (int cnt=0;cnt<Nbr_Attacks;cnt++)
        attackData.Add(tmp);
    }
    else
    {
      attackData.Serialize(ar, ver);
    }

    if (attackData.GetCount()==0)
    {
      WriteDebugString("Forcing monster %s to have at least one attack\n", Name);
      MONSTER_ATTACK_DETAILS tmp;
      tmp.sides = 6;
      tmp.nbr = 1;
      tmp.bonus = 0;
      tmp.msg = "attacks";
      attackData.Add(tmp);
    }

    ar >> Magic_Resistance;

    ar >> temp;
    Size = (creatureSizeType)temp;

    ar >> Morale;
    ar >> XP_Value;
    ar >> ItemMask;
    ar >> Form_Type;
    ar >> Penalty_Type;
    ar >> Immunity_Type;
    ar >> MiscOptions_Type;

    if (ver >= _VERSION_0750_)
    {
      ar >> temp;
      undeadStatus = (undeadType)temp;
    }
    else
      GuessUndeadStatus();
  }

  specAbs.Serialize(ar, ver);
  mon_asl.Serialize(ar);

  if (ar.IsStoring())
  {
    myItems.Serialize(ar,ver);
    money.Serialize(ar, ver);
  }
  else
  {
    if (ver > _VERSION_0693_)
      myItems.Serialize(ar,ver);
    if (ver >= _VERSION_0906_)
      money.Serialize(ar, ver);
  }
}

//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::operator =
//
// PURPOSE: Make a copy of a MONSTER_DATA list
//*****************************************************************************
MONSTER_DATA_TYPE& MONSTER_DATA_TYPE:: operator = (MONSTER_DATA_TYPE& src)
{
  if (&src==this) return *this;
  MonsterData.RemoveAll();
  POSITION p=src.MonsterData.GetHeadPosition();
  while (p!=NULL)
  {
    AddMonsterWithCurrKey(src.MonsterData.GetNext(p));
  };
  //pos=src.pos;
  return *this;
}

//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::operator ==
//
// PURPOSE: Test that two MONSTER_DATA lists are identical...even the ordering.
//          One might like to exclude order but then it is a bit tricky
//          because of the possiblility of two identical entries in a list.
//*****************************************************************************
bool MONSTER_DATA_TYPE:: operator == (MONSTER_DATA_TYPE& src)
{
  POSITION p1=MonsterData.GetHeadPosition();
  POSITION p2=src.MonsterData.GetHeadPosition();
  while (p1!=NULL)
  {
    if (p2==NULL) return false;
    if (!(MonsterData.GetNext(p1)==src.MonsterData.GetNext(p2))) return false;
  };
  if (p2!=NULL) return false;
//  if (pos!=src.pos) return false;
  return true;
}


//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::Serialize
//
// PURPOSE:  
//*****************************************************************************
void MONSTER_DATA_TYPE::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);
  int i;
  POSITION pos;
  MONSTER_DATA data;
  if (ar.IsStoring())
  {
    ar << GetCount();
    pos = MonsterData.GetHeadPosition();
    while (pos != NULL)
      MonsterData.GetNext(pos).Serialize(ar, MONSTER_DATA_TYPE::version);
  }
  else
  {
    //Clear();
    //version = _VERSION_0662_;
    int temp;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      data.Serialize(ar, MONSTER_DATA_TYPE::version);
      AddMonsterWithCurrKey(data);
    }
  }
}

void MONSTER_DATA_TYPE::Serialize(CAR &ar, double ver)
{
  int i;
  MONSTER_DATA data;
  POSITION pos;
  if (ar.IsStoring())
  {
    ar << GetCount();
    pos = MonsterData.GetHeadPosition();
    while (pos != NULL)
    {
      //if (ver <= _VERSION_0693_)
      //  MonsterData.GetNext(pos).Serialize(ar);
      //else
        MonsterData.GetNext(pos).Serialize(ar, ver);
    }
  }
  else
  {
    //Clear();
    //version=ver;
    int temp;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      //if (ver <= _VERSION_0693_)
      //  data.Serialize(ar);
      //else
        data.Serialize(ar, ver);
      AddMonsterWithCurrKey(data);
    }
  }
}

void MONSTER_DATA_TYPE::Save(CArchive& ar)
{
	POSITION p;
	int count;
	if (version < _ASL_LEVEL_ ) return;
	count=MonsterData.GetCount();
	ar << count;
	p=MonsterData.GetHeadPosition();
	while (p!=NULL) {
		ar << MonsterData.GetAt(p).Name;
		MonsterData.GetAt(p).Save(ar);
		MonsterData.GetNext(p);
	};
}

void MONSTER_DATA_TYPE::Restore(CArchive& ar) 
{
	POSITION p;
	int count;
	CString name;
	if (version < _ASL_LEVEL_) return;
	p=MonsterData.GetHeadPosition();
	// Let items know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ASl in case no restore operation clears it.
	while (p!=NULL) MonsterData.GetNext(p).PrepareForRestore();
	ar >> count;
	for (int i=0; i<count; i++) 
	{
		ar >> name;
		p=MonsterData.GetHeadPosition();
		while (p!=NULL)
		{
			if (name==MonsterData.GetAt(p).Name) 
			{
				MonsterData.GetAt(p).Restore(ar);
				break;
			};
			MonsterData.GetNext(p);
		};
		if (p==NULL) 
		{
			CString msg;
			MONSTER_DATA junk;
			msg.Format("SAVE_FILE MONSTER_DATA \"%s\" for monster not in design\n",
				name);
			WriteDebugString(msg);
			junk.Restore(ar); // discard data
		};
	};
}

void MONSTER_DATA_TYPE::CommitRestore(void) {
	POSITION p;
	p=MonsterData.GetHeadPosition();
	while (p!=NULL) MonsterData.GetNext(p).CommitRestore();
}




//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::SaveSounds
//
// PURPOSE:  
//*****************************************************************************
void saveSounds(MONSTER_DATA_TYPE& data)
{
  data.SaveSounds();
}

void MONSTER_DATA_TYPE::SaveSounds()
{
  POSITION p = MonsterData.GetHeadPosition();
  while (p != NULL)
  {
    saveSound(MonsterData.GetAt(p).HitSound, GLOBAL_SOUND);
    saveSound(MonsterData.GetAt(p).MissSound, GLOBAL_SOUND);
    saveSound(MonsterData.GetAt(p).MoveSound, GLOBAL_SOUND);
    saveSound(MonsterData.GetAt(p).DeathSound, GLOBAL_SOUND);
    MonsterData.GetNext(p);
  }
}


//*****************************************************************************
// NAME: MONSTER_DATA_TYPE::SaveArt
//
// PURPOSE:  
//*****************************************************************************
void saveArt(MONSTER_DATA_TYPE& data)
{
  data.SaveArt();
}

void MONSTER_DATA_TYPE::SaveArt()
{
  POSITION p = MonsterData.GetHeadPosition();
  while (p != NULL)
  {
    MonsterData.GetAt(p).Icon.SavePicData(GLOBAL_ART);
    MonsterData.GetNext(p);
  }
}

void clearData(MONSTER_DATA_TYPE& data)
{
  data.Clear();
  data.version = PRODUCT_VER;
}

BOOL saveData(MONSTER_DATA_TYPE& data)
{
  EditorStatusMsg("Saving monster data...");

  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, MONSTER_DB_NAME);

  return saveData(data, fullPath);
}

BOOL saveData(MONSTER_DATA_TYPE& data, LPCSTR fullPath)
{
  BOOL success = TRUE;

  CFileException e;
  CFile myFile;

  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open monster db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  // put signal value and version at start of file
  __int64 hdr=0xFABCDEFABCDEFABF;
  double ver = PRODUCT_VER;
  data.version = ver;
  myFile.Write(&hdr, sizeof(hdr));
  myFile.Write(&ver, sizeof(double));

  CAR ar(&myFile, CArchive::store);
  //CArchive ar(&myFile, CArchive::store);

  try
  {
    data.PreSerialize(ar.IsStoring());
    data.Serialize(ar, ver);    
    data.PostSerialize(ar.IsStoring());
  }
  catch (...)
  {
    success = FALSE;
    WriteDebugString("Failed to read monster dat file\n");
  }

  ar.Close();
  return success;
}

int loadData(MONSTER_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;
  BOOL success=TRUE;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open monster db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  clearData(data);

  // check for version
  __int64 hdr=0;
  double ver;
  myFile.Read(&hdr, sizeof(hdr));
  if (hdr == 0xFABCDEFABCDEFABF)
  {
    myFile.Read(&ver, sizeof(double));
  }
  else
  {
    // special hdr not present, so reset archive to beginning
    myFile.SeekToBegin();
    // set version to last build that didn't save version
    ver = min(globalData.version, _VERSION_0662_);
    //ver = _VERSION_0662_;
  }

  data.version = ver;

  // if loading older level data, don't use compression archive
  if (ver <= _VERSION_0670_)
  {
    WriteDebugString("Loading pre-0670 version monster DB - %f\n", ver);

    CArchive ar(&myFile, CArchive::load);
    try
    {
       data.PreSerialize(ar.IsStoring());
       data.Serialize(ar);
       data.PostSerialize(ar.IsStoring());
    }
    catch (...)
    {
       WriteDebugString("Unable to load monster data file %s\n", ar.GetFile()->GetFileName());
       success=FALSE;
    }
    ar.Close();
  }
  else
  {
    WriteDebugString("Loading monster DB version: %f\n", ver);
    CAR ar(&myFile, CArchive::load);
    //ar.Compress(true); // qqqqq
    try
    {
       data.PreSerialize(ar.IsStoring());
       data.Serialize(ar, data.version);
       data.PostSerialize(ar.IsStoring());
    }
    catch (...)
    {
       WriteDebugString("Unable to load monster data file %s\n", ar.GetFile()->GetFileName());
       success=FALSE;
    }
    ar.Close();
  }

  if (data.GetCount() <= 0)
     WriteDebugString("No monsters in monster db file\n");
  
  return success; //data.GetCount();
}

void MONSTER_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  // before saving, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (IsStoring) ExpandDefaultFilenames();

  POSITION p = GetHeadPosition();
  while (p!=NULL) 
    GetNext(p).PreSerialize(IsStoring);
}

void MONSTER_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
  // after loading, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (!IsStoring) ExpandDefaultFilenames();

  POSITION p = GetHeadPosition();
  while (p!=NULL) 
    GetNext(p).PostSerialize(IsStoring);
}

void MONSTER_DATA::PreSerialize(BOOL IsStoring) 
{
  if (IsStoring)
  {    
    StripFilenamePath(Icon.filename);
    StripFilenamePath(HitSound);
    StripFilenamePath(MissSound);
    StripFilenamePath(MoveSound);
    StripFilenamePath(DeathSound);
  }
}

void MONSTER_DATA::PostSerialize(BOOL IsStoring) 
{
  AddDesignFolderToPath(Icon.filename);
  AddDesignFolderToPath(HitSound);
  AddDesignFolderToPath(MissSound);
  AddDesignFolderToPath(MoveSound);
  AddDesignFolderToPath(DeathSound);
}

void MONSTER_DATA_TYPE::ExpandDefaultFilenames()
{  
  POSITION p = GetHeadPosition();
  int key;

  while (p != NULL)
  {
    MONSTER_DATA data = GetAt(p);

    if (data.Icon.filename.CompareNoCase("DEFAULT") == 0)
    {
#ifdef UAFEDITOR
      data.Icon.filename.Format("%s%s",EditorArt, DEFAULT_MONSTER_ICON);
#else
      data.Icon.filename = DEFAULT_MONSTER_ICON;
#endif
      data.Icon.picType=IconDib;
      data.Icon.SetDefaults();
    }

    if (data.HitSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.HitSound.Format("%s%s",EditorArt, DEF_HIT_SOUND);
#else
      data.HitSound = DEF_HIT_SOUND;
#endif
    }
    if (data.MissSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.MissSound.Format("%s%s",EditorArt, DEF_MISS_SOUND);
#else
      data.MissSound = DEF_MISS_SOUND;
#endif
    }
    if (data.MoveSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.MoveSound.Format("%s%s",EditorArt, DEF_CHAR_MOVE_SOUND);
#else
      data.MoveSound = DEF_CHAR_MOVE_SOUND;
#endif
    }
    if (data.DeathSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.DeathSound.Format("%s%s",EditorArt, DEF_CHAR_DEATH_SOUND);
#else
      data.DeathSound = DEF_CHAR_DEATH_SOUND;
#endif
    }
    key = GetKeyAt(p);
    SetMonster(key, data);

    GetNext(p);
  }
}

#ifdef UAFEDITOR
void MONSTER_DATA_TYPE::CollapseDefaultFilenames()
{  
  int key;
  POSITION p = GetHeadPosition();
  CString DefFilename;

  while (p != NULL)
  {
    MONSTER_DATA data = GetAt(p);

    DefFilename.Format("%s%s",EditorArt, DEFAULT_MONSTER_ICON);
    if (data.Icon.filename.CompareNoCase(DefFilename)==0)
    {
      data.Icon.filename = "DEFAULT";
      data.Icon.picType=IconDib;
      data.Icon.SetDefaults();
    }

    DefFilename.Format("%s%s",EditorArt, DEF_HIT_SOUND);
    if (data.HitSound.CompareNoCase(DefFilename)==0)
      data.HitSound = "DEFAULT";

    DefFilename.Format("%s%s",EditorArt, DEF_MISS_SOUND);
    if (data.MissSound.CompareNoCase(DefFilename)==0)
      data.MissSound = "DEFAULT";

    DefFilename.Format("%s%s",EditorArt, DEF_CHAR_MOVE_SOUND);
    if (data.MoveSound.CompareNoCase(DefFilename)==0)
      data.MoveSound = "DEFAULT";

    DefFilename.Format("%s%s",EditorArt, DEF_CHAR_DEATH_SOUND);
    if (data.DeathSound.CompareNoCase(DefFilename)==0)
      data.DeathSound = "DEFAULT";

    key = GetKeyAt(p);
    SetMonster(key, data);
    GetNext(p);
  }
}

#endif // UAFEDITOR


void MONSTER_DATA::PlayHit()
{
  if (!PlaySoundEffects) return;
  if (!ValidSound(hHitSound))
    hHitSound = LoadSound(HitSound);
  if (!PlaySound(hHitSound,FALSE))
    hHitSound=-1;
}
void MONSTER_DATA::PlayMiss()
{
  if (!PlaySoundEffects) return;
  if (!ValidSound(hMissSound))
    hMissSound = LoadSound(MissSound);
  if (!PlaySound(hMissSound,FALSE))
    hMissSound=-1;
}
void MONSTER_DATA::PlayMove()
{
  if (!PlaySoundEffects) return;
  if (!ValidSound(hMoveSound))
    hMoveSound = LoadSound(MoveSound);

  if (!ValidSound(hMoveSound))
  {
    // default didn't work, try the char move sound
    MoveSound = DEF_CHAR_MOVE_SOUND;
    hMoveSound = LoadSound(MoveSound);
  }
  if (!ValidSound(hMoveSound))
  {
    // that didn't work either, try the party step sound
    MoveSound = globalData.sounds.PartyStep;
    hMoveSound = LoadSound(MoveSound);
  }

  if (!PlaySound(hMoveSound,FALSE))
    hMoveSound=-1;
}
void MONSTER_DATA::PlayDeath()
{
  if (!PlaySoundEffects) return;
  if (!ValidSound(hDeathSound))
    hDeathSound = LoadSound(DeathSound);

  if (!ValidSound(hDeathSound))
  {
    // default didn't work, try the char death sound
    DeathSound = DEF_CHAR_DEATH_SOUND;
    hDeathSound = LoadSound(DeathSound);
  }
  if (!PlaySound(hDeathSound,FALSE))
    hDeathSound=-1;
}

void MONSTER_DATA::ClearSounds()
{
  if (hHitSound >= 0)
    ClearSound(hHitSound);
  if (hMissSound >= 0)
    ClearSound(hMissSound);
  if (hMoveSound >= 0)
    ClearSound(hMoveSound);
  if (hDeathSound >= 0)
    ClearSound(hDeathSound);

  hHitSound = -1;
  hMissSound = -1;
  hMoveSound = -1;
  hDeathSound = -1;
}

int MONSTER_DATA_TYPE::GetNextKey()
{
  int key=0;
  POSITION pos;
  if (GetCount() > 0)
  {
    key = MonsterData.GetTail().key;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_MONSTERS.
      // Gaps occur when deletions are made to the monster list
      pos = MonsterData.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = MonsterData.GetNext(pos).key;
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
#ifdef UAFEDITOR
BOOL MONSTER_DATA_TYPE::Validate(MONSTER_DATA data, FILE *pFile)
{
  BOOL success = TRUE;
  // hit and miss sounds start with defaults, so if they are
  // now blank it is because of a designer decision
  if (data.HitSound != "")
  {
    if (!FileExists(data.HitSound))
    {
      WriteValidateLog(pFile, "Monster '%s' missing Hit Sound file '%s'\n",
                       data.Name, data.HitSound);
      success = FALSE;
    }
  }
  if (data.MissSound != "")
  {
    if (!FileExists(data.MissSound))
    {
      WriteValidateLog(pFile, "Monster '%s' missing Miss Sound file '%s'\n",
                       data.Name, data.MissSound);
      success = FALSE;
    }
  }
  if (data.MoveSound != "")
  {
    if (!FileExists(data.MoveSound))
    {
      WriteValidateLog(pFile, "Monster '%s' missing Move Sound file '%s'\n",
                       data.Name, data.MoveSound);
      success = FALSE;
    }
  }
  if (data.DeathSound != "")
  {
    if (!FileExists(data.DeathSound))
    {
      WriteValidateLog(pFile, "Monster '%s' missing Death Sound file '%s'\n",
                       data.Name, data.DeathSound);
      success = FALSE;
    }
  }
  // icons are required, else a default will be chosen by the
  // engine anyway.
  if (!FileExists(data.Icon.filename))
  {
    WriteValidateLog(pFile, "Monster '%s' missing Icon file '%s'\n",
                     data.Name, data.Icon.filename);
    success = FALSE;
  }

  POSITION pos = data.specAbs.GetHeadPosition();
  while (pos!=NULL)
  {
    SPEC_AB_DATA *spab = data.specAbs.GetNextData(pos);
    if (spab!=NULL)
    {
      if (!spab->script.IsEmpty())
      {
        if (spab->binary.GetLength() == 0)
        {
          success = FALSE;
          WriteValidateLog(pFile, "Monster '%s' special ability script '%s' did not compile\n",
                           data.Name, spab->script);
        }
      }
    }
  }
  return success;
}

BOOL MONSTER_DATA_TYPE::Validate(FILE *pFile)
{
  BOOL success = TRUE;
  POSITION pos = MonsterData.GetHeadPosition();
  while (pos != NULL)
  {
    // hit and miss sounds start with defaults, so if they are
    // now blank it is because of a designer decision
    if (!Validate(MonsterData.GetNext(pos),pFile))
      success = FALSE;
  }
  return success;
}

BOOL MONSTER_DATA_TYPE::Validate(int index, FILE *pFile)
{
  BOOL success = TRUE;
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  {
    // hit and miss sounds start with defaults, so if they are
    // now blank it is because of a designer decision
    if (!Validate(MonsterData.GetAt(pos),pFile))
      success = FALSE;
  }
  else
  {
    WriteValidateLog(pFile, "Bogus monster key %i\n", index);
    success = FALSE;
  }
  return success;
}
#endif
void MONSTER_DATA_TYPE::PlayHit(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    MonsterData.GetAt(pos).PlayHit();
}
void MONSTER_DATA_TYPE::PlayMiss(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    MonsterData.GetAt(pos).PlayMiss();
}
void MONSTER_DATA_TYPE::PlayMove(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    MonsterData.GetAt(pos).PlayMove();
}
void MONSTER_DATA_TYPE::PlayDeath(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    MonsterData.GetAt(pos).PlayDeath();
}
void MONSTER_DATA_TYPE::ClearSounds()
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  pos = MonsterData.GetHeadPosition();
  while (pos != NULL)
    MonsterData.GetNext(pos).ClearSounds();
}  
CString MONSTER_DATA_TYPE::GetMonsterName(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).Name;
  else
    return CString("No Monster");
}

BOOL MONSTER_DATA_TYPE::HaveMonster(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  return (MonsterData.FindKeyPos(index) != NULL);
}

// called when loading from scripts, or
// when adding from editor GUI
int MONSTER_DATA_TYPE::AddMonster(MONSTER_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (strlen(data.Name) == 0) return 0;
  // Do not allow multiple items with same full name.
  if (m_FindName(data.Name)!=0) return 0;
  if (MonsterData.GetCount() < MAX_MONSTERS)
  {
    data.key = GetNextKey();
    MonsterData.Insert(data, data.key);
    return data.key;
  }
  else
    return 0;
}

// called when loading binary monster data that
// was saved with key info
int MONSTER_DATA_TYPE::AddMonsterWithCurrKey(MONSTER_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (MonsterData.GetCount() < MAX_MONSTERS)
  {
    MonsterData.Insert(data, data.key);
    return data.key;
  }
  else
    return 0;
}

BOOL MONSTER_DATA_TYPE::SetMonster(int index, MONSTER_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  {
    MonsterData.GetAt(pos) = data;
    return TRUE;
  }
  else
    return FALSE;
}

BOOL MONSTER_DATA_TYPE::RemoveMonster(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  {
    MonsterData.RemoveAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}


MONSTER_DATA *MONSTER_DATA_TYPE::GetMonsterData(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return &MonsterData.GetAt(pos);
  else
    return NULL;
}

creatureSizeType MONSTER_DATA_TYPE::GetMonsterSize(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).Size;
  else
    return Small;
}

int MONSTER_DATA_TYPE::GetMonsterNbrAttacks(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).attackData.GetCount();
  else
    return 0;
}

int MONSTER_DATA_TYPE::GetMonsterExpValue(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  {
    int val = MonsterData.GetAt(pos).XP_Value;
    double mod = GetMonsterExpMod();
    if (mod != 0.0)
    {
      val += ((mod/100.0)*val);
    }
    else
    {
      mod = GetAllExpMod();
      if (mod != 0.0) 
        val += ((mod/100.0)*val);
    }
    if (val < 0) val=0;
    return val;
  }
  else
    return 0;
}

int MONSTER_DATA_TYPE::GetMonsterTHACO(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).THACO;
  else
    return 0;
}

int MONSTER_DATA_TYPE::GetMonsterAC(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).Armor_Class;
  else
    return 0;
}

void MONSTER_DATA_TYPE::GetMonsterAttackMsg(int index, int attackNum, CString &msg)
{
  CSingleLock sLock(&m_CS, TRUE);
  msg="";
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  {
    int attackMsgCnt = MonsterData.GetAt(pos).attackData.GetCount();
    if (attackMsgCnt==0) // shouldn't happen if monster configured properly
    {
      msg = "attacks";
      return;
    }

    // a monster can have more attacks than is natural due
    // to readying a weapon
    if (attackNum >= attackMsgCnt) attackNum = 0;    
    msg = MonsterData.GetAt(pos).attackData.GetAtIndex(attackNum).msg;
  }
  if (msg.GetLength()==0) msg = "attacks";
}

void MONSTER_DATA_TYPE::GetMonsterDamageDice(int index, int attackNum, int &num, int &sides, int &bonus)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  {
    int attackCnt = MonsterData.GetAt(pos).attackData.GetCount();
    ASSERT(attackCnt > 0);
    if (attackCnt == 0)
    {
      WriteDebugString("Monster %s has zero attacks defined\n", MonsterData.GetAt(pos).Name);
    }
    if (attackNum < attackCnt )
    {
      MONSTER_ATTACK_DETAILS tmp;
      tmp = MonsterData.GetAt(pos).attackData.GetAtIndex(attackNum);
      num = tmp.nbr;
      sides = tmp.sides;
      bonus = tmp.bonus;
    }
    else
    {
      num=0;sides=0;bonus=0;
    }
  }
  else
  {
    num=0;sides=0;bonus=0;
  }
}

BYTE MONSTER_DATA_TYPE::GetMonsterItemMask(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).ItemMask;
  else
    return 0;
}

double MONSTER_DATA_TYPE::GetMonsterHitDice(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  {
    double val = MonsterData.GetAt(pos).Hit_Dice;
    if (MonsterData.GetAt(pos).UseHitDice)
    {
      double mod = GetMonsterHDMod();
      if (mod != 0.0) val += ((mod/100.0)*val);
    }
    if (val < 0.0) val=0.5; // 1/2 hit dice is lowest possible
    return val;
  }
  else
    return 0.0;
}

int MONSTER_DATA_TYPE::GetMonsterHitDiceBonus(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).Hit_Dice_Bonus;
  else
    return 0;
}

int MONSTER_DATA_TYPE::GetMonsterMovement(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).Movement;
  else
    return 0;
}

DWORD MONSTER_DATA_TYPE::GetMonsterFormFlags(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).Form_Type;
  else
    return 0;
}

DWORD MONSTER_DATA_TYPE::GetMonsterOptionFlags(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).MiscOptions_Type;
  else
    return 0;
}

DWORD MONSTER_DATA_TYPE::GetMonsterImmunityFlags(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).Immunity_Type;
  else
    return 0;  
}

DWORD MONSTER_DATA_TYPE::GetMonsterPenaltyFlags(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
    return MonsterData.GetAt(pos).Penalty_Type;
  else
    return 0;  
}

// No locking... for internal use
int MONSTER_DATA_TYPE::m_FindName (const CString& name)
// Return key associated with the named object, or zero
{
  POSITION pos;
  pos=MonsterData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (MonsterData.GetAt(pos).Name==name) 
          return MonsterData.GetAt(pos).key;
    MonsterData.GetNext(pos);
  };
  return 0;
}
// With locking...for public use
int MONSTER_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}


// No locking... for internal use
int MONSTER_DATA_TYPE::m_FindNamei (const CString& name)
// Return key associated with the named object, or zero
{
  POSITION pos;
  pos=MonsterData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (MonsterData.GetAt(pos).Name.CompareNoCase(name)) 
          return MonsterData.GetAt(pos).key;
    MonsterData.GetNext(pos);
  };
  return 0;
}
// With locking...for public use
int MONSTER_DATA_TYPE::FindNamei(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindNamei(name);
}

ActorType MONSTER_DATA::GetContext()
{
  ActorType data;
  data.Clear();
  data.SetMonsterSrc();
  data.Instance = key;
  data.Level = Hit_Dice;
  return data;
}

ActorType MONSTER_DATA_TYPE::GetContext(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = MonsterData.FindKeyPos(index)) != NULL)
  {
    return MonsterData.GetAt(pos).GetContext();
  }
  else 
  {
    ActorType data;
    data.Clear();
    ASSERT(FALSE);
    return data;
  }
}