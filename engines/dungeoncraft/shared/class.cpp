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
//#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
#else
//#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/..\UAFWin\Dungeon.h"
#endif

#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/level.h"
#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/ConfigFile.h"
#ifdef UAFEngine
#include "dungeoncraft/shared/..\UAFWin\combatants.h"
#endif

#include "dungeoncraft/shared/GPDLOpCodes.h"
#include "dungeoncraft/shared/GPDLComp.h"
#include "dungeoncraft/shared/GPDLExec.h"
//#include "dungeoncraft/shared/RuntimeIf.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAR& ASL::Serialize(CAR& car)
{
  if (car.IsStoring())
    car << *this;
  else
    car >> *this;
  /*
  ASSERT(false); // You should not be here.
                 // You should use   car << attribute  and  car >> attribute
  MessageBox(NULL,"Program Error",NULL,MB_OK|MB_TOPMOST|MB_TASKMODAL);
  _exit(1);
  */
  return car;
}



RACE_DATA_TYPE       raceData;
BASE_CLASS_DATA_TYPE baseclassData;
CLASS_DATA_TYPE      classData;
ABILITY_DATA_TYPE    abilityData;
TRAIT_DATA_TYPE      traitData;
SPELLGROUP_DATA_TYPE spellgroupData;
//STRINGINDEX stringIndex;

void loadUADefaults(RACE_DATA_TYPE& data)
{
  data.LoadUADefaults();
}

void clearData(RACE_DATA_TYPE& data) 
{
  data.Clear();
}

int GetCount(RACE_DATA_TYPE& data)
{
  return data.GetCount();
}

BOOL saveData(RACE_DATA_TYPE& data)
{
//  BOOL success = FALSE;
  EditorStatusMsg("Saving race data...");

  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, RACE_DB_NAME);
  return saveData(data, fullPath);
}


int loadData(RACE_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open race db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  data.Clear();
  CAR car(&myFile, CArchive::load);  

  try
  {
    data.PreSerialize(car.IsStoring());
    if (data.Serialize(car)!=0)
    {
      data.Clear();
      car.Close();
      return 0;
    };
    data.PostSerialize(car.IsStoring());
  }
  catch (...)
  {
      WriteDebugString("Unable to load race data file %s\n", car.GetFile()->GetFileName());
      data.Clear();
  }
  
  car.Close();

  if (data.GetCount() == 0)
     WriteDebugString("No races in race db file\n");

  return data.GetCount();
}



BOOL saveData(RACE_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open race db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  CAR car(&myFile, CArchive::store);
  data.PreSerialize(car.IsStoring());
  data.Serialize(car);
  data.PostSerialize(car.IsStoring());
  car.Close();
  return TRUE;
}

int GetDataSize(RACE_DATA_TYPE& data)
{
  return data.GetDataSize();
}


int GetCount(ABILITY_DATA_TYPE& data)
{ 
  return data.GetCount();
}

int GetDataSize(ABILITY_DATA_TYPE& data)
{
  return data.GetDataSize();
}

BOOL saveData(ABILITY_DATA_TYPE& data)
{
//  BOOL success = FALSE;
  EditorStatusMsg("Saving ability data...");

  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, ABILITY_DB_NAME);
  return saveData(data, fullPath);
}

BOOL saveData(ABILITY_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open ability db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  CAR car(&myFile, CArchive::store);
  data.PreSerialize(car.IsStoring());
  data.Serialize(car);
  data.PostSerialize(car.IsStoring());
  car.Close();
  return TRUE;
}

void clearData(ABILITY_DATA_TYPE& data) 
{
  data.Clear();
}

void loadUADefaults(ABILITY_DATA_TYPE& data)
{
  data.LoadUADefaults();
}

int loadData(ABILITY_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open ability db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  data.Clear();
  CAR car(&myFile, CArchive::load);  

  try
  {
    data.PreSerialize(car.IsStoring());
    if (data.Serialize(car)!=0)
    {
      data.Clear();
      car.Close();
      return 0;
    };
    data.PostSerialize(car.IsStoring());
  }
  catch (...)
  {
      WriteDebugString("Unable to load ability data file %s\n", car.GetFile()->GetFileName());
      data.Clear();
  }
  
  car.Close();

  if (data.GetCount() == 0)
     WriteDebugString("No abilities in ability db file\n");

  return data.GetCount();
}

int GetCount(BASE_CLASS_DATA_TYPE& data)
{ 
  return data.GetCount();
}

void loadUADefaults(BASE_CLASS_DATA_TYPE& data)
{
  data.LoadUADefaults();
}

int GetDataSize(BASE_CLASS_DATA_TYPE& data) {
  return data.GetDataSize();
}


BOOL saveData(BASE_CLASS_DATA_TYPE& data)
{
//  BOOL success = FALSE;
  EditorStatusMsg("Saving baseclass data...");

  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, BASE_CLASS_DB_NAME);
  return saveData(data, fullPath);
}

BOOL saveData(BASE_CLASS_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open baseclass db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  CAR car(&myFile, CArchive::store);
  data.PreSerialize(car.IsStoring());
  data.Serialize(car);
  data.PostSerialize(car.IsStoring());
  car.Close();
  return TRUE;
}

void clearData(BASE_CLASS_DATA_TYPE& data)
{
  data.Clear();
}

int loadData(BASE_CLASS_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open baseclass db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  data.Clear();
  CAR car(&myFile, CArchive::load);  

  try
  {
    data.PreSerialize(car.IsStoring());
    if (data.Serialize(car)!=0)
    {
      data.Clear();
      car.Close();
      return 0;
    };
    data.PostSerialize(car.IsStoring());
  }
  catch (...)
  {
      WriteDebugString("Unable to load baseclass data file %s\n", car.GetFile()->GetFileName());
      data.Clear();
  }
  
  car.Close();

  if (data.GetCount() == 0)
     WriteDebugString("No base classes in baseclass db file\n");

  return data.GetCount();
}

int GetDataSize(CLASS_DATA_TYPE& data)
{
  return data.GetDataSize();
}

int GetCount(CLASS_DATA_TYPE& data)
{ 
  return data.GetCount();
}

void clearData(CLASS_DATA_TYPE& data)
{
  data.Clear();
}

void loadUADefaults(CLASS_DATA_TYPE& data)
{
  data.LoadUADefaults();
}

BOOL saveData(CLASS_DATA_TYPE& data)
{
//  BOOL success = FALSE;
  EditorStatusMsg("Saving class data...");

  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, CLASS_DB_NAME);
  return saveData(data, fullPath);
}

BOOL saveData(CLASS_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open class db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  CAR car(&myFile, CArchive::store);
  data.PreSerialize(car.IsStoring());
  data.Serialize(car);
  data.PostSerialize(car.IsStoring());
  car.Close();
  return TRUE;
}


int loadData(CLASS_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open class db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  data.Clear();
  CAR car(&myFile, CArchive::load);  

  try
  {
    data.PreSerialize(car.IsStoring());
    if (data.Serialize(car)!=0)
    {
      data.Clear();
      car.Close();
      return 0;
    };
    data.PostSerialize(car.IsStoring());
  }
  catch (...)
  {
      WriteDebugString("Unable to load class data file %s\n", car.GetFile()->GetFileName());
      data.Clear();
  }
  
  car.Close();

  if (data.GetCount() == 0)
     WriteDebugString("No classes in class db file\n");

  return data.GetCount();
}


int GetCount(TRAIT_DATA_TYPE& data)
{
  return data.GetCount();
}



BOOL saveData(TRAIT_DATA_TYPE& data)
{
//  BOOL success = FALSE;
  EditorStatusMsg("Saving trait data...");

  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, TRAIT_DB_NAME);
  return saveData(data, fullPath);
}

BOOL saveData(TRAIT_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open trait db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  CAR car(&myFile, CArchive::store);
  data.PreSerialize(car.IsStoring());
  data.Serialize(car);
  data.PostSerialize(car.IsStoring());
  car.Close();
  return TRUE;
}

void clearData(TRAIT_DATA_TYPE& data) 
{
  data.Clear();
}

void loadUADefaults(TRAIT_DATA_TYPE& data)
{
  data.LoadUADefaults();
}

int loadData(TRAIT_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open trait db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  data.Clear();
  CAR car(&myFile, CArchive::load);  

  try
  {
    data.PreSerialize(car.IsStoring());
    if (data.Serialize(car)!=0)
    {
      data.Clear();
      car.Close();
      return 0;
    };
    data.PostSerialize(car.IsStoring());
  }
  catch (...)
  {
      WriteDebugString("Unable to load trait data file %s\n", car.GetFile()->GetFileName());
      data.Clear();
  }
  
  car.Close();

  if (data.GetCount() == 0)
     WriteDebugString("No traits in trait db file\n");

  return data.GetCount();
}

int GetCount(SPELLGROUP_DATA_TYPE& data)
{
  return data.GetCount();
}


void loadUADefaults(SPELLGROUP_DATA_TYPE& data)
{
  data.LoadUADefaults();
}

int GetDataSize(SPELLGROUP_DATA_TYPE& data)
{
  return data.GetDataSize();
}



BOOL saveData(SPELLGROUP_DATA_TYPE& data)
{
//  BOOL success = FALSE;
  EditorStatusMsg("Saving spellgroup data...");

  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, SPELLGROUP_DB_NAME);
  return saveData(data, fullPath);
}

BOOL saveData(SPELLGROUP_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open spellgroup db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  CAR car(&myFile, CArchive::store);
  data.PreSerialize(car.IsStoring());
  data.Serialize(car);
  data.PostSerialize(car.IsStoring());
  car.Close();
  return TRUE;
}

void clearData(SPELLGROUP_DATA_TYPE& data) 
{
  data.Clear();
}

int loadData(SPELLGROUP_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open spellgroup db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  data.Clear();
  CAR car(&myFile, CArchive::load);  

  try
  {
    data.PreSerialize(car.IsStoring());
    if (data.Serialize(car)!=0)
    {
      data.Clear();
      car.Close();
      return 0;
    };
    data.PostSerialize(car.IsStoring());
  }
  catch (...)
  {
      WriteDebugString("Unable to load spellgroup data file %s\n", car.GetFile()->GetFileName());
      data.Clear();
  }
  
  car.Close();

  if (data.GetCount() == 0)
     WriteDebugString("No spell groups in spellgroup db file\n");

  return data.GetCount();
}

//////////////////////////////////////////////////////////////////////////////////
//
//  Interface functions above.
//
//  Database member functions below.
//
///////////////////////////////////////////////////////////////////////////////////

void PerhapsPrintCompileError(CString& msg) 
{
  static const CString err("**COMPILE ERROR** ");
  if (LogCompileErrors>0) WriteDebugString(err + msg);
  else if (LogCompileErrors==0) WriteDebugString (err + "More compile errors\n");
  LogCompileErrors--;
}


GENERIC_REFERENCE::GENERIC_REFERENCE(void)
{
  m_refKey=0;
  m_refType=UNKNOWN_DB;
}

GENERIC_REFERENCE::~GENERIC_REFERENCE(void)
{
}

void GENERIC_REFERENCE::Clear(void)
{
  m_refName="";
  m_refKey=0xffffffff;
  m_refType=UNKNOWN_DB;
}

GENERIC_REFERENCE& GENERIC_REFERENCE::operator =(const GENERIC_REFERENCE& src)
{
  if (this == &src) return *this;
  m_refKey=src.m_refKey;
  m_refName=src.m_refName;
  m_refType=src.m_refType;
  return *this;
}

GENERIC_REFERENCE& GENERIC_REFERENCE::operator = (const CString& name)
{
  Clear();
  m_refName=name;
  return *this;
}

BOOL GENERIC_REFERENCE::operator ==(const GENERIC_REFERENCE& src) const
{
  if (this == &src) return TRUE;
  if (m_refKey!=src.m_refKey) return FALSE;
  if (m_refName!=src.m_refName) return FALSE;
  if (m_refType!=src.m_refType) return FALSE;
  return TRUE;
}

int GENERIC_REFERENCE::GetDataSize(void)
{
  int size=sizeof (m_refType);
  size += sizeof (m_refName);
  size += sizeof (m_refKey);
  return size;
}

BOOL GENERIC_REFERENCE::LookupRefKey(void)
{
  if (m_refName == "__const__")
  {
    m_refType=LITERAL_DB;
    return TRUE;
  }

  // Search the databases one at a time.
  if (m_refName.CompareNoCase("level")==0)
  {
    m_refType=LEVEL_DB;
    m_refKey=0xffffffff;
    return TRUE;
  };
  if (m_refName.CompareNoCase("race")==0)
  {
    m_refType=RACE_DB;
    m_refKey=raceUnknown;
    return TRUE;
  };
  if (m_refName.CompareNoCase("class")==0)
  {
    m_refType=BASECLASS_DB;
    m_refKey=classUnknown;
    return TRUE;
  };
  
  if (m_refName.CompareNoCase("male")==0)
  {
    m_refType=GENDER_DB;
    m_refKey=Male;
    return TRUE;
  };

  if (m_refName.CompareNoCase("female")==0)
  {
    m_refType=GENDER_DB;
    m_refKey=Female;
    return TRUE;
  };

  if ((m_refKey=classData.FindName(m_refName))!=classUnknown)
  {
    m_refType=CLASS_DB;
    return TRUE;
  };
  if ((m_refKey=raceData.FindName(m_refName))!=raceUnknown)
  {
    m_refType=RACE_DB;
    return TRUE;
  };
  if ((m_refKey=baseclassData.FindName(m_refName))!=baseclassUnknown)
  {
    m_refType=BASECLASS_DB;
    return TRUE;
  };
  if ((m_refKey=abilityData.FindName(m_refName))!=abilityUnknown)
  {
    m_refType=ABILITY_DB;
    return TRUE;
  };
  if ((m_refKey=spellgroupData.FindName(m_refName))!=spellgroupUnknown)
  {
    m_refType=SPELLGROUP_DB;
    return TRUE;
  };
  if ((m_refKey=traitData.FindName(m_refName))!=traitUnknown)
  {
    m_refType=TRAIT_DB;
    return TRUE;
  };
  m_refType=UNKNOWN_DB;
  m_refKey=0xffffffff;
  return FALSE;
}

// some of these db types assume that the char context
// has been set. 
BOOL GENERIC_REFERENCE::LookupReferenceData(double &result)
{ 
  // This is only important if you need to retrieve
  // CHARACTER or COMBATANT information. Temp characters
  // used during character creation do not have a valid
  // source (neither NPC/Monster db or party member).
  //
  //ASSERT(ctx.IsValidSrc());

  switch (m_refType)
  {
  case LITERAL_DB: 
    {
      int tmp = (int)m_refKey; // convert to integer
      result=tmp; 
      return TRUE;
    }
  case LEVEL_DB:
    {
      ActorType ctx = GetCharContext();
      result=ctx.Level;
      return TRUE;
    }
  case GENDER_DB:
    {
      ActorType ctx = GetCharContext();
      CHARACTER *pChar = GetCurrentlyActiveContext(&ctx);
      if (pChar==NULL) { result=0.0; return FALSE; }
      result=(m_refKey==(DWORD)pChar->GetAdjGender());
      return TRUE;
    }

  case RACE_DB:    
    {
      ActorType ctx = GetCharContext();
      if (m_refKey==raceUnknown)
        result = ctx.Race;// evaluating 'race' keyword, return race key for char context
      else      
        result = (m_refKey==ctx.Race);// return 1 if char race matches specified, else 0
      return TRUE;
    }

  case CLASS_DB:
  case BASECLASS_DB:
    {
      ActorType ctx = GetCharContext();
      result = ctx.Class;
      if (m_refKey==classUnknown)        
        result = ctx.Class;// evaluating 'class' keyword, return class key for char context
      else      
        result = doClassesOverlap((classType)m_refKey,(classType)ctx.Class);// return 1 if char class matches specified, else 0
      return TRUE;
    }

  case ABILITY_DB:
    {
      ActorType ctx = GetCharContext();
      CHARACTER *pChar = GetCurrentlyActiveContext(&ctx);
      if (pChar==NULL) { result=0.0; return FALSE; }
      result = pChar->GetAbility(m_refKey);
      return TRUE;
    }

  case TRAIT_DB:
    {
      //ActorType ctx = GetCharContext();
      //result = traitData.
      result = 0;
    }
    // fallthrough until implemented

  case SPELLGROUP_DB:
    {
      //ActorType ctx = GetCharContext();
      //result = spellgroupData.
      result = 0;
    }
    // fallthrough until implemented
  }
  return FALSE;
}
void GENERIC_REFERENCE::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    if (!LookupRefKey())
    {
      CString msg;
      msg.Format("Cannot locate GENERIC_REFERENCE database reference = \"%s\"\n",m_refName);
      PerhapsPrintCompileError(msg);
    }
  };
}

int GENERIC_REFERENCE::Serialize(CAR& car)
{
  if (car.IsStoring())
  {
    if (m_refName=="") m_refName="*";
    car << m_refName;
    if (m_refName=="*") m_refName="";
    car << m_refType;
    car << m_refKey;
  }
  else
  {
    car >> m_refName; if (m_refName=="*") m_refName="";
    car >> m_refType;
    car >> m_refKey;
  };
  return 0;
}
int GENERIC_REFERENCE::Serialize(CArchive& car)
{
  if (car.IsStoring())
  {
    if (m_refName=="") m_refName="*";
    car << m_refName;
    if (m_refName=="*") m_refName="";
    car << m_refType;
    car << m_refKey;
  }
  else
  {
    car >> m_refName; if (m_refName=="*") m_refName="";
    car >> m_refType;
    car >> m_refKey;
  };
  return 0;
}

BOOL GENERIC_REFERENCE::Evaluate(double &result)
{
  if (m_refType==UNKNOWN_DB)
  {
    LookupRefKey();
    if (m_refType==UNKNOWN_DB)
      return FALSE;
  }
  return (LookupReferenceData(result));
}


BASECLASS_REFERENCE::BASECLASS_REFERENCE(void)
{
  m_baseclassKey=baseclassUnknown;
}

