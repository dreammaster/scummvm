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

#ifndef __GPDLEXEC_H__
#define __GPDLEXEC_H__

// 19 Apr PRS #include "dungeoncraft/shared/GPDLCode.h"
#include "dungeoncraft/shared/RegExp.h"

namespace DungeonCraft {

#define DOWN_ARROW      0x28
#define UP_ARROW        0x26
#define CARRIAGE_RETURN 0x0d
#define BACKSPACE       0x08
#define SPACE           0x20


class GPDL_EVENT;
class GPDLCOMP;

enum GPDL_STATE { // Also used for event return values.

	// Internal states.  These are the only possible states that
	//  will be returned by State().  
	GPDL_UNINITIALIZED=1,
	GPDL_IDLE,
	GPDL_WAIT_INPUT,
	GPDL_WAIT_ACK,

	// Return status values
	GPDL_OK,
	GPDL_ACCEPTED,
	GPDL_IGNORED,
	GPDL_NOSUCHNAME,
	GPDL_EVENT_ERROR,
	GPDL_READ_ERROR,
	GPDL_OVER_RP,
	GPDL_UNDER_RP,
	GPDL_OVER_SP,
	GPDL_UNDER_SP,
	GPDL_GREPERROR,
	GPDL_ILLPARAM,
	GPDL_BADINTEGER,
	GPDL_ILLFUNC,     // Illegal cell at entry to function
	GPDL_ILLCHARNUM

};



class GPDL {
#define DBG_messageBox	  1
#define DBG_functionTrace 2
	class ERRORLOG {
	private:
		FILE   *m_file;
		bool	m_messagePrinted;
	public:
		ERRORLOG(void);
		~ERRORLOG(void);
		void log(CString& msg);
		void log(const char *msg);
	};
  class CONSTANTS {
  private:
	  CString			*m_value;
	  unsigned int	 m_allocatedSize;
  public:
	  CONSTANTS (void);
	  ~CONSTANTS(void);
	  int read(CArchive& ar);
#ifdef UAFEDITOR
    int read(GPDLCOMP *pGPDL);
#endif
	  CString &peek(unsigned int index) {return m_value[index];};
  };
  class INDEX {
  private:
	  CString			*m_name;
	  unsigned int	*m_value;
	  unsigned int	 m_allocatedSize;
  public:
	  INDEX (void);
	  ~INDEX(void);
	  int read(CArchive& ar);
#ifdef UAFEDITOR
    int read(GPDLCOMP *pGPDL);
#endif
	  unsigned int numEntry(void) { return m_allocatedSize;};
	  CString &name(unsigned int index) {return m_name[index];};
	  unsigned int lookup(const CString& funcName);
  };
  class DISCOURSETEXT {
  private:
	  class LINE {
	  public:
		  CString		m_text;
		  LINE	   *m_nextLine;  // or NULL
		  LINE	   *m_prevLine;  // Or NULL
		  LINE(const char *line);
		  ~LINE(void);
		  void		deleteLastChar(unsigned int backspaceLimit);
		  void		addOneChar(UINT c);
	  };
	  //unsigned int	m_MAXLINES; // Set in constructor
	  unsigned int	m_MAXCHARS; // Set in constructor
	  unsigned int	m_promptLen;  // To avoid backspacing over prompt
	  bool			m_listening;
	  LINE		   *m_firstLine;
	  unsigned int	m_numLine;
	  unsigned int	m_topLine;
	  void			m_format(CString& text, unsigned int width, bool colorTags=true);
	  void			m_addLine(const char *line);
  public:
	  DISCOURSETEXT(void);
	  ~DISCOURSETEXT(void);
	  void			Display(void);
	  void			SetText(CString& text, unsigned int width);
	  void			SetNewTop(void); // Set top to text that follows.
	  void			StartInput(void);
	  void			downArrow(void);
	  void			upArrow(void);
	  GPDL_STATE		Char(UINT c); // Return GPDL_ACCEPTED if character accepted
	  void			Clear(void);
	  CString			GetInput(void); // Get result of input operation;
  };
private:
//	19 Apr PRS CODE			m_code;
  unsigned int *m_program;
	CONSTANTS		m_constants;
	INDEX			m_index;
	unsigned int	m_PC;
	unsigned int	m_FP;
	unsigned int	m_SP;
	unsigned int	m_RP;
	unsigned int	m_RP0;
	unsigned int	m_SP0;
  unsigned int  m_H;
  unsigned int  m_programAllocated; // Allocated for program
  unsigned int  m_executionAddress; // latest Opcode fetch address
	GPDL_STATE		m_state;
	GPDL_STATE		m_status;
	unsigned int	m_debugLevel;
	//unsigned int	m_char_pixels_per_line; // Initialized in constructor and
											// modified by $TEXTWIDTH(n);
  GPDL_EVENT *m_event;
	ERRORLOG		m_errorLog;
//	VARIABLES		m_variables;
	GPDL_STATE		m_interpret(void);
  GPDL_STATE    ReadProgram(CArchive& ar);
#ifdef UAFEDITOR
  GPDL_STATE    ReadProgram(GPDLCOMP *pGPDL);
#endif
  unsigned int Peek(unsigned int address)
  {
    ASSERT(address < m_H);
    return m_program[address];
  };
	void			m_pushRP(unsigned int n);
	unsigned int	m_popRP(void);
	void			m_pushSP(CString& val);
	CString			m_popSP (void);
	void			m_popPC(void);
	void			m_pushPC(void);
	void			m_popFP(void);
	void			m_pushFP(void);
	void			m_pushString1(void);
	void			m_popString1(void);
	void			m_pushString2(void);
	void			m_popString2(void);  
	void			m_pushString3(void);
  void      m_popString3(void);
	void			m_pushString4(void);
  void      m_popString4(void);
	GPDL_STATE		m_popInteger(bool& neg, unsigned int& n);
	void			m_popInteger1(GPDL_STATE errorstatus=GPDL_UNDER_SP);
	void			m_popInteger2(GPDL_STATE errorstatus=GPDL_UNDER_SP);
	void			m_popInteger3(GPDL_STATE errorstatus=GPDL_UNDER_SP);
	void			m_pushInteger1(void);
	void			m_pushInteger2(void);
	void			m_pushInteger3(void);
	void			m_pushUInteger1(void);
	void			m_popUInteger1(GPDL_STATE errorstatus=GPDL_UNDER_SP);
	void			m_pushUInteger2(void);
	void			m_popUInteger2(GPDL_STATE errorstatus=GPDL_UNDER_SP);
	void			m_IllegalStateMessageAndQuit(void);
	void			m_interpretError(const char *msg);
	void			m_error_message(char *msg);
  void      m_getCharacterValueWithActor(int keyindex, ActorType actor);
  void      m_getCharacterValue(int keyindex);
  void      m_setCharacterValue(int keyindex);
  void      m_getPartyValue(int keyindex);
  void      m_setPartyValue(int keyindex);
  void      m_getGameValue(int keyindex);
  void      m_setGameValue(int keyindex);

