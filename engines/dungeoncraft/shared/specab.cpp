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

#include "dungeoncraft/shared/GPDLOpCodes.h"
#include "dungeoncraft/shared/GPDLcomp.h"
#include "dungeoncraft/shared/GPDLexec.h"

#ifdef UAFEngine
#include "dungeoncraft/shared/Char.h"
#include "dungeoncraft/shared/..\UAFWin\Combatant.h"
#endif

#include "dungeoncraft/shared/specab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( SPECIAL_ABILITIES, CObject, 1 )

GPDL specAbGPDL;

//
// utility functions
//
int ConvertSpecAbFlagToIndex(DWORD flag)
{
  switch (flag)
  {
  case SPECAB_MSG_NONE: return 0;
  case SPECAB_MSG_CASTING: return 1;
  case SPECAB_MSG_ACTIVATESPELL: return 2;
  case SPECAB_MSG_FLEE: return 3;
  case SPECAB_MSG_TURNUNDEAD: return 4;
  case SPECAB_MSG_BANDAGE: return 5;
  case SPECAB_MSG_GUARD: return 6;
  case SPECAB_MSG_ATTACK: return 7;
  case SPECAB_MSG_MOVE: return 8;
  case SPECAB_MSG_ENDTURN: return 9;
  case SPECAB_MSG_DELAY: return 10;
  case SPECAB_MSG_READY: return 11;
  }
  return 0;
}

DWORD ConvertSpecAbIndexToFlag(int index)
{
  switch (index)
  {
  case 0: return SPECAB_MSG_NONE;
  case 1: return SPECAB_MSG_CASTING;
  case 2: return SPECAB_MSG_ACTIVATESPELL;
  case 3: return SPECAB_MSG_FLEE;
  case 4: return SPECAB_MSG_TURNUNDEAD;
  case 5: return SPECAB_MSG_BANDAGE;
  case 6: return SPECAB_MSG_GUARD;
  case 7: return SPECAB_MSG_ATTACK;
  case 8: return SPECAB_MSG_MOVE;
  case 9: return SPECAB_MSG_ENDTURN;
  case 10: return SPECAB_MSG_DELAY;
  case 11: return SPECAB_MSG_READY;
  }
  return SPECAB_MSG_NONE;
}

specialAbilitiesType ConvertRuntimeIfTextToSpecAb(const CString &text)
{
  if (text.CompareNoCase("$CHAR_BLESS")==0) return SA_Bless;
  if (text.CompareNoCase("$CHAR_CURSE")==0) return SA_Curse;
  if (text.CompareNoCase("$CHAR_UNDEADFEAR")==0) return SA_UndeadFear;
  if (text.CompareNoCase("$CHAR_ENLARGE")==0) return SA_Enlarge;
  if (text.CompareNoCase("$CHAR_REDUCE")==0) return SA_Reduce;
  if (text.CompareNoCase("$CHAR_CHARMPERSON")==0) return SA_CharmPerson;
  if (text.CompareNoCase("$CHAR_DETECTMAGIC")==0) return SA_DetectMagic;
  if (text.CompareNoCase("$CHAR_REFLECTGAZEATTACK")==0) return SA_ReflectGazeAttack;
  if (text.CompareNoCase("$CHAR_PROTFROMEVIL")==0) return SA_ProtFromEvil;
  if (text.CompareNoCase("$CHAR_PROTFROMGOOD")==0) return SA_ProtFromGood;
  if (text.CompareNoCase("$CHAR_SHIELD")==0) return SA_Shield;
  if (text.CompareNoCase("$CHAR_SLEEP")==0) return SA_Sleep;
  if (text.CompareNoCase("$CHAR_FOG")==0) return SA_Fog;
  if (text.CompareNoCase("$CHAR_ENTANGLE")==0) return SA_Entangle;
  if (text.CompareNoCase("$CHAR_INVISIBLETOANIMALS")==0) return SA_InvisibleToAnimals;
  if (text.CompareNoCase("$CHAR_INVISIBLETOUNDEAD")==0) return SA_InvisibleToUndead;
  if (text.CompareNoCase("$CHAR_NONUNDEADFEAR")==0) return SA_NonUndeadFear;
  if (text.CompareNoCase("$CHAR_SANCTUARY")==0) return SA_Sanctuary;
  if (text.CompareNoCase("$CHAR_SHILLELAGH")==0) return SA_Shillelagh;
  if (text.CompareNoCase("$CHAR_DISPLACEMENT")==0) return SA_Displacement;
  if (text.CompareNoCase("$CHAR_WIZARDRY")==0) return SA_Wizardry;
  if (text.CompareNoCase("$CHAR_VORPALATTACK")==0) return SA_VorpalAttack;
  if (text.CompareNoCase("$CHAR_HOLDPERSON")==0) return SA_HoldPerson;
  if (text.CompareNoCase("$CHAR_SILENCE")==0) return SA_Silence;  
  if (text.CompareNoCase("$CHAR_POISONED")==0) return SA_Poisoned;  
  if (text.CompareNoCase("$CHAR_SLOWPOISON")==0) return SA_SlowPoison;  
  if (text.CompareNoCase("$CHAR_MIRRORIMAGE")==0) return SA_MirrorImage;    
  if (text.CompareNoCase("$CHAR_INVISIBLE")==0) return SA_Invisible;
  if (text.CompareNoCase("$CHAR_ENFEEBLED")==0) return SA_Enfeebled;
  if (text.CompareNoCase("$CHAR_BLINDNESS")==0) return SA_Blindness;
  if (text.CompareNoCase("$CHAR_DISEASED")==0) return SA_Diseased;

  ASSERT(FALSE); // unhandled special ability
  return SA_None;
}