BASECLASS_REFERENCE::~BASECLASS_REFERENCE(void)
{
}

BASECLASS_REFERENCE& BASECLASS_REFERENCE::operator = (const BASECLASS_REFERENCE& src)
{
  if (this == &src) return *this;
  m_baseclassKey=src.m_baseclassKey;
  m_baseclassName=src.m_baseclassName;
  return *this;
}

int BASECLASS_REFERENCE::Serialize(CAR& car)
{
  if (car.IsStoring())
  {
    if (m_baseclassName=="") m_baseclassName="*";
    car << m_baseclassName;
    if (m_baseclassName=="*") m_baseclassName="";
    car << m_baseclassKey;
  }
  else
  {
    car >> m_baseclassName; if (m_baseclassName=="*") m_baseclassName="";
    car >> m_baseclassKey;
  };
  return 0;
}

void BASECLASS_REFERENCE::PreSerialize(BOOL IsStoring)
{
  if (!IsStoring) return;
//  stringIndex.AddString(m_baseclassName);
  // Convert the m_baseclassName to m_baseclassKey.  Print error message.
  m_baseclassKey=baseclassData.FindName(m_baseclassName);
  if (m_baseclassKey==baseclassUnknown)
  {
    CString msg;
    msg.Format("Required BaseClass references unknown baseclass = \"%s\"\n",m_baseclassName);
    PerhapsPrintCompileError(msg);
  };
}



int BASECLASS_REFERENCE::GetDataSize(void)
{
  int size=sizeof (*this);
  size += m_baseclassName.GetLength();
  return size;
}

ABILITY_REFERENCE::ABILITY_REFERENCE(void)
{
  m_abilityKey=abilityUnknown;
  m_abilityName="";
}

ABILITY_REFERENCE::~ABILITY_REFERENCE(void)
{
}

void ABILITY_REFERENCE::Clear(void) 
{
  m_abilityName="";
  m_abilityKey=abilityUnknown;
}

int ABILITY_REFERENCE::GetDataSize(void)
{
  int size=sizeof (m_abilityKey);
  size += sizeof (m_abilityName);
  size += m_abilityName.GetLength();
  return size;
}

int ABILITY_REFERENCE::Serialize(CAR& car)
{
  if (car.IsStoring())
  {
    if (m_abilityName=="") m_abilityName="*";
    car << m_abilityName;
    if (m_abilityName=="*") m_abilityName="";
    car << m_abilityKey;
  }
  else
  {
    car >> m_abilityName; if (m_abilityName=="*") m_abilityName="";
    car >> m_abilityKey;
  };
  return 0;
}

ABILITY_REFERENCE& ABILITY_REFERENCE::operator = (const ABILITY_REFERENCE& src)
{
  if (this == &src) return *this;
  m_abilityKey=src.m_abilityKey;
  m_abilityName=src.m_abilityName;
  return *this;
}

void ABILITY_REFERENCE::PreSerialize(BOOL IsStoring)
{
  if (!IsStoring) return;
//  stringIndex.AddString(m_abilityName);
  // Convert the m_abilityName to m_abilityKey.  Print error message.
  m_abilityKey=abilityData.FindName(m_abilityName);
  if (m_abilityKey==abilityUnknown)
  {
    CString msg;
    msg.Format("Required Ability references unknown ability = \"%s\"\n",m_abilityName);
    PerhapsPrintCompileError(msg);
  };
}

SPELL_REFERENCE::SPELL_REFERENCE(void)
{
  m_spellKey=spellUnknown;
}

SPELL_REFERENCE::~SPELL_REFERENCE(void)
{
}

void SPELL_REFERENCE::Clear(void) 
{
  m_spellName="";
  m_spellKey=spellUnknown;
}

int SPELL_REFERENCE::GetDataSize(void)
{
  int size=sizeof (m_spellKey);
  size += sizeof (m_spellName);
  size += m_spellName.GetLength();
  return size;
}


int SPELL_REFERENCE::Serialize(CAR& car)
{
  if (car.IsStoring())
  {
    if (m_spellName=="") m_spellName="*";
    car << m_spellName;
    if (m_spellName=="*") m_spellName="";
    car << m_spellKey;
  }
  else
  {
    car >> m_spellName; if (m_spellName=="*") m_spellName="";
    car >> m_spellKey;
  };
  return 0;
}

SPELL_REFERENCE& SPELL_REFERENCE::operator = (const SPELL_REFERENCE& src)
{
  if (this == &src) return *this;
  m_spellKey=src.m_spellKey;
  m_spellName=src.m_spellName;
  return *this;
}

void SPELL_REFERENCE::PreSerialize(BOOL IsStoring)
{
  if (!IsStoring) return;
//  stringIndex.AddString(m_spellName);
  // Convert the m_spellName to m_spellKey.  Print error message.
  m_spellKey=spellData.FindName(m_spellName);
  if (m_spellKey==spellUnknown)
  {
    CString msg;
    msg.Format("Reference to unknown spell = \"%s\"\n",m_spellName);
    PerhapsPrintCompileError(msg);
  };
}


SPELLGROUP_REFERENCE::SPELLGROUP_REFERENCE(void)
{
  m_spellgroupKey=spellgroupUnknown;
}

SPELLGROUP_REFERENCE::~SPELLGROUP_REFERENCE(void)
{
}

void SPELLGROUP_REFERENCE::Clear(void) 
{
  m_spellgroupName="";
  m_spellgroupKey=spellgroupUnknown;
}

int SPELLGROUP_REFERENCE::GetDataSize(void)
{
  int size=sizeof (m_spellgroupKey);
  size += sizeof (m_spellgroupName);
  size += m_spellgroupName.GetLength();
  return size;
}



void SPELLGROUP_REFERENCE::Serialize(CAR& car)
{
  if (car.IsStoring())
  {
    if (m_spellgroupName=="") m_spellgroupName="*";
    car << m_spellgroupName;
    if (m_spellgroupName=="*") m_spellgroupName="";
    car << m_spellgroupKey;
  }
  else
  {
    car >> m_spellgroupName; if (m_spellgroupName=="*") m_spellgroupName="";
    car >> m_spellgroupKey;
  };
}

SPELLGROUP_REFERENCE& SPELLGROUP_REFERENCE::operator = (const SPELLGROUP_REFERENCE& src)
{
  if (this == &src) return *this;
  m_spellgroupKey=src.m_spellgroupKey;
  m_spellgroupName=src.m_spellgroupName;
  return *this;
}

void SPELLGROUP_REFERENCE::PreSerialize(BOOL IsStoring)
{
  if (!IsStoring) return;
//  stringIndex.AddString(m_spellgroupName);
  // Convert the m_spellgroupName to m_spellgroupKey.  Print error message.
  m_spellgroupKey=spellgroupData.FindName(m_spellgroupName);
  if (m_spellgroupKey==spellUnknown)
  {
    CString msg;
    msg.Format("Reference to unknown Spell Group = \"%s\"\n",m_spellgroupName);
    PerhapsPrintCompileError(msg);
  };
}


RACE_REFERENCE::RACE_REFERENCE(void)
{
  m_raceKey=raceUnknown;
}

RACE_REFERENCE::~RACE_REFERENCE(void)
{
}

void RACE_REFERENCE::Clear(void) 
{
  m_raceName="";
  m_raceKey=raceUnknown;
}

int RACE_REFERENCE::Serialize(CAR& car)
{
  if (car.IsStoring())
  {
    if (m_raceName=="") m_raceName="*";
    car << m_raceName;
    if (m_raceName=="*") m_raceName="";
    car << m_raceKey;
  }
  else
  {
    car >> m_raceName; if (m_raceName=="*") m_raceName="";
    int temp;
    car >> temp;
    m_raceKey = (raceType)temp; // Cast until raceType conversion complete
  };
  return 0;
}

int RACE_REFERENCE::GetDataSize(void)
{
  int size=sizeof (*this);
  size += m_raceName.GetLength();
  return size;
}



RACE_REFERENCE& RACE_REFERENCE::operator = (const RACE_REFERENCE& src)
{
  if (this == &src) return *this;
  m_raceKey=src.m_raceKey;
  m_raceName=src.m_raceName;
  return *this;
}

void RACE_REFERENCE::PreSerialize(BOOL IsStoring)
{
  if (!IsStoring) return;
//  stringIndex.AddString(m_raceName);
  // Convert the m_raceName to m_raceKey.  Print error message.
  m_raceKey=raceData.FindName(m_raceName);
  if (m_raceKey==raceUnknown)
  {
    CString msg;
    msg.Format("Race reference to unknown race = \"%s\"\n",m_raceName);
    PerhapsPrintCompileError(msg);
  };
}


ADJUSTMENT::ADJUSTMENT(void)
{
  Clear();
}

ADJUSTMENT::ADJUSTMENT(const ADJUSTMENT& src)
{
  *this=src;
}

ADJUSTMENT::ADJUSTMENT(char value, CString name)
{
  Clear();
  m_parameter[0] = value;
  m_reference = name;
  m_operator[0]=AJO_gear;
}

ADJUSTMENT::~ADJUSTMENT(void)
{
}

BOOL ADJUSTMENT::Add(int param, char oper)
{
  int index=-1;
  for (int i=0;i<MAX_ADJ;i++)
  {
    if (m_operator[i]==AJO_none)
    {
      index=i;
      break;
    }
  }

  if (index >= 0)
  {
    m_parameter[index] = param;
    m_operator[index]  = oper;
    return TRUE;
  }
  return FALSE;
}

ADJUSTMENT& ADJUSTMENT::operator=(const ADJUSTMENT& src)
{
  if (this==&src) return *this;
  for (int i=0;i<MAX_ADJ;i++)
  {
    m_parameter[i] =src.m_parameter[i];
    m_operator[i]  =src.m_operator[i];
  }
  m_reference    =src.m_reference;
  return *this;
}

BOOL ADJUSTMENT::operator ==(const ADJUSTMENT& src) const
{
  if (this==&src) return TRUE;
  for (int i=0;i<MAX_ADJ;i++)
  {
    if (m_parameter[i] != src.m_parameter[i]) return FALSE;
    if (m_operator[i]  != src.m_operator[i]) return FALSE;
  }
  if ((m_reference   == src.m_reference)==FALSE) return FALSE;
  return TRUE;
}

void ADJUSTMENT::Clear(void)
{
  for (int i=0;i<MAX_ADJ;i++)
  {
    m_parameter[i]=0;
    m_operator[i]=AJO_none;
  }
  m_reference.Clear();
}

CString ADJUSTMENT::EncodeOperator(int op)
{
  switch (op)
  {
  case AJO_none: return " ";
  case AJO_gear:     return " * ";
  case AJO_plus:     return " + ";
  case AJO_equality: return " == ";
  case AJO_greater:  return " > ";
  case AJO_greaterEqual: return " >= ";
  case AJO_andBit:       return " & ";
  case AJO_slash:        return " / ";
  case AJO_andLogical:   return " && ";
  case AJO_continue:     return "";
  case AJO_minus:        return " - ";
  case AJO_less:         return " < ";
  case AJO_lessEqual:    return " <= ";
  case AJO_orBit:        return " | ";
  case AJO_orLogical:    return " || ";
  }
  return "";
}

static double ApplyAdjOperator(double p1, int op, double p2)
{
  switch (op)
  {
  case AJO_none: return p1;
  case AJO_gear:     return (p1*p2);
  case AJO_plus:     return (p1+p2);
  case AJO_equality: return (!(p1!=p2));
  case AJO_greater:  return (p1>p2);
  case AJO_greaterEqual: return (p1>=p2);
  case AJO_andBit:       return ((DWORD)p1&(DWORD)p2);
  case AJO_slash:        if (p2==0.0) return 0.0; else return (p1/p2);
  case AJO_andLogical:   return (p1&&p2);
  case AJO_continue:     return p1;
  case AJO_minus:        return (p1-p2);
  case AJO_less:         return (p1<p2);
  case AJO_lessEqual:    return (p1<=p2);
  case AJO_orBit:        return ((DWORD)p1|(DWORD)p2);
  case AJO_orLogical:    return ((DWORD)p1||(DWORD)p2);
  }
  return p1;
}


CString ADJUSTMENT::EncodeText()
{
  CString tmp("");
  CString otmp;

  tmp += m_reference.m_refName;
  
  for (int i=0;i<MAX_ADJ;i++)
  {
    if ((m_operator[i]!=AJO_none)&&(m_operator[i]!=AJO_continue))
    {
      tmp += EncodeOperator(m_operator[i]);
      otmp.Format("%i", abs(m_parameter[i]));
      tmp += otmp;
    }
  }
  return tmp;
}

double ADJUSTMENT::ApplyOperatorsToInput(double &input)
{
  for (int i=0;i<MAX_ADJ;i++)
    input = ApplyAdjOperator(input, m_operator[i], m_parameter[i]); 
  return input;
}

BOOL ADJUSTMENT::HasTwoOperands()
{  
  if (m_reference.m_refName.IsEmpty()) return FALSE;

  // look for at least one operator, and 
  // make sure the AJO_continue operator isn't
  // present.
  char count=0;
  for (int i=0;i<MAX_ADJ;i++)
  {
    if (m_operator[i]!=AJO_none)
    {
      if (m_operator[i]==AJO_continue) 
        return FALSE;

      count++;
    }    
  }
  return (count>0);
}

// Unary evaluate. There will be no parameters,
// only a reference and one operator which is applied
// to ref and input.
BOOL ADJUSTMENT::Evaluate(double &result)
{
  double refValue=0.0;
  if (m_reference.Evaluate(refValue))
  {
    // reference has been evaluated,
    // now apply the operators to it.
    result = ApplyAdjOperator(refValue, m_operator[0], result); 
    return TRUE;
  }
  return FALSE;
}

// Binary evaluate. There will be a reference and
// at least one parameter/operator pair. 
BOOL ADJUSTMENT::EvaluateBinary(double &result)
{
  double refValue=0.0;
  if (m_reference.Evaluate(refValue))
  {
    // reference has been evaluated,
    // now apply the operators to it.
    ApplyOperatorsToInput(refValue);
    result = refValue;
    return TRUE;
  }
  return FALSE;
}

int ADJUSTMENT::GetDataSize(void)
{
  int size = sizeof (m_parameter);
  size += sizeof (m_operator);
  size += m_reference.GetDataSize();
  return size;
}

void ADJUSTMENT::PreSerialize(BOOL IsStoring)
{
  m_reference.PreSerialize(IsStoring);
}

int ADJUSTMENT::Serialize(CAR& car)
{
  int result=0;
  if (car.IsStoring())
  {
    car << m_parameter[0] << m_parameter[1] << m_parameter[2];
    car << m_operator[0] << m_operator[1] << m_operator[2];
    m_reference.Serialize(car);
  }
  else 
  {
    car >> m_parameter[0] >> m_parameter[1] >> m_parameter[2];
    car >> m_operator[0] >> m_operator[1] >> m_operator[2];
    result=m_reference.Serialize(car);
  };
  return result;
}
int ADJUSTMENT::Serialize(CArchive& car)
{
  int result=0;
  if (car.IsStoring())
  {
    car << m_parameter[0] << m_parameter[1] << m_parameter[2];
    car << m_operator[0] << m_operator[1] << m_operator[2];
    m_reference.Serialize(car);
  }
  else 
  {
    car >> m_parameter[0] >> m_parameter[1] >> m_parameter[2];
    car >> m_operator[0] >> m_operator[1] >> m_operator[2];
    result=m_reference.Serialize(car);
  };
  return result;
}


//***************************************************************************
inline bool whitespace(const char c)
{
  return ((c==' ') || (c=='\t'));
}

int GetInteger (int& column,
                const CString& value,
                int& result,
                CString& errorMsg)
{
  // Skips over leading and trailing whitespace
  // The string must not be all whitespace.
  int origColumn = column;
  int len;
  int sign=1;
  len=value.GetLength();
  result=0;
  while (column < len)
  {
    if (whitespace(value[column]))
    {
      column++;
      continue;
    };
    break;
  };

  // We have found first character, which
  // might be a +/- as in +3 or '- 4'
  if (column < len)
  {
    if (value[column]=='-')
    {
      sign=-1;
      // skip to next token
      column++;
      while ((column < len) && whitespace(value[column])) column++;
    }
    else if (value[column]=='+')    
    {
      sign=1;
      // skip to next token
      column++;
      while ((column < len) && whitespace(value[column])) column++;
    }
  }

  if (column>=len)
  {
    errorMsg="Looking for integer.  Found only blanks";
    column=origColumn;
    return FALSE;
  };
  if ((value[column]<'0') || (value[column]>'9'))
    // There must be at least one digit!
  {
    errorMsg="Non-decimal digit in integer";
    column=origColumn;
    return FALSE;
  };
  while (column < len) 
  {
    if (whitespace(value[column])) break;
    if ((value[column]<'0') || (value[column]>'9')) break;
    result=result*10+value[column]-'0';
    column++;
  };
  // We either found the end of line or a whitespace or a non-digit.
  // If it is whitespace, we promised to skip over it.
  while ((column < len) && whitespace(value[column])) column++;
  result*=sign;
  return TRUE;
}


static bool decodeNdM(const CString& value, int& n, int& m)
{  
  // If it does not decode properly, leave incol as is.
  CString errorMsg; // Discard error message
  int col=0;
  int len=value.GetLength();
  while ( (col<len) && whitespace(value[col])) col++;
  if (!GetInteger(col,value,n,errorMsg)) return false;
  if ( (col>=len) || ( (value[col]!='D')&&(value[col]!='d') ) ) return false;
  col++;
  if (!GetInteger(col,value,m,errorMsg)) return false;
  return true;
}

extern "C" int compileDicePlusRDR(CString& name, int *array)
{
  // int *array can hold up to 20 values
  //
  // array[0] should contain the number of int's
  // that follow.

  // some tokens may have quotes, "Half-Elf"
  name.Remove('\"');

  int qty=0,sides=0;
  if (decodeNdM(name, qty, sides))
  {
    array[0] = 3;
    array[1] = DICE_DB;
    array[2] = qty;
    array[3] = sides;
    return 1;
  }

  GENERIC_REFERENCE ref;
  ref.m_refName = name;
  if (!ref.LookupRefKey())
  {
    WriteDebugString("Unknown RDR value %s in compileDicePlusRDR\n", name);
    return 0; // error
  }

  array[0] = 2;
  array[1] = ref.m_refType;
  array[2] = ref.m_refKey;
  return 1;
}

