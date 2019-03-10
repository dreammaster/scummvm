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

#ifndef DUNGEONCRAFT_SHARED_ITEMS
#define DUNGEONCRAFT_SHARED_ITEMS

//#include "dungeoncraft/shared/externs.h"
#include "dungeoncraft/shared/SpecAb.h"
//#include "dungeoncraft/shared/Spell.h"
#include "dungeoncraft/shared/SharedQueue.h"
#include "dungeoncraft/shared/PicData.h"

namespace DungeonCraft {

class CAR;

const int COST = 1;
const int RDY  = 2;
const int QTY  = 4;
const int DETECTING_MAGIC = 8;
const int MONEY = 16;

const int MAX_ITEMS = 0x00FFFFFF;
const int NO_ITEM = -1;
const int NO_READY_ITEM = -1;
const int MAX_ITEM_NAME = 50;
const int MAX_ITEM_QTY = 65534; // max item qty in one item slot

enum itemRechargeRate 
{ 
  irrNever, // fixed qty of charges, once gone no 'Use' is possible
  irrDaily  // full qty of charges restored each day
};

enum weaponClassType 
{ 
                  // CanAttack?   AttackAdjacent?   AttackRanged?   NeedsAmmoReadied?  ConsumesAmmo?
                  //
  NotWeapon=0,    //   No              No               No                N/A              N/A
  HandBlunt=1,    //   Yes             Yes              Yes               No               No
  HandCutting=2,  //   Yes             Yes              Yes               No               No
  HandThrow=3,    //   Yes             Yes              Yes               No               if range > 1
  SlingNoAmmo=4,  //   Yes             No               Yes               No               No
  Bow=5,          //   Yes             No               Yes               Yes              Yes
  Crossbow=6,     //   Yes             No               Yes               Yes              Yes
  Throw=7,        //   Yes             No               Yes               No               Yes
  Ammo=8          //   No              N/A              No                N/A              N/A

  //
  //  CanAttack = isWeapon()
  //
  //  AttackRanged = WpnCanAttackAtRange(Range>1)
  //
  //  AttackAdjacent = WpnCanAttackAtRange(Range=1)
  //
  //  NeedsAmmoReadied = WpnUsesAmmo()
  //
  //  ConsumesAmmo = WpnConsumesAmmoAtRange(Range)
  //
};
const int MAX_WEAPON_CLASS = 9;

enum itemLocationType 
{ 
  WeaponHand=0,  // Sword								
  ShieldHand=1,  // Shield
	BodyArmor=2,   // Armor
	Hands=3,       // Guantlets
	Head=4,        // Helm
	Waist=5,       // Belt
	BodyRobe=6,    // Robe
	Back=7,        // Cloak
	Feet=8,        // Boots
	Fingers=9,     // Rings
	AmmoQuiver=10, // Arrows, Bolts
  CannotBeReadied=11 // Potions, Necklaces, mirrors, etc
}; 
const int MAX_LOCATION_CARRIED = 12;

enum itemClassType { itemType=0, 
                     PlatinumType=1, 
                     ElectrumType=2, 
                     GoldType=3, 
                     SilverType=4, 
                     CopperType=5, 
                     GemType=6, 
                     JewelryType=7,
                     SpecialItemType=8, 
                     SpecialKeyType=9, 
                     QuestType=10,
                     CoinType6=12,
                     CoinType7=13,
                     CoinType8=14,
                     CoinType9=15,
                     CoinType10=16,
                     BogusItemType=11 };

class READY_ITEMS : public CObject
{
public:
  DECLARE_SERIAL( READY_ITEMS )

  READY_ITEMS() { Clear(); }
  ~READY_ITEMS() { Clear(); }
  READY_ITEMS& operator=(const READY_ITEMS& src);
  BOOL operator==(const READY_ITEMS& src);
  READY_ITEMS(const READY_ITEMS& src) { *this = src; }
  void Serialize(CArchive &ar);
  void Serialize(CAR &ar);
  void Clear();

  BOOL UnReady(int item);
  BOOL Ready(int item, itemLocationType type, BYTE hands);
  BOOL ClearReady(int item, itemLocationType type, BYTE hands);
  int  GetReady(itemLocationType type);
  BOOL CanReady(itemLocationType type);

