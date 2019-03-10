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
#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
#else
#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/..\UAFWin\Dungeon.h"
#endif

#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/SurfaceMgr.h"
//#include "dungeoncraft/shared/globalData.h"
//#include "dungeoncraft/shared/level.h"
#include "dungeoncraft/shared/SoundMgr.h"
#include "dungeoncraft/shared/PicData.h"
#include "dungeoncraft/shared/PicSlot.h"
#include "dungeoncraft/shared/Graphics.h"
#include "dungeoncraft/shared/Viewport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( WallSetSlotMemType, CObject, 1 )
IMPLEMENT_SERIAL( BackgroundSlotMemType, CObject, 1 )
IMPLEMENT_SERIAL( PicDataType, CObject, 1 )

#define IMAGE_CACHE_MAP_SIZE (0xFFFFFFFD)

#ifdef UAFEDITOR
CList<ResourceSaveType, ResourceSaveType&> LevelPicData[MAX_LEVELS+1];
#endif

BackgroundSlotMemType BackgroundSets[MAX_BACKGROUNDS];
WallSetSlotMemType WallSets[MAX_WALLSETS];

#ifdef _DEBUG
// low size forces frequent image reloading, good
// for performance checking.
int WALL_CACHE_SIZE = 4; 
int BACKGROUND_CACHE_SIZE = 4;
#else
// allow up to 16 slots to be loaded
// and in memory at once.
int WALL_CACHE_SIZE = 16*3;       // max 3 images per slot
int BACKGROUND_CACHE_SIZE = 16*2; // max 2 images per slot
#endif

ImageCacheMgr *pWallSlotMgr=NULL;
ImageCacheMgr *pBgSlotMgr=NULL;

PicDataType HBarVPArt;
PicDataType VBarVPArt;
PicDataType FrameVPArt;
PicDataType VBarCombArt;
PicDataType HBarCombArt;
PicDataType CombatWallArt;
//PicDataType CombatCommonArt;
PicDataType CombatCursorArt;
PicDataType CombatDeathIconArt;
PicDataType CombatDeathArt;

CString DEFAULT_TITLE_BG = "Title.png";
CString DEFAULT_CREDITS_BG = "Credits.jpg";

// combat cursor
CString DEFAULT_CC = "cu_DefCC.png";
// combat death icon
CString DEFAULT_CDI = "DefCDI.png";
// combat death
CString DEFAULT_CD = "DefCD.png";
// combat wall set (Dungeon)
CString DEFAULT_CWD = "cw_DefCW.png";
// combat wall set (Wilderness)
CString DEFAULT_CWW = "cww_DefCWW.png";
// frame
CString DEFAULT_FR = "bo_DefFR.png";
// horz bar
CString DEFAULT_HB = "bo_DefHB.png"; 
// vert bar
CString DEFAULT_VB = "bo_DefVB.png"; 
// icon bg, when player picks icon
CString DEFAULT_IB = "DefIB.png";
// camp pic
CString DEFAULT_CA = "sp_DefCA.png";
// treasure pic 
CString DEFAULT_TR = "sp_DefTR.png";
// icon used when monster has none specified
CString DEFAULT_MONSTER_ICON = "cm_DefMI.png";

// item in-route-to-target throw sprite (item type = HandThrow, Throw)
CString DEFAULT_IIRT = "it_DefIIRT.png";
// item in-route-to-target Sling sprite (item type = SlingNoAmmo)
CString DEFAULT_IIRS = "it_DefIIRS.png";
// item in-route-to-target Ammo sprite (item type = Ammo, such as arrows)
CString DEFAULT_IIRA = "it_DefIIRA.png";

// spell in-route-to-target sprite
CString DEFAULT_SIR = "spl_DefSIR.png";
// spell target coverage sprite
CString DEFAULT_STC = "spl_DefSTC.png";
// spell target hit sprite
CString DEFAULT_STH = "spl_DefSTH.png";
// spell target linger sprite
CString DEFAULT_STL = "spl_DefSTL.png";
// NPC default combat icon
CString DEFAULT_NPC_ICON = "cn_Icon1.png";
// NPC default small pic
CString DEFAULT_NPC_PIC = "prt_SPic1.png";
// default cursor art
CString DEFAULT_CURSOR = "cu_Cursor.png";
// default overland background image
CString DEFAULT_OVERLAND = "ovl_overland.jpg";

CString DEFAULT_ZONEAVART = "AreaViewZoneArt.png";


ImageCacheMgr::ImageCacheMgr(DWORD maxsize, CString id)
{
  m_maxsize=maxsize;
  m_sortsize = max(m_maxsize+50, 100);
  m_sortarray=new sortdata[m_sortsize];
  m_id = id;
  WriteDebugString("Init %s image cache with size %u\n",m_id, maxsize);
  int tablesize = m_maxsize+(m_maxsize*0.2);
  m_SurfaceMap.InitHashTable(tablesize);
}

ImageCacheMgr::~ImageCacheMgr() 
{ 
  Clear(); 
  if (m_sortarray!=NULL) 
    delete [] m_sortarray;
}

void ImageCacheMgr::Clear()
{
  CSingleLock sl(&m_cs, TRUE);
  m_SurfaceMap.RemoveAll();
  int tablesize = m_maxsize+(m_maxsize*0.2);
  m_SurfaceMap.InitHashTable(tablesize);
}

BOOL ImageCacheMgr::LockSurface(DWORD hashkey)
{
  CSingleLock sl(&m_cs, TRUE);
  ImageCacheData tmp;
  if (!m_SurfaceMap.Lookup(hashkey, tmp))
    return FALSE;
  tmp.locked=TRUE;
  return TRUE;
}

BOOL ImageCacheMgr::UnlockSurface(DWORD hashkey)
{
  CSingleLock sl(&m_cs, TRUE);
  ImageCacheData tmp;
  if (!m_SurfaceMap.Lookup(hashkey, tmp))
    return FALSE;
  tmp.locked=FALSE;
  return TRUE;
}

BOOL ImageCacheMgr::GetSurface(DWORD hashkey, ImageCacheData &surf)
{
  CSingleLock sl(&m_cs, TRUE);
  if (!m_SurfaceMap.Lookup(hashkey, surf))
  {
    //surf.surface=-1;
    return FALSE;
  }
  return TRUE;
}

BOOL ImageCacheMgr::AddSurface(DWORD hashkey, ImageCacheData &surf)
{
  CSingleLock sl(&m_cs, TRUE);
  /*
  if ((DWORD)m_SurfaceMap.GetCount() >= m_maxsize)
  {
    RemoveOldest();
    //WriteDebugString("ImageCacheMgr(%s): Removed surface to make room for %s (%i)\n",
    //                 m_id, surf.name, surf.surface);
  }
  */

#ifdef _DEBUG
  ImageCacheData tmp;
  if (GetSurface(hashkey, tmp))
  {
    ASSERT (tmp.name == surf.name);
    if (tmp.name!=surf.name)
    {
      WriteDebugString("ImageCacheMgr(%s): Hash collision - %s with %s, key %u\n",
                       m_id, tmp.name, surf.name, hashkey);
    }
  }

  //WriteDebugString("ImageCacheMgr(%s): Adding surf %s (%i) to image cache, %u total\n", 
  //                  m_id, surf.name, surf.surface, m_SurfaceMap.GetCount()+1);
#endif

  surf.insertTime=time(NULL); // in seconds
  m_SurfaceMap.SetAt(hashkey, surf);
  return TRUE;
}

