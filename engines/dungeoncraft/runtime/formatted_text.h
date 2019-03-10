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

#ifndef DUNGEONCRAFT_SHARED_FORMATTED_TEXT
#define DUNGEONCRAFT_SHARED_FORMATTED_TEXT

#include "dungeoncraft/shared/GlobalData.h"

namespace DungeonCraft {

// num pixels across box where text msgs are displayed
extern int TEXTBOX_WIDTH;
// max lines of text that will be displayed at same time
extern int TEXTBOX_LINES; //DLD 12/20/00 //const int TEXTBOX_LINES = 5;  


//const int MAX_TEXT_LINES = 20;

struct TEXT_DISPLAY_DATA 
{
   TEXT_DISPLAY_DATA();
   ~TEXT_DISPLAY_DATA() { text.RemoveAll(); }
   
   BOOL NeedsFrontBuffer();
   BOOL UseSlowText();
   void Quit(void);
   
   BOOL IsFirstBox(void);
   BOOL IsLastBox(void);   
   void PrevBox(void);
   void NextBox(void);
   
   // uses different max display line count
   BOOL IsFirstJournalBox(void);
   BOOL IsLastJournalBox(void);
   void PrevJournalBox(void);
   void NextJournalBox(void);
   void FirstJournalBox(void);
   void LastJournalBox(void);
   
   BOOL highlightAll; // inverse text colors?
   BOOL slowText; // pause after each letter is displayed?
   BOOL InitialBoxDisplay; // true when box full of text is displayed for first time
   BOOL pauseText;

   int numLines;
   int currLine;
   DWORD startTime;
   CArray<CString, CString> text;
};

void FormatDisplayText(TEXT_DISPLAY_DATA &data, const char *pText, BOOL slowText=TRUE, BOOL HighlightAll=FALSE);
void FormatPausedText(TEXT_DISPLAY_DATA &data, const char *pText);

void DisplayFormattedText(TEXT_DISPLAY_DATA &data, COLORREF color=White);
void DisplayPausedText(TEXT_DISPLAY_DATA &data, COLORREF color=White);

void FormatJournalText(TEXT_DISPLAY_DATA &data, JOURNAL_DATA &jdata);
void DisplayJournalText(TEXT_DISPLAY_DATA &data, COLORREF color=White);

void FormatCombatText(TEXT_DISPLAY_DATA &data, const char *pText);

void ClearFormattedText(TEXT_DISPLAY_DATA &data);
BOOL TextIsMultiBox(TEXT_DISPLAY_DATA& data);

unsigned int GetTextBoxCharWidth();
// height of textbox area in pixels
unsigned int GetTextBoxCharHeight(void);

void PreProcessText(const char *pIn, char **pOut);
int NumDigits(int val);
void StripInvalidChars(char *pIn);

extern TEXT_DISPLAY_DATA textData;
extern TEXT_DISPLAY_DATA errorTextData;
extern TEXT_DISPLAY_DATA pausedTextData;
extern TEXT_DISPLAY_DATA journalTextData;
extern TEXT_DISPLAY_DATA combatTextData;

} // End of namespace DungeonCraft

#endif