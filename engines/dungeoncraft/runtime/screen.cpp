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


#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/SoundMgr.h"
#include "dungeoncraft/shared/Graphics.h"
#include "dungeoncraft/shared/Viewport.h"
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/Disptext.h"
#include "dungeoncraft/shared/CharStatsForm.h"
#include "dungeoncraft/shared/GameMenu.h"
#include "dungeoncraft/shared/path.h"
#include "dungeoncraft/shared/DrawTile.h"
#include "dungeoncraft/shared/party.h"
#include "dungeoncraft/shared/FormattedText.h"
#include "dungeoncraft/shared/..\Shared\GPDLopCodes.h"
#include "dungeoncraft/shared/GPDLexec.h"
#include "dungeoncraft/shared/combatants.h"
#include "dungeoncraft/shared/MouseThread.h"

#include "dungeoncraft/shared/Screen.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int IconPickX = 100;
const int IconPickY = 100;

// Decides what to draw in viewport
// If currPic has something to draw it will be done
// draw viewport frame
// draw viewport contents if needed (no pic? then walls or map)
// draw currPic data
void blitView(DWORD flags)
{
  //if ((globalData.BackgroundSurf >= 0) && (currPic.key > 0))
  //{
  //  // so that pic area is clear of background image
  //  RECT rect = { 0, 0, SCREEN_WIDTH, HorzBarMidY };
  //  GraphicsMgr.ClearSurface(BACKBUFFERNUM, BACKGROUND_FILL, &rect, FALSE);
  //}
  
  drawBorder();
  
  if ((currPic.key > 0) && (currPic.picType == BigPicDib) && (flags & BigPicDib))
    drawViewport(FALSE, FALSE);
  else
    drawViewport();
  
  if (currPic.key > 0)
  {
    if ((currPic.picType == BigPicDib) && (flags & BigPicDib))
    {
      GraphicsMgr.BlitSprite(currPic.sx, currPic.sy, currPic.key);
    }
    else if ((currPic.picType == SpriteDib) && (flags & SpriteDib))
    {
      updateViewport(party.Posx, party.Posy, party.facing);
      GraphicsMgr.BlitSprite(currPic.sx, currPic.sy, currPic.key);
    }
    else if ((currPic.picType == SmallPicDib) && (flags & SmallPicDib))
    {
      updateViewport(party.Posx, party.Posy, party.facing);
      GraphicsMgr.BlitSprite(currPic.sx, currPic.sy, currPic.key);
    }
    else
      updateViewport(party.Posx, party.Posy, party.facing);
  }
  else
    updateViewport(party.Posx, party.Posy, party.facing);
}

void DisplayTextAndFlip(void)
{  
  TEXT_DISPLAY_DATA *pText;

  if (errorTextData.numLines > 0)
    pText = &errorTextData;
  else
    pText = &textData;

  if (pText->NeedsFrontBuffer())
  {
    GraphicsMgr.FlipSurfaces();
    DisplayFormattedText(*pText);
  }
  else
  {
    DisplayFormattedText(*pText);
    GraphicsMgr.FlipSurfaces();
  }
}

void UpdateBigSmallSprite(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);  
  blitView(SmallPicDib | BigPicDib | SpriteDib);
  menu.DisplayMenu();
  displayPartyNames(party, false, true);
  DisplayTextAndFlip();
}