	int readyWeaponHand;
	int readyShieldHand;
	int readyMissileAmmo;
	int readyArmor;
	int readyGauntlets;
	int readyHelmet;
	int readyBelt;
	int readyRobe;
	int readyCloak;
	int readyBoots;
	int readyRing1;
	int readyRing2;
};

struct ITEM 
{
  ITEM() { Clear(); }
  void Clear() { key=0;item=NO_ITEM;ready=FALSE;qty=0;identified=FALSE;charges=0;cursed=FALSE;paid=-1; }
  void Serialize(CArchive &ar, double ver);
  void Serialize(CAR &ar, double ver);

  BOOL operator==(const ITEM& src)
  {
    if (&src==this) return TRUE;
    if (key != src.key) return FALSE;
    if (item != src.item) return FALSE;
    if (ready != src.ready) return FALSE;
    if (qty != src.qty) return FALSE;
    if (identified != src.identified) return FALSE;
    if (charges != src.charges) return FALSE;
    if (cursed != src.cursed) return FALSE;
    if (paid != src.paid) return FALSE;
    return TRUE;
  }
  int key;
	int item;
	int ready;
	int qty;
	int identified;
	int charges;
  BYTE cursed;
  // how much was paid for item (important for selling)
  // -1 means not purchased, use base item cost in database
  int paid; 
};

//extern CArchive& operator <<( CArchive& ar, const ITEM& slot );
//extern CArchive& operator >>( CArchive& ar, ITEM& slot );

class ITEM_LIST : public CObject
{
  READY_ITEMS rdyItems;
	OrderedQueue<ITEM, MAX_ITEMS> items;

  int GetNextKey();
  void SetReady(int index, BOOL ready);

public:
  enum { GemItem=-2, JewelryItem=-3 };
  static BOOL IsMoneyItem(int item) { return ((item==GemItem)||(item==JewelryItem)); }


  DECLARE_SERIAL( ITEM_LIST )
  ITEM_LIST() { Clear(); }
  ITEM_LIST(const ITEM_LIST& src) { *this = const_cast<ITEM_LIST&>(src); }
  ~ITEM_LIST() { Clear(); }
  void Clear() { items.RemoveAll(); rdyItems.Clear(); }  
  int GetCount() const { return items.GetCount(); }
  void Serialize(CArchive &ar, double ver);
  void Serialize(CAR &ar, double ver);

  POSITION GetHeadPosition() const { return items.GetHeadPosition(); }
  ITEM& GetAt(POSITION pos) { return items.GetAt(pos); }
  ITEM& GetNext(POSITION &pos) { return items.GetNext(pos); }
  int GetKeyAt(POSITION pos) { return items.GetKeyAt(pos); }

  BOOL ValidItemListIndex(int index);
  // called when loading from scripts
  int AddItem(ITEM &data, BOOL AutoJoin=TRUE);
  // called when loading binary data that
  // was saved with key info
  int AddItemWithCurrKey(const ITEM &data);
  BOOL DeleteItem(int index);
  BOOL GetItem(int index, ITEM &data);
  BOOL SetItem(int index, ITEM &data);
  ITEM_LIST& operator=(const ITEM_LIST& src);
  BOOL operator==(const ITEM_LIST& src) const;
  BOOL CanReady(int item, BYTE ItemMask);
  BOOL CanUnReady(int item);
  BOOL UnReady(int item);
  //BOOL ClearReadyByType(itemLocationType type);
  BOOL Ready(int item, BYTE ItemMask);
  int  GetReady(itemLocationType type);
  BOOL IsReady(int index);
  BOOL IsCursed(int index);
  void RemoveCurse(int index);
  BOOL HaveItem(int item);
  int GetItem(int index);
  int GetListKeyByItemKey(int item);
  BOOL GetId(int index);
  int GetQty(int index);
  int GetCharges(int index);
  int GetPurchasePrice(int index);
  void SetPurchasePrice(int index, int price);
  CString GetAmmoClass(int index);
  void SetId(int index, BOOL id);
  void SetQty(int index, int qty);
  void AdjustQty(int index, int qty);
  void SetCharges(int index, int chrgs);
  void ResetItemCharges(BOOL IsNewDay);

