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
#include "dungeoncraft/shared/DispText.h"
#include "dungeoncraft/shared/Spell.h"
#include "dungeoncraft/shared/FormattedText.h"
#include "dungeoncraft/shared/SpellForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int pagesize=Items_Per_Page;

static const int labelsX=18;
static const int labelsY=18;

static const int itemsY=36;

// 
// all lower = text field
// all upper = field label (static)
// first upper, rest lower = highlight area

static enum ST_SPELLFORM { // Define field names as integers for quicker comparisons
  STSF_none,
  STSF_white=TEXT_FORM::white,    // White items  

  STSF_LEVEL,
  STSF_SELECT,
  STSF_MEMORIZE,
  STSF_COST,
  STSF_NAME, 

  STSF_MUAVAIL,
  STSF_muavail,
  STSF_CLERICAVAIL,
  STSF_clericavail,
  STSF_THIEFAVAIL,
  STSF_thiefavail,
  STSF_FIGHTERAVAIL,
  STSF_fighteravail,
  STSF_PALADINAVAIL,
  STSF_paladinavail,
  STSF_RANGERAVAIL,
  STSF_rangeravail,
  STSF_DRUIDAVAIL,  
  STSF_druidavail,

  STSF_level,
  STSF_select,
  STSF_memorize,
  STSF_cost,
  STSF_name,

  STSF_REPEAT,
  STSF_repeat=TEXT_FORM::autorepeat
};

