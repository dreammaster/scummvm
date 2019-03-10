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

#include "dungeoncraft/shared/Dungeon.h"
#include "dungeoncraft/shared/GetInput.h"
#include "dungeoncraft/shared/CProcInp.h"
#include "dungeoncraft/shared/MainFrm.h"
#include "dungeoncraft/shared/SoundMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char *APP_NAME = "UAFWin";

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : winClassName("")
{
  // this class name is used in PreCreateWindow()
  /*
	winClassName = 
		AfxRegisterWndClass((CS_DBLCLKS | CS_OWNDC),
								        LoadCursor(NULL,IDC_ARROW ),
								        (HBRUSH)GetStockObject(NULL_BRUSH),
                        //NULL);	
								        LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON2)));	
  */
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  //if (!CFrameWnd::PreCreateWindow(cs))
  //      return FALSE;

  if (winClassName.IsEmpty())
  {
	  winClassName = 
		  AfxRegisterWndClass((/*CS_HREDRAW|CS_VREDRAW|CS_BYTEALIGNCLIENT|*/ CS_DBLCLKS | CS_OWNDC),
								          LoadCursor(NULL,IDC_ARROW ),
								          (HBRUSH)GetStockObject(NULL_BRUSH),
                          //NULL);	
								          LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON2)));	
  }

	cs.lpszClass = winClassName;
	//cs.style = WS_POPUP | WS_VISIBLE;
  cs.x = 0;
	cs.y = 0;
	cs.hMenu = NULL;
	cs.lpszName = APP_NAME;

  // use WS_OVERLAPPED instead of WS_POPUP if you want a moveable window frame
#ifdef _DEBUG
  cs.style =  WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
  cs.dwExStyle &= ~WS_EX_TOPMOST;
  cs.cx = SCREEN_WIDTH;
  cs.cy = SCREEN_HEIGHT;
#else
  cs.style = WS_POPUP | WS_VISIBLE | WS_MAXIMIZE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
  cs.dwExStyle |= WS_EX_TOPMOST;
	cs.cx = GetSystemMetrics(SM_CXFULLSCREEN);
	cs.cy = GetSystemMetrics(SM_CYFULLSCREEN);
#endif
	
  // this function adds a window border and
  // we don't need to call it since we are
  // registering our own window class
  //
  // if (!CFrameWnd::PreCreateWindow(cs))
  //   return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	WriteDebugString("Processing WM_DESTROY\n");
	ShowCursor( TRUE );	
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//ProcInput.KeyPressed(nChar);
  if (nChar != VK_SHIFT)
  { //Skip shifts...makes breakpoints impossible!
    nChar &= 0xff;
    if (GetKeyState(VK_SHIFT) & 0x80000000) nChar |= (SHIFT_KEY<<8);
    inputQueue.OnKeyDown(nChar);
  };
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMainFrame::OnTimer(UINT nIDEvent) 
{
  //WriteDebugString("Rcvd timer %u\n", nIDEvent);
  //KillTimer(nIDEvent);
  //ProcInput.handleTimer(nIDEvent);
  /*
  {
    TRACE("Killing event timer\n");
    KillTimer(nIDEvent);
  }
  else
  */
 CFrameWnd::OnTimer(nIDEvent);   
}


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
  switch (message)
  {
  case WM_POWERBROADCAST:
   if(wParam == PBT_APMQUERYSUSPEND)
      return BROADCAST_QUERY_DENY;
   return TRUE;

  case MM_MCINOTIFY:
    MidiStopNotify(wParam, lParam);
    break;

  case WM_SHOWWINDOW:
  case WM_ACTIVATE:
#ifndef _DEBUG
    if ((bActive == FALSE) && ((wParam==WA_ACTIVE)||(wParam==WA_CLICKACTIVE)))
    {
      bActive = TRUE;
      // going active again, restore full screen
      ShowWindow(SW_SHOWMAXIMIZED);      
      WriteDebugString("Engine active: %s\n", (bActive?"Yes":"No"));
    }
#endif    
    break;

  case WM_ACTIVATEAPP:

#ifndef _DEBUG
    if ((bActive == FALSE) && (wParam==1))
    {
      // going active again, restore full screen
      ShowWindow(SW_SHOWMAXIMIZED);
      ::SetForegroundWindow( AfxGetMainWnd()->m_hWnd );
    }
#endif

    bActive = wParam;
    if (pSndMgr!=NULL)
    {
      if (bActive) pSndMgr->UnMuteVolume();
      else pSndMgr->MuteVolume();
    }

    WriteDebugString("Engine app active: %s\n", (bActive?"Yes":"No"));
    break;

  case WM_SYSCOMMAND: // don't let screen saver kick in while engine runs
    if (   ((wParam&0xFFF0) == SC_SCREENSAVE)
        || ((wParam&0xFFF0) == SC_MONITORPOWER))
    {
      if (!DisableScreenSaver) 
        return 0;
    }
    break;
  }
      
  return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON)
     //ProcInput.HandleMouseClick(point.x, point.y);
     inputQueue.OnLButtonDown(point.x, point.y);

  CFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
		
	CFrameWnd::OnLButtonDblClk(nFlags, point);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point) 
{
	//ProcInput.HandleMouseMove(point.x, point.y);
	CFrameWnd::OnMouseMove(nFlags, point);
}