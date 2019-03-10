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

// This is the code for the GPDL Interpreter.
// You feed it events and it will interpret the 
// code found in the GPDL binary file.
//
// Events:
//   Load          Make sure the GPDL binary file has been read.
//                 Accepted in 'UNINITIALIZED' state.
//   BeginExecute  Search for function name and start execution
//                 Accepted in 'IDLE' state.
//   BreakExecute  Terminate current execution
//                 Accepted anytime.
//   Char          A character has been received from the
//                 keyboard.  Returns:
//                    GPDL active 
//                       keystroke ignored    GPDL_IGNORED 
//                       deystroke accepted   GPDL_ACCEPTED
//                    GPDL inactive      GPDL_IDLE
//
// ****  All events return a status ***
//  GPDL_EVENT_ERROR   = Event occurred at inappropriate time.
//                      The event is ignored.
//  GPDL_NOSUCHNAME    = No such function name in index.
//  GPDL_IDLE          = Exection complete.  The only event that will
//                      be honored is 'BeginExecute'
//  GPDL_IGNORED       = Results only from Char event if the keystroke
//                      is ignored.
//  GPDL_ACCEPTED      = Results from Char event.  Keystroke welecomed.
//  GPDL_CODE_ILLOP    = Illegal operation code.  Execution complete
//                      State set to IDLE.
//  GPDL_WAIT_INPUT    = Waiting for keyboard text input from user.
//  GPDL_WAIT_ACK      = Waiting for output acknowledge.
//  GPDL_WAIT_CONTINUE = Waiting for 'continue' input from user when
//                      more output text is waiting for space on the
//                      screen.  We expect the user to press the space
//                      bar to indicate he has read what is already
//                      displayed.
//  GPDL_OVR_SP        = Data stack overflow.  State set to IDLE.
//  GPDL_OVR_RP        = Return stack overflow.  State set to IDLE.
//  GPDL_READ_ERROR    = Error reading GPDL binary file.


#include "dungeoncraft/shared/..\Shared\stdafx.h"

//#include "dungeoncraft/shared/externs.h"
#include <malloc.h>
#include <math.h>
#include "dungeoncraft/shared/GameEvent.h"
#include "dungeoncraft/shared/..\Shared\GPDLopCodes.h"
#include "dungeoncraft/shared/GPDLcomp.h"
#include "dungeoncraft/shared/GPDLexec.h"
#include "dungeoncraft/shared/Graphics.h"
#include "dungeoncraft/shared/GlobalData.h"
#include "dungeoncraft/shared/RuntimeIF.h"
#include "dungeoncraft/shared/party.h"

#ifdef UAFEngine
#include "dungeoncraft/shared/..\UAFWin\combatants.h"
#include "dungeoncraft/shared/..\UAFWin\CharStatsForm.h"
#include "dungeoncraft/shared/..\UAFWin\FormattedText.h"
#else
const int TEXTBOX_LINES = 5;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


GPDL  gpdl;
CArray<CString, CString> discourseLine;
//CString discourseLine[5]; //DLD 12/20/00 CString discourseLine[TEXTBOX_LINES];

//re_pattern_buffer grepPat;
//re_registers      grepReg;

GPDL::GPDL(void) {
  m_state=GPDL_UNINITIALIZED;
  m_RP0=1000;
  m_SP0=1000;
  m_H = 0;
  m_dataStack=NULL;
  m_returnStack=NULL;
  m_program = NULL;
  //m_char_pixels_per_line=GetTextBoxCharWidth();
  m_false="";
  m_listenText=m_false;
  m_true="1";
  SetIntermediateResult(0.0);
}

GPDL::~GPDL(void) {
  if (m_dataStack!=NULL) {
    delete[] m_dataStack;
    m_dataStack=NULL;
  };
  if (m_returnStack!=NULL) {
    delete[] m_returnStack;
    m_returnStack=NULL;
  };
  if (m_program != NULL) 
  {
    delete[] m_program;
    m_program = NULL;
  };
  //discourseLine.RemoveAll();
  m_state=GPDL_UNINITIALIZED;
}


#ifdef UAFEDITOR
GPDL_STATE GPDL::ReadProgram(GPDLCOMP *pGPDL)
{
  m_H = pGPDL->GetCodeSize();
  if (m_H <= 0) return GPDL_READ_ERROR;
  m_programAllocated=m_H;
  if (m_program != NULL) delete[] m_program;
  m_program = new unsigned int[m_programAllocated];
  memcpy(m_program, pGPDL->GetCodeAddress(),4*m_programAllocated);
  return GPDL_OK;
}
#endif UAFEDITOR

GPDL_STATE GPDL::ReadProgram(CArchive& ar)
{
  unsigned int i;
  ar >> m_H;
  m_programAllocated=m_H;
  if (m_program != NULL) delete[] m_program;
  m_program = new unsigned int[m_programAllocated];
  for (i=0; i<m_H; i++) {
    ar>>m_program[i];
  };
  return GPDL_OK;
}

//*****************************************
//  Time to be sure the file has been loaded.
//  Parameter:
//    CArchive = file to load
//  Return value:
//    0 if all is well
//    non-zero is an error code
//*****************************************
GPDL_STATE GPDL::Load(CArchive& ar) {
  if (m_state!=GPDL_UNINITIALIZED) return GPDL_EVENT_ERROR;
  if (ReadProgram(ar) != GPDL_OK)  return GPDL_READ_ERROR;
  //if (m_code.read(ar))      return GPDL_READ_ERROR;
  if (m_constants.read(ar)) return GPDL_READ_ERROR;
  if (m_index.read(ar))     return GPDL_READ_ERROR;
  m_state=GPDL_IDLE;
  if (m_dataStack!=NULL) delete []m_dataStack;
  m_dataStack=NULL;
  if (m_returnStack!=NULL) delete []m_returnStack;
  m_returnStack=NULL;
  m_dataStack=new CString[m_SP0];
  m_returnStack=new unsigned int[m_RP0];
  return GPDL_IDLE;
}

#ifdef UAFEDITOR
GPDL_STATE GPDL::Load(GPDLCOMP *pGPDL)
{
  if (ReadProgram(pGPDL) != GPDL_OK) return GPDL_READ_ERROR;
  if (m_constants.read(pGPDL)) return GPDL_READ_ERROR;
  if (m_index.read(pGPDL))     return GPDL_READ_ERROR;
  m_state = GPDL_IDLE;
  if (m_dataStack!=NULL) delete []m_dataStack;
  m_dataStack=NULL;
  if (m_returnStack!=NULL) delete []m_returnStack;
  m_returnStack=NULL;
  m_dataStack=new CString[m_SP0];
  m_returnStack=new unsigned int[m_RP0];
  return GPDL_OK;
}
#endif

void GPDL::m_pushRP(unsigned int n) {
  if (m_RP<2) {
    if (m_interpStatus==GPDL_OK) m_interpStatus=GPDL_OVER_RP;
    return;
  };
  m_returnStack[--m_RP]=n;
}

unsigned int GPDL::m_popRP(void) {
  if (m_RP>=m_RP0) {
    if (m_interpStatus==GPDL_OK) m_interpStatus=GPDL_UNDER_RP;
    return 0;
  };
  return m_returnStack[m_RP++];
}

void GPDL::m_pushFP(void) {
  m_pushRP(m_FP);
}

void GPDL::m_pushPC(void) {
  m_pushRP(m_PC);
}

void GPDL::m_popPC(void) {
  m_PC=m_popRP();
}

void GPDL::m_popFP(void) {
  m_FP=m_popRP();
}

void GPDL::m_pushSP(CString& val) {
  if (m_SP<2) {
    if (m_interpStatus==GPDL_OK) m_interpStatus=GPDL_OVER_SP;
    return;
  };
  m_dataStack[--m_SP]=val;
}

CString GPDL::m_popSP(void) {
  if (m_SP>=m_SP0) {
    if (m_interpStatus==GPDL_OK) m_interpStatus=GPDL_UNDER_SP;
    return m_false;
  }
  return m_dataStack[m_SP++];
}

void GPDL::m_popString1(void) {
  m_string1=m_popSP();
}
void GPDL::m_popString2(void) {
  m_string2=m_popSP();
}
void GPDL::m_popString3(void) {
  m_string3=m_popSP();
}
void GPDL::m_popString4(void) {
  m_string4=m_popSP();
}

void GPDL::m_pushString1(void) {
  m_pushSP(m_string1);
}
void GPDL::m_pushString2(void) {
  m_pushSP(m_string2);
}
void GPDL::m_pushString3(void) {
  m_pushSP(m_string3);
}
void GPDL::m_pushString4(void) {
  m_pushSP(m_string4);
}

void GPDL::m_pushUInteger1(void) {
  m_string1.Format("%u",m_uInteger1);
  m_pushString1();
}

void GPDL::m_pushUInteger2(void) {
  m_string1.Format("%u",m_uInteger2);
  m_pushString1();
}

void GPDL::m_pushInteger1(void) {
  m_string1.Format("%d",m_Integer1);
  m_pushString1();
}

void GPDL::m_pushInteger2(void) {
  m_string1.Format("%d",m_Integer2);
  m_pushString1();
}

void GPDL::m_pushInteger3(void) {
  m_string1.Format("%d",m_Integer3);
  m_pushString1();
}

GPDL_STATE GPDL::m_popInteger(bool& negative, 
                unsigned int& n) {
  GPDL_STATE status;
  static CString str; // Avoid allocating each time.
  unsigned int len, i;
  unsigned int base=10;
  char c;
  negative=false;
  status=GPDL_OK;
  n=0;
  str=m_popSP();
  len=str.GetLength();
  for (i=0; i<len; i++) {
    c=str.GetAt(i);
    if ((c=='-')&&(i==0)) {
      negative=true;
      continue;
    }
    if ((c<'0') || (c>'9')) {
      if ((c=='x')||(c=='X')) {
        if ((n!=0)||(base!=10)||(i!=1)) {
          status=GPDL_BADINTEGER;
          break;
        };
        base=16;
        continue;
      };
      if (base!=16) {
        status=GPDL_BADINTEGER;
        break;
      };
      if ((c>='a') && (c<='f')) c+='A'-'a';
      if ((c<'A') || (c>'F')) {
        status=GPDL_BADINTEGER;
        break;
      };
      c+='9'+1-'A';
    };
    n=n*base+c-'0';
  };
  if (status!=GPDL_OK) { 
    n=0;
  };
  return status;
}

void GPDL::m_popInteger1(GPDL_STATE errorstatus) {
  bool negative;
  unsigned int n;
  if (m_popInteger(negative, n)!=GPDL_OK) m_interpStatus=errorstatus;
  m_Integer1=negative?-(int)n:n;
}

void GPDL::m_popInteger2(GPDL_STATE errorstatus) {
  bool negative;
  unsigned int n;
  if (m_popInteger(negative, n)!=GPDL_OK) m_interpStatus=errorstatus;
  m_Integer2=negative?-(int)n:n;
}

void GPDL::m_popInteger3(GPDL_STATE errorstatus) {
  bool negative;
  unsigned int n;
  if (m_popInteger(negative, n)!=GPDL_OK) m_interpStatus=errorstatus;
  m_Integer3=negative?-(int)n:n;
}

void GPDL::m_popUInteger1(GPDL_STATE errorstatus) {
  bool negative;
  unsigned int n;
  if (m_popInteger(negative, n)!=GPDL_OK) m_interpStatus=errorstatus;
  if (negative) {
    m_uInteger1=0;
    m_interpStatus=errorstatus;
    m_interpretError("Expected unsigned integer");
  } else m_uInteger1=n;
}

void GPDL::m_popUInteger2(GPDL_STATE errorstatus) {
  bool negative;
  unsigned int n;
  if (m_popInteger(negative, n)!=GPDL_OK) m_interpStatus=errorstatus;
  if (negative) {
    m_uInteger2=0;
    m_interpStatus=errorstatus;
  } else m_uInteger2=n;
}


static const char *callLine;
static int len;

static int GetToken(void)
{
  char c;
  callLine += len;
  while (*callLine == ' ') callLine++;
  c = *callLine;
  if (    (c=='$')
       || (c=='_')
       || ((c>='A') && (c<='Z'))
       || ((c>='a') && (c<='z'))
     )
  {
    for (len=1; ;len++)
    {
      c = callLine[len];
      if (    (c=='$')
           || (c=='_')
           || ((c>='A') && (c<='Z'))
           || ((c>='a') && (c<='z'))
           || ((c>='0') && (c<='9'))
        ) continue;
      break;
    };
    return 1; // 1 is a function name
  };
  if (c == '"')
  {
    len = 1;
    while (callLine[len] != '"')
    {
      if (callLine[len] == 0) return 0;
      len++;
    };
    len++;
    return 5; // 5 is a quoted string
  };
  if ( (c>='0') && (c<='9') )
  {
    len = 1;
    while ((callLine[len]>='0') && (callLine[len]<='9')) len++;
    return 6; //6 is a number
  };
  len = 1;
  if (c == '(') return 2; // 2 is open parethesis
  if (c == ')') return 3;
  if (c == ',') return 4;
  return 0; // 0 is an error
}

GPDL_STATE GPDL::BeginExecute(CString& funcCall, GPDL_EVENT *event) 
{
  int tokenType;
  char c;
  int neededParam, headerIdx, multiplier;
  callLine = LPCTSTR(funcCall);
  len = 0;
  tokenType = GetToken();
  if (tokenType != 1)
  {
    WriteDebugString("GPDL call does not contain a function name\n");
    return GPDL_NOSUCHNAME;
  };
  m_string1 = CString(callLine,len);
  m_PC=m_index.lookup(m_string1);
  if (m_PC==0) 
  {
    m_string2.Format("GPDL cannot find specified function named %s.\n",m_string1);
    WriteDebugString(m_string2);
    return GPDL_NOSUCHNAME;
  };
  m_event=event;
  m_RP=m_RP0;
  m_SP=m_SP0;
  m_pushRP(0xffffffff);          // So we will know when we are done.
  m_pushRP(0xffffffff);          // Old Frame Pointer
  m_string1=m_false;
  m_pushSP(m_string1); // The empty string as default result.
  m_string2 = m_constants.peek(Peek(m_PC) & 0xffffff);
  neededParam = 0;
  multiplier = 1;
  for (headerIdx=m_string2.GetLength()-2;headerIdx>=0;headerIdx--)
  {
    c = m_string2[headerIdx];
    if (c < '0') break;
    if (c > '9') break;
    neededParam += multiplier * (c-'0');
    multiplier *= 10;
  };
  tokenType = GetToken();
  if (tokenType != 2)
  {
    WriteDebugString("GPDL function name not followed by '('\n");
    return GPDL_NOSUCHNAME;
  };
  for ( ; ; )
  { // Get another parameter
    tokenType = GetToken();
    if (tokenType == 3) break;
    if (tokenType == 5) m_string1 = CString(callLine+1, len-2);
    else if (tokenType == 6) m_string1 = CString(callLine, len);
    else
    {
      WriteDebugString("GPDL parameter syntax error\n");
      return GPDL_NOSUCHNAME;
    };
    m_pushSP(m_string1);
    neededParam--;
    tokenType = GetToken();
    if (tokenType == 3) break;
    if (tokenType != 4)
    {
      WriteDebugString("GPDL call parameters missing a comma\n");
      return GPDL_NOSUCHNAME;
    };
  };
  if (neededParam < 0) m_string1 = "many";
  if (neededParam > 0) m_string1 = "few";
  if (neededParam != 0)
  {
    m_string3.Format("GPDL function %s call has too %s parameters\n",m_string2,m_string1);
    WriteDebugString(m_string3);
    return GPDL_NOSUCHNAME;
  };
  m_FP=m_SP;
  return m_interpret();
}

CString GPDL::BeginExecute(int entryPoint) {
  GPDL_STATE status;
  m_PC=entryPoint;
  if (m_PC==0) return m_false;
  m_event=NULL;
  m_RP=m_RP0;
  m_SP=m_SP0;
  m_pushRP(0xffffffff);          // So we will know when we are done.
  m_pushRP(0xffffffff);          // Old Frame Pointer
  m_string1=m_false;
  m_pushSP(m_string1); // The empty string as default result.
  m_FP=m_SP;
  status = m_interpret();
  if (status != GPDL_IDLE) return m_false;
  m_popString1();
  return m_string1;
}

CString GPDL::ExecuteScript(const CString& code, int entryPointOrdinal)
{
  GPDL_STATE status;
  int numEntry;
  const char *binaryCode;
  binaryCode = LPCTSTR(code);
  if (m_program != NULL) free (m_program);
  numEntry = *((int *)(binaryCode + 1)) & 0xffffff;
  m_H = *((int *)(binaryCode + 4));
  if (entryPointOrdinal > numEntry) return m_false;
  if (entryPointOrdinal <= 0) return m_false;
  m_PC = ((int *)(binaryCode + 12))[entryPointOrdinal-1];
  if (m_PC==0) return m_false;
  m_program = (unsigned int *)(binaryCode + 12 + 4*numEntry);
  if (m_dataStack==NULL) 
            m_dataStack=new CString[m_SP0];
  if (m_returnStack==NULL)
            m_returnStack=new unsigned int[m_RP0];
  m_event=NULL;
  m_RP=m_RP0;
  m_SP=m_SP0;
  m_pushRP(0xffffffff);          // So we will know when we are done.
  m_pushRP(0xffffffff);          // Old Frame Pointer
  m_string1=m_false;
  m_pushSP(m_string1); // The empty string as default result.
  m_FP=m_SP;
  status = m_interpret();
  m_program = NULL;
  if (status != GPDL_IDLE) return m_false;
  m_popString1();
  return m_string1;
}

CString GPDL::ExecuteScript(const CString& code, 
                            int entryPointOrdinal, 
                            CString *par,
                            int numPar)
{
  GPDL_STATE status;
  int numEntry;
  const char *binaryCode;
  binaryCode = LPCTSTR(code);
  if (m_program != NULL) free (m_program);
  numEntry = *((int *)(binaryCode + 1)) & 0xffffff;
  m_H = *((int *)(binaryCode + 4));
  if (entryPointOrdinal > numEntry) return m_false;
  if (entryPointOrdinal <= 0) return m_false;
  m_PC = ((int *)(binaryCode + 12))[entryPointOrdinal-1];
  if (m_PC==0) return m_false;
  m_program = (unsigned int *)(binaryCode + 12 + 4*numEntry);
  if (m_dataStack==NULL) 
            m_dataStack=new CString[m_SP0];
  if (m_returnStack==NULL)
            m_returnStack=new unsigned int[m_RP0];
  m_event=NULL;
  m_RP=m_RP0;
  m_SP=m_SP0;
  m_pushRP(0xffffffff);          // So we will know when we are done.
  m_pushRP(0xffffffff);          // Old Frame Pointer
  m_string1=m_false;
  m_pushSP(m_string1); // The empty string as default result.
  for (int i=0; i<numPar; i++) m_pushSP(par[i]); // A through F
  m_FP=m_SP;
  status = m_interpret();
  m_program = NULL;
  if (status != GPDL_IDLE) return m_false;
  m_popString1();
  return m_string1;
}