CString ConvertSpecAbToRuntimeIfText(specialAbilitiesType sa)
{
  CString akey="";
  switch (sa)
  {
  case SA_Bless: akey="$CHAR_BLESS"; break; //
  case SA_Curse: akey="$CHAR_CURSE"; break; //
  case SA_UndeadFear: akey="$CHAR_UNDEADFEAR"; break;
  case SA_Enlarge: akey="$CHAR_ENLARGE"; break; //
  case SA_Reduce: akey="$CHAR_REDUCE"; break; //
  case SA_CharmPerson: akey="$CHAR_CHARMPERSON"; break;
  case SA_DetectMagic: akey="$CHAR_DETECTMAGIC"; break; //
  case SA_ReflectGazeAttack: akey="$CHAR_REFLECTGAZEATTACK"; break;
  case SA_ProtFromEvil: akey="$CHAR_PROTFROMEVIL"; break; //
  case SA_ProtFromGood: akey="$CHAR_PROTFROMGOOD"; break; //
  case SA_Shield: akey="$CHAR_SHIELD"; break; //
  case SA_Sleep: akey="$CHAR_SLEEP"; break;
  case SA_Fog: akey="$CHAR_FOG"; break;
  case SA_Entangle: akey="$CHAR_ENTANGLE"; break; //
  case SA_InvisibleToAnimals: akey="$CHAR_INVISIBLETOANIMALS"; break; //
  case SA_InvisibleToUndead: akey="$CHAR_INVISIBLETOUNDEAD"; break; //
  case SA_NonUndeadFear: akey="$CHAR_NONUNDEADFEAR"; break;
  case SA_Sanctuary: akey="$CHAR_SANCTUARY"; break; //
  case SA_Shillelagh: akey="$CHAR_SHILLELAGH"; break; //
  case SA_Displacement: akey="$CHAR_DISPLACEMENT"; break; //
  case SA_Wizardry: akey="$CHAR_WIZARDRY"; break;
  case SA_VorpalAttack: akey="$CHAR_VORPALATTACK"; break;
  case SA_HoldPerson: akey="$CHAR_HOLDPERSON";break;
  case SA_Silence: akey="$CHAR_SILENCE"; break;
  case SA_Poisoned: akey="$CHAR_POISONED"; break;
  case SA_SlowPoison: akey="$CHAR_SLOWPOISON"; break;
  case SA_MirrorImage: akey="$CHAR_MIRRORIMAGE"; break;
  case SA_Invisible: akey="$CHAR_INVISIBLE"; break;
  case SA_Enfeebled: akey="$CHAR_ENFEEBLED"; break;
  case SA_Blindness: akey="$CHAR_BLINDNESS"; break;
  case SA_Diseased: akey="$CHAR_DISEASED"; break;
  default: ASSERT(FALSE); // unhandled special ability
  }
  return akey;
}

///////////////////////////////////////////////////////////////////////////////


SPEC_AB_DATA::SPEC_AB_DATA() 
{ 
  Clear(); 
}