extern "C" int interpretDicePlusRDR(int *array)
{
  switch (array[1])
  {
  case LEVEL_DB: 
  case RACE_DB: 
  case BASECLASS_DB:
  case ABILITY_DB:
  case CLASS_DB:
  case TRAIT_DB:
  case SPELLGROUP_DB:
  case GENDER_DB:
    // a known database type
    break;

  case DICE_DB:
      return (rollDice(array[3], array[2]));

  default:
    WriteDebugString("Illegal RDR code %i in interpretDicePlusRDR\n", array[1]);
    return 0;
  }

  GENERIC_REFERENCE ref;
  ref.m_refType = array[1];
  ref.m_refKey = array[2];  
  double result=0.0;
  ref.LookupReferenceData(result);
  return result;
}

// this one maximizes the possible dice roll
// otherwise, same as other callback
extern "C" int interpretMaxDicePlusRDR(int *array)
{
  switch (array[1])
  {
  case LEVEL_DB: 
  case RACE_DB: 
  case BASECLASS_DB:
  case ABILITY_DB:
  case CLASS_DB:
  case TRAIT_DB:
  case SPELLGROUP_DB:
  case GENDER_DB:
    // a known database type
    break;

  case DICE_DB:
      return (array[2]*array[1]);

  default:
    WriteDebugString("Illegal RDR code %i in interpretDicePlusRDR\n", array[1]);
    return 0;
  }

  GENERIC_REFERENCE ref;
  ref.m_refType = array[1];
  ref.m_refKey = array[2];  
  double result=0.0;
  ref.LookupReferenceData(result);
  return result;
}


DICEPLUS::DICEPLUS(void)
{
  Clear();
}

DICEPLUS::~DICEPLUS(void)
{
}

void DICEPLUS::Clear(void)
{
  /*
  m_sign=1;
  m_numDice=0;
  m_numSides=0;
  m_bonus=0;
  m_min=-999;
  m_max=999;
  m_adjust.RemoveAll();
  */
  m_Text="";
  m_Bin="";
}

DICEPLUS &DICEPLUS::operator = (const DICEPLUS& src)
{
  if (&src == this) return *this;
  //int count;
  m_Text=src.m_Text;
  m_Bin=src.m_Bin;
  /*
  m_sign=src.m_sign;
  m_numSides=src.m_numSides;
  m_numDice=src.m_numDice;
  m_bonus=src.m_bonus;
  m_min=src.m_min;
  m_max=src.m_max;
  m_adjust.RemoveAll();
  count=src.m_adjust.GetSize();
  m_adjust.SetSize(count);
  for (int i=0; i< count; i++)
  {
    m_adjust[i]=src.m_adjust[i];
    //ADJUSTMENT newAdj=src.m_adjust[i];
    //m_adjust.SetAtGrow(i,src.m_adjust[i]);
  };
  */
  return *this;
}

int DICEPLUS::GetDataSize(void)
{
  //int i,n;
  int size = sizeof(DICEPLUS);
  size += m_Text.GetLength();
  size += m_Bin.GetLength();
  /*
  n=m_adjust.GetSize();
  for (i=0; i<n; i++) 
    size += m_adjust[i].GetDataSize();
  */
  return size;
}
/*
void DICEPLUS::AddAdjust(int value, const CString& name)
{
  ADJUSTMENT newA(value, name);
  m_adjust.SetAtGrow(m_adjust.GetSize(),newA);
}

void DICEPLUS::AddAdjust(ADJUSTMENT& adjust)
{
  m_adjust.SetAtGrow(m_adjust.GetSize(), adjust);
}
*/

void DICEPLUS::AddAdjust(CString &name)
{
  // I don't think 'name' can even
  // have these characters if 'name'
  // is read from a text config file
  name.Remove('\n');
  name.Remove('\r');
  name.Remove('\t');

  CString tmp;
  if (m_Text.IsEmpty())
    tmp.Format("(%s)", name);
  else
    tmp.Format("+(%s)", name);
  m_Text += tmp;
}

BOOL DICEPLUS::Compile()
{
  m_Bin="";
  if (m_Text.GetLength()==0)
    return FALSE;
  
  //WriteDebugString("COMPILING: \'%s\'\n", m_Text);

  RDRCOMP ccalc; //to compile
  m_Bin = ccalc.CompileExpression(m_Text, compileDicePlusRDR);
  if (m_Bin.GetLength()==0)
  {
    CString tmp = m_Text.Left(475); // to avoid text len error
    WriteDebugString("COMPILE FAILED: %s\n", tmp);
  }
  return (m_Bin.GetLength() > 0);
}

BOOL DICEPLUS::Roll(double &result)
{
  if (m_Bin.GetLength()==0)
  {
    if (!Compile())
      return FALSE;
  }

  RDREXEC icalc;      //to interpret
  result = icalc.InterpretExpression(m_Bin,interpretDicePlusRDR);
  return TRUE;

  /*
  BOOL success=TRUE;
  if ((m_numSides==0)&&(m_numDice==0)&&(m_min==-999)&&(m_max==999))
    result = m_bonus;
  else
    result = rollDice(m_numSides,m_numDice,m_bonus);
  result*=m_sign;  

  int num = m_adjust.GetSize();
  if (num > 0)
  {
    double tmp=result;
    char diceop = m_adjust[0].m_operator[0];

    if (num > 1)
    {
      double adj=0.0;
      for (int i=1;i<num;i++)
      {
        if (m_adjust[i].HasTwoOperands())
        {
          // There should be only one of these, if any.
          // It should also be the first adjustment listed.
          if (!m_adjust[i].EvaluateBinary(adj))
            success = FALSE;
        }
        else
        {
          if (!m_adjust[i].Evaluate(adj))
            success=FALSE;
        }
      }

      // update result if adjustments parsed ok,
      // else ignore them and just use the dice value
      if (success)
        result = ApplyAdjOperator(tmp,diceop,adj);
    }
  }

  if (success)
  {
    if (m_sign >= 0)
    {
      if (result < m_min) result=m_min;
      if ((m_max > 0) && (result > m_max)) result=m_max;
    }
    else
    {    
      if ((m_max > 0) && (result < -m_max)) result=-m_max;
      if (result > -m_min) result=-m_min;
    }
  }
  return success;
  */
}

BOOL DICEPLUS::MaxRoll(double &result)
{
  if (m_Bin.GetLength()==0)
  {
    if (!Compile())
      return FALSE;
  }

  RDREXEC icalc;      //to interpret
  result = icalc.InterpretExpression(m_Bin,interpretMaxDicePlusRDR);
  return TRUE;
  /*
  BOOL success=TRUE;
  if ((m_numSides==0)&&(m_numDice==0)&&(m_min==-999)&&(m_max==999))
    result = m_bonus;
  else
    result = (m_numSides*m_numDice)+m_bonus;
  double tmp=result;
  for (int i=0;i<m_adjust.GetSize();i++)
  {
    if (!m_adjust[i].Evaluate(tmp))
      success=FALSE;
  }
  // update result if adjustments parsed ok,
  // else ignore them and just use the dice value
  if (success) result=tmp;
  result *= m_sign;
  if (result < m_min) result=m_min;
  if ((m_max > 0) && (result > m_max)) result=m_max;
  return TRUE;
  */
}

void DICEPLUS::PreSerialize(BOOL IsStoring)
{
  //int i,n;
  //n=m_adjust.GetSize();
  //for (i=0; i<n; i++) m_adjust[i].PreSerialize(IsStoring);
}

CString EncodeOldDicePlusText(char m_numDice,
                              BYTE m_numSides, 
                              char m_bonus,
                              int m_min,
                              int m_max,
                              char m_sign,                              
                              DICEPLUSADJLIST &m_adjust)
{
  CString result;
  CString temp;
  if ((m_numSides==0)&&(m_numDice==0))
  {
    // literal only, such as '10'
    if (m_sign < 0)
      result.Format("-%i", m_bonus);
    else
      result.Format("%i", m_bonus);

    int num=m_adjust.GetSize();
    if (num > 0)
    {
      result += m_adjust[0].EncodeOperator(m_adjust[0].m_operator[0]);

      // for things like 3*level    
      for (int i=1; i<num; i++)
      {
        ADJUSTMENT adj = m_adjust[i];
        result += adj.EncodeText();
      }

      // no need for min/max if just a literal used
      if ((m_bonus!=0)||(num>0))
      {
        // for upper/lower limits
        if (m_min != -999)
        {      
          temp.Format(" |< %d",
                        m_min);
          result += temp;
        }
        if ((m_max != 999)&&(m_max!=0))
        {      
          temp.Format(" >| %d",
                        m_max);
          result += temp;
        }
      }
    }
  }
  else
  {
    // 1d4
    if (m_sign < 0)
    {
      result.Format("-%dd%d",
                m_numDice,
                m_numSides);                
    }
    else
    {
      result.Format("%dd%d",
                m_numDice,
                m_numSides);
    }

    int num=m_adjust.GetSize();
    if (num > 0)
    {
      result += m_adjust[0].EncodeOperator(m_adjust[0].m_operator[0]);

      // for things like 3*level
      for (int i=1; i<num; i++)
      {
        ADJUSTMENT adj = m_adjust[i];
        result += adj.EncodeText();
      }
    }

    // for upper/lower limit
    if (m_min != -999)
    {      
      temp.Format(" |< %d",
                    m_min);
      result += temp;
    }
    if ((m_max != 999)&&(m_max!=0))
    {      
      temp.Format(" >| %d",
                    m_max);
      result += temp;
    }
  }
  return result;
}

int DICEPLUS::Serialize(CAR& car) 
{
  int result=0;
  CString dpVersion("DP2");
  unsigned int i, count;

  // temps used to load older DicePlus data
  char              m_numDice=0;
  BYTE              m_numSides=0;
  char              m_bonus=0;
  int               m_min=-999;
  int               m_max=999;
  char              m_sign=1;
  DICEPLUSADJLIST   m_adjust;

  if (car.IsStoring())
  {
    car << dpVersion;  // DICEPLUS version 2
    /*
    car << m_numDice;
    car << m_numSides;
    car << m_bonus;
    car << m_min;
    car << m_max;
    car << m_sign;
    count = m_adjust.GetSize();
    car.WriteCount(count);
    for (i=0; i<count; i++) 
      m_adjust[i].Serialize(car);
    */
    AS(car, m_Text);
    AS(car, m_Bin);
  }
  else 
  {
    car >> dpVersion;
    if (dpVersion=="DP0")
    {
      BYTE tmp;
      car >> m_numDice;
      car >> m_numSides;
      car >> m_bonus;
      car >> tmp; m_min=tmp;
      car >> tmp; m_max=tmp;
      count = car.ReadCount();
      m_adjust.SetSize(count);
      for (i=0; i<count; i++) 
      {
        result=m_adjust[i].Serialize(car);   
        if (result!=0) break;
      }

      if (m_numDice < 0)
      {
        m_sign = -1;
        m_numDice = abs(m_numDice);
      }
      m_Text = EncodeOldDicePlusText(m_numDice,m_numSides,m_bonus,m_min,m_max,m_sign,m_adjust);
    }
    else if (dpVersion=="DP1")
    {
      car >> m_numDice;
      car >> m_numSides;
      car >> m_bonus;
      car >> m_min;
      car >> m_max;
      car >> m_sign;
      count = car.ReadCount();
      m_adjust.SetSize(count);
      for (i=0; i<count; i++) 
      {
        result=m_adjust[i].Serialize(car);   
        if (result!=0) break;
      }

      if (m_numDice < 0)
      {
        m_sign = -1;
        m_numDice = abs(m_numDice);
      }
      m_Text = EncodeOldDicePlusText(m_numDice,m_numSides,m_bonus,m_min,m_max,m_sign,m_adjust);
    }
    else if (dpVersion=="DP2")
    {
      DAS(car, m_Text);
      DAS(car, m_Bin);
    }
    else 
    {
      CString msg;
      msg.Format("Unknown DICEPLUS format = %s\n",dpVersion);
      WriteDebugString(msg);
      result=1;
    };
  }
  return result;
}

int DICEPLUS::Serialize(CArchive& car) 
{
  int result=0;
  CString dpVersion("DP2");
  unsigned int i, count;

  char              m_numDice=0;
  BYTE              m_numSides=0;
  char              m_bonus=0;
  int               m_min=-999;
  int               m_max=999;
  char              m_sign=1;
  DICEPLUSADJLIST   m_adjust;

  if (car.IsStoring())
  {
    car << dpVersion;  // DICEPLUS version 2
    /*
    car << m_numDice;
    car << m_numSides;
    car << m_bonus;
    car << m_min;
    car << m_max;
    car << m_sign;
    count = m_adjust.GetSize();
    car.WriteCount(count);
    for (i=0; i<count; i++) 
      m_adjust[i].Serialize(car);
    */
    AS(car, m_Text);
    AS(car, m_Bin);
  }
  else 
  {
    car >> dpVersion;
    if (dpVersion=="DP0")
    {
      BYTE tmp;
      car >> m_numDice;
      car >> m_numSides;
      car >> m_bonus;
      car >> tmp; m_min=tmp;
      car >> tmp; m_max=tmp;
      count = car.ReadCount();
      m_adjust.SetSize(count);
      for (i=0; i<count; i++) 
      {
        result=m_adjust[i].Serialize(car);   
        if (result!=0) break;
      };

      if (m_numDice < 0)
      {
        m_sign = -1;
        m_numDice = abs(m_numDice);
      }
      m_Text = EncodeOldDicePlusText(m_numDice,m_numSides,m_bonus,m_min,m_max,m_sign,m_adjust);
    }
    else if (dpVersion=="DP1")
    {
      car >> m_numDice;
      car >> m_numSides;
      car >> m_bonus;
      car >> m_min;
      car >> m_max;
      car >> m_sign;
      count = car.ReadCount();
      m_adjust.SetSize(count);
      for (i=0; i<count; i++) 
      {
        result=m_adjust[i].Serialize(car);   
        if (result!=0) break;
      }

      if (m_numDice < 0)
      {
        m_sign = -1;
        m_numDice = abs(m_numDice);
      }
      m_Text = EncodeOldDicePlusText(m_numDice,m_numSides,m_bonus,m_min,m_max,m_sign,m_adjust);
    }
    else if (dpVersion=="DP2")
    {
      DAS(car, m_Text);
      DAS(car, m_Bin);
    }
    else 
    {
      CString msg;
      msg.Format("Unknown DICEPLUS format = %s\n",dpVersion);
      WriteDebugString(msg);
      result=1;
    };
  }
  return result;
}
CString DICEPLUS::EncodeText()
{
  return m_Text;
  /*
  CString tmp("");
  if (m_numDice < 0)
  {
    m_sign = -1;
    m_numDice = abs(m_numDice);
  }

  CONFIG_ENCODE_dicePlus(tmp,*this);
  return tmp;
  */
}

BOOL DICEPLUS::DecodeText(CString data, CString& err)
{
  if (data.GetLength()==0) return FALSE;
  if (CONFIG_DECODE_dicePlus(data,err,this,0,0,0,-999,999)!=CONFIG_STAT_ok)
  {
    WriteDebugString("Decode DicePlus failed \'%s\': %s\n", data, err);
    return FALSE;
  }
  return TRUE;
}
BOOL DICEPLUS::DecodeText(CString data)
{
  CString msg;
  return (DecodeText(data,msg));
}

//*****************************************************************************


ABILITY_LIMITS::ABILITY_LIMITS(void) 
{
  Clear();
}

ABILITY_LIMITS::~ABILITY_LIMITS(void)
{
}

int ABILITY_LIMITS::GetDataSize(void)
{
  int size = sizeof (m_min);
  size += sizeof (m_minMod);
  size += sizeof (m_max);
  size += sizeof (m_maxMod);
  size += m_ability.GetDataSize();
  return size;
}

void ABILITY_LIMITS::Clear(void)
{
  m_ability.Clear();
  m_min=0;
  m_minMod=0;
  m_max=999;
  m_maxMod=100;
}

ABILITY_LIMITS& ABILITY_LIMITS::operator =(const ABILITY_LIMITS& src)
{
  if (this==&src) return *this;
  m_ability    =src.m_ability;
  m_min        =src.m_min;
  m_minMod     =src.m_minMod;
  m_max        =src.m_max;
  m_maxMod     =src.m_maxMod;
  return *this;
}

int ABILITY_LIMITS::Serialize(CAR& car)
{
  int result=0;
  CString ver("ABL0");

  if (car.IsStoring())
  {
    car << ver;  // version
    m_ability.Serialize(car);  // Like strength
    car << m_min; // Positive
    car << m_minMod;
    car << m_max; // Positive
    car << m_maxMod;
  }
  else 
  {
    car >> ver;
    if (ver=="ABL0")
    {
      result=m_ability.Serialize(car);  // Like strength
      if (result!=0) return result;
      car >> m_min; // Positive
      car >> m_minMod;
      car >> m_max; // Positive
      car >> m_maxMod;
    } 
    else
    {
      WriteDebugString("Unknown ABILITY_LIMITS version = %s \n",ver);
      result=1;
    };
  };
  return result;
}


void ABILITY_LIMITS::PreSerialize(BOOL IsStoring)
{
  m_ability.PreSerialize(IsStoring);
}

RACE_DATA::RACE_DATA(void) :
	    m_race_asl ("RACE_DATA_ATTRIBUTES"),
	    m_temp_asl ("RACE_DATA_ATTRIBUTES")
{
  Clear();
}

RACE_DATA::~RACE_DATA(void)
{
  Clear();
}