int GPDL::GetFunctionEntryPoint(const CString& name)
{
  if (m_state == GPDL_UNINITIALIZED) return -1;
  return m_index.lookup(name);
}

void GPDL::m_interpretError(const char *msg) 
{
#ifdef UAFEngine
  DoTaskStackTrace();
#endif
  CString message;
  message.Format("GPDL interpret error.\n%s\nPC = 0x%06x",msg,m_executionAddress);
  MsgBoxError(message);
}


#define INTERP_RETURN(status) {   \
  if (m_interpStatus!=GPDL_OK) {    \
    BreakExecute();         \
    return m_interpStatus;      \
  };                  \
  return status;            \
}


CString upperPattern(CString& str) {
  // Convert characters to uppercase unless escaped by a backslash.
  // Otherwise constructs such as \b get turned into \B...undesired!
  char *temp;
  int i;
  char c;
  int len=str.GetLength();
  temp=(char *)alloca(len+2);
  if (temp==NULL) return str;
  for (i=0; i<len; i++) {
    c=str.GetAt(i);
    if (c=='\\') {
      temp[i++]=c;
      temp[i]=str.GetAt(i);
    } else {
      if ((c>='a') && (c<='z')) temp[i]=(char)(c+'A'-'a');
      else temp[i]=c;
    };
  };
  temp[len]=0;
  return CString(temp);
}


void GPDL::m_error_message(char *msg){
  if (m_debugLevel&DBG_messageBox) {
    MsgBoxError(msg);
  };
}

/*void GPDL::m_attributeInsert(ATTRIBUTES *atr, 
               const CString& key,
               const CString& value) {
  int flags;
  flags=atr->GetFlags(key);
  if (flags==-1) flags=0; else flags|=ATF_MODIFIED;
  if ((flags&ATF_READONLY)==0)
  {
    atr->Insert(key,value,flags);
  }
  else
  {
    m_interpretError("Attempt to modify READONLY ATTRIBUTE");
  };
}
*/

  

CString GenderNames[2]={"Male", "Female"};

struct IF_KEYWORD_DATA;
IF_KEYWORD_DATA *LocateKeywordData(const int keyindex);
CString GetDataAsCString(IF_KEYWORD_DATA *pData,ActorType *pActor=NULL);
void SetDataFromCString(IF_KEYWORD_DATA *pData, CString value,ActorType *pActor=NULL);


void GPDL::m_getCharacterValueWithActor(int keyindex, ActorType actor)
{
  // We need to convert keyindex to a IF_KEYWORD_DATA pointer
  IF_KEYWORD_DATA *pIfKeyData = LocateKeywordData(keyindex);
  m_string1 = GetDataAsCString(pIfKeyData, &actor);
  m_pushString1();
}

void GPDL::m_getCharacterValue(int keyindex)
{
  //IF_KEYWORD_DATA *pIfKeyData;
  m_popInteger1();
  ActorType actor;
  if (IsCombatActive())
  {
#ifdef UAFEngine
    if (   (m_Integer1<0) 
        || (m_Integer1>=GetNumCombatants()))
    {
      CString errmsg;
      errmsg.Format("Illegal combatant %i in m_getCharacterValue",
        m_Integer1);
      errmsg += "\n";
      errmsg += GetActorDebugText(actor);
      m_interpretError(errmsg);
      WriteStackTrace();
      m_string1 = m_false;
      m_pushString1(); // Have to supply a result.
      return;
    };    
    actor = combatData.combatants[m_Integer1].GetContext();    
#else
    actor=NULL_ACTOR;
#endif
  }
  else
  {
#ifdef UAFEngine
    int partyIndex = -1;
    for (int i=0;i<GetPartySize(party);i++)
    {
      if (party.characters[i].marchOrder == m_Integer1)
      {
        partyIndex=i;
        break;
      }
    }
    if (partyIndex < 0)
    {
      CString errmsg;
      errmsg.Format("Illegal party member %i in m_getCharacterValue",
                    m_Integer1);
      errmsg += "\n";
      errmsg += GetActorDebugText(actor);
      
      m_interpretError(errmsg);
      WriteStackTrace();
      m_string1 = m_false;
      m_pushString1(); // Have to supply a result.
      return;
    };
    actor = party.characters[partyIndex].GetContext();
#else
    actor=NULL_ACTOR;
#endif    
  }
  /*
  // We need to convert keyindex to a IF_KEYWORD_DATA pointer
  pIfKeyData = LocateKeywordData(keyindex);
  m_string1 = GetDataAsCString(pIfKeyData, &actor);
  m_pushString1();
  */
  
  m_getCharacterValueWithActor(keyindex, actor);
}

void GPDL::m_setCharacterValue(int keyindex)
{
  IF_KEYWORD_DATA *pIfKeyData;
  m_popString1();  // value
  m_popInteger1(); // character number
  ActorType actor;
  if (IsCombatActive())
  {
#ifdef UAFEngine
    if (   (m_Integer1<0) 
        || (m_Integer1>=GetNumCombatants()))
    {
      CString errmsg;
      errmsg.Format("Illegal combatant %i in m_getCharacterValue",
        m_Integer1);
      errmsg += "\n";
      errmsg += GetActorDebugText(actor);
      m_interpretError(errmsg);
      WriteStackTrace();
      
      m_string1 = m_false;
      m_pushString1(); // Have to supply a result.
      return;
    };    
    actor = combatData.combatants[m_Integer1].GetContext();    
#else
    actor=NULL_ACTOR;
#endif
  }
  else
  {
#ifdef UAFEngine
    int partyIndex = -1;
    for (int i=0;i<GetPartySize(party);i++)
    {
      if (party.characters[i].marchOrder == m_Integer1)
      {
        partyIndex=i;
        break;
      }
    }
    if (partyIndex < 0)
    {
      CString errmsg;
      errmsg.Format("Illegal party member %i in m_getCharacterValue",
        m_Integer1);
      errmsg += "\n";
      errmsg += GetActorDebugText(actor);
      m_interpretError(errmsg);
      WriteStackTrace();
      m_string1 = m_false;
      m_pushString1(); // Have to supply a result.
      return;
    };
    actor = party.characters[partyIndex].GetContext();
#else
    actor=NULL_ACTOR;
#endif
  }

  pIfKeyData = LocateKeywordData(keyindex);
  SetDataFromCString(pIfKeyData, m_string1, &actor);
  m_string1 = m_false;
  m_pushString1(); // Have to supply a result;
}


void GPDL::m_getPartyValue(int keyindex)
{
  IF_KEYWORD_DATA *pIfKeyData;
  // We need to convert keyindex to a IF_KEYWORD_DATA pointer
  pIfKeyData = LocateKeywordData(keyindex);
  m_string1 = GetDataAsCString(pIfKeyData);
  m_pushString1();
}

void GPDL::m_setPartyValue(int keyindex)
{
  IF_KEYWORD_DATA *pIfKeyData;
  m_popString1();  // value
  pIfKeyData = LocateKeywordData(keyindex);
  SetDataFromCString(pIfKeyData, m_string1);
  m_string1 = m_false;
  m_pushString1(); // Have to supply a result;
}


void GPDL::m_getGameValue(int keyindex)
{
  m_getPartyValue(keyindex); //Works the same
}

void GPDL::m_setGameValue(int keyindex)
{
  m_setPartyValue(keyindex); //Works the same
}


