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

#ifndef DUNGEONCRAFT_SHARED_GPDLOPCODES
#define DUNGEONCRAFT_SHARED_GPDLOPCODES

namespace DungeonCraft {

enum BINOPS {
	// Opcodes that require an address or number
	BINOP_ILLEGAL				=0,
	BINOP_JUMP,			
	BINOP_FETCHCONSTANT,	
	BINOP_JUMPFALSE,
	BINOP_STORE_FP,
	BINOP_FETCH_FP,
	BINOP_SUBOP,       // Opcode with sub-opcode
	BINOP_CALL,        // Call user-defined function
	BINOP_RETURN, //0x08   // subop = #locals<<12 + #parameters to pop
  BINOP_LOCALS,
  BINOP_FETCHTEXT  //c-style string from the code segment
};
// commonly used expression
const unsigned int SHIFTED_BINOP = (BINOP_SUBOP<<24);

enum SUBOPS {
	SUBOP_ILLEGAL			            =0x000,
	SUBOP_NOOP                    =0x001,
	SUBOP_POP                     =0x002,
	SUBOP_DUP                     =0x003,			
	SUBOP_LISTEN                  =0x004,
	SUBOP_LISTENTEXT              =0x005,
	SUBOP_TESTKEY                 =0x006,
	SUBOP_SAY                     =0x007,
	SUBOP_NUMERIC                 =0x008,
	SUBOP_nLESS                   =0x009,
	SUBOP_nTIMES                  =0x00a,
	SUBOP_nMINUS                  =0x00b,
	SUBOP_PLUS                    =0x00c, 
	SUBOP_ISEQUAL                 =0x00d,
	SUBOP_nISEQUAL                =0x00e,
	SUBOP_GREP                    =0x00f, 
	SUBOP_RANDOM                  =0x010,
	SUBOP_SWAP                    =0x011,
	SUBOP_OVER                    =0x012,
	SUBOP_DEBUG                   =0x013,
	SUBOP_iTIMES                  =0x014,
	SUBOP_SET_GLOBAL_ASL          =0x015,
	SUBOP_GET_GLOBAL_ASL          =0x016,
	SUBOP_NOT                     =0x017,
	SUBOP_LENGTH                  =0x018,
	SUBOP_SET_CHAR_ASL            =0x019,
	SUBOP_PARTYSIZE               =0x01a,
	SUBOP_GET_CHAR_SEX            =0x01b,
	SUBOP_GET_CHAR_ASL            =0x01c,
	SUBOP_GET_CHAR_NAME           =0x01d,
	SUBOP_SET_LEVEL_ASL           =0x01e,
	SUBOP_nGREATER                =0x01f,
	SUBOP_DELETE_LEVEL_ASL        =0x020,
	SUBOP_DEPRECATED_SET_LEVEL_STATS_ASL    = 0x21, //SUBOP_SET_LEVEL_STATS_ASL     =0x021,
	SUBOP_DEPRECATED_DELETE_LEVEL_STATS_ASL = 0x22, //DELETE_LEVEL_STATS_ASL  =0x022,
  SUBOP_SET_PARTY_ASL           =0x023,
  SUBOP_GET_PARTY_ASL           =0x024,
  SUBOP_IF_PARTY_ASL            =0x025,
  SUBOP_DELETE_PARTY_ASL        =0x026,
  SUBOP_SMALL_PICTURE           =0x027,
  SUBOP_SET_QUEST               =0x028,
  SUBOP_GET_CHAR_AC             =0x029,
  SUBOP_SET_CHAR_AC             =0x02a,
  SUBOP_GET_CHAR_HITPOINTS      =0x02b,
  SUBOP_SET_CHAR_HITPOINTS      =0x02c,
  SUBOP_GET_CHAR_THACO          =0x02d,
  SUBOP_SET_CHAR_THACO          =0x02e,
  SUBOP_GET_CHAR_RDYTOTRAIN     =0x02f,
  SUBOP_SET_CHAR_RDYTOTRAIN     =0x030,
  SUBOP_GET_CHAR_CLERICEXP      =0x031,
  SUBOP_SET_CHAR_CLERICEXP      =0x032,
  SUBOP_GET_CHAR_MAGICUSEREXP   =0x033,
  SUBOP_SET_CHAR_MAGICUSEREXP   =0x034,
  SUBOP_GET_CHAR_FIGHTEREXP     =0x035,
  SUBOP_SET_CHAR_FIGHTEREXP     =0x036,
  SUBOP_GET_CHAR_RANGEREXP      =0x037,
  SUBOP_SET_CHAR_RANGEREXP      =0x038,
  SUBOP_GET_CHAR_THIEFEXP       =0x039,
  SUBOP_SET_CHAR_THIEFEXP       =0x03a,
  SUBOP_GET_CHAR_DRUIDEXP       =0x03b,
  SUBOP_SET_CHAR_DRUIDEXP       =0x03c,
  SUBOP_GET_CHAR_PALADINEXP     =0x03d,
  SUBOP_SET_CHAR_PALADINEXP     =0x03e,
  SUBOP_GET_CHAR_AGE            =0x03f,
  SUBOP_SET_CHAR_AGE            =0x040,
  SUBOP_GET_CHAR_MAXAGE         =0x041,
  SUBOP_SET_CHAR_MAXAGE         =0x042,
  SUBOP_GET_CHAR_MAXMOVE        =0x043,
  SUBOP_SET_CHAR_MAXMOVE        =0x044,
  SUBOP_GET_CHAR_STR            =0x045,
  SUBOP_SET_CHAR_STR            =0x046,
  SUBOP_GET_CHAR_STRMOD         =0x047,
  SUBOP_SET_CHAR_STRMOD         =0x048,
  SUBOP_GET_CHAR_INT            =0x049,
  SUBOP_SET_CHAR_INT            =0x04a,
  SUBOP_GET_CHAR_WIS            =0x04b,
  SUBOP_SET_CHAR_WIS            =0x04c,
  SUBOP_GET_CHAR_DEX            =0x04d,
  SUBOP_SET_CHAR_DEX            =0x04e,
  SUBOP_GET_CHAR_CON            =0x04f,
  SUBOP_SET_CHAR_CON            =0x050,
  SUBOP_GET_CHAR_CHA            =0x051,
  SUBOP_SET_CHAR_CHA            =0x052,
  SUBOP_GET_CHAR_MAXENC         =0x053,
  SUBOP_SET_CHAR_MAXENC         =0x054,
  SUBOP_GET_CHAR_ENC            =0x055,
  SUBOP_GET_CHAR_GENDER         =0x056,
  SUBOP_SET_CHAR_GENDER         =0x057,
  SUBOP_GET_CHAR_CLASS          =0x058,
  SUBOP_SET_CHAR_CLASS          =0x059,
  SUBOP_GET_CHAR_ALIGNMENT      =0x05a,
  SUBOP_SET_CHAR_ALIGNMENT      =0x05b,
  SUBOP_GET_CHAR_STATUS         =0x05c,
  SUBOP_SET_CHAR_STATUS         =0x05d,
  SUBOP_GET_CHAR_UNDEAD         =0x05e,
  SUBOP_SET_CHAR_UNDEAD         =0x05f,
  SUBOP_GET_CHAR_SIZE           =0x060,
  SUBOP_SET_CHAR_SIZE           =0x061,
  SUBOP_GET_CHAR_MAGICRESIST    =0x062,
  SUBOP_SET_CHAR_MAGICRESIST    =0x063,
  SUBOP_GET_CHAR_SAVEVSPPDM     =0x064,
  SUBOP_SET_CHAR_SAVEVSPPDM     =0x065,
  SUBOP_GET_CHAR_SAVEVSPP       =0x066,
  SUBOP_SET_CHAR_SAVEVSPP       =0x067,
  SUBOP_GET_CHAR_SAVEVSRSW      =0x068,
  SUBOP_SET_CHAR_SAVEVSRSW      =0x069,
  SUBOP_GET_CHAR_SAVEVSBR       =0x06a,
  SUBOP_SET_CHAR_SAVEVSBR       =0x06b,
  SUBOP_GET_CHAR_SAVEVSSP       =0x06c,
  SUBOP_SET_CHAR_SAVEVSSP       =0x06d,
  SUBOP_GET_CHAR_CLERICLVL      =0x06e,
  SUBOP_GET_CHAR_FIGHTERLVL     =0x06f,
  SUBOP_GET_CHAR_RANGERLVL      =0x070,
  SUBOP_GET_CHAR_DRUIDLVL       =0x071,
  SUBOP_GET_CHAR_PALADINLVL     =0x072,
  SUBOP_GET_CHAR_THIEFLVL       =0x073,
  SUBOP_GET_CHAR_MAGUSERLVL     =0x074,
  SUBOP_GET_CHAR_CLERICPREVLVL  =0x075,
  SUBOP_GET_CHAR_FIGHTERPREVLVL =0x076,
  SUBOP_GET_CHAR_RANGERPREVLVL  =0x077,
  SUBOP_GET_CHAR_DRUIDPREVLVL   =0x078,
  SUBOP_GET_CHAR_PALADINPREVLVL =0x079,
  SUBOP_GET_CHAR_THIEFPREVLVL   =0x07a,
  SUBOP_GET_CHAR_MAGUSERPREVLVL =0x07b,
  SUBOP_GET_CHAR_CLERICPDLVL    =0x07c,
  SUBOP_GET_CHAR_FIGHTERPDLVL   =0x07d,
  SUBOP_GET_CHAR_RANGERPDLVL    =0x07e,
  SUBOP_GET_CHAR_DRUIDPDLVL     =0x07f,
  SUBOP_GET_CHAR_PALADINPDLVL   =0x080,
  SUBOP_GET_CHAR_THIEFPDLVL     =0x081,
  SUBOP_GET_CHAR_MAGUSERPDLVL   =0x082,
  SUBOP_GET_CHAR_NBRHITDICE     =0x083,
  SUBOP_GET_CHAR_NBRATTACKS     =0x084,
  SUBOP_GET_CHAR_MORALE         =0x085,
  SUBOP_SET_CHAR_MORALE         =0x086,
  SUBOP_GET_CHAR_OPENDOORS      =0x087,
  SUBOP_SET_CHAR_OPENDOORS      =0x088,
  SUBOP_GET_CHAR_OPENMAGICDOORS =0x089,
  SUBOP_SET_CHAR_OPENMAGICDOORS =0x08a,
  SUBOP_GET_CHAR_BENDLIFT       =0x08b,
  SUBOP_SET_CHAR_BENDLIFT       =0x08c,
  SUBOP_GET_CHAR_PICKPOCKETS    =0x08d,
  SUBOP_SET_CHAR_PICKPOCKETS    =0x08e,
  SUBOP_GET_CHAR_OPENLOCKS      =0x08f,
  SUBOP_SET_CHAR_OPENLOCKS      =0x090,
  SUBOP_GET_CHAR_FINDTRAPS      =0x091,
  SUBOP_SET_CHAR_FINDTRAPS      =0x092,
  SUBOP_GET_CHAR_MOVESILENT     =0x093,
  SUBOP_SET_CHAR_MOVESILENT     =0x094,
  SUBOP_GET_CHAR_HIDESHADOWS    =0x095,
  SUBOP_SET_CHAR_HIDESHADOWS    =0x096,
  SUBOP_GET_CHAR_HEARNOISE      =0x097,
  SUBOP_SET_CHAR_HEARNOISE      =0x098,
  SUBOP_GET_CHAR_CLIMBWALLS     =0x099,
  SUBOP_SET_CHAR_CLIMBWALLS     =0x09a,
  SUBOP_GET_CHAR_READLANG       =0x09b,
  SUBOP_SET_CHAR_READLANG       =0x09c,
  SUBOP_nPLUS                   =0x09d,
  SUBOP_GET_PARTY_FACING        =0x09e,
  SUBOP_SET_PARTY_FACING        =0x09f,
  SUBOP_GET_PARTY_DAYS          =0x0a0,
  SUBOP_SET_PARTY_DAYS          =0x0a1,
  SUBOP_GET_PARTY_HOURS         =0x0a2,
  SUBOP_SET_PARTY_HOURS         =0x0a3,
  SUBOP_GET_PARTY_MINUTES       =0x0a4,
  SUBOP_SET_PARTY_MINUTES       =0x0a5,
  SUBOP_GET_PARTY_ACTIVECHAR    =0x0a6,
  SUBOP_SET_PARTY_ACTIVECHAR    =0x0a7,
  SUBOP_GET_GAME_CURRLEVEL      =0x0a8,
  SUBOP_GET_GAME_VERSION        =0x0a9,
  SUBOP_Myself                  =0x0aa,
  SUBOP_Gender                  =0x0ab,
  SUBOP_LOR                     =0x0ac,
  SUBOP_LAND                    =0x0ad,
  SUBOP_nOR                     =0x0ae,
  SUBOP_nXOR                    =0x0af,
  SUBOP_nAND                    =0x0b0,
   // 0x0b1 NOT USED
   // 0x0b2 NOT USED
  SUBOP_LESS                    =0x0b3,
  SUBOP_LESSEQUAL               =0x0b4,
  SUBOP_nLESSEQUAL              =0x0b5,
  SUBOP_GREATER                 =0x0b6,
  SUBOP_GREATEREQUAL            =0x0b7,
  SUBOP_nGREATEREQUAL           =0x0b8,
  SUBOP_nSLASH                  =0x0b9,
  SUBOP_nPERCENT                =0x0ba,
  SUBOP_nNOTEQUAL               =0x0bb,
  SUBOP_NOTEQUAL                =0x0bc,
  SUBOP_iPLUS                   =0x0bd,
  SUBOP_iMINUS                  =0x0be,
  SUBOP_iDIV                    =0x0bf,
  SUBOP_iMOD                    =0x0c0,
  SUBOP_iEQUAL                  =0x0c1,
  SUBOP_iLESS                   =0x0c2,
  SUBOP_iGREATER                =0x0c3,
  SUBOP_nNEGATE                 =0x0c4,
  SUBOP_Name                    =0x0c5,
  SUBOP_Target                  =0x0c6,
  SUBOP_Class                   =0x0c7,
  SUBOP_Race                    =0x0c8,
  SUBOP_Status                  =0x0c9,
  SUBOP_Alignment               =0x0ca,
  SUBOP_AlignmentGood           =0x0cb,
  SUBOP_AlignmentEvil           =0x0cc,
  SUBOP_AlignmentLawful         =0x0cd,
  SUBOP_AlignmentNeutral        =0x0ce,
  SUBOP_AlignmentChaotic        =0x0cf,
  SUBOP_HitPoints               =0x0d0,
  SUBOP_InParty                 =0x0d1,
  SUBOP_SaveVsSpell             =0x0d2,
  SUBOP_SaveVsBreathWeapon      =0x0d3,
  SUBOP_SaveVsRodStaffWand      =0x0d4,
  SUBOP_SaveVsPetPoly           =0x0d5,
  SUBOP_SaveVsParPoiDM          =0x0d6,
  SUBOP_IsUndead                =0x0d7,
  SUBOP_FALSE                   =0x0d8,
  SUBOP_ONE                     =0x0d9,
  SUBOP_ZERO                    =0x0da,
  SUBOP_MIDDLE                  =0x0db,
  SUBOP_GET_CHAR_BLESS              =0x0dc,
  SUBOP_SET_CHAR_BLESS              =0x0dd,
  SUBOP_GET_CHAR_CURSE              =0x0de,
  SUBOP_SET_CHAR_CURSE              =0x0df,
  SUBOP_GET_CHAR_UNDEADFEAR         =0x0e0,
  SUBOP_SET_CHAR_UNDEADFEAR         =0x0e1,
  SUBOP_GET_CHAR_ENLARGE            =0x0e2,
  SUBOP_SET_CHAR_ENLARGE            =0x0e3,
  SUBOP_GET_CHAR_REDUCE             =0x0e4,
  SUBOP_SET_CHAR_REDUCE             =0x0e5,
  SUBOP_GET_CHAR_CHARMPERSON        =0x0e6,
  SUBOP_SET_CHAR_CHARMPERSON        =0x0e7,
  SUBOP_GET_CHAR_REFLECTGAZEATTACK  =0x0e8,
  SUBOP_SET_CHAR_REFLECTGAZEATTACK  =0x0e9,
  SUBOP_GET_CHAR_PROTFROMEVIL       =0x0ea,
  SUBOP_SET_CHAR_PROTFROMEVIL       =0x0eb,
  SUBOP_GET_CHAR_PROTFROMGOOD       =0x0ec,
  SUBOP_SET_CHAR_PROTFROMGOOD       =0x0ed,
  SUBOP_GET_CHAR_SHIELD             =0x0ee,
  SUBOP_SET_CHAR_SHIELD             =0x0ef,
  SUBOP_GET_CHAR_SLEEP              =0x0f0,
  SUBOP_SET_CHAR_SLEEP              =0x0f1,
  SUBOP_GET_CHAR_FOG                =0x0f2,
  SUBOP_SET_CHAR_FOG                =0x0f3,
  SUBOP_GET_CHAR_ENTANGLE           =0x0f4,
  SUBOP_SET_CHAR_ENTANGLE           =0x0f5,
  SUBOP_GET_CHAR_INVISIBLETOANIMALS =0x0f6,
  SUBOP_SET_CHAR_INVISIBLETOANIMALS =0x0f7,
  SUBOP_GET_CHAR_NONUNDEADFEAR      =0x0f8,
  SUBOP_SET_CHAR_NONUNDEADFEAR      =0x0f9,
  SUBOP_GET_CHAR_SANCTUARY          =0x0fa,
  SUBOP_SET_CHAR_SANCTUARY          =0x0fb,
  SUBOP_GET_CHAR_SHILLELAGH         =0x0fc,
  SUBOP_SET_CHAR_SHILLELAGH         =0x0fd,
  SUBOP_GET_CHAR_DISPLACEMENT       =0x0fe,
  SUBOP_SET_CHAR_DISPLACEMENT       =0x0ff,
  SUBOP_GET_CHAR_WIZARDRY           =0x100,
  SUBOP_SET_CHAR_WIZARDRY           =0x101,
  SUBOP_GET_CHAR_DETECTMAGIC        =0x102,
  SUBOP_SET_CHAR_DETECTMAGIC        =0x103,
  SUBOP_IndexOf                     =0x104,
  SUBOP_GET_CHAR_INVISIBLETOUNDEAD  =0x105,
  SUBOP_SET_CHAR_INVISIBLETOUNDEAD  =0x106,
  SUBOP_GET_CHAR_TYPE               =0x107,
  SUBOP_GET_CHAR_VORPALATTACK       =0x108,
  SUBOP_SET_CHAR_VORPALATTACK       =0x109,
  SUBOP_COINNAME                    =0x10a,
  SUBOP_COINCOUNT                   =0x10b,
  SUBOP_COINRATE                    =0x10c,
  SUBOP_GET_PARTY_MONEYAVAILABLE    =0x10d,
  SUBOP_GET_VAULT_MONEYAVAILABLE    =0x10e,
  SUBOP_LAST_ATTACKER_OF            =0x10f,
  SUBOP_NOT_USED_FOR_ANYTHING1      =0x110,
  SUBOP_NOT_USED_FOR_ANYTHING2      =0x111,
  SUBOP_LEAST_DAMAGED_FRIENDLY      =0x112,
  SUBOP_MOST_DAMAGED_FRIENDLY       =0x113,
  SUBOP_NEAREST_TO                  =0x114,
  SUBOP_NEAREST_ENEMY_TO            =0x115,
  SUBOP_LEAST_DAMAGED_ENEMY         =0x116,
  SUBOP_MOST_DAMAGED_ENEMY          =0x117,
  SUBOP_IS_AFFECTED_BY_SPELL        =0x118,
  SUBOP_IS_AFFECTED_BY_SPELL_ATTR   =0x119,
  SUBOP_CURR_CHANGE_BY_VAL          =0x11A,
  SUBOP_GET_CHAR_HOLDPERSON         =0x11B,
  SUBOP_SET_CHAR_HOLDPERSON         =0x11C,  
  SUBOP_GET_CHAR_SILENCE            =0x11D,
  SUBOP_SET_CHAR_SILENCE            =0x11E,
  SUBOP_GET_ISMAMMAL                =0x11F,
  SUBOP_GET_ISANIMAL                =0x120,
  SUBOP_GET_ISSNAKE                 =0x121,
  SUBOP_GET_ISGIANT                 =0x122,
  SUBOP_GET_ISALWAYSLARGE           =0x123,
  SUBOP_GET_HASDWARFACPENALTY       =0x124,
  SUBOP_GET_HASGNOMEACPENALTY       =0x125,
  SUBOP_GET_HASDWARFTHACOPENALTY    =0x126,
  SUBOP_GET_HASGNOMETHACOPENALTY    =0x127,
  SUBOP_GET_HASRANGERDMGPENALTY     =0x128,
  SUBOP_GET_HASPOISONIMMUNITY       =0x129,
  SUBOP_GET_HASDEATHIMMUNITY        =0x12A,
  SUBOP_GET_HASCONFUSIONIMMUNITY    =0x12B,
  SUBOP_GET_HASVORPALIMMUNITY       =0x12C,
  SUBOP_GET_CANBEHELDORCHARMED      =0x12D,
  SUBOP_GET_AFFECTEDBYDISPELEVIL    =0x12E,
  SUBOP_GET_CHAR_POISONED           =0x12F,
  SUBOP_SET_CHAR_POISONED           =0x130,
  SUBOP_GET_CHAR_SLOWPOISON         =0x131,
  SUBOP_SET_CHAR_SLOWPOISON         =0x132,
  SUBOP_GIVE_CHAR_ITEM              =0x133,
  SUBOP_TAKE_CHAR_ITEM              =0x134,
  SUBOP_GET_CHAR_MIRRORIMAGE        =0x135,
  SUBOP_SET_CHAR_MIRRORIMAGE        =0x136,
  SUBOP_GET_CHAR_INVISIBLE          =0x137,
  SUBOP_SET_CHAR_INVISIBLE          =0x138,
  SUBOP_GET_CHAR_ENFEEBLED          =0x139,
  SUBOP_SET_CHAR_ENFEEBLED          =0x13A,
  SUBOP_LOGIC_BLOCK_VALUE           =0x13B,
  SUBOP_GET_CHAR_DAMAGEBONUS        =0x13C,
  SUBOP_SET_CHAR_DAMAGEBONUS        =0x13D,
  SUBOP_GET_CHAR_HITBONUS           =0x13E,
  SUBOP_SET_CHAR_HITBONUS           =0x13F,
  SUBOP_GET_CHAR_BLINDNESS          =0x140,
  SUBOP_SET_CHAR_BLINDNESS          =0x141,
  SUBOP_GET_CHAR_DISEASED           =0x142,
  SUBOP_SET_CHAR_DISEASED           =0x143,
  SUBOP_CHAR_REMOVEALLSPELLS        =0x144,
  SUBOP_TARGETINDEX                 =0x145,
  SUBOP_MYINDEX                     =0x146,
  SUBOP_INVOKESPELLONTARGET         =0x147,
  SUBOP_INVOKESPELLONTARGETAS       =0x148,
  SUBOP_CHAR_REMOVESPELL            =0x149,
  SUBOP_CHAR_DISPELMAGIC            =0x14A,
  SUBOP_CHAR_REMOVEALLITEMCURSE     =0x14B,
  SUBOP_SET_LEVEL_STATS_ASL         =0x14C,
  SUBOP_DELETE_LEVEL_STATS_ASL      =0x14D
};

//**********************************************************************//RDR
//                                                                      //RDR
//**********************************************************************//RDR
enum COPS {  //Character-sized opcodes                                  //RDR
	COP_ILLEGAL				=0,                                                 //RDR
  COP_LOR           =1,                                                 //RDR
  COP_LAND          =2,                                                 //RDR
  COP_ISEQUAL       =3,                                                 //RDR
  COP_NOTEQUAL      =4,                                                 //RDR
  COP_LESS          =5,                                                 //RDR
  COP_LESSEQUAL     =6,                                                 //RDR
  COP_GREATER       =7,                                                 //RDR
  COP_GREATEREQUAL  =8,                                                 //RDR
  COP_PLUS          =9,                                                 //RDR
  COP_TIMES         =10,                                                //RDR
  COP_SLASH         =11,                                                //RDR
  COP_PERCENT       =12,                                                //RDR
  COP_NEGATE        =13,                                                //RDR
  COP_NOT           =14,                                                //RDR
  COP_RDR           =15,                                                //RDR
  COP_INTEGER1      =16,                                                //RDR
  COP_INTEGER2      =17,                                                //RDR
  COP_INTEGER3      =18,                                                //RDR
  COP_INTEGER4      =19,                                                //RDR
  COP_LXOR          =20,                                                //RDR
  COP_BITOR         =21,                                                //RDR
  COP_BITXOR        =22,                                                //RDR
  COP_BITAND        =23,                                                //RDR
  COP_MINUS         =24,                                                //RDR
  COP_MIN           =25,                                                //RDR
  COP_MAX           =26,                                                //RDR
  COP_NONE          =27  //No operation needed.  Like unary Plus.       //RDR
};                                                                      //RDR
                                                                        //RDR
enum CTOKENTYPE {CTKN_NONE=0,                                           //RDR
        CTKN_NAME,                                                      //RDR
        CTKN_PLUS,                                                      //RDR
        CTKN_MINUS,                                                     //RDR
        CTKN_INTEGER,                                                   //RDR
        CTKN_OPENPAREN,                                                 //RDR
        CTKN_CLOSEPAREN,                                                //RDR
        CTKN_COLON,                                                     //RDR
        CTKN_GEAR,                                                      //RDR
        CTKN_SLASH,                                                     //RDR
        CTKN_EQUAL,                                                     //RDR
        CTKN_LESS,                                                      //RDR
        CTKN_GREATER,                                                   //RDR
        CTKN_ISEQUAL,                                                   //RDR
        CTKN_NOTEQUAL,                                                  //RDR
        CTKN_LOR,                                                       //RDR
        CTKN_LXOR,                                                      //RDR
        CTKN_LAND,                                                      //RDR
        CTKN_BITOR,                                                     //RDR
        CTKN_BITXOR,                                                    //RDR
        CTKN_BITAND,                                                    //RDR
        CTKN_LESSEQUAL,                                                 //RDR
        CTKN_GREATEREQUAL,                                              //RDR
        CTKN_PERCENT,                                                   //RDR
        CTKN_NOT,                                                       //RDR
        CTKN_DICE,                                                      //RDR
        CTKN_MIN,                                                       //RDR
        CTKN_MAX,                                                       //RDR
        CTKN_STRING                                                     //RDR
       };                                                               //RDR
                                                                        //RDR

} // End of namespace DungeonCraft

#endif