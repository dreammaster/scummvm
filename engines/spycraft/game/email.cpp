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

#include "spycraft/game/email.h"
#include "spycraft/dmade/htm.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/knee.h"
#include "spycraft/game/sndanal.h"

namespace Spycraft {

EMailSend *emailsend;
List *choiceList;

int choiceIndex[5];
bool mailFlags[5][80];

EMailPage::EMailPage(int pagenumber) {
	name = "EmailPage";

	init(pagenumber, 64, 123);
}

EMailPage::EMailPage(int pagenumber, bool attach) {
	name = "EmailPage";
	init(pagenumber, 95, 118);
}

void EMailPage::init(int pagename, int theLeft, int theTop) {
	unsigned char *htmpage;

	LeftMargin = theLeft; //64;
	TopMargin = theTop;  //123;
	currentHFile->newFile();
	intrpuzz->add(this);

	htmpage = (unsigned char *)LoadHTM(pagename);
	currentHFile->filesize = GetHTMSize();
	memcpy(&currentHFile->pagecopy, htmpage, currentHFile->filesize - 1);
	while (currentHFile->bytesRead <= currentHFile->filesize - 1)
		currentHFile->readToken();

	UnLoadHTM();
	sfxPurgeRes(pagename, RES_HTM);
}

EMailSend::EMailSend(int pagenumber, Object *whoToCue) {
	name = "EmailSend";
	emailsend = this;
	memset(&choiceIndex, 0, 5 * sizeof(int));
	currentChoiceGroup = 0;
	currentUserGroup = 0;
	decision = false;
	caller = whoToCue;
	sendSFX = new (Sound);
	view = 402;
	loop = 0;
	cel = 0;
	DlgBox::init(20, 20);
	setPri(700);
	noSkip();

	currentHFile = new HTMLFile();
	choiceList = new List();
#if 0
	new MailOK();
	new MailCancel();
	new EMailClose();
#endif
	init(pagenumber);
}

EMailSend::EMailSend(int pagenumber, bool flags[5][80], Object *whoToCue) {
	name = "EmailSend";
	emailsend = this;
	memset(&choiceIndex, 0, 5 * sizeof(int));
	currentChoiceGroup = 0;
	currentUserGroup = 0;
	decision = false;
	caller = whoToCue;
	sendSFX = new Sound();

	view = 402;
	loop = 0;
	cel = 0;
	DlgBox::init(20, 20);
	setPri(700);
	noSkip();

	memset(&scratchFlags, 0, (5 * 80) * sizeof(bool));
	//	scratchFlags = flags;	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 80; j++) {
			scratchFlags[i][j] = flags[i][j];
		}
	}

	currentHFile = new HTMLFile();
	choiceList = new List();
#if 0
	new MailOK();
	new MailCancel();
	new EMailClose();
#endif
	init(pagenumber);
}

void EMailSend::init(int pagename) {
	//	unsigned char * htmpage;
	LeftMargin = 55;
	TopMargin = 55;
	currentPageName = pagename;

	currentHFile->newFile();
	dialogList->addToFront(this);

	user->input = false;
	if (!GameFlag.test(fDCK10))
		emailsend->sendSFX->play(41006, this);
	else
		cue();
}

void EMailSend::cue() {
	// this is to work around the sound problem
	unsigned char *htmpage;

	htmpage = (unsigned char *)LoadHTM(currentPageName);
	currentHFile->filesize = GetHTMSize();
	memcpy(&currentHFile->pagecopy, htmpage, currentHFile->filesize - 1);
	while ((currentHFile->bytesRead <= currentHFile->filesize - 1) && decision != true && !currentHFile->typing)
		currentHFile->typeToken();

	UnLoadHTM();
	sfxPurgeRes(currentPageName, RES_HTM);

	user->input = true;
}

int EMailSend::handleEvent(MADEEventStamp *event) {
	if (decision == true) {
		choiceList->handleEvent(event);
		return true;
	}

	return currentHFile->hyperList->handleEvent(event);
}