BOOL ImageCacheMgr::NeedsFlushing()
{
  return ((DWORD)m_SurfaceMap.GetCount() > m_maxsize);
}

int ImageCacheMgr::compareimagecache( const void *arg1, const void *arg2 )
{
  /*
    must return
  < 0 elem1 less than elem2 
  = 0 elem1 equivalent to elem2 
  > 0 elem1 greater than elem2 
  */
  ASSERT( (arg1!=NULL) && (arg2!=NULL) );
  if ((arg1==NULL)||(arg2==NULL)) return 0;
  sortdata *pElem1 = (sortdata*)arg1;
  sortdata *pElem2 = (sortdata*)arg2;

  if (pElem1->timestamp < pElem2->timestamp) return -1;
  if (pElem1->timestamp > pElem2->timestamp) return  1;
  return 0;
}

void ImageCacheMgr::FlushOldest()
{
  CSingleLock sl(&m_cs, TRUE);
  DWORD count = m_SurfaceMap.GetCount();

  if (count <= m_maxsize)
    return;

  if (count > m_sortsize)
  {
    m_sortsize=count+50;
    if (m_sortarray!=NULL) delete [] m_sortarray;
    m_sortarray=new sortdata[m_sortsize];  
  }

  ImageCacheData tmp;
  DWORD key;
  POSITION pos = m_SurfaceMap.GetStartPosition();
  int index=0;
  while (pos!=NULL)
  {
    m_SurfaceMap.GetNextAssoc(pos, key, tmp);
    m_sortarray[index].key=key;
    m_sortarray[index].timestamp=tmp.insertTime;
    m_sortarray[index].surf=tmp.surface;
    index++;
  }
  qsort( m_sortarray, count, sizeof(sortdata), compareimagecache);
  int deleteqty = count-m_maxsize;
  for (int i=0;i<deleteqty;i++)
  {
    m_SurfaceMap.RemoveKey(m_sortarray[i].key);
    GraphicsMgr.ReleaseSurface(m_sortarray[i].surf);
  }
}

void ImageCacheMgr::RemoveOldest()
{
  POSITION pos = m_SurfaceMap.GetStartPosition();
  DWORD oldestTime=0xFFFFFFFF;
  DWORD oldestKey=0;
  long oldestSurf=-1;
#ifdef _DEBUG
  CString oldestName="";
#endif
  ImageCacheData tmp;
  DWORD key;

  while (pos!=NULL)
  {
    m_SurfaceMap.GetNextAssoc(pos, key, tmp);
    if ((!tmp.locked) && (tmp.insertTime < oldestTime))
    {
      oldestTime = tmp.insertTime;
      oldestKey = key;
      oldestSurf=tmp.surface;
#ifdef _DEBUG
      oldestName=tmp.name;
#endif
    }
  }

  if ((oldestKey > 0) && (oldestTime!=0xFFFFFFFF))
  {
#ifdef _DEBUG
    //WriteDebugString("ImageCacheMgr(%s): Releasing surf %s (%i) from image cache\n", 
    //                 m_id, oldestName, oldestSurf);
#endif
    m_SurfaceMap.RemoveKey(oldestKey);
    GraphicsMgr.ReleaseSurface(oldestSurf);
  }
}

void WallSetSlotMemType::SaveArt(int level)
{
  if (!wallFile.IsEmpty())
     saveArt(wallFile, WallDib, level);

  if (!doorFile.IsEmpty())
     saveArt(doorFile, DoorDib, level);

  if (!overlayFile.IsEmpty())
     saveArt(overlayFile, OverlayDib, level);

  if (!AVArtFile.IsEmpty())
     saveArt(AVArtFile, TransBufferDib, level);
}
void WallSetSlotMemType::SaveSound(int level)
{
  if (!soundFile.IsEmpty())
     saveSound(soundFile, level);
}

void WallSetSlotMemType::PlaySlotSound()
{
  if (!PlaySoundEffects) return;
  if (soundFile == "") return;
  if (!ValidSound(hsound))
    hsound = LoadSound(soundFile);
  if (!PlaySound(hsound,FALSE))
    hsound=-1;
}

void BackgroundSlotMemType::SaveArt(int level)
{
  if (!backgroundFile.IsEmpty())
     saveArt(backgroundFile, BackGndDib, level);
  if (!backgroundFileAlt.IsEmpty())
     saveArt(backgroundFileAlt, BackGndDib, level);
}

void BackgroundSlotMemType::SaveSound(int level)
{
  if (!soundFile.IsEmpty())
     saveSound(soundFile, level);
}

void BackgroundSlotMemType::PlaySlotSound()
{
  if (!PlaySoundEffects) return;
  if (soundFile == "") return;
  if (!ValidSound(hsound))
    hsound = LoadSound(soundFile);
  if (!PlaySound(hsound,FALSE))
    hsound=-1;
}

void WallSetSlotMemType::Serialize(CArchive &ar, double version)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     StripFilenamePath(wallFile);
     StripFilenamePath(doorFile);
     StripFilenamePath(overlayFile);
     StripFilenamePath(soundFile);
     StripFilenamePath(AVArtFile);

     AS(ar,wallFile);
     AS(ar,doorFile);
     AS(ar,overlayFile);
     AS(ar,soundFile);
     AS(ar,AVArtFile);
     ar << used;
     ar << doorFirst;
     ar << drawAV;

     AddDesignFolderToPath(wallFile);
     AddDesignFolderToPath(doorFile);
     AddDesignFolderToPath(overlayFile);
     AddDesignFolderToPath(soundFile);
     AddDesignFolderToPath(AVArtFile);

     ar << UnlockSpell;
     ar << UnlockSpellClass;
     ar << UnlockSpellLevel;
     ar << BlendOverlay;
     ar << BlendAmount;
  }
  else
  {
    DAS(ar,wallFile);
    DAS(ar,doorFile);
    DAS(ar,overlayFile);

     if (version >= _VERSION_05771_)
       DAS(ar,soundFile);

     if (version >= _VERSION_0698_)
       DAS(ar,AVArtFile);

     ar >> used;
     if (version >= _VERSION_0694_)
       ar >> doorFirst;
     if (version >= _VERSION_0698_)
       ar >> drawAV;

     AddDesignFolderToPath(wallFile);
     AddDesignFolderToPath(doorFile);
     AddDesignFolderToPath(overlayFile);
     AddDesignFolderToPath(soundFile);
     AddDesignFolderToPath(AVArtFile);

     wallHash = HashString(wallFile, IMAGE_CACHE_MAP_SIZE);
     doorHash = HashString(doorFile, IMAGE_CACHE_MAP_SIZE);
     overlayHash = HashString(overlayFile, IMAGE_CACHE_MAP_SIZE);
     AVArtHash = HashString(AVArtFile, IMAGE_CACHE_MAP_SIZE);

     ar >> UnlockSpell;
     ar >> UnlockSpellClass;
     ar >> UnlockSpellLevel;

     if (version >= _VERSION_0620_)
     {
       ar >> BlendOverlay;
       ar >> BlendAmount;
     }
  }
}

