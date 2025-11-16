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

#ifndef AGS2_AC_ACDIALOG_H
#define AGS2_AC_ACDIALOG_H

#include "common/scummsys.h"

namespace AGS2 {

#define GET_OPTIONS_HEIGHT {\
  needheight = 0;\
  for (ww=0;ww<numdisp;ww++) {\
    break_up_text_into_lines(areawid-(8+bullet_wid),usingfont,get_translation(dtop->optionnames[disporder[ww]]));\
    needheight += (numlines * txthit) + multiply_up_coordinate(game.options[OPT_DIALOGGAP]);\
  }\
  if (parserInput) needheight += parserInput->hit + multiply_up_coordinate(game.options[OPT_DIALOGGAP]);\
 }

int write_dialog_options(int dlgxp, int curyp, int numdisp, int mouseison, int areawid,
	int bullet_wid, int usingfont, DialogTopic *dtop, char *disporder, short *dispyp,
	int txthit, int utextcol) {
	int ww;

	for (ww = 0; ww < numdisp; ww++) {

		if ((dtop->optionflags[disporder[ww]] & DFLG_HASBEENCHOSEN) &&
			(play.read_dialog_option_colour >= 0)) {
			// 'read' colour
			wtextcolor(play.read_dialog_option_colour);
		} else {
			// 'unread' colour
			wtextcolor(playerchar->talkcolor);
		}

		if (mouseison == ww) {
			if (textcol == get_col8_lookup(utextcol))
				wtextcolor(13); // the normal colour is the same as highlight col
			else wtextcolor(utextcol);
		}

		break_up_text_into_lines(areawid - (8 + bullet_wid), usingfont, get_translation(dtop->optionnames[disporder[ww]]));
		dispyp[ww] = curyp;
		if (game.dialog_bullet > 0)
			wputblock(dlgxp, curyp, spriteset[game.dialog_bullet], 1);
		int cc;
		if (game.options[OPT_DIALOGNUMBERED]) {
			char tempbfr[20];
			int actualpicwid = 0;
			if (game.dialog_bullet > 0)
				actualpicwid = spritewidth[game.dialog_bullet] + 3;

			sprintf(tempbfr, "%d.", ww + 1);
			wouttext_outline(dlgxp + actualpicwid, curyp, usingfont, tempbfr);
		}
		for (cc = 0; cc < numlines; cc++) {
			wouttext_outline(dlgxp + ((cc == 0) ? 0 : 9) + bullet_wid, curyp, usingfont, lines[cc]);
			curyp += txthit;
		}
		if (ww < numdisp - 1)
			curyp += multiply_up_coordinate(game.options[OPT_DIALOGGAP]);
	}
	return curyp;
}

void draw_gui_for_dialog_options(GUIMain *guib, int dlgxp, int dlgyp) {
	if (guib->bgcol != 0) {
		wsetcolor(guib->bgcol);
		wbar(dlgxp, dlgyp, dlgxp + guib->wid, dlgyp + guib->hit);
	}
	if (guib->bgpic > 0)
		put_sprite_256(dlgxp, dlgyp, spriteset[guib->bgpic]);

	wsetcolor(0);
}

bool get_custom_dialog_options_dimensions(int dlgnum) {
	ccDialogOptionsRendering.Reset();
	ccDialogOptionsRendering.dialogID = dlgnum;

	getDialogOptionsDimensionsFunc.param1 = &ccDialogOptionsRendering;
	run_function_on_non_blocking_thread(&getDialogOptionsDimensionsFunc);

	if ((ccDialogOptionsRendering.width > 0) &&
		(ccDialogOptionsRendering.height > 0))
	{
		return true;
	}
	return false;
}