void SPEC_AB_DATA::Clear() 
{ 
  script=""; 
  binary="";
  da_script=""; 
  da_binary="";
  MsgTypes=SPECAB_MSG_NONE;
  DisplayOnce=TRUE;
  for (int i=0;i<NUM_SPECAB_MSGS;i++)
    Msgs[i]="";
}  
  
SPEC_AB_DATA& SPEC_AB_DATA::operator=(const SPEC_AB_DATA& src)
{
  if (this==&src) return *this;
  script=src.script;
  binary=src.binary;
  da_script=src.da_script;
  da_binary=src.da_binary;
  MsgTypes=src.MsgTypes;
  DisplayOnce=src.DisplayOnce;
  for (int i=0;i<NUM_SPECAB_MSGS;i++)
    Msgs[i]=src.Msgs[i];
  return *this;
}

bool SPEC_AB_DATA::operator==(const SPEC_AB_DATA& src) const
{
  if (this==&src) return true;
  if (script.CompareNoCase(src.script)!=0) return false;
  if (da_script.CompareNoCase(src.da_script)!=0) return false;
  if (MsgTypes!=src.MsgTypes) return false;
  if (DisplayOnce!=src.DisplayOnce) return false;
  for (int i=0;i<NUM_SPECAB_MSGS;i++)
  {
    if (Msgs[i].CompareNoCase(src.Msgs[i])!=0) 
      return false;
  }
  return true;
}

void SPEC_AB_DATA::Serialize(CArchive &ar, double ver)
{
  if (ar.IsStoring())
  {
    AS(ar, script);
    AS(ar, binary);
    AS(ar, da_script);
    AS(ar, da_binary);
    ar << MsgTypes;
    ar << DisplayOnce;
    ar << NUM_SPECAB_MSGS;
    for (int i=0;i<NUM_SPECAB_MSGS;i++)
      AS(ar, Msgs[i]);
  }
  else
  {
    if (ver == _VERSION_0850_)
    {
      int unused;
      ar >> unused;
    }

    DAS(ar, script);
    if (ver >= _VERSION_0851_)
      DAS(ar, binary);

    if (ver >= _VERSION_0852_)
    {
      DAS(ar, da_script);
      DAS(ar, da_binary);
    }
    if (ver >= _VERSION_0870_)
    {
      int count;
      CString msg;
      ar >> MsgTypes;
      ar >> DisplayOnce;
      ar >> count;      
      if (count > NUM_SPECAB_MSGS)
      {
        WriteDebugString("Invalid SpecAbMsg count %i in Serialize\n", count);
        ASSERT(FALSE);
      }
      for (int i=0;i<count;i++)
      {
        DAS(ar, msg);
        if (i < NUM_SPECAB_MSGS)
          Msgs[i] = msg;          
      }
    }
  }
}

bool SPEC_AB_DATA::ExecuteScript()
{
  // The script execution can return "1" (for true) 
  // or empty string (for false). 
  //
  // If true, the script indicates it is
  // activated. Whatever that means depends
  // on the special ability this script 
  // represents. (returns true)
  //
  // If false, then the script does not
  // want to become active under the 
  // current circumstances. (returns false)
  //
  // If no script is present, then this
  // special ability is not enabled (returns false)
  //
  // If the script cannot be executed for
  // some reason, then this special ability
  // cannot activate (returns false)

  if (script.IsEmpty()) return false;
  if (binary.GetLength()==0) return false;
  //if (specAbGPDL.State()==GPDL_UNINITIALIZED) return false;
  if (specAbGPDL.ExecuteScript(binary,1)=="1") return true;
  return false;
}

