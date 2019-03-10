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

#ifndef DUNGEONCRAFT_SHARED_GPDLCOMP
#define DUNGEONCRAFT_SHARED_GPDLCOMP

namespace DungeonCraft {

class ASL_VALUE {
	friend class ASL;
  friend class CAR;
	friend void AFXAPI SerializeElements(class CArchive &ar,
		                                 class ASL_VALUE *atr,
										 int count);
	unsigned char m_flags;
	CString m_value;
public:
	ASL_VALUE(void);
	~ASL_VALUE(void);
	ASL_VALUE(const CString& str,unsigned char flags);
  CString Value(void) const { return m_value;};
  bool operator ==(const ASL_VALUE& atv) const;
  int GetDataSize(void);
};


#define ASLF_READONLY     1  // Can't be deleted or changed or saved.  READONLY
                            //  attributes cannot be created during gameplay.
							//  So ASLF_READONLY implies ATF_DESIGN.
#define ASLF_MODIFIED     2  // Changed during gameplay.  The initial insertion
                            //  of an attribute during gameplay does not set
                            //   this bit
#define ASLF_DESIGN       4  // Created by UAFWinEd.  Info only.  Not used.
#define ASLF_SYSTEM       8  // System attribute.  Info only.  Not used.


// just to save typing the two flags throughout the editor code
#define ASLF_EDITOR (ASLF_READONLY | ASLF_DESIGN)

// ASLs will be saved in the .PTY files unless READONLY.
// When restoring a game, all ASLs except READONLY will be discarded before
//  reading the savefile.

//class CAR;

class ASL {
  friend class CAR;
private:
	CString m_mapName;
	CMap<CString, LPCSTR, ASL_VALUE, ASL_VALUE&>	m_aslMap;

  ASL() { m_mapName=""; } // not used
public:
	ASL(LPCSTR mapName);
	~ASL(void);
	CArchive& Serialize(CArchive& ar);
  CAR& Serialize(CAR& car);
	CArchive& Save(CArchive& ar); // Serialize if not ASLF_READONLY
	void CommitRestore(const ASL& src);
	void Insert(LPCSTR key, CString& value, unsigned char flags);
	void Insert(LPCSTR key, const ASL_VALUE& attr);
	void Insert(LPCSTR key, LPCSTR value, unsigned char flags);
#ifdef UAFEDITOR
  // editor only - ignores read-only flags during insertion
  void Update(LPCSTR key, LPCSTR value, unsigned char flags);
#endif
	void Delete(LPCSTR key);
	const CString& Lookup(LPCSTR key) const;
	BOOL           Lookup(LPCSTR key, ASL_VALUE& value);
	BOOL Exists(LPCSTR key) const;
	int GetFlags(LPCSTR key); // -1 if item does not exist.
	ASL& operator = (const ASL& attr);
  bool operator == (const ASL& attr) const;
	void Clear(void);
  int GetDataSize(void);
  CString GetName() const { return m_mapName; }
  int GetCount() const { return m_aslMap.GetCount(); }
  POSITION GetStartPosition() const { return m_aslMap.GetStartPosition(); }
  void GetNextAssoc(POSITION &pos, CString &key, CString &value) const
  {
	  ASL_VALUE data;
    m_aslMap.GetNextAssoc(pos, key, data);
    value = data.m_value;
  }
};

//lint -e762
template <> void AFXAPI SerializeElements<ASL_VALUE>(CArchive& ar, 
															ASL_VALUE* val,
															int count);
//lint +e762

// store/retrieve data from asl maps
void StorePicDataAsASL(ASL &asl, PIC_DATA& data, const char *prefix, unsigned char flags=ASLF_EDITOR);
void RetrievePicDataFromASL(ASL &asl, PIC_DATA& data, const char *prefix);
void StoreIntAsASL(ASL &asl, int data, const char *prefix, unsigned char flags=ASLF_EDITOR);
void RetrieveIntFromASL(ASL &asl, int &data, const char *prefix);
void StoreStringAsASL(ASL &asl, CString &data, const char *prefix, unsigned char flags=ASLF_EDITOR);
void RetrieveStringFromASL(ASL &asl, CString &data, const char *prefix);




// A class to manipulate delimited strings.
// Such strings are of the format:
//    "5.first6.second5.third"
// A DELIMITED_STRING may contain multiple occurances of the same
//  substring:
//     "5.first28.A somewhat longer sub-string5.first"
// A DELIMITED_STRING can contain empty substrings:
//     "5.first0.5.third"
// But an empty DELIMITED_STRING contains no strings...not even empty ones.
//
// The numbers (followed by a decimal point) define the length of
// each sub-string in decimal.

// Why all this fuss????  So we can put multiple strings in a single
// attribute.  The first use was to list allowable MultiClass values
// for a race.  And it really is not that much of a fuss.
class DELIMITED_STRING
{
private:
  CString m_string;
public:
  DELIMITED_STRING(void);
  ~DELIMITED_STRING(void);
  void operator = (const CString& str);
  bool Contains(const CString& str) const;
  bool IsLegal(void) const;
  int Count(void) const;
  CString GetAt(int index) const;
  CString Fetch(int index) const;
  CString Result(void); // The resulting delimited string
  void Insert(const CString& str);  //Inserts at the end
  void Delete(const CString& str);
};

} // End of namespace DungeonCraft

#endif
