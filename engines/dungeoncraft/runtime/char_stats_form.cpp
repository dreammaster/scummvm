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
#include "dungeoncraft/shared/Graphics.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/GetInput.h"
#include "dungeoncraft/shared/Viewport.h"
#include "dungeoncraft/shared/CharStatsForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int LX = 18;
static const int RX = 220;
static int strmod=0;

char *CharStatusTypeText[] =
{
  "OKAY", 
  "UNCONSCIOUS", 
  "DEAD", 
  "FLED", 
  "PETRIFIED", 
  "GONE", 
  "ANIMATED", 
  "TEMP GONE",
  "RUNNING", 
  "DYING"  
};

char *CharGenderTypeText[] =
{
  "MALE",
  "FEMALE"
};

char *CharAlignmentTypeText[] =
{
  "LAWFUL GOOD",
  "NEUTRAL GOOD", 
  "CHAOTIC GOOD",
  "LAWFUL NEUTRAL", 
  "TRUE NEUTRAL", 
  "CHAOTIC NEUTRAL",
  "LAWFUL EVIL",    
  "NEUTRAL EVIL", 
  "CHAOTIC EVIL"
};

static enum ST_FORM { // Define field names as integers for quicker comparisons
  STF_none,
  STF_white=TEXT_FORM::white,    // White items
  STF_STATUS,
  STF_gender,
  STF_name,
  STF_age,
  STF_HITS,
  STF_maxHits,
  STF_align,
  STF_race,
  STF_class,
  STF_EXP,
  STF_exp,
  STF_level,
  STF_PLAT,
  STF_GOLD,
  STF_ELEC,
  STF_SILVER,
  STF_COPPER,
  STF_COIN6,
  STF_COIN7,
  STF_COIN8,
  STF_COIN9,
  STF_COIN10,
  STF_AC,
  STF_THAC0,
  STF_DAMAGE,
  STF_armor,
  STF_ENCUM,
  STF_MOVE,
  STF_green=TEXT_FORM::green,   // Change to Green
  STF_AVAIL,
  STF_avail,
  STF_status,
  STF_hits,
  STF_plat,
  STF_gold,
  STF_elec,
  STF_silver,
  STF_copper,
  STF_coin6,
  STF_coin7,
  STF_coin8,
  STF_coin9,
  STF_coin10,
  STF_ac,
  STF_thac0,
  STF_damage,
  STF_weapon,
  STF_encum,
  STF_move,
  STF_STR,
  STF_str,
  STF_INT,
  STF_int,
  STF_WIS,
  STF_wis,
  STF_DEX,
  STF_dex,
  STF_CON,
  STF_con,
  STF_CHA,
  STF_cha,
  STF_Str=TEXT_FORM::green+TEXT_FORM::tab, 
  STF_Int, // Selection rectangle includes more than one field
  STF_Wis, //   ''     ''
  STF_Dex, //   ''     ''
  STF_Con, //   ''     ''
  STF_Cha //   ''     ''
};

