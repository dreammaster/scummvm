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

#include "common/system.h"
#include "common/savefile.h"
#include "ags2/ac/savegame.h"
#include "ags2/ac/acruntime.h"
#include "ags2/common/csrun.h"
#include "ags2/vars.h"

namespace AGS2 {

#define MAGICNUMBER 0xbeefcafe
#define SGVERSION 8

static char buffer2[60];

const char *load_game_errors[9] = {
	"No error","File not found","Not an AGS save game",
	"Invalid save game version","Saved with different interpreter",
	"Saved under a different game", "Resolution mismatch",
	"Colour depth mismatch", ""
};

static const char *sgsig = "Adventure Game Studio saved game";
static const int sgsiglen = 32;

void restore_after_dialog() {
	set_mouse_cursor(oldmouse);
	if (!play.mouse_cursor_hidden)
		domouse(2);
	construct_virtual_screen(true);
}

void RestoreGameSlot(int slnum) {
	if (displayed_room < 0)
		quit("!RestoreGameSlot: a game cannot be restored from within game_start");

	can_run_delayed_command();
	if (inside_script) {
		curscript->queue_action(ePSARestoreGame, slnum, "RestoreGameSlot");
		return;
	}
	load_game(slnum, NULL, NULL);
}

void get_save_game_path(int slotNum, char *buffer) {
	Common::strcpy_s(buffer, STD_BUFFER_SIZE, saveGameDirectory);
	Common::sprintf_s(&buffer[strlen(buffer)], STD_BUFFER_SIZE, sgnametemplate, slotNum);
	Common::strcat_s(buffer, STD_BUFFER_SIZE, saveGameSuffix);
}

void DeleteSaveSlot(int slnum) {
	char nametouse[260];
	get_save_game_path(slnum, nametouse);

	g_system->getSavefileManager()->removeSavefile(nametouse);
}

int Game_SetSaveGameDirectory(const char *newFolder) {

	// don't allow them to go to another folder
	if ((newFolder[0] == '/') || (newFolder[0] == '\\') ||
		(newFolder[0] == ' ') ||
		((newFolder[0] != 0) && (newFolder[1] == ':')))
		return 0;

	char newSaveGameDir[260];
	platform->ReplaceSpecialPaths(newFolder, newSaveGameDir);
	fix_filename_slashes(newSaveGameDir);

	put_backslash(newSaveGameDir);

	char newFolderTempFile[260];
	Common::strcpy_s(newFolderTempFile, newSaveGameDir);
	Common::strcat_s(newFolderTempFile, "agstmp.tmp");

#ifdef TODO
	FILE *testTemp = fopen(newFolderTempFile, "wb");
	if (testTemp == NULL) {
		return 0;
	}
	fclose(testTemp);
	unlink(newFolderTempFile);

	// copy the Restart Game file, if applicable
	char restartGamePath[260];
	Common::sprintf_s(restartGamePath, "%s""agssave.%d%s", saveGameDirectory, RESTART_POINT_SAVE_GAME_NUMBER, saveGameSuffix);
	FILE *restartGameFile = fopen(restartGamePath, "rb");
	if (restartGameFile != NULL) {
		long fileSize = filelength(fileno(restartGameFile));
		char *mbuffer = (char *)malloc(fileSize);
		fread(mbuffer, fileSize, 1, restartGameFile);
		fclose(restartGameFile);

		Common::sprintf_s(restartGamePath, "%s""agssave.%d%s", newSaveGameDir, RESTART_POINT_SAVE_GAME_NUMBER, saveGameSuffix);
		restartGameFile = fopen(restartGamePath, "wb");
		fwrite(mbuffer, fileSize, 1, restartGameFile);
		fclose(restartGameFile);
		free(mbuffer);
	}
#else
	warning("TODO: Game_SetSaveDirectory");
#endif

	Common::strcpy_s(saveGameDirectory, newSaveGameDir);
	return 1;
}

int GetSaveSlotDescription(int slnum, char *desbuf) {
	VALIDATE_STRING(desbuf);
	if (load_game(slnum, desbuf, NULL) == 0)
		return 1;
	Common::sprintf_s(desbuf, STD_BUFFER_SIZE, "INVALID SLOT %d", slnum);
	return 0;
}

const char *Game_GetSaveSlotDescription(int slnum) {
	char buffer[STD_BUFFER_SIZE];
	if (load_game(slnum, buffer, NULL) == 0)
		return CreateNewScriptString(buffer);
	return NULL;
}

int LoadSaveSlotScreenshot(int slnum, int width, int height) {
	int gotSlot;
	multiply_up_coordinates(&width, &height);

	if (load_game(slnum, NULL, &gotSlot) != 0)
		return 0;

	if (gotSlot == 0)
		return 0;

	if ((spritewidth[gotSlot] == width) && (spriteheight[gotSlot] == height))
		return gotSlot;

	// resize the sprite to the requested size
	block newPic = create_bitmap_ex(bitmap_color_depth(spriteset[gotSlot]), width, height);

	stretch_blit(spriteset[gotSlot], newPic,
		0, 0, spritewidth[gotSlot], spriteheight[gotSlot],
		0, 0, width, height);

	update_polled_stuff();

	// replace the bitmap in the sprite set
	free_dynamic_sprite(gotSlot);
	add_dynamic_sprite(gotSlot, newPic);

	return gotSlot;
}

int load_game_and_print_error(int toload) {
	int ecret = load_game(toload, NULL, NULL);
	if (ecret < 0) {
		// disable speech in case there are dynamic graphics that
		// have been freed
		int oldalways = game.options[OPT_ALWAYSSPCH];
		game.options[OPT_ALWAYSSPCH] = 0;
		Display("Unable to load game (error: %s).", load_game_errors[-ecret]);
		game.options[OPT_ALWAYSSPCH] = oldalways;
	}
	return ecret;
}

void restart_game() {
	can_run_delayed_command();
	if (inside_script) {
		curscript->queue_action(ePSARestartGame, 0, "RestartGame");
		return;
	}
	int errcod;
	if ((errcod = load_game(RESTART_POINT_SAVE_GAME_NUMBER, NULL, NULL)) != 0)
		quitprintf("unable to restart game (error:%s)", load_game_errors[-errcod]);

}

void SetRestartPoint() {
	save_game(RESTART_POINT_SAVE_GAME_NUMBER, "Restart Game Auto-Save");
}


int load_game(int slotn, char *descrp, int *wantShot) {
	char nametouse[260];
	get_save_game_path(slotn, nametouse);

	return do_game_load(nametouse, slotn, descrp, wantShot);
}

void save_game(int slotn, const char *descript) {
	// dont allow save in rep_exec_always, because we dont save
	// the state of blocked scripts
	can_run_delayed_command();

	if (inside_script) {
		Common::strcpy_s(curscript->postScriptSaveSlotDescription[curscript->queue_action(ePSASaveGame, slotn, "SaveGameSlot")], descript);
		return;
	}

	if (platform->GetDiskFreeSpaceMB() < 2) {
		Display("ERROR: There is not enough disk space free to save the game. Clear some disk space and try again.");
		return;
	}

	VALIDATE_STRING(descript);
	char nametouse[260];
	get_save_game_path(slotn, nametouse);

	Common::OutSaveFile *ooo = g_system->getSavefileManager()->openForSaving(nametouse, false);
	if (ooo == NULL)
		quit("save_game: unable to open savegame file for writing");
#ifdef TODO
	// Initialize and write Vista header
	RICH_GAME_MEDIA_HEADER vistaHeader;
	memset(&vistaHeader, 0, sizeof(RICH_GAME_MEDIA_HEADER));
	memcpy(&vistaHeader.dwMagicNumber, RM_MAGICNUMBER, sizeof(long));
	vistaHeader.dwHeaderVersion = 1;
	vistaHeader.dwHeaderSize = sizeof(RICH_GAME_MEDIA_HEADER);
	vistaHeader.dwThumbnailOffsetHigherDword = 0;
	vistaHeader.dwThumbnailOffsetLowerDword = 0;
	vistaHeader.dwThumbnailSize = 0;
	convert_guid_from_text_to_binary(game.guid, &vistaHeader.guidGameId[0]);
	uconvert(game.gamename, U_ASCII, (char *)&vistaHeader.szGameName[0], U_UNICODE, RM_MAXLENGTH);
	uconvert(descript, U_ASCII, (char *)&vistaHeader.szSaveName[0], U_UNICODE, RM_MAXLENGTH);
	vistaHeader.szLevelName[0] = 0;
	vistaHeader.szComments[0] = 0;

	fwrite(&vistaHeader, sizeof(RICH_GAME_MEDIA_HEADER), 1, ooo);

	fwrite(sgsig, sgsiglen, 1, ooo);

	safeguard_string((unsigned char *)descript);

	fputstring((char *)descript, ooo);

	block screenShot = NULL;

	if (game.options[OPT_SAVESCREENSHOT]) {
		int usewid = multiply_up_coordinate(play.screenshot_width);
		int usehit = multiply_up_coordinate(play.screenshot_height);
		if (usewid > virtual_screen->w)
			usewid = virtual_screen->w;
		if (usehit > virtual_screen->h)
			usehit = virtual_screen->h;

		if ((play.screenshot_width < 16) || (play.screenshot_height < 16))
			quit("!Invalid game.screenshot_width/height, must be from 16x16 to screen res");

		if (gfxDriver->UsesMemoryBackBuffer())
		{
			screenShot = create_bitmap_ex(bitmap_color_depth(virtual_screen), usewid, usehit);

			stretch_blit(virtual_screen, screenShot, 0, 0,
				virtual_screen->w, virtual_screen->h, 0, 0,
				screenShot->w, screenShot->h);
		} else
		{
			block tempBlock = create_bitmap_ex(final_col_dep, virtual_screen->w, virtual_screen->h);
			gfxDriver->GetCopyOfScreenIntoBitmap(tempBlock);

			screenShot = create_bitmap_ex(final_col_dep, usewid, usehit);
			stretch_blit(tempBlock, screenShot, 0, 0,
				tempBlock->w, tempBlock->h, 0, 0,
				screenShot->w, screenShot->h);

			destroy_bitmap(tempBlock);
		}
	}

	update_polled_stuff();

	save_game_data(ooo, screenShot);

	if (screenShot != NULL)
	{
		long screenShotOffset = ftell(ooo) - sizeof(RICH_GAME_MEDIA_HEADER);
		long screenShotSize = write_screen_shot_for_vista(ooo, screenShot);
		delete ooo;

		update_polled_stuff();

		ooo = fopen(nametouse, "r+b");
		fseek(ooo, 12, SEEK_SET);
		putw(screenShotOffset, ooo);
		fseek(ooo, 4, SEEK_CUR);
		putw(screenShotSize, ooo);
	}

	if (screenShot != NULL)
		free(screenShot);
#else
	warning("TODO: save_game refactored to use engine saveGameStream");
#endif

	ooo->finalize();
	delete ooo;
}

void serialize_bitmap(block thispic, Common::WriteStream *ooo) {
	if (thispic != NULL) {
		putw(thispic->w, ooo);
		putw(thispic->h, ooo);
		putw(bitmap_color_depth(thispic), ooo);
		for (int cc = 0; cc < thispic->h; cc++)
			ooo->write(&thispic->line[cc][0], thispic->w * bitmap_color_depth(thispic) / 8);
	}
}

long write_screen_shot_for_vista(Common::WriteStream *ooo, block screenshot) {
	long fileSize = 0;
	char tempFileName[MAX_PATH];
	Common::sprintf_s(tempFileName, "%s""_tmpscht.bmp", saveGameDirectory);

	save_bitmap(tempFileName, screenshot, palette);

	update_polled_stuff();

	Common::InSaveFile *input;
	if ((input = g_system->getSavefileManager()->openForLoading(tempFileName)) != nullptr) {
		fileSize = input->size();
		char *buffer = (char *)malloc(fileSize);

		input->read(buffer, fileSize);
		delete input;
		g_system->getSavefileManager()->removeSavefile(tempFileName);

		ooo->write(buffer, fileSize);
		free(buffer);
	}

	return fileSize;
}

// Write the save game position to the file
void save_game_data(Common::WriteStream *ooo, block screenshot) {
	int bb, cc, dd, ff;

	platform->RunPluginHooks(AGSE_PRESAVEGAME, 0);

	putw(SGVERSION, ooo);
	// store the screenshot at the start to make it easily accesible
	putw((screenshot == NULL) ? 0 : 1, ooo);

	if (screenshot)
		serialize_bitmap(screenshot, ooo);

	fputstring(ACI_VERSION_TEXT, ooo);
	fputstring(usetup.main_data_filename, ooo);
	putw(scrnhit, ooo);
	putw(final_col_dep, ooo);
	putw(frames_per_second, ooo);
	putw(cur_mode, ooo);
	putw(cur_cursor, ooo);
	putw(offsetx, ooo); putw(offsety, ooo);
	putw(loopcounter, ooo);

	putw(spriteset.elements, ooo);
	for (bb = 1; bb < spriteset.elements; bb++) {
		if (game.spriteflags[bb] & SPF_DYNAMICALLOC) {
			putw(bb, ooo);
			ooo->writeByte(game.spriteflags[bb]);
			serialize_bitmap(spriteset[bb], ooo);
		}
	}
	// end of dynamic sprite list
	putw(0, ooo);

	// write the data segment of the global script
	int gdatasize = gameinst->globaldatasize;
	putw(gdatasize, ooo);
	ccFlattenGlobalData(gameinst);

	// MACPORT FIX: just in case gdatasize is 2 or 4, don't want to swap endian
	ooo->write(&gameinst->globaldata[0], gdatasize);
	ccUnFlattenGlobalData(gameinst);

	// write the script modules data segments
	putw(numScriptModules, ooo);

	for (bb = 0; bb < numScriptModules; bb++) {
		int glsize = moduleInst[bb]->globaldatasize;
		putw(glsize, ooo);
		if (glsize > 0) {
			ccFlattenGlobalData(moduleInst[bb]);
			ooo->write(&moduleInst[bb]->globaldata[0], glsize);
			ccUnFlattenGlobalData(moduleInst[bb]);
		}
	}

	putw(displayed_room, ooo);

	if (displayed_room >= 0) {
		// update the current room script's data segment copy
		if (roominst != NULL)
			save_room_data_segment();

		// Update the saved interaction variable values
		for (ff = 0; ff < thisroom.numLocalVars; ff++)
			croom->interactionVariableValues[ff] = thisroom.localvars[ff].value;

	}

	// write the room state for all the rooms the player has been in
	for (bb = 0; bb < MAX_ROOMS; bb++) {
		if (roomstats[bb].beenhere) {
			ooo->writeByte(1);
			roomstats[bb].save(ooo);

			if (roomstats[bb].tsdatasize > 0)
				ooo->write(&roomstats[bb].tsdata[0], roomstats[bb].tsdatasize);
		} else {
			ooo->writeByte(0);
		}
	}

	update_polled_stuff();

	if (play.cur_music_number >= 0) {
		if (IsMusicPlaying() == 0)
			play.cur_music_number = -1;
	}

	play.save(ooo);

	for (bb = 0; bb < play.num_do_once_tokens; bb++)
		fputstring(play.do_once_tokens[bb], ooo);
	for (bb = 0; bb < game.numgui; ++bb)
		ooo->writeSint32LE(play.gui_draw_order[bb]);
	for (bb = 0; bb < game.numcharacters + MAX_INIT_SPR + 1; ++bb)
		mls[bb].save(ooo);

	game.save(ooo);
	for (bb = 0; bb < game.numinvitems; ++bb)
		game.invinfo[bb].save(ooo);
	for (bb = 0; bb < game.numcursors; ++bb)
		game.mcurs[bb].save(ooo);

	if (game.invScripts == NULL) {
		for (bb = 0; bb < game.numinvitems; bb++) {
			for (cc = 0; cc < MAX_NEWINTERACTION_EVENTS; ++cc)
				ooo->writeSint32LE(game.intrInv[bb]->timesRun[cc]);
		}
		for (bb = 0; bb < game.numcharacters; bb++) {
			for (cc = 0; cc < MAX_NEWINTERACTION_EVENTS; ++cc)
				ooo->writeSint32LE(game.intrChar[bb]->timesRun[cc]);
		}
	}

	for (bb = 0; bb < OPT_HIGHESTOPTION + 1; ++bb)
		ooo->writeSint32LE(game.options[bb]);
	ooo->writeSint32LE(game.options[OPT_LIPSYNCTEXT]);
	for (bb = 0; bb < game.numcharacters; ++bb)
		game.chars[bb].save(ooo);
	for (bb = 0; bb < game.numcharacters; ++bb)
		charextra[bb].save(ooo);
	for (bb = 0; bb < 256; ++bb)
		palette[bb].writeToFile(ooo);

	for (bb = 0; bb < game.numdialog; ++bb) {
		for (cc = 0; cc < MAXTOPICOPTIONS; ++cc)
			ooo->writeSint32LE(dialog[bb].optionflags[cc]);
	}

	putw(mouse_on_iface, ooo);
	putw(mouse_on_iface_button, ooo);
	putw(mouse_pushed_iface, ooo);
	putw(ifacepopped, ooo);
	putw(game_paused, ooo);
	//putw(mi.trk,ooo);
	write_gui(ooo, guis, &game);
	putw(numAnimButs, ooo);

	for (bb = 0; bb < numAnimButs; ++bb)
		animbuts[bb].save(ooo);

	putw(game.audioClipTypeCount, ooo);
	for (bb = 0; bb < game.audioClipTypeCount; ++bb)
		game.audioClipTypes[bb].save(ooo);

	for (bb = 0; bb < MAX_REGIONS; ++bb)
		ooo->writeSint16LE(thisroom.regionLightLevel[bb]);
	for (bb = 0; bb < MAX_REGIONS; ++bb)
		ooo->writeSint32LE(thisroom.regionTintLevel[bb]);
	for (bb = 0; bb < MAX_WALK_AREAS + 1; ++bb)
		ooo->writeSint16LE(thisroom.walk_area_zoom[bb]);
	for (bb = 0; bb < MAX_WALK_AREAS + 1; ++bb)
		ooo->writeSint16LE(thisroom.walk_area_zoom[bb]);

	for (bb = 0; bb < MAX_SOUND_CHANNELS; ++bb)
		ambient[bb].save(ooo);

	putw(numscreenover, ooo);
	for (bb = 0; bb < numscreenover; ++bb)
		screenover[bb].save(ooo);
	for (bb = 0; bb < numscreenover; bb++)
		serialize_bitmap(screenover[bb].pic, ooo);

	update_polled_stuff();

	for (bb = 0; bb < MAX_DYNAMIC_SURFACES; bb++) {
		if (dynamicallyCreatedSurfaces[bb] == NULL) {
			ooo->writeByte(0);
		} else {
			ooo->writeByte(1);
			serialize_bitmap(dynamicallyCreatedSurfaces[bb], ooo);
		}
	}

	update_polled_stuff();

	if (displayed_room >= 0) {
		for (bb = 0; bb < MAX_BSCENE; bb++) {
			if (play.raw_modified[bb])
				serialize_bitmap(thisroom.ebscene[bb], ooo);
		}

		putw((raw_saved_screen == NULL) ? 0 : 1, ooo);
		if (raw_saved_screen)
			serialize_bitmap(raw_saved_screen, ooo);

		// save the current troom, in case they save in room 600 or whatever
		troom.save(ooo);
		if (troom.tsdatasize > 0)
			ooo->write(&troom.tsdata[0], troom.tsdatasize);
	}

	putw(numGlobalVars, ooo);
	for (bb = 0; bb < numGlobalVars; ++bb)
		globalvars[bb].save(ooo);

	putw(game.numviews, ooo);
	for (bb = 0; bb < game.numviews; bb++) {
		for (cc = 0; cc < views[bb].numLoops; cc++) {
			for (dd = 0; dd < views[bb].loops[cc].numFrames; dd++)
			{
				putw(views[bb].loops[cc].frames[dd].sound, ooo);
				putw(views[bb].loops[cc].frames[dd].pic, ooo);
			}
		}
	}
	putw(MAGICNUMBER + 1, ooo);

	putw(game.audioClipCount, ooo);
	for (bb = 0; bb <= MAX_SOUND_CHANNELS; bb++) {
		if ((channels[bb] != NULL) && (channels[bb]->done == 0) && (channels[bb]->sourceClip != NULL))
		{
			putw(((ScriptAudioClip *)channels[bb]->sourceClip)->id, ooo);
			putw(channels[bb]->get_pos(), ooo);
			putw(channels[bb]->priority, ooo);
			putw(channels[bb]->repeat ? 1 : 0, ooo);
			putw(channels[bb]->vol, ooo);
			putw(channels[bb]->panning, ooo);
			putw(channels[bb]->volAsPercentage, ooo);
			putw(channels[bb]->panningAsPercentage, ooo);
		} else {
			ooo->writeSint32LE(-1);
		}
	}

	putw(crossFading, ooo);
	putw(crossFadeVolumePerStep, ooo);
	putw(crossFadeStep, ooo);
	putw(crossFadeVolumeAtStart, ooo);

	platform->RunPluginHooks(AGSE_SAVEGAME, (int)ooo);
	putw(MAGICNUMBER, ooo);  // to verify the plugins

	// save the room music volume
	putw(thisroom.options[ST_VOLUME], ooo);

	ccSerializeAllObjects(ooo);

	putw(current_music_type, ooo);

	update_polled_stuff();
}

// Some people have been having crashes with the save game list,
// so make sure the game name is valid
void safeguard_string(unsigned char *descript) {
	int it;
	for (it = 0; it < 50; it++) {
		if ((descript[it] < 1) || (descript[it] > 127))
			break;
	}
	if (descript[it] != 0)
		descript[it] = 0;
}

// On Windows we could just use IIDFromString but this is platform-independant
void convert_guid_from_text_to_binary(const char *guidText, unsigned char *buffer) {
	guidText++; // skip {
	for (int bytesDone = 0; bytesDone < 16; bytesDone++)
	{
		if (*guidText == '-')
			guidText++;

		char tempString[3];
		tempString[0] = guidText[0];
		tempString[1] = guidText[1];
		tempString[2] = 0;
		int thisByte = 0;
		sscanf(tempString, "%X", &thisByte);

		buffer[bytesDone] = thisByte;
		guidText += 2;
	}

	// Swap bytes to give correct GUID order
	unsigned char temp;
	temp = buffer[0]; buffer[0] = buffer[3]; buffer[3] = temp;
	temp = buffer[1]; buffer[1] = buffer[2]; buffer[2] = temp;
	temp = buffer[4]; buffer[4] = buffer[5]; buffer[5] = temp;
	temp = buffer[6]; buffer[6] = buffer[7]; buffer[7] = temp;
}

void restore_game_dialog() {
	can_run_delayed_command();
	if (thisroom.options[ST_SAVELOAD] == 1) {
		DisplayMessage(983);
		return;
	}
	if (inside_script) {
		curscript->queue_action(ePSARestoreGameDialog, 0, "RestoreGameDialog");
		return;
	}
	setup_for_dialog();
	int toload = loadgamedialog();
	restore_after_dialog();
	if (toload >= 0) {
		load_game_and_print_error(toload);
	}
}

void save_game_dialog() {
	if (thisroom.options[ST_SAVELOAD] == 1) {
		DisplayMessage(983);
		return;
	}
	if (inside_script) {
		curscript->queue_action(ePSASaveGameDialog, 0, "SaveGameDialog");
		return;
	}
	setup_for_dialog();
	int toload = savegamedialog();
	restore_after_dialog();
	if (toload >= 0)
		save_game(toload, buffer2);
}


void save_room_data_segment() {
	if (croom->tsdatasize > 0)
		free(croom->tsdata);
	croom->tsdata = NULL;
	croom->tsdatasize = roominst->globaldatasize;
	if (croom->tsdatasize > 0) {
		croom->tsdata = (char *)malloc(croom->tsdatasize + 10);
		ccFlattenGlobalData(roominst);
		memcpy(croom->tsdata, &roominst->globaldata[0], croom->tsdatasize);
		ccUnFlattenGlobalData(roominst);
	}

}

void unload_old_room() {
	int ff;

	// if switching games on restore, don't do this
	if (displayed_room < 0)
		return;

	platform->WriteDebugString("Unloading room %d", displayed_room);

	current_fade_out_effect();

	clear(abuf);
	for (ff = 0; ff < croom->numobj; ff++)
		objs[ff].moving = 0;

	if (!play.ambient_sounds_persist) {
		for (ff = 1; ff < MAX_SOUND_CHANNELS; ff++)
			StopAmbientSound(ff);
	}

	cancel_all_scripts();
	numevents = 0;  // cancel any pending room events

	if (roomBackgroundBmp != NULL)
	{
		gfxDriver->DestroyDDB(roomBackgroundBmp);
		roomBackgroundBmp = NULL;
	}

	if (croom == NULL);
	else if (roominst != NULL) {
		save_room_data_segment();
		ccFreeInstance(roominstFork);
		ccFreeInstance(roominst);
		roominstFork = NULL;
		roominst = NULL;
	} else croom->tsdatasize = 0;
	memset(&play.walkable_areas_on[0], 1, MAX_WALK_AREAS + 1);
	play.bg_frame = 0;
	play.bg_frame_locked = 0;
	play.offsets_locked = 0;
	remove_screen_overlay(-1);
	if (raw_saved_screen != NULL) {
		wfreeblock(raw_saved_screen);
		raw_saved_screen = NULL;
	}
	for (ff = 0; ff < MAX_BSCENE; ff++)
		play.raw_modified[ff] = 0;
	for (ff = 0; ff < thisroom.numLocalVars; ff++)
		croom->interactionVariableValues[ff] = thisroom.localvars[ff].value;

	// wipe the character cache when we change rooms
	for (ff = 0; ff < game.numcharacters; ff++) {
		if (charcache[ff].inUse) {
			destroy_bitmap(charcache[ff].image);
			charcache[ff].image = NULL;
			charcache[ff].inUse = 0;
		}
		// ensure that any half-moves (eg. with scaled movement) are stopped
		charextra[ff].xwas = INVALID_X;
	}

	play.swap_portrait_lastchar = -1;

	for (ff = 0; ff < croom->numobj; ff++) {
		// un-export the object's script object
		if (objectScriptObjNames[ff][0] == 0)
			continue;

		ccRemoveExternalSymbol(objectScriptObjNames[ff]);
	}

	for (ff = 0; ff < MAX_HOTSPOTS; ff++) {
		if (thisroom.hotspotScriptNames[ff][0] == 0)
			continue;

		ccRemoveExternalSymbol(thisroom.hotspotScriptNames[ff]);
	}

	// clear the object cache
	for (ff = 0; ff < MAX_INIT_SPR; ff++) {
		if (objcache[ff].image != NULL) {
			destroy_bitmap(objcache[ff].image);
			objcache[ff].image = NULL;
		}
	}
	// clear the actsps buffers to save memory, since the
	// objects/characters involved probably aren't on the
	// new screen. this also ensures all cached data is flushed
	for (ff = 0; ff < MAX_INIT_SPR + game.numcharacters; ff++) {
		if (actsps[ff] != NULL)
			destroy_bitmap(actsps[ff]);
		actsps[ff] = NULL;

		if (actspsbmp[ff] != NULL)
			gfxDriver->DestroyDDB(actspsbmp[ff]);
		actspsbmp[ff] = NULL;

		if (actspswb[ff] != NULL)
			destroy_bitmap(actspswb[ff]);
		actspswb[ff] = NULL;

		if (actspswbbmp[ff] != NULL)
			gfxDriver->DestroyDDB(actspswbbmp[ff]);
		actspswbbmp[ff] = NULL;

		actspswbcache[ff].valid = 0;
	}

	// if Hide Player Character was ticked, restore it to visible
	if (play.temporarily_turned_off_character >= 0) {
		game.chars[play.temporarily_turned_off_character].on = 1;
		play.temporarily_turned_off_character = -1;
	}
}

int LoadImageFile(const char *filename) {
	char loadFromPath[MAX_PATH];
	get_current_dir_path(loadFromPath, filename);

	block loadedFile = load_bitmap(loadFromPath, NULL);

	if (loadedFile == NULL)
		return 0;

	int gotSlot = spriteset.findFreeSlot();
	if (gotSlot <= 0)
		return 0;

	add_dynamic_sprite(gotSlot, gfxDriver->ConvertBitmapToSupportedColourDepth(loadedFile));

	return gotSlot;
}

int do_game_load(const char *nametouse, int slotNumber, char *descrp, int *wantShot) {
	gameHasBeenRestored++;

	Common::InSaveFile *ooo = g_system->getSavefileManager()->openForLoading(nametouse);
	if (ooo == NULL)
		return -1;

	// skip Vista header
	RICH_GAME_MEDIA_HEADER hdr;
	hdr.ReadFromFile(ooo);

	ooo->read(rbuffer, sgsiglen);
	rbuffer[sgsiglen] = 0;
	if (strcmp(rbuffer, sgsig) != 0) {
		// not a save game
		delete ooo;
		return -2;
	}

	int oldeip = our_eip;
	our_eip = 2050;

	fgetstring_limit(rbuffer, ooo, 180);
	rbuffer[180] = 0;
	safeguard_string((unsigned char *)rbuffer);

	if (descrp != NULL) {
		// just want slot description, so return
		Common::strcpy_s(descrp, STD_BUFFER_SIZE, rbuffer);
		delete ooo;
		our_eip = oldeip;
		return 0;
	}
#ifdef TODO
	if (wantShot != NULL) {
		// just want the screenshot
		if (getw(ooo) != SGVERSION) {
			delete ooo;
			return -3;
		}
		int isScreen = getw(ooo);
		*wantShot = 0;

		if (isScreen) {
			int gotSlot = spriteset.findFreeSlot();
			// load the screenshot
			block redin = read_serialized_bitmap(ooo);

			if (gotSlot > 0) {
				// add it into the sprite set
				add_dynamic_sprite(gotSlot, gfxDriver->ConvertBitmapToSupportedColourDepth(redin));

				*wantShot = gotSlot;
			} else
			{
				destroy_bitmap(redin);
			}
		}

		delete ooo;
		our_eip = oldeip;
		return 0;
	}

	our_eip = 2051;

	// do the actual restore
	int ress = restore_game_data(ooo, nametouse);

	our_eip = oldeip;

	if (ress == -5) {
		error("Saved in different game");
		return 0;
	}

	if (ress)
		return ress;

	run_on_event(GE_RESTORE_GAME, slotNumber);

	// ensure keyboard buffer is clean
	// use the raw versions rather than the rec_ versions so we don't
	// interfere with the replay sync
	while (keypressed())
		readkey();
#else
	error("TODO: do_game_load");
#endif
	return 0;
}

void RICH_GAME_MEDIA_HEADER::ReadFromFile(Common::SeekableReadStream *in) {
#ifdef TODO
	dwMagicNumber = in->readSint32LE();
	dwHeaderVersion = in->readSint32LE();
	dwHeaderSize = in->readSint32LE();
	dwThumbnailOffsetLowerDword = in->readSint32LE();
	dwThumbnailOffsetHigherDword = in->readSint32LE();
	dwThumbnailSize = in->readSint32LE();
	in->read(guidGameId, 16);
	in->ReadArrayOfInt16((int16_t *)szGameName, RM_MAXLENGTH);
	in->ReadArrayOfInt16((int16_t *)szSaveName, RM_MAXLENGTH);
	in->ReadArrayOfInt16((int16_t *)szLevelName, RM_MAXLENGTH);
	in->ReadArrayOfInt16((int16_t *)szComments, RM_MAXLENGTH);
#else
	error("TODO: header");
#endif
}

void RICH_GAME_MEDIA_HEADER::WriteToFile(Common::WriteStream *out) {
#ifdef TODO
	out->WriteInt32(dwMagicNumber);
	out->WriteInt32(dwHeaderVersion);
	out->WriteInt32(dwHeaderSize);
	out->WriteInt32(dwThumbnailOffsetLowerDword);
	out->WriteInt32(dwThumbnailOffsetHigherDword);
	out->WriteInt32(dwThumbnailSize);
	out->Write(guidGameId, 16);
	out->WriteArrayOfInt16((int16_t *)szGameName, RM_MAXLENGTH);
	out->WriteArrayOfInt16((int16_t *)szSaveName, RM_MAXLENGTH);
	out->WriteArrayOfInt16((int16_t *)szLevelName, RM_MAXLENGTH);
	out->WriteArrayOfInt16((int16_t *)szComments, RM_MAXLENGTH);
#else
	error("TODO: header");
#endif
}

} // namespace AGS2