void RACE_DATA::Clear(void)
{
  m_raceKey=raceUnknown;
  m_name="";
  m_weight.Clear();
  m_height.Clear();
  m_age.Clear();
  m_maxAge.Clear();
  m_abilityRequired.RemoveAll();
  m_baseMovement.Clear();
  m_race_asl.Clear();
  m_temp_asl.Clear();
}

int RACE_DATA::GetDataSize(void)
{
  int i,n;
  int size=0;
  size += m_race_asl.GetDataSize();
  size += m_temp_asl.GetDataSize();
  size += m_name.GetLength();
  size += m_weight.GetDataSize();
  size += m_height.GetDataSize();
  size += m_age.GetDataSize();
  size += m_maxAge.GetDataSize();
  size += m_baseMovement.GetDataSize();
  n=m_abilityRequired.GetSize();
  for (i=0; i<n; i++) size += m_abilityRequired[i].GetDataSize();
  return size;
}

void RACE_DATA::PreSerialize(BOOL IsStoring)
{
  // We have to convert m_abilityRequired to keys in the abilities database
  int i;
  for (i=0; i<m_abilityRequired.GetSize(); i++)
    m_abilityRequired[i].PreSerialize(IsStoring);
}

void RACE_DATA::LogUndefinedMultiClassReferences(void)
{
  DELIMITED_STRING MCRef;
  CString ref;
  int i, n;
  MCRef = m_race_asl.Lookup(RACE_ALLOWED_CLASS_ATTRIBUTE);
  if (!MCRef.IsLegal())
  {
    CString msg;
    msg.Format("Race %s has an illegal AllowedMultiClass list\n",m_name);
    PerhapsPrintCompileError(msg);
    return;
  };
  n = MCRef.Count();
  for (i=0; i<n; i++)
  {
    ref = MCRef.GetAt(i);
    if (classData.FindName(ref) == classUnknown)
    {
      CString msg;
      msg.Format("Race %s has undefined AllowedMultiClass %s\n",
                             m_name, ref);
      PerhapsPrintCompileError(msg);
    };
  };
}

int RACE_DATA::Serialize(CAR &car)
{
  int i, n;
  int result=0;
  if (car.IsStoring())
  {
    car << m_raceKey;
    if (m_name=="") m_name="*";
    car << m_name;
    if (m_name=="*") m_name="";
    m_weight.Serialize(car);
    m_height.Serialize(car);
    m_age.Serialize(car);
    m_maxAge.Serialize(car);
    car.WriteCount(m_abilityRequired.GetSize());
    for (i=0; i<m_abilityRequired.GetSize(); i++)
      m_abilityRequired[i].Serialize(car);
    m_baseMovement.Serialize(car);
    LogUndefinedMultiClassReferences();
    car << m_race_asl;
  }
  else
  {
    int key;
    car >> key;
    m_raceKey = (raceType)key;
    car >> m_name; if (m_name=="*") m_name="";
#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Gnome|Stupid" will be
    // read as simply "Gnome".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    if (m_name.Find('|') >= 0) m_name = m_name.Left(m_name.Find('|')); // Remove qualifier.
#endif
    result=m_weight.Serialize(car);
    if (result!=0) return result;
    result=m_height.Serialize(car);
    if (result!=0) return result;
    result=m_age.Serialize(car);
    if (result!=0) return result;
    result=m_maxAge.Serialize(car);
    if (result!=0) return result;
    n=car.ReadCount();
    m_abilityRequired.SetSize(n);
    for (i=0; i<n; i++)
    {
      result=m_abilityRequired[i].Serialize(car);
      if (result!=0) return result;
    };
    result=m_baseMovement.Serialize(car);
    if (result!=0) return result;
    car >> m_race_asl;
  }
  return result;
}

void RACE_DATA::AddAbilityRequirement(ABILITY_LIMITS& abReq)
{
  int i, n;
  n=m_abilityRequired.GetSize();
  for (i=0; i<n; i++) 
  {
    if (m_abilityRequired[i].Name()==abReq.Name()) break;
  };

  if (abReq.m_ability.m_abilityKey==abilityUnknown)
    abReq.m_ability.m_abilityKey = abilityData.FindName(abReq.Name());

  m_abilityRequired.SetAtGrow(i,abReq);
}

void RACE_DATA::AddAllowedClass(const CString& className)
{
  CString key = RACE_ALLOWED_CLASS_ATTRIBUTE;
  CString newValue;
  DELIMITED_STRING ds;
  if (m_race_asl.Exists(key))
  {
    ds = m_race_asl.Lookup(key);
  };
  if (ds.Contains(className)) return; //It is already there!
  ds.Insert(className);
  newValue = ds.Result();
  m_race_asl.Insert(key,newValue,ASLF_READONLY|ASLF_DESIGN);
}

//*********************************************************
// Is this particular Class allowed.
//*********************************************************
bool RACE_DATA::IsAllowedClass(const CString& className) const
{
  DELIMITED_STRING ds;
  CString key = RACE_ALLOWED_CLASS_ATTRIBUTE;
  if (!m_race_asl.Exists(key)) return true;
  ds = m_race_asl.Lookup(key);
  if (!ds.IsLegal()) return true;
  return ds.Contains(className);
}

DWORD RACE_DATA::GetWeight(void)
{
  // need character context before doing this
  double result;
  if (m_weight.Roll(result)) return result;
  return 0;
}

DWORD RACE_DATA::GetHeight(void)
{
  // need character context before doing this
  double result;
  if (m_height.Roll(result)) return result;
  return 0;
}

DWORD RACE_DATA::GetStartAge(void)
{
  // need character context before doing this
  double result;
  if (m_age.Roll(result)) return result;
  return 0;
}

DWORD RACE_DATA::GetMaxAge(void)
{
  // need character context before doing this
  double result;
  if (m_maxAge.Roll(result)) return result;
  return 0;
}

DWORD RACE_DATA::GetBaseMovement(void)
{
  // need character context before doing this
  double result;
  if (m_baseMovement.Roll(result)) return result;
  return 0;
}
short RACE_DATA::GetMinAbility(abilityType type)
{
  CString keyText = abilityData.GetName(type);
  if (keyText.IsEmpty()) return DEFAULT_MIN_ABILITY_LIMIT;

  for (int i=0;i<m_abilityRequired.GetSize();i++)
  {
    if (m_abilityRequired[i].Name().CompareNoCase(keyText)==0)
      return (m_abilityRequired[i].GetMin());
  }
  return DEFAULT_MIN_ABILITY_LIMIT;
}

short RACE_DATA::GetMaxAbility(abilityType type)
{
  CString keyText = abilityData.GetName(type);
  if (keyText.IsEmpty()) return DEFAULT_MAX_ABILITY_LIMIT;

  for (int i=0;i<m_abilityRequired.GetSize();i++)
  {
    if (m_abilityRequired[i].Name().CompareNoCase(keyText)==0)
      return (m_abilityRequired[i].GetMax());
  }
  return DEFAULT_MAX_ABILITY_LIMIT;
}

/* Function moved to ALLOWED_CLASSES
void RACE_DATA::RestrictClasses(ALLOWED_CLASSES *pAllowedClasses)
{
  int i;
  for (i=pAllowedClasses->NumAllowedClasses(); i>=0; i--)
  {
    if (IsAllowedClass(pAllowedClasses->GetAt(i)->m_name)) continue;
    if (pAllowedClasses->GetAt(i)->IsRaceAllowed(m_name)) continue;
    pAllowedClasses->RemoveAt(i);
  }; //That was easy.
}
*/

RACE_DATA_TYPE::RACE_DATA_TYPE(void)
{
  Clear();
  LoadUADefaults();
}

void RACE_DATA_TYPE::LoadUADefaults(void)
{ // If you want to Clear() you have to do it yourself!
  RACE_DATA rd;
  rd.m_name="HUMAN";
  rd.m_raceKey=Human;
  AddRaceWithCurrKey(rd);
  rd.m_name="ELF";
  rd.m_raceKey=Elf;
  AddRaceWithCurrKey(rd);
  rd.m_name="DWARF";
  rd.m_raceKey=Dwarf;
  AddRaceWithCurrKey(rd);
  rd.m_name="GNOME";
  rd.m_raceKey=Gnome;
  AddRaceWithCurrKey(rd);
  rd.m_name="HALF-ELF";
  rd.m_raceKey=HalfElf;
  AddRaceWithCurrKey(rd);
  rd.m_name="HALFLING";
  rd.m_raceKey=Halfling;
  AddRaceWithCurrKey(rd);
}

RACE_DATA_TYPE::~RACE_DATA_TYPE(void)
{
}


// called when loading binary race data that
// was saved with key info
raceType RACE_DATA_TYPE::AddRaceWithCurrKey(const RACE_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (m_RaceData.GetCount() < MAX_RACES)
  {
    m_RaceData.Insert(data, data.m_raceKey);
    return (raceType)data.m_raceKey;
  }
  else
    return raceUnknown;
}

int RACE_DATA_TYPE::GetDataSize(void)
{
  int size=sizeof(RACE_DATA_TYPE);
  POSITION pos;
  for (pos=m_RaceData.GetHeadPosition(); 
       pos!=NULL; 
       size+=m_RaceData.GetNext(pos).GetDataSize())
         ;
  return size;
}

void RACE_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  POSITION pos;
  pos=m_RaceData.GetHeadPosition();
  while (pos!=NULL)
     m_RaceData.GetNext(pos).PreSerialize(IsStoring);
}

void RACE_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
}

int RACE_DATA_TYPE::Serialize(CAR& car)
{
  POSITION p;
  CString version("RaceV1");
  int result=0;

  if (car.IsStoring())
  {
    //car << "RaceV0";
    car << version; // V1 is compressed
    car.Compress(true);
    car.WriteCount(GetCount());
    p = m_RaceData.GetHeadPosition();
    while (p != NULL)
      m_RaceData.GetNext(p).Serialize(car);
  }
  else
  {
    RACE_DATA data;
    int count;
    car >> version;
    if ((version =="RaceV0")||(version=="RaceV1"))
    {
      if (version > "RaceV0") car.Compress(true);
      Clear();
      count = car.ReadCount();
      for (int i=0; i<count; i++)
      {
        result=data.Serialize(car);
        if (result!=0) return result;
        AddRaceWithCurrKey(data);
      }
    }
    else
    {
      WriteDebugString("Unknown Race data version = %s",version);
      result=1;
    };
  };
  return result;
}


RACE_DATA *RACE_DATA::operator = (const RACE_DATA& src) 
{
  if (this==&src)return this;
  m_raceKey=src.m_raceKey;
	m_race_asl=src.m_race_asl;
	m_temp_asl=src.m_temp_asl;
  m_name=src.m_name;
  m_weight=src.m_weight;
  m_height=src.m_height;
  m_age          =src.m_age;
  m_maxAge       =src.m_maxAge;
  m_baseMovement =src.m_baseMovement;
  m_abilityRequired.RemoveAll();
  for (int i=0; i<src.m_abilityRequired.GetSize(); i++)
  {
    ABILITY_LIMITS newAL=src.m_abilityRequired[i];
    m_abilityRequired.SetAtGrow(i, newAL);
  };
  return this;
}



// Called when loading from scripts, or
// when adding from editor GUI
raceType RACE_DATA_TYPE::AddRace(const RACE_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (data.m_name.IsEmpty()) return raceUnknown;
  // Do not allow multiple items with same full name.
  if (m_FindName(data.m_name)!=raceUnknown) return raceUnknown;
  if (m_RaceData.GetCount() < MAX_RACES)
  {
    data.m_raceKey = GetNextRace();
    m_RaceData.Insert(data, data.m_raceKey);
    return (raceType)data.m_raceKey;
  }
  else
    return raceUnknown;
}

DWORD RACE_DATA_TYPE::GetWeight(raceType race)
{
  ASSERT ( race < NUM_RACE_TYPES );
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_RaceData.FindKeyPos(race)) != NULL)
  {
    return m_RaceData.GetAt(pos).GetWeight();
  }
  else
    return 0;
}

DWORD RACE_DATA_TYPE::GetHeight(raceType race)
{
  ASSERT ( race < NUM_RACE_TYPES );
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_RaceData.FindKeyPos(race)) != NULL)
  {
    return m_RaceData.GetAt(pos).GetHeight();
  }
  else
    return 0;
}

DWORD RACE_DATA_TYPE::GetStartAge(raceType race)
{
  ASSERT ( race < NUM_RACE_TYPES );
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_RaceData.FindKeyPos(race)) != NULL)
  {
    return m_RaceData.GetAt(pos).GetStartAge();
  }
  else
    return 0;
}

DWORD RACE_DATA_TYPE::GetMaxAge(raceType race)
{
  ASSERT ( race < NUM_RACE_TYPES );
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_RaceData.FindKeyPos(race)) != NULL)
  {
    return m_RaceData.GetAt(pos).GetMaxAge();
  }
  else
    return 0;
}

DWORD RACE_DATA_TYPE::GetBaseMovement(raceType race)
{
  ASSERT ( race < NUM_RACE_TYPES );
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_RaceData.FindKeyPos(race)) != NULL)
  {
    return m_RaceData.GetAt(pos).GetBaseMovement();
  }
  else
    return 0;
}

short RACE_DATA_TYPE::GetMinAbility(raceType race,abilityType type)
{
  ASSERT ( race < NUM_RACE_TYPES );
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_RaceData.FindKeyPos(race)) != NULL)
  {
    return m_RaceData.GetAt(pos).GetMinAbility(type);
  }
  else
    return 0;
}

short RACE_DATA_TYPE::GetMaxAbility(raceType race,abilityType type)
{
  ASSERT ( race < NUM_RACE_TYPES );
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_RaceData.FindKeyPos(race)) != NULL)
  {
    return m_RaceData.GetAt(pos).GetMaxAbility(type);
  }
  else
    return 0;
}


CString RACE_DATA_TYPE::GetRaceName(raceType race)
{
  ASSERT ( race < NUM_RACE_TYPES );
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_RaceData.FindKeyPos(race)) != NULL)
  {
    return m_RaceData.GetAt(pos).Name();
  }
  else
    return CString("No Race");
}

// No locking... for internal use
raceType RACE_DATA_TYPE::m_FindName (const CString& name)
// Return key associated with the named object, or zero
{
  POSITION pos;
  pos=m_RaceData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (m_RaceData.GetAt(pos).m_name.CompareNoCase(name)==0) 
          return (raceType)m_RaceData.GetAt(pos).m_raceKey;
    m_RaceData.GetNext(pos);
  };
  return raceUnknown;
}

// With locking...for public use
raceType RACE_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}

raceType RACE_DATA_TYPE::GetNextRace()
{
  raceType key=raceUnknown;
  if (GetCount() > 0)
  {
    key = (raceType)m_RaceData.GetTail().m_raceKey;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is much greater than
      // MAX_RACES.
      // Gaps occur when deletions are made to the races list
      POSITION pos = m_RaceData.GetHeadPosition();
      BOOL found = FALSE;        
      DWORD prevKey = 0;
      DWORD currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = m_RaceData.GetNext(pos).m_raceKey;
        if (currKey != prevKey+1)
        {
          key = (raceType)(prevKey+1);
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key=(raceType)(key+1);
  }
  else
    key = (raceType)0;
  return key;
}

bool RACE_DATA_TYPE::IsAllowedClass(raceType race,const CString& className)
{
  POSITION p;
  p = m_RaceData.FindKeyPos(race);
  if (p == NULL) return false;
  return m_RaceData.GetAt(p).IsAllowedClass(className);
}

//*****************************************************************************

ABILITY_DATA::ABILITY_DATA (void)
{
  Clear();
}

ABILITY_DATA::~ABILITY_DATA (void)
{
}


void ABILITY_DATA::PreSerialize(BOOL IsStoring)
{
  m_roll.PreSerialize(IsStoring);
}

int ABILITY_DATA::Serialize(CAR& car)
{
  int result=0;
  CString tempStr("Abd0");
  if (car.IsStoring())
  {
    car << tempStr;  // Version
    car << m_abilityKey;
    if (m_name=="") m_name="*";
    if (m_abbreviation=="") m_abbreviation="*";
    car << m_name;
    car << m_abbreviation;
    if (m_name=="*") m_name="";
    if (m_abbreviation=="*") m_abbreviation="";
    m_roll.Serialize(car);
  }
  else 
  {    
    Clear();
    car >> tempStr;
    if (tempStr=="Abd0")
    {
      car >> m_abilityKey;
      car >> m_name; if (m_name=="*") m_name="";
      car >> m_abbreviation; if (m_abbreviation=="*") m_abbreviation="";
      result=m_roll.Serialize(car);
    }
    else
    {
      WriteDebugString("Unknown ABILITY_DATA serialize version = %s\n",tempStr);
      result=1;
    };
  };
  return result;
}

void ABILITY_DATA::Clear(void)
{
  m_abilityKey=0;
  m_name="";
  m_abbreviation="";
  m_modify=0;
  m_roll.Clear();
}

int ABILITY_DATA::GetDataSize(void)
{
  int size=sizeof(ABILITY_DATA);
  size += m_name.GetLength();
  size += m_abbreviation.GetLength();
  size += m_roll.GetDataSize();
  return size;
}
/*
void ABILITY_DATA::AddAdjust(ADJUSTMENT& adjust)
{
  m_roll.AddAdjust(adjust);
}
*/

void ABILITY_DATA::AddAdjust(CString &name)
{
  m_roll.AddAdjust(name);
}

BOOL ABILITY_DATA::RollAbility(double &result)
{
  return (m_roll.Roll(result));

  /*
  BOOL success=TRUE;
  if ((m_roll.m_numSides==0)&&(m_roll.m_numDice==0)&&(m_roll.m_min==-999)&&(m_roll.m_max==999))
    result = m_roll.m_bonus;
  else
    result = rollDice(m_roll.m_numSides,m_roll.m_numDice,m_roll.m_bonus);

  result*=m_roll.m_sign;

  double tmp=result;

  int num = m_roll.m_adjust.GetSize();
  double adj=0.0;
  for (int i=0;i<num;i++)
  {
    if (m_roll.m_adjust[i].HasTwoOperands())
    {
      // There should be only one of these, if any.
      // It should also be the first adjustment listed.
      if (!m_roll.m_adjust[i].EvaluateBinary(adj))
        success = FALSE;
    }
    else
    {
      if (!m_roll.m_adjust[i].Evaluate(adj))
        success=FALSE;
    }
    tmp += adj;
  }

  if (success)
    result = tmp;

  if (success)
  {
    if (m_roll.m_sign >= 0)
    {
      if (result < m_roll.m_min) result=m_roll.m_min;
      if ((m_roll.m_max > 0) && (result > m_roll.m_max)) result=m_roll.m_max;
    }
    else
    {    
      if ((m_roll.m_max > 0) && (result < -m_roll.m_max)) result=-m_roll.m_max;
      if (result > -m_roll.m_min) result=-m_roll.m_min;
    }
  }
  return success;
  */
}


//*****************************************************************************


ABILITY_DATA_TYPE::ABILITY_DATA_TYPE (void)
{
  LoadUADefaults();
}

void ABILITY_DATA_TYPE::LoadUADefaults(void)
{ // You have to do your own Clear() if that is what you want.
  ABILITY_DATA abdat;
  abdat.Clear();
    abdat.m_name="Strength";
    abdat.m_abbreviation="STR";
    AddAbility(abdat);
  abdat.Clear();
    abdat.m_name="Intelligence";
    abdat.m_abbreviation="INT";
    AddAbility(abdat);
  abdat.Clear();
    abdat.m_name="Wisdom";
    abdat.m_abbreviation="WIS";
    AddAbility(abdat);
  abdat.Clear();
    abdat.m_name="Dexterity";
    abdat.m_abbreviation="DEX";
    AddAbility(abdat);
  abdat.Clear();
    abdat.m_name="Constitution";
    abdat.m_abbreviation="CON";
    AddAbility(abdat);
  abdat.Clear();
    abdat.m_name="Charisma";
    abdat.m_abbreviation="CHA";
    AddAbility(abdat);
}

ABILITY_DATA_TYPE::~ABILITY_DATA_TYPE(void)
{
}

void ABILITY_DATA_TYPE::Clear(void)
{
  m_AbilityData.RemoveAll();
//  m_numAbilities=0;
}

// With locking...for public use
abilityType ABILITY_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}

