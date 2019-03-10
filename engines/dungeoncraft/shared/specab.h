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

#ifndef DUNGEONCRAFT_SHARED_SPECAB
#define DUNGEONCRAFT_SHARED_SPECAB

//#include "dungeoncraft/shared/externs.h"
//#include <afxtempl.h>

namespace DungeonCraft {

#ifdef UAFEngine
class CHARACTER;
struct COMBATANT;
#endif

#define SPECAB_MSG_NONE          0x0000
#define SPECAB_MSG_CASTING       0x0001
#define SPECAB_MSG_ACTIVATESPELL 0x0002
#define SPECAB_MSG_FLEE          0x0004
#define SPECAB_MSG_TURNUNDEAD    0x0008
#define SPECAB_MSG_BANDAGE       0x0010
#define SPECAB_MSG_GUARD         0x0020
#define SPECAB_MSG_ATTACK        0x0040
#define SPECAB_MSG_MOVE          0x0080
#define SPECAB_MSG_ENDTURN       0x0100
#define SPECAB_MSG_DELAY         0x0200
#define SPECAB_MSG_READY         0x0400
#define SPECAB_MSG_UNUSED1       0x0800
#define SPECAB_MSG_UNUSED2       0x1000

#define NUM_SPECAB_MSGS 14

// if adding to this list, be sure to:
//  add another line to spellAbilitiesText[] in GlobtText.cpp.
//  add lines in ItemDB.cpp to load specabs based on name for items/etc

// for spells, items, and monsters
enum specialAbilitiesType 
{
  SA_None=0, 
  SA_Bless=1,                 // done
  SA_Curse=2,                 // done
  SA_UndeadFear=3,
  SA_Enlarge=4,               // done
  SA_Reduce=5,                // done
  SA_CharmPerson=6, 
  SA_DetectMagic=7,           // done
  SA_ReflectGazeAttack=8,
  SA_ProtFromEvil=9,          // done
  SA_ProtFromGood=10,         // done
  SA_Shield=11,               // done
  SA_Sleep=12,                // done
  SA_Fog=13,
  SA_Entangle=14,             // done
  SA_InvisibleToAnimals=15,   // done
  SA_InvisibleToUndead=16,    // done
  SA_NonUndeadFear=17,
  SA_Sanctuary=18,            // done
  SA_Shillelagh=19,           // done
  SA_Displacement=20,         // done
  SA_Wizardry=21,
  SA_VorpalAttack=22,         // done
  SA_HoldPerson=23,           // done
  SA_Silence=24,              // done
  SA_Poisoned=25,             // done
  SA_SlowPoison=26,           // done
  SA_MirrorImage=27,          // done
  SA_Invisible=28,            // done
  SA_Enfeebled=29,            // done
  SA_Blindness=30,            
  SA_Diseased=31,

  SA_LastUnusedSpecAb
};

#define FIRST_SPECIAL_ABILITY SA_Bless
#define LAST_SPECIAL_ABILITY (SA_LastUnusedSpecAb-1)
#define NUM_SPECIAL_ABILITIES SA_LastUnusedSpecAb

struct SPEC_AB_DATA
{
  SPEC_AB_DATA();
  void Clear();
  SPEC_AB_DATA& operator=(const SPEC_AB_DATA& src);
  bool operator==(const SPEC_AB_DATA& src) const;  
  void Serialize(CArchive &ar, double ver);

  bool ExecuteScript();
  bool ExecuteDAScript();

  // activation
  CString script; // conditional activation GPDL script
  CString binary; // compiled script ready to execute

  // deactivation
  CString da_script; // conditional activation GPDL script
  CString da_binary; // compiled script ready to execute

  DWORD MsgTypes; // which combat message types will this over-ride
  BOOL DisplayOnce;
  CString Msgs[NUM_SPECAB_MSGS];
};

class SPECIAL_ABILITIES : public CObject
{
  SPEC_AB_DATA *GetSpecAbData(specialAbilitiesType sa);
  CString GenerateDefaultSpecAbScript(specialAbilitiesType sa);
  CString GenerateDefaultSpecAbDAScript(specialAbilitiesType sa);
  DWORD GenerateDefaultSpecAbMsgTypes(specialAbilitiesType sa);
  CString GenerateDefaultSpecAbMsgFormat(specialAbilitiesType sa);

public:
  DECLARE_SERIAL( SPECIAL_ABILITIES )

  SPECIAL_ABILITIES();
  SPECIAL_ABILITIES(const SPECIAL_ABILITIES& src);
  ~SPECIAL_ABILITIES();

  SPECIAL_ABILITIES& operator=(const SPECIAL_ABILITIES& src);
  bool operator==(const SPECIAL_ABILITIES& src) const;  

  void Clear();
  void Serialize(CArchive &ar, double ver);

  POSITION GetHeadPosition();
  specialAbilitiesType GetAt(POSITION pos);
  specialAbilitiesType GetNext(POSITION &pos);
  SPEC_AB_DATA *GetAtData(POSITION pos);
  SPEC_AB_DATA *GetNextData(POSITION &pos);

  CString GenerateSpecAbFuncName(specialAbilitiesType sa);
  CString GenerateSpecAbFuncStart(specialAbilitiesType sa);
  CString GenerateSpecAbFuncEnd(specialAbilitiesType sa);

  void EnableSpecAb(specialAbilitiesType sa, CString scr="", CString dascr="");
  void DisableSpecAb(specialAbilitiesType sa);
  bool HaveSpecAb(specialAbilitiesType sa);
  bool HaveAtLeastOneSpecAb();

  bool CompileSpecAbScript(specialAbilitiesType sa);
  bool CompileSpecAbDAScript(specialAbilitiesType sa);
  bool ExecuteSpecAbScript(specialAbilitiesType sa);
  bool ExecuteSpecAbDAScript(specialAbilitiesType sa);

  CString GetSpecAbScript(specialAbilitiesType sa); // entire script
  CString GetSpecAbUserScript(specialAbilitiesType sa); // no $FUNC { }
  CString GetSpecAbBinary(specialAbilitiesType sa);

  CString GetSpecAbDAScript(specialAbilitiesType sa); // entire script
  CString GetSpecAbUserDAScript(specialAbilitiesType sa); // no $FUNC { }
  CString GetSpecAbBinaryDA(specialAbilitiesType sa);

  DWORD GetMsgTypes(specialAbilitiesType sa);
  void SetMsgTypes(specialAbilitiesType sa, DWORD types);
  void AddMsgType(specialAbilitiesType sa, DWORD type);
  bool HaveMsgType(specialAbilitiesType sa, DWORD type);
  CString GetMsgFormat(specialAbilitiesType sa, DWORD type);
  void SetMsgFormat(specialAbilitiesType sa, DWORD type, CString &format);

#ifdef UAFEngine
  void EnableAllFor(CHARACTER *pChar);
  void EnableAllFor(COMBATANT *pChar);
  void DisableAllFor(CHARACTER *pChar);
  void DisableAllFor(COMBATANT *pChar);
#endif

  SPEC_AB_DATA specAbData[NUM_SPECIAL_ABILITIES];
};

int ConvertSpecAbFlagToIndex(DWORD flag);
DWORD ConvertSpecAbIndexToFlag(int index);
specialAbilitiesType ConvertRuntimeIfTextToSpecAb(const CString &text);
CString ConvertSpecAbToRuntimeIfText(specialAbilitiesType sa);

} // End of namespace DungeonCraft

#endif 