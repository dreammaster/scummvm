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

#ifndef DUNGEONCRAFT_SHARED_OBJECT_SCRIPTS
#define DUNGEONCRAFT_SHARED_OBJECT_SCRIPTS

#include "dungeoncraft/shared/Shared_Queue.h"

namespace DungeonCraft {

class CObjectScript
{
  bool m_Compiled;
  
public:
  CObjectScript();
  ~CObjectScript();
  
  void Clear();
  
  CString m_Source;
  CString m_Binary;

  bool HaveScript() const { return (m_Source.IsEmpty()?false:true); }
  bool IsCompiled() const { return m_Compiled; }

  bool CompileScript(CString funcname);  
};

class CObjectScripts
{
public:

  enum ObjectScriptAction
  {
    OSA_Activate,
    OSA_Deactivate,
    OSA_SavingThrowAttempted,
    OSA_SavingThrowSuccess,
    OSA_SavingThrowFailed,
    OSA_NumActions // must be last enum
  };

private:

  ObjectTypes m_ObjectType;
  OrderedQueue<CObjectScript, OSA_NumActions> m_scripts;
  
  CString GetFuncName(ObjectScriptAction action);

public:
  
  CObjectScripts(ObjectTypes SrcType);
  ~CObjectScripts();
  

  CObjectScript *GetScript(ObjectScriptAction action);

  bool CObjectScripts::AddScript(ObjectScriptAction action);
  bool CObjectScripts::AddScript(CObjectScript &script, ObjectScriptAction action);

  bool CompileScripts(CString basename);

};

} // End of namespace DungeonCraft

#endif
