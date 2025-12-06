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

#include "spycraft/game/onyxhunt.h"
#include "spycraft/game/email.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/html.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

extern BlinkCursor *normalCursor;
WebReport *onyxreport;

/***********************************
 *
 *		  Main MixMatch	Page
 *
 **********************************/
OnyxHuntOne::OnyxHuntOne() {
	IntrPuzzPage::init(WEB_ONYXHUNTONE);
	new OnyxBackOne;
	new BatraFeature;
	new PatelFeature;
	new MehtaFeature;
	new BulsaraFeature;
	GameFlag.set(fGreg14);
	webreport->hide();


	if (!GameFlag.test(fOnyxHunt1))
		onyxreport = new OnyxOneSendBut;
	else if (GameFlag.test(fOnceOnyx48000))
		onyxreport = new OnyxTwoSendBut;
	else
		onyxreport = new OnyxOneSendBut;

}

void OnyxHuntOne::dispose() {
	if (web_intrface)
	{
		onyxreport->dispose();
		webreport->show();
	}
	IntrPuzzPage::dispose();
}

OnyxHuntTwo::OnyxHuntTwo() {
	IntrPuzzPage::init(WEB_ONYXHUNTTWO);
	new OnyxBackTwo;
	new LocationFeature;
	new DateFeature;

	GameFlag.set(fGreg15);
	webreport->hide();

	onyxreport = new OnyxTwoSendBut;
}

void OnyxHuntTwo::dispose() {
	if (web_intrface)
	{
		onyxreport->dispose();
		webreport->show();
	}
	IntrPuzzPage::dispose();
}

/***********************************
 *
 *			Views
 *
 **********************************/
OnyxBackOne::OnyxBackOne() {
	view = 280;
	loop = 0;
	cel = 0;
	init(0, 16);
	noSkip();
	addToPic();
	dispose();
}

/***********************************
 *
 *			Hunt One Features
 **********************************/
BatraFeature::BatraFeature() {
	init(33, 105, 138, 44);
	new CommandLine(33, 105, 138, 44, "Find records in the past week for A. Batra");
}

int BatraFeature::handleEvent(MADEEventStamp *event) {
	if (user->input && onMe(event))
	{
		if (!cursorset)
		{
			theGame->setCursor(hyperCursor);
			cursorset = true;
		}
		if (event_type == USER_LEFT_DOWN)
		{
			intrpuzz->dispose();
			new HTMLPage(99320);
			return true;
		}
	} else if (cursorset)
	{
		cursorset = false;
		theGame->setCursor(normalCursor);
	}
	return false;
}

PatelFeature::PatelFeature() {
	init(33, 153, 145, 42);
	new CommandLine(33, 153, 145, 42, "Find records in the past week for S. Patel");
}

int PatelFeature::handleEvent(MADEEventStamp *event) {
	if (user->input && onMe(event))
	{
		if (!cursorset)
		{
			theGame->setCursor(hyperCursor);
			cursorset = true;
		}
		if (event_type == USER_LEFT_DOWN)
		{
			intrpuzz->dispose();
			new HTMLPage(99325);
			return true;
		}
	} else if (cursorset)
	{
		cursorset = false;
		theGame->setCursor(normalCursor);
	}
	return false;
}

MehtaFeature::MehtaFeature() {
	init(33, 204, 164, 44);
	new CommandLine(33, 204, 164, 44, "Find records in the past week for D. Mehta");
}

int MehtaFeature::handleEvent(MADEEventStamp *event) {
	if (user->input && onMe(event))
	{
		if (!cursorset)
		{
			theGame->setCursor(hyperCursor);
			cursorset = true;
		}
		if (event_type == USER_LEFT_DOWN)
		{
			intrpuzz->dispose();
			new HTMLPage(99330);
			return true;
		}
	} else if (cursorset)
	{
		cursorset = false;
		theGame->setCursor(normalCursor);
	}
	return false;
}

BulsaraFeature::BulsaraFeature() {
	init(33, 255, 165, 44);
	new CommandLine(33, 255, 165, 44, "Find records in the past week for S. Bulsara");
}

int BulsaraFeature::handleEvent(MADEEventStamp *event) {
	if (user->input && onMe(event))
	{
		if (!cursorset)
		{
			theGame->setCursor(hyperCursor);
			cursorset = true;
		}
		if (event_type == USER_LEFT_DOWN)
		{
			intrpuzz->dispose();
			new HTMLPage(99335);
			return true;
		}
	} else if (cursorset)
	{
		cursorset = false;
		theGame->setCursor(normalCursor);
	}
	return false;
}

OnyxBackTwo::OnyxBackTwo() {
	view = 290;
	loop = 0;
	cel = 0;
	init(0, 16);
	noSkip();
	addToPic();
	dispose();
}

/***********************************
 *
 *			Hunt Two Features
 **********************************/
DateFeature::DateFeature() {
	init(43, 146, 120, 37);
	new CommandLine(43, 146, 120, 37, "Display all hits by date");
}

int DateFeature::handleEvent(MADEEventStamp *event) {
	if (user->input && onMe(event))
	{
		if (!cursorset)
		{
			theGame->setCursor(hyperCursor);
			cursorset = true;
		}
		if (event_type == USER_LEFT_DOWN)
		{
			intrpuzz->dispose();
			new HTMLPage(99366);
			return true;
		}
	} else if (cursorset)
	{
		cursorset = false;
		theGame->setCursor(normalCursor);
	}
	return false;
}