void EMailSend::showChoices(char choices[40][80], int numChoices, bool doCondition) {
	ChoiceFeature *choice;
	int maxHeight = 0;
	int maxWidth = 0;
	int rowWidth = 0;
	int hilightWidth = 0;
	int theX = currentHFile->theX + 15;
	int theY = currentHFile->theY + 15;
	int index = 0;
	bool showChoice = false;
	bool choicesAvailable = false;	// could be temp ask about this

	for (int i = 0; i < numChoices; i++) {
		if (doCondition) {
			if (mailFlags[currentUserGroup][i])
				showChoice = true;
			else
				showChoice = false;
		} else {
			showChoice = true;
		}

		if (showChoice) {
			choice = new ChoiceFeature();
			choice->init(theX, theY, choices[i], index);
			theY += 15;

			maxHeight += 15;

			if ((choice->myPrint->width + rowWidth) > maxWidth)
				maxWidth = choice->myPrint->width + rowWidth;

			if ((choice->myPrint->width) > hilightWidth)
				hilightWidth = choice->myPrint->width;

			choicesAvailable = true;
		}

		index++;
		if (theY > 375) {
			theX += maxWidth + 15;
			theY = currentHFile->theY + 15;
			rowWidth = (maxWidth * 2) + 15;
		}
	}

	if (doCondition)
		currentUserGroup++;

	if (!choicesAvailable)
		return;

	decision = true;

	int scaleX, scaleY;
	float scale = (float)512 / (float)16;

	maxWidth += 20;
	maxHeight += 20;
	if (maxWidth < 60)
	{
		scaleX = int((float)60 * scale);
		maxWidth = 60;
	} else {
		scaleX = int((float)maxWidth * scale);
	}

	if (maxHeight < 60) {
		scaleY = int((float)60 * scale);
		maxHeight = 60;
	} else {
		scaleY = int((float)maxHeight * scale);
	}

	choicebar = new ChoiceBar(hilightWidth);
	choiceview = new ChoiceBox(currentHFile->theX, currentHFile->theY, scaleX, scaleY);
	hBordTop = new Border(currentHFile->theX, currentHFile->theY, scaleX, 512, 2);
	hBordBottom = new Border(currentHFile->theX, currentHFile->theY + maxHeight, scaleX, 512, 2);
	vBordLeft = new Border(currentHFile->theX, currentHFile->theY, 512, scaleY, 1);
	vBordRight = new Border(currentHFile->theX + (maxWidth - 2), currentHFile->theY, 512, scaleY, 1);
}

ChoiceBar::ChoiceBar(int maxWidth) {
	view = 402;
	loop = 4;
	cel = 0;
	init(64, 123);
	ratioX = (float)512 / (float)16;
	ratioY = (float)512 / (float)14;

	SRect theRect;
	Sprite *temp;
	SRect_Init(&theRect, 0, 0, right - left, bottom - top);
	int theScaleX = int(ratioX * (float)maxWidth);
	int theScaleY = int(ratioY * (bottom - top - 1));
	sfxSetDrawInfo(ADV_DRAW_SCALEX, theScaleX);
	sfxSetDrawInfo(ADV_DRAW_SCALEY, theScaleY);
	temp = sfxClipSprite(mySprite, left, top, &theRect);
	sfxKillSprite(mySprite);
	mySprite = temp;
	setPri(750);
	sfxSpriteGlass(mySprite, 2, MK_COLOR16(82, 181, 173), 50);
	hide();
}

void EMailSend::dispose() {
	if (currentHFile) {
		currentHFile->dispose();
		delete currentHFile;
		currentHFile = NULL;
	}
	sendSFX->dispose();

	if (choiceList) {
		choiceList->dispose();
		delete choiceList;
		choiceList = NULL;
	}

	if (dialogList) {
		if (!dialogList->isEmpty())
			dialogList->dispose();

		delete dialogList;
		dialogList = NULL;
	}

	if (intrpuzz)
		intrpuzz->puzzle_focus = 0;
}

void ChoiceFeature::init(int theX, int theY, char *choiceName, int idNum) {
	id = idNum;
	left = theX;
	top = theY;

	myPrint = new Print();
	myPrint->x = theX;
	myPrint->y = theY;
	myPrint->size = 8;
	myPrint->priority = 720;

	myPrint->fore_color = TEXT_COLOR_RED;

	myPrint->init(choiceName);
	right = left + myPrint->width;
	bottom = top + myPrint->height;
	choiceList->addToFront(this);
}

