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

#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/Dungeon.h"
#include <string.h>
//#include <math.h>
#include "dungeoncraft/shared/combatants.h"
#include "dungeoncraft/shared/DrawTile.h"
#include "dungeoncraft/shared/Path.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int MAX_STEPS = 100;

PATH_MANAGER pathMgr;

int GetCost(int sx, int sy, int dx, int dy, void *ud)
{
//  if (abs(sx-dx) > 1)
//    return 100;
//  if (abs(sy-dy) > 1)
//    return 100;

  return 1;//GetDist(GetDir(sx, sy, dx, dy));
}

int GetValid(int x, int y, void *ud)
{
  BOOL co;

	if (!ValidCoords(y,x))
    return FALSE;
 
  if ((x == pathMgr.DestX) && (y == pathMgr.DestY))
    co = FALSE;
  else
    co = pathMgr.GetOccBlock();

  BOOL valid = (!isObstical(x, y, 
                      pathMgr.GetPathWidth(), pathMgr.GetPathHeight(), 
                      co));
  return valid;
}

CPathFinder::CPathFinder() 
{
	m_pOpen = m_pClosed = NULL;
	m_pStack = NULL;
  m_pBest = NULL;
  m_iRows=0;
  m_iSX=0;
  m_iSY=0;
  m_iDX=0;
  m_iDY=0;
  m_iDNum=0;
  m_pCBData=NULL;
  udCost=NULL;
  udValid=NULL;
}

CPathFinder::~CPathFinder() {
	Clear();
}

void CPathFinder::Clear()
{
  ClearNodes();
  
  m_iSX=0;
  m_iSY=0;
  m_iDX=0;
  m_iDY=0;
  m_iDNum=0;
  m_pCBData=NULL;
  udCost=0;
  udValid=0;
  m_pBest = NULL;

  if (m_pStack != NULL)
  {
    _asStack *temp = m_pStack, *temp2 = NULL;

    while (temp != NULL)
    {
      temp2 = temp;
      temp = temp->next;
      delete temp2;
    }
  }
  m_pStack = NULL;
  PATH.RemoveAll();
}

void CPathFinder::ClearNodes() {
	_asNode *temp = NULL;

	if (m_pOpen) {
		while (m_pOpen) {
			temp = m_pOpen->next;

			delete m_pOpen;

			m_pOpen = temp;
		}
	}

	if (m_pClosed) {
		while (m_pClosed) {
			temp = m_pClosed->next;

			delete m_pClosed;

			m_pClosed = temp;
		}
	}
}

bool CPathFinder::GeneratePath(int sx, int sy, int dx, int dy) 
{
	m_iSX = sx; m_iSY = sy; m_iDX = dx; m_iDY = dy;
	m_iDNum = Coord2Num(dx,dy);

	_asNode *temp = new _asNode(sx, sy);
	_asNode *best=NULL;

	temp->g = 0;
	temp->h = (dx-sx)*(dx-sx) + (dy-sy)*(dy-sy);
	temp->f = temp->g + temp->h;
	temp->number = Coord2Num(sx, sy);

	m_pOpen = temp;
	while (true) 
  {
    best = GetBest();
		if (best==NULL) return false;
		if (best->number == m_iDNum) break;

		CreateChildren(best);
	}
	
	m_pBest = best;

  temp = m_pBest;

  // create step list in reverse order
  while (temp != NULL)
  {
    if (!((temp->x == sx) && (temp->y == sy)))
      PATH.AddHead(CPoint(temp->x, temp->y));
    temp = temp->parent;
  }
  
  return true;
}

_asNode *CPathFinder::GetBest() {
	if (!m_pOpen) return NULL;

	_asNode *temp = m_pOpen, *temp2 = m_pClosed;
	m_pOpen = temp->next;

	if (temp2) {
		m_pClosed = temp;
		m_pClosed->next = temp2;
	} else {
		m_pClosed = temp;
		m_pClosed->next = NULL;
	}

	return temp;
}