GPDL_STATE GPDL::m_interpret(void) {
  unsigned int i;
  m_interpStatus=GPDL_OK;
  while (m_interpStatus==GPDL_OK) {
    m_executionAddress = m_PC; // for error printout
    // 19 Apr PRS m_bincode=m_code.peek(m_PC++);
    m_bincode=Peek(m_PC++);
    m_opcode=(m_bincode>>24)&0xff;
    m_subop=m_bincode&0xffffff; // Also might be an address
    switch(m_opcode) {
    case BINOP_LOCALS:
      for (i=0; i<m_subop; i++)
        m_pushSP(m_false);
      break;
    case BINOP_JUMP:
      m_PC=m_subop;
      break;
    case BINOP_FETCHCONSTANT:
      m_pushSP(m_constants.peek(m_subop));
      break;
    case BINOP_FETCHTEXT:
      m_string1 = ((char *)m_program) + (m_bincode & 0xffffff);
      m_pushString1();
      break;
    case BINOP_CALL:
      m_pushPC();
      m_pushFP();
      m_FP=m_SP;
      m_PC=m_subop;
      if (m_debugLevel&DBG_functionTrace) {
        int openParenLoc, LSP;
        unsigned int numPar;
        m_string1=m_constants.peek(Peek(m_PC)&0xffffff);
        openParenLoc=m_string1.Find('(');
        if (openParenLoc<0) {
          m_errorLog.log("Function has illegal entry");
          INTERP_RETURN(GPDL_ILLFUNC);
        };
        m_string2.Format("0x%06x call ",m_returnStack[m_RP+1]-1);
        m_string2+=m_string1.Left(openParenLoc+1);
        numPar=0xffffff;
        sscanf((LPCTSTR)m_string1+openParenLoc+1,"%d",&numPar);
        if (numPar>10) numPar=10;
        LSP=m_SP;
        for (unsigned int i=0; i<numPar; LSP++,i++) {
          m_string1=m_dataStack[LSP];
          if (m_string1.GetLength()>20) m_string1=m_string1.Left(20);
          m_string2+=CString("\"") + m_string1 + CString("\"");
          if (i!=numPar-1) m_string2+=',';
        };
        m_string2+=CString(")");
        m_errorLog.log(m_string2);
      };
      m_PC++; // skip over entry information.
      break;
    case BINOP_FETCH_FP:
      m_subop -= (m_subop & 0x800000)<<1;
      m_pushSP(m_dataStack[m_FP+m_subop]);
      break;
    case BINOP_JUMPFALSE:
      m_popString1();
      if ((m_string1==m_false)||(m_string1=="0")) m_PC=m_subop;
      break;
    case BINOP_RETURN:
      m_SP=m_FP;
      m_popFP();
      m_popPC();
      for (;m_subop&0xfff; m_subop--) m_popString1();
        // Leave the parameters behind.
      if (m_PC==0xffffffff) {
        m_state=GPDL_IDLE;
        m_discourseText.Clear();
        INTERP_RETURN(GPDL_IDLE);
      };
      if (m_debugLevel&DBG_functionTrace) {
        m_string1="          return ";
        m_string1+=CString("\"")+m_dataStack[m_SP]+CString("\"");
        m_errorLog.log(m_string1);
      };
      break;
    case BINOP_STORE_FP:
      m_subop -= (m_subop & 0x800000)<<1; 
      m_dataStack[m_FP+m_subop]=m_popSP();
      break;
    case BINOP_SUBOP:
      switch(m_subop) {

      case SUBOP_Alignment:
        m_popString1();
        m_string2 = m_Alignment(m_string1);
        m_pushString2();
        break;
      case SUBOP_AlignmentGood:
        m_popString1();
        m_string2 = m_AlignmentGood(m_string1);
        m_pushString2();
        break;
      case SUBOP_AlignmentEvil:
        m_popString1();
        m_string2 = m_AlignmentEvil(m_string1);
        m_pushString2();
        break;
      case SUBOP_AlignmentLawful:
        m_popString1();
        m_string2 = m_AlignmentLawful(m_string1);
        m_pushString2();
        break;
      case SUBOP_AlignmentNeutral:
        m_popString1();
        m_string2 = m_AlignmentNeutral(m_string1);
        m_pushString2();
        break;
      case SUBOP_AlignmentChaotic:
        m_popString1();
        m_string2 = m_AlignmentChaotic(m_string1);
        m_pushString2();
        break;
      case SUBOP_Class:
        m_popString1();
        m_string2 = m_Class(m_string1);
        m_pushString2();
        break;
      case SUBOP_COINCOUNT:
        if (IsCombatActive())
        {
          m_interpretError("Combat active in $COINCOUNT");
          m_Integer3=0;
          m_pushInteger3();
          break;
        }        
        m_popInteger1();   // The coin ordinal
        if (m_Integer1 < 1) m_Integer1 = 1;
        if (m_Integer1 > MONEY_DATA_TYPE::MAX_COIN_TYPES) m_Integer1 = 1;
        
        m_popInteger2();   // The character ordinal
        if ((m_Integer2 < 1) || (m_Integer2 > GetPartySize(party)))
        {
          // assume active char 
          m_Integer2 = party.activeCharacter+1;
        };   
        
        m_Integer3 = party.characters[m_Integer2-1].money.Coins[m_Integer1-1];
        m_pushInteger3();
        break;
      case SUBOP_COINNAME:
        m_popInteger1(); // The coin ordinal;
        m_string1 = globalData.moneyData.GetCoinName(m_Integer1-1);
        m_pushString1();
        break;
      case SUBOP_COINRATE:
        m_popInteger1(); // The coin ordinal;
        m_Integer2 = globalData.moneyData.GetCoinRate(m_Integer1-1);
        m_pushInteger2();
        break;
      case SUBOP_CURR_CHANGE_BY_VAL:
        {
          // GPDL doesn't handle floats, convert to integer
          double ir = GetIntermediateResult();
          int temp=0;
          if (ir < 0.0)
            temp = ceil(ir + -0.5); // round down
          else
            temp = floor(ir + 0.5); // round up

          m_string1.Format("%i", temp);
          m_pushString1();
        }
        break;
      case SUBOP_DEBUG:
        m_uInteger2=m_debugLevel;
        m_popUInteger1();
        m_string1.Format("$DEBUG(0x%04x)",m_uInteger1);
        m_errorLog.log(m_string1);
        m_debugLevel=m_uInteger1;
        m_pushUInteger2();  // Our 'result'
        break;
      case SUBOP_DEPRECATED_DELETE_LEVEL_STATS_ASL:
        m_popString1();  // key
        DeleteLevelASL(globalData,
                       GetCurrentLevel(),
                       m_string1);
//        globalData.levelInfo.stats[globalData.currLevel]
//          .level_attributes.Delete(m_string1);
        m_pushSP(m_false);  // Must supply a result
        break;
      case SUBOP_DELETE_LEVEL_STATS_ASL:
        m_popString1();  // key
        m_popString3();  //Level
        if (m_string3 == "") m_Integer1 = GetCurrentLevel();
        else m_Integer1 = atoi(m_string3);
        DeleteLevelASL(globalData,
                       m_Integer1,
                       m_string1);
//        globalData.levelInfo.stats[globalData.currLevel]
//          .level_attributes.Delete(m_string1);
        m_pushSP(m_false);  // Must supply a result
        break;
      case SUBOP_DELETE_PARTY_ASL:
        m_popString1();  // key
        DeletePartyASL(party,m_string1);
//        party.party_attributes.Delete(m_string1);
        m_pushSP(m_false
          );  // Must supply a result
        break;
      case SUBOP_DUP:
        m_popString1();
        m_pushString1();
        m_pushString1();
        break;
      case SUBOP_FALSE:
        m_pushSP(m_false);
        break;
      case SUBOP_ISEQUAL:
        m_popString1();
        m_popString2();
        m_pushSP(m_string1==m_string2?m_true:m_false);
        break;
      case SUBOP_Gender:
        m_popString1();
        ASSERT(m_string1.GetLength() == 24);
        m_string2 = m_Gender(m_string1);
        m_pushString2();
        break;
      case SUBOP_GET_CHAR_ASL:
        if (IsCombatActive())
        {
          m_interpretError("Combat active in $GET_CHAR_ASL");
          m_string2="";
          m_pushString2();
          break;
        }
        
        m_popString1();
        m_popInteger1();
        if ((m_Integer1<0) 
            || (m_Integer1>=GetPartySize(party))) 
        {
          m_interpretError("Illegal party member # in $GET_CHAR_ASL");
          break;
        };
        m_string2 = LookupCharacterASL(
          party,
          m_Integer1,
          m_string1);
        m_pushString2();
        break;

      case SUBOP_GET_ISMAMMAL:             
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_ISMAMMAL, actor);      
        }
        break;
      case SUBOP_GET_ISANIMAL:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_ISANIMAL, actor);      
        }
        break;
      case SUBOP_GET_ISSNAKE:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_ISSNAKE, actor);      
        }
        break;
      case SUBOP_GET_ISGIANT:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_ISGIANT, actor);      
        }
        break;        
      case SUBOP_GET_ISALWAYSLARGE:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_ISALWAYSLARGE, actor);      
        }
        break;
      case SUBOP_GET_HASDWARFACPENALTY:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_HASDWARFACPENALTY, actor);      
        }
        break;
      case SUBOP_GET_HASGNOMEACPENALTY:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_HASGNOMEACPENALTY, actor);      
        }
        break;
      case SUBOP_GET_HASDWARFTHACOPENALTY:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_HASDWARFTHACOPENALTY, actor);      
        }
        break;
      case SUBOP_GET_HASGNOMETHACOPENALTY:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_HASGNOMETHACOPENALTY, actor);      
        }
        break;
      case SUBOP_GET_HASRANGERDMGPENALTY:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_HASRANGERDMGPENALTY, actor);      
        }
        break;
      case SUBOP_GET_HASPOISONIMMUNITY:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_HASPOISONIMMUNITY, actor);      
        }
        break;
      case SUBOP_GET_HASDEATHIMMUNITY:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_HASDEATHIMMUNITY, actor);      
        }
        break;
      case SUBOP_GET_HASCONFUSIONIMMUNITY:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_HASCONFUSIONIMMUNITY, actor);      
        }
        break;
      case SUBOP_GET_HASVORPALIMMUNITY:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_HASVORPALIMMUNITY, actor);      
        }
        break;
      case SUBOP_GET_CANBEHELDORCHARMED:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_CANBEHELDORCHARMED, actor);      
        }
        break;
      case SUBOP_GET_AFFECTEDBYDISPELEVIL:
        {
          m_popString1(); // ActorType
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          m_getCharacterValueWithActor(CHAR_AFFECTEDBYDISPELEVIL, actor);      
        }
        break;
      case SUBOP_GET_CHAR_NAME:            m_getCharacterValue(CHAR_NAME);          break;
      case SUBOP_GET_CHAR_AC:              m_getCharacterValue(CHAR_AC);            break;
      case SUBOP_GET_CHAR_HITPOINTS:       m_getCharacterValue(CHAR_HITPOINTS);     break;
      case SUBOP_GET_CHAR_THACO:           m_getCharacterValue(CHAR_THACO);         break;
      case SUBOP_GET_CHAR_RDYTOTRAIN:      m_getCharacterValue(CHAR_RDYTOTRAIN);    break;
      case SUBOP_GET_CHAR_CLERICEXP:       m_getCharacterValue(CHAR_CLERICEXP);     break;
      case SUBOP_GET_CHAR_MAGICUSEREXP:    m_getCharacterValue(CHAR_MAGICUSEREXP);  break;
      case SUBOP_GET_CHAR_FIGHTEREXP:      m_getCharacterValue(CHAR_FIGHTEREXP);    break;
      case SUBOP_GET_CHAR_RANGEREXP:       m_getCharacterValue(CHAR_RANGEREXP);     break;
      case SUBOP_GET_CHAR_THIEFEXP:        m_getCharacterValue(CHAR_THIEFEXP);      break;
      case SUBOP_GET_CHAR_DRUIDEXP:        m_getCharacterValue(CHAR_DRUIDEXP);      break;
      case SUBOP_GET_CHAR_PALADINEXP:      m_getCharacterValue(CHAR_PALADINEXP);    break;
      case SUBOP_GET_CHAR_AGE:             m_getCharacterValue(CHAR_AGE);           break;
      case SUBOP_GET_CHAR_MAXAGE:          m_getCharacterValue(CHAR_MAXAGE);        break;
      case SUBOP_GET_CHAR_MAXMOVE:         m_getCharacterValue(CHAR_MAXMOVE);       break;
      case SUBOP_GET_CHAR_STR:             m_getCharacterValue(CHAR_STR);           break;
      case SUBOP_GET_CHAR_STRMOD:          m_getCharacterValue(CHAR_STRMOD);        break;
      case SUBOP_GET_CHAR_INT:             m_getCharacterValue(CHAR_INT);           break;
      case SUBOP_GET_CHAR_WIS:             m_getCharacterValue(CHAR_WIS);           break;
      case SUBOP_GET_CHAR_DEX:             m_getCharacterValue(CHAR_DEX);           break;
      case SUBOP_GET_CHAR_CON:             m_getCharacterValue(CHAR_CON);           break;
      case SUBOP_GET_CHAR_CHA:             m_getCharacterValue(CHAR_CHA);           break;
      case SUBOP_GET_CHAR_MAXENC:          m_getCharacterValue(CHAR_MAXENC);        break;
      case SUBOP_GET_CHAR_ENC:             m_getCharacterValue(CHAR_ENC);           break;
      case SUBOP_GET_CHAR_GENDER:          m_getCharacterValue(CHAR_GENDER);        break;
      case SUBOP_GET_CHAR_CLASS:           m_getCharacterValue(CHAR_CLASS);         break;
      case SUBOP_GET_CHAR_ALIGNMENT:       m_getCharacterValue(CHAR_ALIGNMENT);     break;
      case SUBOP_GET_CHAR_STATUS:          m_getCharacterValue(CHAR_STATUS);        break;
      case SUBOP_GET_CHAR_UNDEAD:          m_getCharacterValue(CHAR_UNDEAD);        break;
      case SUBOP_GET_CHAR_SIZE:            m_getCharacterValue(CHAR_SIZE);          break;
      case SUBOP_GET_CHAR_MAGICRESIST:     m_getCharacterValue(CHAR_MAGICRESIST);   break;
      case SUBOP_GET_CHAR_SAVEVSPPDM:      m_getCharacterValue(CHAR_SAVEVSPPDM);    break;
      case SUBOP_GET_CHAR_SAVEVSPP:        m_getCharacterValue(CHAR_SAVEVSPP);      break;
      case SUBOP_GET_CHAR_SAVEVSRSW:       m_getCharacterValue(CHAR_SAVEVSRSW);     break;
      case SUBOP_GET_CHAR_SAVEVSBR:        m_getCharacterValue(CHAR_SAVEVSBR);      break;
      case SUBOP_GET_CHAR_SAVEVSSP:        m_getCharacterValue(CHAR_SAVEVSSP);      break;
      case SUBOP_GET_CHAR_CLERICLVL:       m_getCharacterValue(CHAR_CLERICLVL);     break;
      case SUBOP_GET_CHAR_FIGHTERLVL:      m_getCharacterValue(CHAR_FIGHTERLVL);    break;
      case SUBOP_GET_CHAR_RANGERLVL:       m_getCharacterValue(CHAR_RANGERLVL);     break;
      case SUBOP_GET_CHAR_DRUIDLVL:        m_getCharacterValue(CHAR_DRUIDLVL);      break;
      case SUBOP_GET_CHAR_PALADINLVL:      m_getCharacterValue(CHAR_PALADINLVL);    break;
      case SUBOP_GET_CHAR_THIEFLVL:        m_getCharacterValue(CHAR_THIEFLVL);      break;
      case SUBOP_GET_CHAR_MAGUSERLVL:      m_getCharacterValue(CHAR_MAGUSERLVL);    break;
      case SUBOP_GET_CHAR_CLERICPREVLVL:   m_getCharacterValue(CHAR_CLERICPREVLVL); break;
      case SUBOP_GET_CHAR_FIGHTERPREVLVL:  m_getCharacterValue(CHAR_FIGHTERPREVLVL);break;
      case SUBOP_GET_CHAR_RANGERPREVLVL:   m_getCharacterValue(CHAR_RANGERPREVLVL); break;
      case SUBOP_GET_CHAR_DRUIDPREVLVL:    m_getCharacterValue(CHAR_DRUIDPREVLVL);  break;
      case SUBOP_GET_CHAR_PALADINPREVLVL:  m_getCharacterValue(CHAR_PALADINPREVLVL);break;
      case SUBOP_GET_CHAR_THIEFPREVLVL:    m_getCharacterValue(CHAR_THIEFPREVLVL);  break;
      case SUBOP_GET_CHAR_MAGUSERPREVLVL:  m_getCharacterValue(CHAR_MAGUSERPREVLVL);break;
      case SUBOP_GET_CHAR_CLERICPDLVL:     m_getCharacterValue(CHAR_CLERICPDLVL);   break;
      case SUBOP_GET_CHAR_FIGHTERPDLVL:    m_getCharacterValue(CHAR_FIGHTERPDLVL);  break;
      case SUBOP_GET_CHAR_RANGERPDLVL:     m_getCharacterValue(CHAR_RANGERPDLVL);   break;
      case SUBOP_GET_CHAR_DRUIDPDLVL:      m_getCharacterValue(CHAR_DRUIDPDLVL);    break;
      case SUBOP_GET_CHAR_PALADINPDLVL:    m_getCharacterValue(CHAR_PALADINPDLVL);  break;
      case SUBOP_GET_CHAR_THIEFPDLVL:      m_getCharacterValue(CHAR_THIEFPDLVL);    break;
      case SUBOP_GET_CHAR_MAGUSERPDLVL:    m_getCharacterValue(CHAR_MAGUSERPDLVL);  break;
      case SUBOP_GET_CHAR_NBRHITDICE:      m_getCharacterValue(CHAR_NBRHITDICE);    break;
      case SUBOP_GET_CHAR_NBRATTACKS:      m_getCharacterValue(CHAR_NBRATTACKS);    break;
      case SUBOP_GET_CHAR_MORALE:          m_getCharacterValue(CHAR_MORALE);        break;
      case SUBOP_GET_CHAR_OPENDOORS:       m_getCharacterValue(CHAR_OPENDOORS);     break;
      case SUBOP_GET_CHAR_OPENMAGICDOORS:  m_getCharacterValue(CHAR_OPENMAGICDOORS);break;
      case SUBOP_GET_CHAR_BENDLIFT:        m_getCharacterValue(CHAR_BENDLIFT);      break;
      case SUBOP_GET_CHAR_PICKPOCKETS:     m_getCharacterValue(CHAR_PICKPOCKETS);   break;
      case SUBOP_GET_CHAR_OPENLOCKS:       m_getCharacterValue(CHAR_OPENLOCKS);     break;
      case SUBOP_GET_CHAR_FINDTRAPS:       m_getCharacterValue(CHAR_FINDTRAPS);     break;
      case SUBOP_GET_CHAR_MOVESILENT:      m_getCharacterValue(CHAR_MOVESILENT);    break;
      case SUBOP_GET_CHAR_HIDESHADOWS:     m_getCharacterValue(CHAR_HIDESHADOWS);   break;
      case SUBOP_GET_CHAR_HEARNOISE:       m_getCharacterValue(CHAR_HEARNOISE);     break;
      case SUBOP_GET_CHAR_CLIMBWALLS:      m_getCharacterValue(CHAR_CLIMBWALLS);    break;
      case SUBOP_GET_CHAR_READLANG:        m_getCharacterValue(CHAR_READLANG);      break;
      case SUBOP_GET_CHAR_BLESS:           m_getCharacterValue(CHAR_BLESS);         break;
      case SUBOP_GET_CHAR_CURSE:           m_getCharacterValue(CHAR_CURSE);         break;
      case SUBOP_GET_CHAR_UNDEADFEAR:      m_getCharacterValue(CHAR_UNDEADFEAR);    break;
      case SUBOP_GET_CHAR_ENLARGE:         m_getCharacterValue(CHAR_ENLARGE);       break;
      case SUBOP_GET_CHAR_REDUCE:          m_getCharacterValue(CHAR_REDUCE);        break;
      case SUBOP_GET_CHAR_CHARMPERSON:     m_getCharacterValue(CHAR_CHARMPERSON);   break;
      case SUBOP_GET_CHAR_REFLECTGAZEATTACK: m_getCharacterValue(CHAR_REFLECTGAZEATTACK); break;
      case SUBOP_GET_CHAR_PROTFROMEVIL:    m_getCharacterValue(CHAR_PROTFROMEVIL);  break;
      case SUBOP_GET_CHAR_PROTFROMGOOD:    m_getCharacterValue(CHAR_PROTFROMGOOD);  break;
      case SUBOP_GET_CHAR_SHIELD:          m_getCharacterValue(CHAR_SHIELD);        break;
      case SUBOP_GET_CHAR_SLEEP:           m_getCharacterValue(CHAR_SLEEP);         break;
      case SUBOP_GET_CHAR_FOG:             m_getCharacterValue(CHAR_FOG);           break;
      case SUBOP_GET_CHAR_ENTANGLE:        m_getCharacterValue(CHAR_ENTANGLE);      break;
      case SUBOP_GET_CHAR_INVISIBLETOANIMALS: m_getCharacterValue(CHAR_INVISIBLETOANIMALS); break;
      case SUBOP_GET_CHAR_INVISIBLETOUNDEAD: m_getCharacterValue(CHAR_INVISIBLETOUNDEAD); break;
      case SUBOP_GET_CHAR_NONUNDEADFEAR:   m_getCharacterValue(CHAR_NONUNDEADFEAR); break;
      case SUBOP_GET_CHAR_SANCTUARY:       m_getCharacterValue(CHAR_SANCTUARY);     break;
      case SUBOP_GET_CHAR_SHILLELAGH:      m_getCharacterValue(CHAR_SHILLELAGH);    break;
      case SUBOP_GET_CHAR_DISPLACEMENT:    m_getCharacterValue(CHAR_DISPLACEMENT);  break;
      case SUBOP_GET_CHAR_WIZARDRY:        m_getCharacterValue(CHAR_WIZARDRY);      break;
      case SUBOP_GET_CHAR_DETECTMAGIC:     m_getCharacterValue(CHAR_DETECTMAGIC);   break;
      case SUBOP_GET_CHAR_VORPALATTACK:    m_getCharacterValue(CHAR_VORPALATTACK);  break;
      case SUBOP_GET_CHAR_HOLDPERSON:      m_getCharacterValue(CHAR_HOLDPERSON);    break;
      case SUBOP_GET_CHAR_SILENCE:         m_getCharacterValue(CHAR_SILENCE);       break;
      case SUBOP_GET_CHAR_POISONED:        m_getCharacterValue(CHAR_POISONED);      break;
      case SUBOP_GET_CHAR_SLOWPOISON:      m_getCharacterValue(CHAR_SLOWPOISON);    break;
      case SUBOP_GET_CHAR_MIRRORIMAGE:     m_getCharacterValue(CHAR_MIRRORIMAGE);   break;
      case SUBOP_GET_CHAR_INVISIBLE:       m_getCharacterValue(CHAR_INVISIBLE);     break;
      case SUBOP_GET_CHAR_ENFEEBLED:       m_getCharacterValue(CHAR_ENFEEBLED);     break;
      case SUBOP_GET_CHAR_DAMAGEBONUS:     m_getCharacterValue(CHAR_DAMAGEBONUS);   break;
      case SUBOP_GET_CHAR_HITBONUS:        m_getCharacterValue(CHAR_HITBONUS);      break;
      case SUBOP_GET_CHAR_BLINDNESS:       m_getCharacterValue(CHAR_BLINDNESS);     break;
      case SUBOP_GET_CHAR_DISEASED:        m_getCharacterValue(CHAR_DISEASED);      break;
        
      case SUBOP_INVOKESPELLONTARGETAS:
        {
          m_popString1(); // caster actor string
          m_popString2(); // spell name
          m_popString3(); // target actor string

          ActorType actor = m_StringToActor((LPCSTR)m_string3);
          
          CHARACTER *tdude = (CHARACTER*)GetCurrentlyActiveContext(&actor);          
          if (tdude==NULL)
          {
            m_interpretError("Invalid target actor data in $SUBOP_INVOKESPELLONTARGET");
            m_pushSP(m_false);
            break;
          }

          actor = m_StringToActor((LPCSTR)m_string1);

          CHARACTER *cdude = (CHARACTER*)GetCurrentlyActiveContext(&actor);          
          if (cdude==NULL)
          {
            m_interpretError("Invalid caster actor data in $SUBOP_INVOKESPELLONTARGET");
            m_pushSP(m_false);
            break;
          }

          int spelldbkey = spellData.FindName(m_string2);
          if (spelldbkey == -1)
          {
            CString err;
            err.Format("Invalid spell name \'%s\' in $SUBOP_INVOKESPELLONTARGET", m_string2);
            m_interpretError(err);
            m_pushSP(m_false);
            break;
          }

          // invent high-level character to cast spell
          SPELL_DATA *pSpell = spellData.GetSpellData(spelldbkey);
          if (pSpell == NULL)
          {
            CString err;
            err.Format("Failed spell db lookup for spell \'%s\' in $SUBOP_INVOKESPELLONTARGET", m_string2);
            m_interpretError(err);
            m_pushSP(m_false);
            break;
          }

#ifdef UAFEngine    
          cdude->InvokeSpellOnTarget(*tdude, -1, spelldbkey);
#endif
          m_pushSP(m_true);
        }
        break;

        // cast spell on target using imaginary 30th level Cleric/MU
      case SUBOP_INVOKESPELLONTARGET:
        {
          m_popString1(); // spell name
          m_popString2(); // target actor string

          ActorType actor = m_StringToActor((LPCSTR)m_string2);
          
          CHARACTER *tdude = (CHARACTER*)GetCurrentlyActiveContext(&actor);          
          if (tdude==NULL)
          {
            m_interpretError("Invalid target actor data in $SUBOP_INVOKESPELLONTARGET");
            m_pushSP(m_false);
            break;
          }

          int spelldbkey = spellData.FindName(m_string1);
          if (spelldbkey == -1)
          {
            CString err;
            err.Format("Invalid spell name \'%s\' in $SUBOP_INVOKESPELLONTARGET", m_string1);
            m_interpretError(err);
            m_pushSP(m_false);
            break;
          }

          // invent high-level character to cast spell
          SPELL_DATA *pSpell = spellData.GetSpellData(spelldbkey);
          if (pSpell == NULL)
          {
            CString err;
            err.Format("Failed spell db lookup for spell \'%s\' in $SUBOP_INVOKESPELLONTARGET", m_string1);
            m_interpretError(err);
            m_pushSP(m_false);
            break;
          }

#ifdef UAFEngine    
          CHARACTER TempDude;
          // create high-level spell caster
          //
          // create Cleric/MU so either spell class can be cast
          //
          // when other spell classes are added, this will need to be 
          // changed to create specific char classes (can't have cl/mu/dr/rang/etc
          // as valid char class.
          //
          DWORD StartExp = 2 + 
                           GetMinClericExpForLevel(29) +
                           GetMinMagicUserExpForLevel(29);

          TempDude.SetAlignment(ChaoticNeutral);
          TempDude.race(Human);
          TempDude.SetGender(Male);
          TempDude.SetClass(C_MU); // 
          TempDude.SetType(NPC_TYPE);
          TempDude.SetName("TempGPDLClericMU");
          TempDude.generateNewCharacter(StartExp,START_EXP_VALUE);  
          TempDude.SetStr(18);
          TempDude.SetInt(18);
          TempDude.SetWis(18);
          TempDude.SetDex(18);
          TempDude.SetCon(18);
          TempDude.SetCha(18);
          TempDude.InvokeSpellOnTarget(*tdude, -1, spelldbkey);
#else
          m_pushSP(m_true); // editor only
#endif
        }
        break;

      case SUBOP_CHAR_REMOVEALLSPELLS:
        {
          m_popString1(); // spell level
          m_popString2(); // actor string

          ActorType actor = m_StringToActor((LPCSTR)m_string2);
          
          CHARACTER *dude = (CHARACTER*)GetCurrentlyActiveContext(&actor);          
          if (dude==NULL)
          {
            m_interpretError("Invalid actor data in $CHAR_REMOVEALLSPELLS");
            m_pushSP(m_false);
            break;
          }

          int spelllvl = atoi(m_string1);
          int count = 0;

#ifdef UAFEngine          
          for (int i=spelllvl;i>0;i--)          
            count += dude->RemoveAllSpellEffectsByLevel(i);
#endif
          m_string3.Format("%i", count);
          m_pushSP(m_string3);
        }
        break;

      case SUBOP_CHAR_REMOVESPELL:
        {
          m_popString1(); // spell name
          m_popString2(); // target actor string

          ActorType actor = m_StringToActor((LPCSTR)m_string2);
          
          CHARACTER *dude = (CHARACTER*)GetCurrentlyActiveContext(&actor);          
          if (dude==NULL)
          {
            m_interpretError("Invalid actor data in $SUBOP_CHAR_REMOVESPELL");
            m_pushSP(m_false);
            break;
          }

          BOOL wasremoved=FALSE;
#ifdef UAFEngine          
          wasremoved = dude->RemoveSpellByName(m_string1);
#endif
          m_string3.Format("%i", (wasremoved?TRUE:FALSE));
          m_pushSP(m_string3);
        }
        break;

      case SUBOP_CHAR_DISPELMAGIC:
        {
          m_popString1(); // spell level
          m_popString2(); // actor string

          ActorType actor = m_StringToActor((LPCSTR)m_string2);
          
          CHARACTER *dude = (CHARACTER*)GetCurrentlyActiveContext(&actor);          
          if (dude==NULL)
          {
            m_interpretError("Invalid actor data in $SUBOP_CHAR_DISPELMAGIC");
            m_pushSP(m_false);
            break;
          }

          int spelllvl = atoi(m_string1);
          int count = 0;

#ifdef UAFEngine          
          count += dude->DispelSpellEffectsByLevel(spelllvl);
#endif
          m_string3.Format("%i", count);
          m_pushSP(m_string3);
        }
        break;

      case SUBOP_CHAR_REMOVEALLITEMCURSE:
        {
          m_popString1(); // actor string

          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          
          CHARACTER *dude = (CHARACTER*)GetCurrentlyActiveContext(&actor);          
          if (dude==NULL)
          {
            m_interpretError("Invalid actor data in $SUBOP_CHAR_REMOVEALLITEMCURSE");
            m_pushSP(m_false);
            break;
          }

#ifdef UAFEngine          
          dude->ClearAllItemCursedFlags();
#endif
          m_pushSP(m_true);
        }
        break;

      case SUBOP_GET_CHAR_SEX:
        m_popInteger1();
        