void UpdateSmall(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  blitView(SmallPicDib);
  displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateBig(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  blitView(BigPicDib);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateSmallSprite(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  blitView(SmallPicDib | SpriteDib);
  displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void DisplayCombatData(BOOL ShowCursor=TRUE, BOOL FullIcon=TRUE)
{
  GraphicsMgr.ClearSurface(BACKBUFFERNUM, BACKGROUND_COMBAT_FILL); 
  displayCombatWalls();    
  DisplayAnimateList();
  combatData.DisplayCombatants();
  if (ShowCursor)
    combatData.displayCursor(FullIcon);
  displayCombatBorders();
}

void DisplayCombatDataWithSprite(BOOL ShowCursor=TRUE, BOOL FullIcon=TRUE)
{
  GraphicsMgr.ClearSurface(BACKBUFFERNUM, BACKGROUND_COMBAT_FILL); 
  displayCombatWalls();    
  DisplayAnimateList();
  combatData.DisplayCombatants();
  if (ShowCursor) 
    combatData.displayCursor(FullIcon);
  combatSprite.Display();
  displayCombatBorders();
}

///////////////////////////////////////
// Load and Display Credits Screen
///////////////////////////////////////
BOOL Credits()
{  
  WriteDebugString("Loading credits screen and music\n");
  return GraphicsMgr.FadeInImage(globalData.CreditsBgArt, TitleDib);
}

void UpdateStartScreen(void)
{
  int offsetX, offsetY;
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displayPartyNames(party, true, true);
  menu.DisplayMenu();

  // format version as text
  CString DesignVersionInfo;
  DesignVersionInfo.Format("%s  ver %01.3f", 
                           globalData.designName,
                           ((float)globalData.version));

  // determine coords for top/center of screen
  if ((dsgnVerX>=0)&&(dsgnVerY>=0))
  {
    offsetX=dsgnVerX;
    offsetY=dsgnVerY;
  }
  else
  {
    offsetX = (SCREEN_WIDTH/2) - 
            (GraphicsMgr.GetTextWidth(DesignVersionInfo, DesignVersionInfo.GetLength())/2) - 1;
    offsetY = -1; //DLD 12/20/00 change due to new art layout //offsetY = 1;
  }
  // blit to primary surface
  displayText(offsetX, offsetY, DesignVersionInfo, White, FALSE, FALSE, TRUE);
  DisplayTextAndFlip();
}

void UpdateIconPickScreen(void)
{
  int offsetX=0, offsetY=0;
  int width, height, bits, temp;
  //RECT rect;

  if (GraphicsMgr.GetSpriteSurfaceStats(character.icon.key, width, height, bits))
  {  
    temp = width / COMBAT_TILE_WIDTH;
    temp /= (character.icon.NumFrames/2);
    if (temp > 4)
      offsetX = 0;
    else if (temp == 4)
      offsetX = COMBAT_TILE_WIDTH;
    else if (temp == 2)
      offsetX = 2*COMBAT_TILE_WIDTH;
    else 
    {
      offsetX = 0;
      WriteDebugString("Invalid icon width for GS_IconPick in UpdateIconPickScreen\n");
    };
    temp = height / COMBAT_TILE_HEIGHT;
    if (temp == 3)
       offsetY = 0;
    else if (temp == 2)
       offsetY = COMBAT_TILE_HEIGHT;
    else if (temp == 1)
       offsetY = 2*COMBAT_TILE_HEIGHT;
    else 
    {
       WriteDebugString("Invalid icon height for GS_IconPick in UpdateIconPickScreen\n");
       offsetY = 0;
    }
  }

  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);

  drawBorder();
  if (SpriteBkGndArt > 0)
     GraphicsMgr.BlitImage(IconPickX, IconPickY, SpriteBkGndArt, NULL);

  character.icon.SetFrame(0);
  GraphicsMgr.BlitSprite( IconPickX+offsetX, 
                          IconPickY+offsetY, 
                          character.icon.key);  
  character.icon.SetFrame(1);
  offsetX +=  (COMBAT_TILE_WIDTH + (width/2));
  GraphicsMgr.BlitSprite( IconPickX+offsetX, 
                          IconPickY+offsetY, 
                          character.icon.key);  
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateSmallPicScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  if (character.smallPic.key > 0)
     GraphicsMgr.BlitSprite(IconPickX, IconPickY, character.smallPic.key);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateAdventureScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport();
  updateViewport(party.Posx, party.Posy, party.facing);
  displayPartyNames(party, false, true);         

  menu.DisplayMenu();
  DisplayTextAndFlip();
}         

void UpdateTextEventScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateQuestionYesNoScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateQuestionListScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateQuestionButtonScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateVaultScreen(void)
{
  UpdateSmallSprite();
}

void UpdateGiveTreasureScreen(void)
{
  UpdateSmallSprite();
}

void UpdateViewCharacterScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewportFrame(VPFrameXOffset, VPFrameY+VPFrameYOffset);
  if (currPic.key > 0)
  {
    if (currPic.picType == SmallPicDib)
       GraphicsMgr.BlitSprite(VPFrameXOffset+(ViewportX-VPFrameX), VPFrameY+VPFrameYOffset+(ViewportY-VPFrameY), currPic.key);
  }
  menu.DisplayMenu();
  if (errorTextData.numLines == 0)
    displayCharStats(party.getActiveChar());
  DisplayTextAndFlip();
}

void UpdateCombatViewCharacterScreen(void)
{  
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewportFrame(VPFrameXOffset, VPFrameY+VPFrameYOffset);
  if (currPic.key > 0)
  {
    if (currPic.picType == SmallPicDib)
       GraphicsMgr.BlitSprite(VPFrameXOffset+(ViewportX-VPFrameX), VPFrameY+VPFrameYOffset+(ViewportY-VPFrameY), currPic.key);
  }
  menu.DisplayMenu();
  if (errorTextData.numLines == 0)
    displayCharStats(combatData.getActiveCombatCharacter());
  DisplayTextAndFlip();
}

void UpdateEncampScreen(void)
{
  UpdateSmallSprite();
}

void UpdateTransferMenuScreen(void)
{
  UpdateBigSmallSprite();
}
    
void UpdateTransferMsgScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport();
  updateViewport(party.Posx, party.Posy, party.facing);
  displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateTavernMenuScreen(void)
{
  UpdateSmallSprite();
}

void UpdateGiveDamageScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateGainExpScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateHealPartyScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateWhoPaysScreen(void)
{
  UpdateSmall();
}

void UpdatePassTimeScreen(void)
{
  UpdateSmall();
}

void UpdateSmallTownScreen(void)
{
  UpdateSmall();
}
void UpdateRandomEventScreen(void)
{
  UpdateSmall();
}
void UpdateAddNPCScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateRemoveNPCScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateNPCSaysScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateShopScreen(void)
{
  UpdateSmallSprite();
}

void UpdateSpecialItemKeyScreen(void)
{
  UpdateSmall();
}

void UpdateGPDLScreen(void)
{
  GPDL_STATE dscState;
  dscState=gpdl.State();
  if (dscState!=GPDL_UNINITIALIZED) 
  {
    GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
    blitView(SmallPicDib | BigPicDib | SpriteDib);
    displayPartyNames(party, false, true);
    menu.DisplayMenu();
       
    if (errorTextData.numLines == 0)
    {
    // DisplayFormattedText(textData, White);
      int x = textboxX;
      int y = textboxY;
      //discourse.Display();
      GraphicsMgr.ResetFontColorTags();
      GraphicsMgr.EnableFontColorTags(TRUE);
      for (int i=0; i<discourseLine.GetSize(); i++)
      {
        GraphicsMgr.ResetFontColorTags();
        displayText(x,y+18*i, discourseLine[i], White, FALSE, FALSE, TRUE);
      };
      GraphicsMgr.FlipSurfaces();
      GraphicsMgr.EnableFontColorTags(FALSE);
    }
    else
      DisplayTextAndFlip();    
  }; // if (!GPDL_UNINITIALIZED)
}

void UpdateTakePartyItemsScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateTrainHallWelcomeScreen(void)
{
  UpdateSmall();
}

void UpdateWhoTriesScreen(void)
{
  UpdateSmall();
}

void UpdateTempleScreen(void)
{
  UpdateSmallSprite();
}

void UpdateTempleGiveScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  blitView(SmallPicDib | SpriteDib);
  displayPartyNames(party,false, true);
  menu.DisplayMenu();
  if (errorTextData.numLines == 0) displayPoolGoldTotal();
  DisplayTextAndFlip();
}

void UpdatePasswordScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateEncounterScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateQuestScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateGuidedTourScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateGuidedTourMoveScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport();
  updateViewport(party.Posx, party.Posy, party.facing);
  displayPartyNames(party, false, true);
  DisplayTextAndFlip();
}

void UpdateSpeedCombatScreen(void)
{
  DisplayCombatData();
  menu.DisplayMenu();
  displayCombatStats(getCombatantInCell(combatData.cursorX, combatData.cursorY, 1, 1));
  DisplayTextAndFlip();  
}

void UpdateSpeedScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateCombatRoundDelayScreen(void)
{
  DisplayCombatData();
  DisplayTextAndFlip();
}

void UpdateCombatTimeDelayMsg(int dude)
{
  DisplayCombatDataWithSprite();
  menu.DisplayMenu();
  if (dude < 0) dude = combatData.GetCurrCombatant();
  displayCombatStats(dude);
  DisplayTextAndFlip();
}

void UpdateSpriteAnimateScreen(int dude)
{
  DisplayCombatDataWithSprite(FALSE); // no cursor
  menu.DisplayMenu();
  displayCombatStats(dude);
  DisplayTextAndFlip();
}

void UpdateCombatMoveScreen(void)
{
  DisplayCombatData(FALSE); // no cursor
  menu.DisplayMenu();
  displayCombatStats(combatData.GetCurrCombatant());
  DisplayTextAndFlip();
}