bool SPEC_AB_DATA::ExecuteDAScript()
{
  if (da_script.IsEmpty()) return false;
  if (da_binary.GetLength()==0) return false;
  //if (specAbGPDL.State()==GPDL_UNINITIALIZED) return false;
  if (specAbGPDL.ExecuteScript(da_binary,1)=="1") return true;
  return false;
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

SPECIAL_ABILITIES::SPECIAL_ABILITIES() 
{ 
  Clear(); 
}

SPECIAL_ABILITIES::SPECIAL_ABILITIES(const SPECIAL_ABILITIES& src) 
{ 
  *this = src; 
}

SPECIAL_ABILITIES::~SPECIAL_ABILITIES() 
{ 
  Clear(); 
}

void SPECIAL_ABILITIES::Clear() 
{ 
  for (int i=0;i<NUM_SPECIAL_ABILITIES;i++)
    specAbData[i].Clear();
}

SPECIAL_ABILITIES& SPECIAL_ABILITIES::operator=(const SPECIAL_ABILITIES& src)
{
  if (this==&src) return *this;
  for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
    specAbData[i]=src.specAbData[i];
  return *this;
}

bool SPECIAL_ABILITIES::operator==(const SPECIAL_ABILITIES& src) const
{
  if (this==&src) return true;
  for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
  {
    if ( !(specAbData[i]==src.specAbData[i]) ) 
      return false;
  }
  return true;
}

void SPECIAL_ABILITIES::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);
  if (ar.IsStoring())
  {
    ar << NUM_SPECIAL_ABILITIES;
    for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
      specAbData[i].Serialize(ar, ver);
  }
  else
  {
    int count;
    if (ver < _VERSION_0850_)
    {
      // load the old way, just an array of integers
      WORD temp; 
      int i;
      CArray<WORD, WORD> specAbs;

      ar >> count;
      if (count < 0) count = 0;
      if (count > NUM_SPECIAL_ABILITIES) count = NUM_SPECIAL_ABILITIES;

      specAbs.SetSize(count);
      for (i=0; i<count; i++)
      {
        ar >> temp;
        specAbs[i] = temp;
      }

      for (i=0; i<specAbs.GetSize(); i++)
      {
        if (specAbs[i] > 0)
          EnableSpecAb((specialAbilitiesType)specAbs[i]); // with default scripts
      }
    }
    else
    {
      ar >> count;
      // if either of these two are invalid, the loading
      // has gone wrong somewhere, or, possibly loading a
      // file that was saved as a newer version
      if (count < 0) count = 0;
      if (count > NUM_SPECIAL_ABILITIES) count = NUM_SPECIAL_ABILITIES;
      for (int i=0; i<count; i++)
        specAbData[i].Serialize(ar, ver);
    }
  }
}

CString SPECIAL_ABILITIES::GenerateSpecAbFuncStart(specialAbilitiesType sa)
{
  CString name;
  CString script;
  name = GenerateSpecAbFuncName(sa);
  if (name.IsEmpty()) return "";
  script.Format("$PUBLIC $FUNC %s() { ", name);
  return script;
}

CString SPECIAL_ABILITIES::GenerateSpecAbFuncEnd(specialAbilitiesType sa)
{
  CString name;
  CString script;

  name = GenerateSpecAbFuncName(sa);
  if (name.IsEmpty()) return "";
  script.Format(" } %s;", name);
  return script;
}

DWORD SPECIAL_ABILITIES::GenerateDefaultSpecAbMsgTypes(specialAbilitiesType sa)
{
  DWORD types=SPECAB_MSG_NONE;
  switch (sa)
  {
  case SA_None: 
    break;
  case SA_Bless:    
    types = SPECAB_MSG_READY;
    break;
  case SA_Curse:
    types = SPECAB_MSG_READY;
    break;
  case SA_UndeadFear:
    types = SPECAB_MSG_NONE;
    break;
  case SA_Enlarge:    
    types = SPECAB_MSG_READY;
    break;
  case SA_Reduce:    
    types = SPECAB_MSG_READY;
    break;
  case SA_CharmPerson:    
    types = SPECAB_MSG_NONE;
    break;
  case SA_DetectMagic:
    types = SPECAB_MSG_READY;
    break;
  case SA_ReflectGazeAttack:
    types = SPECAB_MSG_NONE;
    break;
  case SA_ProtFromEvil:
    types = SPECAB_MSG_READY;
    break;
  case SA_ProtFromGood:
    types = SPECAB_MSG_READY;
    break;
  case SA_Shield:
    types = SPECAB_MSG_READY;
    break;
  case SA_Sleep:
    types = SPECAB_MSG_NONE;
    break;
  case SA_Fog:
    types = SPECAB_MSG_NONE;
    break;
  case SA_Entangle:
    types = SPECAB_MSG_NONE;
    break;
  case SA_InvisibleToAnimals:
    types = SPECAB_MSG_READY;
    break;
  case SA_InvisibleToUndead:
    types = SPECAB_MSG_READY;
    break;
  case SA_NonUndeadFear:
    types = SPECAB_MSG_NONE;
    break;
  case SA_Sanctuary:
    types = SPECAB_MSG_READY;
    break;
  case SA_Shillelagh:
    types = SPECAB_MSG_READY;
    break;
  case SA_Displacement:
    types = SPECAB_MSG_READY;
    break;
  case SA_Wizardry:
    types = SPECAB_MSG_READY;
    break;
  case SA_VorpalAttack:
    types = SPECAB_MSG_ATTACK;
    break;
  case SA_HoldPerson:
    types = SPECAB_MSG_ENDTURN;
    break;
  case SA_Silence:
    types = SPECAB_MSG_ENDTURN;
    break;
  case SA_Poisoned:
    types = SPECAB_MSG_ENDTURN;
    break;
  case SA_SlowPoison:
    types = SPECAB_MSG_NONE;
    break;    
  case SA_MirrorImage:
    types = SPECAB_MSG_NONE;
    break;  
  case SA_Invisible:
    types = SPECAB_MSG_NONE;
    break;      
  case SA_Enfeebled:
    types = SPECAB_MSG_ENDTURN;
    break;         
  case SA_Blindness:
    types = SPECAB_MSG_ENDTURN;
    break;
  case SA_Diseased:
    types = SPECAB_MSG_ENDTURN;
    break;
  }
  return types;
}