#ifdef UAFEngine
        if (IsCombatActive())
        {
          if (   (m_Integer1<0) 
              || (m_Integer1>=GetNumCombatants())) 
          {
            m_interpretError("Illegal party member # in $GET_CHAR_SEX");
            break;
          }          
          m_string1=GenderNames[GetCombatantGender(m_Integer1)];
        }
        else
#endif
        {          
          if (   (m_Integer1<0) 
              || (m_Integer1>=GetPartySize(party))) 
          {
            m_interpretError("Illegal party member # in $GET_CHAR_SEX");
            break;
          };
          m_string1=GenderNames[GetCharacterGender(party,m_Integer1)];
        }
        m_pushString1();
        break;
      case SUBOP_GET_CHAR_TYPE:            m_getCharacterValue(CHAR_CHARTYPE);      break;
      case SUBOP_GET_GLOBAL_ASL:
        m_popString1(); // Get key.
        m_string2=LookupGlobalASL(globalData,m_string1);
        m_pushSP(m_string2);
        break;
      case SUBOP_GET_PARTY_ASL:
        m_popString1(); // Get key.
        m_string2=LookupPartyASL(party, m_string1);
        m_pushSP(m_string2);
        break;

      case SUBOP_GET_VAULT_MONEYAVAILABLE:
        {
          m_popInteger1();
          
          double total = globalData.VaultTotal();
          
          if (m_Integer1==0) // no conversion
          {
            m_Integer2 = (int)total;
          }
          else
          {
            if ((m_Integer1 >= 1)&&(m_Integer1 <= 10))
            {
              m_Integer2 = 
                (int)globalData.moneyData.Convert(total, 
                                                  globalData.moneyData.GetBaseType(),
                                                  globalData.moneyData.GetItemClass(m_Integer1-1));
            }
            else
              m_Integer2=0;
          }
          m_pushInteger2();
        }
        break;

      case SUBOP_GET_PARTY_MONEYAVAILABLE:
        {
          m_popInteger1();

          double total = 0;
          for (int i=0;i<party.numCharacters;i++)
            total += party.characters[i].money.Total();

          if (m_Integer1==0) // no conversion
          {
            m_Integer2 = (int)total;
          }
          else
          {
            if ((m_Integer1 >= 1)&&(m_Integer1 <= 10))
            {
              m_Integer2 = 
                 (int)globalData.moneyData.Convert(total, 
                                              globalData.moneyData.GetBaseType(),
                                              globalData.moneyData.GetItemClass(m_Integer1-1));
            }
            else
              m_Integer2=0;
          }
          m_pushInteger2();
        }
        break;

      case SUBOP_GIVE_CHAR_ITEM:
        {
          m_popString1(); // item name
          m_popString2(); // actor string
          ActorType actor = m_StringToActor((LPCSTR)m_string2);
          CHARACTER *dude = (CHARACTER*)GetCurrentlyActiveContext(&actor);          
          if (dude==NULL)
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();
            break;
          }

          int itemkey = itemData.FindName(m_string1);
          BOOL success = dude->addCharacterItem(itemkey, 
                                                itemData.GetItemBundleQty(itemkey), 
                                                -1, // default charges
                                                TRUE, // is identified
                                                -1); // no cost

          m_pushSP(success?m_true:m_false);
        }
        break;

      case SUBOP_TAKE_CHAR_ITEM:
        {
          m_popString1(); // item name
          m_popString2(); // actor string
          ActorType actor = m_StringToActor((LPCSTR)m_string2);
          CHARACTER *dude = (CHARACTER*)GetCurrentlyActiveContext(&actor);          
          if (dude==NULL)
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();
            break;
          }
          
          int itemkey = itemData.FindName(m_string1);
          BOOL success = FALSE;

          POSITION pos = dude->myItems.GetHeadPosition();
          while (pos != NULL)
          {
            if (dude->myItems.GetAt(pos).item == itemkey)
            {
              
              success = dude->delCharacterItem(dude->myItems.GetKeyAt(pos), 
                                               itemData.GetItemBundleQty(itemkey));
              break;
            }
            dude->myItems.GetNext(pos);
          }
          
          m_pushSP(success?m_true:m_false);
        }
        break;     
          
      case SUBOP_GREATER:
        m_popString1();
        m_popString2();
        m_pushSP(m_string2>m_string1?m_true:m_false);
        break;
      case SUBOP_GREATEREQUAL:
        m_popString1();
        m_popString2();
        m_pushSP(m_string2>=m_string1?m_true:m_false);
        break;
      case SUBOP_GREP:
        // bool GREP (pattern, string)
        re_Set_EGrep(); // Set egrep mode
        m_popString1();   // The string to search.
        m_popString2();   // The pattern
        m_string1.MakeUpper();
        m_string2=upperPattern(m_string2);
        m_grepCResult=re_compile(LPCTSTR(m_string2), &m_grepPat);
        if (m_grepCResult!=NULL) {
          if (m_debugLevel&&DBG_messageBox) {
            CString msg;
            msg="Error compiling Grep pattern\n";
            msg+=m_string2;
            msg+="\n";
            msg+=CString(m_grepCResult);
            MsgBoxError(msg);
          };
          INTERP_RETURN(GPDL_GREPERROR);
        };
        m_Integer1=re_search(&m_grepPat,
                   LPCTSTR(m_string1),
                   m_string1.GetLength(),
                   0,
                   m_string1.GetLength(),
                   &m_grepReg);
        m_pushSP(m_Integer1<0?m_false:m_true);
        break;
      case SUBOP_HitPoints:
        m_popString1();
        m_string2 = m_HitPoints(m_string1);
        m_pushString2();
        break;
      case SUBOP_IF_PARTY_ASL:
        m_popString1(); // Get key.
        if (ExistsPartyASL(party, m_string1))
          m_pushSP(m_true);
        else 
          m_pushSP(m_false);
        break;

      case SUBOP_IS_AFFECTED_BY_SPELL:
        m_popString1(); // SpellName
        m_popString2(); // ActorType
        m_string3 = m_IsCharAffectedBySpell(m_string2, m_string1);
        m_pushString3();
        break;

      case SUBOP_IS_AFFECTED_BY_SPELL_ATTR:
        m_popString1(); // SpellName
        m_popString2(); // ActorType
        m_string3 = m_IsCharAffectedBySpellAttribute(m_string2, m_string1);
        m_pushString3();        
        break;

      case SUBOP_IndexOf:
        m_popString1();
        m_string2 = m_IndexOf(m_string1);
        m_pushString2();
        break;

      case SUBOP_InParty:
        m_popString1();
        m_string2 = m_InParty(m_string1);
        m_pushString2();
        break;
      case SUBOP_LAND:
        m_popString1();
        m_popString2();
        m_pushSP(((m_string1.GetLength()!=0) && m_string2.GetLength()!=0)?
                        m_true: m_false);
        break;

      case SUBOP_LAST_ATTACKER_OF:
        {
#ifdef UAFEngine
          if (!IsCombatActive())
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();
            break;
          }

          m_popString1();
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          COMBATANT *dude = (COMBATANT*)GetCurrentlyActiveContext(&actor);
      
          if (dude==NULL) 
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();     
            break;
          }

          COMBATANT *pLastAttacker = GetLastAttackerOf(dude);
          if (pLastAttacker != NULL)
          {
            actor = pLastAttacker->GetContext();
            m_string2 = m_ActorToString(&actor);
          }
          else
            m_string2 = m_ActorToString(&NULL_ACTOR);          
          m_pushString2();         
#else
          m_string2 = m_ActorToString(&NULL_ACTOR);
          m_pushString2();                      
#endif          
        }
        break;
         
      case SUBOP_LENGTH:
        m_popString1();
        m_uInteger1=m_string1.GetLength();
        m_pushUInteger1();
        break;
      case SUBOP_LESS:
        m_popString1();
        m_popString2();
        m_pushSP(m_string2<m_string1?m_true:m_false);
        break;
      case SUBOP_LESSEQUAL:
        m_popString1();
        m_popString2();
        m_pushSP(m_string2<=m_string1?m_true:m_false);
        break;

      case SUBOP_LEAST_DAMAGED_FRIENDLY:
        {
#ifdef UAFEngine
          if (!IsCombatActive())
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();
            break;
          }    
          
          COMBATANT *pLastAttacker = GetLeastDamagedFriendly();
          if (pLastAttacker != NULL)
          {
            ActorType actor = pLastAttacker->GetContext();
            m_string2 = m_ActorToString(&actor);
          }
          else
            m_string2 = m_ActorToString(&NULL_ACTOR);          
          m_pushString2();         
#else
          m_string2 = m_ActorToString(&NULL_ACTOR);
          m_pushString2();                      
#endif          
        }
        break;
      case SUBOP_LEAST_DAMAGED_ENEMY:
        {
#ifdef UAFEngine
          if (!IsCombatActive())
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();
            break;
          }
          
          COMBATANT *pLastAttacker = GetLeastDamagedEnemy();
          if (pLastAttacker != NULL)
          {
            ActorType actor = pLastAttacker->GetContext();
            m_string2 = m_ActorToString(&actor);
          }
          else
            m_string2 = m_ActorToString(&NULL_ACTOR);          
          m_pushString2();         
#else
          m_string2 = m_ActorToString(&NULL_ACTOR);
          m_pushString2();                      
#endif          
        }
        break;
        
      case SUBOP_LISTEN:
        m_discourseText.StartInput();
        m_state=GPDL_WAIT_INPUT;
        INTERP_RETURN(GPDL_WAIT_INPUT);
      case SUBOP_LOGIC_BLOCK_VALUE:
        m_popString1();
        m_string2=LookupGlobalASL(globalData,"LOGICBLOCKVALUES");
        if ( (m_string2 == "") || (m_string1 == ""))
        {
          m_string1 = "";
        }
        else
        {
          int index = 0;
          unsigned int k;
          unsigned int len = m_string2.GetLength();
          k = m_string1[0] - 'A';
          if ( (k<0) || (k>11))
          {
            m_string1 = "";
            m_pushString1();
            break;
          };
          for (i=0; i<=k; i++)
          {
            if (len < sizeof(int))
            {
              m_string1 = "";
              break;
            };
            k = *(int *)((LPCTSTR)(m_string2)+index);
            index += sizeof(int);
            len -= sizeof(int);
            if (len < k)
            {
              m_string1 = "";
              break;
            };
            if (i != k)
            {
              index += k;
              len -= k;
              continue;
            };
            m_string1 = CString((LPCTSTR)m_string2.Mid(index,k));
          };
        };
        m_pushString1();
        break;
      case SUBOP_Myself:
        m_string1 = m_Myself();
        m_pushString1();
        break;
      case SUBOP_nAND:
        m_popInteger1(GPDL_ILLPARAM);
        m_popInteger2(GPDL_ILLPARAM);
        m_Integer2 &= m_Integer1;
        m_pushInteger2();
        break;
      case SUBOP_LOR:
        m_popString1();
        m_popString2();
        m_pushSP(((m_string1.GetLength()!=0) || (m_string2.GetLength()!=0))?
                    m_true:m_false);
        break;
      case SUBOP_nISEQUAL:
        m_popInteger1(GPDL_ILLPARAM);
        m_popInteger2(GPDL_ILLPARAM);
        m_pushSP(m_Integer1==m_Integer2?m_true:m_false);
        break;
      case SUBOP_IsUndead:
        m_popString1();
        m_string2 = m_IsUndead(m_string1);
        m_pushString2();
        break;
      case SUBOP_MIDDLE:
        m_popInteger2(GPDL_BADINTEGER); //last index
        m_popInteger1(GPDL_BADINTEGER); //first index
        m_popString1();
        m_string2 = m_string1.Mid(m_Integer1, m_Integer2);
        m_pushString2();
        break;

      case SUBOP_MOST_DAMAGED_FRIENDLY:
        {
#ifdef UAFEngine
          if (!IsCombatActive())
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();
            break;
          }         
          
          COMBATANT *pLastAttacker = GetMostDamagedFriendly();
          if (pLastAttacker != NULL)
          {
            ActorType actor = pLastAttacker->GetContext();
            m_string2 = m_ActorToString(&actor);
          }
          else
            m_string2 = m_ActorToString(&NULL_ACTOR);          
          m_pushString2();         
#else
          m_string2 = m_ActorToString(&NULL_ACTOR);
          m_pushString2();                      
#endif          
        }
        break;
      case SUBOP_MOST_DAMAGED_ENEMY:
        {
#ifdef UAFEngine
          if (!IsCombatActive())
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();
            break;
          }          
          
          COMBATANT *pLastAttacker = GetMostDamagedEnemy();
          if (pLastAttacker != NULL)
          {
            ActorType actor = pLastAttacker->GetContext();
            m_string2 = m_ActorToString(&actor);
          }
          else
            m_string2 = m_ActorToString(&NULL_ACTOR);   
          
          m_pushString2();         
#else
          m_string2 = m_ActorToString(&NULL_ACTOR);
          m_pushString2();                      
#endif          
        }
        break;
        
      case SUBOP_nGREATER:
        m_popInteger1(GPDL_BADINTEGER);
        m_popInteger2(GPDL_BADINTEGER);
        m_pushSP(m_Integer2>m_Integer1?m_true:m_false);
        break;
      case SUBOP_nGREATEREQUAL:
        m_popInteger1(GPDL_ILLPARAM);
        m_popInteger2(GPDL_ILLPARAM);
        m_pushSP(m_Integer2>=m_Integer1?m_true:m_false);
        break;
      case SUBOP_nLESS:
        m_popInteger1(GPDL_BADINTEGER);
        m_popInteger2(GPDL_BADINTEGER);
        m_pushSP(m_Integer2<m_Integer1?m_true:m_false);
        break;
      case SUBOP_nMINUS:
        m_popInteger1(GPDL_BADINTEGER);
        m_popInteger2(GPDL_BADINTEGER);
        m_Integer2-=m_Integer1;
        m_pushInteger2();
        break;
      case SUBOP_Name:
        m_popString1();
        m_string2 = m_Name(m_string1);
        m_pushString2();
        break;

      case SUBOP_NEAREST_TO:
        {
#ifdef UAFEngine
          if (!IsCombatActive())
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();
            break;
          }
          
          m_popString1();
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          COMBATANT *dude = (COMBATANT*)GetCurrentlyActiveContext(&actor);
          
          if (dude==NULL) 
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();     
            break;
          }
          
          COMBATANT *pLastAttacker = GetNearestTo(dude);
          if (pLastAttacker != NULL)
          {
            actor = pLastAttacker->GetContext();
            m_string2 = m_ActorToString(&actor);
          }
          else
            m_string2 = m_ActorToString(&NULL_ACTOR);          
          m_pushString2();         
#else
          m_string2 = m_ActorToString(&NULL_ACTOR);
          m_pushString2();                      
#endif          
        }
        break;
      case SUBOP_NEAREST_ENEMY_TO:    
        {
#ifdef UAFEngine
          if (!IsCombatActive())
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();
            break;
          }
          
          m_popString1();
          ActorType actor = m_StringToActor((LPCSTR)m_string1);
          COMBATANT *dude = (COMBATANT*)GetCurrentlyActiveContext(&actor);
          
          if (dude==NULL) 
          {
            m_string2 = m_ActorToString(&NULL_ACTOR);
            m_pushString2();     
            break;
          }
          
          COMBATANT *pLastAttacker = GetNearestEnemyTo(dude);
          if (pLastAttacker != NULL)
          {
            actor = pLastAttacker->GetContext();
            m_string2 = m_ActorToString(&actor);
          }
          else
            m_string2 = m_ActorToString(&NULL_ACTOR);          
          m_pushString2();         
#else
          m_string2 = m_ActorToString(&NULL_ACTOR);
          m_pushString2();                      
#endif          
        }
        break;
        
      case SUBOP_nLESSEQUAL:
        m_popInteger1(GPDL_ILLPARAM);
        m_popInteger2(GPDL_ILLPARAM);
        m_pushSP(m_Integer2<=m_Integer1?m_true:m_false);
        break;
      case SUBOP_NOOP:
        break;
      case SUBOP_nNEGATE:
        m_popInteger1(GPDL_ILLPARAM);
        m_Integer2 = -m_Integer1;
        m_pushInteger2();
        break;
      case SUBOP_nNOTEQUAL:
        m_popInteger1(GPDL_ILLPARAM);
        m_popInteger2(GPDL_ILLPARAM);
        m_pushSP(m_Integer1!=m_Integer2?m_true:m_false);
        break;
      case SUBOP_nOR:
        m_popInteger1(GPDL_ILLPARAM);
        m_popInteger2(GPDL_ILLPARAM);
        m_Integer2 |= m_Integer1;
        m_pushInteger2();
        break;
      case SUBOP_NOT:
        if (m_dataStack[m_SP]==m_false) m_dataStack[m_SP]=m_true;
        else m_dataStack[m_SP]=m_false;
        break;
      case SUBOP_NOTEQUAL:
        m_popString1();
        m_popString2();
        m_pushSP(m_string1==m_string2?m_false:m_true);
        break;
      case SUBOP_nPERCENT:
        m_popInteger1(GPDL_BADINTEGER);
        m_popInteger2(GPDL_BADINTEGER);
        m_Integer2 %= m_Integer1;
        m_pushInteger2();
        break;
      case SUBOP_nPLUS:
        m_popInteger1(GPDL_BADINTEGER);
        m_popInteger2(GPDL_BADINTEGER);
        m_Integer2 += m_Integer1;
        m_pushInteger2();
        break;
      case SUBOP_nSLASH:
        m_popInteger1(GPDL_BADINTEGER);
        m_popInteger2(GPDL_BADINTEGER);
        m_Integer2 /= m_Integer1;
        m_pushInteger2();
        break;
      case SUBOP_nTIMES:
        m_popInteger1(GPDL_BADINTEGER);
        m_popInteger2(GPDL_BADINTEGER);
        m_Integer2 *= m_Integer1;
        m_pushInteger2();
        break;
      case SUBOP_NUMERIC:
        if (m_popInteger(m_bool1, m_uInteger1)==GPDL_OK)
          m_pushSP(m_true);
        else m_pushSP(m_false);
        break;
      case SUBOP_nXOR:
        m_popInteger1(GPDL_ILLPARAM);
        m_popInteger2(GPDL_ILLPARAM);
        m_Integer2 ^= m_Integer1;
        m_pushInteger2();
        break;
      case SUBOP_OVER:
        m_popString1();
        m_popString2();
        m_pushString2();
        m_pushString1();
        m_pushString2();
        break;
      case SUBOP_LISTENTEXT:
        m_pushSP(m_listenText);
        break;
      case SUBOP_ONE:
        m_pushSP(m_true);
        break;
      case SUBOP_PARTYSIZE:
        m_Integer1=GetPartySize(party);
        m_pushInteger1();
        break;
      case SUBOP_PLUS: // string concatenation
        m_popString1();
        m_popString2();
        m_string2+=m_string1;
        m_pushString2();
        break;
      case SUBOP_POP:
        m_popString1();
        break;
      case SUBOP_Race:
        m_popString1();
        m_string2 = m_Race(m_string1);
        m_pushString2();
        break;
      case SUBOP_RANDOM:
        m_popInteger1(GPDL_ILLPARAM);
        if (m_Integer1<=0) {
          m_interpStatus=GPDL_ILLPARAM;
          break;
        };
        //m_uInteger1=rand()%m_Integer1;
        m_uInteger1=rollDice(m_Integer1, 1, 0.0);
        m_pushUInteger1();
        break;
      case SUBOP_SaveVsSpell:
        m_popString1();
        m_string2 = m_SaveVsSpell(m_string1);
        m_pushString2();
        break;
      case SUBOP_SaveVsBreathWeapon:
        m_popString1();
        m_string2 = m_SaveVsBreathWeapon(m_string1);
        m_pushString2();
        break;
      case SUBOP_SaveVsRodStaffWand:
        m_popString1();
        m_string2 = m_SaveVsRodStaffWand(m_string1);
        m_pushString2();
        break;
      case SUBOP_SaveVsPetPoly:
        m_popString1();
        m_string2 = m_SaveVsPetPoly(m_string1);
        m_pushString2();
        break;
      case SUBOP_SaveVsParPoiDM:
        m_popString1();
        m_string2 = m_SaveVsParPoiDM(m_string1);
        m_pushString2();
        break;
      case SUBOP_SAY:
        m_popString1();