int ABILITY_DATA_TYPE::GetDataSize(void)
{
  POSITION pos;
  int size= sizeof(ABILITY_DATA_TYPE);
  pos=m_AbilityData.GetHeadPosition();
  while (pos!= NULL) size += m_AbilityData.GetNext(pos).GetDataSize();
  return size;
}

// called when loading binary ability data that
// was saved with key info
abilityType ABILITY_DATA_TYPE::AddAbilityWithCurrKey(const ABILITY_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (m_AbilityData.GetCount() < MAX_ABILITIES)
  {
    m_AbilityData.Insert(data, data.m_abilityKey);
    return (abilityType)data.m_abilityKey;
  }
  else
    return abilityUnknown;
}


abilityType ABILITY_DATA_TYPE::AddAbility(const ABILITY_DATA& newab)
// Called when loading from scripts, or
// when adding from editor GUI
{
  CSingleLock sLock(&m_CS, TRUE);
  if (newab.m_name.IsEmpty()) return abilityUnknown;
  // Do not allow multiple items with same full name.
  if (m_FindName(newab.m_name)!=abilityUnknown) return abilityUnknown;
  if (m_AbilityData.GetCount() < MAX_ABILITIES)
  {
    newab.m_abilityKey = GetNextAbility();
    m_AbilityData.Insert(newab, newab.m_abilityKey);
    return (abilityType)newab.m_abilityKey;
  }
  else
    return abilityUnknown;
}

abilityType ABILITY_DATA_TYPE::GetNextAbility()
{
  abilityType key=abilityUnknown;
  if (GetCount() > 0)
  {
    key = (abilityType)m_AbilityData.GetTail().m_abilityKey;
    if (key >= UINT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is much greater than
      // MAX_ABILITIES.
      // Gaps occur when deletions are made to the abilities list
      POSITION pos = m_AbilityData.GetHeadPosition();
      BOOL found = FALSE;        
      DWORD prevKey = 0;
      DWORD currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = m_AbilityData.GetNext(pos).m_abilityKey;
        if (currKey != prevKey+1)
        {
          key = (abilityType)(prevKey+1);
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key=(abilityType)(key+1);
  }
  else
    key = (abilityType)0;
  return key;
}

// No locking... for internal use
abilityType ABILITY_DATA_TYPE::m_FindName (const CString& name)
// Return key associated with the named object, or abilityUnknown
{
  POSITION pos;
  pos=m_AbilityData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (m_AbilityData.GetAt(pos).m_name.CompareNoCase(name)==0) 
          return (abilityType)m_AbilityData.GetAt(pos).m_abilityKey;
    m_AbilityData.GetNext(pos);
  };
  return abilityUnknown;
}

abilityType ABILITY_DATA_TYPE::GetKeyByIndex(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  if (index < 0) return abilityUnknown;
  POSITION pos;
  pos=m_AbilityData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (index <= 0)
      return ((abilityType)m_AbilityData.GetAt(pos).m_abilityKey);
     index--;
    m_AbilityData.GetNext(pos);
  };
  return abilityUnknown;
}


void ABILITY_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  POSITION pos;
  pos=m_AbilityData.GetHeadPosition();
  while (pos!=NULL)
     m_AbilityData.GetNext(pos).PreSerialize(IsStoring);
}

void ABILITY_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
}

int ABILITY_DATA_TYPE::Serialize(CAR& car)
{
  POSITION p;
  CString version("AbilityV1");
  int result=0;

  if (car.IsStoring())
  {
//    car << "AbilityV0";
    car << version; // V1 is compressed
    car.Compress(true);
    car << GetCount();
    p = m_AbilityData.GetHeadPosition();
    while (p != NULL)
      m_AbilityData.GetNext(p).Serialize(car);
  }
  else
  {
    ABILITY_DATA data;
    int count;
    car >> version;
    if ((version =="AbilityV0")||(version=="AbilityV1"))
    {
      if (version > "AbilityV0") car.Compress(true);
      car >> count;
      for (int i=0; i<count; i++)
      {
        result=data.Serialize(car);
        if (result!=0) break;
        AddAbilityWithCurrKey(data);
      }
    }
    else
    {
      WriteDebugString("Unknown Ability data version = %s",version);
      result=1;
    };
  }
  return result;
}

CString ABILITY_DATA_TYPE::GetName(abilityType type)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_AbilityData.FindKeyPos(type)) != NULL)
  {
    return (m_AbilityData.GetAt(pos).m_name);
  }
  else
    return "";
}

CString ABILITY_DATA_TYPE::GetShortName(abilityType type)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_AbilityData.FindKeyPos(type)) != NULL)
  {
    return (m_AbilityData.GetAt(pos).m_abbreviation);
  }
  else
    return "";
}

BOOL ABILITY_DATA_TYPE::RollAbility(abilityType ability, double &result)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_AbilityData.FindKeyPos(ability)) != NULL)
  {
    return (m_AbilityData.GetAt(pos).RollAbility(result));
  }
  else
    return FALSE;

  /*
  int count=0;
  POSITION pos = m_AbilityData.GetHeadPosition();
  while (pos != NULL)
  {
    if (count==index)
    {
      double result;
      if (m_AbilityData.GetAt(pos).RollAbility(result))
        return result;
      return 0.0;
    }
    m_AbilityData.GetNext(pos);
    count++;
  }
  return 0.0;
  */
}


//*****************************************************************************


BASE_CLASS_DATA::BASE_CLASS_DATA(void)
{
  Clear();
}

BASE_CLASS_DATA::~BASE_CLASS_DATA(void)
{
}

void BASE_CLASS_DATA::Clear(void)
{
  m_baseclassKey=baseclassUnknown;
  m_name="";
  m_primeReq.RemoveAll();
  m_abilityReq.RemoveAll();
  m_allowedRaces.RemoveAll();
  m_expLevels.RemoveAll();
  m_allowedAlignments = 0x1ff; // any
}

int BASE_CLASS_DATA::GetDataSize(void)
{
  int i,n;
  int size=sizeof (*this);
  size += m_name.GetLength();
  size += m_expLevels.GetSize() * sizeof(WORD);
  n=m_primeReq.GetSize();
  for (i=0; i<n; i++) size += m_primeReq[i].GetDataSize();
  n=m_abilityReq.GetSize();
  for (i=0; i<n; i++) size += m_abilityReq[i].GetDataSize();
  n=m_allowedRaces.GetSize();
  for (i=0; i<n; i++) size += m_allowedRaces[i].GetDataSize();
  return size;
}

BASE_CLASS_DATA& BASE_CLASS_DATA::operator =(const BASE_CLASS_DATA& src)
{
  int i, n;
  if (this==&src) return *this;
  Clear();
  m_baseclassKey=src.m_baseclassKey;
  m_name        =src.m_name;
  n=src.m_primeReq.GetSize();
    m_primeReq.SetSize(n);
    for (i=0; i<n; i++) m_primeReq[i]=src.m_primeReq[i];
  n=src.m_abilityReq.GetSize();
    m_abilityReq.SetSize(n);
    for (i=0; i<n; i++) m_abilityReq[i]=src.m_abilityReq[i];
  n=src.m_allowedRaces.GetSize();
    m_allowedRaces.SetSize(n);
    for (i=0; i<n; i++) m_allowedRaces[i]=src.m_allowedRaces[i];
  n=src.m_expLevels.GetSize();
    m_expLevels.SetSize(n);
    for (i=0; i<n; i++) m_expLevels[i]=src.m_expLevels[i];
  m_allowedAlignments=src.m_allowedAlignments;
  return *this;
}

void BASE_CLASS_DATA::AddPrimeReq(const CString& reqName)
{
  int n, i;
  n=m_primeReq.GetSize();
  for (i=0; i<n; i++) 
  {
    if (m_primeReq[i].m_abilityName==reqName) return;
  };
  ABILITY_REFERENCE ar;
  ar.m_abilityName=reqName;
  m_primeReq.SetAtGrow(n, ar);
}

void BASE_CLASS_DATA::AddAllowedRace(RACE_REFERENCE& race)
{
  int n, i;
  n=m_allowedRaces.GetSize();
  for (i=0; i<n; i++) 
  {
    if (m_allowedRaces[i].m_raceName==race.m_raceName) return;
  };
  m_allowedRaces.SetAtGrow(n, race);
}

void BASE_CLASS_DATA::PreSerialize(BOOL IsStoring)
{
  int i,n;
  n=m_primeReq.GetSize();
  for (i=0; i<n; i++) m_primeReq[i].PreSerialize(IsStoring);
  n=m_abilityReq.GetSize();
  for (i=0; i<n; i++) m_abilityReq[i].PreSerialize(IsStoring);
  n=m_allowedRaces.GetSize();
  for (i=0; i<n; i++) m_allowedRaces[i].PreSerialize(IsStoring); 
}

int BASE_CLASS_DATA::Serialize(CAR& car)
{
  int i,n;
  int result=0;
  CString ver("Bcd1");
  if (car.IsStoring())
  {
    car << ver; // version
    car << m_baseclassKey;
    if (m_name=="") m_name="*";
    car << m_name;
    if (m_name=="*") m_name="";
    n=m_primeReq.GetSize();
      car.WriteCount(n);
      for (i=0; i<n; i++) m_primeReq[i].Serialize(car);
    n=m_abilityReq.GetSize();
      car.WriteCount(n);
      for (i=0; i<n; i++) m_abilityReq[i].Serialize(car);
    n=m_allowedRaces.GetSize();
      car.WriteCount(n);
      for (i=0; i<n; i++) m_allowedRaces[i].Serialize(car);
    //m_expLevels.Serialize(car);
    car << m_expLevels;
    car << m_allowedAlignments;
  }
  else
  {    
    car >> ver;
    if ((ver=="Bcd0")||(ver=="Bcd1"))
    {
      Clear();
      car >> m_baseclassKey;
      car >> m_name; if (m_name=="*") m_name="";
      n=car.ReadCount();
        m_primeReq.SetSize(n);
        for (i=0; i<n; i++) 
        {
          result=m_primeReq[i].Serialize(car);
          if (result!=0) return result;
        };
      n=car.ReadCount();
        m_abilityReq.SetSize(n);
        for (i=0; i<n; i++) 
        {
          result=m_abilityReq[i].Serialize(car);
          if (result!=0) return result;
        };
      n=car.ReadCount();
        m_allowedRaces.SetSize(n);
        for (i=0; i<n; i++) 
        {
          result=m_allowedRaces[i].Serialize(car);
          if (result!=0) return result;
        };
      car >> m_expLevels;
      //m_expLevels.Serialize(car);
      if (result!=0) return result;
      if (ver>="Bcd1") car >> m_allowedAlignments;
      else m_allowedAlignments=0x1ff;
    }
    else
    {
      WriteDebugString("Unknown BASE_CLASS_DATA serialize version = %s\n",ver);
      result=1;
    };
  };
  return result;
}

void BASE_CLASS_DATA::AddAbilityRequirement(ABILITY_LIMITS& abReq)
{
  int i, n;
  n=m_abilityReq.GetSize();
  for (i=0; i<n; i++) 
  {
    if (m_abilityReq[i].Name()==abReq.Name()) break;
  };
  if (abReq.m_ability.m_abilityKey==abilityUnknown)
    abReq.m_ability.m_abilityKey = abilityData.FindName(abReq.Name());
  m_abilityReq.SetAtGrow(i,abReq);
}

void BASE_CLASS_DATA::AddExpLevel(WORD exp)
{
  int n;
  n=m_expLevels.GetSize();
  m_expLevels.SetAtGrow(n,exp);
}

WORD BASE_CLASS_DATA::GetMinExpForLevel(int level)
{
  if (level <= 1) return 0;
  return (GetMaxExpForLevel(level-1)+1);
}

WORD BASE_CLASS_DATA::GetMaxExpForLevel(int level)
{
  if (m_expLevels.GetSize()==0)
  {
    ASSERT(FALSE);
    return 0;
  }
  if (level < 1) level = 1;
  if (level > m_expLevels.GetUpperBound()) level = m_expLevels.GetUpperBound();
  return m_expLevels[level];
}

struct {
  BYTE min;
  BYTE minMod;
  BYTE max;
  BYTE maxMod;
} UAabilityLimits[] =
{ 
  {9,0,18,100},  // Fighter
  {3,0,18,0},{3,0,18,0},{3,0,18,0},{3,0,18,0},{3,0,18,0},

  {3,0,18,0}, //Cleric
  {3,0,18,0},{9,0,18,0},{3,0,18,0},{3,0,18,0},{3,0,18,0},

  {13,0,18,0  },  // Ranger
  {3,0,18,0},{14,0,18,0},{13,0,18,0},{14,0,18,0},{3,0,18,0},

  {12,0,18,0  },  // Paladin
  {3,0,18,0},{13,0,18,0},{3,0,18,0},{9,0,18,0},{17,0,18,0},

  {3,0,18,0  },  // Magic User
  {9,0,18,0},{3,0,18,0},{3,0,18,0},{3,0,18,0},{3,0,18,0},

  {3,0,18,0  },  // Thief
  {3,0,18,0},{3,0,18,0},{9,0,18,0},{3,0,18,0},{3,0,18,0},

  {3,0,18,0  },  // Druid
  {3,0,18,0},{12,0,18,0},{3,0,18,0},{3,0,18,0},{15,0,18,0},

};

void BASE_CLASS_DATA::SetUAAbilityLimits(void)
{
  BYTE *limit;
  int i;
  ASSERT(m_baseclassKey <= 6);
  ABILITY_LIMITS al;
  if (m_baseclassKey > 6) return;
  limit = ((BYTE *)UAabilityLimits+6*m_baseclassKey);
  m_abilityReq.SetSize(ABREFKEY_COUNT);
  for (i=0; i<ABREFKEY_COUNT; i++) 
  {
    al.m_ability.m_abilityKey=i; // STR, INT, etc
    al.m_min=*(limit++);
    al.m_minMod=*(limit++);
    al.m_max=*(limit++);
    al.m_maxMod=*(limit++);
    m_abilityReq[i]=al;
  };
}

void BASE_CLASS_DATA::SetUAAllowedAlignments(void)
{
  static WORD UAAlignments[7]=
  {
      0x1ff, //Fighter
      0x1ff, //Cleric
      0x007, //Ranger
      0x001, //Paladin
      0x1ff, //MagicUser
      0x1fe, //Thief
      0x1ff  //Druid
  };
  m_allowedAlignments=0x1ff;
  if (m_baseclassKey > 6) return;
  m_allowedAlignments=UAAlignments[m_baseclassKey];
}



ABILITYLIMITS BASE_CLASS_DATA::GetAbilityLimits(abilityType abType)
{
  int i,n;
  n=m_abilityReq.GetSize();
  for (i=0; i<n; i++) 
  {
    if (m_abilityReq[i].m_ability.m_abilityKey==abilityUnknown)
    {
      // make sure this abilityType exists in the database
      CString keytext = abilityData.GetName(abType);
      if (!keytext.IsEmpty())
        m_abilityReq[i].m_ability.m_abilityKey = abType;
    }

    if (m_abilityReq[i].m_ability.m_abilityKey==abType)
    {
      return ASSEMBLEABILITYLIMITS(m_abilityReq[i].m_min,
                                   m_abilityReq[i].m_minMod,
                                   m_abilityReq[i].m_max,
                                   m_abilityReq[i].m_maxMod);
    };
  };
  return ASSEMBLEABILITYLIMITS(3,0,18,0);
}

bool BASE_CLASS_DATA::IsAllowedRace(raceType race)
{
  int i,n;
  n = m_allowedRaces.GetSize();
  for (i=0; i<n; i++)
  {
    if (m_allowedRaces[i].m_raceKey == race) return true;
  };
  return false;
}


BASE_CLASS_DATA_TYPE::BASE_CLASS_DATA_TYPE(void)
{
  LoadUADefaults();
}