ChoiceFeature::~ChoiceFeature() {
	if (hotspot) {
		setHotspot(NULL);
		hotspot = NULL;
	}

	if (myPrint) {
		myPrint->dispose();
		myPrint = NULL;
	}
}

int ChoiceFeature::handleEvent(MADEEventStamp *event) {
	if (user->input) {
		if (onMe(event)) {
			if (!hilit) {
				emailsend->choicebar->show();
				emailsend->choicebar->posn(left, top);
				hilit = true;
			}

			if (event_type == USER_LEFT_UP) {
				char tempStr[80];

				choiceIndex[emailsend->currentChoiceGroup] = id;
				emailsend->currentChoiceGroup++;
				currentHFile->currentAction = 0; //this equals text_token
				Common::strcpy_s(tempStr, myPrint->myString);
				new HyperPrint((unsigned char *)tempStr);

				choiceList->dispose();
				choiceList = new List();
				emailsend->choiceview->dispose();
				emailsend->hBordTop->dispose();
				emailsend->hBordBottom->dispose();
				emailsend->vBordLeft->dispose();
				emailsend->vBordRight->dispose();
				emailsend->choicebar->dispose();
				emailsend->decision = false;

				while ((currentHFile->bytesRead <= currentHFile->filesize) && (emailsend->decision != true) && !currentHFile->typing)
					currentHFile->typeToken();
				return true;
			}
		} else if (hilit) {
			hilit = false;
		}
	}

	return false;
}

ChoiceBox::ChoiceBox(int theX, int theY, int scaleWidth, int scaleHeight) {
	view = 402;
	loop = 1;
	cel = 0;
	x = theX;
	y = theY;
	draw();
	noSkip();
	setPri(708);
	sfxSpriteScale(mySprite, scaleWidth, scaleHeight);

	dialogList->addToFront(this);
}

Border::Border(int theX, int theY, int scaleWidth, int scaleHeight, int theCel) {
	view = 402;
	loop = 1;
	cel = theCel;
	x = theX;
	y = theY;
	draw();
	noSkip();
	setPri(709);
	sfxSpriteScale(mySprite, scaleWidth, scaleHeight);

	dialogList->addToFront(this);
}

MailOK::MailOK() {
	view = 402;
	loop = 2;
	cel = 0;
	init(108, 230);
	setPri(702);
	noSkip();
}

int MailOK::handleEvent(MADEEventStamp *event) {
	if (!emailsend->decision && !currentHFile->typing)
		return DialogButton::handleEvent(event);
	return false;
}

void MailOK::activate() {
	if (!GameFlag.test(fDCK10))
	{
		user->input = false;
		emailsend->sendSFX->play(41007, this);
	} else {
		cue();
	}
}

void MailOK::cue() {
	Object *mailCaller = emailsend->caller;

	emailsend->dispose();
	if (mailCaller) {
		mailCaller->cue();
	}
	user->input = true;
}

int MailCancel::handleEvent(MADEEventStamp *event) {
	if (!emailsend->decision && !currentHFile->typing)
		return DialogButton::handleEvent(event);
	return false;
}

MailCancel::MailCancel() {
	view = 402;
	loop = 3;
	cel = 0;
	init(199, 230);
	noSkip();
	setPri(702);
}

void MailCancel::activate() {
	if (!GameFlag.test(fDCK10)) {
		user->input = false;
		emailsend->sendSFX->play(41008, this);
	} else {
		cue();
	}
}