#ifdef UAFEngine
        m_discourseText.SetNewTop();
        m_discourseText.SetText(m_string1, GetTextBoxCharWidth());
        m_discourseText.Display();
        m_state=GPDL_WAIT_ACK;
#endif        
        m_pushSP(m_false); // All functions must return a value!
        INTERP_RETURN(GPDL_WAIT_ACK);
      case SUBOP_SET_CHAR_AC:            m_setCharacterValue(CHAR_AC);             break;
      case SUBOP_SET_CHAR_HITPOINTS:     m_setCharacterValue(CHAR_HITPOINTS);      break;
      case SUBOP_SET_CHAR_THACO:         m_setCharacterValue(CHAR_THACO);          break;
      case SUBOP_SET_CHAR_RDYTOTRAIN:    m_setCharacterValue(CHAR_RDYTOTRAIN);     break;
      case SUBOP_SET_CHAR_CLERICEXP:     m_setCharacterValue(CHAR_CLERICEXP);      break;
      case SUBOP_SET_CHAR_MAGICUSEREXP:  m_setCharacterValue(CHAR_MAGICUSEREXP);   break;
      case SUBOP_SET_CHAR_FIGHTEREXP:    m_setCharacterValue(CHAR_FIGHTEREXP);     break;
      case SUBOP_SET_CHAR_RANGEREXP:     m_setCharacterValue(CHAR_RANGEREXP);      break;
      case SUBOP_SET_CHAR_THIEFEXP:      m_setCharacterValue(CHAR_THIEFEXP);       break;
      case SUBOP_SET_CHAR_DRUIDEXP:      m_setCharacterValue(CHAR_DRUIDEXP);       break;
      case SUBOP_SET_CHAR_PALADINEXP:    m_setCharacterValue(CHAR_PALADINEXP);     break;
      case SUBOP_SET_CHAR_AGE:           m_setCharacterValue(CHAR_AGE);            break;
      case SUBOP_SET_CHAR_MAXAGE:        m_setCharacterValue(CHAR_MAXAGE);         break;
      case SUBOP_SET_CHAR_MAXMOVE:       m_setCharacterValue(CHAR_MAXMOVE);        break;
      case SUBOP_SET_CHAR_STR:           m_setCharacterValue(CHAR_STR);            break;
      case SUBOP_SET_CHAR_STRMOD:        m_setCharacterValue(CHAR_STRMOD);         break;
      case SUBOP_SET_CHAR_INT:           m_setCharacterValue(CHAR_INT);            break;
      case SUBOP_SET_CHAR_WIS:           m_setCharacterValue(CHAR_WIS);            break;
      case SUBOP_SET_CHAR_DEX:           m_setCharacterValue(CHAR_DEX);            break;
      case SUBOP_SET_CHAR_CON:           m_setCharacterValue(CHAR_CON);            break;
      case SUBOP_SET_CHAR_CHA:           m_setCharacterValue(CHAR_CHA);            break;
      case SUBOP_SET_CHAR_MAXENC:        m_setCharacterValue(CHAR_MAXENC);         break;
      case SUBOP_SET_CHAR_GENDER:        m_setCharacterValue(CHAR_GENDER);         break;
      case SUBOP_SET_CHAR_CLASS:         m_setCharacterValue(CHAR_CLASS);          break;
      case SUBOP_SET_CHAR_ALIGNMENT:     m_setCharacterValue(CHAR_ALIGNMENT);      break;
      case SUBOP_SET_CHAR_STATUS:        m_setCharacterValue(CHAR_STATUS);         break;
      case SUBOP_SET_CHAR_UNDEAD:        m_setCharacterValue(CHAR_UNDEAD);         break;
      case SUBOP_SET_CHAR_SIZE:          m_setCharacterValue(CHAR_SIZE);           break;
      case SUBOP_SET_CHAR_MAGICRESIST:   m_setCharacterValue(CHAR_MAGICRESIST);    break;
      case SUBOP_SET_CHAR_SAVEVSPPDM:    m_setCharacterValue(CHAR_SAVEVSPPDM);     break;
      case SUBOP_SET_CHAR_SAVEVSPP:      m_setCharacterValue(CHAR_SAVEVSPP);       break;
      case SUBOP_SET_CHAR_SAVEVSRSW:     m_setCharacterValue(CHAR_SAVEVSRSW);      break;
      case SUBOP_SET_CHAR_SAVEVSBR:      m_setCharacterValue(CHAR_SAVEVSBR);       break;
      case SUBOP_SET_CHAR_SAVEVSSP:      m_setCharacterValue(CHAR_SAVEVSSP);       break;
      case SUBOP_SET_CHAR_MORALE:        m_setCharacterValue(CHAR_MORALE);         break;
      case SUBOP_SET_CHAR_OPENDOORS:     m_setCharacterValue(CHAR_OPENDOORS);      break;
      case SUBOP_SET_CHAR_OPENMAGICDOORS:m_setCharacterValue(CHAR_OPENMAGICDOORS); break;
      case SUBOP_SET_CHAR_BENDLIFT:      m_setCharacterValue(CHAR_BENDLIFT);       break;
      case SUBOP_SET_CHAR_PICKPOCKETS:   m_setCharacterValue(CHAR_PICKPOCKETS);    break;
      case SUBOP_SET_CHAR_OPENLOCKS:     m_setCharacterValue(CHAR_OPENLOCKS);      break;
      case SUBOP_SET_CHAR_FINDTRAPS:     m_setCharacterValue(CHAR_FINDTRAPS);      break;
      case SUBOP_SET_CHAR_MOVESILENT:    m_setCharacterValue(CHAR_MOVESILENT);     break;
      case SUBOP_SET_CHAR_HIDESHADOWS:   m_setCharacterValue(CHAR_HIDESHADOWS);    break;
      case SUBOP_SET_CHAR_HEARNOISE:     m_setCharacterValue(CHAR_HEARNOISE);      break;
      case SUBOP_SET_CHAR_CLIMBWALLS:    m_setCharacterValue(CHAR_CLIMBWALLS);     break;
      case SUBOP_SET_CHAR_READLANG:      m_setCharacterValue(CHAR_READLANG);       break;           
      case SUBOP_SET_CHAR_BLESS:         m_setCharacterValue(CHAR_BLESS);          break;
      case SUBOP_SET_CHAR_CURSE:         m_setCharacterValue(CHAR_CURSE);          break;
      case SUBOP_SET_CHAR_UNDEADFEAR:    m_setCharacterValue(CHAR_UNDEADFEAR);     break;
      case SUBOP_SET_CHAR_ENLARGE:       m_setCharacterValue(CHAR_ENLARGE);        break;
      case SUBOP_SET_CHAR_REDUCE:        m_setCharacterValue(CHAR_REDUCE);         break;
      case SUBOP_SET_CHAR_CHARMPERSON:   m_setCharacterValue(CHAR_CHARMPERSON);    break;
      case SUBOP_SET_CHAR_REFLECTGAZEATTACK: m_setCharacterValue(CHAR_REFLECTGAZEATTACK); break;
      case SUBOP_SET_CHAR_PROTFROMEVIL:  m_setCharacterValue(CHAR_PROTFROMEVIL);   break;
      case SUBOP_SET_CHAR_PROTFROMGOOD:  m_setCharacterValue(CHAR_PROTFROMGOOD);   break;
      case SUBOP_SET_CHAR_SHIELD:        m_setCharacterValue(CHAR_SHIELD);         break;
      case SUBOP_SET_CHAR_SLEEP:         m_setCharacterValue(CHAR_SLEEP);          break;
      case SUBOP_SET_CHAR_FOG:           m_setCharacterValue(CHAR_FOG);            break;
      case SUBOP_SET_CHAR_ENTANGLE:      m_setCharacterValue(CHAR_ENTANGLE);       break;
      case SUBOP_SET_CHAR_INVISIBLETOANIMALS: m_setCharacterValue(CHAR_INVISIBLETOANIMALS); break;
      case SUBOP_SET_CHAR_INVISIBLETOUNDEAD: m_setCharacterValue(CHAR_INVISIBLETOUNDEAD); break;
      case SUBOP_SET_CHAR_NONUNDEADFEAR: m_setCharacterValue(CHAR_NONUNDEADFEAR);  break;
      case SUBOP_SET_CHAR_SANCTUARY:     m_setCharacterValue(CHAR_SANCTUARY);      break;
      case SUBOP_SET_CHAR_SHILLELAGH:    m_setCharacterValue(CHAR_SHILLELAGH);     break;
      case SUBOP_SET_CHAR_DISPLACEMENT:  m_setCharacterValue(CHAR_DISPLACEMENT);   break;
      case SUBOP_SET_CHAR_WIZARDRY:      m_setCharacterValue(CHAR_WIZARDRY);       break;
      case SUBOP_SET_CHAR_DETECTMAGIC:   m_setCharacterValue(CHAR_DETECTMAGIC);    break;
      case SUBOP_SET_CHAR_VORPALATTACK:  m_setCharacterValue(CHAR_VORPALATTACK);   break;
      case SUBOP_SET_CHAR_HOLDPERSON:    m_setCharacterValue(CHAR_HOLDPERSON);     break;
      case SUBOP_SET_CHAR_SILENCE:       m_setCharacterValue(CHAR_SILENCE);        break;
      case SUBOP_SET_CHAR_POISONED:      m_setCharacterValue(CHAR_POISONED);       break;
      case SUBOP_SET_CHAR_SLOWPOISON:    m_setCharacterValue(CHAR_SLOWPOISON);     break;
      case SUBOP_SET_CHAR_MIRRORIMAGE:   m_setCharacterValue(CHAR_MIRRORIMAGE);    break;
      case SUBOP_SET_CHAR_INVISIBLE:     m_setCharacterValue(CHAR_INVISIBLE);      break;
      case SUBOP_SET_CHAR_ENFEEBLED:     m_setCharacterValue(CHAR_ENFEEBLED);      break;
      case SUBOP_SET_CHAR_DAMAGEBONUS:   m_setCharacterValue(CHAR_DAMAGEBONUS);    break;
      case SUBOP_SET_CHAR_HITBONUS:      m_setCharacterValue(CHAR_HITBONUS);       break;
      case SUBOP_SET_CHAR_BLINDNESS:     m_setCharacterValue(CHAR_BLINDNESS);      break;
      case SUBOP_SET_CHAR_DISEASED:      m_setCharacterValue(CHAR_DISEASED);       break;
         
      case SUBOP_SET_CHAR_ASL:
        if (IsCombatActive())
        {
          m_interpretError("Combat active in $SET_CHAR_ASL");
          m_string2="";
          m_pushSP(m_string2);  // Must supply a result
        }

        m_popString2();  // value
        m_popString1();  // key
        m_popInteger1(GPDL_ILLCHARNUM); // character number
        if (   (m_Integer1<0) 
            || (m_Integer1>=GetPartySize(party))) 
        {
          m_interpretError("Illegal party member # in $SET_CHAR_ASL");
          m_string2="";
          m_pushSP(m_string2);  // Must supply a result
          break;
        };
        InsertCharacterASL(
          party,
          m_Integer1,
          m_string1, 
          m_string2);
        m_pushSP(m_string2);  // Must supply a result
        break;
      case SUBOP_GET_GAME_CURRLEVEL:   m_getGameValue(GAME_CURRLEVEL); break;
      case SUBOP_GET_GAME_VERSION:     m_getGameValue(GAME_VERSION);   break;
      case SUBOP_SET_GLOBAL_ASL:
        m_popString1();  // The value
        m_popString2();  // The key
        InsertGlobalASL(
          globalData,
          m_string2,
          m_string1);
        m_pushSP(m_string1);
//        m_dataStack[m_SP]=m_variables.Set(m_string2, m_string1);
        break;
      case SUBOP_DEPRECATED_SET_LEVEL_STATS_ASL:
        m_popString2();  // value
        m_popString1();  // key
        InsertLevelASL(
          globalData,
          GetCurrentLevel(),
          m_string1,
          m_string2);
//        m_attributeInsert(
//          globalData.levelInfo.stats[globalData.currLevel].level_attributes,
//          m_string1, 
//          m_string2);
        m_pushSP(m_false);  // Must supply a result
        break;
      case SUBOP_SET_LEVEL_STATS_ASL:
        m_popString2();  // value
        m_popString1();  // key
        m_popString3();  //Level
        if (m_string3 == "") m_Integer1 = GetCurrentLevel();
        else m_Integer1 = atoi(m_string3);
        InsertLevelASL(
          globalData,
          m_Integer1,
          m_string1,
          m_string2);
//        m_attributeInsert(
//          globalData.levelInfo.stats[globalData.currLevel].level_attributes,
//          m_string1, 
//          m_string2);
        m_pushSP(m_false);  // Must supply a result
        break;
      case SUBOP_GET_PARTY_FACING:     m_getPartyValue(PARTY_FACING);     break;
      case SUBOP_SET_PARTY_FACING:     m_setPartyValue(PARTY_FACING);     break;
      case SUBOP_GET_PARTY_DAYS:       m_getPartyValue(PARTY_DAYS);       break;
      case SUBOP_SET_PARTY_DAYS:       m_setPartyValue(PARTY_DAYS);       break;
      case SUBOP_GET_PARTY_HOURS:      m_getPartyValue(PARTY_HOURS);      break;
      case SUBOP_SET_PARTY_HOURS:      m_setPartyValue(PARTY_HOURS);      break;
      case SUBOP_GET_PARTY_MINUTES:    m_getPartyValue(PARTY_MINUTES);    break;
      case SUBOP_SET_PARTY_MINUTES:    m_setPartyValue(PARTY_MINUTES);    break;
      case SUBOP_GET_PARTY_ACTIVECHAR: m_getPartyValue(PARTY_ACTIVECHAR); break;
      case SUBOP_SET_PARTY_ACTIVECHAR: m_setPartyValue(PARTY_ACTIVECHAR); break;
      case SUBOP_SET_PARTY_ASL:
        m_popString2();  // value
        m_popString1();  // key
//        m_attributeInsert(
//          party.party_attributes,
//          m_string1, 
//          m_string2);
        InsertPartyASL(
          party,
          m_string1, 
          m_string2);
        m_pushSP(m_string2);  // Must supply a result
        break;
      case SUBOP_SET_QUEST:
        {
          int i;
          char sign=' ', c;
          int value=0, oldValue;
          int valLen;
          m_string1 = m_popSP(); // Get value or increment
          m_string2 = m_popSP(); // Get quest name
          valLen = m_string1.GetLength();
          for (i=0; i<valLen; i++)
          {
            c=m_string1[i];
            if (c=='-') sign = '-';
            if (c=='+') sign = '+';
            if ( (c>='0') && (c<='9') ) value=value*10+c-'0';
          };
          if (sign != ' ')
          {
            oldValue = globalData.questData.GetStage(m_string2);
            if (sign == '+') value = oldValue + value;
            else value = oldValue - value;
          };
          globalData.questData.SetStage(m_string2, value);
          value = globalData.questData.GetStage(m_string2);
          m_string1.Format("%d",value);
          m_pushSP(m_string1);
        };
        break;
      case SUBOP_SMALL_PICTURE:
        m_popString1();
        m_pushString1(); // Must supply result.
        if (m_event->pic.picType!=SmallPicDib) break;
        m_event->pic.FreePic();
        m_event->pic.filename=m_string1;
        break;
      case SUBOP_Status:
        m_popString1();
        m_string2 = m_Status(m_string1);
        m_pushString2();
        break;
      case SUBOP_SWAP:
        m_popString1();
        m_popString2();
        m_pushString1();
        m_pushString2();
        break;

      case SUBOP_TARGETINDEX:
        m_string1 = m_IndexOf(m_Target());
        m_pushString1();
        break;
      case SUBOP_MYINDEX:
        m_string1 = m_IndexOf(m_Myself());
        m_pushString1();
        break;

      case SUBOP_Target:
        m_string1 = m_Target();
        m_pushString1();
        break;
      case SUBOP_iEQUAL:
        m_popString1();
        m_popString2();
        if (LongCompare(m_string1, m_string2) == 0) 
               m_pushSP(m_true); else m_pushSP(m_false);
        break;
      case SUBOP_iGREATER:
        m_popString1();
        m_popString2();
        if (LongCompare(m_string1, m_string2) < 0) 
               m_pushSP(m_true); else m_pushSP(m_false);
        break;
      case SUBOP_iLESS:
        m_popString1();
        m_popString2();
        if (LongCompare(m_string1, m_string2) > 0) 
               m_pushSP(m_true); else m_pushSP(m_false);
        break;
      case SUBOP_iPLUS:
        m_popString1();
        m_popString2();
        m_string3 = LongAdd(m_string1, m_string2);
        m_pushString3();
        break;
      case SUBOP_iMINUS:
        m_popString1();
        m_popString2();
        m_string3 = LongSubtract(m_string2, m_string1);
        m_pushString3();
        break;
      case SUBOP_iDIV:
        m_popString1();
        m_popString2();
        LongDivide(m_string2, m_string1, m_string3, m_string4);
        m_pushString3();
        break;
      case SUBOP_iMOD:
        m_popString1();
        m_popString2();
        LongDivide(m_string2, m_string1, m_string3, m_string4);
        m_pushString4();
        break;
      case SUBOP_iTIMES:
        // n-digit multiplication.
        m_popString1();
        m_popString2();
        m_string3 = LongMultiply(m_string1, m_string2);
        m_pushString3();
        /*
        {
          int k, k1, carry;
          int i,j;
          unsigned char *t;  // Temporary space for all three strings.
          unsigned char *m, *n, *r;
          int len1, len2;
          bool error=false;
          m_string1.TrimLeft();
          m_string2.TrimLeft();
          m_string1.TrimRight();
          m_string2.TrimRight();
          len1=m_string1.GetLength();
          len2=m_string2.GetLength();
          t=(unsigned char *)alloca(len1+len2+(len1+len2));
          if (t==NULL) {
            MsgBoxError("$TIMES alloca failure");
            break;
          };
          m=t;
          n=t+len1;
          r=n+len2;
          for (i=0; i<len1; i++) {
            m[i]=(unsigned char)(m_string1.GetAt(i)-'0');
            if (m[i]>9) error=true;
          };
          for (i=0; i<len2; i++) {
            n[i]=(unsigned char)(m_string2.GetAt(i)-'0');
            if (n[i]>9) error=true;
          };
          if (error) {
            m_error_message("argument to $TIMES not decimal digits");
            break;
          };
          for (i=0; i<len1+len2; i++) r[i]=0;
          for (i=len1-1; i>=0; i--) { // each digit of multiplier
            for (j=len2-1; j>=0; j--) { // each digit of multiplicand
              k=i+j+1; // where answer goes.
              r[k]=(unsigned char)(r[k]+(m[i]*n[j]));
              k1=k;
              carry=0;
              while (r[k1]>=10) {
                carry=r[k1]/10;
                r[k1]=(unsigned char)(r[k1]%10);
                k1--;
                r[k1]=(unsigned char)(r[k1]+carry);
              };
            };
          };
          k=-1;
          for (i=0; i<len1+len2; i++) {
            r[i]+='0';
            if ((r[i]!='0')&&(k==-1))k=i; // first non-zero digit
          };
          if (k==-1) k=0;
          m_dataStack[m_SP]=CString((char *)r+k,len1+len2-k);
        };
        */
        break;
      case SUBOP_ZERO:
        m_string1 = "0";
        m_pushString1();
        break;
      default:
        {
          CString msg;
          msg.Format("Illegal subop code\n0x%08x",m_subop);
          m_interpretError(msg);
        };
        return BreakExecute();
      };
      break;
    default:
      {
        CString msg;
        msg.Format("Illegal opcode\n0x%02x",m_opcode);
        m_interpretError(msg);
      };
      return BreakExecute();
    };
  };
  // Some sort of error must have occurred.
  INTERP_RETURN(m_interpStatus);
}


