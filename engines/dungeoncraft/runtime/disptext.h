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

#ifndef DUNGEONCRAFT_SHARED_DISPTEXT
#define DUNGEONCRAFT_SHARED_DISPTEXT

#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/TextForm.h"

namespace DungeonCraft {

// variables used to maintain the state of various item viewers
//const int ITEM_SEP = 200;
const int ITEMY = 60;
//const int ITEMX = 75;
const int Items_Per_Page = 14; // leave room at the bottom for error messages

extern int ItemsOnPage;
extern int startItem;
extern CString CombatMsg;
extern CString timeDelayMsg;
extern RECT InventoryRects[Items_Per_Page];

void setCharItemTextState(CHARACTER &dude);
void setShopItemState(ITEM_LIST &list, costFactorType type=Normal);
void setSpellEffectItemState(CHARACTER &data);
void setSpellMemorizeItemState(CHARACTER &data);
void setSpellNewLevelItemState(CHARACTER &data);
void setSpellTempleCastItemState(CHARACTER &data, costFactorType type);
void setSpellCastItemState(CHARACTER &data);
void setSpellItemState(CHARACTER &cdata, spellBookType &list, DWORD flags, costFactorType type=Normal);
void setTreasureEventState(GameEvent *event);

void clearCharItemState();
void clearTreasureItemState();
void clearShopItemState();
void clearSpellItemState();
void nextTreasurePage();
void nextCharItemsPage();
void nextShopPage();
void nextSpellPage();
void prevTreasurePage();
void prevCharItemsPage();
void prevShopPage();
void prevSpellPage();
int currSpellTextIndex();
int currShopTextIndex();
int currCharItemsTextIndex();
int currTreasureTextIndex();
BOOL allowTreasureItemNextPage();
BOOL allowTreasureItemPrevPage();
BOOL allowCharItemNextPage();
BOOL allowCharItemPrevPage();
BOOL allowShopItemNextPage();
BOOL allowShopItemPrevPage();
BOOL allowSpellNextPage();
BOOL allowSpellPrevPage();
BOOL shouldHighlight(int index);

//void displayShop(void);
void resetShopState(SHOP &shop);
void displayCombatStats(int dude);
//void displayTreasure();
//void displaySpells();
void displayPartyNames(PARTY &pty, bool useWX, bool showNPC);
//void displayCharacterItems(CHARACTER &dude);
void displayCharacterHP(CHARACTER &dude);
void displayPoolGoldTotal();


int IntersectPointWithParty(long x, long y);
int IntersectPointWithInventory(long x, long y);

#ifdef _DEBUG
void displayCursorStats(); // combat debug data
#endif

} // End of namespace DungeonCraft

#endif
