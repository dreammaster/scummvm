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

#ifndef DUNGEONCRAFT_SHARED_PIC_SLOT
#define DUNGEONCRAFT_SHARED_PIC_SLOT

namespace DungeonCraft {

//lint +e1411
//class CAR;

// default bitmaps

// title screen
extern CString DEFAULT_TITLE_BG;

// exit screen with credits
extern CString DEFAULT_CREDITS_BG;

// combat cursor
extern CString DEFAULT_CC;
// combat death icon
extern CString DEFAULT_CDI;
// combat death
extern CString DEFAULT_CD;
// combat wall set (Dungeon)
extern CString DEFAULT_CWD;
// combat wall set (Wilderness)
extern CString DEFAULT_CWW;
// frame
extern CString DEFAULT_FR;
// horz bar
extern CString DEFAULT_HB;
// vert bar
extern CString DEFAULT_VB;
// icon bg, when player picks icon
extern CString DEFAULT_IB;
// camp pic
extern CString DEFAULT_CA;
// treasure pic 
extern CString DEFAULT_TR;

extern CString DEFAULT_MONSTER_ICON;
   
// item in-route-to-target throw sprite (item type = HandThrow, Throw)  
extern CString DEFAULT_IIRT;
// item in-route-to-target Sling sprite (item type = SlingNoAmmo)
extern CString DEFAULT_IIRS;
// item in-route-to-target Ammo sprite (item type = Ammo, such as arrows)
extern CString DEFAULT_IIRA;

// spell in-route-to-target sprite
extern CString DEFAULT_SIR;
// spell target coverage sprite
extern CString DEFAULT_STC;
// spell target hit sprite
extern CString DEFAULT_STH;
// spell target linger sprite
extern CString DEFAULT_STL;
// NPC default combat icon
extern CString DEFAULT_NPC_ICON;
// NPC default small pic
extern CString DEFAULT_NPC_PIC;
// default cursor art
extern CString DEFAULT_CURSOR;
// default overland background image
extern CString DEFAULT_OVERLAND;
extern CString DEFAULT_ZONEAVART;

extern int WALL_CACHE_SIZE;
extern int BACKGROUND_CACHE_SIZE;

struct ImageCacheData
{
  ImageCacheData() { Clear(); }
  ~ImageCacheData() { Clear(); }

  void Clear() { name=""; surface=-1; insertTime=0; locked=FALSE; type=0; }

  ImageCacheData& operator =(const ImageCacheData& src)
  {
    if (this==&src) return *this;
    name=src.name;
    surface=src.surface;
    insertTime=src.insertTime;
    locked=src.locked;
    type=src.type;
    return *this;
  }
  ImageCacheData(const ImageCacheData& src) { *this=src; }

  CString name;
  BYTE type;
  long surface;
  DWORD insertTime;
  BOOL locked;
};

class ImageCacheMgr
{
  struct sortdata
  {
    DWORD key;
    DWORD timestamp;
    long surf;
  };

public:

  ImageCacheMgr(DWORD maxsize, CString id);
  ~ImageCacheMgr();
  void Clear();
  DWORD GetSize() const { return m_maxsize; }

  BOOL GetSurface(DWORD hashkey, ImageCacheData& surface);
  BOOL AddSurface(DWORD hashkey, ImageCacheData& surface);
  BOOL LockSurface(DWORD hashkey);
  BOOL UnlockSurface(DWORD hashkey);  
  void FlushOldest();
  BOOL NeedsFlushing();
  
private:
  DWORD m_maxsize;
  DWORD m_sortsize;
  sortdata *m_sortarray;
  CString m_id;
  CCriticalSection m_cs;
  CMap<DWORD, DWORD, ImageCacheData, ImageCacheData&> m_SurfaceMap;

  void RemoveOldest();
  static int compareimagecache( const void *arg1, const void *arg2 );

  ImageCacheMgr &operator=(const ImageCacheMgr &src); // not allowed
  ImageCacheMgr(const ImageCacheMgr &src); // not allowed
};

extern ImageCacheMgr *pWallSlotMgr;
extern ImageCacheMgr *pBgSlotMgr;

class WallSetSlotMemType : public CObject
{
public:
  DECLARE_SERIAL( WallSetSlotMemType )
  WallSetSlotMemType() : wallSurface(-1),doorSurface(-1),overlaySurface(-1),hsound(-1),AVArtSurface(-1) 
  { 
    Clear(); 
  }
  ~WallSetSlotMemType() { Clear(); }
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  void SaveArt(int level);
  void SaveSound(int level);
  void PlaySlotSound();

