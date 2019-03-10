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
#include "dungeoncraft/shared/TextForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void TEXT_FORM::FORM_ITEM::Clear(void)
{
  m_left=m_top=m_bottom=m_right=-1;
  m_color=0xffffffff;
  m_text="";
  m_highlight=false;
  m_show=true;
};

TEXT_FORM::TEXT_FORM (DISPLAY_FORM *src) 
{
  DISPLAY_FORM *pdf;
  int i;
  m_numItems=0;
  //for (pdf=src; pdf->fieldName!=0; pdf++, m_numItems++) 
  //  /* empty loop */;  
  for (pdf=src; pdf->fieldName!=0; pdf++) 
  {
    if (pdf->x_relative == autorepeat)
    {
      int repeatcount = pdf->y; // how many repeats
      int repeatitems = pdf->x; // nbr following items to repeat
      m_numItems += (repeatcount * repeatitems);
      pdf += repeatitems; // skip over repeats
    }
    else
      m_numItems++;
  }

  m_formItems=new FORM_ITEM[m_numItems];
  int srcidx=0;
  for (i=0; i<m_numItems; i++) 
  {
    if (src[srcidx].x_relative == autorepeat)
    {
      // This is not very general code. It was created for the
      // specific purpose of automatically creating 
      // place holders for vertically displayed lists
      // of data such as inventory and shop items.
      //
      int repeatcount = src[i].y; // how many repeats
      int repeatitems = src[i].x; // nbr following items to repeat
      srcidx++; // skip past repeat indicator      

      for (int rows=0;rows<repeatcount;rows++)
      {      
        for (int items=0;items<repeatitems;items++)
        {
          m_formItems[i]=src[srcidx+items];
          m_formItems[i].m_fieldName += (repeatitems*rows); // auto-inc the field id

          if (src[srcidx+items].x_relative & sel)
          {
            m_formItems[i].m_xRelative = m_formItems[i-(repeatitems-1)].m_fieldName;
            m_formItems[i].m_yRelative = m_formItems[i-1].m_fieldName;
          }
          else
          {
            // first row uses given coords,
            // the rest are relative to the previous
            if (rows>0)
            {
              m_formItems[i].m_y=0;
              m_formItems[i].m_yRelative=m_formItems[i-repeatitems].m_fieldName+end;
            }
          }
          i++;
        }
      }
      srcidx += repeatitems; // skip over repeats
    }
    else
    {
      m_formItems[i]=src[srcidx];
      srcidx++;
    }
  }
}

TEXT_FORM::~TEXT_FORM(void) 
{
  if (m_formItems!=NULL)
  {
    delete [] m_formItems;
    m_formItems=NULL;
  };
}


void TEXT_FORM::ClearForm(void)
{
  int i;
  for (i=0; i<m_numItems; i++) m_formItems[i].Clear();
}

TEXT_FORM::FORM_ITEM *TEXT_FORM::GetFormItem(int fieldName)
{
  FORM_ITEM *curform;
  for (int i=0; i<m_numItems; i++)
  {
    curform=m_formItems+i;
    if (fieldName==m_formItems[i].m_fieldName)
      return (&m_formItems[i]);
  }
  return NULL;
}

void TEXT_FORM::SetXY(int fieldName, int x, int y)
{
  FORM_ITEM *curform = GetFormItem(fieldName);
  if (curform==NULL) return;
  curform->m_x = x;
  curform->m_y = y;
}


