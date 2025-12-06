/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SPYCRAFT_GAME_CYPHER_H
#define SPYCRAFT_GAME_CYPHER_H

#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/web.h"

namespace Spycraft {

class CypherBack : public IntrPuzzView
{
public:
	CypherBack();
};

class NoCypherBox : public IntrPuzzView
{
public:
	NoCypherBox();
};

class PDHighLight : public IntrPuzzView
{
public:
	PDHighLight(int hilightNum);
};

class CodeInfo : public Print
{
public:
	CodeInfo(char *);
};

class ItemsList : public List
{
public:
	int handleEvent(MADEEventStamp *);
};


class PullDownFeature : public Feature
{
public:
	int index;

	PullDownFeature(int theX, int theY, int featureIndex);
	void init(int x, int y, int width, int height);
	int handleEvent(MADEEventStamp *);
};

class PullDown : public IntrPuzzView
{
public:
	ItemsList *FeatureItems;
	int numItems;
	PDHighLight *hilight;
	void dispose();
	PullDown();
	~PullDown();

	//	int handleEvent ( MADEEventStamp * );
	void createItems();
};

class KeyPullDown : public PullDown
{
public:
	int thisCode;

	KeyPullDown(int theX, int theY, int theLoop, int totalItems, int hilightNum);
	void displayInfo();
	int handleEvent(MADEEventStamp *);
};

class BookKeyPullDown : public PullDown
{
public:
	int currentCode;
	KeyPullDown *keypd;

	BookKeyPullDown();
	~BookKeyPullDown();
	void removePullDown();
	void dispose();
	int handleEvent(MADEEventStamp *);
};

class CodesPullDown : public PullDown
{
public:
	int currentCode;
	KeyPullDown *keypd;

	CodesPullDown();
	void removePullDown();
	int handleEvent(MADEEventStamp *);
};

class AlphaDecrypt : public IntrPuzzProp
{
public:

	AlphaDecrypt();
	void cue(void);
};

class AlphaDecryptScript : public Script
{
public:
	AlphaDecrypt *alphaDecrypt;

	AlphaDecryptScript(AlphaDecrypt *theProp);
	void changeState(int);
};

class Decrypt : public IntrPuzzProp
{
public:
	KeyPullDown *theKey;

	Decrypt(KeyPullDown *thiskey);
	void cue(void);
};

class DecryptScript : public Script
{
public:
	Decrypt *decrypt;

	DecryptScript(Decrypt *theProp);
	void changeState(int);
};

class TextList : public List
{
public:
	void dispose();
};

class CodeButt : public Button
{
public:
	bool menusActive;

	CodeButt();
	int handleEvent(MADEEventStamp *);
	void removeMenus();
};

class Cypher : public IntrPuzzPage
{
public:
	int codeMatch;
	int keyMatch;
	int matchMsgID;
	int numVlinks;
	char vlinks[5][10];
	int numFlags;
	int flags[5];
	Sound *cypherSFX;
	Sound *decryptSFX;

	Cypher();
	~Cypher();
	void dispose();
	void loadCode(int id);
	void loadDecode(int id);
};

class YingCypherNote : public Cypher
{
public:
	WebReport *yingsendbut;

	YingCypherNote();
	void dispose();
};

class YingSendBut : public WebReport
{
public:
	YingSendBut();
	void activate();
	void cue();
};

class PowerBookOne : public Cypher
{
public:
	PowerBookOne();
};

class PowerBookTwo : public Cypher
{
public:
	PowerBookTwo();
};

class PowerBookThree : public Cypher
{
public:
	PowerBookThree();
};

class PowerBookFour : public Cypher
{
public:
	PowerBookFour();
};

class PowerBookFive : public Cypher
{
public:
	PowerBookFive();
};

class PowerBookSix : public Cypher
{
public:
	PowerBookSix();
};

class PowerBookSeven : public Cypher
{
public:
	PowerBookSeven();
};

class CypherPitOne : public Cypher
{
public:
	CypherPitOne();
};

class CypherPitTwo : public Cypher
{
public:
	CypherPitTwo();
};

class CypherPitThree : public Cypher
{
public:
	CypherPitThree();
};

class CypherMoleOne : public Cypher
{
public:
	CypherMoleOne();
};

class CypherMoleTwo : public Cypher
{
public:
	CypherMoleTwo();
};

class CypherMoleThree : public Cypher
{
public:
	CypherMoleThree();
};

class CypherMoleFour : public Cypher
{
public:
	CypherMoleFour();
};

class CypherEmpty : public Cypher
{
public:
	CypherEmpty();
};

} // namespace Spycraft

#endif			