CString SPECIAL_ABILITIES::GenerateDefaultSpecAbMsgFormat(specialAbilitiesType sa)
{
  CString msg("");
  switch (sa)
  {
  case SA_None: 
    break;
  case SA_Bless:    
    msg = "/u blesses";
    break;
  case SA_Curse:
    msg = "/u curses";
    break;
  case SA_UndeadFear:
    //msg = "/u fears undead";
    break;
  case SA_Enlarge:    
    msg = "/u is enlarged";
    break;
  case SA_Reduce:    
    msg = "/u is reduced";
    break;
  case SA_CharmPerson:    
    //msg = "/u is charmed";
    break;
  case SA_DetectMagic:
    msg = "/u detects magic";
    break;
  case SA_ReflectGazeAttack:
    //msg = "/u reflects gaze attack";
    break;
  case SA_ProtFromEvil:
    msg = "/u is protected from evil";
    break;
  case SA_ProtFromGood:
    msg = "/u is protected from good";
    break;
  case SA_Shield:
    msg = "/u is shielded";
    break;
  case SA_Sleep:
    //msg = "/u is asleep";
    break;
  case SA_Fog:
    //msg = "/u is fogged";
    break;
  case SA_Entangle:
    //msg = "/u is entangled";
    break;
  case SA_InvisibleToAnimals:
    msg = "/u is invisible to animals";
    break;
  case SA_InvisibleToUndead:
    msg = "/u is invisible to undead";
    break;
  case SA_NonUndeadFear:
    //msg = "/u is afraid";
    break;
  case SA_Sanctuary:
    msg = "/u has sanctuary";
    break;
  case SA_Shillelagh:
    msg = "/u has shillelagh";
    break;
  case SA_Displacement:
    msg = "/u is displaced";
    break;
  case SA_Wizardry:
    msg = "/u has wizardry";
    break;
  case SA_VorpalAttack:
    msg = "/u beheads /t";
    break;
  case SA_HoldPerson:
    msg = "/t is held";
    break;
  case SA_Silence:
    msg = "/t is silenced";
    break;
  case SA_Poisoned:
    msg = "/t is poisoned";
    break;    
  case SA_SlowPoison:
    //msg = "/t is poisoned";
    break;        
  case SA_MirrorImage:
    //msg = "/t is poisoned";
    break;  
  case SA_Invisible:
    //msg = "/t is poisoned";
    break;      
  case SA_Enfeebled:
    msg = "/t is enfeebled";
    break;         
  case SA_Blindness:
    msg = "/t is blinded";
    break;
  case SA_Diseased:
    msg = "/t is diseased";
    break;
  }
  return msg;
}

CString SPECIAL_ABILITIES::GenerateDefaultSpecAbScript(specialAbilitiesType sa)
{
  CString name;
  CString start;
  CString ability;
  CString end;
  CString script;

  start = GenerateSpecAbFuncStart(sa);
  if (start.IsEmpty()) return "";
  end = GenerateSpecAbFuncEnd(sa);
  if (end.IsEmpty()) return "";

  name = GenerateSpecAbFuncName(sa);
  ability = name.Mid(3);
  ability.MakeUpper();
  ability = "$SET_CHAR_" + ability;

  script.Format("%s$RETURN %s($IndexOf($Target()),true);%s", start,ability,end);
  return script;
} 