  void halveItem(int index);
  void joinItems(int index);
  int findItem(int item, int index);
  BOOL addItem(int item, int qty, int numCharges, BOOL id, int cost);
  BOOL deleteItem(int index, int qty);

  int GetProtectModForRdyItems();
  int GetAttackModForRdyItems();
};

struct ITEM_TEXT 
{
  ITEM_TEXT() 
  { 
    origIndex=-1;item=-1;charges=0;qty=0;
    data=-1;identified=FALSE;ready=FALSE;
    label="";itemClass=BogusItemType; 
  }
	int origIndex;
  int item;  
  int charges;
  int qty;
  int data;
  BOOL identified;
  BOOL ready;
  itemClassType itemClass; 
	CString label;
};

struct ITEM_TEXT_LIST 
{
  ITEM_TEXT_LIST() { Clear(); }
  ~ITEM_TEXT_LIST() { items.RemoveAll(); }

  void Clear()
  {
    title = "";
    items.RemoveAll();
    UseRdy = TRUE;
    UseQty = TRUE;
    UseCost = FALSE;
    UseMoney=FALSE;
    DetectingMagic = FALSE;
    CostFactor=Normal;
    RdyOffset=0;
    QtyOffset=0;
    CostOffset=0;
    NameOffset=0;
  }

  //void UpdateQty(int index, int qty) { items[index].qty += qty; if (items[index].qty <= 0) Remove(index); }
  void UpdateIdentified(int index, BOOL id) { items[index].identified = id; }
  void UpdateCharges(int index, int charges) { items[index].charges += charges; }
  void UpdateIsReady(int index, BOOL ready);
  void UpdateDetectMagic(BOOL detecting);

  int GetCount() const { return items.GetSize(); }
  int GetItemIndex(int index) const { return items[index].item; }
  int GetCharIndex(int index) const { return items[index].origIndex; }
  int GetItemQty(int index) const { return items[index].qty; }
  int GetItemCharges(int index) const { return items[index].charges; }
  int GetItemIdentified(int index) const { return items[index].identified; }
  int GetItemData(int index) const { return items[index].data; }

  itemClassType GetItemClass(int index) const 
  { 
    if ((index >= 0) && (index < GetCount()))
      return items[index].itemClass;
    else
      return BogusItemType;
  }

  BOOL ItemCanBeHalved(int index);
  BOOL ItemCanBeJoined(int index);

  void FillItemListText(ITEM_LIST &list, UINT flags, costFactorType type=Normal);
  void AddItemListText(ITEM_LIST &list, UINT flags, costFactorType type=Normal, int data=-1);
  void AddItemText(ITEM &addItem, UINT flags, costFactorType type=Normal);

  void Add(int qty, itemClassType type, const char *label, int origIndex=0, 
           BOOL id=FALSE, int charges=0, int item=NO_ITEM, BOOL ready=FALSE, int data=-1);
  //void Remove(int index) { items.RemoveAt(index); }

  char *GetItemQtyText(int index);
  char *GetItemRdyText(int index);
  char *GetItemCostText(int index);
  char *GetItemNameText(int index);
  char *GetItemText(int index);

  void FormatItemListTitle(const char *EmptyListTitle=NULL);

  BOOL UseRdy;
  BOOL UseQty;
  BOOL UseCost;
  BOOL UseMoney;
  BOOL DetectingMagic;
  costFactorType CostFactor;

  int RdyOffset;
  int QtyOffset;
  int CostOffset;
  int NameOffset;

  enum { MAX_ITEM_TEXT_LEN = MAX_ITEM_NAME+100 };
	char temp[MAX_ITEM_TEXT_LEN+1];
  char text[MAX_ITEM_TEXT_LEN+1];

