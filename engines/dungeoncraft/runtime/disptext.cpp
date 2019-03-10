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


#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/level.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/Graphics.h"
#include "dungeoncraft/shared/DispText.h"
//#include "dungeoncraft/shared/GameMenu.h"
#include "dungeoncraft/shared/combatants.h"
#include "dungeoncraft/shared/DrawTile.h"
#include "dungeoncraft/shared/monster.h"
#include "dungeoncraft/shared/treas.h"
#include "dungeoncraft/shared/Viewport.h"
#include "dungeoncraft/shared/FormattedText.h"
//#include "dungeoncraft/shared/Getinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int ItemsOnPage = 0;
int startItem;

RECT InventoryRects[Items_Per_Page];
RECT PartyNameRects[MAX_PARTY_MEMBERS];

CString CombatMsg;
CString timeDelayMsg; // appears in menu-bar area for x seconds

// error strings will be wrapped to fit message box, but
// make sure they fit in one box. There is no processing
// to let the player "Press Return to Continue" to see
// multiple boxes of error text continuation.
//
char *miscErrorText[MAX_MISC_ERROR_MSGS] = 
{
   "No Error",
   "UNKNOWN ERROR",
   "THE CHARACTER IS DEAD",
   "THE CHARACTER IS UNCONSCIOUS",
   "THE CHARACTER HAS FLED",
   "THE CHARACTER IS PETRIFIED",
   "THE CHARACTER IS GONE",
   "THE CHARACTER IS ANIMATED",
   "THE CHARACTER IS TEMPORARILY GONE",
   "THE CHARACTER IS RUNNING AWAY",
   "THE CHARACTER IS DYING",
   "NO MONEY",
   "NOT ENOUGH MONEY",
   "YOU HAVE THE MAXIMUM ITEM TYPES THAT YOU CAN CARRY",
   "YOU ARE NOT STRONG ENOUGH TO CARRY ANY MORE",
   "THIS ITEM TAKES TWO HANDS TO USE",
   "NOT USABLE BY YOUR CLASS",
   "A SIMILIAR ITEM IS ALREADY READIED",
   "NO ICONS AVAILABLE",
   "NO SMALL PICTURES AVAILABLE",
   "NO DESIGNS IN DIRECTORY",
   "FAILED TO LOAD LEVEL FROM DISK",
   "INVALID GAME STATE ENCOUNTERED",
   "INVALID EVENT TYPE ENCOUNTERED",
   "INVALID MENU STATE ENCOUNTERED",
   "INVALID TASK STATE ENCOUNTERED",
   "YOUR PARTY CANNOT CONTAIN MORE CHARACTERS",
   "YOUR PARTY CANNOT CONTAIN MORE NPC's",
   "ITEM MUST NOT BE READIED",
   "SOME MONSTER COMBAT ICONS CANNOT BE FOUND",
   "GENERAL GRAPHICS ERROR",
   "Design version higher than runtime, load failed, please upgrade",
   "Saved Game version higher than runtime, load failed, please upgrade",
   "Saved Char version higher than runtime, load failed, please upgrade",
   "Unable to load file",
   "Unable to save file",
   "A CHARACTER WITH THIS NAME ALREADY EXISTS",
   "NOT ENOUGH FREE HANDS",
   "LastErrorType Placeholder"
};

miscErrorType miscError = NoError;




//*****************************************************************************
// NAME: clearCharItemState
//
// PURPOSE:  
//*****************************************************************************
void clearCharItemState()
{
   startItem = 0;   
   party.activeItem = 0;
   ItemsOnPage = 0;
   charItemListText.Clear();
}


//*****************************************************************************
// NAME: clearTreasureItemState
//
// PURPOSE:  
//*****************************************************************************
void clearTreasureItemState()
{
   treasureItemListText.Clear();
   startItem = 0;   
   party.activeItem = 0;
   ItemsOnPage = 0;
}


//*****************************************************************************
// NAME: clearShopItemState
//
// PURPOSE:  
//*****************************************************************************
void clearShopItemState()
{
   itemListText.Clear();
   ItemsOnPage = 0;
   startItem = 0;   
   party.activeItem = 0;
}