int show_dialog_options(int dlgnum, int sayChosenOption, bool runGameLoopsInBackground) {
	int dlgxp, dlgyp = get_fixed_pixel_size(160);
	int usingfont = FONT_NORMAL;
	int txthit = wgetfontheight(usingfont);
	int curswas = cur_cursor;
	int bullet_wid = 0, needheight;
	IDriverDependantBitmap *ddb = NULL;
	BITMAP *subBitmap = NULL;
	GUITextBox *parserInput = NULL;
	DialogTopic *dtop = NULL;

	if ((dlgnum < 0) || (dlgnum >= game.numdialog))
		quit("!RunDialog: invalid dialog number specified");

	can_run_delayed_command();

	play.in_conversation++;

	update_polled_stuff();

	if (game.dialog_bullet > 0)
		bullet_wid = spritewidth[game.dialog_bullet] + 3;

	// numbered options, leave space for the numbers
	if (game.options[OPT_DIALOGNUMBERED])
		bullet_wid += wgettextwidth_compensate("9. ", usingfont);

	said_text = 0;

	update_polled_stuff();

	block tempScrn = create_bitmap_ex(final_col_dep, screen->w, screen->h);

	set_mouse_cursor(CURS_ARROW);

	dtop = &dialog[dlgnum];

	int ww, chose = -1, numdisp = 0;

	//get_real_screen();
	wsetscreen(virtual_screen);

	char disporder[MAXTOPICOPTIONS];
	short dispyp[MAXTOPICOPTIONS];
	int parserActivated = 0;
	if ((dtop->topicFlags & DTFLG_SHOWPARSER) && (play.disable_dialog_parser == 0)) {
		parserInput = new GUITextBox();
		parserInput->hit = txthit + get_fixed_pixel_size(4);
		parserInput->exflags = 0;
		parserInput->font = usingfont;
	}

	wtexttransparent(TEXTFG);
	numdisp = 0;
	for (ww = 0; ww < dtop->numoptions; ww++) {
		if ((dtop->optionflags[ww] & DFLG_ON) == 0) continue;
		ensure_text_valid_for_font(dtop->optionnames[ww], usingfont);
		disporder[numdisp] = ww;
		numdisp++;
	}
	if (numdisp < 1) quit("!DoDialog: all options have been turned off");
	// Don't display the options if there is only one and the parser
	// is not enabled.
	if ((numdisp > 1) || (parserInput != NULL) || (play.show_single_dialog_option)) {
		wsetcolor(0); //wbar(0,dlgyp-1,scrnwid-1,dlgyp+numdisp*txthit+1);
		int areawid, is_textwindow = 0;
		int forecol = 14, savedwid;

		int mouseison = -1, curyp;
		int mousewason = -10;
		int dirtyx = 0, dirtyy = 0;
		int dirtywidth = virtual_screen->w, dirtyheight = virtual_screen->h;
		bool usingCustomRendering = false;

		dlgxp = 1;
		if (get_custom_dialog_options_dimensions(dlgnum))
		{
			usingCustomRendering = true;
			dirtyx = multiply_up_coordinate(ccDialogOptionsRendering.x);
			dirtyy = multiply_up_coordinate(ccDialogOptionsRendering.y);
			dirtywidth = multiply_up_coordinate(ccDialogOptionsRendering.width);
			dirtyheight = multiply_up_coordinate(ccDialogOptionsRendering.height);
		} else if (game.options[OPT_DIALOGIFACE] > 0)
		{
			GUIMain *guib = &guis[game.options[OPT_DIALOGIFACE]];
			if (guib->is_textwindow()) {
				// text-window, so do the QFG4-style speech options
				is_textwindow = 1;
				forecol = guib->fgcol;
			} else {
				dlgxp = guib->x;
				dlgyp = guib->y;
				draw_gui_for_dialog_options(guib, dlgxp, dlgyp);

				dirtyx = dlgxp;
				dirtyy = dlgyp;
				dirtywidth = guib->wid;
				dirtyheight = guib->hit;

				areawid = guib->wid - 5;

				GET_OPTIONS_HEIGHT

					if (game.options[OPT_DIALOGUPWARDS]) {
						// They want the options upwards from the bottom
						dlgyp = (guib->y + guib->hit) - needheight;
					}

			}
		} else {
			//dlgyp=(scrnhit-numdisp*txthit)-1;
			areawid = scrnwid - 5;
			GET_OPTIONS_HEIGHT
				dlgyp = scrnhit - needheight;
			wbar(0, dlgyp - 1, scrnwid - 1, scrnhit - 1);

			dirtyx = 0;
			dirtyy = dlgyp - 1;
			dirtywidth = scrnwid;
			dirtyheight = scrnhit - dirtyy;
		}
		if (!is_textwindow)
			areawid -= multiply_up_coordinate(play.dialog_options_x) * 2;

		int orixp = dlgxp, oriyp = dlgyp;
		int wantRefresh = 0;
		mouseison = -10;

		update_polled_stuff();
		//blit(virtual_screen, tempScrn, 0, 0, 0, 0, screen->w, screen->h);
		if (!play.mouse_cursor_hidden)
			domouse(1);
		update_polled_stuff();

redraw_options:

		wantRefresh = 1;

		if (usingCustomRendering)
		{
			tempScrn = recycle_bitmap(tempScrn, final_col_dep,
				multiply_up_coordinate(ccDialogOptionsRendering.width),
				multiply_up_coordinate(ccDialogOptionsRendering.height));
		}

		clear_to_color(tempScrn, bitmap_mask_color(tempScrn));
		wsetscreen(tempScrn);

		dlgxp = orixp;
		dlgyp = oriyp;
		// lengthy drawing to screen, so lock it for speed
		//acquire_screen();

		if (usingCustomRendering)
		{
			ccDialogOptionsRendering.surfaceToRenderTo = dialogOptionsRenderingSurface;
			ccDialogOptionsRendering.surfaceAccessed = false;
			dialogOptionsRenderingSurface->linkedBitmapOnly = tempScrn;
			dialogOptionsRenderingSurface->hasAlphaChannel = false;

			renderDialogOptionsFunc.param1 = &ccDialogOptionsRendering;
			run_function_on_non_blocking_thread(&renderDialogOptionsFunc);

			if (!ccDialogOptionsRendering.surfaceAccessed)
				quit("!dialog_options_get_dimensions was implemented, but no dialog_options_render function drew anything to the surface");

			if (parserInput)
			{
				parserInput->x = multiply_up_coordinate(ccDialogOptionsRendering.parserTextboxX);
				curyp = multiply_up_coordinate(ccDialogOptionsRendering.parserTextboxY);
				areawid = multiply_up_coordinate(ccDialogOptionsRendering.parserTextboxWidth);
				if (areawid == 0)
					areawid = tempScrn->w;
			}
		} else if (is_textwindow) {
			// text window behind the options
			areawid = multiply_up_coordinate(play.max_dialogoption_width);
			int biggest = 0;
			for (ww = 0; ww < numdisp; ww++) {
				break_up_text_into_lines(areawid - (8 + bullet_wid), usingfont, get_translation(dtop->optionnames[disporder[ww]]));
				if (longestline > biggest)
					biggest = longestline;
			}
			if (biggest < areawid - (12 + bullet_wid))
				areawid = biggest + (12 + bullet_wid);

			if (areawid < multiply_up_coordinate(play.min_dialogoption_width)) {
				areawid = multiply_up_coordinate(play.min_dialogoption_width);
				if (play.min_dialogoption_width > play.max_dialogoption_width)
					quit("!game.min_dialogoption_width is larger than game.max_dialogoption_width");
			}

			GET_OPTIONS_HEIGHT

				savedwid = areawid;
			int txoffs = 0, tyoffs = 0, yspos = scrnhit / 2 - needheight / 2;
			int xspos = scrnwid / 2 - areawid / 2;
			// shift window to the right if QG4-style full-screen pic
			if ((game.options[OPT_SPEECHTYPE] == 3) && (said_text > 0))
				xspos = (scrnwid - areawid) - get_fixed_pixel_size(10);

			// needs to draw the right text window, not the default
			push_screen();
			draw_text_window(&txoffs, &tyoffs, &xspos, &yspos, &areawid, needheight, game.options[OPT_DIALOGIFACE]);
			pop_screen();
			// snice draw_text_window incrases the width, restore it
			areawid = savedwid;
			//wnormscreen();

			dirtyx = xspos;
			dirtyy = yspos;
			dirtywidth = screenop->w;
			dirtyheight = screenop->h;

			wputblock(xspos, yspos, screenop, 1);
			wfreeblock(screenop); screenop = NULL;

			// Ignore the dialog_options_x/y offsets when using a text window
			txoffs += xspos;
			tyoffs += yspos;
			dlgyp = tyoffs;
			curyp = write_dialog_options(txoffs, tyoffs, numdisp, mouseison, areawid, bullet_wid, usingfont, dtop, disporder, dispyp, txthit, forecol);
			if (parserInput)
				parserInput->x = txoffs;
		} else {

			if (wantRefresh) {
				// redraw the black background so that anti-alias
				// fonts don't re-alias themselves
				if (game.options[OPT_DIALOGIFACE] == 0) {
					wsetcolor(16);
					wbar(0, dlgyp - 1, scrnwid - 1, scrnhit - 1);
				} else {
					GUIMain *guib = &guis[game.options[OPT_DIALOGIFACE]];
					if (!guib->is_textwindow())
						draw_gui_for_dialog_options(guib, dlgxp, dlgyp);
				}
			}

			dirtyx = 0;
			dirtywidth = scrnwid;

			if (game.options[OPT_DIALOGIFACE] > 0)
			{
				// the whole GUI area should be marked dirty in order
				// to ensure it gets drawn
				GUIMain *guib = &guis[game.options[OPT_DIALOGIFACE]];
				dirtyheight = guib->hit;
				dirtyy = dlgyp;
			} else
			{
				dirtyy = dlgyp - 1;
				dirtyheight = needheight + 1;
			}

			dlgxp += multiply_up_coordinate(play.dialog_options_x);
			dlgyp += multiply_up_coordinate(play.dialog_options_y);

			// if they use a negative dialog_options_y, make sure the
			// area gets marked as dirty
			if (dlgyp < dirtyy)
				dirtyy = dlgyp;

			//curyp = dlgyp + 1;
			curyp = dlgyp;
			curyp = write_dialog_options(dlgxp, curyp, numdisp, mouseison, areawid, bullet_wid, usingfont, dtop, disporder, dispyp, txthit, forecol);

			/*if (curyp > scrnhit) {
			  dlgyp = scrnhit - (curyp - dlgyp);
			  wbar(0,dlgyp-1,scrnwid-1,scrnhit-1);
			  goto redraw_options;
			}*/
			if (parserInput)
				parserInput->x = dlgxp;
		}

		if (parserInput) {
			// Set up the text box, if present
			parserInput->y = curyp + multiply_up_coordinate(game.options[OPT_DIALOGGAP]);
			parserInput->wid = areawid - get_fixed_pixel_size(10);
			parserInput->textcol = playerchar->talkcolor;
			if (mouseison == DLG_OPTION_PARSER)
				parserInput->textcol = forecol;

			if (game.dialog_bullet)  // the parser X will get moved in a second
				wputblock(parserInput->x, parserInput->y, spriteset[game.dialog_bullet], 1);

			parserInput->wid -= bullet_wid;
			parserInput->x += bullet_wid;

			parserInput->Draw();
			parserInput->activated = 0;
		}

		wantRefresh = 0;
		wsetscreen(virtual_screen);

		update_polled_stuff();

		subBitmap = recycle_bitmap(subBitmap, bitmap_color_depth(tempScrn), dirtywidth, dirtyheight);
		subBitmap = gfxDriver->ConvertBitmapToSupportedColourDepth(subBitmap);

		update_polled_stuff();

		if (usingCustomRendering)
		{
			blit(tempScrn, subBitmap, 0, 0, 0, 0, tempScrn->w, tempScrn->h);
			invalidate_rect(dirtyx, dirtyy, dirtyx + subBitmap->w, dirtyy + subBitmap->h);
		} else
		{
			blit(tempScrn, subBitmap, dirtyx, dirtyy, 0, 0, dirtywidth, dirtyheight);
		}

		if ((ddb != NULL) &&
			((ddb->GetWidth() != dirtywidth) ||
				(ddb->GetHeight() != dirtyheight)))
		{
			gfxDriver->DestroyDDB(ddb);
			ddb = NULL;
		}
		if (ddb == NULL)
			ddb = gfxDriver->CreateDDBFromBitmap(subBitmap, false, false);
		else
			gfxDriver->UpdateDDBFromBitmap(ddb, subBitmap, false);

		render_graphics(ddb, dirtyx, dirtyy);

		while (1) {

			if (runGameLoopsInBackground)
			{
				play.disabled_user_interface++;
				mainloop(false, ddb, dirtyx, dirtyy);
				play.disabled_user_interface--;
			} else
			{
				timerloop = 0;
				NEXT_ITERATION();

				render_graphics(ddb, dirtyx, dirtyy);

				update_polled_stuff_and_crossfade();
			}

			if (kbhit()) {
				int gkey = getch();
				if (parserInput) {
					wantRefresh = 1;
					// type into the parser 
					if ((gkey == 361) || ((gkey == ' ') && (strlen(parserInput->text) == 0))) {
						// write previous contents into textbox (F3 or Space when box is empty)
						for (unsigned int i = strlen(parserInput->text); i < strlen(play.lastParserEntry); i++) {
							parserInput->KeyPress(play.lastParserEntry[i]);
						}
						//domouse(2);
						goto redraw_options;
					} else if ((gkey >= 32) || (gkey == 13) || (gkey == 8)) {
						parserInput->KeyPress(gkey);
						if (!parserInput->activated) {
							//domouse(2);
							goto redraw_options;
						}
					}
				}
				// Allow selection of options by keyboard shortcuts
				else if ((gkey >= '1') && (gkey <= '9')) {
					gkey -= '1';
					if (gkey < numdisp) {
						chose = disporder[gkey];
						break;
					}
				}
			}
			mousewason = mouseison;
			mouseison = -1;
			if (usingCustomRendering)
			{
				if ((mousex >= dirtyx) && (mousey >= dirtyy) &&
					(mousex < dirtyx + tempScrn->w) &&
					(mousey < dirtyy + tempScrn->h))
				{
					getDialogOptionUnderCursorFunc.param1 = &ccDialogOptionsRendering;
					run_function_on_non_blocking_thread(&getDialogOptionUnderCursorFunc);

					if (!getDialogOptionUnderCursorFunc.atLeastOneImplementationExists)
						quit("!The script function dialog_options_get_active is not implemented. It must be present to use a custom dialogue system.");

					mouseison = ccDialogOptionsRendering.activeOptionID;
				} else
				{
					ccDialogOptionsRendering.activeOptionID = -1;
				}
			} else if ((mousey <= dlgyp) || (mousey > curyp));
			else {
				mouseison = numdisp - 1;
				for (ww = 0; ww < numdisp; ww++) {
					if (mousey < dispyp[ww]) {
						mouseison = ww - 1; break;
					}
				}
				if ((mouseison < 0) | (mouseison >= numdisp)) mouseison = -1;
			}

			if (parserInput != NULL) {
				int relativeMousey = mousey;
				if (usingCustomRendering)
					relativeMousey -= dirtyy;

				if ((relativeMousey > parserInput->y) &&
					(relativeMousey < parserInput->y + parserInput->hit))
					mouseison = DLG_OPTION_PARSER;

				if (parserInput->activated)
					parserActivated = 1;
			}

			int mouseButtonPressed = mgetbutton();

			if (mouseButtonPressed != NONE) {
				if (mouseison < 0)
				{
					if (usingCustomRendering)
					{
						runDialogOptionMouseClickHandlerFunc.param1 = &ccDialogOptionsRendering;
						runDialogOptionMouseClickHandlerFunc.param2 = (void *)(mouseButtonPressed + 1);
						run_function_on_non_blocking_thread(&runDialogOptionMouseClickHandlerFunc);

						if (runDialogOptionMouseClickHandlerFunc.atLeastOneImplementationExists)
							goto redraw_options;
					}
					continue;
				}
				if (mouseison == DLG_OPTION_PARSER) {
					// they clicked the text box
					parserActivated = 1;
				} else if (usingCustomRendering)
				{
					chose = mouseison;
					break;
				} else {
					chose = disporder[mouseison];
					break;
				}
			}

			if (usingCustomRendering)
			{
				int mouseWheelTurn = check_mouse_wheel();
				if (mouseWheelTurn != 0)
				{
					runDialogOptionMouseClickHandlerFunc.param1 = &ccDialogOptionsRendering;
					runDialogOptionMouseClickHandlerFunc.param2 = (void *)((mouseWheelTurn < 0) ? 9 : 8);
					run_function_on_non_blocking_thread(&runDialogOptionMouseClickHandlerFunc);

					if (runDialogOptionMouseClickHandlerFunc.atLeastOneImplementationExists)
						goto redraw_options;

					continue;
				}
			}

			if (parserActivated) {
				// They have selected a custom parser-based option
				if (parserInput->text[0] != 0) {
					chose = DLG_OPTION_PARSER;
					break;
				} else {
					parserActivated = 0;
					parserInput->activated = 0;
				}
			}
			if (mousewason != mouseison) {
				//domouse(2);
				goto redraw_options;
			}
			while ((timerloop == 0) && (play.fast_forward == 0)) {
				update_polled_stuff();
				platform->YieldCPU();
			}

		}
		if (!play.mouse_cursor_hidden)
			domouse(2);
	} else
		chose = disporder[0];  // only one choice, so select it

	while (kbhit()) getch(); // empty keyboard buffer
	//leave_real_screen();
	construct_virtual_screen(true);

	if (parserActivated)
	{
		strcpy(play.lastParserEntry, parserInput->text);
		ParseText(parserInput->text);
		chose = CHOSE_TEXTPARSER;
	}

	if (parserInput) {
		delete parserInput;
		parserInput = NULL;
	}

	if (ddb != NULL)
		gfxDriver->DestroyDDB(ddb);
	if (subBitmap != NULL)
		destroy_bitmap(subBitmap);

	set_mouse_cursor(curswas);
	// In case it's the QFG4 style dialog, remove the black screen
	play.in_conversation--;
	remove_screen_overlay(OVER_COMPLETE);

	wfreeblock(tempScrn);

	if (chose != CHOSE_TEXTPARSER)
	{
		dtop->optionflags[chose] |= DFLG_HASBEENCHOSEN;

		bool sayTheOption = false;
		if (sayChosenOption == SAYCHOSEN_YES)
		{
			sayTheOption = true;
		} else if (sayChosenOption == SAYCHOSEN_USEFLAG)
		{
			sayTheOption = ((dtop->optionflags[chose] & DFLG_NOREPEAT) == 0);
		}

		if (sayTheOption)
			DisplaySpeech(get_translation(dtop->optionnames[chose]), game.playercharacter);
	}

	return chose;
}

