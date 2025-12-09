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

#include "spycraft/game/user.h"
#include "spycraft/game/actor.h"

namespace Spycraft {

class HelpView : public View {
public:
	void init(int x, int y);
	void dispose();
};

void HelpView::init(int theX, int theY) {
	x = theX;
	y = theY;
	draw();
	myPrint = new(Print);
	myPrint->x = theX + 7;
	myPrint->y = theY + 5;
	myPrint->size = 12;
	myPrint->fore_color = TEXT_COLOR_BLACK;
	myPrint->init(name);
}

void HelpView::dispose() {
	deathList->add(this);
}

Help::Help(char *theText, Feature *theClient) {
	myText = theText;
	client = theClient;
	waitTime = 100;
	myTime = 0;
	active = false;
}

void Help::doit() {
	if ((!active) && (client->onMe(mouse_x, mouse_y))) {
		if (!event_type) {
			if (!myTime)
				myTime = gameTime;

			if (myTime && (gameTime - myTime > waitTime)) {
				active = true;
				myView = new HelpView();
				myView->setView(942);
				myView->name = myText;
				myView->init(client->right - 20, client->bottom - 20);
				myView->setPri(500);

				//sfxPrintf("help:%s", myText);
				myTime = 0;
			}
		} else if (myView) {
			myView->dispose();
			myView = NULL;
			active = false;
		}
	} else {
		if ((myView) && (!client->onMe(mouse_x, mouse_y))) {
			myView->dispose();
			myView = NULL;
			active = false;
		}
		myTime = 0;
	}
}

} // namespace Spycraft