  CString title;
  CArray<ITEM_TEXT, ITEM_TEXT&> items;
};


extern ITEM_TEXT_LIST itemListText;
extern ITEM_TEXT_LIST charItemListText;
extern ITEM_TEXT_LIST treasureItemListText;

class ITEM_DATA : public CObject
{
public:
  DECLARE_SERIAL( ITEM_DATA )
  ITEM_DATA();
  ITEM_DATA(ITEM_DATA &src) :
    item_asl("ITEM_DATA_ATTRIBUTES"),
    temp_asl("ITEM_DATA_ATTRIBUTES"),
	   hHitSound(-1), hMissSound(-1), hLaunchSound(-1) { *this = src; };

  ~ITEM_DATA() { ClearSounds(); Clear(TRUE); }
  void Serialize(CArchive &ar, double ver);
  void Serialize(CAR &ar, double ver);
 
  void Save(CArchive& ar);
  void PrepareForRestore(void);
  void Restore(CArchive& ar);
  void CommitRestore(void);

  ActorType GetContext();

#ifdef UAFEDITOR
  CString GetItemEventText();
#endif

  void SetASL(CString key, CString value, BYTE flags);
  BOOL GetASL(CString key, CString &value);

  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);

  void Clear(BOOL ctor=FALSE);

  ITEM_DATA& operator =(const ITEM_DATA& src);
  bool operator == (const ITEM_DATA& src);
  CString GetItemName(); 
  void PlayHit();
  void PlayMiss();
  void PlayLaunch();
  void ClearSounds();
  void LoadArt();
  void ClearArt(); // release surface and clear pic data
  void FreeArt(); // release surface

#ifdef UAFEngine
  void CastSpell(int spell);
  void SpellActivate(PENDING_SPELL &data);
#endif

  int key;
 // In the editor, Name means the fully qualified name sith
  // optional modifier.  Example: Battle Axe(Dull).
  // When we write the binary items.dat file, the modifiers
  // are taken off.
	CString  Name;
  CString  Id_Name;
  CString  AmmoType;
  CString  HitSound;
  CString  MissSound;
  CString  LaunchSound;
  int   hHitSound;
  int   hMissSound;
  int   hLaunchSound;

  long  Experience;
  long	Cost;
	long	Encumbrance;
	long	Attack_Bonus;
	BOOL	Cursed;
	long	Bundle_Qty;
	long	Num_Charges;
	itemLocationType	Location_Carried;
	long	Hands_to_Use;  
	long	Dmg_Dice_Sm;
	long	Nbr_Dice_Sm;
	long	Dmg_Bonus_Sm;
	long	Dmg_Dice_Lg;
	long	Nbr_Dice_Lg;
	long	Dmg_Bonus_Lg;
	double ROF_Per_Round;
  long  Protection_Base;
	long	Protection_Bonus;
  weaponClassType	Wpn_Type;
  BOOL	Usable;
  BOOL  CanBeHalvedJoined;
  BOOL  CanBeTradeDropSoldDep;
	long	Usable_by_Class;
	long	Range;
  long  Spell;
  long  Spell_Level;
  long  Spell_Class;
  DWORD event;
  DWORD ExamineEvent;
  CString ExamineLabel;
  CString attackMsg;
  itemRechargeRate Recharge_Rate;
  BOOL IsNonLethal;
  SPECIAL_ABILITIES specAbs;
  PIC_DATA  MissileArt; // in-route sprite
  PIC_DATA  HitArt;     // target hit sprite

  ASL item_asl;
  ASL temp_asl;
};

class ITEM_DATA_TYPE : public CObject
{
  //POSITION pos;
  CCriticalSection m_CS;
  OrderedQueue<ITEM_DATA, MAX_ITEMS> ItemData;
  int GetNextKey();

public:
  DECLARE_SERIAL( ITEM_DATA_TYPE )
  ITEM_DATA_TYPE() { Clear(); }
  ~ITEM_DATA_TYPE() { Clear(); }
  void Clear();
  ITEM_DATA_TYPE& operator =(ITEM_DATA_TYPE& src) ;
  bool operator == (ITEM_DATA_TYPE& src);
  
  void Serialize(CArchive &ar);  
  void Serialize(CAR &ar);
  void Save(CArchive& ar);
  void Restore(CArchive& ar);
  void CommitRestore(void);
  void PreSerialize(BOOL IsStoring);
  void PostSerialize(BOOL IsStoring);