void UpdateCombatAimManualScreen(void)
{
  DisplayCombatData(TRUE,FALSE);
  menu.DisplayMenu();
  
  int dude = getCombatantInCell(combatData.cursorX, combatData.cursorY, 1,1);
  if (dude == NO_DUDE)
    dude = getDeadCombatantInCell(combatData.cursorX, combatData.cursorY, 1,1);

  displayCombatStats(dude);
  
#ifdef _DEBUG
  displayCursorStats();
#endif
  DisplayTextAndFlip();
}

void UpdateCombatAimScreen(void)
{
  DisplayCombatData();
  menu.DisplayMenu();
  displayCombatStats(getCombatantInCell(combatData.cursorX, combatData.cursorY, 1, 1));
  DisplayTextAndFlip();
}

void UpdateCombatMainMenuScreen(void)
{
  DisplayCombatData();
  menu.DisplayMenu();
  displayCombatStats(getCombatantInCell(combatData.cursorX, combatData.cursorY, 1, 1));  
  DisplayTextAndFlip();
}

void UpdateHumanChangeClassScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateTradeTakerSelectScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateCombatTradeTakerSelectScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport();

  if (currPic.key > 0)
  {
    if (currPic.picType == SmallPicDib)
       GraphicsMgr.BlitSprite(ViewportX, ViewportY, currPic.key);
  }
  displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateAlterGameScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdateAlterOrderScreen(void)
{
  UpdateBigSmallSprite();
}

void UpdatePartyDiedScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  // don't have a picture for this yet
  //blitView(SmallPicDib | BigPicDib | SpriteDib);
  //if (currPic.picType != BigPicDib)
  //   displayPartyNames(party, false, true);
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}

void UpdateCombatResultsScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  //blitView(SmallPicDib | BigPicDib | SpriteDib);
  //if (currPic.picType != BigPicDib)
  //   displayPartyNames(party, false, true);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateCombatMagicMenuScreen(void)
{
  DisplayCombatData();
  menu.DisplayMenu();
  displayCombatStats(getCombatantInCell(combatData.cursorX, combatData.cursorY, 1, 1));  
  DisplayTextAndFlip();
}