void BASE_CLASS_DATA_TYPE::LoadUADefaults(void)
{ // You must do your own Clear() if you want it done.
  BASE_CLASS_DATA bc;
  baseclassType i;
  POSITION pos;
  bc.m_name="Fighter";
    baseclassData.AddBaseclass(bc);
  bc.m_name="Cleric";
    baseclassData.AddBaseclass(bc);
  bc.m_name="Ranger";
    baseclassData.AddBaseclass(bc);
  bc.m_name="Paladin";
    baseclassData.AddBaseclass(bc);
  bc.m_name="Magic User";
    baseclassData.AddBaseclass(bc);
  bc.m_name="Thief";
    baseclassData.AddBaseclass(bc);
  bc.m_name="Druid";
    baseclassData.AddBaseclass(bc);
  for (i=0; i<=6; i++)
  {
    pos=m_BaseclassData.FindKeyPos(i);
    ASSERT(pos!=NULL);
    if (pos==NULL) continue;
    m_BaseclassData.GetAt(pos).SetUAAbilityLimits();
    m_BaseclassData.GetAt(pos).SetUAAllowedAlignments();
  };
};

BASE_CLASS_DATA_TYPE::~BASE_CLASS_DATA_TYPE(void)
{
}

void BASE_CLASS_DATA_TYPE::Clear(void)
{
  m_BaseclassData.RemoveAll();
}


int BASE_CLASS_DATA_TYPE::GetDataSize(void)
{
  POSITION pos;
  int size = sizeof (*this);
  pos = m_BaseclassData.GetHeadPosition();
  while (pos!=NULL) size += m_BaseclassData.GetNext(pos).GetDataSize();
  return size;
}



baseclassType BASE_CLASS_DATA_TYPE::GetNextBaseclass()
{
  baseclassType key=baseclassUnknown;
  if (GetCount() > 0)
  {
    key = (baseclassType)m_BaseclassData.GetTail().m_baseclassKey;
    if (key >= UINT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is much greater than
      // MAX_BASE_CLASSES
      // Gaps occur when deletions are made to the baseclass list
      POSITION pos = m_BaseclassData.GetHeadPosition();
      BOOL found = FALSE;        
      DWORD prevKey = 0;
      DWORD currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = m_BaseclassData.GetNext(pos).m_baseclassKey;
        if (currKey != prevKey+1)
        {
          key = (baseclassType)(prevKey+1);
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key=(baseclassType)(key+1);
  }
  else
    key = (baseclassType)0;
  return key;
}


// Called when loading from scripts, or
// when adding from editor GUI
baseclassType BASE_CLASS_DATA_TYPE::AddBaseclass(const BASE_CLASS_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (data.m_name.IsEmpty()) return baseclassUnknown;
  // Do not allow multiple items with same full name.
  if (m_FindName(data.m_name)!=baseclassUnknown) return baseclassUnknown;
  if (m_BaseclassData.GetCount() < MAX_BASE_CLASSES)
  {
    data.m_baseclassKey = GetNextBaseclass();
    m_BaseclassData.Insert(data, data.m_baseclassKey);
    return (baseclassType)data.m_baseclassKey;
  }
  else
    return baseclassUnknown;
}

// called when loading binary baseclass data that
// was saved with key info
baseclassType BASE_CLASS_DATA_TYPE::AddBaseclassWithCurrKey(const BASE_CLASS_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (m_BaseclassData.GetCount() < MAX_BASE_CLASSES)
  {
    m_BaseclassData.Insert(data, data.m_baseclassKey);
    return (baseclassType)data.m_baseclassKey;
  }
  else
    return baseclassUnknown;
}

// With locking...for public use
baseclassType BASE_CLASS_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}



void BASE_CLASS_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  POSITION pos;
  pos=m_BaseclassData.GetHeadPosition();
  while (pos!=NULL)
     m_BaseclassData.GetNext(pos).PreSerialize(IsStoring);
}

void BASE_CLASS_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
}

int BASE_CLASS_DATA_TYPE::Serialize(CAR& car)
{
  POSITION p;
  CString version("BaseclassV1");
  int result=0;

  if (car.IsStoring())
  {
//    car << "BaseclassV0";
    car << version;  // V1 include compression
    car.Compress(true);
    car << GetCount();
    p = m_BaseclassData.GetHeadPosition();
    while (p != NULL)
      m_BaseclassData.GetNext(p).Serialize(car);
  }
  else
  {
    BASE_CLASS_DATA data;
    int count;
    car >> version;
    if ((version =="BaseclassV0")||(version =="BaseclassV1"))
    {
      if (version > "BaseclassV0") car.Compress(true);
      car >> count;
      for (int i=0; i<count; i++)
      {
        result=data.Serialize(car);
        if (result!=0) return result;
        AddBaseclassWithCurrKey(data);
      }
    }
    else
    {
      WriteDebugString("Unknown Baseclass data version = %s\n",version);
      result=1;
    };
  };
  return result;
}


// No locking... for internal use
baseclassType BASE_CLASS_DATA_TYPE::m_FindName (const CString& name)
// Return key associated with the named object, or baseclassUnknown
{
  POSITION pos;
  pos=m_BaseclassData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (m_BaseclassData.GetAt(pos).m_name.CompareNoCase(name)==0) 
          return (baseclassType)m_BaseclassData.GetAt(pos).m_baseclassKey;
    m_BaseclassData.GetNext(pos);
  };
  return baseclassUnknown;
}


ABILITYLIMITS BASE_CLASS_DATA_TYPE::GetAbilityLimits(baseclassType bType, abilityType abType)
{
  POSITION pos;
  pos=m_BaseclassData.FindKeyPos(bType);
  ASSERT(pos!=NULL);
  if (pos==NULL) return ASSEMBLEABILITYLIMITS(3,0,18,0);
  return m_BaseclassData.GetAt(pos).GetAbilityLimits(abType);
}

WORD BASE_CLASS_DATA_TYPE::GetAllowedAlignments(baseclassType bType)
{
  POSITION pos;
  pos=m_BaseclassData.FindKeyPos(bType);
  ASSERT(pos!=NULL);
  if (pos==NULL) return 0x1ff;
  return m_BaseclassData.GetAt(pos).m_allowedAlignments;
}

bool BASE_CLASS_DATA_TYPE::IsAllowedRace(baseclassType baseclass, raceType race)
{
  POSITION pos;
  pos = m_BaseclassData.FindKeyPos(baseclass);
  if (pos == NULL) return false;
  return m_BaseclassData.GetAt(pos).IsAllowedRace(race);
}

WORD BASE_CLASS_DATA_TYPE::GetMaxExpForLevel(baseclassType bType, int level)
{
  POSITION pos=m_BaseclassData.FindKeyPos(bType);
  ASSERT(pos!=NULL);
  if (pos==NULL) return 0;
  return (m_BaseclassData.GetAt(pos).GetMaxExpForLevel(level));
}

WORD BASE_CLASS_DATA_TYPE::GetMinExpForLevel(baseclassType bType, int level)
{
  POSITION pos=m_BaseclassData.FindKeyPos(bType);
  ASSERT(pos!=NULL);
  if (pos==NULL) return 0;
  return (m_BaseclassData.GetAt(pos).GetMinExpForLevel(level));
}


CLASS_DATA::CLASS_DATA(void) 
{
  Clear();
}

CLASS_DATA::~CLASS_DATA(void)
{
}

void CLASS_DATA::Clear(void)
{
  m_baseclasses.RemoveAll();
  m_name="";
  m_classKey = classUnknown;
}

int CLASS_DATA::GetDataSize(void)
{
  int i,n;
  int size = sizeof (*this);
  size += m_name.GetLength();
  n=m_baseclasses.GetSize();
  for (i=0; i<n; i++) size += m_baseclasses[i].GetDataSize();
  return size;
}


void CLASS_DATA::AddBaseclass(baseclassType bclass)
{
  int n;
  BASECLASS_REFERENCE br;
  br.m_baseclassKey=bclass;
  n=m_baseclasses.GetSize();
  for (int i=0; i<n; i++) 
  {
    if (m_baseclasses[i].m_baseclassKey==bclass) return;
  };
  m_baseclasses.SetAtGrow(n,br);
}

void CLASS_DATA::AddBaseclass(BASECLASS_REFERENCE& bclass)
{
  int n;
  n=m_baseclasses.GetSize();
  for (int i=0; i<n; i++) 
  {
    if (m_baseclasses[i].m_baseclassName==bclass.m_baseclassName) return;
  };
  if (bclass.m_baseclassKey==baseclassUnknown)
  {
    bclass.m_baseclassKey =
      baseclassData.FindName(bclass.m_baseclassName);
  }
  m_baseclasses.SetAtGrow(n,bclass);
}



CLASS_DATA& CLASS_DATA::operator =(const CLASS_DATA& src)
{
  int size;
  int i;
  if (this==&src) return *this;
  Clear();
  m_classKey = src.m_classKey;
  m_name     = src.m_name;
  size=src.m_baseclasses.GetSize();
  m_baseclasses.SetSize(size);
  for (i=0; i<size; i++)
    m_baseclasses[i]=src.m_baseclasses[i];
  return *this;
}


ABILITYLIMITS CLASS_DATA::GetAbilityLimits(abilityType abType)
{
  int i,n;
  int min=0,minmod=0,max=9999,maxmod=9999;
  int rmin,rminmod,rmax,rmaxmod;
  ABILITYLIMITS r;
  n=m_baseclasses.GetSize();
  for (i=0; i<n; i++)
  {
    r=baseclassData.GetAbilityLimits(m_baseclasses[i].m_baseclassKey, abType);
    rmin=ABILITYMINBASE(r);
    rminmod=ABILITYMINMOD(r);
    rmax=ABILITYMAXBASE(r);
    rmaxmod=ABILITYMAXMOD(r);
    if (rmin>min) {
      min=rmin;
      minmod=rminmod;
    }
    else if (rmin==min)
    {
      if (rminmod>minmod) minmod=rminmod;
    };
    if (rmax<max) {
      max=rmax;
      maxmod=rmaxmod;
    }
    else if (rmax==max)
    {
      if (rmaxmod>maxmod) maxmod=rmaxmod;
    };
  };
  return ASSEMBLEABILITYLIMITS(min,minmod,max,maxmod);
}

WORD CLASS_DATA::GetAllowedAlignments(void)
{
  int i,n;
  WORD result=0x1ff;
  n=m_baseclasses.GetSize();
  for (i=0; i<n; i++)
  {
    result&=baseclassData.GetAllowedAlignments(m_baseclasses[i].m_baseclassKey);
  };
  return result;
}



bool CLASS_DATA::IsRaceAllowed(raceType race) const 
{ //This race is allowed if we have a single Base Class
  // and the Base Class allows this race.
  // or the race explicitly allows this class.
  //
  baseclassType baseclass;
  if (raceData.IsAllowedClass(race, m_name)) return true;
  if (m_baseclasses.GetSize() != 1) return false;
  baseclass =  m_baseclasses[0].m_baseclassKey;
  return baseclassData.IsAllowedRace(baseclass, race);
}

void CLASS_DATA::PreSerialize(BOOL IsStoring)
{
  int i,n;
  n=m_baseclasses.GetSize();
  for (i=0; i<n; i++) m_baseclasses[i].PreSerialize(IsStoring);
}

int CLASS_DATA::Serialize(CAR &car)
{
  int i,n;
  int result=0;
  CString ver("CL1");

  if (car.IsStoring())
  {
    car << ver; // version
    car << m_classKey;
    if (m_name=="") m_name="*";
    car << m_name;
    if (m_name=="*") m_name="";
    n=m_baseclasses.GetSize();
    car.WriteCount(n);
    for (i=0; i<n; i++) m_baseclasses[i].Serialize(car);
  }
  else
  {
    int key;    
    Clear();
    car >> ver;
    if ((ver=="Bc0")||(ver=="CL1"))
    {
      car >> key;
      m_classKey = (classType)key;
      car >> m_name; if (m_name=="*") m_name="";
      if (ver=="Bc0")
      {
        ABILITY_LIMITS al;
        n=car.ReadCount();
//        m_abilityLimits.SetSize(n);
//        for (i=0; i<n; i++) m_abilityLimits[i].Serialize(ar);
          for (i=0; i<n; i++) al.Serialize(car); // Discard
      };
      n=car.ReadCount();
      m_baseclasses.SetSize(n);
      for (i=0; i<n; i++) 
      {
        result=m_baseclasses[i].Serialize(car);
        if (result!=0) break;
      };
    }
    else
    {
      WriteDebugString("Unknown CLASS_DATA serialize version = %s\n",ver);
      result=1;
    };
#ifdef UAFEngine
    //We have decided not to allow 'Decorated' names for Classes.  prs

    //if (m_name.Find('|') >= 0) m_name = m_name.Left(m_name.Find('|')); // Remove qualifier.
#endif
  };
  return result;
}

void CLASS_DATA_TYPE::Clear(void)
{
  m_ClassData.RemoveAll();
}


int CLASS_DATA_TYPE::GetDataSize(void)
{
  POSITION pos;
  int size = sizeof (*this);
  pos = m_ClassData.GetHeadPosition();
  while (pos!=NULL) size += m_ClassData.GetNext(pos).GetDataSize();
  return size;
}

CLASS_DATA_TYPE::CLASS_DATA_TYPE(void)
{
  LoadUADefaults();
}


void CLASS_DATA_TYPE::LoadUADefaults(void)
{ // You must do your own Clear() if you want it done.
  CLASS_DATA cd;
  cd.Clear();
  cd.m_name="Fighter";
  cd.AddBaseclass(Fighter);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Cleric";
  cd.AddBaseclass(Cleric);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Ranger";
  cd.AddBaseclass(Ranger);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Paladin";
  cd.AddBaseclass(Paladin);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Magic User";
  cd.AddBaseclass(MagicUser);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Thief";
  cd.AddBaseclass(Thief);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Druid";
  cd.AddBaseclass(Druid);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Cleric/Fighter";
  cd.AddBaseclass(Cleric);
  cd.AddBaseclass(Fighter);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Cleric/Fighter/Magic User";
  cd.AddBaseclass(Cleric);
  cd.AddBaseclass(Fighter);
  cd.AddBaseclass(MagicUser);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Cleric/Ranger"; 
  cd.AddBaseclass(Cleric);
  cd.AddBaseclass(Ranger);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Cleric/Magic User"; 
  cd.AddBaseclass(Cleric);
  cd.AddBaseclass(MagicUser);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Cleric/Thief";
  cd.AddBaseclass(Cleric);
  cd.AddBaseclass(Thief);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Fighter/Magic User";
  cd.AddBaseclass(Fighter);
  cd.AddBaseclass(MagicUser);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Fighter/Thief";
  cd.AddBaseclass(Fighter);
  cd.AddBaseclass(Thief);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Fighter/Magic User/Thief";
  cd.AddBaseclass(Fighter);
  cd.AddBaseclass(MagicUser);
  cd.AddBaseclass(Thief);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Magic User/Thief";
  cd.AddBaseclass(MagicUser);
  cd.AddBaseclass(Thief);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Fighter/Druid"; 
  cd.AddBaseclass(Fighter);
  cd.AddBaseclass(Druid);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Druid/Magic User"; 
  cd.AddBaseclass(Druid);
  cd.AddBaseclass(MagicUser);
  classData.AddClass(cd);

  cd.Clear();
  cd.m_name="Fighter/Magic User/Druid";
  cd.AddBaseclass(Fighter);
  cd.AddBaseclass(MagicUser);
  cd.AddBaseclass(Druid);
  classData.AddClass(cd);
}

CLASS_DATA_TYPE::~CLASS_DATA_TYPE(void)
{
}

// No locking... for internal use
classType CLASS_DATA_TYPE::m_FindName (const CString& name)
// Return key associated with the named object, or classUnknown
{
  POSITION pos;
  pos=m_ClassData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (m_ClassData.GetAt(pos).m_name.CompareNoCase(name)==0) 
          return (classType)m_ClassData.GetAt(pos).m_classKey;
    m_ClassData.GetNext(pos);
  };
  return classUnknown;
}

// With locking...for public use
classType CLASS_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}

CLASS_DATA& CLASS_DATA_TYPE::operator [] (classType ctype)
{
  POSITION pos=m_ClassData.FindKeyPos(ctype);
  ASSERT(pos!=NULL);
  return m_ClassData.GetAt(pos);
}

ABILITYLIMITS CLASS_DATA_TYPE::GetAbilityLimits(classType ctype, abilityType abtype)
{
  POSITION pos;
  pos=m_ClassData.FindKeyPos(ctype);
  ASSERT(pos!=NULL);
  if (pos==NULL) return 1;
  return m_ClassData.GetAt(pos).GetAbilityLimits(abtype);
}

WORD CLASS_DATA_TYPE::GetAllowedAlignments(classType ctype)
{
  POSITION pos;
  pos=m_ClassData.FindKeyPos(ctype);
  ASSERT(pos!=NULL);
  if (pos==NULL) return 0x1ff;
  return m_ClassData.GetAt(pos).GetAllowedAlignments();
}


classType CLASS_DATA_TYPE::GetNextClass()
{
  classType key=classUnknown;
  if (GetCount() > 0)
  {
    key = (classType)m_ClassData.GetTail().m_classKey;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is much greater than
      // MAX_CLASSES.
      // Gaps occur when deletions are made to the classes list
      POSITION pos = m_ClassData.GetHeadPosition();
      BOOL found = FALSE;        
      DWORD prevKey = 0;
      DWORD currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = m_ClassData.GetNext(pos).m_classKey;
        if (currKey != prevKey+1)
        {
          key = (classType)(prevKey+1);
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key=(classType)(key+1);
  }
  else
    key = (classType)0;
  return key;
}

// called when loading binary class data that
// was saved with key info
classType CLASS_DATA_TYPE::AddClassWithCurrKey(const CLASS_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (m_ClassData.GetCount() < MAX_CLASSES)
  {
    m_ClassData.Insert(data, data.m_classKey);
    return (classType)data.m_classKey;
  }
  else
    return classUnknown;
}


// Called when loading from scripts, or
// when adding from editor GUI
classType CLASS_DATA_TYPE::AddClass(const CLASS_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (data.m_name.IsEmpty()) return classUnknown;
  // Do not allow multiple items with same full name.
  if (m_FindName(data.m_name)!=classUnknown) return classUnknown;
  if (m_ClassData.GetCount() < MAX_CLASSES)
  {
    data.m_classKey = GetNextClass();
    m_ClassData.Insert(data, data.m_classKey);
    return (classType)data.m_classKey;
  }
  else
    return classUnknown;
}


bool CLASS_DATA_TYPE::IsAllowedRace(classType Class, raceType race)
{
  POSITION p;
  p = GetHeadPosition();
  while (p!=NULL)
  {
    if (GetKeyAt(p) == Class)
    { //We now have found the class of interest.
      return GetAt(p).IsRaceAllowed(race);
    };
    GetNext(p);
  };
  return false; //Ain't no such class.  So There!
}



void CLASS_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  POSITION pos;
  pos=m_ClassData.GetHeadPosition();
  while (pos!=NULL)
     m_ClassData.GetNext(pos).PreSerialize(IsStoring);
}