void WallSetSlotMemType::Serialize(CAR &ar, double version)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     StripFilenamePath(wallFile);
     StripFilenamePath(doorFile);
     StripFilenamePath(overlayFile);
     StripFilenamePath(soundFile);
     StripFilenamePath(AVArtFile);

     AS(ar,wallFile);
     AS(ar,doorFile);
     AS(ar,overlayFile);
     AS(ar,soundFile);
     AS(ar,AVArtFile);
     ar << used;
     ar << doorFirst;
     ar << drawAV;

     AddDesignFolderToPath(wallFile);
     AddDesignFolderToPath(doorFile);
     AddDesignFolderToPath(overlayFile);
     AddDesignFolderToPath(soundFile);
     AddDesignFolderToPath(AVArtFile);

     wallHash = HashString(wallFile, IMAGE_CACHE_MAP_SIZE);
     doorHash = HashString(doorFile, IMAGE_CACHE_MAP_SIZE);
     overlayHash = HashString(overlayFile, IMAGE_CACHE_MAP_SIZE);
     AVArtHash = HashString(AVArtFile, IMAGE_CACHE_MAP_SIZE);

     ar << UnlockSpell;
     ar << UnlockSpellClass;
     ar << UnlockSpellLevel;
     ar << BlendOverlay;
     ar << BlendAmount;
  }
  else
  {
    DAS(ar,wallFile);
    DAS(ar,doorFile);
    DAS(ar,overlayFile);

     if (version >= _VERSION_05771_)
       DAS(ar,soundFile);

     if (version >= _VERSION_0698_)
       DAS(ar,AVArtFile);

     ar >> used;
     if (version >= _VERSION_0694_)
       ar >> doorFirst;
     if (version >= _VERSION_0698_)
       ar >> drawAV;

     AddDesignFolderToPath(wallFile);
     AddDesignFolderToPath(doorFile);
     AddDesignFolderToPath(overlayFile);
     AddDesignFolderToPath(soundFile);
     AddDesignFolderToPath(AVArtFile);

     ar >> UnlockSpell;
     ar >> UnlockSpellClass;
     ar >> UnlockSpellLevel;

     if (version >= _VERSION_0620_)
     {
       ar >> BlendOverlay;
       ar >> BlendAmount;
     }
  }
}

void BackgroundSlotMemType::Serialize(CArchive &ar, double version)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     StripFilenamePath(backgroundFile);
     StripFilenamePath(backgroundFileAlt);
     StripFilenamePath(soundFile);

     AS(ar,backgroundFile);
     AS(ar,backgroundFileAlt);
     AS(ar,soundFile);
     ar << suppressStepSound;
     ar << used;

     AddDesignFolderToPath(backgroundFile);
     AddDesignFolderToPath(backgroundFileAlt);
     AddDesignFolderToPath(soundFile);

     ar << StartTime;
     ar << EndTime;
     ar << useAltBackground;
     ar << useAlphaBlend;
     ar << AlphaBlendPcnt;
     ar << useTransparency;
  }
  else
  {
    DAS(ar,backgroundFile);
    DAS(ar,backgroundFileAlt);
     if (version >= _VERSION_0640_)
     {
       DAS(ar,soundFile);
       ar >> suppressStepSound;
     }
     ar >> used;

     AddDesignFolderToPath(backgroundFile);
     AddDesignFolderToPath(backgroundFileAlt);
     AddDesignFolderToPath(soundFile);
     backgroundFileHash = HashString(backgroundFile, IMAGE_CACHE_MAP_SIZE);
     backgroundFileAltHash = HashString(backgroundFileAlt, IMAGE_CACHE_MAP_SIZE);

     ar >> StartTime;
     ar >> EndTime;
     ar >> useAltBackground;
     if (version >= _VERSION_0620_)
     {
       ar >> useAlphaBlend;
       ar >> AlphaBlendPcnt;
     }

     if (version >= _VERSION_0630_)
      ar >> useTransparency;
  }
}

void BackgroundSlotMemType::Serialize(CAR &ar, double version)
{
  //CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     StripFilenamePath(backgroundFile);
     StripFilenamePath(backgroundFileAlt);
     StripFilenamePath(soundFile);

     AS(ar,backgroundFile);
     AS(ar,backgroundFileAlt);
     AS(ar,soundFile);
     ar << suppressStepSound;
     ar << used;
  
     AddDesignFolderToPath(backgroundFile);
     AddDesignFolderToPath(backgroundFileAlt);
     AddDesignFolderToPath(soundFile);

     ar << StartTime;
     ar << EndTime;
     ar << useAltBackground;
     ar << useAlphaBlend;
     ar << AlphaBlendPcnt;
     ar << useTransparency;
  }
  else
  {
    DAS(ar,backgroundFile);
    DAS(ar,backgroundFileAlt);
     if (version >= _VERSION_0640_)
     {
       DAS(ar,soundFile);
       ar >> suppressStepSound;
     }
     ar >> used;

     AddDesignFolderToPath(backgroundFile);
     AddDesignFolderToPath(backgroundFileAlt);
     AddDesignFolderToPath(soundFile);
     backgroundFileHash = HashString(backgroundFile, IMAGE_CACHE_MAP_SIZE);
     backgroundFileAltHash = HashString(backgroundFileAlt, IMAGE_CACHE_MAP_SIZE);

     ar >> StartTime;
     ar >> EndTime;
     ar >> useAltBackground;
     if (version >= _VERSION_0620_)
     {
       ar >> useAlphaBlend;
       ar >> AlphaBlendPcnt;
     }
     if (version >= _VERSION_0630_)
      ar >> useTransparency;
  }
}

void PicDataType::PreSerialize(BOOL IsStoring)
{
  if (IsStoring)
  {
    StripFilenamePath(name);
  }
}

void PicDataType::PostSerialize(BOOL IsStoring)
{
  AddDesignFolderToPath(name);
}

void PicDataType::Serialize(CArchive &ar)
{
  PreSerialize(ar.IsStoring());
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     ar << (int)picType;

     AS(ar,name);
  }
  else
  {
     int temp;
     ar >> temp;
     picType = (SurfaceType)temp;
     DAS(ar,name);
  }
  PostSerialize(ar.IsStoring());
}

void PicDataType::Serialize(CAR &ar)
{
  PreSerialize(ar.IsStoring());
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
     ar << (int)picType;

     AS(ar,name);
  }
  else
  {
     int temp;
     ar >> temp;
     picType = (SurfaceType)temp;
     DAS(ar,name);
  }
  PostSerialize(ar.IsStoring());
}

void saveArt(CString &filename, SurfaceType type, int level, BOOL alterPath)
{
#ifdef UAFEDITOR
  if (filename.IsEmpty())
    return;

  CString saveName;
  int index = filename.ReverseFind('\\');

  if (index > 2)
    saveName = filename.Right(filename.GetLength()-(index+1));
  else
    saveName = filename;

  POSITION pos = LevelPicData[level].GetHeadPosition();
  while (pos != NULL)
  {
    if (LevelPicData[level].GetAt(pos).saveName == saveName)
    {
      if (alterPath)
      {
        // make this path point to saved pic in design folder
        filename = LevelPicData[level].GetAt(pos).name;
      }
      return;
    }
    LevelPicData[level].GetNext(pos);
  }

  CString temp;
  temp = filename;
  if (writeArtToFile(temp))
  {
    if (alterPath)
      filename = temp;
    ResourceSaveType data;
    data.name = temp;
    data.saveName = saveName;
    LevelPicData[level].AddTail(data);
  }
#endif
}