CString SPECIAL_ABILITIES::GenerateDefaultSpecAbDAScript(specialAbilitiesType sa)
{
  CString name;
  CString start;
  CString ability;
  CString end;
  CString script;

  start = GenerateSpecAbFuncStart(sa);
  if (start.IsEmpty()) return "";
  end = GenerateSpecAbFuncEnd(sa);
  if (end.IsEmpty()) return "";

  name = GenerateSpecAbFuncName(sa);
  ability = name.Mid(3);
  ability.MakeUpper();
  ability = "$SET_CHAR_" + ability;

  script.Format("%s$RETURN %s($IndexOf($Target()),false);%s", start,ability,end);
  return script;
} 

SPEC_AB_DATA *SPECIAL_ABILITIES::GetSpecAbData(specialAbilitiesType sa)
{
  if ((sa < FIRST_SPECIAL_ABILITY)||(sa > LAST_SPECIAL_ABILITY))
    return NULL;
  return (&specAbData[sa]);
}

CString SPECIAL_ABILITIES::GenerateSpecAbFuncName(specialAbilitiesType sa)
{
  CString name="";
  switch (sa)
  {
  case SA_None: 
    break;
  case SA_Bless:
    name="SA_Bless";
    break;
  case SA_Curse:
    name="SA_Curse";
    break;
  case SA_UndeadFear:
    name="SA_UndeadFear";
    break;
  case SA_Enlarge:
    name="SA_Enlarge";
    break;
  case SA_Reduce:
    name="SA_Reduce";
    break;
  case SA_CharmPerson:
    name="SA_CharmPerson";
    break;
  case SA_DetectMagic:
    name="SA_DetectMagic";
    break;
  case SA_ReflectGazeAttack:
    name="SA_ReflectGazeAttack";
    break;
  case SA_ProtFromEvil:
    name="SA_ProtFromEvil";
    break;
  case SA_ProtFromGood:
    name="SA_ProtFromGood";
    break;
  case SA_Shield:
    name="SA_Shield";
    break;
  case SA_Sleep:
    name="SA_Sleep";
    break;
  case SA_Fog:
    name="SA_Fog";
    break;
  case SA_Entangle:
    name="SA_Entangle";
    break;
  case SA_InvisibleToAnimals:
    name="SA_InvisibleToAnimals";
    break;
  case SA_InvisibleToUndead:
    name="SA_InvisibleToUndead";
    break;
  case SA_NonUndeadFear:
    name="SA_NonUndeadFear";
    break;
  case SA_Sanctuary:
    name="SA_Sanctuary";
    break;
  case SA_Shillelagh:
    name="SA_Shillelagh";
    break;
  case SA_Displacement:
    name="SA_Displacement";
    break;
  case SA_Wizardry:
    name="SA_Wizardry";
    break;
  case SA_VorpalAttack:
    name="SA_VorpalAttack";
    break;
  case SA_HoldPerson:
    name="SA_HoldPerson";
    break;
  case SA_Silence:
    name="SA_Silence";
    break;
  case SA_Poisoned:
    name="SA_Poisoned";
    break;
  case SA_SlowPoison:
    name="SA_SlowPoison";
    break;    
  case SA_MirrorImage:
    name="SA_MirrorImage";
    break;            
  case SA_Invisible:
    name="SA_Invisible";
    break;                
  case SA_Enfeebled:
    name="SA_Enfeebled";
    break;         
  case SA_Blindness:
    name="SA_Blindness";
    break;
  case SA_Diseased:
    name="SA_Diseased";
    break;
  }
  return name;
}

void SPECIAL_ABILITIES::EnableSpecAb(specialAbilitiesType sa, CString scr, CString dascr)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return;
  if (scr.IsEmpty()) scr = GenerateDefaultSpecAbScript(sa);
  if (dascr.IsEmpty()) dascr = GenerateDefaultSpecAbDAScript(sa);  
  pdata->script = scr;
  pdata->da_script = dascr;
  pdata->MsgTypes = GenerateDefaultSpecAbMsgTypes(sa);
  CompileSpecAbScript(sa);  
  CompileSpecAbDAScript(sa);  
}