#define STF(item,xrel,x,yrel,y) \
  { DISPLAY_FORM(STF_##xrel,STF_##yrel,STF_##item,x,y) },

// when using relative coords, it is imperative
// that you dont display item B relative to item A 
// before actually displaying item A.
static DISPLAY_FORM statsForm[] =
{
  STF(STATUS   ,none       ,RX    ,none   ,  35)  // The word "STATUS"
  STF(status   ,STATUS+END ,16    ,STATUS ,   0)  // "dead"
  STF(name     ,none       ,LX    ,STATUS ,   0)  // "RICHARD"
  STF(gender   ,none       ,LX    ,STATUS ,  20)   // "MALE"
  STF(age      ,none       ,LX+100,gender ,   0)  // "17 YEARS"
  STF(HITS     ,none       ,RX    ,gender ,   0)  // "HIT POINTS"
  STF(hits     ,HITS+END   ,16    ,gender ,   0)  // # hit points
  STF(maxHits  ,hits+END   , 1    ,gender ,   0)  // "/maxhitpoints"
  STF(align    ,none       ,LX    ,age    ,  20)  // "EVIL" 
  STF(race     ,none       ,RX    ,HITS   ,  20)  // "HUMAN" 
  STF(class    ,none       ,LX    ,race   ,  20)  // "RANGER"
  STF(EXP      ,none       ,LX    ,class  ,  20)  // "EXPERIENCE" 
  STF(exp      ,EXP+END    ,16    ,EXP    ,   0)  // "25460"
  STF(level    ,none       ,LX    ,EXP    ,  20)  // "LEVEL 1"
  STF(STR      ,none       ,LX    ,none   , 172)  // "STR"
  STF(str      ,none       ,LX+35 ,STR    ,   0)  // strength value text
  STF(Str      ,STR+SEL    , 0    ,str+SEL,   0)  // STR + str
  STF(INT      ,STR        , 0    ,STR    ,  20)  // "INT"
  STF(int      ,none       ,LX+35 ,INT    ,   0)  // intelligence
  STF(Int      ,INT+SEL    , 0    ,int+SEL,   0)  // INT + int
  STF(WIS      ,INT        , 0    ,INT    ,  20)  // "WIS"
  STF(wis      ,none       ,LX+35 ,WIS    ,   0)  // wisdom
  STF(Wis      ,WIS+SEL    , 0    ,wis+SEL,   0)  // WIS + wis
  STF(DEX      ,WIS        , 0    ,WIS    ,  20)  // "DEX"
  STF(dex      ,none       ,LX+35 ,DEX    ,   0)  // dexterity
  STF(Dex      ,DEX+SEL    , 0    ,dex+SEL,   0)  // DEX + dex
  STF(CON      ,DEX        , 0    ,DEX    ,  20)  // "CON"
  STF(con      ,none       ,LX+35 ,CON    ,   0)  // constitution
  STF(Con      ,CON+SEL    , 0    ,con+SEL,   0)  // CON + con
  STF(CHA      ,CON        , 0    ,CON    ,  20)  // "CHA"
  STF(cha      ,none       ,LX+35 ,CHA    ,   0)  // charisma
  STF(Cha      ,CHA+SEL    , 0    ,cha+SEL,   0)  // CHA + cha
  STF(AVAIL    ,CHA        , 0    ,CHA    ,  20)  // "AVAIL"
  STF(avail    ,cha        , 0    ,AVAIL  ,   0)  // available to distribute
  STF(PLAT     ,none       ,RX    ,none   , 135/*115*/)  // "PLATINUM"
  STF(plat     ,PLAT+END   , 50   ,PLAT   ,   0)  // platinum
  STF(GOLD     ,PLAT       , 0    ,PLAT   ,  20)  // "GOLD"
  STF(gold     ,plat+RIGHT , 0    ,GOLD   ,   0)  // gold
  STF(ELEC     ,GOLD       , 0    ,GOLD   ,  20)  // "ELECTRUM"
  STF(elec     ,plat+RIGHT , 0    ,ELEC   ,   0)  // electrum  
  STF(SILVER   ,ELEC       , 0    ,ELEC   ,  20)  // "SILVER"
  STF(silver   ,plat+RIGHT , 0    ,SILVER ,   0)  // silver  
  STF(COPPER   ,SILVER     , 0    ,SILVER ,  20)  // "COPPER"
  STF(copper   ,plat+RIGHT , 0    ,COPPER ,   0)  // copper
  STF(COIN6    ,COPPER     , 0    ,COPPER ,  20)  // "COIN6"
  STF(coin6    ,plat+RIGHT , 0    ,COIN6  ,   0)  // coin6
  STF(COIN7    ,COIN6      , 0    ,COIN6  ,  20)  // "COIN7"
  STF(coin7    ,plat+RIGHT , 0    ,COIN7  ,   0)  // coin7
  STF(COIN8    ,COIN7      , 0    ,COIN7  ,  20)  // "COIN8"
  STF(coin8    ,plat+RIGHT , 0    ,COIN8  ,   0)  // coin8
  STF(COIN9    ,COIN8      , 0    ,COIN8  ,  20)  // "COIN9"
  STF(coin9    ,plat+RIGHT , 0    ,COIN9  ,   0)  // coin9
  STF(COIN10   ,COIN9      , 0    ,COIN9  ,  20)  // "COIN10"
  STF(coin10   ,plat+RIGHT , 0    ,COIN10 ,   0)  // coin10
  STF(AC       ,none       ,LX    ,none   , 325)  // "ARMOR CLASS"
  STF(ac       ,AC+END     , 0    ,AC     ,   0)  // armor classs
  STF(THAC0    ,none       ,LX    ,none   , 345)  // "THAC0"
  STF(thac0    ,THAC0+END  , 0    ,THAC0  ,   0)  // thac0
  STF(DAMAGE   ,none       ,LX    ,none   , 365)  // "DAMAGE"
  STF(damage   ,DAMAGE+END ,20    ,DAMAGE ,   0)  // 1d4
  STF(weapon   ,none       ,LX    ,none   , 400)  // "battle axe"
  STF(armor    ,none       ,LX    ,none   , 420)  // "plate mail"
  STF(ENCUM    ,none       ,RX    ,none   , 325)  // "ENCUMBRANCE"
  STF(encum    ,ENCUM+END  ,20    ,ENCUM  ,   0)  // encumbrance
  STF(MOVE     ,none       ,RX    ,none   , 345)  // "MOVEMENT"
  STF(move     ,encum      , 0    ,MOVE   ,   0)  // movement
  STF(none,none,0,none,0)  // End of list
};

void showCharStats(TEXT_FORM *pForm, CHARACTER& dude)
{ 
   ASSERT(pForm!=NULL);

   BOOL IsAdj;
   CString temp;
   CString temp2;
   COLORREF color;
   COLORREF namecolor;

   const COLORREF AdjColor = RGB(255,128,0); // Bright Orange, for stats currently modified by active spells   

   // make sure that highlight flags are set after clearing form
   //pForm->ClearForm();

   int charCurrHp = dude.GetAdjHitPoints();

   if (charCurrHp <= 0)
      color = Red;
   else if (charCurrHp < dude.GetMaxHitPoints())
      color = Yellow;
   else
      color = Green;
   
   if (dude.GetAdjReadyToTrain())
     namecolor = Blue;
   else
     namecolor = Green;

   pForm->SetText(STF_STATUS, "STATUS");   

   // char name
   pForm->SetText(STF_name  , dude.GetName(), namecolor);
   
   // char status
   charStatusType charTrueStatus = dude.GetStatus();
   charStatusType charCurrStatus = dude.GetAdjStatus();   
   COLORREF statusColor = color;
   if (charTrueStatus!=charCurrStatus)
     statusColor = AdjColor;

   if (charCurrStatus != Okay)
     pForm->SetText(STF_status, CharStatusTypeText[charCurrStatus], statusColor);
   else
     pForm->SetText(STF_status, CharStatusTypeText[charCurrStatus], Green);
   
   // char gender
   genderType charTrueGender = dude.GetGender();
   genderType charCurrGender = dude.GetAdjGender();
   temp = CharGenderTypeText[charCurrGender];
   if (charCurrGender!=charTrueGender)
     pForm->SetText(STF_gender,temp, AdjColor);
   else
     pForm->SetText(STF_gender,temp);   
   
   // char age
   int charTrueAge = dude.GetAge();
   int charCurrAge = dude.GetAdjAge();
   temp.Format("%i YEARS", charCurrAge);
   if (charTrueAge!=charCurrAge)
     pForm->SetText(STF_age, temp, AdjColor);
   else
     pForm->SetText(STF_age, temp);

   // char hitpoints/maxhitpoints
   int charTrueHp = dude.GetHitPoints();
   pForm->SetText(STF_HITS, "HIT POINTS");    
   if (charCurrHp!=charTrueHp)
   {
     temp.Format("%i", charCurrHp);
     pForm->SetText(STF_hits, temp, AdjColor);   
     temp.Format("/%i", dude.GetMaxHitPoints());
     pForm->SetText( STF_maxHits, temp, AdjColor);
   }
   else
   {
     temp.Format("%i", charCurrHp);
     pForm->SetText(STF_hits, temp, color);   
     temp.Format("/%i", dude.GetMaxHitPoints());
     pForm->SetText( STF_maxHits, temp, color);
   }

   // char alignment   
   alignmentType charCurrAlignment = dude.GetAdjAlignment();
   alignmentType charTrueAlignment = dude.GetAlignment();
   if (charCurrAlignment!=charTrueAlignment)
     pForm->SetText(STF_align, CharAlignmentTypeText[charCurrAlignment], AdjColor);
   else
     pForm->SetText(STF_align, CharAlignmentTypeText[charCurrAlignment]);
  
   // char race
   //
   // ********  temp fix here  **********
   //
   // the character doesn't have the race key
   // yet, and that is what raceData.GetRaceName() is
   // expecting. It was not mapping raceType to
   // race text and was always returning 'No Race'
   //
   pForm->SetText( STF_race, raceData.GetRaceName(dude.race()));
   
   // char class
   classType charCurrClass = dude.GetAdjClass();
   classType charTrueClass = dude.GetClass();
   if (charCurrClass!=charTrueClass)
     pForm->SetText( STF_class, classData[charCurrClass].m_name, AdjColor);
   else
     pForm->SetText( STF_class, classData[charCurrClass].m_name);
     
   // char levels/experience

   int exp1,exp2,exp3;
   IsAdj=FALSE;
   switch (charCurrClass) 
   {
   case C_F:
      temp.Format("LEVEL %i/%i", dude.GetCurrentLevel(ClericFlag), dude.GetCurrentLevel(FighterFlag));
      exp1=dude.GetAdjClericExp();exp2=dude.GetAdjFighterExp();
      temp2.Format("%i/%i", exp1, exp2);
      IsAdj = ((exp1!=dude.GetClericExp())||(exp2!=dude.GetFighterExp()));
      break;
   case C_F_MU:
      temp.Format("LEVEL %i/%i/%i", dude.GetCurrentLevel(ClericFlag),dude.GetCurrentLevel(FighterFlag),dude.GetCurrentLevel(MagicUserFlag));
      exp1=dude.GetAdjClericExp();exp2=dude.GetAdjFighterExp();exp3=dude.GetAdjMagicUserExp();
      temp2.Format("%i/%i/%i", exp1, exp2, exp3);
      IsAdj = ((exp1!=dude.GetClericExp())||(exp2!=dude.GetFighterExp())||(exp3!=dude.GetMagicUserExp()));
      break;
   case C_R:
      temp.Format("LEVEL %i/%i", dude.GetCurrentLevel(ClericFlag), dude.GetCurrentLevel(RangerFlag));
      exp1=dude.GetAdjClericExp();exp2=dude.GetAdjRangerExp();
      temp2.Format("%i/%i", exp1, exp2);
      IsAdj = ((exp1!=dude.GetClericExp())||(exp2!=dude.GetRangerExp()));
      break;
   case C_MU:
      temp.Format("LEVEL %i/%i", dude.GetCurrentLevel(ClericFlag), dude.GetCurrentLevel(MagicUserFlag));
      exp1=dude.GetAdjClericExp();exp2=dude.GetAdjMagicUserExp();
      temp2.Format("%i/%i", exp1, exp2);
      IsAdj = ((exp1!=dude.GetClericExp())||(exp2!=dude.GetMagicUserExp()));
      break;
   case C_T:
      temp.Format("LEVEL %i/%i", dude.GetCurrentLevel(ClericFlag), dude.GetCurrentLevel(ThiefFlag));
      exp1=dude.GetAdjClericExp();exp2=dude.GetAdjThiefExp();
      temp2.Format("%i/%i", exp1, exp2);
      IsAdj = ((exp1!=dude.GetClericExp())||(exp2!=dude.GetThiefExp()));
      break;
   case F_MU:
      temp.Format("LEVEL %i/%i", dude.GetCurrentLevel(FighterFlag),dude.GetCurrentLevel(MagicUserFlag));
      exp1=dude.GetAdjFighterExp();exp2=dude.GetAdjMagicUserExp();
      temp2.Format("%i/%i", exp1, exp2);
      IsAdj = ((exp1!=dude.GetFighterExp())||(exp2!=dude.GetMagicUserExp()));
      break;
   case F_T:
      temp.Format("LEVEL %i/%i", dude.GetCurrentLevel(FighterFlag),dude.GetCurrentLevel(ThiefFlag));
      exp1=dude.GetAdjFighterExp();exp2=dude.GetAdjThiefExp();
      temp2.Format("%i/%i", exp1, exp2);
      IsAdj = ((exp1!=dude.GetFighterExp())||(exp2!=dude.GetThiefExp()));
      break;
   case F_MU_T:
      temp.Format("LEVEL %i/%i/%i", dude.GetCurrentLevel(FighterFlag),dude.GetCurrentLevel(MagicUserFlag),dude.GetCurrentLevel(ThiefFlag));
      exp1=dude.GetAdjFighterExp();exp2=dude.GetAdjMagicUserExp();exp3=dude.GetAdjThiefExp();
      temp2.Format("%i/%i/%i", exp1, exp2, exp3);
      IsAdj = ((exp1!=dude.GetFighterExp())||(exp2!=dude.GetMagicUserExp())||(exp3!=dude.GetThiefExp()));
      break;
   case MU_T:
      temp.Format("LEVEL %i/%i", dude.GetCurrentLevel(MagicUserFlag),dude.GetCurrentLevel(ThiefFlag));
      exp1=dude.GetAdjMagicUserExp();exp2=dude.GetAdjThiefExp();
      temp2.Format("%i/%i", exp1, exp2);
      IsAdj = ((exp1!=dude.GetMagicUserExp())||(exp2!=dude.GetThiefExp()));
      break;
   case F_D:
      temp.Format("LEVEL %i/%i", dude.GetCurrentLevel(FighterFlag),dude.GetCurrentLevel(DruidFlag));
      exp1=dude.GetAdjFighterExp();exp2=dude.GetAdjDruidExp();
      temp2.Format("%i/%i", exp1, exp2);
      IsAdj = ((exp1!=dude.GetFighterExp())||(exp2!=dude.GetDruidExp()));
      break;
   case D_MU:
      temp.Format("LEVEL %i/%i", dude.GetCurrentLevel(DruidFlag),dude.GetCurrentLevel(MagicUserFlag));
      exp1=dude.GetAdjDruidExp();exp2=dude.GetAdjMagicUserExp();
      temp2.Format("%i/%i", exp1, exp2);
      IsAdj = ((exp1!=dude.GetAdjDruidExp())||(exp2!=dude.GetAdjMagicUserExp()));
      break;
   case F_MU_D:
      temp.Format("LEVEL %i/%i/%i", dude.GetCurrentLevel(FighterFlag),dude.GetCurrentLevel(MagicUserFlag),dude.GetCurrentLevel(DruidFlag));
      exp1=dude.GetAdjFighterExp();exp2=dude.GetAdjMagicUserExp();exp3=dude.GetAdjDruidExp();
      temp2.Format("%i/%i/%i", exp1, exp2, exp3);
      IsAdj = ((exp1!=dude.GetFighterExp())||(exp2!=dude.GetMagicUserExp())||(exp3!=dude.GetDruidExp()));
      break;
   case Fighter:
      temp.Format("LEVEL %i", dude.GetCurrentLevel(FighterFlag));
      exp1=dude.GetAdjFighterExp();
      temp2.Format("%i", exp1);
      IsAdj = (exp1!=dude.GetFighterExp());
      break;
   case Cleric:
      temp.Format("LEVEL %i", dude.GetCurrentLevel(ClericFlag));
      exp1=dude.GetAdjClericExp();
      temp2.Format("%i", exp1);
      IsAdj = (exp1!=dude.GetClericExp());
      break;
   case Ranger:
      temp.Format("LEVEL %i", dude.GetCurrentLevel(RangerFlag));
      exp1=dude.GetAdjRangerExp();
      temp2.Format("%i", exp1);
      IsAdj = (exp1!=dude.GetRangerExp());
      break;
   case Paladin:
      temp.Format("LEVEL %i", dude.GetCurrentLevel(PaladinFlag));
      exp1=dude.GetAdjPaladinExp();
      temp2.Format("%i", exp1);
      IsAdj = (exp1!=dude.GetPaladinExp());
      break;
   case MagicUser:
      temp.Format("LEVEL %i", dude.GetCurrentLevel(MagicUserFlag));
      exp1=dude.GetAdjMagicUserExp();
      temp2.Format("%i", exp1);
      IsAdj = (exp1!=dude.GetMagicUserExp());
      break;
   case Thief:
      temp.Format("LEVEL %i", dude.GetCurrentLevel(ThiefFlag));
      exp1=dude.GetAdjThiefExp();
      temp2.Format("%i", exp1);
      IsAdj = (exp1!=dude.GetThiefExp());
      break;
   case Druid:
      temp.Format("LEVEL %i", dude.GetCurrentLevel(DruidFlag));
      exp1=dude.GetAdjDruidExp();
      temp2.Format("%i", exp1);
      IsAdj = (exp1!=dude.GetDruidExp());
      break;
   default:
      temp.Format("LEVEL ?");
      temp2.Format("%s", "??");
      WriteDebugString("Unknown character class in displayCharStats()\n");
      break;
   }

   // char experience
   pForm->SetText( STF_EXP,"EXPERIENCE");
   if (IsAdj)
     pForm->SetText( STF_exp, temp2, AdjColor); // experience
   else
     pForm->SetText( STF_exp, temp2, namecolor); // experience

   // char level
   pForm->SetText( STF_level, temp); // level

   // char ability stats  

   // strength
   pForm->SetText( STF_STR,"STR     ");   
   BYTE charCurrStMod = dude.GetAdjStrMod();
   BYTE charTrueStMod = dude.GetStrMod();
   IsAdj = (charCurrStMod!=charTrueStMod);

   int charCurrStat = dude.GetAdjStr();
   int charTrueStat = dude.GetStr();
   IsAdj |= (charCurrStat!=charTrueStat);

   if (charCurrStMod>0)
      temp.Format("    %2i (%i)", charCurrStat, charCurrStMod);
   else 
      temp.Format("    %2i", charCurrStat);

   if (IsAdj)
     pForm->SetText( STF_str, temp, AdjColor);
   else
     pForm->SetText( STF_str, temp);

   pForm->SetText( STF_Str,"");

   // intelligence
   charCurrStat = dude.GetAdjInt();
   charTrueStat = dude.GetInt();
   IsAdj = (charCurrStat!=charTrueStat);
   pForm->SetText( STF_INT,"INT     ");
   temp.Format("    %2i", charCurrStat);
   if (IsAdj)
     pForm->SetText( STF_int, temp, AdjColor);
   else
     pForm->SetText( STF_int, temp);
   pForm->SetText( STF_Int,"");
   
   // wisdom
   charCurrStat = dude.GetAdjWis();
   charTrueStat = dude.GetWis();
   IsAdj = (charCurrStat!=charTrueStat);
   pForm->SetText( STF_WIS,"WIS     ");
   temp.Format("    %2i", charCurrStat);
   if (IsAdj)
     pForm->SetText( STF_wis, temp, AdjColor);
   else
     pForm->SetText( STF_wis, temp);
   pForm->SetText( STF_Wis, "");
   
   // dexterity
   charCurrStat = dude.GetAdjDex();
   charTrueStat = dude.GetDex();
   IsAdj = (charCurrStat!=charTrueStat);
   pForm->SetText( STF_DEX,"DEX     ");
   temp.Format("    %2i", charCurrStat);
   if (IsAdj)
     pForm->SetText( STF_dex, temp, AdjColor);
   else
     pForm->SetText( STF_dex, temp);
   pForm->SetText( STF_Dex, "");

   // constitution
   charCurrStat = dude.GetAdjCon();
   charTrueStat = dude.GetCon();
   IsAdj = (charCurrStat!=charTrueStat);
   pForm->SetText( STF_CON,"CON     ");
   temp.Format("    %2i", charCurrStat);
   if (IsAdj)
     pForm->SetText(STF_con, temp, AdjColor);
   else
     pForm->SetText(STF_con, temp);
   pForm->SetText( STF_Con, "");
   
   // charisma
   charCurrStat = dude.GetAdjCha();
   charTrueStat = dude.GetCha();
   IsAdj = (charCurrStat!=charTrueStat);
   pForm->SetText( STF_CHA,"CHA    ");
   temp.Format("    %2i", charCurrStat);
   if (IsAdj)
     pForm->SetText( STF_cha, temp, AdjColor);
   else
     pForm->SetText( STF_cha, temp);
   pForm->SetText( STF_Cha, "");
   
   // char money
   if (dude.money.IsActive(PlatinumType))
   {
      pForm->SetText( STF_PLAT, globalData.moneyData.GetName(PlatinumType));
      temp.Format("%6i", dude.money[PlatinumType]);
      pForm->SetText( STF_plat, temp);
   }
   else
   {
     pForm->SetText( STF_PLAT, "");
     pForm->SetText( STF_plat, "");
   }
   
   if (dude.money.IsActive(GoldType))
   {
      pForm->SetText( STF_GOLD, globalData.moneyData.GetName(GoldType));
      temp.Format("%6i", dude.money[GoldType]);
      pForm->SetText( STF_gold, temp);
   }
   else
   {
     pForm->SetText( STF_GOLD, "");
     pForm->SetText( STF_gold, "");
   }
   
   if (dude.money.IsActive(ElectrumType))
   {
      pForm->SetText( STF_ELEC, globalData.moneyData.GetName(ElectrumType));
      temp.Format("%6i", dude.money[ElectrumType]);
      pForm->SetText( STF_elec, temp);
   }
   else
   {
     pForm->SetText( STF_ELEC, "");
     pForm->SetText( STF_elec, "");
   }
   
   if (dude.money.IsActive(SilverType))
   {
      pForm->SetText( STF_SILVER, globalData.moneyData.GetName(SilverType));
      temp.Format("%6i", dude.money[SilverType]);
      pForm->SetText( STF_silver, temp);
   }
   else
   {
     pForm->SetText( STF_SILVER, "");
     pForm->SetText( STF_silver, "");
   }
   
   if (dude.money.IsActive(CopperType))
   {  
      pForm->SetText( STF_COPPER, globalData.moneyData.GetName(CopperType));
      temp.Format("%6i", dude.money[CopperType]);
      pForm->SetText( STF_copper, temp);
   }
   else
   {
     pForm->SetText( STF_COPPER, "");
     pForm->SetText( STF_copper, "");
   }

   if (dude.money.IsActive(CoinType6))
   {  
     pForm->SetText( STF_COIN6, globalData.moneyData.GetName(CoinType6));
     temp.Format("%6i", dude.money[CoinType6]);
     pForm->SetText( STF_coin6, temp);
   }
   else
   {
     pForm->SetText( STF_COIN6, "");
     pForm->SetText( STF_coin6, "");
   }

   if (dude.money.IsActive(CoinType7))
   {  
     pForm->SetText( STF_COIN7, globalData.moneyData.GetName(CoinType7));
     temp.Format("%6i", dude.money[CoinType7]);
     pForm->SetText( STF_coin7, temp);
   }
   else
   {
     pForm->SetText( STF_COIN7, "");
     pForm->SetText( STF_coin7, "");
   }

   if (dude.money.IsActive(CoinType8))
   {  
     pForm->SetText( STF_COIN8, globalData.moneyData.GetName(CoinType8));
     temp.Format("%6i", dude.money[CoinType8]);
     pForm->SetText( STF_coin8, temp);
   }
   else
   {
     pForm->SetText( STF_COIN8, "");
     pForm->SetText( STF_coin8, "");
   }

   if (dude.money.IsActive(CoinType9))
   {  
     pForm->SetText( STF_COIN9, globalData.moneyData.GetName(CoinType9));
     temp.Format("%6i", dude.money[CoinType9]);
     pForm->SetText( STF_coin9, temp);
   }
   else
   {
     pForm->SetText( STF_COIN9, "");
     pForm->SetText( STF_coin9, "");
   }

   if (dude.money.IsActive(CoinType10))
   {  
     pForm->SetText( STF_COIN10, globalData.moneyData.GetName(CoinType10));
     temp.Format("%6i", dude.money[CoinType10]);
     pForm->SetText( STF_coin10, temp);
   }
   else
   {
     pForm->SetText( STF_COIN10, "");
     pForm->SetText( STF_coin10, "");
   }

   // char AC
   charCurrStat = dude.GetAdjAC();
   charTrueStat = dude.GetAC();
   IsAdj = (charCurrStat!=charTrueStat);
   pForm->SetText( STF_AC, "ARMOR CLASS");
   temp.Format("%5i", charCurrStat);
   if (IsAdj)
     pForm->SetText( STF_ac, temp, AdjColor);
   else
     pForm->SetText( STF_ac, temp);

   // char THACO
   charCurrStat = dude.GetAdjTHACO();
   charTrueStat = dude.GetTHACO();
   IsAdj = (charCurrStat!=charTrueStat);
   pForm->SetText( STF_THAC0, "THACO");
   temp.Format("%5i", charCurrStat);
   if (IsAdj)
     pForm->SetText( STF_thac0, temp, AdjColor);
   else
     pForm->SetText( STF_thac0, temp);

   // char weapon damage
   pForm->SetText( STF_DAMAGE, "DAMAGE"); 
   dude.getCharWeaponText(temp, temp2);  
   if (temp.GetLength()>0) // wpn name
   {
      pForm->SetText( STF_weapon, temp);

      if (temp2.GetLength()>0) // wpn damage
      {
        temp.Format("%6s", temp2);
        pForm->SetText( STF_damage, temp);
      }
      else
        pForm->SetText( STF_damage, "", Black);
   }
   else
   {
     pForm->SetText( STF_weapon, "", Black);
     pForm->SetText( STF_damage, "", Black);
   }
   
   // char worn armor
   dude.getCharArmorText(temp);   
   if (temp.GetLength()>0)
     pForm->SetText( STF_armor, temp);
   else
    pForm->SetText( STF_armor, "", Black);
 
   // char encumbrance
   pForm->SetText( STF_ENCUM, "ENCUMBRANCE");   
   temp.Format("%5i", dude.GetEncumbrance());
   pForm->SetText( STF_encum, temp);

   // char movement rate
   charCurrStat = dude.GetAdjMaxMovement();
   charTrueStat = dude.GetMaxMovement();
   IsAdj = (charCurrStat!=charTrueStat);
   pForm->SetText( STF_MOVE, "MOVEMENT");   
   temp.Format("%5i", charCurrStat);
   if (IsAdj)
     pForm->SetText( STF_move, temp, AdjColor);
   else
     pForm->SetText( STF_move, temp);
}

void displayCharStats(CHARACTER &dude)
{
  TEXT_FORM StatsForm(statsForm);
  StatsForm.ClearForm();
  showCharStats(&StatsForm,dude);
  StatsForm.Display();
}
 
extern bool AllowModifyStats;

class CH
{ // A class to hold a pointer to a character.
  // We need the destructor to release the memory
  // when the program terminates without going through
  // the normal menu-handling process. Otherwise a
  // memory leak warning is issued.
public:
  CHARACTER *c;
  CH(void) { c=NULL;};
  ~CH(void) { if (c) free(c);};
};


static bool STF_highlight (TEXT_FORM *frm, 
                      int *oldSelection,
                      int newSelection) {
  // See if new selection is valid.
  // If so, set old selection.
  // If old selection changes, highlight it and return true.
  int hi1=-1, hi2=-1;
  switch (newSelection)
  {
  case STF_Str:
    hi1=STF_STR;
    hi2=STF_str;
    break;
  case STF_Int:
    hi1=STF_INT;
    hi2=STF_int;
    break;
  case STF_Wis:
    hi1=STF_WIS;
    hi2=STF_wis;
    break;
  case STF_Dex:
    hi1=STF_DEX;
    hi2=STF_dex;
    break;
  case STF_Con:
    hi1=STF_CON;
    hi2=STF_con;
    break;
  case STF_Cha:
    hi1=STF_CHA;
    hi2=STF_cha;
    break;
   default:  // leave things as they are.
    newSelection=-1;
  };
  if ((newSelection!=-1) && (newSelection!=*oldSelection))
  {
    frm->Highlight(hi1);
    frm->Highlight(hi2);
    *oldSelection=newSelection;
    return true;
  };
  return false;
}

bool STF_IncrStat(int selection, int *avail, CHARACTER *dude)
{
  // Returns true if something has changed.
  if (*avail==0) return false;
  BYTE orig;
  switch (selection)
  {
  case STF_Str:
    if (dude->GetStr()>=ABILITYMAXBASE(dude->GetAbilityLimits(0))) return false;
    orig=dude->GetStr();
    dude->SetStr(dude->GetStr()+1);
    if (orig != dude->GetStr()) 
    {
      (*avail)--;
      if (dude->GetStr()==18)
      {
        if (strmod==0) strmod = rollDice(100,1,0.0);
        dude->SetStrMod(strmod);
      }
    }
    break;
  case STF_Int:
    if (dude->GetInt()>=ABILITYMAXBASE(dude->GetAbilityLimits(1))) return false;
    orig=dude->GetInt();
    dude->SetInt(dude->GetInt()+1);
    if (orig != dude->GetInt()) (*avail)--;
    break;
  case STF_Wis:
    if (dude->GetWis()>=ABILITYMAXBASE(dude->GetAbilityLimits(2))) return false;
    orig=dude->GetWis();
    dude->SetWis(dude->GetWis()+1);
    if (orig != dude->GetWis()) (*avail)--;
    break;
  case STF_Dex:
    if (dude->GetDex()>=ABILITYMAXBASE(dude->GetAbilityLimits(3))) return false;
    orig=dude->GetDex();
    dude->SetDex(dude->GetDex()+1);
    if (orig != dude->GetDex()) (*avail)--;
    break;
  case STF_Con:
    if (dude->GetCon()>=ABILITYMAXBASE(dude->GetAbilityLimits(4))) return false;
    orig=dude->GetCon();
    dude->SetCon(dude->GetCon()+1);
    if (orig != dude->GetCon()) (*avail)--;
    break;
  case STF_Cha:
    if (dude->GetCha()>=ABILITYMAXBASE(dude->GetAbilityLimits(5))) return false;
    orig=dude->GetCha();
    dude->SetCha(dude->GetCha()+1);
    if (orig != dude->GetCha()) (*avail)--;
    break;
  default: return false;
  };
  dude->UpdateStats(FALSE);
  return true;
}

bool STF_DecrStat(int selection, int *avail, CHARACTER *dude)
{
  // Returns true if something has changed.
  BYTE orig;
  switch (selection )
  {
  case STF_Str:
    if (dude->GetStr()<=ABILITYMINBASE(dude->GetAbilityLimits(0))) return false;
    orig=dude->GetStr();
    if (orig==18) strmod = dude->GetStrMod();
    dude->SetStr(dude->GetStr()-1);
    dude->UpdateStats(FALSE);
    if (orig != dude->GetStr()) (*avail)++;    
    break;
  case STF_Int:
    if (dude->GetInt()<=ABILITYMINBASE(dude->GetAbilityLimits(1))) return false;
    orig=dude->GetInt();
    dude->SetInt(dude->GetInt()-1);
    dude->UpdateStats(FALSE);
    if (orig != dude->GetInt()) (*avail)++;
    break;
  case STF_Wis:
    if (dude->GetWis()<=ABILITYMINBASE(dude->GetAbilityLimits(2))) return false;
    orig=dude->GetWis();
    dude->SetWis(dude->GetWis()-1);
    dude->UpdateStats(FALSE);
    if (orig != dude->GetWis()) (*avail)++;
    break;
  case STF_Dex:
    if (dude->GetDex()<=ABILITYMINBASE(dude->GetAbilityLimits(3))) return false;
    orig=dude->GetDex();
    dude->SetDex(dude->GetDex()-1);
    dude->UpdateStats(FALSE);
    if (orig != dude->GetDex()) (*avail)++;
    break;
  case STF_Con:
    if (dude->GetCon()<=ABILITYMINBASE(dude->GetAbilityLimits(4))) return false;
    orig=dude->GetCon();
    dude->SetCon(dude->GetCon()-1);
    dude->UpdateStats(FALSE);
    if (orig != dude->GetCon()) (*avail)++;
    break;
  case STF_Cha:
    if (dude->GetCha()<=ABILITYMINBASE(dude->GetAbilityLimits(5))) return false;
    orig=dude->GetCha();
    dude->SetCha(dude->GetCha()-1);
    dude->UpdateStats(FALSE);
    if (orig != dude->GetCha()) (*avail)++;
    break;
  default: return false;
  };  
  return true;
}

int handleChooseStatsInput(CHAR_STATS_INPUT_MESSAGE_TYPE msg,
                            CHARACTER *dude,
                            int P1, 
                            int P2)
// Returns 0x0001 if you should display menu and flip surfaces.
// Returns 0x0002 if we handled a keystroke and you can ignore it
{
  TEXT_FORM StatsForm(statsForm);
  static bool chooseActive=false;
  static int currentSelection;
  static int availPoints;
  static CH origChar;
  int newSelection;
  int result=0;
  bool flip=false, redraw=false;
  switch (msg)
  {
  case CHOOSESTATS_initial:
    ASSERT(chooseActive==false);
    chooseActive=true;
    currentSelection=-1;
    availPoints=0;
    origChar.c=new CHARACTER(*dude);
    redraw=true;
    strmod=0;
    break;
  case CHOOSESTATS_refresh:
    ASSERT(chooseActive);
    ASSERT(dude!=NULL);
    redraw=true;
    break;
  case CHOOSESTATS_mouseClick:
    {
    ASSERT(chooseActive);
    ASSERT(dude!=NULL);
    if (!AllowModifyStats) return false; // Ignore mouse
    StatsForm.ClearForm();
    int junk=-1;
    STF_highlight(&StatsForm,&junk,currentSelection);
    showCharStats(&StatsForm, *dude);  // To define rectangles.
    newSelection=StatsForm.MouseClick(P1, P2);  // P1=x, P2=y
    if (STF_highlight(&StatsForm,&currentSelection,newSelection)) flip=true;
    }
    break;
  case CHOOSESTATS_exit:
    chooseActive=false;
    if (origChar.c!=NULL)
    {
      delete origChar.c;
      origChar.c=NULL;
    };
    break;
  case CHOOSESTATS_key:
    ASSERT(chooseActive);
    if (!AllowModifyStats) break;
    switch(P1)
    {
    case KC_TAB:
      newSelection=StatsForm.Tab(currentSelection);
      if (STF_highlight(&StatsForm,&currentSelection,newSelection)) redraw=true;
      result|=2;
      break;
    case KC_PLUS:
    case KC_UP:
      if (STF_IncrStat(currentSelection,&availPoints,dude))redraw=true;
      result|=2;
      break;
    case KC_MINUS:
    case KC_DOWN:
      if (STF_DecrStat(currentSelection,&availPoints,dude)) redraw=true;
      result|=2;
      break;
    };
    break;
  default:
    ASSERT(0);
    return 0;  // Internal error
  };
  if (redraw)
  {     
    StatsForm.ClearForm();
    int junk=-1;
    STF_highlight(&StatsForm,&junk,currentSelection);
    showCharStats(&StatsForm, *dude);
    if (availPoints)
    {
      char temp[20];
      StatsForm.SetText(STF_AVAIL,"AVAIL");
      sprintf(temp,"    %d",availPoints);
      StatsForm.SetText(STF_avail,temp);
    };
    //STF_highlight(&StatsForm,&junk,currentSelection);
    flip=true;
  };
  if (flip)
  {
    GraphicsMgr.ClearAdventureBackground(BACKBUFFERNUM);   
    drawBorder();
    drawViewportFrame(VPFrameXOffset, VPFrameY+VPFrameYOffset);
    StatsForm.Display();
  };
  return result|(flip?1:0);
}