  DWORD GetCount() { CSingleLock sLock(&m_CS, TRUE); return ItemData.GetCount(); }
  void SaveSounds();
  void SaveArt();

  void ExpandDefaultFilenames();
#ifdef UAFEDITOR
  void CollapseDefaultFilenames();
#endif

  POSITION GetHeadPosition() { return ItemData.GetHeadPosition(); }
  ITEM_DATA& GetAt(POSITION pos) { return ItemData.GetAt(pos); }
  ITEM_DATA& GetNext(POSITION &pos) { return ItemData.GetNext(pos); }
  int GetKeyAt(POSITION pos) { return ItemData.GetKeyAt(pos); }

  BOOL Validate(ITEM_DATA &data, FILE *pFile);
  BOOL Validate(int index, FILE *pFile); // specific item
  BOOL Validate(FILE *pFile); // all items
  void PlayHit(int index);
  void PlayMiss(int index);
  void PlayLaunch(int index);
  void ClearSounds();
  void FreeArt();
  CString GetItemName(int index);
  CString GetItemIdName(int index);
  long GetItemCost(int index);
  long GetItemExpWorth(int index);
  double GetItemROF(int index);
  long GetItemRange(int index);
  long GetItemBundleQty(int index);
  BOOL GetItemUsable(int index);
  CString GetAmmoClass(int index);
  weaponClassType GetWpnType(int index);
  BOOL HaveItem(int index); 
  // called when loading from scripts, or
  // when adding from editor GUI
  int AddItem(ITEM_DATA &data);
  // called when loading binary data that
  // was saved with key info
  int AddItemWithCurrKey(ITEM_DATA &data);
  BOOL SetItem(int index, ITEM_DATA &data);
  BOOL RemoveItem(int index);
  BOOL DeleteItemEvents(int index);
  BOOL GetItemAttribute(int index, CString key, CString value);
  BOOL SetItemAttribute(int index, CString key, CString value);
  //BOOL GetItemData(int index, ITEM_DATA &data);
  ITEM_DATA *GetItemData(int index);

   ActorType GetContext(int index);
#ifdef UAFEngine
  void CastSpell(int index, int spell);
  void SpellActivate(int index, PENDING_SPELL &data);
#endif

  void AddAmmoType(CString data);
  void RemoveAmmoType(CString data);
  int HaveAmmoType(CString data);
  BOOL HaveItemThatUsesAmmoType(CString data);

  CArray<CString, CString> m_AmmoTypes;
  double version;

private:
  // No locking... for internal use
  int m_FindName (const CString& name);

public:
  // With locking...for public use
  int FindName(const CString& name);
};


int getItemEncumbrance(int item, int qty);

// uses known item weapon type
BOOL isWeapon(weaponClassType type);
BOOL WpnUsesAmmo(weaponClassType type);
BOOL WpnConsumesSelfAsAmmo(weaponClassType type);

// checks by item class type
BOOL itemCanBeTraded(itemClassType type);
BOOL itemCanBeDropped(itemClassType type);
BOOL itemCanBeDeposited(itemClassType type);
BOOL itemCanBeJoined(itemClassType type);
BOOL itemCanBeHalved(itemClassType type);
BOOL itemCanBeReadied(itemClassType type);
BOOL itemIsMoney(itemClassType type); 
BOOL itemCanBeSold(itemClassType type);

// checks by item database key
BOOL itemCanBeTraded(int item);
BOOL itemCanBeDropped(int item);
BOOL itemCanBeDeposited(int item);
BOOL itemCanBeSold(int item);
BOOL itemCanBeJoined(int item);
BOOL itemCanBeHalved(int item);
BOOL itemCanBeReadied(int item);
BOOL itemUsesRdySlot(int item);

BOOL isWeapon(int index);
BOOL WpnUsesAmmo(int index);
BOOL isMagical(int index);
BOOL WpnCanAttackAtRange(int WpnIndex, int Range);
BOOL WpnConsumesAmmoAtRange(int WpnIndex, int Range);
BOOL WpnConsumesSelfAsAmmo(int WpnIndex);

// checks by item database record
BOOL itemUsesRdySlot(ITEM_DATA *pData);

} // End of namespace DungeonCraft

#endif