void SPECIAL_ABILITIES::DisableSpecAb(specialAbilitiesType sa)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return;
  pdata->script="";
  pdata->binary="";
  pdata->da_script="";
  pdata->da_binary="";
}

bool SPECIAL_ABILITIES::HaveSpecAb(specialAbilitiesType sa)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return false;
  if (pdata->script.IsEmpty()) return false;
  return true;
}

bool SPECIAL_ABILITIES::HaveAtLeastOneSpecAb()
{
  for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
  {
    if (!specAbData[i].script.IsEmpty())
      return true;
  }
  return false;
}

bool SPECIAL_ABILITIES::CompileSpecAbScript(specialAbilitiesType sa)
{  
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return false;
  pdata->binary="";
  if (pdata->script.IsEmpty()) return false;

#ifdef UAFEDITOR
  GPDLCOMP gpdlComp;  
  CString func = GenerateSpecAbFuncName(sa);
  const char *allowedEntry[2] = { func, NULL };
  
  CString script(pdata->script);
  //script.Remove('\r');
  //script.Remove('\n');
  //script.Remove('\t');

  pdata->binary = gpdlComp.CompileScript(script, allowedEntry);
  if (pdata->binary[0]==0) return true;
#endif

  pdata->binary="";
  return false;
}

bool SPECIAL_ABILITIES::CompileSpecAbDAScript(specialAbilitiesType sa)
{  
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return false;
  pdata->da_binary="";
  if (pdata->da_script.IsEmpty()) return false;

#ifdef UAFEDITOR
  GPDLCOMP gpdlComp;  
  CString func = GenerateSpecAbFuncName(sa);
  const char *allowedEntry[2] = { func, NULL };
  
  CString script(pdata->da_script);
  //script.Remove('\r');
  //script.Remove('\n');
  //script.Remove('\t');

  pdata->da_binary = gpdlComp.CompileScript(script, allowedEntry);
  if (pdata->da_binary[0]==0) return true;
#endif

  pdata->da_binary="";
  return false;
}

bool SPECIAL_ABILITIES::ExecuteSpecAbScript(specialAbilitiesType sa)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return false;
  return (pdata->ExecuteScript());
}

bool SPECIAL_ABILITIES::ExecuteSpecAbDAScript(specialAbilitiesType sa)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return false;
  return (pdata->ExecuteDAScript());
}

// the whole script
CString SPECIAL_ABILITIES::GetSpecAbScript(specialAbilitiesType sa)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return "";
  return (pdata->script);
}

// no $FUNC { } etc
CString SPECIAL_ABILITIES::GetSpecAbUserScript(specialAbilitiesType sa) 
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return "";
  CString script(pdata->script);

  CString start;
  CString end;  

  start = GenerateSpecAbFuncStart(sa);
  if (start.IsEmpty()) return "";
  end = GenerateSpecAbFuncEnd(sa);
  if (end.IsEmpty()) return "";

  script.Replace(start, "");
  script.Replace(end, "");
  return script;
}

CString SPECIAL_ABILITIES::GetSpecAbBinary(specialAbilitiesType sa)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return "";
  return (pdata->binary);
}

// the whole script
CString SPECIAL_ABILITIES::GetSpecAbDAScript(specialAbilitiesType sa)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return "";
  return (pdata->da_script);
}

// no $FUNC { } etc
CString SPECIAL_ABILITIES::GetSpecAbUserDAScript(specialAbilitiesType sa) 
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return "";
  CString script(pdata->da_script);

  CString start;
  CString end;  

  start = GenerateSpecAbFuncStart(sa);
  if (start.IsEmpty()) return "";
  end = GenerateSpecAbFuncEnd(sa);
  if (end.IsEmpty()) return "";

  script.Replace(start, "");
  script.Replace(end, "");
  return script;
}

CString SPECIAL_ABILITIES::GetSpecAbBinaryDA(specialAbilitiesType sa)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return "";
  return (pdata->da_binary);
}
//
// these assist with iterating through only the
// enabled spec abs.
//
POSITION SPECIAL_ABILITIES::GetHeadPosition()
{  
  for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
  {
    if (!specAbData[i].script.IsEmpty())
      return (POSITION)i;
  }
  return 0;
}

specialAbilitiesType SPECIAL_ABILITIES::GetAt(POSITION pos)
{
  int index = (int)pos;
  if ((index < FIRST_SPECIAL_ABILITY)||(index > LAST_SPECIAL_ABILITY))
    return SA_None;
  return ((specialAbilitiesType)index);
}