void UpdateMagicMenuScreen(void)
{
  UpdateBigSmallSprite();
}
/*
void UpdateCombatDisplaySpellEffectsMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
/*
void UpdateDisplaySpellEffectsMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/

/*
void UpdateMemorizeVerifyMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
/*
void UpdateCombatCastMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
/*
void UpdateCastMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
/*
void UpdateMemorizeMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  // draw middle horz bar, but 
  // not viewport and middle ver bar
  drawViewport(FALSE, FALSE);
  displaySpells();
  menu.DisplayMenu();
  DisplayTextAndFlip();
}
*/
void UpdateRestMenuScreen(void)
{

}

void UpdateAppraiseSelectMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}

void UpdateAppraiseEvaluateMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}

void UpdateAskYesNoMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}
/*
// this screen is displayed before a design is loaded, which
// means that the usual art files are not loaded - frames,
// cursors, horz/vert bars, etc.
//
void UpdateDesignSelectMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  displaySpells(); // design list built using spell list code
  menu.DisplayMenu();  
  DisplayTextAndFlip();
}
*/
void UpdateTargetSelectNonCombatScreen(void)
{
  UpdateSmall();
}

void UpdateDisplayPartyJournalScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  menu.DisplayMenu();  

  if (journalTextData.NeedsFrontBuffer())
  {
    GraphicsMgr.FlipSurfaces();
    DisplayJournalText(journalTextData);
  }
  else
  {
    DisplayJournalText(journalTextData);
    GraphicsMgr.FlipSurfaces();
  }
}

void UpdateMovieBigPicWindow(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport(FALSE, FALSE);
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateMovieSmallPicWindow(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();
  drawViewport();
  displayPartyNames(party, false, true); // useWX=0, ShowNPC=1
  menu.DisplayMenu();
  DisplayTextAndFlip();
}

void UpdateRaceMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();

  int totalListItems = spellListText.GetCount();
  if (totalListItems == 0)
  {
    CString tmp;
    tmp.Format("NO RACES AVAILABLE FOR SELECTION");
    displayText(18, 18, tmp);
    menu.DisplayMenu(); 
    DisplayTextAndFlip();
    return;
  }

  memset(InventoryRects, 0, sizeof(InventoryRects));
  int ir_idx = 0;

  displayText(18, 18, spellListText.title);
  int currItem = startItem;
  int shopY = ITEMY;

  while (currItem < totalListItems)
  {
    raceType rt = (raceType)spellListText.GetSpellIndex(currItem);    
    displayText(18, shopY, raceData.GetRaceName(rt), White, shouldHighlight(currItem));

    InventoryRects[ir_idx].left = 18;
    InventoryRects[ir_idx].top = shopY;
    InventoryRects[ir_idx].right = 18 + GraphicsMgr.GetTextWidth(spellListText.GetSpellText(currItem), strlen(spellListText.GetSpellText(currItem)));
    InventoryRects[ir_idx].bottom = shopY + GraphicsMgr.GetMaxCharacterHeight();
    ir_idx++;

    if (totalListItems > Items_Per_Page)
    {
       if (! ((currItem+1) % Items_Per_Page) )
       {
          shopY=ITEMY;
          break;
       }
    }
    currItem++;
    shopY += 18;
  }

  menu.DisplayMenu();  
  DisplayTextAndFlip();
}

void UpdateGenderMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();

  int totalListItems = spellListText.GetCount();
  if (totalListItems == 0)
  {
    CString tmp;
    tmp.Format("NO GENDERS AVAILABLE FOR SELECTION");
    displayText(18, 18, tmp);
    menu.DisplayMenu(); 
    DisplayTextAndFlip();
    return;
  }

  memset(InventoryRects, 0, sizeof(InventoryRects));
  int ir_idx = 0;

  displayText(18, 18, spellListText.title);
  int currItem = startItem;
  int shopY = ITEMY;

  while (currItem < totalListItems)
  {
    genderType type = (genderType)spellListText.GetSpellIndex(currItem);    
    displayText(18, shopY, GetGenderName(type), White, shouldHighlight(currItem));

    InventoryRects[ir_idx].left = 18;
    InventoryRects[ir_idx].top = shopY;
    InventoryRects[ir_idx].right = 18 + GraphicsMgr.GetTextWidth(spellListText.GetSpellText(currItem), strlen(spellListText.GetSpellText(currItem)));
    InventoryRects[ir_idx].bottom = shopY + GraphicsMgr.GetMaxCharacterHeight();
    ir_idx++;

    if (totalListItems > Items_Per_Page)
    {
       if (! ((currItem+1) % Items_Per_Page) )
       {
          shopY=ITEMY;
          break;
       }
    }
    currItem++;
    shopY += 18;
  }

  menu.DisplayMenu();  
  DisplayTextAndFlip();
}
void UpdateClassMenuScreen(void)
{
  // some class names can trigger color codes: 'Cleric/Ranger'
  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  GraphicsMgr.EnableFontColorTags(FALSE);

  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();

  int totalListItems = spellListText.GetCount();
  if (totalListItems == 0)
  {
    CString tmp;
    tmp.Format("NO CLASSES AVAILABLE FOR SELECTION");
    displayText(18, 18, tmp);
    menu.DisplayMenu(); 
    DisplayTextAndFlip();
    return;
  }

  memset(InventoryRects, 0, sizeof(InventoryRects));
  int ir_idx = 0;

  displayText(18, 18, spellListText.title);
  int currItem = startItem;
  int shopY = ITEMY;

  while (currItem < totalListItems)
  {
    classType ct = (classType)spellListText.GetSpellIndex(currItem);        
    displayText(18, shopY, classData[ct].m_name , White, shouldHighlight(currItem));

    InventoryRects[ir_idx].left = 18;
    InventoryRects[ir_idx].top = shopY;
    InventoryRects[ir_idx].right = 18 + GraphicsMgr.GetTextWidth(spellListText.GetSpellText(currItem), strlen(spellListText.GetSpellText(currItem)));
    InventoryRects[ir_idx].bottom = shopY + GraphicsMgr.GetMaxCharacterHeight();
    ir_idx++;

    if (totalListItems > Items_Per_Page)
    {
       if (! ((currItem+1) % Items_Per_Page) )
       {
          shopY=ITEMY;
          break;
       }
    }
    currItem++;
    shopY += 18;
  }

  GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);

  menu.DisplayMenu();  
  DisplayTextAndFlip();  
}
void UpdateAlignmentMenuScreen(void)
{
  GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);
  drawBorder();

  int totalListItems = spellListText.GetCount();
  if (totalListItems == 0)
  {
    CString tmp;
    tmp.Format("NO ALIGNMENTS AVAILABLE FOR SELECTION");
    displayText(18, 18, tmp);
    menu.DisplayMenu(); 
    DisplayTextAndFlip();
    return;
  }

  memset(InventoryRects, 0, sizeof(InventoryRects));
  int ir_idx = 0;

  displayText(18, 18, spellListText.title);
  int currItem = startItem;
  int shopY = ITEMY;
  /*
  enum alignmentType { LawfulGood,    NeutralGood, ChaoticGood, 
                       LawfulNeutral, TrueNeutral, ChaoticNeutral,  
                       LawfulEvil,    NeutralEvil, ChaoticEvil };
  */

  while (currItem < totalListItems)
  {
    alignmentType type = (alignmentType)spellListText.GetSpellIndex(currItem);    
    displayText(18, shopY, GetAlignmentName(type), White, shouldHighlight(currItem));

    InventoryRects[ir_idx].left = 18;
    InventoryRects[ir_idx].top = shopY;
    InventoryRects[ir_idx].right = 18 + GraphicsMgr.GetTextWidth(spellListText.GetSpellText(currItem), strlen(spellListText.GetSpellText(currItem)));
    InventoryRects[ir_idx].bottom = shopY + GraphicsMgr.GetMaxCharacterHeight();
    ir_idx++;

    if (totalListItems > Items_Per_Page)
    {
       if (! ((currItem+1) % Items_Per_Page) )
       {
          shopY=ITEMY;
          break;
       }
    }
    currItem++;
    shopY += 18;
  }

  menu.DisplayMenu();  
  DisplayTextAndFlip();
}