const RECT TEXT_FORM::SetText(
          int fieldName,
          const char *text,
          COLORREF color)
{
  RECT rect={0,0,0,0};
  FORM_ITEM *curform, *relform;
  int relx, rely;
  int f;
  int i, j;
  for (i=0; i<m_numItems; i++)
  {
    curform=m_formItems+i;
    if (fieldName==curform->m_fieldName)
    {
      relx=0;
      if (curform->m_xRelative!=0)  //Is x relative to another field?
      {
        f=curform->m_xRelative & 0xffffff; // The other field's name.
        for (j=0; j<m_numItems; j++)  // Search for the other field.
        {
          relform=m_formItems+j;
          if (relform->m_fieldName==f)
          { //We found the field x is relative to.
            ASSERT (relform->m_left>=0); //It better hava placement already.
            if (curform->m_xRelative&sel) 
            { //
              curform->m_top=relform->m_top;
              curform->m_left=relform->m_left;
            }
            else if (curform->m_xRelative&end) 
              relx=relform->m_right;
            else if (curform->m_xRelative&right)
              relx=relform->m_right-GraphicsMgr.GetTextWidth(text,-1, curform->m_highlight);
            else 
              relx=relform->m_left;
            break;
          };
        };
      };

      rely=0;
      if (curform->m_yRelative!=0) //Is y relative to another field?
      {
        f=curform->m_yRelative & 0xffffff; //The other field's name
        for (j=0; j<m_numItems; j++)
        { //Search for the field we are relative to.
          relform=m_formItems+j;
          if (relform->m_fieldName==f)
          {
            ASSERT (relform->m_top>=0); //The other field better have a place!
            if (curform->m_yRelative&sel) 
            {
              curform->m_bottom=relform->m_bottom;
              curform->m_right=relform->m_right;
            }
            else if (curform->m_yRelative & end) rely=relform->m_bottom;
            else rely=relform->m_top;
            break;
          };
        };
      };
      if ( (curform->m_xRelative & sel) ==0)
          curform->m_left=curform->m_x + relx;
      if ( (curform->m_yRelative & sel) ==0)
          curform->m_top=curform->m_y + rely;
      if (color==0)
      {
        switch (curform->m_fieldName&0xf000)
        {
        case white: color=White; break;
        case green: color=Green; break;
        default:
          color=White;
        };
      };
      if (text!=NULL)
        curform->m_text=text;
      else
        curform->m_text="";
      curform->m_color=color;

      // make sure that highlight flags are set after clearing form
      if ( (curform->m_xRelative & sel) ==0)
          curform->m_right=curform->m_left+GraphicsMgr.GetTextWidth(text,-1, curform->m_highlight);
      if ( (curform->m_yRelative & sel) ==0)
          curform->m_bottom=curform->m_top+GraphicsMgr.GetCharacterHeight('H', curform->m_highlight);
      rect.top=curform->m_top;
      rect.bottom=curform->m_bottom;
      rect.left=curform->m_left;
      rect.right=curform->m_right;
      return rect;
    };
  };
  // An error here!
  return rect;
}

// make sure that highlight flags are set after clearing form
void TEXT_FORM::Highlight(int fieldName, bool highlight) 
{
  FORM_ITEM *curform = GetFormItem(fieldName);
  if (curform==NULL) return;
  curform->m_highlight=highlight;
}

void TEXT_FORM::EnableItem(int fieldName,bool enable)
{
  FORM_ITEM *curform = GetFormItem(fieldName);
  if (curform==NULL) return;
  curform->m_show = enable;
}

int TEXT_FORM::Tab(int oldSelection)
{
  int i;
  if (oldSelection==-1) i=0; // where to start
  else
  {
    for (i=0; i<m_numItems; i++)
    {
      if (m_formItems[i].m_fieldName==oldSelection) break;
    };
    if (i==m_numItems) return -1; // Internal error
    i++;  // Where to start
  };
  while ( (m_formItems[i].m_fieldName & tab) ==0)
  {
    i++;
    if (i==m_numItems)
    {
      if (oldSelection==-1) return -1;
      i=0;
    };
    if (m_formItems[i].m_fieldName==oldSelection)
    {
      return oldSelection;  // There must be only one.
    };
  };
  return m_formItems[i].m_fieldName;
}




int TEXT_FORM::MouseClick(int x, int y)
{
  // Returns the largest (in area) selection;
  int i;
  int selection=-1;
  int area=0, testarea;
  FORM_ITEM *f;
  for (i=0; i<m_numItems; i++)
  {
    f=m_formItems+i;
    if (x < f->m_left)   continue;
    if (x > f->m_right)  continue;
    if (y < f->m_top)    continue;
    if (y > f->m_bottom) continue;
    testarea=(f->m_right-f->m_left)*(f->m_bottom-f->m_top);
    if (testarea>area)
    {
      selection=f->m_fieldName;
      area=testarea;
    };
  };
  return selection;
}

void TEXT_FORM::Display(void)
{
  BOOL CurrFontTagSetting = GraphicsMgr.GetEnableFontColorTags();
  GraphicsMgr.EnableFontColorTags(FALSE);
  int i;
  FORM_ITEM *f;
  for (i=0; i<m_numItems; i++)
  {
    f=m_formItems+i;
    if (f->m_text.GetLength()==0) continue;
    if (f->m_color==0xffffffff) continue;
    if (!f->m_show) continue;
    displayText(f->m_left,
                f->m_top,
                f->m_text,
                f->m_color,
                f->m_highlight);
  };
  GraphicsMgr.EnableFontColorTags(CurrFontTagSetting);
}

int TEXT_FORM::FirstSelectable()
{
  for (int i=0; i<m_numItems; i++)
  {
    if (m_formItems[i].m_xRelative & sel)
      return m_formItems[i].m_fieldName;
  }
  return -1;
}
