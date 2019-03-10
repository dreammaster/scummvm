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

#ifndef DUNGEONCRAFT_SHARED_PATHS
#define DUNGEONCRAFT_SHARED_PATHS

#include "dungeoncraft/shared/taglist.h"

namespace DungeonCraft {

enum PATH_DIR { PathNorth, PathNE, PathEast, PathSE, PathSouth, PathSW, PathWest, PathNW, PathBAD=-1 };

inline BOOL IsNorthDir(PATH_DIR dir) { return ((dir==PathNorth)||(dir==PathNE)||(dir==PathNW)); }
inline BOOL IsSouthDir(PATH_DIR dir) { return ((dir==PathSouth)||(dir==PathSE)||(dir==PathSW)); }
inline BOOL IsEastDir(PATH_DIR dir) { return ((dir==PathEast)||(dir==PathNE)||(dir==PathSE)); }
inline BOOL IsWestDir(PATH_DIR dir) { return ((dir==PathWest)||(dir==PathNW)||(dir==PathSW)); }

inline PATH_DIR GetDir (int cx, int cy, int dx, int dy)
{
  if (cx > dx) // nw, w, sw
  {
    if (cy > dy) return PathNW;
    else if (cy < dy) return PathSW;
    return PathWest;
  }
  else if (cx < dx) // ne, e, se
  {
    if (cy > dy) return PathNE;
    else if (cy < dy) return PathSE;
    return PathEast;
  }
  else // cx == dx
  {
    if (cy > dy) return PathNorth;
    else if (cy < dy) return PathSouth;
    return PathBAD; // curr == dest
  }
}

inline int GetDist (PATH_DIR d)
{
  if (d==PathNorth || d==PathEast || d==PathSouth || d==PathWest)
    return 1;
  else
    return 2;
}

//****************************************************
//****************************************************
//****************************************************

class _asNode {
	public:
		_asNode(int a = -1,int b = -1) : x(a), y(b), number(0), numchildren(0) 
    {
			parent = next = NULL;
			memset(children, 0, sizeof(children));
      f=g=h=0;
		}

		int			f,g,h;			// Bad name, but that's the terminology.
		int			x,y;			// Coordinate position
		int			numchildren;
		int			number;			// x*30+y
		_asNode		*parent;
		_asNode		*next;
		_asNode		*children[8];	// Assumes square tiles
};

// Stack for propagation.
struct _asStack {
	_asNode	 *data;
	_asStack *next;
};

typedef int(*_asFunc)(int, int, void *);
typedef int(*_asCostFunc)(int, int, int, int, void *);

class CPathFinder 
{
	public:
		CPathFinder();
		~CPathFinder();

		bool	GeneratePath(int, int, int, int);
		void	SetValid(_asFunc sv) { udValid = sv;   }
		void	SetCost(_asCostFunc sc)  { udCost = sc;    }
		void	SetData(void *sd)	 { m_pCBData = sd; }
		void	SetRows(int r)		 { m_iRows = r;    }
    void  Clear();

		_asNode	*GetBestNode() { return m_pBest; }

  
  // these are for the entity when he is following the path
  CPoint  GetFirstStep ();  // always the start location
  CPoint  GetNextStep ();
  CPoint  GetLastStep ();
  int     GetStepCount() { return PATH.GetCount(); }

  // maintainence
  bool InUse ();          // true is used, false is avail

	protected:
		int		m_iRows;			// Used to calculate node->number
		int		m_iSX, m_iSY, m_iDX, m_iDY, m_iDNum;
		void	*m_pCBData;			// Data passed back.

		_asNode	*m_pOpen;			// The open list
		_asNode	*m_pClosed;			// The closed list
		_asNode *m_pBest;			// The best node
		_asStack*m_pStack;			// Propagation stack

		_asCostFunc	udCost;				// The user-defined cost function
		_asFunc udValid;			// The user-defined validity function

		// Functions.
		void	AddToOpen(_asNode *);
		void	ClearNodes();
		void	CreateChildren(_asNode *);
		void	LinkChild(_asNode *, int, int);
		void	UpdateParents(_asNode *);

		// Stack Functions.
		void	Push(_asNode *);
		_asNode *Pop();
		
		_asNode *CheckList(_asNode *, int);
		_asNode	*GetBest();
		
		// Inline functions.
		inline int Coord2Num(int x, int y) { return x * m_iRows + y; }

    CList<CPoint, CPoint> PATH;
};

//****************************************************
//****************************************************
//****************************************************

class PATH_MANAGER
{
public:
  enum { MAXPATHS = 100 };
  int DestX, DestY, StartX, StartY;

private:// variables

  CPathFinder*    Path;
  TAGptr    Tags;// for determining which paths are already taken
  int     MaxPaths;
  int     CurrPath;
  BOOL OccupantsBlock;
  BYTE pathWidth;
  BYTE pathHeight; 
  
private:// functions
  
  // usage functions
  int   GetAvailablePath ();// returns -1 if none avail, 0+ for index
  bool  FindPath ();  
  PATH_MANAGER& operator=(const PATH_MANAGER &src); // no implementation
  
public:
  
  PATH_MANAGER (int num = MAXPATHS);// max number of paths
  ~PATH_MANAGER ();
  
  int  GetStepsRemaining(int which);
  BOOL GetOccBlock() { return OccupantsBlock; }
  BYTE GetPathWidth() { return pathWidth; }
  BYTE GetPathHeight() { return pathHeight; }
  void SetPathSize(int w, int h) { pathWidth = w; pathHeight = h; }

  void Clear ();  
  int  GetPath (int startx, int starty, int destx, int desty, BOOL occupantsBlock);
  void  FreePath (int which);
  
  CPathFinder* GetPath (int which);  
};

extern PATH_MANAGER pathMgr;

} // End of namespace DungeonCraft

#endif