//*****************************************************************************
// NAME: clearSpellItemState
//
// PURPOSE:  
//*****************************************************************************
void clearSpellItemState()
{
   spellListText.Clear();
   ItemsOnPage = 0;
   startItem = 0;   
   party.activeItem = 0;
}


//*****************************************************************************
// NAME: setCharItemTextState
//
// PURPOSE:  
//*****************************************************************************
void setCharItemTextState(CHARACTER &dude)
{
   //if (charItemListText.GetCount() <= 0)
     clearCharItemState();

   UINT flags = QTY | RDY;
   if (party.PartyIsDetectingMagic())
     flags |= DETECTING_MAGIC;

   charItemListText.FillItemListText(dude.myItems, flags);
   dude.addCharMoneyToItemList(charItemListText);
   addSpecialItemsToItemList(charItemListText);
   charItemListText.FormatItemListTitle("YOU HAVE NO ITEMS");

   int cnt = charItemListText.GetCount();
   ItemsOnPage = min(Items_Per_Page,cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= charItemListText.GetCount())
        party.activeItem = (BYTE)(charItemListText.GetCount()-1);

      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);

      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
     clearCharItemState();
}


//*****************************************************************************
// NAME: setShopItemState
//
// PURPOSE:  
//*****************************************************************************
void setShopItemState(ITEM_LIST &list, costFactorType type)
{
   //if (itemListText.GetCount() <= 0)
     clearShopItemState();

   UINT flags = COST | QTY | MONEY;
   itemListText.FillItemListText(list, flags, type);
   itemListText.FormatItemListTitle("THIS SHOP HAS NO ITEMS");
   int cnt = itemListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= itemListText.GetCount())
        party.activeItem = (BYTE)(itemListText.GetCount()-1);

      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);

      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}


//*****************************************************************************
// NAME: setSpellItemState
//
// PURPOSE:  
//*****************************************************************************
void setSpellItemState(CHARACTER &cdata, spellBookType &list, DWORD flags, costFactorType type)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();

   if (list.SpellsKnown() > 0)
     list.list.SortSpells();
   spellListText.FillSpellListText(cdata, list, flags, type);
   spellListText.FormatSpellListTitle("NO SPELLS AVAILABLE");
   int cnt = spellListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);

      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);

      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}

void setSpellNewLevelItemState(CHARACTER &data)
{
   clearSpellItemState();

   if (data.spells.SpellsKnown() > 0)
     data.spells.list.SortSpells();
   
   spellListText.FillNewSpellLevelListText(data);

   int cnt = spellListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}

void setSpellMemorizeItemState(CHARACTER &data)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();

   if (data.spells.SpellsKnown() > 0)
     data.spells.list.SortSpells();
   spellListText.FillMemorizeSpellListText(data);
   int cnt = spellListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}

void setSpellTempleCastItemState(CHARACTER &data, costFactorType type)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();

   if (data.spells.SpellsKnown() > 0)
     data.spells.list.SortSpells();
   spellListText.FillTempleCastSpellListText(data, type);
   int cnt = spellListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
   }
}

void setSpellCastItemState(CHARACTER &data)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();

   if (data.spells.SpellsKnown() > 0)
     data.spells.list.SortSpells();
   spellListText.FillCastSpellListText(data);
   int cnt = spellListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
   }
}

void setSpellEffectItemState(CHARACTER &data)
{
   //if (spellListText.GetCount() <= 0)
     clearSpellItemState();

   if (data.spells.SpellsKnown() > 0)
     data.spells.list.SortSpells();
   spellListText.FillSpellEffectsListText(data);
   int cnt = spellListText.GetCount();
   ItemsOnPage = min(Items_Per_Page, cnt);

   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= spellListText.GetCount())
        party.activeItem = (BYTE)(spellListText.GetCount()-1);
      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);
      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
   }
}

