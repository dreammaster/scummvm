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

#include "common/scummsys.h"
#include "dink/update_frame.h"
#include "dink/freedink.h"
#include "dink/events.h"
#include "dink/globals.h"
#include "dink/graphics.h"
#include "dink/sound.h"
#include "dink/text.h"
#include "dink/var.h"
#include "dink/lib/rect.h"
#include "dink/lib/wintypes.h"
#include "dink/directdraw/ddraw.h"

namespace Dink {

#define FALSE false
#define TRUE true

void updateFrame() {
	byte state[256];
	RECT rcRect, boxCrap, boxReal;
	DDBLTFX ddBltFx;
	char msg[100];
	HDC hdc;
	HRESULT ddrVal;
	int move_result;
	bool get_frame = false;
	int h, max_s, j;
	bool bs[max_sprites_at_once];
	int rank[max_sprites_at_once];
	int highest_sprite;

	abort_this_flip = false;

	if (5 > 9) {
trigger_start:
		trig_man = false;
		get_frame = true;
	}

	check_joystick();

	//redink1 Only accept keyboard input when console is not active.
	if (mConsoleActive == false) {
		if ((GetKeyboard(77)) && (GetKeyboard(18))) {
			//shutdown music
			StopMidi();
			return;
		}

		if ((GetKeyboard(68)) && (GetKeyboard(18))) {
			if (debug_mode) {
				debug_mode = false;
			} else {
				strcpy(last_debug, "");
				debug_mode = true;
			}
		}
	}

	if ((GetKeyboard(81)) && (GetKeyboard(18))) {
		//shutdown game
		//  PostMessage(hWndMain, WM_CLOSE, 0, 0);
		finiObjects();
		return;
	}

	if (mode == 1) {
		Scrawl_OnMouseInput();
	} else {
		if (keep_mouse) {
			if ((talk.active) || (spr[1].brain == 13))
				Scrawl_OnMouseInput();
		}
	}

demon:

	/*  while(GetTickCount() < (lastTickCount+ 20))
	{
	//wait
	}
	*/

	lastTickCount = thisTickCount;
	thisTickCount = GetTickCount();

	//Use to test at 30 fps
	//Sleep(66);

	//redink1 - 'lock the framerate to 83 FPS'... Seth told me to.
	while (thisTickCount - lastTickCount < 12) {
		Sleep(1);
		thisTickCount = GetTickCount();
	}

	fps_final = thisTickCount - lastTickCount;

	//redink1 changed to 12-12 from 10-15... maybe work better on faster computers?
	if (fps_final < 12) fps_final = 12;
	if (fps_final > 68) fps_final = 68;

	base_timing = fps_final / 3;
	if (base_timing < 4) base_timing = 4;
	if (fps_final > 1) {
		fps_show = (1000 / fps_final);
		fps_average = ((fps_average + fps_final) / 2);
	}
	int junk3;

	//redink1 added these changes to set Dink's speed correctly, even on fast machines.
	//junk3 = (fps_average / dinkspeed) -  (fps_average / 8);
	if (dinkspeed <= 0)
		junk3 = 0;
	else if (dinkspeed == 1)
		junk3 = 12;
	else if (dinkspeed == 2)
		junk3 = 6;
	else if (dinkspeed == 3)
		junk3 = 3;
	else
		junk3 = 1;

	//redink1... weird bug, only do this for normal mode, as it breaks mouse support
	//if (mode == 2)
	{
		junk3 *= (base_timing / 4);
	}

	spr[1].speed = junk3;


	if (debug_mode) if (!no_cheat) {
		spr[1].speed = 20;
	}
	//junk3;

#ifdef __DEMO
	//cd_inserted = true;
	regcode = mycode;

#endif

	//force free mode
	//if (!no_cheat)

	{
		//cd_inserted = true;
		regcode = mycode;
		sound_support = mcc;
	}

	if (mLoopMidi && !nothing_playing() && strlen(last_midi) > 0) {
		PlayMidi(last_midi);
	}

	if (showb.active) {
		//grab main loop and divert it to show a bmp instead
		process_show_bmp();
		flip_it();
		return;
	}
	/*
	if (!cd_inserted)
	{
	getCDTrackStartTimes();
	}
	*/

	/*
	if (!cd_inserted)
	{
	if (exist("tiles\\cd.bmp"))
	show_bmp("tiles\\cd.bmp", false, 0, 0);
	else
	//look in alt path for CD nag
	show_bmp("..\\dink\\tiles\\cd.bmp", false, 0, 0);
	return;
	}
	*/

	mbase_count++;

	if (thisTickCount > (int)mold + 100) {
		mbase_timing = (mbase_count / 100);
		mold = thisTickCount;
		if (bow.active) bow.hitme = true;
		if (*pupdate_status == 1) update_status_all();


		update_sound();
		process_animated_tiles();
	}

	state[1] = 0;

	//figure out frame rate
	drawthistime = true;

	if (item_screen) {
		process_item();
		return;
	}

	if (total_trigger) {
		if (transition()) goto flip;
		else return;
	}

	if (process_upcycle)
		up_cycle();

	if (process_warp > 0)
		process_warp_man();
	if (process_downcycle)
		CyclePalette();


	if (plane_process) {
		memset(&bs, 0, sizeof(bs));
		max_s = last_sprite_created;
		int height;

		for (int r1 = 1; r1 < max_s + 1; r1++) {
			highest_sprite = 22024; //more than it could ever be

			rank[r1] = 0;

			for (int h1 = 1; h1 < max_s + 1; h1++) {
				if (spr[h1].active) if (spr[h1].disabled == false) {
					if (bs[h1] == FALSE) {
						//Msg( "Ok,  %d is %d", h1,(spr[h1].y + picInfo[spr[h1].pic].yoffset) );
						if (spr[h1].que != 0) height = spr[h1].que;
						else height = spr[h1].y;
						if (height < highest_sprite) {
							highest_sprite = height;
							rank[r1] = h1;
						}
					}
				}
			}

			if (rank[r1] != 0)
				bs[rank[r1]] = TRUE;
		}
	} else {
		//not processing planes
		max_s = max_sprites_at_once;
	}

	rcRect.left = 0;
	rcRect.top = 0;
	rcRect.right = currX;
	rcRect.bottom = currY;

	{
		//Blit from Two, which holds the base scene.
		while (1) {
			ddrVal = lpDDSBack->BltFast(0, 0, lpDDSTwo,
				&rcRect, DDBLTFAST_NOCOLORKEY);

			if (ddrVal == DD_OK) {
				break;
			}
			if (ddrVal == DDERR_SURFACELOST) {
				ddrVal = restoreAll();
				if (ddrVal != DD_OK) {
					//    return;
				}

				goto demon;
			}
			if (ddrVal != DDERR_WASSTILLDRAWING) {
				dderror(ddrVal);
				return;
			}
		}
	}

	if (stop_entire_game == 1) {
		if (talk.active) {
			process_talk();
		} else {
			stop_entire_game = 0;

			draw_map_game_background();
			draw_status_all();
		}
		goto flip;
	}

	for (j = 1; j < max_s + 1; j++) {
		//h  = 1;
		if (plane_process)
			h = rank[j];
		else h = j;
		//Msg( "Ok, rank %d is %d", j,h);

		if (h > 0)
			if (spr[h].active) {

				//check_sprite_status_full(h);

				spr[h].moveman = 0; //init thing that keeps track of moving path
				spr[h].lpx[0] = spr[h].x;
				spr[h].lpy[0] = spr[h].y; //last known legal cords

				spr[h].skiptimer++;
				//inc delay, used by "skip" by all sprites
				boxCrap = picInfo[getpic(h)].box;
				if (spr[h].kill > 0) {
					if (spr[h].kill_timer == 0) spr[h].kill_timer = thisTickCount;
					if (spr[h].kill_timer + spr[h].kill < thisTickCount) {

						spr[h].active = false;
						//          Msg("Killing sprite %d.", h);

						get_last_sprite();
						if (spr[h].callback > 0) {
							//  Msg("Callback running script %d.", spr[h].script);

							run_script(spr[h].callback);
						}
					}
				}

				if (spr[h].timer > 0) {
					if (thisTickCount > (int)spr[h].wait) {
						spr[h].wait = thisTickCount + spr[h].timer;

					} else {
						goto animate;
					}
				}

				//brains - predefined bahavior patterns available to any sprite

				if (spr[h].notouch)
					if (thisTickCount > (int)spr[h].notouch_timer)
						spr[h].notouch = false;

				if (get_frame == false) {
					if ((spr[h].brain == 1)/* || (spr[h].brain == 9) || (spr[h].brain == 3) */) {
						run_through_touch_damage_list(h);
					}

					if (spr[h].brain == 1) {
						if (process_warp == 0)
							human_brain(h);
					}

					if (spr[h].brain == 2) bounce_brain(h);
					if (spr[h].brain == 0) no_brain(h);
					if (spr[h].brain == 3) duck_brain(h);
					if (spr[h].brain == 4) pig_brain(h);
					if (spr[h].brain == 5) one_time_brain(h);
					if (spr[h].brain == 6) repeat_brain(h);
					if (spr[h].brain == 7) one_time_brain_for_real(h);
					if (spr[h].brain == 8) text_brain(h);
					if (spr[h].brain == 9) pill_brain(h);
					if (spr[h].brain == 10) dragon_brain(h);
					if (spr[h].brain == 11) missile_brain(h, true);
					if (spr[h].brain == 12) scale_brain(h);
					if (spr[h].brain == 13) mouse_brain(h);
					if (spr[h].brain == 14) button_brain(h);
					if (spr[h].brain == 15) shadow_brain(h);
					if (spr[h].brain == 16) people_brain(h);
					if (spr[h].brain == 17) missile_brain_expire(h);
				} else {
					goto past;
				}
				if (g_b_kill_app)
					return;
animate:
				move_result = check_if_move_is_legal(h);

				if (flub_mode != -500) {
					Msg("move result is %d", flub_mode);
					move_result = flub_mode;
					flub_mode = -500;
				}

				if (spr[h].brain == 1) if (move_result > 100) {
					if (pam.sprite[move_result - 100].prop == 1)
						special_block(move_result - 100, h);
				}

				if (spr[h].reverse) {
					//reverse instructions
					if (spr[h].seq > 0) {
						if (spr[h].frame < 1) {
							// new anim
							spr[h].pseq = spr[h].seq;
							spr[h].pframe = index[spr[h].seq].last;
							spr[h].frame = index[spr[h].seq].last;
							if (spr[h].frame_delay != 0) spr[h].delay = (thisTickCount + spr[h].frame_delay);
							else
								spr[h].delay = (thisTickCount + seq[spr[h].seq].delay[index[spr[h].seq].last]);
						} else {
							// not new anim

							//is it time?
							if (thisTickCount > (int)spr[h].delay) {
								spr[h].frame--;

								if (spr[h].frame_delay != 0) spr[h].delay = (thisTickCount + spr[h].frame_delay);
								else
									spr[h].delay = (thisTickCount + seq[spr[h].seq].delay[spr[h].frame]);

								spr[h].pseq = spr[h].seq;
								spr[h].pframe = spr[h].frame;

								if (seq[spr[h].seq].frame[spr[h].frame] < 2) {
									spr[h].pseq = spr[h].seq;
									spr[h].pframe = spr[h].frame + 1;

									spr[h].frame = 0;
									spr[h].seq_orig = spr[h].seq;
									spr[h].seq = 0;
									spr[h].nocontrol = false;

									if (h == 1) if (in_this_base(spr[h].seq_orig, mDinkBasePush)) {
										play.push_active = false;
										if (play.push_dir == 2) if (sjoy.down) play.push_active = true;
										if (play.push_dir == 4) if (sjoy.left) play.push_active = true;
										if (play.push_dir == 6) if (sjoy.right) play.push_active = true;
										if (play.push_dir == 8) if (sjoy.up) play.push_active = true;

										goto past;
									}
								}

								if (spr[h].seq > 0) if (seq[spr[h].seq].special[spr[h].frame] == 1) {
									//this sprite can damage others right now!
									//lets run through the list and tag sprites who were hit with their damage

									run_through_tag_list(h, spr[h].strength);
								}
							}
						}
					}
				} else {
					if (spr[h].seq > 0) if (spr[h].picfreeze == 0) {
						if (spr[h].frame < 1) {
							// new anim
							spr[h].pseq = spr[h].seq;
							spr[h].pframe = 1;
							spr[h].frame = 1;
							if (spr[h].frame_delay != 0)
								spr[h].delay = thisTickCount + spr[h].frame_delay;
							else
								spr[h].delay = (thisTickCount + seq[spr[h].seq].delay[1]);
						} else {
							// not new anim

							//is it time?

							if (thisTickCount > (int)spr[h].delay) {
								spr[h].frame++;
								if (spr[h].frame_delay != 0) spr[h].delay = thisTickCount + spr[h].frame_delay;
								else
									spr[h].delay = (thisTickCount + seq[spr[h].seq].delay[spr[h].frame]);

								spr[h].pseq = spr[h].seq;
								spr[h].pframe = spr[h].frame;

								if (seq[spr[h].seq].frame[spr[h].frame] == -1) {
									spr[h].frame = 1;
									spr[h].pseq = spr[h].seq;
									spr[h].pframe = spr[h].frame;
									if (spr[h].frame_delay != 0) spr[h].delay = thisTickCount + spr[h].frame_delay;
									else
										spr[h].delay = (thisTickCount + seq[spr[h].seq].delay[spr[h].frame]);
								}

								if (seq[spr[h].seq].frame[spr[h].frame] < 1) {
									spr[h].pseq = spr[h].seq;
									spr[h].pframe = spr[h].frame - 1;

									spr[h].frame = 0;
									spr[h].seq_orig = spr[h].seq;
									spr[h].seq = 0;
									spr[h].nocontrol = false;

									if (h == 1) if (in_this_base(spr[h].seq_orig, mDinkBasePush)) {
										play.push_active = false;
										if (play.push_dir == 2) if (sjoy.down) play.push_active = true;
										if (play.push_dir == 4) if (sjoy.left) play.push_active = true;
										if (play.push_dir == 6) if (sjoy.right) play.push_active = true;
										if (play.push_dir == 8) if (sjoy.up) play.push_active = true;

										goto past;
									}
								}

								if (spr[h].seq > 0) if (seq[spr[h].seq].special[spr[h].frame] == 1) {
									//this sprite can damage others right now!
									//lets run through the list and tag sprites who were hit with their damage
									run_through_tag_list(h, spr[h].strength);
								}
							}
						}
					}
				}

				if (spr[h].active) if (spr[h].brain == 1) {
					did_player_cross_screen(true, h);
					// if (move_gonna) goto past;
				}

past:
				check_seq_status(spr[h].seq);

				if (mcc == sound_support)
					draw_sprite_game(lpDDSBack, h);

				move_gonna = false;

				//draw a dot to show where the computer is guessing the start of the shadow is

				if (show_dot) {
					ddBltFx.dwSize = sizeof(ddBltFx);
					if (drawthistime) {
						ddBltFx.dwFillColor = 1;

						boxCrap = picInfo[getpic(h)].hardbox;
						//boxCrap.bottom = spr[h].y + picInfo[spr[h].pic].hardbox.bottom;
						//boxCrap.left = spr[h].x + picInfo[spr[h].pic].hardbox.left;
						//boxCrap.right = spr[h].x + picInfo[spr[h].pic].hardbox.right;

						//OffsetRect(&boxCrap, spr[h].x, spr[h].y);

						//  ddrVal = lpDDSBack->Blt(&boxCrap ,NULL, &boxReal, DDBLT_COLORFILL| DDBLT_WAIT, &ddBltFx);


						//to show center pixel
						ddBltFx.dwFillColor = 100;

						boxCrap.top = spr[h].y;
						boxCrap.bottom = spr[h].y + 1;
						boxCrap.left = spr[h].x;
						boxCrap.right = spr[h].x + 1;

						ddrVal = lpDDSBack->Blt(&boxCrap, NULL, &boxReal, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);

						for (int oo = 0; oo < spr[h].moveman + 1; oo++) {
							ddBltFx.dwFillColor = 50;

							boxCrap.top = spr[h].lpy[oo];
							boxCrap.bottom = boxCrap.top + 1;
							boxCrap.left = spr[h].lpx[oo];
							boxCrap.right = boxCrap.left + 1;


							ddrVal = lpDDSBack->Blt(&boxCrap, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);

						}
						ddBltFx.dwFillColor = 1;

						boxCrap.top = spr[h].lpy[0];
						boxCrap.bottom = boxCrap.top + 1;
						boxCrap.left = spr[h].lpx[0];
						boxCrap.right = boxCrap.left + 1;

						ddrVal = lpDDSBack->Blt(&boxCrap, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);
					}
				}
			}
	}

	if ((sjoy.joybit[7] == TRUE)) {
		//space is pressed, lets draw the hitmap, why not?

		if (!no_cheat) drawallhard();
	}

	///copy protection check

	if (regcode != mycode) {
		if (no_cheat)
			if (((rnd() % 9000) + 1) == 1) {
				initFail(hWndMain, "Error 49 with CD, contact RTsoft, Inc.");
				return;
			}
	}

	if (mode == 0) {
		FillMemory(&spr[1], sizeof(spr[1]), 0);

		spr[1].speed = 3;
		init_mouse();
		g_pMouse->Acquire();

		spr[1].timer = 0;
		spr[1].brain = 1;
		spr[1].hard = 1;
		spr[1].pseq = 2;
		spr[1].pframe = 1;
		spr[1].seq = 2;
		spr[1].dir = 2;
		spr[1].damage = 0;
		spr[1].strength = 10;
		spr[1].defense = 0;
		spr[1].skip = 0;
		SetRect(&spr[1].alt, 0, 0, 0, 0);
		spr[1].base_idle = 10;
		spr[1].base_walk = -1;
		spr[1].size = 100;
		spr[1].base_hit = 100;
		spr[1].active = TRUE;

		int crap2 = add_sprite(0, 450, 8, 0, 0);

		spr[crap2].hard = 1;
		spr[crap2].noclip = 1;
		strcpy(spr[crap2].text, dversion_string);

		spr[crap2].damage = -1;
		spr[crap2].owner = 1000;

		int scr = load_script("START", 1000, true);
		if (locate(scr, "MAIN") == false) {
			Msg("Error: Can't locate MAIN in script START!");
		}

		run_script(scr);
		mode = 1;
	}

	if (mode == 2) {
		//   if (  (keypressed()) | (jinfo.dwButtons) | (sjoy.right) | (sjoy.left)
		//       | (sjoy.up)   | (sjoy.down))
		{
			mode = 3;
			load_map(map.loc[*pmap]);
			draw_map_game();
			flife = *plife;

			if (keep_mouse == 0) {
				if (g_pMouse) {
					g_pMouse->Release();
					g_pMouse = NULL;
				}

				if (g_hevtMouse) {
					CloseHandle(g_hevtMouse);
					g_hevtMouse = NULL;
				}
				if (g_pdi) {
					g_pdi->Release();
					g_pdi = NULL;
				}
			}

			// draw_status_all();
		}
	}

	if (spr[1].active)
		if (spr[1].brain == 1)
			did_player_cross_screen(false, 1);

	if (trig_man) {
		goto trigger_start;
	}

	if (get_frame) {
		RECT rcRect1;

		total_trigger = true;
		get_frame = false;
		rcRect1.left = playl;
		rcRect1.top = 0;
		rcRect1.right = 620;
		rcRect1.bottom = 400;
		if (no_transition) return;
		//return;

		ddrVal = lpDDSTrick2->BltFast(0, 0, lpDDSBack,
			&rcRect1, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
		if (ddrVal != DD_OK) dderror(ddrVal);

		return;
	}

	if (screenlock == 1) {
		//Msg("Drawing screenlock.");
		drawscreenlock();
	}

	if (lpDDSBack->GetDC(&hdc) == DD_OK) {
		if (debug_mode) {
			SetBkMode(hdc, OPAQUE);
			//      TextOut(hdc,0,0, msg,lstrlen(msg));
			if (mode == 0)
				strcpy(msg, "");
			if (mode == 1) {
				sprintf(msg, "X is %d y is %d", spr[1].x, spr[1].y);
				//let's add the key info to it.
				for (int x = 0; x < 256; x++) {
					if (GetKeyboard(x)) {
						sprintf(msg, "%s (Key %i)", msg, x);
					}
				}
			}

			if (mode == 3) {
				sprintf(msg, "Sprites: %d  FPS: %d  Show_dot: %d Plane_process: %d Moveman X%d X%d: %d Y%d Y%d Map %d",
					last_sprite_created, fps_show, show_dot, plane_process, spr[1].lpx[0], spr[1].lpy[0], spr[1].moveman, spr[1].lpx[1],
					spr[1].lpy[1], *pmap);
			}

			rcRect.left = 0;
			rcRect.top = 0;
			rcRect.right = playx;
			rcRect.bottom = playy;
			SetTextColor(hdc, RGB(200, 200, 200));
			DrawText(hdc, msg, lstrlen(msg), &rcRect, DT_WORDBREAK);

			if (strlen(last_debug) > 0) {
				//let's also draw this...
				strcpy(msg, last_debug);
				rcRect.left = 0;
				rcRect.top = 20;
				rcRect.right = playx;
				rcRect.bottom = playy;
				SetTextColor(hdc, RGB(200, 200, 200));
				DrawText(hdc, msg, lstrlen(msg), &rcRect, DT_WORDBREAK);
			}
		}

		// Console
		if (mConsoleActive == true) {
			SetBkMode(hdc, OPAQUE);
			RECT lConsoleRectangle;
			lConsoleRectangle.top = 380;
			lConsoleRectangle.left = 20;
			lConsoleRectangle.right = 620;
			lConsoleRectangle.bottom = 400;
			char lLine[255];
			strncpy(lLine, mConsoleLine.c_str(), 254);
			replace("&", "~~", lLine);
			replace("~~", "&&", lLine);
			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(0, 0, 0));
			DrawText(hdc, lLine, lstrlen(lLine), &lConsoleRectangle, DT_WORDBREAK);
			lConsoleRectangle.top = 360;
			lConsoleRectangle.bottom = 380;
			sprintf(lLine, "%d", mConsoleReturnValue);
			SetTextColor(hdc, RGB(255, 0, 0));
			DrawText(hdc, lLine, lstrlen(lLine), &lConsoleRectangle, DT_WORDBREAK);
		}

		SelectObject(hdc, hfont_small);
		SetBkMode(hdc, TRANSPARENT);

		for (j = 1; j < max_s + 1; j++) {
			if (plane_process)
				h = rank[j];
			else
				h = j;

			if (h > 0)
				if (spr[h].active) {
					if (spr[h].brain == 8) {
						//Msg("Drawing text %d..", h);
						text_draw(h, hdc);
					}
				}
		}

		lpDDSBack->ReleaseDC(hdc);
	}

	if (talk.active)
		process_talk();

	process_callbacks();

flip:
	if (g_b_kill_app)
		return;
	if (!abort_this_flip)
		flip_it();

	if (turn_on_plane)
		plane_process = TRUE;
}

} // namespace Dink
