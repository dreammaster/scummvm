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

#ifndef DUNGEONCRAFT_SHARED_THREAD
#define DUNGEONCRAFT_SHARED_THREAD

//#include <afx.h>
//#include <afxmt.h>
//#include <process.h>

namespace DungeonCraft {

class CThread
{
private:
  CWinThread  *m_pThread;    // Thread handle.
  DWORD   m_dwThreadId; // Thread identifier.
  mutable CCriticalSection m_cs;

  virtual DWORD Thread() = 0;
  //static  DWORD ThreadEntryPoint( CWinThread *lpThreadObject )
  static  UINT ThreadEntryPoint( LPVOID pParam )
  { 
    if (pParam != NULL) 
    {
      UINT tmp = ((CThread*)pParam)->Thread(); 
      ((CThread*)pParam)->m_pThread=NULL;
      return tmp;
    }
    return 0;
  }

public:
  CThread( DWORD                  dwCreationFlags     = 0,
           DWORD                  dwStackSize         = 0,
           LPSECURITY_ATTRIBUTES  lpThreadAttributes  = NULL )
  {
    // Create the thread using _beginthreadex() rather than CreateThread() in
    // order to allocate and initialize thread local storage for per thread
    // C runtime variables.
    /*
    unsigned long result  = _beginthreadex( lpThreadAttributes,
                                            dwStackSize,
                                            reinterpret_cast<unsigned (__stdcall *)(void *)>( ThreadEntryPoint ),
                                            this,
                                            dwCreationFlags,
                                            reinterpret_cast<unsigned *>( &m_dwThreadId ) );
                                            */
    m_pThread = AfxBeginThread(ThreadEntryPoint,this, 0, dwStackSize, dwCreationFlags,NULL);
    // Cast the result to a HANDLE which can be used to access the thread
    // kernel object.  Failure to create the thread results in the thread
    // handle being NULL.
    //m_hThread = reinterpret_cast<HANDLE>( result );
  }
  virtual ~CThread()
  {
    CSingleLock sl(&m_cs, TRUE);
    if (m_pThread != NULL)
    {
      TerminateThread( m_pThread->m_hThread, 0 );
      CloseHandle( m_pThread->m_hThread );
    }
    m_pThread = NULL;
  }

  operator  HANDLE()  const { CSingleLock sl(&m_cs, TRUE);if (m_pThread != NULL) return  m_pThread->m_hThread; else return NULL;   }
  HANDLE    Handle()  const { CSingleLock sl(&m_cs, TRUE);if (m_pThread != NULL) return  m_pThread->m_hThread; else return NULL;   }
  DWORD     Id()      const { CSingleLock sl(&m_cs, TRUE);if (m_pThread != NULL) return  m_pThread->m_nThreadID; else return 0; }

  DWORD Suspend() 
  { 
    CSingleLock sl(&m_cs, TRUE);
    if (m_pThread != NULL) 
      return  SuspendThread( m_pThread->m_hThread ); 
    else 
      return 0; 
  }

  // use 'while (Resume() > 1) ;' to ensure thread is actually resumed
  DWORD Resume()  
  { 
    CSingleLock sl(&m_cs, TRUE);
    if (!ThreadActive()) return 0;
    if (m_pThread != NULL) 
    {
      DWORD retVal = ResumeThread ( m_pThread->m_hThread );
      if (retVal == 0xFFFFFFFF) retVal = 0;
      return retVal;
    }
    else 
      return 0; 
  }

  BOOL Terminate( DWORD dwExitCode = 0 )
  {
    CSingleLock sl(&m_cs, TRUE);
    if (ThreadActive())
    {
      BOOL result = TerminateThread( m_pThread->m_hThread, dwExitCode );
      CloseHandle( m_pThread->m_hThread );
      m_pThread = NULL;
      return result;
    }
    else
      return TRUE;
  }

  BOOL  ThreadActive()  const
  {
    CSingleLock sl(&m_cs, TRUE);
    BOOL  result  = FALSE;
    if( NULL != m_pThread )
    {
      DWORD threadExitCode;
      if( GetExitCodeThread( m_pThread->m_hThread, &threadExitCode ) )
      {
        result  = (STILL_ACTIVE == threadExitCode);
      }
    }
    return  result;
  }
}; // CThread

} // End of namespace DungeonCraft

#endif