specialAbilitiesType SPECIAL_ABILITIES::GetNext(POSITION &pos)
{
  int index = (int)pos;
  pos=0;
  
  if ((index < FIRST_SPECIAL_ABILITY)||(index > LAST_SPECIAL_ABILITY))
    return SA_None;

  // point pos to next enabled spec ab
  for (int i=index+1; i<NUM_SPECIAL_ABILITIES; i++)
  {
    if (!specAbData[i].script.IsEmpty())
    {
      pos = (POSITION)i;
      break;
    }
  }
  return ((specialAbilitiesType)index);
}

SPEC_AB_DATA *SPECIAL_ABILITIES::GetAtData(POSITION pos)
{
  int index = (int)pos;
  if ((index < FIRST_SPECIAL_ABILITY)||(index > LAST_SPECIAL_ABILITY))
    return NULL;
  return (&specAbData[index]);
}

SPEC_AB_DATA *SPECIAL_ABILITIES::GetNextData(POSITION &pos)
{
  int index = (int)pos;
  pos=0;
  
  if ((index < FIRST_SPECIAL_ABILITY)||(index > LAST_SPECIAL_ABILITY))
    return NULL;

  // point pos to next enabled spec ab
  for (int i=index+1; i<NUM_SPECIAL_ABILITIES; i++)
  {
    if (!specAbData[i].script.IsEmpty())
    {
      pos = (POSITION)i;
      break;
    }
  }
  return (&specAbData[index]);
}

DWORD SPECIAL_ABILITIES::GetMsgTypes(specialAbilitiesType sa)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return SPECAB_MSG_NONE;
  return (pdata->MsgTypes);
}

void SPECIAL_ABILITIES::SetMsgTypes(specialAbilitiesType sa, DWORD types)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return;
  pdata->MsgTypes = types;
}

void SPECIAL_ABILITIES::AddMsgType(specialAbilitiesType sa, DWORD type)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return;
  pdata->MsgTypes |= type;
}

bool SPECIAL_ABILITIES::HaveMsgType(specialAbilitiesType sa, DWORD type)
{
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return FALSE;
  return ((pdata->MsgTypes & type) ? true : false);
}

CString SPECIAL_ABILITIES::GetMsgFormat(specialAbilitiesType sa, DWORD type)
{
  int index = ConvertSpecAbFlagToIndex(type);
  if ((index <= 0)||(index >= NUM_SPECAB_MSGS))
    return "";
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return "";
  return (pdata->Msgs[index]);
}

void SPECIAL_ABILITIES::SetMsgFormat(specialAbilitiesType sa, DWORD type, CString &format)
{
  int index = ConvertSpecAbFlagToIndex(type);
  if ((index <= 0)||(index >= NUM_SPECAB_MSGS))
    return;
  SPEC_AB_DATA *pdata = GetSpecAbData(sa);
  if (pdata==NULL) return;
  pdata->Msgs[index] = format;
}

#ifdef UAFEngine

void SPECIAL_ABILITIES::EnableAllFor(CHARACTER *pChar)
{
  SetCharContext(pChar->GetContext());
  SetTargetContext(pChar->GetContext());

  for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
    specAbData[i].ExecuteScript();
  
  ClearCharContext();
  ClearTargetContext();
}

void SPECIAL_ABILITIES::EnableAllFor(COMBATANT *pChar)
{
  SetCharContext(pChar->GetContext());
  SetTargetContext(pChar->GetContext());

  for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
      specAbData[i].ExecuteScript();

  ClearCharContext();
  ClearTargetContext();
}

void SPECIAL_ABILITIES::DisableAllFor(CHARACTER *pChar)
{
  SetCharContext(pChar->GetContext());
  SetTargetContext(pChar->GetContext());

  for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
    specAbData[i].ExecuteDAScript();
  
  ClearCharContext();
  ClearTargetContext();
}

void SPECIAL_ABILITIES::DisableAllFor(COMBATANT *pChar)
{
  SetCharContext(pChar->GetContext());
  SetTargetContext(pChar->GetContext());

  for (int i=0; i<NUM_SPECIAL_ABILITIES; i++)
    specAbData[i].ExecuteDAScript();

  ClearCharContext();
  ClearTargetContext();
}

#endif