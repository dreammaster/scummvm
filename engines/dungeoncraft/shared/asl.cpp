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

#include "dungeoncraft/shared/asl.h"

namespace DungeonCraft {

static CString emptyString="";


bool ASL_VALUE::operator ==(const ASL_VALUE& atv) const
{
  if (m_flags!=atv.m_flags) return false;
  if (m_value!=atv.m_value) return false;
  return true;
}

int ASL_VALUE::GetDataSize(void)
{
  return sizeof(ASL_VALUE) + m_value.GetLength();
}

ASL::ASL(LPCSTR mapName) {
	m_mapName=mapName;
  if (m_mapName=="") 
  { 
    m_mapName="EmptyMapName";
    ASSERT(FALSE); // should pass a valid asl name string
  }
}

ASL::~ASL(void) 
{
  try 
  {
    m_aslMap.RemoveAll();
  }
  catch (...)
  {
    // do nothing
  }
}

int ASL::GetDataSize(void)
{
  int size = sizeof (m_mapName) + m_mapName.GetLength();
  POSITION pos;
  CString key;
  ASL_VALUE value;
  pos=m_aslMap.GetStartPosition();
  while (pos!=NULL)
  {
    m_aslMap.GetNextAssoc(pos,key,value);
    size += key.GetLength() + value.GetDataSize();
  };
  return size;
}

void ASL::Insert(LPCSTR key, LPCSTR value, unsigned char flags) {
	CString temp;
  int oldflags=-1;
  static int errorcount=0;
	temp=value;
  
  ASSERT(strlen(key)>0);
  if (strlen(key)==0)
  {
    WriteDebugString("Error: Attempt to insert empty key name into ASL ignored\n");
    errorcount++;
    return;
  }
  if (temp.IsEmpty()) 
    WriteDebugString("Warning: Inserting empty value ASL for key \'%s\'\n", key);

  if ( (flags&(ASLF_READONLY|ASLF_SYSTEM)) == 0)
  { // Don't allow user to write on readonly values.
    oldflags=GetFlags(key);
    if ((oldflags>=0)&&((oldflags&ASLF_READONLY)!=0))
    {
      if (errorcount<2)
      {
        CString msg;
        msg.Format("Error: Attempt to modify read-only ASL entry = \"%s\"",key);
        WriteDebugString(msg);
        errorcount++;
        return;
      };
    };
  };
  if (oldflags>=0) flags=ASLF_MODIFIED;
  ASL_VALUE newAV(temp,flags);
	m_aslMap.SetAt(key, newAV);
}

#ifdef UAFEDITOR
void ASL::Update(LPCSTR key, LPCSTR value, unsigned char flags) 
{
  ASSERT(strlen(key)>0);
  if (strlen(key)==0)
  {
    WriteDebugString("Warning: Attempt to update asl with empty key name ignored\n");
    //errorcount++;
    return;
  }
  if (strlen(value)==0) 
    WriteDebugString("Warning: Updating asl key \'%s\' with empty value\n", key);

  ASL_VALUE newAV(value,flags);
	m_aslMap.SetAt(key, newAV);
}
#endif

ASL& ASL::operator =(const ASL& asl) 
{
	POSITION p;
	CString key;
	ASL_VALUE value;
  if (&asl==this) return *this;
	m_aslMap.RemoveAll();
	m_mapName=asl.m_mapName;
	p=asl.m_aslMap.GetStartPosition();
	while (p!=NULL) {
		asl.m_aslMap.GetNextAssoc(p, key, value);
		m_aslMap.SetAt(key,value);
	};
  return *this;
}


bool ASL::operator ==(const ASL& asl) const {
	POSITION p1, p2;
	CString key1, key2;
	ASL_VALUE value1, value2;
	if (m_mapName!=asl.m_mapName) return false;
  p1=m_aslMap.GetStartPosition();
	while (p1!=NULL) {
		m_aslMap.GetNextAssoc(p1, key1, value1);
    if (!asl.m_aslMap.Lookup(key1,value2)) return false;
    if (!(value1==value2)) return false;
	};
	p2=asl.m_aslMap.GetStartPosition();
	while (p2!=NULL) {
		asl.m_aslMap.GetNextAssoc(p2, key2, value2);
    if (!m_aslMap.Lookup(key2,value1)) return false;
    if (!(value1==value2)) return false;
	};
  return true;
}

void ASL::Delete(LPCSTR key) {
	m_aslMap.RemoveKey(key);
}

void ASL::Clear(void) {
	m_aslMap.RemoveAll();
}

CArchive& ASL::Save(CArchive& ar)
// Serialize non-READONLY entries in ASL..
{
	int count=0;
	POSITION pos;
	CString tempkey;
	ASL_VALUE tempvalue;
	ar << m_mapName;
	pos=m_aslMap.GetStartPosition();
	while (pos!=NULL)
	{ 
		m_aslMap.GetNextAssoc(pos,tempkey,tempvalue);
		if ((tempvalue.m_flags & ASLF_READONLY)==0) count++;
	};
	ar.WriteCount(count);
  if (count!=0) // Some ASLs contain only read-only entries.
  {             // This saves the overhead of searching through the 
                // list twice.  It probably ain't a big deal either way.
	  pos=m_aslMap.GetStartPosition();
	  while (pos!=NULL) 
	  {
		  m_aslMap.GetNextAssoc(pos,tempkey,tempvalue);
		  if ((tempvalue.m_flags & ASLF_READONLY)==0)
		  {
        // I'm not going to check for empty string.  I don't believe
        // that it can cause a problem.  Mr. Turner has yet to 
        // convince me.  Besides, if empty strings are a problem then
        // we have to rewrite ASL::Serialize(CArchive& ar)
        // because it uses the CMap::Serialize which does not check
        // for empty strings.  So there!  PRS
			  ar << tempkey;
			  SerializeElements(ar, &tempvalue, 1);
        count--;
		  };
	  };
  };
  ASSERT(count==0);
	return ar;
}


CArchive& ASL::Serialize(CArchive& ar) {
	if (GetGameVersion(globalData) >= _ASL_LEVEL_) {
		if (ar.IsStoring()) {
			ar << m_mapName;
		} else {
			CString temp;
			ar >> temp;
			if (temp!=m_mapName) {
				CString msg;
				msg.Format("Expected:\n%s\nEncountered:\n%s\nwhile reading file.",
					m_mapName,temp);
        WriteDebugString(msg);
				MsgBoxError(msg);
 				AfxThrowArchiveException(CArchiveException::badIndex,
                  ar.GetFile()->GetFileName());
			};
		};
		m_aslMap.Serialize(ar);
	};
	return ar;
}

BOOL ASL::Exists(LPCSTR key) const{
	ASL_VALUE junk;
	return m_aslMap.Lookup(key, junk);
}

int ASL::GetFlags(LPCSTR key) {
	static ASL_VALUE temp;
	if (m_aslMap.Lookup(key, temp)) return temp.m_flags;
	return -1;
}

const CString& ASL::Lookup(LPCTSTR key) const{
	static ASL_VALUE temp;
	if (m_aslMap.Lookup(key, temp)) return temp.m_value;
	return emptyString;
}

BOOL ASL::Lookup(LPCSTR key, ASL_VALUE& value)
{
  return m_aslMap.Lookup(key,value);
}

void ASL::Insert(LPCSTR key, CString& value, unsigned char flags) 
{
  //ASSERT(value.GetLength()>0);
  //if (value.GetLength()==0) return;
  ASL_VALUE temp(value, flags);
	m_aslMap.SetAt(key, temp);
}


ASL_VALUE::ASL_VALUE(void) {
	m_flags=0;
}

ASL_VALUE::ASL_VALUE(const CString& value, unsigned char flags) {
	m_value=value;
	m_flags=flags;
}

ASL_VALUE::~ASL_VALUE(void) {
}


void ASL::CommitRestore(const ASL& src) 
{
	// Delete current non-READONLY attributes and add all
	// non-READONLY attributes from src.
	POSITION pos;
	ASL_VALUE tempvalue;
	CString tempkey;
	pos=m_aslMap.GetStartPosition();
	// Discard all current non-READONLY entries before adding
	// the entries from src.
	while (pos!=NULL) 
	{
		m_aslMap.GetNextAssoc(pos, tempkey, tempvalue);
		if ((tempvalue.m_flags & ASLF_READONLY)==0)
		{  // Retain only the READONLY entries
			m_aslMap.RemoveKey(tempkey);
		};
	};
	// Now copy all non-READONLY attributes from src.
	pos=src.m_aslMap.GetStartPosition();
	while (pos!=NULL) 
	{
		src.m_aslMap.GetNextAssoc(pos,tempkey,tempvalue);
		if ((tempvalue.m_flags & ASLF_READONLY)==0)
		{
			m_aslMap.SetAt(tempkey, tempvalue);
		};
	};
}


void AFXAPI SerializeElements(class CArchive &ar,class ASL_VALUE *av,int count) {
	int i;
	for (i=0; i<count; i++)
	{
		if (ar.IsStoring()) 
		{
			if (GetGameVersion(globalData) >= _EXTENDED_ASL_LEVEL_)
			{
				ar << av[i].m_flags;
			}
			ar << av[i].m_value;
		}
		else 
		{
			if (GetGameVersion(globalData) >= _EXTENDED_ASL_LEVEL_)
			{
				ar >> av[i].m_flags;
			}
			else
			{
#if defined UAFEDITOR
				// We assume that any ASL entries in the design files before
				// this version were put there by Mr. Turner and are
				// READONLY, SYSTEM ENTRIES.  Nobody else knew how to 
				// put them there except me and I didn't do it..
				av[i].m_flags=ASLF_SYSTEM | ASLF_READONLY | ASLF_DESIGN;
#else
				av[i].m_flags=0;
#endif // defined UAFEDITOR
			};
      DAS(ar,av[i].m_value);
//			ar >> atr[i].m_value;
//      if (atr[i].m_value=="*") atr[i].m_value="";
		};
	};
}


void StoreIntAsASL(ASL &asl, int data, const char *prefix, unsigned char flags)
{
  CString val;
  CString key;
  key.Format("%s", prefix);
  val.Format("%i", data);
  asl.Insert(key, val, flags);
}

void RetrieveIntFromASL(ASL &asl, int &data, const char *prefix)
{
  CString key;
  CString val;
  key.Format("%s", prefix);
  val = asl.Lookup(key);
  data = atoi(val);
}

void StoreStringAsASL(ASL &asl, CString &data, const char *prefix, unsigned char flags)
{
  CString val;
  CString key;
  key.Format("%s", prefix);
  val = data;
  asl.Insert(key, val, flags);
}

void RetrieveStringFromASL(ASL &asl, CString &data, const char *prefix)
{
  CString key;
  CString val;
  key.Format("%s", prefix);
  val = asl.Lookup(key);
  data = val;
}

DELIMITED_STRING::DELIMITED_STRING(void)
{
  m_string = "";
}

DELIMITED_STRING::~DELIMITED_STRING(void)
{
  //That was easy.
}

void DELIMITED_STRING::operator = (const CString& str)
{
  m_string = str;
}


bool DELIMITED_STRING::IsLegal(void) const
{
  const char *str, *end;
  int len, sublen;
  len=m_string.GetLength();
  if (len==0) return true;
  str = LPCTSTR(m_string);
  end = str + len; //Last character + 1 
  while (str < end)
  {
    sublen = 0;
    while (*str != '.')
    {
      if ( (*str<'0') || (*str>'9')) return false;
      sublen = 10*sublen + (*str - '0');
      str++;
      if (str >= end) return false;
    };
    str += sublen+1; //Bypass the period, too.
  };
  if (str > end) return false;
  return true;
}

int DELIMITED_STRING::Count(void) const
{
  const char *str, *end;
  int len, sublen, count;
  len=m_string.GetLength();
  if (len==0) return 0;
  str = LPCTSTR(m_string);
  end = str + len; //Last character + 1 
  count = 0;
  while (str < end)
  {
    sublen = 0;
    while (*str != '.')
    {
      if ( (*str<'0') || (*str>'9')) return count;
      sublen = 10*sublen + (*str - '0');
      str++;
      if (str >= end) return count;
    };
    str += sublen+1; //Bypass the period, too.
    count++;
  };
  if (str > end) return count-1;
  return count;
}

CString DELIMITED_STRING::GetAt(int index) const
{
  const char *str, *end;
  int len, sublen;
  len=m_string.GetLength();
  if (len == 0) return CString("");
  str = LPCTSTR(m_string);
  end = str + len; //Last character + 1 
  while (str < end)
  {
    sublen = 0;
    while (*str != '.')
    {
      if ( (*str<'0') || (*str>'9')) return CString("");
      sublen = 10*sublen + (*str - '0');
      str++;
      if (str >= end) return CString("");
    };
    if (index==0) return CString(str+1, sublen);
    str += sublen+1; //Bypass the period, too.
    index--;
  };
  return CString("");
}


bool DELIMITED_STRING::Contains(const CString& cstr) const
{
  const char *str, *end;
  int len, stringlen, sublen;
  len=m_string.GetLength();
  if (len==0) return false;
  stringlen = cstr.GetLength();
  str = LPCTSTR(m_string);
  end = str + len; //Last character + 1 
  while (str < end)
  {
    sublen = 0;
    while (*str != '.')
    {
      if ( (*str<'0') || (*str>'9')) return false;
      sublen = 10*sublen + (*str - '0');
      str++;
      if (str >= end) return false;
    };
    if (   (sublen == stringlen)
         &&(memcmp(LPCTSTR(cstr),str+1,sublen)==0) ) return true;
    str += sublen+1; //Bypass the period, too.
  };
  return false;
}

CString DELIMITED_STRING::Result(void)
{
  return m_string;
}

void DELIMITED_STRING::Insert(const CString& str)
{
  CString newstr;
  newstr.Format("%d.%s",str.GetLength(),str);
  m_string = m_string + newstr;
}

} // End of namespace DungeonCraft
