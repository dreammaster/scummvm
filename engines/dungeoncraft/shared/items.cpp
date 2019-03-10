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

#include <math.h>

#ifdef UAFEDITOR
#include "dungeoncraft/shared/..\UAFWinEd\UAFWinEd.h"
#else
#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/..\UAFWin\Dungeon.h"
#endif

#include "dungeoncraft/shared/class.h"
#include "dungeoncraft/shared/SurfaceMgr.h"
#include "dungeoncraft/shared/PicSlot.h"
#include "dungeoncraft/shared/SoundMgr.h"
#include "dungeoncraft/shared/items.h"
#include "dungeoncraft/shared/GlobalData.h"
//#include "dungeoncraft/shared/level.h"
#include "dungeoncraft/shared/Graphics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( ITEM_LIST, CObject, 1)
IMPLEMENT_SERIAL( ITEM_DATA, CObject, 1)
IMPLEMENT_SERIAL( ITEM_DATA_TYPE, CObject, 1)
IMPLEMENT_SERIAL( READY_ITEMS, CObject, 1)

ITEM_DATA_TYPE itemData;

ITEM_TEXT_LIST itemListText;
ITEM_TEXT_LIST charItemListText;
ITEM_TEXT_LIST treasureItemListText;

//*****************************************************************************
//    NAME: isWeapon
//
// PURPOSE: 
//
//*****************************************************************************
BOOL isWeapon(weaponClassType type)
{
  switch (type) 
  {
  case NotWeapon:
  case Ammo:
    return FALSE;
  case HandBlunt:
  case HandCutting:
  case HandThrow:
  case SlingNoAmmo:
  case Bow:
  case Crossbow:
  case Throw:
    return TRUE;
  default:
    WriteDebugString("Bogus item type in IsWeapon()\n");
    break;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: WpnUsesAmmo
//
// PURPOSE: 
//
//*****************************************************************************
BOOL WpnUsesAmmo(weaponClassType type)
{
  switch (type) 
  {
  case NotWeapon:
  case Ammo:
  case HandBlunt:
  case HandCutting:
  case HandThrow:
  case SlingNoAmmo:
  case Throw:
    return FALSE;
  case Bow:
  case Crossbow:
    return TRUE;
  default:
    WriteDebugString("Bogus item type in WpnUsesAmmo()\n");
    break;
  }
  return FALSE;
}


//*****************************************************************************
//    NAME: WpnConsumesSelfAsAmmo
//
// PURPOSE: 
//
//*****************************************************************************
BOOL WpnConsumesSelfAsAmmo(weaponClassType type)
{
  switch (type)
  {
   case HandBlunt:
   case HandCutting:
   case SlingNoAmmo:
   case Bow:
   case Crossbow:
     return FALSE;

   case HandThrow: // dec item qty by 1 when item is thrown
   case Throw:
     return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: isWeapon
//
// PURPOSE: 
//
//*****************************************************************************
BOOL isWeapon(int index)
{
  if (ITEM_LIST::IsMoneyItem(index))
    return FALSE;
  return (isWeapon(itemData.GetWpnType(index)));
}

//*****************************************************************************
//    NAME: WpnUsesAmmo
//
// PURPOSE: 
//
//*****************************************************************************
BOOL WpnUsesAmmo(int index)
{
  if (ITEM_LIST::IsMoneyItem(index))
    return FALSE;
  return (WpnUsesAmmo(itemData.GetWpnType(index))); 
}

//*****************************************************************************
//    NAME: WpnCanAttackAtRange
//
// PURPOSE: 
//
//*****************************************************************************
BOOL WpnCanAttackAtRange(int WpnIndex, int Range)
{
  if (ITEM_LIST::IsMoneyItem(WpnIndex))
    return FALSE;

  switch (itemData.GetWpnType(WpnIndex)) 
  {
  case NotWeapon:
  case Ammo:   
    return FALSE;

  case SlingNoAmmo:
  case Bow:
  case Crossbow:
  case Throw:
    return (   (Range >= 2)
            && (Range <= itemData.GetItemRange(WpnIndex)));

  case HandBlunt:
  case HandCutting:
  case HandThrow:
    return (Range <= itemData.GetItemRange(WpnIndex));

  default:
    WriteDebugString("Bogus item type in IsWeapon()\n");
    break;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: WpnConsumesAmmoAtRange
//
// PURPOSE: 
//
//*****************************************************************************
BOOL WpnConsumesAmmoAtRange(int WpnIndex, int Range)
{
  if (ITEM_LIST::IsMoneyItem(WpnIndex))
    return FALSE;

  switch (itemData.GetWpnType(WpnIndex))
  {
   case HandBlunt:
   case HandCutting:
   case SlingNoAmmo:
     return FALSE;

   case HandThrow: // dec item qty by 1 when item is thrown
   case Throw:
     return (Range > 1);

   case Bow:
   case Crossbow:  // dec ammo qty by 1
     return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: WpnConsumesSelfAsAmmo
//
// PURPOSE: 
//
//*****************************************************************************
BOOL WpnConsumesSelfAsAmmo(int WpnIndex)
{
  if (ITEM_LIST::IsMoneyItem(WpnIndex))
    return FALSE;
  return (WpnConsumesSelfAsAmmo(itemData.GetWpnType(WpnIndex)));
}

//*****************************************************************************
// NAME: isMagical
//
// PURPOSE:  
//*****************************************************************************
BOOL isMagical(int index)
{
  if (ITEM_LIST::IsMoneyItem(index))
    return FALSE;

  ITEM_DATA *data = itemData.GetItemData(index);
  if (data == NULL)
    return FALSE;

  if (data->Cursed)
    return TRUE;

  if (data->Protection_Bonus)
    return TRUE;

  if (data->Attack_Bonus)
    return TRUE;

  if ((data->Dmg_Bonus_Lg)||(data->Dmg_Bonus_Sm))
    return TRUE;

  if ((data->Usable) && (data->Spell >= 0) && (data->Spell!=NO_SPELL))
    return TRUE;

  if (data->specAbs.HaveAtLeastOneSpecAb())
    return TRUE;

  return FALSE;
}

//*****************************************************************************
//    NAME: itemUsesRdySlot
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemUsesRdySlot(int itemkey)
{
  if (ITEM_LIST::IsMoneyItem(itemkey)) return FALSE;
  return (itemUsesRdySlot(itemData.GetItemData(itemkey)));
}

//*****************************************************************************
//    NAME: itemUsesRdySlot
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemUsesRdySlot(ITEM_DATA *pData)
{
  if (pData == NULL) return FALSE;
  return (pData->Location_Carried != CannotBeReadied);
}


//*****************************************************************************
// NAME: itemCanBeTraded
//
// PURPOSE:  
//*****************************************************************************
BOOL itemCanBeTraded(int index)
{
  ITEM_DATA *pItem = itemData.GetItemData(index);
  if (pItem==NULL) return FALSE;
  return (pItem->CanBeTradeDropSoldDep);  
}

//*****************************************************************************
//    NAME: itemCanBeDropped
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeDropped(int index)
{
  ITEM_DATA *pItem = itemData.GetItemData(index);
  if (pItem==NULL) return FALSE;
  return (pItem->CanBeTradeDropSoldDep);    
}

//*****************************************************************************
//    NAME: itemCanBeDeposited
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeDeposited(int index)
{
  ITEM_DATA *pItem = itemData.GetItemData(index);
  if (pItem==NULL) return FALSE;
  return (pItem->CanBeTradeDropSoldDep);    
}

//*****************************************************************************
//    NAME: itemCanBeSold
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeSold(int index)
{
  ITEM_DATA *pItem = itemData.GetItemData(index);
  if (pItem==NULL) return FALSE;
  return (pItem->CanBeTradeDropSoldDep);    
}

//*****************************************************************************
// NAME: itemCanBeJoined
//
// PURPOSE:  
//*****************************************************************************
BOOL itemCanBeJoined(int index)
{
  if (ITEM_LIST::IsMoneyItem(index)) return FALSE;
  ITEM_DATA *pItem = itemData.GetItemData(index);
  if (pItem==NULL) return FALSE;
  if (pItem->Bundle_Qty <= 1) return FALSE;
  if (!pItem->CanBeHalvedJoined) return FALSE;
  return TRUE;
}


//*****************************************************************************
// NAME: itemCanBeHalved
//
// PURPOSE:  
//*****************************************************************************
BOOL itemCanBeHalved(int index)
{
  if (ITEM_LIST::IsMoneyItem(index)) return FALSE;
  ITEM_DATA *pItem = itemData.GetItemData(index);
  if (pItem==NULL) return FALSE;
  if (pItem->Bundle_Qty <= 1) return FALSE;
  if (!pItem->CanBeHalvedJoined) return FALSE;
  return TRUE;
}


//*****************************************************************************
// NAME: itemCanBeReadied
//
// PURPOSE:  
//*****************************************************************************
BOOL itemCanBeReadied(int index)
{
  if (ITEM_LIST::IsMoneyItem(index))
    return FALSE;
  return TRUE;
}

// checks by item class type
//*****************************************************************************
//    NAME: itemCanBeTraded
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeTraded(itemClassType type) 
{ 
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeDropped
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeDropped(itemClassType type) 
{ 
  return ( /*(type!=SpecialItemType)&&(type!=SpecialKeyType)&&*/ (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeDeposited
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeDeposited(itemClassType type) 
{ 
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeSold
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeSold(itemClassType type)
{
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}

//*****************************************************************************
//    NAME: itemCanBeJoined
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeJoined(itemClassType type) 
{ 
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeHalved
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeHalved(itemClassType type) 
{ 
  return ((type != SpecialItemType) && (type != SpecialKeyType) && (type != QuestType)); 
}
//*****************************************************************************
//    NAME: itemCanBeReadied
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemCanBeReadied(itemClassType type) 
{ 
  return (type == itemType); 
}
//*****************************************************************************
//    NAME: itemIsMoney
//
// PURPOSE: 
//
//*****************************************************************************
BOOL itemIsMoney(itemClassType type) 
{
  return (   (type == PlatinumType) || (type == ElectrumType)
          || (type == GoldType)     || (type == SilverType)
          || (type == CopperType)   || (type == GemType)
          || (type == JewelryType)  || (type == CoinType6)
          || (type == CoinType7)    || (type == CoinType8)
          || (type == CoinType9)    || (type == CoinType10));
}

//*****************************************************************************
// NAME: getItemEncumbrance
//
// PURPOSE:  
//*****************************************************************************
int getItemEncumbrance(int item, int qty)
{
  if (qty<=0) return 0;

  if (ITEM_LIST::IsMoneyItem(item))
  {    
    if (globalData.moneyData.GetWeight()>0)
    {
      // GetWeight() returns number of coins
      // per unit of encumbrance
      int result = qty/globalData.moneyData.GetWeight();
      if (result <= 0) result = 1;
      return result;
    }
    else
      return qty;
  }

  ITEM_DATA *data = itemData.GetItemData(item);
  if (data == NULL)
    return 0;
  double bundle = max(data->Bundle_Qty, 1);

  // if bundle qty > 1 then stated encumbrance is for
  // the whole group of items. Figure out the encumbrance
  // for just one item.
	double encumbrance = (data->Encumbrance/bundle);
  /*
  // item encumbrance is stated in terms of number of gold peices.
  // the encumbrance rating given from strength table is in terms
  // of pounds. Divide item encumbrance by number of GP per pound
  int w = globalData.moneyData.GetWeight();
  encumbrance /= w;
  */

  // multiply encumbrance by qty of this item
  encumbrance *= (double)qty;
  return max((int)encumbrance, 0);
}


//*****************************************************************************
//    NAME: ITEM::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM::Serialize(CArchive &ar, double ver)
{
  if (ar.IsStoring())
  {
    ar << key
      << item
      << ready
      << qty
      << identified
      << charges
      << cursed
      << paid;
  }
  else
  {
    ar >> key
      >> item
      >> ready
      >> qty
      >> identified
      >> charges;

    if (ver >= _VERSION_05662_)
     ar >> cursed;
    if (ver >= _VERSION_0696_)
     ar >> paid;
  }
}

//*****************************************************************************
//    NAME: ITEM::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM::Serialize(CAR &ar, double ver)
{
  if (ar.IsStoring())
  {
    ar << key
      << item
      << ready
      << qty
      << identified
      << charges
      << cursed
      << paid;
  }
  else
  {
    ar >> key
      >> item
      >> ready
      >> qty
      >> identified
      >> charges;

    if (ver >= _VERSION_05662_)
     ar >> cursed;
    if (ver >= _VERSION_0696_)
     ar >> paid;
  }
}

//*****************************************************************************
// NAME: READY_ITEMS::GetReady
//
// PURPOSE:  
//*****************************************************************************
int READY_ITEMS::GetReady(itemLocationType type)
{
	switch (type) 
  {
		case WeaponHand:
			return readyWeaponHand;
		case ShieldHand:
			return readyShieldHand;
		case BodyArmor:
			return readyArmor;
		case Hands:
			return readyGauntlets;
		case Head:
			return readyHelmet;
		case Waist:
			return readyBelt;
		case BodyRobe:
			return readyRobe;
		case Back:
			return readyCloak;
		case Feet:
			return readyBoots;

		case Fingers:
      // This is an ambiguous function call for fingers since
      // what you get back as a readied item depends on how
      // many finger slots are currently being used.
      //
      // This function is mostly used for combat purposes to get the
      // current sword/sheild/ammo items being used. The ASSERT is here
      // as a warning that there probably isn't a good reason to call
      // this function for finger items.
      ASSERT(FALSE);

			if (readyRing1 != NO_READY_ITEM) 
        return readyRing1;
      else
        return readyRing2;

		case AmmoQuiver:
			return readyMissileAmmo;
		default:
			WriteDebugString("BOGUS body location %i in READY_ITEMS::Ready\n", 
                       type);
			return NO_READY_ITEM;
  }
}

//*****************************************************************************
//    NAME: READY_ITEMS::CanReady
//
// PURPOSE: 
//
//*****************************************************************************
int READY_ITEMS::CanReady(itemLocationType type)
{
  switch (type) 
  {
		case WeaponHand:
      return (readyWeaponHand == NO_READY_ITEM);
    case ShieldHand:
      return (readyShieldHand == NO_READY_ITEM);
    case BodyArmor:
      return (readyArmor == NO_READY_ITEM);
    case Hands:
      return (readyGauntlets == NO_READY_ITEM);
    case Head:
      return (readyHelmet == NO_READY_ITEM);
    case Waist:
      return (readyBelt == NO_READY_ITEM);
    case BodyRobe:
      return (readyRobe == NO_READY_ITEM);
    case Back:
      return (readyCloak == NO_READY_ITEM);
    case Feet:
      return (readyBoots == NO_READY_ITEM);
    case Fingers:
      return ((readyRing1 == NO_READY_ITEM) || (readyRing2 == NO_READY_ITEM));    
    case AmmoQuiver:
      return (readyMissileAmmo == NO_READY_ITEM);

    default:
      WriteDebugString("BOGUS body location %i in READY_ITEMS::Ready\n", type);
      return NO_READY_ITEM;
  }
}

//*****************************************************************************
// NAME: READY_ITEMS::ClearReady
//
// PURPOSE:  
//*****************************************************************************
/*BOOL READY_ITEMS::ClearReadyByType(itemLocationType type, BYTE hands)
{
	switch (type) 
  {
		case WeaponHand:
			readyWeaponHand = NO_READY_ITEM;
			if (hands > 1)
				readyShieldHand = NO_READY_ITEM;
			break;
		case ShieldHand:
			readyShieldHand = NO_READY_ITEM;
			if (hands > 1)
				readyWeaponHand = NO_READY_ITEM;
			break;
		case BodyArmor:
			readyArmor = NO_READY_ITEM;
			break;
		case Hands:
			readyGauntlets = NO_READY_ITEM;
			break;
		case Head:
			readyHelmet = NO_READY_ITEM;
			break;
		case Waist:
			readyBelt = NO_READY_ITEM;
			break;
		case BodyRobe:
			readyRobe = NO_READY_ITEM;
			break;
		case Back:
			readyCloak = NO_READY_ITEM;
			break;
		case Feet:
			readyBoots = NO_READY_ITEM;
			break;
		case Fingers:
			if (readyRing1 == item) 
        readyRing1 = NO_READY_ITEM;
      else if (readyRing2 == item)
        readyRing2 = NO_READY_ITEM;
			break;
		case AmmoQuiver:
			readyMissileAmmo = NO_READY_ITEM;
			break;
		default:
			WriteDebugString("BOGUS body location %i in READY_ITEMS::ClearReady()\n", 
                       type);
			return FALSE;
  }
  return TRUE;
}
*/

//*****************************************************************************
// NAME: READY_ITEMS::Ready
//
// PURPOSE:  
//*****************************************************************************
BOOL READY_ITEMS::Ready(int item, itemLocationType type, BYTE hands)
{
	switch (type) 
  {
		case WeaponHand:
			readyWeaponHand = item;
			if (hands > 1)
				readyShieldHand = item;
			break;

		case ShieldHand:
			readyShieldHand = item;
			if (hands > 1)
				readyWeaponHand = item;
			break;

		case BodyArmor:
			readyArmor = item;
			break;

		case Hands:
			readyGauntlets = item;
			break;

		case Head:
			readyHelmet = item;
			break;

		case Waist:
			readyBelt = item;
			break;

		case BodyRobe:
			readyRobe = item;
			break;

		case Back:
			readyCloak = item;
			break;

		case Feet:
			readyBoots = item;
			break;

		case Fingers:
			if (readyRing1 == NO_READY_ITEM) 
        readyRing1 = item;
      else if (readyRing2 == NO_READY_ITEM)
        readyRing2 = item;
			break;

		case AmmoQuiver:
			readyMissileAmmo = item;
			break;

		default:
			WriteDebugString("BOGUS body location %i in READY_ITEMS::Ready\n", 
                       type);
			return FALSE;
	}

  return TRUE;
}

//*****************************************************************************
// NAME: READY_ITEMS::UnReady
//
// PURPOSE:  
//*****************************************************************************
BOOL READY_ITEMS::UnReady(int item)
{

  // items can be assigned to one, possibly two, slots
	if (readyWeaponHand == item)
		readyWeaponHand = NO_READY_ITEM;

	if (readyShieldHand == item)
		readyShieldHand = NO_READY_ITEM;

	if (readyMissileAmmo == item)
		readyMissileAmmo = NO_READY_ITEM;

	if (readyArmor == item)
		readyArmor = NO_READY_ITEM;

	if (readyGauntlets == item)
		readyGauntlets = NO_READY_ITEM;

	if (readyHelmet == item)
		readyHelmet = NO_READY_ITEM;

	if (readyBelt == item)
		readyBelt = NO_READY_ITEM;

	if (readyRobe == item)
		readyRobe = NO_READY_ITEM;

	if (readyCloak == item)
		readyCloak = NO_READY_ITEM;

	if (readyBoots == item)
		readyBoots = NO_READY_ITEM;

	if (readyRing1 == item)
		readyRing1 = NO_READY_ITEM;

	if (readyRing2 == item)
		readyRing2 = NO_READY_ITEM;

  return TRUE;
}


//*****************************************************************************
// NAME: READY_ITEMS::operator=
//
// PURPOSE:  
//*****************************************************************************
READY_ITEMS& READY_ITEMS::operator=(const READY_ITEMS& src)
{
  if (&src == this) return *this;
	readyWeaponHand = src.readyWeaponHand;
	readyShieldHand = src.readyShieldHand;
	readyMissileAmmo = src.readyMissileAmmo;
	readyArmor = src.readyArmor;
	readyGauntlets = src.readyGauntlets;
	readyHelmet = src.readyHelmet;
	readyBelt = src.readyBelt;
	readyRobe = src.readyRobe;
	readyCloak = src.readyCloak;
	readyBoots = src.readyBoots;
	readyRing1 = src.readyRing1;
	readyRing2 = src.readyRing2;  
  return *this;
}

//*****************************************************************************
//    NAME: READY_ITEMS::operator==
//
// PURPOSE: 
//
//*****************************************************************************
BOOL READY_ITEMS::operator==(const READY_ITEMS& src)
{
  if (&src == this) return TRUE;
	if (readyWeaponHand != src.readyWeaponHand) return FALSE;
	if (readyShieldHand != src.readyShieldHand) return FALSE;
	if (readyMissileAmmo != src.readyMissileAmmo) return FALSE;
	if (readyArmor != src.readyArmor) return FALSE;
	if (readyGauntlets != src.readyGauntlets) return FALSE;
	if (readyHelmet != src.readyHelmet) return FALSE;
	if (readyBelt != src.readyBelt) return FALSE;
	if (readyRobe != src.readyRobe) return FALSE;
	if (readyCloak != src.readyCloak) return FALSE;
	if (readyBoots != src.readyBoots) return FALSE;
	if (readyRing1 != src.readyRing1) return FALSE;
	if (readyRing2 != src.readyRing2) return FALSE;
  return TRUE;
}

//*****************************************************************************
// NAME: READY_ITEMS::Clear
//
// PURPOSE:  
//*****************************************************************************
void READY_ITEMS::Clear()
{
	readyWeaponHand = NO_READY_ITEM;
	readyShieldHand = NO_READY_ITEM;
	readyMissileAmmo = NO_READY_ITEM;
	readyArmor = NO_READY_ITEM;
	readyGauntlets = NO_READY_ITEM;
	readyHelmet = NO_READY_ITEM;
	readyBelt = NO_READY_ITEM;
	readyRobe = NO_READY_ITEM;
	readyCloak = NO_READY_ITEM;
	readyBoots = NO_READY_ITEM;
	readyRing1 = NO_READY_ITEM;
	readyRing2 = NO_READY_ITEM;
}

//*****************************************************************************
// NAME: READY_ITEMS::Serialize
//
// PURPOSE:  
//*****************************************************************************
void READY_ITEMS::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);

  if (ar.IsStoring())
  {
	  ar << readyWeaponHand;
	  ar << readyShieldHand;
	  ar << readyMissileAmmo;
	  ar << readyArmor;
	  ar << readyGauntlets;
	  ar << readyHelmet;
	  ar << readyBelt;
	  ar << readyRobe;
	  ar << readyCloak;
	  ar << readyBoots;
	  ar << readyRing1;
	  ar << readyRing2;
  }
  else
  {
	  ar >> readyWeaponHand;
	  ar >> readyShieldHand;
	  ar >> readyMissileAmmo;
	  ar >> readyArmor;
	  ar >> readyGauntlets;
	  ar >> readyHelmet;
	  ar >> readyBelt;
	  ar >> readyRobe;
	  ar >> readyCloak;
	  ar >> readyBoots;
	  ar >> readyRing1;
	  ar >> readyRing2;
  }
}
//*****************************************************************************
//    NAME: READY_ITEMS::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void READY_ITEMS::Serialize(CAR &ar)
{
  //CObject::Serialize(ar);

  if (ar.IsStoring())
  {
	  ar << readyWeaponHand;
	  ar << readyShieldHand;
	  ar << readyMissileAmmo;
	  ar << readyArmor;
	  ar << readyGauntlets;
	  ar << readyHelmet;
	  ar << readyBelt;
	  ar << readyRobe;
	  ar << readyCloak;
	  ar << readyBoots;
	  ar << readyRing1;
	  ar << readyRing2;
  }
  else
  {
	  ar >> readyWeaponHand;
	  ar >> readyShieldHand;
	  ar >> readyMissileAmmo;
	  ar >> readyArmor;
	  ar >> readyGauntlets;
	  ar >> readyHelmet;
	  ar >> readyBelt;
	  ar >> readyRobe;
	  ar >> readyCloak;
	  ar >> readyBoots;
	  ar >> readyRing1;
	  ar >> readyRing2;
  }
}

//*****************************************************************************
// NAME: ITEM_LIST::CanReady
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_LIST::CanReady(int index, BYTE ItemMask)
{
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) == NULL)
    return FALSE;

  if (IsMoneyItem(items.GetAt(pos).item))
    return FALSE;

  if (items.GetAt(pos).ready)
    return FALSE;

  if (items.GetAt(pos).qty <= 0)
    return FALSE;

  ITEM_DATA *data = itemData.GetItemData(items.GetAt(pos).item);
  if (data == NULL)
    return FALSE;

  // dont handle items using more than 2 hands yet
  if (data->Hands_to_Use > 2)
    return FALSE;

	if ((data->Usable_by_Class & ItemMask) == 0)
	{
		SetMiscError(WrongClass);
		return FALSE;
	}

  if (itemUsesRdySlot(data))
  {
	  if (   (data->Hands_to_Use == 2) 
        && ((data->Location_Carried == WeaponHand) || (data->Location_Carried == ShieldHand)))
	  {
		  if (   (rdyItems.GetReady(WeaponHand) != NO_READY_ITEM)
			    || (rdyItems.GetReady(ShieldHand) != NO_READY_ITEM))
      {
			  SetMiscError(TakesTwoHands);
		    return FALSE;
      }
	  }
    else if (data->Hands_to_Use > 0)
    {
      if (   (rdyItems.GetReady(WeaponHand) != NO_READY_ITEM)
          && (rdyItems.GetReady(ShieldHand) != NO_READY_ITEM))
      {
        SetMiscError(NoFreeHands);
        return FALSE;
      }
    }

    // something in this slot already
    if (!rdyItems.CanReady(data->Location_Carried))
    {
      SetMiscError(ItemAlreadyReadied);
      return FALSE;
    }
  }

  return TRUE;
}

//*****************************************************************************
// NAME: ITEM_LIST::Ready
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_LIST::Ready(int index, BYTE ItemMask)
{
  ITEM_DATA *data = itemData.GetItemData(GetItem(index));
  if (data == NULL) return FALSE;

  if (!CanReady(index, ItemMask)) return FALSE;

  BOOL result = TRUE;
  if (itemUsesRdySlot(data))
    result = rdyItems.Ready(index, data->Location_Carried, data->Hands_to_Use);
  if (result)
    SetReady(index, TRUE);
  return (IsReady(index));
}

//*****************************************************************************
//    NAME: ITEM_LIST::ClearReady
//
// PURPOSE: 
//
//*****************************************************************************
/*
BOOL ITEM_LIST::ClearReadyByType(itemLocationType type);
{  
  rdyItems.ClearReady(data->Location_Carried,);
  SetReady(item, FALSE);
  return TRUE;
}*/



//*****************************************************************************
// NAME: ITEM_LIST::UnReady
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_LIST::UnReady(int item)
{
  if (!IsReady(item)) return TRUE;
  if (!CanUnReady(item)) return FALSE;
  if (itemUsesRdySlot(item))
    rdyItems.UnReady(item);
  SetReady(item, FALSE);
  return TRUE;
}

//*****************************************************************************
// NAME: CanUnReady
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_LIST::CanUnReady(int item)
{
  if (!ValidItemListIndex(item))
    return FALSE;
  if (!IsReady(item)) 
    return FALSE;
  return (!IsCursed(item));
}

//*****************************************************************************
// NAME: ITEM_LIST::Serialize
//
// PURPOSE:  
//*****************************************************************************
void ITEM_LIST::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);
  int count;
  POSITION pos;
  ITEM data;

  if (ar.IsStoring())
  {
    ar << GetCount();
    pos = GetHeadPosition();
    while (pos != NULL)
      GetNext(pos).Serialize(ar, ver);
  }
  else
  {
    Clear();
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Serialize(ar, ver);
      AddItemWithCurrKey(data);
    }
  }

  rdyItems.Serialize(ar);
}

//*****************************************************************************
//    NAME: ITEM_LIST::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::Serialize(CAR &ar, double ver)
{
  //CObject::Serialize(ar);
  int count;
  ITEM data;
  POSITION pos;

  if (ar.IsStoring())
  {
    ar << GetCount();
    pos = GetHeadPosition();
    while (pos != NULL)
      GetNext(pos).Serialize(ar, ver);
  }
  else
  {
    Clear();
    ar >> count;
    for (int i=0;i<count;i++)
    {
      data.Serialize(ar, ver);
      AddItemWithCurrKey(data);
    }
  }

  rdyItems.Serialize(ar);
}

//*****************************************************************************
// NAME: ITEM_LIST::GetProtectModForRdyItems
//
// PURPOSE:  
//*****************************************************************************
int ITEM_LIST::GetProtectModForRdyItems()
{
  int acMod = 0; 
  POSITION pos;
  pos = items.GetHeadPosition();
  while (pos != NULL)
  {
    if (items.GetAt(pos).ready)
    {
      ITEM_DATA *data = itemData.GetItemData(items.GetAt(pos).item);
      if (data != NULL)
		    acMod += (data->Protection_Base + data->Protection_Bonus);
    }
    items.GetNext(pos);
  }
  return acMod;
}


//*****************************************************************************
// NAME: ITEM_LIST::GetAttackModForRdyItems
//
// PURPOSE:  
//*****************************************************************************
int ITEM_LIST::GetAttackModForRdyItems()
{
  int mod = 0; 
  POSITION pos;
  pos = items.GetHeadPosition();
  while (pos != NULL)
  {
    if (items.GetAt(pos).ready)
    {
      ITEM_DATA *data = itemData.GetItemData(items.GetAt(pos).item);
      if (data != NULL)
		    mod += data->Attack_Bonus;
    }
    items.GetNext(pos);
  }

  return mod;
}

#ifdef UAFEngine

//*****************************************************************************
//    NAME: ITEM_TEXT_LIST::UpdateDetectMagic
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_TEXT_LIST::UpdateDetectMagic(BOOL detecting)
{
  DetectingMagic = detecting;
}

//*****************************************************************************
//    NAME: ITEM_TEXT_LIST::UpdateIsReady
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_TEXT_LIST::UpdateIsReady(int index, BOOL ready) 
{ 
  items[index].ready = ready;
  DetectingMagic = party.PartyIsDetectingMagic();
}

//*****************************************************************************
// NAME: ITEM_TEXT_LIST::FillItemListText
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::FillItemListText(ITEM_LIST &list, UINT flags, costFactorType type)
{
	Clear();

  UseQty  = flags & QTY;
  UseRdy  = flags & RDY;
  UseCost = flags & COST;
  UseMoney = flags & MONEY;

  DetectingMagic = flags & DETECTING_MAGIC;
  CostFactor = type;

  POSITION pos = list.GetHeadPosition();
	while (pos != NULL)
	{
    Add(list.GetAt(pos).qty,
        itemType, 
        NULL, 
        list.GetAt(pos).key,
        list.GetAt(pos).identified,
        list.GetAt(pos).charges,
        list.GetAt(pos).item,
        list.GetAt(pos).ready);

    list.GetNext(pos);
  }

  FormatItemListTitle();
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::AddItemListText
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::AddItemListText(ITEM_LIST &list, UINT flags, costFactorType type, int data)
{
  UseQty  = flags & QTY;
  UseRdy  = flags & RDY;
  UseCost = flags & COST;
  UseMoney = flags & MONEY;

  DetectingMagic = flags & DETECTING_MAGIC;
  CostFactor = type;

  POSITION pos = list.GetHeadPosition();
	while (pos != NULL)
	{
    Add(list.GetAt(pos).qty,
        itemType, 
        NULL, 
        list.GetAt(pos).key,
        list.GetAt(pos).identified,
        list.GetAt(pos).charges,
        list.GetAt(pos).item,
        list.GetAt(pos).ready,
        data);

    list.GetNext(pos);
  }

  FormatItemListTitle();
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::AddItemText
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::AddItemText(ITEM &addItem, UINT flags, costFactorType type)
{
  UseQty  = flags & QTY;
  UseRdy  = flags & RDY;
  UseCost = flags & COST;
  UseMoney = flags & MONEY;

  DetectingMagic = flags & DETECTING_MAGIC;
  CostFactor = type;

	if (addItem.item > 0)
	{
    Add(addItem.qty, 
        itemType, 
        NULL, 
        0,
        addItem.identified,
        addItem.charges,
        addItem.item,
        addItem.ready);
	}

  FormatItemListTitle();
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::Add
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::Add(int qty, itemClassType type, const char *label,
                         int origIndex, BOOL id, int charges, 
                         int item, BOOL ready, int data)
{
  if (qty <= 0)
    return;

  ITEM_TEXT newItem;
  newItem.charges = charges;    
  newItem.itemClass = type;
  newItem.qty = qty;
  newItem.origIndex = origIndex;
  newItem.item = item;            
  newItem.identified = id;
  newItem.ready = ready;
  newItem.data = data; // data supplied for later identification of item context
  if (label != NULL)
    newItem.label = label;
  else
    newItem.label = "";
  items.Add(newItem);
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::ItemCanBeHalved
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_TEXT_LIST::ItemCanBeHalved(int index)
{
  itemClassType type = GetItemClass(index);
  int item = GetItemIndex(index);

  if (   (itemCanBeHalved(type))
      && (itemCanBeHalved(item)))
    return TRUE;
  else
    return FALSE;
}


//*****************************************************************************
// NAME: ITEM_TEXT_LIST::ItemCanBeJoined
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_TEXT_LIST::ItemCanBeJoined(int index)
{
  return (   (itemCanBeJoined(GetItemClass(index)))
          && (itemCanBeJoined(GetItemIndex(index))));
}

//*****************************************************************************
// NAME: ITEM_TEXT_LIST::FormatItemListTitle
//
// PURPOSE:  
//*****************************************************************************
void ITEM_TEXT_LIST::FormatItemListTitle(const char *EmptyListTitle)
{
  //const int width = 6;
  
  RdyOffset=0;
  QtyOffset=0;
  CostOffset=0;
  NameOffset=0;

  int RdyColWidth=0;
  int QtyColWidth=0;
  int CostColWidth=0;

  if (GetCount() <= 0)
  {
    if (EmptyListTitle != NULL)
      title = EmptyListTitle;
    else
      title = "No Items";
    return;
  }

	char tmp[300];
	memset(tmp, ' ', sizeof(tmp));

	if (UseRdy)
    RdyColWidth =  GraphicsMgr.GetTextWidth("Rdy   ", -1);

	if (UseQty)
    QtyColWidth =  GraphicsMgr.GetTextWidth("Qty   ",-1);

	if (UseCost)
    CostColWidth = GraphicsMgr.GetTextWidth("Cost  ",-1);

  int PixelWidth = GraphicsMgr.GetTextWidth(" ", 1) + 1;

  RdyOffset  = 0; // will always start in col 0 if used
  QtyOffset  = RdyOffset  + (RdyColWidth  / PixelWidth);
  CostOffset = QtyOffset  + (QtyColWidth  / PixelWidth);
  NameOffset = CostOffset + (CostColWidth / PixelWidth);

  if (UseRdy)
    memmove(&tmp[RdyOffset], "Rdy", 3);
  if (UseQty)
    memmove(&tmp[QtyOffset], "Qty", 3);
  if (UseCost)
    memmove(&tmp[CostOffset], "Cost", 4);

	strcpy(&tmp[NameOffset], "Item");

	title = tmp;
}

char *ITEM_TEXT_LIST::GetItemRdyText(int index)
{
  if (!UseRdy)
    return NULL;  
  if (items[index].itemClass != itemType)
    return NULL;

  if (ITEM_LIST::IsMoneyItem(items[index].item))
    return NULL;

	if (items[index].ready)
		strcpy(temp, " Yes");
	else
		strcpy(temp, " No ");
  return temp;
}

//*****************************************************************************
//    NAME: *ITEM_TEXT_LIST::GetItemQtyText
//
// PURPOSE: 
//
//*****************************************************************************
char *ITEM_TEXT_LIST::GetItemQtyText(int index)
{
  if (!UseQty)
    return NULL;

  if (GetRunAsVersion(SpecialItemKeyQtyVersion) >= _VERSION_0820_)
  {
    if (   (items[index].itemClass == SpecialItemType)
        || (items[index].itemClass == SpecialKeyType)
        || (items[index].itemClass == QuestType))
      return NULL;
  }

	sprintf(temp,"%9i", items[index].qty);
  return temp;
}

//*****************************************************************************
//    NAME: *ITEM_TEXT_LIST::GetItemCostText
//
// PURPOSE: 
//
//*****************************************************************************
char *ITEM_TEXT_LIST::GetItemCostText(int index)
{
  if (!UseCost)
    return NULL;

  if (items[index].itemClass != itemType)
    return NULL;

  DWORD cost;

  if (ITEM_LIST::IsMoneyItem(items[index].item))
  {
    cost = items[index].charges;
  }
  else
  {
    long iprice = itemData.GetItemCost(items[index].item);
    cost = ApplyCostFactor(CostFactor, iprice);
  }
	sprintf(temp,"%9i", cost);
  return temp;
}

//*****************************************************************************
//    NAME: *ITEM_TEXT_LIST::GetItemNameText
//
// PURPOSE: 
//
//*****************************************************************************
char *ITEM_TEXT_LIST::GetItemNameText(int index)
{
  if (items[index].itemClass == itemType)
  {
    if (ITEM_LIST::IsMoneyItem(items[index].item))
    {
      if (items[index].item == ITEM_LIST::GemItem)
  		  sprintf(temp, "%s (%u %s)", globalData.moneyData.GetName(GemType), 
                items[index].charges,
                globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()));
      else
        sprintf(temp, "%s (%u %s)", globalData.moneyData.GetName(JewelryType),
                items[index].charges,
                globalData.moneyData.GetName(globalData.moneyData.GetDefaultType()));
    }
    else
    {
      if ((DetectingMagic) && (isMagical(items[index].item)))
      {
        if ((items[index].identified) && (!itemData.GetItemIdName(items[index].item).IsEmpty()))
        {
          strcpy(temp, "*");
          strncat(temp,(LPCSTR)itemData.GetItemIdName(items[index].item) , MAX_ITEM_TEXT_LEN);
          temp[MAX_ITEM_TEXT_LEN]='\0';
  		    //sprintf(temp, "*%s",
	  			//        (LPCSTR)itemData.GetItemIdName(items[index].item));
        }
        else
        {
          strcpy(temp, "*");
          strncat(temp,(LPCSTR)itemData.GetItemName(items[index].item) , MAX_ITEM_TEXT_LEN);
          temp[MAX_ITEM_TEXT_LEN]='\0';
  		    //sprintf(temp, "*%s",
	  			//        (LPCSTR)itemData.GetItemName(items[index].item));
        }
      }
      else 
      {
        if ((items[index].identified) && (!itemData.GetItemIdName(items[index].item).IsEmpty()))
        {
			    strncpy(temp, itemData.GetItemIdName(items[index].item), MAX_ITEM_TEXT_LEN);
          temp[MAX_ITEM_TEXT_LEN]='\0';
        }
        else
        {
			    strncpy(temp, itemData.GetItemName(items[index].item), MAX_ITEM_TEXT_LEN);
          temp[MAX_ITEM_TEXT_LEN]='\0';
        }
      }
    }
  }
  else
  {
    strncpy(temp, items[index].label, MAX_ITEM_TEXT_LEN);
    temp[MAX_ITEM_TEXT_LEN]='\0';
  }
  return temp;
}

//*****************************************************************************
// NAME: *ITEM_TEXT_LIST::GetItemText
//
// PURPOSE:  
//*****************************************************************************
char *ITEM_TEXT_LIST::GetItemText(int index)
{
  memset(text,' ',sizeof(text));
  char *tmp;

  if (UseRdy)
  {
    tmp = GetItemRdyText(index);
    if (tmp)
		  memmove(&text[RdyOffset], tmp, strlen(tmp));
  }

	if (UseQty)
  {
    tmp = GetItemQtyText(index);
    if (tmp)
		  memmove(&text[QtyOffset], tmp, strlen(tmp));
  }

  if (UseCost)
  {
    tmp = GetItemCostText(index);
    if (tmp)
      memmove(&text[CostOffset], tmp, strlen(tmp));
  }

  int buffRemaining = MAX_ITEM_TEXT_LEN-NameOffset;
  strncpy(&text[NameOffset], GetItemNameText(index), buffRemaining);

  return text;
}

#endif
//*****************************************************************************
// NAME: ITEM_DATA::Serialize
//
// PURPOSE:  
//*****************************************************************************
void ITEM_DATA::Serialize(CArchive &ar, double ver)
{
  CObject::Serialize(ar);
  long temp;

  if (ar.IsStoring())
  {
    if (ver <= _VERSION_0830_)
      Name.Replace('/','|');
    ar << key;
    AS(ar,Name);
    AS(ar,Id_Name);
    AS(ar,HitSound);
    AS(ar,MissSound);
    AS(ar,LaunchSound);
    AS(ar,AmmoType);
    ar << Experience;
    ar << Cost;
    ar << Encumbrance;
    ar << Attack_Bonus;
    ar << Cursed;
    ar << Bundle_Qty;
    ar << Num_Charges;
    temp = Location_Carried;
    ar << temp;
    ar << Hands_to_Use;
    ar << Dmg_Dice_Sm;
    ar << Nbr_Dice_Sm;
    ar << Dmg_Bonus_Sm;
    ar << Dmg_Dice_Lg;
    ar << Nbr_Dice_Lg;
    ar << Dmg_Bonus_Lg;
    ar << ROF_Per_Round;
    ar << Protection_Base;
    ar << Protection_Bonus;

    temp = Wpn_Type;
    ar << temp;
    ar << Usable;
    ar << Usable_by_Class;
    ar << Range;
    ar << Spell;
    ar << Spell_Level;
    ar << Spell_Class;
    ar << event;
    ar << ExamineEvent;
    AS(ar, ExamineLabel);      
    AS(ar, attackMsg);
    ar << (int)Recharge_Rate;
    ar << IsNonLethal;
    HitArt.Serialize(ar,ver);
    ar << CanBeHalvedJoined;
    ar << CanBeTradeDropSoldDep;
    // MissileArt is serialized in attribute map
  }
  else
  {
    ar >> key;
    DAS(ar,Name);

#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Battle Axe|Dull" will be
    // read as simply "Battle Axe".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    if (Name.Find('|') >= 0) Name = Name.Left(Name.Find('|')); // Remove qualifier.
#endif

    DAS(ar,Id_Name);
    DAS(ar,HitSound);
    DAS(ar,MissSound);
    if (ver >= _VERSION_05691_)
      DAS(ar,LaunchSound);
    if (ver >= _VERSION_0690_)
      DAS(ar,AmmoType);
    ar >> Experience;
    ar >> Cost;
    ar >> Encumbrance;
    ar >> Attack_Bonus;
    ar >> Cursed;
    ar >> Bundle_Qty;
    ar >> Num_Charges;
  
    ar >> temp;
    Location_Carried = (itemLocationType)temp;

    ar >> Hands_to_Use;
    ar >> Dmg_Dice_Sm;
    ar >> Nbr_Dice_Sm;
    ar >> Dmg_Bonus_Sm;
    ar >> Dmg_Dice_Lg;
    ar >> Nbr_Dice_Lg;
    ar >> Dmg_Bonus_Lg;
    ar >> ROF_Per_Round;
    ar >> Protection_Base;
    ar >> Protection_Bonus;
  
    ar >> temp;
    Wpn_Type = (weaponClassType)temp;

    ar >> Usable;
    ar >> Usable_by_Class;
    ar >> Range;
  
    ar >> Spell;
    ar >> Spell_Level;
    ar >> Spell_Class;

    if (ver >= _VERSION_0662_)
      ar >> event;

    if (ver >= _VERSION_0800_)
    {
      ar >> ExamineEvent;
      DAS(ar, ExamineLabel);
    }

    if (ver >= _VERSION_0860_)
    {
      DAS(ar, attackMsg);
    }
    else
      attackMsg="attacks";

    if (ver >= _VERSION_0690_)
    {
      ar >> temp;
      Recharge_Rate = (itemRechargeRate)temp;
      ar >> IsNonLethal;
      HitArt.Serialize(ar,ver);
    }
    if (ver >= _VERSION_0881_)
      ar >> CanBeHalvedJoined;
    else
      CanBeHalvedJoined = TRUE;

    if (ver >= _VERSION_0904_)
      ar >> CanBeTradeDropSoldDep;
    else
      CanBeTradeDropSoldDep = TRUE;

    // MissileArt is serialized in attribute map
  }

  specAbs.Serialize(ar, ver);
  item_asl.Serialize(ar);
}
//*****************************************************************************
//    NAME: ITEM_DATA::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::Serialize(CAR &ar, double ver)
{
  long temp;

  if (ar.IsStoring())
  {
    if (ver <= _VERSION_0830_)
      Name.Replace('/','|');
    ar << key;
    AS(ar,Name);
    AS(ar,Id_Name);
    AS(ar,HitSound);
    AS(ar,MissSound);
    AS(ar,LaunchSound);
    AS(ar,AmmoType);
    ar << Experience;
    ar << Cost;
    ar << Encumbrance;
    ar << Attack_Bonus;
    ar << Cursed;
    ar << Bundle_Qty;
    ar << Num_Charges;
    temp = Location_Carried;
    ar << temp;
    ar << Hands_to_Use;
    ar << Dmg_Dice_Sm;
    ar << Nbr_Dice_Sm;
    ar << Dmg_Bonus_Sm;
    ar << Dmg_Dice_Lg;
    ar << Nbr_Dice_Lg;
    ar << Dmg_Bonus_Lg;
    ar << ROF_Per_Round;
    ar << Protection_Base;
    ar << Protection_Bonus;

    temp = Wpn_Type;
    ar << temp;
    ar << Usable;
    ar << Usable_by_Class;
    ar << Range;
    ar << Spell;
    ar << Spell_Level;
    ar << Spell_Class;
    ar << event;
    ar << ExamineEvent;
    AS(ar, ExamineLabel);
    AS(ar, attackMsg);
    ar << Recharge_Rate;
    ar << IsNonLethal;
    HitArt.Serialize(ar,ver);
    ar << CanBeHalvedJoined;
    ar << CanBeTradeDropSoldDep;
    // MissileArt is serialized in attribute map
  }
  else
  {
    ar >> key;
    DAS(ar,Name);
#ifdef UAFEngine
    // In the runtime, Name means only the un-decorated
    // part of the name.  For example:  "Battle Axe|Dull" will be
    // read as simply "Battle Axe".  There can be multiple objects with
    // the same name but once we are at the point where we have the binary 
    // data then everything is identified uniquely and is referenced by the 
    // binary key.  The name is only for printing.
    if (Name.Find('|') >= 0) Name = Name.Left(Name.Find('|')); // Remove qualifier.
#endif
    DAS(ar,Id_Name);
    DAS(ar,HitSound);
    DAS(ar,MissSound);
    if (ver >= _VERSION_05691_)
      DAS(ar,LaunchSound);
    if (ver >= _VERSION_0690_)
      DAS(ar,AmmoType);
    ar >> Experience;
    ar >> Cost;
    ar >> Encumbrance;
    ar >> Attack_Bonus;
    ar >> Cursed;
    ar >> Bundle_Qty;
    ar >> Num_Charges;
  
    ar >> temp;
    Location_Carried = (itemLocationType)temp;

    ar >> Hands_to_Use;
    ar >> Dmg_Dice_Sm;
    ar >> Nbr_Dice_Sm;
    ar >> Dmg_Bonus_Sm;
    ar >> Dmg_Dice_Lg;
    ar >> Nbr_Dice_Lg;
    ar >> Dmg_Bonus_Lg;
    ar >> ROF_Per_Round;
    ar >> Protection_Base;
    ar >> Protection_Bonus;
  
    ar >> temp;
    Wpn_Type = (weaponClassType)temp;

    ar >> Usable;
    ar >> Usable_by_Class;
    ar >> Range;
  
    ar >> Spell;
    ar >> Spell_Level;
    ar >> Spell_Class;

    if (ver >= _VERSION_0662_)
      ar >> event;
    
    if (ver >= _VERSION_0800_)
    {
      ar >> ExamineEvent;
      DAS(ar, ExamineLabel);
    }
    
    if (ver >= _VERSION_0860_)
    {
      DAS(ar, attackMsg);
    }
    else
      attackMsg="attacks";

    if (ver >= _VERSION_0690_)
    {
      ar >> temp;
      Recharge_Rate = (itemRechargeRate)temp;
      ar >> IsNonLethal;
      HitArt.Serialize(ar,ver);
    }
    if (ver >= _VERSION_0881_)
      ar >> CanBeHalvedJoined;
    else
      CanBeHalvedJoined = TRUE;

    if (ver >= _VERSION_0904_)
      ar >> CanBeTradeDropSoldDep;
    else
      CanBeTradeDropSoldDep = TRUE;

    // MissileArt is serialized in attribute map
  }

  specAbs.Serialize(ar, ver);
  if (ar.IsStoring())
    ar << item_asl;
  else
    ar >> item_asl;
}

#ifdef UAFEDITOR
// event descriptive text for editor
//*****************************************************************************
//    NAME: ITEM_DATA::GetItemEventText
//
// PURPOSE: 
//
//*****************************************************************************
CString ITEM_DATA::GetItemEventText() 
{ 
  CString tmp;
  tmp.Format("Item Event: %s", Name);
  return tmp;
}
#endif

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Clear
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::Clear() 
{ 
  CSingleLock sLock(&m_CS, TRUE); 
  version = PRODUCT_VER;
  ItemData.RemoveAll(); 
  m_AmmoTypes.RemoveAll(); 
  m_AmmoTypes.Add("None");
}

//*****************************************************************************
// NAME: ITEM_DATA_TYPE::operator =
//
// PURPOSE: Make a copy of an ITEM_DATA list
//*****************************************************************************
ITEM_DATA_TYPE& ITEM_DATA_TYPE:: operator=(ITEM_DATA_TYPE& src)
{
  if (&src==this) return *this;
  version=src.version;
  ItemData.RemoveAll();
  POSITION p=src.ItemData.GetHeadPosition();
  while (p!=NULL)
  {
    AddItemWithCurrKey(src.ItemData.GetNext(p));
  };

  m_AmmoTypes.RemoveAll();
  m_AmmoTypes.SetSize(src.m_AmmoTypes.GetSize());
  for (int i=0;i<src.m_AmmoTypes.GetSize();i++)
    m_AmmoTypes[i]=src.m_AmmoTypes[i];
  return *this;
}

//*****************************************************************************
// NAME: ITEM_DATA_TYPE::operator ==
//
// PURPOSE: Test that two ITEM_DATA lists are identical...even the ordering.
//          One might like to exclude order but then it is a bit tricky
//          because of the possiblility of two identical entries in a list.
//*****************************************************************************
bool ITEM_DATA_TYPE:: operator ==(ITEM_DATA_TYPE& src)
{
  if (&src == this) return true;
  if (version!=src.version) return false;
  POSITION p1=ItemData.GetHeadPosition();
  POSITION p2=src.ItemData.GetHeadPosition();
  while (p1!=NULL)
  {
    if (p2==NULL) return false;
    if (!(ItemData.GetNext(p1)==src.ItemData.GetNext(p2))) return false;
  };
  if (p2!=NULL) return false;
  if (m_AmmoTypes.GetSize() != src.m_AmmoTypes.GetSize()) return false;
  for (int i=0;i<m_AmmoTypes.GetSize();i++)
    if (m_AmmoTypes[i] != src.m_AmmoTypes[i]) return false;
  return true;
}

//*****************************************************************************
// NAME: ITEM_DATA_TYPE::Serialize
//
// PURPOSE:  
//*****************************************************************************
void ITEM_DATA_TYPE::Serialize(CArchive &ar)
{
  CObject::Serialize(ar);  
  int i;
  POSITION pos;
  if (ar.IsStoring())
  {
    ar << GetCount();
    pos = ItemData.GetHeadPosition();
    while (pos != NULL)
      ItemData.GetNext(pos).Serialize(ar, version);
    ar << m_AmmoTypes.GetSize();
    for (i=0;i<m_AmmoTypes.GetSize();i++)
    {
      AS(ar,m_AmmoTypes[i]);
    }
  }
  else
  {
    //Clear();
    ITEM_DATA data;
    int temp;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      data.Serialize(ar,version);
      AddItemWithCurrKey(data);
    }
    if (version >= _VERSION_0690_)
    {
      ar >> temp;
      ASSERT((temp>=0)&&(temp<=MAX_ITEMS));
      m_AmmoTypes.SetSize(temp);
      for (i=0;i<temp;i++)
      {
        DAS(ar,m_AmmoTypes[i]);
      }
    }
  }
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Serialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::Serialize(CAR &ar)
{
  int i;
  POSITION pos;
  if (ar.IsStoring())
  {
    ar << GetCount();
    pos = ItemData.GetHeadPosition();
    while (pos != NULL)
      ItemData.GetNext(pos).Serialize(ar, version);
    ar << m_AmmoTypes.GetSize();
    for (i=0;i<m_AmmoTypes.GetSize();i++)
    {
      AS(ar,m_AmmoTypes[i]);
    }
  }
  else
  {
    //Clear();
    ITEM_DATA data;
    int temp;
    ar >> temp;
    for (i=0; i<temp; i++)
    {
      data.Clear();
      data.Serialize(ar, version);
      AddItemWithCurrKey(data);
    }
    if (version >= _VERSION_0690_)
    {
      ar >> temp;
      ASSERT((temp>=0)&&(temp<=MAX_ITEMS));
      m_AmmoTypes.SetSize(temp);
      for (i=0;i<temp;i++)
        DAS(ar,m_AmmoTypes[i]);
    }
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA::ITEM_DATA
//
// PURPOSE: 
//
//*****************************************************************************
ITEM_DATA::ITEM_DATA() : 
	item_asl("ITEM_DATA_ATTRIBUTES"),
	temp_asl("ITEM_DATA_ATTRIBUTES"),
	hHitSound(-1), hMissSound(-1), hLaunchSound(-1), event(0),ExamineEvent(0)
{  
  Clear(TRUE);
}

//*****************************************************************************
//    NAME: ITEM_DATA::GetContext
//
// PURPOSE: 
//
//*****************************************************************************
ActorType ITEM_DATA::GetContext()
{
  ActorType data;
  data.Clear();
  data.SetItemSrc();
  data.Instance = this->key;
  return data;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetContext
//
// PURPOSE: 
//
//*****************************************************************************
ActorType ITEM_DATA_TYPE::GetContext(int index)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
  {
    return ItemData.GetAt(pos).GetContext();
  }
  else 
  {
    ActorType data;
    data.Clear();
    ASSERT(FALSE);
    return data;
  }
}

//*****************************************************************************
// NAME: ITEM_DATA::Clear
//
// PURPOSE:  
//*****************************************************************************
void ITEM_DATA::Clear(BOOL ctor)
{
  //if (event > 0)
  //  globalData.eventData.DeleteEvents(event);  
  //if (ExamineEvent > 0)
  //  globalData.eventData.DeleteEvents(ExamineEvent);
  key=0;
  event=0;
  ExamineEvent=0;
  ExamineLabel="EXAMINE";
  attackMsg="attacks";
  Name="";
  Id_Name="";
  HitSound = "";
  MissSound = "";
  LaunchSound = "";
  AmmoType="";
  ClearSounds();
  ClearArt();
  Experience = 0;
  Cost = 0;
  Encumbrance = 0;
  Attack_Bonus = 0;
  Cursed = FALSE;
  Bundle_Qty = 1;
  Num_Charges = 0;
  Location_Carried = WeaponHand;
  Hands_to_Use = 1;  
  Dmg_Dice_Sm = 6;
  Nbr_Dice_Sm = 1;
  Dmg_Bonus_Sm = 0;
  Dmg_Dice_Lg = 6;
  Nbr_Dice_Lg = 1;
  Dmg_Bonus_Lg = 0;
  ROF_Per_Round = 1.0;
  Protection_Base = 0;
  Protection_Bonus = 0;
  Wpn_Type = NotWeapon;
  Usable = FALSE;
  Usable_by_Class = FighterFlag;
  Recharge_Rate = irrNever;
  IsNonLethal=FALSE;
  CanBeHalvedJoined=TRUE;
  CanBeTradeDropSoldDep=TRUE;
  Range = 0;
  Spell = NO_SPELL;
  Spell_Level = 1; 
  Spell_Class = MagicUserFlag;
  item_asl.Clear();
}


//*****************************************************************************
// NAME: saveData
//
// PURPOSE:  
//*****************************************************************************
BOOL saveData(ITEM_DATA_TYPE& data)
{
  EditorStatusMsg("Saving item data...");

  char fullPath[_MAX_PATH+1];
  GetDesignPath(fullPath);
  strcat(fullPath, ITEM_DB_NAME);
  return saveData(data, fullPath);
}


//*****************************************************************************
// NAME: saveData
//
// PURPOSE:  
//*****************************************************************************
BOOL saveData(ITEM_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;
  BOOL success = TRUE;
    
  SetFileAttributes(fullPath, FILE_ATTRIBUTE_NORMAL);
  if (!myFile.Open(fullPath, CFile::modeCreate | CFile::modeWrite, &e))
  {
    WriteDebugString("Unable to open item db file %s, error %i\n", fullPath, e.m_cause);
    return FALSE;
  }   

  // put signal value and version at start of file
  __int64 hdr=0xFABCDEFABCDEFABF;
  double ver = PRODUCT_VER;
  data.version = ver;
  myFile.Write(&hdr, sizeof(hdr));
  myFile.Write(&ver, sizeof(double));

  CAR ar(&myFile, CArchive::store);

  try
  {
    data.PreSerialize(ar.IsStoring());
    data.Serialize(ar);
    data.PostSerialize(ar.IsStoring());
  }
  catch (...)
  {
    WriteDebugString("Failed to read item dat file\n");
    data.Clear();
    success = FALSE;
  }

  ar.Close();
  return success;
}


//*****************************************************************************
// NAME: loadData
//
// PURPOSE:  
//*****************************************************************************
int loadData(ITEM_DATA_TYPE& data, LPCSTR fullPath)
{
  CFileException e;
  CFile myFile;
  BOOL success=TRUE;

  if (!myFile.Open(fullPath, CFile::modeRead, &e))
  {
    WriteDebugString("Unable to open item db file %s, error %i\n", fullPath, e.m_cause);
    return 0;
  }   

  clearData(data);

  // check for version
  __int64 hdr=0;
  double ver;
  myFile.Read(&hdr, sizeof(hdr));
  if (hdr == 0xFABCDEFABCDEFABF)
  {
    myFile.Read(&ver, sizeof(double));
  }
  else
  {
    // special hdr not present, so reset archive to beginning
    myFile.SeekToBegin();
    // set version to last build that didn't save version
    ver = min(globalData.version, _VERSION_0696_);
  }

  data.version = ver;

  // if loading older level data, don't use compression archive
  if (ver < _VERSION_0697_)
  {
    WriteDebugString("Loading pre-0697 version item DB - %f\n", ver);

    CArchive ar(&myFile, CArchive::load);
    try
    {
       data.PreSerialize(ar.IsStoring());
       data.Serialize(ar);
       data.PostSerialize(ar.IsStoring());
    }
    catch (...)
    {
       WriteDebugString("Unable to load item data file %s\n", ar.GetFile()->GetFileName());
       success=FALSE;
    }
    ar.Close();
  }
  else
  {
    WriteDebugString("Loading item DB version: %f\n", ver);
    CAR ar(&myFile, CArchive::load);
    //ar.Compress(true); // qqqqq
    try
    {
       data.PreSerialize(ar.IsStoring());
       data.Serialize(ar);
       data.PostSerialize(ar.IsStoring());
    }
    catch (...)
    {
       WriteDebugString("Unable to load item data file %s\n", ar.GetFile()->GetFileName());
       success=FALSE;
    }
    ar.Close();
  }  
  
  if (data.GetCount() == 0)
     WriteDebugString("No items in item db file\n");

  return success; //data.GetCount();
}


//*****************************************************************************
// NAME: clearData
//
// PURPOSE:  
//*****************************************************************************
void clearData(ITEM_DATA_TYPE& data)
{
  data.Clear();
  data.version = PRODUCT_VER;
}


//*****************************************************************************
// NAME: halveItem
//
// PURPOSE:  
//*****************************************************************************
void ITEM_LIST::halveItem(int index)
{
  ITEM theItem;
  if (!GetItem(index, theItem))
    return;

  if (!itemCanBeHalved(theItem.item))
    return;  
 
  if (theItem.qty <= 1)
    return;

  int newQty = theItem.qty/2;
  
  // update original qty
  theItem.qty -= newQty;
  SetItem(index, theItem);

  // insert new item
  ITEM newItem = theItem; 
  newItem.qty = newQty;
  newItem.ready = FALSE;
  
  AddItem(newItem,FALSE); // don't auto join them back together!
}


//*****************************************************************************
// NAME: joinItems
//
// PURPOSE:  
//*****************************************************************************
void ITEM_LIST::joinItems(int index)
{
  ITEM origItem;
  if (!GetItem(index, origItem))
    return;

  if (!itemCanBeJoined(origItem.item))
    return;

  // look for another instance of item
  int joinIndex = -1;
  int joinQty=0;
  POSITION pos;
  pos = items.GetHeadPosition();

  CList<int, int> delList;
  while ((pos != NULL) && (joinIndex < 0))
  {
    if (   (items.GetAt(pos).item == origItem.item) 
        && (items.GetAt(pos).key != origItem.key))
    {
      //joinIndex = items.GetAt(pos).key;
      joinQty += items.GetAt(pos).qty;
      delList.AddTail(items.GetAt(pos).key);
    }
    items.GetNext(pos);
  }

  if (joinQty > 0)
  {
    ITEM joinItem;
    if (GetItem(index, origItem))
    {
      origItem.qty += joinQty;
      SetItem(index, origItem);
      pos=delList.GetHeadPosition();
      while (pos!=NULL)
        DeleteItem(delList.GetNext(pos));
    }
  }
}


//*****************************************************************************
// NAME: findItem
//
// PURPOSE:  
//*****************************************************************************
int ITEM_LIST::findItem(int item, int index)
{
  BOOL found = FALSE;
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) == NULL)
  {
    WriteDebugString("bogus index %i in findItem()\n", index);
    return -1;
  }

  if (item < 0)
  {
    WriteDebugString("bogus item index %i in findItem()\n", item);
    return -1;
  }

	while ((pos != NULL) && (!found)) 
  {
    if (items.GetAt(pos).item == item)
      found = TRUE;
    else
		  items.GetNext(pos);
  }

	if (found)
		return items.GetAt(pos).key;

	return -1;
}

//*****************************************************************************
// NAME: addItem
//
// PURPOSE:  Add a single item to the inventory (or single bundle qty)
//*****************************************************************************
BOOL ITEM_LIST::addItem(int item, int qty, int numCharges, BOOL id, int cost)
{
  if (qty < 1)
    return FALSE;

  ITEM newItem;

  if (ITEM_LIST::IsMoneyItem(item))
  {
    if (numCharges < 0)
      numCharges = 0;
    
	  newItem.item = item;
	  newItem.qty = qty;
	  newItem.ready = FALSE;
 	  newItem.charges = numCharges;
	  newItem.identified = id;
    newItem.cursed = FALSE;
  }
  else
  {
    ITEM_DATA *theItem = itemData.GetItemData(item);
    if (theItem == NULL)
      return FALSE;

    if ((theItem->Bundle_Qty <= 1) && (qty > 1))
    {
      WriteDebugString("qty > 1 && BundleQty <= 1 in addItem()\n");
      qty = 1;
    }

    if (numCharges < 0)
      numCharges = theItem->Num_Charges;
    
	  newItem.item = item;
	  newItem.qty = qty;
	  newItem.ready = FALSE;
 	  newItem.charges = numCharges;
	  newItem.identified = id;
    newItem.cursed = theItem->Cursed;
    newItem.paid = cost;
  }

  return AddItem(newItem,FALSE); // no auto-join
}

//*****************************************************************************
// NAME: deleteItem
//
// PURPOSE:  
//*****************************************************************************
BOOL ITEM_LIST::deleteItem(int index, int qty)
{
  ITEM theItem;

  if (!GetItem(index, theItem))
  {
    WriteDebugString("bogus item index in deleteItem()\n");
    return FALSE;
  }

  if (qty > theItem.qty)
  {
    WriteDebugString("bogus qty in deleteItem()\n");
    qty = theItem.qty;
  }

	theItem.qty -= qty;
	SetItem(index, theItem);

	if (theItem.qty < 1)
    DeleteItem(index);

  return TRUE;
}


//*****************************************************************************
// NAME: ITEM_DATA::operator=
//
// PURPOSE:  
//*****************************************************************************
ITEM_DATA& ITEM_DATA::operator=(const ITEM_DATA& src)
{
  if (&src == this) return *this;
  Clear();
  key = src.key;
  event = src.event;
  ExamineEvent = src.ExamineEvent;
  ExamineLabel = src.ExamineLabel;
  attackMsg=src.attackMsg;
  Name = src.Name;
  Id_Name = src.Id_Name;
  HitSound = src.HitSound;
  MissSound = src.MissSound;
  LaunchSound = src.LaunchSound;
  AmmoType=src.AmmoType;
  Experience = src.Experience;
  Cost = src.Cost;
  Encumbrance = src.Encumbrance;
  Attack_Bonus = src.Attack_Bonus;
  Cursed = src.Cursed;
  Bundle_Qty = src.Bundle_Qty;
  Num_Charges = src.Num_Charges;
  Location_Carried = src.Location_Carried;
  Hands_to_Use = src.Hands_to_Use;
  Recharge_Rate = src.Recharge_Rate;
  IsNonLethal=src.IsNonLethal;
  CanBeHalvedJoined=src.CanBeHalvedJoined;
  CanBeTradeDropSoldDep=src.CanBeTradeDropSoldDep;
  Dmg_Dice_Sm = src.Dmg_Dice_Sm;
  Nbr_Dice_Sm = src.Nbr_Dice_Sm;
  Dmg_Bonus_Sm = src.Dmg_Bonus_Sm;
  Dmg_Dice_Lg = src.Dmg_Dice_Lg;
  Nbr_Dice_Lg = src.Nbr_Dice_Lg;
  Dmg_Bonus_Lg = src.Dmg_Bonus_Lg;
  ROF_Per_Round = src.ROF_Per_Round;
  Protection_Base = src.Protection_Base;
  Protection_Bonus = src.Protection_Bonus;
  Wpn_Type = src.Wpn_Type;
  Usable = src.Usable;
  Usable_by_Class = src.Usable_by_Class;
  Range = src.Range;
  Spell = src.Spell;
  Spell_Level = src.Spell_Level;
  Spell_Class = src.Spell_Class;
  MissileArt = src.MissileArt;
  HitArt = src.HitArt;
  item_asl=src.item_asl;
  temp_asl=src.temp_asl;// Equal means equal
  hHitSound = src.hHitSound;
  hMissSound = src.hMissSound;
  hLaunchSound = src.hLaunchSound;
  specAbs = src.specAbs;
  return *this;
}

//*****************************************************************************
// NAME: ITEM_DATA::operator==
//
// PURPOSE:  
//*****************************************************************************
bool ITEM_DATA::operator==(const ITEM_DATA& src)
{
  if (&src == this) return true;
  if (key              != src.key             ) return false;
  if (Name             != src.Name            ) return false;
  if (Id_Name          != src.Id_Name         ) return false;
  if (HitSound         != src.HitSound        ) return false;
  if (MissSound        != src.MissSound       ) return false;
  if (LaunchSound      != src.LaunchSound     ) return false;
  if (AmmoType         != src.AmmoType        ) return false;
  if (Experience       != src.Experience      ) return false;
  if (Cost             != src.Cost            ) return false;
  if (Encumbrance      != src.Encumbrance     ) return false;
  if (Attack_Bonus     != src.Attack_Bonus    ) return false;
  if (Cursed           != src.Cursed          ) return false;
  if (Bundle_Qty       != src.Bundle_Qty      ) return false;
  if (Num_Charges      != src.Num_Charges     ) return false;
  if (Location_Carried != src.Location_Carried) return false;
  if (Hands_to_Use     != src.Hands_to_Use    ) return false;
  if (Recharge_Rate    != src.Recharge_Rate   ) return false;
  if (IsNonLethal      != src.IsNonLethal     ) return false;
  if (CanBeHalvedJoined!= src.CanBeHalvedJoined)return false;
  if (CanBeTradeDropSoldDep!=src.CanBeTradeDropSoldDep)return false;
  if (Dmg_Dice_Sm      != src.Dmg_Dice_Sm     ) return false;
  if (Nbr_Dice_Sm      != src.Nbr_Dice_Sm     ) return false;
  if (Dmg_Bonus_Sm     != src.Dmg_Bonus_Sm    ) return false;
  if (Dmg_Dice_Lg      != src.Dmg_Dice_Lg     ) return false;
  if (Nbr_Dice_Lg      != src.Nbr_Dice_Lg     ) return false;
  if (Dmg_Bonus_Lg     != src.Dmg_Bonus_Lg    ) return false;
  if (ROF_Per_Round    != src.ROF_Per_Round   ) return false;
  if (Protection_Base  != src.Protection_Base ) return false;
  if (Protection_Bonus != src.Protection_Bonus) return false;
  if (Wpn_Type         != src.Wpn_Type        ) return false;
  if (Usable           != src.Usable          ) return false;
  if (Usable_by_Class  != src.Usable_by_Class ) return false;
  if (Range            != src.Range           ) return false;
  if (Spell            != src.Spell           ) return false;
  if (Spell_Level      != src.Spell_Level     ) return false;
  if (Spell_Class      != src.Spell_Class     ) return false;
  if (!(MissileArt     == src.MissileArt)     ) return false;
  if (!(HitArt         == src.HitArt)         ) return false;
  if (!(item_asl==src.item_asl) ) return false;
  if (!(temp_asl==src.temp_asl) ) return false;
  if (hHitSound        != src.hHitSound       ) return false;
  if (hMissSound       != src.hMissSound      ) return false;
  if (hLaunchSound     != src.hLaunchSound    ) return false;
  if (!(specAbs        == src.specAbs)        ) return false;
  if (event            != src.event           ) return false;        
  if (ExamineEvent     != src.ExamineEvent    ) return false;        
  if (ExamineLabel     != src.ExamineLabel    ) return false;        
  if (attackMsg        != src.attackMsg       ) return false;
  return true;
}


//*****************************************************************************
// NAME: ITEM_DATA_TYPE::SaveItemSounds
//
// PURPOSE:  
//*****************************************************************************
void saveSounds(ITEM_DATA_TYPE& data)
{
  data.SaveSounds();
}

void ITEM_DATA_TYPE::SaveSounds()
{
  POSITION p = ItemData.GetHeadPosition();
  while (p != NULL)
  {
    saveSound(ItemData.GetAt(p).HitSound, GLOBAL_SOUND);
    saveSound(ItemData.GetAt(p).MissSound, GLOBAL_SOUND);
    saveSound(ItemData.GetAt(p).LaunchSound, GLOBAL_SOUND);
    ItemData.GetNext(p);
  }
}

//*****************************************************************************
//    NAME: saveArt
//
// PURPOSE: 
//
//*****************************************************************************
void saveArt(ITEM_DATA_TYPE& data)
{
  data.SaveArt();
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::SaveArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::SaveArt()
{
  POSITION p = ItemData.GetHeadPosition();
  while (p != NULL)
  {
    ItemData.GetAt(p).MissileArt.SavePicData(GLOBAL_ART);
    ItemData.GetAt(p).HitArt.SavePicData(GLOBAL_ART);
    ItemData.GetNext(p);
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PreSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::PreSerialize(BOOL IsStoring)
{
  // before saving, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (IsStoring) ExpandDefaultFilenames();

  POSITION p = ItemData.GetHeadPosition();
  while (p!=NULL) 
    ItemData.GetNext(p).PreSerialize(IsStoring);
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PostSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::PostSerialize(BOOL IsStoring)
{
  // after loading, make sure data that is set to "DEFAULT"
  // is converted to proper filename
  if (!IsStoring) ExpandDefaultFilenames();

  POSITION p = ItemData.GetHeadPosition();
  while (p!=NULL) 
  {
    ItemData.GetAt(p).PostSerialize(IsStoring);
    
    if ((!IsStoring)&&(version<=_VERSION_0840_))
    {
      ItemData.GetAt(p).MissileArt.SetPre0840NonLoopDefaults();
      ItemData.GetAt(p).HitArt.SetPre0840NonLoopDefaults();
    }

    ItemData.GetNext(p);
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Save
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::Save(CArchive& ar) 
{
	int count;
	POSITION p;
	if (version < _ASL_LEVEL_ ) return;

	count=ItemData.GetCount();
	ar << count;
	p=ItemData.GetHeadPosition();
	while (p!=NULL) 
	{
		ar << ItemData.GetAt(p).Name;
		ItemData.GetAt(p).Save(ar);
		ItemData.GetNext(p);
	};
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Restore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::Restore(CArchive& ar) 
{
	int count;
	CString name;
	POSITION p;
	if (version < _ASL_LEVEL_) return;
	p=ItemData.GetHeadPosition();
	// Let items know they will be asked to CommitRestore.  They
	// can make a copy of their data or otherwise note that 
	// if no Restore comes along they do something special.
	// Probably will want to clear their temporary copy of
	// ATTRIBUTES in case no restore operation clears it.
  while (p!=NULL) ItemData.GetNext(p).PrepareForRestore();
	ar >> count;
	for (int i=0; i<count; i++) {
		ar >> name;
		p=ItemData.GetHeadPosition();
		while (p!=NULL) {
			if (name=ItemData.GetAt(p).Name)
			{
				ItemData.GetAt(p).Restore(ar);
				break;
			};
			ItemData.GetNext(p);
		};
		if (p==NULL) 
		{	
    	ITEM_DATA junk;
			CString msg;
			msg.Format("SAVE-FILE ITEM_DATA \"%s\" does not exist in design\n",
				name);
			WriteDebugString(msg);
			junk.Restore(ar); // Discard saved data.  Any better ideas?
		};
	};
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::CommitRestore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::CommitRestore(void) {
	POSITION p;
	p=ItemData.GetHeadPosition();
	while (p!=NULL) ItemData.GetNext(p).CommitRestore();
}

//*****************************************************************************
//    NAME: ITEM_DATA::Save
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::Save(CArchive& ar) 
{
	item_asl.Save(ar);
}

//*****************************************************************************
//    NAME: ITEM_DATA::PrepareForRestore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PrepareForRestore(void) 
{
	// In case no Restore comes along then we will simply use what we have.
	temp_asl.Clear();
}

//*****************************************************************************
//    NAME: ITEM_DATA::PreSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PreSerialize(BOOL IsStoring) 
{
  if (IsStoring)
  {
    // move ITEM_DATA fields that need to be serialized
    // as attributes into the attribute map  

    StripFilenamePath(HitSound);
    StripFilenamePath(MissSound);
    StripFilenamePath(LaunchSound);
    MissileArt.PreSerialize(IsStoring);
    HitArt.PreSerialize(IsStoring);
    // MissileArt
    StorePicDataAsASL(item_asl, MissileArt, "MissArt");
    
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA::PostSerialize
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PostSerialize(BOOL IsStoring) 
{
  if (!IsStoring)
  {
    // retrieve ITEM_DATA fields that are serialized as
    // attributes from the attribute map

    // MissileArt
    RetrievePicDataFromASL(item_asl, MissileArt, "MissArt");
  }
  
  AddDesignFolderToPath(HitSound);
  AddDesignFolderToPath(MissSound);
  AddDesignFolderToPath(LaunchSound);
  MissileArt.PostSerialize(IsStoring);
  HitArt.PostSerialize(IsStoring);
}

//*****************************************************************************
//    NAME: ITEM_DATA::Restore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::Restore(CArchive& ar) 
{
	temp_asl.Serialize(ar);
}

//*****************************************************************************
//    NAME: ITEM_DATA::CommitRestore
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::CommitRestore(void) 
{
	item_asl.CommitRestore(temp_asl);
	temp_asl.Clear();
}


//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::ExpandDefaultFilenames
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::ExpandDefaultFilenames()
{  
  POSITION p = GetHeadPosition();
  int key;

  while (p != NULL)
  {
    ITEM_DATA &data = GetAt(p);
    
    if (data.HitSound.CompareNoCase("DEFAULT") == 0)
    {
#ifdef UAFEDITOR
      data.HitSound.Format("%s%s",EditorArt, DEF_HIT_SOUND);
#else
      data.HitSound = DEF_HIT_SOUND;
#endif
    }
    if (data.MissSound.CompareNoCase("DEFAULT") == 0)
    {
#ifdef UAFEDITOR
      data.MissSound.Format("%s%s",EditorArt, DEF_MISS_SOUND);
#else
      data.MissSound = DEF_MISS_SOUND;
#endif
    }
    if (data.LaunchSound.CompareNoCase("DEFAULT") == 0)
    {
#ifdef UAFEDITOR
      data.LaunchSound.Format("%s%s",EditorArt, DEF_LAUNCH_SOUND);
#else
      data.LaunchSound = DEF_LAUNCH_SOUND;
#endif
    }

    if (data.MissileArt.filename.CompareNoCase("DEFAULT")==0)
    {
      switch (data.Wpn_Type)
      {
      case HandThrow:
      case Throw:
#ifdef UAFEDITOR
		    data.MissileArt.filename.Format("%s%s",EditorArt, DEFAULT_IIRT);
#else
        data.MissileArt.filename = DEFAULT_IIRT;
#endif
        data.MissileArt.style = PIC_DATA::AS_None;
        data.MissileArt.NumFrames = 2;
        break;
      case SlingNoAmmo:
#ifdef UAFEDITOR
		    data.MissileArt.filename.Format("%s%s",EditorArt, DEFAULT_IIRS);
#else
        data.MissileArt.filename = DEFAULT_IIRS;
#endif
        data.MissileArt.style = PIC_DATA::AS_None;
        data.MissileArt.NumFrames = 2;
        break;
      case Ammo:
#ifdef UAFEDITOR
		    data.MissileArt.filename.Format("%s%s",EditorArt, DEFAULT_IIRA);
#else
        data.MissileArt.filename = DEFAULT_IIRA;
#endif
        data.MissileArt.style = PIC_DATA::AS_Directional;
        data.MissileArt.NumFrames = 8;
        break;
      default:
        data.MissileArt.Clear();
        break;
      }

      data.MissileArt.picType = SpriteDib;
      data.MissileArt.FrameHeight = COMBAT_TILE_HEIGHT;
      data.MissileArt.timeDelay = 0;
      data.MissileArt.FrameWidth = COMBAT_TILE_WIDTH;      
      data.MissileArt.MaxLoops=1;
      data.MissileArt.flags=6;
    }

    switch (data.Wpn_Type)
    {
    case HandThrow:
    case Throw:
    case SlingNoAmmo:
      break;
    case Ammo:
      if (data.AmmoType.IsEmpty())
      {
        if (strstr(data.Name, "Bolt") != NULL)
          data.AmmoType="CrossBow";
        else
          data.AmmoType="Bow";
      }
      AddAmmoType(data.AmmoType);
      break;
    case Bow:
      if (data.AmmoType.IsEmpty())
      {
        data.AmmoType="Bow";
        AddAmmoType(data.AmmoType);
      }
      break;
    case Crossbow:
      if (data.AmmoType.IsEmpty())
      {
        data.AmmoType="CrossBow";
        AddAmmoType(data.AmmoType);
      }
      break;

    default:
      break;
    }
    key = GetKeyAt(p);
    SetItem(key, data);
    GetNext(p);
  }
}

#ifdef UAFEDITOR
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::CollapseDefaultFilenames
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::CollapseDefaultFilenames()
{  
  int key;
  POSITION p = GetHeadPosition();
  CString DefFilename;

  while (p != NULL)
  {
    ITEM_DATA &data = GetAt(p);

    DefFilename.Format("%s%s",EditorArt, DEF_HIT_SOUND);
    if (data.HitSound.CompareNoCase(DefFilename)==0)
      data.HitSound = "DEFAULT";
    DefFilename.Format("%s%s",EditorArt, DEF_MISS_SOUND);
    if (data.MissSound.CompareNoCase(DefFilename)==0)
      data.MissSound = "DEFAULT";
    DefFilename.Format("%s%s",EditorArt, DEF_LAUNCH_SOUND);
    if (data.LaunchSound.CompareNoCase(DefFilename)==0)
      data.LaunchSound = "DEFAULT";

    switch (data.Wpn_Type)
    {
    case HandThrow:
    case Throw:
		  DefFilename.Format("%s%s",EditorArt, DEFAULT_IIRT);
      break;
    case SlingNoAmmo:
		  DefFilename.Format("%s%s",EditorArt, DEFAULT_IIRS);
      break;
    case Ammo:
		  DefFilename.Format("%s%s",EditorArt, DEFAULT_IIRA);
      break;
    default: break;
    }

    if (data.MissileArt.filename.CompareNoCase(DefFilename)==0)
      data.MissileArt.filename = "DEFAULT";

    key = GetKeyAt(p);
    SetItem(key, data);

    GetNext(p);
  }
}

#endif // UAFEDITOR

//*****************************************************************************
//    NAME: ITEM_LIST::GetNextKey
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetNextKey()
{
  int key=0;
  POSITION pos;
  if (GetCount() > 0)
  {
    key = items.GetTail().key;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_ITEMS.
      // Gaps occur when deletions are made to item list
      pos = items.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = items.GetNext(pos).key;
        if (currKey != prevKey+1)
        {
          key = prevKey+1;
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key++;
  }
  else
    key = 1;
  return key;
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetReady
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetReady(int index, BOOL ready)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    items.GetAt(pos).ready = ready;
}


//*****************************************************************************
//    NAME: ITEM_LIST::ValidItemListIndex
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::ValidItemListIndex(int index)
{ 
  return (items.FindKeyPos(index) != NULL); 
}

// called when loading from scripts
//*****************************************************************************
//    NAME: ITEM_LIST::AddItem
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::AddItem(ITEM &data, BOOL AutoJoin)
{
  int newkey = 0;
  BOOL joined = FALSE;
  if (AutoJoin)
  {
    if (itemCanBeJoined(data.item))
    {
      // look for another instance of item
      int joinIndex = -1;
      POSITION pos = GetHeadPosition();
      while ((pos != NULL) && (joinIndex < 0))
      {
        if (   (GetAt(pos).item == data.item)
            /*&& (GetAt(pos).key != origItem.key)*/ ) // not needed here
          joinIndex = GetAt(pos).key;
        GetNext(pos);
      }

      // if item instance in list
      if (joinIndex >= 0)
      {
        ITEM joinItem;
        if (GetItem(joinIndex, joinItem)) 
        {
          joinItem.qty += data.qty;
          SetItem(joinIndex, joinItem);
          joined=TRUE;
          newkey=joinIndex;
        }
      }
    }
  }

  if (!joined)
  {
    if (items.GetCount() < MAX_ITEMS)
    {
      data.key = GetNextKey();
      items.Insert(data, data.key);
      newkey = data.key;
    }
  }
  return newkey;
}

// called when loading binary data that
// was saved with key info
//*****************************************************************************
//    NAME: ITEM_LIST::AddItemWithCurrKey
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::AddItemWithCurrKey(const ITEM &data)
{
  if (items.GetCount() < MAX_ITEMS)
  {
    items.Insert(data, data.key);
    return data.key;
  }
  else
    return 0;
}

//*****************************************************************************
//    NAME: ITEM_LIST::DeleteItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::DeleteItem(int index)
{
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    if (UnReady(index))
    {
      items.RemoveAt(pos);
      return TRUE;
    }
  }
  return FALSE;
}  

//*****************************************************************************
//    NAME: ITEM_LIST::GetItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::GetItem(int index, ITEM &data)
{
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    data = items.GetAt(pos);
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::SetItem(int index, ITEM &data)
{
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    items.GetAt(pos) = data;
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::operator=
//
// PURPOSE: 
//
//*****************************************************************************
ITEM_LIST& ITEM_LIST::operator=(const ITEM_LIST& src)
{
  if (&src == this) return *this;
  Clear();
  POSITION pos;
  pos = const_cast<ITEM_LIST&>(src).GetHeadPosition();
  while (pos != NULL)
    AddItemWithCurrKey(const_cast<ITEM_LIST&>(src).GetNext(pos));
  rdyItems = src.rdyItems;
  return *this;
}

//*****************************************************************************
//    NAME: ITEM_LIST::operator==
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::operator==(const ITEM_LIST& src) const
{
  if (&src == this) return TRUE;
  POSITION p1=GetHeadPosition();
  POSITION p2=const_cast<ITEM_LIST&>(src).GetHeadPosition();
  while (p1!=NULL)
  {
    if (p2==NULL) return FALSE;
    ITEM item1 = const_cast<ITEM_LIST&>(*this).GetNext(p1);
    ITEM item2 = const_cast<ITEM_LIST&>(src).GetNext(p2);
    if (!(item1 == item2)) return FALSE;
  }
  if (p2!=NULL) return FALSE;
  return TRUE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetReady
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetReady(itemLocationType type)
{ 
  return rdyItems.GetReady(type); 
}

//*****************************************************************************
//    NAME: ITEM_LIST::IsReady
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::IsReady(int index)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).ready; 
  else 
    return FALSE; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::IsCursed
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::IsCursed(int index)
{
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).cursed;
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::RemoveCurse
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::RemoveCurse(int index)
{
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    items.GetAt(pos).cursed = FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::HaveItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::HaveItem(int item)
{
  POSITION pos;
  pos = GetHeadPosition();
  while (pos != NULL)
  {
    if (items.GetNext(pos).item == item)
      return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetListKeyByItemKey
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetListKeyByItemKey(int item)
{
  POSITION pos;
  pos = GetHeadPosition();
  while (pos != NULL)
  {
    if (items.GetAt(pos).item == item)
      return items.GetAt(pos).key;
    items.GetNext(pos);
  }
  return -1;
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetItem
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetItem(int index)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).item; 
  else 
    return NO_ITEM; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetId
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_LIST::GetId(int index)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).identified; 
  else 
    return FALSE; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetQty
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetQty(int index)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).qty; 
  else 
    return 0; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetPurchasePrice
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetPurchasePrice(int index)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).paid; 
  else 
    return -1; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetPurchasePrice
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetPurchasePrice(int index, int price)
{
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    items.GetAt(pos).paid = price; 
}

//*****************************************************************************
//    NAME: ITEM_LIST::GetCharges
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_LIST::GetCharges(int index)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    return items.GetAt(pos).charges; 
  else 
    return 0; 
}
//*****************************************************************************
//    NAME: ITEM_LIST::GetAmmoClass
//
// PURPOSE: 
//
//*****************************************************************************
CString ITEM_LIST::GetAmmoClass(int index)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    CString tmp(itemData.GetAmmoClass(items.GetAt(pos).item));
    return tmp;
  }
  else 
    return ""; 
}
//*****************************************************************************
//    NAME: ITEM_LIST::SetId
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetId(int index, BOOL id)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    items.GetAt(pos).identified = id;
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetQty
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetQty(int index, int qty)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    items.GetAt(pos).qty = qty;
    if (items.GetAt(pos).qty <= 0)
    {
      if (UnReady(index))
        DeleteItem(index);
    }
  }
}

//*****************************************************************************
//    NAME: ITEM_LIST::AdjustQty
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::AdjustQty(int index, int qty)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
  {
    items.GetAt(pos).qty += qty;
    if (items.GetAt(pos).qty <= 0)
    {
      if (UnReady(index))
        DeleteItem(index);
    }
  }
}

//*****************************************************************************
//    NAME: ITEM_LIST::ResetItemCharges
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::ResetItemCharges(BOOL IsNewDay)
{
  ITEM_DATA *pIdata;
  POSITION ipos = GetHeadPosition();
  while (ipos != NULL)
  {
    pIdata = itemData.GetItemData(GetAt(ipos).item);
    if (pIdata != NULL)
    {
      switch (pIdata->Recharge_Rate)
      {
      case irrNever:
        // no adjustments needed
        break;
      case irrDaily:
        // reset available charges
        if (IsNewDay)
          GetAt(ipos).charges = pIdata->Num_Charges;
        break;
      }
    }
    GetNext(ipos);
  }
}

//*****************************************************************************
//    NAME: ITEM_LIST::SetCharges
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_LIST::SetCharges(int index, int chrgs)
{ 
  POSITION pos;
  if ((pos = items.FindKeyPos(index)) != NULL)
    items.GetAt(pos).charges = chrgs; 
}

//*****************************************************************************
//    NAME: ITEM_DATA::GetItemName
//
// PURPOSE: 
//
//*****************************************************************************
CString ITEM_DATA::GetItemName() 
{ 
  if (!Id_Name.IsEmpty()) 
    return Id_Name; 
  else 
    return Name; 
}

//*****************************************************************************
//    NAME: ITEM_DATA::PlayHit
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PlayHit()
{
  if (!PlaySoundEffects) return;
  if (HitSound == "") return;
  if (!ValidSound(hHitSound))
    hHitSound = LoadSound(HitSound);
  if (!PlaySound(hHitSound,FALSE))
    hHitSound=-1;
}
//*****************************************************************************
//    NAME: ITEM_DATA::PlayMiss
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PlayMiss()
{
  if (!PlaySoundEffects) return;
  if (MissSound == "") return;
  if (!ValidSound(hMissSound))
    hMissSound = LoadSound(MissSound);
  if (!PlaySound(hMissSound,FALSE))
    hMissSound=-1;
}
//*****************************************************************************
//    NAME: ITEM_DATA::PlayLaunch
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::PlayLaunch()
{
  if (!PlaySoundEffects) return;
  if (LaunchSound == "") return;
  if (!ValidSound(hLaunchSound))
    hLaunchSound = LoadSound(LaunchSound);
  if (!PlaySound(hLaunchSound,FALSE))
    hLaunchSound=-1;
}

//*****************************************************************************
//    NAME: ITEM_DATA::ClearSounds
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::ClearSounds()
{
  if (hHitSound >= 0)
    ClearSound(hHitSound);
  if (hMissSound >= 0)
    ClearSound(hMissSound);
  if (hLaunchSound >= 0)
    ClearSound(hLaunchSound);
  hHitSound = -1;
  hMissSound = -1;
  hLaunchSound = -1;
}
//*****************************************************************************
//    NAME: ITEM_DATA::LoadArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::LoadArt()
{
  MissileArt.LoadPicSurfaces("it_");
  HitArt.LoadPicSurfaces("it_");
}
//*****************************************************************************
//    NAME: ITEM_DATA::ClearArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::ClearArt() // release surface and clear pic data
{
  MissileArt.Clear();
  HitArt.Clear();
}
//*****************************************************************************
//    NAME: ITEM_DATA::FreeArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::FreeArt() // release surface
{
  MissileArt.FreePic();
  HitArt.FreePic();
}
//*****************************************************************************
//    NAME: ITEM_DATA::SetAttribute
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::SetASL(CString akey, CString value, BYTE flags)
{
  item_asl.Insert(akey, value, flags);
}

//*****************************************************************************
//    NAME: ITEM_DATA::GetAttribute
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA::GetASL(CString akey, CString &value)
{
  if (item_asl.Exists(akey))
  {
    value = item_asl.Lookup(akey);
    return TRUE;
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetNextKey
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_DATA_TYPE::GetNextKey()
{
  int key=0;
  POSITION pos;
  if (GetCount() > 0)
  {
    key = ItemData.GetTail().key;
    if (key >= INT_MAX)
    {
      // Need to wrap key.
      // Search for non-sequential gap between current keys.
      // A gap is guaranteed since the key range is greater than
      // MAX_ITEMS.
      // Gaps occur when deletions are made to item list
      pos = ItemData.GetHeadPosition();
      BOOL found = FALSE;        
      int prevKey = 0;
      int currKey;
      while ((pos != NULL) && (!found))
      {
        currKey = ItemData.GetNext(pos).key;
        if (currKey != prevKey+1)
        {
          key = prevKey+1;
          found = TRUE;
        }
        else
          prevKey = currKey; 
      }
      VERIFY(found); // should never happen
    }
    else
      key++;
  }
  else
    key = 1;
  return key;
}
#ifdef UAFEDITOR
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Validate
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::Validate(ITEM_DATA &data, FILE *pFile)
{
  BOOL success = TRUE;
  if (data.Wpn_Type != NotWeapon)
  {
    if (data.HitSound != "")
    {
      if (!FileExists(data.HitSound))
      {
        WriteValidateLog(pFile, "Item '%s' missing Hit Sound file '%s'\n",
                         data.Name, 
                         data.HitSound);
        success = FALSE;
      }
    }

    if (data.MissSound != "")
    {
      if (!FileExists(data.MissSound))
      {
        WriteValidateLog(pFile, "Item '%s' missing Miss Sound file '%s'\n",
                         data.Name, 
                         data.MissSound);
        success = FALSE;
      }
    }
    if (   (data.Wpn_Type == SlingNoAmmo)
        || (data.Wpn_Type == Bow)
        || (data.Wpn_Type == Crossbow)
        || (data.Wpn_Type == Throw))
    {
      if (data.LaunchSound != "")
      {
        if (!FileExists(data.LaunchSound))
        {
          WriteValidateLog(pFile, "Item '%s' missing Launch Sound file '%s'\n",
                           data.Name, 
                           data.LaunchSound);
          success = FALSE;
        }
      }
    }
  }

  POSITION pos = data.specAbs.GetHeadPosition();
  while (pos!=NULL)
  {
    SPEC_AB_DATA *spab = data.specAbs.GetNextData(pos);
    if (spab!=NULL)
    {
      if (!spab->script.IsEmpty())
      {
        if (spab->binary.GetLength() == 0)
        {
          success = FALSE;
          WriteValidateLog(pFile, "Item '%s' special ability script '%s' did not compile\n",
                           data.Name, spab->script);
        }
      }
    }
  }
  return success;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Validate
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::Validate(FILE *pFile)
{
  BOOL success = TRUE;
  POSITION pos = ItemData.GetHeadPosition();
  while (pos != NULL)
  {
    if (!Validate(ItemData.GetNext(pos),pFile))
      success = FALSE;
  }
  return success;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::Validate
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::Validate(int index, FILE *pFile)
{
  BOOL success = TRUE;
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
  {
    if (!Validate(ItemData.GetAt(pos),pFile))
      success = FALSE;
  }
  else
  {
    WriteValidateLog(pFile, "Bogus item key %i\n", index);
    success = FALSE;
  }
  return success;
}
#endif
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PlayHit
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::PlayHit(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    ItemData.GetAt(pos).PlayHit();
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PlayMiss
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::PlayMiss(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    ItemData.GetAt(pos).PlayMiss();
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::PlayLaunch
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::PlayLaunch(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    ItemData.GetAt(pos).PlayLaunch();
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::ClearSounds
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::ClearSounds()
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  pos = ItemData.GetHeadPosition();
  while (pos != NULL)
    ItemData.GetNext(pos).ClearSounds();
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::FreeArt
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::FreeArt()
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  pos = ItemData.GetHeadPosition();
  while (pos != NULL)
    ItemData.GetNext(pos).FreeArt();
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemName
//
// PURPOSE: 
//
//*****************************************************************************
CString ITEM_DATA_TYPE::GetItemName(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
  {
    return ItemData.GetAt(pos).Name;
  }
  else
    return CString("No Item");
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemIdName
//
// PURPOSE: 
//
//*****************************************************************************
CString ITEM_DATA_TYPE::GetItemIdName(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return ItemData.GetAt(pos).GetItemName();
  else
    return CString("No Item");
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemCost
//
// PURPOSE: 
//
//*****************************************************************************
long ITEM_DATA_TYPE::GetItemCost(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return ItemData.GetAt(pos).Cost;
  else
    return 0;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemExpWorth
//
// PURPOSE: 
//
//*****************************************************************************
long ITEM_DATA_TYPE::GetItemExpWorth(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
  {
    long val = ItemData.GetAt(pos).Experience;
    double mod = GetAllExpMod();
    if (mod != 0.0) val += ((mod/100.0)*val);
    if (val < 0) val=0;
    return val;
  }
  else
    return 0;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemROF
//
// PURPOSE: 
//
//*****************************************************************************
double ITEM_DATA_TYPE::GetItemROF(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return ItemData.GetAt(pos).ROF_Per_Round;
  else
    return 0.0;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemRange
//
// PURPOSE: 
//
//*****************************************************************************
long ITEM_DATA_TYPE::GetItemRange(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return ItemData.GetAt(pos).Range;
  else
    return 0;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemBundleQty
//
// PURPOSE: 
//
//*****************************************************************************
long ITEM_DATA_TYPE::GetItemBundleQty(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return ItemData.GetAt(pos).Bundle_Qty;
  else
    return 0;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemUsable
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::GetItemUsable(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return ItemData.GetAt(pos).Usable;
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetAmmoClass
//
// PURPOSE: 
//
//*****************************************************************************
CString ITEM_DATA_TYPE::GetAmmoClass(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return ItemData.GetAt(pos).AmmoType;
  else
    return "";
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetWpnType
//
// PURPOSE: 
//
//*****************************************************************************
weaponClassType ITEM_DATA_TYPE::GetWpnType(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return ItemData.GetAt(pos).Wpn_Type;
  else
    return NotWeapon;
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::HaveItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::HaveItem(int index) 
{
  CSingleLock sLock(&m_CS,TRUE);
  return (ItemData.FindKeyPos(index) != NULL);
}

// called when loading from scripts, or
// when adding from editor GUI
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::AddItem
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_DATA_TYPE::AddItem(ITEM_DATA &data)
{
  CSingleLock sLock(&m_CS,TRUE);
  if (strlen(data.Name) == 0) return 0;
  // Do not allow multiple items with same full name.
  if (m_FindName(data.Name)!=0) return 0;
  if (ItemData.GetCount() < MAX_ITEMS)
  {
    data.key = GetNextKey();
    ItemData.Insert(data, data.key);
    AddAmmoType(data.AmmoType);
    return data.key;
  }
  else
    return 0;
}

// called when loading binary data that
// was saved with key info
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::AddItemWithCurrKey
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_DATA_TYPE::AddItemWithCurrKey(ITEM_DATA &data)
{
  CSingleLock sLock(&m_CS,TRUE);
  if (ItemData.GetCount() < MAX_ITEMS)
  {
    ItemData.Insert(data, data.key);
    AddAmmoType(data.AmmoType);
    return data.key;
  }
  else
    return 0;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::SetItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::SetItem(int index, ITEM_DATA &data)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
  {
    CString oldAmmoType(ItemData.GetAt(pos).AmmoType);
    ItemData.GetAt(pos) = data;

    if (data.AmmoType != oldAmmoType)
    {
      RemoveAmmoType(oldAmmoType);
      AddAmmoType(data.AmmoType);
    }
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::DeleteItemEvents
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::DeleteItemEvents(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
  {
    globalData.eventData.DeleteEvents(ItemData.GetAt(pos).event);
    globalData.eventData.DeleteEvents(ItemData.GetAt(pos).ExamineEvent);
    ItemData.GetAt(pos).ExamineLabel="EXAMINE";
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::RemoveItem
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::RemoveItem(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
  {
    //globalData.eventData.DeleteEvents(ItemData.GetAt(pos).event);
    //globalData.eventData.DeleteEvents(ItemData.GetAt(pos).ExamineEvent);
    //ItemData.GetAt(pos).ExamineLabel="EXAMINE";
    CString oldAmmoType(ItemData.GetAt(pos).AmmoType);
    ItemData.RemoveAt(pos);
    RemoveAmmoType(oldAmmoType);
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::HaveItemThatUsesAmmoType
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::HaveItemThatUsesAmmoType(CString data)
{
  data.TrimLeft();
  data.TrimRight();
  if (data.IsEmpty()) return FALSE;
  POSITION ipos = ItemData.GetHeadPosition();
  while (ipos!=NULL)
  {
    if (ItemData.GetAt(ipos).AmmoType == data)
      return TRUE;
    ItemData.GetNext(ipos);
  }
  return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::AddAmmoType
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::AddAmmoType(CString data)
{
  data.TrimLeft();
  data.TrimRight();
  if (data.IsEmpty()) return;
  if (HaveAmmoType(data)==-1)
    m_AmmoTypes.SetAtGrow(m_AmmoTypes.GetSize(), data);
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::RemoveAmmoType
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::RemoveAmmoType(CString data)
{
  int index = HaveAmmoType(data);
  if (index >= 0)
  {
    if (!HaveItemThatUsesAmmoType(data))
      m_AmmoTypes.RemoveAt(index);
  }
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::HaveAmmoType
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_DATA_TYPE::HaveAmmoType(CString data)
{
  if (data.IsEmpty()) return -1;
  for (int i=0;i<m_AmmoTypes.GetSize();i++)
  {
    if (m_AmmoTypes[i]==data)
      return i;
  }
  return -1;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::GetItemAttribute
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::GetItemAttribute(int index, CString akey, CString value)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return (ItemData.GetAt(pos).GetASL(akey, value));
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::SetItemAttribute
//
// PURPOSE: 
//
//*****************************************************************************
BOOL ITEM_DATA_TYPE::SetItemAttribute(int index, CString akey, CString value)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
  {
    ItemData.GetAt(pos).SetASL(akey, value, ASLF_READONLY);
    return TRUE;
  }
  else
    return FALSE;
}

//*****************************************************************************
//    NAME: *ITEM_DATA_TYPE::GetItemData
//
// PURPOSE: 
//
//*****************************************************************************
ITEM_DATA *ITEM_DATA_TYPE::GetItemData(int index)
{
  CSingleLock sLock(&m_CS,TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    return &(ItemData.GetAt(pos));
  else
    return NULL;
}

// No locking... for internal use
//*****************************************************************************
//    NAME: 
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_DATA_TYPE::m_FindName (const CString& name)
// Return key associated with the named object, or zero
{
  POSITION pos;
  pos=ItemData.GetHeadPosition(); 
  while (pos!=NULL)
  {
    if (ItemData.GetAt(pos).Name==name) 
          return ItemData.GetAt(pos).key;
    ItemData.GetNext(pos);
  };
  return 0;
}
// With locking...for public use
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::FindName
//
// PURPOSE: 
//
//*****************************************************************************
int ITEM_DATA_TYPE::FindName(const CString& name)
// Return key associated with the named object, or zero
{
  CSingleLock sLock(&m_CS,TRUE);
  return m_FindName(name);
}


#ifdef UAFEngine

//*****************************************************************************
//    NAME: ITEM_DATA::CastSpell
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::CastSpell(int spell)
{
  // nothing yet
  ASSERT(FALSE);
}
//*****************************************************************************
//    NAME: ITEM_DATA::SpellActivate
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA::SpellActivate(PENDING_SPELL &data)
{
  // nothing yet
  ASSERT(FALSE);
}


//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::CastSpell
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::CastSpell(int index, int spell)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    ItemData.GetAt(pos).CastSpell(spell);
}
//*****************************************************************************
//    NAME: ITEM_DATA_TYPE::SpellActivate
//
// PURPOSE: 
//
//*****************************************************************************
void ITEM_DATA_TYPE::SpellActivate(int index, PENDING_SPELL &data)
{
  CSingleLock sLock(&m_CS, TRUE);
  POSITION pos;
  if ((pos = ItemData.FindKeyPos(index)) != NULL)
    ItemData.GetAt(pos).SpellActivate(data);
}

#endif // UAFEngine spell code