void CPathFinder::CreateChildren(_asNode *node) {
	int x = node->x, y = node->y;

	for (int i=-1;i<2;i++) {
		for (int j=-1;j<2;j++) {
			if (i == 0 && j == 0 || !udValid(x+i, y+j, m_pCBData)) continue;

			LinkChild(node, x+i, y+j);
		}
	}
}

void CPathFinder::LinkChild(_asNode *node, int x, int y) 
{
	int g = node->g + udCost(node->x, node->y, x,y,m_pCBData);
	int num = Coord2Num(x,y);

	_asNode *check = NULL;
  	
  check = CheckList(m_pOpen, num);
  if (check != NULL) 
  {
		node->children[node->numchildren++] = check;
		
		// A better route found, so update
		// the node and variables accordingly.
		if (g < check->g) 
    {
			check->parent = node;
			check->g = g;
			check->f = g + check->h;
		}
	} 
  else if ((check=CheckList(m_pClosed, num))!=NULL) 
  {
		node->children[node->numchildren++] = check;

		if (g < check->g) 
    {
			check->parent = node;
			check->g = g;
			check->f = g + check->h;

			// The fun part...
			UpdateParents(check);
		}
	} 
  else 
  {
		_asNode *newnode = new _asNode(x,y);
		newnode->parent = node;
		newnode->g = g;
		newnode->h = (x-m_iDX)*(x-m_iDX) + (y-m_iDY)*(y-m_iDY);
		newnode->f = newnode->g + newnode->h;
		newnode->number = Coord2Num(x,y);

		AddToOpen(newnode);

		node->children[node->numchildren++] = newnode;
	}
}

_asNode *CPathFinder::CheckList(_asNode *node, int num) 
{
	while (node) 
  {
		if (node->number == num) 
      return node;
		node = node->next;
	}

	return NULL;
}

// Sort with respect to 'f'
void CPathFinder::AddToOpen(_asNode *addnode) 
{
	_asNode *node = m_pOpen;
	_asNode *prev = NULL;

	if (!m_pOpen) 
  {
		m_pOpen = addnode;
		return;
	}

	while(node) 
  {
		if (addnode->f > node->f) 
    {
			prev = node;
			node = node->next;
		} 
    else 
    {
			if (prev) 
      {
				prev->next = addnode;
				addnode->next = node;
			} 
      else 
      {
				_asNode *temp = m_pOpen;

				m_pOpen = addnode;
				m_pOpen->next = temp;
			}

			return;
		}
	}

	prev->next = addnode;
}

// Have to propagate the changes down.
void CPathFinder::UpdateParents(_asNode *node) 
{
	int g = node->g, c = node->numchildren;
  
	_asNode *kid = NULL;
  int i;
	for (i=0;i<c;i++) 
  {
		kid = node->children[i];
		if (g+1 < kid->g) 
    {
			kid->g = g+1;
			kid->f = kid->g + kid->h;
			kid->parent = node;
		
			Push(kid);
		}
	}

	_asNode *parent;

	while (m_pStack) 
  {
		parent = Pop();
		c = parent->numchildren;
		for (i=0;i<c;i++) 
    {
			kid = parent->children[i];
			
			if (parent->g+1 < kid->g) 
      {
				kid->g = parent->g + udCost(parent->x, parent->y, kid->x, kid->y,m_pCBData);
				kid->f = kid->g + kid->h;
				kid->parent = parent;

				Push(kid);
			}
		}
	}
}

void CPathFinder::Push(_asNode *node) 
{

	if (!m_pStack) 
  {
		m_pStack = new _asStack;
		m_pStack->data = node;
		m_pStack->next = NULL;
	} 
  else 
  {
		_asStack *temp = new _asStack;

		temp->data = node;
		temp->next = m_pStack;
		m_pStack = temp;
	}
}

