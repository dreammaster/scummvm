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

#ifndef SPYCRAFT_GAME_MAPTOOL_H
#define SPYCRAFT_GAME_MAPTOOL_H

#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/web.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

#define	EREVAN		725
#define DUCHANBE	731
#define IRKUTSK		737
#define MOSCOW		743
#define NOVOSIBIRSK 749
#define ROSTOV		755
#define PETERSBURG	761

/*	DUCHANBE,
	IRKUTSK,
	MOSCOW,
	NOVOSIBIRSK,
	ROSTOV,
	PETERSBURG
	};*/

class MapTool;

class MapSound : public Sound
{
public:
	void playStream(int soundNumber);
	void play(int soundNumber);
	void play(int soundNumber, Object *whoToCue);
};

class MapInfo : public Object
{
public:
	int jet;
	int car;
	int bird;
	int church;
	int arm;

	bool seenDuchanbe;
	bool seenErevan;
	bool seenPeters;
	bool seenNov;
	bool seenMoscow;
	bool seenRostov;
	bool seenIrkutsk;

	MapInfo();
	void load();
	void save();
	void dispose();
};

class MapToolBack : public IntrPuzzView
{
protected:
	const char *name;
public:
	MapToolBack();
};

class MapSendBut : public WebReport
{
public:
	MapSendBut();
	void activate();
	void cue();
};

class MapButton : public Button
{
public:
	const char *name;
	int buttID;

	void setMap();
	int handleEvent(MADEEventStamp *event);
	void activate();
};

class RussMapButton : public Button
{
public:
	IntrPuzzView *sound;
	bool hidden;
	int buttID;

	void setMap();
	void activate();
	int handleEvent(MADEEventStamp *event);
};

class RangeButt : public MapButton
{
protected:
	const char *name;
public:
	RangeButt(int index, int id);
};

class RussRangeButt : public RussMapButton
{
public:
	RussRangeButt(int id);
};

class ChurchButt : public MapButton
{
protected:
	const char *name;
public:
	ChurchButt(int index, int id);
};

class RussChurchButt : public RussMapButton
{
public:
	RussChurchButt(int id);
};

class BirdButt : public MapButton
{
protected:
	const char *name;
public:
	BirdButt(int index, int id);
};

class RussBirdButt : public RussMapButton
{
public:
	RussBirdButt(int id);
};

class CarButt : public MapButton
{
protected:
	const char *name;
public:
	CarButt(int index, int id);
};

class RussCarButt : public RussMapButton
{
public:
	RussCarButt(int id);
};

class JetButt : public MapButton
{
protected:
	const char *name;
public:
	JetButt(int index, int id);
};

class RussJetButt : public RussMapButton
{
public:
	RussJetButt(int id);
};

class BackButt : public Button
{
public:
	CommandLine *comLine;

	BackButt();
	void activate();
};

class Map : public IntrPuzzView
{
public:
	int currentMapView;

	Map(int theView);
};

class ErevanSpot : public IntrPuzzFeature
{
public:
	CommandLine *comLine;

	ErevanSpot();
	int handleEvent(MADEEventStamp *event);
};

class MoscowSpot : public IntrPuzzFeature
{
public:
	CommandLine *comLine;

	MoscowSpot();
	int handleEvent(MADEEventStamp *event);
};

class PeterSpot : public IntrPuzzFeature
{
public:
	CommandLine *comLine;

	PeterSpot();
	int handleEvent(MADEEventStamp *event);
};

class NovosSpot : public IntrPuzzFeature
{
public:
	CommandLine *comLine;

	NovosSpot();
	int handleEvent(MADEEventStamp *event);
};

class DuchSpot : public IntrPuzzFeature
{
public:
	CommandLine *comLine;

	DuchSpot();
	int handleEvent(MADEEventStamp *event);
};

class RostovSpot : public IntrPuzzFeature
{
public:
	CommandLine *comLine;

	RostovSpot();
	int handleEvent(MADEEventStamp *event);
};

class IrkSpot : public IntrPuzzFeature
{
public:
	CommandLine *comLine;

	IrkSpot();
	int handleEvent(MADEEventStamp *event);
};

class MapTool : public IntrPuzzPage
{
public:
	int numButtsChecked;
	int numActiveButts;
	int totalButts;
	bool checked[5];
	MapSound *mapSound;
	MapSendBut *mapreport;
	IrkSpot *irkspot;
	MoscowSpot *moscowspot;
	RostovSpot *rostovspot;
	DuchSpot *duchspot;
	ErevanSpot *erevanspot;
	NovosSpot *novosspot;
	PeterSpot *peterspot;
	IntrPuzzView *names;

	MapTool();
	~MapTool();
	void dispose();
	void removeSoundButtons();
	void saveState();
	void createSpots();
	void removeSpots();
};

} // namespace Spycraft

#endif