  //
  // Spell and AI script functions
  //

  // convert between ActorType and string
  ActorType m_StringToActor(const char *data);
  CString   m_ActorToString(const ActorType *data);

  // returns an ActorType in string format
  CString m_Myself(void); // based on current character ActorType
  CString m_Name(const char *name); // search based on name that is passed in
  CString m_Target(void); // based on current target ActorType

  // returns a string representing the requested value
  // input parameter is a string containing an ActorType
  CString m_Gender(CString &data);
  CString m_Class(CString &data); 
  CString m_Race(CString &data);
  CString m_Status(CString &data);
  CString m_Alignment(CString &data);
  CString m_HitPoints(CString &data);

  // returns "1" or "0" (true or false)
  // input parameter is a string containing an ActorType
  CString m_AlignmentGood(CString &data);
  CString m_AlignmentEvil(CString &data);
  CString m_AlignmentLawful(CString &data);
  CString m_AlignmentNeutral(CString &data);
  CString m_AlignmentChaotic(CString &data);  
  CString m_InParty(CString &data);
  CString m_SaveVsSpell(CString &data);
  CString m_SaveVsBreathWeapon(CString &data);
  CString m_SaveVsRodStaffWand(CString &data);
  CString m_SaveVsPetPoly(CString &data);
  CString m_SaveVsParPoiDM(CString &data);
  CString m_IsUndead(CString &data);
  CString m_IndexOf(const CString &data);
  CString m_IsCharAffectedBySpell(const CString &ActorString, const CString &SpellName);
  CString m_IsCharAffectedBySpellAttribute(const CString &ActorString, const CString &SpellAttribName);