void CLASS_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
}

int CLASS_DATA_TYPE::Serialize(CAR& car)
{
  POSITION p;
  CString version("ClassV1");
  int result=0;

  if (car.IsStoring())
  {
//    car << "ClassV0";
    car << version; // V1 is compressed
    car.Compress(true);
    car << GetCount();
    p = m_ClassData.GetHeadPosition();
    while (p != NULL)
      m_ClassData.GetNext(p).Serialize(car);
  }
  else
  {
    CLASS_DATA data;
    int count;
    car >> version;
    if ((version =="ClassV0")||(version=="ClassV1"))
    {
      if (version > "ClassV0") car.Compress(true);
      car >> count;
      for (int i=0; i<count; i++)
      {
        result=data.Serialize(car);
        if (result!=0) break;
        AddClassWithCurrKey(data);
      }
    }
    else
    {
      WriteDebugString("Unknown Class data version = %s",version);
      result=1;
    };
  };
  return result;
}

ALLOWED_CLASSES::ALLOWED_CLASSES(void)
{
  m_allowedClasses = NULL;
  m_numAllowedClasses = 0;
}

ALLOWED_CLASSES::~ALLOWED_CLASSES(void)
{
  if (m_allowedClasses != NULL) free(m_allowedClasses);
  m_allowedClasses = NULL;
}

void ALLOWED_CLASSES::Initialize(void)
{
  POSITION p;
  int i;
  //Remove any old list we might have.
  if (m_allowedClasses != NULL) free(m_allowedClasses);
  //How many classes are there?
  m_numAllowedClasses = classData.GetCount();
  m_allowedClasses = 
    (classType*)malloc(m_numAllowedClasses*sizeof(*m_allowedClasses));
  if (m_allowedClasses == NULL)
  {
    m_numAllowedClasses = 0;
    return;
  };
  p = classData.GetHeadPosition();
  i=0;
  while (p != NULL)
  {
    if (i >= m_numAllowedClasses) break; //Safety first.  Should not need this,
    m_allowedClasses[i++] = classData.GetNext(p).WhatIsYourClassType();
  };
}

void ALLOWED_CLASSES::RestrictByRace(raceType race)
{
  int i;
  for (i=NumAllowedClasses()-1; i>=0; i--)
  {
    if (!classData.IsAllowedRace(m_allowedClasses[i], race))
            RemoveAt(i);
  }; //That was easy.
}



void ALLOWED_CLASSES::RemoveAt(int i)
{
  ASSERT(i < m_numAllowedClasses);
  m_numAllowedClasses--;
  for (; i<m_numAllowedClasses; i++) 
        m_allowedClasses[i] = m_allowedClasses[i+1];
}

classType ALLOWED_CLASSES::GetAt(int i) const 
{
  if (i < m_numAllowedClasses) return m_allowedClasses[i];
  return classUnknown;
}

int ALLOWED_CLASSES::NumAllowedClasses(void) const
{
  return m_numAllowedClasses;
}



TRAIT_DATA::TRAIT_DATA (void)
{
  Clear();
}

TRAIT_DATA::TRAIT_DATA(TRAIT_DATA& src)
{
  *this=src;
}

TRAIT_DATA::~TRAIT_DATA (void)
{
}

TRAIT_DATA& TRAIT_DATA::operator = (const TRAIT_DATA& src)
{
  if (&src==this) return *this;
  Clear();
  m_traitKey=src.m_traitKey;
  m_name=src.m_name;
  m_abbreviation=src.m_abbreviation;
  m_roll=src.m_roll;
  return *this;
}

void TRAIT_DATA::PreSerialize(BOOL IsStoring)
{
}

int TRAIT_DATA::Serialize(CAR& car)
{
  int i,n;
  int result=0;
  CString tempStr("Tr1");

  DICEPLUSADJLIST m_adjustments;

  if (car.IsStoring())
  {
    car << tempStr;  // Version
    car << m_traitKey;
    AS(car, m_name);
    AS(car, m_abbreviation);
    m_roll.Serialize(car);
  }
  else 
  {    
    Clear();
    car >> tempStr;
    if (tempStr=="Tr0")
    {
      car >> m_traitKey;
      DAS(car, m_name);
      DAS(car, m_abbreviation);

      n=car.ReadCount();
      m_adjustments.SetSize(n);
      for (i=0; i<n; i++) 
      {
        result=m_adjustments[i].Serialize(car);
        if (result!=0) return result;
      };

      CString diceresult("");
      int num=m_adjustments.GetSize();
      if (num > 0)
      {
        diceresult += m_adjustments[0].EncodeOperator(m_adjustments[0].m_operator[0]);

        // for things like 3*level
        for (i=1; i<num; i++)
        {
          ADJUSTMENT adj = m_adjustments[i];
          diceresult += adj.EncodeText();
        }
      }
      m_roll.m_Text = diceresult;
    }
    else if (tempStr=="Tr1")
    {
      car >> m_traitKey;
      DAS(car, m_name);
      DAS(car, m_abbreviation);
      m_roll.Serialize(car);
    }
    else
    {
      WriteDebugString("Unknown TRAIT_DATA serialize version = %s\n",tempStr);
      result=1;
    };
  };
  return result;
}

void TRAIT_DATA::Clear(void)
{
  m_name="";
  m_abbreviation="";
  m_roll.Clear();
}

void TRAIT_DATA::AddAdjust(CString &name)
{
  m_roll.AddAdjust(name);
}

BOOL TRAIT_DATA::EvaluateTrait(double &result)
{
  return (m_roll.Roll(result));
}


TRAIT_DATA_TYPE::TRAIT_DATA_TYPE (void)
{
  Clear();
}

TRAIT_DATA_TYPE::~TRAIT_DATA_TYPE(void)
{
}

void TRAIT_DATA_TYPE::Clear(void)
{
  m_TraitData.RemoveAll();
}

// With locking...for public use
traitType TRAIT_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}

void TRAIT_DATA_TYPE::LoadUADefaults(void)
{
}

// called when loading binary trait data that
// was saved with key info
traitType TRAIT_DATA_TYPE::AddTraitWithCurrKey(const TRAIT_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (m_TraitData.GetCount() < MAX_TRAITS)
  {
    // over-ride if key already exists.
    //
    // A bug in versions < 0.900 didn't copy m_traitKey
    // in operator= so all inserts ended up using the
    // same m_traitKey in the binary file.
    if (m_TraitData.HaveKey(data.m_traitKey))
    {
      data.m_traitKey = GetNextTrait();
    }

    m_TraitData.Insert(data, data.m_traitKey);
    return (traitType)data.m_traitKey;
  }
  else
    return traitUnknown;
}


traitType TRAIT_DATA_TYPE::AddTrait(const TRAIT_DATA& newtr)
// Called when loading from scripts, or
// when adding from editor GUI
{
  CSingleLock sLock(&m_CS, TRUE);
  if (newtr.m_name.IsEmpty()) return traitUnknown;
  // Do not allow multiple items with same full name.
  if (m_FindName(newtr.m_name)!=traitUnknown) return traitUnknown;
  if (m_TraitData.GetCount() < MAX_TRAITS)
  {
    newtr.m_traitKey = GetNextTrait();
    m_TraitData.Insert(newtr, newtr.m_traitKey);
    return (traitType)newtr.m_traitKey;
  }
  else
    return traitUnknown;
}

traitType TRAIT_DATA_TYPE::GetNextTrait()
{
  traitType key=traitUnknown;
  if (GetCount() > 0)
  {
    key = (traitType)m_TraitData.GetTail().m_traitKey;
    if (key >= UINT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is much greater than
      // MAX_TRAITS.
      // Gaps occur when deletions are made to the traits list
      POSITION pos = m_TraitData.GetHeadPosition();
      BOOL found = FALSE;        
      DWORD prevKey = 0;
      DWORD currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = m_TraitData.GetNext(pos).m_traitKey;
        if (currKey != prevKey+1)
        {
          key = (traitType)(prevKey+1);
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key=(traitType)(key+1);
  }
  else
    key = (traitType)1;
  return key;
}

// No locking... for internal use
traitType TRAIT_DATA_TYPE::m_FindName (const CString& name)
// Return key associated with the named object, or traitUnknown
{
  POSITION pos;
  pos=m_TraitData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (m_TraitData.GetAt(pos).m_name.CompareNoCase(name)==0) 
          return (traitType)m_TraitData.GetAt(pos).m_traitKey;
    m_TraitData.GetNext(pos);
  };
  return traitUnknown;
}

void TRAIT_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  POSITION pos;
  pos=m_TraitData.GetHeadPosition();
  while (pos!=NULL)
     m_TraitData.GetNext(pos).PreSerialize(IsStoring);
}

void TRAIT_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
}

int TRAIT_DATA_TYPE::Serialize(CAR& car)
{
  POSITION p;
  CString version("TraitV1");
  int result=0;

  if (car.IsStoring())
  {
    car << version; // V1 is compressed
//    car << "TraitV0";
    car.Compress(true);
    car << GetCount();
    p = m_TraitData.GetHeadPosition();
    while (p != NULL)
      m_TraitData.GetNext(p).Serialize(car);
  }
  else
  {
    TRAIT_DATA data;
    int count;
    car >> version;
    if ((version =="TraitV0")||(version=="TraitV1"))
    {
      if (version > "TraitV0") car.Compress(true);
      car >> count;
      for (int i=0; i<count; i++)
      {
        result=data.Serialize(car);
        if (result!=0) break;
        AddTraitWithCurrKey(data);
      }
    }
    else
    {
      WriteDebugString("Unknown Trait data version = %s",version);
      result=1;
    };
  };
  return result;
}

BOOL TRAIT_DATA_TYPE::EvaluteTrait(traitType trait, double &result)
{  
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = m_TraitData.FindKeyPos(trait)) != NULL)
  {
    return (m_TraitData.GetAt(pos).EvaluateTrait(result));
  }
  else
    return FALSE;
}

SPELLGROUP_DATA::SPELLGROUP_DATA (void)
{
  Clear();
}

SPELLGROUP_DATA::~SPELLGROUP_DATA (void)
{
}


SPELLGROUP_DATA& SPELLGROUP_DATA::operator = (const SPELLGROUP_DATA& src)
{
  int i, n;
  if (&src==this) return *this;
  Clear();
  m_name=src.m_name;
  m_spellgroupKey=src.m_spellgroupKey;
  n=src.m_spells.GetSize();
  m_spells.SetSize(n);
  for (i=0; i<n; i++) m_spells[i]=src.m_spells[i];
  n=src.m_spellgroups.GetSize();
  m_spellgroups.SetSize(n);
  for (i=0; i<n; i++) m_spellgroups[i]=src.m_spellgroups[i];
  return *this;
}

void SPELLGROUP_DATA::PreSerialize(BOOL IsStoring)
{
  int i, n;  
  n=m_spells.GetSize();
  for (i=0; i<n; i++) m_spells[i].PreSerialize(IsStoring);
  n=m_spellgroups.GetSize();
  for (i=0; i<n; i++) m_spellgroups[i].PreSerialize(IsStoring);
}

int SPELLGROUP_DATA::Serialize(CAR& car)
{
  int i,n;
  int result=0;
  CString tempStr("SG0");
  if (car.IsStoring())
  {
    car << tempStr;  // Version
    car << m_spellgroupKey;
    if (m_name=="") m_name="*";
    car << m_name;
    if (m_name=="*") m_name="";
    n=m_spells.GetSize();
    car.WriteCount(n);
    for (i=0; i<n; i++) m_spells[i].Serialize(car);
    n=m_spellgroups.GetSize();
    car.WriteCount(n);
    for (i=0; i<n; i++) m_spellgroups[i].Serialize(car);
  }
  else 
  {    
    Clear();
    car >> tempStr;
    if (tempStr=="SG0")
    {
      car >> m_spellgroupKey;
      car >> m_name; if (m_name=="*") m_name="";
      n=car.ReadCount();
      m_spells.SetSize(n);
      for (i=0; i<n; i++) 
      {
        result=m_spells[i].Serialize(car);
        if (result!=0) return result;
      };
      n=car.ReadCount();
      m_spellgroups.SetSize(n);
      for (i=0; i<n; i++) 
      {
        m_spellgroups[i].Serialize(car);
        if (result!=0) return result;
      };
    }
    else
    {
      WriteDebugString("Unknown SPELLGROUP_DATA serialize version = %s\n",tempStr);
      result=1;
    };
  };
  return result;
}

void SPELLGROUP_DATA::Clear(void)
{
  m_name="";
  m_spells.RemoveAll();
  m_spellgroups.RemoveAll();
}

int SPELLGROUP_DATA::GetDataSize(void) 
{
  int i, n;
  int size=sizeof *(this);
  n=m_spells.GetSize();
  for (i=0; i<n; i++) size+=m_spells[i].GetDataSize();
  n=m_spellgroups.GetSize();
  for (i=0; i<n; i++) size+=m_spellgroups[i].GetDataSize();
  return size;
}


void SPELLGROUP_DATA::AddSpell(const CString& name)
{
  int i,n;
  n=m_spells.GetSize();
  for (i=0; i<n; i++) if (m_spells[i].m_spellName==name) return;
  SPELL_REFERENCE temp;
  temp.m_spellName=name;
  m_spells.SetAtGrow(n,temp);
}

void SPELLGROUP_DATA::AddSpellgroup(const CString& name)
{
  int i,n;
  n=m_spellgroups.GetSize();
  for (i=0; i<n; i++) if (m_spellgroups[i].m_spellgroupName==name) return;
  SPELLGROUP_REFERENCE temp;
  temp.m_spellgroupName=name;
  m_spellgroups.SetAtGrow(n,temp);
}



SPELLGROUP_DATA_TYPE::SPELLGROUP_DATA_TYPE (void)
{
  Clear();
}

SPELLGROUP_DATA_TYPE::~SPELLGROUP_DATA_TYPE(void)
{
}

void SPELLGROUP_DATA_TYPE::Clear(void)
{
  m_SpellgroupData.RemoveAll();
}

void SPELLGROUP_DATA_TYPE::LoadUADefaults(void)
{
}


int SPELLGROUP_DATA_TYPE::GetDataSize(void) 
{
  int size=sizeof (*this);
  POSITION pos;
  for (pos=m_SpellgroupData.GetHeadPosition(); 
       pos!=NULL; 
       size+=m_SpellgroupData.GetNext(pos).GetDataSize())
         ;
  return size;
}



// With locking...for public use
spellgroupType SPELLGROUP_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}



// called when loading binary spellgroup data that
// was saved with key info
spellgroupType SPELLGROUP_DATA_TYPE::AddSpellgroupWithCurrKey(const SPELLGROUP_DATA &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  if (m_SpellgroupData.GetCount() < MAX_SPELLGROUPS)
  {
    m_SpellgroupData.Insert(data, data.m_spellgroupKey);
    return (spellgroupType)data.m_spellgroupKey;
  }
  else
    return spellgroupUnknown;
}


spellgroupType SPELLGROUP_DATA_TYPE::AddSpellgroup(const SPELLGROUP_DATA& newsg)
// Called when loading from scripts, or
// when adding from editor GUI
{
  CSingleLock sLock(&m_CS, TRUE);
  if (newsg.m_name.IsEmpty()) return spellgroupUnknown;
  // Do not allow multiple items with same full name.
  if (m_FindName(newsg.m_name)!=spellgroupUnknown) return spellgroupUnknown;
  if (m_SpellgroupData.GetCount() < MAX_SPELLGROUPS)
  {
    newsg.m_spellgroupKey = GetNextSpellgroup();
    m_SpellgroupData.Insert(newsg, newsg.m_spellgroupKey);
    return (spellgroupType)newsg.m_spellgroupKey;
  }
  else
    return spellgroupUnknown;
}

spellgroupType SPELLGROUP_DATA_TYPE::GetNextSpellgroup()
{
  spellgroupType key=spellgroupUnknown;
  if (GetCount() > 0)
  {
    key = (spellgroupType)m_SpellgroupData.GetTail().m_spellgroupKey;
    if (key >= UINT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is much greater than
      // MAX_SPELLGROUPS.
      // Gaps occur when deletions are made to the traits list
      POSITION pos = m_SpellgroupData.GetHeadPosition();
      BOOL found = FALSE;        
      DWORD prevKey = 0;
      DWORD currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = m_SpellgroupData.GetNext(pos).m_spellgroupKey;
        if (currKey != prevKey+1)
        {
          key = (spellgroupType)(prevKey+1);
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key=(spellgroupType)(key+1);
  }
  else
    key = (spellgroupType)1;
  return key;
}

// No locking... for internal use
spellgroupType SPELLGROUP_DATA_TYPE::m_FindName (const CString& name)
// Return key associated with the named object, or spellgroupUnknown
{
  POSITION pos;
  pos=m_SpellgroupData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (m_SpellgroupData.GetAt(pos).m_name.CompareNoCase(name)==0) 
          return (spellgroupType)m_SpellgroupData.GetAt(pos).m_spellgroupKey;
    m_SpellgroupData.GetNext(pos);
  };
  return spellgroupUnknown;
}

void SPELLGROUP_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  POSITION pos;
  pos=m_SpellgroupData.GetHeadPosition();
  while (pos!=NULL)
     m_SpellgroupData.GetNext(pos).PreSerialize(IsStoring);
}

