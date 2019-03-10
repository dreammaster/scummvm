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

//#ifdef UAFEDITOR
//#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
//#else
//#include "dungeoncraft/shared/externs.h"
//#include "dungeoncraft/shared/..\UAFWin\dgngame.h"
//#endif

#include "dungeoncraft/shared/SurfaceMgr.h"
#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/PicData.h"
#include "dungeoncraft/shared/PicSlot.h"
#include "dungeoncraft/shared/SoundMgr.h"
#include "dungeoncraft/shared/spell.h"
#include "dungeoncraft/shared/GlobalData.h"
#ifdef UAFEngine
#include "dungeoncraft/shared/..\UAFWin\combatants.h"
#include "dungeoncraft/shared/..\UAFWin\DrawTile.h"
#endif
#include "dungeoncraft/shared/party.h"

#include "dungeoncraft/shared/GPDLOpCodes.h"
#include "dungeoncraft/shared/GPDLcomp.h"
#include "dungeoncraft/shared/GPDLexec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( SPELL_DATA, CObject, 1 )
IMPLEMENT_SERIAL( SPELL_DATA_TYPE, CObject, 1 )

SPELL_DATA_TYPE spellData;

#ifdef UAFEngine
SPELL_TEXT_LIST spellListText;
PendingSpellList PendingSpellData;
ActiveSpellList ActiveSpellData;
#endif

GPDL spellGPDL;