#define STSF(item,xrel,x,yrel,y) \
  { DISPLAY_FORM(STSF_##xrel,STSF_##yrel,STSF_##item,x,y) },

// when using relative coords, it is imperative
// that you dont display item B relative to item A 
// before actually displaying item A.
static DISPLAY_FORM spellForm[] =
{
  //
  //   enum      xrel            x          yrel       y
  // 
  STSF(LEVEL    ,none           ,labelsX   ,none     ,labelsY)  // "LEVEL"
  STSF(SELECT   ,LEVEL+END      ,5         ,none     ,labelsY)  // "SELECTED"
  STSF(MEMORIZE ,SELECT+END     ,5         ,none     ,labelsY)  // "MEMORIZED"
  STSF(COST     ,MEMORIZE+END   ,5         ,none     ,labelsY)  // "COST"
  STSF(NAME     ,COST+END       ,40        ,none     ,labelsY)  // "SPELL"

  // TEXTBOX_WIDTH varies at runtime depending on
  // current resolution mode (640,800,1024).
  //
  // The MUAVAIL x,y values (non-relative) are updated on the fly below,
  // so these numbers don't mean anything.
  STSF(MUAVAIL      ,none          ,0         ,none             ,0)
  STSF(muavail      ,MUAVAIL+END   ,0         ,MUAVAIL          ,0)

  // the rest are drawn relative to the first pair
  STSF(CLERICAVAIL  ,MUAVAIL+RIGHT ,0         ,MUAVAIL+END      ,0)
  STSF(clericavail  ,muavail       ,0         ,CLERICAVAIL      ,0)
  STSF(THIEFAVAIL   ,MUAVAIL+RIGHT ,0         ,CLERICAVAIL+END  ,0)
  STSF(thiefavail   ,muavail       ,0         ,THIEFAVAIL       ,0)
  STSF(FIGHTERAVAIL ,MUAVAIL+RIGHT ,0         ,THIEFAVAIL+END   ,0)
  STSF(fighteravail ,muavail       ,0         ,FIGHTERAVAIL     ,0)
  STSF(PALADINAVAIL ,MUAVAIL+RIGHT ,0         ,FIGHTERAVAIL+END ,0)
  STSF(paladinavail ,muavail       ,0         ,PALADINAVAIL     ,0)
  STSF(RANGERAVAIL  ,MUAVAIL+RIGHT ,0         ,PALADINAVAIL+END ,0)
  STSF(rangeravail  ,muavail       ,0         ,RANGERAVAIL      ,0)
  STSF(DRUIDAVAIL   ,MUAVAIL+RIGHT ,0         ,RANGERAVAIL+END  ,0)
  STSF(druidavail   ,muavail       ,0         ,DRUIDAVAIL       ,0)

  // Repeat lines must be last
  // This one says, take the next 5 lines and repeat them 'pagesize' times.
  // The enums (fieldname) will be auto incremented
  STSF(REPEAT   ,repeat         ,5       ,none     ,pagesize) // auto repeat the rest

  STSF(level    ,LEVEL+RIGHT    ,0       ,none     ,itemsY)   // "1"
  STSF(select   ,SELECT+RIGHT   ,0       ,none     ,itemsY)   // "Yes"
  STSF(memorize ,MEMORIZE+RIGHT ,0       ,none     ,itemsY)   // "1"
  STSF(cost     ,COST+RIGHT     ,0       ,none     ,itemsY)   // "100"  
  STSF(name     ,NAME           ,0       ,none     ,itemsY)   // "Cure Light Wounds"
  
  STSF(none,none,0,none,0)  // End of list
};

CString BuildAvailSpellsForClass(WORD ClassFlag, SPELL_TEXT_LIST &data)
{
  BOOL NonZeroAvail = FALSE;
  CString temp;
  CString result("");

  int classindex = FlagToIndex(GetSpellCastingClassForCharClass(ClassFlag));
  
  for (int j=0;j<MAX_SPELL_LEVEL/*data.MaxLevel[classindex]*/;j++)
  {
    if (data.MaxSelect[j][classindex] > 0)
    {
      temp.Format("   %u", data.MaxSelect[j][classindex]-data.CurrSelect[j][classindex]);
      result += temp;
      NonZeroAvail = TRUE;
    }
    else
    {
      result += "   0";
    }
  }
  
  if (!NonZeroAvail)
    result.Empty();
  
  return result;
}

void showItems(TEXT_FORM *pForm, SPELL_TEXT_LIST &data)
{ 
   ASSERT(pForm!=NULL);

   // make sure that highlight flags are set after clearing form
   //pForm->ClearForm();
   
   memset(InventoryRects, 0, sizeof(InventoryRects));
   int ir_idx = 0;   

   if (data.UseMemorizeLayout())
   {
	    pForm->SetText( STSF_LEVEL,"LEVEL");
	    pForm->SetText( STSF_SELECT,"SELECT");
	    pForm->SetText( STSF_MEMORIZE,"MEMORIZED");
      pForm->SetText( STSF_COST,"");
   }
   else if (data.UseCastLayout())
   {
	    pForm->SetText( STSF_LEVEL,"LEVEL");
      pForm->SetText( STSF_SELECT,"");
	    pForm->SetText( STSF_MEMORIZE,"MEMORIZED");
      pForm->SetText( STSF_COST,"");
   }
   else if (data.UseTempleCastLayout())
   {
	    pForm->SetText( STSF_LEVEL,"LEVEL");
      pForm->SetText( STSF_SELECT,"");
	    pForm->SetText( STSF_MEMORIZE,"");
      pForm->SetText( STSF_COST,"COST");
   }
   else if (data.UseNewSpellLevelLayout())
   {
	    pForm->SetText( STSF_LEVEL,"LEVEL");
	    pForm->SetText( STSF_SELECT,"SELECT");
	    pForm->SetText( STSF_MEMORIZE,"");
      pForm->SetText( STSF_COST,"");
   }
   else
   {
	    if (data.UseLevel())
 	      pForm->SetText( STSF_LEVEL,"LEVEL");
      else
        pForm->SetText( STSF_LEVEL,"");
	    pForm->SetText( STSF_SELECT,"");
	    pForm->SetText( STSF_MEMORIZE,"");
	    if (data.UseCost())
		    pForm->SetText( STSF_COST,"COST");
      else
        pForm->SetText( STSF_COST,"");
   }
   pForm->SetText( STSF_NAME,"SPELL");

   if ((data.UseMemorizeLayout()) || (data.UseNewSpellLevelLayout()))
   {
     // display available spell slots for each spell school

     // TEXTBOX_WIDTH varies at runtime depending on
     // current resolution mode (640,800,1024)
     int mux = textboxX+(TEXTBOX_WIDTH/4);
     int muy = textboxY+10;
     pForm->SetXY(STSF_MUAVAIL, mux,    muy); // 'MAGIC USER SPELLS:'
     pForm->SetXY(STSF_muavail, 50/*mux+50*/, 0);   // '7 5 3 3 2 2 1 1'
     // ... the rest are relative to the two above fields

     WORD flag;
     CString format;

     // Magic User spells
     flag = (data.spellClasses & MagicUserFlag);
     if (flag)
     {            
       CString AvailSpells = BuildAvailSpellsForClass(flag, data);
       if (!AvailSpells.IsEmpty())
       {
         CString temp(classData[ClassFlagToCharClass(flag)].m_name);
         temp += " Spells: ";
         pForm->SetText( STSF_MUAVAIL,temp);
         pForm->SetText( STSF_muavail,AvailSpells);
       }
       else
       {
         pForm->SetText( STSF_MUAVAIL,"");
         pForm->SetText( STSF_muavail,"");
       }
     }
     else
     {
       pForm->SetText( STSF_MUAVAIL,"MAGIC USER SPELLS:");
       pForm->EnableItem(STSF_MUAVAIL, false);
       pForm->SetText( STSF_muavail,"");
     }

     // Cleric spells
     flag = (data.spellClasses & ClericFlag);
     if (flag)
     {
       CString AvailSpells = BuildAvailSpellsForClass(flag, data);
       if (!AvailSpells.IsEmpty())
       {
         CString temp(classData[ClassFlagToCharClass(flag)].m_name);
         temp += " Spells: ";
         pForm->SetText( STSF_CLERICAVAIL,temp);
         pForm->SetText( STSF_clericavail,AvailSpells);
       }
       else
       {
         pForm->SetText( STSF_CLERICAVAIL,"");
         pForm->SetText( STSF_clericavail,"");
       }
     }
     else
     {
       pForm->SetText( STSF_CLERICAVAIL,"");
       pForm->SetText( STSF_clericavail,"");
     }

     // Thief spells
     flag = (data.spellClasses & ThiefFlag);
     if (flag)
     {
       CString AvailSpells = BuildAvailSpellsForClass(flag, data);
       if (!AvailSpells.IsEmpty())
       {
         CString temp(classData[ClassFlagToCharClass(flag)].m_name);
         temp += " Spells: ";
         pForm->SetText( STSF_THIEFAVAIL,temp);
         pForm->SetText( STSF_thiefavail,AvailSpells);
       }
       else
       {
         pForm->SetText( STSF_THIEFAVAIL,"");
         pForm->SetText( STSF_thiefavail,"");
       }
     }
     else
     {
       pForm->SetText( STSF_THIEFAVAIL,"");
       pForm->SetText( STSF_thiefavail,"");
     }

     // Fighter spells
     flag = (data.spellClasses & FighterFlag);
     if (flag)
     {
       CString AvailSpells = BuildAvailSpellsForClass(flag, data);
       if (!AvailSpells.IsEmpty())
       {
         CString temp(classData[ClassFlagToCharClass(flag)].m_name);
         temp += " Spells: ";
         pForm->SetText( STSF_FIGHTERAVAIL,temp);
         pForm->SetText( STSF_fighteravail,AvailSpells);
       }
       else
       {
         pForm->SetText( STSF_FIGHTERAVAIL,"");
         pForm->SetText( STSF_fighteravail,"");
       }
     }
     else
     {
       pForm->SetText( STSF_FIGHTERAVAIL,"");
       pForm->SetText( STSF_fighteravail,"");
     }

     // Paladin spells
     flag = (data.spellClasses & PaladinFlag);
     if (flag)
     {
       CString AvailSpells = BuildAvailSpellsForClass(flag, data);
       if (!AvailSpells.IsEmpty())
       {
         CString temp(classData[ClassFlagToCharClass(flag)].m_name);
         temp += " Spells: ";
         pForm->SetText( STSF_PALADINAVAIL,temp);
         pForm->SetText( STSF_paladinavail,AvailSpells);
       }
       else
       {
         pForm->SetText( STSF_PALADINAVAIL,"");
         pForm->SetText( STSF_paladinavail,"");
       }
     }
     else
     {
       pForm->SetText( STSF_PALADINAVAIL,"");
       pForm->SetText( STSF_paladinavail,"");
     }

     // Ranger spells
     flag = (data.spellClasses & RangerFlag);
     if (flag)
     {
       CString AvailSpells = BuildAvailSpellsForClass(flag, data);
       if (!AvailSpells.IsEmpty())
       {
         CString temp(classData[ClassFlagToCharClass(flag)].m_name);
         temp += " Spells: ";
         pForm->SetText( STSF_RANGERAVAIL,temp);
         pForm->SetText( STSF_rangeravail,AvailSpells);
       }
       else
       {
         pForm->SetText( STSF_RANGERAVAIL,"");
         pForm->SetText( STSF_rangeravail,"");
       }
     }
     else
     {
       pForm->SetText( STSF_RANGERAVAIL,"");
       pForm->SetText( STSF_rangeravail,"");
     }

     // Druid spells
     flag = (data.spellClasses & DruidFlag);
     if (flag)
     {
       CString AvailSpells = BuildAvailSpellsForClass(flag, data);
       if (!AvailSpells.IsEmpty())
       {
         CString temp(classData[ClassFlagToCharClass(flag)].m_name);
         temp += " Spells: ";
         pForm->SetText( STSF_DRUIDAVAIL,temp);
         pForm->SetText( STSF_druidavail,AvailSpells);
       }
       else
       {
         pForm->SetText( STSF_DRUIDAVAIL,"");
         pForm->SetText( STSF_druidavail,"");
       }
     }
     else
     {
       pForm->SetText( STSF_DRUIDAVAIL,"");
       pForm->SetText( STSF_druidavail,"");
     }
   }
   else
   {
     pForm->SetText( STSF_MUAVAIL,"");
     pForm->SetText( STSF_muavail,"");
     pForm->SetText( STSF_CLERICAVAIL,"");
     pForm->SetText( STSF_clericavail,"");
     pForm->SetText( STSF_THIEFAVAIL,"");
     pForm->SetText( STSF_thiefavail,"");
     pForm->SetText( STSF_FIGHTERAVAIL,"");
     pForm->SetText( STSF_fighteravail,"");
     pForm->SetText( STSF_PALADINAVAIL,"");
     pForm->SetText( STSF_paladinavail,"");
     pForm->SetText( STSF_RANGERAVAIL,"");
     pForm->SetText( STSF_rangeravail,"");
     pForm->SetText( STSF_DRUIDAVAIL,"");       
     pForm->SetText( STSF_druidavail,"");
   }

   // blank out all repeat items
   for (int i=0;i<pagesize;i++)
   {
      int EnumOffset = (i*5);
      pForm->SetText(STSF_level+EnumOffset,    "");
      pForm->SetText(STSF_select+EnumOffset,   "");
      pForm->SetText(STSF_memorize+EnumOffset, "");
      pForm->SetText(STSF_cost+EnumOffset,     "");
      pForm->SetText(STSF_name+EnumOffset,     "");
   }

   int totalListItems = data.GetCount();
   int currItem = startItem;
   int displaycount = 0;
   CRect tmpRECT,totalRECT;

   while ((currItem<totalListItems) && (displaycount<pagesize))
   {      
      int EnumOffset = (displaycount*5);
      totalRECT = pForm->SetText(STSF_level+EnumOffset,  data.GetSpellLevelText(currItem));

      tmpRECT = pForm->SetText(STSF_select+EnumOffset,   data.GetSpellSelectedText(currItem));
      totalRECT.UnionRect(totalRECT, tmpRECT);

      tmpRECT = pForm->SetText(STSF_memorize+EnumOffset, data.GetSpellMemorizedText(currItem));
      totalRECT.UnionRect(totalRECT, tmpRECT);

      tmpRECT = pForm->SetText(STSF_cost+EnumOffset,     data.GetSpellCostText(currItem));
      totalRECT.UnionRect(totalRECT, tmpRECT);

      int selcnt=0;       
      int memcnt=0; 
      if (data.GetSpellSelectedText(currItem) != NULL)
        selcnt = atoi(data.GetSpellSelectedText(currItem));
      if (data.GetSpellMemorizedText(currItem) != NULL)
        memcnt = atoi(data.GetSpellMemorizedText(currItem));

      if (selcnt == 0)
        tmpRECT = pForm->SetText(STSF_name+EnumOffset,     data.GetSpellNameText(currItem), White);
      else if (selcnt == memcnt)
        tmpRECT = pForm->SetText(STSF_name+EnumOffset,     data.GetSpellNameText(currItem), Green);
      else if (selcnt != memcnt)
        tmpRECT = pForm->SetText(STSF_name+EnumOffset,     data.GetSpellNameText(currItem), Yellow);

      totalRECT.UnionRect(totalRECT, tmpRECT);

      //pForm->SetText(STSF_Row+EnumOffset, "");

      pForm->Highlight(STSF_name+EnumOffset, shouldHighlight(currItem)?true:false);
      
      CopyRect(&InventoryRects[ir_idx], &totalRECT);

      ir_idx++;
      currItem++;
      displaycount++;
   }
}

void displaySpells(SPELL_TEXT_LIST &data)
{
  TEXT_FORM SpellForm(spellForm);
  SpellForm.ClearForm();
  showItems(&SpellForm,data);
  SpellForm.Display();
}

int handleSpellFormInput(SPELL_FORM_INPUT_MESSAGE_TYPE msg,
                         SPELL_TEXT_LIST &data,
                         int P1, 
                         int P2)
// Returns 0x0001 if you should display menu and flip surfaces.
// Returns 0x0002 if we handled a keystroke and you can ignore it
{
  TEXT_FORM SpellForm(spellForm);
  int result=0;
  bool flip=false, redraw=false;

  switch (msg)
  {
  case SPELLFORM_initial:
    redraw=true;
    break;
  case SPELLFORM_refresh:
    redraw=true;
    break;
  case SPELLFORM_exit:
    break;
  default:
    ASSERT(0);
    return 0;  // Internal error
  };
  if (redraw)
  {     
    SpellForm.ClearForm();
    showItems(&SpellForm, data);  // To define rectangles.
    flip=true;
  };
  if (flip)
  {
    SpellForm.Display();
  };
  return result|(flip?1:0);
}
