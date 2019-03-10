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

#ifndef DUNGEONCRAFT_SHARED_FILEPARSE
#define DUNGEONCRAFT_SHARED_FILEPARSE

#include "dungeoncraft/shared/externs.h"

namespace DungeonCraft {

class FileParse
{
public:
  FileParse() { m_tlist.RemoveAll(); m_fileError=""; m_filename="No File Opened"; m_tempToken[0]='\0'; }
  ~FileParse() { Clear(); }

  FileParse& operator =(const FileParse& src);

  CString m_fileError;

  BOOL Parse(const char *filename, int SearchDepth=4);
  void Clear() { if (m_file.m_hFile != CFile::hFileNull) m_file.Close(); m_tlist.RemoveAll(); m_fileError=""; }
  CString GetFilename() { return m_filename; }

  int GetCount() { return m_tlist.GetSize(); }
  BOOL GetTokenAt(int index, CString &Token, CString &Value);
  BOOL FindToken(CString Token, char *Value, int ValueLen, bool Remove=TRUE);
  BOOL FindToken(CString Token, CString &Value, bool Remove=TRUE);  
  BOOL FindToken(CString Token, DWORD &Value, bool Remove=TRUE);
  BOOL FindToken(CString Token, WORD &Value, bool Remove=TRUE);
  BOOL FindToken(CString Token, BYTE &Value, bool Remove=TRUE);
  BOOL FindToken(CString Token, int &Value, bool Remove=TRUE);

  BOOL FindTokens(CString Token, int &Val1, int &Val2, int &Val3, int &Val4, bool Remove=TRUE);
  BOOL FindTokens(CString Token, int &Val1, int &Val2, int &val3, bool Remove=TRUE);
  BOOL FindTokens(CString Token, int &Val1, int &Val2, bool Remove=TRUE);

  DWORD FlushTokens();

  enum { MAX_VALUE_LEN = 255 };

private:
  CStdioFile m_file;
  CString m_filename;
  char m_tempToken[MAX_VALUE_LEN+1];

  void SetFileError(CFileException* pFE);

  struct TokenDataType
  {
    TokenDataType() { Token="";Value="";Remove=FALSE; }
    TokenDataType& operator =(const TokenDataType& src)
    {
      if (&src == this)
        return *this;
      Token = src.Token;
      Value = src.Value;
      Remove=src.Remove;
      return *this;
    }
    
    CString Token;
    CString Value;
    bool Remove;
  };

  CArray<TokenDataType, TokenDataType&> m_tlist;
};

//extern FileParse ConfigFile;

BOOL FindToken(FileParse& cfile, CString Token, CString &Value);

} // End of namespace DungeonCraft

#endif