///////////////////////////////////////////////////////////////////////
//
// SPELL_EFFECTS_DATA Functions
//
///////////////////////////////////////////////////////////////////////

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::SPELL_EFFECTS_DATA
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_EFFECTS_DATA::SPELL_EFFECTS_DATA() 
{ 
  Clear(); 
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::SPELL_EFFECTS_DATA
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_EFFECTS_DATA::SPELL_EFFECTS_DATA(const SPELL_EFFECTS_DATA &src) 
{ 
  *this=src; 
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::~SPELL_EFFECTS_DATA
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_EFFECTS_DATA::~SPELL_EFFECTS_DATA() 
{ 
  //Clear(); 
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_EFFECTS_DATA::Serialize(CAR &ar, double version)
{
  if (ar.IsStoring())
  {
    AS(ar,affectedAttr);
    ar << flags;
    ar << changeResult;
    AS(ar,activationScript);
    ar << key;
    ar << parent;
    AS(ar, activationBinary);
    AS(ar, modificationScript);
    AS(ar, modificationBinary);
    AS(ar, savingThrowScript);
    AS(ar, savingThrowBinary); 
    AS(ar, savingThrowFailedScript);
    AS(ar, savingThrowFailedBinary); 
    AS(ar, savingThrowSucceededScript);
    AS(ar, savingThrowSucceededBinary); 
    ar << StopTime;
    ar << data;
  }
  else
  {
    CString unused;
    Clear();
    DAS(ar,affectedAttr);
    if (version <= _VERSION_0682_)
      ar >> unused; // was changeText
    ar >> flags;
    if (version >= _VERSION_0690_)
      ar >> changeResult;
    
    if (version >= _VERSION_0699_)
    {
      DAS(ar,activationScript);
      ar >> key;
      ar >> parent;
    }
    if (version >= _VERSION_0851_)
    {
      DAS(ar, activationBinary);
    }
    
    if (version >= _VERSION_0904_)
    {
      DAS(ar, modificationScript);
      DAS(ar, modificationBinary);       
      DAS(ar, savingThrowScript);
      DAS(ar, savingThrowBinary);       

      if (version >= _VERSION_0910_)
      {
        DAS(ar, savingThrowFailedScript);
        DAS(ar, savingThrowFailedBinary);       
        DAS(ar, savingThrowSucceededScript);
        DAS(ar, savingThrowSucceededBinary);       
      }
    }    

    if (version >= _VERSION_0906_)
    {
      ar >> StopTime;
    }      

    if (version >= _VERSION_0909_)
    {
      ar >> data;
    }
  }
  changeData.Serialize(ar);
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_EFFECTS_DATA::Serialize(CArchive &ar, double version)
{
  if (ar.IsStoring())
  {
    AS(ar,affectedAttr);
    ar << flags;
    ar << changeResult;
    AS(ar,activationScript);
    ar << key;
    ar << parent;
    AS(ar, activationBinary);
    AS(ar, modificationScript);
    AS(ar, modificationBinary); 
    AS(ar, savingThrowScript);
    AS(ar, savingThrowBinary);         
    AS(ar, savingThrowFailedScript);
    AS(ar, savingThrowFailedBinary);         
    AS(ar, savingThrowSucceededScript);
    AS(ar, savingThrowSucceededBinary);         
    ar << StopTime;
    ar << data;
  }
  else
  {
    CString unused;
    Clear();
    DAS(ar,affectedAttr);
    if (version <= _VERSION_0682_)
      ar >> unused; // was changeText
    ar >> flags;
    if (version >= _VERSION_0690_)
      ar >> changeResult;
    
    if (version >= _VERSION_0699_)
    {
      DAS(ar,activationScript);
      ar >> key;
      ar >> parent;
    }
    if (version >= _VERSION_0851_)
    {
      DAS(ar,activationBinary);
    }
    if (version >= _VERSION_0904_)
    {
      DAS(ar, modificationScript);
      DAS(ar, modificationBinary);
      DAS(ar, savingThrowScript);
      DAS(ar, savingThrowBinary);            

      if (version >= _VERSION_0910_)
      {
        DAS(ar, savingThrowFailedScript);
        DAS(ar, savingThrowFailedBinary);            
        DAS(ar, savingThrowSucceededScript);
        DAS(ar, savingThrowSucceededBinary);            
      }
    }        
    if (version >= _VERSION_0906_)
    {
      ar >> StopTime;
    }   
    if (version >= _VERSION_0909_)
    {
      ar >> data;
    }
  }
  changeData.Serialize(ar);
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::PreSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_EFFECTS_DATA::PreSerialize(BOOL IsStoring) 
{ 
  changeData.PreSerialize(IsStoring); 
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::operator==
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_EFFECTS_DATA::operator==(const SPELL_EFFECTS_DATA &rhs) const
{
  if (affectedAttr != rhs.affectedAttr) return false;
  if (changeResult != rhs.changeResult) return false;
  if (activationScript != rhs.activationScript) return false;
  if (modificationScript != rhs.modificationScript) return false;
  if (savingThrowScript != rhs.savingThrowScript) return false;
  if (savingThrowFailedScript != rhs.savingThrowFailedScript) return false;
  if (savingThrowSucceededScript != rhs.savingThrowSucceededScript) return false;
  if (changeData != rhs.changeData) return false;
  if (flags != rhs.flags) return false;
  // don't include dynamic data
  return true;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_EFFECTS_DATA::Clear() 
{
  ClearChangeResult();
  affectedAttr="";
  activationScript="";
  modificationScript="";
  savingThrowScript="";
  savingThrowFailedScript="";
  savingThrowSucceededScript="";
  activationBinary="";
  modificationBinary="";
  savingThrowBinary="";
  savingThrowFailedBinary="";
  savingThrowSucceededBinary="";
  StopTime=0;
  data=0;
  changeData.Clear();
  flags=SPELL_EFFECTS_DATA::EFFECT_TARGET|EFFECT_CUMULATIVE;
  // this data is dynamic and is
  // determined when spell is cast.
  parent=0;
  key=0;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::GetData
//
// PURPOSE: 
//
//*****************************************************************************
DWORD SPELL_EFFECTS_DATA::GetData() const
{
  return data;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::SetData
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_EFFECTS_DATA::SetData(DWORD val)
{
  data = val;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::ApplyChange
//
// PURPOSE: 
//
//*****************************************************************************
double SPELL_EFFECTS_DATA::ApplyChange(double src)
{
  double amount=0.0;
  if (changeResult==-1234567890123456789.0) // not rolled yet
  {
    ASSERT(FALSE); // GetChange() should be called at least once before this
    GetChange();
  }
  if (flags & EFFECT_PCNT)
    amount = ((changeResult*0.01)*src);
  else if (flags & EFFECT_ABS)
    amount = changeResult;
  else
    amount = (src+changeResult);
  return amount;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::GetChange
//
// PURPOSE: 
//
//*****************************************************************************
double SPELL_EFFECTS_DATA::GetChange() 
{ 
  changeData.Roll(changeResult);
  spellGPDL.SetIntermediateResult(changeResult);
  changeResult += ExecuteModificationScript();
  return changeResult; 
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::ClearChangeResult
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_EFFECTS_DATA::ClearChangeResult() 
{ 
  changeResult=-1234567890123456789.0; 
  spellGPDL.SetIntermediateResult(0.0);
}
   
//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::ExecuteActivationScript
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_EFFECTS_DATA::ExecuteActivationScript(void) const
{
  // The script execution can return "1" (for true) 
  // or empty string (for false). 
  //
  // If true, the script indicates it is
  // activated. Whatever that means depends
  // on the spell effect this script 
  // represents. (returns true)
  //
  // If false, then the script does not
  // want to become active under the 
  // current circumstances. (returns false)
  //
  // If no script is present, then this
  // spell effect is automatically enabled (returns true)
  //
  // If the script cannot be executed for
  // some reason, then this spell effect
  // cannot activate (returns false)
  
    
  bool success = true;
  if (!activationScript.IsEmpty())
  {
    if (activationBinary.GetLength()>0)
    {
      if (spellGPDL.ExecuteScript(activationBinary,1)!="1") 
        success = false;
    }
    else
      success = false; // should have been compiled previously!
  }
  
  return success;  
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::ExecuteModificationScript
//
// PURPOSE: 
//
//*****************************************************************************
double SPELL_EFFECTS_DATA::ExecuteModificationScript() const
{
  // The modification script returns a value that is treated as a
  // delta to the existing 'Change By' value for this attribute.
  // Absolute and Percentage settings do not affect how this
  // modification is treated, it is always a delta. 
  //
  // The 'Change By' value is computed first, then this mod is
  // added to it, and then the Delta/Percentage/Absolute flags
  // are used to determine how the input value is changed to it's
  // final value.
  // 
  // This script is optional.
  
  double dresult = 0.0;
  
  if (!modificationScript.IsEmpty())
  {
    if (modificationBinary.GetLength() > 0)
    {
      CString sresult = spellGPDL.ExecuteScript(modificationBinary,1);
      dresult = atof(sresult);
    }
  }  
  return dresult;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::ExecuteSavingThrowScript
//
// PURPOSE: 
//
//*****************************************************************************
double SPELL_EFFECTS_DATA::ExecuteSavingThrowScript() const
{
  // The saving throw script returns a value that is treated as a
  // bonus (plus or minus) to the saving throw rolled by a character
  //
  // This script is optional.
  
  double dresult = 0.0;
  
  if (!savingThrowScript.IsEmpty())
  {
    if (savingThrowBinary.GetLength() > 0)
    {
      CString sresult = spellGPDL.ExecuteScript(savingThrowBinary,1);
      dresult = atof(sresult);
    }
  }  
  return dresult;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::ExecuteSavingThrowFailedScript
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_EFFECTS_DATA::ExecuteSavingThrowFailedScript() const
{
  // This script is optional.
    
  if (!savingThrowFailedScript.IsEmpty())
  {
    if (savingThrowFailedBinary.GetLength() > 0)
    {
      spellGPDL.ExecuteScript(savingThrowFailedBinary,1);
    }
  }  
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::ExecuteSavingThrowSucceededScript
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_EFFECTS_DATA::ExecuteSavingThrowSucceededScript() const
{
  // This script is optional.
  
  if (!savingThrowSucceededScript.IsEmpty())
  {
    if (savingThrowSucceededBinary.GetLength() > 0)
    {
      spellGPDL.ExecuteScript(savingThrowSucceededBinary,1);
    }
  }  
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::CompileScripts
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_EFFECTS_DATA::CompileScripts(const CString &basename)
{
  bool success = true;
  activationBinary="";  
  modificationBinary="";
  savingThrowBinary="";
  savingThrowFailedBinary="";
  savingThrowSucceededScript="";
  
#ifdef UAFEDITOR 
  const char *allowedEntry[2];
  
  GPDLCOMP gpdlComp;
  
  if (!activationScript.IsEmpty()) 
  {
    CString func(basename);
    func += GetEffectActivationFuncName();
    CString ascript;
    CString scriptbegin;
    CString scriptend;

    scriptbegin.Format("$PUBLIC $FUNC %s() { ", func);
    scriptend.Format(" } %s;", func);
    ascript = scriptbegin + activationScript + scriptend;
    
    allowedEntry[0] = (LPCSTR)func;
    allowedEntry[1] = NULL;     
    activationBinary = gpdlComp.CompileScript(ascript, allowedEntry);
    
    if (activationBinary[0]!=0) success = false;
  }
  
  if (!modificationScript.IsEmpty()) 
  {
    CString func(basename);
    func += GetEffectModificationFuncName();
    CString mscript;
    CString scriptbegin;
    CString scriptend;
    
    scriptbegin.Format("$PUBLIC $FUNC %s() { ", func);
    scriptend.Format(" } %s;", func);
    mscript = scriptbegin + modificationScript + scriptend;
    
    allowedEntry[0] = (LPCSTR)func;
    allowedEntry[1] = NULL;     
    modificationBinary = gpdlComp.CompileScript(mscript, allowedEntry);
    
    if (modificationBinary[0]!=0) success = false;
  }

  if (!savingThrowScript.IsEmpty()) 
  {
    CString func(basename);
    func += GetEffectSavingThrowFuncName();
    CString mscript;
    CString scriptbegin;
    CString scriptend;
    
    scriptbegin.Format("$PUBLIC $FUNC %s() { ", func);
    scriptend.Format(" } %s;", func);
    mscript = scriptbegin + savingThrowScript + scriptend;
    
    allowedEntry[0] = (LPCSTR)func;
    allowedEntry[1] = NULL;     
    savingThrowBinary = gpdlComp.CompileScript(mscript, allowedEntry);
    
    if (savingThrowBinary[0]!=0) success = false;
  }

  if (!savingThrowFailedScript.IsEmpty()) 
  {
    CString func(basename);
    func += GetEffectSavingThrowFailedFuncName();
    CString mscript;
    CString scriptbegin;
    CString scriptend;
    
    scriptbegin.Format("$PUBLIC $FUNC %s() { ", func);
    scriptend.Format(" } %s;", func);
    mscript = scriptbegin + savingThrowFailedScript + scriptend;
    
    allowedEntry[0] = (LPCSTR)func;
    allowedEntry[1] = NULL;     
    savingThrowFailedBinary = gpdlComp.CompileScript(mscript, allowedEntry);
    
    if (savingThrowFailedBinary[0]!=0) success = false;
  }

  if (!savingThrowSucceededScript.IsEmpty()) 
  {
    CString func(basename);
    func += GetEffectSavingThrowSucceededFuncName();
    CString mscript;
    CString scriptbegin;
    CString scriptend;
    
    scriptbegin.Format("$PUBLIC $FUNC %s() { ", func);
    scriptend.Format(" } %s;", func);
    mscript = scriptbegin + savingThrowSucceededScript + scriptend;
    
    allowedEntry[0] = (LPCSTR)func;
    allowedEntry[1] = NULL;     
    savingThrowSucceededBinary = gpdlComp.CompileScript(mscript, allowedEntry);
    
    if (savingThrowSucceededBinary[0]!=0) success = false;
  }
#endif
  
  if (!success)
  {
    activationBinary="";
    modificationBinary="";
    savingThrowBinary="";
    savingThrowFailedBinary="";
    savingThrowSucceededBinary="";
  }
  
  return success;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::GetEffectActivationFuncName
//
// PURPOSE: 
//
//*****************************************************************************
CString SPELL_EFFECTS_DATA::GetEffectActivationFuncName(void)
{
  CString name("");
  name.Format("%s_a_%u", affectedAttr, GetTickCount());
  return name;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::GetEffectModificationFuncName
//
// PURPOSE: 
//
//*****************************************************************************
CString SPELL_EFFECTS_DATA::GetEffectModificationFuncName(void)
{
  CString name("");
  name.Format("%s_m_%u", affectedAttr, GetTickCount());
  return name;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::GetEffectSavingThrowFuncName
//
// PURPOSE: 
//
//*****************************************************************************
CString SPELL_EFFECTS_DATA::GetEffectSavingThrowFuncName(void)
{
  CString name("");
  name.Format("%s_s_%u", affectedAttr, GetTickCount());
  return name;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::GetEffectSavingThrowFailedFuncName
//
// PURPOSE: 
//
//*****************************************************************************
CString SPELL_EFFECTS_DATA::GetEffectSavingThrowFailedFuncName(void)
{
  CString name("");
  name.Format("%s_sf_%u", affectedAttr, GetTickCount());
  return name;
}

//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::GetEffectSavingThrowSucceededFuncName
//
// PURPOSE: 
//
//*****************************************************************************
CString SPELL_EFFECTS_DATA::GetEffectSavingThrowSucceededFuncName(void)
{
  CString name("");
  name.Format("%s_ss_%u", affectedAttr, GetTickCount());
  return name;
}

#ifdef UAFEngine
//*****************************************************************************
//    NAME: SPELL_EFFECTS_DATA::ProcessTimeSensitiveData
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_EFFECTS_DATA::IsReadyToExpire()
{
  if (StopTime == 0) return TRUE;

  SPELL_DATA *pSpell = spellData.GetSpellData(key);
  
  if (pSpell == NULL)
  {
    ASSERT(FALSE);
    return TRUE;
  }
  
  switch (pSpell->Duration_Rate)
  {
  case byNbrAttacks:
  case byDamageTaken: 
    ASSERT(FALSE);
    return TRUE;
    
  default:
    return (party.getElapsedMinutes() > StopTime);
  }
}
#endif

///////////////////////////////////////////////////////////////////////
//
// Character Spell Functions
//
///////////////////////////////////////////////////////////////////////

//*****************************************************************************
//    NAME: CHARACTER_SPELL::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER_SPELL::Clear() 
{ 
  memorized=0; 
  spell=NO_SPELL; 
  level=0; 
  selected=0; 
  memTime=0;
  JustMemorized=FALSE;
}

//*****************************************************************************
//    NAME: CHARACTER_SPELL::operator=
//
// PURPOSE: 
//
//*****************************************************************************
CHARACTER_SPELL& CHARACTER_SPELL::operator=(const CHARACTER_SPELL &cSpell)
{
  if (&cSpell == this) return *this;
  selected = cSpell.selected;
  memorized = cSpell.memorized;
  spell = cSpell.spell;
  level = cSpell.level;
  memTime = cSpell.memTime;
  JustMemorized=cSpell.JustMemorized;
  return *this;
}

//*****************************************************************************
//    NAME: CHARACTER_SPELL::operator==
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER_SPELL::operator==(const CHARACTER_SPELL &src)
{
  if (&src == this) return TRUE;
  //if (selected != src.selected) return FALSE;
  //if (memorized != src.memorized) return FALSE;
  if (spell != src.spell) return FALSE;
  if (level != src.level) return FALSE;
  //if (JustMemorized != src.JustMemorized) return FALSE;
  //if (memTime != src.memTime) return FALSE; // don't include this
  return TRUE;
}

//*****************************************************************************
//    NAME: CHARACTER_SPELL::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER_SPELL::Serialize(CArchive &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << spell;
    ar << memorized;
    ar << level;
    ar << selected;
  }
  else
  {
    Clear();
    ar >> spell;
    ar >> memorized;
    ar >> level;
    ar >> selected;
  }
}

//*****************************************************************************
//    NAME: CHARACTER_SPELL::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER_SPELL::Serialize(CAR &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << spell;
    ar << memorized;
    ar << level;
    ar << selected;
  }
  else
  {
    Clear();
    ar >> spell;
    ar >> memorized;
    ar >> level;
    ar >> selected;
  }
}

//*****************************************************************************
//    NAME: CHARACTER_SPELL::IncMemorizedTime
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER_SPELL::IncMemorizedTime(int minuteInc) 
{ 
  JustMemorized=FALSE;
  if ((selected<=0) || (selected <= memorized)) return FALSE;
  memTime += minuteInc; 
  if (MemTimeSufficient())
  {
    SetMemorized();
    return TRUE;
  }  
  return FALSE;
}

//*****************************************************************************
//    NAME: CHARACTER_SPELL::HaveUnmemorized
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER_SPELL::HaveUnmemorized()
{
  if (selected <= 0) return FALSE;
  return (selected > memorized);
}

//*****************************************************************************
//    NAME: CHARACTER_SPELL::MemTimeSufficient
//
// PURPOSE: 
//
//*****************************************************************************
BOOL CHARACTER_SPELL::MemTimeSufficient() 
{ 
  return (memTime >= GetSpellMemorizeTime(level)); 
}

//*****************************************************************************
//    NAME: CHARACTER_SPELL::SetMemorized
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER_SPELL::SetMemorized() 
{ 
  if ((!selected) || (selected <= memorized)) return;
  memorized++;
  JustMemorized=TRUE;
  memTime=0; 
}

//*****************************************************************************
//    NAME: CHARACTER_SPELL::SetUnMemorized
//
// PURPOSE: 
//
//*****************************************************************************
void CHARACTER_SPELL::SetUnMemorized() 
{ 
  if ((!selected) || (memorized <= 0)) return;
  memorized--;
}


///////////////////////////////////////////////////////////////////////
//
// Spell List Functions
//
///////////////////////////////////////////////////////////////////////

//*****************************************************************************
//    NAME: SPELL_LIST::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_LIST::Serialize(CArchive &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << GetCount();
    POSITION p = spells.GetHeadPosition();
    while (p != NULL)
      spells.GetNext(p).Serialize(ar, version);
      //ar << spells.GetNext(p);  
  }
  else
  {
    int count;
    CHARACTER_SPELL spell;
    ar >> count;  
    for (int i=0; i<count; i++)
    {
      //ar >> spell;
      spell.Serialize(ar, version);
      AddSpell(spell);
    }
  }
}

//*****************************************************************************
//    NAME: SPELL_LIST::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_LIST::Serialize(CAR &ar, double version)
{
  if (ar.IsStoring())
  {
    ar << GetCount();
    POSITION p = spells.GetHeadPosition();
    while (p != NULL)
      spells.GetNext(p).Serialize(ar, version);
      //ar << spells.GetNext(p);  
  }
  else
  {
    int count;
    CHARACTER_SPELL spell;
    ar >> count;  
    for (int i=0; i<count; i++)
    {
      //ar >> spell;
      spell.Serialize(ar, version);
      AddSpell(spell);
    }
  }
}

//*****************************************************************************
//    NAME: SPELL_LIST::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_LIST::Clear() 
{ 
  spells.RemoveAll();
  sortedAccess.RemoveAll();
}
//*****************************************************************************
//    NAME: SPELL_LIST::GetCount
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_LIST::GetCount() const 
{ 
  return spells.GetCount(); 
}

//*****************************************************************************
//    NAME: SPELL_LIST::operator=
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_LIST& SPELL_LIST::operator=(const SPELL_LIST &list)
{
  if (this==&list)return *this;
  Clear();
  spells = list.spells;
  //SortSpells(); // update ordered list
  //sortedAccess not copied
  return *this;
}

//*****************************************************************************
//    NAME: SPELL_LIST::operator==
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::operator==(const SPELL_LIST &src)
{
  if (&src == this) return TRUE;
  POSITION p1=spells.GetHeadPosition();
  POSITION p2=const_cast<SPELL_LIST&>(src).spells.GetHeadPosition();
  while (p1!=NULL)
  {
    if (p2==NULL) return FALSE;
    if (!(spells.GetNext(p1)==const_cast<SPELL_LIST&>(src).spells.GetNext(p2))) return FALSE;
  }
  if (p2!=NULL) return FALSE;
  return TRUE;
}

//*****************************************************************************
//    NAME: SPELL_LIST::GetHeadPosition
//
// PURPOSE: 
//
//*****************************************************************************
POSITION SPELL_LIST::GetHeadPosition() 
{ 
  POSITION pos = sortedAccess.GetHeadPosition();
  if (pos == NULL)
  {
    if (spells.GetCount() == 0) return NULL;
    SortSpells();
    pos = sortedAccess.GetHeadPosition();
  }
  return pos;
}
//*****************************************************************************
//    NAME: SPELL_LIST::GetAt
//
// PURPOSE: 
//
//*****************************************************************************
CHARACTER_SPELL& SPELL_LIST::GetAt(POSITION pos) 
{   
  ASSERT( sortedAccess.GetCount() > 0);
  POSITION pos2 = spells.FindKeyPos(sortedAccess.GetAt(pos));
  if (pos2 != NULL)
  {
    return spells.GetAt(pos2);
  }
  else
  {
    ASSERT(FALSE);
    WriteDebugString("Failed to find CHAR_SPELL in SPELL_LIST::GetAt()\n");
    return spells.GetHead();
  }  
}
//*****************************************************************************
//    NAME: SPELL_LIST::GetNext
//
// PURPOSE: 
//
//*****************************************************************************
CHARACTER_SPELL& SPELL_LIST::GetNext(POSITION &pos) 
{ 
  ASSERT( sortedAccess.GetCount() > 0);
  POSITION pos2 = spells.FindKeyPos(sortedAccess.GetNext(pos));
  if (pos2 != NULL)
  {
    return spells.GetAt(pos2);
  }
  else
  {
    ASSERT(FALSE);
    WriteDebugString("Failed to find CHAR_SPELL in SPELL_LIST::GetNext()\n");
    return spells.GetHead();
  }  
}
//*****************************************************************************
//    NAME: SPELL_LIST::GetKeyAt
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_LIST::GetKeyAt(POSITION pos)
{ 
  ASSERT( sortedAccess.GetCount() > 0);
  POSITION pos2 = spells.FindKeyPos(sortedAccess.GetAt(pos));
  if (pos2 != NULL)
    return spells.GetKeyAt(pos2);
  else
    return 0;
}

//*****************************************************************************
//    NAME: SPELL_LIST::IsMemorized
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::IsMemorized(int index)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
    return (spells.GetAt(pos).IsMemorized());
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_LIST::DecMemorized
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::DecMemorized(int index, int memorized_count)
{
  if (memorized_count==0) return FALSE;
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
  {
    for (int i=0;i<memorized_count;i++)
      spells.GetAt(pos).SetUnMemorized();
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_LIST::SetSelected
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::SetSelected(int index, int selected)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
  {
    spells.GetAt(pos).selected = selected;
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_LIST::IsSelected
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::IsSelected(int index)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
    return (spells.GetAt(pos).IsSelected());
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_LIST::HaveSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::HaveSpell(int index) const
{
  return (spells.HaveKey(index));
}

//*****************************************************************************
//    NAME: SPELL_LIST::AddSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::AddSpell(CHARACTER_SPELL &data)
{
  if (!HaveSpell(data.spell))
  {
    BOOL result = (spells.Insert(data, data.spell));
    //if (result) SortSpells();
    return result;
  }
  else
    return TRUE;
}

struct CHAR_SPELL_SORT_DATA
{
  int level;
  int type;
  CString name;
  CHARACTER_SPELL origData;
};

//*****************************************************************************
//    NAME: compare
//
// PURPOSE: 
//
//*****************************************************************************
int compare( const void *arg1, const void *arg2 )
{
  /*
    must return
  < 0 elem1 less than elem2 
  = 0 elem1 equivalent to elem2 
  > 0 elem1 greater than elem2 
  */
  ASSERT( (arg1!=NULL) && (arg2!=NULL) );
  if ((arg1==NULL)||(arg2==NULL)) return 0;
  CHAR_SPELL_SORT_DATA *pElem1 = (CHAR_SPELL_SORT_DATA*)arg1;
  CHAR_SPELL_SORT_DATA *pElem2 = (CHAR_SPELL_SORT_DATA*)arg2;

  // sort by spell class, then by level, then by spell name
  if (pElem1->type < pElem2->type) return -1;
  if (pElem1->type > pElem2->type) return  1;

  if (pElem1->type == pElem2->type)
  {
    if (pElem1->level < pElem2->level) return -1;
    if (pElem1->level > pElem2->level) return  1;
    if (pElem1->level == pElem2->level)
    {
      if (pElem1->name < pElem2->name) return -1;
      if (pElem1->name > pElem2->name) return  1;
    }
  }

  // type, level, and name are all equal
  return 0;
}

//*****************************************************************************
//    NAME: SPELL_LIST::SortSpells
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_LIST::SortSpells(void)
{
  sortedAccess.RemoveAll();
  if (spellData.GetCount() == 0) return;
  if (spells.GetCount() == 0) return;

  CHAR_SPELL_SORT_DATA tsortspell;
  CArray<CHAR_SPELL_SORT_DATA, CHAR_SPELL_SORT_DATA> sortList;

  POSITION pos = spells.GetHeadPosition();
  while (pos != NULL)
  {
    CHARACTER_SPELL &tspell = spells.GetAt(pos);
    SPELL_DATA *pSdata = spellData.GetSpellData(spells.GetAt(pos).spell);
    if (pSdata==NULL) 
    {
      WriteDebugString("Spell book references invalid spellData %u\n", spells.GetAt(pos).spell);
      spells.GetNext(pos);
      continue;
    }
    tsortspell.origData = tspell;
    tsortspell.level = pSdata->Level;
    tsortspell.name = pSdata->Name;
    tsortspell.type = pSdata->ClassMask;
    sortList.SetAtGrow(sortList.GetSize(), tsortspell);
    spells.GetNext(pos);
  }

  if (sortList.GetSize() == 0)
  {
    ASSERT( FALSE );
    return;
  }

  if (sortList.GetSize() <= 1) 
  {
    sortedAccess.RemoveAll();
    sortedAccess.AddTail(sortList[0].origData.spell);
    return;
  }

  // now all spells are in a array, so
  // start the sorting
  qsort( &sortList[0], sortList.GetSize(), sizeof(CHAR_SPELL_SORT_DATA), compare);
  
  sortedAccess.RemoveAll();

  // now the list is sorted, re-insert into spell list  
  for (int i=0;i<sortList.GetSize();i++)
    sortedAccess.AddTail(sortList[i].origData.spell);
}

//*****************************************************************************
//    NAME: SPELL_LIST::RemoveSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::RemoveSpell(int index)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
  {
    spells.RemoveAt(pos);
    //SortSpells(); // update ordered list
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_LIST::RemoveSpellLevel
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_LIST::RemoveSpellLevel(int level)
{
  POSITION currPos;
  POSITION pos = spells.GetHeadPosition();    
  while (pos != NULL)
  {
    currPos = pos;
    spells.GetNext(pos);
    if (level == spells.GetAt(currPos).level) 
      spells.RemoveAt(currPos);
  }
  //SortSpells(); // update ordered list
}

//*****************************************************************************
//    NAME: SPELL_LIST::SpellsMemorized
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_LIST::SpellsMemorized()
{
  int count = 0;
  POSITION pos = spells.GetHeadPosition();    
  while (pos != NULL)
    count += spells.GetNext(pos).memorized;
  return count;
}

//*****************************************************************************
//    NAME: SPELL_LIST::GetSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::GetSpell(int index, CHARACTER_SPELL &spell)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
  {
    spell = spells.GetAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_LIST::getSelectedCount
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_LIST::getSelectedCount(int index)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
    return spells.GetAt(pos).selected;
  else
    return 0;
}

//*****************************************************************************
//    NAME: SPELL_LIST::getMemorizedCount
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_LIST::getMemorizedCount(int index)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
    return spells.GetAt(pos).memorized;
  else
    return 0;
}

//*****************************************************************************
//    NAME: SPELL_LIST::getMemorizedTime
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_LIST::getMemorizedTime(int index)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
    return spells.GetAt(pos).memTime;
  else
    return 0;
}

//*****************************************************************************
//    NAME: SPELL_LIST::resetMemorizedTime
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_LIST::resetMemorizedTime(int index)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
  {
    int prev = spells.GetAt(pos).memTime;
    spells.GetAt(pos).memTime = 0;
    return prev;
  }
  else
    return 0;
}

//*****************************************************************************
//    NAME: SPELL_LIST::incMemorizedTime
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::incMemorizedTime(int index, int minuteInc)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
    return (spells.GetAt(pos).IncMemorizedTime(minuteInc));
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_LIST::SetSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_LIST::SetSpell(int index, CHARACTER_SPELL &spell)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
  {
    spells.GetAt(pos) = spell;
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_LIST::GetRestTime
//
// PURPOSE: 
//
//*****************************************************************************
DWORD SPELL_LIST::GetRestTime(int index)
{
  POSITION pos;
  if ((pos = spells.FindKeyPos(index)) != NULL)
  {
    CHARACTER_SPELL &cdata = spells.GetAt(pos);
    SPELL_DATA *sdata = spellData.GetSpellData(cdata.spell);
    if (sdata != NULL)
      return (GetSpellMemorizeTime(sdata->Level));
  }
  ASSERT(FALSE);
  return 0;
}


///////////////////////////////////////////////////////////////////////
//
// Spell Book Functions
//
///////////////////////////////////////////////////////////////////////

//*****************************************************************************
//    NAME: spellBookType::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void spellBookType::Serialize(CArchive &ar, double version)
{
  spellLimits.Serialize(ar, version);
  list.Serialize(ar, version);
}

//*****************************************************************************
//    NAME: spellBookType::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void spellBookType::Serialize(CAR &ar, double version)
{
  spellLimits.Serialize(ar, version);
  list.Serialize(ar, version);
}

//*****************************************************************************
//    NAME: spellBookType::SpellsKnown
//
// PURPOSE: 
//
//*****************************************************************************
int spellBookType::SpellsKnown() 
{ 
  return list.GetCount(); 
}
//*****************************************************************************
//    NAME: spellBookType::SpellsMemorized
//
// PURPOSE: 
//
//*****************************************************************************
int spellBookType::SpellsMemorized() 
{ 
  return list.SpellsMemorized(); 
}

//*****************************************************************************
//    NAME: spellBookType::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void spellBookType::Clear()
{
  spellLimits.Clear();
  list.Clear();
  spellPrepTimeNeeded=0;
  spellPrepTimeUsed=0;
}

//*****************************************************************************
//    NAME: spellBookType::operator=
//
// PURPOSE: 
//
//*****************************************************************************
spellBookType& spellBookType::operator=(const spellBookType &book)
{
  if (this==&book)return *this;
  spellLimits = book.spellLimits;
  list = book.list;
  spellPrepTimeNeeded=book.spellPrepTimeNeeded;
  spellPrepTimeUsed=book.spellPrepTimeUsed;
  return *this;
}

//*****************************************************************************
//    NAME: spellBookType::operator==
//
// PURPOSE: 
//
//*****************************************************************************
BOOL spellBookType::operator==(const spellBookType &book)
{
  if ((spellLimits == book.spellLimits)==FALSE) return FALSE;
  if ((list == book.list)==FALSE) return FALSE;
  return TRUE;
}

//*****************************************************************************
//    NAME: spellBookType::isKnown
//
// PURPOSE: 
//
//*****************************************************************************
BOOL spellBookType::isKnown(int index) const 
{ 
  return (list.HaveSpell(index)); 
}

//*****************************************************************************
//    NAME: spellBookType::isMemorized
//
// PURPOSE: 
//
//*****************************************************************************
BOOL spellBookType::isMemorized(int index)
{ 
  // index is spell book key, not spell database key
  return list.IsMemorized(index); 
}

//*****************************************************************************
//    NAME: spellBookType::isSelected
//
// PURPOSE: 
//
//*****************************************************************************
BOOL spellBookType::isSelected(int index)
{ 
  return list.IsSelected(index); 
}

//*****************************************************************************
//    NAME: spellBookType::RemoveSpellLevel
//
// PURPOSE: 
//
//*****************************************************************************
void spellBookType::RemoveSpellLevel(int level) 
{ 
  // all spells, all classes, for this level.
  // only used by editor for cleanup.
  spellLimits.RemoveSpellLevel(level);
  list.RemoveSpellLevel(level); 
}

//*****************************************************************************
//    NAME: spellBookType::CanKnowSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL spellBookType::CanKnowSpell(WORD CasterClassFlag, WORD SpellClassFlag, WORD CastMask, int SpellLevel)
{ 
  return spellLimits.CanKnowSpell(CasterClassFlag, SpellClassFlag, CastMask, SpellLevel); 
}

// this will add a spell to the book of known spells (or remove it). Spells that
// cannot be cast yet by a character (due to char level or other limitations) can
// still be added to the book, but cannot be cast by the player until such
// limitations are reached.
//
// spells which exceed the casters capability limitations cannot be added
//
//*****************************************************************************
//    NAME: spellBookType::knowSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL spellBookType::knowSpell(WORD CasterClassFlag, WORD SpellClassFlag, int SpellDbKey, int SpellLevel, BOOL known)
{
  if (known)
  {
    // already know it?
    if (list.HaveSpell(SpellDbKey))
      return TRUE;
    
    WORD CastMask = spellData.GetSpellCastMask(SpellDbKey);

    // make sure spell can be put into spell book, even if character is not currently
    // at high enough character level to cast it
    if (spellLimits.AddSpell(CasterClassFlag, SpellClassFlag, CastMask, SpellLevel))
    {
      CHARACTER_SPELL data;
      data.spell = SpellDbKey;
      data.level = SpellLevel;
      data.memorized = 0;
      data.selected = 0;

      if (list.AddSpell(data))
      {
        //list.SortSpells();
        return TRUE;
      }
      else
        spellLimits.RemoveSpell(CasterClassFlag, SpellLevel);
    }
    return FALSE;
  }
  else // remove spell from book
  {
    // already don't have it?
    if (!list.HaveSpell(SpellDbKey))
      return TRUE;

    spellLimits.RemoveSpell(CasterClassFlag, SpellLevel);
    if (list.RemoveSpell(SpellDbKey))
    {
      list.SortSpells();
      return TRUE;
    }
    return FALSE;
  }
}

//*****************************************************************************
//    NAME: spellBookType::CalcRestTime
//
// PURPOSE: 
//
//*****************************************************************************
DWORD spellBookType::CalcRestTime(void)
{  
  spellPrepTimeNeeded = CalcSpellPrepTime();

  DWORD total=0;
  POSITION p = list.spells.GetHeadPosition();
  while (p != NULL)
  {
    int selcnt = list.spells.GetAt(p).selected;
    int memcnt = list.spells.GetAt(p).memorized;
    
    if ((selcnt > 0) && (selcnt > memcnt))
    {     
      int single = list.GetRestTime(list.spells.GetKeyAt(p));
      total += (single * (selcnt-memcnt));
    }
    list.spells.GetNext(p);
  }

  total += spellPrepTimeNeeded;

  return total;
}

//*****************************************************************************
//    NAME: spellBookType::CalcSpellPrepTime
//
// PURPOSE: 
//
//*****************************************************************************
DWORD spellBookType::CalcSpellPrepTime(void)
{
  int highlevel=-1;
  POSITION p = list.spells.GetHeadPosition();
  while (p != NULL)
  {
    int s = list.spells.GetAt(p).selected;
    int m = list.spells.GetAt(p).memorized;
    
    if ((s > 0) && (s > m))
    {     
      if (list.spells.GetAt(p).level > highlevel)
        highlevel = list.spells.GetAt(p).level;
    }
    list.spells.GetNext(p);
  }

  if (highlevel > 0)
    return (GetSpellPrepTime(highlevel));

  return 0;
}

//*****************************************************************************
//    NAME: spellBookType::UnselectIfNotMemorized
//
// PURPOSE: 
//
//*****************************************************************************
void spellBookType::UnselectIfNotMemorized(void)
{
  POSITION p = list.spells.GetHeadPosition();
  while (p != NULL)
  {
    int s = list.spells.GetAt(p).selected;
    int m = list.spells.GetAt(p).memorized;

    if ((s > 0) && (s > m))
    {
      list.spells.GetAt(p).selected = m;
    }
    list.spells.GetNext(p);
  }
}

//*****************************************************************************
//    NAME: spellBookType::UnselectAll
//
// PURPOSE: 
//
//*****************************************************************************
void spellBookType::UnselectAll(void)
{
  POSITION p = list.spells.GetHeadPosition();
  while (p != NULL)
    list.spells.GetNext(p).selected=FALSE;
}

//*****************************************************************************
//    NAME: spellBookType::selectSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL spellBookType::selectSpell(int index, int selected)
{
  if (isKnown(index))
    return (list.SetSelected(index, selected));
  else
    return FALSE; // don't know this spell
}

//*****************************************************************************
//    NAME: spellBookType::getSelectedCount
//
// PURPOSE: 
//
//*****************************************************************************
int spellBookType::getSelectedCount(int index)
{
  return list.getSelectedCount(index);
}

//*****************************************************************************
//    NAME: spellBookType::getMemorizedCount
//
// PURPOSE: 
//
//*****************************************************************************
int spellBookType::getMemorizedCount(int index)
{
  return list.getMemorizedCount(index);
}

//*****************************************************************************
//    NAME: spellBookType::getMemorizedTime
//
// PURPOSE: 
//
//*****************************************************************************
int spellBookType::getMemorizedTime(int index)
{
  return list.getMemorizedTime(index);
}

//*****************************************************************************
//    NAME: spellBookType::resetMemorizedTime
//
// PURPOSE: 
//
//*****************************************************************************
int spellBookType::resetMemorizedTime(int index)
{
  return list.resetMemorizedTime(index);
}

//*****************************************************************************
//    NAME: spellBookType::incMemorizedTime
//
// PURPOSE: 
//
//*****************************************************************************
int spellBookType::incMemorizedTime(int index, int minuteInc)
{
  return list.incMemorizedTime(index,minuteInc);
}

//*****************************************************************************
//    NAME: spellBookType::resetAllMemorizedTime
//
// PURPOSE: 
//
//*****************************************************************************
void spellBookType::resetAllMemorizedTime()
{
  POSITION p = list.spells.GetHeadPosition();
  while (p != NULL)
  {
    CHARACTER_SPELL &data = list.spells.GetAt(p);
    data.ResetMemorizedTime();
    list.spells.GetNext(p);
  }
}

//*****************************************************************************
//    NAME: spellBookType::incAllMemorizedTime
//
// PURPOSE: 
//
//*****************************************************************************
BOOL spellBookType::incAllMemorizedTime(int minuteInc)
{
  spellPrepTimeUsed += minuteInc;
  if (spellPrepTimeUsed <= spellPrepTimeNeeded)
  {
    return FALSE;
  }
  else if (spellPrepTimeNeeded > 0)
  {
    int delta = spellPrepTimeUsed - spellPrepTimeNeeded;
    if (minuteInc > delta)
      minuteInc -= delta;
  }

  spellPrepTimeNeeded = 0;
  spellPrepTimeUsed = 0;

  POSITION p = list.GetHeadPosition(); // access sorted list
  // should be at least one spell, 
  // and they should be sorted by now
  ASSERT( p != NULL );
  while (p != NULL)
  {
    CHARACTER_SPELL &data = list.GetAt(p);
    if (data.HaveUnmemorized())
    {
      // Only 1 spell can be memorized at a time.
      // There may be several unmemorized spells, so
      // inc mem time for first spell until it is memorized
      // and then add mem time to next one, and so on.
      return (data.IncMemorizedTime(minuteInc));
    }
    list.GetNext(p);
  }
  return FALSE;
}

///////////////////////////////////////////////////////////////////////
//
// Spell Data Functions
//
///////////////////////////////////////////////////////////////////////

//*****************************************************************************
//    NAME: SPELL_DATA::SPELL_DATA
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_DATA::SPELL_DATA() : 
	spell_asl("SPELL_DATA_ATTRIBUTES"),
		temp_asl("SPELL_DATA_ATTRIBUTES"),
	hCastSound(-1),hMissileSound(-1),hCoverageSound(-1),hHitSound(-1),hLingerSound(-1)
{ 
  key = 0;
  Name="";
  CastSound = "";
  MissileSound="";
  CoverageSound="";
  HitSound="";
  LingerSound="";
  CastMsg="";
  ClearSounds();
  ClearArt();
  ClassMask = MagicUserFlag;
  CastMask = MagicUserFlag;
  Level = 0;
  Casting_Time = 0;
  Casting_Time_Type = sctInitiative;
  Friendly_Fire_Ok = TRUE;
  IsCumulative=TRUE;
  Combat_Only = FALSE;
  CanBeDispelled=TRUE;
  UsableByParty=TRUE;
  CanScribeAtLevelChange=FALSE;
  AutoScribeAtLevelChange=FALSE;
  Lingers=FALSE;
  LingerOnceOnly=FALSE;
  Save_Vs = Sp;
  Save_Result = NoSave;
  specAbs.Clear();
  m_EffectedAttributes.RemoveAll();
  Duration.Clear();
  EffectDuration.Clear();
  Duration_Rate = Instantaneous;
  NumTargets.Clear();
  TargetRange.Clear();
  Targeting = Self;
  Cast_Cost = 0;
  Cast_Priority = 0;
  SpellBeginSource="";
  SpellBeginBinary="";
  SpellEndSource="";
  SpellEndBinary="";
}
//*****************************************************************************
//    NAME: SPELL_DATA::SetSpellAnimDefaults
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::SetSpellAnimDefaults(PIC_DATA &art, const char *file)
{
  if (strcmp(art.filename, "NA") == 0)
  {
    art.Clear();
    return;
  }

  if (art.filename.CompareNoCase("DEFAULT") == 0)
  {
#ifdef UAFEDITOR
    art.filename.Format("%s%s",EditorArt, file);
#else
    art.filename = file;
#endif
    art.picType = SpriteDib;
    art.NumFrames = 2;
    art.FrameWidth = COMBAT_TILE_WIDTH;
    art.FrameHeight = COMBAT_TILE_HEIGHT;
    art.style = PIC_DATA::AS_None;
    art.timeDelay = 210;
    art.MaxLoops = 5;
    art.flags = 6;
  }
}

//*****************************************************************************
//    NAME: SPELL_DATA::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::Clear(BOOL ctor)
{
  key = 0;
  Name="";
  CastSound = "";
  MissileSound="";
  CoverageSound="";
  HitSound="";
  LingerSound="";
  CastMsg="";
  ClearSounds();
  ClearArt();
  ClassMask = MagicUserFlag;
  CastMask = MagicUserFlag;
  Level = 0;
  Casting_Time = 0;
  Casting_Time_Type = sctInitiative;
  IsCumulative=TRUE;
  Friendly_Fire_Ok = TRUE;
  Combat_Only = FALSE;
  CanBeDispelled=TRUE;
  UsableByParty=TRUE;
  CanScribeAtLevelChange=FALSE;
  AutoScribeAtLevelChange=FALSE;
  Lingers=FALSE;
  LingerOnceOnly=FALSE;
  Save_Vs = Sp;
  Save_Result = NoSave;
  specAbs.Clear();
  m_EffectedAttributes.RemoveAll();
  Duration.Clear();
  EffectDuration.Clear();
  Duration_Rate = Instantaneous;
  NumTargets.Clear();
  TargetRange.Clear();
  Targeting = Self;
  Cast_Cost = 0;
  Cast_Priority = 0;
  spell_asl.Clear();
  SpellBeginSource="";
  SpellBeginBinary="";
  SpellEndSource="";
  SpellEndBinary="";
}

//*****************************************************************************
//    NAME: &SPELL_DATA::operator=
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_DATA &SPELL_DATA::operator=(const SPELL_DATA &src)
{
  if (&src == this)
    return *this;
  hCastSound = src.hCastSound;
  hMissileSound=src.hMissileSound;
  hCoverageSound=src.hCoverageSound;
  hHitSound=src.hHitSound;
  hLingerSound=src.hLingerSound;
  key = src.key;
  Name=src.Name;
  CastSound = src.CastSound;
  MissileSound=src.MissileSound;
  CoverageSound=src.CoverageSound;
  HitSound=src.HitSound;
  LingerSound=src.LingerSound;
  CastMsg=src.CastMsg;
  ClassMask = src.ClassMask;
  CastMask = src.CastMask;
  Level = src.Level;
  Casting_Time = src.Casting_Time;
  Casting_Time_Type = src.Casting_Time_Type;
  Friendly_Fire_Ok = src.Friendly_Fire_Ok;
  IsCumulative = src.IsCumulative;
  Combat_Only = src.Combat_Only;
  CanBeDispelled=src.CanBeDispelled;
  UsableByParty=src.UsableByParty;
  CanScribeAtLevelChange=src.CanScribeAtLevelChange;
  AutoScribeAtLevelChange=src.AutoScribeAtLevelChange;
  Lingers=src.Lingers;
  LingerOnceOnly=src.LingerOnceOnly;
  Save_Vs = src.Save_Vs;
  Save_Result = src.Save_Result;
  specAbs = src.specAbs;
  Targeting = src.Targeting;
  Cast_Cost = src.Cast_Cost;
  Cast_Priority = src.Cast_Priority;

  CastArt = src.CastArt;
  MissileArt = src.MissileArt;
  TargCoverageArt = src.TargCoverageArt;
  TargHitArt = src.TargHitArt;
  TargLingerArt = src.TargLingerArt;
  Duration = src.Duration;
  EffectDuration = src.EffectDuration;
  Duration_Rate = src.Duration_Rate;
  NumTargets = src.NumTargets;
  TargetRange = src.TargetRange;

  spell_asl = src.spell_asl;
  temp_asl = src.temp_asl;
  m_EffectedAttributes.RemoveAll();
  POSITION pos = src.m_EffectedAttributes.GetHeadPosition();
  while (pos != NULL)
  {
    SPELL_EFFECTS_DATA tmp=src.m_EffectedAttributes.GetNext(pos);
    m_EffectedAttributes.AddTail(tmp);
  }
  SpellBeginSource=src.SpellBeginSource;
  SpellBeginBinary=src.SpellBeginBinary;
  SpellEndSource=src.SpellEndSource;
  SpellEndBinary=src.SpellEndBinary;
  return *this;
}

//*****************************************************************************
//    NAME: SPELL_DATA::operator==
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_DATA::operator==(const SPELL_DATA &src) const
{
  if (&src == this) return true;
  if (hCastSound             != src.hCastSound            ) return false;
  if (hMissileSound          != src.hMissileSound         ) return false;
  if (hCoverageSound         != src.hCoverageSound        ) return false;
  if (hHitSound              != src.hHitSound             ) return false;
  if (hLingerSound           != src.hLingerSound          ) return false;
  if (key                    != src.key                   ) return false;
  if (Name                   != src.Name                  ) return false;
  if (CastSound              != src.CastSound             ) return false;
  if (MissileSound           != src.MissileSound          ) return false;
  if (CoverageSound          != src.CoverageSound         ) return false;
  if (HitSound               != src.HitSound              ) return false;
  if (LingerSound            != src.LingerSound           ) return false;
  if (CastMsg                != src.CastMsg               ) return false;
  if (ClassMask              != src.ClassMask             ) return false;
  if (CastMask               != src.CastMask              ) return false;
  if (Level                  != src.Level                 ) return false;
  if (Casting_Time           != src.Casting_Time          ) return false;
  if (Casting_Time_Type      != src.Casting_Time_Type     ) return false;
  if (Friendly_Fire_Ok       != src.Friendly_Fire_Ok      ) return false;
  if (IsCumulative           != src.IsCumulative          ) return false;
  if (Combat_Only            != src.Combat_Only           ) return false;
  if (UsableByParty          != src.UsableByParty         ) return false;
  if (CanBeDispelled         != src.CanBeDispelled        ) return false;
  if (CanScribeAtLevelChange != src.CanScribeAtLevelChange) return false;
  if (AutoScribeAtLevelChange!= src.AutoScribeAtLevelChange)return false;
  if (Lingers                != src.Lingers               ) return false;
  if (LingerOnceOnly         != src.LingerOnceOnly        ) return false;
  if (Save_Vs                != src.Save_Vs               ) return false;
  if (Save_Result            != src.Save_Result           ) return false;
  if (!(specAbs              == src.specAbs)              ) return false;
  if (Targeting              != src.Targeting             ) return false;  
  if (Cast_Cost              != src.Cast_Cost             ) return false;
  if (Cast_Priority          != src.Cast_Priority         ) return false;

  if (!(CastArt              == src.CastArt)              ) return false;
  if (!(MissileArt           == src.MissileArt)           ) return false;
  if (!(TargCoverageArt      == src.TargCoverageArt)      ) return false;
  if (!(TargHitArt           == src.TargHitArt)           ) return false;
  if (!(TargLingerArt        == src.TargLingerArt)        ) return false;
  if (!(Duration             == src.Duration)             ) return false;
  if (!(EffectDuration       == src.EffectDuration)       ) return false;
  if (Duration_Rate          != src.Duration_Rate         ) return false;
  if (!(NumTargets           == src.NumTargets)           ) return false;
  if (!(TargetRange          == src.TargetRange)          ) return false;
  if (SpellBeginSource.CompareNoCase(src.SpellBeginSource)!=0) return false;
  if (SpellEndSource.CompareNoCase(src.SpellEndSource)!=0) return false;
  if (!(spell_asl            == src.spell_asl)            ) return false;
  if (!(temp_asl             == src.temp_asl)             ) return false;
  if (m_EffectedAttributes.GetCount() != src.m_EffectedAttributes.GetCount()) return false;
  POSITION pos = src.m_EffectedAttributes.GetHeadPosition();
  while (pos != NULL)
    if (!HaveEffectAttr(src.m_EffectedAttributes.GetNext(pos))) return false;
  return true;
}

//*****************************************************************************
//    NAME: SPELL_DATA::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);
  long temp;
  static long Target_Dice;
  static long Target_Dice_Nbr;
  static long Target_Bonus_Level;
  static long Target_Bonus;
  static long Target_Max;
  static long Duration_Dice;
  static long Duration_Dice_Nbr;
  static long Duration_Bonus_Level;
  static long Duration_Bonus;
  static long Duration_Max;
  static long Range_Dice;
  static long Range_Dice_Nbr;
  static long Range_Bonus_Level;
  static long Range_Bonus;
  static long Range_Max;
  static long Attack_Dice;
  static long Attack_Dice_Nbr;
  static long Attack_Bonus;
  static long Attack_Bonus_Level;
  static long Attack_Max;
  static long Damage_Dice;
  static long Damage_Dice_Nbr;
  static long Damage_Bonus;
  static long Damage_Bonus_Level;
  static long Damage_Max;
  static long Protection_Dice;
  static long Protection_Dice_Nbr;
  static long Protection_Bonus;
  static long Protection_Bonus_Level;
  static long Protection_Max;
  static long Heal_Dice;
  static long Heal_Dice_Nbr;
  static long Heal_Bonus;
  static long Heal_Bonus_Level;
  static long Heal_Max;

  if (ar.IsStoring())
  {
    ASSERT(FALSE); // should not be saved in this format anymore

    if (ver <= _VERSION_0830_)
      Name.Replace('/','|');

    ar << key;
    AS(ar,Name);
    AS(ar,CastSound);
    ar << ClassMask;
    ar << CastMask;
    ar << Level;
    ar << Casting_Time;
    temp = Casting_Time_Type;
    ar << temp;
    ar << Friendly_Fire_Ok;
    ar << IsCumulative;
    ar << Combat_Only;
    ar << CanBeDispelled;
    ar << UsableByParty;
    ar << CanScribeAtLevelChange;
    ar << AutoScribeAtLevelChange;
    ar << Lingers;
    ar << LingerOnceOnly;

    temp = Save_Vs;
    ar << temp;
    temp = Save_Result;
    ar << temp;
    temp = Targeting;
    ar << temp;

    temp = Duration_Rate;
    ar << temp;
    ar << Cast_Cost;
    ar << Cast_Priority;

    AS(ar, MissileSound);
    AS(ar, CoverageSound);
    AS(ar, HitSound);
    AS(ar, LingerSound);
    AS(ar, CastMsg);

    // this data is stored in attribute map
    // MissileArt.Serialize(ar);
    // TargCoverageArt.Serialize(ar);
    // TargHitArt.Serialize(ar);
    // TargLingerArt.Serialize(ar);

    AS(ar,SpellBeginSource);
    AS(ar,SpellBeginBinary);
    AS(ar,SpellEndSource);
    AS(ar,SpellEndBinary);    
  }
  else
  {
    ar >> key;
    DAS(ar,Name);

#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Fireball|Green" will be
    // read as simply "FireBall".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    if (Name.Find('|') >= 0) Name = Name.Left(Name.Find('|')); // Remove qualifier.
#endif

    DAS(ar,CastSound);
    ar >> ClassMask;
    
    if (ver >= _VERSION_0910_)
      ar >> CastMask;
    else
      CastMask = 0;

    // this new flag is certainly not going
    // to be correct for older designs
    //
    // try to fix it...
    if (ClassMask == MagicUserFlag)
    {
      // only MU can cast MU spells
      CastMask = MagicUserFlag;
    }
    else // ClericFlag
    {
      // MU and Cleric are mutually exclusive
      //
      // remove MU flag
      CastMask &= ~MagicUserFlag;
      
      // if that was the only bit set,
      // then at least set the cleric flag
      if (CastMask == 0)
        CastMask = ClericFlag;
    }

    ar >> Level;
    ar >> Casting_Time;
    if (ver >= _VERSION_0662_)
    {
      ar >> temp;
      Casting_Time_Type = (spellCastingTimeType)temp;
    }
    ar >> Friendly_Fire_Ok;
    if (ver >= _VERSION_06991_)
      ar >> IsCumulative;

    ar >> Combat_Only;

    if (ver >= _VERSION_0909_)
      ar >> CanBeDispelled;

    if (ver >= _VERSION_0670_)
      ar >> UsableByParty;

    if (ver >= _VERSION_0692_)
      ar >> CanScribeAtLevelChange;

    if (ver >= _VERSION_0910_)
      ar >> AutoScribeAtLevelChange;

    // for older design versions
    if ((CanScribeAtLevelChange) && (ver < _VERSION_0910_))
      AutoScribeAtLevelChange = TRUE;
    
    if (ver >= _VERSION_0906_)
    {
      ar >> Lingers;
      ar >> LingerOnceOnly;
    }
    
    ar >> temp;
    Save_Vs = (spellSaveVsType)temp;

    ar >> temp;
    Save_Result = (spellSaveEffectType)temp;

    ar >> temp;
    Targeting = (spellTargetingType)temp;

    if (ver <= _VERSION_06992_)
    {
      ar >> Target_Dice;
      ar >> Target_Dice_Nbr;
      ar >> Target_Bonus_Level;
      ar >> Target_Bonus;
      ar >> Target_Max;
    }
  
    ar >> temp;
    Duration_Rate = (spellDurationType)temp;

    if (ver <= _VERSION_06992_)
    {
      ar >> Duration_Dice;
      ar >> Duration_Dice_Nbr;
      ar >> Duration_Bonus_Level;
      ar >> Duration_Bonus;
      ar >> Duration_Max;

      ar >> Range_Dice;
      ar >> Range_Dice_Nbr;
      ar >> Range_Bonus_Level;
      ar >> Range_Bonus;
      ar >> Range_Max;

      ar >> Attack_Dice;
      ar >> Attack_Dice_Nbr;
      ar >> Attack_Bonus;
      ar >> Attack_Bonus_Level;
      ar >> Attack_Max;

      ar >> Damage_Dice;
      ar >> Damage_Dice_Nbr;
      ar >> Damage_Bonus;
      ar >> Damage_Bonus_Level;
      ar >> Damage_Max;

      ar >> Protection_Dice;
      ar >> Protection_Dice_Nbr;
      ar >> Protection_Bonus;
      ar >> Protection_Bonus_Level;
      ar >> Protection_Max;

      ar >> Heal_Dice;
      ar >> Heal_Dice_Nbr;
      ar >> Heal_Bonus;
      ar >> Heal_Bonus_Level;
      ar >> Heal_Max;
    }

    ar >> Cast_Cost;
    ar >> Cast_Priority;

    if (ver >= _VERSION_0840_)
    {
      DAS(ar, MissileSound);
      DAS(ar, CoverageSound);
      DAS(ar, HitSound);
      DAS(ar, LingerSound);
    }
    if (ver >= _VERSION_0841_)
    {
      DAS(ar, CastMsg);
    }
    // this data is stored in attribute map
    // MissileArt.Serialize(ar);
    // TargCoverageArt.Serialize(ar);
    // TargHitArt.Serialize(ar);
    // TargLingerArt.Serialize(ar);

    if (ver >= _VERSION_0904_)
    {
      DAS(ar,SpellBeginSource);
      DAS(ar,SpellBeginBinary);
      DAS(ar,SpellEndSource);
      DAS(ar,SpellEndBinary);
    }
    
  }

  specAbs.Serialize(ar, ver);
  spell_asl.Serialize(ar);
}

//*****************************************************************************
//    NAME: SPELL_DATA::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::Serialize(CAR &ar, double ver)
{
  long temp;
  static long Target_Dice;
  static long Target_Dice_Nbr;
  static long Target_Bonus_Level;
  static long Target_Bonus;
  static long Target_Max;
  static long Duration_Dice;
  static long Duration_Dice_Nbr;
  static long Duration_Bonus_Level;
  static long Duration_Bonus;
  static long Duration_Max;
  static long Range_Dice;
  static long Range_Dice_Nbr;
  static long Range_Bonus_Level;
  static long Range_Bonus;
  static long Range_Max;
  static long Attack_Dice;
  static long Attack_Dice_Nbr;
  static long Attack_Bonus;
  static long Attack_Bonus_Level;
  static long Attack_Max;
  static long Damage_Dice;
  static long Damage_Dice_Nbr;
  static long Damage_Bonus;
  static long Damage_Bonus_Level;
  static long Damage_Max;
  static long Protection_Dice;
  static long Protection_Dice_Nbr;
  static long Protection_Bonus;
  static long Protection_Bonus_Level;
  static long Protection_Max;
  static long Heal_Dice;
  static long Heal_Dice_Nbr;
  static long Heal_Bonus;
  static long Heal_Bonus_Level;
  static long Heal_Max;


  if (ar.IsStoring())
  {
    if (ver <= _VERSION_0830_)
      Name.Replace('/','|');

    ar << key;
    AS(ar,Name);
    AS(ar,CastSound);
    ar << ClassMask;
    ar << CastMask;
    ar << Level;
    ar << Casting_Time;
    temp = Casting_Time_Type;
    ar << temp;
    ar << Friendly_Fire_Ok;
    ar << IsCumulative;
    ar << Combat_Only;
    ar << CanBeDispelled;
    ar << UsableByParty;
    ar << CanScribeAtLevelChange;
    ar << AutoScribeAtLevelChange;
    ar << Lingers;
    ar << LingerOnceOnly;

    temp = Save_Vs;
    ar << temp;
    temp = Save_Result;
    ar << temp;
    temp = Targeting;
    ar << temp;

    temp = Duration_Rate;
    ar << temp;

    ar << Cast_Cost;
    ar << Cast_Priority;

    Duration.Serialize(ar);
    NumTargets.Serialize(ar);
    TargetRange.Serialize(ar);
    ar << m_EffectedAttributes.GetCount();
    POSITION pos = m_EffectedAttributes.GetHeadPosition();
    while (pos != NULL)
      m_EffectedAttributes.GetNext(pos).Serialize(ar, ver);

    if (ver >= _VERSION_0840_)
      CastArt.Serialize(ar,ver);
    MissileArt.Serialize(ar,ver);
    TargCoverageArt.Serialize(ar,ver);
    TargHitArt.Serialize(ar,ver);
    TargLingerArt.Serialize(ar,ver);
    AS(ar, MissileSound);
    AS(ar, CoverageSound);
    AS(ar, HitSound);
    AS(ar, LingerSound);
    AS(ar, CastMsg);

    AS(ar,SpellBeginSource);
    AS(ar,SpellBeginBinary);
    AS(ar,SpellEndSource);
    AS(ar,SpellEndBinary); 
    EffectDuration.Serialize(ar);
  }
  else
  {
    ar >> key;
    DAS(ar,Name);

#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Fireball|Green" will be
    // read as simply "FireBall".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    if (Name.Find('|') >= 0) Name = Name.Left(Name.Find('|')); // Remove qualifier.
#endif
    DAS(ar,CastSound);
    ar >> ClassMask;
    
    if (ver >= _VERSION_0910_)
      ar >> CastMask;
    else
      CastMask = AllClasses;

    // this new flag is certainly not going
    // to be correct for older designs
    //
    // try to fix it...
    if (ClassMask == MagicUserFlag)
    {
      // only MU can cast MU spells
      CastMask = MagicUserFlag;
    }
    else // ClericFlag
    {
      // MU and Cleric are mutually exclusive
      //
      // remove MU flag
      CastMask &= ~MagicUserFlag;
      
      // if that was the only bit set,
      // then at least set the cleric flag
      if (CastMask == 0)
        CastMask = ClericFlag;
    }
    
    ar >> Level;
    ar >> Casting_Time;
    if (ver >= _VERSION_0662_)
    {
      ar >> temp;
      Casting_Time_Type = (spellCastingTimeType)temp;
    }
    ar >> Friendly_Fire_Ok;
    if (ver >= _VERSION_06991_)
      ar >> IsCumulative;

    ar >> Combat_Only;

    if (ver >= _VERSION_0909_)
      ar >> CanBeDispelled;
    
    if (ver >= _VERSION_0670_)
      ar >> UsableByParty;

    if (ver >= _VERSION_0692_)
      ar >> CanScribeAtLevelChange;

    if (ver >= _VERSION_0910_)
      ar >> AutoScribeAtLevelChange;

    // for older design versions
    if ((CanScribeAtLevelChange) && (ver < _VERSION_0910_))
      AutoScribeAtLevelChange = TRUE;

    if (ver >= _VERSION_0906_)
    {
      ar >> Lingers;
      ar >> LingerOnceOnly;
    }

    ar >> temp;
    Save_Vs = (spellSaveVsType)temp;

    ar >> temp;
    Save_Result = (spellSaveEffectType)temp;

    ar >> temp;
    Targeting = (spellTargetingType)temp;

    if (ver <= _VERSION_06992_)
    {
      ar >> Target_Dice;
      ar >> Target_Dice_Nbr;
      ar >> Target_Bonus_Level;
      ar >> Target_Bonus;
      ar >> Target_Max;
    }
  
    ar >> temp;
    Duration_Rate = (spellDurationType)temp;

    if (ver <= _VERSION_06992_)
    {
      ar >> Duration_Dice;
      ar >> Duration_Dice_Nbr;
      ar >> Duration_Bonus_Level;
      ar >> Duration_Bonus;
      ar >> Duration_Max;

      ar >> Range_Dice;
      ar >> Range_Dice_Nbr;
      ar >> Range_Bonus_Level;
      ar >> Range_Bonus;
      ar >> Range_Max;

      ar >> Attack_Dice;
      ar >> Attack_Dice_Nbr;
      ar >> Attack_Bonus;
      ar >> Attack_Bonus_Level;
      ar >> Attack_Max;

      ar >> Damage_Dice;
      ar >> Damage_Dice_Nbr;
      ar >> Damage_Bonus;
      ar >> Damage_Bonus_Level;
      ar >> Damage_Max;

      ar >> Protection_Dice;
      ar >> Protection_Dice_Nbr;
      ar >> Protection_Bonus;
      ar >> Protection_Bonus_Level;
      ar >> Protection_Max;

      ar >> Heal_Dice;
      ar >> Heal_Dice_Nbr;
      ar >> Heal_Bonus;
      ar >> Heal_Bonus_Level;
      ar >> Heal_Max;
    }

    ar >> Cast_Cost;
    ar >> Cast_Priority;

    if (ver >= _VERSION_0670_)
    {
      Duration.Serialize(ar);
      NumTargets.Serialize(ar);
      TargetRange.Serialize(ar);

      int count;
      ar >> count;
      SPELL_EFFECTS_DATA tmp;
      m_EffectedAttributes.RemoveAll();
      for (int i=0; i<count; i++)
      {
        tmp.Serialize(ar, ver);
        AddEffect(tmp);
      }
    }
    else
    {
      CString diceformat;
      diceformat.Format("%ud%u+%u+%u*level min 0 max %u",
                        Duration_Dice_Nbr,
                        Duration_Dice,
                        Duration_Bonus,
                        Duration_Bonus_Level,
                        Duration_Max);
      Duration.DecodeText(diceformat);

      diceformat.Format("%ud%u+%u+%u*level min 0 max %u",
                        Target_Dice_Nbr,
                        Target_Dice,
                        Target_Bonus,
                        Target_Bonus_Level,
                        Target_Max);
      NumTargets.DecodeText(diceformat);

      diceformat.Format("%ud%u+%u+%u*level min 0 max %u",
                        Range_Dice_Nbr,
                        Range_Dice,
                        Range_Bonus,
                        Range_Bonus_Level,
                        Range_Max);
      TargetRange.DecodeText(diceformat);

      if ((Attack_Dice_Nbr!=0))
      {
        diceformat.Format("%ud%u+%u+%u*level min 0 max %u",
                          Attack_Dice_Nbr,
                          Attack_Dice,
                          Attack_Bonus,
                          Attack_Bonus_Level,
                          Attack_Max);
        SPELL_EFFECTS_DATA seData;
        seData.affectedAttr = "$CHAR_THACO";
        seData.changeData.DecodeText(diceformat);
        seData.flags = SPELL_EFFECTS_DATA::EFFECT_TARGET;
        AddEffect(seData);
      }

      if ((Damage_Dice_Nbr!=0))
      {
        diceformat.Format("%ud%u+%u+%u*level min 0 max %u",
                          Damage_Dice_Nbr,
                          Damage_Dice,
                          Damage_Bonus,
                          Damage_Bonus_Level,
                          Damage_Max);
        SPELL_EFFECTS_DATA seData;
        seData.affectedAttr = "$CHAR_DAMAGEBONUS";
        seData.changeData.DecodeText(diceformat);
        seData.flags = SPELL_EFFECTS_DATA::EFFECT_TARGET;
        AddEffect(seData);
      }

      if ((Protection_Dice_Nbr!=0))
      {
        diceformat.Format("%ud%u+%u+%u*level min 0 max %u",
                          Protection_Dice_Nbr,
                          Protection_Dice,
                          Protection_Bonus,
                          Protection_Bonus_Level,
                          Protection_Max);
        SPELL_EFFECTS_DATA seData;
        seData.affectedAttr = "$CHAR_AC";
        seData.changeData.DecodeText(diceformat);
        seData.flags = SPELL_EFFECTS_DATA::EFFECT_TARGET;
        AddEffect(seData);
      }

      if ((Heal_Dice_Nbr!=0))
      {
        diceformat.Format("%ud%u+%u+%u*level min 0 max %u",
                          Heal_Dice_Nbr,
                          Heal_Dice,
                          Heal_Bonus,
                          Heal_Bonus_Level,
                          Heal_Max);
        SPELL_EFFECTS_DATA seData;
        seData.affectedAttr = "$CHAR_HITPOINTS";
        seData.changeData.DecodeText(diceformat);
        seData.flags = SPELL_EFFECTS_DATA::EFFECT_TARGET;
        AddEffect(seData);
      }
    }

    if (ver >= _VERSION_0840_)
      CastArt.Serialize(ar,ver);
    MissileArt.Serialize(ar,ver);
    TargCoverageArt.Serialize(ar,ver);
    TargHitArt.Serialize(ar,ver);
    TargLingerArt.Serialize(ar,ver);

    if (ver >= _VERSION_0840_)
    {
      DAS(ar, MissileSound);
      DAS(ar, CoverageSound);
      DAS(ar, HitSound);
      DAS(ar, LingerSound);
    }
    if (ver >= _VERSION_0841_)
    {
      DAS(ar, CastMsg);
    }

    if (ver >= _VERSION_0904_)
    {
      DAS(ar,SpellBeginSource);
      DAS(ar,SpellBeginBinary);
      DAS(ar,SpellEndSource);
      DAS(ar,SpellEndBinary);      
    }

    if (ver >= _VERSION_0906_)
    {
      EffectDuration.Serialize(ar);
    }
  }

  specAbs.Serialize(ar,ver);
  spell_asl.Serialize(ar);
}

//*****************************************************************************
//    NAME: SPELL_DATA::PreSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PreSerialize(BOOL IsStoring) 
{
  if (IsStoring)
  {
    StripFilenamePath(CastSound);
    StripFilenamePath(MissileSound);
    StripFilenamePath(CoverageSound);
    StripFilenamePath(HitSound);
    StripFilenamePath(LingerSound);

    CastArt.PreSerialize(IsStoring);
    MissileArt.PreSerialize(IsStoring);
    TargCoverageArt.PreSerialize(IsStoring);
    TargHitArt.PreSerialize(IsStoring);
    TargLingerArt.PreSerialize(IsStoring);

    // move fields that need to be serialized
    // as attributes into the attribute map  
    StorePicDataAsASL(spell_asl, CastArt, "CastArt");
    StorePicDataAsASL(spell_asl, MissileArt, "MissArt");
    StorePicDataAsASL(spell_asl, TargCoverageArt, "TargCovArt");
    StorePicDataAsASL(spell_asl, TargHitArt, "TargHitArt");
    StorePicDataAsASL(spell_asl, TargLingerArt, "TargLingerArt");
  }
}

//*****************************************************************************
//    NAME: SPELL_DATA::PostSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PostSerialize(BOOL IsStoring) 
{
  if (!IsStoring)
  {
    // retrieve fields that are serialized as
    // attributes from the attribute map
    RetrievePicDataFromASL(spell_asl, CastArt, "CastArt");
    RetrievePicDataFromASL(spell_asl, MissileArt, "MissArt");
    RetrievePicDataFromASL(spell_asl, TargCoverageArt, "TargCovArt");
    RetrievePicDataFromASL(spell_asl, TargHitArt, "TargHitArt");
    RetrievePicDataFromASL(spell_asl, TargLingerArt, "TargLingerArt");
  }

  AddDesignFolderToPath(CastSound);
  AddDesignFolderToPath(MissileSound);
  AddDesignFolderToPath(CoverageSound);
  AddDesignFolderToPath(HitSound);
  AddDesignFolderToPath(LingerSound);

  CastArt.PostSerialize(IsStoring);
  MissileArt.PostSerialize(IsStoring);
  TargCoverageArt.PostSerialize(IsStoring);
  TargHitArt.PostSerialize(IsStoring);
  TargLingerArt.PostSerialize(IsStoring);
}


//*****************************************************************************
//    NAME: SPELL_DATA::PreCARSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PreCARSerialize(BOOL IsStoring) 
{
  if (IsStoring)
  {
    StripFilenamePath(CastSound);
    StripFilenamePath(MissileSound);
    StripFilenamePath(CoverageSound);
    StripFilenamePath(HitSound);
    StripFilenamePath(LingerSound);

    CastArt.PreSerialize(IsStoring);
    MissileArt.PreSerialize(IsStoring);
    TargCoverageArt.PreSerialize(IsStoring);
    TargHitArt.PreSerialize(IsStoring);
    TargLingerArt.PreSerialize(IsStoring);
  }
}

//*****************************************************************************
//    NAME: SPELL_DATA::PostCARSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PostCARSerialize(BOOL IsStoring) 
{
  AddDesignFolderToPath(CastSound);
  AddDesignFolderToPath(MissileSound);
  AddDesignFolderToPath(CoverageSound);
  AddDesignFolderToPath(HitSound);
  AddDesignFolderToPath(LingerSound);

  CastArt.PostSerialize(IsStoring);
  MissileArt.PostSerialize(IsStoring);
  TargCoverageArt.PostSerialize(IsStoring);
  TargHitArt.PostSerialize(IsStoring);
  TargLingerArt.PostSerialize(IsStoring);
}

//*****************************************************************************
//    NAME: SPELL_DATA::Save
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::Save(CArchive& ar)
{
	spell_asl.Save(ar);
}

//*****************************************************************************
//    NAME: SPELL_DATA::PrepareForRestore
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PrepareForRestore(void) 
{
	// Just in case no Restore comes along.
	temp_asl.Clear();
}

//*****************************************************************************
//    NAME: SPELL_DATA::Restore
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::Restore(CArchive& ar)
{
	temp_asl.Serialize(ar);
}

//*****************************************************************************
//    NAME: SPELL_DATA::CommitRestore
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::CommitRestore(void) 
{
	spell_asl.CommitRestore(temp_asl);
	temp_asl.Clear();
}

//*****************************************************************************
//    NAME: SPELL_DATA::PlayCast
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PlayCast()
{
  if (!PlaySoundEffects) return;
  if (!ValidSound(hCastSound))
    hCastSound = LoadSound(CastSound);
  if (!PlaySound(hCastSound,FALSE))
    hCastSound=-1;
}
//*****************************************************************************
//    NAME: SPELL_DATA::PlayMissile
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PlayMissile()
{
  if (!PlaySoundEffects) return;
  if (!ValidSound(hMissileSound))
    hMissileSound = LoadSound(MissileSound);
  if (!PlaySound(hMissileSound,FALSE))
    hMissileSound=-1;
}
//*****************************************************************************
//    NAME: SPELL_DATA::PlayCoverage
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PlayCoverage()
{
  if (!PlaySoundEffects) return;
  if (!ValidSound(hCoverageSound))
    hCoverageSound = LoadSound(CoverageSound);
  if (!PlaySound(hCoverageSound,FALSE))
    hCoverageSound=-1;
}
//*****************************************************************************
//    NAME: SPELL_DATA::PlayHit
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PlayHit()
{
  if (!PlaySoundEffects) return;
  if (!ValidSound(hHitSound))
    hHitSound = LoadSound(HitSound);
  if (!PlaySound(hHitSound,FALSE))
    hHitSound=-1;
}
//*****************************************************************************
//    NAME: SPELL_DATA::PlayLinger
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::PlayLinger()
{
  if (!PlaySoundEffects) return;
  if (!ValidSound(hLingerSound))
    hLingerSound = LoadSound(LingerSound);
  if (!PlaySound(hLingerSound,FALSE))
    hLingerSound=-1;
}
//*****************************************************************************
//    NAME: SPELL_DATA::ClearSounds
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::ClearSounds()
{
  if (hCastSound >= 0) ClearSound(hCastSound);
  if (hMissileSound >= 0) ClearSound(hMissileSound);
  if (hCoverageSound >= 0) ClearSound(hCoverageSound);
  if (hHitSound >= 0) ClearSound(hHitSound);
  if (hLingerSound >= 0) ClearSound(hLingerSound);
  hCastSound = -1;
  hMissileSound=-1;
  hCoverageSound=-1;
  hHitSound=-1;
  hLingerSound=-1;
}

//*****************************************************************************
//    NAME: SPELL_DATA::LoadArt
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::LoadArt()
{
  CastArt.LoadPicSurfaces("spl_");
  MissileArt.LoadPicSurfaces("spl_");
  TargCoverageArt.LoadPicSurfaces("spl_");
  TargHitArt.LoadPicSurfaces("spl_");
  TargLingerArt.LoadPicSurfaces("spl_");
}
//*****************************************************************************
//    NAME: SPELL_DATA::ClearArt
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::ClearArt()
{
  CastArt.Clear();
  MissileArt.Clear();
  TargCoverageArt.Clear();
  TargHitArt.Clear();
  TargLingerArt.Clear();
}
//*****************************************************************************
//    NAME: SPELL_DATA::FreeArt
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::FreeArt()
{
  CastArt.FreePic();
  MissileArt.FreePic();
  TargCoverageArt.FreePic();
  TargHitArt.FreePic();
  TargLingerArt.FreePic();
}

//*****************************************************************************
//    NAME: SPELL_DATA::AddEffect
//
// PURPOSE: 
//
//*****************************************************************************
POSITION SPELL_DATA::AddEffect(SPELL_EFFECTS_DATA &src)
{
  if (HaveEffectAttr(src)) return NULL;  
  CompileScript(src);
  POSITION pos = m_EffectedAttributes.AddTail(src);  
  return pos;
}

//*****************************************************************************
//    NAME: SPELL_DATA::DeleteEffect
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::DeleteEffect(SPELL_EFFECTS_DATA &src)
{
  POSITION pos = m_EffectedAttributes.GetHeadPosition();
  while (pos != NULL)
  {
    if (m_EffectedAttributes.GetAt(pos) == src) 
    {
      m_EffectedAttributes.RemoveAt(pos);
      return;
    }
    m_EffectedAttributes.GetNext(pos);
  }
}

//*****************************************************************************
//    NAME: SPELL_DATA::HaveEffectAttr
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_DATA::HaveEffectAttr(const SPELL_EFFECTS_DATA &src) const
{
  POSITION pos = m_EffectedAttributes.GetHeadPosition();
  while (pos != NULL)
  {
    if (m_EffectedAttributes.GetAt(pos) == src) 
      return true;
    m_EffectedAttributes.GetNext(pos);
  }
  return false;
}

//*****************************************************************************
//    NAME: SPELL_DATA::CanEnableAffectAttr
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_DATA::CanEnableAffectAttr(const CString &attr) const
{
  POSITION pos = m_EffectedAttributes.GetHeadPosition();
  while (pos != NULL)
  {
    if (m_EffectedAttributes.GetAt(pos).affectedAttr.CompareNoCase(attr)==0) 
    {
      if (m_EffectedAttributes.GetAt(pos).ApplyChange(0.0) > 0.0) 
        return true;
    }
    m_EffectedAttributes.GetNext(pos);
  }
  return false;
}

//*****************************************************************************
//    NAME: SPELL_DATA::CompileScript
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_DATA::CompileScript(SPELL_EFFECTS_DATA &src)
{
  CString name(""); 

  /*
  POSITION pos = m_EffectedAttributes.GetHeadPosition();
  int count=0;
  while (pos != NULL)
  {
    if (m_EffectedAttributes.GetAt(pos) == src) 
    {
      // all gpdl function names must be unique, so 
      // we create a unique name by using the spell name,
      // spell effect index, and later on, the spell
      // effect's affected attribute name and current
      // clock tick count are appended.
    }
    count++;
    m_EffectedAttributes.GetNext(pos);
  }
  */
  
  CString sname(Name); // start with spell name
  sname.Remove(' '); // but remove spaces from name
  name.Format("%s_", sname);
  return (src.CompileScripts(name));  
}

//*****************************************************************************
//    NAME: SPELL_DATA::CompileScripts
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_DATA::CompileScripts()
{

  CString sname(Name); // start with spell name
  sname.Remove(' '); // but remove spaces from name
  sname.Replace('|', '_');
  //sname += "_";

  CString func("");  
  
  bool success = true;
  SpellBeginBinary="";  
  SpellEndBinary="";
  
#ifdef UAFEDITOR 
  const char *allowedEntry[2];
  
  GPDLCOMP gpdlComp;
  
  if (!SpellBeginSource.IsEmpty()) 
  {
    func.Format("%s_b_%u", sname, GetTickCount());
    CString ascript;
    CString scriptbegin;
    CString scriptend;
    
    scriptbegin.Format("$PUBLIC $FUNC %s() { ", func);
    scriptend.Format(" } %s;", func);
    ascript = scriptbegin + SpellBeginSource + scriptend;
    
    allowedEntry[0] = (LPCSTR)func;
    allowedEntry[1] = NULL;     
    SpellBeginBinary = gpdlComp.CompileScript(ascript, allowedEntry);
    
    if (SpellBeginBinary[0]!=0) success = false;
  }
  
  if (!SpellEndSource.IsEmpty()) 
  {
    func.Format("%s_e_%u", sname, GetTickCount());
    CString mscript;
    CString scriptbegin;
    CString scriptend;
    
    scriptbegin.Format("$PUBLIC $FUNC %s() { ", func);
    scriptend.Format(" } %s;", func);
    mscript = scriptbegin + SpellEndSource + scriptend;
    
    allowedEntry[0] = (LPCSTR)func;
    allowedEntry[1] = NULL;     
    SpellEndBinary = gpdlComp.CompileScript(mscript, allowedEntry);
    
    if (SpellEndBinary[0]!=0) success = false;
  }
      
  // scripts with each spell effect
  POSITION pos = m_EffectedAttributes.GetHeadPosition();
  while (pos != NULL)
    m_EffectedAttributes.GetNext(pos).CompileScripts(sname);

#endif
  
  if (!success)
  {
    SpellBeginBinary="";
    SpellEndBinary="";
  }
  
  return success;
}

//*****************************************************************************
//    NAME: SPELL_DATA::ExecuteActivationScript
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_DATA::ExecuteActivationScript(const SPELL_EFFECTS_DATA &src)
{
  return (src.ExecuteActivationScript());
}

//*****************************************************************************
//    NAME: SPELL_DATA::ExecuteModificationScript
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_DATA::ExecuteModificationScript(const SPELL_EFFECTS_DATA &src)
{
  return (src.ExecuteModificationScript());
}

//*****************************************************************************
//    NAME: SPELL_DATA::ExecuteSavingThrowScript
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_DATA::ExecuteSavingThrowScript(const SPELL_EFFECTS_DATA &src)
{
  return (src.ExecuteSavingThrowScript());
}

//*****************************************************************************
//    NAME: SPELL_DATA::ExecuteSavingThrowFailedScript
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::ExecuteSavingThrowFailedScript(const SPELL_EFFECTS_DATA &src)
{
  src.ExecuteSavingThrowFailedScript();
}

//*****************************************************************************
//    NAME: SPELL_DATA::ExecuteSavingThrowSucceededScript
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::ExecuteSavingThrowSucceededScript(const SPELL_EFFECTS_DATA &src)
{
  src.ExecuteSavingThrowSucceededScript();
}

//*****************************************************************************
//    NAME: SPELL_DATA::ExecuteSpellBeginScript
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_DATA::ExecuteSpellBeginScript() const
{ 
  bool result = true;
  if (!SpellBeginSource.IsEmpty())
  {
    if (SpellBeginBinary.GetLength() > 0)
    {
      if (spellGPDL.ExecuteScript(SpellBeginBinary,1)!="1")
        result = false;
    }
  }  
  return result;
}

//*****************************************************************************
//    NAME: SPELL_DATA::ExecuteSpellEndScript
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA::ExecuteSpellEndScript() const
{ 
  if (!SpellEndSource.IsEmpty())
  {
    if (SpellEndBinary.GetLength() > 0)
      spellGPDL.ExecuteScript(SpellEndBinary,1); // don't care about result
  }  
}

//*****************************************************************************
//    NAME: SPELL_DATA::CanPerformSpecAb
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_DATA::CanPerformSpecAb(specialAbilitiesType sa)
{
  return (specAbs.HaveSpecAb(sa)?TRUE:FALSE);
}

///////////////////////////////////////////////////////////////////////
//
// Spell Data Type Functions
//
///////////////////////////////////////////////////////////////////////

//*****************************************************************************
//    NAME: saveSounds
//
// PURPOSE: 
//
//*****************************************************************************
void saveSounds(SPELL_DATA_TYPE& data)
{
  data.SaveSounds();
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::SaveSounds
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::SaveSounds()
{
  POSITION p = SpellData.GetHeadPosition();
  while (p != NULL)
  {
    saveSound(SpellData.GetAt(p).CastSound, GLOBAL_SOUND);
    saveSound(SpellData.GetAt(p).MissileSound, GLOBAL_SOUND);
    saveSound(SpellData.GetAt(p).CoverageSound, GLOBAL_SOUND);
    saveSound(SpellData.GetAt(p).HitSound, GLOBAL_SOUND);
    saveSound(SpellData.GetAt(p).LingerSound, GLOBAL_SOUND);
    SpellData.GetNext(p);
  }
}

//*****************************************************************************
//    NAME: saveArt
//
// PURPOSE: 
//
//*****************************************************************************
void saveArt(SPELL_DATA_TYPE& data)
{
  data.SaveArt();
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::SaveArt
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::SaveArt()
{
  POSITION p = SpellData.GetHeadPosition();
  while (p != NULL)
  {
    saveArt(SpellData.GetAt(p).CastArt.filename, SpriteDib, GLOBAL_ART);
    saveArt(SpellData.GetAt(p).MissileArt.filename, SpriteDib, GLOBAL_ART);
    saveArt(SpellData.GetAt(p).TargCoverageArt.filename, SpriteDib, GLOBAL_ART);
    saveArt(SpellData.GetAt(p).TargHitArt.filename, SpriteDib, GLOBAL_ART);
    saveArt(SpellData.GetAt(p).TargLingerArt.filename, SpriteDib, GLOBAL_ART);
    SpellData.GetNext(p);
  }
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::PreSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  // before saving, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (IsStoring) ExpandDefaultFilenames();

  POSITION p = SpellData.GetHeadPosition();
  while (p!=NULL) 
    SpellData.GetNext(p).PreSerialize(IsStoring);
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::PostSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
  // after loading, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (!IsStoring) ExpandDefaultFilenames();

  POSITION p = SpellData.GetHeadPosition();
  while (p!=NULL) 
    SpellData.GetNext(p).PostSerialize(IsStoring);
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::PreCARSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::PreCARSerialize(BOOL IsStoring)
{
  // before saving, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (IsStoring) ExpandDefaultFilenames();

  POSITION p = SpellData.GetHeadPosition();
  while (p!=NULL) 
    SpellData.GetNext(p).PreCARSerialize(IsStoring);
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::PostCARSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::PostCARSerialize(BOOL IsStoring)
{
  // after loading, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (!IsStoring) ExpandDefaultFilenames();

  POSITION p = SpellData.GetHeadPosition();
  while (p!=NULL) 
    SpellData.GetNext(p).PostCARSerialize(IsStoring);
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::ExpandDefaultFilenames
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::ExpandDefaultFilenames()
{  
  POSITION p = GetHeadPosition();
  int key;

  while (p != NULL)
  {
    SPELL_DATA data = GetAt(p);

    if (data.CastSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.CastSound.Format("%s%s",EditorArt, DEF_CAST_SOUND);
#else
      data.CastSound = DEF_CAST_SOUND;
#endif
    }
    if (data.MissileSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.MissileSound.Format("%s%s",EditorArt, DEF_CAST_SOUND);
#else
      data.MissileSound = DEF_CAST_SOUND;
#endif
    }
    if (data.CoverageSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.CoverageSound.Format("%s%s",EditorArt, DEF_CAST_SOUND);
#else
      data.CoverageSound = DEF_CAST_SOUND;
#endif
    }
    if (data.HitSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.HitSound.Format("%s%s",EditorArt, DEF_HIT_SOUND);
#else
      data.HitSound = DEF_HIT_SOUND;
#endif
    }
    if (data.LingerSound.CompareNoCase("DEFAULT")==0)
    {
#ifdef UAFEDITOR
      data.LingerSound.Format("%s%s",EditorArt, DEF_CAST_SOUND);
#else
      data.LingerSound = DEF_CAST_SOUND;
#endif
    }

    data.SetSpellAnimDefaults(data.CastArt, DEFAULT_STH);
    data.SetSpellAnimDefaults(data.MissileArt, DEFAULT_SIR);
    data.SetSpellAnimDefaults(data.TargCoverageArt, DEFAULT_STC);
    data.SetSpellAnimDefaults(data.TargHitArt, DEFAULT_STH);
    data.SetSpellAnimDefaults(data.TargLingerArt, DEFAULT_STL);

    key = GetKeyAt(p);
    SetSpell(key, data);
    GetNext(p);
  }
}

#ifdef UAFEDITOR
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::CollapseDefaultFilenames
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::CollapseDefaultFilenames()
{  
  int key;
  POSITION p = GetHeadPosition();
  CString DefFilename;

  while (p != NULL)
  {
    SPELL_DATA data = GetAt(p);

    DefFilename.Format("%s%s",EditorArt, DEF_CAST_SOUND);
    if (data.CastSound.CompareNoCase(DefFilename)==0)
      data.CastSound = "DEFAULT";
    DefFilename.Format("%s%s",EditorArt, DEF_CAST_SOUND);
    if (data.MissileSound.CompareNoCase(DefFilename)==0)
      data.MissileSound = "DEFAULT";
    DefFilename.Format("%s%s",EditorArt, DEF_CAST_SOUND);
    if (data.CoverageSound.CompareNoCase(DefFilename)==0)
      data.CoverageSound = "DEFAULT";
    DefFilename.Format("%s%s",EditorArt, DEF_HIT_SOUND);
    if (data.HitSound.CompareNoCase(DefFilename)==0)
      data.HitSound = "DEFAULT";
    DefFilename.Format("%s%s",EditorArt, DEF_CAST_SOUND);
    if (data.LingerSound.CompareNoCase(DefFilename)==0)
      data.LingerSound = "DEFAULT";
    
    DefFilename.Format("%s%s",EditorArt, DEFAULT_STH);
    if (data.CastArt.filename.CompareNoCase(DefFilename)==0)
      data.CastArt.filename = "DEFAULT";

    DefFilename.Format("%s%s",EditorArt, DEFAULT_SIR);
    if (data.MissileArt.filename.CompareNoCase(DefFilename)==0)
      data.MissileArt.filename = "DEFAULT";

    DefFilename.Format("%s%s",EditorArt, DEFAULT_STC);
    if (data.TargCoverageArt.filename.CompareNoCase(DefFilename)==0)
      data.TargCoverageArt.filename = "DEFAULT";
    
    DefFilename.Format("%s%s",EditorArt, DEFAULT_STH);
    if (data.TargHitArt.filename.CompareNoCase(DefFilename)==0)
      data.TargHitArt.filename = "DEFAULT";
    
    DefFilename.Format("%s%s",EditorArt, DEFAULT_STL);
    if (data.TargLingerArt.filename.CompareNoCase(DefFilename)==0)
      data.TargLingerArt.filename = "DEFAULT";

    key = GetKeyAt(p);
    SetSpell(key, data);
    GetNext(p);
  }
}

#endif // UAFEDITOR

//*****************************************************************************
//    NAME: saveData
//
// PURPOSE: 
//
//*****************************************************************************
BOOL saveData(SPELL_DATA_TYPE& data)
{
  EditorStatusMsg("Saving spell data...");

  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, SPELL_DB_NAME);
  return saveData(data, fullPath);
}

//*****************************************************************************
//    NAME: saveData
//
// PURPOSE: 
//
//*****************************************************************************
BOOL saveData(SPELL_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open spell db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  // put signal value and version at start of file
  __int64 hdr=0xFABCDEFABCDEFABF;
  double ver = PRODUCT_VER;
  data.version = ver;
  myFile.Write(&hdr, sizeof(hdr));
  myFile.Write(&ver, sizeof(double));

  CAR ar(&myFile, CArchive::store);
  //ar.Compress(true); // qqqqq

  data.PreCARSerialize(ar.IsStoring());
  data.Serialize(ar, ver);
  data.PostCARSerialize(ar.IsStoring());
  ar.Close();
  return TRUE;
}

//*****************************************************************************
//    NAME: clearData
//
// PURPOSE: 
//
//*****************************************************************************
void clearData(SPELL_DATA_TYPE& data)
{
  data.Clear();
  data.version = PRODUCT_VER;
}

//*****************************************************************************
//    NAME: loadData
//
// PURPOSE: 
//
//*****************************************************************************
int loadData(SPELL_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;
  BOOL success=TRUE;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open spell db file %s, error %i\n", fullPath, e.m_cause);
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
    ver = min(globalData.version, _VERSION_0620_);
    //ver = _VERSION_0620_;
  }

  data.version = ver;  

  // if loading older level data, don't use compression archive
  if (ver < _VERSION_0630_)
  {
    WriteDebugString("Loading pre-0630 spell DB version - %f\n", ver);
    CArchive ar(&myFile, CArchive::load);
    try
    {
      data.PreSerialize(ar.IsStoring());
      data.Serialize(ar, ver);
      data.PostSerialize(ar.IsStoring());
    }
    catch (...)
    {
        WriteDebugString("Unable to load spell data file %s\n", ar.GetFile()->GetFileName());
        data.Clear();
        success=FALSE;
    }
    ar.Close();
  }
  else
  {
    WriteDebugString("Loading spell DB version: %f\n", ver);
    CAR ar(&myFile, CArchive::load);
    //ar.Compress(true); // qqqqq
    try
    {
      data.PreCARSerialize(ar.IsStoring());
      data.Serialize(ar, ver);
      data.PostCARSerialize(ar.IsStoring());
    }
    catch (...)
    {
        WriteDebugString("Unable to load spell data file %s\n", ar.GetFile()->GetFileName());
        data.Clear();
        success=FALSE;
    }
    ar.Close();
  }

  if (data.GetCount() <= 0)
     WriteDebugString("No spells in spell db file\n");

  return success; //data.GetCount();
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::Clear() 
{ 
  CSingleLock sLock(&m_CS, TRUE); 
  SpellData.RemoveAll(); 
  version=0.0; 
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::GetNextKey
//
// PURPOSE: 
//
//*****************************************************************************
DWORD SPELL_DATA_TYPE::GetNextKey()
{
  DWORD key=0;
  if (GetCount() > 0)
  {
    key = SpellData.GetTail().key;
    if (key >= UINT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_SPELLS.
      // Gaps occur when deletions are made to the spellData list
      POSITION pos = SpellData.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = SpellData.GetNext(pos).key;
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
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::Validate
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_DATA_TYPE::Validate(SPELL_DATA &data,FILE *pFile)
{
  BOOL success=TRUE;
  if (data.CastSound != "")
  {
    if (!FileExists(data.CastSound))
    {
      WriteValidateLog(pFile, "Spell '%s' missing Cast Sound file '%s'\n",
                       data.Name, data.CastSound);
      success = FALSE;
    }
  }
  if (data.MissileSound != "")
  {
    if (!FileExists(data.MissileSound))
    {
      WriteValidateLog(pFile, "Spell '%s' missing InRoute Sound file '%s'\n",
                       data.Name, data.MissileSound);
      success = FALSE;
    }
  }
  if (data.CoverageSound != "")
  {
    if (!FileExists(data.CoverageSound))
    {
      WriteValidateLog(pFile, "Spell '%s' missing Coverage Sound file '%s'\n",
                       data.Name, data.CoverageSound);
      success = FALSE;
    }
  }
  if (data.HitSound != "")
  {
    if (!FileExists(data.HitSound))
    {
      WriteValidateLog(pFile, "Spell '%s' missing Hit Sound file '%s'\n",
                       data.Name, data.HitSound);
      success = FALSE;
    }
  }
  if (data.LingerSound != "")
  {
    if (!FileExists(data.LingerSound))
    {
      WriteValidateLog(pFile, "Spell '%s' missing Linger Sound file '%s'\n",
                       data.Name, data.LingerSound);
      success = FALSE;
    }
  }

  POSITION pos = data.specAbs.GetHeadPosition();
  while (pos!=NULL)
  {
    SPEC_AB_DATA *spab = data.specAbs.GetNextData(pos);
    if (spab!=NULL)
    {
      if (!spab->script.IsEmpty())
      {
        if (spab->binary.GetLength()==0)
        {
          success = FALSE;
          WriteValidateLog(pFile, "Spell '%s' special ability script '%s' did not compile\n",
                           data.Name, spab->script);
        }
      }
    }
  }

  if (!data.SpellBeginSource.IsEmpty())
  {
    if (data.SpellBeginBinary.GetLength() == 0)
    {
      success = FALSE;
      WriteValidateLog(pFile, "Spell '%s', begin script '%s' did not compile\n",
        data.Name, data.SpellBeginSource);      
    }
  }
  if (!data.SpellEndSource.IsEmpty())
  {
    if (data.SpellEndBinary.GetLength() == 0)
    {
      success = FALSE;
      WriteValidateLog(pFile, "Spell '%s', end script '%s' did not compile\n",
        data.Name, data.SpellEndSource);      
    }
  }
    
  pos = data.m_EffectedAttributes.GetHeadPosition();
  while (pos!=NULL)
  {
    SPELL_EFFECTS_DATA spef = data.m_EffectedAttributes.GetNext(pos);
    if (!spef.activationScript.IsEmpty())
    {
      if (spef.activationBinary.GetLength()==0)
      {
        success = FALSE;
        WriteValidateLog(pFile, "Spell '%s', attribute %s, activate script '%s' did not compile\n",
                         data.Name, spef.affectedAttr, spef.activationScript.Left(400));
      }
    }
    if (!spef.modificationScript.IsEmpty())
    {
      if (spef.modificationBinary.GetLength()==0)
      {
        success = FALSE;
        WriteValidateLog(pFile, "Spell '%s', attribute %s, modification script '%s' did not compile\n",
          data.Name, spef.affectedAttr, spef.modificationScript.Left(400));
      }
    }
    if (!spef.savingThrowScript.IsEmpty())
    {
      if (spef.savingThrowBinary.GetLength()==0)
      {
        success = FALSE;
        WriteValidateLog(pFile, "Spell '%s', attribute %s, saving throw script '%s' did not compile\n",
          data.Name, spef.affectedAttr, spef.savingThrowScript.Left(400));
      }
    }  
    if (!spef.savingThrowFailedScript.IsEmpty())
    {
      if (spef.savingThrowFailedBinary.GetLength()==0)
      {
        success = FALSE;
        WriteValidateLog(pFile, "Spell '%s', attribute %s, saving throw failed script '%s' did not compile\n",
          data.Name, spef.affectedAttr, spef.savingThrowFailedScript.Left(400));
      }
    }  
    if (!spef.savingThrowSucceededScript.IsEmpty())
    {
      if (spef.savingThrowSucceededBinary.GetLength()==0)
      {
        success = FALSE;
        WriteValidateLog(pFile, "Spell '%s', attribute %s, saving throw succeeded script '%s' did not compile\n",
          data.Name, spef.affectedAttr, spef.savingThrowSucceededScript.Left(400));
      }
    }  
  }
  return success;
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::Validate
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_DATA_TYPE::Validate(FILE *pFile)
{
  BOOL success = TRUE;
  POSITION pos = SpellData.GetHeadPosition();
  while (pos != NULL)
  {
    if (!Validate(SpellData.GetNext(pos), pFile))
      success = FALSE;
  }
  return success;
}
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::Validate
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_DATA_TYPE::Validate(int index, FILE *pFile)
{
  BOOL success = TRUE;
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
  {
    if (!Validate(SpellData.GetAt(pos), pFile))
      success = FALSE;
  }
  else
  {
    WriteValidateLog(pFile, "Bogus spell key %i\n", index);
    success = FALSE;
  }
  return success;
}
#endif
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::PlayCast
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::PlayCast(DWORD index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
    SpellData.GetAt(pos).PlayCast();
}
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::ClearSounds
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::ClearSounds()
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  pos = SpellData.GetHeadPosition();
  while (pos != NULL)
     SpellData.GetNext(pos).ClearSounds();
}  
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::FreeArt
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::FreeArt()
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  pos = SpellData.GetHeadPosition();
  while (pos != NULL)
     SpellData.GetNext(pos).FreeArt();
}  
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::GetSpellClass
//
// PURPOSE: 
//
//*****************************************************************************
WORD SPELL_DATA_TYPE::GetSpellClass(DWORD index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
    return SpellData.GetAt(pos).ClassMask;
  else
    return 0;
}

//*****************************************************************************
//    NAME: GetSpellCastMask
//
// PURPOSE: 
//
//*****************************************************************************
WORD SPELL_DATA_TYPE::GetSpellCastMask(DWORD index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
    return SpellData.GetAt(pos).CastMask;
  else
    return 0;
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::GetSpellLevel
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_DATA_TYPE::GetSpellLevel(DWORD index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
    return SpellData.GetAt(pos).Level;
  else
    return 0;
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::GetSpellCastCost
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_DATA_TYPE::GetSpellCastCost(DWORD index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
    return SpellData.GetAt(pos).Cast_Cost;
  else
    return 0;
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::GetSpellName
//
// PURPOSE: 
//
//*****************************************************************************
CString SPELL_DATA_TYPE::GetSpellName(DWORD index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
    return SpellData.GetAt(pos).Name;
  else
    return CString("No Spell");
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::HaveSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_DATA_TYPE::HaveSpell(DWORD index)
{
  CSingleLock sLock(&m_CS, TRUE);
  return (SpellData.FindKeyPos(index) != NULL);
}


// called when loading from scripts, or
// when adding from editor GUI
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::AddSpell
//
// PURPOSE: 
//
//*****************************************************************************
DWORD SPELL_DATA_TYPE::AddSpell(SPELL_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (strlen(data.Name) == 0) return 0;
  // Do not allow multiple items with same full name.
  if (m_FindName(data.Name)!=0) return 0;
  if (SpellData.GetCount() < MAX_SPELLS)
  {
    // begin/end scripts
    data.CompileScripts();

    CString basename(data.Name); // start with spell name
    basename.Remove(' '); // but remove spaces from name
    basename += "_";

    // scripts with each spell effect
    POSITION pos = data.m_EffectedAttributes.GetHeadPosition();
    while (pos != NULL)
      data.m_EffectedAttributes.GetNext(pos).CompileScripts(basename);

    data.key = GetNextKey();
    SpellData.Insert(data, data.key);
    return data.key;
  }
  else
    return 0;
}

// called when loading binary monster data that
// was saved with key info
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::AddSpellWithCurrKey
//
// PURPOSE: 
//
//*****************************************************************************
DWORD SPELL_DATA_TYPE::AddSpellWithCurrKey(SPELL_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (SpellData.GetCount() < MAX_SPELLS)
  {
    SpellData.Insert(data, data.key);
    return data.key;
  }
  else
    return 0;
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::SetSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_DATA_TYPE::SetSpell(DWORD index, SPELL_DATA &spell)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
  {
    SpellData.GetAt(pos) = spell;
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::RemoveSpell
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_DATA_TYPE::RemoveSpell(DWORD index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
  {
    SpellData.RemoveAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: *SPELL_DATA_TYPE::GetSpellData
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_DATA *SPELL_DATA_TYPE::GetSpellData(DWORD index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = SpellData.FindKeyPos(index)) != NULL)
    return &SpellData.GetAt(pos);//return &SpellData.GetAt(index);
  else
    return NULL;
}
// No locking... for internal use
//*****************************************************************************
//    NAME: 
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_DATA_TYPE::m_FindName (const CString& name)
// Return key associated with the named object, or zero
{
  POSITION pos;
  pos=SpellData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (SpellData.GetAt(pos).Name==name) 
          return SpellData.GetAt(pos).key;
    SpellData.GetNext(pos);
  };
  return 0;
}
// With locking...for public use
//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::FindName
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}

//*****************************************************************************
// NAME: SPELL_DATA_TYPE::operator =
//
// PURPOSE: Make a copy of a SPELL_DATA list
//*****************************************************************************
SPELL_DATA_TYPE& SPELL_DATA_TYPE:: operator = (SPELL_DATA_TYPE& src)
{
  if (&src==this) return *this;
  SpellData.RemoveAll();
  POSITION p=src.SpellData.GetHeadPosition();
  while (p!=NULL)
  {
    AddSpellWithCurrKey(src.SpellData.GetNext(p));
  };
  //pos=src.pos;
  return *this;
}

//*****************************************************************************
// NAME: SPELL_DATA_TYPE::operator ==
//
// PURPOSE: Test that two SPELL_DATA lists are identical...even the ordering.
//          One might like to exclude order but then it is a bit tricky
//          because of the possiblility of two identical entries in a list.
//*****************************************************************************
bool SPELL_DATA_TYPE:: operator == (SPELL_DATA_TYPE& src)
{
  POSITION p1=SpellData.GetHeadPosition();
  POSITION p2=src.SpellData.GetHeadPosition();
  while (p1!=NULL)
  {
    if (p2==NULL) return false;
    if (!(SpellData.GetNext(p1)==src.SpellData.GetNext(p2))) return false;
  };
  if (p2!=NULL) return false;
//  if (pos!=src.pos) return false;
  return true;
}


//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);
  POSITION p;
  if (ar.IsStoring())
  {
    ar << GetCount();
    p = SpellData.GetHeadPosition();
    while (p != NULL)
      SpellData.GetNext(p).Serialize(ar, ver);
  }
  else
  {
    //Clear();
    //this->version = ver;
    SPELL_DATA data;
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar, ver);
      AddSpellWithCurrKey(data);
    }
  }
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::Serialize(CAR &ar, double ver)
{
  POSITION p;
  if (ar.IsStoring())
  {
    ar << GetCount();
    p = SpellData.GetHeadPosition();
    while (p != NULL)
      SpellData.GetNext(p).Serialize(ar, ver);
  }
  else
  {
    //Clear();
    //this->version = ver;

    SPELL_DATA data;
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar, ver);
      AddSpellWithCurrKey(data);
    }
  }
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::Save
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::Save(CArchive& ar) 
{
	if (version < _ASL_LEVEL_) return;
	ar << GetCount();
	POSITION p=SpellData.GetHeadPosition();
	while (p!=NULL)
	{
		ar << SpellData.GetAt(p).Name;
		ar << SpellData.GetAt(p).key;
		SpellData.GetAt(p).Save(ar);
		SpellData.GetNext(p);
	};
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::Restore
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::Restore(CArchive& ar)
{		
	CString spellName;
	DWORD spellKey;
	int count;
	POSITION p;
	if (version < _ASL_LEVEL_) return;

	// Let items know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ATTRIBUTES in case no restore operation clears it.
	p=SpellData.GetHeadPosition();
  while (p!=NULL) 
    SpellData.GetNext(p).PrepareForRestore();

	ar >> count;

	for (int i=0; i<count; i++)
	{
		ar >> spellName;
		ar >> spellKey;

		p=SpellData.GetHeadPosition();
		while (p!=NULL) 
    {
			if (SpellData.GetAt(p).Name==spellName)
			{
				SpellData.GetAt(p).Restore(ar);
				break;
			}
			SpellData.GetNext(p);
		}

		if (p==NULL)
		{
			SPELL_DATA junk;
			CString msg;
			junk.Restore(ar); // discard
			msg.Format("Restoring Spell \"%s\".  No such spell in design\n",
				spellName);
			WriteDebugString(msg);
		}
	}
}

//*****************************************************************************
//    NAME: SPELL_DATA_TYPE::CommitRestore
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_DATA_TYPE::CommitRestore(void) 
{
	POSITION p;
	p=SpellData.GetHeadPosition();
	while (p!=NULL) 
	{
		SpellData.GetNext(p).CommitRestore();
	};
}


//*****************************************************************************
//    NAME: clearSpellData
//
// PURPOSE: 
//
//*****************************************************************************
void clearSpellData(void)
{
  spellData.Clear();
}


///////////////////////////////////////////////////////////////////////
//
// Spell targeting Data Functions
//
///////////////////////////////////////////////////////////////////////

#ifdef UAFEngine 

//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::SPELL_TARGETING_DATA
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_TARGETING_DATA::SPELL_TARGETING_DATA() 
{ 
  Clear(); 
}

//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::SPELL_TARGETING_DATA
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_TARGETING_DATA::SPELL_TARGETING_DATA(const SPELL_TARGETING_DATA &src) 
{ 
  *this=src; 
}

//*****************************************************************************
//    NAME: &SPELL_TARGETING_DATA::operator=
//
// PURPOSE: 
//
//*****************************************************************************
SPELL_TARGETING_DATA &SPELL_TARGETING_DATA::operator=(const SPELL_TARGETING_DATA &src)
{
  if (this==&src)return *this;
  m_type=src.m_type;
  m_SelectingUnits=src.m_SelectingUnits;
  m_FriendlyFireOk=src.m_FriendlyFireOk;
  m_IsPartyAligned=src.m_IsPartyAligned;
  m_MaxHitDice=src.m_MaxHitDice;
  m_MaxTargets=src.m_MaxTargets;
  m_MaxRange=src.m_MaxRange;
  m_MapTargetX=src.m_MapTargetX;
  m_MapTargetY=src.m_MapTargetY;
  m_HDTotal = src.m_HDTotal;
  m_targets.RemoveAll();
  m_targets.SetSize(src.m_targets.GetSize());
  for (int i=0;i<src.m_targets.GetSize(); i++)
    m_targets[i] = src.m_targets[i];

  m_maptarglist.RemoveAll();
  POSITION pos = src.m_maptarglist.GetHeadPosition();
  while (pos != NULL)
  {
    POINT pt = src.m_maptarglist.GetNext(pos);
    m_maptarglist.AddTail(pt);
  }
  return *this;
}
//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::operator==
//
// PURPOSE: 
//
//*****************************************************************************
bool SPELL_TARGETING_DATA::operator==(const SPELL_TARGETING_DATA& src) const
{
  if (m_type != src.m_type) return false;
  if (m_SelectingUnits != src.m_SelectingUnits) return false;
  if (m_FriendlyFireOk != src.m_FriendlyFireOk) return false;
  if (m_IsPartyAligned != src.m_IsPartyAligned) return false;
  if (m_MaxHitDice != src.m_MaxHitDice) return false;
  if (m_MaxTargets != src.m_MaxTargets) return false;
  if (m_MaxRange != src.m_MaxRange) return false;
  if (m_MapTargetX != src.m_MapTargetX) return false;
  if (m_MapTargetY != src.m_MapTargetY) return false;
  if (m_HDTotal != src.m_HDTotal) return false;
  if (m_targets.GetSize() != src.m_targets.GetSize()) return false;
  for (int i=0;i<m_targets.GetSize();i++)
    if (m_targets[i] != src.m_targets[i]) return false;
  if (m_maptarglist.GetCount() != src.m_maptarglist.GetCount()) return false;
  POSITION p1=m_maptarglist.GetHeadPosition();
  POSITION p2=src.m_maptarglist.GetHeadPosition();
  while (p1!=NULL)
  {
    if (p2==NULL) return false;
    POINT pt1 = m_maptarglist.GetNext(p1);
    POINT pt2 = src.m_maptarglist.GetNext(p2);
    if (pt1.x != pt2.x) return false;
    if (pt1.y != pt2.y) return false;
  }
  if (p2!=NULL) return false;
  
  return true;
}
//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TARGETING_DATA::Clear()
{
  m_type=Self;
  m_SelectingUnits=TRUE;
  m_FriendlyFireOk=TRUE;
  m_IsPartyAligned=TRUE;
  m_MaxHitDice=0;
  m_MaxTargets=0;
  m_MaxRange=0;
  m_MapTargetX=-1;
  m_MapTargetY=-1;
  m_HDTotal=0.0;
  m_targets.RemoveAll();
  m_maptarglist.RemoveAll();
}
//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::NumTargets
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_TARGETING_DATA::NumTargets() 
{ 
  return m_targets.GetSize(); 
}

//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::CanAddTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TARGETING_DATA::CanAddTarget(int hd, int range) 
{ 
  if (m_SelectingUnits)
  {
    if ((m_MaxTargets > 0) && (NumTargets() >= m_MaxTargets))
      return FALSE;
    if ((m_MaxHitDice > 0) && (hd+m_HDTotal > m_MaxHitDice))
      return FALSE;
    if ((m_MaxRange > 0) && (range > m_MaxRange))
      return FALSE;
  }
  else
  {
    return ((m_MapTargetX<0)&&(m_MapTargetY<0));  
  }
  return TRUE;
}

//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::HDLimitReached
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TARGETING_DATA::HDLimitReached()
{
  if (m_type!=SelectByHitDice) return FALSE;
  return ((m_MaxHitDice > 0) && (m_HDTotal >= m_MaxHitDice));
}

//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::FormatRemainingTargsText
//
// PURPOSE: 
//
//*****************************************************************************
CString SPELL_TARGETING_DATA::FormatRemainingTargsText()
{
  char title[50];

  switch (m_type)
  {
  case Self:
  case SelectedByCount:
  case WholeParty:
  case TouchedTargets:
    {
      int diff = (m_MaxTargets-NumTargets());
      if (IsCombatActive())
      {
        if (diff > GetNumCombatants()) diff = GetNumCombatants();
      }
      else
      {
        if (diff > party.numCharacters) diff = party.numCharacters;
      }
      sprintf(title, "CHOOSE %u TARGETS", diff);
    }
    break;
  case SelectByHitDice:
    sprintf(title, "CHOOSE %2.1f HIT DICE", (m_MaxHitDice-m_HDTotal));
    break;
  case AreaCircle:
    strcpy(title, "CHOOSE CENTER OF CIRCLE");
    break;
  case AreaLinePickStart:
    strcpy(title, "CHOOSE START OF LINE");
    break;
  case AreaLinePickEnd:  
    strcpy(title, "CHOOSE END OF LINE");
    break;
  case AreaSquare:       
    strcpy(title, "CHOOSE CENTER OF SQUARE");
    break;
  case AreaCone:         
    strcpy(title, "CHOOSE START OF CONE");
    break;
  default:
    ASSERT(FALSE);
    strcpy(title, "CHOOSE");
    break;
  }
  return (CString(title));
}

//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::ValidNumTargets
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TARGETING_DATA::ValidNumTargets()
{
  switch (m_type)
  {
  case Self:
  case SelectedByCount:
  case WholeParty:
  case TouchedTargets:
    return (m_MaxTargets > 0);

  case SelectByHitDice:
    return (m_MaxHitDice > 0);

  case AreaSquare:
  case AreaCone:
  case AreaLinePickStart:
  case AreaLinePickEnd:  
  case AreaCircle:
    return ((m_MaxRange>0)&&(m_MaxTargets > 0));  

  default:
    ASSERT(FALSE);
    break;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::AllTargetsChosen
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TARGETING_DATA::AllTargetsChosen()
{
  switch (m_type)
  {
  case Self:
  case SelectedByCount:
  case WholeParty:
  case TouchedTargets:
    if (NumTargets() >= m_MaxTargets)
      return TRUE;
    if (IsCombatActive())
    {
      if (NumTargets() >= GetNumCombatants())
        return TRUE;
    }
    else
    {
      if (NumTargets() >= party.numCharacters)
        return TRUE;
    }
    break;
  case SelectByHitDice:
    if (HDLimitReached()) return TRUE;
    if (IsCombatActive())
    {
      if (NumTargets() >= GetNumCombatants())
        return TRUE;
    }
    else
    {
      if (NumTargets() >= party.numCharacters)
        return TRUE;
    }
    break;
  case AreaLinePickStart:
  case AreaLinePickEnd:
  case AreaSquare:     
  case AreaCone:       
  case AreaCircle:
    return ((m_MapTargetX>=0)&&(m_MapTargetY>=0));
    break;

  default:
    ASSERT(FALSE);
    break;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::AddTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TARGETING_DATA::AddTarget(int target, double hd, int range)
{
  if (m_SelectingUnits==FALSE) return FALSE;
  if (!CanAddTarget(hd,range)) return FALSE;  
  for (int i=0;i<NumTargets(); i++)
  {
    if (m_targets[i] == target)
      return FALSE; // this target already added
  }
  int index = m_targets.GetSize();
  m_targets.SetAtGrow(index, target);
  if (m_type==SelectByHitDice)
  {
    m_HDTotal+=hd;
    TRACE("HD adding %f, total %f, max %f\n",hd, m_HDTotal, m_MaxHitDice);
  }
  return TRUE;
}

/*
//
// data format for map targeting:
// 32 bits, 16 bits each for mapx and mapy: XX XX YY YY
//
void SplitMapTargetingData(DWORD data, WORD &mapx, WORD &mapy)
{
  mapx  = static_cast<WORD>((data >> 16) & 0x0000FFFF);
  mapy  = static_cast<WORD>(data & 0x0000FFFF);
}

void EncodeMapTargetingData(DWORD &data, WORD mapx, WORD mapy)
{
  data = 0;
  mapx  &= 0xFFFF;  data |= (mapx << 16);
  mapy  &= 0xFFFF;  data |= mapy;  
}
*/

//*****************************************************************************
//    NAME: SPELL_TARGETING_DATA::AddMapTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TARGETING_DATA::AddMapTarget(int mapx, int mapy)
{
  if (m_SelectingUnits==TRUE) return FALSE;
  if (!CanAddTarget(0,0)) return FALSE;
  m_MapTargetX=mapx;
  m_MapTargetY=mapy;
  return TRUE;
}

///////////////////////////////////////////////////////////////////////
//
// Pending Spell List Functions
//
///////////////////////////////////////////////////////////////////////

//*****************************************************************************
//    NAME: PendingSpellList::GetNextKey
//
// PURPOSE: 
//
//*****************************************************************************
int PendingSpellList::GetNextKey()
{
  int key=-1;
  if (GetCount() > 0)
  {
    key = m_spells.GetTail().key;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than max elements
      // Gaps occur when deletions are made to the list
      POSITION pos = GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = GetNext(pos).key;
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

//*****************************************************************************
//    NAME: PendingSpellList::Add
//
// PURPOSE: 
//
//*****************************************************************************
int PendingSpellList::Add(PENDING_SPELL &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  data.key = GetNextKey();
  m_spells.Insert(data, data.key);
  return data.key;
}

//*****************************************************************************
//    NAME: PendingSpellList::Set
//
// PURPOSE: 
//
//*****************************************************************************
BOOL PendingSpellList::Set(int index, PENDING_SPELL &spell)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos=NULL;
  if ((pos = m_spells.FindKeyPos(index)) != NULL)
  {
    GetAt(pos) = spell;
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: PendingSpellList::Remove
//
// PURPOSE: 
//
//*****************************************************************************
BOOL PendingSpellList::Remove(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos=NULL;
  if ((pos = m_spells.FindKeyPos(index)) != NULL)
  {
    m_spells.RemoveAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: *PendingSpellList::Get
//
// PURPOSE: 
//
//*****************************************************************************
PENDING_SPELL *PendingSpellList::Get(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos=NULL;
  if ((pos = m_spells.FindKeyPos(index)) != NULL)
    return &GetAt(pos);
  else
    return NULL;
}

// check to see if casting time has passed, then inform caster
// to activate spell and remove from pending.
//
// ** elapsed time check only needed when in combat **
//
//*****************************************************************************
//    NAME: PendingSpellList::ProcessTimeSensitiveData
//
// PURPOSE: 
//
//*****************************************************************************
void PendingSpellList::ProcessTimeSensitiveData(int roundInc, int currInitiative)
{
  //
  // what happens to pending spells when combat ends?
  //
  ASSERT( IsCombatActive() );
  if (IsCombatActive() == FALSE) return;

  POSITION pos=GetHeadPosition();
  while (pos != NULL)
  {
    POSITION tmp=pos;
    GetNext(tmp);
    PENDING_SPELL data = GetAt(pos);
    bool castIt=false;

    ASSERT(data.caster.IsValidSrc());
    ASSERT(data.caster.IsCombatSrc());

    switch (data.caster.GetSrc())
    {
    case FLAG_PARTY_MEMBER:
    case FLAG_NPC:
    case FLAG_MONSTER:
      {        
        spellCastingTimeType type = (spellCastingTimeType) data.flags;
        switch (type)
        {
        case sctImmediate:
          castIt=true;
          break;
        case sctInitiative:
          {
            COMBATANT &dude = GetCombatCharacterContext(data.caster);
            castIt = ((currInitiative>dude.initiative)||(roundInc>0));
          }
          break;
        case sctRounds:
        case sctTurns:
          castIt = (data.waitUntil >= (DWORD)CurrCombatRound());
          break;
        }    
      }
      break;

    case FLAG_ITEM:
      // item spells are cast right away
      castIt=true;
      break;
    }
    
    if (castIt)
    {      
      WriteDebugString("Activating pending spell: sdbkey %i\n", data.key);
      WriteDebugString("%u pending spells remaining\n", GetCount()-1);      
      SpellActivate(data);
      RemoveAt(pos);
    }   

    pos=tmp;
  }
}

///////////////////////////////////////////////////////////////////////
//
// Active Spell List Functions
//
///////////////////////////////////////////////////////////////////////


//*****************************************************************************
//    NAME: ActiveSpellList::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void ActiveSpellList::Clear() 
{ 
  CSingleLock sLock(&m_CS, TRUE); 
  //POSITION pos = GetHeadPosition();
  //while (pos != NULL)
  //  party.RemoveSpellEffect(GetNext(pos).key);
  m_spells.RemoveAll();
}

//*****************************************************************************
//    NAME: ActiveSpellList::GetNextKey
//
// PURPOSE: 
//
//*****************************************************************************
int ActiveSpellList::GetNextKey()
{
  int key=-1;
  if (GetCount() > 0)
  {
    key = m_spells.GetTail().key;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than max elements
      // Gaps occur when deletions are made to the list
      POSITION pos = GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = GetNext(pos).key;
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

// called when loading from scripts, or
// when adding from editor GUI
//*****************************************************************************
//    NAME: ActiveSpellList::Add
//
// PURPOSE: 
//
//*****************************************************************************
int ActiveSpellList::Add(ACTIVE_SPELL &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  //if (strlen(data.Name) == 0) return 0;
  // Do not allow multiple items with same full name.
  //if (m_FindName(data.Name)!=0) return 0;
  data.key = GetNextKey();
  m_spells.Insert(data, data.key);
  return data.key;
}

// called when loading binary monster data that
// was saved with key info
//*****************************************************************************
//    NAME: ActiveSpellList::AddWithCurrKey
//
// PURPOSE: 
//
//*****************************************************************************
int ActiveSpellList::AddWithCurrKey(ACTIVE_SPELL &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  WriteDebugString("ActiveSpellList::AddWithCurrKey(): key %i, spelldbkey %i\n", 
                   data.key,data.spellKey);

  m_spells.Insert(data, data.key);
  return data.key;
}

//*****************************************************************************
//    NAME: ActiveSpellList::Set
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ActiveSpellList::Set(int index, ACTIVE_SPELL &spell)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos=NULL;
  if ((pos = m_spells.FindKeyPos(index)) != NULL)
  {
    GetAt(pos) = spell;
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ActiveSpellList::Remove
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ActiveSpellList::Remove(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos=NULL;
  if ((pos = m_spells.FindKeyPos(index)) != NULL)
  {
    m_spells.RemoveAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: *ActiveSpellList::Get
//
// PURPOSE: 
//
//*****************************************************************************
ACTIVE_SPELL *ActiveSpellList::Get(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos=NULL;
  if ((pos = m_spells.FindKeyPos(index)) != NULL)
    return &GetAt(pos);
  else
    return NULL;
}
//*****************************************************************************
//    NAME: &ActiveSpellList::operator=
//
// PURPOSE: 
//
//*****************************************************************************
ActiveSpellList &ActiveSpellList::operator=(ActiveSpellList &src)
{
  if (this == &src) return *this;
  m_spells.RemoveAll();
  POSITION pos = src.GetHeadPosition();
  while (pos!=NULL)
    AddWithCurrKey(src.GetNext(pos));
  return *this;
}
//*****************************************************************************
//    NAME: ActiveSpellList::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ActiveSpellList::Serialize(CAR &ar, double ver)
{
  POSITION p;
  if (ar.IsStoring())
  {
    ar << GetCount();
    p = GetHeadPosition();
    while (p != NULL)
      GetNext(p).Serialize(ar, ver);
  }
  else
  {
    m_spells.RemoveAll();
    ACTIVE_SPELL data;
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar, ver);
      AddWithCurrKey(data);
      WriteDebugString("Loading active spell: k %i, sk %i, st %u, c %u\n",
                       data.key, data.spellKey, data.StopTime, data.caster.Instance);
    }
  }
}

//*****************************************************************************
//    NAME: ActiveSpellList::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ActiveSpellList::Serialize(CArchive &ar, double ver)
{
  POSITION p;
  if (ar.IsStoring())
  {
    ar << GetCount();
    p = GetHeadPosition();
    while (p != NULL)
      GetNext(p).Serialize(ar, ver);
  }
  else
  {
    m_spells.RemoveAll();
    ACTIVE_SPELL data;
    int count;
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Clear();
      data.Serialize(ar, ver);
      AddWithCurrKey(data);
      WriteDebugString("Loading active spell: k %i, sk %i, st %u, c %u\n",
                       data.key, data.spellKey, data.StopTime, data.caster.Instance);
    }
  }
}

//*****************************************************************************
//    NAME: ActiveSpellList::DeactivateActiveSpell
//
// PURPOSE: 
//
//*****************************************************************************
void ActiveSpellList::DeactivateActiveSpell(int index, SPELL_DATA *pSpell)
{ 
  if (pSpell != NULL)
    WriteDebugString("Deactivating spell from ActiveSpellList: akey %i, spell %s\n", index, pSpell->Name);
  else
    WriteDebugString("Deactivating spell from ActiveSpellList: akey %i, spell NULL\n", index);

  if (IsCombatActive())
    combatData.RemoveSpellEffect(index,pSpell);
  else
    party.RemoveSpellEffect(index,pSpell);
  
  // 'linger' spell animations
  RemoveAnimationFromList(index);  

  Remove(index);
}

// check to see if spell is ready to expire, then
// remove effect from all characters
//
//*****************************************************************************
//    NAME: ActiveSpellList::ProcessTimeSensitiveData
//
// PURPOSE: 
//
//*****************************************************************************
void ActiveSpellList::ProcessTimeSensitiveData(int roundInc)
{
  POSITION pos=GetHeadPosition();
  while (pos != NULL)
  {
    POSITION tmp=pos;
    GetNext(tmp);

    ACTIVE_SPELL &data = GetAt(pos);

    SPELL_DATA *pSpell = spellData.GetSpellData(data.spellKey);
    
    bool SpellOver=false;

    if (pSpell != NULL)
    {     
      switch (pSpell->Duration_Rate)
      {
      case byNbrAttacks:
      case byDamageTaken: 
        if (data.CountTime >= data.StopTime)
          SpellOver=true;
        break;

      default:
        if (party.getElapsedMinutes() > data.StopTime)
          SpellOver=true;
        break;
      }

      if (SpellOver)
        WriteDebugString("Removing active spell cast by %i\n", data.caster.Instance);        
    }
    else
    {
      ASSERT(FALSE);
      SpellOver=true;
      WriteDebugString("Removing active spell cast by %i - invalid spell key\n", data.caster.Instance);
    }

    if (SpellOver)
      DeactivateActiveSpell(data.key, pSpell);

    pos=tmp;
  }
}

//*****************************************************************************
//    NAME: ActiveSpellList::RemoveLingerSpells
//
// PURPOSE: After combat is over, all spells that linger on the combat map
//          need to be removed.
//
//*****************************************************************************
void ActiveSpellList::RemoveLingerSpells()
{
  POSITION pos = GetHeadPosition();
  POSITION temp;
  while (pos != NULL)
  {
    temp = pos;
    ACTIVE_SPELL &as = GetNext(pos);
    ASSERT( as.key >= 0 );

    if (as.Lingers)
    {
      // 'linger' spell animations
      RemoveAnimationFromList(as.key);       
      Remove(as.key);      
    }   
  }   
}

//*****************************************************************************
//    NAME: ActiveSpellList::ActivateLingerSpellsOnTarget
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ActiveSpellList::ActivateLingerSpellsOnTarget(int targ, int mapx, int mapy)
{
  CSingleLock sLock(&m_CS, TRUE);
  
  ASSERT( targ != NO_DUDE );

  BOOL AtLeastOneSpellActivated = FALSE;
  
  POSITION pos = GetHeadPosition();
  while (pos != NULL)
  {
    ACTIVE_SPELL &as = GetAt(pos);
    ASSERT( as.key >= 0 );

    if (as.LingerSpellAffectsTarget(targ, mapx, mapy))
    {
      AtLeastOneSpellActivated=TRUE;
      as.lingerData.AddTarget(targ);

      int caster = as.caster.GetInstance();
      ASSERT( caster != NO_DUDE );

      if (caster == NO_DUDE)
        continue;        
      
      COMBATANT *cdude = GetCombatantPtr(caster);
      
      combatData.InvokePendingSpellOnCombatant(cdude->self, targ, as.key, as.spellKey);
    }

    GetNext(pos);
  }

  return AtLeastOneSpellActivated;
}

#endif // ifdef UAFEngine


///////////////////////////////////////////////////////////////////////
//
// Spell Text List Functions
//
///////////////////////////////////////////////////////////////////////

#ifdef UAFEngine

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::Clear()
{
  title = "";
  //m_SpellLevelFilter=0;
  spellClasses=0;  
  m_flags=LEVEL_FLAG;
  CostFactor = Normal;
  m_CharData=NULL;
  memset(CurrSelect,0,sizeof(CurrSelect));
  memset(MaxSelect,0,sizeof(MaxSelect));
  memset(MaxLevel, 0, sizeof(MaxLevel));
  isSelected.RemoveAll();
  spells.RemoveAll();
}

int CompareSpellsForNewSpellList( const void *arg1, const void *arg2 )
{
  /*
    must return
  < 0 elem1 less than elem2 
  = 0 elem1 equivalent to elem2 
  > 0 elem1 greater than elem2 
  */
  ASSERT( (arg1!=NULL) && (arg2!=NULL) );
  if ((arg1==NULL)||(arg2==NULL)) return 0;

  SPELL_DATA *pElem1 = spellData.GetSpellData( *((DWORD*)arg1) );
  SPELL_DATA *pElem2 = spellData.GetSpellData( *((DWORD*)arg2) );

  // sort by spell level, then by spell name
  if (pElem1->Level < pElem2->Level) return -1;
  if (pElem1->Level > pElem2->Level) return  1;

  if (pElem1->Level == pElem2->Level)
  {
    if (pElem1->Name < pElem2->Name) return -1;
    if (pElem1->Name > pElem2->Name) return  1;
  }

  // level and name are both equal
  return 0;
}


//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::FillNewSpellLevelListText
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::FillNewSpellLevelListText(CHARACTER &data)
{
  int i;
	Clear();
  m_CharData=&data;
  m_flags=NEW_SPELL_LEVEL_FORMAT_FLAG; 
  spellClasses = SetClassBitsFromCharClass(data.GetAdjClass());
  
  CArray<DWORD, DWORD> sortarray;

  // only MU spells for now
  // clerics (and derivative classes) have access to all spells automatically
  //
  if (!CanCastMagicUserSpells(data.GetAdjClass()))
  {
    ASSERT(FALSE);
    return;
  }  

  // When a character advances a level, the spell book is
  // auto updated per the following:
  //
  // Magic Users will automatically be able to select spells that
  //    CanScribeAtLevelChange=TRUE
  //    character is eligible to memorize (intel, level)
  //    is not already in spell book
  //
  // For spells that are not added automatically, the
  // char gets to choose from a list of spells of a 
  // certain level, and pick spells until the 
  // max spell limit for char level is reached.
  //

  // fill in the list of spells that are allowed to be known
  POSITION pos = spellData.GetHeadPosition();
  while (pos != NULL)
  {
    SPELL_DATA sdata = spellData.GetAt(pos);
    // if spell level matches new spell level
    if ((sdata.UsableByParty) && (sdata.CanScribeAtLevelChange) && (sdata.ClassMask == MagicUserFlag) && (sdata.CastMask & spellClasses))
    {
      // fill in the number of spells that *can* be known

      // !!!!!!!! this next line makes the assumption that we are selecting spells for a MU only !!!!!!!!!!
      // which is true for the current rules, but may change when rules are more configurable
      if (data.spells.spellLimits.MaxSpellsForLevel(MagicUserFlag, data.GetCurrentLevel(MagicUserFlag), sdata.Level, data.GetPrimeSpellCastingScore(MagicUserFlag)) > 0)
      {
        // if not already in character's spell book
        if (!data.spells.list.HaveSpell(sdata.key))
        {
          if (data.spells.CanKnowSpell(MagicUserFlag, sdata.ClassMask, sdata.CastMask, sdata.Level))
          {          
            ASSERT( NumClassBits(sdata.ClassMask) == 1 );
            
            // add to temp list so that we can sort the output properly (later)
            //Add(sdata.key);
            sortarray.Add(sdata.key);
          }
        }
      }
    }

    spellData.GetNext(pos);
  }

  // if more than one element, sort them
  if (sortarray.GetSize() > 1)
    qsort( &sortarray[0], sortarray.GetSize(), sizeof(DWORD), CompareSpellsForNewSpellList);

  // add sorted elements to list
  for (i=0;i<sortarray.GetSize();i++)
    Add(sortarray[i]);

  sortarray.RemoveAll();

  // fill in the number of spells currently known
  pos = data.spells.list.GetHeadPosition(); // access ordered list 
  while (pos != NULL)
  {
    int slevel = data.spells.list.GetAt(pos).level;
    
    ASSERT ((slevel>=1)&&(slevel<=MAX_SPELL_LEVEL));
    if (slevel <= 0) slevel = 1;

    SPELL_DATA *sdata = spellData.GetSpellData(data.spells.list.GetAt(pos).spell); 

    ASSERT( NumClassBits(sdata->ClassMask) == 1 );

    if (sdata->ClassMask == MagicUserFlag)
      CurrSelect[slevel-1][FlagToIndex(GetSpellCastingClassForCharClass(sdata->ClassMask))] += 1; // inc num spells known at this level
    
    data.spells.list.GetNext(pos);
  }

  // fill in the number of spells that *can* be known
  for (i=0;i<NumClassFlags;i++)
  {
    WORD flag = (spellClasses & (0x0001 << i));
    if (flag)
    {
      if (GetSpellCastingClassForCharClass(flag) == MagicUserFlag)
      {
        int index = FlagToIndex(GetSpellCastingClassForCharClass(flag));
        MaxLevel[index] += CurrSpellLevel(flag,
                                         data.GetCurrentLevel(flag),
                                         data.GetPrimeSpellCastingScore(flag), 
                                         GetSpellCastingClassForCharClass(flag));

        for (int j=0;j<MaxLevel[index];j++)
        {
          MaxSelect[j][index] += 
            data.spells.spellLimits.MaxSpellsForLevel(flag, data.GetCurrentLevel(flag), j+1, data.GetPrimeSpellCastingScore(flag));
        }
      }
    }
  }

  FormatSpellListTitle("NO SPELLS AVAILABLE");

  // this flag should be set by now
  ASSERT(UseNewSpellLevelLayout());
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::FillMemorizeSpellListText
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::FillMemorizeSpellListText(CHARACTER &data)
{
	Clear(); 
  m_CharData=&data;
  m_flags=MEMORIZE_FORMAT_FLAG;  
  spellClasses = SetClassBitsFromCharClass(data.GetAdjClass());

  POSITION pos = data.spells.list.GetHeadPosition(); // access ordered list
  // should be at least one spell, 
  // and they should be sorted by now
  ASSERT( pos != NULL ); // could be legit if no spells exist or none in book

  while (pos != NULL)
  {
    int level = data.spells.list.GetAt(pos).level;
    ASSERT ((level>=1)&&(level<=MAX_SPELL_LEVEL));
    if (level <= 0) level = 1;
    SPELL_DATA *sdata = spellData.GetSpellData(data.spells.list.GetAt(pos).spell);  
    ASSERT( NumClassBits(sdata->ClassMask) == 1 );
    CurrSelect[level-1][FlagToIndex(GetSpellCastingClassForCharClass(sdata->ClassMask))] += data.spells.list.GetAt(pos).selected;

    Add((DWORD)pos);
    data.spells.list.GetNext(pos);
  }

  for (int i=0;i<NumClassFlags;i++)
  {
    WORD flag = (spellClasses & (0x0001 << i));
    if (flag)
    {
      int index = FlagToIndex(GetSpellCastingClassForCharClass(flag));

      MaxLevel[index] += CurrSpellLevel(flag,
                                       data.GetCurrentLevel(flag),
                                       data.GetPrimeSpellCastingScore(flag), 
                                       GetSpellCastingClassForCharClass(flag));

      for (int j=0;j<MaxLevel[index];j++)
      {
        MaxSelect[j][index] += 
          data.spells.spellLimits.MaxSpellsForLevel(flag, 
                                                    data.GetCurrentLevel(flag), 
                                                    j+1, 
                                                    data.GetPrimeSpellCastingScore(flag));
      }
    }
  }

  FormatSpellListTitle("NO SPELLS AVAILABLE");
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::FillTempleCastSpellListText
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::FillTempleCastSpellListText(CHARACTER &data, costFactorType type)
{
	Clear(); 
  m_CharData=&data;
  m_flags=TEMPLE_CAST_FORMAT_FLAG|COST_FLAG|LEVEL_FLAG;  
  CostFactor = type;

  spellClasses = SetClassBitsFromCharClass(data.GetAdjClass());  

  POSITION pos = data.spells.list.GetHeadPosition(); // access ordered list
  // should be at least one spell, 
  // and they should be sorted by now
  ASSERT( pos != NULL );

  while (pos != NULL)
  {
    if (data.spells.list.GetAt(pos).IsMemorized())
    {
      // don't really need CurrSelect[] info when casting a spell that is already in
      // the char's spell book, but just to be consistent...
      int level = data.spells.list.GetAt(pos).level;
      ASSERT ((level>=1)&&(level<=MAX_SPELL_LEVEL));
      if (level <= 0) level = 1;
      SPELL_DATA *sdata = spellData.GetSpellData(data.spells.list.GetAt(pos).spell); 
      ASSERT( NumClassBits(sdata->ClassMask) == 1 );
      CurrSelect[level-1][FlagToIndex(GetSpellCastingClassForCharClass(sdata->ClassMask))] += data.spells.list.GetAt(pos).selected;

      Add((DWORD)pos);
    }
    data.spells.list.GetNext(pos);
  }

  // don't really need this info when casting a spell that is already in
  // the char's spell book, but just to be consistent...
  for (int i=0;i<NumClassFlags;i++)
  {
    WORD flag = (spellClasses & (0x0001 << i));
    if (flag)
    {
      int index = FlagToIndex(GetSpellCastingClassForCharClass(flag));
      MaxLevel[index] += CurrSpellLevel(flag,data.GetCurrentLevel(flag),data.GetPrimeSpellCastingScore(flag), GetSpellCastingClassForCharClass(flag));
      for (int j=0;j<MaxLevel[index];j++)
        MaxSelect[j][index] += 
          data.spells.spellLimits.MaxSpellsForLevel(flag, data.GetCurrentLevel(flag), j+1, data.GetPrimeSpellCastingScore(flag));
    }
  }
  FormatSpellListTitle("NO SPELLS AVAILABLE");
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::FillCastSpellListText
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::FillCastSpellListText(CHARACTER &data)
{
	Clear(); 
  m_CharData=&data;
  m_flags=CAST_FORMAT_FLAG|LEVEL_FLAG;  

  spellClasses = SetClassBitsFromCharClass(data.GetAdjClass());  

  POSITION pos = data.spells.list.GetHeadPosition(); // access ordered list
  // should be at least one spell, 
  // and they should be sorted by now
  ASSERT( pos != NULL );

  while (pos != NULL)
  {
    if (data.spells.list.GetAt(pos).IsMemorized())
    {
      // don't really need CurrSelect[] info when casting a spell that is already in
      // the char's spell book, but just to be consistent...
      int level = data.spells.list.GetAt(pos).level;
      ASSERT ((level>=1)&&(level<=MAX_SPELL_LEVEL));
      if (level <= 0) level = 1;
      SPELL_DATA *sdata = spellData.GetSpellData(data.spells.list.GetAt(pos).spell); 
      ASSERT( NumClassBits(sdata->ClassMask) == 1 );
      CurrSelect[level-1][FlagToIndex(GetSpellCastingClassForCharClass(sdata->ClassMask))] += data.spells.list.GetAt(pos).selected;

      Add((DWORD)pos);
    }
    data.spells.list.GetNext(pos);
  }

  // don't really need this info when casting a spell that is already in
  // the char's spell book, but just to be consistent...
  for (int i=0;i<NumClassFlags;i++)
  {
    WORD flag = (spellClasses & (0x0001 << i));
    if (flag)
    {
      int index = FlagToIndex(GetSpellCastingClassForCharClass(flag));
      MaxLevel[index] += CurrSpellLevel(flag,data.GetCurrentLevel(flag),data.GetPrimeSpellCastingScore(flag), GetSpellCastingClassForCharClass(flag));
      for (int j=0;j<MaxLevel[index];j++)
        MaxSelect[j][index] += 
          data.spells.spellLimits.MaxSpellsForLevel(flag, data.GetCurrentLevel(flag), j+1, data.GetPrimeSpellCastingScore(flag));
    }
  }
  FormatSpellListTitle("NO SPELLS AVAILABLE");
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::Have
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TEXT_LIST::Have(DWORD index)
{
  for (int i=0;i<spells.GetSize(); i++)
  {
    if (spells[i] == index)
      return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::FillSpellEffectsListText
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::FillSpellEffectsListText(CHARACTER &data)
{
	Clear(); 
  m_CharData = &data;
  spellClasses = SetClassBitsFromCharClass(data.GetAdjClass());

  POSITION pos = data.spellEffects.GetHeadPosition();
  while (pos != NULL)
  {
    ACTIVE_SPELL *aspell = ActiveSpellData.Get(data.spellEffects.GetAt(pos).parent);
    if (aspell != NULL)
    {      
      SPELL_DATA *sdata = spellData.GetSpellData(aspell->spellKey);
      if (sdata != NULL)
      {
        if (!Have(sdata->key))
          Add(sdata->key);
      }
    }
    data.spellEffects.GetNext(pos);
  }

  for (int i=0;i<NumClassFlags;i++)
  {
    WORD flag = (spellClasses & (0x0001 << i));
    if (flag)
    {
      int index = FlagToIndex(GetSpellCastingClassForCharClass(flag));
      MaxLevel[index] += CurrSpellLevel(flag,data.GetCurrentLevel(flag),data.GetPrimeSpellCastingScore(flag), GetSpellCastingClassForCharClass(flag));
      for (int j=0;j<MaxLevel[index];j++)
        MaxSelect[j][index] += 
          data.spells.spellLimits.MaxSpellsForLevel(flag, data.GetCurrentLevel(flag), j+1, data.GetPrimeSpellCastingScore(flag));
    }
  }
  FormatSpellListTitle("NO SPELL EFFECTS");
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::FillSpellListText
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::FillSpellListText(CHARACTER &cdata, spellBookType &sbook, UINT flags, costFactorType type)
{
	Clear();
  m_CharData = &cdata;
  m_flags = flags;
  spellClasses = SetClassBitsFromCharClass(m_CharData->GetAdjClass());
  CostFactor = type;
  
  POSITION p = sbook.list.GetHeadPosition(); // access ordered list
  // should be at least one spell, 
  // and they should be sorted by now
  ASSERT( p != NULL );

  while (p != NULL)
  {
    if (flags & SHOW_SELECTED_FLAG)
    {
      // only display spells selected for use
      if (sbook.list.GetAt(p).selected == 0)
      {
        //spells.list.GetNext(p);
        //continue;
        goto bypass_add;
      }
    }
    else if (flags & SHOW_NOT_SELECTED_FLAG)
    {
      // only display spells not selected for use
      if (sbook.list.GetAt(p).selected > 0)
      {
        //spells.list.GetNext(p);
        //continue;
        goto bypass_add;
      }
    }

    if (flags & SHOW_MEMORIZED_FLAG)
    {
      // only display memorized spells 
      if (sbook.list.GetAt(p).memorized == 0)
      {
        //spells.list.GetNext(p);
        //continue;
        goto bypass_add;
      }
    }
    else if (flags & SHOW_NOT_MEMORIZED_FLAG)
    {
      // only display unmemorized spells
      if (sbook.list.GetAt(p).memorized > 0)
      {
        //spells.list.GetNext(p);
        //continue;
        goto bypass_add;
      }
    }

    Add(sbook.list.GetAt(p).spell);
    
bypass_add:
    sbook.list.GetNext(p);
  }

  // don't really need this info , but just to be consistent...
  for (int i=0;i<NumClassFlags;i++)
  {
    WORD flag = (spellClasses & (0x0001 << i));
    if (flag)
    {
      int index = FlagToIndex(GetSpellCastingClassForCharClass(flag));
      MaxLevel[index] += CurrSpellLevel(flag,m_CharData->GetCurrentLevel(flag),m_CharData->GetPrimeSpellCastingScore(flag), GetSpellCastingClassForCharClass(flag));
      for (int j=0;j<MaxLevel[index];j++)
        MaxSelect[j][index] += 
          m_CharData->spells.spellLimits.MaxSpellsForLevel(flag, m_CharData->GetCurrentLevel(flag), j+1, m_CharData->GetPrimeSpellCastingScore(flag));
    }
  }
  FormatSpellListTitle();
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::FormatSpellListTitle
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::FormatSpellListTitle(const char *EmptySpellTitle)
{
  const int width = 6;
  if (GetCount() <= 0)
  {
    if (EmptySpellTitle != NULL)
      title = EmptySpellTitle;
    else
      title = "NO SPELLS";
    return;
  }

	text[0] = '\0';

  if (UseMemorizeLayout())
  {
		sprintf(temp, "%-*s", width, "LEVEL");
		strcat(text, temp);
		sprintf(temp, "%-*s", width, "SEL");
		strcat(text, temp);
		sprintf(temp, "%-*s", width, "MEM");
		strcat(text, temp);
	  sprintf(temp, "%-*s", MAX_SPELL_NAME, "     SPELL");
	  strcat(text, temp);
  }
  else if (UseTempleCastLayout())
  {
		sprintf(temp, "%-*s", width, "LEVEL");
		strcat(text, temp);
		sprintf(temp,"%*s", width, "COST");
		strcat(text, temp);
	  sprintf(temp, "%-*s", MAX_SPELL_NAME, "     SPELL");
	  strcat(text, temp);
  }
  else if (UseCastLayout())
  {
		sprintf(temp, "%-*s", width, "LEVEL");
		strcat(text, temp);
		sprintf(temp, "%-*s", width, "MEM");
		strcat(text, temp);
	  sprintf(temp, "%-*s", MAX_SPELL_NAME, "     SPELL");
	  strcat(text, temp);
  }
  else if (UseNewSpellLevelLayout())
  {
		sprintf(temp, "%-*s", width, "LEVEL");
		strcat(text, temp);
		sprintf(temp, "%-*s", width, "SEL");
		strcat(text, temp);
	  sprintf(temp, "%-*s", MAX_SPELL_NAME, "     SPELL");
	  strcat(text, temp);
  }
  else
  {
	  if (UseLevel())
	  {
		  sprintf(temp, "%-*s", width, "LEVEL");
		  strcat(text, temp);
	  }
	  if (UseCost())
	  {
		  sprintf(temp,"%*s", width, "COST");
		  strcat(text, temp);
	  }
	  sprintf(temp, "%-*s", MAX_SPELL_NAME, "     SPELL");
	  strcat(text, temp);
  }
	title = text;
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::GetSpellMemorizeCount
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_TEXT_LIST::GetSpellMemorizeCount(int index)
{
  ASSERT( UsesSpellBookPos() );
  if (!UsesSpellBookPos()) return 0;
  return (m_CharData->spells.list.GetAt((POSITION)spells[index]).memorized);
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::DecSpellMemorizedCount
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::DecSpellMemorizedCount(int index)
{
  ASSERT( UsesSpellBookPos() );
  if (!UsesSpellBookPos()) return;
  int total = 
    m_CharData->spells.list.GetAt((POSITION)spells[index]).memorized - 1;
  total = max(0, total);
  m_CharData->spells.list.GetAt((POSITION)spells[index]).memorized = total;
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::ToggleSelected
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::ToggleSelected(int index)
{
  ASSERT(UsesSelectionArrays());
  if (spells.GetSize()==0) return;
  isSelected[index] = !isSelected[index];

  int lvl,sindex;
  GetSpellLevelAndSchoolIndex(index, lvl, sindex);
  //int level = GetSpellLevel(index);
  //ASSERT(level >= 1);
  //ASSERT( (m_school>=0)&&(m_school<=1) ); 

  if (isSelected[index])
    (CurrSelect[lvl-1][sindex])++;
  else
    (CurrSelect[lvl-1][sindex])--;
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::IsSelected
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TEXT_LIST::IsSelected(int index)
{
  if (spells.GetSize()==0) return FALSE;
  return isSelected[index];
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::CanSelect
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TEXT_LIST::CanSelect(int index)
{
  ASSERT( UseNewSpellLevelLayout() || UseMemorizeLayout() );
  if (spells.GetSize()==0) return FALSE;
  //ASSERT(m_SpellLevelFilter >= 1);
  //ASSERT( (m_school>=0)&&(m_school<=1) ); 
  int lvl,sindex;
  GetSpellLevelAndSchoolIndex(index, lvl, sindex);
  return (CurrSelect[lvl-1][sindex] < MaxSelect[lvl-1][sindex]);
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::CanSelectAnything
//
// PURPOSE: 
//
//*****************************************************************************
BOOL SPELL_TEXT_LIST::CanSelectAnything()
{
  ASSERT(UseNewSpellLevelLayout());
  if (spells.GetSize()==0) return FALSE;

  BOOL found = FALSE;
  for (int lvl=0;lvl<MAX_SPELL_LEVEL;lvl++)
  {
    for (int index=0;index<NumClassFlags;index++)
    {
      if (CurrSelect[lvl][index] < MaxSelect[lvl][index])
        found=TRUE;
    }
  }
  return found;
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::GetSpellSelectedCount
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_TEXT_LIST::GetSpellSelectedCount(int index)
{
  ASSERT( !UseNewSpellLevelLayout() );
  if (UseNewSpellLevelLayout()) return 0;
  return (m_CharData->spells.list.GetAt((POSITION)spells[index]).selected);
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::IncSpellSelectedCount
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::IncSpellSelectedCount(int index)
{
  ASSERT( !UseNewSpellLevelLayout() );
  if (UseNewSpellLevelLayout()) return;
  ASSERT(UseMemorizeLayout());
  int total = 
    m_CharData->spells.list.GetAt((POSITION)spells[index]).selected + 1;
  m_CharData->spells.list.GetAt((POSITION)spells[index]).selected = total;    

  int lvl,sindex;
  GetSpellLevelAndSchoolIndex(index, lvl, sindex);
  (CurrSelect[lvl-1][sindex])++;
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::DecSpellSelectedCount
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::DecSpellSelectedCount(int index)
{
  ASSERT( !UseNewSpellLevelLayout() );
  if (UseNewSpellLevelLayout()) return;
  ASSERT(UseMemorizeLayout());
  int total = 
    m_CharData->spells.list.GetAt((POSITION)spells[index]).selected - 1;
  total = max(0, total);
  m_CharData->spells.list.GetAt((POSITION)spells[index]).selected = total;

  int lvl,sindex;
  GetSpellLevelAndSchoolIndex(index, lvl, sindex);
  (CurrSelect[lvl-1][sindex])--;
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::GetSpellLevelAndSchoolIndex
//
// PURPOSE: 
//
//*****************************************************************************
void SPELL_TEXT_LIST::GetSpellLevelAndSchoolIndex(int index, int &lvl, int &sindex)
{
  lvl=1;
  sindex=0;
  int dbkey = GetSpellDatabaseKey(index);
  SPELL_DATA *sdata = spellData.GetSpellData(dbkey);
  ASSERT(sdata!=NULL);
  if (sdata == NULL) return;
  lvl = sdata->Level;
  sindex = FlagToIndex(GetSpellCastingClassForCharClass(sdata->ClassMask));
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::GetSpellDatabaseKey
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_TEXT_LIST::GetSpellDatabaseKey(int index)
{
  if (UseNewSpellLevelLayout()) return spells[index];
  return (m_CharData->spells.list.GetAt((POSITION)spells[index]).spell);
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::GetSpellBookKey
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_TEXT_LIST::GetSpellBookKey(int index)
{
  ASSERT( !UseNewSpellLevelLayout() );
  if (UseNewSpellLevelLayout()) return -1;
  return (m_CharData->spells.list.GetKeyAt((POSITION)spells[index]));
}

//*****************************************************************************
//    NAME: SPELL_TEXT_LIST::GetSpellSchool
//
// PURPOSE: 
//
//*****************************************************************************
int SPELL_TEXT_LIST::GetSpellSchool(int index)
{  
  int dbkey = GetSpellDatabaseKey(index);
  SPELL_DATA *sdata = spellData.GetSpellData(dbkey);
  ASSERT(sdata!=NULL);
  if (sdata == NULL) return ClericFlag;
  return (GetSpellCastingClassForCharClass(sdata->ClassMask));
}

//*****************************************************************************
//    NAME: *SPELL_TEXT_LIST::GetSpellLevelText
//
// PURPOSE: 
//
//*****************************************************************************
char *SPELL_TEXT_LIST::GetSpellLevelText(int index)
{
  if (spells.GetSize() == 0) return NULL;
  if (UseMemorizeLayout())
    sprintf(text, "%i", m_CharData->spells.list.GetAt((POSITION)spells[index]).level);
  else if (UseTempleCastLayout())
    sprintf(text, "%i", m_CharData->spells.list.GetAt((POSITION)spells[index]).level);
  else if (UseCastLayout())
    sprintf(text, "%i", m_CharData->spells.list.GetAt((POSITION)spells[index]).level);
  else if (UseNewSpellLevelLayout())
    sprintf(text, "%i", spellData.GetSpellLevel(spells[index]));
  else if (UseLevel())
    sprintf(text, "%i", spellData.GetSpellLevel(spells[index]));
  else
    return NULL;
  return text;
}
//*****************************************************************************
//    NAME: *SPELL_TEXT_LIST::GetSpellSelectedText
//
// PURPOSE: 
//
//*****************************************************************************
char *SPELL_TEXT_LIST::GetSpellSelectedText(int index)
{
  if (spells.GetSize() == 0) return NULL;
  if (UseMemorizeLayout())
    sprintf(text, "%i", m_CharData->spells.list.GetAt((POSITION)spells[index]).selected);
  else if (UseNewSpellLevelLayout())
  {
    //int lvl,sindex;
    //GetSpellLevelAndSchoolIndex(index, lvl, sindex);
    sprintf(text, "%s", (isSelected[index]?"Yes":"No"));
    //sprintf(text, "%s", (CurrSelect[lvl-1][sindex]?"Yes":"No"));
  }
  else
    return NULL;
  return text;
}
//*****************************************************************************
//    NAME: *SPELL_TEXT_LIST::GetSpellMemorizedText
//
// PURPOSE: 
//
//*****************************************************************************
char *SPELL_TEXT_LIST::GetSpellMemorizedText(int index)
{
  if (spells.GetSize() == 0) return NULL;
  if (UseMemorizeLayout())
    sprintf(text, "%i", m_CharData->spells.list.GetAt((POSITION)spells[index]).memorized);
  else if (UseCastLayout())
    sprintf(text, "%i", m_CharData->spells.list.GetAt((POSITION)spells[index]).memorized);
  else
    return NULL;
  return text;
}
//*****************************************************************************
//    NAME: *SPELL_TEXT_LIST::GetSpellCostText
//
// PURPOSE: 
//
//*****************************************************************************
char *SPELL_TEXT_LIST::GetSpellCostText(int index)
{
  if (spells.GetSize() == 0) return NULL;
  if (!UseCost())	return NULL;
  sprintf(text,"%i", ApplyCostFactor(CostFactor, spellData.GetSpellCastCost(spells[index])));
  return text;
}
//*****************************************************************************
//    NAME: *SPELL_TEXT_LIST::GetSpellNameText
//
// PURPOSE: 
//
//*****************************************************************************
char *SPELL_TEXT_LIST::GetSpellNameText(int index)
{
  if (spells.GetSize() == 0) return NULL;
  if (UseMemorizeLayout())
    sprintf(text,"%s", spellData.GetSpellName(m_CharData->spells.list.GetAt((POSITION)spells[index]).spell));
  else if (UseTempleCastLayout())
    sprintf(text,"%s", spellData.GetSpellName(m_CharData->spells.list.GetAt((POSITION)spells[index]).spell));
  else if (UseCastLayout())
    sprintf(text,"%s", spellData.GetSpellName(m_CharData->spells.list.GetAt((POSITION)spells[index]).spell));
  else if (UseNewSpellLevelLayout())
    sprintf(text,"%s", spellData.GetSpellName(spells[index]));
  else
    sprintf(text,"%s", spellData.GetSpellName(spells[index]));
  return text;
}
//*****************************************************************************
//    NAME: *SPELL_TEXT_LIST::GetSpellText
//
// PURPOSE: 
//
//*****************************************************************************
char *SPELL_TEXT_LIST::GetSpellText(int index)
{
  if (spells.GetSize() == 0) return NULL;

  const int width = 7;  
  temp[0] = '\0';
  text[0] = '\0';

  if (UseMemorizeLayout())
  {
    // level
    sprintf(temp, "%*i", width, m_CharData->spells.list.GetAt((POSITION)spells[index]).level);
		strcat(text, temp);
    // selected
    sprintf(temp, "%*i", width, m_CharData->spells.list.GetAt((POSITION)spells[index]).selected);
		strcat(text, temp);
    // memorized
    sprintf(temp, "%*i", width, m_CharData->spells.list.GetAt((POSITION)spells[index]).memorized);
		strcat(text, temp);
    // name
    sprintf(temp,"     %-.*s", MAX_SPELL_NAME, spellData.GetSpellName(m_CharData->spells.list.GetAt((POSITION)spells[index]).spell));
	  strcat(text, temp);
  }
  else if (UseTempleCastLayout())
  {
    // level
    sprintf(temp, "%*i", width, m_CharData->spells.list.GetAt((POSITION)spells[index]).level);
		strcat(text, temp);
    // cost
    DWORD cost = ApplyCostFactor(CostFactor, spellData.GetSpellCastCost(spells[index]));
    sprintf(temp,"%*i", width, cost);
		strcat(text, temp);
    // name
    sprintf(temp,"     %-.*s", MAX_SPELL_NAME, spellData.GetSpellName(m_CharData->spells.list.GetAt((POSITION)spells[index]).spell));
	  strcat(text, temp);
  }
  else if (UseCastLayout())
  {
    // level
    sprintf(temp, "%*i", width, m_CharData->spells.list.GetAt((POSITION)spells[index]).level);
		strcat(text, temp);
    // qty memorized
    sprintf(temp, "%*i", width, m_CharData->spells.list.GetAt((POSITION)spells[index]).memorized);
		strcat(text, temp);
    // name
    sprintf(temp,"     %-.*s", MAX_SPELL_NAME, spellData.GetSpellName(m_CharData->spells.list.GetAt((POSITION)spells[index]).spell));
	  strcat(text, temp);
  }
  else if (UseNewSpellLevelLayout())
  {
    //int lvl,sindex;
    //GetSpellLevelAndSchoolIndex(index, lvl, sindex);

    // level
    sprintf(temp, "%*i", width, spellData.GetSpellLevel(spells[index]));
		strcat(text, temp);
    // selected    
    //sprintf(temp, "%*s", width, (CurrSelect[lvl-1][sindex]?"Yes":"No"));
    sprintf(temp, "%*s", width, (isSelected[index]?"Yes":"No"));
		strcat(text, temp);
    // name
    sprintf(temp,"     %-.*s", MAX_SPELL_NAME, spellData.GetSpellName(spells[index]));
	  strcat(text, temp);
  }
  else
  {
    if (UseLevel())
	  {
      sprintf(temp, "%*i", width, spellData.GetSpellLevel(spells[index]));
		  strcat(text, temp);
	  }
	  if (UseCost())	
    {   
      DWORD cost = ApplyCostFactor(CostFactor, spellData.GetSpellCastCost(spells[index]));
      sprintf(temp,"%*i", width, cost);
		  strcat(text, temp);
	  }
    sprintf(temp,"     %-.*s", MAX_SPELL_NAME, spellData.GetSpellName(spells[index]));
	  strcat(text, temp);
  }

  return text;
}
#endif // UAFEngine