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

#include "ags2/ac/acsavegame.h"
#include "ags2/vars.h"

namespace AGS2 {

#define MAGICNUMBER 0xbeefcafe
#define SGVERSION 8

static const char *load_game_errors[9] = {
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
	strcpy(buffer, saveGameDirectory);
	sprintf(&buffer[strlen(buffer)], sgnametemplate, slotNum);
	strcat(buffer, saveGameSuffix);
}

void DeleteSaveSlot(int slnum) {
	char nametouse[260];
	get_save_game_path(slnum, nametouse);
	unlink(nametouse);
	if ((slnum >= 1) && (slnum <= MAXSAVEGAMES)) {
		char thisname[260];
		for (int i = MAXSAVEGAMES; i > slnum; i--) {
			get_save_game_path(i, thisname);
			FILE *fin = fopen(thisname, "rb");
			if (fin != NULL) {
				fclose(fin);
				// Rename the highest save game to fill in the gap
				rename(thisname, nametouse);
				break;
			}
		}

	}
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

#ifdef LINUX_VERSION
	mkdir(newSaveGameDir, 0);
#else
	mkdir(newSaveGameDir);
#endif

	put_backslash(newSaveGameDir);

	char newFolderTempFile[260];
	strcpy(newFolderTempFile, newSaveGameDir);
	strcat(newFolderTempFile, "agstmp.tmp");

	FILE *testTemp = fopen(newFolderTempFile, "wb");
	if (testTemp == NULL) {
		return 0;
	}
	fclose(testTemp);
	unlink(newFolderTempFile);

	// copy the Restart Game file, if applicable
	char restartGamePath[260];
	sprintf(restartGamePath, "%s""agssave.%d%s", saveGameDirectory, RESTART_POINT_SAVE_GAME_NUMBER, saveGameSuffix);
	FILE *restartGameFile = fopen(restartGamePath, "rb");
	if (restartGameFile != NULL) {
		long fileSize = filelength(fileno(restartGameFile));
		char *mbuffer = (char *)malloc(fileSize);
		fread(mbuffer, fileSize, 1, restartGameFile);
		fclose(restartGameFile);

		sprintf(restartGamePath, "%s""agssave.%d%s", newSaveGameDir, RESTART_POINT_SAVE_GAME_NUMBER, saveGameSuffix);
		restartGameFile = fopen(restartGamePath, "wb");
		fwrite(mbuffer, fileSize, 1, restartGameFile);
		fclose(restartGameFile);
		free(mbuffer);
	}

	strcpy(saveGameDirectory, newSaveGameDir);
	return 1;
}

int GetSaveSlotDescription(int slnum, char *desbuf) {
	VALIDATE_STRING(desbuf);
	if (load_game(slnum, desbuf, NULL) == 0)
		return 1;
	sprintf(desbuf, "INVALID SLOT %d", slnum);
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

void save_game(int slotn, const char *descript) {
	// dont allow save in rep_exec_always, because we dont save
	// the state of blocked scripts
	can_run_delayed_command();

	if (inside_script) {
		strcpy(curscript->postScriptSaveSlotDescription[curscript->queue_action(ePSASaveGame, slotn, "SaveGameSlot")], descript);
		return;
	}

	if (platform->GetDiskFreeSpaceMB() < 2) {
		Display("ERROR: There is not enough disk space free to save the game. Clear some disk space and try again.");
		return;
	}

	VALIDATE_STRING(descript);
	char nametouse[260];
	get_save_game_path(slotn, nametouse);

	FILE *ooo = fopen(nametouse, "wb");
	if (ooo == NULL)
		quit("save_game: unable to open savegame file for writing");

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
		fclose(ooo);

		update_polled_stuff();

		ooo = fopen(nametouse, "r+b");
		fseek(ooo, 12, SEEK_SET);
		putw(screenShotOffset, ooo);
		fseek(ooo, 4, SEEK_CUR);
		putw(screenShotSize, ooo);
	}

	if (screenShot != NULL)
		free(screenShot);

	fclose(ooo);
}

int find_highest_room_entered() {
	int qq, fndas = -1;
	for (qq = 0; qq < MAX_ROOMS; qq++) {
		if (roomstats[qq].beenhere != 0) fndas = qq;
	}
	// This is actually legal - they might start in room 400 and save
	//if (fndas<0) quit("find_highest_room: been in no rooms?");
	return fndas;
}

void serialize_bitmap(block thispic, FILE *ooo) {
	if (thispic != NULL) {
		putw(thispic->w, ooo);
		putw(thispic->h, ooo);
		putw(bitmap_color_depth(thispic), ooo);
		for (int cc = 0; cc < thispic->h; cc++)
			fwrite(&thispic->line[cc][0], thispic->w, bitmap_color_depth(thispic) / 8, ooo);
	}
}

long write_screen_shot_for_vista(FILE *ooo, block screenshot) {
	long fileSize = 0;
	char tempFileName[MAX_PATH];
	sprintf(tempFileName, "%s""_tmpscht.bmp", saveGameDirectory);

	save_bitmap(tempFileName, screenshot, palette);

	update_polled_stuff();

	if (exists(tempFileName))
	{
		fileSize = file_size(tempFileName);
		char *buffer = (char *)malloc(fileSize);

		FILE *input = fopen(tempFileName, "rb");
		fread(buffer, fileSize, 1, input);
		fclose(input);
		unlink(tempFileName);

		fwrite(buffer, fileSize, 1, ooo);
		free(buffer);
	}
	return fileSize;
}

// Write the save game position to the file
void save_game_data(FILE *ooo, block screenshot) {
	int bb, cc, dd;

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
			fputc(game.spriteflags[bb], ooo);
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
	fwrite(&gameinst->globaldata[0], 1, gdatasize, ooo);
	ccUnFlattenGlobalData(gameinst);
	// write the script modules data segments
	putw(numScriptModules, ooo);
	for (bb = 0; bb < numScriptModules; bb++) {
		int glsize = moduleInst[bb]->globaldatasize;
		putw(glsize, ooo);
		if (glsize > 0) {
			ccFlattenGlobalData(moduleInst[bb]);
			fwrite(&moduleInst[bb]->globaldata[0], 1, glsize, ooo);
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
			fputc(1, ooo);
			fwrite(&roomstats[bb], sizeof(RoomStatus), 1, ooo);
			if (roomstats[bb].tsdatasize > 0)
				fwrite(&roomstats[bb].tsdata[0], 1, roomstats[bb].tsdatasize, ooo);
		} else
			fputc(0, ooo);
	}

	update_polled_stuff();

	if (play.cur_music_number >= 0) {
		if (IsMusicPlaying() == 0)
			play.cur_music_number = -1;
	}

	fwrite(&play, sizeof(GameState), 1, ooo);

	for (bb = 0; bb < play.num_do_once_tokens; bb++)
	{
		fputstring(play.do_once_tokens[bb], ooo);
	}
	fwrite(&play.gui_draw_order[0], sizeof(int), game.numgui, ooo);

	fwrite(&mls[0], sizeof(MoveList), game.numcharacters + MAX_INIT_SPR + 1, ooo);

	fwrite(&game, sizeof(GameSetupStructBase), 1, ooo);
	fwrite(&game.invinfo[0], sizeof(InventoryItemInfo), game.numinvitems, ooo);
	fwrite(&game.mcurs[0], sizeof(MouseCursor), game.numcursors, ooo);

	if (game.invScripts == NULL)
	{
		for (bb = 0; bb < game.numinvitems; bb++)
			fwrite(&game.intrInv[bb]->timesRun[0], sizeof(int), MAX_NEWINTERACTION_EVENTS, ooo);
		for (bb = 0; bb < game.numcharacters; bb++)
			fwrite(&game.intrChar[bb]->timesRun[0], sizeof(int), MAX_NEWINTERACTION_EVENTS, ooo);
	}

	fwrite(&game.options[0], sizeof(int), OPT_HIGHESTOPTION + 1, ooo);
	fputc(game.options[OPT_LIPSYNCTEXT], ooo);

	fwrite(&game.chars[0], sizeof(CharacterInfo), game.numcharacters, ooo);
	fwrite(&charextra[0], sizeof(CharacterExtras), game.numcharacters, ooo);
	fwrite(&palette[0], sizeof(color), 256, ooo);
	for (bb = 0; bb < game.numdialog; bb++)
		fwrite(&dialog[bb].optionflags[0], sizeof(int), MAXTOPICOPTIONS, ooo);
	putw(mouse_on_iface, ooo);
	putw(mouse_on_iface_button, ooo);
	putw(mouse_pushed_iface, ooo);
	putw(ifacepopped, ooo);
	putw(game_paused, ooo);
	//putw(mi.trk,ooo);
	write_gui(ooo, guis, &game);
	putw(numAnimButs, ooo);
	fwrite(&animbuts[0], sizeof(AnimatingGUIButton), numAnimButs, ooo);

	putw(game.audioClipTypeCount, ooo);
	fwrite(&game.audioClipTypes[0], sizeof(AudioClipType), game.audioClipTypeCount, ooo);

	fwrite(&thisroom.regionLightLevel[0], sizeof(short), MAX_REGIONS, ooo);
	fwrite(&thisroom.regionTintLevel[0], sizeof(int), MAX_REGIONS, ooo);
	fwrite(&thisroom.walk_area_zoom[0], sizeof(short), MAX_WALK_AREAS + 1, ooo);
	fwrite(&thisroom.walk_area_zoom2[0], sizeof(short), MAX_WALK_AREAS + 1, ooo);

	fwrite(&ambient[0], sizeof(AmbientSound), MAX_SOUND_CHANNELS, ooo);
	putw(numscreenover, ooo);
	fwrite(&screenover[0], sizeof(ScreenOverlay), numscreenover, ooo);
	for (bb = 0; bb < numscreenover; bb++) {
		serialize_bitmap(screenover[bb].pic, ooo);
	}

	update_polled_stuff();

	for (bb = 0; bb < MAX_DYNAMIC_SURFACES; bb++)
	{
		if (dynamicallyCreatedSurfaces[bb] == NULL)
		{
			fputc(0, ooo);
		} else
		{
			fputc(1, ooo);
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
		fwrite(&troom, sizeof(RoomStatus), 1, ooo);
		if (troom.tsdatasize > 0)
			fwrite(&troom.tsdata[0], troom.tsdatasize, 1, ooo);

	}

	putw(numGlobalVars, ooo);
	fwrite(&globalvars[0], sizeof(InteractionVariable), numGlobalVars, ooo);

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
	for (bb = 0; bb <= MAX_SOUND_CHANNELS; bb++)
	{
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
		} else
		{
			putw(-1, ooo);
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

} // namespace AGS2