bool GPDL::IsActive(void) {
  return (m_state==GPDL_WAIT_ACK)||(m_state==GPDL_WAIT_INPUT);
}

GPDL_STATE GPDL::State(void) {
  if (m_state==GPDL_WAIT_ACK) return m_state;
  if (m_state==GPDL_WAIT_INPUT) return m_state;
  if (m_state==GPDL_UNINITIALIZED) return m_state;
  if (m_state==GPDL_IDLE) return m_state;
  MsgBoxError("Internal GPDL::State error");
  return BreakExecute();

}

CString GPDL::indexName(unsigned int i) {
  return m_index.name(i);
}

GPDL_STATE GPDL::Display(void) {
  switch (m_state) {
  case GPDL_IDLE:
    return m_state;
  case GPDL_WAIT_INPUT:
  case GPDL_WAIT_ACK:
    m_discourseText.Display();
    return m_state;
  default:
    MsgBoxError("Internal GPDL::Display error");
  };
  return m_state;
}

unsigned int GPDL::numIndex(void) {
  return m_index.numEntry();
}

void GPDL::m_IllegalStateMessageAndQuit(void) {
  CString msg;
  CString PC;
  switch(m_state) {
  case GPDL_UNDER_RP:
    msg="Return stack underflow";
    break;
  case GPDL_OVER_RP:
    msg="Return stack overflow";
    break;
  case GPDL_UNDER_SP:
    msg="Data stack underflow";
    break;
  case GPDL_OVER_SP:
    msg="Data stack overflow";
    break;
  case GPDL_ILLPARAM:
    msg="Illegal parameter to system function";
    break;
  default:
    msg="Unknown illegal state";
  };
  PC.Format("\nPC = 0x%06x",m_PC);
  msg="GPDL interpreter\n"+msg+PC;
  MsgBoxError(msg);
  m_state=GPDL_IDLE;
  m_discourseText.Clear();
}


GPDL_STATE GPDL::Char(UINT c) {
  // Return GPDL_ACCEPTED if character accepted.
  switch (m_state) {
  case GPDL_UNINITIALIZED:
  case GPDL_IDLE:
    return GPDL_IDLE;
  case GPDL_WAIT_ACK:
    switch(c) {
    case CARRIAGE_RETURN:
      BreakExecute();
      break;
    case DOWN_ARROW:
      m_discourseText.downArrow();
      break;
    case UP_ARROW:
      m_discourseText.upArrow();
      break;
    case SPACE:
      m_status=m_interpret();
      if (  (m_status==GPDL_IDLE)
        ||(m_status==GPDL_WAIT_INPUT)
        ||(m_status==GPDL_WAIT_ACK) ) {
        m_state=m_status;
      } else {
        BreakExecute();
      };
      break;
    default:
      return GPDL_IGNORED;
    };
    return GPDL_ACCEPTED;
  case GPDL_WAIT_INPUT:
    switch(c) {
    case DOWN_ARROW:
      m_discourseText.downArrow();
      return GPDL_ACCEPTED;
    case UP_ARROW:
      m_discourseText.upArrow();
      return GPDL_ACCEPTED;
    case CARRIAGE_RETURN:
        m_listenText=m_discourseText.GetInput();
        m_pushSP(m_listenText);
        m_status=m_interpret();
        if (  (m_status==GPDL_IDLE)
          ||(m_status==GPDL_WAIT_INPUT)
          ||(m_status==GPDL_WAIT_ACK) ) {
          m_state=m_status;
        } else {
          BreakExecute();
        };
        return GPDL_ACCEPTED;
    default:
      return m_discourseText.Char(c);
    };
  case GPDL_UNDER_RP:
  case GPDL_OVER_RP:
  case GPDL_UNDER_SP:
  case GPDL_OVER_SP:
  case GPDL_ILLPARAM:
    m_IllegalStateMessageAndQuit();
    return GPDL_IGNORED;
//  case GPDL_WAIT_INPUT:
//    if (!m_discourseText.Char(c)) return GPDL_IGNORED;
//    switch(m_discourseText.State()) {
//    case GPDL_WAIT_INPUT:
//      m_state=GPDL_WAIT_INPUT;
//      return GPDL_ACCEPTED;
//    default:
//      MessageBox(NULL,"Internal GPDL::Char wait_input error",NULL,MB_OK|MB_TOPMOST);
//      return GPDL_IGNORED;
//    };
  default:
    MsgBoxError("Internal GPDL::Char error");
    return GPDL_IGNORED;
  };
}


GPDL_STATE GPDL::BreakExecute(void) {
  m_state=GPDL_IDLE;
  m_discourseText.Clear();
  return GPDL_IDLE;
}


GPDL::DISCOURSETEXT::DISCOURSETEXT(void) {
  m_MAXCHARS=500;
  m_firstLine=NULL;
  m_numLine=0;
  m_topLine=0;
  m_listening=false;
}

GPDL::DISCOURSETEXT::~DISCOURSETEXT(void) {  
  LINE *FL;
  while (m_firstLine!=NULL) {
    FL=m_firstLine;
    m_firstLine=FL->m_nextLine;
    if (m_firstLine!=NULL) m_firstLine->m_prevLine=NULL;
    FL->m_nextLine=NULL;
    FL->m_prevLine=NULL;
    delete FL;
  };
}

void GPDL::DISCOURSETEXT::SetText(CString& text, unsigned int width) {
  m_format(text, width);
}

void GPDL::DISCOURSETEXT::StartInput(void) {
  char prompt[]="You say:   ";
  m_addLine(prompt);  // The line being input.
  m_promptLen=strlen(prompt);
  m_listening=true;
  m_topLine=m_numLine-1;
  Display();
}

void GPDL::DISCOURSETEXT::SetNewTop(void) {
  m_topLine=m_numLine;
}

void GPDL::DISCOURSETEXT::m_addLine(const char *line) {
  LINE *newline;
  LINE *LL; // LL=='L'ast  'L'ine
  newline=new LINE(line);
  m_numLine++;
  // Find last line in list.
  if (m_firstLine==NULL) 
  {
    m_firstLine=newline;
    if (discourseLine.GetSize() == 0)
      discourseLine.SetSize(TEXTBOX_LINES);
    return;
  };
  for (LL=m_firstLine; LL->m_nextLine!=NULL; LL=LL->m_nextLine) ;
  newline->m_prevLine=LL;
  LL->m_nextLine=newline;
  if (discourseLine.GetSize() == 0)
    discourseLine.SetSize(TEXTBOX_LINES);
}

CString GPDL::DISCOURSETEXT::GetInput(void) {
  LINE *LL;
  for (LL=m_firstLine; LL->m_nextLine!=NULL; LL=LL->m_nextLine);
  if (LL==NULL) return CString("");
  return LL->m_text.Right(LL->m_text.GetLength()-m_promptLen);
}

static bool whitespace(char c) {
  if (c==' ') return true;
  if (c=='\t') return true;
  if (c=='\n') return true;
  return false;
}

static bool controlChar(UINT c) {
  return c<0x20;
}

static bool systemChar(UINT c) {
  return c>0x7e;
}

class tempchar {
public:
  char *m_text;
  tempchar(unsigned int size) {m_text=(char *)malloc(size);};
  ~tempchar(void) {free(m_text);};
  char& operator[](int index) {return m_text[index];};
}; 

void GPDL::DISCOURSETEXT::m_format(CString& text, unsigned int width, bool colorTags)
{
// We want to format the 'text' so that it fits in a screen width of 'width'.
// We want to provide 'color-tag' capability ('/g' = green, etc.) in a scrolling
// environment.  So, for example, when there is a '/g/ in the first line but the
// scrolling area begins with the second line we want the second line to be green.
// Our solution is to remember the color as we reformat the text and to begin
// each line with a color-tag.
// We also need to accomocate text substitutions (^1= first character name, etc.).
// So we need to do the substitutions here in order to format the text correctly.
// All of this gets a bit complicated so we do it in two steps.
// 1) First we do any substitutions.  We make a copy of the text with the 
//    substitutions.
// 2) We go through the copy, deleting any (recursive) substitutions and measure
//    the characters and emit the result one line at a time.  We remember the
//    last color-tag in each emitted line and start the following line with that tag.
#ifdef UAFEngine
  char *textCopy = NULL, *outLine;
  const char *tagLast;
  const char *tagCur, *tagFit;
  const char *srcCur, *srcEnd, *srcFit= NULL;
  int outLineSize, charLen;
  unsigned int lineSize;
  int dstCur, dstFit;
  PreProcessText((LPCSTR)text, &textCopy);
  // There....that is step one.  char textCopy[] contains
  // the text with all substitutions completed.
  srcCur = textCopy;
  srcEnd = srcCur + strlen(srcCur);
  tagLast = "/w";
  outLineSize = 10;
  outLine = (char *) malloc(outLineSize);
  while (srcCur < srcEnd) // Fetch another line
  {
    dstCur = 0;
    dstFit = 0;
    outLine[dstCur++] = tagLast[0];
    outLine[dstCur++] = tagLast[1];
    tagFit = NULL;
    tagCur = NULL;
    lineSize = 0; //#pixels in current line
    while (srcCur < srcEnd)  // next character
    {
      if (outLineSize-dstCur < 3)
      {
        outLineSize = dstCur+100;
        outLine = (char *)realloc(outLine, outLineSize);
      };
      if (srcCur[0] == '/')
      {
        if (   (srcCur[1] == 'g')
            || (srcCur[1] == 'r')
            || (srcCur[1] == 'y')
            || (srcCur[1] == 'w')
            || (srcCur[1] == 'b')
            || (srcCur[1] == 'c'))
        {
          tagCur = srcCur;
          outLine[dstCur++] = *(srcCur++);
          outLine[dstCur++] = *(srcCur++);
          continue;
        };
      }
      else if (srcCur[0] == '^')
      {
        if (   ((srcCur[1] >= '1') && (srcCur[1] <= '8'))
            || ((srcCur[1] >= 'a') && (srcCur[1] <= 'z'))
            || (srcCur[1] == 'D')
            || (srcCur[1] == 'H')
            || (srcCur[1] == 'M') )
        {
          srcCur += 2;
          continue;
        };
      };
      if (srcCur[0] == '\n')
      {
        srcCur++;
        srcFit = srcCur;
        dstFit = dstCur;
        break;  // No more characters in this line
      };
      if (*srcCur == ' ')
      {
        srcFit = srcCur;
        dstFit = dstCur;
        tagFit = tagCur;
      };
#ifdef UAFEngine
      charLen=GraphicsMgr.GetTextWidth(srcCur,1);
#else
      charLen = 16;
#endif
      lineSize += charLen;
      if (lineSize > width)
      {
        break;
      };
      outLine[dstCur++] = *(srcCur++);
    };
    if (srcCur == srcEnd) 
    {
      srcFit = srcCur;
      dstFit = dstCur;
    };
    // Ready to output a line.
    if (dstFit > 0)
    {
      outLine[dstFit] = 0;
      srcCur = srcFit;
    }
    else
    {
      outLine[dstCur] = 0; // No breaks in the line
    };
    m_addLine(outLine);
    if (*srcCur == ' ') srcCur++; //If we broke at space then skip one space.
    if (tagFit != NULL) tagLast = tagFit;
  };
  if (outLine != NULL) free(outLine);
  if (textCopy != NULL) delete[] textCopy;
#else // if not UAFEngine
  m_addLine((LPCSTR)text);
#endif
}


/*
void GPDL::DISCOURSETEXT::m_format(CString& text, unsigned int width, bool colorTags)
{
  tempchar outline(m_MAXCHARS+2);
  unsigned int col, linelen, linePixelLen, line, end, linestart;
  unsigned int charLen, skipWhitespaceAtFrontOfLine=0;
  int wordstart;
  bool wordended;
  char c, c1;
  col=0;  // Where to start.
  linestart=col;
  line=0;               // Line we are assembling
  linelen=0;
  linePixelLen=0;
  wordstart=-1;    // No word started on this line.
  wordended=false; // No word has ended on this line.
  end=text.GetLength(); // Where we can quit
  skipWhitespaceAtFrontOfLine = 0;
  while (col < end) { 
    c = text.GetAt(col);
    if (colorTags && (c=='/'))
    {
      c1 = text.GetAt(col+1);
      if (   (c1 == 'g')
          || (c1 == 'r')
          || (c1 == 'y')
          || (c1 == 'w')
          || (c1 == 'b')
          || (c1 == 'c')
         )
      {
        outline[linelen++] = c;   //Do not include colorTags in length computation
        outline[linelen++] = c1;
        col += 2;
        continue; //while (col < end)
      };
    };
    if (c == '^')
    {
      c1 = text.GetAt(col+1);
      if (   ((c1 >= 'a') && (c1 <= 'z'))
          || ((c1 >= '1') && (c1 <= '8'))
          || (c1 == 'D')
          || (c1 == 'H')
          || (c1 == 'M')
          || (c1 == ' ')
         )
      {
        col += 2; // Delete any character or string substitutions
        continue; //while (col < end)
      };
    };
    if (c=='\n') 
    { // Force end-of-line
      // The whole line is a single word!
      // Nothing to do but break it up.
      skipWhitespaceAtFrontOfLine = 0;
      outline[linelen]=0;
      m_addLine(outline.m_text);
      wordstart=-1;
      wordended=false;
      linelen=0;
      linePixelLen=0;
      col++;
      continue;
    };
    if (whitespace(text.GetAt(col)))  
    {
      if (wordstart>=0) wordended=true;
      wordstart=-1;
      if ((linelen == 0) && (skipWhitespaceAtFrontOfLine > 0)) 
      {
        col++;
        skipWhitespaceAtFrontOfLine--;
        continue;  // Skip whitespace at front of line.
      };
#ifdef UAFEngine
      charLen=GraphicsMgr.GetTextWidth(" ",1);
#else
      charLen=16;
#endif
      if (linePixelLen+charLen<width && linelen<m_MAXCHARS) 
      {
        outline[linelen]=' ';
        linelen++;  // Whitespace in line.
        linePixelLen+=charLen;
        col++;
        continue;
      };
      wordstart=-1;
      wordended=false;
      outline[linelen]=0;
      m_addLine(outline.m_text);
      skipWhitespaceAtFrontOfLine = 1;
      line++;
      linelen=0;
      linePixelLen=0;
      continue;  // Whitespace at end of line.
    };
    // We have taken care of whitespace.  Now deal with non-blanks
    if (wordstart==-1) wordstart=col;
    outline[linelen]=text.GetAt(col); // Assume it will fit.
#ifdef UAFEngine
    charLen=GraphicsMgr.GetTextWidth(&outline[linelen],1);
#else
    charLen = 16;
#endif
    if (linePixelLen+charLen>=width && linelen<m_MAXCHARS) 
    {
      // Won't fit on this line.
      if (wordended==false) {
        // The whole line is a single word!
        // Nothing to do but break it up.
        outline[linelen]=0;
        m_addLine(outline.m_text);
        skipWhitespaceAtFrontOfLine = 1;
        wordstart=-1;
        wordended=false;
        linelen=0;
        linePixelLen=0;
        continue;
      };
      // So we can fall back to where the last word
      // started.
      linelen-=col-wordstart;
      col=wordstart;
      outline[linelen]=0;
      m_addLine(outline.m_text);
      skipWhitespaceAtFrontOfLine = 1;
      wordstart=-1;
      wordended=false;
      linelen=0;
      linePixelLen=0;
      continue;
    };
    if (linelen==0) linestart=col;
    outline[linelen++]=text.GetAt(col++);
    linePixelLen+=charLen;
  };
  if (linelen>0) 
  {
    outline[linelen]=0;
    m_addLine(outline.m_text);
  };
}
*/
void GPDL::DISCOURSETEXT::Display(void) {
  LINE *L;
  unsigned int i;
  L=m_firstLine;
//  if (m_numLine>m_MAXLINES) {
//    if (m_topLine>m_numLine-m_MAXLINES) {
//      m_topLine=m_numLine-m_MAXLINES;
//    };
//  };
  // First, we find the top line
  for (i=0; i < m_topLine; i++)  {
    if (L==NULL) break;
    L=L->m_nextLine;
  };
  for (i=0; i < (unsigned)discourseLine.GetSize(); i++) {
    if (L==NULL) discourseLine[i]=CString("");
    else {
      discourseLine[i]=L->m_text;
      L=L->m_nextLine;
    };
  };
}

void GPDL::DISCOURSETEXT::Clear(void) {
  int i;
  LINE *FL;
  if (discourseLine.GetSize() == 0)
    discourseLine.SetSize(TEXTBOX_LINES);
  for (i=0; i<discourseLine.GetSize(); i++) discourseLine[i]="";
  while (m_firstLine!=NULL) {
    FL=m_firstLine;
    m_firstLine=FL->m_nextLine;
    FL->m_nextLine=NULL;
    FL->m_prevLine=NULL;
    delete FL;
  };
  m_topLine=0;
  m_numLine=0;
}


void GPDL::DISCOURSETEXT::downArrow(void) {
  if (m_topLine+discourseLine.GetSize()>=m_numLine) return;
  m_topLine++;
  Display();
};

void GPDL::DISCOURSETEXT::upArrow(void) {
  if (m_topLine==0) return;
  m_topLine--;
  Display();
};

