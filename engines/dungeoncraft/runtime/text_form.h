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

#ifndef DUNGEONCRAFT_SHARED_TEXTFORM
#define DUNGEONCRAFT_SHARED_TEXTFORM

#include "dungeoncraft/shared/externs.h"

namespace DungeonCraft {

struct DISPLAY_FORM
{
  DISPLAY_FORM() { memset(this, 0, sizeof(DISPLAY_FORM)); }

  DISPLAY_FORM(int xrel, int yrel, __int16 fname,  __int16 xpos, __int16 ypos)
  {
    fieldName=fname;
    x_relative=xrel;
    x=xpos;
    y_relative=yrel;
    y=ypos;
  }

  int x_relative;  // Relative to right side
  int y_relative;  // Relative to bottom
  __int16 fieldName;   
  __int16 x; 
  __int16 y;
  __int16 left;    // Filled in when displayed
  __int16 right;   // Filled in when displayed
  __int16 top;     // Filled in when displayed
  __int16 bottom;  // Filled in when displayed
};

class TEXT_FORM
{
  class FORM_ITEM
  {
  public:
    FORM_ITEM(void) {Clear();};
    ~FORM_ITEM(void){};
    void Clear(void);
    void operator =(const DISPLAY_FORM& src) 
    {
      // Only used one place.  Might as well define it here.
      Clear();
      m_xRelative=src.x_relative;
      m_yRelative=src.y_relative;
      m_x=src.x;
      m_y=src.y;
      m_fieldName=src.fieldName;
    };
    int m_xRelative;  // Relative to right side
    int m_yRelative;  // Relative to bottom
    __int16 m_fieldName;   
    __int16 m_x; 
    __int16 m_y;
    __int16 m_left;    // Filled in when displayed
    __int16 m_right;   // Filled in when displayed
    __int16 m_top;     // Filled in when displayed
    __int16 m_bottom;  // Filled in when displayed
    CString m_text;
    COLORREF m_color;
    bool   m_highlight;
    bool   m_show;
  };

private:
  FORM_ITEM *m_formItems;
  int m_numItems;
  TEXT_FORM(); // no implementation
  TEXT_FORM& operator=(const TEXT_FORM& src); // no implementation
  TEXT_FORM(const TEXT_FORM& src); // no implementation
  FORM_ITEM *GetFormItem(int fieldName);

public:
  enum flags 
  { 
    tab        =0x100,    
    white      =0x1000,
    green      =0x2000,    
    end        =0x10000000,    
    sel        =0x20000000,
    autorepeat =0x30000000,
    right      =0x40000000
  };

  TEXT_FORM( DISPLAY_FORM *src );
  ~TEXT_FORM (void);
  void ClearForm(void);
  const RECT SetText(int fieldName, const char *text, COLORREF color=0);  
  void Highlight(int fieldName, bool highlight=true);
  void Display(void);
  void EnableItem(int fieldName,bool enable=true);
  int MouseClick(int x, int y); // Find selected item
  int Tab(int oldSelection); // Go to next tab stop
  int NumItems() const { return m_numItems; }
  int FirstSelectable();
  void SetXY(int fieldName, int x, int y);
};

#define END    (TEXT_FORM::end)
#define SEL    (TEXT_FORM::sel)
#define REPEAT (TEXT_FORM::autorepeat)
#define RIGHT  (TEXT_FORM::right)

} // End of namespace DungeonCraft

#endif