_asNode *CPathFinder::Pop() 
{
	_asNode *data = m_pStack->data;
	_asStack *temp = m_pStack;

	m_pStack = temp->next;
	
	delete temp;

	return data;
}

CPoint CPathFinder::GetFirstStep()// always the start location
{
  return GetNextStep();
}

CPoint CPathFinder::GetNextStep()
{
  CPoint tmp;

  if (PATH.GetCount() > 0)
  {
    tmp = PATH.GetHead();
    PATH.RemoveHead();
  }
  else
  {
    tmp.x = -1;
    tmp.y = -1;
  }
  return tmp;
}

CPoint CPathFinder::GetLastStep()
{
  if (PATH.GetCount() > 0)
    return PATH.GetTail();
  else
	  return CPoint(-1,-1);
}

bool CPathFinder::InUse()
{
	return (PATH.GetCount() > 0);
}
//****************************************************
//****************************************************
//****************************************************

PATH_MANAGER::PATH_MANAGER(int num )
{
	MaxPaths = num;
	Tags = new TAG_LIST(num);
	Path = new CPathFinder[num];

  OccupantsBlock = TRUE;	
	CurrPath = 0;
	StartX = 0, StartY = 0, DestX = 0, DestY = 0;
  pathWidth=1;pathHeight=1;
}


PATH_MANAGER::~PATH_MANAGER()
{
	if (Tags) delete Tags;
	if (Path) delete[] Path;
}

void PATH_MANAGER::Clear()
{
  pathWidth=1;pathHeight=1;
  OccupantsBlock = TRUE;
	for (int i=0; i<MaxPaths; i++)
		Path[i].Clear();
	Tags->Clear ();
	CurrPath = 0;
	StartX = 0, StartY = 0, DestX = 0, DestY = 0;
}

int	PATH_MANAGER::GetPath(int startx, int starty, int destx, int desty, BOOL occupantsBlock)
{
  //WriteDebugString("Starting GetPath(%i, %i, %i, %i)\n", 
  //                 startx, starty, destx, desty);

  OccupantsBlock = occupantsBlock;
  if (startx == destx && starty == desty) return -1;// we are already there

	StartX = startx;
  StartY = starty;
  DestX = destx;
  DestY = desty;
	
  if (FindPath())
  {
    //WriteDebugString("GetPath() returning path %u\n", CurrPath);
    return CurrPath;
  }
  else
  {
    //WriteDebugString("GetPath() returning path -1\n");
	  return -1;
  }
}


CPathFinder* PATH_MANAGER::GetPath(int which)
{
	if (which<0 || which>=MaxPaths) return NULL;
	Tags->Set(which);
	return &Path[which];
}


void	PATH_MANAGER::FreePath(int which)
{
	if (which<0 || which>=MaxPaths) return;
	Tags->Clear(which);
	Path[which].Clear();
}

int PATH_MANAGER::GetStepsRemaining(int which)
{
	if (which<0 || which>=MaxPaths) return 0;
	return Path[which].GetStepCount();
}

bool PATH_MANAGER::FindPath()
{
	CurrPath = GetAvailablePath ();
	if (CurrPath == -1) return false;
  Path[CurrPath].Clear();

  Path[CurrPath].SetData(NULL);
  Path[CurrPath].SetValid(GetValid);
  Path[CurrPath].SetCost(GetCost);
  Path[CurrPath].SetRows(MAX_TERRAIN_HEIGHT);

  bool Found = Path[CurrPath].GeneratePath(StartX, StartY, DestX, DestY);

  if (!Found)
  {
    Tags->Clear(CurrPath);
    Path[CurrPath].Clear();
  }
  else
    Tags->Set(CurrPath);

	return Found;
}

int  PATH_MANAGER::GetAvailablePath()
{
	int val = Tags->FindOpenTag();
	if (val > -1) return val;

	for (int i=0; i<MaxPaths; i++)
  {
		if (!Path[i].InUse())
    { 
      Tags->Clear(i);
      return i;
    }
  }
	return -1;
}