GPDL_STATE GPDL::DISCOURSETEXT::Char(UINT  c  ) {
  // Return GPDL_ACCEPTED if we gobble up the character
  LINE *LL;
  if (!m_listening) return GPDL_IGNORED;
  // Find the last line.
  for (LL=m_firstLine; LL->m_nextLine!=NULL; LL=LL->m_nextLine) ;
  if (LL==NULL) MsgBoxError("DISCOURSETEXT::Char internal error");
  if (c==BACKSPACE) LL->deleteLastChar(m_promptLen);
  else if (controlChar(c)) return GPDL_IGNORED;
  else if (systemChar(c)) return GPDL_IGNORED;
  else {
    LL->addOneChar(c);
  };
  Display();
  return GPDL_ACCEPTED;
}


GPDL::DISCOURSETEXT::LINE::LINE(const char *line) {
  m_text=CString(line);
  m_nextLine=NULL;
  m_prevLine=NULL;
}

GPDL::DISCOURSETEXT::LINE::~LINE(void) {
  // All work should have been done by ~DISCOURSETEXT
  if ((m_prevLine!=NULL) || (m_nextLine!=NULL)) {
    MsgBoxError("DISCOURSETEXT::LINE::~LINE internal error");
  };
}

void GPDL::DISCOURSETEXT::LINE::deleteLastChar(unsigned int backspaceLimit) {
  if (m_text.GetLength()>(int)backspaceLimit) {
    m_text=m_text.Left(m_text.GetLength()-1);
  };
}

void GPDL::DISCOURSETEXT::LINE::addOneChar(UINT c) {
  m_text+=(char)c;
}

GPDL::CONSTANTS::CONSTANTS (void) {
  m_value=NULL;
  m_allocatedSize=0;
}

GPDL::CONSTANTS::~CONSTANTS(void) {
  if (m_value!=NULL) delete []m_value;
  m_value=NULL;
  m_allocatedSize=0;
}

int GPDL::CONSTANTS::read(CArchive& ar) {
  unsigned int i;
  if (m_value!=NULL) delete []m_value;
  m_value=NULL;
  ar >> m_allocatedSize;
  m_value=new CString[m_allocatedSize];
  for (i=0; i<m_allocatedSize; i++) ar >> m_value[i];
  return 0;
}

#ifdef UAFEDITOR
int GPDL::CONSTANTS::read(GPDLCOMP *pGPDL) {
  unsigned int i;
  if (m_value!=NULL) delete []m_value;
  m_value=NULL;
  m_allocatedSize = pGPDL->constantsSize(); 
  m_value=new CString[m_allocatedSize];
  for (i=0; i<m_allocatedSize; i++)
  {
    m_value[i] = pGPDL->constantValue(i);
  };
  return 0;
}
#endif

GPDL::INDEX::INDEX(void) {
  m_name=NULL;
  m_value=NULL;
  m_allocatedSize=0;
}

GPDL::INDEX::~INDEX(void) {
  if (m_name!=NULL) delete []m_name;
  if (m_value!=NULL) delete []m_value;
  m_name=NULL;
  m_value=NULL;
  m_allocatedSize=0;
}

unsigned int GPDL::INDEX::lookup(const CString& funcName) {
  unsigned int i;
  for (i=0; i<m_allocatedSize; i++) {
    if (m_name[i]==funcName) return m_value[i];
  };
  return 0;
}

int GPDL::INDEX::read(CArchive& ar) {
  unsigned int i;
  if (m_name!=NULL) delete []m_name;
  if (m_value!=NULL) delete []m_value;
  m_name=NULL;
  m_value=NULL;
  ar >> m_allocatedSize;
  m_name=new CString [m_allocatedSize];
  m_value=new unsigned int [m_allocatedSize];
  for (i=0; i<m_allocatedSize; i++) 
  {
    ar >> m_name[i]; m_name[i].TrimLeft(); m_name[i].TrimRight();
    ar >> m_value[i];
  };
  return 0;
}

#ifdef UAFEDITOR
int GPDL::INDEX::read(GPDLCOMP *pGPDL) {
  unsigned int i;
  if (m_name!=NULL) delete []m_name;
  if (m_value!=NULL) delete []m_value;
  m_name=NULL;
  m_value=NULL;
  m_allocatedSize = pGPDL->indexSize();
  m_name=new CString [m_allocatedSize];
  m_value=new unsigned int [m_allocatedSize];
  for (i=0; i<m_allocatedSize; i++) 
  {
    m_name[i] = pGPDL->indexName(i); 
    m_name[i].TrimLeft(); m_name[i].TrimRight();
    m_value[i] = pGPDL->indexValue(i);
  };
  return 0;
}
#endif

GPDL::ERRORLOG::ERRORLOG(void) {
  m_file=NULL;
  m_messagePrinted=false;
}

GPDL::ERRORLOG::~ERRORLOG(void) {
  if (m_file!=NULL) fclose(m_file);
}

void GPDL::ERRORLOG::log(const char *msg) 
{
  WriteDebugString("GPDL LOG: %s\n", msg);
  if (m_file==NULL) 
  {
    m_file=fopen("interp.log","w");

    if (m_file==NULL) 
    {
#ifdef UAFEDITOR
      if (m_messagePrinted==false) 
      {
        MsgBoxError("Cannot open interp.log");
        m_messagePrinted=true;
      };
#endif
      return;
    };
  };
  fprintf(m_file,"%s\n",msg);
}

void GPDL::ERRORLOG::log(CString& msg) {
  log((LPCTSTR)msg);
}

ActorType GPDL::m_StringToActor(const char *data)
{
  if (data==NULL) return NULL_ACTOR;
  if (strlen(data)==0) return NULL_ACTOR;
  ActorType actor = *((ActorType*)data);
  if (!actor.IsValidSrc()) 
  { 
    ASSERT(FALSE); 
    CString errmsg;
    errmsg.Format("Invalid source string \'%s\' in m_StringToActor()\n", data);
    errmsg += GetActorDebugText(actor);
    WriteDebugString(errmsg);
    WriteStackTrace();
    return NULL_ACTOR; 
  }
  return actor;
}

CString GPDL::m_ActorToString(const ActorType *data)
{
  if (data==NULL) return m_false;
  m_string1 = CString((LPCTSTR)(data), sizeof (ActorType));
  return m_string1;
}

CString GPDL::m_Myself(void)
{
  ActorType actor = GetCharContext();
  ASSERT(actor!=NULL_ACTOR);
  m_string1 = m_ActorToString(&actor);
  return m_string1;  
}

CString GPDL::m_Target(void)
{
  ActorType actor = GetTargetContext();
  ASSERT(actor!=NULL_ACTOR);
  m_string1 = m_ActorToString(&actor);
  return m_string1;  
}

CString GPDL::m_Name(const char *name)
{
  ActorType actor;
  if (IsCombatActive())
  {
#ifdef UAFEngine
    for (int i=0;i<combatData.numCombatants;i++)
    {
      if (stricmp(name, combatData.combatants[i].GetName())==0)
      {
        actor = combatData.GetContext(i);
        m_string1 = m_ActorToString(&actor);
        return m_string1;
      }
    }
#else
    m_string1 = m_ActorToString(&NULL_ACTOR);
    return m_string1;
#endif
  }
  else
  {
    for (int i=0;i<GetPartySize(party);i++)
    {
      if (stricmp(name, party.characters[i].GetName())==0)
      {
        actor = party.characters[i].GetContext();
        m_string1 = m_ActorToString(&actor);
        return m_string1;
      }
    }
  }
  return m_false;
}

CString GPDL::m_Gender(CString &data)
{  
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
#ifdef UAFEngine
  return (CharGenderTypeText[dude->GetAdjGender()]);
#else
  return "Male";
#endif
}

CString GPDL::m_Class(CString &data)
{  
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return (classData[dude->GetAdjClass()].m_name);
}

CString GPDL::m_Race(CString &data)
{  
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return (raceData.GetRaceName(dude->race()));
}

CString GPDL::m_Status(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
#ifdef UAFEngine
  return (CharStatusTypeText[dude->GetAdjStatus()]);
#else
  return "Okay";
#endif
}

CString GPDL::m_Alignment(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
#ifdef UAFEngine
  return (CharAlignmentTypeText[dude->GetAdjAlignment()]);
#else
  return "Chaotic Neutral";
#endif
}
CString GPDL::m_AlignmentGood(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  if (dude->IsAlignmentGood()) return m_true;
  return m_false;
}
CString GPDL::m_AlignmentEvil(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  if (dude->IsAlignmentEvil()) return m_true;
  return m_false;
}
CString GPDL::m_AlignmentLawful(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  switch (dude->GetAdjAlignment())
  {
  case LawfulEvil:
  case LawfulNeutral:
  case LawfulGood:
    return m_true;
  }
  return m_false;
}
CString GPDL::m_AlignmentNeutral(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  switch (dude->GetAdjAlignment())
  {
  case TrueNeutral:
  case LawfulNeutral:
  case ChaoticNeutral:
  case NeutralGood:
  case NeutralEvil:
    return m_true;
  }
  return m_false;
}
CString GPDL::m_AlignmentChaotic(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  switch (dude->GetAdjAlignment())
  {
  case ChaoticEvil:
  case ChaoticNeutral:
  case ChaoticGood:
    return m_true;
  }
  return m_false;
}
CString GPDL::m_HitPoints(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return CString("0");
  CString tmp;
  tmp.Format("%i", dude->GetAdjHitPoints());
  return tmp;
}
CString GPDL::m_InParty(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return dude->IsPartyMember()?m_true:m_false;
}
CString GPDL::m_SaveVsSpell(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return dude->didCharacterSaveVs(Sp,0)?m_true:m_false;
}
CString GPDL::m_SaveVsBreathWeapon(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return dude->didCharacterSaveVs(BreathWeapon,0)?m_true:m_false;
}
CString GPDL::m_SaveVsRodStaffWand(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return dude->didCharacterSaveVs(RodStaffWand,0)?m_true:m_false;
}
CString GPDL::m_SaveVsPetPoly(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return dude->didCharacterSaveVs(PetPoly,0)?m_true:m_false;
}
CString GPDL::m_SaveVsParPoiDM(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return dude->didCharacterSaveVs(ParPoiDM,0)?m_true:m_false;
}
CString GPDL::m_IsUndead(CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return (dude->GetAdjUndeadType()!=NotUndead)?m_true:m_false;
}

CString GPDL::m_IndexOf(const CString &data)
{
  ActorType actor = m_StringToActor((LPCSTR)data);
  CString index("");
  // Instance is marchOrder for party, not 0..numCharacters!
  // Instance is 0..numCombatants during combat
  index.Format("%i", actor.Instance); 
  return index;
}

CString GPDL::m_IsCharAffectedBySpell(const CString &ActorString, const CString &SpellName)
{
#ifdef UAFEngine
  ActorType actor = m_StringToActor((LPCSTR)ActorString);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return (dude->IsAffectedBySpell(SpellName)?m_true:m_false);
#else
  return m_false;
#endif
}
CString GPDL::m_IsCharAffectedBySpellAttribute(const CString &ActorString, const CString &SpellAttribName)
{
#ifdef UAFEngine
  ActorType actor = m_StringToActor((LPCSTR)ActorString);
  CHARACTER *dude = GetCurrentlyActiveContext(&actor);
  if (dude==NULL) return m_false;
  return (dude->IsAffectedBySpellAttribute(SpellAttribName)?m_true:m_false);
#else
  return m_false;
#endif
}
int CleanNumber(const CString& num, char *digits)             //ARITHMETIC
{                                                             //ARITHMETIC
  // Get rid of leading zeroes, non-digits.                   //ARITHMETIC
  // Put a zero preceeded by a single sign character at front.//ARITHMETIC
  // Returns length of result.                                //ARITHMETIC
  // **Warning** you better provide a digits array at least   //ARITHMETIC
  // as long as the input string plus 2;                      //ARITHMETIC
  char c;                                                     //ARITHMETIC
  int col=2;                                                  //ARITHMETIC
  int len=num.GetLength();                                    //ARITHMETIC
  digits[0]='+';                                              //ARITHMETIC
  digits[1]=0;                                                //ARITHMETIC
  for (int i=0; i<len; i++)                                   //ARITHMETIC
  {                                                           //ARITHMETIC
    c=num[i];                                                 //ARITHMETIC
    if ((c>='0')&&(c<='9'))                                   //ARITHMETIC
      if ((c>='1') || (col!=2)) digits[col++]=c;              //ARITHMETIC
    if ((c=='-') && (col==2)) digits[0]='-';                  //ARITHMETIC
  };                                                          //ARITHMETIC
  return col;                                                 //ARITHMETIC
}                                                             //ARITHMETIC
                                                              //ARITHMETIC
static int LongPlus(const char *x,const char *y, char **rd)   //ARITHMETIC
{                                                             //ARITHMETIC
  // Add two positive numbers.                                //ARITHMETIC
  // return length of result.  Set *rd = sign of result;      //ARITHMETIC
  int len;                                                    //ARITHMETIC
  char *r=*rd;  // End+1 of result                            //ARITHMETIC
  char carry=0;                                               //ARITHMETIC
  while (*(x-1)!=0)                                           //ARITHMETIC
  {                                                           //ARITHMETIC
    carry += *(--x)-'0';                                      //ARITHMETIC
    if (*(y-1)!=0) carry += *(--y) - '0';                     //ARITHMETIC
    *(--r) = (carry%10) + '0';                                //ARITHMETIC
    carry /= 10;                                              //ARITHMETIC
  };                                                          //ARITHMETIC
  while (*(y-1)!=0)                                           //ARITHMETIC
  {                                                           //ARITHMETIC
    carry += *(--y) - '0';                                    //ARITHMETIC
    *(--r) = (carry%10) + '0';                                //ARITHMETIC
    carry /= 10;                                              //ARITHMETIC
  };                                                          //ARITHMETIC
  if (carry != 0) *(--r) = carry + '0';                       //ARITHMETIC
  if (r==*rd) *(--r)='0';                                     //ARITHMETIC
  *(--r)='+';                                                 //ARITHMETIC
  len=*rd-r;                                                  //ARITHMETIC
  *rd=r;                                                      //ARITHMETIC
  return len;                                                 //ARITHMETIC
}                                                             //ARITHMETIC
                                                              //ARITHMETIC
static int LongMinus(const char *x,const char *y, char **rd)  //ARITHMETIC
{                                                             //ARITHMETIC
  // Subtract two positive numbers.  *rd = x - y              //ARITHMETIC
  // return length of result.  Set *rd = sign of result;      //ARITHMETIC
  int len;                                                    //ARITHMETIC
  char *r=*rd;  // End+1 of result                            //ARITHMETIC
  char borrow=0;                                              //ARITHMETIC
  while (*(x-1)!=0)                                           //ARITHMETIC
  {                                                           //ARITHMETIC
    borrow += *(--x)-'0'+10;                                  //ARITHMETIC
    if (*(y-1)!=0) borrow -= *(--y) - '0';                    //ARITHMETIC
    *(--r) = (borrow%10) + '0';                               //ARITHMETIC
    borrow = borrow/10-1;                                     //ARITHMETIC
  };                                                          //ARITHMETIC
  while (*(y-1)!=0)                                           //ARITHMETIC
  {                                                           //ARITHMETIC
    borrow -= *(--y)-'0'-10;                                  //ARITHMETIC
    *(--r) = (borrow%10) + '0';                               //ARITHMETIC
    borrow = borrow/10-1;                                     //ARITHMETIC
  };                                                          //ARITHMETIC
  if (borrow<0)                                               //ARITHMETIC
  {  // We have a negative result.                            //ARITHMETIC
    char *d=*rd;                                              //ARITHMETIC
    borrow=0;                                                 //ARITHMETIC
    while (d!=r)                                              //ARITHMETIC
    {                                                         //ARITHMETIC
      borrow -= *(--d)-'0'-10;                                //ARITHMETIC
      *d=borrow%10+'0';                                       //ARITHMETIC
      borrow = borrow/10-1;                                   //ARITHMETIC
    };                                                        //ARITHMETIC
  };                                                          //ARITHMETIC
  // Now get rid of leading zeroes.                           //ARITHMETIC
  while (*r == '0') r++;                                      //ARITHMETIC
  if (r==*rd) *(--r)='0';  // Need one digit minimum          //ARITHMETIC
  *(--r) = borrow?'-':'+';                                    //ARITHMETIC
  len=*rd-r;                                                  //ARITHMETIC
  *rd=r;                                                      //ARITHMETIC
  return len;                                                 //ARITHMETIC
}                                                             //ARITHMETIC
                                                              //ARITHMETIC
CString LongAdd(const CString& x, const CString& y)           //ARITHMETIC
{                                                             //ARITHMETIC
  int xl, yl, rl;                                             //ARITHMETIC
  char *xd, *yd, *rd;                                         //ARITHMETIC
  xl=x.GetLength()+3;                                         //ARITHMETIC
  yl=y.GetLength()+3;                                         //ARITHMETIC
  rl=xl+yl+3;                                                 //ARITHMETIC
  xd=(char *)_alloca(xl+yl+rl+20);                            //ARITHMETIC
  yd=xd+xl;                                                   //ARITHMETIC
  rd=yd+yl+rl;                                                //ARITHMETIC
  xl=CleanNumber(x, xd);                                      //ARITHMETIC
  yl=CleanNumber(y, yd);                                      //ARITHMETIC
  if (*xd=='-')                                               //ARITHMETIC
  {                                                           //ARITHMETIC
    if (*yd=='-')                                             //ARITHMETIC
    {                                                         //ARITHMETIC
      rl=LongPlus (xd+xl, yd+yl, &rd);                        //ARITHMETIC
      *rd='-';                                                //ARITHMETIC
    }                                                         //ARITHMETIC
    else                                                      //ARITHMETIC
    {                                                         //ARITHMETIC
      rl=LongMinus (yd+yl, xd+xl, &rd);                       //ARITHMETIC
    };                                                        //ARITHMETIC
  }                                                           //ARITHMETIC
  else                                                        //ARITHMETIC
  {                                                           //ARITHMETIC
    if (*yd=='-')                                             //ARITHMETIC
    {                                                         //ARITHMETIC
      rl=LongMinus(xd+xl, yd+yl, &rd);                        //ARITHMETIC
    }                                                         //ARITHMETIC
    else                                                      //ARITHMETIC
    {                                                         //ARITHMETIC
      rl=LongPlus(xd+xl, yd+yl, &rd);                         //ARITHMETIC
    };                                                        //ARITHMETIC
  };                                                          //ARITHMETIC
  if (*rd=='+') return CString(rd+1,rl-1);                    //ARITHMETIC
  return CString(rd,rl);                                      //ARITHMETIC
}                                                             //ARITHMETIC
                                                              //ARITHMETIC