void deleteAllLevelArt(int level)
{
#ifdef UAFEDITOR
  LevelPicData[level].RemoveAll();
#endif
}

void WriteRequiredArtList()
{
#ifdef UAFEDITOR
  static char dsnPath[_MAX_PATH+1];
  GetDesignPath(dsnPath);
  CString filename;
  filename.Format("%sReqArt.txt", dsnPath);
  FILE *fp = fopen(filename, "w");
  CString temp;
  
  if (fp != NULL)
  {
    for (int i=0;i<=MAX_LEVELS;i++)
    {
      POSITION pos = LevelPicData[i].GetHeadPosition();
      while (pos != NULL)
      {
        if (i < MAX_LEVELS)
        {
          temp.Format("Level %u: %s\n", i+1, LevelPicData[i].GetNext(pos).name);
          fprintf(fp, temp);
        }
        else
        {
          temp.Format("Global: %s\n", LevelPicData[i].GetNext(pos).name);
          fprintf(fp, temp);
        }
      }
    }
  }

  if (fp != NULL)
    fclose(fp);
#endif
}

BOOL writeArtToFile(CString &filename)
{
#ifdef UAFEDITOR
 static char dsnPath[_MAX_PATH+1];

 if (filename.IsEmpty())
    return TRUE;

  GetDesignPath(dsnPath);

  // determine path to design folder
  CString saveName;
  int index = filename.ReverseFind('\\');

  if (index > 2)
    saveName.Format("%s%s", dsnPath, filename.Right(filename.GetLength()-(index+1)));
  else
    saveName.Format("%s%s", dsnPath, filename);

  // source file missing?
  if (!FileExists(filename))
  {
    CString tmp;
    tmp = filename;
    if (!SearchForFile(tmp)) // returns full path file
    {
      WriteDebugString("Cant find file \'%s\' in writeArtToFile()\n", tmp);
      return FALSE;
    }
    filename = tmp;
  }

  // if same file is already in design folder,
  // skip the copy
  if (   (FileExists(saveName)) 
      && (FileSize(saveName) == FileSize(filename)))
    return TRUE;

  SetFileAttributes(saveName, FILE_ATTRIBUTE_NORMAL);
  // copy src to design folder
  if (CopyFile(filename, saveName, FALSE))
  {
    SetFileAttributes(saveName, FILE_ATTRIBUTE_NORMAL);
    filename = saveName;
    return TRUE;
  }  
  else
  {
    WriteDebugString("CopyFile failed (%u): \'%s\' to \'%s\'\n", GetLastError(), filename, saveName);
    return FALSE;
  }
#else
  return FALSE;
#endif
}

void WallSetSlotMemType::ClearResources()
{
  GraphicsMgr.ReleaseSurface(wallSurface);
  GraphicsMgr.ReleaseSurface(doorSurface);
  GraphicsMgr.ReleaseSurface(overlaySurface);
  GraphicsMgr.ReleaseSurface(AVArtSurface);
  if (pSndMgr != NULL) pSndMgr->RemoveSound(hsound);
}

void BackgroundSlotMemType::ClearResources()
{
  GraphicsMgr.ReleaseSurface(backgroundSurface);
  GraphicsMgr.ReleaseSurface(backgroundSurfaceAlt);
  if (pSndMgr != NULL) pSndMgr->RemoveSound(hsound);
}

void clearArtSlots(void)
{
  int i;
  
   HBarVPArt.Clear();
   VBarVPArt.Clear();
   FrameVPArt.Clear();
   VBarCombArt.Clear();
   HBarCombArt.Clear();
   CombatCursorArt.Clear();
   CombatDeathIconArt.Clear();
   CombatDeathArt.Clear();
   CursorArt.Clear();

  for (i=0; i<MAX_WALLSETS;i++)
    WallSets[i].Clear();
  for (i=0; i<MAX_BACKGROUNDS;i++)
    BackgroundSets[i].Clear();

  if (pWallSlotMgr!=NULL)
    pWallSlotMgr->Clear();
  if (pBgSlotMgr!=NULL)
    pBgSlotMgr->Clear();
}