void MailCancel::cue() {
	emailsend->dispose();

	if (!intrpuzz->page->site)
	{
		Website *id = webmap->findPage();
		if (id)
		{
			//let the game know a button was used
			usedbutton = true;
			//allow forward button to operate
	//		fwdbutton = true;
			intrpuzz->dispose();
			changePage(id);
		}
	}
	webreport->dispose();
	webreport = new WebReport;

	switch (intrpuzz->page->site->flag) {
	case WEB_SOUND_C8:
	case WEB_SOUND_C13:
	case WEB_SOUND_C21:
	case WEB_SOUND_C26:
	case WEB_SOUND_B6:
	case WEB_SOUND_B14:
	case WEB_SOUND_B14B:
	case WEB_SOUND_B20:
	case WEB_SOUND_M14:
	case WEB_SOUND_M28:
	case WEB_SOUND_S19:
	case WEB_SOUND_S22:
	case WEB_SOUND_VRC5:
	case WEB_SOUND_VRC9:
	case WEB_SOUND_VRC23:
	case WEB_SOUND_VRB9:
	case WEB_SOUND_VRB15:
	case WEB_SOUND_VRM5:
	case WEB_SOUND_VRM9:
	case WEB_SOUND_VRS9:
	case WEB_SOUND_VRS29:
	case WEB_GRENDELMATCH:
	case WEB_YINGCYPHER:
	case WEB_IMAGEPLATE:
	case WEB_IMAGETANK:
	case WEB_IMAGEPEG:
	case WEB_KAT:
	case WEB_KATMATCH:
	case WEB_ONYXHUNTONE:
	case WEB_ONYXHUNTTWO:
	case WEB_MAP:
	case WEB_SOUND_BIRD:
	case WEB_KNEE:
	case WEB_IMAGEKNEEID:
		webreport->hide();
		break;
	}

	if (GameFlag.test(fDCK10)) {
		if (lasttool == WEB_KNEE)
			intrpuzz->setScript(new KneeDrawScript);
		else
			intrpuzz->setScript(new RedrawScript);
	}
	user->input = true;
}

EMailClose::EMailClose() {
	view = 402;
	loop = 5;
	cel = 0;
	init(290, 9);
	noSkip();
	setPri(702);
}

int EMailClose::handleEvent(MADEEventStamp *event) {
	if (!currentHFile->typing)
		return DialogButton::handleEvent(event);
	return false;
}

void EMailClose::activate() {
	if (!GameFlag.test(fDCK10)) {
		user->input = false;
		emailsend->sendSFX->play(41008, this);
	} else {
		cue();
	}
}

void EMailClose::cue() {
	emailsend->dispose();
	if (!intrpuzz->page->site) {
		Website *id = webmap->findPage();
		if (id) {
			//let the game know a button was used
			usedbutton = true;
			//allow forward button to operate
	//		fwdbutton = true;
			intrpuzz->dispose();
			changePage(id);
		}
	}
	webreport->dispose();
	webreport = new WebReport;

	switch (intrpuzz->page->site->flag) {
	case WEB_SOUND_C8:
	case WEB_SOUND_C13:
	case WEB_SOUND_C21:
	case WEB_SOUND_C26:
	case WEB_SOUND_B6:
	case WEB_SOUND_B14:
	case WEB_SOUND_B14B:
	case WEB_SOUND_B20:
	case WEB_SOUND_M14:
	case WEB_SOUND_M28:
	case WEB_SOUND_S19:
	case WEB_SOUND_S22:
	case WEB_SOUND_VRC5:
	case WEB_SOUND_VRC9:
	case WEB_SOUND_VRC23:
	case WEB_SOUND_VRB9:
	case WEB_SOUND_VRB15:
	case WEB_SOUND_VRM5:
	case WEB_SOUND_VRM9:
	case WEB_SOUND_VRS9:
	case WEB_SOUND_VRS29:
	case WEB_GRENDELMATCH:
	case WEB_YINGCYPHER:
	case WEB_IMAGEPLATE:
	case WEB_IMAGETANK:
	case WEB_IMAGEPEG:
	case WEB_KAT:
	case WEB_KATMATCH:
	case WEB_ONYXHUNTONE:
	case WEB_ONYXHUNTTWO:
	case WEB_MAP:
	case WEB_SOUND_BIRD:
	case WEB_KNEE:
	case WEB_IMAGEKNEEID:
		webreport->hide();
		break;
	}

	if (GameFlag.test(fDCK10)) {
		if (lasttool == WEB_KNEE)
			intrpuzz->setScript(new KneeDrawScript);
		else
			intrpuzz->setScript(new RedrawScript);
	}
	user->input = true;
}

} // namespace Spycraft