//*****************************************************************************
// NAME: setTreasureEventState
//
// PURPOSE:  
//*****************************************************************************
void setTreasureEventState(GameEvent *event)
{
   //if (treasureItemListText.GetCount() <= 0)
   clearTreasureItemState();

   getTreasureEventItems(event);
   int cnt = treasureItemListText.GetCount();
   ItemsOnPage = min(Items_Per_Page,cnt);
  
   if (ItemsOnPage > 0)
   {
      if (party.activeItem >= treasureItemListText.GetCount())
        party.activeItem = (BYTE)(treasureItemListText.GetCount()-1);

      if (party.activeItem >= (startItem+ItemsOnPage))
        party.activeItem = (BYTE)((startItem+ItemsOnPage)-1);

      if (party.activeItem < 0)
        party.activeItem = 0;
   }
   else
   {
     party.activeItem = 0;
     startItem = 0;
     //currShopItem = 0;
   }
}


//*****************************************************************************
// NAME: nextTreasurePage
//
// PURPOSE:  
//*****************************************************************************
void nextTreasurePage()
{
  if (startItem+Items_Per_Page < treasureItemListText.GetCount())
    startItem += Items_Per_Page;  
  int cnt = (treasureItemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: prevTreasurePage
//
// PURPOSE:  
//*****************************************************************************
void prevTreasurePage()
{
  if (startItem-Items_Per_Page >= 0)
    startItem -= Items_Per_Page;
  else
    startItem = 0;
  int cnt = (treasureItemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: allowTreasureItemNextPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowTreasureItemNextPage()
{
  // only one page of data
  if (treasureItemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // end of list
  if (startItem > (treasureItemListText.GetCount()-Items_Per_Page)) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: allowTreasureItemPrevPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowTreasureItemPrevPage()
{
  // only one page of data
  if (treasureItemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // start of list
  if (startItem < Items_Per_Page) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: nextCharItemsPage
//
// PURPOSE:  
//*****************************************************************************
void nextCharItemsPage()
{
  if (startItem+Items_Per_Page < charItemListText.GetCount())
    startItem += Items_Per_Page;
  int cnt = (charItemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: prevCharItemsPage
//
// PURPOSE:  
//*****************************************************************************
void prevCharItemsPage()
{
  if (startItem-Items_Per_Page >= 0)
    startItem -= Items_Per_Page;
  else
    startItem = 0;
  int cnt = (charItemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);
  
  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: allowCharItemNextPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowCharItemNextPage()
{
  // only one page of data
  if (charItemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // end of list
  if (startItem > (charItemListText.GetCount()-Items_Per_Page)) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: allowCharItemPrevPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowCharItemPrevPage()
{
  // only one page of data
  if (charItemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // start of list
  if (startItem < Items_Per_Page) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: nextShopPage
//
// PURPOSE:  
//*****************************************************************************
void nextShopPage()
{
  if (startItem+Items_Per_Page < itemListText.GetCount())    
    startItem += Items_Per_Page;
  int cnt = (itemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: prevShopPage
//
// PURPOSE:  
//*****************************************************************************
void prevShopPage()
{
  if (startItem-Items_Per_Page >= 0)
    startItem -= Items_Per_Page;
  else
    startItem = 0;
  int cnt = (itemListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}

//*****************************************************************************
// NAME: allowShopItemNextPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowShopItemNextPage()
{
  // only one page of data
  if (itemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // end of list
  if (startItem > (itemListText.GetCount()-Items_Per_Page)) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: allowShopItemPrevPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowShopItemPrevPage()
{
  // only one page of data
  if (itemListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // start of list
  if (startItem < Items_Per_Page) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: nextSpellPage
//
// PURPOSE:  
//*****************************************************************************
void nextSpellPage()
{
  if (startItem+Items_Per_Page < spellListText.GetCount())    
    startItem += Items_Per_Page;
  int cnt = (spellListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: prevSpellPage
//
// PURPOSE:  
//*****************************************************************************
void prevSpellPage()
{
  if (startItem-Items_Per_Page >= 0)
    startItem -= Items_Per_Page;
  else
    startItem = 0;
  int cnt = (spellListText.GetCount()-startItem);
  ItemsOnPage = min(cnt, Items_Per_Page);

  if (party.activeItem >= ItemsOnPage)
    party.activeItem = ItemsOnPage-1;
}


//*****************************************************************************
// NAME: allowSpellNextPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowSpellNextPage()
{
  // only one page of data
  if (spellListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // end of list
  if (startItem > (spellListText.GetCount()-Items_Per_Page)) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: allowSpellPrevPage
//
// PURPOSE:  
//*****************************************************************************
BOOL allowSpellPrevPage()
{
  // only one page of data
  if (spellListText.GetCount() <= Items_Per_Page)
    return FALSE;

  // start of list
  if (startItem < Items_Per_Page) 
    return FALSE;

  return TRUE;
}


//*****************************************************************************
// NAME: currSpellTextIndex
//
// PURPOSE:  
//*****************************************************************************
int currSpellTextIndex()
{
  return startItem+party.activeItem;
}


//*****************************************************************************
// NAME: currShopTextIndex
//
// PURPOSE:  
//*****************************************************************************
int currShopTextIndex()
{
  return startItem+party.activeItem;
}


//*****************************************************************************
// NAME: currCharItemsTextIndex
//
// PURPOSE:  
//*****************************************************************************
int currCharItemsTextIndex()
{
  return startItem+party.activeItem;
}


//*****************************************************************************
// NAME: currTreasureTextIndex
//
// PURPOSE:  
//*****************************************************************************
int currTreasureTextIndex()
{
  return startItem+party.activeItem;
}


BOOL shouldHighlight(int index)
{
  return ((index-startItem)== party.activeItem);
}

//*****************************************************************************
// NAME: displayPoolGoldTotal
//
// PURPOSE:  
//*****************************************************************************
void displayPoolGoldTotal()
{
  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  GraphicsMgr.EnableFontColorTags(FALSE);

  CString temp;
  int qty;

  if (party.moneyPooled)
    qty = party.GetPoolGoldValue();
  else
    qty = party.getActiveChar().poolCharacterGold();

  temp.Format("AVAILABLE %s: %u", globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()), qty);
  displayText(textboxX+(TEXTBOX_WIDTH/3), textboxY+20, temp);

  GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
}

//*****************************************************************************
// NAME: displayCharacterHP
//
// PURPOSE: Displays character name and hitpoints in message box window
//          during temple spell casting.
//*****************************************************************************
void displayCharacterHP(CHARACTER &dude)
{
  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  GraphicsMgr.EnableFontColorTags(FALSE);

  COLORREF color;
  CString temp;
  int hp = dude.GetAdjHitPoints();
  temp.Format("%s     %i/%i", dude.GetName(), hp, dude.GetMaxHitPoints());
  
  if (hp <= 0)
    color = Red;
  else if (hp < dude.GetMaxHitPoints())
     color=Yellow;
  else if (dude.GetAdjReadyToTrain())
     color = Blue;
  else
     color=Green;
  displayText(textboxX+(TEXTBOX_WIDTH/3), textboxY+20, temp, color);

  GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
}

// figure out if x,y is within one of the displayed party member RECTs
int IntersectPointWithInventory(long x, long y)
{
// If you ain't selecting, don't call!
  POINT pt = { x, y };

  for (int i=0; i < Items_Per_Page; i++)
  {
    if (PtInRect(&InventoryRects[i], pt))
      return i;
  }

  return -1;
}

// figure out if x,y is within one of the displayed party member RECTs
int IntersectPointWithParty(long x, long y)
{
  // Don't call me if you ain't selecting party members!
  POINT pt = { x, y };

  for (int i=0; i < party.numCharacters; i++)
  {
    if (PtInRect(&PartyNameRects[i], pt))
      return i;
  }

  return NO_DUDE;
}

//*****************************************************************************
// NAME: displayPartyNames
//
// PURPOSE:  
//*****************************************************************************
void displayPartyNames(PARTY &pty, bool useWX, bool showNPC)
{  
  memset(PartyNameRects, 0, sizeof(PartyNameRects));

   // a couple of cases when party data isn't drawn
   if (   (globalData.levelInfo.stats[globalData.currLevel].overland)
       && (globalData.useAreaView))
     return;

   if (currPic.picType == BigPicDib)
     return;

  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  GraphicsMgr.EnableFontColorTags(FALSE);
  int LineHeight = GraphicsMgr.GetMaxCharacterHeight()+2;

   int i,x;
   int y;                //DLD 12/20/00 //int y=18;
   int wx=PartyNames_wx; //const int wx=18;
   int nx=PartyNames_nx; //const int nx=275;
   int wy=PartyNames_wy; //
   int ny=PartyNames_ny; //end change/add
   
   COLORREF color;
   COLORREF namecolor;
   CString temp;   

   x = useWX ? wx : nx;
   y = useWX ? wy : ny; //DLD 12/20/00
   
   displayText(x, y, "NAME", White);
   displayText(x + 225, y, " AC", White); //DLD 12/20/00 //displayText(500, y, " AC", White);
   displayText(x + 300, y, " HP", White); //displayText(575, y, " HP", White);
   
   for (i=0; i<pty.numCharacters;i++)
   {
      if ((!showNPC) && (pty.characters[i].GetType() == NPC_TYPE))
        continue;

      int hp = pty.characters[i].GetAdjHitPoints();
      if (hp <= 0)
        color = Red;
      else if (hp < pty.characters[i].GetMaxHitPoints())
         color=Yellow;
      else
         color=Green;

      if (pty.characters[i].GetAdjReadyToTrain())
        namecolor=Blue;
      else
        namecolor=Green;

      y+=LineHeight;//20;
      
      if (pty.activeCharacter == i)
         displayText(x,y, pty.characters[i].GetName(), namecolor, TRUE);
      else
         displayText(x,y, pty.characters[i].GetName(), namecolor);
      
      temp.Format("%3i", pty.characters[i].GetAdjAC());
      displayText(x + 225,y, temp, namecolor); //DLD 12/20/00 //displayText(500,y, temp, color);
      
      temp.Format("%3i", hp);
      displayText(x + 300,y, temp, color); //DLD 12/20/00 //displayText(575,y, temp, color);

      PartyNameRects[i].left = x;
      PartyNameRects[i].top = y;
      PartyNameRects[i].right = x + 300 + GraphicsMgr.GetTextWidth(temp, temp.GetLength()); //DLD 12/20/00 //PartyNameRects[i].right = 575 + GraphicsMgr.GetTextWidth(temp, strlen(temp));
      PartyNameRects[i].bottom = y + GraphicsMgr.GetMaxCharacterHeight();
   }
     
   if (x == nx)
   {
      temp.Format("%i,%i", pty.Posx, pty.Posy);
      displayText(LocationText_x, LocationText_y, temp, White); //DLD 12/20/00 //displayText(275, 280, temp, White);
      
      if (!globalData.levelInfo.stats[globalData.currLevel].overland)
      {
        switch (pty.facing) 
        {
        case FACE_NORTH:
           temp = "North";
           break;
        case FACE_EAST:
           temp = "East";
           break;
        case FACE_SOUTH:
           temp = "South";
           break;
        case FACE_WEST:
           temp = "West";
           break;
        }      
        displayText(LocationText_x + 25 + 40, LocationText_y, temp, White); //DLD 12/20/00 //displayText(340, 280, temp, White);
      }
      
      if (pty.searching)
         displayText(LocationText_x + 25 + 85, LocationText_y, "Searching", White); //DLD 12/20/00 //displayText(385, 280, "Searching", White);
      
      temp.Format("Day %i %02i:%02i", pty.days, pty.hours, pty.minutes);
            displayText(LocationText_x + 125 + 70, LocationText_y, temp, White); //DLD 12/20/00 displayText(470, 280, temp, White);
   }
   GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
}

//*****************************************************************************
// NAME: displayCombatStats
//
// PURPOSE:  
//*****************************************************************************
void displayCombatStats(int dude)
{ 
   if ((dude < 0) || (dude >= combatData.numCombatants))
      return;

   BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
   GraphicsMgr.EnableFontColorTags(FALSE);
   int LineHeight = GraphicsMgr.GetMaxCharacterHeight()+2;

   int x = COMBAT_SCREEN_WIDTH+(VertBarSrc.right-VertBarSrc.left)+2;
   int y = 18;
   CString wp, dmg, temp;
   COLORREF color, color2; 
   const COLORREF AdjColor = RGB(255,128,0); // Bright Orange, for stats currently modified by active spells   
   
   if (combatData.combatants[dude].GetHitPoints() <= 0)
     color2 = CombatRed;
   else if (combatData.combatants[dude].GetIsFriendly())
      color2 = CombatBlue;
   else
      color2 = CombatYellow;

   int hp = combatData.combatants[dude].GetAdjHitPoints();
   if (hp <= 0)
      color = CombatRed;
   else if (hp < combatData.combatants[dude].GetMaxHitPoints())
      color = CombatYellow;
   else
      color = CombatGreen;
   
   // display name
#ifdef _DEBUG
   CString DisplayName;   
   DisplayName.Format("%s (%i:%i,%i)",
             combatData.combatants[dude].GetName(), 
             dude,
             combatData.combatants[dude].x,
             combatData.combatants[dude].y);
   displayText(x,y, DisplayName, color2);
#else
   displayText(x,y, combatData.combatants[dude].GetName(), color2);
#endif
   
   // display current hit points
   y += LineHeight;
   displayText(x,y, "HITPOINTS ", CombatYellow);
   temp.Format("%3i", hp);
   int hpx = x + GraphicsMgr.GetTextWidth("HITPOINTS ", strlen("HITPOINTS "));
   if (hp!=combatData.combatants[dude].GetHitPoints())
     displayText(hpx,y, temp, AdjColor);
   else
     displayText(hpx,y, temp, color);
   
   // display current armor class
   y += LineHeight;//18;
   displayText(x,y, "AC ", CombatYellow);
   int currAC = combatData.combatants[dude].GetAdjAC();
   temp.Format("%3i", currAC);
   if (currAC != combatData.combatants[dude].GetAC())
     displayText(x+25, y, temp, AdjColor);
   else
     displayText(x+25, y, temp, CombatYellow);
   y += LineHeight;//18;

   // display current readied weapon
   combatData.getCharWeaponText(dude, wp, dmg);
   if ((!wp.IsEmpty()) && (wp.CompareNoCase("NONE")!=0))
   {
      displayText(x,y, wp, CombatYellow);
      y += LineHeight;//18;
   }

#ifdef _DEBUG
  temp.Format("Attacks: %f", combatData.combatants[dude].AttacksRemaining());
  displayText(x,y, temp, White);
  y += LineHeight;//18;
  temp.Format("Cursor: %u, %u", combatData.cursorX, combatData.cursorY);
  displayText(x,y, temp, White);
  y += LineHeight;//18;

#endif

  /*
  POSITION pos = combatMsgs.msgs.GetHeadPosition();
  while (pos != NULL)
  {
    displayText(x,y, combatMsgs.msgs.GetNext(pos), White);
    y += 18;
  }
  */
  y+=(LineHeight*2);
  for (int i=0;i<combatTextData.numLines;i++)
  {
    displayText(x,y, combatTextData.text[i], White);
    y += LineHeight;//18;
  }
  GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
} 

#ifdef _DEBUG
// debug only, shows details about terrain under cursor
// while aiming manually
void displayCursorStats()
{
   BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
   GraphicsMgr.EnableFontColorTags(FALSE);

   const int x = COMBAT_SCREEN_WIDTH+18;
   int y = 150; // closer to the bottom
   
   TILE_DATA data;
   int cellNum = GetCellNum(combatData.cursorX,combatData.cursorY);
   data = GetTileData(combatData.cursorX,combatData.cursorY);

   CString tmp;

   tmp.Format("Cursor X,Y: %i,%i", combatData.cursorX,combatData.cursorY);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("Cell: %i", cellNum);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("enab: %i", data.enabled);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("X,Y: %i,%i", data.posx, data.posy);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("visi: %i", data.visibility);
   displayText(x,y, tmp, White);   
   y += 18;

   tmp.Format("obst: %i", data.obstacle);
   displayText(x,y, tmp, White);
   
   GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
} 
#endif