  void Clear();
  void ClearResources();
  WallSetSlotMemType& operator =(const WallSetSlotMemType& src);
  WallSetSlotMemType(const WallSetSlotMemType& src) { *this=src; }

  long GetWallSurface();
  long GetDoorSurface();
  long GetOverlaySurface();
  long GetAVArtSurface();
  BYTE GetFormat();

  BOOL used;
  BOOL doorFirst; // draw door before overlay?
  BOOL drawAV;    // include in area view?
  WORD SlotType;
  CString wallFile;
  CString doorFile;
  CString overlayFile;
  CString soundFile;
  CString AVArtFile;

  DWORD wallHash;
  DWORD doorHash;
  DWORD overlayHash;
  DWORD AVArtHash;
  bool AVArtTried;
  BOOL AVZoneStyle;
  
  long wallSurface;
  long doorSurface;
  long overlaySurface;
  long hsound;
  long AVArtSurface;

  long UnlockSpell;
  long UnlockSpellClass;
  long UnlockSpellLevel;

  BOOL BlendOverlay;
  int  BlendAmount;
};

class BackgroundSlotMemType : public CObject
{
  DECLARE_SERIAL( BackgroundSlotMemType )
  BackgroundSlotMemType() : backgroundSurface(-1),backgroundSurfaceAlt(-1),hsound(-1)
  { 
    Clear(); 
  }
  ~BackgroundSlotMemType() { Clear(); }
  void Serialize(CArchive &ar, double version);
  void Serialize(CAR &ar, double version);
  void SaveArt(int level);
  void SaveSound(int level);
  void PlaySlotSound();
  
  void Clear();
  void ClearResources();
  BackgroundSlotMemType& operator =(const BackgroundSlotMemType& src);
  BackgroundSlotMemType(const BackgroundSlotMemType& src) { *this=src; }

  long GetBackgroundSurface();
  long GetBackgroundAltSurface();

  // if the current level is an overland map, the backgroundFile and
  // backgroundSurface are used to hold the map image data. There are
  // no walls or backdrops loaded if the level is an overland map.
  BOOL used;
  WORD SlotType;
  CString backgroundFile;
  CString backgroundFileAlt;
  CString soundFile;
  BOOL useAltBackground;  // use time to determine alt backdrop usage
  BOOL useAlphaBlend;     // use alpha-blend on alt backdrop
  BOOL useTransparency;   
  BOOL suppressStepSound;
  long backgroundSurface;
  long backgroundSurfaceAlt;
  DWORD backgroundFileHash;
  DWORD backgroundFileAltHash;

  long hsound;
  int StartTime;
  int EndTime;
  int AlphaBlendPcnt;
  RECT surfRect; // used for overland only
};

extern BackgroundSlotMemType BackgroundSets[MAX_BACKGROUNDS];
extern WallSetSlotMemType WallSets[MAX_WALLSETS];

class PicDataType : public CObject
{
public:
   DECLARE_SERIAL( PicDataType )
   PicDataType() { Clear(); }
   ~PicDataType() { Clear(); }
   void Serialize(CArchive &ar);
   void Serialize(CAR &ar);
   void PreSerialize(BOOL IsStoring);
   void PostSerialize(BOOL IsStoring);

   void Clear();
   PicDataType& operator =(const PicDataType& src);
   PicDataType(const PicDataType& src);

   SurfaceType  picType;
   CString name;
   long surface;
};

const int GLOBAL_ART = MAX_LEVELS;

extern PicDataType HBarVPArt;
extern PicDataType VBarVPArt;
extern PicDataType FrameVPArt;
extern PicDataType VBarCombArt;
extern PicDataType HBarCombArt;
extern PicDataType CombatWallArt;
//extern PicDataType CombatCommonArt;
extern PicDataType CombatCursorArt;
extern PicDataType CombatDeathIconArt;
extern PicDataType CombatDeathArt;

void saveArt(CString &filename, SurfaceType type, int level, BOOL alterPath=TRUE);
void deleteAllLevelArt(int level);
BOOL writeArtToFile(CString& filename);
void clearArtSlots(void);
void flushOldImages();
void WriteRequiredArtList();

BOOL GetWallSlotSurfaces(WallSetSlotMemType &data);
BOOL GetBackgroudSlotSurfaces(BackgroundSlotMemType &data);
BOOL loadWallSetSurfaces(); // load all wall/bg slots

} // End of namespace DungeonCraft

#endif