void do_conversation(int dlgnum) {
	EndSkippingUntilCharStops();

	int dlgnum_was = dlgnum;
	int previousTopics[MAX_TOPIC_HISTORY];
	int numPrevTopics = 0;
	DialogTopic *dtop = &dialog[dlgnum];

	// run the startup script
	int tocar = run_dialog_script(dtop, dlgnum, dtop->startupentrypoint, 0);
	if ((tocar == RUN_DIALOG_STOP_DIALOG) ||
		(tocar == RUN_DIALOG_GOTO_PREVIOUS))
	{
		// 'stop' or 'goto-previous' from first startup script
		remove_screen_overlay(OVER_COMPLETE);
		play.in_conversation--;
		return;
	} else if (tocar >= 0)
		dlgnum = tocar;

	while (dlgnum >= 0)
	{
		if (dlgnum >= game.numdialog)
			quit("!RunDialog: invalid dialog number specified");

		dtop = &dialog[dlgnum];

		if (dlgnum != dlgnum_was)
		{
			// dialog topic changed, so play the startup
			// script for the new topic
			tocar = run_dialog_script(dtop, dlgnum, dtop->startupentrypoint, 0);
			dlgnum_was = dlgnum;
			if (tocar == RUN_DIALOG_GOTO_PREVIOUS) {
				if (numPrevTopics < 1) {
					// goto-previous on first topic -- end dialog
					tocar = RUN_DIALOG_STOP_DIALOG;
				} else {
					tocar = previousTopics[numPrevTopics - 1];
					numPrevTopics--;
				}
			}
			if (tocar == RUN_DIALOG_STOP_DIALOG)
				break;
			else if (tocar >= 0) {
				// save the old topic number in the history
				if (numPrevTopics < MAX_TOPIC_HISTORY) {
					previousTopics[numPrevTopics] = dlgnum;
					numPrevTopics++;
				}
				dlgnum = tocar;
				continue;
			}
		}

		int chose = show_dialog_options(dlgnum, SAYCHOSEN_USEFLAG, (game.options[OPT_RUNGAMEDLGOPTS] != 0));

		if (chose == CHOSE_TEXTPARSER)
		{
			said_speech_line = 0;

			tocar = run_dialog_request(dlgnum);

			if (said_speech_line > 0) {
				// fix the problem with the close-up face remaining on screen
				DisableInterface();
				mainloop(); // redraw the screen to make sure it looks right
				EnableInterface();
				set_mouse_cursor(CURS_ARROW);
			}
		} else
		{
			tocar = run_dialog_script(dtop, dlgnum, dtop->entrypoints[chose], chose + 1);
		}

		if (tocar == RUN_DIALOG_GOTO_PREVIOUS) {
			if (numPrevTopics < 1) {
				tocar = RUN_DIALOG_STOP_DIALOG;
			} else {
				tocar = previousTopics[numPrevTopics - 1];
				numPrevTopics--;
			}
		}
		if (tocar == RUN_DIALOG_STOP_DIALOG) break;
		else if (tocar >= 0) {
			// save the old topic number in the history
			if (numPrevTopics < MAX_TOPIC_HISTORY) {
				previousTopics[numPrevTopics] = dlgnum;
				numPrevTopics++;
			}
			dlgnum = tocar;
		}

	}

}

} // namespace AGS2

#endif
