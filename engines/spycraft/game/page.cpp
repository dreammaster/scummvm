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

#include "spycraft/game/web.h"
#include "spycraft/dmade/gamebox.h"
#include "spycraft/game/sndanal.h"
#include "spycraft/game/kat.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/mixmatch.h"
#include "spycraft/game/phodoc.h"
#include "spycraft/game/image.h"
#include "spycraft/game/maptool.h"
#include "spycraft/game/html.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/maptool.h"
#include "spycraft/game/cypher.h"
#include "spycraft/game/torture.h"
#include "spycraft/game/target.h"
#include "spycraft/game/knee.h"
#include "spycraft/game/onyxhunt.h"

namespace Spycraft {

void changePage(Website *id) {
	switch (id->flag)
	{
	case WEB_HOME:
		new (Home);
		break;
	case WEB_KNEE:
		new (Knee);
		break;
	case WEB_SOUND_BIRD:
		new Sndanal(0);
		break;
	case WEB_SOUND_C8:
		new Sndanal(1);
		break;
	case WEB_SOUND_C13:
		new Sndanal(2);
		break;
	case WEB_SOUND_C21:
		new Sndanal(3);
		break;
	case WEB_SOUND_C26:
		new Sndanal(4);
		break;
	case WEB_SOUND_B6:
		new Sndanal(5);
		break;
	case WEB_SOUND_B14:
		new Sndanal(6);
		break;
	case WEB_SOUND_B14B:
		new Sndanal(7);
		break;
	case WEB_SOUND_B20:
		new Sndanal(8);
		break;
	case WEB_SOUND_M14:
		new Sndanal(9);
		break;
	case WEB_SOUND_M28:
		new Sndanal(10);
		break;
	case WEB_SOUND_S19:
		new Sndanal(11);
		break;
	case WEB_SOUND_S22:
		new Sndanal(12);
		break;
	case WEB_SOUND_VRC5:
		new Sndanal(13);
		break;
	case WEB_SOUND_VRC9:
		new Sndanal(14);
		break;
	case WEB_SOUND_VRC23:
		new Sndanal(15);
		break;
	case WEB_SOUND_VRB9:
		new Sndanal(16);
		break;
	case WEB_SOUND_VRB15:
		new Sndanal(17);
		break;
	case WEB_SOUND_VRM5:
		new Sndanal(18);
		break;
	case WEB_SOUND_VRM9:
		new Sndanal(19);
		break;
	case WEB_SOUND_VRS9:
		new Sndanal(20);
		break;
	case WEB_SOUND_VRS29:
		new Sndanal(21);
		break;
	case WEB_TARGET:
		new Target;
		break;
	case WEB_YINGCYPHER:
		new (YingCypherNote);
		break;
	case WEB_CYPHERPITONE:
		new(CypherPitOne);
		break;
	case WEB_CYPHERPITTWO:
		new(CypherPitTwo);
		break;
	case WEB_CYPHERPITTHREE:
		new(CypherPitThree);
		break;
	case WEB_CYPHERMOLEONE:
		new(CypherMoleOne);
		break;
	case WEB_CYPHERMOLETWO:
		new(CypherMoleTwo);
		break;
	case WEB_CYPHERMOLETHREE:
		new(CypherMoleThree);
		break;
	case WEB_CYPHERMOLEFOUR:
		new(CypherMoleFour);
		break;
	case WEB_CYPHEREMPTY:
		new(CypherEmpty);
		break;
	case WEB_KAT:
		new (KAT);
		break;
	case WEB_VLINKS:
		new (Vplay);
		break;
	case WEB_PHOTODOC:
		new (PhotoDoc);
		break;
	case WEB_IMAGEPLATE:
		new(ImageAnalPlate);
		break;
	case WEB_IMAGETANK:
		new(ImageAnalTank);
		break;
	case WEB_IMAGECOURIER:
		new(ImageAnalCourier);
		break;
	case WEB_IMAGEKNEEID:
		new(ImageAnalCaps);
		break;
	case WEB_IMAGEPEG:
		new(ImageAnalPeg);
		break;
	case WEB_KNEECAPSMATCH:
		new(KneeCapsMatch);
		break;
	case WEB_AMESMATCH:
		new(AmesMatch);
		break;
	case WEB_GRENDELMATCH:
		new(GrendelMatch);
		break;
	case WEB_KATMATCH:
		new KatMatch(true);
		break;
	case WEB_MAP:
		new(MapTool);
		break;
	case WEB_COM:
		new (Vplay);
		break;
	case WEB_DATA:
		new (DataPage);
		break;
	case WEB_NOTE:
		new(NotePage);
		break;
	case WEB_TRAVEL:
		new(TravelPage);
		break;
	case WEB_ONYXHUNTONE:
		new(OnyxHuntOne);
		break;
	case WEB_ONYXHUNTTWO:
		new(OnyxHuntTwo);
		break;
	case WEB_POWERBOOKONE:
		new(PowerBookOne);
		break;
	case WEB_POWERBOOKTWO:
		new(PowerBookTwo);
		break;
	case WEB_POWERBOOKTHREE:
		new(PowerBookThree);
		break;
	case WEB_POWERBOOKFOUR:
		new(PowerBookFour);
		break;
	case WEB_POWERBOOKFIVE:
		new(PowerBookFive);
		break;
	case WEB_POWERBOOKSIX:
		new(PowerBookSix);
		break;
	case WEB_POWERBOOKSEVEN:
		new(PowerBookSeven);
		break;
	case WEB_HTMLPAGE:
		new HTMLPage((char *)&id->htmlname);
		break;
	}
}

void launchPuzz(char *linkTo) {
	sc_strupr(linkTo);

	if (!strncmp((char *)linkTo, "PUZZKAT", 7))
	{
		new (KAT);
	} else if (!strncmp((char *)linkTo, "PUZZMATCHKAT", 12))
	{
		new KatMatch(0, 0);
	} else if (!strncmp((char *)linkTo, "PUZZPLATE", 9))
	{
		new(ImageAnalPlate);
	} else if (!strncmp((char *)linkTo, "PUZZTANK", 8))
	{
		new(ImageAnalTank);
	} else if (!strncmp((char *)linkTo, "PUZZAMES", 8))
	{
		new(ImageAnalCourier);
	} else if (!strncmp((char *)linkTo, "PUZZIMAGECAPS", 13))
	{
		new(ImageAnalCaps);
	} else if (!strncmp((char *)linkTo, "PUZZIMAGEPEG", 12))
	{
		new(ImageAnalPeg);
	} else if (!strncmp((char *)linkTo, "PUZZPHOTODOC", 12))
	{
		new(PhotoDoc);
	} else if (!strncmp((char *)linkTo, "PUZZHOME", 7))
	{
		new (Home);
	} else if (!strncmp((char *)linkTo, "PUZZPOWERBOOKONE", 16))
	{
		new(PowerBookOne);
	} else if (!strncmp((char *)linkTo, "PUZZPOWERBOOKTWO", 16))
	{
		new(PowerBookTwo);
	} else if (!strncmp((char *)linkTo, "PUZZPOWERBOOKTHREE", 18))
	{
		new(PowerBookThree);
	} else if (!strncmp((char *)linkTo, "PUZZPOWERBOOKFOUR", 17))
	{
		new(PowerBookFour);
	} else if (!strncmp((char *)linkTo, "PUZZPOWERBOOKFIVE", 17))
	{
		new(PowerBookFive);
	} else if (!strncmp((char *)linkTo, "PUZZPOWERBOOKSIX", 16))
	{
		new(PowerBookSix);
	} else if (!strncmp((char *)linkTo, "PUZZPOWERBOOKSEVEN", 18))
	{
		new(PowerBookSeven);
	} else if (!strncmp((char *)linkTo, "PUZZYINGNOTECYPHER", 18))
	{
		new(YingCypherNote);
	} else if (!strncmp((char *)linkTo, "PUZZCYPHERPITONE", 16))
	{
		new(CypherPitOne);
	} else if (!strncmp((char *)linkTo, "PUZZCYPHERPITTWO", 16))
	{
		new(CypherPitTwo);
	} else if (!strncmp((char *)linkTo, "PUZZCYPHERPITTHREE", 18))
	{
		new(CypherPitThree);
	} else if (!strncmp((char *)linkTo, "PUZZCYPHERMOLEONE", 17))
	{
		new(CypherMoleOne);
	} else if (!strncmp((char *)linkTo, "PUZZCYPHERMOLETWO", 17))
	{
		new(CypherMoleTwo);
	} else if (!strncmp((char *)linkTo, "PUZZCYPHERMOLETHREE", 19))
	{
		new(CypherMoleThree);
	} else if (!strncmp((char *)linkTo, "PUZZCYPHERMOLEFOUR", 18))
	{
		new(CypherMoleFour);
	} else if (!strncmp((char *)linkTo, "PUZZMAPTOOL", 11))
	{
		new(MapTool);
	} else if (!strncmp((char *)linkTo, "PUZZYING", 8))
	{
		intrpuzz->quitIntrface();
		new(Torture);
	} else if (!strncmp((char *)linkTo, "PUZZMIXC5", 9))
	{
		new GrendelMatch(99909);
	} else if (!strncmp((char *)linkTo, "PUZZMIXC9", 9))
	{
		new GrendelMatch(99910);
	} else if (!strncmp((char *)linkTo, "PUZZMIXC11", 10))
	{
		new GrendelMatch(99911);
	} else if (!strncmp((char *)linkTo, "PUZZMIXC23", 10))
	{
		new GrendelMatch(99912);
	} else if (!strncmp((char *)linkTo, "PUZZMIXC26", 10))
	{
		new GrendelMatch(99913);
	} else if (!strncmp((char *)linkTo, "PUZZMIXC28", 10))
	{
		new GrendelMatch(99914);
	} else if (!strncmp((char *)linkTo, "PUZZMIXB9", 9))
	{
		new GrendelMatch(99915);
	} else if (!strncmp((char *)linkTo, "PUZZMIXB11", 10))
	{
		new GrendelMatch(99916);
	} else if (!strncmp((char *)linkTo, "PUZZMIXB15", 10))
	{
		new GrendelMatch(99917);
	} else if (!strncmp((char *)linkTo, "PUZZMIXB20", 10))
	{
		new GrendelMatch(99918);
	} else if (!strncmp((char *)linkTo, "PUZZMIXB28", 10))
	{
		new GrendelMatch(99919);
	} else if (!strncmp((char *)linkTo, "PUZZMIXM5", 9))
	{
		new GrendelMatch(99920);
	} else if (!strncmp((char *)linkTo, "PUZZMIXM9", 9))
	{
		new GrendelMatch(99921);
	} else if (!strncmp((char *)linkTo, "PUZZMIXM28", 10))
	{
		new GrendelMatch(99922);
	} else if (!strncmp((char *)linkTo, "PUZZMIXM30", 10))
	{
		new GrendelMatch(99923);
	} else if (!strncmp((char *)linkTo, "PUZZMIXS9", 9))
	{
		new GrendelMatch(99924);
	} else if (!strncmp((char *)linkTo, "PUZZMIXS29", 10))
	{
		new GrendelMatch(99925);
	} else if (!strncmp((char *)linkTo, "PUZZSNDBIRD", 11))
	{
		new Sndanal(0);
	} else if (!strncmp((char *)linkTo, "PUZZSNDC8", 9))
	{
		new Sndanal(1);
	} else if (!strncmp((char *)linkTo, "PUZZSNDC13", 10))
	{
		new Sndanal(2);
	} else if (!strncmp((char *)linkTo, "PUZZSNDC21", 10))
	{
		new Sndanal(3);
	} else if (!strncmp((char *)linkTo, "PUZZSNDC26", 10))
	{
		new Sndanal(4);
	} else if (!strncmp((char *)linkTo, "PUZZSNDB6", 9))
	{
		new Sndanal(5);
	} else if (!strncmp((char *)linkTo, "PUZZSNDB14B", 11))
	{
		new Sndanal(7);
	} else if (!strncmp((char *)linkTo, "PUZZSNDB14", 10))
	{
		new Sndanal(6);
	} else if (!strncmp((char *)linkTo, "PUZZSNDB20", 10))
	{
		new Sndanal(8);
	} else if (!strncmp((char *)linkTo, "PUZZSNDM14", 10))
	{
		new Sndanal(9);
	} else if (!strncmp((char *)linkTo, "PUZZSNDM28", 10))
	{
		new Sndanal(10);
	} else if (!strncmp((char *)linkTo, "PUZZSNDS19", 10))
	{
		new Sndanal(11);
	} else if (!strncmp((char *)linkTo, "PUZZSNDS22", 10))
	{
		new Sndanal(12);
	} else if (!strncmp((char *)linkTo, "PUZZVRC5", 8))
	{
		new Sndanal(13);
	} else if (!strncmp((char *)linkTo, "PUZZVRC9", 8))
	{
		new Sndanal(14);
	} else if (!strncmp((char *)linkTo, "PUZZVRC23", 9))
	{
		new Sndanal(15);
	} else if (!strncmp((char *)linkTo, "PUZZVRB9", 8))
	{
		new Sndanal(16);
	} else if (!strncmp((char *)linkTo, "PUZZVRB15", 9))
	{
		new Sndanal(17);
	} else if (!strncmp((char *)linkTo, "PUZZVRM5", 8))
	{
		new Sndanal(18);
	} else if (!strncmp((char *)linkTo, "PUZZVRM9", 8))
	{
		new Sndanal(19);
	} else if (!strncmp((char *)linkTo, "PUZZVRS9", 8))
	{
		new Sndanal(20);
	} else if (!strncmp((char *)linkTo, "PUZZVRS29", 9))
	{
		new Sndanal(21);
	} else if (!strncmp((char *)linkTo, "PUZZKNEE", 8))
	{
		new Knee;
	} else if (!strncmp((char *)linkTo, "PUZZONYXHUNTONE", 15))
	{
		new OnyxHuntOne;
	} else if (!strncmp((char *)linkTo, "PUZZONYXHUNTTWO", 15))
	{
		new OnyxHuntTwo;
	} else if (!strncmp((char *)linkTo, "PUZZCLOSEYELLOW", 15))
	{
		intrpuzz->quitIntrface();
	}

}

} // namespace Spycraft