LocationFeature::LocationFeature() {
	init(45, 203, 128, 37);
	new CommandLine(45, 203, 128, 37, "Display all hits by location");
}

int LocationFeature::handleEvent(MADEEventStamp *event) {
	if (user->input && onMe(event))
	{
		if (!cursorset)
		{
			theGame->setCursor(hyperCursor);
			cursorset = true;
		}
		if (event_type == USER_LEFT_DOWN)
		{
			intrpuzz->dispose();
			new HTMLPage(99367);
			return true;
		}
	} else if (cursorset)
	{
		cursorset = false;
		theGame->setCursor(normalCursor);
	}
	return false;
}

/***********************************
 *
 *			Onyx Hunt send buttons
 *
 **********************************/
OnyxOneSendBut::OnyxOneSendBut() {
	init(432, 383, 666, 6, 0);
	noSkip();
}

void OnyxOneSendBut::activate() {
	GameFlag.set(fReportOnyxOne);

	if (!GameFlag.test(fOnyxHunt1))
	{
		new EMailSend(212, this);
	} else if (!(GameFlag.test(fFullBriefing)))
		new EMailSend(229, this);
	else
		new EMailSend(230, this);
}

void OnyxOneSendBut::cue() {
	if (!GameFlag.test(fOnyxHunt1))
	{
		new VoiceMail("Onyx info: Holt, David", 98334, HOLT);
		GameFlag.set(fOnyxHunt1);  //larry wants this puzzle solved no matter what you do
	}

	if (!(webreport->flag & OBJECT_HIDDEN))
		WebReport::cue();
}


OnyxTwoSendBut::OnyxTwoSendBut() {
	init(432, 383, 666, 6, 0);
	noSkip();
}

void OnyxTwoSendBut::activate() {
	GameFlag.set(fReportOnyxTwo);

	if (!GameFlag.test(fOnyxHunt2))
	{
		new EMailSend(213, this);
	} else if (!(GameFlag.test(fFullBriefing)))
		new EMailSend(229, this);
	else
		new EMailSend(230, this);
}

void OnyxTwoSendBut::cue() {
	int fireTheJackass = false;

	if (choiceIndex[0] == 3 && choiceIndex[1] == 4)
	{
		GameFlag.set(fHeidelbergAvailable);
		GameFlag.set(fOnyxHunt2);
	} else if (!GameFlag.test(fOnyxHunt2))
	{
		if (GameFlag.test(fGreg23) && GameFlag.test(fGreg24))
		{
			fireTheJackass = true;
		} else
		{
			if (GameFlag.test(fGreg23))
			{
				GameFlag.set(fGreg24);
			} else
				GameFlag.set(fGreg23);

			if (choiceIndex[1] == 0)
			{
				if (choiceIndex[0] == 11)
					new EMail("Careless Analysis re: Onyx", 98353);
				else
					new EMail("Not enough evidence re: Onyx", 98364);
			} else if (choiceIndex[1] == 1)
			{
				if (choiceIndex[0] == 2)
					new EMail("Careless Analysis re: Onyx", 98354);
				else
					new EMail("Final destination?", 98365);
			} else if (choiceIndex[1] == 2)
			{
				if (choiceIndex[0] == 2)
					new EMail("Careless Analysis re: Onyx", 98355);
				else
					new EMail("Final destination?", 98365);
			} else if (choiceIndex[1] == 3)
			{
				if (choiceIndex[0] == 6)
					new EMail("Careless Analysis re: Onyx", 98356);
				else
					new EMail("Final destination?", 98365);
			} else if (choiceIndex[1] == 4)
			{
				new EMail("Careless Analysis re: Onyx", 98357);
			} else if (choiceIndex[1] == 5)
			{
				if (choiceIndex[0] == 0)
					new EMail("Careless Analysis re: Onyx", 98358);
				else
					new EMail("Not enough evidence re:Onyx", 98364);
			} else if (choiceIndex[1] == 6)
			{
				if (choiceIndex[0] == 4)
					new EMail("Careless Analysis re: Onyx", 98359);
				else
					new EMail("Not enough evidence re:Onyx", 98364);
			} else if (choiceIndex[1] == 7)
			{
				if (choiceIndex[0] == 2)
					new EMail("Careless Analysis re: Onyx", 98360);
				else
					new EMail("Final destination?", 98365);
			} else if (choiceIndex[1] == 8)
			{
				if (choiceIndex[0] == 8)
					new EMail("Careless Analysis re: Onyx", 98361);
				else
					new EMail("Not enough evidence re: Onyx", 98364);
			} else if (choiceIndex[1] == 9)
			{
				if (choiceIndex[0] == 10)
					new EMail("Careless Analysis re: Onyx", 98362);
				else
					new EMail("Not enough evidence re: Onyx", 98364);
			} else if (choiceIndex[1] == 10)
			{
				new EMail("Careless Analysis re: Onyx", 98363);
			}
		}
	}
	if (!(webreport->flag & OBJECT_HIDDEN))
		WebReport::cue();

	if (fireTheJackass)
	{
		comlinkshort = NULL;
		intrpuzz->quitIntrface();
		if (theInset)
			theInset->dispose();
		theGame->newRoom(new Rm60000(END_FIRED));
	}
}

} // namespace Spycraft