BOOL GetWallSlotSurfaces(WallSetSlotMemType &data)
{
  if (pWallSlotMgr==NULL)
  {
    ASSERT(FALSE);
    return FALSE;
  }

  BOOL success=TRUE;
  ImageCacheData tmp;  
  RECT rect;
  int tmpFormat = -1;
  data.SlotType = DEFAULT_FORMAT_TYPE;

  if (!data.wallFile.IsEmpty())
  {             
    if (data.wallHash==0) data.wallHash = HashString(data.wallFile, IMAGE_CACHE_MAP_SIZE);
    if (pWallSlotMgr->GetSurface(data.wallHash, tmp))
    {
      data.wallSurface = tmp.surface;
    }
    else
    {
      GraphicsMgr.ReleaseSurface(data.wallSurface);
      if ((data.wallSurface = GraphicsMgr.AddSurface(data.wallFile, WallDib)) < 0)
      {
        if (data.wallFile.Find("wa_",0) < 0)
        {
          CString name(data.wallFile);
          CString path("");
          GetFilenamePath(data.wallFile, path);
          StripFilenamePath(name);
          path = "wa_" + name;

          if ((data.wallSurface = GraphicsMgr.AddSurface(path, WallDib)) >= 0)
          {
            WriteDebugString("Adjusted wall file %s to %s in GetWallSlotSurfaces\n", data.wallFile, path);
            data.wallFile = path;
          }
        }

        if (data.wallSurface < 0)
        {
          WriteDebugString("Failed to load wall surface \'%s\' in GetWallSlotSurfaces\n", data.wallFile);
          success = FALSE;
        }
      }
      else
      {
        GraphicsMgr.GetSurfaceRect(data.wallSurface ,&rect);
        tmpFormat = WallFormatMgr.GetFormat(rect.right-rect.left, rect.bottom-rect.top);
        tmp.name = data.wallFile;
        tmp.surface = data.wallSurface;
        tmp.type = tmpFormat;
        pWallSlotMgr->AddSurface(data.wallHash, tmp);
      }
    }
  }
  
  if (!data.doorFile.IsEmpty())
  {        
    if (data.doorHash==0) data.doorHash = HashString(data.doorFile, IMAGE_CACHE_MAP_SIZE);
    if (pWallSlotMgr->GetSurface(data.doorHash, tmp))
    {
      data.doorSurface = tmp.surface;
    }
    else
    {
      GraphicsMgr.ReleaseSurface(data.doorSurface);      
      if ((data.doorSurface = GraphicsMgr.AddSurface(data.doorFile, DoorDib)) < 0)
      {
        if (data.doorFile.Find("dr_",0) < 0)
        {
          CString name(data.doorFile);
          CString path("");
          GetFilenamePath(data.doorFile, path);
          StripFilenamePath(name);
          path = "dr_" + name;

          if ((data.doorSurface = GraphicsMgr.AddSurface(path, DoorDib)) >= 0)
          {
            WriteDebugString("Adjusted door file %s to %s in GetWallSlotSurfaces\n", data.doorFile, path);
            data.doorFile = path;
          }
        }

        if (data.doorSurface < 0)
        {
          WriteDebugString("Failed to load door surface \'%s\' in GetWallSlotSurfaces\n", data.doorFile);
          success = FALSE;
        }
      }
      else
      {
        if (tmpFormat == -1)
        {
          GraphicsMgr.GetSurfaceRect(data.doorSurface ,&rect);
          tmpFormat = WallFormatMgr.GetFormat(rect.right-rect.left, rect.bottom-rect.top);
        }
        tmp.name = data.doorFile;
        tmp.surface = data.doorSurface;
        tmp.type = tmpFormat;
        pWallSlotMgr->AddSurface(data.doorHash, tmp);
      }
    }
  }
  
  if (!data.overlayFile.IsEmpty())
  {            
    if (data.overlayHash==0) data.overlayHash = HashString(data.overlayFile, IMAGE_CACHE_MAP_SIZE);
    if (pWallSlotMgr->GetSurface(data.overlayHash, tmp))
    {
      data.overlaySurface = tmp.surface;
    }
    else
    {
      GraphicsMgr.ReleaseSurface(data.overlaySurface);
      if ((data.overlaySurface = GraphicsMgr.AddSurface(data.overlayFile, OverlayDib)) < 0)
      {
        if (data.overlayFile.Find("ov_",0) < 0)
        {
          CString name(data.overlayFile);
          CString path("");
          GetFilenamePath(data.overlayFile, path);
          StripFilenamePath(name);
          path = "ov_" + name;

          if ((data.overlaySurface = GraphicsMgr.AddSurface(path, OverlayDib)) >= 0)
          {
            WriteDebugString("Adjusted overlay file %s to %s in GetWallSlotSurfaces\n", data.overlayFile, path);
            data.overlayFile = path;
          }
        }

        if (data.overlaySurface < 0)
        {
          WriteDebugString("Failed to load overlay surface \'%s\' in GetWallSlotSurfaces\n", data.overlayFile);
          success = FALSE;
        }
      }
      else
      {
        if (tmpFormat == -1)
        {
          GraphicsMgr.GetSurfaceRect(data.overlaySurface ,&rect);
          tmpFormat = WallFormatMgr.GetFormat(rect.right-rect.left, rect.bottom-rect.top);
        }
        tmp.name = data.overlayFile;
        tmp.surface = data.overlaySurface;
        tmp.type = tmpFormat;
        pWallSlotMgr->AddSurface(data.overlayHash, tmp);
      }
    }
  }

  if (tmpFormat == -1) tmpFormat = DEFAULT_FORMAT_TYPE;
  data.SlotType = tmpFormat;

  if ((!data.AVArtTried) && (!data.AVArtFile.IsEmpty()))
  {            
    if (data.AVArtHash==0) data.AVArtHash = HashString(data.AVArtFile, IMAGE_CACHE_MAP_SIZE);
    if (pWallSlotMgr->GetSurface(data.AVArtHash, tmp))
    {
      data.AVArtSurface = tmp.surface;
    }
    else
    {
      GraphicsMgr.ReleaseSurface(data.AVArtSurface);
      if (!LoadAreaViewArt(data.AVArtFile, data.AVArtSurface, data.AVZoneStyle))
      {
        data.AVArtTried=true;
        WriteDebugString("Failed to load area view art \'%s\' in GetWallSlotSurfaces\n", data.AVArtFile);
        // will default to globalData.mapart
      }
      else
      {
        tmp.name = data.AVArtFile;
        tmp.surface = data.AVArtSurface;
        tmp.type = DEFAULT_FORMAT_TYPE;
        pWallSlotMgr->AddSurface(data.AVArtHash, tmp);
      }
    }
  }

  return success;
}

BOOL GetBackgroudSlotSurfaces(BackgroundSlotMemType &data)
{
  if (pBgSlotMgr==NULL)
  {
    ASSERT(FALSE);
    return FALSE;
  }

  BOOL success=TRUE;
  ImageCacheData tmp;  
  data.SlotType = DEFAULT_FORMAT_TYPE;
  
  if (!data.backgroundFile.IsEmpty())
  {  
    if (data.backgroundFileHash==0) data.backgroundFileHash = HashString(data.backgroundFile, IMAGE_CACHE_MAP_SIZE);
    if (pBgSlotMgr->GetSurface(data.backgroundFileHash, tmp))
    {
      data.backgroundSurface = tmp.surface;
    }
    else
    {
      GraphicsMgr.ReleaseSurface(data.backgroundSurface);  
      if ((data.backgroundSurface = GraphicsMgr.AddSurface(data.backgroundFile, BackGndDib)) < 0)
      {
        if (data.backgroundFile.Find("bd_",0) < 0)
        {
          CString name(data.backgroundFile);
          CString path("");
          GetFilenamePath(data.backgroundFile, path);
          StripFilenamePath(name);
          path = "bd_" + name;

          if ((data.backgroundSurface = GraphicsMgr.AddSurface(path, BackGndDib)) >= 0)
          {
            WriteDebugString("Adjusted background file %s to %s in GetBackgroudSlotSurfaces\n", data.backgroundFile, path);
            data.backgroundFile = path;
          }
        }

        if (data.backgroundSurface < 0)
        {
          WriteDebugString("Failed to load background surface \'%s\' in GetBackgroudSlotSurfaces\n", data.backgroundFile);
          success = FALSE;
        }
      }
      else
      {
        tmp.name = data.backgroundFile;
        tmp.surface = data.backgroundSurface;
        pBgSlotMgr->AddSurface(data.backgroundFileHash, tmp);
        GraphicsMgr.GetSurfaceRect(data.backgroundSurface, &data.surfRect);
      }
    }
  }
  
  if (!data.backgroundFileAlt.IsEmpty())
  { 
    if (data.backgroundFileAltHash==0) data.backgroundFileAltHash = HashString(data.backgroundFileAlt, IMAGE_CACHE_MAP_SIZE);
    if (pBgSlotMgr->GetSurface(data.backgroundFileAltHash, tmp))
    {
      data.backgroundSurfaceAlt = tmp.surface;
    }
    else
    {
      GraphicsMgr.ReleaseSurface(data.backgroundSurfaceAlt); 
      SurfaceType type = (data.useTransparency?TransBufferDib:BackGndDib);
      if ((data.backgroundSurfaceAlt = GraphicsMgr.AddSurface(data.backgroundFileAlt, type)) < 0)
      {
        if (data.backgroundFileAlt.Find("bd_",0) < 0)
        {
          CString name(data.backgroundFileAlt);
          CString path("");
          GetFilenamePath(data.backgroundFileAlt, path);
          StripFilenamePath(name);
          path = "bd_" + name;

          if ((data.backgroundSurfaceAlt = GraphicsMgr.AddSurface(path, type)) >= 0)
          {
            WriteDebugString("Adjusted background alt file %s to %s in GetBackgroudSlotSurfaces\n", data.backgroundFileAlt, path);
            data.backgroundFileAlt = path;
          }
        }

        if (data.backgroundSurfaceAlt < 0)
        {
          WriteDebugString("Failed to load background alt surface \'%s\' in GetBackgroudSlotSurfaces\n", data.backgroundFileAlt);
          success = FALSE;
          data.useAltBackground = FALSE;
          data.useAlphaBlend = FALSE;
          data.useTransparency = FALSE;
        }
      }
      else
      {
        tmp.name = data.backgroundFileAlt;
        tmp.surface = data.backgroundSurfaceAlt;
        pBgSlotMgr->AddSurface(data.backgroundFileAltHash, tmp);
      }
    }
  }

  if (data.StartTime < 0)        data.StartTime = 0;
  if (data.StartTime > 2359)     data.StartTime = 2359;
  if (data.EndTime < 0)          data.EndTime = 0;
  if (data.EndTime > 2359)       data.EndTime = 2359;
  if (data.AlphaBlendPcnt < 0)   data.AlphaBlendPcnt = 0;
  if (data.AlphaBlendPcnt > 256) data.AlphaBlendPcnt = 256;

  return success;
}

