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

#ifndef DUNGEONCRAFT_SHARED_PROPERTY
#define DUNGEONCRAFT_SHARED_PROPERTY

#include "dungeoncraft/shared/class.h"

namespace DungeonCraft {

enum PropModifier 
{
  pmNONE       = 0,
  pmPCNT       = 1, // percentage of current
  pmCUMULATIVE = 2, // added to current
  pmABS        = 3  // base value is set to this before mods are applied
};

class CPropertyModifier
{
public:

  CPropertyModifier();
  CPropertyModifier(double mod, PropModifier type, int srckey);

  ~CPropertyModifier();

  void Clear();

  CPropertyModifier &operator=(const CPropertyModifier &rhs);
  bool operator==(const CPropertyModifier &rhs);
  bool operator!=(const CPropertyModifier &rhs);

  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);

  double ApplyModVal(double CurrVal);

  double GetModVal() const;

  PropModifier GetModType() const;

  int GetSrcKey() const;

private: 

  double m_ModValue;         // modifier applied to base property
  PropModifier m_ModType; // how mod is interpreted 
  int m_ModSrcKey;        // which spell applied this mod
};

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

typedef CList<int, int> PropModKeyListType;

class CBaseProperty
{
public:  

  CBaseProperty(); // don't use this one, only for serialization
  CBaseProperty(const char *pName);
  CBaseProperty(const char *pName, double baseval);
  CBaseProperty(const CBaseProperty &rhs);
  virtual ~CBaseProperty();

  bool MatchesNamedProperty(const char *pName);
  const char *GetName();

  void ClearMods();

  void AddMod(double ModVal, PropModifier ModType, int SrcKey);
  void RemoveMods(int SrcKey);

  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);

  double GetProperty() const;
  BYTE GetPropertyAsBYTE() const;
  int GetPropertyAsINT() const;
  unsigned int GetPropertyAsUINT() const;

  double SetProperty(double BaseVal);
  
  double operator=(const double rhs);
  bool operator==(const double rhs);
  bool operator < (const double rhs);
  bool operator <= (const double rhs);
  bool operator > (const double rhs);
  bool operator >= (const double rhs);

  CBaseProperty &operator=(const CBaseProperty &rhs);
  bool operator==(const CBaseProperty &rhs);
  bool operator!=(const CBaseProperty &rhs);

private:

  CString m_Name;      // property name
  double  m_BaseValue; // base value for this property

  CPropertyModifier m_AbsMod;
  CPropertyModifier m_PcntMod;

  // list of modifiers applied to this property
  CList<CPropertyModifier, CPropertyModifier&> m_ModList;

  // only one of this type allowed in list
  void AddSingletonMod(CPropertyModifier &mod);
  // any number of this type allowed in list
  void AddMod(CPropertyModifier &mod);  
};

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

class CPropertyContainer
{
public:  

  CPropertyContainer() { }
  virtual ~CPropertyContainer() { Clear(); }
  
  void Clear(); // removes all properties

  void AddMod(const char *pName, double ModVal, PropModifier ModType, int SrcKey);

  void UpdateProperty(CBaseProperty &prop);
  CBaseProperty *GetProperty(const char *pName);
  bool RemoveProperty(const char *pName);

  void ClearAllMods();  
  void ClearMods(const char *pName); // only for given property 
  void RemoveMods(int SrcKey); // all mods that match SrcKey
   
  void GetSpellKeyList(PropModKeyListType &list);
  
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
    
  CPropertyContainer &operator=(const CPropertyContainer &rhs);
  bool operator==(const CPropertyContainer &rhs);
  bool operator!=(const CPropertyContainer &rhs);
  
private:
   
  // list of spells currently modifying items in this list
  PropModKeyListType m_SrcKeys; // need xref between keys in use and prop records!
  bool IsKeyInList(int SrcKey);

  POSITION FindProperty(const char *pName);
  CList<CBaseProperty, CBaseProperty&> m_PropList;
};

} // End of namespace DungeonCraft

#endif