	unsigned int   *m_returnStack;
//	void			m_attributeInsert(ATTRIBUTES *atr, 
//							   const CString& key,
//						       const CString& val);
	CString        *m_dataStack;
	CString			m_listenText;
	DISCOURSETEXT   m_discourseText;
	//
	// Some temporary variables for the interpreter
	CString			m_false;
	CString			m_true;
	unsigned int    m_bincode;   // The 32-bit code at m_PC
	unsigned int    m_opcode;    // The top byte of m_bincode;
	unsigned int    m_subop;     // The bottom 24 bits of m_bincode;
	CString			m_string1;
	CString			m_string2;
  CString     m_string3;
  CString     m_string4;
	int				m_Integer1;
	int				m_Integer2;
	int				m_Integer3;
	unsigned int    m_uInteger1;
	unsigned int	m_uInteger2;
	bool			m_bool1;
	GPDL_STATE		m_interpStatus;
	re_pattern_buffer m_grepPat;
	re_registers	m_grepReg;
	const char	   *m_grepCResult;
  double m_IntermediateResult; // holds results between scripts

public:
	GPDL(void);
	~GPDL(void);
	GPDL_STATE State(void);
	GPDL_STATE Load(CArchive& ar);
#ifdef UAFEDITOR
  GPDL_STATE Load(GPDLCOMP *pGPDL);
#endif
	unsigned int numIndex(void); // Number of entries in index
	CString indexName(unsigned int i); // Name of ith function in index.
  int GetFunctionEntryPoint(const CString& name); //-1 if interpreter not initialized
                                                  // 0 if entry point not found
	bool IsActive(void);
	GPDL_STATE BreakExecute(void);
	GPDL_STATE BeginExecute(CString& funcName, GPDL_EVENT *event);
  CString    BeginExecute(int entryPoint);
  CString    ExecuteScript(const CString& code, int entryPointOrdinal);
  CString    ExecuteScript(const CString& code, int entryPointOrdinal,CString *par, int numPar);
	GPDL_STATE Display(void);  // If nothing to display...no harm.
	GPDL_STATE Char(UINT c); // Returns GPDL_IGNORED if keystroke ignored.
  void SetIntermediateResult(double val) { m_IntermediateResult = val; }
  double GetIntermediateResult(void) const { return m_IntermediateResult; }
};

extern GPDL	gpdl;
extern CArray<CString, CString> discourseLine;
//extern CString discourseLine[5]; //DLD 12/20/00 //extern CString discourseLine[TEXTBOX_LINES]; 



int LongCompare(const CString& x, const CString& y);          //ARITHMETIC
CString LongAdd(const CString& x, const CString& y);          //ARITHMETIC
CString LongSubtract(const CString& x, const CString& y);     //ARITHMETIC
CString LongMultiply(const CString& s1, const CString& s2);   //ARITHMETIC
void LongDivide(const CString& dividend,                      //ARITHMETIC
                const CString& divisor,                       //ARITHMETIC
                CString& quotient,                            //ARITHMETIC
                CString& remainder);                          //ARITHMETIC
int CleanNumber(const CString& num, char *digits);            //ARITHMETIC


class RDREXEC                                                           //RDR
{                                                                       //RDR
  char *m_code;                                                         //RDR
  int   m_getInteger(const char **str);                                 //RDR
public:                                                                 //RDR
  RDREXEC(void);                                                        //RDR
  ~RDREXEC(void);                                                       //RDR
  int InterpretExpression(const CString& expr,                          //RDR
                          int (*interpretRDR)(int *));                  //RDR
};                                                                      //RDR

} // End of namespace DungeonCraft

#endif