// loads first 16 wall slots and background slots
BOOL loadWallSetSurfaces()
{
  BOOL success = TRUE;
  int i;
  int count=0;
  
  //for (i=0; i<MAX_WALLSETS; i++)
  for (i=0; i<16; i++)
  {
    if (!GetWallSlotSurfaces(WallSets[i]))
    {
#ifdef _DEBUG
      // substitute default art for the missing art, just to
      // help out when debugging designs from other people
      // that doesn't include the artwork
      if ((!WallSets[i].wallFile.IsEmpty()) && (WallSets[i].wallSurface == -1))
        WallSets[i].wallFile = "wa_wall1.png";
      if ((!WallSets[i].doorFile.IsEmpty()) && (WallSets[i].doorSurface == -1))
        WallSets[i].doorFile = "dr_door5.png";
      if ((!WallSets[i].overlayFile.IsEmpty()) && (WallSets[i].overlaySurface == -1))
        WallSets[i].overlayFile = "";

      if (!GetWallSlotSurfaces(WallSets[i]))
        WriteDebugString("Failed GetWallSlotSurfaces() after substituting default art\n");
#endif
    }
    else
      count++;
  }
  
  //for (i=0; i<MAX_BACKGROUNDS;i++)
  for (i=0; i<16;i++)
  {
    if (!GetBackgroudSlotSurfaces(BackgroundSets[i]))
    {
#ifdef _DEBUG
      // substitute default art for the missing art
      if ((!BackgroundSets[i].backgroundFile.IsEmpty()) && (BackgroundSets[i].backgroundSurface == -1))
      {
        BackgroundSets[i].backgroundFile = "bd_background1.png";
        BackgroundSets[i].backgroundFileAlt="";
      }
      if (!GetBackgroudSlotSurfaces(BackgroundSets[i]))
        WriteDebugString("Failed GetBackgroudSlotSurfaces() after substituting default art\n");
#endif
    }
    else
      count++;
  }
  
  if (count <= 0)
    WriteDebugString("No Wallsets or Backdrops were loaded in loadWallSetSurfaces()\n");
  return success;
}

void flushOldImages()
{
  static bool flushswitch=true;
  if (flushswitch)
  {
    if (pWallSlotMgr!=NULL) pWallSlotMgr->FlushOldest();
  }
  else
  {
    if (pBgSlotMgr!=NULL) pBgSlotMgr->FlushOldest();
  }
  flushswitch=!flushswitch;
}

BYTE WallSetSlotMemType::GetFormat()
{
  // Format is asked for before images are displayed.
  // To ensure correct format is used, load images
  // first if needed.
  if (wallSurface<=0)    GetWallSurface();
  if (doorSurface<=0)    GetDoorSurface();
  if (overlaySurface<=0) GetOverlaySurface();
  return SlotType;
}

long WallSetSlotMemType::GetWallSurface()
{
  if (wallFile.IsEmpty()) return -1;
  //WriteDebugString("WallSlot-GetWallSurface() entry %s (%i)\n", wallFile, wallSurface);
  //if (wallSurface <= 0)
  {
    static ImageCacheData tmp;
    if (wallHash==0) wallHash = HashString(wallFile, IMAGE_CACHE_MAP_SIZE);
    if ((pWallSlotMgr!=NULL) && (pWallSlotMgr->GetSurface(wallHash, tmp)))
    {
      //WriteDebugString("WallSlot-GetWallSurface() hash lookup success %s (%i)\n", wallFile, tmp.surface);
      wallSurface = tmp.surface;
      SlotType = tmp.type;
      if (!GraphicsMgr.ValidSurface(wallSurface))
      {
        wallSurface=-1;
        //WriteDebugString("WallSlot-GetWallSurface() hash lookup surface invalid %s\n", wallFile);
      }
    }
    else
    {
      wallSurface=-1;
      //WriteDebugString("WallSlot-GetWallSurface() hash lookup failed %s\n", wallFile);
    }
    if (wallSurface < 0)
    {
      if ((wallSurface = GraphicsMgr.AddSurface(wallFile, WallDib)) < 0)
      {
        if (wallFile.Find("wa_",0) < 0)
        {
          CString name(wallFile);
          CString path("");
          GetFilenamePath(wallFile, path);
          StripFilenamePath(name);
          path = "wa_" + name;

          if ((wallSurface = GraphicsMgr.AddSurface(path, WallDib)) >= 0)
          {
            WriteDebugString("Adjusted wall file %s to %s in GetWallSurface\n", wallFile, path);
            wallFile = path;
          }
        }

        if (wallSurface < 0)
          WriteDebugString("Failed to load wall surface \'%s\' in GetWallSurface\n", wallFile);
      }
      else
      {
        RECT rect;
        GraphicsMgr.GetSurfaceRect(wallSurface ,&rect);
        SlotType = WallFormatMgr.GetFormat(rect.right-rect.left, rect.bottom-rect.top);
        tmp.name = wallFile;
        tmp.surface = wallSurface;
        tmp.type=SlotType;
        pWallSlotMgr->AddSurface(wallHash, tmp);
      }
    }
  }
  //WriteDebugString("WallSlot-GetWallSurface() returns %s (%i)\n", wallFile, wallSurface);
  return wallSurface;
}

