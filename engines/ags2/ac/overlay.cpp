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

#include "ags2/ac/overlay.h"
#include "ags2/vars.h"

namespace AGS2 {

void remove_screen_overlay_index(int cc) {
	int dd;
	if (screenover[cc].pic != NULL)
		wfreeblock(screenover[cc].pic);
	screenover[cc].pic = NULL;

	if (screenover[cc].bmp != NULL)
		gfxDriver->DestroyDDB(screenover[cc].bmp);
	screenover[cc].bmp = NULL;

	if (screenover[cc].type == OVER_COMPLETE) is_complete_overlay--;
	if (screenover[cc].type == OVER_TEXTMSG) is_text_overlay--;

	// if the script didn't actually use the Overlay* return
	// value, dispose of the pointer
	if (screenover[cc].associatedOverlayHandle)
		ccAttemptDisposeObject(screenover[cc].associatedOverlayHandle);

	numscreenover--;
	for (dd = cc; dd < numscreenover; dd++)
		screenover[dd] = screenover[dd + 1];

	// if an overlay before the sierra-style speech one is removed,
	// update the index
	if (face_talking > cc)
		face_talking--;
}

void remove_screen_overlay(int type) {
	int cc;
	for (cc = 0; cc < numscreenover; cc++) {
		if (screenover[cc].type == type);
		else if (type == -1);
		else continue;
		remove_screen_overlay_index(cc);
		cc--;
	}
}

int find_overlay_of_type(int typ) {
	int ww;
	for (ww = 0; ww < numscreenover; ww++) {
		if (screenover[ww].type == typ) return ww;
	}
	return -1;
}

int add_screen_overlay(int x, int y, int type, block piccy, bool alphaChannel) {
	if (numscreenover >= MAX_SCREEN_OVERLAYS)
		quit("too many screen overlays created");
	if (type == OVER_COMPLETE) is_complete_overlay++;
	if (type == OVER_TEXTMSG) is_text_overlay++;
	if (type == OVER_CUSTOM) {
		int uu;  // find an unused custom ID
		for (uu = OVER_CUSTOM + 1; uu < OVER_CUSTOM + 100; uu++) {
			if (find_overlay_of_type(uu) == -1) {
				type = uu; break;
			}
		}
	}
	screenover[numscreenover].pic = piccy;
	screenover[numscreenover].bmp = gfxDriver->CreateDDBFromBitmap(piccy, alphaChannel);
	screenover[numscreenover].x = x;
	screenover[numscreenover].y = y;
	screenover[numscreenover].type = type;
	screenover[numscreenover].timeout = 0;
	screenover[numscreenover].bgSpeechForChar = -1;
	screenover[numscreenover].associatedOverlayHandle = 0;
	screenover[numscreenover].hasAlphaChannel = alphaChannel;
	screenover[numscreenover].positionRelativeToScreen = true;
	numscreenover++;
	return numscreenover - 1;
}

void get_overlay_position(int overlayidx, int *x, int *y) {
	int tdxp, tdyp;

	if (screenover[overlayidx].x == OVR_AUTOPLACE) {
		// auto place on character
		int charid = screenover[overlayidx].y;
		int charpic = views[game.chars[charid].view].loops[game.chars[charid].loop].frames[0].pic;

		tdyp = multiply_up_coordinate(game.chars[charid].get_effective_y()) - offsety - 5;
		if (charextra[charid].height < 1)
			tdyp -= spriteheight[charpic];
		else
			tdyp -= charextra[charid].height;

		tdyp -= screenover[overlayidx].pic->h;
		if (tdyp < 5) tdyp = 5;
		tdxp = (multiply_up_coordinate(game.chars[charid].x) - screenover[overlayidx].pic->w / 2) - offsetx;
		if (tdxp < 0) tdxp = 0;

		if ((tdxp + screenover[overlayidx].pic->w) >= scrnwid)
			tdxp = (scrnwid - screenover[overlayidx].pic->w) - 1;
		if (game.chars[charid].room != displayed_room) {
			tdxp = scrnwid / 2 - screenover[overlayidx].pic->w / 2;
			tdyp = scrnhit / 2 - screenover[overlayidx].pic->h / 2;
		}
	} else {
		tdxp = screenover[overlayidx].x;
		tdyp = screenover[overlayidx].y;

		if (!screenover[overlayidx].positionRelativeToScreen)
		{
			tdxp -= offsetx;
			tdyp -= offsety;
		}
	}
	*x = tdxp;
	*y = tdyp;
}

// draw_screen_overlay: draws any stuff currently on top of the background,
// like a message box or popup interface
void draw_screen_overlay() {
	int gg;

	add_thing_to_draw(NULL, AGSE_PREGUIDRAW, 0, TRANS_RUN_PLUGIN, false);

	// draw overlays, except text boxes
	for (gg = 0; gg < numscreenover; gg++) {
		// complete overlay draw in non-transparent mode
		if (screenover[gg].type == OVER_COMPLETE)
			add_thing_to_draw(screenover[gg].bmp, screenover[gg].x, screenover[gg].y, TRANS_OPAQUE, false);
		else if (screenover[gg].type != OVER_TEXTMSG) {
			int tdxp, tdyp;
			get_overlay_position(gg, &tdxp, &tdyp);
			add_thing_to_draw(screenover[gg].bmp, tdxp, tdyp, 0, screenover[gg].hasAlphaChannel);
		}
	}

	// Draw GUIs - they should always be on top of overlays like
	// speech background text
	our_eip = 35;
	mouse_on_iface_button = -1;
	if (((debug_flags & DBG_NOIFACE) == 0) && (displayed_room >= 0)) {
		int aa;

		if (playerchar->activeinv >= MAX_INV) {
			quit("!The player.activeinv variable has been corrupted, probably as a result\n"
				"of an incorrect assignment in the game script.");
		}
		if (playerchar->activeinv < 1) gui_inv_pic = -1;
		else gui_inv_pic = game.invinfo[playerchar->activeinv].pic;
		/*    for (aa=0;aa<game.numgui;aa++) {
			  if (guis[aa].on<1) continue;
			  guis[aa].draw();
			  guis[aa].poll();
			  }*/
		our_eip = 37;
		if (guis_need_update) {
			block abufwas = abuf;
			guis_need_update = 0;
			for (aa = 0; aa < game.numgui; aa++) {
				if (guis[aa].on < 1) continue;
				eip_guinum = aa;
				our_eip = 370;
				clear_to_color(guibg[aa], bitmap_mask_color(guibg[aa]));
				abuf = guibg[aa];
				our_eip = 372;
				guis[aa].draw_at(0, 0);
				our_eip = 373;

				bool isAlpha = false;
				if (guis[aa].is_alpha())
				{
					isAlpha = true;

					if ((game.options[OPT_NEWGUIALPHA] == 0) && (guis[aa].bgpic > 0))
					{
						// old-style (pre-3.0.2) GUI alpha rendering
						repair_alpha_channel(guibg[aa], spriteset[guis[aa].bgpic]);
					}
				}

				if (guibgbmp[aa] != NULL)
				{
					gfxDriver->UpdateDDBFromBitmap(guibgbmp[aa], guibg[aa], isAlpha);
				} else
				{
					guibgbmp[aa] = gfxDriver->CreateDDBFromBitmap(guibg[aa], isAlpha);
				}
				our_eip = 374;
			}
			abuf = abufwas;
		}
		our_eip = 38;
		// Draw the GUIs
		for (gg = 0; gg < game.numgui; gg++) {
			aa = play.gui_draw_order[gg];
			if (guis[aa].on < 1) continue;

			// Don't draw GUI if "GUIs Turn Off When Disabled"
			if ((game.options[OPT_DISABLEOFF] == 3) &&
				(all_buttons_disabled > 0) &&
				(guis[aa].popup != POPUP_NOAUTOREM))
				continue;

			add_thing_to_draw(guibgbmp[aa], guis[aa].x, guis[aa].y, guis[aa].transparency, guis[aa].is_alpha());

			// only poll if the interface is enabled (mouseovers should not
			// work while in Wait state)
			if (IsInterfaceEnabled())
				guis[aa].poll();
		}
	}

	// draw text boxes (so that they appear over GUIs)
	for (gg = 0; gg < numscreenover; gg++)
	{
		if (screenover[gg].type == OVER_TEXTMSG)
		{
			int tdxp, tdyp;
			get_overlay_position(gg, &tdxp, &tdyp);
			add_thing_to_draw(screenover[gg].bmp, tdxp, tdyp, 0, false);
		}
	}

	our_eip = 1099;

	// *** Draw the Things To Draw List ***

	SpriteListEntry *thisThing;

	for (gg = 0; gg < thingsToDrawSize; gg++) {
		thisThing = &thingsToDrawList[gg];

		if (thisThing->bmp != NULL) {
			// mark the image's region as dirty
			invalidate_sprite(thisThing->x, thisThing->y, thisThing->bmp);
		} else if ((thisThing->transparent != TRANS_RUN_PLUGIN) &&
			(thisThing->bmp == NULL))
		{
			quit("Null pointer added to draw list");
		}

		if (thisThing->bmp != NULL)
		{
			if (thisThing->transparent <= 255)
			{
				thisThing->bmp->SetTransparency(thisThing->transparent);
			}

			gfxDriver->DrawSprite(thisThing->x, thisThing->y, thisThing->bmp);
		} else if (thisThing->transparent == TRANS_RUN_PLUGIN)
		{
			// meta entry to run the plugin hook
			gfxDriver->DrawSprite(thisThing->x, thisThing->y, NULL);
		} else
			quit("Unknown entry in draw list");
	}

	clear_draw_list();

	our_eip = 1100;


	if (display_fps)
	{
		draw_fps();
	}
	/*
	  if (channels[SCHAN_SPEECH] != NULL) {

		char tbuffer[60];
		Common::sprintf_s(tbuffer,"mpos: %d", channels[SCHAN_SPEECH]->get_pos_ms());
		write_log(tbuffer);
		int yp = scrnhit - (wgetfontheight(FONT_SPEECH) + 25 * symult);
		wtextcolor(14);
		draw_and_invalidate_text(1, yp, FONT_SPEECH,tbuffer);
	  }*/

	if (play.recording) {
		// Flash "REC" while recording
		wtextcolor(12);
		//if ((loopcounter % (frames_per_second * 2)) > frames_per_second/2) {
		char tformat[30];
		Common::sprintf_s(tformat, "REC %02d:%02d:%02d", replay_time / 3600, (replay_time % 3600) / 60, replay_time % 60);
		draw_and_invalidate_text(get_fixed_pixel_size(5), get_fixed_pixel_size(10), FONT_SPEECH, tformat);
		//}
	} else if (play.playback) {
		wtextcolor(10);
		char tformat[30];
		Common::sprintf_s(tformat, "PLAY %02d:%02d:%02d", replay_time / 3600, (replay_time % 3600) / 60, replay_time % 60);

		draw_and_invalidate_text(get_fixed_pixel_size(5), get_fixed_pixel_size(10), FONT_SPEECH, tformat);
	}

	our_eip = 1101;
}

// ** SCRIPT OVERLAY OBJECT

int ScriptOverlay::Dispose(const char *address, bool force) {
	// since the managed object is being deleted, remove the
	// reference so it doesn't try and dispose something else
	// with that handle later
	int overlayIndex = find_overlay_of_type(overlayId);
	if (overlayIndex >= 0)
	{
		screenover[overlayIndex].associatedOverlayHandle = 0;
	}

	// if this is being removed voluntarily (ie. pointer out of
	// scope) then remove the associateed overlay
	// Otherwise, it's a Restre Game or something so don't
	if ((!force) && (!isBackgroundSpeech) && (Overlay_GetValid(this)))
	{
		Remove();
	}

	delete this;
	return 1;
}

const char *ScriptOverlay::GetType() {
	return "Overlay";
}

int ScriptOverlay::Serialize(const char *address, char *buffer, int bufsize) {
	StartSerialize(buffer);
	SerializeInt(overlayId);
	SerializeInt(borderWidth);
	SerializeInt(borderHeight);
	SerializeInt(isBackgroundSpeech);
	return EndSerialize();
}

void ScriptOverlay::Unserialize(int index, const char *serializedData, int dataSize) {
	StartUnserialize(serializedData, dataSize);
	overlayId = UnserializeInt();
	borderWidth = UnserializeInt();
	borderHeight = UnserializeInt();
	isBackgroundSpeech = UnserializeInt();
	ccRegisterUnserializedObject(index, this, this);
}

void ScriptOverlay::Remove() {
	int overlayIndex = find_overlay_of_type(overlayId);
	if (overlayIndex < 0)
	{
		quit("ScriptOverlay::Remove: overlay is not there!");
	}
	remove_screen_overlay_index(overlayIndex);
	overlayId = -1;
}

ScriptOverlay::ScriptOverlay() {
	overlayId = -1;
}

// *** OVERLAY SCRIPT FUNCTINS

void RemoveOverlay(int ovrid) {
	if (find_overlay_of_type(ovrid) < 0) quit("!RemoveOverlay: invalid overlay id passed");
	remove_screen_overlay(ovrid);
}

void Overlay_Remove(ScriptOverlay *sco) {
	sco->Remove();
}

int CreateGraphicOverlay(int xx, int yy, int slott, int trans) {
	multiply_up_coordinates(&xx, &yy);

	block screeno = create_bitmap_ex(final_col_dep, spritewidth[slott], spriteheight[slott]);
	wsetscreen(screeno);
	clear_to_color(screeno, bitmap_mask_color(screeno));
	wputblock(0, 0, spriteset[slott], trans);

	bool hasAlpha = (game.spriteflags[slott] & SPF_ALPHACHANNEL) != 0;
	int nse = add_screen_overlay(xx, yy, OVER_CUSTOM, screeno, hasAlpha);

	wsetscreen(virtual_screen);
	return screenover[nse].type;
}

int CreateTextOverlayCore(int xx, int yy, int wii, int fontid, int clr, const char *tex, int allowShrink) {
	if (wii < 8) wii = scrnwid / 2;
	if (xx < 0) xx = scrnwid / 2 - wii / 2;
	if (clr == 0) clr = 16;
	int blcode = crovr_id;
	crovr_id = 2;
	return _display_main(xx, yy, wii, (char *)tex, blcode, fontid, -clr, 0, allowShrink, false);
}

int CreateTextOverlay(int xx, int yy, int wii, int fontid, int clr, char *texx, ...) {
	char displbuf[STD_BUFFER_SIZE];
	va_list ap;
	va_start(ap, texx);
	my_sprintf(displbuf, get_translation(texx), ap);
	va_end(ap);

	int allowShrink = 0;

	if (xx != OVR_AUTOPLACE) {
		multiply_up_coordinates(&xx, &yy);
		wii = multiply_up_coordinate(wii);
	} else  // allow DisplaySpeechBackground to be shrunk
		allowShrink = 1;

	return CreateTextOverlayCore(xx, yy, wii, fontid, clr, displbuf, allowShrink);
}

void SetTextOverlay(int ovrid, int xx, int yy, int wii, int fontid, int clr, char *texx, ...) {
	char displbuf[STD_BUFFER_SIZE];
	va_list ap;
	va_start(ap, texx);
	my_sprintf(displbuf, get_translation(texx), ap);
	va_end(ap);
	RemoveOverlay(ovrid);
	crovr_id = ovrid;
	if (CreateTextOverlay(xx, yy, wii, fontid, clr, displbuf) != ovrid)
		quit("SetTextOverlay internal error: inconsistent type ids");
}

void Overlay_SetText(ScriptOverlay *scover, int wii, int fontid, int clr, char *texx, ...) {
	char displbuf[STD_BUFFER_SIZE];
	va_list ap;
	va_start(ap, texx);
	my_sprintf(displbuf, get_translation(texx), ap);
	va_end(ap);

	int ovri = find_overlay_of_type(scover->overlayId);
	if (ovri < 0)
		quit("!Overlay.SetText: invalid overlay ID specified");
	int xx = divide_down_coordinate(screenover[ovri].x) - scover->borderWidth;
	int yy = divide_down_coordinate(screenover[ovri].y) - scover->borderHeight;

	RemoveOverlay(scover->overlayId);
	crovr_id = scover->overlayId;

	if (CreateTextOverlay(xx, yy, wii, fontid, clr, displbuf) != scover->overlayId)
		quit("SetTextOverlay internal error: inconsistent type ids");
}

int Overlay_GetX(ScriptOverlay *scover) {
	int ovri = find_overlay_of_type(scover->overlayId);
	if (ovri < 0)
		quit("!invalid overlay ID specified");

	int tdxp, tdyp;
	get_overlay_position(ovri, &tdxp, &tdyp);

	return divide_down_coordinate(tdxp);
}

void Overlay_SetX(ScriptOverlay *scover, int newx) {
	int ovri = find_overlay_of_type(scover->overlayId);
	if (ovri < 0)
		quit("!invalid overlay ID specified");

	screenover[ovri].x = multiply_up_coordinate(newx);
}

int Overlay_GetY(ScriptOverlay *scover) {
	int ovri = find_overlay_of_type(scover->overlayId);
	if (ovri < 0)
		quit("!invalid overlay ID specified");

	int tdxp, tdyp;
	get_overlay_position(ovri, &tdxp, &tdyp);

	return divide_down_coordinate(tdyp);
}

void Overlay_SetY(ScriptOverlay *scover, int newy) {
	int ovri = find_overlay_of_type(scover->overlayId);
	if (ovri < 0)
		quit("!invalid overlay ID specified");

	screenover[ovri].y = multiply_up_coordinate(newy);
}

void MoveOverlay(int ovrid, int newx, int newy) {
	multiply_up_coordinates(&newx, &newy);

	int ovri = find_overlay_of_type(ovrid);
	if (ovri < 0) quit("!MoveOverlay: invalid overlay ID specified");
	screenover[ovri].x = newx;
	screenover[ovri].y = newy;
}

int IsOverlayValid(int ovrid) {
	if (find_overlay_of_type(ovrid) < 0)
		return 0;

	return 1;
}

int Overlay_GetValid(ScriptOverlay *scover) {
	if (scover->overlayId == -1)
		return 0;

	if (!IsOverlayValid(scover->overlayId)) {
		scover->overlayId = -1;
		return 0;
	}

	return 1;
}


ScriptOverlay *Overlay_CreateGraphical(int x, int y, int slot, int transparent) {
	ScriptOverlay *sco = new ScriptOverlay();
	sco->overlayId = CreateGraphicOverlay(x, y, slot, transparent);
	sco->borderHeight = 0;
	sco->borderWidth = 0;
	sco->isBackgroundSpeech = 0;

	ccRegisterManagedObject(sco, sco);
	return sco;
}

ScriptOverlay *Overlay_CreateTextual(int x, int y, int width, int font, int colour, const char *text, ...) {
	ScriptOverlay *sco = new ScriptOverlay();

	char displbuf[STD_BUFFER_SIZE];
	va_list ap;
	va_start(ap, text);
	my_sprintf(displbuf, get_translation(text), ap);
	va_end(ap);

	multiply_up_coordinates(&x, &y);
	width = multiply_up_coordinate(width);

	sco->overlayId = CreateTextOverlayCore(x, y, width, font, colour, displbuf, 0);

	int ovri = find_overlay_of_type(sco->overlayId);
	sco->borderWidth = divide_down_coordinate(screenover[ovri].x - x);
	sco->borderHeight = divide_down_coordinate(screenover[ovri].y - y);
	sco->isBackgroundSpeech = 0;

	ccRegisterManagedObject(sco, sco);
	return sco;
}

int DisplaySpeechBackground(int charid, char *speel) {
	// remove any previous background speech for this character
	int cc;
	for (cc = 0; cc < numscreenover; cc++) {
		if (screenover[cc].bgSpeechForChar == charid) {
			remove_screen_overlay_index(cc);
			cc--;
		}
	}

	int ovrl = CreateTextOverlay(OVR_AUTOPLACE, charid, scrnwid / 2, FONT_SPEECH,
		-game.chars[charid].talkcolor, get_translation(speel));

	int scid = find_overlay_of_type(ovrl);
	screenover[scid].bgSpeechForChar = charid;
	screenover[scid].timeout = GetTextDisplayTime(speel, 1);
	return ovrl;
}

} // namespace AGS2