void SPELLGROUP_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
}

int SPELLGROUP_DATA_TYPE::Serialize(CAR& car)
{
  POSITION p;
  CString version("SpGrpV1");
  int result=0;

  if (car.IsStoring())
  {
//    car << "SpGrpV0";
    car << version;  // V1 includes compression
    car.Compress(true);
    car << GetCount();
    p = m_SpellgroupData.GetHeadPosition();
    while (p != NULL)
      m_SpellgroupData.GetNext(p).Serialize(car);
  }
  else
  {
    SPELLGROUP_DATA data;
    int count;
    car >> version;
    if ((version =="SpGrpV0")||(version=="SpGrpV1"))
    {
      if (version > "SpGrpV0") car.Compress(true);
      car >> count;
      for (int i=0; i<count; i++)
      {
        result=data.Serialize(car);
        if (result!=0) break;
        AddSpellgroupWithCurrKey(data);
      }
    }
    else
    {
      WriteDebugString("Unknown Spell group data version = %s",version);
      result=1;
    };
  };
  return result;
}

CAR::CAR(CFile *pFile, UINT nMode) :  CArchive(pFile, nMode)
{
  m_compressType=0; // No compression yet.
  m_nextIndex=1;
  m_cData=NULL;
  m_dData=NULL;
  m_codes=NULL;
  m_w=0;
  m_bufferIndex=0;
  memset(m_buffer,0,sizeof(m_buffer));
  m_stkLen=0;
  m_OC=0;
  m_numCode=0;
  m_C=0;
}


CAR::~CAR(void)
{
  // Make sure we flush the output buffer
  Flush();
  if (m_cData!=NULL) delete m_cData;
  if (m_dData!=NULL) delete m_dData;
  if (m_codes!=NULL) delete m_codes;
}

void CAR::Flush(void)
{
  if (!IsStoring()) return;
  if (m_compressType==0) return;
  // And output the old string from w.
  *((unsigned int *)(((char *)m_buffer)+(m_bufferIndex>>3)))|=m_w<<(m_bufferIndex&7);
  m_bufferIndex=(m_bufferIndex+13)%416;
  if (m_bufferIndex==0)
  {
    Write(m_buffer,sizeof(m_buffer));
    memset(m_buffer,0,sizeof(m_buffer));
  };
// We need at least one EOF character.  if (m_bufferIndex==0) return;
  m_w=8191;
  do
  {
    *((unsigned int *)(((char *)m_buffer)+(m_bufferIndex>>3)))|=m_w<<(m_bufferIndex&7);
    m_bufferIndex=(m_bufferIndex+13)%416;
  } while  (m_bufferIndex!=0);
  Write(m_buffer,sizeof(m_buffer));
}

void CAR::Close(void)
{
  Flush();
  m_compressType=0;
  CArchive::Close();
}


void CAR::Compress(bool compression)
{
  m_compressType=0;
  if (compression)
  {
    m_codes=new CODES;
    m_w=0xffff;
    m_bufferIndex=0;
    m_stkLen=0;
    m_OC=0xffff;
    m_numCode=256;
    if (IsStoring())
    {
      *this << (char) 1; // Compression type in clear
      memset(m_buffer,0,sizeof(m_buffer));
      m_compressType=1;
      m_cData=new CDATA; // Compression Data
    }
    else
    { 
      m_dData=new DDATA; // Decompression Data
      *this >> m_compressType;
    };
  }
  else
  {
  if (m_cData!=NULL) delete m_cData;
  if (m_dData!=NULL) delete m_dData;
  if (m_codes!=NULL) delete m_codes;
  m_cData=NULL;
  m_dData=NULL;
  m_codes=NULL;
  };

}

void CAR::WriteCount(int n)
{
  if (m_compressType==0) CArchive::WriteCount(n);
  else operator << (n);
}

void CAR::WriteCount(unsigned int n)
{
  if (m_compressType==0) CArchive::WriteCount(n);
  else operator << (n);
}

DWORD CAR::ReadCount(void)
{
  if (m_compressType==0) 
  {
    return CArchive::ReadCount();
  }
  else
  {
    DWORD temp;
    operator >> (temp);
    return temp;
  };
}

CAR& CAR::operator << (int Int)
{
  if (m_compressType==0) CArchive::operator << (Int);
  else compress((char *)(&Int),4);
  return *this;
}

CAR& CAR::operator >> (int& Int)
{
  if (m_compressType==0) CArchive::operator >> (Int);
  else decompress((char *)(&Int),4);
  return *this;
}


CAR& CAR::operator << (unsigned int UInt)
{
  if (m_compressType==0) CArchive::operator << (UInt);
  else compress((char *)(&UInt),4);
  return *this;
}

CAR& CAR::operator >> (unsigned int& UInt)
{
  if (m_compressType==0) CArchive::operator >> (UInt);
  else decompress((char *)(&UInt),4);
  return *this;
}

CAR& CAR::operator << (unsigned long ULong)
{
  return operator << ((unsigned int) ULong);
}

CAR& CAR::operator >> (unsigned long& ULong)
{
  return operator >> ((unsigned int&) ULong);
}

CAR& CAR::operator << (long Long)
{
  return operator << ((int) Long);
}

CAR& CAR::operator >> (long& Long)
{
  return operator >> ((int&) Long);
}

CAR& CAR::operator << (double Double) 
{
  if (m_compressType==0) CArchive::operator << (Double);
  else compress ((char *)(&Double), 8);
  return *this;
}

CAR& CAR::operator >> (double& Double) 
{
  if (m_compressType==0) CArchive::operator >> (Double);
  else decompress ((char *)(&Double), 8);
  return *this;
}
CAR& CAR::operator << (float Float) 
{
  if (m_compressType==0) CArchive::operator << (Float);
  else compress ((char *)(&Float), 4);
  return *this;
}

CAR& CAR::operator >> (float& Float) 
{
  if (m_compressType==0) CArchive::operator >> (Float);
  else decompress ((char *)(&Float), 4);
  return *this;
}
CAR& CAR::operator << (BYTE Byte) 
{
  if (m_compressType==0) CArchive::operator << (Byte);
  else compress ((char *)(&Byte), 1);
  return *this;
}

CAR& CAR::operator >> (BYTE& Byte) 
{
  if (m_compressType==0) CArchive::operator >> (Byte);
  else decompress((char *)(&Byte), 1);
  return *this;
}

CAR& CAR::operator << (char Char) 
{
  if (m_compressType==0) CArchive::operator << (Char);
  else compress (&Char, 1);
  return *this;
}

CAR& CAR::operator >> (char& Char) 
{
  if (m_compressType==0) CArchive::operator >> (Char);
  else decompress(&Char, 1);
  return *this;
}

CAR& CAR::operator << (short Short) 
{
  if (m_compressType==0) CArchive::operator << (Short);
  else compress((char *)(&Short), 2);
  return *this;
}

CAR& CAR::operator >> (short& Short) 
{
  if (m_compressType==0) CArchive::operator >> (Short);
  else decompress((char *)(&Short), 2);
  return *this;
}

CAR& CAR::operator << (unsigned short UShort) 
{
  if (m_compressType==0) CArchive::operator << (UShort);
  else compress((char *)(&UShort), 2);
  return *this;
}

CAR& CAR::operator >> (unsigned short& UShort) 
{
  if (m_compressType==0) CArchive::operator >> (UShort);
  else decompress((char *)(&UShort), 2);
  return *this;
}

CAR& CAR::operator << (const char *str)
{
  CString temp(str);
  *this << temp;
  return *this;
}

CAR& CAR::operator << (CString& str)
{ // Keeping a list of strings not only shortens the output file but
  // perhaps more importantly it causes all identical strings to
  // share the same memory when the file is loaded.  It thereby makes
  // string loading faster, too.
  if (m_compressType==0) 
  {
    *(CArchive*)(this) << str;
    return *this;
  };
  unsigned int index;
  if (stringIndex.Lookup(str,index))
  {
    operator << (index);
    return *this;
  };
  index=0;
  operator << (index);
//  *((CArchive *) this) << str;
  int len=str.GetLength();
  operator << (len);
  compress((LPCSTR)str, len);
  stringIndex.SetAt(str,m_nextIndex);
  m_nextIndex++;
  return *this;
}

CAR& CAR::operator >> (CString& str)
{
  if (m_compressType==0)
  {
    *(CArchive *)this >> str;
    return *this;
  };
  unsigned int index;
  operator >> (index);
  if (index==0) 
  { // A new string
//    *(CArchive *)this >> str;
    int len;
    char *temp=NULL;
    operator >> (len);
    if (len==0)
    {
      str="";
    }
    else
    {
      temp = (char *)malloc(len);
      if (temp==NULL)
      {
        MsgBoxError("Memory exhausted");
        str="";
      }
      else
      {
        decompress(temp,len);
        str = CString(temp, len);
      };
    };
    m_stringArray.SetAtGrow(m_nextIndex,str);
    m_nextIndex++;
    if (temp!=NULL) free(temp);
    return *this;
  };
  str = m_stringArray[index];
  return *this;
}

CAR& CAR::operator << (CArray<WORD,WORD>& warray)
{
  if (m_compressType==0)
  {
    warray.Serialize(*(CArchive *)this);
    return *this;
  };
  *this << warray.GetSize();
  compress((char *)(&warray[0]),warray.GetSize()*sizeof(WORD));
  return *this;
}

CAR& CAR::operator >> (CArray<WORD,WORD>& warray)
{
  if (m_compressType==0)
  {
    warray.Serialize(*(CArchive *)this);
    return *this;
  };
  int size;
  *this >> size;
  warray.SetSize(size);
  decompress((char *)(&warray[0]),size*sizeof(WORD));
  return *this;
}


CAR& CAR::operator << (ASL& asl)
{
  if (m_compressType==0)
  {
    asl.Serialize(*(CArchive *)this);
    return *this;
  };
  POSITION pos;
  CString key;
  ASL_VALUE value;
  *this << asl.m_mapName;
  *this << asl.m_aslMap.GetCount();
  pos = asl.m_aslMap.GetStartPosition();
  while (pos!= NULL)
  {
    asl.m_aslMap.GetNextAssoc(pos, key, value);
    *this << key;
    *this << value.m_flags;
    *this << value.m_value;
  };
  return *this;
}

CAR& CAR::operator >> (ASL& asl)
{
  if (m_compressType==0)
  {
    asl.Serialize(*(CArchive *)this);
    return *this;
  };
  CString key;
  ASL_VALUE value;
  int count;
  *this >> key;
  if (key!=asl.m_mapName) 
  {
	  CString msg;
	  msg.Format("Expected:\n%s\nEncountered:\n%s\nwhile reading file.",
		            asl.m_mapName,key);
    WriteDebugString(msg);
	  MsgBoxError(msg);
		AfxThrowArchiveException(CArchiveException::badIndex,
               GetFile()->GetFileName());
  };
  *this >> count;
  asl.m_aslMap.RemoveAll();
  for (;count>0; count--)
  {
    *this >> key;
    *this >> value.m_flags;
    *this >> value.m_value;
    asl.m_aslMap.SetAt(key,value);
  };
  return *this;
}



void CAR::compress(const char *chars, int n)
{
//  static unsigned int buffer[13];
//  static unsigned int bufferIndex=0;
//  static unsigned _int16 w=0xffff;
  unsigned __int16 cd;
  unsigned char c;
  for (;n>0; n--, chars++)
  {
    if (m_codes->Full())
    {
      // And output the old code from w.
      *((unsigned int *)(((char *)m_buffer)+(m_bufferIndex>>3)))|=m_w<<(m_bufferIndex&7);
      m_bufferIndex=(m_bufferIndex+13)%416;
      if (m_bufferIndex==0)
      {
        Write(m_buffer,52);
        memset(m_buffer,0,52);
      };
      m_w=0xffff;
      m_codes->Clear();
      // And output the RESET code
      *((unsigned int *)(((char *)m_buffer)+(m_bufferIndex>>3)))|=8190<<(m_bufferIndex&7);
      m_bufferIndex=(m_bufferIndex+13)%416;
      if (m_bufferIndex==0)
      {
        Write(m_buffer,52);
        memset(m_buffer,0,52);
      };
    };
    c=*chars;
    if (m_w==0xffff) 
    { // Single characters are in code list by definition.
      m_w=c;
      continue;
    };
    if (((cd=m_codes->FindCode((m_w<<8)|c))!=0) /*&& (c<256) What was this for????*/)
    {
      // We found the new, larger string in the table.
      m_w=cd;
      if (m_w > 8189) // 8191 reserved for EOF, 8190 for RESET
      {
        _exit(1);
      };
      continue;
    };
    // We want to add the new, larger string to the table.
    m_codes->AddCode((m_w<<8)|c);
    // And output the old string from m_w.
    *((unsigned int *)(((char *)m_buffer)+(m_bufferIndex>>3)))|=m_w<<(m_bufferIndex&7);
    m_bufferIndex=(m_bufferIndex+13)%416;
    if (m_bufferIndex==0)
    {
      Write(m_buffer,sizeof(m_buffer));
      memset(m_buffer,0,sizeof(m_buffer));
    };
    m_w=c;
  };
}

void CAR::decompress(char *chars, int n)
{
 // static unsigned _int16 OC=0xffff;
 // static unsigned _int16 prefix[8192];
 // static unsigned char   postfix[8192];
 // static unsigned int numCode=256;
    unsigned __int16 NC;
 // static unsigned char stack[1000];
 // static unsigned char C;
 // static unsigned int stkLen=0;
    unsigned int code;
 // static unsigned int buffer[13];
 // static unsigned int bufferIndex=0;
  unsigned __int16 theN;
  for (;n>0; n--, chars++)
  {
    if (m_stkLen!=0) 
    {
      *chars = m_dData->m_stack[--m_stkLen];
      continue;
    };
    if (m_bufferIndex==0)
    {
      if (Read(m_buffer,sizeof(m_buffer))!=sizeof(m_buffer)) return;
    };
    theN = (unsigned __int16)(((*((unsigned int *)(((char *)m_buffer)+(m_bufferIndex>>3))))>>(m_bufferIndex&7))&0x1fff);
    m_bufferIndex=(m_bufferIndex+13)%416;
    while (theN==8190) // RESET
    {
      m_OC=0xffff;
      m_numCode=256;
      if (m_bufferIndex==0)
      {
        if (Read(m_buffer,52)!=52) return;
      };
      theN = (unsigned __int16)(((*((unsigned int *)(((char *)m_buffer)+(m_bufferIndex>>3))))>>(m_bufferIndex&7))&0x1fff);
      m_bufferIndex=(m_bufferIndex+13)%416;
    };
    if (m_OC == 0xffff)
    {
      m_OC=theN;
      *chars = theN;
      continue;
    };
    NC=theN;
    if (NC > 255)
    {
      if (NC >= m_numCode)
      {
        if (NC == 8191) return;
        // Expand OC+C into stack;
        m_dData->m_stack[m_stkLen++] = m_C;
        code=m_OC;
      }
      else
      {
        // Expand NC onto stack.
        code = NC;
      };
      while (code > 255)
      {
        ASSERT (code < m_numCode);
        m_dData->m_stack[m_stkLen++] = m_dData->m_postfix[code];
        code = m_dData->m_prefix[code];
      };
      m_dData->m_stack[m_stkLen++] = (unsigned char)code;
      m_C = m_dData->m_stack[m_stkLen-1];
    }
    else
    {
      m_C = (unsigned char)NC;
      m_dData->m_stack[m_stkLen++]=m_C;
    };
    m_dData->m_prefix[m_numCode] = m_OC;
    m_dData->m_postfix[m_numCode++] = m_C;
    m_OC = NC;
    *chars = m_dData->m_stack[--m_stkLen];
  };
}



CAR::CODES::CODES(void)
{
  Clear();
  m_numMisses=0;
  m_numCodes=0;
}

CAR::CODES::~CODES(void)
{
}

void CAR::CODES::Clear(void)
{
  memset(m_hashTable,0,sizeof (m_hashTable));
  memset(m_codes,0xff,256*sizeof (unsigned int));
  m_nextCode=256;
}

unsigned __int16 CAR::CODES::AddCode(unsigned int code)
{
  unsigned int hashcode, incr;
  m_numCodes++;
  ASSERT(m_nextCode <= 8189);
  incr = hashcode = code % (sizeof(m_hashTable) / sizeof(m_hashTable[0]));
  if (incr==0) incr=1;
  while (m_hashTable[hashcode]!=0)
  {
    m_numMisses++;
    hashcode = (hashcode + incr) % (sizeof(m_hashTable) / sizeof(m_hashTable[0]));
  };
  m_hashTable[hashcode] = m_nextCode;
  m_codes[m_nextCode] = code;
  return m_nextCode++;
}

unsigned __int16 CAR::CODES::FindCode(unsigned int code)
{
  unsigned int hashcode, incr;
  incr = hashcode = code % (sizeof(m_hashTable) / sizeof(m_hashTable[0]));
  if (incr==0) incr=1;
  while (m_hashTable[hashcode]!=0)
  {
    if (m_codes[m_hashTable[hashcode]]==code) return m_hashTable[hashcode];
    hashcode = (hashcode + incr) % (sizeof(m_hashTable) / sizeof(m_hashTable[0]));
  };
  return 0;
}

void CAR::CODES::HashDump(FILE *f)
{
  int code, stklen;
  int stk[1000];
  for (int i=256; i<m_nextCode; i++)
  {
    code =i;
    fprintf(f,"%4d ",code);
    stklen=0;
    while (code > 255)
    {
      code = m_codes[code];
      stk[stklen++]=code & 0xff;
      code = code >> 8;
    };
    stk[stklen++]=code;
    while (stklen > 0)
    {
      fprintf(f,"%3d ",stk[--stklen]);
    };
    fprintf(f,"\n");
  };
}