long WallSetSlotMemType::GetDoorSurface()
{
  if (doorFile.IsEmpty()) return -1;
  //if (doorSurface <= 0)
  //WriteDebugString("WallSlot-GetDoorSurface() entry %s (%i)\n", doorFile, doorSurface);
  {
    static ImageCacheData tmp;
    if (doorHash==0) doorHash = HashString(doorFile, IMAGE_CACHE_MAP_SIZE);
    if ((pWallSlotMgr!=NULL) && (pWallSlotMgr->GetSurface(doorHash, tmp)))
    {
      //WriteDebugString("WallSlot-GetDoorSurface() hash lookup success %s (%i)\n", doorFile, tmp.surface);
      doorSurface = tmp.surface;
      if (wallSurface <= 0) SlotType=tmp.type;
      if (!GraphicsMgr.ValidSurface(doorSurface))
      {
        doorSurface=-1;
        //WriteDebugString("WallSlot-GetDoorSurface() hash lookup surface invalid %s\n", doorFile);
      }
    }
    else
    {
      doorSurface=-1;
      //WriteDebugString("WallSlot-GetDoorSurface() hash lookup failed %s\n", doorFile);
    }

    if (doorSurface < 0)
    {
      if ((doorSurface = GraphicsMgr.AddSurface(doorFile, DoorDib)) < 0)
      {
        if (doorFile.Find("dr_",0) < 0)
        {
          CString name(doorFile);
          CString path("");
          GetFilenamePath(doorFile, path);
          StripFilenamePath(name);
          path = "dr_" + name;

          if ((doorSurface = GraphicsMgr.AddSurface(path, DoorDib)) >= 0)
          {
            WriteDebugString("Adjusted door file %s to %s in GetDoorSurface\n", doorFile, path);
            doorFile = path;
          }
        }

        if (doorSurface < 0)
          WriteDebugString("Failed to load door surface \'%s\' in GetDoorSurface\n", doorFile);
      }
      else
      {
        if (wallSurface <= 0) // only reset format if wall surface hasn't already
        {
          RECT rect;
          GraphicsMgr.GetSurfaceRect(doorSurface ,&rect);        
          SlotType = WallFormatMgr.GetFormat(rect.right-rect.left, rect.bottom-rect.top);
        }
        tmp.name = doorFile;
        tmp.surface = doorSurface;        
        tmp.type=SlotType;       
        pWallSlotMgr->AddSurface(doorHash, tmp);
      }
    }
  }
  //WriteDebugString("WallSlot-GetDoorSurface() returns %s (%i)\n", doorFile, doorSurface);
  return doorSurface;
}

long WallSetSlotMemType::GetOverlaySurface()
{
  if (overlayFile.IsEmpty()) return -1;
  //if (overlaySurface <= 0)
  {
    static ImageCacheData tmp;
    if (overlayHash==0) overlayHash = HashString(overlayFile, IMAGE_CACHE_MAP_SIZE);
    if ((pWallSlotMgr!=NULL) && (pWallSlotMgr->GetSurface(overlayHash, tmp)))
    {
      overlaySurface = tmp.surface;
      if ((wallSurface <= 0)&&(doorSurface <= 0)) SlotType = tmp.type;
      if (!GraphicsMgr.ValidSurface(overlaySurface))
        overlaySurface=-1;
    }
    else
    {
      overlaySurface=-1;
      //WriteDebugString("WallSlot-GetOverlaySurface() hash lookup failed %s\n", overlayFile);
    }

    if (overlaySurface < 0)
    {
      if ((overlaySurface = GraphicsMgr.AddSurface(overlayFile, OverlayDib)) < 0)
      {
        if (overlayFile.Find("ov_",0) < 0)
        {
          CString name(overlayFile);
          CString path("");
          GetFilenamePath(overlayFile, path);
          StripFilenamePath(name);
          path = "ov_" + name;

          if ((overlaySurface = GraphicsMgr.AddSurface(path, OverlayDib)) >= 0)
          {
            WriteDebugString("Adjusted overlay file %s to %s in GetOverlaySurface\n", overlayFile, path);
            overlayFile = path;
          }
        }

        if (overlaySurface < 0)
          WriteDebugString("Failed to load overlay surface \'%s\' in GetOverlaySurface\n", overlayFile);
      }
      else
      {
        if ((wallSurface <= 0)&&(doorSurface <= 0)) // only reset format if wall and door surfaces haven't already
        {
          RECT rect;
          GraphicsMgr.GetSurfaceRect(overlaySurface ,&rect);        
          SlotType = WallFormatMgr.GetFormat(rect.right-rect.left, rect.bottom-rect.top);
        }
        tmp.name = overlayFile;
        tmp.surface = overlaySurface;
        tmp.type = SlotType;
        pWallSlotMgr->AddSurface(overlayHash, tmp);
      }
    }
  }
  //WriteDebugString("WallSlot-GetOverlaySurface() returns %s (%i)\n", overlayFile, overlaySurface);
  return overlaySurface;
}

long WallSetSlotMemType::GetAVArtSurface()
{
  if ((AVArtTried) || (AVArtFile.IsEmpty()))
  {
    AVZoneStyle=FALSE;
    return -1;
  }

  {
    static ImageCacheData tmp;
    if (AVArtHash==0) AVArtHash = HashString(AVArtFile, IMAGE_CACHE_MAP_SIZE);
    if ((pWallSlotMgr!=NULL) && (pWallSlotMgr->GetSurface(AVArtHash, tmp)))
    {
      AVArtSurface = tmp.surface;
      if (!GraphicsMgr.ValidSurface(AVArtSurface)) 
      {
        AVArtSurface=-1;
      }
      else
      {
        RECT tmpRect;
        GraphicsMgr.GetSurfaceRect(AVArtSurface, &tmpRect);
        // Make sure area view art is big enough to contain
        // the zone squares before enabling their usage.
        // The old style file was 92x80 pixels
        if (   ((tmpRect.right-tmpRect.left) >= 92)
            && ((tmpRect.bottom-tmpRect.top) > 80))
          AVZoneStyle = TRUE;
        else
          AVZoneStyle = FALSE;
      }
    }
    else
      AVArtSurface=-1;

    if (AVArtSurface < 0)
    {
      if (!LoadAreaViewArt(AVArtFile, AVArtSurface, AVZoneStyle))
      {
        AVArtTried=true;
        WriteDebugString("Failed to load area view art \'%s\' in WallSetSlotMemType::GetAVArtSurface\n", AVArtFile);
      }
      else
      {
        tmp.name = AVArtFile;
        tmp.surface = AVArtSurface;
        tmp.type=DEFAULT_FORMAT_TYPE;
        pWallSlotMgr->AddSurface(AVArtHash, tmp);
      }
    }
  }
  return AVArtSurface;
}

long BackgroundSlotMemType::GetBackgroundSurface()
{
  if (backgroundFile.IsEmpty()) return -1;
  //if (backgroundSurface <= 0)
  {    
    ImageCacheData tmp;
    if (backgroundFileHash==0) backgroundFileHash = HashString(backgroundFile, IMAGE_CACHE_MAP_SIZE);
    if ((pBgSlotMgr!=NULL) && (pBgSlotMgr->GetSurface(backgroundFileHash, tmp)))
    {
      backgroundSurface = tmp.surface;
      if (!GraphicsMgr.ValidSurface(backgroundSurface))
        backgroundSurface=-1;
    }
    else
    {
      backgroundSurface=-1;
      //WriteDebugString("WallSlot-GetBackgroundSurface() hash lookup failed %s\n", backgroundFile);
    }

    if (backgroundSurface < 0)
    {
      SlotType = DEFAULT_FORMAT_TYPE;
      if ((backgroundSurface = GraphicsMgr.AddSurface(backgroundFile, BackGndDib)) < 0)
      {
        if (backgroundFile.Find("bd_",0) < 0)
        {
          CString name(backgroundFile);
          CString path("");
          GetFilenamePath(backgroundFile, path);
          StripFilenamePath(name);
          path = "bd_" + name;

          if ((backgroundSurface = GraphicsMgr.AddSurface(path, BackGndDib)) >= 0)
          {
            WriteDebugString("Adjusted background file %s to %s in GetBackgroundSurface\n", backgroundFile, path);
            backgroundFile = path;
          }
        }

        if (backgroundSurface < 0)
          WriteDebugString("Failed to load background surface \'%s\' in GetBackgroundSurface\n", backgroundFile);
      }
      else
      {
        tmp.name = backgroundFile;
        tmp.surface = backgroundSurface;
        pBgSlotMgr->AddSurface(backgroundFileHash, tmp);
        GraphicsMgr.GetSurfaceRect(backgroundSurface,&surfRect);        
      }
    }
  }
  return backgroundSurface;
}