CString LongSubtract(const CString& x, const CString& y)      //ARITHMETIC
{                                                             //ARITHMETIC
  int xl, yl, rl;                                             //ARITHMETIC
  char *xd, *yd, *rd;                                         //ARITHMETIC
  xl=x.GetLength()+3;                                         //ARITHMETIC
  yl=y.GetLength()+3;                                         //ARITHMETIC
  rl=xl+yl+3;                                                 //ARITHMETIC
  xd=(char *)_alloca(xl+yl+rl+20);                            //ARITHMETIC
  yd=xd+xl;                                                   //ARITHMETIC
  rd=yd+yl+rl;                                                //ARITHMETIC
  xl=CleanNumber(x, xd);                                      //ARITHMETIC
  yl=CleanNumber(y, yd);                                      //ARITHMETIC
  if (*xd=='-')                                               //ARITHMETIC
  {                                                           //ARITHMETIC
    if (*yd=='-')                                             //ARITHMETIC
    {                                                         //ARITHMETIC
      rl=LongMinus (yd+yl, xd+xl, &rd);                       //ARITHMETIC
    }                                                         //ARITHMETIC
    else                                                      //ARITHMETIC
    {                                                         //ARITHMETIC
      rl=LongPlus (yd+yl, xd+xl, &rd);                        //ARITHMETIC
      *rd='-';                                                //ARITHMETIC
    };                                                        //ARITHMETIC
  }                                                           //ARITHMETIC
  else                                                        //ARITHMETIC
  {                                                           //ARITHMETIC
    if (*yd=='-')                                             //ARITHMETIC
    {                                                         //ARITHMETIC
      rl=LongPlus(xd+xl, yd+yl, &rd);                         //ARITHMETIC
    }                                                         //ARITHMETIC
    else                                                      //ARITHMETIC
    {                                                         //ARITHMETIC
      rl=LongMinus(xd+xl, yd+yl, &rd);                        //ARITHMETIC
    };                                                        //ARITHMETIC
  };                                                          //ARITHMETIC
  if (*rd=='+') return CString(rd+1,rl-1);                    //ARITHMETIC
  return CString(rd,rl);                                      //ARITHMETIC
}                                                             //ARITHMETIC
                                                              //ARITHMETIC
int LongCompare(const CString& x, const CString& y)           //ARITHMETIC
{                                                             //ARITHMETIC
  CString result;                                             //ARITHMETIC
  result = LongSubtract(x, y);                                //ARITHMETIC
  if (result[0] == '-') return -1;                            //ARITHMETIC
  if (result[0]=='0') return 0;                               //ARITHMETIC
  return 1;                                                   //ARITHMETIC
}                                                             //ARITHMETIC
                                                              //ARITHMETIC
CString LongMultiply(const CString& s1, const CString& s2)    //ARITHMETIC
{                                                             //ARITHMETIC
/*  Test cases in the event that we need to fix something.    //ARITHMETIC
    We should check it out again.                             //ARITHMETIC
CString t1, t2;                                               //ARITHMETIC
                                                              //ARITHMETIC
LongDivide(CString("0"), CString("5"), t1, t2);               //ARITHMETIC
ASSERT(t1=="0");                                              //ARITHMETIC
ASSERT(t2=="0");                                              //ARITHMETIC
LongDivide(CString("5"), CString("0"), t1, t2);               //ARITHMETIC
ASSERT(t1=="999999");                                         //ARITHMETIC
ASSERT(t2=="0");                                              //ARITHMETIC
LongDivide(CString("9") , CString("5"), t1, t2);              //ARITHMETIC
ASSERT(t1=="1");                                              //ARITHMETIC
ASSERT(t2=="4");                                              //ARITHMETIC
LongDivide(CString("47"), CString("9"), t1, t2);              //ARITHMETIC
ASSERT(t1=="5");                                              //ARITHMETIC
ASSERT(t2=="2");                                              //ARITHMETIC
LongDivide(CString("123456"), CString("00987654321"), t1, t2);//ARITHMETIC
ASSERT(t1=="0");                                              //ARITHMETIC
ASSERT(t2=="123456");                                         //ARITHMETIC
LongDivide(CString("123456"), CString("00103"), t1, t2);      //ARITHMETIC
ASSERT(t1=="1198");                                           //ARITHMETIC
ASSERT(t2=="62");                                             //ARITHMETIC
LongDivide(CString("123456"), CString("000183"), t1, t2);     //ARITHMETIC
ASSERT(t1=="674");                                            //ARITHMETIC
ASSERT(t2="114");                                             //ARITHMETIC
LongDivide(CString("99"), CString("11"), t1, t2);             //ARITHMETIC
ASSERT(t1=="9");                                              //ARITHMETIC
ASSERT(t2=="0");                                              //ARITHMETIC
LongDivide(CString("613"), CString("614"), t1, t2);           //ARITHMETIC
ASSERT(t1=="0");                                              //ARITHMETIC
ASSERT(t2=="613");                                            //ARITHMETIC
LongDivide(CString("-5"),CString("3"), t1, t2);               //ARITHMETIC
ASSERT(t1=="-1");                                             //ARITHMETIC
ASSERT(t2=="-2");                                             //ARITHMETIC
LongDivide(CString("5"),CString("-3"), t1, t2);               //ARITHMETIC
ASSERT(t1=="-1");                                             //ARITHMETIC
ASSERT(t2=="2");                                              //ARITHMETIC
LongDivide(CString("5"),CString("-6"), t1, t2);               //ARITHMETIC
ASSERT(t1=="0");                                              //ARITHMETIC
ASSERT(t2=="5");                                              //ARITHMETIC
LongDivide(CString("-44"), CString("11"), t1, t2);            //ARITHMETIC
ASSERT(t1=="-4");                                             //ARITHMETIC
ASSERT(t2=="0");                                              //ARITHMETIC
LongDivide(CString("-678"), CString("-22"), t1, t2);          //ARITHMETIC
ASSERT(t1=="30");                                             //ARITHMETIC
ASSERT(t2=="-18");                                            //ARITHMETIC
*/                                                            //ARITHMETIC
                                                              //ARITHMETIC
  int xl, yl, rl;                                             //ARITHMETIC
  int i, j, c, p;                                             //ARITHMETIC
  char *x, *y, *r, *s, *t;                                    //ARITHMETIC
  char *xd, *yd, *rd;                                         //ARITHMETIC
  xl=s1.GetLength()+4; // 3 should suffice                    //ARITHMETIC
  yl=s2.GetLength()+4; // 3 should suffice                    //ARITHMETIC
  rl=xl+yl+3; //0 should suffice                              //ARITHMETIC
  xd=(char *)_alloca(xl+yl+rl+20);                            //ARITHMETIC
  yd=xd+xl;                                                   //ARITHMETIC
  rd=yd+yl;                                                   //ARITHMETIC
  xl=CleanNumber(s1, xd);                                     //ARITHMETIC
  yl=CleanNumber(s2, yd);                                     //ARITHMETIC
  rl=xl+yl-4;                                                 //ARITHMETIC
  memset(rd+1,'0',rl);                                        //ARITHMETIC
  r=rd+rl+1;                                                  //ARITHMETIC
  *r=0;                                                       //ARITHMETIC
  x=xd+xl;                                                    //ARITHMETIC
  for (i=2; i<xl; i++)                                        //ARITHMETIC
  {                                                           //ARITHMETIC
    x--;                                                      //ARITHMETIC
    y=yd+yl;                                                  //ARITHMETIC
    s=r--;                                                    //ARITHMETIC
    for (j=2; j<yl; j++)                                      //ARITHMETIC
    {                                                         //ARITHMETIC
      t=--s;                                                  //ARITHMETIC
      p=((*x)-'0') * ((*(--y))-'0');                          //ARITHMETIC
      c=p/10;                                                 //ARITHMETIC
      *t += p%10;                                             //ARITHMETIC
      if (*t > '9')                                           //ARITHMETIC
      {                                                       //ARITHMETIC
        *t -= 10;                                             //ARITHMETIC
        c++;                                                  //ARITHMETIC
      };                                                      //ARITHMETIC
      while (c)                                               //ARITHMETIC
      {                                                       //ARITHMETIC
        *(--t) += c;                                          //ARITHMETIC
        c = 0;                                                //ARITHMETIC
        if (*t > '9')                                         //ARITHMETIC
        {                                                     //ARITHMETIC
          *t -= 10;                                           //ARITHMETIC
          c++;                                                //ARITHMETIC
        };                                                    //ARITHMETIC
      };                                                      //ARITHMETIC
    };                                                        //ARITHMETIC
  };                                                          //ARITHMETIC
  if (rd[1] == '0')                                           //ARITHMETIC
  {                                                           //ARITHMETIC
    rd++; //skip leading zero.                                //ARITHMETIC
  };                                                          //ARITHMETIC
  if (rd[1] != 0)                                             //ARITHMETIC
  {                                                           //ARITHMETIC
    *rd='-';                                                  //ARITHMETIC
    if (!(((*xd)=='-') ^ ((*yd)=='-'))) rd++;                 //ARITHMETIC
  };                                                          //ARITHMETIC
  return CString(rd);                                         //ARITHMETIC
}                                                             //ARITHMETIC
                                                              //ARITHMETIC
void LongDivide(const CString& dividend,                      //ARITHMETIC
                const CString& divisor,                       //ARITHMETIC
                CString& quotient,                            //ARITHMETIC
                CString& remainder)                           //ARITHMETIC
{                                                             //ARITHMETIC
  int b, xl = dividend.GetLength(), yl = divisor.GetLength(); //ARITHMETIC
  char *xd = (char *)_alloca(xl+yl+20);                       //ARITHMETIC
  char *yd = xd+xl+10, *rc, *yb;                              //ARITHMETIC
  xl = CleanNumber(dividend, xd+2);                           //ARITHMETIC
  xd[xl+2]=0;                                                 //ARITHMETIC
  yl = CleanNumber(divisor, yd);                              //ARITHMETIC
  if (yl == 2) {quotient="999999"; remainder="0"; return;};   //ARITHMETIC
  xd[0] = xd[2]; xd[2] = 0; xd[1] = xd[3]; xd[3] = '0';       //ARITHMETIC
  if (yl > xl) {quotient="0"; remainder=dividend; return;};   //ARITHMETIC
  char *rb=xd+yl+1, *q=xd;                                    //ARITHMETIC
  while (*(rb++)!=0)                                          //ARITHMETIC
  {                                                           //ARITHMETIC
    *(++q)='0'-1;                                             //ARITHMETIC
    do                                                        //ARITHMETIC
    {                                                         //ARITHMETIC
      yb=yd+yl; b=0; rc=rb;                                   //ARITHMETIC
      while (*(--yb)!=0)                                      //ARITHMETIC
      {                                                       //ARITHMETIC
        *(--rc)-=*yb-'0'+b;                                   //ARITHMETIC
        if (*rc<'0') {*rc+=10; b=1;} else b=0;                //ARITHMETIC
      };                                                      //ARITHMETIC
      *(--rc)-=b; (*q)++;                                     //ARITHMETIC
    } while (*rc>='0');                                       //ARITHMETIC
    yb=yd+yl; b=0; rc=rb;                                     //ARITHMETIC
    while (*(--yb)!=0)                                        //ARITHMETIC
    {                                                         //ARITHMETIC
      *(--rc)+=*yb-'0'+b;                                     //ARITHMETIC
      if (*rc>'9') {*rc-=10; b=1;} else b=0;                  //ARITHMETIC
    };                                                        //ARITHMETIC
    *(--rc)+=b;                                               //ARITHMETIC
  };                                                          //ARITHMETIC
  for (b=3+xl-yl; b<=xl; b++) if (xd[b] != '0') break;        //ARITHMETIC
  xd[b-1] = xd[0];                                            //ARITHMETIC
  if ((xd[b-1]=='-')&&(xd[b]!='0'))b--;                       //ARITHMETIC
  remainder=xd+b;                                             //ARITHMETIC
  q[1]=0;                                                     //ARITHMETIC
  q=xd+(((xl!=yl)&&(xd[1]=='0'))?1:0);                        //ARITHMETIC
  *q = (*xd=='-')^(*yd=='-')?'-':'+';                         //ARITHMETIC
  if ((q[0]=='+')||(q[1]=='0'))q++;                           //ARITHMETIC
  quotient = q;                                               //ARITHMETIC
}                                                             //ARITHMETIC



RDREXEC::RDREXEC(void)                                                  //RDR
{                                                                       //RDR
  m_code = "";                                                          //RDR
}                                                                       //RDR
                                                                        //RDR
RDREXEC::~RDREXEC(void)                                                 //RDR
{                                                                       //RDR
}                                                                       //RDR
                                                                        //RDR
int RDREXEC::m_getInteger(const char **str)                             //RDR
{                                                                       //RDR
  COPS op;                                                              //RDR
  int result;                                                           //RDR
  op = (COPS)(**str);                                                   //RDR
  *str += 1;                                                            //RDR
  if (op == COP_INTEGER1)                                               //RDR
  {                                                                     //RDR
    result = **str;                                                     //RDR
    *str += 1;                                                          //RDR
  }                                                                     //RDR
  else if (op == COP_INTEGER2)                                          //RDR
  {                                                                     //RDR
    result = *((*str)+1);  // fill with proper sign                     //RDR
    result = ((*(int *)(*str)) & 0xffff) | (result & 0xffff0000);       //RDR
    *str += 2;                                                          //RDR
  }                                                                     //RDR
  else if (op == COP_INTEGER3)                                          //RDR
  {                                                                     //RDR
    result = *((*str)+2);  // fill with proper sign                     //RDR
    result = ((*(int *)(*str)) & 0xffffff) | (result & 0xff000000);     //RDR
    *str += 3;                                                          //RDR
  }                                                                     //RDR
  else                                                                  //RDR
  {                                                                     //RDR
    ASSERT(op == COP_INTEGER4);                                         //RDR
    result = *((int *)(*str));                                          //RDR
    *str += 4;                                                          //RDR
  };                                                                    //RDR
  return result;                                                        //RDR
}                                                                       //RDR
                                                                        //RDR
#define C1 ASSERT(SP >= 1);                                             //RDR
#define C2 ASSERT(SP >= 2); SP--;                                       //RDR
#define IS ASSERT(SP < 38); SP++;                                       //RDR
#define OP2(op) C2 stk[SP] = stk[SP] op stk[SP+1]; break;               //RDR
                                                                        //RDR
int RDREXEC::InterpretExpression(const CString& expression,             //RDR
                              int (*interpretRDR)(int *array))          //RDR
{                                                                       //RDR
  COPS op;                                                              //RDR
  int i, SP = 0, array[20], stk[40];                                    //RDR
  const char *expr = expression;                                        //RDR
  const char *end = expr + expression.GetLength();                      //RDR
  stk[SP] = -1;                                                         //RDR
  while (expr < end)                                                    //RDR
  {                                                                     //RDR
    op = (COPS)*(expr++);                                               //RDR
    switch (op)                                                         //RDR
    {                                                                   //RDR
    case COP_ILLEGAL: ASSERT(false); return -1;                         //RDR
    case COP_LOR:          OP2(||)                                      //RDR
    case COP_LAND:         OP2(&&)                                      //RDR
    case COP_ISEQUAL:      OP2(==)                                      //RDR
    case COP_NOTEQUAL:     OP2(!=)                                      //RDR
    case COP_LESS:         OP2(<)                                       //RDR
    case COP_LESSEQUAL:    OP2(<=)                                      //RDR
    case COP_GREATER:      OP2(>)                                       //RDR
    case COP_GREATEREQUAL: OP2(>=)                                      //RDR
    case COP_PLUS:         OP2(+)                                       //RDR
    case COP_TIMES:        OP2(*)                                       //RDR
    case COP_SLASH:                                                     //RDR
      C2                                                                //RDR
      if (stk[SP+1] == 0) stk[SP] = 0;                                  //RDR
      else stk[SP] = stk[SP]/stk[SP+1];                                 //RDR
      break;                                                            //RDR
    case COP_PERCENT:                                                   //RDR
      C2                                                                //RDR
      if (stk[SP+1] == 0) stk[SP] = 0;                                  //RDR
      else stk[SP] = stk[SP]%stk[SP+1];                                 //RDR
      break;                                                            //RDR
    case COP_BITOR:        OP2(|)                                       //RDR
    case COP_BITXOR:       OP2(^)                                       //RDR
    case COP_BITAND:       OP2(&)                                       //RDR
    case COP_MINUS:        OP2(-)                                       //RDR
    case COP_NEGATE: C1  stk[SP] = -stk[SP]; break;                     //RDR
    case COP_NOT:C1  stk[SP] = !stk[SP]; break;                         //RDR
    case COP_MIN:C2 stk[SP]=stk[SP]>stk[SP+1]?stk[SP]:stk[SP+1]; break; //RDR
    case COP_MAX:C2 stk[SP]=stk[SP]<stk[SP+1]?stk[SP]:stk[SP+1]; break; //RDR    
    case COP_RDR:                                                       //RDR
        array[0] = m_getInteger(&expr);                                 //RDR
        for (i=1; i<=array[0]; i++)                                     //RDR
        {                                                               //RDR
          array[i] = m_getInteger(&expr);                               //RDR
        };                                                              //RDR
        IS stk[SP] = (*interpretRDR)(array);                            //RDR
        break;                                                          //RDR
    case COP_INTEGER1:                                                  //RDR
    case COP_INTEGER2:                                                  //RDR
    case COP_INTEGER3:                                                  //RDR
    case COP_INTEGER4:                                                  //RDR
        expr--; IS  stk[SP] = m_getInteger(&expr); break;               //RDR
    case COP_LXOR:                                                      //RDR
        C2                                                              //RDR
        stk[SP] = stk[SP] && !stk[SP+1] || !stk[SP] && stk[SP+1];       //RDR
        break;                                                          //RDR
    default: ASSERT(false); // That should cause an error!              //RDR
             return -1;                                                 //RDR
    };                                                                  //RDR
  };                                                                    //RDR
  return stk[SP];                                                       //RDR
}                                                                       //RDR

//GPDL::VARIABLES::VARIABLES(void) {
//  m_firstVariable=NULL;
//}

//GPDL::VARIABLES::~VARIABLES(void) {
//  VARIABLE *v;
//  while (m_firstVariable!=NULL) {
//    v=m_firstVariable;
//    m_firstVariable=v->m_next;
//    v->m_next=NULL;
//    delete v;
//  };
//}


//CString GPDL::VARIABLES::Set(const CString& name, 
//                     const CString& value) {
//  VARIABLE *v;
//  for (v=m_firstVariable; v!=NULL; v=v->m_next) {
//    if (name==v->m_name) {
//      CString result;
//      result=v->m_value;
//      v->m_value=value;
//      return result;
//    };
//  };
//  v=new VARIABLE;
//  v->m_name=name;
//  v->m_value=value;
//  v->m_next=m_firstVariable;
//  m_firstVariable=v;
//  return m_false;
//}
//
//CString GPDL::VARIABLES::Get(const CString& name) {
//  VARIABLE *v;
//  for (v=m_firstVariable; v!=NULL; v=v->m_next) {
//    if (name==v->m_name) {
//      return v->m_value;
//    };
//  };
//
//  return m_false;
//}
//
//GPDL::VARIABLES::VARIABLE::VARIABLE(void) {
//}
//
//  
//GPDL::VARIABLES::VARIABLE::~VARIABLE(void) {
//  if (m_next!=NULL) {
//    MessageBox(NULL,
//             "GPDL::VARIABLES::VARIABLE::~VARIABLE error",
//           NULL,
//           MB_OK|MB_TOPMOST);
//    return;
//  };
//}