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

#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/level.h"
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/party.h"
//#include "dungeoncraft/shared/monster.h"
//#include "dungeoncraft/shared/combatants.h"
#include "dungeoncraft/shared/treas.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//*****************************************************************************
// NAME: getTreasureEventItems
//
// PURPOSE:  
//*****************************************************************************
void getTreasureEventItems(GameEvent *event)
{
	int i;
  UINT flags = QTY;
  if (party.PartyIsDetectingMagic())
    flags |= DETECTING_MAGIC;
  if (event == NULL)
    return;

  switch (event->GetEventType())
  {
  case ShopEvent:
  case TempleEvent:
    addPooledMoneyToTreasure(party);
    treasureItemListText.FormatItemListTitle("NOTHING POOLED");
    break;

  case CombatTreasure:
  case GiveTreasure:  
    for (i=0;i<((GIVE_TREASURE_DATA*)event)->moneyRemain.NumCoinTypes();i++)
    {
      itemClassType srcType = MONEY_DATA_TYPE::GetItemClass(i);
      treasureItemListText.Add(((GIVE_TREASURE_DATA*)event)->moneyRemain[i],
                               srcType, 
                               globalData.moneyData.GetName(srcType));
    }

    treasureItemListText.Add(((GIVE_TREASURE_DATA*)event)->moneyRemain.NumGems(),
                             GemType,
                             ((GIVE_TREASURE_DATA*)event)->moneyRemain.Name(GemType));
    treasureItemListText.Add(((GIVE_TREASURE_DATA*)event)->moneyRemain.NumJewelry(),
                             JewelryType,
                             ((GIVE_TREASURE_DATA*)event)->moneyRemain.Name(JewelryType));

    treasureItemListText.AddItemListText(((GIVE_TREASURE_DATA*)event)->itemsRemain,
                                         flags,
                                         Normal,
                                         0);

    if (party.moneyPooled)
      addPooledMoneyToTreasure(party);

    treasureItemListText.FormatItemListTitle("NO TREASURE");
    break;

  case Vault:
    {
      BYTE vaultIdx = ((VAULT_EVENT_DATA*)event)->WhichVault;

      treasureItemListText.FillItemListText(globalData.vault[vaultIdx].items, flags);

      for (i=0;i<globalData.vault[vaultIdx].money.NumCoinTypes();i++)
      {
        itemClassType srcType = MONEY_DATA_TYPE::GetItemClass(i);
        treasureItemListText.Add(globalData.vault[vaultIdx].money[i], srcType, globalData.moneyData.GetName(srcType));
      }

      treasureItemListText.Add(globalData.vault[vaultIdx].money.NumGems(),GemType,globalData.vault[vaultIdx].money.Name(GemType));
      treasureItemListText.Add(globalData.vault[vaultIdx].money.NumJewelry(),JewelryType,globalData.vault[vaultIdx].money.Name(JewelryType));
      treasureItemListText.FormatItemListTitle("NO ITEMS IN VAULT");
    }
    break;

  default:    
    WriteDebugString("Unhandled event type %u in getTreasureEventItems()\n", 
                     event->GetEventType());
    ASSERT(FALSE);
    break;
  }
}


//*****************************************************************************
// NAME: addPooledMoneyToTreasure
//
// PURPOSE:  
//*****************************************************************************
void addPooledMoneyToTreasure(PARTY& pty)
{
  if (!pty.moneyPooled)
    return;

  treasureItemListText.UseQty = TRUE;

  for (int i=0;i<pty.poolSack.NumCoinTypes();i++)
  {
    if (pty.poolSack.IsActive(globalData.moneyData.GetItemClass(i)))
    {
      itemClassType srcType = MONEY_DATA_TYPE::GetItemClass(i);
      treasureItemListText.Add(pty.poolSack[i],srcType,pty.poolSack.Name(srcType));
    }
  }
  treasureItemListText.Add(pty.poolSack.NumGems(),GemType,pty.poolSack.Name(GemType));
  treasureItemListText.Add(pty.poolSack.NumJewelry(),JewelryType,pty.poolSack.Name(JewelryType));
}

//*****************************************************************************
// NAME: takeTreasureMoney
//
// PURPOSE:  
//*****************************************************************************
BOOL takeTreasureMoney(itemClassType type, int qty, CHARACTER& dude)
{
  if (qty == 0) 
    return FALSE;

	int total = qty; 
  int CoinsPerUnitOfEncumbrance = globalData.moneyData.GetWeight();
  if (CoinsPerUnitOfEncumbrance > 0)
  {
    total = total / CoinsPerUnitOfEncumbrance;
    total = max(1, total);
  }
  else
    total = 0;

  if ((total + dude.GetEncumbrance()) > dude.GetAdjMaxEncumbrance())
  {
		SetMiscError(TooMuchWeight);
    return FALSE;
  }

  int i;
  switch (type) 
  {
  case GemType:
    for (i=0;i<qty;i++)
      dude.money.AddGem();
    break;
  case JewelryType:
    for (i=0;i<qty;i++)
      dude.money.AddJewelry();
    break;
  default:
    dude.money.Add(type, qty);
    break;
  }
  
 	dude.SetEncumbrance(dude.determineEffectiveEncumbrance());
	dude.determineMaxMovement();

  return TRUE;
}


//*****************************************************************************
// NAME: takeTreasure
//
// PURPOSE:  
//*****************************************************************************
BOOL takeTreasure(int item, int qty, int charges, int id, CHARACTER& dude, int cost)
{
  return (dude.addCharacterItem(item, qty, charges, id, cost));
}