long BackgroundSlotMemType::GetBackgroundAltSurface()
{
  if (backgroundFileAlt.IsEmpty()) return -1;
  //if (backgroundSurfaceAlt <= 0)
  {    
    ImageCacheData tmp;
    if (backgroundFileAltHash==0) backgroundFileAltHash = HashString(backgroundFileAlt, IMAGE_CACHE_MAP_SIZE);
    if ((pBgSlotMgr!=NULL) && (pBgSlotMgr->GetSurface(backgroundFileAltHash, tmp)))
    {
      backgroundSurfaceAlt = tmp.surface;
      if (!GraphicsMgr.ValidSurface(backgroundSurfaceAlt))
        backgroundSurfaceAlt=-1;
    }
    else
    {
      backgroundSurfaceAlt=-1;
      //WriteDebugString("WallSlot-GetBackgroundAltSurface() hash lookup failed %s\n", backgroundFileAlt);
    }
    if (backgroundSurfaceAlt < 0)
    {
      SlotType = DEFAULT_FORMAT_TYPE;
      SurfaceType type = (useTransparency?TransBufferDib:BackGndDib);
      if ((backgroundSurfaceAlt = GraphicsMgr.AddSurface(backgroundFileAlt, type)) < 0)
      {
        if (backgroundFileAlt.Find("bd_",0) < 0)
        {
          CString name(backgroundFileAlt);
          CString path("");
          GetFilenamePath(backgroundFileAlt, path);
          StripFilenamePath(name);
          path = "bd_" + name;

          if ((backgroundSurfaceAlt = GraphicsMgr.AddSurface(path, type)) >= 0)
          {
            WriteDebugString("Adjusted background alt file %s to %s in GetBackgroundAltSurface\n", backgroundFileAlt, path);
            backgroundFileAlt = path;
          }
        }

        if (backgroundSurfaceAlt < 0)
          WriteDebugString("Failed to load background alt surface \'%s\' in GetBackgroundAltSurface\n", backgroundFileAlt);
      }
      else
      {
        tmp.name = backgroundFileAlt;
        tmp.surface = backgroundSurfaceAlt;
        pBgSlotMgr->AddSurface(backgroundFileAltHash, tmp);
        GraphicsMgr.GetSurfaceRect(backgroundSurfaceAlt,&surfRect);        
      }
    }
  }
  return backgroundSurfaceAlt;
}

void WallSetSlotMemType::Clear()
{ 
  SlotType = DEFAULT_FORMAT_TYPE;
  ClearResources();  
  wallHash=0;
  doorHash=0;
  overlayHash=0;
  AVArtHash=0;
  wallSurface=doorSurface=overlaySurface=-1;hsound=-1;
  wallFile="";doorFile="";overlayFile="";soundFile="";
  used=FALSE;
  doorFirst=FALSE;
  drawAV=TRUE;
  UnlockSpell=-1;UnlockSpellClass=0;UnlockSpellLevel=0;
  BlendOverlay=FALSE;
  BlendAmount=0;
  AVArtTried=false;
  AVZoneStyle=FALSE;
  AVArtFile="";
  AVArtSurface=-1;
}

WallSetSlotMemType& WallSetSlotMemType::operator =(const WallSetSlotMemType& src)
{
  if (&src == this)
    return *this;
  Clear();
  used=src.used;
  doorFirst=src.doorFirst;
  drawAV=src.drawAV;
  wallSurface=src.wallSurface;
  doorSurface=src.doorSurface;
  overlaySurface=src.overlaySurface;
  AVArtSurface=src.AVArtSurface;
  hsound=src.hsound;
  BlendOverlay=src.BlendOverlay;
  BlendAmount=src.BlendAmount;
  UnlockSpell=src.UnlockSpell;UnlockSpellClass=src.UnlockSpellClass;UnlockSpellLevel=src.UnlockSpellLevel;
  wallHash=src.wallHash;
  doorHash=src.doorHash;
  overlayHash=src.overlayHash;
  AVArtHash=src.AVArtHash;
  if (!src.wallFile.IsEmpty()) wallFile=src.wallFile;
  if (!src.doorFile.IsEmpty()) doorFile=src.doorFile;
  if (!src.overlayFile.IsEmpty()) overlayFile=src.overlayFile;
  if (!src.soundFile.IsEmpty()) soundFile=src.soundFile;
  if (!src.AVArtFile.IsEmpty()) AVArtFile=src.AVArtFile;
  return *this;
}

void BackgroundSlotMemType::Clear()
{ 
  SlotType = DEFAULT_FORMAT_TYPE;
  ClearResources();
  suppressStepSound=FALSE;
  useAltBackground=FALSE; 
  backgroundSurface=backgroundSurfaceAlt=hsound-1; 
  backgroundFileAlt="";backgroundFile="";soundFile="";
  used=FALSE; 
  AlphaBlendPcnt=0;
  useAlphaBlend=FALSE;
  useTransparency = FALSE;
  backgroundFileHash=0;
  backgroundFileAltHash=0;
  memset(&surfRect, 0, sizeof(surfRect));
}

BackgroundSlotMemType& BackgroundSlotMemType::operator =(const BackgroundSlotMemType& src)
{
   if (&src == this)
     return *this;
   Clear();
   suppressStepSound=src.suppressStepSound;
   used=src.used; 
   hsound=src.hsound;
   useAltBackground=src.useAltBackground;
   useAlphaBlend=src.useAlphaBlend;
   useTransparency=src.useTransparency;
   AlphaBlendPcnt=src.AlphaBlendPcnt;
   backgroundSurface=src.backgroundSurface;
   backgroundSurfaceAlt=src.backgroundSurfaceAlt;
   StartTime=src.StartTime;
   EndTime=src.EndTime;
   backgroundFileHash=src.backgroundFileHash;
   backgroundFileAltHash=src.backgroundFileAltHash;
   if (!src.backgroundFile.IsEmpty()) backgroundFile = src.backgroundFile;
   if (!src.backgroundFileAlt.IsEmpty()) backgroundFileAlt = src.backgroundFileAlt;
   if (!src.soundFile.IsEmpty()) soundFile=src.soundFile;
   return *this;
}

void PicDataType::Clear() 
{ 
  picType=BogusDib;name="";surface=-1;
}

PicDataType& PicDataType::operator =(const PicDataType& src) 
{
 if (&src == this)
   return *this;
 Clear();
 picType=src.picType;
 surface=src.surface;
 if (!src.name.IsEmpty()) name=src.name;
 return *this;
}

PicDataType::PicDataType(const PicDataType& src) 
{ 
  *this=src; 
}